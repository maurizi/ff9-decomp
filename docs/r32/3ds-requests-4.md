# R32d: answers for the 3DS port (Tetra Master: phases, cursors, board, card data, panels)

Answers from the matched C (`src/ovl_0d/tm_game.c`, `src/ovl_0d/tm_main.c`, `include/ovl_0d.h`,
`src/ovl_36/ovl_36.c`, `include/ovl_36.h`, `src/main/card.c`; line numbers at commit 321dd16e), checked in PCSX-Redux on
disc 1 (forced card game from `field_dali_inn`, opponent 12, see "How the match state was made" at the end).
All screen coordinates are in the game's 320x240 (NTSC 320x224 visible) space. All overlay addresses are ovl_0d's
(loaded at 0x800A7000, only resident while GameState.mode == 9).

## Short answers

1. **One overlay, one state machine, one stack local.** The whole card game, from "Card Selection" to the rematch
   prompt, is ovl_0d. `tm_main` = `tm_init` + `tm_mainLoop` + `tm_exit`. `tm_mainLoop` keeps all state in a local
   `TmState s` (0x5D0 bytes) and each frame runs the handler for `s.task` (`TM_TASK_*`, 0-35, `include/ovl_0d.h:12`).
   At runtime `&s` is **0x801FF9E0** (same address in every frame and every game of the session, because the call chain
   game_main → tm_main → tm_mainLoop has a fixed depth). The main menu's Card screen is ovl_36 and **shares no code** with
   ovl_0d. It has its own state `s_menuCard` (0x801F347C, `CardMenu`). Both read the same card stock, `g_cards`
   (0x80083248, main).
2. **The cursors are plain TmState fields that the handlers read every frame.** Write the field and the game draws and
   uses it from the next handler call (verified for the hand, board, stock and GET SELECT cursors). There are no separate
   confirm functions: each "Cross" is an inline block in the task handler, gated on `g_padRepeat & BTN_CROSS`. For each
   one the minimal state and the side effects are listed below. Port pattern: set the cursor field, then run that block
   (factored out into a function) **inside the same task's handler call**, i.e. in tm_mainLoop between `tm_updatePad`
   and `game_updateWindows`. The handlers also emit primitives and read `TmState`, which is a local of tm_mainLoop, so
   calling them a second time per frame from outside would draw twice. The only standalone "commit" function is
   `tm_checkBattles(s)`, which places nothing itself and only decides BATTLE / BATTLE SELECT / flips / TURN.
3. **Board and hands are inside TmState:** `cards[10]` (`TmCard`, 0x3C each; 0-4 the player's hand, 5-9 the enemy's;
   ownership changes by `owner`, not by index), `board[4][4]` (`TmCell {state, card*}`, [y][x]), `blocks[6]`, `turn`,
   scores counted live (`tm_countOnBoard`). Layouts and addresses below.
4. **Card data:** name = `card_getName(id)` (main, `text_getFrom(g_cardNames 0x8006FD60, id)`, FF9 text encoding).
   Arrows = `TmCard.arrows` bit d = direction d, clockwise from up. Stat string = hex digit of `power>>4`, type letter
   P/M/X/A, `pdef>>4`, `mdef>>4`. Picture: 8-bit TIM in VRAM, `tpage = GetTPage(1, 0, (id/30)*128, 256)`,
   `u = (id%30%6)*42`, `v = (id%30/6)*51`, 42x51, `clut = GetClut(0x180, 0x19C+id)` (reproduced from a VRAM dump).
   **There is no opponent name or portrait** in the PS1 card game: no text id, image or table entry refers to the
   opponent. The opponent id only selects a deck and an AI level.
5. **Panels:** "Card Selection" / "Discard" titles and WIN/LOSE/DRAW/PERFECT banners are sprites from the TM image file
   (VRAM 0x280,0x100, CLUT row 0xE9). The Yes/No questions are free windows (`win_openFree`) whose content callback
   draws one text id. The hint lines ("Select cards to use…") go through the shared help box `g_menuHelp`. Positions
   and OT slots are below. The OT is reverse-cleared (`ClearOTagR`, drawn from `ot+0xFFF`), so a **lower OT index is
   drawn later, in front**.

## 1. Phases, functions, state number

`TmState.task` = `*(s32 *)0x801FF9E0`. Handlers, as dispatched in `tm_mainLoop` (`tm_game.c:3-283`). The draw
functions that always run first each frame: `tm_drawScore` (if `showScore`), `tm_drawBlocks` (if `showBlocks`),
`tm_drawBackground`, `tm_drawBoard` (if `showBoard` or FIELD_FADE), `tm_drawCards`, `tm_drawCombo`, `tm_drawBattleValues`.

