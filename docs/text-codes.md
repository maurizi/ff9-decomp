# Text control codes

Game text is a byte string in the game's own encoding, ended by `TEXT_END` (0xFF). The menu renderer
(`text_buildBlock`, `text_measure`, `text_buildCode`, `text_getCodeWidth`) and the message windows
(`msg_renderText`, `msg_advanceText`, `msg_stepControl`, `msg_measureLine`, `msg_getLayout`) share one set of
codes. The macros are in `include/ff9.h` (`TEXT_*`). Each name comes from what the code does with the byte.

## Plain bytes

| code | macro | effect |
|---|---|---|
| F7 | `TEXT_NEWLINE` | next line (menu: y += 15, messages: += 14) |
| F8 | `TEXT_SPACE_1PX` | messages: x += 1 |
| F9 | `TEXT_ESC` | an escape byte follows (below) |
| FA-FD | `TEXT_WIDE_FIRST`..`TEXT_WIDE_LAST` | messages: lead byte of a two-byte glyph. The menu font uses `Font.extCode >> 8` as its limit instead |
| FE | `TEXT_PAGE` | messages: wait for Cross, then start a new page |
| FF | `TEXT_END` | end. Messages wait for Cross and close |

## Escape bytes (after F9)

Arguments are bytes unless shown otherwise. "msg" means only the message windows handle the code, "menu" means only
`text_buildBlock` / `text_measure` / `text_buildCode` do.

### Message timing and flow (msg_advanceText, msg_stepControl)

