#!/usr/bin/env bash
# Prepare a fresh git worktree for building: copy the extracted game binaries and the
# submodule checkouts from the main checkout (Docker mounts only the worktree, so
# symlinks out of it don't work), then split. Usage, from the worktree root:
#   tools/worktree_setup.sh /path/to/main/checkout
set -euo pipefail

MAIN="${1:?path to the main checkout}"
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

mkdir -p disks/us/ovl
cp "$MAIN/disks/us/SLUS_012.51" disks/us/
cp "$MAIN"/disks/us/ovl/*.bin disks/us/ovl/

for sm in tools/maspsx tools/m2c tools/asm-differ tools/decomp-permuter; do
    mkdir -p "$sm"
    # Copy contents without the .git link (it points into the main checkout's .git).
    (cd "$MAIN/$sm" && tar cf - --exclude=.git .) | (cd "$sm" && tar xf -)
done

tools/docker.sh make resplit >/dev/null
tools/docker.sh make -j8 verify | tail -3
