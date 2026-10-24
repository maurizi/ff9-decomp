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
- **type 3** (only dir 13, 511 slots): `count + 1` u16 sector offsets relative to the directory's `first_sector`; 0xFFFF marks an absent id. A file ends where the next present one starts. The boot code copies the `count` u16s to RAM and `file_find(13, id)` indexes them directly (ovl_0b `btlfx_loadFile`: sector = battle data base + entry).

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
| 13 | 16 | 511 | ~70 MB | **battle effect files** (type 3; 372 present): effect data, textures and an effect code module each |

MIPS code lives in two places: dir 0 (the code overlays below) and dir 13, whose battle effect files each carry an effect code module (see "Battle effect files"). `tools/ff9img.py ... code` only scans type-2 directories, so it never saw dir 13.

## Code overlays (dir 0)

Load addresses were found by voting internal `jal` targets against function prologues, then confirmed by byte-identical rebuilds. See `config/overlays.us.txt` for boundaries and purpose guesses.

- **0x800A7000** (main overlay area; the exe's 1.3 MB zero-filled region starts here): 0x0A field, 0x0B battle, 0x0C world map, 0x0D Tetra Master, 0x14 opening/logos, 0x15 Blackjack.
- **0x801EDF00** (small slot ending below the stack): 0x32-0x3E, 0x65. Their strings use FF9's text encoding (letters are ASCII - 0x31).
- **0x80198700**: 0x64, the movie (FMV/MBG) player, which links PsyQ libpress. Field, opening and ending code load it there and call it (`docs/library-audit.md`). Its first string, `\TEST\FMV990.STR`, is a dead entry of an unused test list.

Each overlay is laid out like the exe: `.rodata`, `.text`, `.data`, packed with 4-byte alignment (splat `subalign: 4`, no section-end alignment). Overlay 0x0B embeds a u16 table inside `.text` at 0x28A80 (splat `textbin`) and is split into two C files there.

## Battle effect files (dir 13)

Read by ovl_0b's loader: `btlfx_loadFile(id)` resolves the start sector, `btlfx_updateLoader` (per frame) streams
the file group by group. Reader: `tools/effect_modules.py`; list of modules: `docs/effect-modules.md`.

Sector 0 of a file is a directory of chunks, all little-endian:

| field | type | meaning |
|---|---|---|
| ngroups | u16 | number of load groups |
| group | `{u16 kind, u16 nchunks}` | followed directly by its `nchunks` chunk records |
| chunk | `{u8 type, u8 flag, u16 sectors}` | type 2 with a nonzero flag has one more `u16` (sectors of common data read first) |

Chunk data follows from sector 1 in record order, each chunk sector-padded. Group 0 loads when the effect starts;
`func_800DA5C4` (exported to modules) requests the next group, toggling between two slots (`g_btlfxLoader.unk8`).
Chunk types, by what the loader does with them:

| type | destination / action |
|---|---|
| 0 | staged at the module area; header `{u32 next, u32 imgs, u32 nimgs}`, images `{u16 x, y, w, h, pixels}` go to VRAM |
| 1 | 64x128 16-bit VRAM pages, staged |
| 2 | effect data: ring buffer at 0x801F1700 (flag: the extra sectors hold common data at its start); `btlfx_getFileData(slot, idx)` reads its offset table |
| 3 | **code module**, see below; after the read the loader flushes the cache, then installs it |
| 4, 5, 6 | summon model header / model data / work data (`Hi_RegisterSummonModel`) |
| 7, 8 | sound data (battle sound buffer) |
| 9 | VRAM pages placed by the flag bitmask |
| 10 | effect data appended to the ring |

The group `kind` only matters for code: a module in an odd group of kind 1 loads at **0x801EC700**, every other
module at **0x801E7700**. Ordinary spells have one group with one 20-sector code chunk (the whole 0xA000 area
0x801E7700-0x801F1700); summon files have up to 9 groups with 10-sector chunks that alternate between the two
halves.

### Code module layout

A module is linked for its load address, with no relocations, and is laid out like an overlay:

| offset | contents |
|---|---|
| 0x0 | pointer to the module header (first `.rodata` word) |
| 0x4 | rest of `.rodata`: jump tables, the odd string (usually empty) |
| ... | `.text` |
| ... | `.data`, zero-initialised variables included; contains the header |
| end | 0xFF fill to the end of the chunk (not part of the module) |

Header: `{u32 0, init, op80, op81, ...}`. The loader sets the slot's op table (`D_80100C70[slot]`) to `header + 8`
and calls `init(effect data, g_btlfxEnv, g_btlfxEnv->unk10)` when it is non-NULL (it is NULL in every module on
the discs). Script opcodes 0x80 and up dispatch to `ops[op - 0x80]` (`btlfx_spawnTask`) with the usual effect
handler signature `(mode, work, env, entry, args)`; modules use up to 8 ops.

Modules never call the resident code directly (no `jal` leaves the module). They reach ovl_0b through
`env->unk10`, i.e. `g_btlfxModuleExports` (0x800F7510: 216 function pointers, followed by the `g_btlfxEnv`
block itself), loading a pointer at a fixed offset and calling it with `jalr`. Their own variables use absolute
`lui 0x801E/0x801F` addresses, so a module only works at the address it was linked for.

`tools/effect_modules.py` trims the 0xFF fill (to a 4-byte boundary), so a module whose last data word was
0xFFFFFFFF would lose it; the rebuilt binaries match the trimmed images.
