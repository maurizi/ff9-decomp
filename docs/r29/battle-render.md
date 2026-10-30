# Battle rendering facts (ff9-ds R29, items 1, 2, 4 and 9)

Answer to ff9-ds request R29, items 1, 2, 4 and 9. It builds on docs/battle-models.md (R26/R27: the model format,
`BtlMesh`, the consumers table) and docs/battle-backgrounds.md (the arena and `btlbg_draw`). Items 3, 5-8 are not
covered here. Sources are ovl_0b unless noted. `file:line` refers to this tree (`src/ovl_0b/...`). The hand-written asm
functions are cited by their behaviourally equivalent C in `src_equiv/ovl_0b/` (checked against the original with
`tools/equiv.py`).

Run-time checks were done in PCSX-Redux on disc 3 (`disc3_malboro_lv99_calm`, `disc3_malboro_eiko_trance`,
`disc3_malboro_trance_pre`) with `tools/emu.py trace`. They are marked **[runtime]**. Everything else comes from the
code.

## Summary
1. **Immutability.** After `btl_applyModelTextures`, nothing touches a unit model's `prims` records (vertex/uv
   indices, texture index, flags), its `uvs`, `colors`, `vertexCounts` or `vertices`, with three exceptions. (a)
   `btl_setMeshRgb` rewrites the **flat rgb** of the FT4/FT3 source records. That happens at unit init (black),
   during the intro and escape fades (every scene while the fade runs), and in `btl_applyBgLight`, which runs once
   per event (weapon init, the end of a Trance animation, effect "restore looks", script attribute 0x35). (b)
   Effect fx_17f moves the vertices of model 14 (Zombie) every frame (R26). (c) A party model's **whole file is
   replaced in place**: Trance on/off and summon reloads read a new file into the same `partyBufs[k]` buffer and
   relocate it again. `btlfx_splitMeshVertices` only runs on effect models. Every per-frame colour or
   translucency change (Trance glow, Shell/Protect pulse, status tints, effect fades, KO fades) is written into the
   **built GPU packets** of the current scene. The packets are rebuilt from the source every scene, so those
   changes last one scene and are re-applied each scene. No code changes a polygon's texture index. The tpage
   **abr bits** change in built packets only (`btl_setModelAbr`). The CLUT field changes through
   `BtlUnit9C.texPages[].clut`, only when Petrify is added or removed (`btl_setStoneClut`). Texture animations
   rewrite VRAM (`MoveImage`), not texture words. One party model (kind 0xE, model 39) has 3 double-sided polygons.
   No enemy or weapon model has any.
2. **OT.** Model polygons use the scene OT `gd->ot[drawBuf]`, which `ClearOTagR` sets up with entries -8..0x1007
   and `DrawOTag` walks from 0xFFF down to -8 (larger index = farther = drawn first). Each polygon goes to entry
   `(SZ0+SZ1+SZ2+SZ3) >> 6` (quads) or `AVSZ3_OTZ >> 2` with ZSF3 = 0x155 (triangles), both about mean SZ / 16,
   minus the group's `otBias` (s8, -4..+4 on the discs). There is **no clamp and no z = 0 test**. Every link is a
   head insert (LIFO): within one entry, a packet linked later is drawn **earlier** (further back). Link order per
   scene: units in `unitList` order (body groups, weapon groups, shadow), then `btlbg_draw`'s layer-0 records,
   effects, popups, banner. The other background layers go to a separate 256-entry OT that is drawn whole before
   the scene OT. Effects use the same depth scale as models (`(SZ>>2 - otZ) >> 2`). Because they are linked after
   the units, an effect packet in the same entry as a model polygon is drawn **behind** it. Shadows use
   `maxSZ >> 3`, twice the model scale, so they never share an entry with their unit. Nothing in the code needs a
   model polygon and an effect or shadow packet in the same entry. Effects do rely on in-entry order for their
   **own** draw-mode packets (linked after the primitive, so drawn before it).
3. **Frame structure.** The fields asked about (formerly `unk842`/`unk8C4`) are `GameData.musicFadeFrames` /
   `musicFadeStart`, the music fade, not frame pacing. Pacing is `GameState.vsyncMode` (4 in battle,
   `btl_initMode`), applied by `btlui_waitFrame`: one scene every 4 VBlanks (15 Hz) **[runtime: 60 scenes in 240
   VBlanks]**. Effect scripts can change it (`btlfx_opSetFrameRate`: vsyncMode = 60 / fps). The battle UI draws
   every VBlank from the VSync callback. Every non-hidden unit recomputes all its bone matrices every scene. The
   motion frame stays put under Petrify/Venom/Stop/Freeze or flag bit 21 ("motion held"), but the base matrix is
   rebuilt from `rot`/`pos` each scene (Float bobs `pos[1]`, Confuse turns `rot[1]`), so the matrices repeat only
   when all inputs repeat. Hidden units (flag bit 22) are skipped entirely. `btl_updatePause` replays nothing: once,
   when the pause starts, it draws a dimming tile and the PAUSE sprite (or "Disc Cover Open") over the last image,
   and then the loop stops drawing and flipping.
