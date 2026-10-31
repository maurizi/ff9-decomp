# R35: 3D models for a native renderer (answers for the 3DS port)

Answers from the matched C (ovl_0a field, ovl_0b battle, ovl_0c world map, main), the C twins of the hand-written asm
in `src_equiv/`, the existing docs (docs/battle-models.md = R26/R27, docs/field-models.md = R28, docs/r29/battle-render.md,
docs/r29/field-world.md) and a few scratch scans of the disc 1 files (not committed, `build/3ds5d/`). No emulator run;
what still needs one is listed at the end. Line numbers are those of commit 5181ea1c.

The short version: **all three modes use one model format and one skinning scheme** (rigid, one bone per vertex, no
normals, no GTE lighting). Battle, field and world map differ only in where the model matrix and the camera come
from, in how packets are kept, and in how the per-model colour is set. Every model on the discs that the party, the
NPCs, the enemies or the weapons use has only FT4/FT3 (flat-coloured textured) polygons, and every source colour is
0x808080. The colour you see is one value per model (or per mesh group) written into the packets, not lighting.

## Short answers

1. **Formats.** Header `BtlUnit9C8` (ovl_0a/0c: `WmModel`), 0x14 bytes → `BtlBoneDef[boneCount]` (4 bytes: z length,
   parent) and `BtlMesh[meshCount]` (ovl_0a/0c `WmModelGroup`, 0x28 bytes: eight polygon counts, `otBias`, and five
   arrays: per-bone vertex counts, `SVECTOR` vertices, polygon records, u8 uv pairs, rgba colours). Polygons are the
   eight `WmSrc*` records (FT4 FT3 GT4 GT3 G4 G3 F4 F3), which carry vertex, uv and colour indices, a texture index and
   flags (semi-transparent + abr, double-sided). **No normals** (the SVECTOR pad is garbage and nobody reads it).
   **Skinning**: a group's vertices are stored bone by bone (`vertexCounts[b]` vertices for bone b). Each vertex is
   rigidly bound to that one bone, and a polygon may join vertices of different bones (31.8% do). Motions (`BtlUnit9CC`,
   ovl_0a `WmAnim`) give per frame one ZYX Euler rotation (12-bit angles) per bone plus a root translation. They have
   no per-bone translation, no scale and no interpolation. Full layouts: docs/battle-models.md "Layout" and "Motions".
   This note adds the gaps (texture placement, the per-mode object structs) in Q1.
2. **Draw path.** The same three steps in every mode: (a) the model matrix (`math_rotMatrixZYX` battle/field, `YXZ` world
   map, + position, + scale), (b) `*_calcBoneMatrices` → per-bone world matrices (32 bytes each, `MATRIX` layout), and (c)
   per mesh group, `*_projectMesh` (view × bone, RTPT into `g_vtxScreenXY/Z`) and `*_drawMesh` (NCLIP, OT link). The
   hook point "model + matrices" is right after (b) and replaces (c). The view matrix is always
   `GameData.viewMatrix`, H = `GameData.projDist`, and OFX/OFY are (160, 112) except in the field (camera centre +
   scroll). Per-mode tables in Q2.
3. **Equip view.** Use the **battle** model: `kind = Character.info.b.kind` (= `chr_calcAppearance`, recomputed by the
   Equip screen itself), `model = g_btlCharModelIds[kind]` (ovl_0b 0x800F5200), file = FF9.IMG dir 10 with file id &
   0xFFFF = model. Idle = motion `0x030000 | g_btlPartyMotionIds[kind][0]` (ovl_0b 0x800F524C, 34 per kind) in the
   same file. The weapon is `g_weaponTable[equip[0]].modelId` (main 0x8006E0E8), FF9.IMG dir 8 file = model id
   (1 bone, 1 group). Its matrix is exactly the body's bone `Character.weaponBone` (per-kind table). The game's loaders
   need the battle work area, so reimplement them (~6 steps, Q3). Tables for all 19 kinds are below.
