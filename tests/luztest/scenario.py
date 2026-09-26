"""Scenario files: loading, validation, and the runner-neutral step timeline.

A scenario is a list of steps on Luz key positions (0-41) plus the USB reports the host
is expected to receive. This module turns the YAML into plain data every adapter can
replay, so the step semantics (and therefore the timing) are defined once, here.
"""
from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path

import yaml

POSITIONS = 42
OSES = ("linux", "macos")

# Modifier names, in HID bit order (bit 0 = LCTL ... bit 7 = RGUI).
MODS = ("LCTL", "LSFT", "LALT", "LGUI", "RCTL", "RSFT", "RALT", "RGUI")

# Consumer-page usages Luz can send (the ones on ADJUST), by QMK keycode name.
CONSUMER = {
    "MUTE": 0x00E2, "VOLU": 0x00E9, "VOLD": 0x00EA,
    "BRIU": 0x006F, "BRID": 0x0070,
    "MNXT": 0x00B5, "MPRV": 0x00B6, "MPLY": 0x00CD, "MSTP": 0x00B7,
}
CONSUMER_NAMES = {v: k for k, v in CONSUMER.items()}

# Aliases accepted in `expect`, normalised to the names QMK's keycode_string prints.
ALIASES = {
    "LCTRL": "LCTL", "LSHIFT": "LSFT", "LOPT": "LALT", "LCMD": "LGUI",
    "RCTRL": "RCTL", "RSHIFT": "RSFT", "ROPT": "RALT", "RCMD": "RGUI", "ALGR": "RALT",
    "ENTER": "ENT", "SPACE": "SPC", "BACKSPACE": "BSPC", "ESCAPE": "ESC",
    "RIGHT": "RGHT", "QUOTE": "QUOT", "COMMA": "COMM", "SLASH": "SLSH",
    "MINUS": "MINS", "EQUAL": "EQL", "GRAVE": "GRV", "SEMICOLON": "SCLN",
}


class ScenarioError(ValueError):
    pass


def key_name(name: str) -> str:
    """Normalise a key name from YAML (`a`, `KC_A`, `Shift`...) to QMK's short form."""
    n = str(name).strip().upper()
    if n.startswith("KC_"):
        n = n[3:]
    return ALIASES.get(n, n)


@dataclass(frozen=True)
class Action:
    """One timeline entry: at `t` ms after the start, press or release `pos`."""
    t: int
    kind: str  # "down" | "up"
    pos: int


@dataclass(frozen=True)
class Expected:
    kind: str                   # "keyboard" | "consumer"
    value: frozenset | str      # keyboard: set of names; consumer: usage name or "none"
    at: int | None = None       # optional timing assertion, ms from scenario start

    def describe(self) -> str:
        body = describe_value(self.kind, self.value)
        return body if self.at is None else f"{body} @{self.at}ms"


def describe_value(kind: str, value) -> str:
    if kind == "consumer":
        return f"consumer:{value}"
    return "[" + ", ".join(sorted(value, key=_order)) + "]"


def _order(name: str):
    return (0, MODS.index(name)) if name in MODS else (1, name)


@dataclass
class Scenario:
    name: str
    os: str
    actions: list[Action]
    duration: int               # ms the runner must keep running after the last action
    expect: list[Expected]
    description: str = ""
    index: int = 0
    source: str = ""


@dataclass
class Suite:
    keymap: str
    path: Path
    scenarios: list[Scenario] = field(default_factory=list)


