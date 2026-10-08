# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

Matching decompilation of Final Fantasy IX (PS1, NTSC-U, 4 discs). Goal: C source that compiles to byte-identical binaries.

## Toolchain runs in Docker
- Everything except disc extraction and `make progress` runs in the container: `tools/docker.sh make verify` or `tools/docker.sh <cmd>`. The image (`tools/docker/Dockerfile`) rebuilds itself when the Dockerfile or `requirements.txt` changes. The host has no mips binutils.
- Submodules: `tools/maspsx`, `tools/m2c`, `tools/asm-differ`. Run `git submodule update --init` after cloning.

## Binaries
- 20 binaries, all rebuilt byte-identically by `make verify`: the boot exe (`src/main/`) and 19 code overlays from `FF9.IMG` dir 0 (`src/ovl_XX/`, XX = file id). Overlays are identical on all 4 discs. Format, load addresses and what each overlay probably is: `docs/ff9img.md` and `config/overlays.us.txt`. The overlays hold about 70% of the game code.
- Overlay yamls come from `tools/gen_ovl_configs.py`, which never overwrites an existing yaml (`--force` does). Hand edits stay, e.g. ovl_0b's `textbin` table and its second C file `ovl_0b_2`. Overlays need `subalign: 4` and no section-end alignment, otherwise the build gains 0x10 of padding.
- Overlays at the same address overlap (six load at 0x800A7000), so their symbols mean nothing outside that overlay. Overlay-local declarations go in `include/ovl_XX.h` (which includes `ff9.h`), and `src/ovl_XX/*.c` includes that header.
- splat prints "Unable to determine a segment for … user-declared symbols" for every overlay because the main exe's symbol files are shared. It's harmless: overlay asm still uses the names (`DrawSync`, …).

## Decomp workflow
1. Pick a target: `make progress` shows per-binary totals. `python3 tools/progress.py --list [--bin ovl_0a] --min 40 --max 160 [--range 0x80026000-0x80030000]` lists remaining game functions.
2. Draft: `tools/docker.sh python3 tools/m2c/m2c.py asm/us/main/nonmatchings/main/<func>.s`. m2c guesses arguments from register use. A function that only touches `$a3` still takes 4 params, and `jalr` on `$a2` doesn't mean `$a2` is a parameter.
3. Batch-test in isolation: write several functions in a scratch file under `build/` (e.g. `build/batchN.c`) that only includes `ff9.h` (or `ovl_XX.h`), then run `tools/docker.sh python3 tools/ccprobe.py build/batchN.c --cc gcc-2.8.0-psx --aspsx 2.77 [--bin ovl_XX]`. A `*` means a byte match with relocations masked. A compile error prints `FAILED`, so don't read a missing score as a match.
4. Near misses: add `--show func_X` to ccprobe for our output next to the original. Typical fixes: param/return types (`andi`/`lbu` show u8, `sll`/`sra 16` show s16, `multu` shows unsigned), statement order, a temporary in a new local, and pointer-vs-index loops. Register-allocation-only differences go to the permuter: `tools/docker.sh python3 tools/permute.py <file.c> func_X --run -j 8` (results in `build/permuter/func_X/output-*/source.c`). Clean up its output before keeping it.
5. `python3 tools/integrate.py build/batchN.c [func ...] [--into src/ovl_XX/ovl_XX.c]` moves function bodies over their `INCLUDE_ASM` lines (default target `src/main/main.c`). Then `tools/docker.sh make verify` (under 1 s); only a sha1 `OK` counts.
6. In a built tree, `tools/docker.sh python3 tools/asm-differ/diff.py --no-pager -s <func>` also works. `-s` stops at `jr ra`.
- **All declarations live in headers** (`include/ff9.h` for game code, `include/psyq.h` for SDK types, macros and prototypes), so batch files and `main.c` share them. `main.c`'s preamble is just `#include "ff9.h"`. Use `unkXX` field names (XX = hex offset) with pad arrays until the meaning is known, and prefer struct fields over byte-offset casts.
- `ccprobe` masks relocated fields, so a wrong struct offset or relocation addend can still score `*`. Treat it as a filter; only `make verify` proves a match.
- Jump tables: gcc emits `.align 3` for them, but the original objects were packed at 4 bytes, so all compile paths (Makefile, ccprobe, permute) rewrite it to `.align 2`. Switch functions that failed `verify` before this fix (e.g. ovl_15 `func_800A713C`) are worth retrying.
- Permuter on an overlay function: `tools/permute.py <file> <func> --bin ovl_XX --run -j 8`. `tools/docker.sh` runs containers with `--init`, so `timeout` really stops them; check `docker ps` for strays anyway.
- Square's code uses PsyQ macros such as `addPrim`/`setlen`, which compile to the bitfield mask sequences. It also inlines helpers: `func_80027F34` only matches by calling `static inline func_80027EDC_inline` (in `ff9.h`).

