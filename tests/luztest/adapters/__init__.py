"""Runner adapters. An adapter replays scenarios on some implementation of the firmware and
returns, per scenario, the timestamped reports the host would have received.

Contract (duck-typed):

    class Adapter:
        name: str
        def __init__(self, target: Target, settings: dict): ...
        tolerance: int                                   # ms, see compare.py
        def run(self, scenarios) -> dict[int, list[Observed] | str]
            # scenario.index -> observed reports, or an error string if it could not run

The adapter never judges; compare.py does.
"""
from . import qmk

ADAPTERS = {qmk.QmkAdapter.name: qmk.QmkAdapter}