4. **Lighting.** None in the GTE sense: no NCDS/NCCS/NCT on any character, enemy or weapon model. Every textured packet
   carries one flat colour (0x80 = texture unchanged, PS1 modulation `texel × rgb / 128`). That colour is set **per
   model** in battle (the arena's `lightR/G/B`, plus per-scene status tints that add to it or replace it), **per actor**
   in the field (`fld_applyLights`: (model colour + floor light) × 8, or script op 0x8F), and **never** for world-map
   actors (0x80 always), which instead get distance fog by **CLUT replacement** (`wm_setActorFog`, DPCL on 15-colour
   palettes). Only the world-map terrain blocks use the GTE light/colour matrices (docs/r29/field-world.md section 7).

## 1. Model data in RAM

### 1.1 Shared format (battle, field, world map, weapons, effect models)
Structs: `include/ovl_0b.h:26-160` (`BtlMesh`, `WmSrcFT4`..`WmSrcF3`, `BtlBoneDef`, `BtlUnit9C8`, `BtlUnit9CC`, `BtlBone`);
`include/ovl_0a.h:800-889` (`WmModelGroup`, `WmModel`, `WmBone`, `WmAnim`); `include/ovl_0c.h` has the same.
docs/battle-models.md "Layout" gives every offset. Recap:

| struct | size | fields |
|---|---|---|
| `BtlUnit9C8` / `WmModel` header (DB item 0x02xxxx) | 0x14 | +0 flags (bit 0 relocated), +2 u8 boneCount, +3 u8 meshCount (≤ 32 field, ≤ 16 battle in practice), +4 u16 packetSize, +6 u8 lookAtBone (field only), +8 u16 height, +A u16 radius, +C bones (offset → `BtlBoneDef*`), +10 meshes (offset → `BtlMesh*`) |
| `BtlBoneDef` / `WmBone` | 4 | +0 s16 length (z offset from the parent's origin, along the parent's z axis), +2 u8 unused, +3 u8 parent (< own index; bone 0 = root) |
| `BtlMesh` / `WmModelGroup` | 0x28 | +0 u16 packetSize, +2..+10 u16 counts FT4 FT3 GT4 GT3 G4 G3 F4 F3, +13 s8 otBias, +14 `u16 vertexCounts[boneCount]`, +18 `SVECTOR vertices[]`, +1C polygon lists (back to back in count order), +20 `u8 uvs[][2]`, +24 `u8 colors[][4]` |
| motion `BtlUnit9CC` / `WmAnim` (0x03xxxx) | 0x14 | +2 frameCount, +4/6/8 root x/y/z (constant or offset of s16[frames]), +A constMask, +C rotKeys, +10 rotKeysLo |
| bone matrix `BtlBone` (output) | 0x20 | `MATRIX`: s16 m[3][3] row-major (4096 = 1.0) + pad, s32 t[3] |

Polygon records (`include/ovl_0b.h:44-121`). `vtx` indexes the group's vertices, `uv` its uvs, `col` its colours:

| list | bytes | vtx | uv | colour | tex | flags |
|---|---|---|---|---|---|---|
| FT4 | 0x18 | +0 [4] | +8 [4] | rgb +0x10 | +0x13 | +0x15 |
| FT3 | 0x14 | +0 [3] | +0xC [3] | rgb +8 | +6 | +0x12 |
| GT4 | 0x20 | +0 [4] | +8 [4] | col +0x10 [4] | +0x1D | +0x1C |
| GT3 | 0x18 | +0 [3] | +6 [3] | col +0x10 [3] | +0x16 | +0xF |
| G4 / G3 | 0x18 / 0x14 | [4] / [3] | – | col +8 | – | +0x14 / +0x13 |
| F4 / F3 | 0x10 / 0xC | [4] / [3] | – | rgb +8 | – | +0xD / +0xB |

- flags (`WMSRC_*`, `include/ovl_0b.h:56-59`): bit 0 semi-transparent, bits 5-7 the abr (ORed into the tpage), bit 1
  double-sided (no NCLIP). Quads are drawn as triangles (0,1,2) + (1,2,3); NCLIP tests 0,1,2 only (front = clockwise in
  screen space, NCLIP > 0).
- **What the discs use** (R26/R28/R29 scans, plus a scan of the party and weapon files for this note): FT4 and FT3
  only, in every enemy (182), field (345 + 17 world) model, battle party body (42) and weapon (85). **Every** FT4/FT3
  rgb in the party battle bodies, the field party models and the sampled weapons is 0x808080. The gouraud/untextured
  lists exist in the code (effect models may use them; effect files were not scanned).
- **Normals**: none. The `SVECTOR.pad` of model vertices holds unrelated values (e.g. 31251, -18418) and no model code
  reads it. The world-map **terrain** uses the pad as a normal index; that is a different format.
- **Skinning**: rigid. Vertex k of a group belongs to the bone b with
  `sum(vertexCounts[0..b-1]) <= k < sum(vertexCounts[0..b])`, in bone-local coordinates: `p = T_b + (R_b · v) >> 12`.
  Polygons join bones freely inside a group (docs/r29/battle-render.md item 9: 31.8% of polygons, mostly parent/child).
  Vertices are shared between polygons inside a group (index buffer), uvs are per corner, and nothing is shared
  across groups. For a cel outline, build adjacency per group.
- Coordinates: PS1 axes, **y down** (characters stand at y ≤ 0). Units: battle party heights 356-622 (`height`
  field), field Zidane about 450.

### 1.2 Textures (the gap in the earlier docs: where texel (u, v) is)
Texture index `tex` → the model's texture word `{s16 tpage, s16 clut}` (`BtlUnit9C18` / `WmTexInfo`). Final
page-local texel = `(u + du[tex], v + dv[tex])` with the uv offset table of the model info entry. The PS1 then samples
the 256×256 texture page `tpage` (8-bit CLUT mode on every model: `getTPage(1, ...)`) with CLUT `clut`. Model info =
DB item 0x12xxxx of the model's container: 12-byte entries `{u16 modelId, u16 animId, u32 tableOffset:24 | count:8,
u16 texAnimId, u16 pad}`, and at entry + tableOffset three tables of `count`: texture words, uv offsets {s16 du, dv},
texture positions (8 bytes). docs/battle-models.md "Textures", docs/field-models.md "Model info".

Where the TIMs go and what du/dv become:

| models | uploaded by | VRAM image | CLUT | du, dv |
|---|---|---|---|---|
| battle party (dir 10, two TIMs) | `btl_uploadModelTextures(.., slot, 1)` (`src/ovl_0b/btl_main.c:1085`) | x = slot·64 + 0xC0 (page-aligned), y = h = 0x80 + 128·texPos[n].y (texture 0 at y 0x80, texture 1 at 0x100) | (0x100, 0xE5 + 3·slot + texPos[n].y) | du 0, dv = h%256 − h%128 (128 for y 0x80, 0 for 0x100) |
| battle weapon (dir 8) | same, arg3 = 0 | x = slot·64 + 0xC0, y 0x1D0 | (0x100, 0xE8 + 3·slot) | du 0, dv 0x80 |
| enemy | `btl_loadEnemyModelsStep` (btl_main.c:684), `file_setTimPos` at the model info's positions | from the model info | from the model info | from the model info |
| field dir-10 party | `fld_loadModelBundle` (`src/ovl_0a/fld_movie.c:26`), VRAM slots from the field's `FieldVramPos` | allocated per field | allocated | written into the model info tables |
| field-local / world | TIMs at their own positions (`fld_loadFile`, `wm_*` file load) | TIM header | TIM header | from the file |

The page-local v = file v + dv puts a texture's v range into the 128-row half where the image was placed. **For the
party bodies the file's uvs are TIM-local** (TIM at page row 0x80 with dv 0x80, or at row 0 with dv 0). For weapons the
TIM lands at page row 0xD0 with dv 0x80, so TIM-local v = file v − 0x50. The general rule is
`TIM-local = (u + du, v + dv) − (TIM x within the page, TIM y % 256)`. Battle applies du/dv once to the source `uvs`
at load (`btl_applyModelTextures`, `src/ovl_0b/btl_mesh.c:508`). Field and world apply them to the packets only
(`fldmdl_buildPackets` `src/ovl_0a/tu_800B61A4.c:125`, `wm_buildPackets` `src/ovl_0c/wm_model.c:125`), so their
source uvs stay as in the file.

For in-mode rendering the simplest exact choice is to sample the port's VRAM copy with (tpage, clut, page-local uv),
because **texture animations** (eyes, mouths: `btl_updateTexAnims` `src/ovl_0b/btl_model.c:116`,
`fldmdl_updateTexAnims` `tu_800B61A4.c:1699`; item 0x19xxxx) rewrite VRAM rectangles with `MoveImage` and never touch
uvs, and Petrify swaps `texPages[].clut` to grey copies (`btl_setStoneClut`). Texel 0x0000 is transparent. With the
semi-transparent flag only texels with the STP bit blend. abr: 0 = B/2+F/2, 1 = B+F, 2 = B−F, 3 = B+F/4.

### 1.3 The per-mode model objects

| mode | object | fields the renderer needs |
|---|---|---|
| battle | `BtlUnit9C` (`include/ovl_0b.h:283-310`), one per unit (`ModelSlot.model`) and per weapon (`Battle.weaponModels[n].model`) | +0 flags (`BTL_MODEL_SCALED` 2, `OFFSCREEN` 0x10, `DRAWN` 0x20), +4 parentBone, +5 abr, +8 header, +C motion, +10 prims (first built packet), +14 hiddenMeshes (bit per group), +18 texPages (`{tpage, clut}` per tex), +1C parent, +20 bones, +24 matrix (model matrix), +50 texAnims, +54 scaleX/Y/Z, +74 attachX/Y/Z |
| field | `ModelObj` (`include/ovl_0a.h:891-919`) in `ModelSlot.model` (`FieldObjSlot`) | +0 flags (`MDL_LOOK_AT` 1, `MDL_SCALED` 2, `MDL_BEHIND_CAMERA` 0x10), +4 parentBone, +8 model, +C anim, +10 packets, +14 hiddenGroups, +18 texInfo, +1C parent, +20 boneMatrices, +24 worldMatrix, +44 groupOut (`WmModelOut[groups]`, +4 = this group's packets), +48 lookAtTarget, +50 texAnims, +54 scale, +5A clipY, +60.. mirror, +74 attach |
| world map | `ModelObj` (`include/ovl_0c.h:472-498`), same layout as the field's | as the field (no look-at) |

The object for party character c is `GameData.chrModels[c]` (`game_getModel(c)`, main `src/main/file.c:328`), and
NPCs/enemies use `npcModels[n]` (slot 9 + n).

## 2. Draw path per mode

### 2.0 Common math
- **Model matrix** `W`: rotation from the actor's angles (12-bit, 4096 = 360°) by `math_rotMatrixZYX` (mode 0, CPU
  products) or `YXZ`/`ZXY`, translation = position, then `*_applyScale` multiplies column j by scale_j / 4096 with GTE
  rounding (`btl_applyModelScale` `btl_mesh.c:617`, `fldmdl_applyScale` `tu_800B61A4.c:684`, `wm_applyScale`
  `wm_model.c:688`).
- **Bones** (docs/battle-models.md "Composing the bones"): `R_0 = sat16(W.R·Rot_0 >> 12)`, `T_0 = W.T + (W.R·root) >> 12`;
  `R_i = sat16(R_p·Rot_i >> 12)`, `T_i = T_p + (R_p·(0,0,len_i)) >> 12`; `Rot = Rz·Ry·Rx` from `math_rotMatrixZYX_gte`
  (mode 1). For an **attached** model (weapon, field op 0x4C), bone 0 = the parent's bone `parentBone` with
  `T += R·attach >> 12`, and the decoded bone 0 is dropped. With **no motion**, the model gets one matrix: the parent
  bone as above, or W with its y and z columns negated. `tools/battle_pose.py` is bit-exact for all of this.
- **Projection** (per group, per bone with vertices; `btl_projectMesh` `src/ovl_0b/btl_mesh.c:5`, `fldmdl_projectMesh`
  `tu_800B61A4.c:3`, `wm_projectMesh` `wm_model.c:3`): `M.R = sat16(V.R · B.R >> 12)` (RTIR per column),
  `M.T = V.T + (V.R · B.T) >> 12` (RTV0TR), then RTPT per vertex: `(x, y, z) = M.T + (M.R·v) >> 12`,
  `SZ = clamp(z, 0, 0xFFFF)`, `SX = OFX + H·x/SZ`, `SY = OFY + H·y/SZ` (GTE UNR division, saturating at
  H/SZ ≥ 2 and to −1024..1023). V = `GameData.viewMatrix`, H = `GameData.projDist` (`gte_SetGeomScreen` at the top
  of every projectMesh). So a native renderer's projection is a pinhole with focal length H pixels and principal
  point (OFX, OFY) in a 320×224 frame, y down, camera looking down +z. There is no near-plane clip, and polygons with a
  corner at z ≤ 0 project garbage on the PS1 (see the culls below).
- **OT depth** (needed for the painter's order of semi-transparent polygons and to understand `otBias`): battle quads
  `(ΣSZ) >> 6`, triangles `AVSZ3 >> 2` (ZSF3 = 0x155); field quads `(ΣSZ) >> 4`, triangles AVSZ3; world quads
  `(ΣSZ) >> 5`, triangles AVSZ3 >> 1. The group's `otBias` is then subtracted (s8, positive = nearer). Groups with a
  bias are layering hacks (eyes, hair, weapons on top). With a z-buffer, polygons with otBias ≠ 0 need a matching
  depth offset or the PS1 order (docs/r29/battle-render.md item 2).

### 2.1 Battle (ovl_0b)

| step | where | what |
|---|---|---|
| camera | `btlcam_step` (`src/ovl_0b/btlcam_o1.c:248`): `btlcam_lookAt(&gd->viewMatrix, &eye, &target)` (:387, function :820, up = (0, −30000, 0), roll = `eye.pad` via `math_rotMatrixXYZ` + `CompMatrix`), `gd->projDist = g_btlCamera.projDist` (:401), `SetGeomScreen` (:403) | once per scene |
| screen offset | (160, 112) from main `game_reset` (`src/main/init.c:24`); battle never changes it, except effect helpers that save and restore around their own drawing (`btlfx_op.c:1076/1140/1195/1208`) | constant |
| scene | `btl_main` → `btl_update` → per unit `btl_updateUnitModel(u, &prim)` (`src/ovl_0b/btl_unit.c:30`; callers btl_cmd.c:39, btl_update.c:94/144/330) | one scene per 4 VBlanks (15 Hz) |
| model matrix | btl_unit.c:56-73: `math_rotMatrixZYX(u->rot)` (yaw forced to 0x800 in the escape motion, slot 17), `t = u->pos >> 16`; scale :83 | per unit |
| bones | btl_unit.c:91: `btl_calcBoneMatrices(m, frame, prim)`: **hand-written asm** (`btl_mesh.c:669`; C twin `src_equiv/ovl_0b/bones.c`). It writes the matrices **into the scene's primitive buffer** at the cursor, sets `m->bones` to them and returns the advanced cursor. They are only valid until that buffer is reused two scenes later, so copy them right after this call | per unit, then `frame++` (:92) |
| visibility | `btl_updateModelVisibility` (`btl_mesh.c:647`): the model is skipped (OFFSCREEN) when its origin's SZ is 0; a weapon follows its parent | |
| faces | btl_unit.c:96-104: for each group not in `hiddenMeshes`: `btl_projectMesh` + `btl_drawMesh` (**asm**, `btl_mesh.c:3`; C twin `src_equiv/ovl_0b/mesh_packets.c`; unused matching twin `btl_buildMeshPrims` `btl_mesh.c:71`). It builds a fresh packet per front face every scene: rgb from the source, clut/tpage from `texPages[tex]`, tpage \| (flags & 0xE0) when semi-transparent | |
| texture anims | btl_unit.c:106 `btl_updateTexAnims` | |
| weapon | btl_unit.c:108-121: `btl_calcBoneMatrices(weapon, 0, ..)` (motion NULL → the body's bone `parentBone` + R·attach, attach = 0), then project/draw its one group | |
| shadow | btl_unit.c:122ff `btl_drawShadow` (`btl_main.c:1523`): its own 4 FT4 quads from bones `shadowBone`/`shadowDirBone` | |
| colour overrides | after the unit is drawn: `btl_updateUnitTint` (`btl_status.c:395`), fades, effects. They rewrite the **packets just built** (Q4) | per scene |
| effect / summon models | `hi_model.c:164-323` (`Hi_DrawEffModel` etc.): same `btl_calcBoneMatrices` + `btl_projectMesh` + `btl_drawMesh`/`btl_drawMeshOffset` (asm, `btl_mesh_asm.c:3`); slice/morph variants `btlfx_projectSliceMesh`/`btlfx_projectMorphMesh` (asm, `src_equiv/ovl_0b/mesh_project.c`) | |

**Hook point**: in `btl_updateUnitModel`, after line 91 (body) and 110 (weapon), submit
`(m->header, m->bones, gd->viewMatrix, gd->projDist, hiddenMeshes, texPages, abr)`. Then skip lines 96-104 / 113-121, but
keep `btl_updateModelVisibility` and the DRAWN flag (popups and the colour writers test them). Because the colour
writers run after the draw in the same scene, the native draw has to be **deferred to the end of the scene**
(`btl_endFrame`), with the colour state recorded per model (Q4).

### 2.2 Field (ovl_0a)

| step | where | what |
|---|---|---|
| camera | `fldbg_setCamera(id)` (`src/ovl_0a/tu_800B61A4.c:3876`): `BgCamera` (`include/ovl_0a.h:471-485`, 0x34 bytes: projDist, rot[3][3], trans[3], centerX/Y, size, scroll limits, otBias) copied to `gd->viewMatrix` (through `g_fieldBgState.viewMatrix`), `projDist`, the scene otBias. Movie backgrounds drive it from `mbg` (`mbg_getStartCamera`) | on camera change (op 0x7E) |
| screen offset | `fld_drawModels` (`fld_movie.c:381-386`): `fldbg_applyGeomOffset` (`tu_800B61A4.c:3996`) = `(centerX + geomOffsetX, centerY + geomOffsetY)`, with geomOffset = `160 − (scroll.x − width/2)`, `112 − (scroll.y − height/2)` (`tu_800B61A4.c:3278-3282`) or the follow code (:3404); `mbg_getGeomOffset` with a movie camera; reset to (160, 112) after the models (fld_movie.c:516) | per field frame |
| frame | `fld_main` (`fld_main.c:20`) → `fld_drawModels` (`src/ovl_0a/fld_movie.c:356`) | 30 Hz |
| model matrix | fld_movie.c:391-411: `math_rotMatrixZYX(rotX, rotY, rotZ)`, or `math_rotMatrixZXY(tiltX, rotY, tiltZ)` with `FLD_OBJ_TILT` (walkmesh tilt); t = actor pos; `fldmdl_applyScale` | per actor |
| motion | fld_movie.c:413-426: `db_lookupRegistered(4, obj->anim \| 0x30000)` | |
| bones | fld_movie.c:428-439: `fldmdl_calcBoneMatrices(m, animFrame)` (C, `tu_800B61A4.c:731`) or `fldmdl_calcBoneMatricesLookAt` (`:1442`, look-at bone replaced). Output `ModelObj.boneMatrices` (the actor's own heap block, world space, kept between frames) | |
| cull | fld_movie.c:440-458: bone 0's origin through the view, `SZ/4 + otBias` outside [100, 0xF9C] drops the model | |
| faces | fld_movie.c:459-485: per group not hidden, `fldmdl_projectMesh` + `fldmdl_drawMesh` (**asm** `fldmdl_draw_asm.c`; C twin `src_equiv/ovl_0a/model_link.c`; matching twin `fldmdl_drawMeshC` `tu_800B61A4.c:400`), or `fldmdl_projectMeshClipY`/`fldmdl_drawMeshClipY` (:1814/:1872; wading: model-space y clamped to `clipY`, fully clamped faces dropped). Packets are **prebuilt once** per actor, double-buffered (`groupOut[g].prims + (drawBuf ? packetSize : 0)`); drawMesh only writes xy and links | |
| after | `fld_drawShadow` (bone 1), texture animations, `fld_applyLights` (fld_movie.c:509, Q4), `fld_applyModelCommands` (:513: semi-transparency, **mirror copies** recomputed and drawn by `fldmdl_drawMeshMirrored` `tu_800B61A4.c:2210` after `fldmdl_reflectModel` :2170) | |

**Hook point**: after the bone loop (fld_movie.c:439) and the cull, submit
`(m->model, m->boneMatrices, viewMatrix, projDist, geom offset, hiddenGroups, texInfo, uv offsets, clipY)` per
actor instead of lines 459-485, and take the colour and blend state after `fld_applyLights` / `fld_applyModelCommands`
(same frame). Mirror copies are a second `ModelObj` with a reflected matrix set; submit them too.

### 2.3 World map (ovl_0c)

| step | where | what |
|---|---|---|
| camera | `wm_updateCamera` (`src/ovl_0c/w_movement.c:40`) → `wm_lookAt(g_worldViewMatrix, eyeRel, lookAt, up)` (:74, :499). `g_worldViewMatrix = &gd->viewMatrix` (`w_frame.c:50`); the eye is **relative to the camera target**; `gd->projDist = g_worldCameraParams.projDist` (:50; 0xDC at init, then the presets) | per logic frame (20 Hz) |
| screen offset | (160, 112), `wm_initCamera` (w_movement.c:9) | constant |
| actors | `wm_drawActors` (w_movement.c:721) → per visible actor `wm_setActorFog` (:789) and `wm_drawActorModel` (:842) | |
| model matrix | w_movement.c:857-866: `math_rotMatrixYXZ(rotX, rotY, rotZ)` (note **YXZ**), t = the actor position **relative to the camera target with the planet curvature applied to y**: `wm_worldToView` (`tu_800BA528.c:438`): `s = 4096 − (vz/16 + vz/64)` from the view-space z, `y' = (y·s >> 12) − target.y`, x/z wrapped around the 0x60000 × 0x50000 world | |
| bones | `wm_calcBoneMatrices(m, animFrame)` (C, `wm_model.c:735`; the field algorithm; bones in scratchpad when < 31) → `ModelObj.boneMatrices` | |
| faces | w_movement.c:880-893: `wm_projectMeshClipY`/`wm_drawMeshClipY` (vehicles in terrain: `clipY = y − rideOffset + 100`) or `wm_projectMesh` + `wm_drawMesh` (**asm** `wm_model_asm.c:3`; C twin `src_equiv/ovl_0c/model_link.c`; matching twin `wm_drawMeshC` `wm_model.c:400`) | |
| shadow | `wm_drawActorShadow` (w_movement.c:903) | |

