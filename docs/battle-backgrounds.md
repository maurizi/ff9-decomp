# Battle backgrounds (arenas)

Follow-up to ff9-ds request R26 (docs/battle-models.md covers the enemy models). Everything here is read from the
code named below (ovl_0b unless noted) and checked against the four discs with `tools/battle_budget.py
--backgrounds`, which parses every background file and reports any inconsistency (none on the discs). Nothing
was observed at run time.

## Summary for the port
- **File**: FF9.IMG **dir index 6, file id = background id**, the id of the encounter bundle's dependency key
  `0x0C0000 | id` (docs/encounters.md). 178 files (ids 0-178 with gaps; 151/157/157/163 on discs 1-4); a file
  present on several discs is byte-identical on all of them. Files 177 and 178 (scene id 177) are named by no
  encounter.
- **Geometry**: one or more *groups*, each a list of *records* (draw batches of at most 126 vertices). Every
  record has POLY_FT4 quads and POLY_FT3 triangles only (textured, flat colour), with per-record vertex indices,
  uvs and colour words. No other primitive kind exists in the format.
- Sizes on the discs: 320-898 polygons (quads + triangles), 432-1110 vertices, 1-4 groups, at most 10 records.
- Nothing in the file is a pointer: every offset is a u16 resolved at draw time, so a simplified file only has
  to keep its offsets and counts consistent (list below). The texture animations (item 0x19) are relocated once,
  like a model's.
- Moving parts: the sky layer turns (`skySpeed`, 80 files); groups after the first move by code for six scene ids
  (hard-coded in `btlbg_draw`); texture animations in 25 files; uv scrolls are supported but no file has one.
- `python3 tools/battle_budget.py` ranks the battles by background + enemy polygons; `--backgrounds` lists the
  backgrounds. Usage and output: "tools/battle_budget.py" below.

## Loading (`btl_load`)
1. The field / world map that starts the battle leaves the background's CD range in `GameData.sceneSector[1]` /
   `sceneSectors[1]` (docs/encounters.md).
2. `btl_loadBackgroundStep` reads the **whole file** to `Battle11C0.bgData` (`btl_allocBuffers`: work + 0x5800 +
   0x2800) and registers it with `db_register`.
3. `btl_setupBackground`: `btl_findBackgroundId` takes the last `0x0C` key of the bundle's dependency table, then
   - `bgData` = item `0x0C0000 | id` (the first group),
   - `bgHeader` = item 0 of block 0x1A (`db_lookupRegistered(9, id, 0x1A0000)`: the key of item 0 of block
     0x1A in the container holding `id`),
   - `bgTexAnims` = item 0 of block 0x19 if the file has that block, relocated by `btl_relocTexAnims`,
   - every item of block 0x04 is a TIM uploaded with `file_loadTim` at the position in its own header (two per
     file; file 177 has none),
   - `Battle.flags |= 1` turns on `btlbg_draw` in `btl_update`.
4. `btl_startBattle` → `btlbg_init(bgHeader, bgTexAnims)` copies the header into the `g_btlbg*` globals and
   starts the texture animations.

**Memory**: the work heap (`heapStart`, enemy and party models) starts 0x6000 bytes after `bgData`, so the
items before the first TIM (geometry, header, texture animations) must fit in **0x6000 bytes**; the TIMs behind
them are overwritten once uploaded. On the discs the largest is 0x52A8 (file 27). The whole file has to fit
the 0x3B800 bytes up to the end of the heap (largest file: 0x37800).

## File layout
A DB container (docs/encounters.md), items in this order:

| key | contents | read by |
|---|---|---|
| `0x0C0000 \| file id` | the groups, back to back | `btlbg_draw` (`Battle11C0.bgData`) |
| `0x1Axxxx` | `BtlBgHeader` (0x10 bytes) + `BtlBgScroll[scrollCount]` | `btlbg_init` and the users below |
| `0x19xxxx` (optional) | texture animations (`TexAnimSet`, as in models) | `btl_relocTexAnims`, `btl_updateTexAnims` |
| `0x04xxxx` | TIMs (texture pages and CLUTs) | `file_loadTim` |

