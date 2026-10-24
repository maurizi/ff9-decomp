# ff9-decomp

A matching decompilation of Final Fantasy IX (PS1, NTSC-U, SLUS-01251): C source that compiles back to
byte-identical copies of the boot executable and the code overlays.

This repository contains no game data. To build it you need your own copy of the game. Final Fantasy IX © is
owned by Square Enix; this project is not affiliated with or endorsed by Square Enix.

## Building

Requires Docker, Python 3, and raw 2352-byte images of the four discs at `disks/us/raw/FF9 CD1.img` …
`FF9 CD4.img`.

```
git submodule update --init
make extract extract-img extract-ovl
tools/docker.sh make split
tools/docker.sh make verify
```

`CLAUDE.md` describes the workflow in more detail.

## Progress

All 3840 game functions and 1421 battle effect functions are decompiled to matching C, and all 20 binaries
and 380 battle effect modules are fully decompiled. PsyQ SDK library code is identified by signature and left as assembly.

`make progress` prints the current per-binary numbers, and `docs/progress.csv` records them over time.

## License

The scripts in `tools/` written for this project are released under CC0 (`tools/LICENSE`). The submodules under
`tools/` keep their own licenses.