The model vertices themselves are **not** curved, only the model's origin. `SetFarColor` stays at the actor's fog
colour during its draw and is reset to 0 at the end of the pass (:786).

## 3. A party member's model outside battle and field (Equip view)

### 3.1 Which files
- **Appearance kind**: `Character.info.b.kind` (bits 4-8 of `info`, `include/ff9.h:99-111`), recomputed by
  `chr_calcAppearance(id, (classFlags >> 4) & 1, equip)` (`src/main/chr.c:152`) whenever equipment changes, including
  in the Equip screen itself (`menuEquip_applyEquip`, `src/ovl_33/menu_equip.c:752-756`). So the preview can follow the
  equipped weapon by reading `kind` again after each change. Rules (`g_chrAppearanceTypes` main 0x8006CAA4, weapon kind
  = `g_itemTable[equip[0]].icon`, story = `g_evtGlobalVars.h[0]`):

| kind | who | rule | battle model | trance model | weaponBone | idle motion (slot 0) | weak idle (slot 1) |
|---|---|---|---|---|---|---|---|
| 0 | Zidane | weapon icon 1 (daggers) | 5414 | 581 | 0x0D | 0x03007D | 0x03006D |
| 1 | Zidane | other weapons (thief swords) | 285 | 580 | 0x0D | 0x0335F8 | 0x0335D8 |
| 2 | Vivi | always | 5415 | 584 | 0x10 | 0x030BDC | 0x030BB6 |
| 3 | Garnet | story < 10300, not icon 7 | 281 | 583 | 0x0F | 0x0317D5 | 0x0317B7 |
| 4 | Garnet | story < 10300, icon 7 (rackets) | 283 | 602 | 0x0F | 0x032CE2 | 0x032CCA |
| 5 | Garnet | story ≥ 10300, not icon 7 | 288 | 582 | 0x0F | 0x032BA2 | 0x032B82 |
| 6 | Garnet | story ≥ 10300, icon 7 | 287 | 601 | 0x0F | 0x032D72 | 0x032D50 |
| 7 | Steiner | always | 655 | 606 | 0x10 | 0x030CE1 | 0x030CDF |
| 8 | (Steiner, unreachable through `chr_calcAppearance`) | – | 286 | 606 | 0x10 | 0x030CE1 | 0x030CDF |
| 9 | Quina | not guest | 289 | 607 | 0x0E | 0x032996 | 0x032974 |
| 10 | Eiko | not icon 7 (flutes) | 284 | 645 | 0x0F | 0x0331ED | 0x0331CF |
| 11 | Eiko | icon 7 (rackets) | 291 | 644 | 0x0F | 0x0337F4 | 0x0337D4 |
| 12 | Freya | always | 290 | 585 | 0x06 | 0x031D19 | 0x031CF9 |
| 13 | Amarant | not guest | 444 | 648 | 0x10 | 0x0337AE | 0x03378E |
| 14 | Cinna | slot 5 as guest | 39 | – | 0x19 | 0x0313DA | 0x030928 |
| 15 | Marcus | slot 6 as guest | 45 | – | 0x06 | 0x0315A8 | 0x031586 |
| 16 | Blank | slot 7 as guest | 42 | – | 0x0E | 0x03155A | 0x031534 |
| 17 | Blank | slot 7 as guest, story 1500-1599 | 608 | – | 0x0E | 0x032F3E | 0x032F1C |
| 18 | Beatrix | always | 358 | – | 0x10 | 0x0332A9 | 0x033295 |

  Sources: `g_btlCharModelIds` (ovl_0b .data 0x800F5200; index kind, trance at kind + 0x13; `btl_initParty`
  `src/ovl_0b/btl_init.c:17`, `btl_getPartyModelId` `btl_main.c:1316`), `g_btlPartyMotionIds[19][0x22]` (ovl_0b
  0x800F524C, `btl_loadPartyMotions` `src/ovl_0b/btl_attr.c:55`; key = 0x030000 | value), `Character.weaponBone`
  (`btl_initWeapon` `src/ovl_0b/btl_init.c:289-341`). Guests (classFlags 0x10) have no trance model (0). The racket
  forms (4, 6, 11) repeat the idle in slots 21-25 (fewer motions).
