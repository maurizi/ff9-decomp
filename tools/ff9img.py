#!/usr/bin/env python3
"""FF9.IMG archive reader (format reverse-engineered here; see docs/ff9img.md).

    python3 tools/ff9img.py disks/us/disc1/FF9.IMG list            # dirs and files
    python3 tools/ff9img.py disks/us/disc1/FF9.IMG code            # files that look like MIPS code
    python3 tools/ff9img.py disks/us/disc1/FF9.IMG extract D F OUT # dir index D, file id F
    python3 tools/ff9img.py disks/us/disc1/FF9.IMG extract-dir D OUTDIR   # all files as <id>.bin

Layout (all little-endian, 2048-byte sectors):
  sector 0:   "FF9 " u32 ?(6) u32 ndirs u32 ?(1), then ndirs+1 entries of
              {u32 type, u32 count, u32 dir_id, u32 first_sector}; type 4 ends the list.
  sectors 1..: file tables, packed back to back. type 2: count x {u32 file_id, u32 sector},
              then {0xFFFFFFFF, end_sector}; a table may span several sectors.
              type 3: different (u16) format, not parsed yet.
"""
import struct
import sys

SECTOR = 2048


class Img:
    def __init__(self, path):
        self.f = open(path, "rb")
        hdr = self.read(0, 16)
        magic, _, ndirs, _ = struct.unpack("<4s3I", hdr)
        if magic != b"FF9 ":
            sys.exit("not an FF9.IMG")
        self.dirs = []
        for i in range(ndirs + 1):
            t, count, did, sec = struct.unpack_from("<4I", self.read(16 + i * 16, 16))
            if t == 4:
                break
            self.dirs.append({"index": i, "type": t, "count": count, "id": did, "sector": sec})
        # File tables are packed back to back from sector 1.
        tsec = 1
        for d in self.dirs:
            d["table"] = tsec
            if d["type"] == 2:
                tsec += ((d["count"] + 1) * 8 + SECTOR - 1) // SECTOR
            else:
                tsec += 1  # type 3: assumed single sector until its format is known

    def read(self, off, n):
        self.f.seek(off)
        return self.f.read(n)

    def files(self, d):
        """[(file_id, sector, size_bytes)] for a type-2 directory."""
        if d["type"] != 2:
            return []
        n = d["count"] + 1
        raw = self.read(d["table"] * SECTOR, n * 8)
        ents = [struct.unpack_from("<2I", raw, k * 8) for k in range(n)]
        out = []
        for k in range(d["count"]):
            fid, sec = ents[k]
            out.append((fid, sec, (ents[k + 1][1] - sec) * SECTOR))
        return out


def code_score(data):
    """(jr_ra count, prologue count) over word-aligned data."""
    jr = pro = 0
    for i in range(0, len(data) - 3, 4):
        w = struct.unpack_from("<I", data, i)[0]
        if w == 0x03E00008:
            jr += 1
        elif (w >> 16) == 0x27BD and (w & 0x8000):  # addiu sp, sp, -N
            pro += 1
    return jr, pro


def main():
    img = Img(sys.argv[1])
    cmd = sys.argv[2] if len(sys.argv) > 2 else "list"
    if cmd == "list":
        for d in img.dirs:
            fs = img.files(d)
            total = sum(s for _, _, s in fs)
            print(f"dir {d['index']:2} type {d['type']} id {d['id']:2} table {d['table']:3} files {d['count']:4} "
                  f"sector {d['sector']:7} size {total / 1e6:7.1f} MB")
    elif cmd == "code":
        for d in img.dirs:
            for fid, sec, size in img.files(d):
                if size > 4 * 1024 * 1024:
                    continue
                jr, pro = code_score(img.read(sec * SECTOR, size))
                if jr >= 4 and pro >= 4:
                    print(f"dir {d['index']:2} file {fid:#010x} sector {sec:7} size {size:8} jr_ra {jr:5} prologues {pro:5}")
    elif cmd == "extract-dir":
        import os
        d = img.dirs[int(sys.argv[3])]
        os.makedirs(sys.argv[4], exist_ok=True)
        for fid, sec, size in img.files(d):
            open(os.path.join(sys.argv[4], f"{fid:02x}.bin"), "wb").write(img.read(sec * SECTOR, size))
        print(f"wrote {d['count']} files to {sys.argv[4]}")
    elif cmd == "extract":
        d = img.dirs[int(sys.argv[3])]
        fid = int(sys.argv[4], 0)
        for f, sec, size in img.files(d):
            if f == fid:
                open(sys.argv[5], "wb").write(img.read(sec * SECTOR, size))
                print(f"wrote {size} bytes")
                return
        sys.exit("no such file")


if __name__ == "__main__":
    main()
