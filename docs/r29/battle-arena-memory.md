# R29 items 3 and 8: when a battle arena is still, and ovl_0b's memory

Answers to ff9-ds request R29 (2026-10-30), items 3 ("when is a battle arena still") and 8 ("ovl_0b's memory map
with lifetimes"). Everything here is read from the code (ovl_0b unless a file says otherwise) and from the four
discs with `tools/battle_budget.py`; nothing was observed at run time. Background format and loading:
`docs/battle-backgrounds.md`; models: `docs/battle-models.md`.

## Summary for the port

**Item 3.**
- `btlbg_draw` (btlbg.c:38) changes four things between calls: the **sky** (layer-8 records turn about y), the
  **texture animations** (VRAM `MoveImage`s, item 0x19), **uv scrolls** (supported, no file has one) and the
  **groups 1+** of six hard-coded scene ids. A fifth thing, rain streaks, comes from the place that started the
  battle (`GameData.rainCount`), not from the background. Otherwise the arena only changes when the view matrix,
  the projection or `g_btlbgBrightness` changes.
- Over the 178 files: **80 static, 68 sky-only, 30 animated** (`battle_budget.py --motion`, table below). 25
  files have texture animations, all looping, and 6 move groups (32, 156, 157, 160, 162, 172).
- The sky turns slowly. 76 of the 80 files with a non-zero `skySpeed` use 3: the matrix angle `g_btlbgSkyAngle >> 3`
  moves one unit (360/4096 degrees) every 8/3 calls, a full turn in 10923 calls. The exceptions are 40 (-90), 46
  (-120), 58 (128) and 160 (-50).
- **Depth rule.** Only layer 0 is depth sorted against the units: it goes into the frame's scene OT at index
  average SZ / 16, the same scale as unit polygons (`btl_drawMesh`). Layers 8, 4 and 2 go into the background's own
  256-entry OT, which `btl_endFrame` draws in full **before** the scene OT (btl_main.c:2162-2163). So sky, layer-4
  scenery and the ground (layer 2) are always behind every unit, effect and layer-0 polygon. Inside that OT the sky
  sits at a fixed entry (0xFF, farthest), the ground at a fixed entry (0, nearest), and layer 4 is sorted (average
  SZ / 256).
- **Layer 0 can sit in front of units.** It holds the props around the arena. In the median file 42% of the
  polygons are layer 0. Examples: Evil Forest arenas 26, 27 and 29 (Prison Cage, Plant Brain, Fang: 337, 482 and
  424 layer-0 polygons), 50 (622), 28 (555), 88/91 (532). Only 37, 58, 151 and 176 have none.
- **`g_btlbgSkyOwnProj`** (header `skyOwnProj`, 60 files) draws the sky with screen distance 0x200 instead of
  `GameData.projDist`. Its translation is (0, the camera's y, a z between 12500 and the camera's z): the camera's x
  is ignored, and when the camera is level the z is pinned at 12500. So the sky has no sideways parallax and only
  follows the camera's rotation.
- **Effects that draw the background themselves.** `btlfx_drawBgOffscreen` (btlfx_op.c:1172), called by 13 effect
  modules, draws the ground layer (layer 2 only) from a fixed top-down view into an off-screen VRAM rectangle,
  immediately (`DrawOTagEnv`). As a side effect that call advances the sky angle, the texture animations, the uv
  scrolls and `g_btlbgFrame` a second time in that frame. Effects also change the arena's colours:
  `g_btlbgBrightness` (`btlfx_opFadeBackground`). Every spell cast dims it to 0x40 or 0 (black: nothing is
  drawn) over 12 or 20 frames, unless the effect header's bits 8-9 are 3.

**Item 8.**
- The overlay itself is 0x800A7000-0x80105800. `.data` is 0x800F51E8-0x80105800 (67,096 bytes): 0x8C76 bytes of
  initialised tables and text, then 0x79A2 bytes that are zero in the file (bss-like). `.text` (314 KB) is dead in
  a port that doesn't run MIPS, except for the three data words in `docs/text-data.md`.
- The battle claims RAM from g_loadAreaEnd + 0x61800 = **0x80108800** to **0x801DC800** at fixed offsets
  (`btl_allocBattle`, `btl_allocBuffers`). Above that are the loader/effect work buffer (0x801DD700, 0xA000), the
  effect module area (0x801E7700-0x801F1700) and the effect data ring (0x801F1700-0x801FDF00).
- **Largest dead or reclaimable ranges** (detail in "Reclaimable memory"):
  - ovl_0b `.text`: 314 KB.
  - 2 x 0x20000 primitive buffers: real use is at most about 0x1A000 each, before effects.
  - The party buffers' free tails: at least 0x27D8 each.
  - 0x80105800-0x80108800 (12 KB) and 0x8010B070-0x8010E000 (about 12 KB): nothing in battle uses them.
  - 0x801DC800-0x801DD700 (3.75 KB).
  - The help texts in `.data` (about 17 KB of constant text, only read while the help window is open).
  - Load-only staging, which is free after the intro: the common data file's items 0x1E onwards, the background
    file's TIMs, and the texture halves of the enemy, weapon and party model files.
- **Worst use against the fixed buffers** (`battle_budget.py --memory`, an upper bound from the disc data):
  - bundle 0x2800 of 0x2800 (battle 942)
  - background resident part 0x52A8 of 0x6000 (battle 303)
  - enemy model heap 0x34000 of 0x35800 (battle 936)
  - texture staging 0x22000 of 0x4C000 (battle 338)
  - packet bytes with every face drawn: 0x32B88 against one 0x20000 primitive buffer (battle 51). Back faces are
    culled, so the real figure is about half: ~0x19600 plus effects.
- **OTs.** The background OT is 0x100 entries per buffer, and layer 4 can reach all of them. The scene OT (exe,
  `GameData.ot`) is 0x1010 entries per buffer, starting 8 entries before `ot[0]`. Every index from -8 to 0xFFF is
  reachable (depth / 16, minus `otBias`).

---

## Item 3: when is a battle arena still?

### What changes from one `btlbg_draw` call to the next
`btlbg_draw(view, bgData, prims, sceneOt, bgOt)` runs once per battle frame from `btl_drawScene` (btl_cmd.c:52)
and from `btl_updateIntro` (btl_update.c:341), with the frame's view matrix. Per call:

| what | where | driven by | per file |
|---|---|---|---|
| `g_btlbgFrame++` | btlbg.c:61 | call | used only by scene 7's jitter |
| nothing drawn if the low byte of `g_btlbgBrightness` is 0 | btlbg.c:62 | effects | - |
| uv scrolls: u/v bytes of the geometry += du/dv, wrap or bounce | btlbg.c:75-95 | call (paused by `g_btlbgScrollPaused`, no caller) | `scrollCount` = 0 in every file |
| texture animations: `btl_updateTexAnims` (btl_model.c:116), VRAM `MoveImage`s | btlbg.c:96-98 | call; 20.12 position advances by `speed` per call | 25 files |
| rain: up to 31 random `LINE_G2`s in scene OT entry 2, new every call | btlbg.c:101-103, 272-313 | call, `rand()` | `GameData.rainCount`, from the place |
| sky: `g_btlbgSkyAngle += skySpeed`, rotation `angle >> 3` about y | btlbg.c:110-118 | call, once per call, only at the first layer-8 record | 139 files have sky records, 80 a non-zero speed |
| groups 1+: matrix per group | btlbg.c:147-260 | `VSync(-1)` (scene 7: `rand()` and `g_btlbgFrame`) | moving in 6 files |

Notes:
- Group motion follows the VBlank counter, not the battle frame, so it keeps real-time speed when the battle slows
  down. The sky and the texture animations advance per call.
- The **default group case** (btlbg.c:242-247) is not motion. `rot2` is cleared once per call, so group k
  (k >= 1) of any other multi-group file gets the fixed offset (0, -10, 0) and a fixed y rotation of 3k units. Only
  files 177 and 178 (scene 177, used by no encounter) take that path.
- The moving scene ids (BtlBgHeader.id, not the file id):
  - 0x44: z sway, sin of VSync x 26 / 5 (file 32)
  - 0x70: groups 1 and 3 spin about z and y and bob, group 2 bobs (file 156)
  - 0x6E: z sway (file 157)
  - 7: groups 1 and 2 get a random y angle every call, base re-rolled every 64 calls (`g_btlbgJitterBase1/2`,
    file 160)
  - 0xAB: z spin and y spin (file 162)
  - 0xA8: y spin, VSync / 16 and / 8 (file 172)
- The sky's rotation matrix stays loaded after the first sky record (btlbg.c:117), as does its translation with
  `skyOwnProj` (btlbg.c:121). A later non-sky record of the same group would be drawn with them. No file has one
  (`--motion` checks: "none").
- `btlbg_init` (btlbg.c:3) starts every texture animation (flag 1, position 0, `lastFrame` 0x1000, so the first
  call uploads frame 0). It also resets the sky angle, takes the rain count (at most 31) and sets the OT shift to 0
  and the layer mask to 0xF.
- Texture animations on the discs come in two kinds:
  - "loop": frames 3-22, a new frame every 1-7 calls, no random waits.
  - "scroll" (flag 4, no frames): the rectangle moves up `pos >> 12` rows per call with two `MoveImage`s, i.e.
    every call when the speed is at least 0x1000, otherwise every 0x1000/speed calls.

  None stops ("once") and none is static. They change VRAM only: the geometry stays the same, but a captured
  image of the arena is stale whenever one steps.

### Classes (`tools/battle_budget.py --motion`)
- **static** (80): no sky turning, no texture animation, no moving group. The packets only change with the view
  matrix, the projection distance, `g_btlbgBrightness` and the rain.
- **sky-only** (68): as static, but the sky records turn. With speed 3 the matrix changes every 8/3 calls by
  0.088 degrees (less than a pixel at screen distance 0x200).
- **animated** (30): texture animations (VRAM) and/or moving groups, with or without the sky.

Static: 0 1 3 4 6 9 11 12 15 16 26 27 28 29 33 35 37 38 39 41 42 47 49 50 51 52 53 56 59 60 63 69 71 72 73 74 75 76
80 85 117 120 122 123 124 126 127 128 129 130 131 132 133 134 135 137 138 139 143 144 145 146 147 148 149 150 152 153
155 164 165 167 168 170 171 173 174 175 177 178.

Sky-only (sky speed 3 unless noted): 2 8 10 13 14 17 18 19 20 21 23 24 30 40 (-90) 43 46 (-120) 54 55 57 61 62 64 65 66
67 68 77 78 79 81 83 84 86 87 88 89 90 91 92 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 111 113 114
118 119 140 141 154 158 159 161 166 176.

Animated (the `--motion` rows; layer polygons are sky/l4/l2/scene; texture animations "loop F x C" = F frames, a
new one every C calls, "scroll R rows" = rows moved per call):

| bg | scene | moves | sky/l4/l2/scene | sky deg/call | own proj | texture animations | groups | encounters |
|---|---|---|---|---|---|---|---|---|
| 5 | 9 | tex | 0/0/165/214 | 0 | no | loop 16x2 | 1 | 74 |
| 7 | 66 | tex | 0/146/176/280 | 0 | no | loop 4x1 | 1 | 73 916 |
| 22 | 57 | sky+tex | 136/401/72/192 | 0.033 | yes | scroll 3 rows | 1 | 297 |
| 25 | 56 | sky+tex | 120/0/451/326 | 0.033 | yes | scroll 2 rows | 1 | 300 |
| 31 | 67 | tex | 0/126/152/394 | 0 | no | loop 8x2 | 1 | 83 92 98 |
| 32 | 0x44 | groups | 16/367/144/112 | 0 | no | - | 2: z sway | 84 91 97 |
| 34 | 69 | tex | 28/281/199/235 | 0 | no | scroll 0.2, 0.2, 2, 0.5 rows | 1 | 86 102 |
| 36 | 71 | tex | 0/108/169/494 | 0 | no | scroll 4 rows, loop 3x3, scroll 0.7 rows | 1 | 88 94 95 |
| 44 | 52 | tex | 0/148/184/492 | 0 | no | loop 8x2 | 1 | 56 313 318 |
| 45 | 51 | tex | 0/192/185/279 | 0 | no | loop 8x4 | 1 | 314 |
| 48 | 29 | tex | 0/189/170/388 | 0 | no | loop 16x1 | 1 | 101 325 327 328 |
| 58 | 82 | sky+tex | 252/173/131/0 | 1.406 | no | scroll 6 rows | 1 | 118 127 |
| 70 | 4 | tex | 0/20/140/409 | 0 | no | loop 7x1 | 1 | 334 335 |
| 82 | 137 | sky+tex | 144/62/204/422 | 0.033 | yes | loop 18x3, loop 18x3 | 1 | 5 180 232 234 259 827 |
| 93 | 128 | sky+tex | 144/42/204/422 | 0.033 | no | loop 18x3, loop 18x3 | 1 | 145 175 222 228 258 828 |
| 110 | 155 | sky+tex | 144/106/200/296 | 0.033 | no | loop 11x4 | 1 | 133 655 671 811 813 |
| 112 | 147 | sky+tex | 144/116/200/296 | 0.033 | yes | loop 11x4 | 1 | 284 659 674 814 815 |
| 115 | 164 | sky+tex | 144/106/200/296 | 0.033 | no | loop 11x4 | 1 | 740 752 |
| 116 | 143 | sky+tex | 144/106/200/296 | 0.033 | no | loop 11x4 | 1 | 741 758 |
| 125 | 32 | tex | 0/274/112/166 | 0 | no | loop 7x1 | 1 | 468 475 499 912 |
| 136 | 92 | tex | 0/119/188/345 | 0 | no | scroll 0.05 rows | 1 | 203 858 |
| 142 | 108 | tex | 11/124/121/272 | 0 | no | loop 16x2 | 1 | 871 874 880 883 |
| 151 | 42 | sky+tex | 141/336/258/0 | 0.033 | yes | scroll 0.1 rows | 1 | 0 |
| 156 | 0x70 | groups | 80/16/80/336 | 0 | no | - | 4: spin + bob | 925 953 |
| 157 | 0x6E | groups | 80/124/80/231 | 0 | no | - | 2: z sway | 568 924 |
| 160 | 7 | sky+tex+groups | 636/0/172/24 | -0.549 | no | scroll 2 rows, loop 12x2, loop 22x2 | 3: y jitter (rand) | 931 |
| 162 | 0xAB | groups | 168/325/84/97 | 0 | no | - | 3: spin | 937 |
| 163 | 172 | sky+tex | 190/76/0/278 | 0.033 | yes | loop 12x7 | 1 | 938 |
| 169 | 118 | tex | 86/97/154/190 | 0 | no | scroll 4 rows | 1 | 948 957 |
| 172 | 0xA8 | groups | 160/0/0/160 | 0 | yes | - | 2: y spin | 191 952 960 |

The full per-file table (all 178, with layers, sky speed and own projection) is `battle_budget.py --motion`; the
same data is under `backgrounds[].motion`, `tex_anim_list` and `records_after_sky` in `--json`.

### `btlbg_drawRecord`: the depth rule
`btlbg_drawRecord` is hand-written asm; its C twin is `src_equiv/ovl_0b/background.c:121`.
`btlbg_projectVertices` leaves SZ >> 4 per vertex in the scratchpad. For a quad, z = the sum of its four
(SZ >> 4); for a triangle, z = ((z0 + z1) / 2 + z2) / 2 of the (SZ >> 4) values. The triangle value is weighted
towards its third vertex: it is not the mean.

| layer | OT | entry (index) | culled |
|---|---|---|---|
| 8 sky | background OT (`Battle11C0.ot`, 0x100) | fixed 0xFF (byte 0x3FC), the farthest | back faces only |
| 4 | background OT | quads (sum >> 6), triangles z >> 4, i.e. average SZ / 256; clamped to 0xFF | back faces, index 0 |
| 2 ground | background OT | fixed 0, the nearest | back faces only |
| 0 scene | the frame's scene OT (`GameData.ot[drawBuf]`, shared with units, shadows, effects) | quads (sum << shift) & ~3 bytes = sum / 4, triangles z << shift; i.e. average SZ / 16; byte offsets with bit 14 or 15 set clamp to 0xFFF | back faces, depth 0 |

background.c:153-178 (quads) and 216-239 (triangles).
- `g_btlbgOtShift` is always 0. `btlbg_init` sets it, and `btlbg_setOtShift` has no other caller (none in the
  effect modules either), so the shift never changes the scale.
- Unit meshes use the same scale. `btl_drawMesh` links quads at (sum of 4 SZ) >> 6 and triangles at AVSZ3 >> 2,
  into `ot - otBias` (`src_equiv/ovl_0b/mesh_packets.c:23`, `docs/battle-models.md`). A layer-0 polygon and a unit
  polygon at the same depth land in the same entry, less the model's `otBias`. Shadows (`btl_drawShadow`,
  btl_main.c:1608) use the same OT.
