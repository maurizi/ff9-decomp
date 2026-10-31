# Round 33 (main exe and overlays except ovl_0b): runtime questions

Code-only round. What was applied is in `docs/wip/names_r33_main_overlays.txt`. The fields this round marked
"write-only" / "not read by C" / "not used by C" in the headers have no reader in any C file (checked by renaming every
remaining unkXX field and building with `-k`). The rows below are the ones where a runtime check could still give a name.

| # | item | binary | what the code shows | how to observe | state |
|---|---|---|---|---|---|
| 1 | `GameData.flags` 0x40 | ovl_0a `fld_movieFinish`, `fld_updateEncounter` | only ever cleared (`~(GD_MENU_DISABLED \| GD_HIDE_ALL \| 0x40)`, `~(... \| GD_NO_FLIP \| 0x40)`); no C outside ovl_0b sets or tests it (ovl_0b's 0x40 is in the separate `battleFlags`) | `watch` writes to GameData +0 bit 6 through a field, a movie field and a battle | `field_dali_inn`, `field_d1_502` |
| 2 | `FieldMap.flags` 0x100 | ovl_0a `fld_updateEncounter` (cleared with FLD_MAP_NO_WINDOWS etc.) | no `FLD_MAP_*` for it, no C reader | `watch` FieldMap +0 (0x800D1000) bit 8 in a field with cutscenes | `field_dali_inn` |
| 3 | `FieldBgState.flags` 0xC00 | ovl_0a `fldbg_initOverlays` (clears it) | only cleared, no FLDBG_* bit there | `watch` g_fieldBgState +0 bits 10-11 | any field |
| 4 | `GameData.soundLoadBank` 0x84 | ovl_0a `fld_soundCommand` (no `akao_beginBankUpload` for bank 0x84) | a bank that skips the bank upload; which bank it is is not visible in the code | `trace` the load path with soundLoadBank 0x84; find the field that requests it | `field_dali_inn` |
| 5 | `AkaoStreamState.unk20` / `unk30` | main `akao_streamRefillRing`, `akao_cmdStreamSetupRing` | still write-only (round 31 row 5) | read watchpoints while a streamed FMV plays | `field_opening_dark` |
| 6 | `FieldActorExt.flags` 0x02000000 | ovl_0a `fld_modelCommand` 8-11 | set with FLD_EXT_BUILT, no C reader | read watchpoint on the word in a mirror scene | a field with a reflective floor |
| 7 | `GameState.mode` 5 / 6 | main `vib_update`, `msg_init`, ovl_0a `bgi_move`, ovl_64 `mbg_*` | compared but never written by C (the constants runtime pass never reached them) | none; left literal | - |
