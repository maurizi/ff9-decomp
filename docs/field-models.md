# Field model files

Answer to ff9-ds request R28. Read from the ovl_0a code named below (main for the event script) and checked
against the four discs with `tools/field_models.py --check` (0 problems). The model-id → file mapping, the layout
as loaded and the motion arithmetic were also checked at run time in PCSX-Redux (Redux states `field_dali_inn`,
disc 1, and `disc3_pandemonium_lv99_field`, disc 3; "Runtime check" below).

## Summary for the port
- **Where a field model comes from**: the field file's dependency table (FF9.IMG dir index 4, DB block 0x14,
  `GameData.depTable`) has one entry `{0x020000 | model id, sector, sectors, link}` per model the field loads.
  `link` is the **event-script entry of the actor** that shows it. `sector` 0 means the model is **inside the field
  file** (item `0x02xxxx` of its nested container `0x1B0001`); otherwise it is **FF9.IMG dir index 10, file id &
  0xFFFF = model id** (only the party's models, which are cached across fields).
- **Layout**: the model header and its groups are exactly the battle model format (`WmModel` = `BtlUnit9C8`,
  `WmModelGroup` = `BtlMesh`, the eight `WmSrc*` lists; docs/battle-models.md "Layout"). Every field model on the
  discs uses only FT4 and FT3. What differs is how the field builds and keeps packets (once per actor, double
  buffered, uv offsets applied to the packets), below.
- **Motions are the battle format and composition, bit for bit** (`fldmdl_calcBoneMatrices` decodes the same keys
  with the same `gte_ldrotkey` macros and composes bones with the same GTE sequence). `tools/battle_pose.py` poses
  field models unchanged (`battle_pose.Model(data, id, pack=, motion_sources=)`), and `field_models.py --pose`
  wraps it. Checked at run time against the PS1's bone matrices: max error 0.
- **Indices that must survive a simplification**: bone numbers (count, order, parents, the header's look-at bone,
  bone 0 and bone 1), group numbers (count ≤ 32, order), texture indices, the 1:1 order between each list's source
  records and its packets, and per group the ≤ 801-vertex limit. **Nothing in the base game indexes a field
  model's vertices or polygons by number** outside the mesh routines themselves (consumers below).
- **The party**: script entries `scriptCount-9 .. scriptCount-1` are party characters 0..8. Their models are
  98 Zidane, 8 Vivi, 185/76 Garnet (+202, 205, 381), 5489 Steiner (+658), 192 Freya, 273 Quina, 443 Eiko,
  509 Amarant, 204 Beatrix, and in the same slots early on 107 Cinna, 109 Marcus, 5467 Blank (+190), 20 (slot 8).
  Plus local copies of party models under other ids (668/669/670 = 98, 662 = 8, 660 = 109, 659 = 5467, 118 = 20).
  `tools/field_models.py --party` lists them all.
- `python3 tools/field_models.py` lists every field model with its counts and the fields that load it; usage and
  sample output at the end.

## 1. Where a field model comes from

### The dependency table and the actor's script entry
1. `fld_load` → `fld_loadFile` (0x800A9BDC) reads the field file (FF9.IMG dir index 4, file id `area << 16 |
   field`) and sets `GameData.depTable` to its block 0x14. It loads the TIMs of the file's container `0x1B0000`
   into VRAM (at the TIMs' own positions) and registers the second container `0x1B0001` (`FieldScene.fieldFile`,
   `db_register`). That container holds the field's own models (`0x02xxxx`), all motions the field's actors use
   (`0x03xxxx`, party motions included), the model info item `0x120000` for the local models, and texture
   animations `0x19xxxx`.
2. Every actor is created by script opcode `EVT_OP_NEW_ACTOR` (0x09: `entry, uid`). In field mode
   `evt_execInstr_newactor` calls `fld_queueModelBundle(entry)` (also called for every kind-4 object when a saved
   field state is restored, `evt_init`). It looks up `fld_depFindByLink(entry)`: the dependency entry whose
   **fourth word equals the actor's script entry index** (scanned from the end, so the last one wins; no other
   entry kind has such small link values), takes its key, applies `fld_remapModelId`, and queues the key in
   `g_fieldModelQueue` if no registered container has it yet (`db_lookupRegistered(4, key)`). Field-local models
   are already registered, so only dir-10 models are queued.