- **Draw order.** `btl_beginFrame` (btl_main.c:2129) clears both OTs reversed (`ClearOTagR`), so entry 0xFF (or
  0xFFF) is drawn first and entry 0 last; within one entry the packet added last is drawn first. `btl_endFrame`
  (btl_main.c:2147) closes the background OT with a DR_STP in entry 0xFF, then calls `btlui_drawOTagEnv(&bgOt[0xFF],
  drawEnv)` (a `DrawOTagEnv` with the frame's DRAWENV) and then `DrawOTag(&sceneOt[0xFFF])`. The whole
  background OT (sky, then sorted layer 4, then ground) reaches the GPU before anything of the scene OT.
- **So layers 8, 4 and 2 can never cover a unit, and layer 0 can.** Layer 0 is the scenery around and inside the
  arena that the cameras look past. Its extents match the ground's, for example:
  - file 26: layer-0 records span x -4539..5114, z -4215..6303, up to y -3501;
  - file 27: six layer-0 records from y 0 up to -4217 (tree trunks and canopy).

  The units stand near the middle of the ground (layer 2: x ±1750, z ±3500 in both files). When a camera is
  placed beyond a prop, the prop's polygons are nearer and are drawn over the unit. Which encounters actually show
  that depends on the camera scripts; it was not checked at run time.
- Layer 4 sorts only among itself (and the sky and ground). Layer 2 always covers layers 4 and 8, whatever the
  depth. `btlfx_drawBgOffscreen` relies on that: it picks layer 2 as "the ground".
- Brightness: each packet's colour is the record colour scaled by `g_btlbgBrightness` (GPF, then >> 7), unless the
  brightness is 0x80. A brightness change therefore rewrites every packet's colour.

### `g_btlbgSkyOwnProj`
`btlbg_init` sets it when header +6 is non-zero (60 files, every one with sky records). For layer-8 records
(btlbg.c:119-127), `btlbg_draw` first builds `m2`, the translation from the view matrix `view` (btlbg.c:65-74):
- `t.x = 0`
- `t.y = view.t.y`
- `t.z = ((0x1000 - d) * 12500 + view.t.z * d) / 0x1000`, where `d = |view.m[2][1]|`

It then draws the sky with that translation, the rotated view (`view x Ry(angle >> 3)`) and `SetGeomScreen(0x200)`,
and restores `GameData.projDist` afterwards. In effect:
- the sky ignores the camera's x translation;
- its depth is 12500 when the camera is level (`m[2][1]` = 0) and moves towards the camera's z as the camera
  pitches;
- it is projected with the fixed distance 0x200, not the battle's. So it doesn't zoom with camera zooms that change
  `projDist`, and it behaves like a distant dome that follows the camera's rotation only.

Without the flag the sky records are ordinary geometry with the full view matrix (parallax and zoom included),
still at the background OT's last entry.

### Effects that draw or change the background
- **`btlfx_drawBgOffscreen(x, y, w, h, ot, cx, cz, dist)`** (btlfx_op.c:1172, export +0x2F4 in
  `include/btlfx_mod.h`) does the following:
  - builds a DRAWENV for the VRAM rectangle (x, y, w, h), clearing it to (7, 7, 7) (`isbg`);
  - clears the module's 0x100-entry OT;
  - sets screen distance 0x200 and offset (w/2, h/2);
  - draws `btlbg_draw(&D_800F8D50, ..., NULL, ot)` with layer mask 2, so **only the ground records**, with rain
    switched off;
  - submits it at once with `DrawOTagEnv`, then restores the DRAWENV, projection and offset.

  `D_800F8D50` is a fixed top-down view: rows (1,0,0), (0,0,-1), (0,1,0), translation (-cx, cz, dist + 0x2900).
  The result is a plan-view picture of the floor around (cx, cz), used as a texture by the effect.
  - Callers, all once per effect (init or frame 0): fx_03a, fx_062, fx_063, fx_07a, fx_0d2, fx_0e2, fx_114, fx_12c,
    fx_186, fx_1a8, fx_1b2, fx_1bd, fx_1fc.
  - Rectangles: 0x70-0x100 square at x 0x1C0-0x300, y 0x100-0x1C0.
  - **Side effects on the arena.** The call runs the whole per-call part of `btlbg_draw`: `g_btlbgFrame++`, uv
    scrolls, `btl_updateTexAnims` and, for a file with sky records, `g_btlbgSkyAngle += skySpeed` (the angle step
    is outside the layer-mask test, btlbg.c:111-118). In the frame of the call the texture animations and the sky
    advance twice. Its packets come from `g_btlfxEnv->prim`, the frame's primitive buffer.
  - The earlier statement in `docs/battle-backgrounds.md` that no effect module reaches the background is corrected
    there: they reach it through this export.
- **Brightness.**
  - `btlfx_startFadeBackground(k, frames)` (btlfx_op.c:841) spawns task 0xA, `btlfx_opFadeBackground`
    (btlfx_op.c:851), which interpolates `g_btlbgBrightness` and sets it to 0 below 0x10.
  - The cast sequence calls it on the first frame of the caster's cast phase (btlfx_op.c:418-421) with k = the
    effect header's bits 8-9. `D_800F8D0C` = {0x40, 0}: k = 0 dims to half, k = 1 goes black (`btlbg_draw` then
    draws nothing), k = 3 keeps the arena. k = 2 would read past the table (into `g_btlfxResidentOps`); whether any
    effect header uses it was not checked.
  - The fade takes 12 frames for a kind-1 cast and 20 otherwise. The same task is resident effect op 0x2A
    (`g_btlfxResidentOps[10]`), so effect scripts set and restore the brightness themselves. fx_0ba and fx_19f also
    call the export directly (restore: `startFadeBackground(-1, n)` = 0x80).