| phase | task (value) | handler / drawer (tm_game.c line) | what happens / next |
|---|---|---|---|
| deck selection | CHOICE (18) | `tm_stockInput` 1009, `tm_drawStock` 4164, `tm_showStockCard` 2964 | 10x10 stock grid, Cross moves a copy into the hand (`handCount`++); at 5 opens "Confirm selection?" → CHOICE2. Circle returns the last picked card; Square (or Circle with 0 picked) → GAME_QUIT window (not for forced opponents 0x7C-0x7F) |
| | CHOICE2 (19) | `tm_taskChoice2` 1164 | Yes → FIELD_FADE; No → CHOICE with handCount 4 (card 4 returned to the stock) |
| | GAME_QUIT (35) | `tm_taskGameQuit` 1521 | "End card game?" Yes → hand returned, `card_setGameResult(1)`, END_FADE; No → CHOICE |
| | STAGE_SELECT (20) | `tm_taskStageSelect` 1226 | debug only (opponent/arrow odds editor), nothing sets it |
| match start | FIELD_FADE (21) | `tm_drawBoard` 525 | board fades in for 32 frames, then `tm_dealEnemyHand`, showBoard/showScore/showBlocks = 1 → ENEMY_APPEAR |
| | ENEMY_APPEAR (33) | `tm_taskEnemyAppear` 3211, `tm_revealBlocks` 3541 | enemy cards drop in, blocks appear one by one → COIN, random `turn` |
| | COIN (28) | `tm_taskCoin` 4639 | coin toss; → PLAYER_SELECT (turn 0) or ENEMY_THINK (turn 1) |
| match | PLAYER_SELECT (0) | `tm_taskPlayerSelect` 675 (`tm_autoPlace` 909 if `autoPlay`) | Up/Down `cursor` over the hand, R1 slides out the name, Cross → PLAYER_SET. `tm_checkGameEnd` runs first |
| | PLAYER_SET (1) | `tm_taskPlayerSet` 778 | board cursor `boardX/boardY`; Cross on an empty cell places → `tm_checkBattles`; Circle → PLAYER_SELECT |
| | BATTLE_SELECT (6) | `tm_taskBattleSelect` 1337 | ≥2 possible battles: cursor over `targets[]`; Cross → BATTLE |
| | BATTLE (7) | `tm_taskBattle` 1639 | 0x5A frames: values, clash sprite at 0x23, rolls from 0x43 → FLASH with flashNext WIN/LOSE |
| | FLASH (5), FLASH_ARROW (29) | `tm_drawFlash` 1835, `tm_taskFlashArrow` 2041 | capture / combo flips |
| | BATTLE_WIN (9) / BATTLE_LOSE (10) | 1804 / 1812 | combo flips (`tm_setFlips`), then more battles or TURN |
| | TURN (8) | `tm_taskTurn` 1821 | swaps `turn`; player → PLAYER_SELECT, enemy → ENEMY_THINK |
| | ENEMY_THINK (30), ENEMY_SELECT (2) | `tm_taskEnemyThink` 3694 + `tm_animEnemyThink` 3244, `tm_enemySelect` 3709 | AI delay, then the AI places a card (and runs `tm_checkGameEnd`) |
| result | RESULT (3) | `tm_drawResult` 3349, `tm_taskResult` 861 | `tm_checkGameEnd` 3319 sets `result` and RESULT when both hands are empty. Banner fades in; Cross/Circle after 0x22 frames: win/lose → CARD_MOVE, draw → rematch (or EXTENSION for forced opponents) |
| exchange | CARD_MOVE (14) | `tm_taskCardMove` 2322 | all 10 cards slide to the two columns; then GET_SELECT, or flips → FLASH2 |
| | GET_SELECT (34) | `tm_taskGetSelect` 1430 | **win with 2-4 enemy cards captured and not all 10 owned**: Up/Down `getCursor` over the captured enemy cards, Cross → FLASH2 |
| | FLASH2 (15) | `tm_drawFlash2` 1989 | cards that go back change colour |
| | CARD_CHANGE (13) → TRADE (16) → CHANGE2 (17) | `tm_taskCardChange` 2521, `tm_taskTrade` 938 + `tm_drawTradeBadge` 2593, `tm_taskChange2` 2652, `tm_nextTrade` 2459 | one card at a time: moves to the centre (0x8B,0x55), "Received X" / "Lost X" window (Cross/Square closes it), moves to the winner's column; loops via `tm_nextTrade` |
| | AWAY_INFO (22), CARD_AWAY (23), CARD_AWAY2 (24) | `tm_taskAwayInfo` 1267, `tm_stockInput`, `tm_taskCardAway2` 1274 | over 100 cards: "Select card to discard" on the stock grid, "Discard? Yes/No" |
| | TRY_AGAIN (11) | `tm_taskTryAgain` 944 | "Play a rematch?" Yes → INIT (or CARD_LESS5 if under 5 cards); No → END_FADE |
| | EXTENSION (26), CARD_LESS5 (25) | 898, 1401 | draw against a forced opponent → INIT; "You don't have 5 cards" → END_FADE |
| | INIT (4) | `tm_taskInit` 407 | resets cursors/windows, `tm_initHands`, `tm_initBoard` (places the blocks) → CHOICE |
| exit | END_FADE (27) → EXIT (12) | `tm_taskEndFade` 4886 | white fade 31 frames; EXIT ends the loop, `tm_exit` sets mode 1 (field), prevMode 9 |

