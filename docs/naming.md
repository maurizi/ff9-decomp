# Naming symbols

Function and global names replace splat's `func_XXXXXXXX` / `D_XXXXXXXX` placeholders. A wrong name is worse than
none, so every name needs evidence, and its source is recorded in the symbols file entry.

## Evidence, strongest first
Tag each entry in `config/symbols*.txt` with its source in brackets (splat parses `key:value` in these comments,
so no colons in the tag):

| tag | meaning | example |
|---|---|---|
| `[string]` | Square's own name, from a debug or error string inside the function | `Hi_DrawEffModel`: `"Hi_DrawEffModel()\nid:%d cannot use!"` |
| `[bios A(3Fh)]` | BIOS call stub, by table and call number (psx-spx names) | `printf` |
| `[runtime]` | observed in the running game; entry in `docs/runtime-findings.md` | |
| `[code]` | reasoned from the code; say which functions show it | |

The comments already in `include/*.h` were written while matching code and were never checked. Treat them
as leads, not evidence.

## Conventions
- Square's own names are kept verbatim, prefixes included (`w_` world map, `Hi_` battle effect models).
- Library and BIOS functions use the PsyQ names. psx-spx's `…2` variants (`InitPAD2`) are used where PsyQ
  already uses the plain name for its wrapper.
- Otherwise: functions `subsystem_verbObject` in lower camelCase after the prefix (as Square's names are),
  globals `g_camelCase`, file-local statics `s_camelCase`, types `PascalCase`.
- Leave a placeholder when unsure. `D_80077170 /* packet pointer? */` beats a confident wrong name.

## Workflow
```
python3 tools/rename.py --from build/names.txt --build   # "OLD NEW  # [tag] why" per line
```
`tools/rename.py` writes the symbols file entry, renames uses in every C file and header that can see the
symbol (main: everything; an overlay: its own src/ and header), and updates `config/functions.us.txt`.
`--build` then runs `make split` (asm only; `src/` is untouched, no resplit needed) and `make verify`.
A name that gcc treats as a builtin (`memcpy`, `memset`, `strlen`) can change codegen. See the memcpy note in
`include/psyq.h`.

`make progress` prints naming progress per binary (functions named, globals named, unkXX struct fields left).
`python3 tools/progress.py --unnamed [--bin B]` lists placeholders by how often C refers to them.
`make progress CSV=1` appends to `docs/naming.csv` along with `docs/progress.csv`.

## Struct fields
Rename the members in the header by hand, then `python3 tools/rename_fields.py unk10=cur.hp,unkB=level` fixes the
uses: it builds, and rewrites a member on each line gcc rejects, but only when that line uses the member once
(the error proves that use is on the renamed struct). Lines using it more than once are listed for a hand fix.
Don't fix uses by variable name: `c` is a `Character *` in one function and a battle unit in the next.
