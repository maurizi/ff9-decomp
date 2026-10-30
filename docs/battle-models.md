# Battle model files

Answer to ff9-ds request R26. Everything here is read from the code named below (ovl_0b unless noted) and checked
against the four discs with `tools/battle_models.py`. The enemy → file mapping was also checked at run time, on the
Malboro of encounter 899 (`disc3_malboro_lv99_calm`): `BtlUnit[4].modelId` is 96, and the model header it points to
holds the same bytes as item 0x020060 of FF9.IMG dir index 7, file 96, with the offsets relocated.

## Summary for the port
- **Model of an enemy type**: the enemy type record (0x74 bytes, `Unk800C9D1C` = `Battle1334`) has the model id at
  +0x1E (`modelId`, read by `btl_getEnemyModelId`). The six u16 at +0x20 (`motionIds`, formerly "file ids") are
  its **motions** (keys 0x03xxxx, `btl_initEnemyType`), which live in the same model file.
- **Model id → FF9.IMG**: key `0x020000 | id` of the encounter bundle's dependency table. Its entry
  `{key, sector, sectors, split}` is FF9.IMG **dir index 7, file id = model id**. This holds for
  every model entry of every encounter on all four discs (431 (disc, model) pairs, 182 distinct models).
- All 182 enemy models use only **FT4 and FT3** polygons (no gouraud or untextured lists), none is double-sided,
  and the largest mesh group has 798 vertices (the limit is 801).
- **The indices a simplified file must keep valid**: bone numbers (count, order, parents), mesh-group numbers
  (count, order), texture indices, and, within each mesh group, the polygon → vertex / uv indices, which only the
  group itself uses. No base-game code and no effect module refers to an *enemy* model's vertices or polygons by
  number, with one exception: effect 0x17F (fx_17f) on the Zombie (model 14, see "Consumers"). The heavy enemies
  the port named are free to change inside their mesh groups.
- `python3 tools/battle_models.py` lists every model with its counts, heaviest first. Usage and sample output are at
  the end of this file.
- **Motions and posing** (R27): "Motions" below has the key format and the bone formulas; idle = motionIds[0].
  `tools/battle_pose.py` gives every vertex's model-space position for a motion frame, bit-exact with the PS1.

## Where the file comes from
1. `btl_loadEncounterFile` reads the encounter bundle (FF9.IMG dir index 5, file id = battle id; docs/encounters.md)
   and makes its dependency table (DB block 0x14) `GameData.depTable`.
