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
   takes its name from the handler. Where the handler is still a placeholder (`akao_cmdSetSePan`, `akao_cmdSetStreamVolume`, ...), the
   name says what its body does: it writes `sePan`, `sePitch`, `AkaoSeq.pan` or `g_akaoStream` fields, or calls
   `akao_streamStart`/`Stop`/`PlayOneShot`/`PlayResident`. 0x98/0x99 and 0xD8-0xDA are combined commands that
   `akao_command` expands into two handler calls.

## Sound command word fields
The event script's sound ops (`EVT_OP_SOUND`, `EVT_OP_SOUND_ARG*` in main event.c) read the 16-bit word from the
script and pass it unchanged, so script data uses the same encoding. The handlers switch on three fields:

| field | bits | macros | values |
|---|---|---|---|
| type | `SND_TYPE(w)`, `(w >> 12) & 3` | `SND_TYPE_*` | 0 music (`DB_MUSIC`), 1 SE (`DB_SE`), 2 stream (key 0x18xxxx, field only), 3 bank (`DB_SOUND_BANK`) |
| command | `SND_CMD(w)`, `(w >> 8) & 0xF` | `SND_CMD_*` | 0 play, 1 stop, 2 set, 3 fade, 4 fade from, 5 set all, 6 fade all, 7 load, 8 suspend, 9 resume, 10 set condition, 11 fade all from, 12 get state |
| parameter | `SND_PARAM(w)`, `w & 0x3F` | `SND_PARAM_*` | 1 volume, 2 tempo, 3 pitch, 4 tempo+pitch, 5 pan, 6 condition, 7 play count, 9 all resident slots, 10 movie volume |

Evidence is the akao_api call each case makes: set/fade/fade-from call `akao_set*`/`akao_fade*`/`akao_fade*From`
with (frames, target) or (frames, from, to); parameter 3 on an SE goes to `akao_sendCmdA4`/`A5`/`AC`/`AD`
(`AKAO_CMD_*_SE_PITCH`), parameter 5 to `akao_sendCmdA2`/`A3`/`AA`/`AB` (`AKAO_CMD_*_SE_PAN`). "All" on an SE
means every SE channel (no id lookup); on music it is handle 0, which the driver applies to the main song
whatever its id (akao_cmdFadeMusicVolume). Suspend/resume save the song (field: also its bank and file sectors)
or the resident SE slot, and resume only replays it on the same field or world. Get state steps the pending
bank/song load and returns non-zero until it is done (6: the music is still fading out). Bit 7
(`SND_RESIDENT`) plays the SE from a resident slot (field only), bit 6 (`SND_SLOT1`) picks slot 1 for play,
suspend and resume; other commands find the slot by id. Parameter 8 (always a no-op returning 0) and the
field stream's parameter 12 (stores `FieldScene.unk5F`) are left raw. `btl_soundCmd` (ovl_0b) decodes the same
fields; its switches are the ovl_0b owner's.

## Sequence opcodes
`AKAO_OP_*` / `AKAO_OPFE_*` cover only the sequence opcodes whose handler in `g_akaoOps` / `g_akaoOpsFE` has a name
(end, program, octave, tempo, jump, conditional jump, call, return). The others stay literals until their
handlers are named.

## SE play arguments
In `SND_SE_PLAY` the third argument is a channel group mask, `AkaoChannel.seMask` (`snd_playSe` passes `SND_SE_GROUP_MENU` 0x800000, the field resident slots `SND_SE_GROUP_SLOT(n)`; `AKAO_SE_DEFAULT` is another group bit). The driver's own per-channel bits are `AKAO_SE_CH_BIT(i)`. The fourth is the pan
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
| 0x6C | SE_ITEMS | code: items or a card received (ovl_65 rewards), world-map sound 2 (ovl_0c WM_SE_ITEMS) |
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
`SE_LEVEL_UP`, ...), ovl_3c `SE_SHOP`, ovl_15 `BJ_SE_WIN`/`BJ_SE_LOSE` (0x6C/0x6B, the same samples as
`SE_ITEMS` and `SE_SET`). ovl_0d's `TM_SE_*` values are indices into `g_tmSeIds`, not ids (except `TM_SOUND_BANK_SE`,
the card game's SE bank, next to `TM_SOUND_BANK_MUSIC`).

## World-map sound numbers
ovl_0c's world sounds are numbers that `w_musicGetID` maps to one or two SE ids (`WM_SE_*` in ovl_0c.h). Most are
requested only by the world script (op C4, `WM_REQ_SE_PLAY`); R43 logged `wm_sePlay` during each event
(docs/runtime-findings.md "R43 runtime pass: world map"):

