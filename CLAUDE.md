# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

Matching decompilation of Final Fantasy IX (PS1, NTSC-U, 4 discs). Goal: C source that compiles to byte-identical binaries.

## Toolchain runs in Docker
- Everything except disc extraction and `make progress` runs in the container: `tools/docker.sh make verify` or `tools/docker.sh <cmd>`. The image (`tools/docker/Dockerfile`) rebuilds itself when the Dockerfile or `requirements.txt` changes. The host has no mips binutils.
- Submodules: `tools/maspsx`, `tools/m2c`, `tools/asm-differ`. Run `git submodule update --init` after cloning.

## Decomp workflow
1. Pick a target: `make progress` for totals, or `python3 tools/progress.py --list --min 40 --max 160 [--range 0x80026000-0x80030000]` for remaining game functions with address and size.
2. Draft: `tools/docker.sh python3 tools/m2c/m2c.py asm/us/main/nonmatchings/main/<func>.s`. m2c guesses arguments from register use. A function that only touches `$a3` still takes 4 params, and `jalr` on `$a2` doesn't mean `$a2` is a parameter.
3. Batch-test in isolation: write several functions in a scratch file under `build/` (e.g. `build/batchN.c`) that only does `#include "ff9.h"`, then run `tools/docker.sh python3 tools/ccprobe.py build/batchN.c --cc gcc-2.8.0-psx --aspsx 2.77`. A `*` means a byte match with relocations masked. A compile error prints `FAILED`, so don't read a missing score as a match.
4. Near misses: add `--show func_X` to ccprobe for our output next to the original. Typical fixes: param/return types (`andi`/`lbu` show u8, `sll`/`sra 16` show s16, `multu` shows unsigned), statement order, a temporary in a new local, and pointer-vs-index loops. Register-allocation-only differences go to the permuter: `tools/docker.sh python3 tools/permute.py <file.c> func_X --run -j 8` (results in `build/permuter/func_X/output-*/source.c`). Clean up its output before keeping it.
5. `python3 tools/integrate.py build/batchN.c [func ...]` moves function bodies over their `INCLUDE_ASM` lines in `src/main/main.c`. Then `tools/docker.sh make verify` (under 1 s); only a sha1 `OK` counts.
6. In a built tree, `tools/docker.sh python3 tools/asm-differ/diff.py --no-pager -s <func>` also works. `-s` stops at `jr ra`.
- **All declarations live in headers** (`include/ff9.h` for game code, `include/psyq.h` for SDK types, macros and prototypes), so batch files and `main.c` share them. `main.c`'s preamble is just `#include "ff9.h"`. Use `unkXX` field names (XX = hex offset) with pad arrays until the meaning is known, and prefer struct fields over byte-offset casts.
- Square's code uses PsyQ macros such as `addPrim`/`setlen`, which compile to the bitfield mask sequences. It also inlines helpers: `func_80027F34` only matches by calling `static inline func_80027EDC_inline` (in `ff9.h`).

## Compiler (verified empirically, not documented anywhere)
- FF9 game code is **gcc 2.8.x** with `-O2 -G0`, plus maspsx `--expand-div` (divides carry the ASPSX `break 7`/`break 6` guards). In `tools/probes/compiler_id_game.c`, 6 of 6 Square functions match only under 2.8.0 and 2.8.1; 2.7.2-cdk (FF8's main compiler) misses 3. 2.8.0 and 2.8.1 have produced identical output so far, so it's unknown which was used.
- `ASPSX_VERSION=2.77` is a guess; nothing tested so far depends on it.
- PsyQ library code was built by Sony with a different toolchain (e.g. `CdInit` stores globals via `lui $at`), so don't use library functions to test compiler hypotheses.

## Library code
- 229 PsyQ functions were identified by signature (`tools/psyq_match.py`, using lab313ru/psx_psyq_signatures cloned outside the repo) and listed in `config/symbols.psyq.us.txt`. They match libs from PsyQ 4.1–4.7. They are **interleaved** with game code (dense at 0x80012000–0x80024000), not just at the end of `.text`. Don't decompile them; `progress.py` excludes them.
- Unnamed functions next to library clusters may be unsigned library code (e.g. libds). Check what calls them before investing in one.

## Splitting
- `.text` is one C subsegment (`src/main/main.c`) with per-function asm in `asm/us/main/nonmatchings/main/`. `.rodata` is paired with it (`.rodata, main`), so jump tables migrate into the function files. Unpaired rodata fails to link on `.L` labels.
- splat **never rewrites an existing `.c` file**. After adding symbols or changing the yaml, run `tools/docker.sh make resplit` (`tools/resplit.py`): it regenerates `main.c` and merges the decompiled bodies and preamble back in. Plain `make split` is only for the first split. Never delete `main.c` to regenerate it.
- Symbols go in `config/symbols.us.txt` (`name = 0xADDR; // type:func`), then `make resplit`. Renaming a decompiled function also needs its C definition renamed.

## Game data — never commit
- Disc images (raw 2352-byte CloneCD `.img`) are in `disks/us/raw/`; the extracted exe is `disks/us/SLUS_012.51`, and `FF9.IMG` is in `disks/us/discN/` (`make extract-img`). Never commit `disks/`, `asm/`, `build/` or `assets/`.

## Layout facts
- Boot exe `SLUS_012.51` (sha1 `e37e8c4d…1764`) is **byte-identical** on all 4 discs. Per-disc differences live in `FF9.IMG`, which starts with an `FF9 ` magic and a directory table (format not yet reverse-engineered); most game logic is probably in overlays there.
- Exe: vram `0x80010000`, entry `__SN_ENTRY_POINT` `0x80012B68`, gp=0. `.rodata` 0x800 → `.text` 0x2FA0 → `.data` 0x56DE0–end.
- `D_8006794C` is the main game-state pointer (`GameState` in `include/ff9.h`); `->unk1C` points to a large `GameData` block.