- **Layer mask**: only `btlfx_drawBgOffscreen` changes it (to 2 and back to 0xF).
- **Covering the arena**: many modules draw full-screen tiles (`btlfx_drawScreenTint`, 8 TILEs into scene OT
  entry 1, 112 modules) and VRAM moves (`btlfx_drawScrollMove`: DR_MOVEs in scene OT entry 0xFFF, i.e. after
  the background OT and before every scene packet; 67 modules). Modules with their own `StoreImage` / `MoveImage` /
  `SetDrawMove`:
  - `StoreImage`: fx_007 048 095 097 098 0d3 0d6 0e1_0 114 186 1bd 1cc 1fe
  - `MoveImage`: fx_001 08e 090 095 112
  - `SetDrawMove`: fx_026 07d 07e 0b3 0cb 0d3 0df 0e2 114 138 18b 196 197 1bd 1fd

  Whether a given call reads the displayed frame (and so the drawn arena) depends on its rectangle; that was not
  traced.

---

## Item 8: ovl_0b's memory, with lifetimes

### The battle's RAM map
`g_loadAreaEnd` is 0x800A7000 (initialised in the exe, never written), so every address below is fixed. Sizes
are hexadecimal.

| range | size | what | set up by | lifetime |
|---|---|---|---|---|
| 800A7000-800A859C | 159C | ovl_0b `.rodata` (jump tables, strings) | overlay load | code; port: dead |
| 800A859C-800F51E8 | 4CC4C | ovl_0b `.text` | | port: dead except 800CF588, 800CF5A0, 800CFA80-800CFC0C (`docs/text-data.md`) |
| 800F51E8-800FDE5E | 8C76 | `.data`, initialised: text, tables, pointer tables | | constant (table below) |
| 800FDE60-80105800 | 79A0 | `.data`, zero in the file: state and buffers | | per battle / per effect (table below) |
| 80105800-80108800 | 3000 | nothing (overlay end to `BattleMem`) | - | not referenced by ovl_0b or the effect modules: dead |
| 80108800-8010ABCC | 23CC | `Battle` (`BattleMem.battle`, `GameData.ctx.battle`): units, commands, `statusInfo`, `abilities` (copied from common data), `statusSets`, popups | `btl_allocBattle` (btl_main.c:167) | whole battle mode |
| 8010ABCC-8010AC1C | 50 | `BtlFiles` (registered file pointers), `partyBufEnds` | `btl_allocBattle` | battle mode |
| 8010AC1C-8010AEAC | 290 | common data file 0x271B (dir 1), items before 0x1E | `btl_loadCommonData` (btl_main.c:233) | battle mode (registered) |
| 8010AEAC-... | 2E800 total | the rest of file 0x271B: item 0x1E (portraits), 0x14 (party names), 0x1F4-0x1F6 (copied into `Battle`), 0x70005 (music), sound banks 0x9009A/0x90201, TIM 0x42774 | `btl_initMode` (btl_main.c:130) | **load only**: consumed by `func_800A8F18`-`func_800A9090`, then `savedHeap` = item 0x1E and the space is reused |
| 8010AEAC-8010AEF8 | 4C | `Battle11C0` work header | `btl_allocBuffers` (btl_main.c:346) | per battle |
| 8010AEF8-8010AF08 | 10 | `weaponBufs[4]` pointers | | per battle |
| 8010AF08-... | 0xAC x n | `npcModelObjs` (0x98) and `npcModels` (0x14) for n = `evt_countNpcActors(0)` | `btl_load` (btl_main.c:259) | per battle |
| ...-8010B070 (n = 0) | 168 + 0x28 n | shadow slots (`func_800AD4AC` → main `func_80062E3C`, 9 + n x 0x28, no packets) | `btl_load` | per battle |
| 8010B070-8010E000 | 2F90 - 0xD4 n | nothing (`freeHeap` is stored and never read) | - | **unused**, about 12 KB |
| 8010E000-80110800 | 2800 | encounter bundle (`sceneTables` = `Unk800AA08C.sceneBuf`): script, texts, encounter, sequences, dependency table | `btl_loadEncounterFile` (btl_main.c:559) | per battle (registered) |
| 80110800-80116800 | 6000 | background, items up to the first TIM (`bgData`) | `btl_loadBackgroundStep` | per battle |
| 80110800-80148000 | up to 37800 | the whole background file as read: the TIMs behind the resident part | | **load only** (uploaded in `btl_setupBackground`, then overwritten by the heap) |
| 80116800-8014C000 | 35800 | work heap (`heapStart`/`heap`): enemy model parts (registered), copies of enemy texture tables (`btl_getModelTexInfo`) | `btl_loadEnemyModelsStep` (btl_main.c:684) | per battle; script sound loads also stage at the heap cursor; `func_800E87C4` (summons) unloads the enemies and moves summon data to `heapStart` |
| 8014C000-80198000 | 4 x 13000 | `partyBufs[i]`: party model file part; last 0x28 bytes = `Battle1538` allocation header | `btl_allocBuffers` | per battle; staging during load; summons take them over |
| 80198000-8019C000 | 4 x 1000 | `weaponBufs[i]`: weapon model part (first 0x1000 of the file) | | per battle |
| 8019C000-8019C800 | 2 x 400 | background OT per buffer (0x100 entries) | | per frame |
| 8019C800-801DC800 | 2 x 20000 | `prims[2]`: the frame's packets (background, units, shadows, effects, popups, banner) | | per frame |
| 801DC800-801DD700 | F00 | nothing | - | unused in battle |
| 801DD700-801E7700 | A000 | `loadBuf` (g_loadAreaEnd + 0x136700) | `btl_allocBuffers` | per battle: loader states at +0x9FA0 (CD request) and +0x9FD8 (step state); `g_btlfxEnv->workBuf`; effect sound (chunk 7) and buffer (chunk 8) data; data indices >= 0xC0 of `btlfx_getFileData`; party texture staging |
| 801E7700-801F1700 | A000 | effect module area (two halves at 801E7700 / 801EC700) | `btlfx_updateLoader` | per effect; also staging for common effect data file 0x2BF (VRAM pages up to 801F7B00, `btlfx_reloadCommonData`) and for VRAM tables (`btlfx_runScript` StoreImage) |
| 801F1700-801FDF00 | C800 | effect data ring (common data at its start when the file says so) | `btlfx_updateLoader` (btlfx_pool.c:362) | per effect |
| 801EDF00- | | ovl_65 (battle end) | `btl_preloadExit` (btl_main.c:2174) | after the battle |

