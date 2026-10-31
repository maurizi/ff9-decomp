# Main-exe constants that need a runtime check

Values left as literals (or named only partly) because the code alone doesn't show what they mean.
Each entry: value, field, hypothesis, how to observe it.

| value | field | hypothesis | how to observe |
|---|---|---|---|
| 5 | `GameState.mode` | A mode without an overlay or a `game_main` case; `vib_update` vibrates even with vibration off in it, ovl_64 `mbg.c` and ovl_0a `bgi_move.c` treat it like the field. Probably a debug/test mode that is never entered. | Watch writes of 5 to `g_gameState->mode` (byte at GameState+8) through a playthrough segment. |
| bits 11-13 | `GameState.config` | 3-bit setting indexing `g_btlBannerDurations` (ovl_0b `btlui_frame.c`), maybe the Config screen's battle message speed row. | Change each Config row in the menu and diff `config`. |
