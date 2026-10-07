#!/usr/bin/env python3
"""Generate config/ovl_XX.yaml splat configs for the FF9.IMG dir-0 code overlays.

Boundaries come from config/overlays.us.txt (one line per overlay):
    <file id hex> <vram hex> <text start hex> <text end hex>   # comment
Existing yamls are kept (they may carry hand edits, e.g. textbin splits); pass --force to
regenerate them. Checksums and empty symbol files are always (re)written as needed.
"""
import hashlib
import os
import sys

TEMPLATE = """name: ovl_{id}
sha1: {sha1}
options:
  basename: ovl_{id}
  target_path: disks/us/ovl/{id}.bin
  base_path: ..
  platform: psx
  compiler: GCC

  asm_path: asm/us/ovl_{id}
  src_path: src/ovl_{id}
  build_path: build/us
  ld_script_path: build/us/ovl_{id}.ld
  ld_dependencies: yes
  elf_path: build/us/ovl_{id}.elf

  symbol_addrs_path:
    - config/symbols.us.txt
    - config/symbols.psyq.us.txt
    - config/symbols.ovl_{id}.us.txt
  undefined_funcs_auto_path: config/undefined_funcs_auto.ovl_{id}.us.txt
  undefined_syms_auto_path: config/undefined_syms_auto.ovl_{id}.us.txt

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
  - name: ovl_{id}
    type: code
    start: 0x0
    vram: 0x{vram:X}
    bss_size: 0x0
    subsegments:
      - [0x0, .rodata, ovl_{id}]
      - [0x{text:X}, c, ovl_{id}]
      - [0x{data:X}, data]

  - [0x{size:X}]
"""


def main():
    out_sums = []
    for line in open("config/overlays.us.txt"):
        line = line.split("#")[0].strip()
        if not line:
            continue
        fid, vram, text, data = (int(x, 16) for x in line.split())
        ident = f"{fid:02x}"
        path = f"disks/us/ovl/{ident}.bin"
        blob = open(path, "rb").read()
        sha1 = hashlib.sha1(blob).hexdigest()
        yaml = f"config/ovl_{ident}.yaml"
        if "--force" in sys.argv or not os.path.exists(yaml):
          with open(yaml, "w") as f:
            f.write(TEMPLATE.format(id=ident, sha1=sha1, vram=vram, text=text, data=data, size=len(blob)))
        syms = f"config/symbols.ovl_{ident}.us.txt"
        if not os.path.exists(syms):
            open(syms, "w").close()
        out_sums.append(f"{sha1}  build/us/ovl_{ident}.bin")
    with open("config/checksums.ovl.us.sha1", "w") as f:
        f.write("\n".join(out_sums) + "\n")


if __name__ == "__main__":
    main()