Outside this range, a battle also uses the exe's scene OTs (`GameData.ot`, 0x1010 entries x 2 =
0x10100 bytes; per `docs/runtime-findings.md` the frame OTs are at 0x8008BB58 / 0x8008FB98) and the exe's
`GameData.packetBuf` (0x8000), which the UI takes as its heap (`btlui_initHeap`, btlui.c:3203).
- UI allocations from `btlui_alloc`: 0x12D8 `g_btlUi`, 0x100 help text, 0x2E0 items, 0x8C panel, 0x368 prims, 0xC0
  + 0x4000 UI draw buffers. That is 0x5A84 used of 0x8000.

### Load order and staging
`btl_load` (btl_main.c:259) runs synchronously. `btl_loadModelsStep` (btl_main.c:1207) then runs once per frame
from the intro update until everything is in. The order:
1. `btl_initMode`: `btl_loadCommonData`, uploads and copies, then `savedHeap`.
2. `btl_load`: `btl_allocBuffers`; bundle to 8010E000; background file to 80110800 and its TIMs to VRAM.
3. Per frame:
   - **sounds** (`btl_loadSoundsStep`): staged at `partyBufs[0]`, then uploaded to the SPU;
   - **enemy models** (`btl_loadEnemyModelsStep`): every 0x02 key of the bundle's dependency table, in sector
     order. The model part goes to the heap cursor. The texture part is staged from `partyBufs[0]` upwards
     (`dst += size`), then each TIM is uploaded;
   - **weapons**: 0x1000 to `weaponBufs[i]`, textures staged at `partyBufs[i]`;
   - **party models**: the model part to `partyBufs[i]`. The texture part goes to another slot's buffer still
     unloaded, or to `loadBuf`.