- **Battle body file**: FF9.IMG **dir 10**, the entry whose file id & 0xFFFF = the model id (`file_find(10, id)`, main
  `src/main/file.c:292`; the directory tables stay resident in every mode). The file id's high half = `split` = sectors
  of item 0. File = DB container, block 0x1B: item 0 (sectors 0..split) is a nested container with the header
  `0x02<id>`, **34 motions** `0x03xxxx` (all of that kind's `g_btlPartyMotionIds`), the model info `0x120000` (one
  entry, 2 textures) and texture animations `0x19xxxx` (most models). Item 1 (from sector `split`) holds the TIMs
  `0x040000 + n`. Checked on disc 1 for all 19 bodies and Zidane's trance model 581.
- **Weapon**: `g_weaponTable[Character.equip[0]].modelId` (main 0x8006E0E8, 0xC-byte records, u16 at +2; `Unk8006E0E8`
  `include/ovl_0b.h:2782-2789`), key `0x020000 | id`. File = FF9.IMG **dir 8**, file id = model id
  (`btl_loadWeaponModelsStep` `btl_main.c:828`): the first 0x1000 bytes are a DB container whose `DB_PACK` holds the
  header `0x02<id>`, the model info `0x12`, an event list `0x14` (its sound bank) and two `0x08` items; the rest of
  the file is the TIM container. All 85 weapon models have **1 bone and 1 group**, no motion. Weapons 0x55-0x57 have
  model 0 (none).
