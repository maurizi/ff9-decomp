# Sound ids

All sound constants are in `include/snd_ids.h` (included by `ff9.h`). This page explains the groups and the evidence
for each one. Sound-effect ids are only named where their meaning is known; other literals stay as they are.

## Layers
1. **Mode sound hook.** Game code calls `GameData.soundHook(cmd, id, a, pan, vol)`. In practice that is `snd_playSe` /
   `snd_startSe` / `snd_stopSe`, or the mode's own handler directly: `fld_soundCommand` (field), `wm_soundCommand`
   (world map), `btl_soundCmd` (battle), `opening_soundCmd`, `tm_soundCmd` and `ending_soundCmd`. All of them
   decode `cmd` the same way: bits 12-13 type (0 music, 1 SE, 2 stream, 3 bank), bits 8-11 kind, bits 0-5
   sub-command, bit 7 resident SE slot. The `SND_*` words name these commands. The SE, state and volume words are
   named after the world map's debug strings (`WM_REQ_*` in ovl_0c). `SND_SONG_VOL_FROM_INTPL` (0xC401) and
   `SND_STREAM_VOL` (0x6201) are named from what the handlers do with them.
2. **AKAO API** (`src/main/akao_api.c`). Each sender fills `g_akaoArgs` and calls `akao_command(n)`.
3. **AKAO driver.** `akao_command` runs `g_akaoCommandHandlers[n]` (main 0x8006F740, 256 entries). `AKAO_CMD_*`
   takes its name from the handler. Where the handler is still a placeholder (`akao_cmdA2`, `akao_cmdE4`, ...), the
   name says what its body does: it writes `sePan`, `sePitch`, `AkaoSeq.pan` or `g_akaoStream` fields, or calls
   `akao_streamStart`/`Stop`/`PlayOneShot`/`PlayResident`. 0x98/0x99 and 0xD8-0xDA are combined commands that
   `akao_command` expands into two handler calls.

## SE play arguments
In `SND_SE_PLAY` the third argument is a channel mask (`snd_playSe` passes 0x800000). The fourth is the pan
(`VoiceParams.pan`, `SND_PAN_CENTER` 0x80) and the fifth is the volume (`& 0x7F`, `SND_VOL_MAX`).

## Sound-effect ids
Runtime evidence (snd_playSe traced in Redux while pressing buttons): `docs/runtime-findings.md`, "Sound ids".

| id | name | evidence |
|---|---|---|
| 0x65 | SE_CANCEL | runtime: Circle backs out, help closes |
| 0x66 | SE_BUZZER | runtime: unusable item, potion on a full-HP member |
| 0x67 | SE_CURSOR | runtime: cursor move, Cross pick |
| 0x6A | SE_USE_ITEM | code: item use (ovl_37), ability use (ovl_33) |
| 0x6B | SE_SET | runtime: equipment changed; code: chocograph set (ovl_32) |
| 0x2AA | SE_HELP_OPEN | runtime: Select opens help (field menu and battle) |
| 0x414 | SE_WINDOW_OPEN | runtime: battle command menu opens; code: save file list (ovl_39) |
| 0x416 | SE_MESSAGE | code: message windows (ovl_39, ovl_65) |
| 0x417 | SE_PAGE | runtime: menuList page, R1 member switch, battle side commands |
| 0x4ED | SE_COMPLETE | code: save/load done (ovl_39), right disc found (ovl_3e) |
| 0xC18 | SE_GAME_START | code: New Game (ovl_14), the ending's secret code (ovl_15) |
| 0x44A | SE_CHOCOBO_CALL | code: the "CHOCOBO"/"Kweeeh!" item-menu window on the world map (ovl_37), world SE 36 |
| 0x27C/0x27B/0x27A | SE_ENCOUNTER_1..3 | code: played together as a random battle starts (field, world map) |
| 0x79/0x78 | SE_ENEMY_DEATH_1/2 | code: btl_updateKo, enemy units only, as the body fades |
| 0xB5A..0xB5C | SE_FLEE_1..3 | code: btl_victory, BTL_END_ESCAPE |

Ids used by a single screen keep their names in that overlay's header: ovl_65 rewards (`SE_EXP_COUNT`,
`SE_LEVEL_UP`, ...), ovl_3c `SE_SHOP`, ovl_15 `BJ_SE_WIN`/`BJ_SE_LOSE` (0x6C/0x6B, the same samples as the rewards'
`SE_ITEMS` and `SE_SET`). ovl_0d's `TM_SE_*` values are indices into `g_tmSeIds`, not ids.

## Music ids
`MUSIC_VICTORY` 5 and `MUSIC_DEFEAT` 6 come from btl_playVictoryMusic's callers in btl_victory and the defeat exit
phase. `MUSIC_TETRA_MASTER` 0x42 is what ovl_0d's tm_init starts. The title and ending songs are db ids (`TITLE_MUSIC`
in ovl_14.h, `ENDING_MUSIC` in ovl_15.h).

## SPU
SPU registers use psx-spx names. `SPU_VOICE_*` are the per-voice register offsets used with `SPU_VOICE_REG(v, r)`.
`SPU_MAIN_VOL_MAX` 0x3FFF and `SPU_CD_VOL_MAX` 0x7FFF are the values akao_initState writes.
