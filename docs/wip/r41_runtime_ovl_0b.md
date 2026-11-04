# R41 constants pass, ovl_0b: runtime questions

Names the R41 pass gave from code (and offline data) whose meaning depends on game content. Each entry: symbols,
where, current name, what to observe, and how to get there. Answers go to docs/runtime-findings.md as usual; the
decomp side renames afterwards. Motion slots 8 / 0x12 / 0x13 / 0x1D / 0x1E / 0x20 are in docs/wip/literals_runtime.md
and are not repeated here.

Resolved offline during the pass (no question needed):
- Cast kinds `BTLFX_CASTK_*` (white / summon / black / enemy / short summon / none): the effect header table is
  FILE_BTLFX_COMMON + 0x2000, identical to the R31 runtime dump `build/r31rt/fxhdr.bin`.
- `BTLCAM_RADIUS_WORM_HYDRA` 1239: the only enemy model with that radius is 340, Worm Hydra (tools/battle_models.py).
- `BTL_COMMON_*` db keys: the kind-0 block of FILE_BTL_COMMON (0x271B) holds exactly 0x14, 0x1E, 0x1F4-0x1F6.
- `BTLFX_EFFECT_BLOOD_SWORD` 0x16D: g_btlfxWeaponScripts[0x13], item 0x13 is "Blood Sword".

## Q1. Moving battle-background scenes: which location is each? (`BTLBG_SCENE_*`)
- Binary / code: ovl_0b, src/ovl_0b/btlbg.c:154 (`btlbg_draw`, `switch (g_btlbgHeader->id)`), defines in
  include/ovl_0b.h after `BtlBgHeader`.
- Current names describe the motion: `BTLBG_SCENE_JITTER` 7, `_SWING` 0x44, `_SWAY` 0x6E, `_TUMBLE` 0x70,
  `_SLOW_SPIN` 0xA8, `_ROLL` 0xAB, all marked (runtime: pending R41).
- Offline data (tools/battle_budget.py, tools/encounter_catalog.py):

  | scene id | name now | bg file | encounter(s) to enter | field that starts it | disc |
  |---|---|---|---|---|---|
  | 7 | JITTER | 160 | 931 (Nova Dragon) | 2756 | 4 |
  | 0x44 | SWING | 32 | 84 (Armodullahan), 91, 97 | 1423 | 2 |
  | 0x6E | SWAY | 157 | 568 / 924 (Ash, Veteran) | 2902 | 4 |
  | 0x70 | TUMBLE | 156 | 925 / 953 (Ash, Veteran) | 2906 | 4 |
  | 0xA8 | SLOW_SPIN | 172 | 191 (Hades), 952, 960 | 2914 / 2921 | 4 |
  | 0xAB | ROLL | 162 | 937 (Trance Kuja) | 2927 | 4 |
- Observe: `tools/rt/enter_battle.py ENC --field F --disc N ...` (docs/emulator.md "Entering any battle"; a disc-4
  base state such as the Crystal World / Treno saves, a disc-2 one for 0x44), wait for the battle HUD, read
  `g_btlbgId` (0x800FE50C) to confirm the scene id, and take two screenshots ~1 s apart. Report the location (the
  field's name as the game shows it, e.g. from the field before the battle) and what the moving group is (a floating
  rock, a turning ring, ...). Proposed rename: `BTLBG_SCENE_<LOCATION>` (or `<LOCATION>_<PART>` if two share one).

## Q2. `BTL_SOUND_BASE_BANK` (0x201): which sounds are in the base bank?
- Binary / code: ovl_0b, src/ovl_0b/btl_main.c ~line 635-655 (`btl_loadSoundsStep`, states 2 and 5): file 0x201 of
  DIR_SOUND_BANKS is always the first id in the list and is loaded with `akao_loadBankRetry` (0x80055D80, no slot)
  instead of `akao_loadBankInSlot` (0x80056048).
- Observe (any battle, e.g. `battle_iifa_start` reload or `enter_battle.py 899 --field 2714 --base-state
  disc3_pandemonium_lv99_field`): break on 0x80055D80 during the battle load and note it is hit once per battle
  (also for a world-map battle). Then compare: with the call skipped (return immediately at 0x80055D80), which sounds
  are missing in battle (menu cursor/confirm, hits, cast sounds of g_btlfxSoundSets, victory)? Proposed name from
  the result, e.g. `BTL_SOUND_COMMON_BANK` if it holds the common battle SE.

## Q3. Eiko's cast sound set (`BTLFX_SNDSET_CAST_EIKO`) and the cast textures
- Binary / code: ovl_0b, src/ovl_0b/btlfx_op.c `btlfx_startCastEmitter` (0x800E4A3C): for kinds WHITE / BLACK it
  plays sound set 1 when the caster's kind is g_btlfxUnitKinds[0] (= 10, CHR_KIND_EIKO), else set 0; set 2 for
  summons, set 3 per reflected target (`btlfx_playSoundSet` 0x800E49D0, a0 = set).
- Observe: trace 0x800E49D0 (a0) while (a) Eiko casts Cure, (b) Dagger or Vivi casts a white/black spell, (c) an enemy
  casts, (d) a summon. Expected a0: 1, 0, 0, 2. If Eiko's set is really hers, keep the name; if set 1 turns out to be
  e.g. "the White Magic caster's set" with g_btlfxUnitKinds[0] read differently, report the value read at 0x800F8D70.
- Same session, cheap extra: screenshot the cast loop of Cure (WHITE: textures common 7 + 6) and Fire (BLACK: 8 + 6)
  and a summon (0x14 + 6 + 2) to name `BTLFX_COMMON_TEX_CAST_A` / `_CAST_B` / `_BLACK_CAST` / `_SUMMON_A` / `_SUMMON_C`
  by what they look like (glow, ring, sparks, ...). Each particle's texture is `emitter->elems[i].tex`.