## Compiler (verified empirically, not documented anywhere)
- FF9 game code is **gcc 2.8.x** with `-O2 -G0`, plus maspsx `--expand-div` (divides carry the ASPSX `break 7`/`break 6` guards). In `tools/probes/compiler_id_game.c`, 6 of 6 Square functions match only under 2.8.0 and 2.8.1; 2.7.2-cdk (FF8's main compiler) misses 3. 2.8.0 and 2.8.1 have produced identical output so far, so it's unknown which was used.
- `ASPSX_VERSION=2.77` is a guess; nothing tested so far depends on it.
- PsyQ library code was built by Sony with a different toolchain (e.g. `CdInit` stores globals via `lui $at`), so don't use library functions to test compiler hypotheses.

## Library code
- 229 PsyQ functions in the main exe were identified by signature (`tools/psyq_match.py [--bin PATH:VRAM:TEXT_START:TEXT_END]`, using lab313ru/psx_psyq_signatures cloned outside the repo) and listed in `config/symbols.psyq.us.txt`. Overlays contain essentially no library code; the exception is ovl_64 (FMV player) with libpress, in `config/symbols.psyq.ovl_64.us.txt`. Single 32-byte signature hits are noise. They match libs from PsyQ 4.1–4.7. They are **interleaved** with game code (dense at 0x80012000–0x80024000), not just at the end of `.text`. Don't decompile them; `progress.py` excludes them.
- Unnamed functions next to library clusters may be unsigned library code (e.g. libds). Check what calls them before investing in one.

## Splitting
- `.text` is one C subsegment (`src/main/main.c`) with per-function asm in `asm/us/main/nonmatchings/main/`. `.rodata` is paired with it (`.rodata, main`), so jump tables migrate into the function files. Unpaired rodata fails to link on `.L` labels.
- splat **never rewrites an existing `.c` file**. After adding symbols or changing a yaml, run `tools/docker.sh make resplit` (`tools/resplit.py`): it re-splits every binary and merges each file's preamble and hand-written bodies back in, even when a function moves to a different file. If it reports missing slots it keeps `*.bak` files and refuses to run again until they're resolved; merge by hand, then delete them. Never delete a `.c` file to regenerate it. Commit before risky resplits.
- `make clean` or deleting `build/` also removes splat's linker scripts (`build/us/*.ld`), so run `make resplit` before building again.
- Symbols go in `config/symbols.us.txt` (`name = 0xADDR; // type:func`), then `make resplit`. Renaming a decompiled function also needs its C definition renamed.

## Runtime investigation (emulator)
- `tools/emu.py` (host-side, not Docker) drives PCSX-Redux running FF9: trace calls to a function, watch who writes a global, diff RAM across a button press, take screenshots, and use save states. Setup and workflow: `docs/emulator.md`. The `game-tester` subagent (`.claude/agents/game-tester.md`) does this work and appends evidence plus proposed names to `docs/runtime-findings.md`; applying those names is the decomp side's job.

## Game data — never commit
- Disc images (raw 2352-byte CloneCD `.img`) are in `disks/us/raw/`. The extracted exe is `disks/us/SLUS_012.51`, `FF9.IMG` is in `disks/us/discN/` (`make extract-img`), and the code overlays are in `disks/us/ovl/XX.bin` (`make extract-ovl`). Never commit `disks/`, `asm/`, `build/` or `assets/`.

## Layout facts
- Boot exe `SLUS_012.51` (sha1 `e37e8c4d…1764`) is **byte-identical** on all 4 discs, as are the code overlays. Per-disc differences are data in other `FF9.IMG` directories.
- Exe: vram `0x80010000`, entry `__SN_ENTRY_POINT` `0x80012B68`, gp=0. `.rodata` 0x800 → `.text` 0x2FA0 → `.data` 0x56DE0–end.
- `D_8006794C` is the main game-state pointer (`GameState` in `include/ff9.h`); `->unk1C` points to a large `GameData` block.

## Parallel agents (worktrees)
- In a new worktree, run `tools/worktree_setup.sh /path/to/main/checkout` before anything else. It copies the game binaries and submodule checkouts in and splits; Docker only mounts the worktree.
- Each agent owns specific binaries and edits only `src/<its binaries>/`, `include/ovl_XX.h` for those overlays, and scratch files under `build/`. Only the agent assigned to `main` edits `src/main/`, `include/ff9.h` and `include/psyq.h`. Nobody else edits configs, tools, `CLAUDE.md` or other agents' files.
- An overlay that needs main-exe or PsyQ declarations missing from `ff9.h`/`psyq.h` declares them in its own `ovl_XX.h` under a `/* main exe: candidates for ff9.h */` heading. The coordinator hoists them later.
- Commit after every verified batch, i.e. when `tools/docker.sh make -j8 verify` shows all 20 binaries OK. Commit messages look like `ovl_37: decompile 12 functions`.
