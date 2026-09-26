"""keyspec — replay keyboard scenarios on a runner and check the USB reports.

    keyspec run  [-t KEYMAP[@KEYBOARD]]... [-r RUNNER] [-k TEXT] [-v] [--keep] [--junit FILE]
    keyspec list [-t ...]
    keyspec check [-t ...]              # validate scenario files only (no runner)

Targets (keymap x keyboard -> scenario files) and runner settings live in keyspec.yaml,
found in the current directory or above (or given with -c).
Exit status: 0 all passed, 1 a scenario failed, 2 usage/config/build error.
"""
from __future__ import annotations

import argparse
import sys
import time
from dataclasses import dataclass, field
from pathlib import Path
from xml.sax.saxutils import escape

import yaml

from .adapters import ADAPTERS
from .adapters.qmk import AdapterError
from .compare import Verdict, compare
from .scenario import ScenarioError, Suite, load

CONFIG_NAME = "keyspec.yaml"


@dataclass
class Target:
    keymap: str
    keyboard: str
    scenarios: list[Path]
    runners: list[str]
    overrides: dict = field(default_factory=dict)   # runner name -> settings

    @property
    def id(self) -> str:
        return f"{self.keymap}@{self.keyboard}"


@dataclass
class Config:
    path: Path
    userspace: Path
    targets: list[Target]
    runners: dict


def _color(enabled: bool):
    def c(code, s):
        return f"\033[{code}m{s}\033[0m" if enabled else s
    return c


def find_config(start: Path) -> Path:
    for d in [start, *start.parents]:
        if (d / CONFIG_NAME).is_file():
            return d / CONFIG_NAME
    raise ScenarioError(f"no {CONFIG_NAME} in {start} or above (use -c)")


def _userspace(config_dir: Path, configured) -> Path:
    if configured:
        return (config_dir / configured).resolve()
    for d in [config_dir, *config_dir.parents]:
        if (d / "qmk.json").is_file():
            return d
    return config_dir


def load_config(path: Path) -> Config:
    data = yaml.safe_load(path.read_text(encoding="utf-8")) or {}
    runners = data.get("runners") or {}
    targets = []
    for i, t in enumerate(data.get("targets") or []):
        try:
            scen = t["scenarios"] if isinstance(t["scenarios"], list) else [t["scenarios"]]
            targets.append(Target(
                keymap=t["keymap"], keyboard=t["keyboard"],
                scenarios=[(path.parent / s).resolve() for s in scen],
                runners=t.get("runners") or list(runners) or ["qmk"],
                overrides=t.get("settings") or {},
            ))
        except KeyError as e:
            raise ScenarioError(f"{path.name}: target {i + 1} is missing {e}") from None
    return Config(path, _userspace(path.parent, data.get("userspace")), targets, runners)


def select(targets: list[Target], wanted: list[str]) -> list[Target]:
    if not wanted:
        return targets
    out = []
    for w in wanted:
        km, _, kb = w.partition("@")
        hits = [t for t in targets if t.keymap == km and (not kb or t.keyboard == kb)]
        if not hits:
            raise ScenarioError(f"no target matches {w!r} (known: {', '.join(t.id for t in targets)})")
        out += [h for h in hits if h not in out]
    return out


def suites(t: Target) -> list[Suite]:
    out = []
    for p in t.scenarios:
        s = load(p)
        if s.keymap and s.keymap != t.keymap:
            raise ScenarioError(f"{p.name} is written for {s.keymap}, but target {t.id} uses it")
        out.append(s)
    return out


def cmd_check(targets: list[Target], cfg: Config, out) -> int:
    seen = set()
    for t in targets:
        for s in suites(t):
            if s.path not in seen:
                seen.add(s.path)
                print(f"ok  {_rel(s.path, cfg)}: {len(s.scenarios)} scenarios", file=out)
    return 0


def cmd_list(targets: list[Target], cfg: Config, out) -> int:
    for t in targets:
        print(f"{t.id}  [{', '.join(t.runners)}]", file=out)
        for s in suites(t):
            for sc in s.scenarios:
                setup = f"  ({', '.join(sc.setup)})" if sc.setup else ""
                print(f"  {_rel(s.path, cfg)}:{sc.index:<3d} {sc.name}{setup}", file=out)
    return 0


def _rel(p: Path, cfg: Config) -> str:
    try:
        return str(p.relative_to(cfg.path.parent))
    except ValueError:
        return str(p)


def junit(results, path: Path) -> None:
    q = {'"': "&quot;"}
    by_suite: dict[str, list] = {}
    for suite, runner, v in results:
        by_suite.setdefault(f"{suite} [{runner}]", []).append(v)
    xml = ['<?xml version="1.0" encoding="UTF-8"?>', "<testsuites>"]
    for name, vs in by_suite.items():
        fails = sum(1 for v in vs if isinstance(v, str) or not v.passed)
        xml.append(f'  <testsuite name="{escape(name, q)}" tests="{len(vs)}" failures="{fails}">')
        for v in vs:
            if isinstance(v, str):
                xml.append(f'    <testcase name="(setup)"><error message="{escape(v, q)}"/></testcase>')
                continue
            xml.append(f'    <testcase name="{escape(v.scenario.name, q)}">')
            if not v.passed:
                xml.append(f'      <failure message="{escape("; ".join(v.problems), q)}"/>')
            xml.append("    </testcase>")
        xml.append("  </testsuite>")
    xml.append("</testsuites>")
    path.write_text("\n".join(xml) + "\n", encoding="utf-8")


