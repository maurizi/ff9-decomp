#!/usr/bin/env python3
"""Generate splat configs for the battle effect modules (FF9.IMG dir 13; docs/ff9img.md).

Reads config/fxmods.us.txt (tools/effect_modules.py catalogue) and the extracted images in
disks/us/fxmod/ (make extract-fxmod). For each module it writes, unless the file exists already:
    config/fxmod/<name>.yaml        splat config: .rodata (header pointer, jump tables), c, data
    config/fxmod/<name>.syms.txt    header and op handler symbols (from the header, see below)
and always rewrites config/checksums.fxmod.us.sha1. --force rewrites yamls and symbol files too.

Binary name = module name (fx_0b2), so asm goes to asm/us/fx_0b2/ and C to src/fxmod/fx_0b2/fx_0b2.c
(one directory per module: modules share addresses, and tools/resplit.py matches bodies per directory).
"""
import hashlib
import os
import struct
import sys

TEMPLATE = """name: {name}
sha1: {sha1}
options:
  basename: {name}
  target_path: disks/us/fxmod/{name}.bin
  base_path: ../..
  platform: psx
  compiler: GCC
  generated_c_preamble: '#include "btlfx_mod.h"'

  asm_path: asm/us/{name}
  src_path: src/fxmod/{name}
  build_path: build/us
  ld_script_path: build/us/{name}.ld
  ld_dependencies: yes
  elf_path: build/us/{name}.elf

  symbol_addrs_path:
    - config/fxmod/{name}.syms.txt
  undefined_funcs_auto_path: config/fxmod/undefined_funcs_auto.{name}.txt
  undefined_syms_auto_path: config/fxmod/undefined_syms_auto.{name}.txt

  find_file_boundaries: yes
  use_legacy_include_asm: no
  migrate_rodata_to_functions: yes
  section_order: [".rodata", ".text", ".data", ".bss"]
  string_encoding: ASCII
  data_string_encoding: ASCII
  rodata_string_guesser_level: 2
  data_string_guesser_level: 2
  asm_jtbl_label_macro: jlabel
  ld_bss_is_noload: false
  subalign: 4
  ld_align_section_vram_end: false
  ld_align_segment_vram_end: false

segments:
  - name: {name}
    type: code
    start: 0x0
    vram: 0x{vram:X}
    bss_size: 0x0
    subsegments:
      - [0x0, .rodata, {name}]
      - [0x{text:X}, c, {name}]
      - [0x{data:X}, data]

  - [0x{size:X}]
"""


def symbols(name, blob, vram, text, data):
    """Word 0 points at the header {0, init, op80, op81, ...} (btlfx_updateLoader installs header + 8 as the
    op table, btlfx_spawnTask dispatches script op 0x80 + i to entry i)."""
    words = struct.unpack_from(f"<{len(blob) // 4}I", blob)
    hdr = words[0]
    out = [f"{name}_headerPtr = 0x{vram:08X}; // type:u32 size:0x4 [code] word 0 of a module, read by btlfx_updateLoader",
           f"{name}_header = 0x{hdr:08X}; // [code] module header (0, init, ops), btlfx_updateLoader"]
    seen = set()
    k = (hdr - vram) // 4 + 2
    for op in range(16):
        if k + op >= len(words):
            break
        w = words[k + op]
        if w == 0:
            continue
        if not vram + text <= w < vram + data:
            break
        if w in seen:
            continue
        seen.add(w)
        out.append(f"{name}_op{0x80 + op:02X} = 0x{w:08X}; // type:func [code] header op entry {op}, "
                   f"effect script op 0x{0x80 + op:02X} (btlfx_spawnTask)")
    return "\n".join(out) + "\n"


def main():
    force = "--force" in sys.argv
    os.makedirs("config/fxmod", exist_ok=True)
    sums = []
    for line in open("config/fxmods.us.txt"):
        f = line.split("#")[0].split()
        if not f:
            continue
        name = f[0]
        vram, text, data, size = (int(x, 16) for x in f[1:5])
        blob = open(f"disks/us/fxmod/{name}.bin", "rb").read()
        sha1 = hashlib.sha1(blob).hexdigest()
        if sha1 != f[5] or len(blob) != size:
            sys.exit(f"{name}: disks/us/fxmod/{name}.bin does not match config/fxmods.us.txt")
        yaml = f"config/fxmod/{name}.yaml"
        if force or not os.path.exists(yaml):
            with open(yaml, "w") as fh:
                fh.write(TEMPLATE.format(name=name, sha1=sha1, vram=vram, text=text, data=data, size=size))
        syms = f"config/fxmod/{name}.syms.txt"
        if force or not os.path.exists(syms):
            with open(syms, "w") as fh:
                fh.write(symbols(name, blob, vram, text, data))
        sums.append(f"{sha1}  build/us/{name}.bin")
    with open("config/checksums.fxmod.us.sha1", "w") as fh:
        fh.write("\n".join(sums) + "\n")
    print(f"{len(sums)} modules")


if __name__ == "__main__":
    main()
