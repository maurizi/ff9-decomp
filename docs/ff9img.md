# FF9.IMG format (NTSC-U)

Reverse-engineered for this project. Reader: `tools/ff9img.py`. All values little-endian; sectors are 2048 bytes (offsets are relative to the start of FF9.IMG).

## Header (sector 0)

| Offset | Type | Value |
|---|---|---|
| 0x0 | char[4] | `"FF9 "` |
| 0x4 | u32 | 6 (unknown) |
| 0x8 | u32 | number of directories (15) |
| 0xC | u32 | 1 (unknown) |
| 0x10 | dir[] | `{u32 type, u32 count, u32 dir_id, u32 first_sector}`; an entry with type 4 ends the list |

The rest of sector 0 is padded with repeated `"FF9 "`.

## File tables (sectors 1..)

Packed back to back from sector 1, one per directory in header order.

- **type 2**: `count` entries of `{u32 file_id, u32 sector}`, then `{0xFFFFFFFF, end_sector}`. A table may span several sectors (e.g. dir 5 with 337 files). File size = next entry's sector - this sector (so sizes are sector-rounded).
- **type 3** (only dir 13, 511 entries, ~70 MB on disc 1): different u16-based layout, not parsed yet.

## Directories (disc 1)

| idx | id | files | size | contents |
|---|---|---|---|---|
| 0 | 1 | 21 | 1.4 MB | **code overlays** (19 code files + 2 small data files); identical on all 4 discs |
| 1 | 2 | 34 | 5.2 MB | ? |
| 2 | 3 | 19 | 0.5 MB | ? |
| 3 | 4 | 15 | 1.3 MB | ? (ids look like packed u16 pairs) |
| 4 | 5 | 222 | 122.5 MB | ? (largest) |
| 5 | 6 | 337 | 2.0 MB | ? |
| 6 | 8 | 151 | 33.4 MB | ? |
| 7 | 9 | 101 | 12.3 MB | ? |
| 8 | 10 | 85 | 1.0 MB | ? |
| 9 | 11 | 55 | 14.8 MB | ? |
| 10 | 12 | 42 | 3.3 MB | ? |
| 11 | 13 | 427 | 50.6 MB | ? |
| 12 | 15 | 2 | 11.4 MB | ? |
| 13 | 16 | 511 | ~70 MB | type 3, unparsed |

Only dir 0 contains MIPS code (`tools/ff9img.py ... code` on all four discs).

## Code overlays (dir 0)

Load addresses were found by voting internal `jal` targets against function prologues, then confirmed by byte-identical rebuilds. See `config/overlays.us.txt` for boundaries and purpose guesses.

- **0x800A7000** (main overlay area; the exe's 1.3 MB zero-filled region starts here): 0x0A field, 0x0B battle, 0x0C world map, 0x0D Tetra Master, 0x14 opening/logos, 0x15 Blackjack.
- **0x801EDF00** (small slot ending below the stack): 0x32-0x3E, 0x65. Their strings use FF9's text encoding (letters are ASCII - 0x31).
- **0x80198700**: 0x64, a debug FMV player that links PsyQ libpress.

Each overlay is laid out like the exe: `.rodata`, `.text`, `.data`, packed with 4-byte alignment (splat `subalign: 4`, no section-end alignment). Overlay 0x0B embeds a u16 table inside `.text` at 0x28A80 (splat `textbin`) and is split into two C files there.
