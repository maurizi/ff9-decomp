# R44 Tetra Master layout: runtime questions (ovl_0d)

R44 named the card game's screen layout, VRAM/CLUT placement, OT layers, timings and odds tables in
`include/ovl_0d.h` ("Tetra Master layout and tuning (R44)"). Everything there follows from the code except the sprites
below, whose define comments say `(runtime: pending R44)`: the code places them, but only a screenshot says what they
show. Recipe for all of them: `tools/rt/enter_tm.py OPPONENT FRAMES` from a field state (docs/emulator.md), then
`tools/rt/tmdump.py` for the TmState at 0x801FF9E0 (`task` at +0, TM_TASK_* in ovl_0d.h). Each answer needs one
screenshot, cropped around the given rectangle. The names R44 already uses don't depend on the answers. Only a few
get renamed once the pictures are known.

### Q1. The piece between the two score counts (tm_drawScore 0x800A993C, src/ovl_0d/tm_game.c, `TM_SCORE_MID_X/Y`)
- Code: a 0x30 x 0x20 SPRT at screen (0x10, 0xAA), from the parts page (tpage 0x300,0x100, CLUT row 0xEA), u 0xC0
  v 0. The player's count is drawn at (0x28, 0xB9) and the enemy's at (0x1A, 0x93).
- Observe: any match after the cards are dealt (task 0 PLAYER CARD SELECT). Screenshot the rectangle x 0x10-0x40,
  y 0x93-0xD9. What sits between the counts (a slash, "vs", a frame)? Proposed rename `TM_SCORE_<WHAT>_X/Y`.

### Q2. The label beside the R1 icon (tm_taskPlayerSelect 0x800A9B2C, `TM_R1_LABEL_X/Y`)
- Code: during PLAYER CARD SELECT the R1 button icon is text_drawBlock'd at (0x12, 9), and a 0x1C x 8 SPRT sits at
  (0x1F, 0xC) from the coin file (tpage 0x300,0x100, CLUT row 0xEB, u 0x64 v 0xD0). Holding R1 slides in the selected
  card's name box.
- Observe: task 0 with the player's turn. Screenshot x 0x10-0x40, y 8-0x18. What does it say ("Name"?). Proposed
  `TM_R1_<WHAT>_LABEL_X/Y`.

### Q3. Stock screen title sprites (tm_drawStock 0x800B1A84, `TM_TITLE_SELECT*`, `TM_TITLE_DISCARD*`)
- Code, all from the result-text page (tpage 0x280,0x100, CLUT row 0xE9):
  - card selection (tasks CHOICE 18 / CHOICE2 19 / GAME QUIT 35 / END FADE 27): sprite A at (0x58, 0xA), 0x30 x 0x18,
    uv (0xB8, 0x78), and sprite B at (0x8C, 0xC), 0x61 x 0x20, uv (0x94, 0xD2);
  - otherwise (discarding: CARD AWAY 23 / CARD AWAY2 24): one 0x4E x 0x15 sprite at (0x79, 0xE), uv (0x98, 0xBC),
    between two 16 x 16 blinking arrows at (0x66, 0x12) and (0xCA, 0x12), uv (0xF0, 0xB8).
- Observe: (a) the stock screen right after `enter_tm.py`, task 18: screenshot x 0x50-0xF0, y 8-0x30. (b) The
  discard screen: task 23 appears after a won game when the stock holds over 100 cards. Fastest way: poke TmState.task
  (0x801FF9E0) to 23 while task 18 is showing, then screenshot the same rectangle. Name each picture ("Card",
  "Select", "Selection", "Discard"?). Proposed `TM_TITLE_<TEXT>_X/Y`.

### Q4. Result banner layers (tm_drawResult 0x800AFCE4, `TM_RESULT_*`), optional
- Code: each banner is two SPRTs at the same spot (y 0x58), the first drawn with tpage abr 1 (additive), the second
  with abr 2 (subtractive). For WIN (x 0x6C) and LOSE (x 0x6A) the second layer is the next 0x70 texels of the same
  texture row (u 0x68). For PERFECT (x 0x4A) and DRAW (x 0x5C) it is the next row down (v + 0x28).
- Observe: end a match (task 3 RESULT) as a win and as a loss (TmState.result at +0x45C: 0 win, 1 lose, 2 draw; poke
  it before the banner fades in if needed). Screenshot x 0x40-0x110, y 0x50-0x88. Is the second layer an outline or
  a shadow of the text? Only the comment changes.