Outcome of a game for the field script: `g_cardGameResult` (0x800834C8, main): 0 win, 1 loss or quit, 2 draw
(`card_setGameResult`). Totals: `g_cards.wins/losses/draws` (s16 at 0x80083248/4A/4C).

How the game is started: a field script's **Square-button function** (tag 8, `EVT_FUNC_SQUARE`,
`evt_checkPlayerAction` event.c:2781) runs `AE 00 lo hi` (`EVT_OP_CARD_GAME`, event.c:1368):
`evt_setCardOpponent(id)` writes `GameData.cardOpponent` (+0x8B2, s16) and the instruction returns 7. `fld_main`
(ovl_0a fld_main.c:56) turns result 7 into `exitReason = FLD_EXIT_TETRA_MASTER`, `fld_exit` (fld_main.c:458) suspends
the field music and sets `mode = GS_MODE_TETRA_MASTER (9)`, `prevMode = 1`, and `game_main` loads ovl_0d. After the game
the field reloads and the script continues after the AE instruction (Dali 354 entry 3: `AE 00 0C 00` at +0xB7, then it
reads the result). Disc 1 has 41 such instructions (e.g. Alexandria 107-114, Dali 354/356, Lindblum 551-571), all in
tag 8, opponent ids 0-34. `tm_setup` (tm_game.c:356) reads the id: `deck = g_tmOpponents[id].deck`,
`aiLevel = g_tmOpponents[id].aiLevel` (table at 0x80104000, 2 bytes per opponent; the first 64 entries are
`{id/4, id%4}`).

## 2. Cursors and the confirm blocks

TmState offsets (base 0x801FF9E0):