So the party buffers and the end of the heap are staging areas until the party is in. After that, the only buffers
written outside load are the heap (script sound loads, summons), `partyBufs` (summons: `btlfx_updateLoader` chunk 4
calls `btl_unloadPartyModels` and loads the summon at `partyBufs[0]`; the party is reloaded afterwards), `loadBuf`
and the effect areas.

Script sound loads go through `btl_soundCmd`, kind 3/7 (btl_main.c:1886-1898), and take one of two places:
- slot < 4 (from `arg2 % 4` in `btl_soundCmd`): staged at `work->heap`, with chunks of up to `ot[0] - heap`
  (rounded down to sectors). The span reaches through the party and weapon buffers, so the chunk size, not the
  space, bounds what is overwritten.
- slot >= 4: staged in the free tail of `partyBufs[slot % 4]`, at most 0x12FD8 minus what is used.

### ovl_0b `.data` buffers of 256 bytes or more
Sizes run to the next label, so a size can include an unlabelled neighbour. Users are from the C files (all of
them have C now).

| address | size | symbol | contents / users | lifetime |
|---|---|---|---|---|
| 800F524C | 50C | `D_800F524C` | party motion file ids, [kind][0x22] (`btl_loadPartyMotions`) | load (and party reloads) |
| 800F5858 | 110 | `g_btlseqOps` | function pointers (`btlseq_update`) | constant (code pointers) |
| 800F5968-800F6A28 | 10C0 (9 tables of 140-1DC, plus `D_800F650C`) | `D_800F5968` ... `D_800F684C` | status-icon sprite animations, reached through the pointers in `g_btlStatusIcons` (800F6A90-800F6B40) | constant, per frame (UI) |
| 800F6CB4 | 304 | `D_800F6CB4` | message table (`TextTbl`): element names, then `btl_showMessage` texts | constant |
| 800F6FB8 | 16C | `D_800F6FB8` | Scan / Detect texts (`btl_startDetect`, `btl_updateScan`) | constant |
| 800F7510 | 360 | `g_btlfxModuleExports` | 216 function pointers + data pointers for the effect modules | constant (port: its own table) |
| 800F78B4 | 1400 | `D_800F78B4` | common effect data, self-relative table (`g_btlfxCommonTable`, `btlfx_getCommonData`) | constant, per effect |
| 800F909C | 648 | `D_800F909C` | ability names | constant, UI |
| 800F96E4 | 1B0C | `D_800F96E4` | ability help texts (`btlui_getAbilityHelpText`) | constant, help window only |
| 800FB1F0 | 300 | `D_800FB1F0` | ability help offsets | constant, help window only |
| 800FB4F0 | 1E7C | `D_800FB4F0` | item / throw help texts | constant, help window only |
| 800FD36C | 200 | `D_800FD36C` | item help offsets | constant, help window only |
| 800FD56C | 174 | `g_btlCmdTable` | command table | constant |
| 800FD7AC | 120 | `g_btlCmdNames` | command names | constant |
| 800FD8CC | 444 | `g_btlCmdHelp` | command help texts | constant, help window only |
| 800FDF60 | 400 | `g_btlClutBuf` | two CLUT rows, `StoreImage` → grey → `LoadImage` inside one `btl_makeGreyCluts` call | **scratch** (one call) |
| 800FE530 | 708 | `D_800FE530` | vibration data copied from the effect file (`btlfx_runScript` case 15, `vib_init`) | per effect (while it vibrates) |
| 800FEC68 | 800 | `D_800FEC68` + `D_800FF068` | the effect file's sector 0: chunk directory (`btlfx_updateLoader`), and from +0x400 the effect script (`d->pc = D_800FF068`) | per effect |
| 800FF470 | 1800 | `D_800FF470` | effect task heap (`btlfx_initTasks`, `btlfx_allocTaskMem`) | per effect |
| 80100C78 | 110 | `g_btlfxTasks` | 11 task slots | per effect |
| 80100E30 | EC | `g_btlfxHaltBufs` | DRAWENV/DISPENV of `btlfx_halt` (error loop) | **dead** in normal play |
| 80100F1C | 500 | `g_btlfxEffModels` | 32 effect-model slots | per effect |
| 801014B8 | 3000 | `g_btlfxParticleBuf` | particle pool storage | per effect |
| 801044C8 | 920 | `g_btlfxEmitters` | emitter pool | per effect |
| 8010528C | 8 | `g_btlBannerOt` | one OT entry per buffer | per frame |
| 80105294 | 56C | (no label) | nothing references it | **dead** |