2. `btl_loadEnemyModelsStep`, state 2, collects every **distinct key of kind 0x02** in that table. State 4 then picks
   the one with the lowest sector, `dep_find(depTable, 0x020000 | id)`, and reads it with `cd_readChunks` in two
   chunks:
   - chunk 0: `split` sectors (the entry's fourth word) to the battle work heap. This is the model data.
   - chunk 1: the remaining `sectors - split` sectors to the texture staging area (`Battle.unk153C[0]` and on).
3. State 6: the model data is a DB container. Its block 0x1B item 0 is a nested container that is registered with
   `db_register` (`BtlFiles.enemyModels[n]`), and the heap is moved to its end (`db_getSize`). Chunk 1 is block 0x1B
   item 1, a container of TIMs (keys 0x04xxxx, one per texture). They are uploaded with `file_setTimPos` /
   `file_loadTim` at the VRAM positions that the model info gives (below).

Model file (dir index 7) as stored:

| part | contents |
|---|---|
| DB container, 1 block (0x1B) with 2 items | |
| item 0, sector 0 .. `split` | nested container: `0x02xxxx` model header (one, id = file id), `0x03xxxx` motions |
| item 1, from sector `split` | nested container: `0x04xxxx` TIMs (texture images + CLUTs) |

So a file whose size changes also changes its dependency entry in every bundle that loads it: `sectors`, and
`split` when item 0 changes size.

## Who uses the header
- **Enemy units**: `btl_initEnemies` → `func_800BBF30` → main `evt_createModel` → `GameData.unk838` =
  `btl_registerModel(0x020000 | id, obj)`. That sets `BtlUnit9C.header = db_lookupRegistered(4, key)`, takes the
  texture tables from the model info (`btl_getModelTexInfo`), calls `btl_applyModelTextures`, and relocates the
  texture animations (key 0x19, `btl_relocTexAnims`).
- **Party / weapon models**: the same format, from their own files (`btl_loadPartyModelsStep`:
  FF9.IMG dir index 10, model id from `g_btlCharModelIds`; `btl_loadWeaponModelsStep`: dir index 8, model data in
  the first 0x1000 bytes; `btl_getWeaponModelHeader`). Backgrounds have a format of their own:
  docs/battle-backgrounds.md.
- **Effect and summon models**: also the same format, inside effect files (FF9.IMG dir 13), registered through
  `Hi_Register*`.

## Layout (as stored; offsets relative to the header unless noted)

```c
typedef struct BtlUnit9C8 {     /* model header, DB item 0x02xxxx (ovl_0a: WmModel) */
    u16 flags;                  /* +0  bit 0: relocated (set by btl_applyModelTextures) */
    u8 boneCount;               /* +2  bones; also the number of keys per motion frame */
    u8 meshCount;               /* +3  mesh groups (at most 16 in practice: hiddenMeshes masks are u16) */
    u16 packetSize;             /* +4  sum of the groups' packetSize */
    u8 unk6;                    /* +6  ovl_0a: look-at bone; not read by ovl_0b */
    u8 pad7;
    u16 height;                 /* +8  btl_getUnitHeight (party units / effect positions) */
    u16 radius;                 /* +A  btlfx_getScaledRadius / btlfx_getUnitRadius (party units) */
    s32 bones;                  /* +C  offset of BtlBoneDef[boneCount] (pointer after relocation) */
    BtlMesh *meshes;            /* +10 offset of BtlMesh[meshCount] (pointer after relocation) */
} BtlUnit9C8;                   /* 0x14 */

typedef struct {                /* bone (ovl_0a: WmBone) */
    s16 length;                 /* z offset from the parent */
    u8 unk2;
    u8 parent;                  /* parent bone; bone 0 is the root (its entry is not read) */
} BtlBoneDef;                   /* 4 */

typedef struct {                /* mesh group (ovl_0a: WmModelGroup) */
    u16 packetSize;             /* +0  GPU packet bytes if every polygon were drawn (sizes below) */
    u16 nFT4, nFT3, nGT4, nGT3, nG4, nG3, nF4, nF3;  /* +2 polygon counts, in list order */
    u8 pad12;
    s8 otBias;                  /* +13 OT entries the group is moved forward (btl_drawMesh: ot -= otBias) */
    u16 *vertexCounts;          /* +14 u16[boneCount]: vertices per bone */
    SVECTOR *vertices;          /* +18 sum(vertexCounts) SVECTORs, bone 0's first */
    u32 *prims;                 /* +1C the eight polygon lists back to back, in count order */
    u8 (*uvs)[2];               /* +20 u8 u, v */
    u8 (*colors)[4];            /* +24 u8 r, g, b, pad (gouraud lists only) */
} BtlMesh;                      /* 0x28 */
```

In the file, each group's five arrays follow each other in that order (vertexCounts, vertices, prims, uvs, colours),
group after group, with 4-byte alignment. Array lengths are not stored: vertices = sum of vertexCounts, uvs =
(colors - uvs) / 2, colours run to the next group's vertexCounts. The tool checks for every group that the
lists end exactly where the uvs start and that `packetSize` matches the counts.

### Polygon lists (`prims`)
Eight source record types (`WmSrc*`), in this order. `vtx` indexes the group's `vertices` (and the screen
arrays), `uv` the group's `uvs`, `col` the group's `colors`. `tex` selects a texture word (below), and `flags`
are: bit 0 semi-transparent (`flags & 0xE0` = blend mode, ORed into the tpage), bit 1 double-sided (no NCLIP
back-face test). Each record becomes one GPU packet of the size shown, *if* it faces the camera.

| list | record | size | fields (offset) | packet |
|---|---|---|---|---|
| FT4 | `WmSrcFT4` | 0x18 | vtx[4] 0, uv[4] 8, rgb[3] 0x10, tex 0x13, flags 0x15 | POLY_FT4 0x28 |
| FT3 | `WmSrcFT3` | 0x14 | vtx[3] 0, tex 6, rgb[3] 8, uv[3] 0xC, flags 0x12 | POLY_FT3 0x20 |
| GT4 | `WmSrcGT4` | 0x20 | vtx[4] 0, uv[4] 8, col[4] 0x10, flags 0x1C, tex 0x1D | POLY_GT4 0x34 |
| GT3 | `WmSrcGT3` | 0x18 | vtx[3] 0, uv[3] 6, flags 0xF, col[3] 0x10, tex 0x16 | POLY_GT3 0x28 |
| G4 | `WmSrcG4` | 0x18 | vtx[4] 0, col[4] 8, flags 0x14 | POLY_G4 0x24 |
| G3 | `WmSrcG3` | 0x14 | vtx[3] 0, col[3] 8, flags 0x13 | POLY_G3 0x1C |
| F4 | `WmSrcF4` | 0x10 | vtx[4] 0, rgb[3] 8, flags 0xD | POLY_F4 0x18 |
| F3 | `WmSrcF3` | 0xC | vtx[3] 0, rgb[3] 8, flags 0xB | POLY_F3 0x14 |

Quads are drawn by the GPU as triangles (0, 1, 2) and (1, 2, 3). NCLIP tests vertices 0, 1, 2 only. The OT depth is (sum of the 4 SZ) >> 6 for quads and AVSZ3 >> 2 for triangles. The flat lists copy
their rgb word (with the byte after it, which setcode then overwrites) straight into the packet.

