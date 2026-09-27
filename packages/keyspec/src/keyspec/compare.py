"""Runner-neutral verdict: compare the reports a runner observed with a scenario's `expect`.

Rules (see tests/README.md):
  - the observed reports must equal the expected ones, in order, with nothing extra;
  - a keyboard report is compared as a set (modifier order and key slot don't matter);
  - an expectation with `at: T` must be observed within [T, T + tolerance] ms, where the
    tolerance is the runner's (its scan/debounce/USB latency), never earlier than T.
"""
from __future__ import annotations

from dataclasses import dataclass

from .scenario import Expected, Scenario, describe_value


@dataclass(frozen=True)
class Observed:
    t: int                      # ms from scenario start
    kind: str                   # "keyboard" | "consumer"
    value: frozenset | str

    def describe(self) -> str:
        return f"{describe_value(self.kind, self.value)} @{self.t}ms"


@dataclass
class Verdict:
    scenario: Scenario
    passed: bool
    problems: list[str]
    observed: list[Observed]


def _same(e: Expected, o: Observed) -> bool:
    return e.kind == o.kind and e.value == o.value


def compare(scenario: Scenario, observed: list[Observed], tolerance: int) -> Verdict:
    problems: list[str] = []
    exp = scenario.expect
    # After the first sequence mismatch the two lists are out of step, so stop there;
    # the CLI prints both sequences in full for a failing scenario.
    for i in range(max(len(exp), len(observed))):
        e = exp[i] if i < len(exp) else None
        o = observed[i] if i < len(observed) else None
        if e is None:
            problems.append(f"#{i + 1}: unexpected report {o.describe()}")
            break
        if o is None:
            problems.append(f"#{i + 1}: missing report {e.describe()}")
            break
        if not _same(e, o):
            problems.append(f"#{i + 1}: expected {e.describe()}, got {o.describe()}")
            break
        if e.at is not None and not (e.at <= o.t <= e.at + tolerance):
            problems.append(
                f"#{i + 1}: {describe_value(e.kind, e.value)} at {o.t}ms, "
                f"expected {e.at}..{e.at + tolerance}ms (tolerance {tolerance}ms)")
    return Verdict(scenario, not problems, problems, observed)