Everything else in the zero part (800FDE60-80105800) is small state: camera, UI, background, loader and banner
globals, all under 0x100 bytes. "Per effect" means the buffer is only meaningful while an effect runs (between
`btlfx_start` and its end). Between effects it holds nothing that is read again, but the next effect re-initialises
it at a time that is not known in advance, so a port can share it only with memory that is free whenever an effect
can start.

### Primitive buffers and OTs against their worst use
- **Packets.** `work->prims[drawBuf]` (0x20000 each) holds, per frame, in this order:
  - the units (`btl_updateUnitModel` → `btl_drawMesh`, front faces only, plus 8 bytes per mesh group) and their
    shadows and weapon models;
  - the background (`btlbg_draw`: FT4 0x28 / FT3 0x20 per front face, rain 31 x `LINE_G2` + 3 mode packets);
  - effects (`btlfx_runFrame`, `g_btlfxEnv->prim`);
  - popups (`btl_drawPopups`) and the banner.

  Nothing checks for overflow. `battle_budget.py --memory` adds, for each encounter, the background
  (all records), the heaviest enemy group that can appear (any group) and the party bound (the four heaviest
  bodies' `packetSize` + 4 x the heaviest weapon's = 0x10CC0), with every face drawn:
  - 474 of 562 encounters exceed 0x20000 with all faces; none exceeds it at half.
  - The worst is battle 51 (Mimic + 3 Magic Vice, background 15): 0x5798 + 0x1C730 + 0x10CC0 = 0x32B88.

  With back faces culled (about half of a closed model, and of most arena geometry, faces away) the units and
  arena of the worst battle need about 0x19600, leaving roughly 0x6A00 (27 KB) for effects, popups and the banner.
  The real peak depends on the camera and the effects and was not measured.