- **Field model alternative** (no separate weapon, 5 motions): the dir-10 ids of docs/field-models.md section 4
  (98 Zidane, 8 Vivi, 185/76 Garnet, 5489 Steiner, 192 Freya, 273 Quina, 443 Eiko, 509 Amarant, 204 Beatrix). The default
  motion = model info entry 0's `animId`: 98 → 0x030019, 8 → 0x030094, 185 → 0x030824, 76 → 0x03082B,
  5489 → 0x0307C2, 192 → 0x0309F7, 273 → 0x030C9B, 443 → 0x031D4F, 509 → 0x032073, 204 → 0x030B9E. In the field
  these files are also held, unrelocated, in `g_fieldModelCache` below 0x801EDF00.

### 3.2 Why not call the game's loaders
`btl_loadPartyModelsStep` / `btl_loadWeaponModelsStep` / `btl_uploadModelTextures` use `Battle.work` buffers, the
battle VRAM layout and `db_register`. `btl_registerModel` and `btl_initWeapon` need a `BtlUnit` and `ModelSlot`, and
`btl_calcBoneMatrices` writes into the scene primitive buffer. `fld_setupActorModel` (`fld_movie.c:148`) needs an
`EvtObject`, the field heap and `FieldScene`. None of these is safe while the menu is up (the menu overlay sits at
0x801EDF00). Reimplement instead. The data is self-contained.

