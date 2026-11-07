# R46: runtime questions from the last-literals pass over the overlays

Round R46 (overlays: ovl_0a, 0c, 0d, 14, 15, 32-3e, 65) named fixed-point 1.0 as `ONE`, sizes and loop bounds as
`sizeof` / `ARRAY_COUNT` / `PARTY_SIZE` / `EQUIP_SLOT_COUNT`, the world sounds' SE ids (`SE_WM_*`, ovl_0c.h), the
world map's streaming counts (`WM_CHUNK_GRID`, `WM_CHUNK_BUFFERS`, `WM_BLOCK_SLOTS`, `WM_READTX_BUFFERS`), the weather
circles (`WM_WEATHER_SUNSET_A_*`, `_DARK_*`, `_SUNSET_B_*`, `_IIFA_*`), the field splash exit (`FLD_SPLASH_*`),
Garnet's haircut model swap (`FLD_MODEL_GARNET_LONG` / `_SHORT`, `STORY_GARNET_HAIRCUT`), the steep walkmesh test
(`BGI_IS_STEEP`), New Game's first field (`NEW_GAME_FIELD`), the boot screen TIM, and more. The rows below are what
the code alone doesn't settle. Each is a screenshot or a short trace.

Addresses: ovl_0a `fld_showSplashImage` 0x800B4A5C, `fld_needsSplashImage` 0x800B4F9C; ovl_0c `wm_getWeatherByPos`
0x800C291C, `wm_spawnEffect` 0x800C6F00 (a3 = type), `wm_updateBlockFx` 0x800C69F4, `w_frameSetParameter` 0x800B73F8,
`wm_naviUpdate` 0x800BEECC, `g_worldNaviPlaces` 0x800CD10C (`WmNaviPlace[2][64]`, 0xC bytes: s16 mapX, s16 mapY,
s32 worldX, s32 worldZ), `g_worldPlayer` 0x800CE3DC (x at +20, z at +28).

| # | symbol | binary / file | current name and what the code shows | what to observe | mode / state |
|---|---|---|---|---|---|
| 1 | `FLD_SPLASH_TIM` (DB_TIM \| 20000) | ovl_0a tu_800B0124.c `fld_showSplashImage` | drawn centred for 240 frames (fade in 60, hold 120, fade out 60) when field 52 "Prima Vista/Meeting" exits to field 100 "Alexandria/Main Street" (`FLD_SPLASH_FROM_FIELD` / `_TO_FIELD`) | break on `fld_showSplashImage`, step ~90 frames, screenshot: what the image is (the game's title logo?). Rename e.g. `FLD_TITLE_LOGO_TIM` | disc 1 New Game, or set fieldId 52 and trigger its exit (`wm_enter`-style poke of the field exit, docs/r32/3ds-requests-7.md recipes) |
| 2 | navi places 5 and 41 (page 0 / 1) | ovl_0c `g_worldNaviPlaces`, ovl_0c.h `WM_WEATHER_SUNSET_A_X` / `_DARK_X` (next to place 5 (328239, -245345)), `WM_WEATHER_SUNSET_B_X` (105 from place 41 (101014, -204347)) | the weather circles were named by result (sunset / dark) for want of the place names | read the place names (world text bank 0, entry place + 1, as R44 did for the `WM_NOCACHE_*` boxes) or move the full-map cursor onto places 5 and 41 and screenshot the name; then teleport the walker to (329728, -240896) and (128000, -202240) and screenshot the sky: rename the circles after what is there (e.g. a place with an everlasting sunset) | `cpass_wm_hadris` (disc 3) |
| 3 | navi page 1 places 6-9 and 15 | ovl_0c tu_800BA528.c `wm_naviUpdate` | `(u32)(i - 6) >= 4 && i != 15` on page 1: the full-map cursor never snaps or jumps to these places although they are listed | the names of page 1 places 6, 7, 8, 9, 15 (same text bank); if they are places that disappear later in the story, a name like `WM_NAVI_PAGE1_NO_SNAP_*` can say why | any world state (data only) |
| 4 | `WM_TOPO_BEACH_2` 0x1E, `WM_TOPO_BEACH_3` 0x23 | ovl_0c w_movement.c (`wm_controlWalk` effect spawn) | walking on them spawns `WM_FX_BEACH_SPLASH` like `WM_TOPO_BEACH` 0x22 | find a cell of each topography (R41b's topography sweep data, or trace `wm_spawnEffect` type 7 while walking along coasts and read `(u8)g_worldActorTypeInfo[type].cellAttr >> 2`), screenshot the ground: rename after it (e.g. rocky shore, snowy beach) | `cpass_wm_hadris` |
| 5 | `wm_spawnEffect` types 0, 1, 5, 8, 9, 13, 28 (script requests, `WM_REQ_SPAWN_EFFECT`) and 1, 3, 4, 24, 29 (block emitters); 11, 12, 17, 21 never spawned | ovl_0c wm_effect.c `wm_spawnEffect`, `wm_updateEffects`, `wm_drawEffects` | still numbers in the switch cases. Code: 8 is drawn flat (`g_worldEffectFlatRot`), 17 lives 9 frames like `WM_FX_FOREST_LEAVES`, 24 drifts (-x, +z, random), 32 dies 32000 units away from the player | for each script-requested type: which world script event requests it (`opscan.py c4000a` per type, R43 row 4 method), log `wm_spawnEffect` there and screenshot the effect; for emitter type 4 (`WM_BLOCKFX_STORY`, Kiera Desert, chance 32/4096) and 24 / 29 (Memoria orb block emitters, disc 4) screenshot with the effect forced (poke the emitter chance to 4096). Name each `WM_FX_*` after what it shows | `cpass_wm_hadris` (story 0x2500 for type 4), `r41b_wm_disc4_memoria_orb` |

Not asked: `STORY_GARNET_HAIRCUT` (10300, the same split as main chr_calcAppearance's CHR_KIND_GARNET_SHORT; the
haircut reading is from earlier runtime work, docs/runtime-findings.md near line 3358), `NEW_GAME_FIELD` 70 (script
name "Opening-For FMV"), `BOOT_SCREEN_TIM` (what opening_bootScreenLoad loads from FILE_BOOT_SCREEN), and the counts /
fixed-point names, which don't depend on game content.