- **Background OT**: 0x100 entries per buffer. Layer 4 reaches every index (average SZ / 256, clamped to 0xFF),
  and the sky and ground use 0xFF and 0.
- **Scene OT**: 0x1010 entries per buffer, cleared from `ot - 8` (btl_main.c:2130) and drawn from entry 0xFFF.
  - Unit polygons can reach -8 (`otBias` up to 8 entries below 0).
  - Layer 0, units and shadows reach up to 0xFFF (average SZ / 16; layer 0 clamps there).
  - Fixed entries: rain uses 2, cursors 2, screen tints 1, the frame STP 0, DR_MOVEs and the end STP 0xFFF.
  - Entries 0x1000-0x1007 are cleared but never drawn.

### Reclaimable memory, largest first
For a port that keeps the PS1's memory layout. Sizes in bytes.

| range | size | why it is free | when |
|---|---|---|---|
| ovl_0b `.rodata` + `.text` 800A7000-800F51E8 | 0x4E1E8 (320 KB) | code, never executed by the port (keep the 3 `textbin` ranges) | always |
| party buffers' tails | at least 0x27D8 each (largest party model part 0x10800 of 0x12FD8) | free until a summon or a script sound load uses them | between loads; summons need the whole 0x4C000 |
| model heap tail 8014C000 - heap cursor | 0x1800 at worst (battle 936), usually far more (`--memory`) | above the last enemy model | after load, unless a script sound load or a summon (`func_800E87C4`) uses it |
| `prims` slack | ~0x6A00 per buffer at the worst battle | see above | per frame |
| 80105800-80108800 | 0x3000 | between the overlay and `BattleMem`, not referenced | always in battle |
| 8010B070-8010E000 | 0x2F90 - 0xD4 per NPC actor | between the shadow slots and the bundle area; `freeHeap` is never read | always in battle |
| help texts in `.data` | 0x1B0C + 0x300 + 0x1E7C + 0x200 + 0x444 = 0x42CC (17 KB) | constant, read only by the `btlui_get*HelpText` functions while the help window is open | could stay compressed or on disc |
| `D_800FF470` + particles + emitters + effect models + file sector + vibration | 0x1800 + 0x3000 + 0x920 + 0x500 + 0x800 + 0x708 = 0x6528 | per effect state | only while no effect runs |
| 801DC800-801DD700 | 0xF00 | between `prims[1]` and `loadBuf` | always in battle |
| 80105294-80105800 | 0x56C | unreferenced `.data` | always |
| `D_800F524C` | 0x50C | read only when party motions are (re)loaded | after load (keep for reloads) |
| `g_btlClutBuf` | 0x400 | one-call scratch | any time outside `btl_makeGreyCluts` |
| `bgData` slack | 0x6000 - resident part (0xD58 at worst) | above the background's resident items | after load |
| `g_btlfxHaltBufs` | 0xEC | error path only | always |

Load-only staging that is free after the intro: the common data file from 8010AEAC (0x2E570), the background
TIMs, the enemy texture parts (up to 0x22000 in the party buffers), the weapon and party texture parts, and
`loadBuf` until the first effect uses it. A port that loads the files itself can keep none of it.