### 3.3 Port recipe (Equip view)
1. `kind = Character.info.b.kind`; `body = g_btlCharModelIds[kind]` (or the table above, or `+0x13` for a
   Trance preview); `weapon = g_weaponTable[equip[0]].modelId`; `bone = weaponBone[kind]` (table above; the
   game stores it in `Character.weaponBone` at battle init, so it may be stale outside battle; use the table).
2. Read dir-10 file `body` and dir-8 file `weapon` (sectors from `file_find`). Walk the DB containers
   (`tools/encounter_catalog.py` `db_items`/`db_find` show the format) to the header `0x02<id>`, the motion
   `0x030000 | g_btlPartyMotionIds[kind][0]`, the model info `0x12` and the TIMs `0x04`.
3. Treat the header offsets as relative to the header (`bones`, `meshes`, per group the five arrays) and the motion's
   `rotKeys`/`rotKeysLo` and table offsets as relative to the motion's start. No other pointer exists.
4. Textures: decode each TIM (8-bit, own 256-colour CLUT). Battle bodies use the file uvs as TIM-local texel
   coordinates. For a weapon, use the rule of 1.2 (TIM-local v = file v − 0x50) or emulate the placement: a 256×256
   page with the TIM at row 0xD0 and dv 0x80. Optionally run the 0x19 texture animations (blinking) with
   `btl_updateTexAnims`'s rules.