3. After `evt_init`, `fld_loadModelBundlesInCdOrder` loads the queued ids in ascending sector order
   (`file_find(10, id)->sector`) with `fld_loadModelBundle(id)`: `file_find(10, id)` (dir index 10, matched on the
   low 16 bits of the file id), whole file. If `g_fieldModelCache` (10 slots at the top of RAM below 0x801EDF00,
   fixed ids 98, 8, 5489, 185, 76, 192, 273, 509, 443, 204) already holds it, the cached copy is copied through
   VRAM instead of read from CD. `fld_compactModelCache` drops cached ids the new field doesn't queue.
4. The model itself is attached by `EVT_OP_SET_MODEL` (0x2F: `u16 model, u8 height`): main `evt_createModel` →
   `GameData.unk838` = `fld_setupActorModel(0x020000 | model, actor)`, which finds the header with
   `db_lookupRegistered(4, key)` in whatever container holds it (field file or dir-10 bundle).

`fld_remapModelId`: model 185 is replaced by 76 (both in dir 10) once `g_evtGlobalVars.h[0]` (story progress) is
≥ 0x283C. Both the queue and `fld_setupActorModel` apply it, so 76 is loaded wherever a field lists 185 late in
the story; the dependency tables list 76 itself only in field 2171 (discs 3, 4).

On all four discs (`--check`): every `0x02` entry with sector 0 has its header in the field file and no other copy;
every other `0x02` entry is the dir-10 file with that id (same sector count); no field model is both; every model
in a field container has a dependency entry; every link is an entry of the field's script; the same id has the same
data in every field and on every disc (6,023 entries, 345 (model, source) pairs, 335 local ids and the 10 dir-10
ids). The world-map files (dir index 3, `--worlds`) use the same scheme with only local models.

### Dir index 10 files (party models)
The directory holds both the field party models (ids 8 … 5489) and the battle party models (ids 281-291, 580-608,
5414, 5415, …; `btl_loadPartyModelsStep`, docs/battle-models.md). A field one is:

| part | contents |
|---|---|
| file id | `split << 16 \| model id`: `split` = sector of item 1 (checked on every file) |
| DB container, 1 block (0x1B) with 2 items | |
| item 0, from +0x40 | nested container: `0x02xxxx` header (id = model id), five `0x03xxxx` motions (the default one among them), `0x190xxx` texture animations, `0x120000` model info (one entry) |
| item 1, from sector `split` | `0x04xxxx` TIMs, one per texture (two for every party model) |

`fld_loadModelBundle` uploads the TIMs to VRAM slots it allocates from the field's VRAM layout item `0x1C0000`
(`FieldVramPos`: an 8-bit page column and a 4-bit page column, CLUT rows below), writes the resulting
`{tpage, clut}` and `{du, dv}` into the model info tables, registers item 0 (`FieldScene.modelBundles`), and moves
the heap to its end (the TIM part is not kept). The motions an actor plays are mostly in the **field** file
(item 0x1B0001), since `fld_drawModels` looks each frame's motion up in every registered container.

### How the actor names it
| what | where |
|---|---|
| model of an actor | the dependency entry whose link = the actor's script entry (`EvtObject.scriptIdx`); at run time `EvtObject.modelId` (op 0x2F) |
| model slot | `EvtObject.modelSlot`: 0-8 = party character (`evt_getObjectChrIndex` = `scriptIdx + 9 - scriptCount`), 9 + n for NPCs (`GameData.npcModelSlots`) |
| per-slot record | `game_getModel(slot)`: `Unk8E0`/`FieldEvtCtx` `{flags, modelId, ModelObj *model, FieldEvtObj *obj}` in `GameData.unk8D8[9]` / `unk8E0[npcCount]` |
| the model object | `ModelObj` (0x98-byte records `GameData.unk8D4` / `unk8DC`; main sees it as `Unk8004AFF0`) |

## 2. The layout as loaded