### Header (`BtlBgHeader`, item 0x1A)
```c
typedef struct BtlBgHeader {
    s16 id;            /* +0  scene id (g_btlbgId); NOT the file id (file 0 has 65) */
    s16 texAnimCount;  /* +2  entries of the 0x19 item (g_btlbgTexAnimCount) */
    s16 skySpeed;      /* +4  g_btlbgSkyAngle += it every frame; layer 8 turns by angle >> 3 about y */
    s16 skyOwnProj;    /* +6  layer 8 drawn with screen distance 0x200 and its own translation */
    s16 lastGroup;     /* +8  groups - 1 */
    s16 scrollCount;   /* +A  BtlBgScroll entries after the header (0 in every file) */
    u8 lightR, lightG, lightB; /* +C colour the units take (btl_applyBgLight, btl_setUnitFade, ...) */
    u8 shadowShade;    /* +F  grey level of the unit shadows (btl_setUnitAttr, btl_setUnitBrightness) */
    BtlBgScroll scrolls[]; /* +10 */
} BtlBgHeader;

typedef struct {       /* uv scroll, 8 bytes; btlbg_draw, unless g_btlbgScrollPaused */
    s16 uvOfs;         /* the uv pair it moves, offset from the first group (bgData) */
    u8 mode;           /* at the end of a period: 0 wrap (subtract period x (du, dv)), 1 bounce (negate du, dv) */
    u8 du, dv;         /* added to the u and v bytes every frame */
    u8 step;           /* frame counter (written at run time) */
    u8 period;
    u8 pad7;
} BtlBgScroll;
```

### Group (`BtlBgGroup`, 0x18 bytes)
Offsets are from the group; the next group starts `size` bytes later. The code reads `size`, `recCount`,
`recOfs`, `texOfs` and `vertexOfs`; the other members are consistent on every file but not read.

```c
typedef struct {
    u16 size;          /* +0  to the next group (word aligned) */
    u16 cornerCount;   /* +2  4 x quads + 3 x triangles of all records */
    u16 recCount;      /* +4 */
    u16 recOfs;        /* +6  always 0x18: BtlBgRec[recCount] follow the header */
    u16 texCount;      /* +8  texture words (at most 16 on the discs; a colour word can address 32) */
    u16 texOfs;        /* +A  u32 texture words: clut << 16 | tpage */
    u16 vertexCount;   /* +C  sum of the records' vertexCount */
    u16 vertexOfs;     /* +E  vertices, 6 bytes each: s16 x, y, z (word aligned) */
    u16 polyCount;     /* +10 quads + triangles = colour words */
    u16 colorOfs;      /* +12 first record's colours */
    u16 indexOfs;      /* +14 first record's indices */
    u16 uvOfs;         /* +16 first record's uvs */
} BtlBgGroup;
```

In the file a group is: header, records, texture words, vertices, then (word aligned) the colours, the vertex
indices and the uvs of all its records, each region in record order. A record's index and uv arrays start on a
word (they are read as words).

### Record (`BtlBgRec`, 0x10 bytes)
Offsets are from the record itself.

```c
typedef struct {
    u16 layer;         /* +0  8 sky, 4, 2, 0 scene (below) */
    u16 vertexCount;   /* +2  vertices projected: non-zero multiple of 6, at most 126 */
    u16 firstVertex;   /* +4  into the group's vertex array (records take consecutive ranges) */
    u16 colorOfs;      /* +6  u32 per polygon, quads then triangles */
    u16 indexOfs;      /* +8  u16 per corner, 4 per quad then 3 per triangle: 4 x the vertex number in the record */
    u16 uvOfs;         /* +A  u8 u, v per corner, same order */
    u16 quadCount;     /* +C */
    u16 triCount;      /* +E */
} BtlBgRec;
```

Colour word: bits 0-23 the flat colour (r, g, b), bits 24-28 the texture word index, bit 29 semi-transparent
(code 0x2E / 0x26 instead of 0x2C / 0x24), bits 30-31 the blend mode (ORed into the tpage bits 5-6). 454
polygons are semi-transparent on the discs.