4. **Bone/vertex layout.** Yes: each mesh group's vertices are stored bone by bone, `vertexCounts[b]` of them for
   bone b, in bone order. `btl_projectMesh` depends on that. **31.8%** of all polygons (65,259 of 205,291 over 309
   models) have vertices on more than one bone: enemies 32.4% (median model 33.7%, range 0-79.4%), party 36.9%,
   weapons 0%. Of the two-bone polygons, 64% join a parent and its child. Details are in item 9 below.

## 1. Is battle model data immutable after init?

### Source data: what writes it after `btl_applyModelTextures`
`btl_applyModelTextures` (btl_mesh.c:508) relocates once under `h->flags & 1` and adds the uv offsets once. After
that, the writers are:

| what | writer | when | evidence |
|---|---|---|---|
| FT4/FT3 `rgb` (3 bytes; the 4th byte is kept: FT4's `tex`, FT3's pad) | `btl_setMeshRgb` (btl_model.c:17) | see the next table | `p[4] = (p[4] & 0xFF000000) \| v` (FT4 +0x10), `p[2]` (FT3 +8). GT/G/F lists are not visited |
| same | `btl_addMeshRgb` (btl_model.c:3) | never: no callers | |
| `vertices` of model 14 (Zombie) | fx_17f via `btl_forEachModelVertex` + `btlfx_swapModelData` | every frame of effect 0x17F, then restored | docs/battle-models.md "fx_17f"; export scan: the only module calling either export |
| an effect model's `prims` vertex indices, `vertices` pointer, `vertexCounts[group]` | `btlfx_splitMeshVertices` (hi_vertex.c:125) via `GeoSplitVertices` / `Hi_SplitMdlVertex` | when an effect module calls it (11 modules: fx_026 034 096 0ec 179 197 1a2 1b0 1b3 1b6 1b7). The module then writes the split vertex buffer itself | `Hi_SplitMdlVertex` takes `btlfx_getEffModel(...)`, so effect models only, never units |
| a party member's **whole model file**, same address | `btl_swapPartyModelStep` (btl_main.c:1346; Trance on/off: `BTL_CMD_TRANCE` picks `g_btlCharModelIds[kind + 0x13]`, btl_cmd.c:669-675; called through `func_800AB3E4` from the effect script, btlfx_script.c:140/343) and `btl_reloadPartyStep` (`btl_reloadParty`, btlfx_script.c:136/324: party models evicted by a big effect and read back) | Trance start/end, after summons that evict the party | case 1 reuses `partyBufs[k]` and `db_unregister`s the old file. Case 6 calls `btl_applyModelTextures` on the new header (flag bit 0 still clear: a fresh file) |
| `BtlUnit9C.texPages[].clut` (the model-info table, not the mesh) | `btl_setStoneClut` (btl_status.c:517) | Petrify added (btl_status.c:103, btl_cmd.c:707), removed (btl_status.c:223), `btl_reapplyStatusLook` (btl_status.c:731) | moves the CLUT y by ±6 rows (enemies) / ±12 (party) to grey copies |
| motion `rotKeys`/`rotKeysLo` (relocated once when still offsets) | `btl_updateUnitModel` (btl_unit.c:84-89) and the motion setters | first use of each motion | not mesh data |