### Text data and effect-module regions
- Code regions read as data (`docs/text-data.md`): only ovl_0b's hand-written background asm keeps data in `.text`:
  - `g_btlbgBrightness` at 800CF588 (initial 0x80);
  - `g_btlbgOtShift` at 800CF5A0 (initial 0);
  - the reciprocal square root table 800CFA80-800CFC0C.

  Effect modules keep none.
- Effect modules (`docs/ff9img.md` "Battle effect files", `docs/effect-modules.md`):
  - Code loads at **0x801E7700**, or at **0x801EC700** for the odd groups of kind 1 (summons alternate halves).
    Ordinary spells take the whole 0xA000.
  - Effect data goes to the ring at 0x801F1700 (0xC800 bytes; common data first when the chunk flag says so).
  - Modules reach ovl_0b only through `g_btlfxModuleExports` (`env->unk10`), and the scan found no direct
    reference from a module into ovl_0b's `.data` or the battle buffers. Their own variables are absolute
    0x801E/0x801F addresses inside their chunk.
  - The area is also used as staging: the common effect file 0x2BF is read to 0x801E7700 and uploaded as four
    64x128 VRAM pages plus a CLUT strip from 0x801F7700 (`btlfx_initCommonData`, `btlfx_reloadCommonData`), and
    `btlfx_runScript` pulls the effect header table from VRAM (0x300, 0x140) into 0x801E7700 between effects.

---

## tools/battle_budget.py
```
python3 tools/battle_budget.py --motion              # per background: class, moving parts, layers, sky, texture animations, groups
python3 tools/battle_budget.py --motion --bg 160     # one file
python3 tools/battle_budget.py --memory [--top 20]   # per encounter: bundle, bg resident, model heap, texture staging, packets
python3 tools/battle_budget.py --json out.json       # adds backgrounds[].motion/tex_anim_list/records_after_sky/packet_bytes,
                                                     # encounters[].memory, party.estimate.packet
```
- `--motion` prints a class per file:
  - "static": nothing changes;
  - "sky-only": layer-8 records exist and `skySpeed` != 0;
  - "animated": texture animations, uv scrolls or moving groups.

  It also prints the parts (sky, tex, scroll, groups), the layer polygon counts, the sky's degrees per call, the
  own-projection flag, each texture animation (loop: frames x calls per frame; scroll: rows per call) and the group
  motion. It ends with a check for non-sky records after a sky record (none).
- `--memory` sums, per encounter, the parts the loader puts in each fixed buffer: every model key of the bundle's
  dependency table, split into the model part (heap) and the texture part (staging). The packet column is the
  all-faces upper bound described above. Model bytes are whole sectors as read, so they are upper bounds of what
  stays resident: the heap cursor ends at the end of the model's 0x1B item.

## Proposed names (applied)
Evidence is the code cited above (tag `[code]`); none was checked at run time.

| symbol | proposed | evidence |
|---|---|---|
| `D_800F524C` | `g_btlPartyMotionIds` | `btl_loadPartyMotions`: `db_lookupRegistered(4, D_800F524C[kind][i] \| 0x30000)` |
| `D_800F6CB4` | `g_btlMessages` | `TextTbl` read by `btl_showMessage` / `btl_getElementName` |
| `D_800F6FB8` | `g_btlScanTexts` | `text_getFrom` in `btl_startDetect` / `btl_updateScan` |
| `D_800F78B4` | `g_btlfxCommonData` | `btlfx_initCommonData`: `g_btlfxCommonTable = D_800F78B4` |
| `D_800F909C` | `g_btlAbilityNames` | `btl_getAbilityName`: `abilities[i].nameOfs + D_800F909C` |
| `D_800F96E4` / `D_800FB1F0` | `g_btlAbilityHelp` / `g_btlAbilityHelpOfs` | `btlui_getAbilityHelpText` |
| `D_800FB4F0` / `D_800FD36C` | `g_btlItemHelp` / `g_btlItemHelpOfs` | `btlui_getItemHelpText`, `btlui_getThrowHelpText` |
| `D_800F8D0C` | `g_btlfxCastBgBrightness` | `btlfx_startFadeBackground(k)`: target brightness {0x40, 0} |
| `D_800F8D50` | `g_btlfxBgTopView` | `btlfx_drawBgOffscreen`: top-down view matrix for the ground snapshot |
| `D_800FE530` | `g_btlfxVibData` | `btlfx_runScript`: copy of 0x708 bytes passed to `vib_init` |
| `D_800FEC68` | `g_btlfxFileHeader` | `btlfx_updateLoader`: `cd_readAsync(sector, 0x800, D_800FEC68)`, block count and commands |
| `D_800FF068` | `g_btlfxFileScript` | `btlfx_runScript`: `d->pc = D_800FF068` (second half of the header sector) |
| `D_800FF470` | `g_btlfxTaskHeap` | `btlfx_initTasks`: `btlfx_heapInit(&D_800FF470, 0x1800)` |
| `func_800AD4AC` | `btl_allocShadows` | `btl_load`: `p->shadows = g_modeHeap; g_modeHeap = func_800AD4AC(g_modeHeap, npcCount + 9)` |
| `func_800E87C4` | `btlfx_moveSummonToHeap` | unloads the enemy models, copies the summon's `data[0]` to `work->heapStart` and repoints it |
| `Battle11C0.sceneTables` | `bundle` | the encounter bundle buffer (`Unk800AA08C.sceneBuf` at the same offset, `btl_loadEncounterFile`) |
| `Battle11C0.freeHeap` | (note only) | written by `btl_load` / `btl_initReturnInfo`, never read |

## Not verified
- Which encounters' cameras actually put a layer-0 prop between the camera and a unit (needs the camera scripts
  or a run).
- Real per-frame packet use (front faces depend on the camera); the figures above are bounds.
- Whether any effect header uses cast-fade k = 2.
- Whether `npcCount` is ever non-zero in battle (it sizes the NPC arrays after the work header).