5. Pose frame N (`0 ≤ N < frameCount`, loop): `bones = battle_pose.bone_matrices(motion, bonedefs, N, base)` with
   `base = (math_rotMatrixZYX(0, yaw, 0), (0, 0, 0))` for a turntable. In C, the port already has `src_equiv/ovl_0b/bones.c`
   (or `fldmdl_calcBoneMatrices`, same arithmetic) and `math_rotMatrixZYX_gte`. The game shows 15 frames per second in
   battle (one motion frame per scene).
6. Weapon: its single matrix = `bones[bone]` (attach offset 0: `btl_applyModelTextures` clears `attachX/Y/Z`,
   `btl_mesh.c:536`, and nothing sets it for weapons). All its vertices are on bone 0.
7. Draw every group of both (battle party models start with no hidden groups: `hiddenMeshes = 0`,
   `btl_resetUnit` `btl_init.c:281`, `btl_applyModelTextures` `btl_mesh.c:539`). Colour: 0x80 (neutral) or a fixed
   light of your choice. In battle the units take the arena light (Q4), usually darker than 0x80.

## 4. Lighting and colour

### 4.0 Common facts
- No code path runs NCDS/NCCS/NCS/NCT on a character, NPC, enemy, weapon or effect model. A grep of all C and asm
  twins: GTE light/colour matrices are set only by the world-map terrain, backdrop and block effects (ovl_0c), and by
  one battle effect helper (`btlfx.c:148`, effect sprites). `DpqColorFar` in `btlfx.c` is for effect primitives only.
- Every model polygon is **flat**: one rgb per polygon (FT4/FT3), copied from the source record (always 0x808080 on
  the discs) and then overwritten **per model or per group** by the mode's colour code. Textured result =
  `clamp(texel × rgb / 128)` per channel (0x80 = 1.0, up to ~2.0), with GPU dithering on (`dtd = 1`,
  `src/main/db.c:278-279`).
- The GTE depth cue is never applied to model vertices. The world map fogs actor models through their CLUTs (4.3).

### 4.1 Battle
- **Base colour = the arena light**: `btl_applyBgLight` (`src/ovl_0b/btl_status.c:1234`) → `btl_setMeshRgb`
  (`btl_model.c:17`) writes `BtlBgHeader.lightR/G/B` (the battle background header, `include/ovl_0b.h:2453-2455`;
  `btlbg_getHeader()`) into **every FT4/FT3 source record** of every group (other lists untouched). Runs at weapon init,
  script attribute 0x35 in the intro, and `btl_reapplyStatusLook` (end of Trance, effect restores).
- **Intro / escape fades**: `btl_setUnitBrightness(u, level)` (`btl_status.c:1056`), level 0..0x1F step 4 per scene:
  source rgb = `light × level / 32`, plus abr 3 on the packets while level < 0x20. Units start black (`btl_resetUnit`).
  Shadow shade = `shadowShade × level / 32`.
- **Per-scene overrides on the built packets** (after the unit is drawn; docs/r29/battle-render.md item 1 has every
  caller). `btl_updateUnitTint` (`btl_status.c:395`) runs per scene per unit, nothing for petrified or hidden units:
  - Zombie/Berserk/Heat/Freeze: `btl_fillModelRgb(model, tint)` = **replace** every packet colour by `u->tint`
    (from `btl_updateStatusColor`), body and weapon.
  - Shell/Protect: **add** `(c × v) >> 3` per channel (`btl_setModelRgb` `btl_mesh.c:671`, add with clamp 0..255),
    c = light + (−0x40, +0x18, +0x48) (Shell: blue) or light + (+0x28, +0x28, −0x50) (Protect: yellow; both: alternating
    every 24 ticks), v = triangle 0..8..0 over 24 ticks (`tick % 24`: 0-7 ramp, 8-15 hold 8, 16-23 down).
  - Trance (not KO): add `(c × v) >> 3`, c = light + (tbl[slot] − 0x80), v = triangle over 16 ticks (0..8..0),
    tbl = {FF,60,60} {68,78,FF} {FF,B8,78} {80,C0,D0} {C0,68,90} {D0,70,78} {C8,D8,68} {D0,B8,68} by party slot
    (`btl_status.c:442-445`).
  - KO and sequence fades: `btl_setUnitFade` (`btl_cmdfx.c:17`) → fill + abr 3. Effects: `btlfx_fadeUnit`,
    `btlfx_setUnitBrightness`, `btlfx_setUnitLight` (fill/add/abr 1). `btl_setModelAbr` (`btl_model.c:55`) sets the
    semi-transparency bit and abr on every packet.
  - Float bobs `pos.y` by `−0xC8 − sin(tick & 0xF) × 480 / 65536` (end of `btl_updateUnitTint`), and Confuse turns
    `rot.y`. These are geometry, not colour.
- **Petrify**: CLUT swap to grey copies (`btl_setStoneClut` / `btl_makeGreyCluts`), no colour write.
- A native renderer's battle shader is therefore: `colour = clamp(modelBase + scene add) or sceneFill`, one value per
  model per scene (the same for every group), times the texel / 128, then blend by abr if the scene set it.

### 4.2 Field
- `fld_applyLights(list, n)` (`src/ovl_0a/tu_800B0124.c:1014`), once per frame after the draw, from the field's light
  item `0x1F0000 | bgKey` (`fld_main.c:707`, `FieldScene.lights`, `Unk800B3254` `include/ovl_0a.h:547-555`):
  `{+6 colorOffset, +7 lightCount, +9 colorCount, +0xC FieldLight lights[lightCount]}`. Then, at
  `lights[colorOffset]`, `colorCount` model entries `Unk800B3294` `{u16 modelId (0xFFFF = default), s8 shadowShade,
  s8 shadowSize, u32 color}`. `FieldLight` (12 bytes) = `{s8 type (0 floor, 1 default/ambient, 2 directional),
  pad, s8 shadowShade, s8 shadowSize, u32 color, u32 floors (4 walkmesh floor ids)}`.