Who calls `btl_setMeshRgb` (all write the background light, scaled, into every group of the unit's body and weapon):

| caller | when | frequency |
|---|---|---|
| `btl_resetUnit` (btl_init.c:278), `btl_initPartyUnit` (btl_init.c:152, weapon) | unit (re)initialisation: rgb = 0 (black, ready for the fade-in) | once |
| `btl_setUnitBrightness` (btl_status.c:1056) from `btl_updateIntro` (btl_update.c:332/334) | intro fade-in, level 0, 4, ..., 0x1C (`partyBrightness`/`enemyBrightness` += 4 per scene) | **every scene while level < 0x20**, then it returns without writing. By the code, the last value written is light × 0x1C / 32 (7/8), which stays until `btl_applyBgLight` writes full light (not checked at run time) |
| `btl_setUnitBrightness` from `btl_victory` `BTL_END_ESCAPE` (btl_update.c:191) | escape fade-out of fleeing party members | every scene while `fleeBrightness` < 0x20 |
| `btl_applyBgLight` (btl_status.c:1234) | `btl_initWeapon` (btl_init.c:299); script attribute 0x35 in the intro (btl_attr.c:463); `btl_reapplyStatusLook` (end of the Trance command animation, btl_cmd.c:833; `btlfx_restorePartyLooks` / `btlfx_restoreUnitLook`, btlfx_op.c:1796ff.) | once per event. **[runtime]** In `disc3_malboro_eiko_trance`: 2 calls (body + weapon) at the end of Eiko's Trance animation, called from `btl_updatePartyCmd`, rgb (0x70, 0x70, ...) |

So, for the port's per-mesh caches: the uv/vertex/index/texture/flags content of a unit model only changes when
its file is replaced (Trance swap, party reload: same buffer address, new content, so key a cache on a load
generation, not on the pointer) or, for model 14, during fx_17f. The FT4/FT3 flat colours change at init, during
the intro and escape fades, and on the `btl_applyBgLight` events. They don't affect texture bounds.

### Built packets: the per-scene colour and translucency changes
`btl_updateUnitModel` (btl_unit.c:30) sets `model->prims = 0` (:74). `btl_drawMesh` then records the first packet
of the scene there, and each group's packets end with a 0xFF terminator (src_equiv/ovl_0b/mesh_packets.c). The
following functions walk **these packets** (only for models drawn this scene: `(model->flags & 0x30) == 0x20`, or
`!(flags & 0x10)`) and change only what they are told to:

| function | changes in each packet | callers and frequency |
|---|---|---|
| `btl_setModelRgb` (btl_mesh.c:671) | colour word(s) += (r, g, b), clamped | `btl_updateUnitTint` (btl_status.c:395): **every scene** while Shell/Protect (pulse, 24-tick period) or Trance (glow, 16-tick period, colour per party slot) is on (body + weapon). **[runtime]** Eiko in Trance: 2 calls per scene (30 scenes / 120 VBlanks: 60 calls). Also `btlfx_setUnitBrightness` / `btlfx_setUnitLight` (effects, below), `Hi_ModifyEffModelRGB` / `Hi_ModifySummonModelRGB` (effect/summon models; **[runtime]** 116 calls in 400 VBlanks of Eiko's trance effect) |
| `btl_fillModelRgb` (btl_mesh.c:747) | colour word(s) = the colour | `btl_updateUnitTint`: **every scene** under Zombie/Berserk/Heat/Freeze (`tint[]` from `btl_updateStatusColor`); `btl_setUnitFade` (btl_cmdfx.c:16): KO fade-out (btl_unit.c:315, each scene of `koState` 4) and sequence fades (btlseq.c:169, each scene while `fadeFrames`) |
| `btl_setModelAbr` (btl_model.c:55) | sets the semi-transparency bit and replaces the tpage abr bits (0x60 mask); for an E1 packet (`btl_drawMeshOffset`'s Gouraud) the mode word; G4/G3 just the bit | abr 0x60 (mode 3, B + F/4): `btl_setUnitBrightness` (intro/escape fades, every scene while < 0x20), `btl_setUnitFade` (KO and sequence fades). abr 0x20 (mode 1, B + F): `btlfx_setUnitBrightness` when the level is < 0x46 (`btlfx_fadeUnitById`, 23 modules). `Hi_Modify{Eff,Summon}ModelAbr` (effect models, 77 + 3 modules) |

`btl_updateUnitTint` runs every scene for every non-hidden unit: from `btl_updateUnitStatus` in combat
(btl_status.c:322), `btl_updateUnits` (btl_update.c:120), `btl_victory` (:206) and `btl_updateIntro` (:336). It
does nothing for a petrified unit and nothing when no tint status is on (`goto end`). It always runs after the
unit's `btl_updateUnitModel` of the same scene, and the effect-side writers run in `btlfx_runFrame`
(btl_cmd.c:59), also after all units. So a port that keeps its own vertex/colour data has to apply these as
**per-scene overrides of the colour** (add-clamped or replaced) and of semi-transparency + abr. The source
packets come back unchanged next scene.

**[runtime]** `disc3_malboro_lv99_calm` (no statuses): over 240 VBlanks, 0 calls to `btl_setMeshRgb`,
`btl_setModelAbr`, `btl_fillModelRgb`, `btl_setModelRgb`, `btl_setStoneClut`, `btl_makeEnemyGreyCluts`. A
quiet battle never recolours.

Effect modules and unit models (export scan of the 380 built modules, `build/us/fx_*.elf`, with the dataflow of
`tools/fxmod_void_returns.py`): `btlfx_fadeUnitById` 23 modules (built packets, or `hiddenMeshes` 0xFFFF on/off for
-1/-2), `btlfx_setUnitLight` 1 (fx_0fb_1), `btlfx_setUnitVisible` 2 (flag bit 22), `btl_forEachModelVertex` +
`btlfx_swapModelData` 1 (fx_17f), `btlfx_holdMotion` 3 (fx_105, fx_180, fx_1ec) and `btlfx_setMotionHeld` 1
(fx_105) (flag bit 21, item 4). No module calls `btlfx_setUnitTexAnim`, `btl_setVsyncMode` or
`Hi_DrawMorphModelByBone`. Effect *models* are recoloured through `Hi_ModifyEffModelRGB` (134 modules),
`Hi_ModifyEffModelAbr` (77), morphed by `Hi_DrawMorphEffModel` (32: `btlfx_projectMorphMesh` blends into the
screen arrays and writes no mesh data), sliced by `Hi_DrawSliceEffModel` (19), split by `Hi_SplitMdlVertex` (11).

### Texture words, CLUTs and texture animations
- **Texture index** (`tex` byte of a source record): never written after load (the `btl_setMeshRgb` masks keep it).
  In packets it becomes the clut half of uv word 0 and the tpage half of uv word 1 (`tex[...]`, mesh_packets.c);
  only the abr bits of that tpage change afterwards (`btl_setModelAbr`).
- **`texPages[].clut`**: only `btl_setStoneClut` (Petrify on/off/reapply, above). Not per frame.
- **Grey CLUTs** (VRAM, not words): `btl_saveEnemyCluts` (btl_status.c:713) stores CLUT rows 0xE0-0xE5 (x 0x100,
  w 0x100) and `btl_makeEnemyGreyCluts` (btl_status.c:570) writes their grey version to rows 0xDA-0xDF. Both run
  once in the intro (btl_update.c:320/350, guarded by `loadFlags`). The party's equivalents are
  `btl_savePartyCluts` (rows 0xE6-0xF1) and `btl_makePartyGreyCluts`. `btl_makeGreyCluts(u)` (btl_status.c:537) runs
  when a party member is petrified (btl_cmd.c:704, btl_status.c:99): StoreImage, grey, LoadImage 12 rows lower,
  with `DrawSync`. None of these runs per frame.
- **Texture animations** (`btl_updateTexAnims`, btl_model.c:116): `MoveImage` of VRAM rectangles. A frame
  animation copies only when its frame changes (`lastFrame`). A scroll animation (flags & 4) runs two `MoveImage`s
  every call. Units call it from `btl_updateUnitModel` (btl_unit.c:106, every scene unless Jump), the arena from
  `btlbg_draw`. They change texel contents, never a packet's tpage/CLUT words. A port that caches textures by
  VRAM rectangle has to watch these rectangles. Started/stopped by `btl_resetUnit`, script attributes
  (btl_attr.c:40, 498-510), `btl_removeStatus` (KO motions, btl_status.c:247) and `btlfx_setUnitTexAnim`
  (btlfx_op.c, no module callers).

### Double-sided polygons (no NCLIP)
Flag bit 1 (`flags & 2`) skips the NCLIP test in `btl_drawMesh`. Counted over every model file on the four discs
(scan below): enemies (182 models, dir 7) **0**, weapons (85, dir 8) **0**, party bodies (42 models, dir 10)
**3 polygons**, all in model **39** (`g_btlCharModelIds[0xE]`, 340 polygons). Everything else is culled with NCLIP
<= 0 on corners 0, 1, 2. Background records have no such flag (always culled, docs/battle-backgrounds.md). Effect
models inside the effect files (dir 13) were not counted: their chunks are not DB containers that the model scan
can walk.

## 2. Battle OT rules

### The OTs of a battle scene
| OT | entries | cleared / linked | drawn |
|---|---|---|---|
| scene OT `gd->ot[drawBuf]` | -8 .. 0x1007 (`ClearOTagR(ot - 8, 0x1010)`, btl_main.c:2130) | model polygons, shadows, layer-0 background, effects, popups (ot[4]), active marker (ot[2]); at ot[0] in `btl_beginFrame`: an STP(0,0) packet, then the fade OT, then the banner OT; at ot[0xFFF] in `btl_endFrame`: STP(1,0) | `DrawOTag(&ot[0xFFF])` (btl_main.c:2163): entries 0xFFF down to -8. Entries 0x1000-0x1007 are never drawn |
| background OT `work->ot[drawBuf]` | 0 .. 0xFF (btl_main.c:2131) | `btlbg_draw` layers 8 (0xFF), 4 (projected, clamped), 2 (0); STP(0,0) at 0 (begin), STP(1,0) at 0xFF (end) | first, whole: `btlui_drawOTagEnv(&bgot[0xFF], &drawEnv[drawBuf])` (btl_main.c:2162) |
| fade OT, banner OT | 1 each | `fade_*` (main), `btl_drawBanner` | inside scene ot[0] (above) |
| battle UI `g_ot` | 0x18 | `btlui_*` in the VSync callback | every VBlank, by `BreakDraw`/`ContinueDraw` (btlui_frame.c, `btlui_submitOt`) |

ClearOTagR links entry i to i-1, so a larger index is farther away and is drawn first. Every link is a head insert
(`addPrim`; btl_drawMesh: `tag = *entry | len << 24; *entry = packet`, mesh_packets.c `meshLink`). So **within an
entry the most recently linked packet is drawn first** (behind the earlier ones).

### `btl_drawMesh` (asm, btl_mesh.c:3; src_equiv/ovl_0b/mesh_packets.c)
- Base: `ot = gd->ot[gd->drawBuf] - otBias` (byte offset `otBias * 4`, `lb` = signed). `btl_drawMeshOffset` (effect
  models) adds `+ otOfs`. A positive `otBias` moves the group **nearer** (drawn later). On the discs `otBias` is
  -4..+4: enemies {-4:1, -3:4, -2:14, -1:62, 0:318, 1:38, 2:2, 3:1, 4:1} groups, party {-2:3, -1:39, 0:46, 1:25,
  2:2, 3:1, 4:1}, weapons all 0.
- **Quads** (FT4/GT4/G4/F4): entry = `(SZ0 + SZ1 + SZ2 + SZ3) >> 6` (raw SZ from `g_vtxScreenZ`, i.e. mean / 16).
- **Triangles** (FT3/GT3/G3/F3): AVSZ3, `OTZ = ZSF3 * (SZ1 + SZ2 + SZ3) >> 12` (saturated to 0..0xFFFF), byte offset
  `OTZ & ~3`, so entry = `OTZ >> 2`. ZSF3 is 0x155 for the whole game: only PsyQ `InitGeom` writes control register
  29 (main `InitGeom.s`: `ctc2 0x155, $29`, ZSF4 = 0x100). No other `ctc2 …, $29` exists in any binary's asm, and no
  C uses a ZSF macro. So entry ≈ `0x155 * sum / 16384` ≈ mean / 16.02.
- **No clamp, no skip at z = 0.** Since SZ <= 0xFFFF, the unbiased entry is at most 0xFFF. With a negative `otBias`
  a polygon can reach 0x1000-0x1003, which are linked but never drawn. With a positive one near the camera it can
  reach -1..-4, which are drawn *after* entry 0, i.e. after the banner and the fade. Per model, only
  `btl_updateModelVisibility` (btl_mesh.c:647) skips the whole model when its origin has SZ = 0.
- Front faces only (NCLIP > 0 on corners 0, 1, 2) unless flag bit 1. Order of emission: groups in order, and within
  a group the eight lists in order (FT4 FT3 GT4 GT3 G4 G3 F4 F3), source order inside each list.
- `btlbg_setOtShift` (btlbg_asm_b.c) has one caller, `btlbg_init` with 0 (btlbg.c:9): `g_btlbgOtShift` is always 0.
  It only applies to background layer 0, which is then `sum4(SZ >> 4) & ~3` bytes for quads (≈ mean / 16
  entries) and a halving average of the three `SZ >> 4` for triangles, clamped to 0x3FFC bytes (entry 0xFFF),
  skipped at 0 (src_equiv/ovl_0b/background.c, drawRecord). So layer-0 arena polygons and model polygons share one
  depth scale and sort against each other.

### Link order in one scene
`btl_main` (btl_main.c:11) → `btl_beginFrame` → `btl_update` (btl_update.c:3) → per phase:

1. **Units** in `Battle.unitList` order (`btl_updateCombat` btl_cmd.c:23, `btl_updateUnits` btl_update.c:84,
   `btl_victory`, `btl_updateIntro`). Units with flag bit 22 (hidden) are skipped, and units with bit 23 ("model
   part") return early without drawing (btl_unit.c:43). For each unit, `btl_updateUnitModel`:
   body groups not in `hiddenMeshes` (`btl_projectMesh` + `btl_drawMesh` per group), then weapon groups, then the
   shadow (`btl_drawShadow`, 4 FT4 quads). In combat the unit's turn marker follows (`btl_drawActiveMarker`, ot[2]).
   Then the per-unit colour overrides of item 1 (`btl_updateUnitTint`) are applied to the packets just built.
2. `btl_drawScene` (btl_cmd.c:52): `btlseq_update` (sequence fades via `btl_setUnitFade`), `btlbg_draw` (layer 0 to
   the scene OT, the rest to the background OT), `btlfx_runFrame` (effects and effect models), `btl_drawPopups`
   (ot[4]), `btl_drawBanner` (banner OT). The intro phase draws units, background and banner only
   (btl_update.c:311-345).
3. `btl_endFrame`: STP(1,0) at ot[0xFFF] and bgot[0xFF], wait, draw both OTs.

Order on screen: background OT, then scene entries 0xFFF..0. Within an entry the order is effects (latest first),
then layer-0 background, then units in reverse `unitList` order, and inside a unit shadow, weapon, body (last list
first). After entry 0's own packets come the banner OT, the fade OT, STP(0,0), then entries -1..-8.

### Effects and shadows against model polygons
- **Effects** link into the same scene OT (`g_btlfxEnv->ot = gd->ot[drawBuf]`, btlfx_op.c:1219) at
  `(OTZ_sz - otZ) >> 2` with `gte_stszotz` = SZ3 >> 2 (btlfx.c:229 `btlfx_addPrim`, :236, :630-645, :849-855).
  So the entry is SZ/16 - otZ/4: **the model scale**. Out-of-range entries (>= 0x1000 unsigned) are dropped.
  `otZ` is a per-task bias that modules raise to come forward (`btlfx_heap.c:338`). Some full-screen effects use
  ot[0xFFF] (btlfx_op.c:1020/1029). Since effects are linked after all units, an effect packet in a model
  polygon's entry is drawn **before** it, i.e. behind the model polygon. That is the only interaction: the code
  never picks an entry because a model polygon is there. Effects do depend on the LIFO order of their **own**
  packets: `btlfx_linkPrimAbr` (btlfx.c:217) and the sprite/dot emitters (btlfx.c:849-855) link a DR_TPAGE/draw-mode
  packet *after* the primitive in the same entry, so the mode packet runs first. A port has to keep "mode packet
  before its primitive" and "effects behind same-entry model polygons" to be exact.
- **Shadows** (`btl_drawShadow`, btl_main.c:1523): each of the 4 quads goes to `ot[maxSZ(4 corners) >> 3]`
  (btl_main.c:1608-1640), with no clamp (a corner SZ above 0x7FF8 would index past the drawn range). That is
  **twice** the model scale (SZ / 8 against SZ / 16), so a shadow lands at about twice its unit's entry index:
  behind its unit, behind any unit or layer-0 polygon closer than twice its depth, and drawn right after the
  background OT for practical camera distances. It never shares an entry with its own unit, and it relies on the
  floor being in a background-OT layer (4/8/2) rather than layer 0. Shade: `BtlShadow.shade`, semi-transparent
  tpage 0xC8 (abr 2, subtract), CLUT 0x3FD0.
- **Mask bit**: both OTs start with STP(1,0) (drawn first, set-mask on) and end with STP(0,0), so the 3D scene is
  drawn with the mask bit set. `btlfx_resetMaskBit` and the UI depend on it. That is not a depth matter, but a port
  emulating the GPU needs it.

## 4. Battle frame structure

### Pacing
- `gd->unk842` / `unk8C4` are now `GameData.musicFadeFrames` / `musicFadeStart` (include/ff9.h). `func_800AE9C8`
  (btl_main.c:2115) starts a music fade (`musicVolume[0]`, length, `VSync(-1)` timestamp), and `func_800AEA08` says
  whether it is still running. They are not about frames.
- The scene rate is `GameState.vsyncMode`, 4 in battle (`btl_initMode`, btl_main.c:136). `btl_endFrame` calls
  `btlui_waitFrame` (btlui.c:106): `DrawSync(0)` (the previous scene's GPU work), `btlfx_pollDrawSync`, then
  `VSync(vsyncMode - 1)` if fewer than `vsyncMode - 1` VBlanks were counted since `btl_beginFrame` (the VSync
  callback `btlui_vsync` counts them in `g_btluiState.vsyncCount`), then `VSync(0)`. **[runtime]**
  `disc3_malboro_lv99_calm`: `btl_beginFrame` every 4 VBlanks (60 calls in 240), the same in Eiko's Trance and in
  the Trance effect (100 in 400). So one scene = 4 VBlanks = 15 Hz.
- The scene OT is submitted at the end of `btl_endFrame` after that wait, so the GPU draws scene N while the CPU
  builds scene N + 1. `drawBuf` flips unless paused (`gd->flags & 0x20`).
- Effect scripts can change the rate: `btlfx_opSetFrameRate` (btlfx_op.c:146) → `btl_setVsyncMode(60 / fps)`.
  No effect module calls the export directly (scan), and the traced effect 0x101 kept 4. `menu.c` saves and
  restores `vsyncMode` around the main menu.
- The battle UI (menus, cursor, ATB) runs in the VSync callback every VBlank (`btlui_vsync`, btlui.c:3: when
  enabled and `skipFrames` is 0). It draws its own 0x18-entry OT by interrupting the GPU (`BreakDraw`, draw, then
  `ContinueDraw`). It is therefore 60 Hz on top of the 15 Hz scene.

### Does every unit re-animate every scene?
`btl_updateUnitModel` (btl_unit.c:30) runs once per scene for every unit without flag bit 22 (hidden: Vanish-like
hiding by effects/scripts, escaped members, flag set by `btlfx_fadeUnit` below level 8). The calls are in all
phases: btl_cmd.c:39, btl_update.c:94/144/330. **[runtime]** 5 units × 60 scenes = 300 calls in 240 VBlanks.
For each such unit it:
- rebuilds `model->matrix` from `u->rot` (`math_rotMatrixZYX`) and `u->pos >> 16` (:56-73) and applies the scale
  (`btl_applyModelScale`; Mini sets the scale),
- **always** calls `btl_calcBoneMatrices(m, a->frame, ...)` (:91), i.e. all bones, every scene, then the weapon's
  (frame 0, attached to the parent's bone),
- advances `a->frame++` (:92-93) **unless** the unit has Petrify, Venom, Stop or Freeze, or flag bit 21. At the end
  of a motion (:75-80) it picks the idle (`btl_setIdleMotion`) or restarts at 0, and with those statuses it steps
  back one frame instead.

Flag bit 21 ("motion held") is set when a KO'd enemy's motion is held on its last frame (`btl_endEnemyHitMotion`,
btl_unit.c:272; `btl_updateKo` koState 2, btl_unit.c:306) and by effects (`btlfx_holdMotion` / `btlfx_setMotionHeld`, btlfx_script.c:1148/1155; modules
fx_105, fx_180, fx_1ec). It is cleared by `btl_resetUnit`, by removing KO (btl_status.c:246) and every scene of the
entrance phase (btl_update.c:97).

So the bone matrices are **deterministic** in (motion, frame, `rot`, `pos`, scale, parent bone). For a held or
stopped unit they come out identical from scene to scene only if nothing else moves it: Float changes `pos[1]`
every scene (`btl_updateUnitTint` end, btl_status.c:470), Confuse adds 0x80 to `rot[1]` each scene while the unit
idles (btl_status.c:390), and sequences and effects move units. The camera doesn't enter the bone matrices.
`btl_projectMesh` combines camera and bone per scene. Hidden units keep their last matrices but are neither
updated nor drawn. "Model part" units (bit 23) copy the body unit's rotation and their home position from its
bones and draw nothing themselves.

### What `btl_updatePause`'s `DrawOTag(D_800FDEB0)` draws (btl_main.c:46-106)
Nothing from the scene. On the first pause request (`gd->flags & 0x100` clear, or the disc cover opened) it
builds a one-entry OT `D_800FDEB0` and draws it once. Links: STP(0,0), the PAUSE sprite
(`ui_buildSprite(0xEB)`, centred) or the "Disc Cover Open" text, a semi-transparent TILE (code 0x62) 320×218 at
(0, 0) of grey 0x60, a draw-mode packet with abr 2 (B − F), STP(1,0). Head inserts reverse that, so the GPU does:
mask on, subtract mode, darken the whole frame by 0x60, sprite/text on top, mask off. It goes into the current
drawing area, i.e. over the last scene image. Then it sets `gd->flags |= 0x100 | 0x20`. While paused, `btl_main`
skips `btl_update`, `btl_endFrame` neither draws nor flips, and `btlui_vsync` skips the UI. The image stays
frozen. On the next request (else branch) it resumes: `func_800EA34C` sets the UI's `skipFrames = 2` and buffer, and
the flags are cleared.

## 9. Battle bone/vertex layout

### Layout
Yes. A mesh group's `vertices` are `vertexCounts[0]` vertices of bone 0, then `vertexCounts[1]` of bone 1, ...,
up to `boneCount - 1` (every group has one count per bone, most of them 0). `btl_projectMesh` (btl_mesh.c:5) walks
`i = 0 .. boneCount-1`, takes `n = vertexCounts[i]` vertices from `&vertices[k]` with bone i's matrix and advances
`k += n`. The screen arrays are indexed the same way. `tools/battle_pose.py`'s `Model.meshes` and
`battle_models.py` use that layout. The one exception is an effect model after `btlfx_splitMeshVertices`: all
split corners become one block stored in `vertexCounts[group number]` (the group number used as a bone index), so
the per-bone layout no longer holds there.

### How often polygons span bones
Measured with a scratch scan (`build/r29_bones.py`, not committed; it parses with `battle_models.header_stats` and
maps each vertex to its bone from `vertexCounts`) over every model file on the four discs: enemy files (dir 7,
182 models), party bodies (dir 10, 42 models), weapons (dir 8, first 0x1000 bytes, 85 models). Each model is
counted once. A polygon "spans bones" when its corners (3 or 4 vertex indices) map to more than one bone.

| kind | models | polygons | span > 1 bone | 2 bones: parent-child | 2 bones: other (siblings / grandparent / farther) | 3+ bones |
|---|---|---|---|---|---|---|
| enemy | 182 | 182,075 | 58,949 (**32.4%**) | 36,032 | 20,199 (3,158 / 12,156 / 4,885) | 2,718 |
| party | 42 | 17,081 | 6,310 (**36.9%**) | 3,754 | 2,168 (472 / 1,365 / 331) | 388 |
| weapon | 85 | 6,135 | 0 (0%) | 0 | 0 | 0 |
| all | 309 | 205,291 | 65,259 (**31.8%**) | 39,786 | 22,367 | 3,106 |

Per model: enemies median 33.7%, min 0% (7 models, e.g. 14 Zombie: one bone; 354, 1), max 79.4% (model 154), 12
models at 50% or more. Ark (512) has 0.8%, Armodullahan (344) 6.1%. Party bodies range from 21.7% (606) to 53.9%
(5415, Vivi). The port's heavy enemies:

| model | name | bones | groups | polygons | span > 1 bone |
|---|---|---|---|---|---|
| 166 | Hecteyes | 56 | 2 | 1889 | 49.1% |
| 95 | Ochu | 42 | 2 | 1897 | 23.4% |
| 355 | Grand Dragon | 56 | 2 | 1816 | 32.5% |
| 324 | Adamantoise | 40 | 2 | 1788 | 17.3% |
| 341 | Abadon | 55 | 3 | 1782 | 28.3% |
| 78 | Whale Zombie | 26 | 3 | 1632 | 28.4% |
| 163 | Zaghnol | 26 | 2 | 1384 | 23.3% |
| 157 | Wyerd | 27 | 1 | 850 | 28.2% |
| 145 | Bandersnatch | 41 | 1 | 908 | 37.4% |
| 150 | Clipper | 30 | 1 | 618 | 25.6% |
| 610 | Red Dragon | 46 | 1 | 978 | 41.9% |
| 96 | Malboro | 63 | 2 | 1748 | 22.7% |

For per-bone normal-cone culling at load: about two thirds of the polygons sit on one bone and can be culled by
that bone's cone. The remaining third joins bones, mostly a bone and its parent (61% of the spanning ones) or its
grandparent (21%). Those need either a cone per bone pair or the per-polygon test.

## Proposed names
All `[code]` evidence, from the functions cited above. None applied (docs-only task).

| symbol | proposed | evidence |
|---|---|---|
| `func_800AB3E4` (ovl_0b) | `btl_swapPartyModel` | Wrapper of `btl_swapPartyModelStep(arg0, 0x17 \| arg1 << 15)`, the twin of `btl_reloadParty` → `btl_reloadPartyStep`. Called from the effect script with `d->modelSwap` (btlfx_script.c:140, 343) |
| `func_800EA34C` (ovl_0b) | `btlui_resumeFromPause` | Only caller: the resume path of `btl_updatePause`. Sets `g_btluiState.skipFrames = 2`, `g_drawBufs.cur = drawBuf` |
| `func_800EA290` (ovl_0b) | `btl_putDispEnvNoUi` | `PutDispEnv(&dispEnv[drawBuf])` only while the battle UI isn't started (`g_btluiState.started == 0`), from `btl_endFrame` |
| `D_800FDEB0` | `g_btlPauseOt` | 1-entry OT: `ClearOTag(.., 1)`, filled and `DrawOTag`'d only in `btl_updatePause` |
| `D_800FDE98` / `D_800FDEA8` | `g_btlPauseDimTile` / `g_btlPauseDrawMode` | the 320×218 semi-transparent grey-0x60 TILE and its abr-2 draw mode, `btl_updatePause` |
| `D_800FDF18` / `D_800FDF24` | `g_btlPauseStpOff` / `g_btlPauseStpOn` | `GPU_STP(0,0)` / `GPU_STP(1,0)` packets of the pause OT |
| `D_800FDEB8` / `D_800FDED0` (`[2]`, type `Unk800FDED0`) | `g_btlFrameStartStp` / `g_btlFrameEndStp` | per draw buffer: STP(0,0) linked at scene ot[0] and bgot[0] in `btl_beginFrame`; STP(1,0) at ot[0xFFF] and bgot[0xFF] in `btl_endFrame` |
| `Unk800FDED0` | `BtlStpPair` | two 0xC-byte DR_STP packets (`unk0` scene OT, `unkC` background OT) |
| `BtlUnit.flags` bit 21 (comment) | "motion held" | set by `btlfx_holdMotion`, `btl_endEnemyHitMotion` and `btl_updateKo` (KO motion held on its last frame); blocks `a->frame++` (btl_unit.c:92) |
