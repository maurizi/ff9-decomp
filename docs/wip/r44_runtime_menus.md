# R44: runtime questions from the menu / minigame layout pass

**Status: answered and applied** (docs/runtime-findings.md, "R44 runtime pass: layout and tuning"): #1
`CONFIG_SPRITE_PORT1` 0xF8 / `CONFIG_SPRITE_PORT2` 0x100 (bit set = controller 2; the flag argument dims, comments
fixed), #2 `WM_CHOCOGRAPH_PICTURE_X/Y`, `WM_CHOCOGRAPH_CLUT_X/Y` (res_ids.h, used by ovl_0c wm_setupWorldData and
ovl_32), #3 `MENU_HELP_MARGIN_X` 0xC (ff9.h, ovl_37 item and ovl_33 ability menus).

Round R44 named the screen layout (window rects, OT slots, rows and columns, cursor positions), timings and sprite
sheet coordinates of the menu and minigame overlays (ovl_14, 15, 32, 33, 34, 36, 37, 38, 39, 3a, 3b, 3c, 3e, 64, 65).
Nearly every name follows from the code together with the screen's own text and labels (`g_label*`, the menu text
banks) and the existing `UI_SPRITE_*` ids. The rows below are the few where the code shows the mechanism but not
what the player sees. Save states: `docs/emulator.md` ("Disc 1 menu states": `menu_config`, `menu_chocograph2`,
field/world states).

| # | symbol | binary / file | current name and what the code shows | what to observe | mode / state |
|---|---|---|---|---|---|
| 1 | `CONFIG_SPRITE_PORT_SET` 0xF8, `CONFIG_SPRITE_PORT_CLEAR` 0x100 | ovl_34 `menuConfig_drawBattleControl` (ovl_34.c), ovl_34.h | the Battle Control "Custom" window draws, per recruited character, sprite 0xF8 at x+0x60 lit when the character's bit in `config >> CFG_SHIFT_CHAR_PORT` is set and sprite 0x100 at x+0x92 lit when it is clear. Which icon is controller port 1 and which port 2? | `menu_config`: move to Battle Control, set Custom, press Cross to open the window; screenshot. Toggle one character (Left/Right) and screenshot again, and read `g_gameState->config` (GameState at `*0x8006794C`, `config` field) before and after. Rename to `CONFIG_SPRITE_PORT1` / `_PORT2` (or the reverse) and say which bit value means port 2 | `menu_config` |
| 2 | `CHOCO_WM_CLUT_X/Y` (0x280, 0xF6), `CHOCO_WM_PICTURE_X/Y` (0x318, 0xB0) | ovl_32 `menuChocograph_restoreVram` (ovl_32.c), ovl_32.h | on the world map, closing the Chocograph menu first copies the selected chocograph's CLUT and 42x28 picture to these VRAM spots (MoveImage), then restores its own picture area. What does the world map draw from there: the chocograph shown while digging with Chocobo, the hint window? | world map with a chocograph selected (`menu_chocograph2` pokes three chocographs; or a world state with a chocograph owned, story >= Chocobo's Forest): open the Chocograph menu (Triangle menu > Item? / Chocobo menu), select one, close; dump VRAM around (0x318, 0xB0) and (0x280, 0xF6) and screenshot the world screen; then ride the chocobo and dig. Name after what uses it (e.g. `CHOCO_WM_*` -> `WM_CHOCOGRAPH_*`, move to ovl_0c if the world map owns the slot) | world map, chocograph owned |
| 3 | help bounds x 0xC (left as a number) in `menuItem_updateHelp` / `menuAbility_updateHelp` | ovl_37 `menuItem_updateHelp`, ovl_33 `menuAbility_updateHelp` | with the character window open over the left column (cursor in the right column) the help bounds are (0xC, 0, full width, full height), otherwise the left half of the screen. Is 0xC where the help window may start so it doesn't cover the target window, or just a margin? | `menu_items`: pick a usable item in the right column (Potion), Cross to open the character window; screenshot with help on (Select). Do the same from the left column. Name the 0xC after what it keeps clear | `menu_items` |