| n | SE | name | event |
|---|---|---|---|
| 2 | 0x6C | WM_SE_ITEMS | each "Found N <item>!" line of a chocobo dig treasure |
| 3 / 4 | 0x27D / 0x27E | WM_SE_CHEST_OPEN / _2 | together, the dug-up chest opens |
| 6 | 0x552 | WM_SE_MOOGLE_APPEAR | the moogle arrives |
| 7 | 0x553 | WM_SE_TENT_START | "Rest" chosen at the moogle's tent question |
| 20 | 0x374 + 0x375 | WM_SE_FLEET_LOOP (low) | world file 9001, the disc 1 airship fleet cutscene |
| 21 | 0x376 + 0x377 | WM_SE_AIRSHIP_CUTSCENE_LOOP (low) | world files 9004-9006, one airship over the mist |
| 30 | 0x44B | WM_SE_DEAD_PEPPER | the chocobo's reaction to a Dead Pepper |
| 31 | 0x4CF | WM_SE_TENT_REST | the black screen before the tent night scene |
| 32 | 0x596 | WM_SE_MOOGLE_CALL | Square: Zidane calls the moogle |
| 33 | 0x37A + 0x37B | WM_SE_CHOCOBO_DIG | Square on the chocobo at a dig spot (with 36) |
| 35 / 40 | 0x37C / 0x37B | WM_SE_CHOCOBO_STEP_2 / _STEP (low) | chocobo step sound, by topography |
| 36 | 0x44A | WM_SE_CHOCOBO_CALL | SE_CHOCOBO_CALL |
| 37 | 0x37A | WM_SE_CHOCOBO_PECK | the chocobo pecks at the glowing ground |
| 41 | 0xA47 | WM_SE_CHEST_RISE | the dug-up chest shoots out of the ground |
| 42 | 0xC26 | WM_SE_CHEST_LAND | the chest lands next to the chocobo |

0 (SE 0x68, never requested) and 34 (0x44C, an unreached chocobo script tag) stay numbers. The vehicle and ambient
loops 22-26 and 38 are named from the code's tables (`WmMoveMode.loopSe`, `g_worldAmbientSe`).

## Music ids
`MUSIC_VICTORY` 5 and `MUSIC_DEFEAT` 6 come from btl_playVictoryMusic's callers in btl_victory and the defeat exit
phase. `MUSIC_TETRA_MASTER` 0x42 is what ovl_0d's tm_init starts. The title and ending songs are db ids (`TITLE_MUSIC`
in ovl_14.h, `ENDING_MUSIC` in ovl_15.h).
The world-map songs (`MUSIC_WORLD_MAP` 0x45, `MUSIC_CHOCOBO` 0x64, `MUSIC_AIRSHIP` 0x70, `MUSIC_WORLD_DISC4` 0x2D)
are ovl_0c's `g_worldSongMusicIds` entries by `WM_SONG_*`, each heard playing in R41b (docs/runtime-findings.md);
entries 4 and 5 (0x5F, 0x60, the "Field Song" slots) were not heard and stay numbers.

## Sound banks
`SND_BANK_COMMON` 0x84 is the resident system SE bank main's game_loadCommonAkao uploads at boot (the field skips
it). Per-mode banks are in the overlay headers: ovl_0b `BTL_SOUND_SE_BANK` 0x201, ovl_0c `WM_SOUND_BANK*`, ovl_0d
`TM_SOUND_BANK_MUSIC` 0xE2 / `TM_SOUND_BANK_SE` 0xFA.

Bank header ids fall in ranges (`AKAO_BANK_*_FIRST` / `_COUNT` in snd_ids.h), which akao_loadBankInSlot uses to place
a bank loaded into slot n > 0 above its header's SPU address. R44 (load traces at akao_loadBankInSlot, scan of the
bank files in FF9.IMG dir 11):

| range | name | header instrument / SPU | slot step | observed |
|---|---|---|---|---|
| 200-299 | `AKAO_BANK_BATTLE_SE_FIRST` | 192 / 0x4F100 (8 instruments) | 20 KB | only 208, loaded unslotted as the battle's own SE bank (low) |
| 300-499 | `AKAO_BANK_ENEMY_SE_FIRST` | 224 / 0x43100 (3 instruments) | 16 KB, loaded-bank index slot + 4 | 350 for the Malboro (battle 899), slot 0 (medium) |
| 500-599 | `AKAO_BANK_FIELD_SE_FIRST` | 200 / 0x6F100 (8 instruments) | 4 KB | 504 (field 2714), 511 (field 2600), slot 0 (medium) |

6xx-7xx banks (instrument 160, SPU 0x63100) have no slot rule.

## SPU
SPU registers use psx-spx names. `SPU_VOICE_*` are the per-voice register offsets used with `SPU_VOICE_REG(v, r)`.
`SPU_MAIN_VOL_MAX` 0x3FFF and `SPU_CD_VOL_MAX` 0x7FFF are the values akao_initState writes.