On the discs, every enemy model has only FT4 and FT3 lists. Every textured corner has its own uv entry (uv count =
number of textured corners, rounded up to an even count; not in list order; no entry is shared by two textures), and no polygon is double-sided. Some polygons are semi-transparent (1,209 over
the 431 per-disc model entries).

### Textures
`tex` indexes `BtlUnit9C.texPages`, an array of `{s16 tpage, s16 clut}` (`BtlUnit9C18`). `btl_drawMesh` puts clut
in the high half of the first uv word and tpage in the second. The array, and the other two per-texture tables,
come from the **model info** entry for the model id: DB block 0x12, item 0 of the encounter bundle
(`db_findModelInfo`). Entries are 12 bytes `{u16 model id, u16 ?, u32 offset:24 | count:8, u32 ?}`, and at entry +
offset there are three tables of `count` entries:

| table | entry | used by |
|---|---|---|
| texture words (`db_getModelTexPages`, was func_800634A8) | `{tpage, clut}` | `BtlUnit9C.texPages`; CLUT position for each TIM's upload; `btl_setStoneClut` / `btl_makeGreyCluts` (petrify) |
| uv offsets (`db_getModelUvOffsets`) | `{s16 du, dv}` | `btl_applyModelTextures`: added once to every uv entry that a polygon with that texture uses |
| texture positions (`db_getModelTexPos`) | 8 bytes, `{x, y, ...}` | VRAM position of TIM n's image; texture-animation offsets (`btl_relocTexAnims`) |

`count` (`db_getModelTexCount`, was func_80063830) is also the number of TIMs in item 1 of the file. Keeping the
texture indices of the kept polygons is enough. The texture data doesn't change with the geometry.

### Motions (key 0x03xxxx, `BtlUnit9CC`)
```c
typedef struct BtlUnit9CC {
    u8 pad0[2];
    u16 frameCount;             /* +2 */
    u16 rootX, rootY, rootZ;    /* +4 constant (constMask bit 0/1/2) or offset of an s16[frameCount] table */
    u16 constMask;              /* +A */
    s32 rotKeys;                /* +C offset (relocated): boneCount keys {u16 x, u16 y, u16 z, u16 flags}, each a constant or the offset of an s8 per-frame table */
    s32 rotKeysLo;              /* +10 offset (relocated) or 0: the low nibbles, packed two frames per byte */
} BtlUnit9CC;
```
`btl_calcBoneMatrices` builds `boneCount` matrices from one key each, then composes bone i (i >= 1) with
`bones[i].parent` and translation (0, 0, length). The parent's matrix must already be built, so parent < i. Motions
are indexed by bone number only, so they stay valid if the bones stay as they are. The full decoding, with formulas,
is in "Motions" below.

### What is relocated at load
All of it is relocated in place, once per loaded file:
- `btl_applyModelTextures` (with `flags & 1` clear; sets it): `meshes`, `bones` and, per group, `vertexCounts`,
  `vertices`, `prims`, `uvs`, `colors` become `header + offset`. Then (when uv offsets are given, as they are
  for enemies) each group's uv entries get the `{du, dv}` of the texture of the first polygon that uses them. A
  2-byte mark per uv index in `g_vtxScreenZ` (0x1908 entries) makes sure each entry is shifted only once, so uv
  indices must stay below 0x1908 (6408) per group (largest on the discs: 4420). A uv entry shared by polygons with
  different textures would get the first one's offset only.
- Motions: `rotKeys` / `rotKeysLo` when still offsets (`IS_OFFSET`: >= 0), in `btl_initEnemyType`,
  `btl_registerModel`, `btl_updateUnitModel` and the sequence/effect motion setters.
- Texture animations (key 0x19xxxx): `btl_relocTexAnims`.

Nothing else in the file is a pointer. Vertex, uv and colour indices are 16-bit indices, not offsets.

### Per-group limits
- **Vertices per group ≤ 0x321 (801)**. `btl_projectMesh` writes screen xy / z of each group's vertices to
  `g_vtxScreenXY` / `g_vtxScreenZ` (main 0x800711A8 / 0x80070520; Z holds 802 entries before XY starts). It
  projects in triples, bone by bone, so a bone block whose count isn't a multiple of 3 writes up to two entries
  past its end. The next bone overwrites them, but the last bone's overrun lands past the total.
  `GeoSplitVertices` warns and clamps at 0x322. Largest enemy group on the discs: 798.
- Polygon vertex indices < the group's vertex count. Indices are per group and may cross bones, so a polygon can
  span two bones (that is how skinning works here: the vertex belongs to one bone, the polygon joins them).
- The vertices of bone b are the `vertexCounts[b]` entries after those of bones 0..b-1. Reordering vertices means
  rewriting the polygon indices, but no other data refers to vertex numbers (below).