## Drawing (`btlbg_draw`, every frame)
1. Nothing is drawn while the low byte of `g_btlbgBrightness` is 0 (`btlbg_setBrightness`; `btlfx_opFadeBackground`
   fades it).
2. uv scrolls (none on the discs) and texture animations (`btl_updateTexAnims`); rain streaks
   (`btlbg_drawRain`, `GameData.rainCount`, not background data).
3. For each group, for each record, by `layer` (`g_btlbgLayerMask` bit: 8 → 8, 4 → 4, 2 → 2, 1 → 0):
   - **8 (sky)**: rotated about y by `g_btlbgSkyAngle >> 3` (the angle grows by `skySpeed` per frame); with
     `skyOwnProj` drawn with `SetGeomScreen(0x200)` and a translation of its own (the camera's y, a z between
     the camera's and 12500 depending on its pitch), then the battle projection is restored. 60 files set it,
     80 have a non-zero `skySpeed`. Goes to the background OT (`Battle11C0.ot`, 0x100 entries) at index 0x3FC,
     its farthest entry.
   - **4**: background OT, index (sum of the quad's four SZ >> 4) >> 6 or (triangle's average) >> 4, times 4;
     clamped to 0x3FC; skipped at 0.
   - **2**: background OT index 0, its nearest entry. `btlfx_drawBgOffscreen` draws only
     this layer (mask 2) into an off-screen buffer for effects.
   - **0 (scene)**: the scene OT shared with the units, index (depth sum << `g_btlbgOtShift`), clamped to
     0x3FFC; skipped at 0. So this layer is depth-sorted against the units.
4. After group 0 the next group gets its own matrix: for scene ids 7 (random y spin, `g_btlbgJitterBase1/2`),
   0x44, 0x6E, 0x70, 0xA8 and 0xAB there is a hard-coded motion per group (rotation and offset driven by
   `VSync(-1)`); any other scene id with several groups gets offset (0, -10, 0) and a y rotation of 3. Files with
   several groups: 32 (scene 0x44), 156 (0x70, 4 groups), 157 (0x6E), 160 (7), 162 (0xAB), 172 (0xA8), 177 and
   178 (scene 177, unused).

`btlbg_drawRecord` (hand-written asm; C twin in `src_equiv/ovl_0b/background.c`):
- `btlbg_projectVertices` runs RTPT over the record's `vertexCount` vertices six at a time and leaves SXY at
  scratchpad + 4i and SZ >> 4 at + 0x200 + 4i. Hence at most 128 vertices (126 used on the discs), a count that
  is a non-zero multiple of 6 (with 0 the loop never ends), and indices stored as byte offsets 4i. It reads one
  word past the record's last vertex.
- Per quad then per triangle: NCLIP on corners 0, 1, 2, back faces (< 0) culled; OT index as above; a POLY_FT4
  (0x28 bytes) or POLY_FT3 (0x20) with the colour scaled by `g_btlbgBrightness` (GPF, unless 0x80), the texture
  word, and the uvs. Quads are drawn as GPU quads (triangles 0-1-2 and 1-2-3).

## Who else reads the background
| function | reads |
|---|---|
| `btl_applyBgLight`, `btl_setUnitFade`, `btl_updateUnitTint`, `btl_updateStatusColor`, `btl_setUnitBrightness` | `lightR/G/B` |
| `btl_setUnitAttr`, `btl_setUnitBrightness` | `shadowShade` |
| `btlfx_drawBgOffscreen` | layer 2 records (`btlbg_draw` with mask 2) |
| `btlfx_opFadeBackground` | `g_btlbgBrightness` |

Nothing else reads the geometry. The module exports have no `btlbg_*` function, but effect modules reach the
background through two exports: 13 of them call `btlfx_drawBgOffscreen` (a top-down snapshot of the ground into
VRAM), and fx_0ba / fx_19f call `btlfx_startFadeBackground` (docs/r29/battle-arena-memory.md). `btlbg_getId`, `btlbg_addSkySpeed`, `btlbg_getSkySpeed`, `btlbg_pauseScroll`,
`btlbg_startRain` and `btlbg_stopRain` have no callers.

## What a simplified background must keep
- The DB container with the 0x0C (key = file id), 0x1A, optional 0x19 and 0x04 items; the bundle's dependency
  entry (`sectors`) follows the file's size.
- Everything before the first TIM within 0x6000 bytes.
- Per group: `size`, `recCount`, `recOfs`, `texOfs`, `vertexOfs` correct; `lastGroup` = groups - 1. A
  multi-group file with a moving scene id must keep its group order.
- Per record: `vertexCount` a non-zero multiple of 6, at most 126 (pad with unused vertices); `firstVertex`
  inside the group's vertex array; indices = 4 x (vertex number within the record) < 4 x `vertexCount`; colour
  texture indices < the group's texture words; index and uv arrays word aligned.
- Texture words and TIMs can stay as they are. The layers can be merged or dropped freely: no other data refers
  to a record, polygon or vertex number (the only offsets into the geometry from outside are the uv scrolls,
  and no file has one).

## tools/battle_budget.py
```
python3 tools/battle_budget.py                          # one row per encounter, heaviest (background + enemies) first
python3 tools/battle_budget.py --battle 678 --battle 679
python3 tools/battle_budget.py --name Clipper --sort bg   # --sort total|bg|enemies|battle, --verts, --top N
python3 tools/battle_budget.py --backgrounds [--bg 94 --records]   # per background (and per record)
python3 tools/battle_budget.py --party                    # what the party column assumes
python3 tools/battle_budget.py --motion                   # static / sky-only / animated per background (R29)
python3 tools/battle_budget.py --memory                   # battle buffer use per encounter (R29)
python3 tools/battle_budget.py --json build/battle_budget.json
```
Disc images: `--img DIR` (or an FF9.IMG with one `--disc`), else `$FF9_DISKS`, else `disks/` in this checkout or
in any directory above it, or an `ff9-decomp/disks` next to one (`encounter_catalog.find_image`; the same applies
to `encounter_catalog.py` and `battle_models.py`). So from the port's `decomp/` submodule the superproject's or a
sibling checkout's `disks/` is found without arguments. A directory may hold `discN/FF9.IMG`, `us/discN/FF9.IMG`
or `disks/us/discN/FF9.IMG`.

Columns:
- **bg**: every record of the arena: quads + triangles, projected vertices (padding included; `--backgrounds`
  also shows the `used` ones). Front and back faces: the game culls back faces per frame.
- **enemy**: the heaviest enemy group `btl_pickEnemyGroup` can pick. Group weights are percentages taken in
  order, so a group with no share left is only started when event op 0x8C forces a group; the row names such a
  group when it is heavier (`--any-group` counts it). Each placement with its own model adds its type's model
  (battle_models.py's polygon counts).
- **total** = bg + enemy, the ranking key. **+party** adds a constant upper bound: the four heaviest of the 19
  party body models in `g_btlCharModelIds` (models 444, 5414, 281, 283: 421 + 412 + 411 + 411 polygons) plus four
  times the heaviest weapon model (model 43, 90 polygons): 2015 polygons, 1810 vertices. Party models are FF9.IMG
  dir index 10 (not 7), weapons dir index 8; the party bodies weigh 340-482 polygons each (`--party`).
- An encounter with identical data on several discs is one row (`--per-disc` splits them); only discs where its
  background file is present are counted (`--all` adds the others).

### The port's examples
```
discs battle   bg  bg polys/verts  enemy polys/verts  total polys/verts  +party   enemies (heaviest group, its chance)
 1234    185   94    590/864       1236/670         1826/1534       3841   Clipper 618x2 (60%)
  124    221   90    764/840       1809/1116        2573/1956       4588   Ladybug 603x3 (50%)
  124    223   79    745/672       1809/1116        2554/1788       4569   Ladybug 603x3 (50%)
 1234    678   94    590/864       1384/795         1974/1659       3989   Zaghnol 1384x1 (100%)
 1234    679  103    784/744       1384/795         2168/1539       4183   Zaghnol 1384x1 (100%)
  124    840   80    718/648       1572/936         2290/1584       4305   Ghost 524x3 (50%); forced group 4: Ladybug 603x3 = 1809
```
- 678 and 679 load the same Zaghnol, with identical data on every disc; only the arena differs: 678 uses
  background 94, 679 background 103.
- "Ghost x3 + Ladybug x3" never meet: in 221 and 223 the groups the game can pick are Ladybug x2 / x3 (the Ghost
  groups have weight 0: op 0x8C only), in 840 Ghost x2 / x3 (Ladybug x3 only when forced). The heaviest real
  group is Ladybug x3, 1809 polygons.
- The common factor of the two battles that don't fit (678, 185) is **background 94** (also used by 230, 238,
  288 and 292; its near twin 92 by 186, 220, 226, 283, 291 and 673). But by count it is a *light* arena: 590
  polygons (rank 131 of 178), against 784 for 103 and 718-764 for the Ghost/Ladybug arenas, and the totals of
  678 and 185 are below those of 679, 221, 223 and 840. Splitting quads into triangles doesn't change that (94:
  1000 triangles, 103: 998). What sets 94 apart is its make-up: many quads (410, rank 28; 103 has 214), 244
  layer-2 polygons (rank 9; 103 has 120) and 864 vertices (rank 35). A rough front-facing estimate (cameras on a
  ring around the origin, not the game's camera data) also leaves 94 with fewer visible polygons than 103. So
  if the port's budget is a per-frame polygon or vertex count, these arena counts don't explain 678 / 185;
  what else differs between the fixtures (party size and members, camera, how quads are handled, screen
  coverage) is the next thing to measure.

### Top 15 by total
```
discs battle   bg  bg polys/verts  enemy polys/verts  total polys/verts  +party   enemies (heaviest group, its chance)
 1234     51   15    614/804       3519/2282        4133/3086       6148   Mimic 951x1, Magic Vice 856x3 (100%)
 1234     45   12    566/792       3519/2282        4085/3074       6100   Mimic 951x1, Magic Vice 856x3 (100%)
    3    467  124    828/696       2619/1736        3447/2432       5462   Epitaph 1194x1, <F9>E 475x3 (100%)
    3    465  124    828/696       2613/1922        3441/2618       5456   Epitaph 1194x1, <F9>G 473x3 (100%)
    3    464  124    828/696       2592/2333        3420/3029       5435   Epitaph 1194x1, <F9>D 466x3 (100%)
    3    519  123    600/690       2676/1949        3276/2639       5291   Epitaph 1194x1, <F9>F 494x3 (100%)
    3    499  125    552/672       2709/1967        3261/2639       5276   Epitaph 1194x1, <F9>H 505x3 (100%)
    3    502  123    600/690       2574/2180        3174/2870       5189   Epitaph 1194x1, <F9>A 460x3 (100%)
  124    727   79    745/672       2416/1596        3161/2268       5176   Zombie 604x1, Zombie 604x3 (50%)
    2    129   57    738/900       2416/1596        3154/2496       5169   Zombie 604x1, Zombie 604x3 (70%)
    3    468  125    552/672       2544/1892        3096/2564       5111   Epitaph 1194x1, <F9>C 450x3 (100%)
    3    475  125    552/672       2544/1778        3096/2450       5111   Epitaph 1194x1, <F9>B 450x3 (100%)
   23    130   61    650/798       2416/1596        3066/2394       5081   Zombie 604x1, Zombie 604x3 (70%)
   23    128   60    627/756       2416/1596        3043/2352       5058   Zombie 604x1, Zombie 604x3 (70%)
    2    127   58    556/654       2416/1596        2972/2250       4987   Zombie 604x1, Zombie 604x3 (70%)
```
The heaviest arenas (`--backgrounds --top 5`): 37 (898 polygons; battles 75 and 76), 25 (897; 300), 50 (894; 21
and 29), 8 (888; 294), 137 (885; 849, 854, 859).
