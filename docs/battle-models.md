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
  `{key, sector, sectors, split}` is FF9.IMG **dir index 7 (directory id 10), file id = model id**. This holds for
  every model entry of every encounter on all four discs (431 entries, 182 distinct models).
- All 182 enemy models use only **FT4 and FT3** polygons (no gouraud or untextured lists), none is double-sided,
  and the largest mesh group has 798 vertices (the limit is 801).
- **The indices a simplified file must keep valid**: bone numbers (count, order, parents), mesh-group numbers
  (count, order), texture indices, and, within each mesh group, the polygon → vertex / uv indices, which only the
  group itself uses. No base-game code and no effect module refers to an *enemy* model's vertices or polygons by
  number, with one exception: effect 0x17F (fx_17f) on the Zombie (model 14, see "Consumers"). The heavy enemies
  the port named are free to change inside their mesh groups.
- `python3 tools/battle_models.py` lists every model with its counts, heaviest first. Usage and sample output are at
  the end of this file.

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
- **Party / weapon models**: the same format, from their own files (`btl_loadPartyModelsStep`,
  `btl_loadWeaponModelsStep`, `btl_getWeaponModelHeader`).
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
| texture words (`func_800634A8` → `db_getModelTexPages`) | `{tpage, clut}` | `BtlUnit9C.texPages`; CLUT position for each TIM's upload; `btl_setStoneClut` / `btl_makeGreyCluts` (petrify) |
| uv offsets (`db_getModelUvOffsets`) | `{s16 du, dv}` | `btl_applyModelTextures`: added once to every uv entry that a polygon with that texture uses |
| texture positions (`db_getModelTexPos`) | 8 bytes, `{x, y, ...}` | VRAM position of TIM n's image; texture-animation offsets (`btl_relocTexAnims`) |

`count` (`func_80063830` → `db_getModelTexCount`) is also the number of TIMs in item 1 of the file. Keeping the
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
are indexed by bone number only, so they stay valid if the bones stay as they are.

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
