# Poryscript in this project

Event scripts can be written in Poryscript instead of raw `scripts.inc`.

- Write `data/maps/<Map>/scripts.pory` (or `data/scripts/<name>.pory`).
- `make` compiles every `.pory` to the `.inc` beside it before scanning
  sources, using `font_config.json` and `command_config.json` from this
  directory. Commit both the `.pory` and the generated `.inc`; CI fails if a
  committed `.inc` is stale against its `.pory`.
- A map has either a hand-written `scripts.inc` or a `scripts.pory`. Once a
  map has a `.pory`, do not edit its `.inc` by hand; it is overwritten.
- The compiler binary is fetched by `make tools` (release 3.6.1, checksum
  pinned in the Makefile here) and is not committed.

Reference: https://github.com/huderlem/poryscript#readme
