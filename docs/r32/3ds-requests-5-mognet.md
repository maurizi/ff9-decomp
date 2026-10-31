# R34: answers for the 3DS port (Mognet)

Answers from the matched C (`src/main/msg.c`, `src/main/event.c`, `src/main/evt_main.c`, the hand-written expression
evaluator `asm/us/main/nonmatchings/evt_expr_asm/evt_runExpr.s`; line numbers at commit 5181ea1c), a static survey of
every Moogle talk function on the four discs, and PCSX-Redux runs on disc 1 (Dali inn Moogle, field 351). Background:
`docs/r32/3ds-requests-2.md` section 6 (Mognet is field script + message windows, no overlay). One correction to it: the
Moogle's first menu is msg **3**, not 8. msg 8 is the Mognet sub-menu ("What do you want to do? Give X a letter / Read
mail / Cancel"), and msg 0xC there was the "You have a letter from Kumop to Mogki" status window.

Notation used below: `b[n]` = `g_evtGlobalVars.b[n]` (main 0x8007AEF0 + n, the scripts' global bank 0, survives saves);
`Gbit[i]` = global bit i = bit `i & 7` of `b[i >> 3]`; `h[0]` = story counter (u16 at 0x8007AEF0). Field-bank variables
(`Fu8[...]`, bank 1 = `g_evtState->fieldVars`, cleared per field) and object locals (`Lu8[...]`) are allocated
**differently in every field**, so the port must not hard-code them.

## Short answers

1. **Opening Mognet.** The Moogle is an actor (model 220, or 129 in 7 fields) whose **talk function (tag 3)** runs on
   Cross. It opens msg 3 (window 2): "Can I help you, kupo? / Save / Tent / Mognet / Mogshop / Switch party members /
   Debug / Cancel", rows filtered by a choice mask (0x47 = Save, Tent, Mognet, Cancel; +0x08 Mogshop when the Moogle has
   a shop; +0x10 party switch on a field flag; Debug never). Row 2 (Mognet) runs a fixed template, identical in all 58
   Moogle fields except for variable offsets, message ids and the letter tables: status window (carried letters, msg
   0xB-0x12 in window 5) → Mognet menu msg 8 (`Give <moogle> a letter` / `Read mail` / `Cancel`, mask from what is
   possible; skipped when only Cancel would show) → **Read mail**: letter list (msg 0x2C in field 351, per-field id
   44-63) → the letter body in window 3 on a darkened screen; **Give**: the Moogle takes the carried letters addressed
   to it, reads each (intro msg, body, reaction) and marks it received. Afterwards the Moogle may receive letters on
   its own (story-gated "auto" letters, read the same way) and may ask you to carry one ("hand-out"). The talk function
   then loops back to msg 3 (cursor on Mognet) until Cancel.
2. **Choice state.** Every list is the generic message-window choice: `g_msgChoice` (0x8007B958, **visible** row among
   the enabled rows), `g_msgChoiceCount` (0x8007B95C, visible rows), `g_msgChoiceEnabled` (0x8007B960, bit k = logical
   row k shown), `g_msgChoiceCancel` (0x8007B96C, visible index of Cancel), `g_msgChoiceWindow` (0x8007B970). The
   script reads the result as system variable 9 = `msg_getChoiceIndex()` (logical row). In the letter list, **logical
   row k = inbox slot k → letter id `b[0x428 + k]`, sender `b[0x437 + k]`**, row 15 = Cancel. **Confirm is the
   script itself**: it polls `msg_getPadPressed() & 0xB0000` (Circle/Cross/Square) once per frame, then closes the
   window and reads the choice. So the port sets `g_msgChoice` (visible index) and ORs `BTN_CROSS` (0x20000) into
   `g_msgPadPressed` (0x8007B97C) **after `msg_update` and before `evt_updateObjects`** in `evt_update`
   (src/main/evt_main.c:12). Verified: Mognet, Read mail and the letter all opened this way without a pad press.
   The letter window (3) closes with `msg_close(3)`; the script waits for that (op 0x54) and continues (verified).
3. **Letter text.** 46 letters, ids **3-48** (switch tables reserve 0-63). The text is not global: each field's own
   message table (`g_msgTable`, 0x8007B954, the area file in FF9.IMG dir 2) holds the bodies of the letters that
   field's Moogle can show (msg id chosen by a per-field switch on the letter id). The body is the message opened with
   **window 3, flags 0x10** (`MSG 03 10 id`): hook `msg_open(msg, win, layout, flags)` (main 0x8004DCEC) or read
   `g_msgWindows[3].msgId`, then `msg_getText(msgId)` (main 0x8004F058). List titles = rows of the list message (row k
   ↔ slot k; rows `Letter 01..14` are unused placeholders). Names: number slot 0 = this Moogle, slot 1 = sender
   (`MSG_NUMBER`, op 0x66), printed from text bank 0 (msg 0 of the table, `F9 04` bank of 41 Moogle names). Full letter
   table below.
4. **Mail state** (all persistent globals): carried letters = 3 slots at `b[0x40A + 4k]` {used, letter, from, to};
   taken bits `b[0x417..0x41E]`, received bits `b[0x41F..0x426]` (letter L ↔ byte `base + L/8`, mask `0x80 >> L%8`);
   `b[0x400]` data version (1); `b[0x408]` letters-given counter; `Gbit[0x213F]` Mognet intro heard. There is **no
   unread flag**: a letter becomes "received" when the Moogle shows it. Port badges: "letter for this Moogle" =
   any used slot whose `to` = the field's Moogle id (table below); "new auto letter / hand-out" = the per-field
   conditions listed below (story counter windows + received/taken bits).
