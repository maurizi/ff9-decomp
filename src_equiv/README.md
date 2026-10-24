# src_equiv: behaviourally equivalent C for asm-only routines

Port-only C bodies (for the DS port, ../ff9-ds) for routines that are still assembly in `src/` because they are
hand-written (no ABI, global registers) or don't match yet. They are **not** byte-matching and the PS1 build
(`make verify`) never compiles this tree.

- One file per routine group: `src_equiv/<binary>/<group>.c`, each including the same headers as `src/`.
- Every function keeps the name (and C prototype) it has in `src/`/`include/`, so a port links these in place of
  the `INCLUDE_ASM` stubs.
- Where the original uses a custom register convention, the file's header comment documents it, and the C
  signature given is the one C callers should use.
- Each body is checked against the asm by reading; where a routine was also checked in the emulator, it says so.
- Files that compile with the PS1 compiler can be run against the original with `tools/equiv.py`
  (`tools/docker.sh python3 tools/equiv.py src_equiv/ovl_0b/math.c --bin ovl_0b --func btlfx_distance`). It compares
  the whole GTE register file, so register-level GTE code uses `GTE_MTC2`/`GTE_CMD`/… (`src_equiv.h`) and writes
  the same registers with the same words as the original. `--end` covers an entry that jumps into the next
  function's body, `--arg-mask` bounds a loop-count argument.
