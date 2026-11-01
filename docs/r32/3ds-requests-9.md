# R39: field walkmeshes and exit regions, read offline (for a build-time map baker)

Answers from the matched C (paths relative to the repo, lines at commit 180a1c91), the field files on all four discs
(`FF9.IMG` dir index 4) and the reference reader **`tools/field_maps.py`** (committed with this note). No emulator.
Script layout and instruction encoding: `docs/r32/3ds-requests.md` section 2; call ops and the static recipe:
`docs/r32/3ds-requests-3.md`.

## Short answers

1. **Where.** A field is one file of `FF9.IMG` dir index 4 (`DIR_FIELDS`), file id `area << 16 | field`. The file is a
   DB container. Its item `0x1B0001` (`DB_PACK | 1`) is the nested container that `fld_loadFile` keeps in RAM and
   registers (`src/ovl_0a/fld_main.c:813`, `db_register` at :824). In that container:
   - the **walkmesh** is the first item of kind `0x0B` (`DB_WALKMESH`): `fld_setupBackground`
     (`fld_main.c:689-704`) takes the background key (first item of kind `0x0A`, `fld_getBgKey` :849) and asks for
     `DB_GET_KEY_AT_IN(bg key, DB_WALKMESH)`, i.e. item number 0 of kind 0x0B in the container holding the
     background. The walkmesh id is not the field id (field 50: background `0xA001F`, walkmesh `0xB0004`).
   - the **event script** is item `0x50000 | field` (`fld_findEventScript`, :668).
2. **Walkmesh = "BGI".** Item = `u32 0xACDCDEAD` magic, then the `BgiHeader` block (`include/ovl_0a.h:595-625`).
   `bgi_init((u8 *)walkmesh + 4)` (`src/ovl_0a/bgi_move.c:618`) uses the block **in place**: every table is a `u16`
   offset from the header, there are no pointers and no relocation. What changes in RAM: triangle flag bit 0
   (`bgi_initAnim` at load, script ops later), triangle bit 7 (search scratch), floor `curPos` (floor moves),
   `BgiAnim` state, `BgiKeyframe.value`, and `neighbors` / edge `clone` of animated triangles
   (`bgi_linkFrameNeighbors`). Header `curPos` is never written.
3. **Coordinates.** A vertex in field space is `header.curPos + floors[tri.floor].curPos + vertex` (every user does
   this, e.g. `bgi_pointInTriangle`, `bgi.c:37-43`). The floor plane is x/z, the same space as actor positions and
   region polygons: 1430 of the 1441 exit polygons overlap the walkmesh in x/z with that formula (the other 11 sit
   just past the mesh edge). On disc, `curPos == orgPos` for the header and every floor.
4. **Walkable = triangle `flags & 1`** (`BGI_FLAG_ACTIVE`), **and** the mask byte `flags >> 8` ANDed with the
   mask filter has neither `0x80` (`BGI_MASK_NO_PLAYER`) for the player nor `0x40` (`BGI_MASK_NO_OTHERS`) for
   anyone else (`bgi_getPassableNeighbor`, `bgi_move.c:28-64`). Stored low bytes: `0x01` (154213 triangles over all
   copies), `0x02` (102) and `0x03` (14); bit 1 marks exactly the triangles of walkmesh animations and has no reader.
   The filter is 0xFF after `bgi_init`; every exit function runs `27 00 7F` (BG_MASK_FILTER 0x7F) before it walks the
   player out, which turns the 0x80 triangles walkable. So: mask 0x80 = "player only during scripted moves";
   draw it as not walkable. Mask bit 0x10 picks the second footstep sound (`fld_playAnimSfx`,
   `src/ovl_0a/tu_800B0124.c:664-669`); 0x20 and 0x01 have no reader.
