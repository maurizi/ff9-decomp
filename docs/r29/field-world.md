# R29 items 5-7: field models, field background layers, world map lighting

Answer to ff9-ds request R29, items 5, 6 and 7 (facts for the speed work on the original DS). Everything below is
read from the decompiled C (ovl_0a field, ovl_0c world map, main's event VM), the asm of the routines that are
still asm, and a scan of the four discs' field background files. No emulator run was made for this note; the
runtime evidence quoted comes from earlier campaigns in `docs/runtime-findings.md`. Line numbers are those of this
commit.

## Summary for the port
- **5. Field models.** `EvtObject.animFrame` is stepped by `evt_stepAnim` (main `src/main/event.c:3177`), called
  from `evt_updateObjects` (`event.c:2935`) once per field frame for every actor (kind 4) in state 1. The field
  loop (`fld_main`, `src/ovl_0a/fld_main.c:20`) runs exactly one `evt_update` and one `fld_drawModels` per
  iteration and ends with `VSync(2)`, so **animation, logic and draw are 1:1 at 30 Hz** (one field frame = 2
  VBlanks; 4, later 3, while a movie background plays). The step is `animSpeed / 16` frames per field frame
  (default 16 = one frame; idle loops pick 1 of 4 random speeds each loop). There is **no dirty state** for
  position, rotation, frame or motion: `fld_drawModels` rebuilds every visible model's world matrix and all its
  bone matrices every draw (`fld_movie.c:391-439`), and `prevPos` is not usable as one (see 5.4). R28's
  `lastFrame` belongs to **texture animations** (VRAM copies), not to bones. Bone matrices are world-space and
  depend only on the inputs listed in 5.3, none of them on the camera; they can be reused when that input tuple
  is unchanged, which in practice happens for actors with a held frame, no motion, a speed < 16 (repeated frames),
  or while a menu is fading in. Projection must still be redone whenever the view matrix, projection distance
  or **background scroll** (GTE screen offset) changes.
- **6. Background layers.** All layer motion is in `fldbg_draw` (`tu_800B61A4.c:3564`), once per field frame:
  flipbook animations (`fldbg_updateOverlayAnims`, layer visibility), scripted scrolls (`fldbg_updateScroll`),
  player follow (`fldbg_followPlayer`), loop / skew / parallax layers (`fldbg_updateOverlayScroll`), and the event
  script's direct layer ops (colour, show, move, position, loop, parallax, viewport). `bgi_updateAnims` /
  `bgi_showAnimFrame` / `bgi_updateFloorMoves` are **walkmesh** animations: they never touch what is drawn.
  There are **no** background texture or palette animations: `FieldPalAnim` is built only by
  `fldbg_startClutFade`, which has no callers and no consumer. The original keeps **no per-layer change state**;
  a layer's drawn output is fully determined by a ~12-value tuple (6.3), which the port can compare instead of
  hashing tiles. On the discs, 95% of fields have a scrollable camera, 60% have flipbook animations, and the event
  scripts recolour layers every frame in many fields (6.4).
- **7. World map lighting.** The light matrix is a constant (`wm_initLighting`, written once). The colour matrix
  and back colour are rebuilt **every logic frame** by `wm_weatherUpdate` from a weather blend and the camera
  target's altitude, but only change value during a weather transition (32 frames at the automatic speed) or
  while the camera target moves between altitudes 0x898 and 0x898 + 4500 (airship flight). Block normals are
  **static**: each vertex's pad byte indexes a fixed 240-entry normal table in ovl_0c's `.data`. NCT's result
  therefore depends only on (normal index, colour matrix, back colour): the port can light the 240 table
  normals once whenever those 12 values change and look the colour up per vertex. The depth cue, fog colour,
  curvature and screen position depend on each vertex's SZ and must be redone whenever the camera moves.

## 5. Field model animation and the draw

### 5.1 The field frame
`fld_main` (`src/ovl_0a/fld_main.c:20`), per iteration:

| line | call | what it does for models |
|---|---|---|
| 24 | `fld_beginFrame` | clears this buffer's OT |
| 38 | `evt_update` → `evt_updateObjects` (main `evt_main.c:18`) | saves `prevPos`, steps every actor's animation, runs the scripts (which move, turn and re-animate actors) |
| 74 | `fldbg_draw` | background; sets the scroll and the GTE screen offset models are projected with |
| 77 | `fld_drawModels` | world matrices, bone matrices, projection, packets, shadows, texture animations, lights |
| 80-81 | `bgi_updateAnims`, `bgi_updateFloorMoves` | walkmesh animations; floor moves shift actors standing on a moving floor **after** the draw |
| 108 | `fld_endFrame` | `DrawSync`, `VSync(vsyncMode)`, `DrawOTag`, buffer flip |

`fld_endFrame` (`fld_movie.c:1342`) sets `vsyncMode = 2` every frame (line 1355) unless a movie background is
playing (`mbg_getStatus() & 2`): `fld_movieUpdate` sets 4 when the movie starts (line 1219) and `mbg_update`
lowers it to 3 once (`src/ovl_64/mbg.c:485`). `VSync(2)` waits until two VBlanks have passed since the previous
call, so a frame that takes longer simply stretches: animation is counted in field frames, never in VBlanks, and
there is no catch-up.

Exceptions to the 1:1 rule:
- `GD_MENU_OPEN` (menu opening/fading): `evt_update` is skipped, `fldbg_draw` and `fld_drawModels` still run
  until `menu_onFadedOut` sets `GD_HIDE_ALL`. During those frames every model is drawn with **identical** inputs.
- `GD_PAUSED` (`fld_togglePause`): nothing runs and nothing is drawn (`DrawOTag` is skipped too).
- `GD_HIDE_MODELS` / `GD_HIDE_BG`: the respective draw is skipped, logic still runs.

### 5.2 Who steps `animFrame`, and at what rate
`evt_updateObjects` (`event.c:2874`): for every object of kind 4, first `prevPos = pos` (lines 2904-2932; for the
player before `evt_movePlayer`), then, when `state == 1` and not in world-map mode, `evt_stepAnim(o)`
(line 2935). Then the scripts run (`evt_runScripts`), so script ops that change `anim`, the frame range or the
speed take effect at the **next** step; the walk/run/stand switch at lines 3034-3049 (after the scripts, same
frame) resets `animFrame = 0` when the animation changes.

`evt_stepAnim` (`event.c:3177`) keeps a 12.4 fixed-point frame position in `animFrame` (u8) and
`animSubFrame` (low 4 bits):

- `animFlags & 4` (hold: set by `evt_onAnimEnd` for a scripted animation that ends with `animFlags & 8`):
  **returns at once, the frame is frozen**.
- `animFlags & 0x80` (scripted animation, ops 0x40 `ANIM` etc.): `pos += animSpeed` (or `-=` with 0x40, reverse)
  between `animStart` and `animEnd`; at the limit `evt_onAnimLimit` loops, bounces (frame ∓ 2) or ends
  (`evt_onAnimEnd`: back to `standAnim`, frame 0, a random idle speed). With an `animSyncSource` (op 0xB5) it
  copies the other actor's anim and frame instead.
- otherwise (stand/walk/run loop): `pos += animSpeed`, wrap to 0 at `animFrames`; wrapping the stand animation
  draws a new `animSpeed` from `idleAnimSpeeds` (4 bytes, op 0x86).

Speeds: new actors get `animSpeed = defaultAnimSpeed = 16` and `idleAnimSpeeds = 16,16,16,16`
(`evt_execInstr_newactor`, `include/ff9.h` ~4075); op 0x3E sets `defaultAnimSpeed` (15 calls in 8 fields in the
opcode census), op 0x86 the idle speeds (Zidane 14/16/18/20 at run time), a jump sets
`(animEnd - animStart) * 16 / frames` (`event.c:1270`). **So a playing animation advances one motion frame per
field frame**; with speed 14 one frame in eight repeats the previous `animFrame`, with 18-20 a frame is skipped
now and then.

### 5.3 What `fld_drawModels` recomputes every draw
`fld_drawModels` (`fld_movie.c:356`):

| lines | step | inputs |
|---|---|---|
| 381-386 | GTE screen offset: `fldbg_applyGeomOffset` (camera centre + `g_fieldBgState.geomOffsetX/Y`), or `mbg_getGeomOffset` while a movie background drives the camera | background scroll |
| 390 | `fld_collectVisibleModels` (297): actors with `state 1`, `flags & 1`, kind 4; parents first, then attached children | |
| 391-411 | `ModelObj.worldMatrix`: `math_rotMatrixZXY(tiltX, rotY, tiltZ)` with `FLD_OBJ_TILT` and `bgi_getActorTilt`, else `math_rotMatrixZYX(rotX, rotY, rotZ)`; `t = pos`; `fldmdl_applyScale` (`ModelObj.flags & 2`, `scaleX/Y/Z`) | `pos`, `rotX/Y/Z`, walkmesh triangle, scale |
| 413-426 | motion pointer: `db_lookupRegistered(4, obj->anim \| DB_ANIM)` (relocated on first use) | `anim` |
| 428-439 | `fldmdl_calcBoneMatricesLookAt` (`ModelObj.flags & 1`) or `fldmdl_calcBoneMatrices(m, obj->animFrame)` | motion, `animFrame`, `worldMatrix`, look-at target, parent bones |
| 440-458 | near/far cull: bone 0's origin through the view matrix, `SZ / 4 + scene otBias` outside `[100, 0x1000 - 100]` drops the model | view |
| 459-485 | per group not in `hiddenGroups`: `fldmdl_projectMesh` + `fldmdl_drawMesh` (or the `ClipY` pair) | bones, view matrix, `projDist`, screen offset |
| 486-491 | `fld_drawShadow` (bone 1's matrix) | bones, view |
| 492-501 | restarts texture animation 2 when none is active (R28's NULL read) | |
| 503-507 | `fldmdl_updateTexAnims` | |
| 509 | `fld_applyLights` | floor light, colours |
| 513 | `fld_applyModelCommands`: semi-transparency; **mirror copies recompute their bone matrices again** (`fld_movie.c:724`, `fldmdl_calcBoneMatrices(x->model, src animFrame)`) and project/draw mirrored | |

`fldmdl_calcBoneMatrices` (`tu_800B61A4.c:731`) writes **world-space** bone matrices into
`ModelObj.boneMatrices` (built in the scratchpad and copied out when the model has fewer than 31 bones). It never
reads the view: the view is composed per bone in `fldmdl_projectMesh` (`tu_800B61A4.c:3`, the
`gte_rtir`/`gte_rtv0tr` sequence), **once per group** (a model with 6 groups multiplies view × bone 6 times per
bone that has vertices in each group).

So a model's bone matrices are a pure function of:
1. the motion (`EvtObject.anim` → `ModelObj.anim`) and `animFrame` (u8);
2. `pos.vx/vy/vz`, `rotX`, `rotY`, `rotZ` (FieldEvtObj +0x14..+0x25), and with `FLD_OBJ_TILT` (slot flags 0x2)
   the tilt `bgi_getActorTilt(uid)` of the walkmesh triangle under the actor;
3. `ModelObj.flags & 2` and `scaleX/Y/Z` (op 0x9F);
4. `ModelObj.flags & 1` (look-at), `lookAtTarget[3]` (written every frame by main's `evt_updateLookAt` /
   `evt_relaxLookAt`, which move it by at most 100 units per frame toward the target and stop exactly on it);
5. for an attached model (op 0x4C): the parent's bone `parentBone` and `attachX/Y/Z` (op 0xD4);
6. the model header (static after `fldmdl_initModel`).

The packets add: the view matrix (`GameData.viewMatrix`, changed by `fldbg_setCamera`, op 0x7E, or by the movie
background's camera `mbg_getStartCamera`), `GameData.projDist`, the GTE screen offset (follows the background
scroll every frame the scroll moves, 6.2), `hiddenGroups`, `clipY` with `FLD_OBJ_CLIP_Y`, the scene `otBias`
(per camera, `BgCamera.otBias`), and the current draw buffer (packets are double-buffered:
`prims + (drawBuf ? packetSize : 0)`).

### 5.4 Is there any "didn't change" state?
None for geometry. What exists:

| state | meaning | usable as a dirty flag? |
|---|---|---|
| `EvtObject.prevPos` | `pos` at the start of `evt_updateObjects` (`event.c:2868-2932`) | No. It is rewritten every frame before the scripts move the actor, and `bgi_updateFloorMoves` (`bgi.c:761`), which runs **after** the draw, moves actors on a moving floor and sets `prevPos = pos` on that axis, so that move is invisible to a `pos != prevPos` test. It says nothing about rotation, frame or motion. |
| `EvtObject.animPrevFrame` | frame before the last step (used by `evt_onAnimEnd` to hold) | Only for frames; not touched by script anim changes. |
| `g_evtCameraChanged` (main) | camera id changed this frame (`event.c:2956-2958`) | Camera only. |
| `FLDBG_CAMERA_CHANGED` | `fldbg_setCamera` / `fldbg_setOverlayPos`; cleared by `fldbg_updateOverlayScroll` | Background only. |
| `TexAnim.lastFrame` (`Unk800AB0B0Entry.lastFrame`, R28) | texture animation frame last copied with `MoveImage`; `fldmdl_updateTexAnims` (`tu_800B61A4.c:1699`) copies only when `pos >> 12` differs; 0x1000 forces a copy; `fldmdl_initTexAnims` starts it at 1 | VRAM only (eyes, mouths). Scroll-mode animations (`TEXANIM_SCROLL`) copy every frame. |
| `FieldActorInfo.litFloor` / `flags & 4`, `color` / `nextColor` (`fld_applyLights`, `tu_800B0124.c:1014`) | the light colour is recomputed only when the actor's floor light or `FLD_OBJ_DIR_LIGHT` changes; the new colour is written into the current buffer's packets, and next frame (`nextColor`) into the other buffer's | Packet colours only. |
| `FieldActorInfo.flags & 0x10000`, slot flags 0x20000 / 0x40000 (`fld_applyModelCommands`, `fld_movie.c:667`) | semi-transparency applied; repeated one frame later for the other buffer | Packet blend bits only. |

The port therefore has to keep its own copy of the tuple in 5.3 per actor. The cheapest exact test is the list
above (≈ 12 words per actor plus the parent's result).

### 5.5 When can bone matrices be reused?
Whenever the tuple of 5.3 (items 1-5) equals the previous draw's. Cases where that happens in the game:
- the actor holds a frame (`animFlags & 4`) or has no motion (`anim` not found: one matrix from `worldMatrix`),
  and did not move or turn (only state-1 actors are stepped, and only state-1 actors are drawn);
- `animSpeed < 16` and the 12.4 position did not cross a frame boundary (1 frame in 8 at speed 14); a speed of 0
  (op 0x3E 0) freezes the frame;
- the menu fade frames (`GD_MENU_OPEN` without `GD_HIDE_ALL`), and any frame where `evt_update` is skipped;
- props: actors whose script never animates them still play their stand animation in a loop unless the
  animation has one frame; a one-frame stand animation repeats frame 0.

In ordinary play every visible animated actor gets a new frame almost every field frame (idle loops never
stop), so reuse is mostly a win for **still** objects (one-frame motions, held frames) and for the
**projection** of everything when neither the bones nor the camera/scroll changed. A model attached to a parent
must be invalidated with its parent. The mirror copy (`fld_modelCommand` 8-11) takes its source's frame and is
reflected each frame (`fldmdl_reflectModel`), so it follows the source.

## 6. Field background layers

### 6.1 Data
The background is item `0x0Axxxx` (`DB_BG`) of the field file's container `0x1B0001` (`fld_setupBackground`,
`fld_main.c:689`; `BgFile` in `include/ovl_0a.h`). It holds `overlayCount` layers (`BgOverlay`, 0x38 bytes: flags,
depth, position, scroll fields, camera id, viewport, tile count, tile tables), `overlayAnimCount` flipbook
animations (`BgOverlayAnim`, a list of `{overlay, delay}` steps) and the cameras (`BgCamera`: view matrix,
`projDist`, centre, size, scroll limits, `otBias`). `fldbg_buildOverlaySprites` (`tu_800B61A4.c:2493`) builds
**two** `DR_TPAGE + SPRT_16` copies of every tile at load (one per draw buffer); after that only the sprites'
xy (every draw) and rgb (colour ops) are written. Texture page, CLUT and uv never change.

### 6.2 What moves or animates layers, and how often
Everything runs once per field frame (30 Hz), from `fldbg_draw` (`tu_800B61A4.c:3564`) or from the event
script (main `evt_execInstr`, `event.c:1683-1850`, during `evt_update`, i.e. before the draw of the same frame):

| code | effect | when |
|---|---|---|
| `fldbg_updateOverlayAnims` (2594) | flipbooks: for every animation that is `FLDBG_ANIM_ACTIVE` and `LOOP` or `PLAY_ONCE` and belongs to the current camera, **clears the visible bit (0x2) of all its step layers and sets it on the current step's layer**, every frame; advances one step after `delay` frames (8.8 `pos += speed`, 0x100 = one step), wraps / ping-pongs / stops | only after the script starts it: `fldbg_init` resets every animation to flags 1 (stopped, first step visible). Ops 0x5F (frame), 0x60 (play), 0x61 (speed), 0x63 (delay), 0x64 (loop/ping-pong), 0x65 (range), 0xCA (reset), 0xE7 (`fldbg_showAnimFrame`: show one step and stop). Quirk: a step with a negative delay (end of a `playAnimRange`) `return`s, so the animations after it are not processed that frame. |
| `fldbg_updateScroll` (3163) | scripted scroll: `scroll` moves linearly or with a cosine ease from `scrollFrom` to `scrollTo` over `scrollFrames` frames; loop / skew layers are shifted by the delta (scaled by their viewport's scroll rate); sets `geomOffsetX/Y` | ops 0x6F (`fldbg_scrollTo`), 0x70 (`fldbg_scrollToPlayer`); after a 0x6F scroll the follow stays off (mode 4) until the next one; after 0x70 it resumes |
| `fldbg_followPlayer` (3297) | when following (`flags & 0x40`) and no scripted scroll is running: projects the player (`pos.y - followHeight`), averages the screen point with the previous one (`(prev + new) >> 1`, so the scroll settles a few frames after the player stops), clamps to the camera's scroll limits, sets `scroll` and `geomOffsetX/Y`, shifts loop / skew layers by the delta | every frame; changes value only while the player's projected position moves and the camera can scroll (limits min ≠ max) |
| `fldbg_updateOverlayScroll` (3106) | `FLDBG_OVL_LOOP` layers auto-scroll by `scrollX/Y` (8.8 px per frame, 0x7FFF = none) modulo their size; `FLDBG_OVL_SKEW_LOOP` the same along one axis; `FLDBG_OVL_PARALLAX` layers are placed at `base + (scroll - parallaxOrigin) * scrollX/Y / 256` | every frame for layers with those flags (set by ops 0x5C, 0xE4, 0x5D; only one layer on the four discs has one of them preset in its file) |
| `fldbg_updateFollowOverlays` (2748) | op 0x92: a layer pinned to the player's screen position plus four mask quads | 1 field in the opcode census |
| `fldbg_draw` (3592-3593) | `screenX/Y = posX/Y - (scroll - (160, 112))`: every non-screen layer moves with the scroll | every frame |
| ops 0x59 (`fldbg_setOverlayColor`), 0x5A (`fldbg_moveOverlay`), 0x5B (`fldbg_showOverlay`), 0x5E (`fldbg_setOverlayPos`), 0xC3 (`fldbg_setOverlayViewport`), 0xE6 (`fldbg_setOverlayFlag1`, the screen flag), 0xC9 / 0xED (viewport rectangle / scroll rate), 0x1E / 0x74 (scroll limits), 0x7E (camera), 0x58 (`fldbg_moveOrigin`, moves every layer; not seen at run time) | direct writes | when the script runs them |

**Colour is per draw buffer.** `fldbg_setOverlayColor` (`tu_800B61A4.c:4119`, op 0x59) writes the rgb of the
**current buffer's** tile copy only (`spr + (bufIndex ? n : 0)`); the other copy keeps its colour until another
0x59 in a frame where it is current. The opcode census (`docs/runtime-findings.md`, "Event opcode table") counted
71,002 calls in 134 of the 296 fields entered, 4,900 in field 100 alone over 900 VBlanks: the scripts that fade or
pulse layers issue it every frame. A port that keeps one copy per layer must still model this (a single 0x59
leaves the two buffers different). `fldbg_setOverlaySpriteColor` (both copies) and `fldbg_toggleOverlayColor`
have no callers.

**Not layer animation:**
- `bgi_updateAnims` / `bgi_showAnimFrame` / `bgi_setAnimFrame` … (`bgi.c:411, 501`; ops `FF xx`) switch sets of
  **walkmesh triangles** on and off (`BGI_FLAG_ACTIVE`) and move actors between them; `bgi_updateFloorMoves`
  (`bgi.c:761`) moves a walkmesh floor and the actors on it. Neither touches sprites, layers or VRAM. The field
  campaign's ablation saw no visible change from either in seven fields.
- `FieldPalAnim` / `fldbg_startClutFade` (`tu_800B61A4.c:3774`): builds a CLUT fade record (type 0x80 entry,
  `StoreImage` of the CLUT row) but nothing calls it, and nothing reads the entries: `fldbg_draw`'s loop over
  `g_fieldBgCtx.entries` (lines 3573-3583) only switches on the type with dead results (`func_800C1CC0` is the
  same dead loop). **No palette or VRAM animation of the background exists in the field code.**
- Model texture animations (`fldmdl_updateTexAnims`, `MoveImage`) belong to models (5.4). Field-local models'
  TIMs are uploaded with the background's TIMs, so an animated model texture can sit on a page the background
  also uses; the copies only touch the animation's own rectangles.

### 6.3 Per-layer "unchanged" state
The original has none: `fldbg_drawOverlay` (`tu_800B61A4.c:2877`) re-culls and re-links every tile of every
visible layer every frame and writes each linked tile's x/y into the current buffer's copy. The only change bits
are `FLDBG_CAMERA_CHANGED` and `g_evtCameraChanged` (camera switch) and `BgOverlay.flags & 2` (visible).

A layer's output in a frame is a function of:
- `flags & 2` (visible) and `cameraId == g_fieldBgState.cameraId`;
- `o->x`, `o->y` (plus `fracX/Y`, `scrollX/Y`, `width/height`, `viewport` for loop / skew layers);
- `BgFile.screenX/Y` (i.e. `g_fieldBgState.scroll` and `posX/Y`); for loop / skew / screen layers also
  `viewportPos/Size[viewport]`;
- `o->depth` and `BgFile.baseDepth` (OT index = `baseDepth + depth + tile depth`, tiles outside 8..0xFF8 dropped);
- the rgb of this buffer's copy (`g_fieldBgCtx.overlayRgb` mirrors the last value written);
- the OT base (scene `otBias` does not apply to the background).

Comparing that tuple per layer per buffer is exact and costs a few words per layer, against hashing tiles.

### 6.4 Which layers are static in practice (scan of the four discs)
`build/`-only script (not committed; it reads `BgFile` from every field file with `tools/encounter_catalog.py`'s
readers, 818 distinct fields):

| | count |
|---|---|
| fields / layers / tiles | 818 / 13,805 / 935,797 |
| layers visible at load (file flag 0x2) | 7,634 (756,735 tiles) |
| layers that are steps of a flipbook animation | 7,197 (208,417 tiles, 22%) in 495 fields (1,081 animations) |
| fields without any flipbook animation | 323 |
| layers with a loop / parallax / skew flag preset in the file | 1 |
| fields whose every camera has min = max scroll limits (camera can't follow) | 39 |
| fields without flipbook animations **and** with fixed cameras | 21 |
| cameras per field | 1: 740, 2: 66, 3: 8, 4: 4 |

File flag bytes: 0x12 (6,382 layers), 0x20 (5,769: a flipbook step, hidden), 0x22 (995: flipbook step, visible),
0x52 (198), 0x60 (375), 0x62 (58), 0x10 (27), 0x1A (1). Bit 0x40 is `FLDBG_OVL_ANIMATED`, 0x20 marks a flipbook
group (`fldbg_initEffectList` skips the group's other steps), 0x10 is not read by any code.

What the scripts do to layers can't be read reliably without a script disassembler (instruction lengths depend on
expression arguments; a byte-pattern scan over-reports loop/parallax ops ~15× against the run-time census, so
its numbers are not given). The run-time census over 296 fields (900 VBlanks each) is the better guide:
colour 0x59 in 134 fields, show 0x5B in 66, move 0x5A in 29, loop 0x5C in 18, parallax 0x5D in 14,
anim speed 0x61 in 102, anim flags 0x64 in 98, anim frame 0x5F in 62, play 0x60 in 41, show-frame 0xE7 in 20,
scroll-to 0x6F in 37, camera 0x7E in 15.

So: a layer that is not a flipbook step, has no loop/parallax/skew flag and is not named by a colour/show/move op
is static **relative to the scroll** for the whole field; the scroll itself moves every non-screen layer by the
same amount whenever the camera follows the player (95% of fields can). In a still frame (player standing, no
scripted scroll), all such layers are unchanged; the moving parts are then running flipbooks, loop/parallax
layers and per-frame colour ops. That is exactly what the tuple of 6.3 detects.

## 7. World map lighting

### 7.1 The world frame
`wm_frameMain` (`w_frame.c:77`) → `wm_updateLogic` (177) → `wm_updateDraw` (225); the world's end-of-frame
(`wm_main.c:1270-1277`) uses `VSync(3)` (20 Hz) in normal play, `VSync(2)` during transitions, 0 on the navi
screen or with everything hidden. Logic (state 1/2, not the navi map), in order: actors, events, effects,
camera, map, **`wm_weatherUpdate`** (`w_frmbuf.c:140`), navi, VRAM animations (`wm_updateVramAnims`: texture
scroll, texture and CLUT animations). Draw: actors (`wm_drawActors`, ends with `SetFarColor(0, 0, 0)`,
`w_movement.c:786`), `wm_drawMap` (`tu_800BA528.c:171`), navi, effects, rain.

### 7.2 Light matrix, colour matrix, back colour
| GTE state | value | written by | changes |
|---|---|---|---|
| light matrix (LLM) for blocks | `g_worldMapLightMatrix` = rows (0, -1, 0), (1, 0, 0), (0, -0.25, 1) | `wm_initLighting` (`w_frame.c:795`) only, at `wm_systemInit` | **never** |
| colour matrix (LCM) | `g_worldMapColorMatrix`, columns = `work[0].lightColor[0..2]` | `wm_updateColorMatrices` (`w_frame.c:829`) from `wm_weatherUpdate`, every logic frame | when `work[0]` changes (below) |
| back colour (BK) | `work[0].backColor` (low bytes of its three s16) | `wm_drawMap` (line 175) before the blocks; `wm_drawBlockFx` restores it after a block effect (`wm_effect.c:403-405`) | same |
| far colour (FC) | 0 for the block pass | `wm_initLighting`, end of the actor pass (`w_movement.c:786`); `wm_setActorFog` sets it per actor model, the actor pass resets it; `wm_blockVertsFar` zeroes FC's red around its INTPL and restores it | constant 0 during blocks |

`wm_drawMap` sets LLM, LCM and BK explicitly before `wm_drawBlocks`, so the block pass never inherits other
values. Block effects (`wm_drawBlockFx`, called between blocks from `wm_drawBlocks`, line 1081) switch to
`g_worldBlockFxLightMatrix` / `g_worldBlockFxColorMatrix` (all zero: their models get the flat
`fxBackColor >> 5`) and restore the map's three before returning. The backdrop (after the blocks) uses its own
`g_worldBackdropLightMatrix` / `ColorMatrix` and `backdropBackColor`.

`wm_weatherUpdate` (`w_frmbuf.c:140`), every logic frame:
1. `wm_weatherAuto` when automatic weather is on: `wm_getWeatherByPos` (four circles on the map, one only on the
   disc-4 data set) picks mode 0-3; a change calls `wm_setWeather(mode, storyFog, 0x80)`. On the 10th ready frame,
   in areas 9/12/13, it patches preset 3's fog and re-applies.
2. A weather transition in progress (`g_worldWeatherBlend >= 0`): blend += speed (0x80 automatic: 32 frames;
   scripts pass their own speed, `w_frameSetParameter` `WM_REQ_WEATHER`; `wm_weatherStart` uses 0x1000, one
   frame) into `work[1]` (low-altitude sky) and `work[2]` (high-altitude sky) from the old to the new presets
   (16 presets: 4 modes × fog on/off × low/high).
3. `work[0] = blend(work[1], work[2], t)` with `t = clamp(((-cameraTarget.y - 0x898) << 12) / 4500, 0, 0x1000)`:
   the camera target's altitude.
4. `wm_updateFogParams` (`g_worldFogParams[0..1]`, backdrop fog), `wm_updateColorMatrices`, clear colour.

So LCM / BK are recomputed every frame but only take new values during a weather transition, while the camera
target's altitude is between 0x898 and 0x898 + 4500 and changing (flying), or when the fog flag changes
(`wm_getStoryFog`: story counter in [0x1766, 0x1766 + 0x13ED) means no fog). There is no time-of-day cycle. The
navi screen (`g_worldNaviMode == 2`) skips the update.

### 7.3 Block vertices and normals
The vertex routines are hand-written asm (`wm_mesh_asm.c`; C equivalent `src_equiv/ovl_0c/world_verts.c`):
`wm_drawBlock` (`w_memoryman.c:619`) picks `wm_blockVertsFog` (fog on), `wm_blockVertsNear` (fog off, block
near) or `wm_blockVertsFar` (fog off, block view z outside -10000..11000, decided per block per frame in
`wm_drawBlocks`). Per three vertices: RTPT (for SZ), **NCT** on the normals, then:

- Near: lit colour = NCT colour. No depth term.
- Fog / Far: lit colour = DPCS(NCT colour → FC = 0, `p = clamp((SZ - 0x8000 + fogOffset2) >> 2, 0, 4096)`; `>> 1`
  in Far); fog colour = DPCS(fog entry colour → 0, `4096 - clamp((SZ - 0x8000 + fogOffset) >> 2)`). The fog entry
  is `g_worldFogParams[pad bit 9]` (two entries per frame from `work[0].fog[1]` and `fog[0]`).
- Curvature: `y *= (4096 - (SZ/16 + SZ/64)) / 4096` (Fog/Near by a multiply, Far by INTPL with FC red = 0), then
  RTPT again for the screen xy and the stored depth.

**Normals are static.** The normal is `D_800CD794[pad & 0xFF]`: a 240-entry `SVECTOR` table in ovl_0c's
initialised `.data` (0x800CD794-0x800CDF14), referenced only by the six vertex routines, which read it. The
vertex records (from the streamed chunk packs) are read, never written (the curvature works on a register copy).
LLM is constant and NCT does not use the rotation matrix, so the normal is lit in world space independently of
the camera, and NCT does not multiply by a vertex colour. Hence

  NCT colour(vertex) = f(normal index, LCM, BK)

exactly (same GTE arithmetic, same saturation).

### 7.4 What a per-block (or per-normal) lit-colour cache can keep
- **Cacheable:** the NCT colour. Best keyed by the normal index, not by block: light the 240 table normals when
  any of the 9 LCM + 3 BK values differs from the cached ones (compare the values themselves; they are
  recomputed every frame), then every vertex's NCT colour is a table lookup. Near blocks without fog need nothing
  else for their lit colour. A per-block cache works too and is invalidated by the same 12 values or by the
  block slot being reloaded with another block.
- **Per frame, per vertex (camera-dependent):** SZ (the block's view transform: `g_worldViewMatrix` composed with
  the block position minus `g_worldCameraTarget`, `wm_drawBlocks` lines 1050-1055), hence the DPCS factor, the fog
  colour, the curvature and the screen xy. Their other inputs: `g_worldFogParams[0..1]` (colour, `fogOffset`,
  `fogOffset2`: change with the same weather/altitude blend), the vertex's fog-entry bit, FC = 0, the
  near/far choice per block, and `g_worldFog`.
- **Invalidations:** weather transitions (automatic by map position, or script requests), altitude changes of
  the camera target in the blend band, the story fog flag, a new data set (disc 1 / disc 4 maps), and nothing
  else. Block effects and the backdrop use their own matrices and do not disturb the block values.

## Proposed names (applied)
Code evidence only (`[code]`); none applied here (docs-only task).

| symbol | proposed | evidence |
|---|---|---|
| ovl_0c `D_800CD794` | `g_worldNormalTable` | 240 `SVECTOR` normals indexed by a vertex's pad byte in all six vertex routines (NCT input) |
| ovl_0c `D_800D23F0` | `g_worldVtxLitColors` | per-vertex lit colours written by the vertex pass (`src_equiv/ovl_0c/world_verts.c`) |
| ovl_0c `D_800D1FF0` | `g_worldVtxFogColors` | per-vertex fog colours, same |
| ovl_0c `D_800D29F8` | `g_worldVtxDepth` | per-vertex SZ (u32) or SZ / 8 (u16), same |
| ovl_0a `func_800C1CC0` | `fldbg_scanEntries` (dead) | the same dead type switch over `g_fieldBgCtx.entries` as `fldbg_draw` |
| ovl_0a `fldbg_setOverlayFlag1` | `fldbg_setOverlayScreen` | sets/clears `FLDBG_OVL_SCREEN` (0x1); op 0xE6 |
| define `BgOverlay.flags` 0x2 | `FLDBG_OVL_VISIBLE` | `fldbg_showOverlay`, `fldbg_draw`, `fldbg_updateOverlayAnims` |
| define `BgOverlay.flags` 0x20 | `FLDBG_OVL_ANIM_GROUP` | `fldbg_initEffectList` skips the group's steps; set on every flipbook step layer in the files |
| define `ModelObj.flags` 0x1 / 0x2 | `MDL_LOOK_AT` / `MDL_SCALED` | `fld_drawModels` picks `fldmdl_calcBoneMatricesLookAt`; `fldmdl_applyScale` / look-at variant scale |
| define `FieldEvtCtx.flags` 0x20000 | `FLD_OBJ_SEMITRANS_AGAIN` | `fld_applyModelCommands` sets it to repeat the semi-transparency write for the other buffer |
| `WmSky` `work[0..6]` comment | `work[0]` current, `[1]`/`[2]` low/high-altitude sky, `[3]`/`[5]` blend sources, `[4]`/`[6]` blend targets | `wm_weatherUpdate`, `wm_setWeather` |