# The step semantics. Every adapter replays the resulting timeline; none re-interprets
# the YAML. A press or release takes effect at its timestamp, and consecutive actions are
# 1 ms apart (one scan), so `down: [a, b]` and a `down: a` / `down: b` pair are the same.
def _timeline(steps, tap_ms: int, where: str) -> tuple[list[Action], int]:
    t = 0
    actions: list[Action] = []
    held: set[int] = set()

    def act(kind, pos):
        nonlocal t
        if not isinstance(pos, int) or not 0 <= pos < POSITIONS:
            raise ScenarioError(f"{where}: position {pos!r} is not in 0..{POSITIONS - 1}")
        if kind == "down" and pos in held:
            raise ScenarioError(f"{where}: position {pos} pressed while already down")
        if kind == "up" and pos not in held:
            raise ScenarioError(f"{where}: position {pos} released while not down")
        (held.add if kind == "down" else held.discard)(pos)
        actions.append(Action(t, kind, pos))
        t += 1

    for i, step in enumerate(steps):
        here = f"{where}, step {i + 1}"
        if not isinstance(step, dict) or len(step) != 1 and not ("tap" in step and set(step) <= {"tap", "hold"}):
            raise ScenarioError(f"{here}: expected one of down/up/tap/wait, got {step!r}")
        if "wait" in step:
            ms = step["wait"]
            if not isinstance(ms, int) or ms < 0:
                raise ScenarioError(f"{here}: wait needs a non-negative integer (ms)")
            t += ms
        elif "down" in step or "up" in step:
            kind = "down" if "down" in step else "up"
            for pos in step[kind] if isinstance(step[kind], list) else [step[kind]]:
                act(kind, pos)
        elif "tap" in step:
            hold = step.get("hold", tap_ms)
            if not isinstance(hold, int) or hold < 1:
                raise ScenarioError(f"{here}: hold needs a positive integer (ms)")
            act("down", step["tap"])
            t += hold - 1
            act("up", step["tap"])
        else:
            raise ScenarioError(f"{here}: unknown step {step!r}")
    if held:
        raise ScenarioError(f"{where}: positions {sorted(held)} are still down at the end")
    return actions, t


def _expected(entry, where: str) -> Expected:
    at = None
    if isinstance(entry, dict) and "report" in entry:
        at = entry.get("at")
        if set(entry) - {"report", "at"}:
            raise ScenarioError(f"{where}: unknown keys {sorted(set(entry) - {'report', 'at'})}")
        entry = entry["report"]
    elif isinstance(entry, dict) and "consumer" in entry:
        at = entry.get("at")
        usage = key_name(entry["consumer"]) if entry["consumer"] not in (None, "none") else "none"
        if usage != "none" and usage not in CONSUMER:
            raise ScenarioError(f"{where}: unknown consumer usage {entry['consumer']!r}")
        return Expected("consumer", usage, at)
    if not isinstance(entry, list):
        raise ScenarioError(f"{where}: expected a report list, {{report: ...}} or {{consumer: ...}}")
    if at is not None and (not isinstance(at, int) or at < 0):
        raise ScenarioError(f"{where}: `at` needs a non-negative integer (ms)")
    names = frozenset(key_name(k) for k in entry)
    if len(names - set(MODS)) > 6:
        raise ScenarioError(f"{where}: a keyboard report holds at most 6 keys")
    return Expected("keyboard", names, at)


def load(path: Path) -> Suite:
    data = yaml.safe_load(path.read_text(encoding="utf-8"))
    if not isinstance(data, dict) or "keymap" not in data or "scenarios" not in data:
        raise ScenarioError(f"{path}: needs top-level `keymap` and `scenarios`")
    defaults = data.get("defaults") or {}
    suite = Suite(keymap=data["keymap"], path=path)
    names = set()
    for i, raw in enumerate(data["scenarios"]):
        where = f"{path.name}: scenario {i + 1}"
        if not isinstance(raw, dict) or "name" not in raw:
            raise ScenarioError(f"{where}: needs a `name`")
        where = f"{path.name}: {raw['name']!r}"
        if raw["name"] in names:
            raise ScenarioError(f"{where}: duplicate scenario name")
        names.add(raw["name"])
        os_ = raw.get("os", defaults.get("os", "linux"))
        if os_ not in OSES:
            raise ScenarioError(f"{where}: os must be one of {OSES}")
        actions, end = _timeline(raw.get("steps") or [], raw.get("tap", defaults.get("tap", 10)), where)
        expect = [_expected(e, f"{where}, expect {j + 1}") for j, e in enumerate(raw.get("expect") or [])]
        suite.scenarios.append(Scenario(
            name=raw["name"], os=os_, actions=actions, duration=end, expect=expect,
            description=raw.get("description", ""), index=i, source=str(path),
        ))
    return suite
