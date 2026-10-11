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
