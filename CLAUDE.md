# Luz

Luz is documentation-first: the normative definition is the specification in `spec/`
(chapters, RFC 2119 wording, positions 0-41). Implementations follow it; when code and spec
disagree, fix the code (or fix the spec first, deliberately).

- `spec/`: the specification.
- `packages/qmk/`: the QMK userspace, holding the reference implementations, the community
  modules (`modules/luz/`) and the conformance scenarios (`tests/`). Open that directory to
  work on the firmware; its `CLAUDE.md` covers the code, builds and tests.
- `packages/keyspec/`: the behavioural test runner, a standalone package with no Luz
  assumptions.
- `README.md` is the tour; `CHANGELOG.md`'s `## v…` sections become the GitHub release notes.
