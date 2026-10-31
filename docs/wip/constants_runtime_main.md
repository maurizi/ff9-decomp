# Main-exe constants that need a runtime check

Runtime results: docs/runtime-findings.md, "Constants runtime pass (2026-10-31)" (applied; the rows it left literal are listed there).

Values left as literals (or named only partly) because the code alone doesn't show what they mean.
Each entry: value, field, hypothesis, how to observe it. The emulator wasn't used for any of the names in
`include/ff9.h`; everything named there is from the code.

| value | field / argument | hypothesis | how to observe |
|---|---|---|---|
| 5 | `GameState.mode` | No overlay (game_loadModeOverlay's table has 0xFF) and no `game_main` case; `vib_update` vibrates even with vibration off in it, ovl_64 `mbg.c` and ovl_0a `bgi_move.c` treat it like the field. A debug/test mode that is never entered? | Watch writes to `g_gameState->mode` (byte at GameState+8) through boot, field, battle, world map, FMV. |
| 6 | `GameState.mode` | No overlay either; `msg_beginDraw` treats it like `GS_MODE_BATTLE` (packets to D_8007B9B0). | Same watch. |
| bits 11-13 | `GameState.config` | 3-bit setting that indexes `g_btlBannerDurations` (ovl_0b `btlui_frame.c`): probably the Config screen's battle message speed row. | Change each Config row in the menu and diff `config`. |
| 0-9 | text palettes: `text_drawBlock` / `ui_drawSmallDigits` last argument, `g_textColor`, text code `TEXT_COLOR + n` | Index into `g_textPalettes` (CLUT rows 0x3827 ... 0x3FA6 of the font). 0 is the default (98 of 125 `text_drawBlock` calls); the colours of 1-9 (grey, yellow, red, ...?) are only visible in VRAM. | Dump the font CLUT rows (VRAM y = (clut >> 6), x = (clut & 0x3F) * 16) or screenshot a menu text drawn with each palette. |
| 0-7 | `ui_drawDigits` / `text_buildDigits` style | Index into `g_numberClutRows` (styles 0-2 row +0, 3 +0x40, 4 +0x80, 7 +0xC0); 2 is also drawn dimmed and semi-transparent. | As above, for the digit font CLUT. |
| 1 | `ui_drawLabel` mode | 0 = full brightness, 2 = dimmed (win_drawTitle of an inactive window); 1 = the CLUT 0x40 rows down with semi-transparency and no colour: what does it look like? | Screenshot a label drawn with mode 1 (ovl_37 / ovl_3c titles). |
| 0x30-0x33 | `text_buildCode` codes / `ui_drawSprite` ids 4-7 | Button glyphs in text (codes 0x30-0x33 draw UI sprites 5, 4, 7, 6). Which button is which? | Show a message with each code (e.g. the tutorial texts) and screenshot. |
| 1, 0x10 | `ModelSlot.flags` | 1 is cleared for every slot by fld_exit / btl_cleanup / wm exit; 0x10 is set by EVT_OP_JUMP and cleared at the end of the jump animation. Readers are in ovl_0a (model drawing) through other views. | Watch `chrModels[i].flags` while an actor jumps and across a field exit. |
| 0x40 | `GameData.flags` | Unnamed bit cleared with the GD_HIDE_* bits by ovl_0a `fld_movie.c` (`&= ~0x5D`, `&= ~0x7F`). | Watch who sets bit 6 of GameData.flags (field, FMV). |
| 1/2 | `AkaoFlags.mode` | Output mode set by `akao_cmdSetOutputMode1/2` (stereo / mono?); nothing in C reads the value. | Toggle the Config screen's Sound row and watch the word. |
| 1, 2, 4 | `AkaoChannel.legato` | 1 legato on (op CC), 2 the current note is tied, 4 set by op D0 for sound effects. | Trace op D0 users (SE data). |
