# R44: runtime questions from the ending pass (ovl_15)

Round R44's integration pass named the ending's remaining file / db / VRAM literals (ovl_15 `include/ovl_15.h`):

- The Blackjack textures `BJ_TIM_*` (`DB_TIM | 0x4F4C..0x4F51`, all in `FILE_ENDING_DATA` 0x2F44 of FF9.IMG dir 1)
  were decoded from disc 4 and rendered with their own palettes: card blank + suit pips (0x4F4C), rank glyphs 2-A in
  black / red, upright / upside-down columns (0x4F4D), three face card pictures (0x4F4E, 0x4F4F, 0x4F50), and the
  256x256 banner sheet shared with Tetra Master (0x4F51: WIN, LOSE, PERFECT, DRAW with shadow copies, COMBO, Card
  Select, Discard, digits). `bj_drawCard` fixes which picture is J / Q / K (clut row `BJ_CLUT_PICTURE_Y + face`,
  u/v `BJ_QUEEN_U`, `BJ_KING_V`), and the card face palette is the back's with entries 0x20-0x2F made white by
  `g_bjClut` (`BJ_FACE_WHITE_FIRST`). The card tpage `BJ_CARDS_VRAM_*` (0x1C0, 0x100) and banner page
  `BJ_RESULT_VRAM_*` (0x280, 0x100) follow from the TIM positions and the tpages built.
- `g_endingImage` 0x4EE8 is the 320x224 16-bit "THE END" picture with the copyright line (decoded).
- `LOAD_MOVIE_OVL` / `LOAD_MOVIE_OVL_SIZE` / `LOAD_MOVIE_WORK` (res_ids.h, also used by ovl_0a and ovl_14) and
  `LOAD_ENDING_DATA`: load-area offsets from the code (0x800A7000 + 0xF1700 = OVL_MOVIE's link address).
- Timings: `ENDING_MUSIC_FADE_FRAMES`, `ENDING_MUSIC_START_FRAMES`, `ENDING_MOVIE_FADE_FRAMES`, `ENDING_MSG_SECOND_AT`,
  `BJ_FADE_FRAMES`, `BJ_SHUFFLE_FRAMES`; the Blackjack side panel `BJ_PANEL_*` (labels from `g_bjStr*`); the unused
  ending menu `ENDING_MENU_*`.

Only what a screen shows is left to confirm; none of the names is marked pending. Addresses: ovl_15 `bj_main`
0x800A9488, `bj_init` 0x800AAD24, `bj_drawResult` 0x800A7ABC.

| # | symbol | binary / file | current name and what the code shows | what to observe | mode / state |
|---|---|---|---|---|---|
| 1 | `BJ_TIM_JACK` / `BJ_TIM_QUEEN` / `BJ_TIM_KING` (0x4F4E / 0x4F50 / 0x4F4F) | ovl_15 blackjack.c `bj_drawCard`, `bj_init` | decoded: a black mage, a woman in white, a crowned bird; the code draws them for ranks J, Q, K | deal until a J, Q and K show, screenshot: confirm the mapping, and say who the three characters are, so the define comments can name them | `tools/rt/bj_force.py` |
| 2 | `BJ_PANEL_*` (x 0x10C centre; y 0x58 / 0x64 / 0x70 / 0x7C, actions 0x90-0xC0) | ovl_15 blackjack.c `bj_main` | "Bankroll" + amount, "Wager" + wager, then Stand / Hit / Double / Split, centred in a column right of the table | one screenshot at the action prompt: confirm the column right of the table (and that Double / Split grey out when not allowed) | `tools/rt/bj_force.py` |
| 3 | `bj_drawResult` `BJ_RESULT_PUSH` | ovl_15 blackjack.c | a push draws the sheet's "DRAW" banner (u/v of the DRAW rows) | play to a push (or poke both hands' values equal before settling), screenshot the banner | `tools/rt/bj_force.py` |

Not asked: the music / fade timings and the load-area offsets are what the code does with them; the ending menu is
never entered.
