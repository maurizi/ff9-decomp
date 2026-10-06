# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

Matching decompilation of Final Fantasy IX (PS1, NTSC-U, 4 discs). Goal: C source that compiles to byte-identical binaries.

## Toolchain runs in Docker
- Everything except disc extraction runs in the container: `tools/docker.sh make verify`, `tools/docker.sh make split`, `tools/docker.sh <cmd>`. The image (`tools/docker/Dockerfile`) rebuilds automatically when the Dockerfile or `requirements.txt` changes. The host has no mips binutils.
- In the image: mipsel binutils, cpp, splat/spimdisasm venv on PATH, and the PSX compilers under `/opt/cc/<gcc-ver>/cc1`.
- maspsx and m2c are git submodules (`tools/maspsx`, `tools/m2c`). Run `git submodule update --init` after cloning.
- Draft C for a function with `tools/docker.sh python3 tools/m2c/m2c.py <func.s>`. Cut the function out of the splat asm between `nonmatching func_X` and `endlabel func_X`.

## Compiler (verified empirically, not documented anywhere)
- FF9 game code matches **gcc 2.8.x** (`gcc-2.8.0-psx` and `gcc-2.8.1-psx` gave identical output; it's not yet known which one). The flags are `-O2 -G0`, plus maspsx `--expand-div`, because divides carry the ASPSX `break 7`/`break 6` guards. Evidence: `tools/probes/compiler_id.c` has 3 functions that match byte-for-byte under 2.8.x and match under no other compiler. 2.7.2-cdk (FF8's main compiler) is close but never exact.
- `ASPSX_VERSION=2.77` is a guess; the probes didn't depend on it.
- Some units probably use different flags (as in FF8). `func_80021140` stores 4-byte globals via `lui $at`/`sw %lo($at)`, which `-O2 -G0` doesn't produce, but global `-G4` breaks other functions. Expect per-file flag overrides.
- `tools/docker.sh python3 tools/ccprobe.py file.c [--cc ...] [--flags ...]` compiles `func_XXXXXXXX` functions with every compiler and scores them word-by-word against the original, with relocations masked. Use it to test compiler or flag hypotheses.

## Game data — never commit
- Disc images (raw 2352-byte CloneCD `.img`) live in `disks/us/raw/` (copied from the SMB share; override with `make DISC_DIR=...`). The extracted exe is `disks/us/SLUS_012.51`, and `FF9.IMG` goes to `disks/us/discN/` via `make extract-img`.
- Never commit anything in `disks/`, `asm/`, `build/` or `assets/`.

## Layout facts
- Boot exe `SLUS_012.51` (sha1 `e37e8c4d…1764`) is **byte-identical** on all 4 discs. Decompile it once. Per-disc differences are in `FF9.IMG` (overlays and assets; format not yet reverse-engineered).
- Exe: vram `0x80010000`, entry `0x80012B68`, gp=0. File order: `.rodata` 0x800 → `.text` 0x2FA0 → `.data` 0x56DE0–end. About 1560 functions. The start of `.text` (around `0x80012B68`) is hand-written SN startup asm and isn't compiler output.
- Linked against PsyQ libs from about 1997–98 (`libgpu sys.c v1.140 1998/01/12`).

## Commands
- `tools/docker.sh make split`: re-run splat (`config/slus_012.51.yaml`). Splat prints suggested `.rodata` file splits; apply them when splitting files out of `2FA0`.
- `tools/docker.sh make verify`: assemble/compile, link, objcopy, then sha1 check against `config/checksums.us.sha1`. This is the only proof of a match.
- C objects: `src/main/*.c` → cpp → `$(CC_VER)/cc1 $(CC_FLAGS)` → maspsx → as. Override `CC_VER`, `CC_FLAGS` and `ASPSX_VERSION` on the make command line.
- Symbols go in `config/symbols.us.txt` (`name = 0xADDR; // type:func`); re-split afterwards.
