# Active Time Events: catalogue for test fixtures

Request from the 3DS port: every ATE with what a test needs to start it, in their scenario format
(`kind=field, disc, field, story=N, event.K=V`; `event.K` = byte K of `g_evtGlobalVars`, the offsets the
`EVT_GB_*` macros in `include/ff9.h` use, multi-byte values little-endian). Static analysis of the field scripts on
all four discs; **nothing here was run in the emulator** except the Lindblum fixture already verified in R36
(`docs/r32/3ds-requests-6.md`). Each row carries a confidence; "runtime check" marks what needs one.

Tool: `tools/docker.sh python3 tools/ate_list.py [--disc N] [--field F] [--json OUT]` lists every ATE menu (rows,
title cards, target fields, arrival codes), every title card shown outside a menu (forced ATEs), every write of
`EVT_GB_ATE_BITS` with the if-conditions around it, the field-local masks some towns compute instead, and each field's
return code. The table below is curated from its output.

## Counts

84 ATEs (81 story ATEs plus 3 "Teach me, Mogster!" tutorials, which use the same title card):

| story disc | ATEs | required (script starts it) | optional (Select menu only) | other |
|---|---|---|---|---|
| 1 | 20 | 4 (Do As I Say, Queen Brahne's Steiner, Vivi's Shopping, What can I do?) | 12 | Forest Keeper (window opened by the script, Cancel allowed), 3 tutorials |
| 2 | 33 | 7 (Unexpected Visitor, Brahne's Fleet Arrives, Gwok-gwok, Friendship, Different Language, Nuptial Joy, Eiko's Feelings) | 26 | |
| 3 | 31 | 7 (Clowns on a Dark Night, Self-reproach, Deep Anger, The Rally, Team, Recovery, The Truth) | 24 | Give Me My Money! (counted opt) also starts from a talk choice in 2173 |
| 4 | 0 | | | |

"Story disc" is the disc whose story range the ATE falls in (`STORY_DISC2` 3900, `STORY_DISC3` 6990,
`STORY_DISC4` 10995). Discs 2-4 carry copies of earlier towns' fields (Dali 350-358/450 on 1-4, Lindblum
2105-2169 and Treno 1901-1915 on 3-4, Black Mage Village / Conde Petie / Madain Sari on 2-3), menus included; those
copies are listed once. Disc 4 has no ATE of its own. Several required ATEs also appear in their town's Select menu
(the forced path and the menu path use different arrival codes; the forced one is listed as REQ).

## How ATEs work in the scripts

The engine knows nothing about ATEs (R32 section 3); everything is script convention:

- **Prompt**: op `0xD7 ATE_PROMPT mode` (`msg_setAtePrompt`): 0 off, 1 "ACTIVE TIME EVENT / PRESS [SELECT] TO SEE
  ATE", 5 the same while the player has no control, 6 the grey blinking "ACTIVE TIME EVENT" shown while a forced ATE
  starts.
- **Availability** (field init, entry 0 tag 0, in every town field): `if (mask != 0) { D7 1; Gs8[0xEE] = 1;
  Gs16[0xF1] = mask; if (Gu16[0xFB] != (Gu16[0xFB] | mask)) Fbit[0x98] = 1 /* jingle */; Gu16[0xFB] |= mask }` else
  `D7 0; Gs8[0xEE] = 0; Gs16[0xF1] = 0`. The block is skipped (prompt off) when `Gs16[0x2]` is the code the town
  uses for "back from an ATE" (1, 2, 16, 19, ...) and, in Treno / Black Mage Village / Conde Petie / Bran Bal, when the
  field's bit is set in `Gu16[0xEA]`.
  - `mask` is `Gu16[0xEC]` (`EVT_GB_ATE_BITS`) in Prima Vista/Evil Forest, Lindblum (discs 1, 2), Treno (discs 2,
    3), Black Mage Village, Conde Petie, Bran Bal. Some fields mask it (`Gu16[0xEC] & ~16` in 200-208: Orchestra in
    the Forest is offered only in the forest fields).
  - Dali, Cleyra, Madain Sari, Alexandria (disc 3), Lindblum (disc 3) build a **field-local** mask (`Fu8[0x18]`,
    `Fu16[0x1E]`, ...) in the init from global flags, so poking `event.236` does nothing there. Formulas per town
    below.