- Per actor, recomputed only when its walkmesh floor or `FLD_OBJ_DIR_LIGHT` changes:
  `a` = the model's entry colour (else the 0xFFFF default), `b` = the directional light if the actor has
  `FLD_OBJ_DIR_LIGHT` (slot flag 4), else the floor light for its current floor (`fld_findFloorLight` :1193), else
  the type-1 light, else 0. Then per channel **`c = ((a_c + b_c) × 8) & 0xFF`** (light values are in 1/8 units:
  a + b = 16 gives the neutral 0x80, and a + b ≥ 32 wraps). Script op 0x8F (`EVT_OP_OBJ_COLOR`,
  `fld_setObjectColor` → `fld_setSlotColor` :1122) sets the colour directly in packet units and marks
  `FLD_OBJ_COLOR_SET`, which keeps the lights from overriding it. Shadow shade = (sum of shadowShade) × 8, size from shadowSize.
- The colour goes through `fldmdl_setGroupRgb` (`tu_800B61A4.c:1279`) into the first colour word of **every packet of
  every group** of the current draw buffer, and one frame later into the other buffer (`FieldActorInfo.color` /
  `nextColor`). It is one colour per actor. Without a light item (`lights == NULL` or no colour entries) the packets
  keep the source 0x808080.
- Also on packets: `fldmdl_setSemiTrans` / `fldmdl_setAbr` (`fld_applyModelCommands`, op 0x88), and the mirror copy's
  tint (`fld_modelCommand` 11, `fld_movie.c:732`).

### 4.3 World map
- Actor models keep the source colour (0x808080). `wm_buildPackets` copies it, and nothing in ovl_0c writes a
  model packet's colour afterwards.
- **Fog = palette replacement**: `wm_setActorFog(actor, viewZ >> 3)` (`src/ovl_0c/w_movement.c:789`): with
  `t = clamp(((−pos.y − 0xC1F) << 12) / 2176, 0, 4096)` (altitude), `fogOffset` and `colour` lerped between
  `work[0].fog[1]` and `fog[0]`, `SetFarColor(colour)`, `f = clamp((viewZ + fogOffset − 0x8000) >> 2, 0, 0xFFF)`.
  Then for every `WmPalAnim` entry (item 0x1D0000 of the world file, `include/ovl_0c.h:140-147`: modelId, CLUT x/y,
  16 source colours, 2 work copies) whose modelId = the actor's: `wm_depthCueClut(src, work[buf], f, fxBackColor)`
  (C twin `src_equiv/ovl_0c/wm_misc.c:48`, DPCL per colour: `RGB = c·k + f·(FC − c·k)` with k = fxBackColor / 4096 and f / 4096, the GTE's
  4.12 maths, colour 0 copied, bit 15 kept), then `LoadImage` to the CLUT. Models without a palette entry get no
  fog at all. `wm_setMountPalette` (:603) also rewrites the source palettes of the chocobo models by chocobo
  colour.
- The terrain blocks use LLM/LCM/BK + NCT + DPCS: docs/r29/field-world.md section 7.

## Proposed names (not applied)

| symbol | proposed | evidence |
|---|---|---|
| ovl_0b `BtlUnit9C` | `BtlModel` | the model object of a unit, weapon or effect (header, motion, bones, matrix, texPages) |
| ovl_0b `BtlUnit9C8` | `BtlModelHeader` (= ovl_0a/0c `WmModel`) | DB item 0x02xxxx, docs/battle-models.md |
| ovl_0b `BtlUnit9CC` | `BtlMotion` (= `WmAnim`) | DB item 0x03xxxx, `btl_calcBoneMatrices` |
| ovl_0b `BtlUnit9C18` | `BtlTexWord` (= `WmTexInfo`) | `{tpage, clut}` per texture index |
| ovl_0b `Unk800AA504Pos` / `Unk800AA504Pos8` | `ModelTexWord` / `ModelTexPos` | the model info's first and third tables, written by `btl_uploadModelTextures` |
| ovl_0b `Unk8006E0E8`, main `WeaponEntry` | one `WeaponEntry {u8 flags, pad, u16 modelId, pad, u8 attack, ...}` | two views of `g_weaponTable`; `btl_initWeapon` reads `modelId` (+2), `chr_calcEquipStats` `attack` (+5) |
| ovl_0a `Unk800B3254` | `FieldLightSet` | `fld_applyLights`, `fld_findGlobalLights`, `fld_findFloorLight`, item 0x1F0000 |
| ovl_0a `Unk800B3294` | `FieldModelLight` | per-model colour entry, `fld_findLightColor` |
| ovl_0b `g_btlCharModelIds` comment | add "19 kinds, trance at + 0x13 (0 for guests); [0x26] = 125 is read for a non-party unit's trance (`btl_cmd.c:672`)" | table dump above |

## Runtime questions (need the emulator)
1. Battle intro: is the final body colour the full `lightR/G/B` (a later `btl_applyBgLight`) or the fade's 7/8
   (`light × 0x1C / 32`)? (R29 left this open.) Read a party FT4 source rgb after the intro.
2. Garnet's kinds 3/4 against 5/6: which is long and which is short hair, and is story 10300 the haircut? (The
   `disc3_malboro_*` states show Dagger as model 283 = kind 4.)
3. Kind 8 (model 286, Steiner's 655 twin by size) is never returned by `chr_calcAppearance`. Does any save or script
   put 8 into `info.kind`?
4. Field: is the dir-10 model's default `animId` (e.g. 0x030019 for Zidane) the standing idle the field shows (the
   `field_dali_inn` check saw Zidane on 0x0300C8)? This only matters if the Equip view uses field models.
5. Weapon attach: confirm `weaponModel->attachX/Y/Z` is 0 at run time and the weapon's bone matrix equals the body's
   `bones[weaponBone]` (expected from the code).
6. Weapon TIM placement: confirm a weapon TIM lands at VRAM y 0x1D0 and its uvs + 0x80 fall inside it (the TIM-local
   v = file v − 0x50 rule).