### Model header, groups, polygon lists
Identical to battle models; docs/battle-models.md "Layout" has the structs, the eight `WmSrc*` record layouts and
sizes (FT4 0x18, FT3 0x14, GT4 0x20, GT3 0x18, G4 0x18, G3 0x14, F4 0x10, F3 0xC), the array order in the file and
the per-group limits. Field names in ovl_0a.h: `WmModel` {`flags`, `boneCount`, `groupCount`, `packetSize`,
`lookAtBone` (+6), `bones`, `groups`}, `WmModelGroup` {`packetSize`, eight counts, `otBias` (+0x13),
`boneVertexCounts`, `vertices`, `prims`, `uvs`, `colors`}, `WmBone` {`length`, `unk2`, `parent`}. On the discs:

- every field model uses only the FT4 and FT3 lists (345 models, plus 17 more in world files);
- 65 double-sided polygons in 11 models (188 in 14 with the world files), 427 semi-transparent ones;
- up to 84 bones (46 models have ≥ 31), up to 13 groups, at most 799 vertices in a group, uv indices up to 4355;
- `otBias` from -7 to 7;
- one model (200) has more than 0xFFFF packet bytes: the header's u16 total wraps; the field code never reads it
  (`fld_setupActorModel` sums the groups' own sizes).

`packetSize` per group is still required: the field uses it as the offset of the second packet buffer.

### Model info (item 0x120000 of the container that holds the model)
`fld_getModelAnimId`, `fld_getModelTables`, `fld_setupActorModel` (`WmAnimTable`): `{u8 ?, u8 count, u16 pad}`,
then 12-byte entries `WmAnimTableEntry` `{u16 modelId, u16 animId, u32 tableOffset:24 | texCount:8, u16
texAnimId, u16 pad}`. `animId` is the motion set at setup (`0x03xxxx`, 0xFFFF in 88 models: no motion until the
script sets one; fld_drawModels replaces it every frame by the actor's `anim`); `texAnimId` the `0x19xxxx` item
(0xFFFF none). **Only entry 0 is ever matched** for these two: the search loops of `fld_getModelAnimId` and
`fld_setupActorModel` never advance their entry pointer (`fld_getModelTables` does), so a model gets its default
motion and its texture animations only when it is the first entry of its container's table: always for a dir-10
model (one entry), for one local model per field file (978 of the 3,601 field-file entries on the four discs are first; 1,277
later ones name a texture animation that is never attached). A mirror copy (`fld_modelCommand` 8-11) instead
takes the container's first `0x19` item whatever its model (`db_lookupRegistered(9, ...)`). At entry +
`tableOffset` there are three tables of `texCount` entries:

| table | entry | used by |
|---|---|---|
| texture words | `{u16 tpage, u16 clut}` (`WmTexInfo`) | `ModelObj.texInfo`; packets get `clut` and `tpage & ~0x20` |
| uv offsets | `{s16 du, s16 dv}` | added to the four/three uv bytes of every **packet** with that texture |
| texture positions | `{srcX, srcY, destX, destY}` (`ModelTexOfs`; for dir-10 models `FieldTexEnt` before the upload) | `fldmdl_initTexAnims` adds them to the texture animations' source / destination rectangles |

### What `fld_setupActorModel` does with it (per actor)
1. `ModelObj.model` = the header; `ModelObj.anim` = the default motion, relocated if still an offset (below).
2. Work memory at `FieldScene.heapTop`: `2 × Σ group.packetSize` (packets for both draw buffers) + 32 per bone
   (bone matrices, `MATRIX`) + 12 per group (`WmModelOut` `{s16 unk0 = 0, u8 *prims, s32 unk8 = 0}`).
   `tools/field_models.py` prints this as "ram".
3. `fldmdl_initModel` relocates the header once (below) and lays out `packets`, `boneMatrices`, `groupOut`;
   scale 0x1000, no parent, no hidden groups.
4. `fldmdl_buildPackets(model, texWords, uvOffsets)`: for each group, **twice** (buffer 0, then buffer 1 at
   `prims + packetSize`), one packet per source record in list order: `setPolyFT4` etc., colour = the record's
   rgb (gouraud lists: `colors[col[i]]` per corner), uv = `uvs[uv[i]] + uvOffsets[tex]` (u8 arithmetic),
   `clut = texWords[tex].clut`, `tpage = texWords[tex].tpage & ~0x20`, and when `flags & 1` (semi-transparent)
   `tpage |= flags & 0xE0` plus the semi-transparency code bit. The flat lists take the record's rgb only.
5. The texture animation item (`texAnimId`, first info entry only) is relocated with the model's texture positions
   (`fldmdl_initTexAnims`) and becomes `ModelObj.texAnims`.

| list | source | packet |
|---|---|---|
| FT4 / FT3 | `WmSrcFT4` 0x18 / `WmSrcFT3` 0x14 | `POLY_FT4` 0x28 / `POLY_FT3` 0x20 |
| GT4 / GT3 | `WmSrcGT4` 0x20 / `WmSrcGT3` 0x18 | `POLY_GT4` 0x34 / `POLY_GT3` 0x28 |
| G4 / G3 | `WmSrcG4` 0x18 / `WmSrcG3` 0x14 | `POLY_G4` 0x24 / `POLY_G3` 0x1C |
| F4 / F3 | `WmSrcF4` 0x10 / `WmSrcF3` 0xC | `POLY_F4` 0x18 / `POLY_F3` 0x14 |

### Per frame (`fld_drawModels`, 0x800AB3CC)
`fldmdl_calcBoneMatrices` (or the look-at variant), then for every group whose bit in `ModelObj.hiddenGroups` is
clear: `fldmdl_projectMesh` + `fldmdl_drawMesh` (asm; `src_equiv/ovl_0a/model_link.c`), or the `ClipY` pair when
`FLD_OBJ_CLIP_Y` is set. `fldmdl_projectMesh` is battle's `btl_projectMesh` (per bone, RTPT in triples into
`g_vtxScreenXY/Z`, with the same 2-entry overrun per bone block). `fldmdl_drawMesh` only writes the screen xy into
the prebuilt packets of the current buffer (`prims + (GameData.unk8 ? packetSize : 0)`), skips back faces with
NCLIP unless the record's `flags & 2`, and links the packet at `ot - otBias`: quads at `(z0+z1+z2+z3) >> 4`,
triangles at the GTE's AVSZ3 (battle uses `>> 6` / `AVSZ3 >> 2`). There is no terminator: packet k of a list is
always source record k.

Differences from battle in short: packets are built once and kept (two copies), uv offsets go into the packets and
leave the source `uvs` untouched (so a uv entry may be shared by polygons of different textures), and there is no
per-frame packet rebuild, so every colour or blend change is a write into the kept packets (consumers below).

### What is relocated at load, and by which function
| data | function | test |
|---|---|---|
| header `bones`, `groups`; per group `boneVertexCounts`, `vertices`, `prims`, `uvs`, `colors` | `fldmdl_initModel` (from `fld_setupActorModel`, `fld_modelCommand` 8-11) | `WmModel.flags` bit 0 (set by it) |
| motion `rotKeys` (+0xC), `rotKeysLo` (+0x10, if non-zero) | `fld_setupActorModel` (default motion), `fld_drawModels` (each frame's motion) | `IS_OFFSET(+0xC)` |
| texture animation `rects` / `frames` offsets, plus the model's texture positions | `fldmdl_initTexAnims` | offset ≤ 0x80000000 |
| model info texture tables of a dir-10 model | `fld_loadModelBundle` (VRAM placement) | — |

Nothing else is a pointer; vertex, uv and colour references are 16-bit indices. The world map's copy (ovl_0c
`wm_initModel`, `wm_buildPackets`, `wm_loadActorModel`, `wm_drawActorModel`) does the same with
`db_getModelTexPages` / `db_getModelUvOffsets`.

### Motions: same as battle
`fldmdl_calcBoneMatrices(model, frame)` (0x800B7F14) is the C form of the same algorithm as `btl_calcBoneMatrices`
(docs/battle-models.md "Motions"): the header `WmAnim` = `BtlUnit9CC` (`frameCount` +2, `rootPos[3]` +4,
`constMask` +0xA, `rotKeys` +0xC, `rotKeysLo` +0x10), keys `{u16 x, y, z, flags}` decoded by `gte_ldrotkey` (high
8 bits per frame, s8) and `gte_ldrotkey_lo` (low nibble, two frames per byte), 12-bit angles, `math_rotMatrixZYX_gte`
(Rz·Ry·Rx), and

```
R_0 = sat16(W.R · Rot_0 >> 12)        T_0 = W.T + (W.R · root) >> 12      W = ModelObj.worldMatrix
R_i = sat16(R_p · Rot_i >> 12)        T_i = T_p + (R_p · (0, 0, length_i)) >> 12     p = bones[i].parent
```

with the same GTE rounding. `W` is built by `fld_drawModels` each frame: `math_rotMatrixZYX(rotX, rotY, rotZ)`
(or `math_rotMatrixZXY` with the floor tilt when `FLD_OBJ_TILT`), translation = actor position, then
`fldmdl_applyScale` multiplies its columns by `scaleX/Y/Z` (`ModelObj.flags` bit 1, set by op 0x9F with
`scale = (s8)arg << 6`). The frame is `FieldEvtObj.animFrame` (u8; the matrices in RAM belong to the frame value
shown, not frame - 1 as in battle). Only these differ from battle:

- **Look-at** (`ModelObj.flags` bit 0, set by main `evt_relaxLookAt` / `evt_updateLookAt*` with
  `ModelObj.lookAtTarget`): `fldmdl_calcBoneMatricesLookAt` replaces the rotation of bone `WmModel.lookAtBone`
  (header +6) by a basis built from `bone.T - lookAtTarget` and the up vector (0, 0x1000, 0) (two cross products,
  `VectorNormal`), scaled like `W` when flags bit 1; its children inherit it. All other bones are as above.
- **Attached to a parent model** (`ModelObj.parent`, op `EVT_OP_ATTACH` 0x4C `obj, parent, bone`): bone 0 is the
  parent's bone `parentBone` moved by `attach` (op 0xD4): rotated by that bone (`sat16(R · attach >> 12)`) in
  `fldmdl_calcBoneMatrices`, added unrotated in the look-at variant. Battle weapons do the rotated form.
- **No motion**: one matrix, the parent bone (as above) or `W` with its y and z columns negated (as battle).
- Bones are built in scratchpad (0x1F800018) and copied out when there are fewer than 31; with 31 or more they are
  built in place. No numeric difference.

`tools/field_models.py --pose MODEL --field F [--anim KEY] [--frame N] [--bones] [--obj FILE]` gives the pose;
`--check` decodes every frame of every model's default motion (no saturation, enough keys for the bones).

### Runtime check
`build/`-only script (not committed) over the Redux RAM, Redux paused in each state:

- `field_dali_inn` (disc 1, field 351): the headers of Zidane (98, dir 10, at 0x8012D888) and the field-local
  models 115, 179, 220, 133, 134, 258, 225 equal the disc's bytes word for word (3,613 words for Zidane), with
  exactly the relocated words changed (`flags` bit 0, `bones`, `groups`, the five group pointers = header +
  file offset). `GameData.unk8D8[0]` holds model id 98 for character 0.
- Bone matrices (`ModelObj.boneMatrices`) of Zidane (27 bones, motion 0x0300C8 frame 2), 115 (frame 19) and 179
  (frame 8) equal `battle_pose` with `base = ModelObj.worldMatrix` and `frame = FieldEvtObj.animFrame`: max
  error 0. Model 220 has look-at on: only its look-at bone (11) and its children differ.
- `disc3_pandemonium_lv99_field` (disc 3, field 2714): Zidane again (look-at on; only bone 8, his look-at bone,
  differs), three actors sharing local model 91 (frame 0, error 0).

## 3. What indexes a field model's vertices, polygons, bones or groups

### Vertex and polygon numbers
Only the mesh routines use them, all inside one group and in the same frame:

| function | reads | notes |
|---|---|---|
| `fldmdl_projectMesh` | `boneVertexCounts`, `vertices`, bone matrices | writes `g_vtxScreenXY/Z[0 .. group vertices + 2]` |
| `fldmdl_drawMesh` (asm) / `fldmdl_drawMeshC` (C twin, no callers) | source records' `vtx` and `flags & 2`, `otBias`, packets | packet k ↔ record k |
| `fldmdl_projectMeshClipY` / `fldmdl_drawMeshClipY` (`fld_modelCommand` 4, `ModelObj.clipY`) | same | each vertex's model-space y is clamped to `clipY` and marked; a polygon whose corners are all clamped is not drawn (wading, sinking) |
| `fldmdl_drawMeshMirrored` (mirror copy, `fld_modelCommand` 8-10) | same, NCLIP sign reversed | the copy is a second `ModelObj` on the same header with its own packets |
| `fldmdl_buildPackets` | every list: `vtx` count, `uv`, `col`, `rgb`, `tex`, `flags` | builds the kept packets once |
| `fldmdl_setSemiTrans` (`fld_applyModelCommands`, op 0x88 cmd 0) | FT4/FT3/GT4/GT3 source `flags` in parallel with the packets | sets / restores each packet's semi-transparency bit |
| `fldmdl_tintGroup`, `fldmdl_tintGroupCur` | source rgb in parallel with the packets | no callers |

Nothing else in ovl_0a, main or any other overlay reads `g_vtxScreenXY/Z`, a group's `vertices` or its polygon
records. There are no vertex anchors:

- **Speech balloons** (main `evt_placeBalloon_pos`): actor position + `height` (op 0x2F's u8 × scale) + the
  balloon offsets; no model data. `EVT_OP_OBJ_SCREEN_POS` (0xA9) projects the actor position.
- **Held items / attachments**: a whole model attached to a **bone** of another (op 0x4C `parentBone`, op 0xD4
  `attach` offset); `fld_spriteCommand` 0x96 attaches a 3D sprite to a **bone** (`FieldSprite.parent/bone`,
  `fld_drawSprites` reads that bone's translation).
- **Shadows** (`fld_drawShadow` / `fld_calcShadowMatrix`): their own 4 quads; place and direction from **bone 1**'s
  matrix (unless `FLD_OBJ_SHADOW_FIXED`), y from the actor.
- **Culling**: `fld_drawModels` drops a model whose **bone 0** origin is too near or far in OT depth;
  `fldmdl_cullBehindCamera` (no callers) used the world origin.
- **Light, colour, fade**: `fld_applyLights` → `fldmdl_setGroupRgb` for every group: writes the first colour word
  of **every packet** of the current buffer (code byte kept), i.e. one colour per group, gouraud corners 1-3
  untouched. The mirror tint (`fld_modelCommand` 11) does the same. `fldmdl_setAbr` sets the blend mode in every
  textured packet's tpage. `fldmdl_addGroupRgb(Cur)` / `fldmdl_tintGroup(Cur)` (no callers) work the same way. So
  colours are per group and per packet, never per vertex; the source `rgb`/`colors` only matter at
  `fldmdl_buildPackets`.
- **Texture animations** (`fldmdl_updateTexAnims`; started / stopped by ops 0xC0-0xC2 in main, and
  `fld_drawModels` restarts animation 2 of a model with at least 2 whenever none is active, unless the slot's
  flags have 0x1000): `MoveImage` of VRAM rectangles; they never touch polygons. A simplified model must keep the uvs
  of polygons that sample animated rectangles (eyes, mouths) where they are, or the animation shows up distorted.
- **Mirror** (`fld_modelCommand` 8-11, `FieldActorExt`): the same header and the actor's motion, reflected.
- **Port note, a NULL read and write**: `fld_drawModels` reads `ModelObj.texAnims->count` for every drawn model
  whose slot flags lack 0x1000, without a NULL check. For a model without texture animations (every local model
  that is not its container's first info entry, see "Model info") that reads the u16 at address 0 (3 on the PS1,
  BIOS RAM), and since `fld_isModelTexAnimActive(NULL)` returns 0 it then writes "animation 2" fields at
  physical 0x2C (`flags |= 1`), 0x34 (`pos = 0`, s32) and 0x3A (`lastFrame = 0x1000`). The `field_dali_inn` state's
  RAM holds exactly these values (01 at 0x2C, 0 at 0x34, 0x1000 at 0x3A), and `ModelObj.texAnims` is NULL for all
  seven local models of field 351 (Zidane's is 0x80133200, 3 animations). Like F4, a low-RAM dependency of the
  original, not something to fix in the matching code.
- **World map** (ovl_0c `wm_*`): the same code on world files' own models.

### Bone and group numbers

| index | used by |
|---|---|
| bone count, order, parents, lengths | every motion (one key per bone, in order); parent < bone (checked on all discs) |
| `WmModel.lookAtBone` (+6) | look-at variant |
| bone 0 | origin for the near/far cull; replaced by the parent bone when attached |
| bone 1 | shadow position and direction |
| any bone number | op 0x4C (attach: the parent's bone), `fld_spriteCommand` 0x96 (sprite on a bone); both come from script literals |
| group count (≤ 32) and order | `ModelObj.hiddenGroups` bits: ops 0x39 (show: clears the bit) / 0x3A (hide: sets it), `fld_drawModels`; `fld_applyLights` / mirror tint loop over all groups |
| texture index | model info tables (texture words, uv offsets, texture positions), `fldmdl_initTexAnims` |
| texture animation index | ops 0xC0-0xC2, the automatic restart of animation 2 in `fld_drawModels` |

### What a simplified model must keep
- `boneCount` and every bone's parent and length (motions, look-at bone, attachments by bone number, shadow on
  bone 1, sprites on bones), and the header's `lookAtBone`;
- `groupCount` (≤ 32) and the group order (script hide/show masks), each group's `otBias`;
- each vertex's bone (the `boneVertexCounts` layout: positions are bone-local);
- texture indices and the uvs of texture-animated areas; the model info entry (texture count) unchanged;
- per group ≤ 801 vertices (`g_vtxScreenZ` holds 802 entries; triples overrun by up to two);
- consistent `packetSize` per group (= Σ count × packet size: it is the second buffer's offset) and the list order;
- the RAM it needs can only shrink, so a smaller model always fits where the original did (`heapTop` budget,
  `fld_collectVisibleModels` drops a model whose packets end past `g_loadAreaEnd + 0xF1700` while a movie plays).

Vertex, uv, colour and polygon numbering inside a group is free to change; nothing outside the group's own lists
refers to it. A file whose size changes also changes its dependency entry in the field file (dir 10 models:
`sectors` in every field that lists it, and the `split` in the file id), as for battle models.

## 4. The party's field models

Script entries `scriptCount - 9 + c` (c = 0..8) are party character c (`evt_getObjectChrIndex`, main
`evt_execInstr` battle path `evt_execInstr_newactor(r + g_evtScriptCount - 9)`), and character c is
`GameData.characters[c]`. Who holds a slot was read at run time from `characters[c].name`: in `field_dali_inn`
(disc 1) slots 0-8 are Zidane, Vivi, Dagger, Steiner, Freya, **Cinna, Marcus, Blank**, Beatrix; in
`disc3_pandemonium_lv99_field` slots 5-7 are **Quina, Eiko, Amarant**. The models below are the dependency
entries linked to those entries over all fields of the four discs; "cached" = one of `g_fieldModelCache`'s ten
fixed ids (ovl_0a .data 0x800C9CCC, the permanent party, kept across fields):

| slot | character | model (fields linking it) |
|---|---|---|
| 0 | Zidane | **98** (1007, cached, dir 10) |
| 1 | Vivi | **8** (332, cached) |
| 2 | Garnet | **185** (320, cached), **76** (cached; replaces 185 from story 0x283C, `fld_remapModelId`; listed itself only in field 2171), local 202 (11 fields on disc 1, 51-166) and 205 (field 207; same data as 202), local 381 (field 932) |
| 3 | Steiner | **5489** (180, cached), local 658 (46 fields, 301-3050; same data as 5489) |
| 4 | Freya | **192** (153, cached) |
| 5 | Quina; Cinna early | **273** (171, cached; discs 1-4 from field 157) = Quina; local 107 (18 fields, 50-59 and the ending 3011) = Cinna |
| 6 | Eiko; Marcus early | **443** (140, cached; from field 956, disc 2) = Eiko; local 109 (46 fields, 50-117 and 3008) = Marcus |
| 7 | Amarant; Blank early | **509** (73, cached; from field 956) = Amarant; local 5467 (39 fields, 50-64 and 3010) = Blank; local 190 (fields 51, 53, 161) |
| 8 | Beatrix | **204** (41, cached); local 20 (field 1369) |

Which model of a shared slot is which character follows from the slot's occupant at the time: the cached model
is the permanent party member (Quina / Eiko / Amarant join for good), the local ones appear only in the opening
fields and the ending, where the runtime names show Cinna / Marcus / Blank.

Copies of party models under other ids (same bytes, found by `field_models.py`), loaded by non-party actors in
cutscenes: 668, 669, 670 = 98 (field 2933), 662 = 8 (fields 2900, 3000), 660 = 109 (fields 956, 1816, 1861),
659 = 5467 (field 1861), 118 = 20 (63 fields on disc 1, 103-…). Apart from the forms in the table, the party slots load
nothing else: no chocobo-riding or disguise form is a separate party-slot model (slot 0 is 98 in all 1,007
fields; mounts and props are other actors, which the script can join to a bone with op 0x4C).

The battle party models (dir 10 ids 281-291, 580-608, 5414, …) are separate files; the field never loads them.

## 5. tools/field_models.py
```
python3 tools/field_models.py                        # one row per field model, heaviest first, all discs
python3 tools/field_models.py --top 20
python3 tools/field_models.py --party                # party slot models, cached models and their copies
python3 tools/field_models.py --field 351 --disc 1   # one field: model, source, actor entry, party slot, counts
python3 tools/field_models.py --fields --top 15      # heaviest fields (sum over the models they load)
python3 tools/field_models.py --fields --by-non-party --top 15
python3 tools/field_models.py --model 98 --meshes    # per group: vertices, uvs, otBias, textures, bone:vertices
python3 tools/field_models.py --check [--worlds]     # validation pass, exit 1 on any problem
python3 tools/field_models.py --json build/field_models.json
python3 tools/field_models.py --pose 98 --field 351 --anim 0x30019 --frame 0 --bones --obj build/zidane.obj
```
It shares `find_image`, `Img` and the container readers with `encounter_catalog.py`, the header walker
(`header_stats`) with `battle_models.py` and the pose code with `battle_pose.py`. "party" is the slot's
character, `= N` a copy of party model N, `cached` a `g_fieldModelCache` id. Counts are per model (front and back
faces); a field's sum counts each model it lists once, though several actors may show the same model. "ram" is
`fld_setupActorModel`'s allocation per actor (two packet buffers, bone matrices, group records).

Heaviest models (`--top 10`):

```
model source  bones grp  verts polys   FT4  FT3 ...  packets  party  places
  200 local      47   6   2732  1999  1295  704        74328          15: 1:f551 1:f563 1:f572 1:f609 1:f610 2:f1301 +9
  534 local      84   3   1467  1830   578 1252        63184           7: 2:f1251 2:f1252 2:f1253 2:f1254 2:f1604 3:f1604 +1
  625 local       8   2   1463  1364  1022  342        51824           3: 3:f2203 3:f2204 3:f2205
  436 local      22   2   1361  1090   630  460        39920          12: 1:f602 1:f603 1:f605 2:f602 2:f1352 2:f1353 +6
  252 local      57   3   1340  1566   430 1136        53552           2: 2:f1422 2:f1423
  533 local      52   2   1242  1500   390 1110        51120           2: 3:f2510 4:f2510
  529 local      78   2   1171  1625   375 1250        55000           2: 3:f2552 3:f2553
  256 local      53   2   1170  1794   206 1588        59056           1: 4:f1917
  442 local      45   2   1135  1550   446 1104        53168           1: 2:f1052
  617 local      71   2   1123  1850   140 1710        60320           1: 3:f1916
```
The party models have 290-476 vertices each (Steiner 444, Zidane 357, Garnet 337).

Heaviest fields by the vertices of the models they load (`--fields --top 8`), and by their non-party models
(`--fields --by-non-party --top 8`):

```
disc field  area models party  verts polys  non-party verts  ram          disc field  area models party  verts polys  non-party verts
   3 f2301   908     15     8   4683  5739             1944  407448         1 f609     22      7     2   4543  3874             3742
   1 f609     22      7     2   4543  3874             3742  286704         3 f2101   595      5     1   3823  3352             3466
   3 f2169   943     14     8   4430  5447             1691  386564         2 f1310   485      5     1   3819  3361             3462
   4 f2169   943     14     8   4430  5447             1691  386564         1 f551    276      4     1   3618  3044             3261
   3 f2715   344     10     8   4222  5546             1483  386384         1 f572    276      4     1   3609  3068             3252
   4 f1917   741     10     8   4193  5357             1454  373616         1 f610     22      4     1   3608  3059             3251
   3 f1916   741     10     8   4146  5413             1407  376720         1 f563    276      4     1   3584  3053             3227
   2 f956    359     10     8   4077  4742             1398  336984         2 f1359   525      4     1   3563  2831             3206
```