5. **Moogle presence.** Use the game's own talk test: `evt_findNearestObject(player, 4, NULL)` (main 0x80038618,
   src/main/event.c:246) returns the object Cross would talk to (talk radius, height window, in front); enable the
   Mognet button when it is an actor (`kind` 4) with `modelId` 220 or 129, or better, when its `scriptIdx` matches the
   Moogle entry of the current field (table below), and `g_evtState->controlEnabled` is 1. To open the menu without a
   press call `evt_callObjectFunc(moogle, 1, 3, 0)` (main 0x800384D0) - exactly what Cross does - then confirm row 2
   as in answer 2. Verified in Redux: near the Dali Moogle the query returns it (uid 8, model 220), from the inn
   entrance it returns NULL; the native call opened the Moogle menu.

## 1. Script path (field 351, Dali inn, disc 1: Moogle "Gumo", id 7, entry 8)

Field 351's script is item 0x5015F of FF9.IMG dir 4 (file+0xA52FC, 28 entries). Entry 8 is created by `09 08 00`
(uid 8). Init (tag 0, 128 bytes): `2F 00 DC 00 32` SET_MODEL 220, `4B 00 0E 0E 16` SET_RADIUS (talk radius 0x16),
`05 D5 25 7D 07 00 2C 7F` Fu8[0x25] = 7 (this Moogle's id), `05 D2 01 ...` Ls8[1] = -1 (no shop). Tag 1 is the idle
loop. Tag 3 (6707 bytes) is the talk function. Listing made with `build/3ds5m/edis.py 351 --entry 8 --tag 3`
(scratch; evtdis plus an infix decoder for expressions, see "Expression encoding").

Main menu (0x0000-0x0139):
```
  0000: 05 7a 02 7f                 EXPR               sys.controlEnabled          ; 03 01 00 JMP_IF ->8, else 04 RETURN
  000e: 05 d6 00 7d 01 00 20 7f     EXPR               (Lu8[0x0] == 1)             ; Moogle active, else RETURN
  0066: 10 00 04 fa 16              CALL               0x4, 0xfa, 0x16             ; player's talk pose
  007e: 2d                          DISABLE_CONTROL
  00a2: 05 d6 09 7d 7f 00 3d 7f     EXPR               (Lu8[0x9] &= 127)           ; last row (cursor default)
  00aa: 05 d6 0a 7d 47 00 e5 44 ..  EXPR               (Lu8[0xa] = (71 | (Fbit[0x144] << 4)))
  00ba: 05 d2 01 7d 00 00 1b 7f     EXPR               (Ls8[0x1] >= 0)  -> Lu8[0xa] |= 8 (Mogshop row)
  00db: 7c 03 d6 0a 7f d6 09 7f     MSG_CHOICE_MASK    {Lu8[0xa]}, {Lu8[0x9]}
  00e3: 20 00 02 08 03 00           MSG                win 2, flags 8 (MOGNET title), msg 3
  00e9: 22 00 01                    WAIT               1
  00ec: 05 7e 00 00 0b 00 4f 0e 7f  EXPR               !padPressed(0xb0000)        ; 03 f1 ff JMP_IF ->0xe9
  00f8: 21 00 02                    MSG_CLOSE          2
  0118: 05 7a 09 7f                 EXPR               sys.msgChoice
  011c: 0b 06 00 00 c2 18 11 00 ..  SWITCH_TABLE       0 Save->0x12e 1 Tent->0x318 2 Mognet->0x652 3 Shop->0x1978
                                                       4 Party->0x199c 5 Debug->0x19dc, default (Cancel)->0x19df
  ...
  19ed: 05 d6 09 7d 80 00 18 7f     EXPR               (Lu8[0x9] < 128)            ; 03 d8 e6 JMP_IF ->0xd0 (menu again)
```
Save = `75 00 04 00` MENU(4, 0); Mogshop = `75 02 02 d2 01 7f` MENU(2, Ls8[1]); Tent uses item 253 and ops F1/F2.

Mognet (0x0652-0x19EA), in order:
- 0x0668: `(b[0x400] < 1) && (b[0x40A] || b[0x40E] || b[0x412])` → msg 0xA "Old letter data. Erasing..." and zero
  `b[0x40A..0x415]`; then `b[0x400] = 1`.
- 0x0710: `Gbit[0x213F] == 0 && h[0] < 5990` → intro msgs 0x22-0x26 ("Mognet is a mail delivery system between
  moogles!"), `Gbit[0x213F] = 1`. (Moogle 36 has its own "What's Mognet?" variant on `Gbit[0x213E]`, msg 0x2A.)
- 0x0782 (loop head): count used slots → field var; `MSG_NUMBER 2..7` = from/to of slots 0-2; msg `0xB + count`
  (0xB "You have no mail", 0xC-0xE "You have a letter from <2> to <3>" ... one line per slot) in window 5, flags 8.
- 0x086E-0x0B0D, the Mognet menu and Read mail:
```
  086e: 05 d9 21 7d 00 00 2c 7f     EXPR   (Fs16[0x21] = 0)
  0876: 05 f4 0a 04 7d 00 00 21 ..  EXPR   (b[0x40a] && b[0x40d] == Fu8[0x25]) || (slot 1) || (slot 2) -> Fs16[0x21] = 1
  08af: 05 d9 23 7d 00 00 2c 7f     EXPR   (Fs16[0x23] = 0)                     ; inbox mask
  08b7: 05 e4 fa 20 7f              EXPR   Gbit[0x20fa]                         ; letter 5 received?
  08bf: 05 f4 28 04 7d 05 00 2c 7f  EXPR   (b[0x428] = 5)                       ; slot 0 = letter 5
  08c8: 05 d9 23 7d 01 00 3f 7f     EXPR   (Fs16[0x23] |= 1)
  08d0: 05 f4 37 04 7d 06 00 2c 7f  EXPR   (b[0x437] = 6)                       ; sender Mois
  08e4: 05 d9 21 7d 02 00 3f 7f     EXPR   (Fs16[0x21] |= 2)   if inbox non-empty
  08ec: 05 d9 21 7f / 02 1a 02      JMP_IFNOT ->0xb0d                           ; nothing to offer: skip the menu
  08f3: 7c 01 d9 21 7d 04 00 26 7f 00  MSG_CHOICE_MASK {(Fs16[0x21] | 4)}, 0
  08fd: 20 00 02 08 08 00           MSG    win 2, flags 8, msg 8   "Give <0> a letter / Read mail / Cancel"
  0903..0912                        wait for padPressed(0xb0000), MSG_CLOSE 2
  0936: 0b 03 00 00 d6 01 0b 00 ..  SWITCH_TABLE sys.msgChoice: 0 Give->0x942  1 Read->0x95e  2 Cancel->0xb02
  094d: 05 d5 2a 7d 01 00 2c 7f     EXPR   (Fu8[0x2a] = 1)                      ; Give
  0955: 05 f4 08 04 04 7f           EXPR   b[0x408]++
  0970: 7c 01 d9 23 7e 00 80 00 00 26 7f 00  MSG_CHOICE_MASK {(Fs16[0x23] | 0x8000)}, 0
  097c: 20 00 02 08 2c 00           MSG    win 2, flags 8, msg 0x2c  "Which letter do you want to read? / Trapped In Ice! /
                                           Letter 01 .. Letter 14 / Cancel"
  09bd: 0b 0a 00 00 cd 00 ..        SWITCH_TABLE sys.msgChoice, rows 0-9:
  09d7: 05 d5 1f f4 28 04 2c 7f       (Fu8[0x1f] = b[0x428 + k]) ; 66 02 01 f4 37 04 7f MSG_NUMBER 1, b[0x437 + k]
  0a8b: 05 7e 00 00 01 00 4f 7f       padPressed(0x10000) (Circle) -> Fu8[0x2a] = 3
  0a9e: 21 00 05                    MSG_CLOSE 5
  0aa1: 05 7a 09 7d 0f 00 18 7f     EXPR   (sys.msgChoice < 15)
  0aaf: ec 04 02 18 d5 11 7f dc dc fa  FADE   (darken)
  0ac0: 0b 01 05 00 10 00 07 00     SWITCH_TABLE Fu8[0x1f]: letter 5 -> 0x0ac8
  0ac8: 20 00 03 10 2e 00           MSG    win 3, flags 0x10 (frameless), msg 0x2e   = the letter body
  0ad2: 54 00 03                    MSG_WAIT_WIN 3
  0af2: ec 04 07 10 d5 11 7f 00 00 00  FADE (back)
```
- 0x0B0D Give: for each slot with `to == Fu8[0x25]`: copy {letter, from, to} to field vars, remove the slot (slots
  above move down), then (0x0CF0) msg 0x2D ("A letter from <1>? Hmm... I don't like <1>, but I'll read it anyway"),
  fade, body (window 3 flags 0x10, msg 0x2E), fade back, set the received bit with a 64-way switch (0x0D7C:
  letter L → `Gbit[0x20FF - ...]`, i.e. `b[0x41F + L/8] |= 0x80 >> L%8`), reaction msg 0x2F; 0x1117: one-time
  item 0x126 (msg 0x1B) when `Fu8[0x2a] == 1 && !item(294) && h[0] > 3740`. Loops while letters remain.
- Auto letters (not in 351; e.g. field 706 0x0DB6): `(Gbit[0x20f8] == 0) && (Gu16[0x13c] >= 100)` → count++, sender 9,
  letter 7; shown exactly like a given letter.
- Hand-out (not in 351; e.g. field 300 0x0D8D): `h[0] < 2990 && Gbit[0x20BA] == 0` → letter 5 for Moogle 7. If you
  accept, the taken bit is set (64-way switch at 0x136A: `b[0x417 + L/8] |= 0x80 >> L%8`) and the first free slot
  gets {1, letter, this Moogle, destination} (0x16F0). With three letters carried the Moogle first asks which one to
  drop (msg 0x1E/0x1F, slots compacted).
- 0x1859: nothing happened → story-dependent chatter (msg 0x13-0x18, e.g. 0x13 "I want mail! Kupo!").
- Back to the main menu (0x19EA → 0x19ED → 0x00D0).

Moogle main menu rows (msg 3 of field 351): 0 Save, 1 Tent, 2 Mognet, 3 Mogshop, 4 Switch party members, 5 Debug,
6 Cancel. The second argument of `MSG_CHOICE_MASK` (op 0x7C) is the default logical row (`msg_setChoiceMask`,
src/main/msg.c:1162, converts it to the visible index in `g_msgChoiceDefault`, which `msg_open` copies to
`g_msgChoice`).

## 2. Choice window and confirm

- `msg_input` (main 0x8004DAD0, src/main/msg.c:719) is the window's pad handler, called from `msg_update`
  (src/main/msg.c:1039) for the focused window: Up/Down move `g_msgChoice` (wrapping over `g_msgChoiceCount`), Circle
  sets `g_msgChoice = g_msgChoiceCancel`. It does **not** confirm.
- Confirm = the script loop `22 00 01 / 05 7e 00 00 0b 00 4f 0e 7f / 03 f1 ff` (WAIT 1; JMP_IF !padPressed(0xB0000)).
  Expression op 0x4F tests `msg_getPadPressed()` = `g_msgPadPressed & 0x3FFFFFF`, recomputed by `msg_readPad` at the
  start of `msg_update`. Then the script closes the window and reads sysvar 9 (`msg_getChoiceIndex`, msg.c:1147: the
  visible `g_msgChoice` mapped to the logical row through `g_msgChoiceEnabled`).
- Frame order: `evt_update` (src/main/evt_main.c:12) = `msg_update()` → `evt_updateIdleTimer()` →
  `evt_updateObjects()` (runs the scripts) → bubbles/ATE. Port recipe for a tap on logical row k:
  `g_msgChoice = popcount(g_msgChoiceEnabled & ((1 << k) - 1)); g_msgPadPressed |= BTN_CROSS;` between `msg_update`
  and `evt_updateObjects` (or set it before `evt_update` and skip `msg_readPad`'s overwrite). Circle-cancel: set the
  Cancel row the same way (the script plays the cancel sound itself when it sees row 15 with Cross/Square).
- Letter list mask: `g_msgChoiceEnabled = inbox mask | 0x8000`. In the Dali save: 0x8001 (row 0 "Trapped In Ice!",
  Cancel), `g_msgChoiceCount` 2, `g_msgChoiceCancel` 1. Mognet menu mask 6 (Read mail, Cancel) or 5/7 with a letter to
  give.
- Closing the letter: the body window waits for Cross (`MSGW_WAIT_INPUT`); the script only waits for window 3 to
  close (op 0x54 → `EVT_WAIT_WINDOW`, `msg_isOpen`). Calling `msg_close(3)` (main 0x8004EA4C) natively closed it and
  the script went on (fade back, then the Moogle's closing line).

## 3. Letters

Each field's area text (FF9.IMG dir 2, file = the field's link id, DB item kind 6; at runtime `g_msgTable`) contains:
msg 0 = text bank of the 41 Moogle names (`F9 04`; index = Moogle id: 0 Ruby, 1 Kupo, 2 Mosh, 3 Mosco, 4 Monty,
5 Mogpi, 6 Mois, 7 Gumo, 8 Kumop, 9 Moodon, 10 Mogki, 11 Moonte, 12 Mogmi, 13 Atla, 14 Grimo, 15 Nazna, 16 Mogrich,
17 Mochos, 18 Monev, 19 Mopli, 20 Serino, 21 Mogrody, 22 Mozme, 23 Kuppo, 24 Mogryo, 25 Mogmatt, 26 Suzuna,
27 Mocchi, 28 Mojito, 29 Mogsam, 30 Mimoza, 31 Mooel, 32 Moolan, 33 Mogtaka, 34 Kumool, 35 Mogrika, 36 Moorock,
37 Noggy, 38 Stiltzkin, 39 Artemicion, 40 Mogribs), msg 3 main menu, msg 8 Mognet menu, msg 0xB-0x12 status lines,
the list message (0x2C in most fields; 44-63) and per-letter intro / body / reaction messages. Body layout (field 351
msg 0x2E): `F9 02 FF` then layout codes (`F9 4B`, `F9 44`, `F9 49`, `F9 4A`, `F9 43`), title line
`F9 25 "From " F9 06 00 01 " to " F9 06 00 00 F9 20` (names from bank 0 by number slots 1 and 0), then the text.
`tools/ff9text.py decode` handles the characters; it stops at the `FF` of `F9 02 FF`, so skip the first three bytes.

Letter table (built from all 58 Moogle talk functions by `build/3ds5m/mogscan.py` + `lettab.py`, scratch). "auto" =
the recipient gets it by itself (conditions in section 4); other letters are handed out by the sender's Moogle.

| letter | title (list row) | from -> to | received bit | taken bit | hand-out fields | read-mail fields |
|---|---|---|---|---|---|---|
| 3 | Jump-rope Champion Appears! | Kupo -> Mosh | b[0x41f] & 0x10 | b[0x417] & 0x10 | auto | 150,1205,1213,1803 |
| 4 | Prince on a White Horse | Kupo -> Monty | b[0x41f] & 0x08 | b[0x417] & 0x08 | 115,1865,2456 | 253,1252 |
| 5 | Trapped In Ice! | Mois -> Gumo | b[0x41f] & 0x04 | b[0x417] & 0x04 | 300,306,662 | 351 |
| 6 | Stiltzkin Visited Me! | Kumop -> Mogki | b[0x41f] & 0x02 | b[0x417] & 0x02 | 407 | 611,1418,2161 |
| 7 | It Was So Exciting! | Moodon -> Mogmi | b[0x41f] & 0x01 | b[0x417] & 0x01 | auto | 706 |
| 8 | Map of the Entire World in His Bag? | Mogki -> Atla | b[0x420] & 0x80 | b[0x418] & 0x80 | 611,1418,2161 | 764 |
| 9 | Mary's Unrequited Love | Grimo -> Nazna | b[0x420] & 0x40 | b[0x418] & 0x40 | 802 | 810 |
| 10 | Am I Right? | Nazna -> Mochos | b[0x420] & 0x20 | b[0x418] & 0x20 | 810 | 950 |
| 11 | No More Pointy Hats! | Atla -> Monev | b[0x420] & 0x10 | b[0x418] & 0x10 | 764 | 1008 |
| 12 | I Have a Bad Feeling | Monev -> Mopli | b[0x420] & 0x08 | b[0x418] & 0x08 | auto | 1056,1102,1106,1109 |
| 13 | In Danger | Mopli -> Serino | b[0x420] & 0x04 | b[0x418] & 0x04 | 1056,1102,1106,1109 | 1152 |
| 14 | Eidolon Odin's Power | Serino -> Moodon | b[0x420] & 0x02 | b[0x418] & 0x02 | 1152 | 554,1304,2104 |
| 15 | Rumor About Princess Garnet | Kupo -> Mosh | b[0x420] & 0x01 | b[0x418] & 0x01 | auto | 150,1205,1213,1803 |
| 16 | Escaping Evil Forest | Mosh -> Monty | b[0x421] & 0x80 | b[0x419] & 0x80 | 150,1205,1213,1803 | 253,1252 |
| 17 | Missing!  Kupo! | Moodon -> Moonte | b[0x421] & 0x40 | b[0x419] & 0x40 | 554,1304,2104 | 602,1352,2152 |
| 18 | Very Bored, Kupo! | Kuppo -> Mogki | b[0x421] & 0x20 | b[0x419] & 0x20 | auto | 611,1418,2161 |
| 19 | The Most Important Thing in Life | Kuppo -> Kupo | b[0x421] & 0x10 | b[0x419] & 0x10 | 1421 | 115,1865,2456 |
| 20 | Rally-Kupo! | Mogmatt -> Suzuna | b[0x421] & 0x08 | b[0x419] & 0x08 | 1509 | 1553 |
| 21 | `<F911>`'s Eyes | Mogryo -> Mocchi | b[0x421] & 0x04 | b[0x419] & 0x04 | 1458,3057 | 1652,1663,1759 |
| 22 | Vanity | Mogrika -> Kupo | b[0x421] & 0x02 | b[0x419] & 0x02 | auto | 115,1865,2456 |
| 23 | How Ya Doin'? | Ruby -> Mosco | b[0x421] & 0x01 | b[0x419] & 0x01 | auto | 206,853 |
| 24 | Stiltzkin on Ice | Stiltzkin -> Monty | b[0x422] & 0x80 | b[0x41a] & 0x80 | auto | 253,1252 |
| 25 | Opening a Mini-Theater | Ruby -> Moodon | b[0x422] & 0x40 | b[0x41a] & 0x40 | auto | 554,1304,2104 |
| 26 | Shelter From the Rain | Stiltzkin -> Moonte | b[0x422] & 0x20 | b[0x41a] & 0x20 | auto | 602,1352,2152 |
| 27 | Vube Desert | Stiltzkin -> Mogrich | b[0x422] & 0x10 | b[0x41a] & 0x10 | auto | 904,1904 |
| 28 | Tantalus | Ruby -> Mopli | b[0x422] & 0x08 | b[0x41a] & 0x08 | auto | 1056,1102,1106,1109 |
| 29 | This Might Be the End | Stiltzkin -> Monty | b[0x422] & 0x04 | b[0x41a] & 0x04 | auto | 253,1252 |
| 30 | Narcissus From Lindblum | Ruby -> Moodon | b[0x422] & 0x02 | b[0x41a] & 0x02 | auto | 554,1304,2104 |
| 31 | To Conde Petie | Stiltzkin -> Mogryo | b[0x422] & 0x01 | b[0x41a] & 0x01 | auto | 1458,3057 |
| 32 | Blessing or Curse? | Stiltzkin -> Mocchi | b[0x423] & 0x80 | b[0x41b] & 0x80 | auto | 1652,1663,1759 |
| 33 | My Vagabond Life | Stiltzkin -> Kupo | b[0x423] & 0x40 | b[0x41b] & 0x40 | auto | 115,1865,2456 |
| 34 | New Champion | Kupo -> Mogrich | b[0x423] & 0x20 | b[0x41b] & 0x20 | auto | 904,1904 |
| 35 | Alexandria Destroyed | Mogki -> Moodon | b[0x423] & 0x10 | b[0x41b] & 0x10 | 611,1418,2161 | 554,1304,2104 |
| 36 | Very Mad!  Kupo! | Mimoza -> Mooel | b[0x423] & 0x08 | b[0x41b] & 0x08 | 2250 | 2259 |
| 37 | Stiltzkin, On the Move | Mojito -> Mogsam | b[0x423] & 0x04 | b[0x41b] & 0x04 | 2203 | 2220 |
| 38 | Favor | Artemicion -> Mogrika | b[0x423] & 0x02 | b[0x41b] & 0x02 | auto | 2304 |
| 39 | Problem | Mogrika -> Moolan | b[0x423] & 0x01 | b[0x41b] & 0x01 | 2304 | 2354 |
| 40 | Where Is Mognet Central? | Moolan -> Mogtaka | b[0x424] & 0x80 | b[0x41c] & 0x80 | 2354 | 2360 |
| 41 | Where Is Mognet Central? | Mogki -> Kumool | b[0x424] & 0x40 | b[0x41c] & 0x40 | 611,1418,2161 | 2504 |
| 42 | My First Mognet | Moorock -> Mozme | b[0x424] & 0x20 | b[0x41c] & 0x20 | 2655,2706 | 2714 |
| 43 | Something Missing | Kupo -> Atla | b[0x424] & 0x10 | b[0x41c] & 0x10 | 115,1865,2456 | 764 |
| 44 | That Special Something | Atla -> Mogryo | b[0x424] & 0x08 | b[0x41c] & 0x08 | 764 | 1458,3057 |
| 45 | Rare Item | Mogryo -> Kumool | b[0x424] & 0x04 | b[0x41c] & 0x04 | 1458,3057 | 2504 |
| 46 | Where Is That Item!? | Kumool -> Mois | b[0x424] & 0x02 | b[0x41c] & 0x02 | 2504 | 300,306,662 |
| 47 | It's That Thing | Mois -> Noggy | b[0x424] & 0x01 | b[0x41c] & 0x01 | 300,306,662 | 2801 |
| 48 | Superslick | Noggy -> Kupo | b[0x425] & 0x80 | b[0x41d] & 0x80 | 2801 | 115,1865,2456 |

Read mail rebuilds the list on every visit from the received bits of the letters addressed to this Moogle (slot order
fixed per field), so the inbox of a Moogle = letters with `to` = its id and the received bit set. For the bottom
screen the port can draw the list from `g_msgChoiceEnabled` and the list message's rows while msg 0x2C (or the
field's list id) is open in window 2, or build it itself from the received bits and this table (titles and bodies
then still come from the current field's `g_msgTable`).

## 4. Mail state

| offset (`b[]`, address) | type | meaning |
|---|---|---|
| 0x400 (0x8007B2F0) | u8 | Mognet data version; 0 with letters in the slots → "Old letter data. Erasing..." then 1 |
| 0x408 (0x8007B2F8) | u8 | +1 each time "Give <moogle> a letter" is chosen; printed by field 3100 (disc 3, `MSG_NUMBER 7`) |
| 0x409 | u8 | only `++` under an always-false branch (dead) |
| 0x40A + 4k, k = 0..2 (0x8007B2FA) | u8 x4 | carried letter slot k: +0 used (1), +1 letter id, +2 from (Moogle id), +3 to (Moogle id); used slots are packed from slot 0 |
| 0x417-0x41E (0x8007B307) | bits | letter taken from its sender (`Gbit[0x20BF - ...]`, mask `0x80 >> L%8` of byte `0x417 + L/8`) |
| 0x41F-0x426 (0x8007B30F) | bits | letter received by its recipient (shown once; then listed under Read mail) |
| 0x427 bit 7 (Gbit 0x213F) | bit | Mognet intro heard; bit 6 (0x213E) Moorock's intro |
| 0x428-0x431 | u8 x10 | scratch: letter id of list row k (rebuilt per talk) |
| 0x437-0x440 | u8 x10 | scratch: sender of list row k |

Runtime (Dali save, `build3ds5m_mognet_menu`): `b[0x400..0x415]` = `01 00 .. 02 01 | 01 06 08 0a | 00 ..` (one letter
carried: letter 6 Kumop → Mogki, shown as "You have a letter from Kumop to Mogki"), taken `b[0x417]` = 0x0E (letters
4, 5, 6), received `b[0x41F]` = 0x0C (4, 5), `b[0x422]` = 0x80 (24), list scratch `b[0x428]` = 5, `b[0x437]` = 6.

Auto letters (the recipient's talk function counts them as "letters for me" before the hand-out check; same intro /
body / reaction as a given letter). Conditions from `build/3ds5m/autolet.py` (scratch):

| fields | moogle | letter | condition |
|---|---|---|---|
| 115,1865,2456 | 1 | 22 | `!Gbit[0x2109] && h[0] < 8800 && b[0x1fc] >= 2` |
| 115,1865,2456 | 1 | 33 | `!Gbit[0x211e] && 7100 <= h[0] < 8800` |
| 150,1205,1213,1803 | 2 | 3 | `!Gbit[0x20fc] && s24 b[0x1f0] >= 50 && h[0] < 1900` |
| 150,1205,1213,1803 | 2 | 15 | `5030 <= h[0] < 5120 && !Gbit[0x2100]` |
| 206,853 | 3 | 23 | `!Gbit[0x2108] && 2200 <= h[0] < 2400` |
| 253,1252 | 4 | 24 | `Gbit[0x20fb] && h[0] < 2400` (and `!Gbit[0x2117]`) |
| 253,1252 | 4 | 29 | `!Gbit[0x2112] && Gbit[0x835]` |
| 554,1304,2104 | 9 | 25 | `!Gbit[0x2116] && h[0] < 5598` |
| 554,1304,2104 | 9 | 30 | `!Gbit[0x2111] && 4600 < h[0] < 5990` |
| 602,1352,2152 | 11 | 26 | `!Gbit[0x2115] && 3700 <= h[0] < 5598` |
| 611,1418,2161 | 10 | 18 | `5600 <= h[0] < 5990 && !Gbit[0x210d] && !Gbit[0xcb7]` |
| 706 | 12 | 7 | `!Gbit[0x20f8] && u16 b[0x13c] >= 100` |
| 904,1904 | 16 | 27 | `!Gbit[0x2114] && h[0] < 4990` |
| 904,1904 | 16 | 34 | `b[0x1eb] >= 20 && !Gbit[0x211d] && 6990 < h[0] < 8800` |
| 1056,1102,1106,1109 | 19 | 28 | `!Gbit[0x2113] && h[0] < 4800` |
| 1056,1102,1106,1109 | 19 | 12 | `!Gbit[0x2103] && 4800 <= h[0] < 4900` |
| 1458,3057 | 24 | 31 | `!Gbit[0x2110] && 6212 <= h[0] < 6300` |
| 1652,1663,1759 | 27 | 32 | `!Gbit[0x211f] && h[0] >= 6875` |
| 2304 | 35 | 38 | `!Gbit[0x2119] && 9910 <= h[0] < 9990` |

Hand-outs (offered after the Give/auto part; accepted letters set the taken bit and fill a slot; nothing is offered
while the sender's taken bit is set):

| fields | moogle | letter → to | condition |
|---|---|---|---|
| 115,1865,2456 | 1 | 4 → 4 | `h[0] < 1900` |
| 115,1865,2456 | 1 | 43 → 13 | `h[0] >= 11100` |
| 150,1205,1213,1803 | 2 | 16 → 4 | `5120 <= h[0] < 6990` |
| 300,306,662 | 6 | 5 → 7 | `h[0] < 2990` |
| 300,306,662 | 6 | 47 → 37 | `Gbit[0x2121] && h[0] >= 11100` |
| 407 | 8 | 6 → 10 | `h[0] < 2900` |
| 554,1304,2104 | 9 | 17 → 11 | `!Gbit[0x210e] && 4600 < h[0] < 5990` |
| 611,1418,2161 | 10 | 8 → 13 | `h[0] < 3740 && Gbit[0x20f9]` |
| 611,1418,2161 | 10 | 35 → 9 | `8800 <= h[0] < 9400` |
| 611,1418,2161 | 10 | 41 → 34 | `9990 <= h[0] < 10600` |
| 764 | 13 | 11 → 18 | `h[0] < 3900` |
| 764 | 13 | 44 → 24 | `Gbit[0x2124] && h[0] >= 11100` |
| 802 | 14 | 9 → 15 | `h[0] < 4990` |
| 810 | 15 | 10 → 17 | `Gbit[0x2106] && h[0] < 4990` |
| 1056,1102,1106,1109 | 19 | 13 → 20 | `h[0] >= 4900` |
| 1152 | 20 | 14 → 9 | `Gbit[0x2102]` |
| 1421 | 23 | 19 → 1 | none besides the taken bit |
| 1458,3057 | 24 | 21 → 27 | `h[0] < 6212` |
| 1458,3057 | 24 | 45 → 34 | `Gbit[0x2123] && h[0] >= 11100` |
| 1509 | 25 | 20 → 26 | `h[0] < 6200` |
| 2203 | 28 | 37 → 29 | none |
| 2250 | 30 | 36 → 31 | none |
| 2304 | 35 | 39 → 32 | `Gbit[0x2119] && h[0] < 9990` |
| 2354 | 32 | 40 → 33 | `Gbit[0x2118] && h[0] < 9990` |
| 2504 | 34 | 46 → 6 | `Gbit[0x2122] && h[0] >= 11100` |
| 2655,2706 | 36 | 42 → 22 | none |
| 2801 | 37 | 48 → 1 | `Gbit[0x2120] && h[0] >= 11100` |

Badges the port can compute from globals alone: "carrying N letters" (used slots); "letter to give here" (a used slot
with `to` = this field's Moogle id); "Moogle has news" (any auto-letter or hand-out row of this field whose condition
holds and whose received / taken bit is clear). The per-field field-bank flags (e.g. 351's `Fbit[0x145]`) only gate
cosmetic paths.

## 5. Moogle presence

Moogle objects: actors with `modelId` 220 (51 fields) or 129 (706, 810, 1152, 1553, 2250, 2304, 2714), talk radius
0x16 (op `4B 00 0E 0E 16`), talk function = tag 3. Moogle entry (= uid) and id per field:

| field | disc | entry | id | field | disc | entry | id | field | disc | entry | id |
|---|---|---|---|---|---|---|---|---|---|---|---|
| 115 | 1 | 2 | 1 | 802 | 1 | 5 | 14 | 1458 | 2 | 5 | 24 |
| 150 | 1 | 10 | 2 | 662 | 2 | 6 | 6 | 1509 | 2 | 10 | 25 |
| 206 | 1 | 19 | 3 | 810 | 2 | 14 | 15 | 1553 | 2 | 7 | 26 |
| 253 | 1 | 4 | 4 | 904 | 2 | 16 | 16 | 1652 / 1663 / 1759 | 2 | 7 / 12 / 3 | 27 |
| 300 | 1 | 3 | 6 | 950 | 2 | 9 | 17 | 853 | 3 | 2 | 3 |
| 306 | 1 | 13 | 6 | 1008 | 2 | 3 | 18 | 1803 | 3 | 5 | 2 |
| 351 | 1 | 8 | 7 | 1056 | 2 | 9 | 19 | 1865 | 3 | 5 | 1 |
| 407 | 1 | 5 | 8 | 1102 / 1106 / 1109 | 2 | 3 / 11 / 16 | 19 | 1904 | 3 | 16 | 16 |
| 554 | 1 | 7 | 9 | 1152 | 2 | 2 | 20 | 2104 | 3 | 5 | 9 |
| 602 | 1 | 8 | 11 | 1205 / 1213 | 2 | 11 / 5 | 2 | 2152 / 2161 | 3 | 6 / 8 | 11 / 10 |
| 611 | 1 | 10 | 10 | 1252 | 2 | 10 | 4 | 2203 / 2220 | 3 | 8 / 14 | 28 / 29 |
| 706 | 1 | 3 | 12 | 1304 | 2 | 6 | 9 | 2250 / 2259 | 3 | 4 / 8 | 30 / 31 |
| 764 | 1 | 4 | 13 | 1352 | 2 | 5 | 11 | 2304 / 2354 / 2360 | 3 | 1 / 2 / 2 | 35 / 32 / 33 |
| | | | | 1418 / 1421 | 2 | 8 / 8 | 10 / 23 | 2456 / 2504 | 3 | 7 / 15 | 1 / 34 |
| | | | | | | | | 2655 / 2706 / 2714 | 3 | 8 / 5 / 21 | 36 / 36 / 22 |
| | | | | | | | | 2801 | 3 | 19 | 37 |
| | | | | | | | | 3057 | 4 | 4 | 24 |

(The 7 Memoria save spheres of disc 4 have no Mognet: in the dir-3/4 scripts of all discs only these 58 talk functions touch `b[0x40A]` or
`b[0x428]`.)

Check per frame (field mode, `g_evtState->controlEnabled` = 1): `o = evt_findNearestObject(player, 4, NULL)`; enabled
when `o != NULL && o->kind == 4 && o->scriptIdx == moogleEntry[field]` (or modelId 220/129), plus
`|evt_angleToObject(player, o, player->pos.rotY) + 0x3FF| < 0x7FF` as `evt_checkPlayerAction` (event.c:2770) does
before it calls `evt_callObjectFunc(o, 1, EVT_FUNC_TALK, 0)`. The talk function itself returns at once unless control
is enabled and the Moogle's local var 0 is 1, so a Moogle that is hidden or busy in a story scene does not open.
Objects: `g_evtState` (0x8007AECC) → `EvtState.objects` (+0x16D8, `EvtListNode {next, obj}`), `playerUid` +0x16ED;
`EvtObject` +5 kind, +6 scriptIdx, +7 uid, +0x34 modelId, +0x3D talkRadius.

## Expression encoding (for reading the listings)

Event-script expressions (op 0x05 and flagged arguments) are RPN, ending at 0x7F (`evt_runExpr`, jump table
0x8006E718). Operands: `7D s16` literal, `7E u32` (26-bit) literal, `78 uid prop` object property, `79 n` battle var,
`7A n` system var (`evt_getSysVar`: 2 controlEnabled, 9 msgChoice, 0 rand, ...), `29 n` property of the expression
object, `5F uid` object id. A byte >= 0x80 is a variable: `0x80 | 0x20 (16-bit index follows) | type (0x1C) | bank (3)`;
type 0x00/0x04 bit, 0x08/0x0C 24-bit, 0x10 s8, 0x14 u8, 0x18 s16, 0x1C u16; bank 0 global `g_evtGlobalVars`, 1 field
(`g_evtState->fieldVars`), 2/3 the object's locals (`g_evtObjVars`); bit 0x40 doesn't change the variable. Operators
(right operand popped first): 0x04/0x05 post-inc/dec, 0x06/0x07 pre-inc/dec, 0x0D neg, 0x0E `!`, 0x10 `~`, 0x11 `*`,
0x12 `/`, 0x13 `%`, 0x14 `+`, 0x15 `-`, 0x16 `<<`, 0x17 `>>`, 0x18 `<`, 0x19 `>`, 0x1A `<=`, 0x1B `>=`, 0x20 `==`,
0x21 `!=`, 0x24 `&`, 0x25 `^`, 0x26 `|`, 0x27 `&&`, 0x28 `||`, 0x2C `=`, 0x2F-0x35 and 0x3D-0x3F compound
assignments, 0x4F/0x58/0x59 pad pressed/released/held `& x`, 0x64 item count, 0x6B in party.

## Runtime evidence (PCSX-Redux, disc 1)

- `build3ds2_mog_choice` (Moogle menu, Mognet highlighted, `g_msgChoice` 2, mask 0x47): `until evt_updateObjects`,
  `write g_msgPadPressed 0x20000 --size 4`, 90 frames → Mognet menu "Give/Read mail/Cancel" with only Read mail and
  Cancel (`g_msgChoiceEnabled` 6, count 2) and window 5 "You have a letter from Kumop to Mogki". Saved as
  `build3ds5m_mognet_menu`.
- Same poke with `g_msgChoice` 0 → letter list "Which letter do you want to read? / Trapped In Ice! / Cancel"
  (`g_msgChoiceEnabled` 0x8001, count 2, cancel 1). Saved as `build3ds5m_letter_list`.
- Poke with row 0 → screen darkens, window 3 opens with msgId 0x2E, flags 0x10 ("From Mois to Gumo / Yo! Man, I got
  totally iced up by some dude with wings..."). Saved (text typing) as `build3ds5m_letter_open`.
- From `build3ds5m_letter_open`: `until msg_update`, then pc = `msg_close`, a0 = 3 (so msg_close runs instead of that
  frame's msg_update and returns to evt_update) → the letter is gone, the screen fades back and the Moogle says "I want
  mail! Kupo!" (msg 0x13, window 1).
- `field_dali_inn_moogle`: same hijack into `evt_findNearestObject(0x8007805C, 4, NULL)` → 0x80078588 (kind 4,
  scriptIdx 8, uid 8, modelId 220, talkRadius 22). `field_dali_inn` (inn entrance): NULL. Hijack into
  `evt_callObjectFunc(0x80078588, 1, 3, 0)` → the Moogle menu opens (`g_msgChoiceEnabled` 0x47).
- Give: `field_dali_inn_moogle` with `b[0x40A..0x40D]` = 01 05 06 07 (letter 5, Mois → Gumo) and `b[0x41F]` bit 0x04
  cleared, native talk, Mognet (row 2) → Mognet menu "Give Gumo a letter / Cancel" (mask 5); row 0 → `b[0x408]` 2 → 3,
  slot 0 emptied, msg "A letter from Mois? Hmm... I don't like Mois, but I'll read it anyway. Kupo."; after the letter
  `b[0x41F]` = 0x0C again (received bit set), then the talk function returned to the main menu with the cursor on
  Mognet.
- Scripts used (scratch, `build/3ds5m/`): `edis.py` (disassembler with expressions), `mogscan.py` (survey of the 58
  talk functions and their texts), `lettab.py`, `autolet.py [hand]`, `condtab.py`, `msgdump.py` (decode the live
  `g_msgTable`), `nearq.py STATE [--talk]`, `givetest.py`, `bytescan.py`.

## Proposed names (not applied)

- `#define EVT_SYSVAR_MSG_CHOICE 9` (evt_getSysVar case 9 = `msg_getChoiceIndex()`), `EVT_SYSVAR_CONTROL_ENABLED 2`.
- Expression ops for a future evaluator header: `EVT_EXPR_PAD_PRESSED 0x4F`, `EVT_EXPR_END 0x7F`, `EVT_EXPR_SYSVAR 0x7A`,
  `EVT_EXPR_CONST16 0x7D`, `EVT_EXPR_CONST26 0x7E`, `EVT_EXPR_OBJPROP 0x78`.
- Script-global offsets for ff9.h next to `EVT_GB_ATE_*`: `EVT_GB_MOGNET_VERSION 0x400`, `EVT_GB_MOGNET_GIVE_COUNT
  0x408`, `EVT_GB_MOGNET_SLOTS 0x40A` (3 x {used, letter, from, to}), `EVT_GB_MOGNET_TAKEN 0x417` (bits, MSB first),
  `EVT_GB_MOGNET_RECEIVED 0x41F` (bits, MSB first), `EVT_GB_MOGNET_LIST_LETTER 0x428`, `EVT_GB_MOGNET_LIST_SENDER 0x437`.
- `EVT_FUNC_TALK` is already 3; `MOOGLE_MODEL 220` / `MOOGLE_MODEL_ALT 129` if the port wants them in a header.