| field | off | meaning |
|---|---|---|
| `task` | 0x000 | phase (above) |
| `cursor` | 0x004 | PLAYER_SELECT: index among the player's cards still in hand (`tm_getHandCard(s, 0, cursor)`: n-th card with owner 0 and onBoard 0, in cards[] order). Yes/No windows (CHOICE2, TRY_AGAIN, CARD_AWAY2): 0 Yes, 1 No |
| `boardX`, `boardY` | 0x008, 0x00C | board cursor 0-3 (PLAYER_SET, BATTLE_SELECT) |
| `stockCol`, `stockRow` | 0x474, 0x478 | stock grid cursor; **card id = col*10 + row** (`tm_getStockCursorId`), the grid is column-major |
| `copyIdx` | 0x4B4 | which copy of that card id (L1/R1); index for `card_find(id, copyIdx)` |
| `handCount` | 0x47C | cards picked so far (0-5); picked cards are `cards[0..handCount-1]` |
| `getCursor` | 0x5B0 | GET_SELECT: index among the enemy-hand cards (5-9) now owned by the player (`tm_getCaptured`) |
| `quitChoice` | 0x5B4 | GAME_QUIT window: 0 Yes, 1 No |
| `targets[8]`, `numTargets` | 0x48C, 0x4AC | BATTLE_SELECT: TmCard pointers the placed card may battle |
| `srcX/srcY`, `dstX/dstY` | 0x424-0x430 | square of the card just placed / of the card it battles |
| `animFrame`, `savedFrame` | 0x480, 0x488 | cursor hand animation (savedFrame keeps the hand-card cursor's frame during PLAYER_SET) |
| `nameSlide` | 0x564 | R1 name panel slide 0-10 (PLAYER_SELECT) |

Common gate: Yes/No handlers do nothing while `g_tmWindowOpening` (0x800B3CD0) != 0 (the window is still opening;
`tm_waitWindow` sets it, the window's on-opened callback clears it). RESULT needs `timer` (0x2E8) >= 0x22.

Confirm blocks (what to run instead of Cross; each also plays a sound via `tm_playSe`):

- **CHOICE, pick a card** (tm_game.c:1105-1146): `d = card_find(col*10+row, copyIdx)`; if `d != -1`:
  `cards[handCount]` = d's id/arrows/power/type/pdef/mdef, `card_remove(id, copyIdx)`, `copyIdx = 0`, `handCount++`;
  when it reaches 5, open the confirm window (`win_openFree` rect {0xA0-w/2, 0x53, w, 0x3A}, `tm_drawConfirmWin`,
  `tm_waitWindow`), `cursor = 0`, `task = CHOICE2`. Undo (Circle): `handCount--`, `card_addWithStats` of
  `cards[handCount]`. With `autoPlay` set the block first rebuilds the stock (debug).
- **CHOICE2 Yes/No** (1170-1182): Yes: `timer = 0`, `task = FIELD_FADE`; No: `task = CHOICE`, `handCount = 4`,
  return `cards[4]` to the stock; then `win_close(msgWin)`.
- **PLAYER_SELECT → PLAYER_SET** (768-772): `task = PLAYER_SET`, `savedFrame = animFrame`. Requires a valid
  `cursor` (< `tm_countInHand(s, 0)`).
- **PLAYER_SET, place** (823-841): `cell = tm_getCell(s, boardX, boardY)`; only if `cell->state == TM_CELL_EMPTY (1)`:
  `c = tm_getHandCard(s, 0, cursor)`; `c->x = boardX*43+72`, `c->y = boardY*52+8`, `c->onBoard = 1`, `c->small = 0`;
  `cell->state = TM_CELL_CARD (0)`, `cell->card = c`; `cursor = 0`; `srcX/srcY = boardX/boardY`; `tm_checkBattles(s)`.
  Otherwise buzzer. Cancel (Circle): `nameSlide = 0`, `task = PLAYER_SELECT`, `animFrame = savedFrame`.
- **BATTLE_SELECT** (1365-1377): if `board[boardY][boardX]` holds a card that is in `targets[0..numTargets-1]`:
  `timer = 0`, `task = BATTLE`, `dstX/dstY = boardX/boardY`. The d-pad here only moves within ±1 of `srcX/srcY`; a port
  that sets `boardX/boardY` directly should apply the same target test.
- **GET_SELECT** (1468-1495): clears the flips, then queues a `TM_FLIP_CHANGE` flip for every player hand card owned by
  the enemy (arg 0) and every captured enemy card except the selected one (arg 1); `flashMode = 1`, `task = FLASH2`
  (also clears `hasLostCard` if the selected card is the one the enemy won last game).
- **TRY_AGAIN Yes/No** (950-973): `win_close(msgWin)`; Yes: `card_countAll() < 5` → "don't have 5 cards" window and
  CARD_LESS5, else `task = INIT`; No: `tm_startEndFade(s)`.
- **GAME_QUIT** (1549-1564), **CARD_AWAY2** (1279-1293), **CARD_AWAY pick** (1148-1160): same pattern (see the C).
- **RESULT / TRADE / EXTENSION / AWAY_INFO / CARD_LESS5**: "any button" waits; the actions are `tm_startCardMove(s)` (win
  or lose), `tm_endTrade(s)`, `win_close(...)` + next task.

Is it safe outside the handler? The blocks only touch TmState, the card stock and the window system, but they assume
they run in their own task, after `pad_update` and before `game_updateWindows`/`help_update` in the same frame (the
windows they open are updated there). Run them from that spot. Running a handler a second time from elsewhere would
add a second set of primitives (they draw cursors and help into the same `p` buffer). `tm_checkBattles(s)` and
`tm_startCardMove(s)` are self-contained and safe to call whenever the corresponding confirm would.

Main menu Card screen (ovl_36): cursor `s_menuCard.row` (+1) / `.col` (+2), card id = col*10+row,
`s_menuCard.state` (+0) 1 grid / 2 discard window / 0 none, `discardCursor` (+5) 0 Yes 1 No.
`menuCard_inputGrid` (ovl_36.c:50): Cross → `menuCard_openDiscard()` (98, needs `count[id] != 0`), Circle →
`menuScreen_open(0)`, moving the cursor → `menuCard_selectCard(id)` (732, loads the picture), L1/R1 →
`menuCard_flipCopy(id, dir)`. `menuCard_inputDiscard` (591): Yes → `card_remove(id, shown[id])` + `menuCard_refresh()`,
then `state = 0`, `win_close`. Input only runs when `!fade_isBusy() && !flip.active` (`menuCard_input`, 566). Grid cell
of id: x = gridRect.x + col*15 + 4, y = gridRect.y + row*15 + 10, 13x13 (gridRect = {8, 0x26, 0x9E, 0xB6}).

## 3. Board, hands, score, turn

`TmCard` (`include/ovl_0d.h:146`, 0x3C bytes; `cards[i]` at 0x801FF9F0 + i*0x3C):

| off | field | notes |
|---|---|---|
| 0 | u8 id | 0-99 |
| 1 | u8 arrows | bit d = arrow toward direction d: 0 up, 1 up-right, 2 right, 3 down-right, 4 down, 5 down-left, 6 left, 7 up-left |
| 2 | u8 power | shown as `power>>4` (hex digit) |
| 3 | u8 type | 0 P (vs pdef), 1 M (vs mdef), 2 X (vs lower def), 3 A (highest own stat vs lowest target stat) |
| 4, 5 | u8 pdef, mdef | shown as `>>4` |
| 8 | s32 hidden | 1 = face down (enemy hand until placed) |
| 0xC, 0x10 | s32 x, y | screen position, recomputed every frame by `tm_layoutCards` (2061) for hand cards |
| 0x14 | s32 owner | 0 player (blue frame), 1 enemy (red frame); changes on capture |
| 0x18 | s32 onBoard | 1 = placed |
| 0x1C | s32 small | drawn shrunk (34x41 POLY_FT4): player hand while it has 5 cards, and in CHOICE |
| 0x20-0x2C | fromX/fromY/toX/toY | move animations, 24.8 fixed |
| 0x30 | s32 depth | OT index |
| 0x34 | s32 visible | |

`board[y][x]` at 0x801FFC48 + (y*4+x)*8: `{s32 state; TmCard *card}`, state 0 card, 1 empty, 2 block, 3 off-board
(`g_tmOffBoardCell` 0x800B3CE8 is returned for x/y outside 0-3). Cell (x, y) is drawn at
(x*43 + 72, y*52 + 8), 42x51. Blocks: `blocks[6]` at 0x801FFCD0 (`{hidden, x, y, style}`), `numBlocks` (0x570),
0-6 per game (70 % five), placed at INIT by `tm_placeBlocks` (3648).

Hands: the player's cards are those with owner 0 and onBoard 0; drawn in a column at x 0x104 (y 8, 0x34 apart) or, with
5 in hand, shrunk at x 0x108 (0x2A apart). Enemy hand: owner 1, onBoard 0, x 0x10, y 0x1A + 14*n, `hidden = 1`. "Played"
is `onBoard`. During CHOICE, `cards[0..handCount-1]` are the picks and **cards[5..9] are reused as the preview stack** of
the card under the stock cursor (x 0xC2+4i, y 0x81+i), so ignore them there.

Turn: `turn` (0x434) 0 player, 1 enemy. Score: `tm_countOnBoard(s, side)` (cards with onBoard 1 and that owner),
drawn by `tm_drawScore` (602): player at (0x28,0xB9), enemy at (0x1A,0x93), big digits from VRAM (0x300,0x100) CLUT 0xEA.
Battle numbers: `atkShown`/`defShown` (0x574/0x578) while `valuesTimer` (0x57C) > 0, at card.x+8, card.y+0x12.
Combo: `comboTimer`/`comboX`/`comboY`/`comboShown` (0x4F0-0x4FC). Other: `result` 0x45C, `wins/losses/draws` of this
session 0x43C-0x444, `opponent` 0x450, `deck` 0x448, `aiLevel` 0x44C, `timer` 0x2E8, `phase` 0x418,
`tradeCard` 0x470, `hasLostCard`/`lostSlot`/`lostCard` 0x54C/0x550/0x510 (the card the enemy won last game is dealt
back to it in the rematch).

Rules in short (for checking a port's display): placing a card with an arrow at an adjacent enemy card whose opposite
arrow points back starts a battle (`tm_checkBattles`); arrows at enemy cards without a back arrow flip them at once
(`tm_evalPlacement` + `tm_setFlips` mode 1). Battle value = `tm_getBattleValues` (1743), each side rolls
`rand() % value`, higher roll wins; the winner then flips the loser's unopposed neighbours (combo).

## 4. Card data

- **Name:** `card_getName(id)` (main card.c, `text_getFrom(g_cardNames, id)`, g_cardNames = 0x8006FD60), width
  `card_getNameWidth(id)`. Same in ovl_0d and ovl_36.
- **Owned copies (the stock):** `g_cards` (0x80083248): s16 wins, losses, draws, then `CardEntry cards[105]`
  {id (0xFF = empty), arrows, power, type, physDef, magicDef}. Helpers: `card_countOf(id)`, `card_find(id, n)`,
  `card_countAll()`, `card_countKinds()`, `card_getRarity(id)` (0-6 by id range).
- **Base stats:** `g_tmCardData` (0x80103800, 5 bytes per id: power, type, pdef, mdef, ?), copied from the TM image file;
  main has its own `g_cardInfo` (0x800121C8) for `card_rollStats`.
- **Stat string:** `tm_drawCardStats` (3269): four 8x8 glyphs at y = card.y + 0x26 (small cards: x-5 and y = card.y + 0x1B),
  x = card.x + 9, +15, +0x15, +0x1B: `power>>4`, type glyph (P 0x10, M 0x11, X 0x12, A 0x0A), `pdef>>4`, `mdef>>4`.
  Glyph n: tpage (0x300,0x100) 4-bit, u = n*8 + 0x50, v = 0xC0, CLUT row 0xEB. Observed: id 3 (raw 11,1,5,13) shows
  "0M00".
- **Arrows:** `tm_drawArrows` (4007), SPRT_8 per set bit, offsets and uv from `g_tmArrowSprites` (0x800A73C4)
  `{x, y, smallX, smallY, u, v}`: 0 (17,0 / 13,0) uv(200,32); 1 (34,0 / 25,0) (208,32); 2 (34,21 / 26,16) (200,40);
  3 (34,43 / 25,32) (232,32); 4 (17,44 / 13,33) (224,32); 5 (0,43 / 0,32) (216,32); 6 (0,21 / 0,16) (192,40);
  7 (0,0 / 0,0) (192,32). Tpage (0x300,0x100), CLUT row 0xEA.
- **Picture (`tm_drawCardFace`, 4579):** `tm_setup` (313) uploads the 100 TIMs from entry 0 of file 0x44C (dir 1),
  8-bit, image `i` at VRAM x = (i/30)*128 + (i%30%6)*21 (halfwords), y = 0x100 + (i%30/6)*51, CLUT at (0x180, 0x19C+i).
  Draw: SPRT 42x51 (0x2A x 0x33), `u = (n%6)*42`, `v = (n/6)*51` with n = id%30, `clut = GetClut(0x180, 0x19C+id)`,
  `DR_TPAGE get_mode(1, 0, GetTPage(1, 0, (id/30)<<7, 0x100))` (8-bit). Small cards: POLY_FT4 34x41 with the same uv
  and `tpage = ((page*2) & 0xE) | 0x90`. Then the frame: SPRT 42x51 from tpage (0x300,0x100) CLUT 0xEA, uv (8,0x40)
  player / (0x38,0x40) enemy / (0xC8,0x40) card back (hidden).
- **Main menu Card screen (ovl_36)** uses a different picture set: file 0x32 of dir 1, one sector per card, RLE-packed,
  decoded into a 12-slot VRAM cache (`menuCard_loadCardImage`, `menuCard_showCachedImage`: 19x47 halfwords per slot at
  (slot%3*19, 0x100 + slot/3*47), CLUT row 0x1F4+slot, copied to (0x3C0,0x1B0) / CLUT (0, 0x1F2) and drawn 38x47 by
  `menuCard_drawFace`).
- **Opponent:** none drawn. `GameData.cardOpponent` → `g_tmOpponents[id]` = {deck, aiLevel}; `g_tmDecks[deck]` (0x80103C00,
  16 card ids by rarity roll) feeds `tm_dealEnemyHand`. Opponents 0x7C-0x7F are "forced" games (no quit, no rematch, a
  draw replays).

## 5. Messages and panels

Text: `g_tmText` (0x800B3668, the overlay's bank, `text_setBank` in `tm_setup`), `text_get(id)`; ids in
`include/ovl_0d.h:95-128`. Windows are `win_openFree(&rect, otz, WIN_STYLE_BEVEL)` + `win_setDrawContent` + shadow.

| phase | panel | source, position |
|---|---|---|
| CHOICE/CHOICE2/GAME_QUIT/END_FADE | "Card Selection" title | `tm_drawStock`: two SPRTs at (0x58,0xA) 48x24 uv(0xB8,0x78) and (0x8C,0xC) 97x32 uv(0x94,0xD2), tpage (0x280,0x100) 4-bit, CLUT 0xE9, ot+0xFFA |
| CARD_AWAY* | "Discard" title + two blinking "!" | (0x79,0xE) 78x21 uv(0x98,0xBC); 16x16 uv(0xF0,0xB8) at (0xCA,0x12) and (0x66,0x12) |
| stock | grid of 100 cells | cell id i at ((i/10)*15+0x12, (i%10)*15+0x2D), 16x16 icon (rarity, ×2 if several) + count digits, tpage (0x340,0x1E0) CLUT 0xED, ot+11; frame bars CLUT 0xEE |
| stock | "Stock:" n, "Type:" n | `ui_drawSmallDigits` (0x19,0xCB) / digits (0x43,0xC9); (0x65,0xCB) / (0x89,0xC9) |
| CHOICE | WIN/LOSE/DRAW totals box | labels at (0xBC,0x42/0x4C/0x56), numbers (0xDC,…), frame (0xB5..0xF6, 0x3D..0x5A) |
| CHOICE | preview: "NO" + number, copies, name, L1 n/m R1 | `tm_showStockCard`: "NO" (0xD8-w/2,0x78), number (0xE6-w/2,0x76), stack at (0xC2,0x81), name centred at x 0xD9, y 0xC7; copy counter (0xBF/0xD4/0xDB, 0xBA) with L1/R1 icons (0xB5/0xF1, 0xB6) |
| CHOICE | help "Select 5 cards to be used…" (0x24, forced 0x32) | `g_menuHelp` at the stock cursor (col*15+3, row*15+0x30), bounds {0xD,0,0x9E,0xDE} |
| CHOICE→CHOICE2 | "Confirm selection? Yes/No" (6) | window {0xA0-w/2, 0x53, w, 0x3A}, otz 6; hand at (0x7B,0x6C) Yes / (0x7B,0x7B) No; help 0x25 / 0x26 |
| GAME_QUIT | "End card game? Yes/No" (0x2D) | {0xA0-w/2, 0x54, w, 0x38}; hand (0x7B,0x6C/0x7B); help 0x2E/0x2F |
| PLAYER_SELECT | R1 "NAME" label, name panel | text 0x1C icon at (0x12,9) + SPRT (0x1F,0xC) 28x8 uv(0x64,0xD0) CLUT 0xEB, ot+0x28; R1 slides out a box with `card_getName` at x 0x11A-w/2 shifted by `nameSlide*12-0x78`, y = card y+0x2A (5 cards) / +0x34, otz 5. Help 0x21 at the card cursor |
| PLAYER_SET | help 0x22 / 0x30 (blocked) | at the board cursor (x*43+72-0xD, y*52+8+7); cursor sprite `tm_drawCursor` (4052) 24x16 at (cell.x-0xF, cell.y+6), ot+3; the hand card keeps a translucent cursor (ot+6) |
| BATTLE_SELECT | help 0x23 / 0x31 (no battle here), targets blink, "select" badge | targets' brightness blinks, others dimmed to 0x40 (`tm_drawCards`); 32x16 badge at card (x+5, y+0x15), uv(0x80,0xF0), tpage (0x340,0x1E0) CLUT 0xED |
| COIN | coin | `tm_taskCoin`, around (0x9D,0x5C), ot+10 |
| BATTLE | clash sprite, numbers | 40x48 at the midpoint, tpage (0x200,0x100) CLUT 0xEC, ot+30; big digits 10x16 uv(n*10+0x18,0xF0) CLUT 0xED, ot+11 |
| FLASH | "COMBO" n | `tm_drawCombo` (4721) ot+10, CLUT 0xE9 |
| RESULT | WIN / PERFECT / LOSE / DRAW banner | `tm_drawResult` ot+1, two semi-transparent SPRTs each (abr 1 then 2) from tpage (0x280,0x100) 8-bit, CLUT 0xE9, brightness `timer*8` (sound at ~0x1F frames): WIN (0x6C,0x58) 104+112x40 v 0; PERFECT (all 10 on the board yours) (0x4A,0x58) 168x40 v 0x50 + 176x40 v 0x78; LOSE (0x6A,0x58) 104+112x40 v 0x28; DRAW (0x5C,0x58) 136x40 v 0xA0 + 144x40 v 0xC8 |
| GET_SELECT | help "Select card to take." (0x2C), "Stock: n" box | cursor (mode 2, mirrored) at (card.x+0x22, card.y+5); box at (0x52, card.y-9), text at (0x5A, card.y-3), otz 3 |
| CARD_CHANGE/TRADE | "Received X" / "Lost X" (3 / 4 + name + 7) | window {0xA0-w/2, 0x96, w, 0x1A}; badge 56x24 at (0xA3,0x45) (uv (0x20,0x90) win / (0xC0,0x80) loss, CLUT 0xEB) when it is the first copy received / last copy lost |
| AWAY_INFO | "You have over 100 cards. Select card to discard." (0x16) | {0xA0-w/2, 0x5B, w, 0x2A} |
| CARD_AWAY2 | "Discard? Yes/No" (0x18) | {0xB9, 0x35, w, 0x38}; hand (0xBC,0x4D/0x5C) |
| TRY_AGAIN | "Play a rematch? Yes/No" (2) | {0xA0-w/2, 0x54, w, 0x39}; hand (0x7D,0x6B) Yes / (0x7D,0x7B) No; help 0x29 / 0x28 |
| draw | "The game ended in a draw. Another game will begin." (0x19) | forced opponents only, {0xA0-w/2, 0x59, w, 0x2B} |
| CARD_LESS5 | "You don't have 5 cards. End card game." (0x1A) | {0xA0-w/2, 0x5B, w, 0x2B}, otz 6 |

Static layers: background `tm_drawBackground` (16x15 SPRT_16 tiles, VRAM 0x280/0x300, CLUT 0xE0, ot+0xFFE), board
`tm_drawBoard` (14x15 tiles from x 0x30, VRAM 0x340, CLUT 0xE7, ot+0xFFA). Help box: `g_menuHelp` (0x80077DE8) .x/.y/
.text, drawn by main's `help_update`; `help_setLocked(0)` only in the input tasks.

## Runtime evidence (PCSX-Redux, disc 1)

- Entry: from `field_dali_inn`, `build/3ds4/enter_tm.py 12 300` (field cleanup to main's dispatcher 0x80012834 as in
  `tools/rt/enter_battle.py`, then `GameData+0x8B2 = 12`, `GameState.mode = 9`). `ovl` showed ovl_0d resident and the
  "Card Selection" screen came up (stock 100, type 10). State `3ds4_tm_card_select`.
- `trace tm_stockInput tm_drawStock --frames 2`: a0 / a2 = 0x801FF9E0 on both frames. `TmState.unk5C8` there read
  {50, 30, 200, 100}, the constants tm_mainLoop writes, so the struct base and the header offsets agree. Same address in
  every later task, after a rematch INIT, and in the second game.
- Stock: writing `stockRow` (0x801FFE58) = 3 and pressing Cross put card id 3 (stats 0M00) into `cards[0]`,
  `handCount` 1. Five picks → CHOICE2 with the "Confirm selection?" window (`3ds4_tm_confirm`).
- Yes → tasks 21 → 33 → 28 → 30/8 → 0, i.e. FIELD_FADE, ENEMY_APPEAR, COIN, ENEMY_THINK, TURN, PLAYER_SELECT (`3ds4_tm_player_select`).
  Enemy hand at x 16, y 26 + 14n, hidden 1; blocks at five cells.
- Writing `cursor` = 4, Cross → PLAYER_SET with cursor 4; writing `boardX/boardY` = 1/0, Cross → card 4 at (1,0),
  x 115 y 8, onBoard 1, then ENEMY_THINK.
- `3ds4_tm_turn2` + cursor 0 at (1,2) → BATTLE (card 0's up-left arrow vs card 6's down-right); the numbers 8 / 6 showed.
- BATTLE_SELECT: (1,1) unblocked (`board[1][1].state` = 1) and card 5's arrows = 0xFF, card 0 placed at (1,1) →
  task 6, `numTargets` 2, `targets` = 0x801FFB58 (card 6), 0x801FFB1C (card 5) (`3ds4_tm_battle_select`). Writing
  boardX/boardY = 0/1 and Cross → BATTLE with dst (0,1).
- `autoPlay` (0x801FFE18) = 1 ran the rest: LOSE banner, 10-0 (`3ds4_tm_result_lose`). With boosted player cards: WIN
  (`3ds4_tm_result_win`); card 9's owner poked to 0 to get two captures → CARD_MOVE → GET_SELECT (`3ds4_tm_get_select`):
  captured cards slide to x 0x16 when selected, "Stock: 4" box. `getCursor` (0x801FFF90) = 1 + Cross → FLASH2 →
  CARD_CHANGE → TRADE "Received Lizard Man", card 9 at (139,85). Cross → CHANGE2 → AWAY_INFO (the stock was at 101):
  "You have over 100 cards" (`3ds4_tm_over100`) → CARD_AWAY → CARD_AWAY2 → Yes → TRY_AGAIN "Play a rematch?"
  (`3ds4_tm_rematch`). `cursor` = 1 + Cross → END_FADE → back in the Dali inn with mode 1, prevMode 9,
  `g_cardGameResult` 0, `g_cards` wins 2.
- VRAM dump during the match (`tools/rt/vram.lua`) rendered with the formula of section 4 (`build/3ds4/cardimg.py`) gave
  the right pictures for ids 0, 1, 3, 5, 31, 99.
- `g_tmArrowSprites` read from RAM gave the table in section 4; `g_tmOpponents` starts 00 00 00 01 00 02 00 03 01 00 …
  (opponent 12 → deck 3, AI 0, as `TmState.deck/aiLevel` showed).

## Proposed names (not applied)

- `tm_taskGetSelect` symbol comment: "picks the opponent card to take after a perfect win" is wrong. GET SELECT runs
  after any win where 2-4 enemy cards were captured and the player doesn't own all 10 (`tm_taskCardMove`); one capture is
  taken automatically, a perfect game takes all five.
- `TmState` base: `TM_STATE_ADDR 0x801FF9E0` (runtime-only constant for tools; not a symbol, it's a stack local).
- `TmCardData.unk4` → `points`? Not proven (ovl_36 sums `card_getInfoTable()[id].unk4` as collector points; the TM
  table's 5th byte was not compared). Leave as is.
- `g_tmOpponents` comment: first 64 entries are {id/4, id%4} (runtime read).
- Constants: `TM_SEL_YES 0`, `TM_SEL_NO 1` for `TmState.cursor`/`quitChoice` in the Yes/No tasks;
  `EVT_FUNC_SQUARE` already exists (tag 8 = card-game challenge in every AE instruction on disc 1).
- `tm_drawStock` sprite sources: the two title SPRTs are the "Card Selection" logo, the CARD_AWAY one the "Discard" logo
  (screenshots `build/3ds4/tm1.png`, `tm11.png`).