- **Select handler** (a code object the init starts with `NEW_CODE`): `if (controlEnabled == 1 && Gs8[0xEE] == 1 &&
  padPressed(SELECT))` → `7C MSG_CHOICE_MASK (Gs16[0xF1] | 0x8000)`, `1F MSG_WAIT win, 0x40, LIST` (window flag 0x40
  = the "ACTIVE TIME EVENT" title bar; LIST = "Select event / title 0 / title 1 / ... / ATE-NN placeholders /
  Cancel"), switch on `sys.msgChoice`. **Choice row k = mask bit k.** Each case: preload the target field, fade, show
  the **title card** (`20 MSG win, 0x40, CARD`, the title alone in the ATE window), save the player position
  (`Gs16[0xF3]`, `[0xF5]`, `[0xF7]`, `Gu8[0xF9]` = x, y, z, dir), set the **arrival code** `Gs16[0x2]` and
  `GOTO_FIELD target`.
- **Forced ATEs** run the same tail (D7 6, card, arrival code, GOTO_FIELD) from a cutscene, a region or a talk
  handler instead of the menu.
- **The ATE field** recognises the arrival code in its init (actor creation and the scene's state machine switch on
  `Gs16[0x2]`), plays the scene, clears its own bit (`Gu16[0xEC] &= ~bit` or the town's flag), sets the "back from an
  ATE" code and goes to the field in `Gs16[0xEF]` (`EVT_GB_ATE_RETURN_FIELD`): a field id in most towns, a slot number
  (switch in the ATE field) in Dali, Treno, Black Mage Village, Conde Petie, Madain Sari, Bran Bal.

Variables a fixture should know (byte offsets = `event.K`):

| bytes | var | meaning |
|---|---|---|
| 0-1 | `Gu16[0x0]` | story counter (`story=`) |
| 2-3 | `Gs16[0x2]` | arrival code (`EVT_GB_ARRIVAL_POINT`): which entrance / which ATE; leave 0 for a normal start |
| 0xEA-0xEB (234-235) | `Gu16[0xEA]` | per-town "ATE already played from this field" bits (Treno, Black Mage Village, Conde Petie: bit = return slot - 3 or - 5); a set bit hides the prompt in that field. Leave 0 |
| 0xEC-0xED (236-237) | `Gu16[0xEC]` | available-ATE bits |
| 0xEE (238) | `Gs8[0xEE]` | prompt shown / Select handler armed (set by the init). It is a global and outlives the field: a field whose init doesn't touch it (611) keeps the previous field's 1, so "menu available here" is g_atePromptMode (reset by the event init, set by D7 1), not this byte |
| 0xEF-0xF0 (239-240) | `Gs16[0xEF]` | return field or slot after the ATE |
| 0xF1-0xF2 (241-242) | `Gs16[0xF1]` | menu row mask (set by the init) |
| 0xF3-0xF9 | | saved player x, y, z, dir for the return |
| 0xFB-0xFC (251-252) | `Gu16[0xFB]` | ATEs announced so far (new bits play the jingle C8 0xD000 0x546) |
| 0x129-0x12A (297-298) | `Gu16[0x129]` | chapter bits several towns test: 1 Dali, 2 South Gate, 8 Madain Sari, 64 Mountain/observatory, 2048 Berkmea |
| `Gbit[n]` | byte n >> 3, bit n & 7 | e.g. Gbit[0x807] = event.256 bit 7 (0x80) |

Two ways to start an ATE in a test:
1. **At the menu**: the from-field with the mask (or the flags that build the local mask) set before the init runs,
   `event.2 = 0`. Prompt appears; Select, pick the row, Cross twice. Exercises the prompt, the window and the card.
2. **In the ATE field**: the target field with its arrival code in `event.2/3` and the return field/slot in
   `event.239/240`. This is what the game does after the card; it skips the prompt and the window. Works the same
   for forced ATEs.

## Dali (disc 1): "Queen Brahne's Steiner", field 355 (required)

**Verified (2026-11-04, 3DS port in Azahar):** the direct line starts 355 with Steiner as the player.

**What it is.** The Dali ATE with Steiner in the pub (field 355 "Dali/Pub"). It is forced: nobody presses Select.

**How the game starts it (field 352 "Dali/Inn", entry 17 = Dagger's state machine, entry 19 tag 3 = talking to
Dagger):**
- 352's init creates Dagger (entry 19, model 185) only while `2610 <= story <= 2640` (`NEW_ACTOR 0x13`).
- Talking to her while `story < 2650`: if `Gbit[0x807] == 0` she has one line (state 37, nothing more); if
  `Gbit[0x807] == 1` the inn scene starts (state 41: clears `Gs16[0xEF]`, `Gu16[0xFB]` and the saved position, then
  states 42 → 49 alternate between Dagger's entry 17 and entry 19 dialogue). `Gbit[0x807]` is set in Dali by field
  350 (entry 34's talk tags) and 358 (windmill, entries 4 and 12).
- State 49 (352 e17t1 +0x139F..+0x1578): `D7 6` (grey blinking "ACTIVE TIME EVENT"), preload field 355, fade, `D7 0`,
  `Gs8[0xEE] = 0`, `Gs16[0xF1] = 0`, **title card msg 0x63 "Queen Brahne's [Steiner]"** (`20 00 00 40 63 00`, the
  name is `TEXT_CHAR_NAME` 3, so it shows the player's name for Steiner), saves the position, `Gs16[0x2] = 18`,
  `GOTO_FIELD 355`.
- 352's init stores its own return slot first: `if (Gs16[0x2] != 16 && != 18) Gs16[0xEF] = 6`.

**What 355 does with arrival 18:**
- init (e0t0): `Gs16[0xEF] = 10` is skipped for 16/18 (so the return stays 6); no Zidane (`NEW_ACTOR 0x12/0x14` only
  when `Gs16[0x2] != 18`); `story < 2990` → actor 5, `!Gbit[0x807] && story < 2840` → actor 6, **`story < 2650` →
  actor 21 = Steiner (model 5489)**; then the ATE music (`SOUND 0x0, 0x3E` for code 18 instead of the town theme).
- Steiner's init (entry 21 tag 0) switches on `Gs16[0x2]`: **18 → `SET_PLAYER`, `Fs16[0x1D] = 12`** (the scene's
  first state) at (-320, -60, 733), dir 50. Steiner is the controllable character for the ATE.
- End of the scene (e21t1 +0x29A..): `story = 2640` (the debug branch on START sets the same value), `Gs16[0x2] =
  19`, prompt off, `Gbit[0x837] = 1`, then `switch (Gs16[0xEF])`: 6 → `GOTO_FIELD 352` (the other slots: 1 → 350,
  2 → 351, 7 → 354, 9 → 353, 10 → 355, 11 → 356, 12 → 357, 13 → 358, 25 → 450).
- Back in 352 with code 19, Dagger's init sets state 50 (the continuation). `Gbit[0x837]` suppresses the prompt in 355
  until the player leaves through 355's region (entry 17 clears it).

**Fixtures:**
```
# natural trigger: talk to Dagger in the inn
kind=field, disc=1, field=352, story=2610, event.2=0, event.3=0, event.256=128
# direct: the ATE itself (Steiner as player in the pub), returns to 352
kind=field, disc=1, field=355, story=2610, event.2=18, event.3=0, event.239=6, event.240=0, event.297=1
```
`event.256=128` is `Gbit[0x807]`. `event.297=1` (`Gu16[0x129]` bit 0, "in Dali") is what the real game has at this
point; the forced path doesn't test it, the menu does. Any story 2610..2640 works for the direct start (Steiner is
created below 2650); 2610 is the value the weapon-shop scene (354) leaves before the inn. Confidence: high for the
script path, the scene content itself not runtime-checked.

**The same title in the Dali menu.** Row 0 of the Dali list (msg 0x62 in 350-358, 0x50 in 450) is also "Queen
Brahne's Steiner", offered while `Gu16[0x129] & 1` and `Gbit[0x818] ^ Gbit[0x81D]` (0x818 is set in 450 by the farmer,
entry 3 tag 3; 0x81D in 455/457 on the mountain). That row sends arrival **16**, and 355 creates Steiner only below
story 2650 with no case for 16 in his init, so what the menu row plays in 355 is unclear. Runtime check.

**Dali menu mechanics** (all of 350-358 and 450; the mask is local, so `event.236` is ignored): row k is offered when
`Gu16[0x129] & 1` (Dali chapter: field 359 sets `Gu16[0x129] = 1`, the mountain summit 456 sets 64) **and** exactly
one flag of a pair is set: row 0 0x818/0x81D, row 1 0x819/0x81E, row 2 0x827/0x826, row 3 0x81B/0x810, row 4
0x81C/0x811, row 5 0x81A/0x81F. Setting the first flag of the pair (bytes 0x103/0x104, table below) offers the row.
Also `Gbit[0x837]` (event.262 bit 7) must be 0 in 355, and 450 offers the menu only while `Gbit[0x826] == 0` or
`Gbit[0x827] == 1`.

## Dali field 450 ("Dali/Field", the farm): scene by story value

**Verified (2026-11-04, 3DS port in Azahar):** story 2840 shows the barrels and the parked cargo ship, so layer 0x15 = the ship and model 240 = the barrels.

450's init (e0t0 +0x149) picks the actor set and one background layer from the story counter:

| story | actors created (entry: model) | BG layer 0x15 | what it is |
|---|---|---|---|
| < 11090, not 2810/2840 | 21 Zidane (98), or 23 Garnet (185) when `Gs16[0x2] == 16` (the "Dagger Tries" ATE) or Zidane isn't in the party; 3 (12); 5 (114) | hidden (`BG_LAYER_SHOW 0x15, 0`) | the normal farm, Dali ATE prompt possible |
| == 2810 | 8 (259), 9 (262), 10 (240), 11 (240), 7 (50) twice (instances 0x80, 0x81) | shown | cargo-ship arrival cutscene: e0t1 plays a BG animation (`BG_ANIM_RANGE 0, 7, 7`) and camera scrolls; entry 10 follows entry 8 (`Fu8[0x28] = 1`); exits to 451 (`Gs16[0x2] = 25`, e0t12) or 453 (e0t13) |
| == 2840 | 10 (240) and 11 (240) unless `Gs16[0x2] == 1`, 21 Zidane, 3 (12) | **shown** | **after the Black Waltz No. 2 battle**: free roaming, world-map exit blocked (entry 18 tag 2 has a 2840 branch) |
| >= 11090 | layer hidden; 21, 3, 15, 9 (262), 12, 13, 14, 16 | hidden | disc 4 ending |

Entry 20 (model 75) is created in every case.

Story sequence around it (all disc 1): 457 Mountain/Shack sets 2810 → 452 (sets 2810, `Gs16[0x2] = 27`) → **450 at
2810** (ship lands) → 451 (`Gs16[0x2] = 25`): its scene sets **story 2840** and runs `BATTLE 0x126` (encounter 294,
the only battle in these fields, so the Black Waltz No. 2 fight) → 453 (`Gs16[0x2] = 26`) → **450 at 2840 with
`Gs16[0x2] = 28`** (453 e5t1) or on to 454 (`Gs16[0x2] = 28`; 454 sets 2900, plays a movie, goes to 505 Cargo
Ship/Deck).

So the post-BW2 farm is exactly **story 2840**: the background layer 0x15 that every other story value hides stays
visible, and actors 10/11 (model 240) are placed. The script and the port's report agree that the cargo ship and the
barrels show up after the fight. **Which drawn element is layer 0x15 and which is model 240 is a guess**: model 240 is a
small prop (2 bones, 86 vertices, 96 polygons, scaled down by `MODEL_SCALE 0x36`), so the barrels are likely; the parked
cargo airship is then likely layer 0x15 (the flying ship at 2810 would be entry 9, model 262, 4 bones, 411 vertices).
Runtime check (screenshot at 2840 with the layer hidden). `Gu16[0x129]` is 64 by then (set on the summit), so the
Dali ATE prompt is off; entry 11's main loop moves its prop to (1652, 1712, 8626) and plays anim 0x20A4 when
`Gu16[0x129] & 64 && Gbit[0x807]` (451 sets 0x807 in one dialogue branch).

**Fixtures:**
```
# post-BW2 farm (barrels + ship), arriving from 453 as the game does
kind=field, disc=1, field=450, story=2840, event.2=28, event.3=0, event.297=64
# same with the second prop pose (451's dialogue branch)
kind=field, disc=1, field=450, story=2840, event.2=28, event.3=0, event.297=64, event.256=128
# cargo ship landing cutscene (plays by itself, then leaves for 451)
kind=field, disc=1, field=450, story=2810, event.2=27, event.3=0, event.297=64
# normal farm for comparison
kind=field, disc=1, field=450, story=2640, event.297=1
```

## Table

Columns: **kind** REQ = the script starts it (cutscene, region or talk), opt = Select menu only. **menu fields (row)**
= fields whose Select menu lists it, with its row = mask bit. **story** = the value the unlocking script uses or the
scene's window (see notes); **conf** = confidence in that story value and the trigger. **start in the ATE field** =
direct start (arrival code + return field/slot); **start at the menu** = prompt fixture (mask in `event.236/237` for
`Gu16[0xEC]` towns, the flags behind the local mask otherwise; `-` = no menu line, use the direct start).

| disc | title | kind | menu fields (row) | ATE field | story | conf | start in the ATE field | start at the menu | notes |
|---|---|---|---|---|---|---|---|---|---|
| 1 | Time to Escape | opt | 200-208, 250-255 (0) | 208 Prima Vista/Storage | 2055 | med | `kind=field, disc=1, field=208, story=2055, event.2=1, event.3=0, event.239=201, event.240=0` | `kind=field, disc=1, field=201, story=2055, event.236=1, event.237=0` | bits 0+1 set by 201 (=3) at the end of the 2055 scene |
| 1 | Girl Who Was Left Behind | opt | 200-208, 250-255 (1) | 112 Alexandria/Pub | 2055 | med | `kind=field, disc=1, field=112, story=2055, event.2=1, event.3=0, event.239=201, event.240=0` | `kind=field, disc=1, field=201, story=2055, event.236=2, event.237=0` | same writer as Time to Escape |
| 1 | My Little Baby | opt | 200-208, 250-255 (2) | 206 Prima Vista/Crash Site | 2055 | low | `kind=field, disc=1, field=206, story=2055, event.2=1, event.3=0, event.239=208, event.240=0` | `kind=field, disc=1, field=201, story=2055, event.236=4, event.237=0` | bit 2 set by 208 (\|= 4) after Time to Escape |
| 1 | Do As I Say, Not As I Do | REQ | 200-208, 250-255 (3) | 204 Prima Vista/Hallway | 2300 | low | `kind=field, disc=1, field=204, story=2300, event.2=1, event.3=0, event.239=253, event.240=0` | `kind=field, disc=1, field=201, story=2300, event.236=8, event.237=0` | forced: 253 Evil Forest/Spring scene sets mask 8, D7 6, card, GOTO 204; story between 2260 and 2350 |
| 1 | Orchestra in the Forest | opt | 200-208, 250-255 (4) | 202 Prima Vista/Cargo Room | 2260 | high | `kind=field, disc=1, field=202, story=2260, event.2=1, event.3=0, event.239=250, event.240=0` | `kind=field, disc=1, field=250, story=2260, event.236=16, event.237=0` | bit 4 set by 250 when story == 2260 && !Gbit[0x98A]; only the forest fields offer it |
| 1 | The Forest Keeper | forced window | 206 only (-) | 251 Evil Forest/Trail | 2005 | med | `kind=field, disc=1, field=251, story=2005, event.2=0, event.3=0, event.239=206, event.240=0` | - | 206 Crash Site opens a one-row "Select event" window itself (D7 5) at story 2005; Cancel allowed |
| 1 | Queen Brahne's Steiner | REQ | 350-358, 450 (0) | 355 Dali/Pub | 2610 | high | `kind=field, disc=1, field=355, story=2610, event.2=18, event.3=0, event.239=6, event.240=0, event.259=1, event.297=1` | `kind=field, disc=1, field=350, story=2610, event.259=1, event.297=1` | forced from 352 (talk to Dagger, Gbit[0x807] = 1, story 2610..2640); the menu row sends code 16 instead (Dali section) |
| 1 | Vivi, Confused | opt | 350-358, 450 (1) | 350 Dali/Village Road | 2640 | low | `kind=field, disc=1, field=350, story=2640, event.2=16, event.3=0, event.239=1, event.240=0, event.259=2, event.297=1` | `kind=field, disc=1, field=350, story=2640, event.259=2, event.297=1` | mask bit = Gbit[0x819] ^ Gbit[0x81E] |
| 1 | Dagger Tries | opt | 350-358, 450 (2) | 450 Dali/Field | 2640 | low | `kind=field, disc=1, field=450, story=2640, event.2=16, event.3=0, event.239=1, event.240=0, event.260=128, event.297=1` | `kind=field, disc=1, field=350, story=2640, event.260=128, event.297=1` | mask bit = Gbit[0x827] ^ Gbit[0x826]; 450 creates Garnet as the player for code 16 |
| 1 | Cat's Eye | opt | 350-358, 450 (3) | 351 Dali/Inn | 2640 | low | `kind=field, disc=1, field=351, story=2640, event.2=16, event.3=0, event.239=1, event.240=0, event.259=8, event.297=1` | `kind=field, disc=1, field=350, story=2640, event.259=8, event.297=1` | mask bit = Gbit[0x81B] ^ Gbit[0x810] |
| 1 | Cat's Eye 2 | opt | 350-358, 450 (4) | 351 Dali/Inn | 2640 | low | `kind=field, disc=1, field=351, story=2640, event.2=16, event.3=0, event.239=1, event.240=0, event.259=16, event.297=1` | `kind=field, disc=1, field=350, story=2640, event.259=16, event.297=1` | mask bit = Gbit[0x81C] ^ Gbit[0x811] |
| 1 | Dagger Tries Harder | opt | 350-358, 450 (5) | 354 Dali/Wpn. Shop | 2640 | low | `kind=field, disc=1, field=354, story=2640, event.2=16, event.3=0, event.239=1, event.240=0, event.259=4, event.297=1` | `kind=field, disc=1, field=350, story=2640, event.259=4, event.297=1` | mask bit = Gbit[0x81A] ^ Gbit[0x81F] |
| 1 | Small-Town Knight in a Big City | opt | 550-573, 609, 850 (0) | 555 Lindblum/Shopping Area | 3110 | high | `kind=field, disc=1, field=555, story=3110, event.2=1, event.3=0, event.239=40, event.240=2` | `kind=field, disc=1, field=552, story=3110, event.236=1, event.237=0` | set by 553 at story == 3110 && !Gbit[0xA57]; the menu line was verified (R36) |
| 1 | Vivi's Shopping | REQ | 550-573, 609, 850 (1) | 561 Lindblum/Item Shop | 3110 | high | `kind=field, disc=1, field=561, story=3110, event.2=1, event.3=0, event.239=40, event.240=2` | `kind=field, disc=1, field=552, story=3110, event.236=2, event.237=0` | forced from 552 (region, entry 22) while story < 3115 && !Gbit[0xA56] |
| 1 | Steam Engine | opt | 550-573, 609, 850 (2) | 609 L. Castle/Castle Bridge | 3110 | med | `kind=field, disc=1, field=609, story=3110, event.2=1, event.3=0, event.239=54, event.240=2` | `kind=field, disc=1, field=552, story=3110, event.236=4, event.237=0` | set by 566 if story < 3115, Gbit[0xA57] && !Gbit[0xA55] and bit 0 already gone |
| 1 | Baku and His Crew | opt | 550-573, 609, 850 (3) | 850 S. Gate/Bohden Arch | 3118 | med | `kind=field, disc=1, field=850, story=3118, event.2=1, event.3=0, event.239=53, event.240=2` | `kind=field, disc=1, field=552, story=3118, event.236=8, event.237=0` | set by 565 Hideout (\|= 8) after the 3118 scene |
| 1 | What can I do? | REQ | - (-) | 611 L. Castle/Guest Room | 3118 | med | `kind=field, disc=1, field=611, story=3118, event.2=22, event.3=0, event.239=53, event.240=2` | - | forced from 565 Hideout init (story 3115 -> 3118), arrival 22 |
| 1 | "Teach me, Mogster!" Lesson 1 | tutorial | - (-) | 652 Marsh/Shore | 2400 | med | `kind=field, disc=1, field=652, story=2400, event.2=0, event.3=208, event.239=6, event.240=1` | - | forced from 262 Evil Forest/Exit (story 2350 -> 2400) |
| 1 | "Teach me, Mogster!" Lesson 2 | tutorial | - (-) | 652 Marsh/Shore | 2500 | low | `kind=field, disc=1, field=652, story=2500, event.2=0, event.3=224, event.239=50, event.240=1` | - | forced from 306 Ice Cavern/Cave (sets Gbit[0xD82]) |
| 1 | Teach me, Mogster! (My first synthesis lesson) | tutorial | - (-) | 652 Marsh/Shore | 3110 | med | `kind=field, disc=1, field=652, story=3110, event.2=0, event.3=240, event.239=42, event.240=2` | - | forced from 554 Lindblum/Inn (story 3110, !Gbit[0xA5B]) |
| 2 | Treno Tradition | opt | 900-915 (0) | 905 Treno/Walkway | 4400 | med | `kind=field, disc=2, field=905, story=4400, event.2=1, event.3=0, event.239=11, event.240=0` | `kind=field, disc=2, field=903, story=4400, event.236=1, event.237=0` | 908 Gate sets mask 33 (bits 0, 5) on arrival from the world map while story < 4400, then story 4400 |
| 2 | Pursuit | opt | 900-915 (1) | 915 Treno/Bishop's House | 4400 | med | `kind=field, disc=2, field=915, story=4400, event.2=1, event.3=0, event.239=8, event.240=0` | `kind=field, disc=2, field=903, story=4400, event.236=2, event.237=0` | 905 sets bit 1 on return from Treno Tradition |
| 2 | Confusion | opt | 900-915 (2) | 904 Treno/Knight's House | 4400 | med | `kind=field, disc=2, field=904, story=4400, event.2=1, event.3=0, event.239=6, event.240=0` | `kind=field, disc=2, field=903, story=4400, event.236=4, event.237=0` | 903/912 set bit 2 while story < 4420; 915 after Pursuit |
| 2 | Meeting Place | opt | 900-915 (3) | 900 Treno/Pub | 4400 | med | `kind=field, disc=2, field=900, story=4400, event.2=1, event.3=0, event.239=15, event.240=0` | `kind=field, disc=2, field=903, story=4400, event.236=8, event.237=0` | 912 Slums: bit 5 -> bit 3 after Ambition |
| 2 | Unexpected Visitor | REQ | 900-915 (4) | 909 Treno/Auction Site | 4420 | med | `kind=field, disc=2, field=909, story=4420, event.2=253, event.3=255, event.239=7, event.240=0` | `kind=field, disc=2, field=903, story=4420, event.236=16, event.237=0` | forced from 904 (region, entry 10) while story < 4430 (arrival -3); menu row 4 (code 1) after 904 sets bit 4 (mask & 7) |
| 2 | Ambition | opt | 900-915 (5) | 912 Treno/Slums | 4400 | med | `kind=field, disc=2, field=912, story=4400, event.2=1, event.3=0, event.239=11, event.240=0` | `kind=field, disc=2, field=903, story=4400, event.236=32, event.237=0` | set with Treno Tradition (=33) |
| 2 | Crime and Punishment | opt | 900-915 (6) | 809 S. Gate/Summit Sta. | 4460 | med | `kind=field, disc=2, field=809, story=4460, event.2=0, event.3=0` | `kind=field, disc=2, field=903, story=4460, event.236=64, event.237=0` | 914 Tot Residence sets mask = 64 (story 4460); the row clears bit 6, sets code 0 and leaves Treno for 809 S. Gate/Summit Sta. (no return) |
| 2 | Don't Hate Me | opt | 1051-1060 (0) | 1055 Cleyra/Town Area | 4720 | low | `kind=field, disc=2, field=1055, story=4720, event.2=255, event.3=255, event.239=27, event.240=4, event.482=2` | - | bit 0 offered where Gu8[0x1E3] == 0 (1053) |
| 2 | No Yummy-Yummies! | opt | 1051-1060 (1) | 1057 Cleyra/Observation Point | 4720 | low | `kind=field, disc=2, field=1057, story=4720, event.2=254, event.3=255, event.239=27, event.240=4, event.482=2` | - | bit 1 where Gu8[0x1E4] == 0 (1051, 1056); mask cleared unless Gbit[0xF11] |
| 2 | I-I Haven't Hurt Anyone | opt | 1051-1060 (2) | 1056 Cleyra/Inn | 4720 | low | `kind=field, disc=2, field=1056, story=4720, event.2=252, event.3=255, event.239=27, event.240=4, event.482=2, event.483=2` | - | bit 2 where Gu8[0x1E3] == 2 |
| 2 | There a Mushroom! | opt | 1051-1060 (3) | 1053 Cleyra/Water Mill Area | 4720 | low | `kind=field, disc=2, field=1053, story=4720, event.2=248, event.3=255, event.239=27, event.240=4, event.482=2, event.484=2` | - | bit 3 where Gu8[0x1E4] == 2 |
| 2 | The Treetop Town of Cleyra | opt | 1051-1060 (4) | 1060 Cleyra/Cathedral | 4725 | low | `kind=field, disc=2, field=1060, story=4725, event.2=240, event.3=255, event.239=27, event.240=4, event.482=2` | - | bit 4: not set by any entry-0 init (other entries; gap) |
| 2 | The Third Jewel | opt | 1300-1314, 1355-1370, 1660 (0) | 1369 L. Castle/Hall | 5660 | low | `kind=field, disc=2, field=1369, story=5660, event.2=1, event.3=0, event.239=25, event.240=5` | `kind=field, disc=2, field=1302, story=5660, event.236=1, event.237=0` | 1305 Shopping Area sets bit 0 if !Gbit[0xA53]; 1370 Lift sets mask 4 (unused row 2) |
| 2 | Brahne's Fleet Arrives | REQ | 1300-1314, 1355-1370, 1660 (1) | 1660 Brahne's Fleet/Event | 5680 | med | `kind=field, disc=2, field=1660, story=5680, event.2=1, event.3=0, event.239=27, event.240=5` | `kind=field, disc=2, field=1302, story=5680, event.236=2, event.237=0` | forced from 1307 Square (talk, entry 4) at the 5680 scene, !Gbit[0xA52] |
| 2 | <Gwok-gwok!>  How infuriating! | REQ | - (-) | 1355 L. Castle/Serpent's Gate | 5690 | low | `kind=field, disc=2, field=1355, story=5690, event.2=1, event.3=0, event.239=73, event.240=5` | - | forced from 1353 L. Castle/Base Level init after Quina joins (story >= 5690, 1352 sets 5690) |
| 2 | Friendship | REQ | - (-) | 152 Evil Forest/Memory | 5090 | med | `kind=field, disc=2, field=152, story=5090, event.2=0, event.3=0, event.239=181, event.240=4` | - | forced from 1205 A. Castle/Chapel (entry 20) at the 5080 -> 5090 scene; code 0 |
| 2 | Life On The Run | opt | 1450-1460 (0) | 1451 Mage Village/Pond | 6207 | med | `kind=field, disc=2, field=1451, story=6207, event.2=1, event.3=0, event.239=5, event.240=0` | `kind=field, disc=2, field=1455, story=6207, event.236=1, event.237=0` | 1450 Entrance sets mask 25 (bits 0, 3, 4) on arrival code 4 |
| 2 | Different Language | REQ | 1450-1460 (1) | 1452 Mage Village/Cemetery | 6208 | med | `kind=field, disc=2, field=1452, story=6208, event.2=1, event.3=0, event.239=6, event.240=0` | `kind=field, disc=2, field=1455, story=6208, event.236=2, event.237=0` | forced from 1451 Pond (regions, entries 10/11; 6207 -> 6208); 1451 init turns bit 0 into bits 1+2 |
| 2 | Visitor, Not Invader | opt | 1450-1460 (2) | 1455 Mage Village/Synthesist | 6208 | low | `kind=field, disc=2, field=1455, story=6208, event.2=1, event.3=0, event.239=5, event.240=0` | `kind=field, disc=2, field=1455, story=6208, event.236=4, event.237=0` | bit 2 from 1451 (after Life On The Run) |
| 2 | Gourmand's Nose | opt | 1450-1460 (3) | 1458 Mage Village/Water Mill | 6207 | med | `kind=field, disc=2, field=1458, story=6207, event.2=1, event.3=0, event.239=5, event.240=0` | `kind=field, disc=2, field=1455, story=6207, event.236=8, event.237=0` | set with Life On The Run (=25) |
| 2 | Everyday Life | opt | 1450-1460 (4) | 1460 Mage Village/Item Shop | 6207 | med | `kind=field, disc=2, field=1460, story=6207, event.2=1, event.3=0, event.239=5, event.240=0` | `kind=field, disc=2, field=1455, story=6207, event.236=16, event.237=0` | set with Life On The Run (=25) |
| 2 | Life Cycle | opt | 1450-1460 (5) | 1459 Mage Village/Water Mill | 6207 | low | `kind=field, disc=2, field=1459, story=6207, event.2=1, event.3=0, event.239=7, event.240=0` | `kind=field, disc=2, field=1455, story=6207, event.236=32, event.237=0` | 1458 Water Mill: bit 3 -> bit 5 after Gourmand's Nose |
| 2 | Vivi and the Couple | opt | 1501-1509 (0) | 1507 Conde Petie/Pathway | 6100 | med | `kind=field, disc=2, field=1507, story=6100, event.2=1, event.3=0, event.239=4, event.240=0` | `kind=field, disc=2, field=1505, story=6100, event.236=1, event.237=0` | 1501 Corridor sets mask 11 (bits 0, 1, 3) on arrival 3 while story < 6100 |
| 2 | Quina Accused | opt | 1501-1509 (1) | 1509 Conde Petie/Item Shop | 6100 | med | `kind=field, disc=2, field=1509, story=6100, event.2=1, event.3=0, event.239=4, event.240=0` | `kind=field, disc=2, field=1505, story=6100, event.236=2, event.237=0` | set with Vivi and the Couple |
| 2 | Quina Can't Communicate | opt | 1501-1509 (2) | 1503 Conde Petie/Wpn. Stall | 6110 | low | `kind=field, disc=2, field=1503, story=6110, event.2=1, event.3=0, event.239=10, event.240=0` | `kind=field, disc=2, field=1505, story=6110, event.236=4, event.237=0` | 1505/1509: bit 1 -> bit 2 after Quina Accused |
| 2 | Dagger and William | opt | 1501-1509 (3) | 1508 Conde Petie/Inn | 6100 | med | `kind=field, disc=2, field=1508, story=6100, event.2=1, event.3=0, event.239=4, event.240=0` | `kind=field, disc=2, field=1505, story=6100, event.236=8, event.237=0` | set with Vivi and the Couple |
| 2 | Hungry Bryan | opt | 1501-1509 (4) | 1502 Conde Petie/Corridor | 6110 | low | `kind=field, disc=2, field=1502, story=6110, event.2=1, event.3=0, event.239=10, event.240=0` | `kind=field, disc=2, field=1505, story=6110, event.236=16, event.237=0` | 1505 Shrine entry 15 (\|= 16) at 6110 |
| 2 | Nuptial Joy | REQ | - (-) | 1506 Conde Petie/Event | 6110 | low | `kind=field, disc=2, field=1506, story=6110, event.2=255, event.3=255, event.239=10, event.240=0` | - | forced from 1505 Shrine init on arrival -1 after the wedding choice |
| 2 | Vivi's Feelings | opt | 1600-1603, 1607 (0) | 1610 Mdn. Sari/Cove | 6620 | low | `kind=field, disc=2, field=1610, story=6620, event.2=1, event.3=0, event.239=6, event.240=0, event.256=128, event.297=8` | `kind=field, disc=2, field=1600, story=6620, event.256=128, event.297=8` | bit = Gbit[0x807] ^ Gbit[0x804] |
| 2 | Dagger's Feelings | opt | 1600-1603, 1607 (1) | 1600 Mdn. Sari/Entrance | 6620 | low | `kind=field, disc=2, field=1600, story=6620, event.2=1, event.3=0, event.239=6, event.240=0, event.256=64, event.297=8` | `kind=field, disc=2, field=1600, story=6620, event.256=64, event.297=8` | bit = Gbit[0x806] ^ Gbit[0x803] |
| 2 | Eiko's Feelings | REQ | 1600-1603, 1607 (2) | 1607 Mdn. Sari/Kitchen | 6610 | med | `kind=field, disc=2, field=1607, story=6610, event.2=3, event.3=0, event.239=7, event.240=0, event.256=32, event.297=8` | `kind=field, disc=2, field=1600, story=6610, event.256=32, event.297=8` | forced from 1601 (story 6610, sets Gbit[0x804]) or 1602 (Gbit[0x80C]) with code 3; menu bit = Gbit[0x805] ^ Gbit[0x802] |
| 2 | Eiko's Kitchen | opt | 1600-1603, 1607 (3) | 1607 Mdn. Sari/Kitchen | 6620 | low | `kind=field, disc=2, field=1607, story=6620, event.2=1, event.3=0, event.239=6, event.240=0, event.257=4, event.297=8` | `kind=field, disc=2, field=1600, story=6620, event.257=4, event.297=8` | bit = Gbit[0x80A] ^ Gbit[0x809] |
| 2 | Eiko's Kitchen 2 | opt | 1600-1603, 1607 (4) | 1607 Mdn. Sari/Kitchen | 6620 | low | `kind=field, disc=2, field=1607, story=6620, event.2=1, event.3=0, event.239=6, event.240=0, event.258=4, event.297=8` | `kind=field, disc=2, field=1600, story=6620, event.258=4, event.297=8` | bit = Gbit[0x812] ^ Gbit[0x811] |
| 3 | I Forgot! | opt | 1850-1866 (0) | 1864 Alexandria/Mini-Theater | 7030 | low | `kind=field, disc=3, field=1864, story=7030, event.2=255, event.3=255, event.239=58, event.240=7, event.489=1` | `kind=field, disc=3, field=1850, story=7030, event.489=1` | bit 0 if Gu8[0x1E9] == 1 (and the field's own Fu8 phase == 0) |
| 3 | Long Time No See! | opt | 1850-1866 (1) | 1858 Alexandria/Wpn. Shop | 7100 | low | `kind=field, disc=3, field=1858, story=7100, event.2=254, event.3=255, event.239=58, event.240=7, event.490=1` | `kind=field, disc=3, field=1850, story=7100, event.490=1` | bit 1 if Gu8[0x1EA] == 1 (phase 1) |
| 3 | To Fly High | opt | 1850-1866 (2) | 1854 Alexandria/Alley | 7100 | low | `kind=field, disc=3, field=1854, story=7100, event.2=252, event.3=255, event.239=58, event.240=7, event.490=3` | `kind=field, disc=3, field=1850, story=7100, event.490=3` | bit 2 if Gu8[0x1EA] == 3 |
| 3 | It's So Big! | opt | 1850-1866 (3) | 1811 A. Castle/Courtyard | 7200 | low | `kind=field, disc=3, field=1811, story=7200, event.2=248, event.3=255, event.239=58, event.240=7, event.488=2` | `kind=field, disc=3, field=1850, story=7200, event.488=2` | bit 3 if Gbit[0xF41] && !Gbit[0xF40] |
| 3 | Artemicion | opt | 1850-1866 (4) | 1865 Alexandria/Steeple | 7100 | low | `kind=field, disc=3, field=1865, story=7100, event.2=240, event.3=255, event.239=61, event.240=7, event.508=1` | `kind=field, disc=3, field=1853, story=7100, event.508=1` | bit 4 if Gu8[0x1FC] == 1 (1853 Square) |
| 3 | Clowns on a Dark Night | REQ | - (-) | 2054 Alexandria/Main Street | 6990 | med | `kind=field, disc=3, field=2054, story=6990, event.2=0, event.3=0, event.239=188, event.240=3` | - | forced from 956 Gargan Roo/Last Stop: init sets code -3 when story >= 6990, then the card |
| 3 | Hippo's Prize | opt | 1901-1915 (0) | 1912 Treno/Treno Slums | 7650 | med | `kind=field, disc=3, field=1912, story=7650, event.2=1, event.3=0, event.239=6, event.240=0` | `kind=field, disc=3, field=1904, story=7650, event.236=1, event.237=0` | 1903 card game (7650..7700) if !Gbit[0xB3C] && !Gbit[0xB3F]; 1908 after How He Ended Up Here |
| 3 | Eiko Talks Life | opt | 1901-1915 (1) | 1905 Treno/Walkway | 7550 | low | `kind=field, disc=3, field=1905, story=7550, event.2=1, event.3=0, event.239=17, event.240=0` | `kind=field, disc=3, field=1904, story=7550, event.236=2, event.237=0` | 1914 sets mask 2 on arrival -1 |
| 3 | How He Ended Up Here | opt | 1901-1915 (2) | 1908 Treno/Gate | 7550 | low | `kind=field, disc=3, field=1908, story=7550, event.2=1, event.3=0, event.239=8, event.240=0` | `kind=field, disc=3, field=1904, story=7550, event.236=4, event.237=0` | 1905 sets bit 2 on return from Eiko Talks Life |
| 3 | Home Sweet Home | opt | 1901-1915 (3) | 1950 Quan's/Cave | 7650 | med | `kind=field, disc=3, field=1950, story=7650, event.2=255, event.3=255, event.239=6, event.240=0` | `kind=field, disc=3, field=1904, story=7650, event.236=8, event.237=0` | 1903 (7650..7700) if Gbit[0xB3F] && !Gbit[0xB85]; row clears bit 3, code -1, field 1950 Quan's/Cave |
| 3 | Memories By The Water | opt | 1901-1915 (4) | 1907 Treno/Queen's House | 7500 | med | `kind=field, disc=3, field=1907, story=7500, event.2=1, event.3=0, event.239=8, event.240=0` | `kind=field, disc=3, field=1904, story=7500, event.236=16, event.237=0` | 1905 sets bit 4 while story < 7550 |
| 3 | Good Old Days | opt | 1901-1915 (5) | 1906 Treno/King's House | 7650 | med | `kind=field, disc=3, field=1906, story=7650, event.2=1, event.3=0, event.239=6, event.240=0` | `kind=field, disc=3, field=1904, story=7650, event.236=32, event.237=0` | 1903 (7650..7700) |
| 3 | Lessons Of Life | opt | 1901-1915 (6) | 1906 Treno/King's House | 7650 | low | `kind=field, disc=3, field=1906, story=7650, event.2=255, event.3=255, event.239=9, event.240=0` | `kind=field, disc=3, field=1904, story=7650, event.236=64, event.237=0` | 1906 sets bit 6 after Good Old Days (choice) |
| 3 | City People | opt | 1901-1915 (7) | 1901 Treno/Bishop's House | 7600 | low | `kind=field, disc=3, field=1901, story=7600, event.2=1, event.3=0, event.239=6, event.240=0` | `kind=field, disc=3, field=1904, story=7600, event.236=128, event.237=0` | 1903 init: !Gbit[0xB3D] && story < 7700 && Gbit[0xB3E] |
| 3 | Hallucination | opt | 1901-1915 (8) | 1904 Treno/Knight's House | 7700 | med | `kind=field, disc=3, field=1904, story=7700, event.2=1, event.3=0, event.239=6, event.240=0` | `kind=field, disc=3, field=1904, story=7700, event.236=0, event.237=1` | 1903 at 7700 if Gbit[0xB3D] |
| 3 | Premonition | opt | 1901-1915 (9) | 1914 Treno/Tot Residence | 7700 | med | `kind=field, disc=3, field=1914, story=7700, event.2=1, event.3=0, event.239=6, event.240=0` | `kind=field, disc=3, field=1904, story=7700, event.236=0, event.237=2` | 1903 at 7700 |
| 3 | Something Washed Ashore | opt | 2105-2173, 2211 (0) | 2173 L. Castle/Harbor | 9050 | low | `kind=field, disc=3, field=2173, story=9050, event.2=1, event.3=0, event.239=114, event.240=8` | - | 2162/2161 local masks at story 9050 |
| 3 | My Bad | opt | 2105-2173, 2211 (1) | 2168 L. Castle/Hallway | 9050 | low | `kind=field, disc=3, field=2168, story=9050, event.2=2, event.3=0, event.239=114, event.240=8` | - | local masks at 9050 |
| 3 | Self-reproach | REQ | 2105-2173, 2211 (2) | 2172 L. Castle/Telescope | 9100 | high | `kind=field, disc=3, field=2172, story=9100, event.2=4, event.3=0, event.239=121, event.240=8` | - | forced on entering 2169 Conf. Room at story == 9100 |
| 3 | Deep Anger | REQ | 2105-2173, 2211 (3) | 2211 Palace/Dock | 9150 | high | `kind=field, disc=3, field=2211, story=9150, event.2=8, event.3=0, event.239=121, event.240=8` | - | forced on entering 2169 at story == 9150 |
| 3 | No Free Lunch | opt | 2105-2173, 2211 (4) | 2105 Lindblum/Shopping Area | 9370 | low | `kind=field, disc=3, field=2105, story=9370, event.2=16, event.3=0, event.239=121, event.240=8` | - | 2169 local bit 4 at 9370 on arrival 35 with Gbit[0xE27] |
| 3 | The Rally | REQ | 2105-2173, 2211 (5) | 2162 L. Castle/Hallway | 9320 | high | `kind=field, disc=3, field=2162, story=9320, event.2=32, event.3=0, event.239=65, event.240=8` | - | forced from 2113 Station Area init at story == 9320 && Gbit[0xE23] != 1 |
| 3 | My Hammer | opt | 2105-2173, 2211 (6) | 2114 Lindblum/Theater Ave. | 9320 | low | `kind=field, disc=3, field=2114, story=9320, event.2=64, event.3=0, event.239=121, event.240=8` | - | 2169 local bit 6 at 9320 on arrival 35, !Gbit[0xE18], Gbit[0xE21] != 1 |
| 3 | Give Me My Money! | opt | 2105-2173, 2211 (7) | 2105 Lindblum/Shopping Area | 9370 | med | `kind=field, disc=3, field=2105, story=9370, event.2=128, event.3=0, event.239=125, event.240=8, event.452=1` | - | from 2173 Harbor talk (entry 8) at story 9370 with Gbit[0xE20], first choice |
| 3 | Team | REQ | 2105-2173, 2211 (8) | 2114 Lindblum/Theater Ave. | 10010 | high | `kind=field, disc=3, field=2114, story=10010, event.2=0, event.3=1, event.239=121, event.240=8` | - | forced on entering 2169 at story == 10010 |
| 3 | Recovery | REQ | 2105-2173, 2211 (9) | 2157 L. Castle/Hangar | 10030 | high | `kind=field, disc=3, field=2157, story=10030, event.2=0, event.3=2, event.239=121, event.240=8` | - | forced on entering 2169 at story == 10030 |
| 3 | Place to Call Home | opt | 2652-2660 (0) | 2656 Bran Bal/Meeting Hall | 10910 | med | `kind=field, disc=3, field=2656, story=10910, event.2=1, event.3=0, event.239=10, event.240=0` | `kind=field, disc=3, field=2654, story=10910, event.236=1, event.237=0` | 2657 Storage sets mask 1 on arrival -2 (story 10910) |
| 3 | Where the Heart Is | opt | 2652-2660 (1) | 2653 Bran Bal/Pond | 10915 | med | `kind=field, disc=3, field=2653, story=10915, event.2=1, event.3=0, event.239=9, event.240=0` | `kind=field, disc=3, field=2654, story=10915, event.236=2, event.237=0` | 2656: bit 0 -> bit 1 |
| 3 | Came into this World | opt | 2652-2660 (2) | 2652 Bran Bal/Gate | 10920 | med | `kind=field, disc=3, field=2652, story=10920, event.2=1, event.3=0, event.239=6, event.240=0` | `kind=field, disc=3, field=2654, story=10920, event.236=4, event.237=0` | 2653: bits 0/1 -> bit 2 |
| 3 | The Truth | REQ | 2652-2660 (3) | 2660 Bran Bal/Hilltop | 10925 | med | `kind=field, disc=3, field=2660, story=10925, event.2=255, event.3=255, event.239=5, event.240=0` | `kind=field, disc=3, field=2654, story=10925, event.236=8, event.237=0` | 2652: bit 2 -> bit 3; forced from 2652 (region, entry 11) at story >= 10925 with mask & 8 |
| 3 | The Elevator | opt | 2712 (0) | 2711 Pand./Control Room | 10980 | low | `kind=field, disc=3, field=2711, story=10980, event.2=254, event.3=255, event.239=152, event.240=10, event.363=4` | `kind=field, disc=3, field=2712, story=10980, event.363=4` | 2712 Pand./Elevator: one-row list, prompt while Gbit[0xB5A] |

Byte values in the lines: `event.2/3` is the arrival code as s16 little-endian (-1 = 255/255, -3 = 253/255, -4096 =
0/240, 256 = 0/1); `event.239/240` the return field or slot; `event.236/237` the mask. Bit flags are written as
whole bytes (`event.259=8` = `Gbit[0x81B]`), so a line overwrites the other bits of that byte: harmless for a fresh
fixture, merge by OR otherwise.

## Town notes (local masks and other gates)

- **Prima Vista / Evil Forest** (200-208, 250-255): standard `Gu16[0xEC]`; 200-208 offer `mask & ~16`, 206 `mask &
  ~20`. Return codes are the field ids.
- **Dali** (350-358, 450): see the Dali section. Return slots 1 350, 2 351, 6 352, 7 354, 9 353, 10 355, 11 356, 12
  357, 13 358, 25 450. 351 builds its mask in `Fu8[0x2B]` (same formula).
- **Lindblum, disc 1** (550-573; 609/850 show the same list; 611 L. Castle/Guest Room carries the list function (entry 3) but its init has no D7 prompt and starts no Select handler, so it never offers the menu: verified on the 3DS port's PC build at story 3120 with Baku and His Crew available, F39): standard. 552 and 553 hide the prompt for arrival 1
  and 2. Return codes are field ids.
- **Treno, disc 2** (900-915): standard plus `Gu16[0xEA]` (bit = return slot - 3). Return slots: 903 6, 904 7, 905 8,
  908 11, 912 15, 915 18 (909, 914, 900 have none in their init).
- **Cleyra** (1051-1060): each field computes its own `Fu16[0x1E]` (1056: `Fu16[0x31]`): bit 0 if `Gu8[0x1E3] == 0`
  and bit 2 if `== 2` (1053); bit 1 if `Gu8[0x1E4] == 0` and bit 3 if `== 2` (1051, 1056); the mask is cleared while
  `Gbit[0xF11] == 0` (event.482 bit 1). Bit 4 (The Treetop Town of Cleyra) isn't set by any entry-0 init. Arrival codes
  are negative bit values (-1, -2, -4, -8, -16); return codes are field ids.
- **Lindblum, disc 2** (1300-1314, 1355-1370, 1660): standard. 1370 L. Castle/Lift sets mask 4, a row the list
  doesn't have (only 2 titles).
- **Black Mage Village** (1450-1460): standard plus `Gu16[0xEA]` (bit = slot - 5). 1450 sets mask 25 only on arrival
  code 4; 1451 turns bit 0 into bits 1+2 on entry, 1458 bit 3 into bit 5.
- **Conde Petie** (1501-1509): `Gu16[0xEC]`, copied to `Lu16[0x0]` in 1501-1503 and 1509; return slots 4..16.
- **Madain Sari** (1600-1603, 1607): local `Fu8[0x18]` while `story < 6640` and `Gu16[0x129] & 8`: row k when exactly
  one of the pair is set: row 0 0x807/0x804, row 1 0x806/0x803, row 2 0x805/0x802, row 3 0x80A/0x809, row 4
  0x812/0x811. Note the reuse of 0x80x flags (Dali uses 0x807 for something else; they are chapter-local).
- **Alexandria, disc 3** (1850-1866): local `Fu16[0x1E]` (1851 `Fu16[0x20]`, 1865 `Fu16[0x31]`) gated by a per-field
  phase byte (`Fu8[0x20]`, ...) and: bit 0 `Gu8[0x1E9] == 1` (phase 0), bit 1 `Gu8[0x1EA] == 1`, bit 2 `Gu8[0x1EA] ==
  3`, bit 3 `Gbit[0xF41] && !Gbit[0xF40]` (phase 1); 1853 bit 4 `Gu8[0x1FC] == 1`. The phase byte is field-local, so
  the menu line may need a runtime check; the direct line doesn't depend on it.
- **Treno, disc 3** (1901-1915): standard plus `Gu16[0xEA]`; 1903 (card stadium) hands out most bits from its
  tournament script between story 7600 and 7700.
- **Lindblum, disc 3** (2105-2173, 2211): the list is in 2105/2113/2114, 2157-2173 and 2211; each field builds a local
  mask (`Fu8[0x18]`, `Fu8[0x1A]`, `Fu8[0x2B]`) from exact story values and arrival codes, e.g. 2169 Conf. Room: 9100
  → row 2, 9150-9249 → row 3, 9320 with arrival 35 → row 6, 9370 with arrival 35 and `Gbit[0xE27]` → row 4, 10010 →
  row 8, 10030 → row 9. Arrival codes are the bit values (1 .. 512). No menu lines: use the direct starts.
- **Bran Bal** (2652-2660): standard; each ATE field turns its bit into the next one (chain 0 → 1 → 2 → 3).
- **Pandemonium** (2712 Elevator): one-row list with the mask constant 1, prompt while `Gbit[0xB5A]`; arrival -2 into
  2711.

## Known gaps

- Nothing was run. Every story value marked med/low comes from the script that unlocks the ATE (the value it tests or
  the scene value just before/after) or from the ATE field's own story tests, not from a playthrough. The ATE field's
  scene may need a narrower window; a runtime pass over the direct lines (enter, wait for the scene's first message)
  would settle them.
- Dali menu row 0 ("Queen Brahne's Steiner" with arrival 16): what plays in 355 is unclear (Steiner only exists below
  story 2650 and his init has no case for 16).
- Dali 450 at 2840: which visible element is BG layer 0x15 and which is model 240 (barrels vs. ship) is inferred, not
  seen.
- Cleyra "The Treetop Town of Cleyra" (bit 4): its unlock isn't in any field init's local mask; not traced further.
- Field-local masks built outside entry 0 (Madain Sari, Lindblum disc 3, parts of Alexandria) are summarised from the
  init code only; extra conditions in other entries may exist.
- Encounter 294 in field 451 is taken to be Black Waltz No. 2 from the story order (only battle in 450-454); the
  enemy name wasn't read.
- Alexandria disc 1 (100-117) has the prompt code (field-local `Fu8[0x18]`) but no Select menu or title card: no
  ATE starts there (the "Girl Who Was Left Behind" scene plays in 112 but is chosen from Prima Vista).
- Title texts contain character-name codes (`TEXT_CHAR_NAME`); the table shows the default names.
