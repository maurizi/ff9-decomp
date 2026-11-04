# R41 overlay constants: runtime questions (ovl_0a, ovl_0c, small overlays)

Names added in R41 whose define comment says `(runtime: pending R41)`, plus literals left as numbers that a cheap check
could name. Each entry: symbol and binary, where the code is, current/proposed name, what to observe, and how to get
there. World map recipes: docs/emulator.md "World map anywhere (disc 3)" (`cpass_wm_hadris`, `cpass_wm_airship9`,
teleport = write the player object's x/z at `*0x800CE3DC` + 20 / + 28, zero encounters first) and
`tools/rt/wm_enter.py` / `tools/rt/wm_force.py` (any world file and story counter, R37b). Story counter =
`g_evtGlobalVars.h[0]` at 0x8007AEF0. Don't duplicate docs/wip/constants_runtime_overlays.md (answered separately).

## ovl_0c (world map)

### Q1. World-map sound numbers (w_musicGetID 0x800C3FE4, src/ovl_0c/w_music.c:37)
- Named by code/data only: `WM_SE_INVINCIBLE_LOOP 22`, `WM_SE_HILDA_GARDE_LOOP 23`, `WM_SE_BOAT_LOOP 24` (the move
  modes' loopSe), `WM_SE_RUMBLE_LOOP 25` (g_worldAmbientSe[2]), `WM_SE_VORTEX_LOOP 26` (g_worldAmbientSe[0] and [1]).
- Unnamed: 0, 2, 3, 4, 6, 7, 20, 21, 30, 31, 32, 33, 34, 35, 37, 40, 41, 42.
- Observe: break on w_musicGetID (or wm_sePlay 0x800C4718 / wm_seFadeVolume 0x800C4850) and log a0 with what is on
  screen, in `cpass_wm_hadris` (walk, call the moogle with Square, open/close the navi map with Select, Triangle
  chocobo menu in `r37b_wm_chocobo_ride`), `r37b_wm_boat_entry`, `cpass_wm_airship9` (take off, land, fly over
  water), `r37b_wm_invincible_entry` (disc 4). Most numbers come from the world scripts (WM_REQ_SE_PLAY: a0 =
  g_worldReqParams[0]); note the event (dig, landing, chocobo call). Listen only if a name needs it.
- For 25/26: teleport next to each ambient source and confirm a0 = 26 / 25 at wm_sePlay; screenshot. Sources (x, z):
  0 (229279, -198701), 1 (130781, -227819), 2 (299201, -93859); ranges are in `pos >> 8` units (63 = on).

### Q2. WM_TOPO_PUSHBACK 0x17 and ambient source 1 (wm_updateMap 0x800BB07C, tu_800BA528.c:256)
- Code: within 0x22 (pos >> 8) of ambient source 1 at (130781, -227819), a walker/chocobo on topography 0x17 is pushed
  toward +z every frame; wm_findGetoffPos refuses within 0x16.
- Observe: teleport the walker to (130781, -227819 - 0x1800) in `cpass_wm_hadris`, run 120 frames, screenshot; read
  the topography (g_worldActorTypeInfo[type].cellAttr >> 2) and the player z per frame. What place is it (a whirlwind,
  a sandstorm, a cliff)? Proposed rename from what is seen.

### Q3. Chocobo call cells WM_TOPO_CHOCOBO_CALL_A..E (3, 0x12, 0x15, 0x16, 0x1C) (wm_canCallChocobo 0x800B8124)
- Observe: walk onto chocobo tracks on each continent (Mist continent tracks near Chocobo's Forest, the Outer Continent
  tracks near Chocobo's Lagoon, the Forgotten Continent's near Chocobo's Air Garden, and any others) and read the
  topography; screenshot the cell. Do the five differ by region? Proposed `WM_TOPO_CHOCOBO_TRACKS_<REGION>`.

### Q4. WM_TOPO_NO_LANDING 0x2D / _2 0x2E (wm_findGetoffPos 0x800B105C, w_movement.c:1414)
- Code: the airships can't set down on them. Observe: in `cpass_wm_airship9`, fly over cells with topography 0x2D/0x2E
  (R37's `build/r37/topo.csv` sweep may list positions), screenshot; press Cross to land and confirm the refusal. Also
  the no-landing circle around (49120, -121408) radius 1440 (x >> 3 - 0x17FC, z >> 3 + 0x3B48 < 0xB4): what is there?

### Q5. Story points WM_STORY_TITLE_0..3 (0x960, 0x1766, 0x2585, 0x26A2) (wm_setupWorldData 0x800BA58C, tu_800BA528.c:97)
- Code: entering the world map with the story counter exactly at one of these sets g_worldTitleId (0x800D3E75) 0-3 and
  loads title images WM_ITEM_NAVI_TIMS 5-12.
- Observe: `tools/rt/wm_enter.py --poke 0x8007AEF0=<value>` for each; screenshot the title shown on entry (area or
  continent name?). Proposed names from the title text, e.g. `WM_STORY_TITLE_<CONTINENT>`.

### Q6. Fog and data-set story points (wm_getStoryFog 0x800C3E88)
- `WM_STORY_FOG_END 0x1766`, `WM_STORY_FOG_BACK 0x2B53`: no fog in [0x1766, 0x2B53). Confirm with `wm_enter.py`
  pokes 0x1765 / 0x1766 / 0x2B53 (screenshot: is "fog" the Mist?). If it is the Mist, rename to
  `WM_STORY_MIST_GONE` / `WM_STORY_MIST_BACK`. medium priority.

### Q7. Block effects (wm_updateBlockFx 0x800C69F4, wm_drawBlockFx 0x800C5A48, wm_effect.c)
- `WM_BLOCKFX_CLAY 4`: five spinning models at (229007, 0, -198927), drawn on data set 1 until form CLAY. Teleport
  near it on disc 1-2 story (`wm_enter.py --poke 0x8007AEF0=0x1000`), screenshot (Cleyra's sandstorm?). Also check
  effect type 23 (`WM_FX_CLAY_SWIRL`) in the active list (g_worldEffectActive 0x800D53A0) there.
- `WM_BLOCKFX_DISC4_CORE 6`: data set 4 only, at (196602, -10882, -82002) (also where effect types 25/26 turn to);
  disc 4 world (`r37b_wm_invincible_entry`), fly there, screenshot (Memoria / the light at Iifa?).
- `WM_BLOCKFX_SPINNER 7`: one model at (280140, -7784, -206465), turning except in [0x1B4E, 0x2B52); screenshot it with
  story 0x1000 and 0x2000 (a windmill? a propeller?).
- `WM_BLOCKFX_STORY 3`: emits only with story in [0x24EA, 0x26A3); find its block: break on wm_updateBlockFx with
  a0 == 3 at story 0x2500, log the player position, screenshot the effects (patch wm_drawEffects off to diff).
- `WM_BLOCKFX_GULUG_SUMMIT 2` / effects `WM_FX_RISING_PUFF(_2)` 2/3: R37 saw the emitter at Mount Gulug's summit but
  not the effect. Teleport the airship above (129057, -2400, -29156) looking down, ablate wm_drawEffects, diff
  (smoke?). Rename to `WM_FX_SMOKE` if so.

### Q8. Resident world music WM_MUSIC_RESIDENT 0x64 (wm_loadResidentMusic 0x800A8F18, wm_main.c:150)
- Code: DB_MUSIC | 0x64 goes to GameData.musicSeq2 (slot 1). Observe: after it runs, which song plays from slot 1
  (GameData.musicId[1])? Probably the chocobo or airship theme (WM_SONG_* through g_worldSongMusicIds, 0x800CDF30, is
  filled at runtime; read the six ids too and match them to MUSIC_* names / the songs heard).

### Q9. g_worldSongMusicIds contents (0x800CDF30)
- The overlay's initial bytes are not music ids; read the six u16 after wm_init in `cpass_wm_hadris` and on disc 4,
  so WM_SONG_* can be tied to snd_ids.h MUSIC_* names (World map theme, Chocobo theme, airship theme, disc 4 theme,
  the two field songs of WM_STORY_FIELD_SONG1/2).

### Q10. EVT_GB_WM_FILE values 0 and 8 (wm_updateMap tu_800BA528.c:263, w_frmbuf.c:181)
- Code: world file 0 or 8 rains around (247888, -816, -186968) within 0x47C (pos >> 3); file 0 patches preset 3's fog on
  the 10th frame. Observe b[0x66] (0x8007AF56) against GameData.worldId (+0x7DC) for 9000-9012 (R37b's wm_force.py);
  screenshot the rain spot. Proposed `WM_FILE_*` names for the two files.

## ovl_0a (field)

### Q11. FLD_SOUND_BANK_NO_UPLOAD 0x84 (fld_soundCommand 0x800B0124, tu_800B0124.c:452; fld_soundBankReadCallback 0x800B29F8)
- Code: a sound bank (DB_SOUND_BANK id 0x84) that is read but never uploaded to the SPU. Observe: break in
  fld_soundCommand's SND_LOAD_START with GameData.soundLoadBank == 0x84 (scan field music changes, or `tools/rt/varscan.py`
  for music whose dep link is bank 0x84); which field/music, and does the music play? Proposed name from it.

## Small overlays
- No runtime questions: ovl_39 STORY_DISC2/3/4 (menuSave_getDiscBit, SaveHeader.discs) and ovl_64 MBG_FRAME_* are code.
  GameState.mode 5/6 and GameData.battleResult 2/5 stay literal (constants pass, no writer reached).