5. **Exits.** A region object (`08 entry uid`, kind 3) gets its convex polygon (3-8 points) from op **0x29**
   `29 flags n {s16 x, s16 z}[n]` in one of its own functions (always tag 0, always literal on the discs). Its tag 2
   ("range", runs while the player stands inside with control enabled) or tag 8/3 ("action", Square/Cross inside)
   holds **`2B flags u16 field`** (GOTO_FIELD) or **`B6 flags u16 world`** (GOTO_WORLD, 9000-9012), usually after
   `05 d8 02 7d lo hi 2c 7f` (`Gs16[2] = entry point`). Over the 818 distinct field scripts: **1441 exits**
   (1370 range, 71 action), **all with literal polygons and literal destinations**; 1036 are simple (one
   destination, no guard beyond "control enabled"); **604 depend on story state** in some way (198 have several
   destinations or a global-variable branch deciding the GOTO, 482 have their `NEW_REGION`/`SET_REGION` behind a
   global-variable branch). Details and the definitions below.
6. **Walkable area at run time.** Ops **0x9A** `BGI_TRI_ACTIVE (u16 tri, u8 on)`, **0xCB** `BGI_FLOOR_ACTIVE
   (u8 floor, u8 on)` and the walkmesh animation ops **FF 09..FF 11** switch triangles. 179 fields use them, 89 of
   those behind a story branch (lists below). Floor moves (FF 00..FF 08, 6 fields) only move floors along y. 0x27
   (mask filter) and 0xA8 (detach one actor) do not change the map.
7. **Shops / save Moogles**: `npcs` in the reader's JSON: actors whose talk function reaches MENU, with the literal
   position ops of their tag 0 (`1D SET_POS_XZ x z`, `A1 SET_POS x y z`). 97 such actors in the 818 scripts: 57 save
   Moogles (MENU 4; the same 57 fields as R32c) and 39 shopkeepers (MENU 2), all with an init position op.
8. **Field ids**: dir index 4's table; per disc **222 / 322 / 372 / 215** fields (1131 files), **818 distinct ids**
   (50-3100). A field that is on several discs has identical walkmesh header, regions and walkmesh ops on each.

## 1. The field file

### FF9.IMG to the field
- Header sector 0: `dirs[i] = {u32 type, u32 count, u32 tableSector, u32 firstSector}` at `0x10 + 16 i`
  (`docs/ff9img.md`). Dir index 4 is type 2: `count` entries `{u32 id, u32 sector}` at `tableSector * 2048`,
  then a sentinel `{0xFFFFFFFF, endSector}`. `id = area << 16 | field` (`FieldDirEnt`, `include/ovl_0a.h:1159-1163`;
  `fld_refindSectors`, `fld_main.c:604`). Size in sectors = next entry's sector - this one.
- Area bundles are dir index 2 (`id = area`); they hold no walkmesh or script data needed here.
- `tools/encounter_catalog.py` `Img.files(4)` / `find_image(disc)` read this; `field_maps.py list` prints it.

### DB container (main `db_lookup`, `src/main/db.c:61`; parser `encounter_catalog.db_items`)

