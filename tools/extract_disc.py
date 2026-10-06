#!/usr/bin/env python3
"""Extract files from a raw 2352-byte/sector PS1 disc image (CloneCD .img or .bin).

Usage: extract_disc.py IMAGE OUTDIR [FILE ...]
With no FILE arguments, lists the root directory.
"""
import os
import struct
import sys

RAW_SECTOR = 2352
USER_OFFSET = 24  # Mode 2 Form 1: 12 sync + 4 header + 8 subheader
USER_SIZE = 2048


class Disc:
    def __init__(self, path):
        self.f = open(path, "rb")
        pvd = self.read(16, USER_SIZE)
        if pvd[1:6] != b"CD001":
            sys.exit(f"{path}: no ISO9660 PVD at sector 16 (not a raw 2352 image?)")
        root = pvd[156 : 156 + 34]
        self.root = (struct.unpack_from("<I", root, 2)[0], struct.unpack_from("<I", root, 10)[0])

    def read(self, lba, size):
        out = bytearray()
        for i in range((size + USER_SIZE - 1) // USER_SIZE):
            self.f.seek((lba + i) * RAW_SECTOR + USER_OFFSET)
            out += self.f.read(USER_SIZE)
        return bytes(out[:size])

    def listdir(self, lba, size):
        data = self.read(lba, size)
        i = 0
        while i < len(data):
            n = data[i]
            if n == 0:
                i = (i // USER_SIZE + 1) * USER_SIZE
                continue
            name = data[i + 33 : i + 33 + data[i + 32]]
            if name not in (b"\x00", b"\x01"):
                yield (
                    name.decode("ascii").split(";")[0],
                    struct.unpack_from("<I", data, i + 2)[0],
                    struct.unpack_from("<I", data, i + 10)[0],
                    bool(data[i + 25] & 2),
                )
            i += n

    def lookup(self, path):
        lba, size = self.root
        parts = [p for p in path.replace("\\", "/").split("/") if p]
        for depth, part in enumerate(parts):
            for name, e_lba, e_size, is_dir in self.listdir(lba, size):
                if name.upper() == part.upper():
                    lba, size = e_lba, e_size
                    break
            else:
                sys.exit(f"not found on disc: {path}")
        return lba, size


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    disc = Disc(sys.argv[1])
    outdir = sys.argv[2]
    files = sys.argv[3:]
    if not files:
        for name, lba, size, is_dir in disc.listdir(*disc.root):
            print(f"{'D' if is_dir else ' '} {lba:8d} {size:10d} {name}")
        return
    os.makedirs(outdir, exist_ok=True)
    for path in files:
        lba, size = disc.lookup(path)
        dest = os.path.join(outdir, os.path.basename(path))
        with open(dest, "wb") as out:
            # Stream in chunks; FF9.IMG is ~400MB
            chunk = 4096
            for off in range(0, size, chunk * USER_SIZE):
                n = min(chunk * USER_SIZE, size - off)
                out.write(disc.read(lba + off // USER_SIZE, n))
        print(f"{path} -> {dest} ({size} bytes)")


if __name__ == "__main__":
    main()