| code | macro | args | effect |
|---|---|---|---|
| 00 | `TEXT_WAIT_PAGE` | frames | pause (Cross can't skip it: `MSGW_NO_SKIP`), then start a new page |
| 01 | `TEXT_WAIT_CLOSE` | frames | the same pause, then close the window |
| 02 | `TEXT_SPEED` | speed | typing speed (`MsgWindow.speed`) |
| 08 | `TEXT_CHOICE_START` | | the choice lines start at the next line (`MSGW_CHOICE`, `g_msgChoiceLine`) |
| 09 | `TEXT_HOLD` | | wait with Cross ignored (`MSGW_WAIT_INPUT` + `MSGW_NO_INPUT`), so the script has to close the window |
| 0A | `TEXT_NO_SKIP` | | Cross doesn't skip the rest of the page |
| 0C | `TEXT_SKIP` | | the rest of the page shows at once (`MSGW_SKIP`) |
| 0F | `TEXT_SYNC_TAG` | tag | `g_msgSyncTag` = tag (scripts read it as `EVT_SYSVAR_MSG_SYNC_TAG`) |
| 41 | `TEXT_WAIT` | frames | pause (not while skipping), then continue |
| 42 | `TEXT_SPEED_DEFAULT` | | typing speed back to the config setting (`g_msgTextSpeeds`) |
| 45 | `TEXT_NO_INPUT` | | Cross is ignored (`MSGW_NO_INPUT`). In the message header it is `WINLAYOUT_NO_FOCUS` |
| 46 | `TEXT_SYNC_TAG_NEXT` | | `g_msgSyncTag++` |

### Message header (msg_getLayout reads these from the start of the message)

| code | macro | args | effect |
|---|---|---|---|
| 03 | `TEXT_WIN_POS` | u16 x, y | window position (anchor `WINLAYOUT_ANCHOR_POS`) |
| 04 | `TEXT_BANK` | | the message is a text bank for `TEXT_BANK_TEXT` (`msg_resetChoices` fills `g_msgTextBanks`) |
| 07 | `TEXT_CHOICES` | count, cancel + 1 | the message ends in `count` choices; Circle picks choice `cancel` (0: no cancel) |
| 0B | `TEXT_NO_OPEN_ANIM` | | the window opens without its animation (`WIN_ANIM_NONE`) |
| 0D | `TEXT_LINE_WIDTHS` | {u16 choice << 12 \| width, codes up to FF}..., u16 FFFF | window width = the widest line. A line with a choice number counts only while that choice is enabled. `msg_measureLine` adds the widths of the embedded values |
| 4F | `TEXT_CHOICES_MASKED` | lines, cancel line | choices, some of them hidden by `g_msgChoiceMaskReq` (`EVT_OP_MSG_CHOICE_MASK`). The window loses 14 px per hidden line |
| 50-55, 58-5C | `TEXT_WIN_*` | | window anchor (below) |
| 5F | `TEXT_BALLOON_OFS` | 3 x s16 | `g_evtBalloonOfsX/Y/Z`: offset of the point on the speaker that the balloon tail points at |

Window anchors (`WinLayout.anchor`, `evt_getMsgLayout`). For a balloon (message flag 0x80) the low two bits pick
the side of the speaker (bit 0: left, bit 1: above, i.e. `MSG_TAIL_DIR` from the tail's point of view). Without a
balloon the window goes to a fixed screen position:

| code | macro | balloon | fixed |
|---|---|---|---|
| 50 | `TEXT_WIN_BOTTOM_RIGHT` | below right, flips at the screen edge | x 308 - w, y -48 - h |
| 51 | `TEXT_WIN_BOTTOM_LEFT` | below left | x 12, y -48 - h |
| 52 | `TEXT_WIN_TOP_RIGHT` | above right | x 308 - w, y 12 |
| 53 | `TEXT_WIN_TOP_LEFT` | above left | x 12, y 12 |
| 54 | `TEXT_WIN_BOTTOM` | automatic | x centred, y -48 - h |
| 55 | `TEXT_WIN_TOP` | automatic | x centred, y 12 |
| 58-5B | `TEXT_WIN_FIXED_*` | as 50-53, never flipped | (unchanged) |
| 5C | `TEXT_WIN_CENTER` | automatic | x centred, y centred on 74 |
| (none) | `WINLAYOUT_ANCHOR_NONE` | automatic | x centred, y centred on 110 |

`WinLayout.y` is an s8, so -48 - h is 208 - h on screen.

### Embedded values

| code | macro | args | effect |
|---|---|---|---|
| 06 | `TEXT_BANK_TEXT` | bank, slot | text number `g_msgNumbers[slot]` of text bank `bank` (msg) |
| 0E | `TEXT_MSG_ITEM_NAME` | slot | name of item / key item / card `g_msgNumbers[slot]`, in yellow (msg) |
| 10-17 | `TEXT_CHAR_NAME` + n | | name of character n |
| 18-1B | `TEXT_PARTY_NAME` + n | | name of the character in party slot n |
| 20-29 | `TEXT_COLOR` + n | | palette n (`TEXT_PAL_*` in menu text; messages use 0-8 through `pen_setPalette`) |
| 40 | `TEXT_MSG_NUMBER` | slot | the number `g_msgNumbers[slot]` |
| 4C | `TEXT_ITEM_ICON` | item | the item's icon (menu) |
| 4D | `TEXT_ARG_SPRITE` | arg | UI sprite `g_textIconArgs[arg]` (menu) |
| 4E | `TEXT_ARG_ITEM_ICON` | arg | icon of item `g_textIconArgs[arg]` (menu) |
| 68-6F | `TEXT_ARG_NUMBER` / `TEXT_ARG_STRING` (`_SMALL`, `_DIGITS`, `_LABEL`) | arg | `g_textNumberArgs[arg]` / `g_textStringArgs[arg]` in the font, small font, digits or label font (menu) |
| 80-87 | `TEXT_CHAR_NAME_CAPS` + n | | name of character n in capitals (msg) |

### Layout and glyphs

| code | macro | args | effect |
|---|---|---|---|
| 2F | `TEXT_DPAD` | | the d-pad sprite (msg) |
| 30-39 | `TEXT_BTN_*` | | button glyphs: circle, cross, triangle, square, L1, R1, L2, R2, start, select |
| 3A-3D | `TEXT_DPAD_UP/DOWN/LEFT/RIGHT` | | the d-pad with that arrow |
| 3E, 3F | `TEXT_DPAD_UP_DOWN`, `TEXT_DPAD_LEFT_RIGHT` | | the d-pad with both arrows (menu) |
| 43 | `TEXT_X_ADD` | s8 | x += n |
| 44 | `TEXT_X_SET` | x | x = line start + n |
| 48 | `TEXT_CENTER` | u16 width, codes up to FF | centre the line: its width plus the embedded values listed (msg) |
| 49 | `TEXT_Y_DOWN` | px | y += n |
| 4A | `TEXT_Y_UP` | px | y -= n |
| 4B | `TEXT_SPRITE` | sprite | a UI sprite |
| 60-67 | `TEXT_PAD_BTN` + n | | button n (`TEXT_BTN_*` order) as remapped by the controller config (`text_getButtonSprite`, `GameState.padMap`) |
| 70 | `TEXT_FLAG_ICON` | bit | a blinking two-sprite icon (sprites 0xB5, 0xB6) when `g_msgNumbers[0]` has the bit (msg) |
| 71 | `TEXT_TAB` | | x += 18 (msg) |

## Quirks
- `msg_measureLine` reads one argument byte for `TEXT_BANK_TEXT` (bank in bits 4-5, slot in bits 0-2), while the
  renderer and `msg_advanceText` read two.
- `text_getCodeWidth` measures `TEXT_PAD_BTN + 0` with `text_getButtonSprite(0)` and `+ 1` with `(1)`, while
  `text_buildCode` draws them the other way round (as `msg_renderText` does: `+ 0` is circle).