## Indices that other data refers to
| index | refers to | used by (data) |
|---|---|---|
| bone number | `BtlBoneDef[]`, `BtlUnit9C.bones` matrices | motions (one key per bone, in order); enemy record `bones[3]` (+0x48: camera/effect anchors, `bones[2]` = head for `btl_getUnitBodyPos`), `baseBone` (+0x4B, unit position), `shadowBone` / `shadowDirBone` (+0x68 / +0x6E, `btl_calcShadowMatrix`); camera data; effect modules (`btlfx_getUnitBonePos`, `…BoneMatrix`, `…ArgBone*` with literal bone numbers or the target list's `unk25`); weapons (`Character.unk21` = parent bone) |
| mesh-group number | `meshes[]` | `BtlUnit9C.hiddenMeshes` bits: enemy record `meshMasks` (+0x2C: hidden \| vanish << 16), sequence ops 0x0D / 0x0F (`btlseq_opHideMeshes` / `btlseq_opShowMeshes`, u16 mask in the sequence data), script attribute `BTL_ATTR_HIDE/SHOW_MESHES`, Vanish (`vanishMeshes`), `btlfx_setUnitVisible` (0xFFFF); `btl_setMeshRgb` / `btl_addMeshRgb` per group |
| texture index | model info tables | polygon `tex` |
| vertex / uv / colour index | the group's arrays | that group's polygons only (for enemy models; effect models: `Hi_SplitMdlVertex` etc., below) |
| polygon number / list position | — | nothing for unit models. The order is only the order packets are built in; `btl_setModelRgb` and co. walk the *built* packets until each group's 0xFF terminator |

## Consumers: who reads a model's vertices, polygons and packets
This covers every ovl_0b function that takes a `BtlUnit9C` or `BtlUnit9C8`, the `src_equiv/ovl_0b` routines, and
all 380 effect modules. The modules were scanned through the `btlfx_mod.h` exports they call: no module reaches
ovl_0b any other way, and none reads a model header's fields directly. Main, ovl_65 and the other overlays never
touch a battle model's geometry.

### Base game (ovl_0b), unit models (party, enemies, weapons)
| function | reads | writes |
|---|---|---|
| `btl_updateUnitModel` (per unit per tick) | calls `btl_calcBoneMatrices`, then `btl_projectMesh` + `btl_drawMesh` for each group not in `hiddenMeshes`; the same for the weapon model | |
| `btl_calcBoneMatrices` (asm; `src_equiv/ovl_0b/bones.c`) | `boneCount`, `bones[].parent/length`, the motion's keys (one per bone), the parent model's bone (`parentBone`) | bone matrices (`BtlUnit9C.bones`) |
| `btl_projectMesh` | `vertexCounts[boneCount]`, `vertices`, bone matrices | `g_vtxScreenXY/Z[0 .. group vertices + 2]` |
| `btl_drawMesh` (asm; `src_equiv/ovl_0b/mesh_packets.c`; its C twin `btl_buildMeshPrims` has no callers) | all eight lists: `vtx` → screen arrays, `uv` → `uvs`, `col` → `colors`, `rgb`, `tex` → `texPages`, `flags`; `otBias` | GPU packets for the front-facing polygons, then a 0xFF terminator per group; the first packet is stored in `BtlUnit9C.prims` |
| `btl_applyModelTextures` (from `btl_registerModel`, `btl_initWeapon`, weapon/party reloads) | header, groups, the FT4/FT3/GT4/GT3 `uv` and `tex` | relocation (`flags` bit 0), `uvs` (+ per-texture offsets, once) |
| `btl_setMeshRgb` | the FT4 and FT3 lists of one group (strides 0x18 / 0x14; the other lists are not visited) | their `rgb` (the byte after it is kept). Callers: `btl_resetUnit` and `btl_initPartyUnit` (start black), `btl_setUnitBrightness` (intro/escape fades), `btl_applyBgLight` (background light on weapon models) |
| `btl_addMeshRgb` | the same lists | `rgb` + clamp; nothing calls it |
| `btl_setModelRgb`, `btl_fillModelRgb`, `btl_setModelAbr` | the **built packets** from `BtlUnit9C.prims`, group by group up to each 0xFF terminator, skipping hidden groups | packet colours / semi-transparency. Callers: `btl_updateUnitTint` (Zombie/Berserk/Heat/Freeze fills, Shell/Protect pulse, the **Trance** glow), `btl_setUnitBrightness`, `btl_cmdfx.c`, `btlfx_fadeUnit` / `btlfx_setUnitBrightness` / `btlfx_setUnitLight` |
| `btl_setStoneClut`, `btl_makeGreyCluts` (**petrify**) | `texPages[0 .. db_getModelTexCount)` | `texPages[].clut` (moved 6 or 12 rows), grey CLUT copies in VRAM; no geometry |
| `btl_hideMeshes`, `btl_showMeshes`, `btl_hideWeapon`, `btl_showWeapon` | `meshCount` | `hiddenMeshes` bits |
| `btl_drawShadow` / `btl_calcShadowMatrix` | bone matrices `shadowBone`, `shadowDirBone` | its own 4 FT4 quads; no model polygons |
| `btl_getBaseBonePos`, `btl_getUnitBodyPos`, `btl_getBonePos`, `btl_getBoneMatrix` | bone matrices (`baseBone`, enemy `bones[2]`, any bone number) | |
| `btl_getUnitHeight`, `btlfx_getScaledRadius`, `btl_getMeshCount` | `height`, `radius`, `meshCount` | |
| `btl_updateTexAnims` / `btl_relocTexAnims` | the texture-animation item 0x19xxxx, model info positions | VRAM rectangles only |
| `btl_forEachModelVertex` (export 0x06C) | every group's `vertexCounts` and `vertices` | calls `fn(&vertices[i], i)`, i = vertex number within the group |
| `btlfx_swapModelData` (export 0x2E0) | the whole item `0x020000 \| id` (`db_lookupRegistered` modes 4/5) | copies it to a buffer behind a 0x1234 marker, or back |

### Effect and summon models (`Hi_*`, models inside effect files)
These run the same drawing path on `g_btlfxEffModels` / `g_btlfxSummonModels` slots:
- `Hi_DrawEffModel`, `Hi_DrawEffModelByBone`, `Hi_DrawSummonModel`: `btl_projectMesh` + `btl_drawMesh` /
  `btl_drawMeshOffset`.
- `Hi_DrawSliceEffModel`: `btlfx_projectSliceMesh` + `btlfx_drawSliceMesh`.
- `Hi_DrawMorphEffModel` / `Hi_DrawMorphModelByBone`: `btlfx_projectMorphMesh` blends vertex i of each bone of the
  model with vertex i of the morph target, so both models need the same bone and vertex counts.
- `Hi_Modify*RGB/Abr`: built packets. `Hi_Show/HideSummonModelMesh`: group bits.

The vertex-number users all work on effect models:
- `Hi_SplitMdlVertex` → `GeoSplitVertices` → `btlfx_splitMeshVertices` gives every polygon corner its own copy of
  its vertex, in a buffer the module passes. It rewrites every `vtx` index to point there (in list order) and
  points `vertices` at the buffer. It stores the new count in `vertexCounts[group number]` (the group number used
  as a bone index), clamped to 0x321 with a warning.
- `Hi_GetSplitMdlVertex` → `btlfx_getSplitVertexSize`: 8 bytes per corner of all lists.
- `Hi_GetMdlVertexPtr` → `btlfx_getMeshPrims`: where list k's split vertices start in that buffer, and its polygon
  count (32 bytes per quad, 24 per triangle).

Every `Hi_Register*` / `btlfx_registerEffectModel` / `btlfx_registerSummonModel` call in the 380 modules passes a
header from the module's own effect file (`btlfx_getFileData`). No module draws, splits or recolours a unit's model
through these functions.

### Effect modules and unit models
These exports reach a unit's (party or enemy) model. The middle column is how many of the 380 modules call each one:

| export | modules | touches |
|---|---|---|
| `btlfx_getUnitBodyPos` | 219 | bones (`baseBone`, enemy `bones[2]`) |
| `btlfx_getUnitBonePos` / `…ArgBonePos` / `…BoneMatrix` / `…ArgBoneMatrix` | 41 / 44 / 31 / 1 | a bone number (a literal, or the target list's `unk25`) |
| `btlfx_getWeaponPos` | 13 | the weapon's parent bone |
| `btlfx_fadeUnitById` | 23 | built packets (`btlfx_fadeUnit` → `btl_setModelRgb` / `btl_setModelAbr`); -1 / -2: `hiddenMeshes` 0xFFFF on / off |
| `btlfx_setUnitLight` | 1 (fx_0fb_1) | built packets |
| `btlfx_setUnitVisible` | 2 | the unit's hidden flag (`BtlUnit.flags` bit 22), not the model |
| `btlfx_getCasterTexInfo` | 2 (fx_12d, fx_1b3) | model info texture tables of the caster's model id |
| `btlfx_swapModelData` + `btl_forEachModelVertex` | 1 (fx_17f) | **model 14 (the Zombie enemy), vertices by number**: below |

**fx_17f** (effect file 0x17F) is the only code that uses an enemy model's vertex numbers. When unit 0x10 is an
enemy, it saves item `0x02000E` (19,340 bytes on the discs) in its buffer `D_801E88E0` (26,620 bytes after the
marker). Every frame it moves each vertex of the model down by `D_801E7C60[i]`, a table of 700 random entries
indexed by the vertex's number within its group (the model has one group of 399 vertices). It also squeezes x/z
near the floor and fades unit 0x11. At the end it restores the saved copy. A simplified Zombie must keep at most
700 vertices per group and still fit the buffer. None of the port's heavy enemies is affected.

So for the enemy models the port wants to simplify, the vertex, uv and polygon numbering inside each group is used
only by `btl_projectMesh`, `btl_drawMesh`, `btl_applyModelTextures` and `btl_setMeshRgb`. These must stay:
- `boneCount`, and every bone's parent and length (motions, anchors, shadows);
- `meshCount` and the group order (hide/show masks);
- each group's `vertexCounts` layout by bone (each vertex keeps the bone it moves with);
- the texture indices, `otBias`, `height` and `radius`;
- the per-group limits above.

## Motions
Answer to ff9-ds request R27. Read from `btl_calcBoneMatrices` (ovl_0b 0x800C7DAC, hand-written asm; behaviourally
equivalent C in `src_equiv/ovl_0b/bones.c`) and main `math_buildRotMatrixZYX` (mode 1, `math_rotMatrixZYX_gte`).
`tools/battle_pose.py` implements it bit for bit (validation below).

### Summary
- A motion is a sequence of frames. Each frame gives every bone **one rotation** (three 12-bit angles, 4096 =
  360°), and gives bone 0 a **root translation**. There is no per-bone translation, no scale and no interpolation:
  the game shows frame `obj->frame` exactly and adds 1 per `btl_updateUnitModel` call.
- A bone's local matrix is **Rz · Ry · Rx** (column vectors: x is applied first, then y, then z).
- Bone i (i ≥ 1) sits at its parent's origin moved by `BtlBoneDef.length` **along the parent's z axis**, and then
  turns by its own rotation: `World_i = World_parent · Translate(0, 0, length_i) · Rot_i`.
- Bone 0: `World_0 = Base · Translate(root) · Rot_0`, where Base is the model matrix `BtlUnit9C.matrix` (the
  unit's rotation and position; identity = model space).
- A vertex of bone b (the `vertexCounts[b]` block of a mesh group, docs above) is in bone-local coordinates:
  `p = World_b · v`.
- PS1 axes: y points down (models stand at y ≤ 0, e.g. the Malboro's idle frame spans y -1024..28).

### Header (`BtlUnit9CC`)
| offset | field | meaning |
|---|---|---|
| +0 | u16 | 0 in every motion on the discs (2,732 in the 182 enemy model files); not read |
| +2 | `frameCount` | frames 0 .. frameCount-1 (1 to 170 on the discs) |
| +4, +6, +8 | `rootX`, `rootY`, `rootZ` | root translation: the value itself (s16) when `constMask` bit 0 / 1 / 2 is set, else the offset (from the motion's start) of an s16[frameCount] table |
| +A | `constMask` | bits 0-2: rootX/Y/Z constant; no other bit is set |
| +C | `rotKeys` | offset (relocated to a pointer at load) of `boneCount` keys of 8 bytes: high bits of the angles |
| +10 | `rotKeysLo` | offset (relocated) of `boneCount` keys of 8 bytes: low 4 bits of the angles; 0 = none (every motion on the discs has them) |

A key (both tables) is `{u16 x, u16 y, u16 z, u16 constMask}`: for axis j (bit j of constMask), the value is a
constant, else the offset (from the motion's start) of a per-frame table.

### Decoding frame f
For bone i, with `K = rotKeys[i]`, `L = rotKeysLo[i]` and `m` = the motion's bytes:

```
hi_j = K.v[j]                       if K.constMask bit j     (only its low 8 bits survive the mask below)
     = (s8) m[K.v[j] + f]           otherwise               (one byte per frame)
lo_j = L.v[j] & 0xF                 if L.constMask bit j
     = (m[L.v[j] + f/2] >> (4 * (f & 1))) & 0xF  otherwise  (two frames per byte: even f low nibble, odd f high)
lo_j = 0                            if rotKeysLo == 0
angle_j = ((hi_j << 4) | lo_j) & 0xFFF                       (j = x, y, z; 4096 = 360 degrees)

root_j = (s16) rootJ                if constMask bit j
       = ((s16 *)(m + rootJ))[f & 0xFFFF]  otherwise
```
So an angle is 12 bits: 8 per frame in `rotKeys`, refined by 4 in `rotKeysLo`. The SVECTOR handed to
`math_rotMatrixZYX_gte` is (angle_x, angle_y, angle_z) (the routine only looks at the low 12 bits).

### Local rotation (`math_rotMatrixZYX_gte`)
With `s_a = sin(angle_a)`, `c_a = cos(angle_a)` from `g_sinTable` (main 0x8006C1C4, 0x401 s16: exactly
`round(4096 · sin(i · π / 2048))`, so `sin(a)` = 4096 · sin(2π a / 4096) rounded, sign and quadrant by the angle's
bits 10 and 11), and every product shifted right by 12 (arithmetic, so floor):

```
Rot = Rz(z) · Ry(y) · Rx(x) =
  | cy·cz    sy·sx·cz − cx·sz    sy·cx·cz + sx·sz |
  | cy·sz    sy·sx·sz + cx·cz    sy·cx·sz − sx·cz |
  | −sy      cy·sx               cy·cx            |
```
The GTE version computes the products in this order (each `>> 12`, MAC values, stored as s16):
`a = cz·sx`, `b = sz·sx`, `c = cx·sz`, `t = cx·cz`; `m00 = cy·cz`, `m10 = cy·sz`, `m21 = cy·sx`, `m22 = cx·cy`,
`m20 = −sy`, `m01 = sy·a − c`, `m11 = sy·b + t`, `m02 = sy·t + b`, `m12 = sy·c − a`. (Mode 0, the CPU version,
rounds the products slightly differently; motions only use mode 1.)

### Composing the bones
All in 4096 = 1.0 fixed point, with the GTE's rounding: MVMVA with sf=1 shifts the full sum right by 12
(floor), rotation results come from IR1-3 and saturate to s16 (lm=0), translations come from MAC1-3 (32 bits).

```
Bone 0, no parent model:  R_0 = sat16((B.R · Rot_0) >> 12)          (column by column)
                          T_0 = B.T + ((B.R · (s16) root) >> 12)     (MAC: (B.T << 12 + B.R · root) >> 12)
Bone i >= 1, p = bones[i].parent:
                          R_i = sat16((R_p · Rot_i) >> 12)
                          T_i = T_p + ((R_p · (0, 0, length_i)) >> 12)   i.e. T_p + (R_p column 2 · length_i) >> 12
```
B is `BtlUnit9C.matrix`: `btl_updateUnitModel` builds it each frame from the unit's `rot[3]`
(`math_rotMatrixZYX`) and position (`pos >> 16`), and `btl_applyModelScale` scales it for scaled models.
Bones are built in order, so a parent must come before its children; that holds for every enemy model (the tool
checks). A bone's translation uses its parent's full rotation, but not its own: the bone's own rotation turns its
children and its vertices, not its position.

Other cases in the routine:
- **Parent model** (weapons: `BtlUnit9C.parent`): bone 0 is replaced by the parent's bone `parentBone`, moved by
  `sat16(R · (attachX, attachY, attachZ) >> 12)`; the decoded rotation and root translation of bone 0 are dropped.
  `btl_updateUnitModel` always calls the weapon with frame 0.
- **No motion** (`motion == NULL`): one matrix, the parent's bone moved as above, or else `BtlUnit9C.matrix` with
  its y and z columns negated.
- Motions are not checked against the model: a motion with fewer keys than the model has bones reads the bytes after
  its key tables for the extra bones. One case on the discs: motion 0x0326E5 has 25 keys and plays on model 671
  (28 bones); the tool reads the same bytes the PS1 does (the file is loaded as one block).

### Vertices
`btl_projectMesh` loads (camera · bone) into the GTE per bone and projects that bone's vertices with RTPT, so the
game never stores model-space positions. The model-space position used by the tool is what one MVMVA with the bone
matrix gives (`btlfx_projectSliceMesh` uses this one): `p = T_b + ((R_b · v) >> 12)`.

### Idle stance
- **Enemies**: `btl_initEnemyUnit` sets `model->motion = type->motions[0]`, i.e. **motionIds[0]** of the enemy
  type record (+0x20). `btl_initEnemyState` then starts each enemy on a **random frame** (`rand % frameCount`), except
  units with flags bit 23 ("model part"), whose bones are built at frame 0 to place their home position. When the frame reaches
  `frameCount`, `btl_updateUnitModel` calls `btl_setIdleMotion`, which picks `motions[(flags >> 18) & 1]` and frame 0:
  the idle loops through all its frames. Bit 18 (weak stance) is only set for an enemy by its AI script (attribute
  0x45, `btl_setUnitAttr`) or a sequence (`btlseq_opSetStance`), and then **motionIds[1]** is its idle. Of the other slots,
  `btl_setKoMotion` plays slot 4; attack sequences play motions from their own list (`Battle.seq.anims`).
  Any frame of motionIds[0] is a fair "as mostly seen" pose; frame 0 is the conventional choice. At run time, the
  Malboro in `disc3_malboro_lv99_calm` plays 0x030C2F = motionIds[0] (32 frames).
- **Party**: `btl_loadPartyMotions` fills 0x22 slots per member from `D_800F524C[kind][slot]` (ovl_0b; keys
  0x03xxxx in the member's model file, FF9.IMG dir index 10, file id & 0xFFFF = model id). Idle is slot 0, or slot 1
  with weak stance (bit 18 = Venom / Low HP / Poison / Sleep, `btl_initPartyUnit`, `btl_setIdleMotion`). Other slots
  `btl_setIdleMotion` uses: 5 (back from weak, from slots 1 and 6), 7 (weak, from slot 0), 9 (ready: the member whose
  command menu is up), 10/11 (stance + 10 with flag 19), 13 (Defend), 17 (escaping). At run time in
  `disc3_malboro_lv99_calm`, all four members played slot 0: Zidane (model 5414, kind 0) 0x03007D, Vivi (5415, kind 2)
  0x030BDC, Dagger (283, kind 4) 0x032CE2, Eiko (291, kind 11) 0x0337F4.

### Validation of tools/battle_pose.py
- Against `src_equiv/ovl_0b/bones.c` and main's `math_buildRotMatrixZYX` C, compiled natively with the ff9-ds port's
  software GTE (`port/psx/gte.c`): every frame of every motion in all 182 enemy model files (45,172 frames,
  2,162,261 bone matrices) is identical (max error 0).
- Against the PS1 (PCSX-Redux, `disc3_malboro_lv99_calm`): the bone matrices in RAM (`BtlUnit9C.bones`, read after
  the frame, `obj->frame - 1`) of the Malboro (63 bones, frames 3, 5 and 8 of 0x030C2F, the last with the unit's
  rotation poked to (0x50, 0x321, 0x10)) and of the four party members are identical to the tool's with
  `base = BtlUnit9C.matrix` (max error 0).

## tools/battle_models.py
```
python3 tools/battle_models.py                         # one row per enemy model, heaviest first, all discs
python3 tools/battle_models.py --name Hecteyes --name Ochu
python3 tools/battle_models.py --encounters --disc 3   # one row per (encounter, enemy type)
python3 tools/battle_models.py --model 166 --meshes    # per mesh group detail (bones, textures, flags)
python3 tools/battle_models.py --json build/battle_models.json   # everything, per disc
```
The tool reads the encounter bundles (enemy type records, names, dependency tables) and each model file's header.
On the way it checks that each entry is dir index 7 with file id = model id, the list sizes, `packetSize`, the
index ranges and the 801-vertex limit. Problems go to stderr, and there are none on the four discs. "max copies"
is the largest number of placements of the type in one enemy group that have their own model (placements with the
no-model bit are left out). Counts are for the whole model, front and back faces. Max copies of two types don't
add up to what is on screen when the types never share a group: `tools/battle_budget.py` (background + heaviest
enemy group per encounter, docs/battle-backgrounds.md) counts real groups.

The port's heavy enemies (`--name` for each):

```
model dir/file d:sector  sects bones mesh  verts polys   FT4  FT3  GT4  GT3   G4   G3   F4   F3  max copies  names / battles
  166 7/166   1:83926    93    56    2   1308  1889    43 1846    0    0    0    0    0    0  x1  Hecteyes / discs 1234 battles 15 293 333 420 569 570 756 757 +5
   95 7/95    2:111103   87    42    2   1183  1897    98 1799    0    0    0    0    0    0  x1  Ochu / discs 23 battles 110 111 113
  355 7/355   1:85602   115    56    2   1181  1816   184 1632    0    0    0    0    0    0  x1  Grand Dragon / discs 1234 battles 262 277 278 280 617 620 621 622 +3
  324 7/324   1:84453    89    40    2   1115  1788   192 1596    0    0    0    0    0    0  x1  Adamantoise / discs 1234 battles 68 69 291 292 777 778
  341 7/341   3:128511  119    55    3   1100  1782   218 1564    0    0    0    0    0    0  x1  Abadon / discs 3 battles 160 901 902 903
   78 7/78    1:81664   100    26    3    986  1632   272 1360    0    0    0    0    0    0  x1  Whale Zombie / discs 1234 battles 740 758 813 815 827 828
  163 7/163   1:83740    69    26    2    795  1384   114 1270    0    0    0    0    0    0  x1  Zaghnol / discs 1234 battles 14 81 674 677 678 679 680
  157 7/157   1:83444    58    27    1    514   850    94  756    0    0    0    0    0    0  x2  Wyerd / discs 1 battles 23 24
  145 7/145   1:83032    53    41    1    514   908    86  822    0    0    0    0    0    0  x2  Bandersnatch / discs 1234 battles 65 66 273 274 275 913 916
  150 7/150   1:83170    56    30    1    335   618    48  570    0    0    0    0    0    0  x2  Clipper / discs 1234 battles 185 186
```
To make the list complete, these models outweigh Hecteyes by vertex count (`--top 8`): Nova Dragon (model 545,
1408 vertices), Trance Kuja (566, 1371), Zuu (87, 1358) and Armodullahan (344, 1315). Ark (512, 1305), Garland
(446, 1292) and Iron Man (343, 1290) come right after Hecteyes.

## tools/battle_pose.py
```
python3 tools/battle_pose.py --model 96 --list                  # the file's motions; which are the enemy's motionIds
python3 tools/battle_pose.py --model 96                         # idle (motionIds[0]) frame 0: counts and bounds
python3 tools/battle_pose.py --model 96 --motion 4 --frame 10 --bones --vertices
python3 tools/battle_pose.py --model 5414 --key 0x03007d --obj build/zidane.obj   # party model (dir 10), any key
python3 tools/battle_pose.py --check-all --disc 3               # every enemy model's motions: bone order, key counts
```
As a module: `meshes, bones = battle_pose.pose(model_id, motion=0, frame=0, disc=None, img=None, key=None,
base=None)`; `meshes[g][v]` = (x, y, z) of vertex v of mesh group g (the group's own vertex numbering), `bones[b]` =
(R, T), R[row][col] in 4096 = 1.0. The OBJ export negates y (PS1 y points down) and writes one object per mesh
group, quads split as the GPU draws them.