| offset | size | meaning |
|---|---|---|
| 0 | 1 | `0xDB` |
| 1 | 1 | number of kind blocks `k` |
| 4 + 4i | 4 | `kind << 24 \| rel` (kind 7 bits); block at (this word's address + `rel & 0xFFFFFF`) |
| block + 1 | 1 | item count `c` |
| block + 4 | 2c | `u16 id[c]` |
| aligned 4 | 4c | `u32 ofs[c]`: item j at (address of `ofs[j]`) + `(ofs[j] & 0xFFFFFF)` |

Key = `kind << 16 | id`. Field file top level: `0x140000` dependency table, `0x1B0000` (TIMs / image packs, uploaded
to VRAM by `fld_loadFile` and dropped), `0x1B0001` (sector-aligned; models 0x02, motions 0x03, script 0x05,
sounds 0x08, background 0x0A, walkmesh 0x0B, sprites 0x16, 0x18, tex anims 0x19, model info 0x12, VRAM cursors
0x1C, vibration 0x1E, lights 0x1F). `DB_GET_KEY_AT` returns the key of item number n of a kind in container order,
so "first item of kind 0x0B" = `field_maps.first_of_kind`.

### Walkmesh item layout as stored (all little-endian; offsets from the header = item + 4)

Every field file has exactly one kind-0x0B item. Item: `+0 u32 0xACDCDEAD`, `+4 BgiHeader`. `dataSize` counts the block without the magic (`bgi_dumpText`,
`bgi.c:308`, "+4 for file magic").

`BgiHeader` (0x3C bytes, `include/ovl_0a.h:595-625`):

| off | type | field | note |
|---|---|---|---|
| 0x00 | u16 | dataSize | |
| 0x02 | s16[3] | orgPos | |
| 0x08 | s16[3] | curPos | origin of every vertex; = orgPos on disc, never written |
| 0x0E | s16[3] | min | = orgPos on every disc file |
| 0x14 | s16[3] | max | |
| 0x1A | s16[3] | charPos | default character position (unused by bgi code) |
| 0x20 | s16 | activeFloor | 0 on disc |
| 0x22 | s16 | activeTriangle | 0 on disc |
| 0x24 | u16, u16 | triangleCount, triangleOffset | `BgiTriangle[]` (always at 0x3C) |
| 0x28 | u16, u16 | edgeCount, edgeOffset | `BgiEdge[]`, 3 per triangle |
| 0x2C | u16, u16 | animCount, animOffset | `BgiAnim[]` |
| 0x30 | u16, u16 | floorCount, floorOffset | `BgiFloor[]` |
| 0x34 | u16, u16 | normalCount, normalOffset | `BgiNormal[]` |
| 0x38 | u16, u16 | vertexCount, vertexOffset | `BgiVertex[]` |

Then (in this order on disc): triangles, edges, anims (+ keyframes), floors, normals, vertices, the `s32` triangle
index lists of the floors and keyframes.

`BgiTriangle` (0x28, `ovl_0a.h:667-681`):

| off | type | field | note |
|---|---|---|---|
| 0x00 | u16 | flags | bit 0 walkable; bit 1 animated (no reader); bit 7 search scratch (RAM); high byte = mask |
| 0x02 | u16 | data | 0 everywhere |
| 0x04 | s16 | floor | `BgiFloor` index |
| 0x06 | s16 | normal | `BgiNormal` index, -1 = flat (height = vertex 0's y) |
| 0x08 | u16 | thetaX | tilt for actors standing on it (`bgi_getActorTilt`) |
| 0x0A | s16 | thetaZ | |
| 0x0C | s16[3] | vertices | `BgiVertex` indexes |
| 0x12 | s16[3] | edges | `BgiEdge` indexes, side i = vertex i to vertex i+1 |
| 0x18 | s16[3] | neighbors | triangle across side i, -1 = wall |
| 0x1E | s16[3] | center | relative like the vertices (`bgi_setActorTriangle`) |
| 0x24 | s32 | d | plane distance 16.16 (height: `(d - nx x - nz z) * invNy`) |

`BgiEdge` (4, `ovl_0a.h:684-687`): `u16 flags` (0 everywhere), `s16 clone` = the side index of this edge in the neighbour (-1 none).
`BgiFloor` (0x20, `ovl_0a.h:632-641`): `u16 flags` (0 on disc; bit 0 set/cleared by BGI_FLOOR_ACTIVE, never
tested), `u16 floorIndex`, `s16 orgPos[3]`, `u16 curPos[3]` (= orgPos on disc), `s16 min[3]`, `s16 max[3]`,
`u16 triangleCount`, `u16 triangleOffset` (`s32` triangle ids).
`BgiNormal` (0x10, `ovl_0a.h:643-648`): `s32 nx, ny, nz, invNy` (16.16). `BgiVertex` (6): `s16 x, y, z`.
`BgiAnim` (0x10, `ovl_0a.h:650-657`): `u16 flags`, `u16 frameCount`, `s16 frameRate` (8.8), `s16 counter`,
`s32 curFrame` (8.8), `s32 frameOffset` → `BgiKeyframe[frameCount]` (8, `ovl_0a.h:660-665`): `u16 flags`, `s16 value` (frames to hold),
`u16 triangleCount`, `u16 triangleOffset` (`s32` ids). A keyframe is a set of triangles that replace the previous
frame's set (`bgi_updateAnims`, `bgi.c:411`).

### What the load does (offline equivalent)
- `bgi_init` (`bgi_move.c:618-651`): pointers = header + offsets, mask filter 0xFF, 9 actor slots, then
  `bgi_initAnim(i)` for every anim (`bgi.c:998`): **frame 0's triangles get bit 0 set, every other frame's
  triangles get it cleared**, and `bgi_resetFloorMoves`. The reader applies the same rule to produce
  `triangles[i].active`; the stored bit differs from it on 4 triangles of field 404 and 24 of field 2930.
- Only 2 walkmeshes have anims: 404 (Dali, 16 frames of 2 triangles, stepped by `FF 0A` from entry 2 tags 11/12)
  and 2930 (Memoria, 84 triangles). All others have `animCount = 0`.

## 2. Event script: regions and exits

- Script item: `"EV"`, `+3 u8 entry count`, `+4` debug name (0x28 bytes Shift-JIS, not always NUL-terminated),
  `+0x80` entry table; per entry `u16 (nfunc << 8)` then `{u16 tag, u16 offset}`. Parsed by
  `encounter_catalog.script_functions`; instructions by `tools/evtscript/evtdis.py` (`edis.py` prints
  expressions infix). Both reused by the reader.
- Objects: `07/08/09 entry uid` (code / region / actor, no flags byte; uid 0 means uid = entry). Entry 0 is the
  field's main code object and the last nine entries are the party (both started by `evt_init`).
- **Polygon**: `29 flags n {x, z}[n]` (`event.c:1290`): `g_evtActor` = the object running the code, so a region
  sets its own polygon. Flags bit 0 = n is an expression, then one bit per coordinate in the order x0, z0, x1, ...
  (bits run out after x3; later coordinates are always literal). `76 flags x z` (2 points, both (x,z)), `77 flags x z`
  (overwrite the last point), `78` (duplicate the last point, max 8) edit it (`event.c:1340-1364`). On the discs every
  one of the 2053 regions in the 818 scripts sets its polygon with a literal 0x29 in tag 0 (one region has none; 6
  have two 0x29, 5 of them behind a story branch); 0x76-0x78 never occur in region code. Polygon sizes: range exits
  3 points 3, 4 points 1227, 5: 123, 6: 13, 7: 4; action exits 3: 2, 4: 42, 5: 13, 6: 9, 7: 2, 8: 3. Test: `evt_isInRegion` (all edge crosses >= 0 or all <= 0,
  edges included; convex, either winding).
- **Triggers** (`evt_checkPlayerAction`, `event.c:2768`): while control is enabled, the first region in the object
  list that contains the player and has tag 2 gets `tag 2` every field frame; Square / Cross with no actor in talk
  range calls the first containing region's tag 8 / tag 3.
- **Exit** = a GOTO reachable from a region's tag 2 (range) or tags 8/3 (action), directly or through
  `10/12/14 flags level uid tag` (CALL / CALL_WAIT_FREE / CALL_SYNC; uid 0xFF = self, 0xFA-0xFE = player, else the
  object with that uid) to depth 2. `2B flags u16` / `B6 flags u16` (`event.c:1365-1370`); flags bit 0 = expression.
  `0x3E80` is the special "ending" field (one GOTO on the discs, not from a region).
- **Arrival point**: global `s16` at byte 2 of the global variables, set by `05 d8 02 7d lo hi 2c 7f` right before
  the GOTO; the destination's init reads it. The reader reports it when it is in the same straight-line code
  (1419 of 1441 exits).
- **Conditionality** (reader's definition): the reader builds each function's control-flow graph (jump/switch
  targets exactly as `evt_runScripts` computes them, `src_equiv/main/script_text.c:1156-1201`), folds branches on a
  constant EXPR and drops unreachable code. A *guard* of a GOTO is a live branch with one successor that reaches the
  GOTO and one that does not; its class comes from what the EXPR before the branch reads: `story` (a global
  variable: story counter `Gu16[0]`, global flags), `field` (a field variable; e.g. `Fs16[6]` facing tests
  `< 48 || > 208`), `local`, `object` (another object's property, e.g. player position), `system` (only sysvars:
  `sys.controlEnabled`, present in nearly every exit), `constant`, `unknown`.
  - `conditional` = some guard other than system/constant.
  - `story_dependent` = several destinations, an expression destination, or a `story` guard.
  - `created_story_guarded` = the NEW_REGION op (or the SET_REGION) itself is behind a `story` guard: the exit only
    exists in some story states (e.g. field 300 region 1).

### Exit statistics (818 distinct field scripts = every field id once; copies on other discs are identical)

| | count |
|---|---|
| regions (NEW_REGION entries) | 2053 (2052 with literal polygons, 1 without) |
| exits | 1441: 1370 range (tag 2), 71 action (tag 8/3) |
| literal polygon / literal destination | 1441 / 1441 (no expression operands at all) |
| polygon overlaps the walkmesh in x/z | 1430 |
| reached through a CALL | 66 |
| GOTO_WORLD among the destinations | 66 exits |
| simple (one destination, only system guards) | 1036 |
| conditional (field/story/local/object guard) | 405 |
| several destinations | 139 |
| story_dependent | 198 |
| created_story_guarded | 482 |
| story_dependent or created_story_guarded | 604 |
| arrival value found | 1419 |
| GOTOs outside region triggers (`other_gotos`) | 3329: main object 1002, actors 870 (talk/event code), code objects 737, party 535, never-created entries 180, region non-trigger tags 5 |

Example (field 101, disc 1): range exits 15 → 100 (arrival 201), 16 → 102, 17 → 112, each with a 4-point literal
polygon and only the control-enabled guard (17 also has a facing test, so it is `conditional`). Field 300 region 2:
13 world-map destinations chosen by the story counter.

## 3. Ops that change the walkmesh

All are in `evt_execInstr` (`src/main/event.c:1795-1812`, FF page `:2244-2325`); argument widths from the C,
`flags` bit i = argument i is an expression. Names: `include/ff9.h:4035, 4124-4148, 4249-4266`.

| op | args | C | effect | uses / fields (distinct scripts) |
|---|---|---|---|---|
| 27 BG_MASK_FILTER | u8 filter | `bgi_setMaskFilter` | mask byte AND filter (0x7F = player ignores mask 0x80) | 19594 / 816 |
| 9A BGI_TRI_ACTIVE | s16 tri, u8 on | `bgi_setTriangleActive` (`bgi.c:937`) | triangle bit 0 | 802 / 91 |
| CB BGI_FLOOR_ACTIVE | u8 floor, u8 on | `bgi_setFloorActive` (`bgi_move.c:896`) | bit 0 of the floor and all its triangles | 481 / 109 |
| A8 BGI_ACTOR_ACTIVE | u8 on | `bgi_setActorActive` | the running actor stops/starts using the walkmesh | 6901 / 517 |
| FF 00..08 | u8 move, then u8 / u16 (FF05 speed u16, FF07 u16 min, u16 max) | `bgi_*FloorMove*` | floor `curPos[axis]` oscillation; all uses on disc set axis 1 (y) | 10-24 / 6 |
| FF 09 BGI_ANIM_FRAME | u8 anim, u8 frame | `bgi_setAnimFrame` | | 0 |
| FF 0A BGI_ANIM_SHOW_FRAME | u8 anim, u8 frame | `bgi_showAnimFrame` (`bgi.c:501`) | switch to that keyframe's triangle set | 46 / 2 |
| FF 0B..FF 11 | u8 anim, u8 (FF0D u16 rate; FF0F/FF10 3 x u8) | playing, mode, rate, frame values, range, reset | | 0 |

The floor-move ops occur in 1609, 1610, 1652, 1658, 1759, 2952 (bobbing floors / lifts, y only): the x/z map does
not change. Ops that change which triangles are walkable (9A, CB, FF09-FF11) occur in **179 fields**: CB only 86,
9A only 68, both 23, FF0A 2. In **89** of them at least one such op sits behind a story branch:

51, 64, 105, 113, 150, 156, 300, 302, 303, 304, 306, 307, 357, 403, 455, 568, 609, 612, 613, 618, 651, 652, 653, 654,
655, 663, 702, 703, 704, 753, 755, 759, 806, 807, 809, 952, 1002, 1007, 1151, 1206, 1212, 1217, 1301, 1364, 1421,
1422, 1423, 1458, 1460, 1503, 1505, 1602, 1603, 1606, 1607, 1608, 1663, 1759, 1809, 1856, 2100, 2160, 2162, 2169,
2201, 2211, 2214, 2215, 2217, 2220, 2221, 2251, 2253, 2258, 2259, 2361, 2500, 2502, 2504, 2505, 2508, 2710, 2753,
2853, 2919, 2951, 2952, 3057, 3100.

The other 90 (unguarded, i.e. in event code or the same in every state): 50, 57, 100, 103, 107, 108, 116, 158, 162,
301, 310, 312, 404, 405, 407, 408, 504, 551, 600, 604, 751, 752, 756, 757, 801, 802, 803, 805, 808, 812, 813, 815,
1006, 1017, 1018, 1059, 1201, 1202, 1203, 1209, 1210, 1223, 1225, 1227, 1350, 1356, 1368, 1418, 1420, 1652, 1656,
1707, 1751, 1752, 1753, 1755, 1801, 1814, 1820, 1824, 1857, 2008, 2101, 2112, 2150, 2164, 2207, 2212, 2216, 2254,
2255, 2351, 2364, 2456, 2506, 2602, 2603, 2605, 2606, 2702, 2703, 2901, 2908, 2909, 2918, 2930, 2950, 2954, 2955,
3059.

For a baker: the stored state plus `bgi_initAnim` is the walkable area at load. The reader lists every walkmesh
op with its location, literal arguments and `story_guarded`; ops in tag 0 of entry 0 that are not story-guarded
always run at field entry. Scripts can also read the actor's current triangle / floor in expressions (`0x70`
`EVT_EXPR_ACTOR_TRIANGLE`, `0x71` `EVT_EXPR_ACTOR_FLOOR`, `src/main/evt_object.c:570-585`), so a few triggers are triangle ids rather than regions.

## 4. Shopkeepers and save Moogles

`npcs` = actors (09) whose tag 3 or 8 reaches MENU (`75 flags type arg`) directly or through a CALL (depth 2), with
the position ops of their own tag 0: `1D flags x z` (SET_POS_XZ: y from the walkmesh) or `A1 flags x y z`
(SET_POS). 97 actors: 57 Moogles (MENU 4 save, plus MENU 2 with the expression shop id `Ls8[1]`), 39 shopkeepers
(MENU 2 literal). Region hand-offs (a counter region calling the shopkeeper, R32c) are not listed here; the region's
polygon is in `regions`. Positions behind story branches (shopkeepers that move) appear as several entries.

## 5. Field ids per disc

`field_maps.py list --disc N` (dir index 4 in table order):

| disc | files | field ids | range | areas |
|---|---|---|---|---|
| 1 | 222 | 222 | 50-2953 | 19 |
| 2 | 322 | 322 | 70-2953 | 33 |
| 3 | 372 | 372 | 300-3100 | 40 |
| 4 | 215 | 215 | 350-3100 | 20 |
| all | 1131 | 818 distinct | | |

## Reference reader: tools/field_maps.py

```
python3 tools/field_maps.py list [--disc N]             # [{disc, field, area, file_id, sector, sectors, name}]
python3 tools/field_maps.py dump FIELD [--disc N] [--no-mesh] [--indent 1]
python3 tools/field_maps.py check [--disc N] [--json F]  # every field of every disc; exit 1 on a parse problem
```

It imports `encounter_catalog` (Img, find_image, db_items, script_functions) and `evtscript/evtdis.py` /
`edis.py`; the JSON keys are documented in its docstring. Validation (`check`, all four discs, 87 s): 1131 field
files, 1131 walkmeshes (154329 triangles, at most 463 triangles / 410 vertices / 23 floors per field), 818
distinct scripts, **0 problems** (every function decodes to its end; every index in range; every table inside
`dataSize`).

Not covered: the world map (dir index 3 files have no BGI walkmesh; ovl_0c has its own collision), and positions
that come from expressions (reported as `null`).