def cmd_run(args, targets: list[Target], cfg: Config, out) -> int:
    c = _color(out.isatty() and not args.no_color)
    results = []
    failed = errored = 0
    for t in targets:
        scenarios = [sc for s in suites(t) for sc in s.scenarios
                     if not args.filter or args.filter.lower() in sc.name.lower()]
        # Scenario indexes must be unique per target across its files.
        for i, sc in enumerate(scenarios):
            sc.index = i
        for rname in ([args.runner] if args.runner else t.runners):
            if rname not in ADAPTERS:
                raise ScenarioError(f"unknown runner {rname!r} (available: {', '.join(ADAPTERS)})")
            settings = {**(cfg.runners.get(rname) or {}), **(t.overrides.get(rname) or {})}
            if args.keep:
                settings["keep"] = True
            print(c("1", f"== {t.id} on {rname} =="), file=out)
            if not scenarios:
                print("   no scenario matches the filter", file=out)
                continue
            started = time.monotonic()
            log = (lambda m: print(c("2", f"   {m}"), file=out)) if args.verbose else (lambda m: None)
            try:
                adapter = ADAPTERS[rname](t, settings, cfg.userspace, log)
                observed = adapter.run(scenarios)
            except AdapterError as e:
                print(c("31", f"   ERROR {e}"), file=out)
                results.append((t.id, rname, str(e)))
                errored += 1
                continue
            for s in scenarios:
                obs = observed.get(s.index)
                if isinstance(obs, str):
                    print(c("31", f"   ERROR {s.name}\n") + _indent(obs, 9), file=out)
                    results.append((t.id, rname, obs))
                    errored += 1
                    continue
                v = compare(s, obs, adapter.tolerance)
                results.append((t.id, rname, v))
                if v.passed:
                    print(c("32", "   pass ") + s.name, file=out)
                    if args.verbose:
                        print(_indent("  ".join(o.describe() for o in obs), 9), file=out)
                    continue
                failed += 1
                print(c("31", "   FAIL ") + s.name, file=out)
                for p in v.problems:
                    print(f"         {p}", file=out)
                print(f"         expected: {'  '.join(e.describe() for e in s.expect) or '(no reports)'}", file=out)
                print(f"         observed: {'  '.join(o.describe() for o in obs) or '(no reports)'}", file=out)
            print(c("2", f"   {time.monotonic() - started:.1f}s"), file=out)
    total = sum(1 for r in results if isinstance(r[2], Verdict))
    summary = f"{total - failed} passed, {failed} failed" + (f", {errored} errors" if errored else "")
    print(c("1;32" if not failed and not errored else "1;31", summary), file=out)
    if args.junit:
        junit(results, Path(args.junit))
    return 2 if errored else 1 if failed else 0


def _indent(text: str, n: int) -> str:
    return "\n".join(" " * n + line for line in text.splitlines())


def main(argv=None) -> int:
    p = argparse.ArgumentParser(prog="keyspec", description=__doc__.split("\n\n")[0])
    p.add_argument("-c", "--config", type=Path, help=f"config file (default: {CONFIG_NAME} here or above)")
    sub = p.add_subparsers(dest="cmd", required=True)
    for name in ("run", "list", "check"):
        sp = sub.add_parser(name)
        sp.add_argument("-t", "--target", action="append", default=[], metavar="KEYMAP[@KEYBOARD]",
                        help="restrict to these targets (repeatable); default: all")
        if name == "run":
            sp.add_argument("-r", "--runner", help="override the targets' runners (e.g. qmk)")
            sp.add_argument("-k", "--filter", help="only scenarios whose name contains TEXT")
            sp.add_argument("-v", "--verbose", action="store_true", help="show progress and every observed trace")
            sp.add_argument("--keep", action="store_true", help="keep the generated harness for debugging")
            sp.add_argument("--junit", metavar="FILE", help="also write a JUnit XML report")
            sp.add_argument("--no-color", action="store_true")
    args = p.parse_args(argv)
    out = sys.stdout
    try:
        cfg = load_config((args.config or find_config(Path.cwd())).resolve())
        targets = select(cfg.targets, args.target)
        if args.cmd == "check":
            return cmd_check(targets, cfg, out)
        if args.cmd == "list":
            return cmd_list(targets, cfg, out)
        return cmd_run(args, targets, cfg, out)
    except (ScenarioError, FileNotFoundError, yaml.YAMLError) as e:
        print(f"keyspec: {e}", file=sys.stderr)
        return 2
