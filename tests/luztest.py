#!/usr/bin/env -S uv run --script
# /// script
# requires-python = ">=3.10"
# dependencies = ["pyyaml>=6"]
# ///
"""Entry point: `uv run tests/luztest.py run` (uv fetches PyYAML), or
`python3 -m luztest run` from tests/ when PyYAML is already installed."""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from luztest.cli import main  # noqa: E402

sys.exit(main())
