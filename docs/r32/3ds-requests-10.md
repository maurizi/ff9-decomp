# R40: auction bid entry, direct minigame entry, Quina for the frog fixture (3DS port, follow-up to R38)

Sources: the field scripts (`tools/evtscript/edis.py FIELD --disc N [--entry E --tag T]`), the matched C
(`src/main/event.c`, `src/main/msg.c`, `src/main/chr.c`, `src/main/evt_main.c`, `include/ff9.h`), and PCSX-Redux runs
on discs 1-3 (2026-11-01). Notation as in R37/R38 (`docs/r32/3ds-requests-8.md`): `event.N` = `g_evtGlobalVars.b[N]`
(main 0x8007AEF0 + N), `story` = h[0]; script banks `G` global, `F` field (`EvtState.fieldVars`, EvtState at
0x80077E60 at runtime), `L` = the running object's locals (`g_evtObjVars` = object + `varsOfs` * 4,
`include/ff9.h:1379`). Every forced field entry also needs R38's `event.13=0 event.14=0`.

## Paste-ready answers

### 1. Auction bid entry (disc 2, field 909)

```
bid value    : Ls16[4]  = s16 at (object uid 10).vars + 4   (field 909 entry 10; runtime 0x80077F1C)   bid / 100 Gil
digit cursor : Ls8[1]   = s8  at (object uid 10).vars + 1   (runtime 0x80077F19)   0 = 100s, 1 = 1000s, 2 = 10000s
hold counter : Ls8[0]   = s8  at (object uid 10).vars + 0   (runtime 0x80077F18)   Up/Down auto-repeat, leave alone
vars address : o = evt_getObject(10); vars = (u8 *)o + o->varsOfs * 4   (varsOfs 5 -> o + 0x14; o was 0x80077F04)
price        : Fs24[0x1B] = EvtState.fieldVars + 0x1B (u24, runtime 0x80077E7B)
top bidder   : Fu8[0x39]  = fieldVars + 0x39 (0 = the player, 1..5 = NPC entries 11..15, 255 = no item / sold)
touch bid    : write Ls16[4] = N (clamp: price/100 < N <= min(999, gil/100)), then g_msgPadPressed |= BTN_CROSS (0x20000)
               between msg_update and evt_updateObjects (evt_update, src/main/evt_main.c:16-18)
touch cancel : g_msgPadPressed |= BTN_CIRCLE (0x10000) at the same point
```
Verified: poke 50 + Cross → price 5000, Fu8[0x39] = 0, balloon "5000 Gil!" over Zidane; poke 5 + Cross → "You must bid at
least 100 Gil higher than the current bid."; Circle → Ls16[4] = -1, no bid, the "?" bubble returns; poke 123 → the
window reads "12300 Gil" on the next frame. **The confirm does not check funds**: a poked 2000 (200,000 Gil, with 127,972
held) was accepted as the new price. The port must clamp, because the sale later does `REMOVE_GIL price`, and
`EVT_OP_REMOVE_GIL` sets gil to 0 when it would go below zero (`event.c:2011`).

### 2. Direct minigame entry (all verified from a fresh field entry)

Use the game's own talk function, as R34 did for the Moogle: `evt_callObjectFunc(evt_getObject(uid), 1, 3, 0)` (3 =
`EVT_FUNC_TALK`), called while `g_evtState->controlEnabled` = 1 and outside the script run (the R34 hook point: after
`msg_update`, before `evt_updateObjects`). Each talk function returns at once unless `sys.controlEnabled` is set.

```
jump rope  disc 1 field 103 story 2840 event.2=0 event.13=0 event.14=0
           after load (controlEnabled=1): SET_POS_XZ player(uid 30) = (-2349, 59)  [+ SET_DIR 192]   (see "teleport" below)
           then evt_callObjectFunc(evt_getObject(8), 1, 3, 0)          -> "You wanna try?"
           Cross -> the player walks to (-2494, 170) by itself -> "0 times" + "!" (~250 vsyncs)
hippaul    disc 3 field 1850 story 7000 event.2=0 event.13=0 event.14=0 [event.491=level]
           after load: evt_callObjectFunc(evt_getObject(7), 1, 3, 0)   (no move needed: the race fade teleports everyone)
           Cross x2 ("My son plays..." / choice "I don't mind" = row 0) -> Cross ("Okay, let's start!", fade)
           -> "When I say GO!..." -> Cross -> "On your mark!" / "Get set!" / "GO!" + "Press [Square] and [Circle]" (~320 vsyncs)
auction    disc 2 field 909 story 6960 event.2=0 event.13=0 event.14=0 gil >= price+100
           wait until Fu8[0x39] (fieldVars+0x39) is 1..5 (an NPC has bid; ~260 vsyncs after control is enabled)
           and Fu8[0x3C] == 0, then evt_callObjectFunc(evt_getObject(16), 1, 3, 0)   (region 16 = the bid spot)
           -> "Bid Status" + "Bid: 01100 Gil" windows at once. The default arrival (-14, -1638, dir 128) passes the
           region's facing test, so no move is needed.
```
**Teleport** (jump rope only): do what event op `EVT_OP_SET_POS_XZ` (0x1D) does (`src/main/event.c:1004-1023`):
`bgi_setActorActive(o->modelSlot, 0)` (ovl_0a 0x800C3E18), `pos = prevPos = (x, -0x8000, z)`, `bgi_setActorActive(slot,
1)`, `moveDistSq = EVT_MOVE_DIST_NONE`, `pitch = 0`, `moveFlags &= ~(EVT_MOVE_FACING_SET | EVT_MOVE_APPROACHING)`,
`g_evtPosSetThisFrame = 1`; facing = `EVT_OP_SET_DIR` (`rotY = dir8 << 4`). Writing x/z alone (R38) leaves the walkmesh
triangle stale, so the actor stops moving; with the deactivate/activate pair the player walks normally (verified: Right
held 20 vsyncs moved x from -2349 to -1749).
Without a teleport the jump rope fails: from the default arrival (-75, -2210) the scripted walk to (-2494, 170) stops
at (-2054, 409) against an obstacle, and from arrival 208 (-2519, 1673, on the raised walkway y = -1101) it stops at
(-2581, 1448).

### 3. Frog fixture: Quina

```
party 0,1,2,5    Quina is character record 5 (GameData.characters[5], GameData+0x3C+5*0x90; runtime 0x8008B55C);
                 slot order does not matter (verified with Quina in party slot 3)
record 5 must be Quina, not Cinna: classFlags (+0x0A) = 0x05 (no CHR_CLASS_GUEST 0x10), info.row (+0x34 bits 12-16) = 5,
                 then chr_join(5, 1, 5)  = what the game's own join op does: PARTY_JOIN 05 01 05 05 05
                 (chr 5, level = average of the recruited, default equipment set 5, classFlags 5, ability row 5)
                 -> info.kind = 9 (Quina), level 51 with the Dali party, equip 4F FF FF FF FF
pokes used       name (+0x00, 10 bytes) = 20 44 38 3D 30 FF FF FF FF FF ("Quina"), classFlags 05, info 0x00005EE5
                 (id 5, row 5, recruited, Cinna's battle-row/victory bits kept; chr_join rewrites kind),
                 event.19 |= 0x20 (b[0x13] bit 5: record 5 initialised, see below)
plus R38         story 3880 event.2=0x1060 event.399=1 event.785..788 = 33 66 55 99 event.13=0 event.14=0
```
Verified: with record 5 = Cinna (field_dali_inn's record: classFlags 0x15, kind 14, row 8) and party 0,1,2,5, field 657
shows no prompt (`Fu8[0x40]` stays 0); with the pokes above + `chr_join(5, 1, 5)` → "Quina: Zidane, can I catch
frogs?" → Cross x3 → **"Frogs Captured: 0"** with Quina (model 273) as the player (`EvtState.playerUid` 22) and frogs in
the pond. Minimal condition: the frog prompt only needs a party member with `info.id == 5 && info.kind < 14`
(`evt_isInParty(5)`), but kind 9 is what makes the field use Quina's model, and the name is printed in the speaker line.

## Details

### Auction bid loop (field 909 entry 10 tag 31, called by region 16 tag 3 with `CALL_SYNC 2, 10, 31`)

`edis.py 909 --disc 2 --entry 10 --tag 31` (offsets in the function):
```
0003  MSG_NUMBER 0 = Fs16[0x2d] (item), 1 = Fs24[0x1b] (price), 4 = sys.gil; MSG 1 0xC9 (or 0xCA when price >= 10000)
002f  if (sys.gil >= price + 100) {                         else 02d4: MSG_WAIT 0xD0 "Not enough funds."
003d    Ls16[4] = (price + 100) / 100; Ls8[1] = 0; Ls8[0] = 0
005c    MSG_NUMBER 7/6/5 = Ls16[4] % 10, (Ls16[4] / 10) % 10, Ls16[4] / 100;  MSG 6 0xCB (bid window), MSG 3 0x10 0xCE
0093    loop {  padPressed(0x20) Right: digit 1 -> 0 (window 3 0xCE), 2 -> 1 (window 4 0xCD)
00d5            padPressed(0x80) Left:  digit 0 -> 1 (window 4 0xCD), 1 -> 2 (window 5 0xCC)
0114            padHeld(0x10) Ls8[0]++ / padHeld(0x40) Ls8[0]-- / else Ls8[0] = 0
013e            if (padPressed(0x40) || Ls8[0] == -8)  Down: digit 0: Ls16 > 0 -> -1; 1: > 9 -> -10; 2: > 99 -> -100;  Ls8[0] += 2
01ab            if (padPressed(0x10) || Ls8[0] ==  8)  Up:   digit 0: Ls16 < gil/100 && < 999 -> +1
                                                              digit 1: Ls16 < gil/100 - 9 && < 990 -> +10
                                                              digit 2: Ls16 < gil/100 - 99 && < 900 -> +100;  Ls8[0] -= 2
023b            MSG_NUMBER 7/6/5 from Ls16[4] (every frame); WAIT 1
0260         } while (!padPressed(0xB0000))                  (Circle | Cross | Square)
026c  MSG_CLOSE 3, 4, 5, 6;  if (padPressed(0x10000)) Ls16[4] = -1  (Circle = cancel); MSG_CLOSE 1
028e  if (Ls16[4] != -1) { if (Ls16[4] * 100 > price) { price = Ls16[4] * 100; MSG_WAIT_WIN 7; MSG_NUMBER 1 = price;
                           Fu8[0x39] = 0; CALL_SYNC 2, 0xFA, 24 (player: MSG_WAIT 7 0x80 0xBE "N Gil!") }
                           else MSG_WAIT 0xCF "You must bid at least 100 Gil higher than the current bid." }
```
- The pad ops are expression ops 0x4F `padPressed(mask)` → `msg_getPadPressed()` = `g_msgPadPressed & 0x3FFFFFF`
  (main 0x8007B97C, `src/main/msg.c:1066`) and 0x59 `padHeld(mask)` → `g_padPortHeld[0]` (`msg.c:1062`). Cross and
  Square do the same thing (the loop exit); Circle is tested again after the windows close, so Circle wins when both
  are injected. Rules: unit 100 Gil; start = price + 100; minimum accepted = price + 100 (strictly greater than the
  price); Up stops at 999 (99,900 Gil) and at gil / 100 (per digit: the step must fit); Down stops at 0 / 9 / 99 per
  digit; the confirm itself checks only `bid * 100 > price`.
- Region 16 tag 3 (Cross in the bid spot): `sys.controlEnabled`, facing test `Fs16[6] = (player.dir8 -
  atan2(obj7.x - player.x, obj7.z - player.z)) & 255`, `< 56 || > 200`, then `Fu8[0x39] != 0 && !Fu8[0x3C]`
  (`Fu8[0x3C]` = the auctioneer is presenting); sets `Fu8[0x3C] = 1`, disables control, `CALL_SYNC 2, 10, 31`,
  re-enables control, `Fu8[0x3C] = 0`. Its tag 2 shows the "?" bubble (`PLAYER_BUBBLE 0`) under the same conditions.
- Entry 10 tag 1 (auction driver) shares the object's locals: `Ls8[2]` (+2) item presented, `Lu8[3]` (+3) the random
  bidder 1..5. Entry 7 tag 27 sells (`REMOVE_GIL Fs24[0x1B]` when `Fu8[0x39] == 0`).

### Arrival codes (event.2 = `EVT_GB_ARRIVAL_POINT`) of the three fields

| field | entry (player) | code → x, z, dir8 |
|---|---|---|
| 103 (Alexandria Square, disc 1) | 29-37 tag 0 | 209 → 45, -950, 0; 111 → -3373, -680, 192; 228 → 2247, 2446, 4; 229 → 3688, 561, 64; 207 → -2309, 1486, 234; 208 → -2519, 1673, 233 (raised walkway); default → -75, -2210, 128 |
| 1850 (Alexandria Main Street, disc 3) | 15-22 tag 0 | 831 → Gs16[0xF3], Gs16[0xF7], Gu8[0xF9] (a stored position); 201 → 0, 6092, 0; 231 → 654, 1073, 64; default → 0, 332, 128 |
| 909 (Treno Auction Site, disc 2) | 17-25 tag 0 | -1 → 83, -1228, 156; 1 / -3 → -23, -2787, 128; -2 → -74, -2028, 0; 9 → -14, -1548, 128; default → -14, -1638, 128 |

None of them puts the player next to the jump-rope girls or Hippolady; field 1850's 831 would (any position via
b[0xF3..0xF9]) but the talk call makes it unnecessary. In 909, codes -1/1/-3 skip region 9 and -2 skips the auction
actors (entries 10-16), so keep event.2 = 0 there.

### Talk functions used

- Field 103 entry 8 (girl, uid 8) tag 3: needs `sys.controlEnabled`; resets the game (`Fu8[0x29..0x30]`, count
  `Fs24[0x2B]`), msg 0xE9, walks the girl to the rope, `CALL_WAIT_FREE 4, 0x1E, 12` → player tag 12: `MOVE_XZ (-2494,
  170)`, then state 2 and the "0 times" window. No distance or facing test, which is why the walk matters.
- Field 1850 entry 7 (Hippolady, uid 7) tag 3: needs `sys.controlEnabled`; msg 0x8F, choice 0x90 (row 1 = "I'd rather
  not" → 0x91 and end), 0x92, fade, `CALL_SYNC 4, x, 14` on the player, Hippaul and the spectators (start line), msg
  0x93, then the race as in R38.
- Field 909 region 16 tag 3: above.

### Quina's record

- `evt_isInParty(slot)` (`src/main/event.c:3855`, inlines `include/ff9.h:5043-5085`): slots 9-11 map to records 5-7
  with `kind >= 14` (Cinna, Marcus, Blank), slots 0-8 need `kind < 14`. Field 657 entry 0 tag 0 tests
  `isInParty(5)` (Quina), so Cinna in record 5 fails.
- `EVT_OP_PARTY_JOIN` 0xFE (`event.c:2029-2046`): `chr, avg, eq, classFlags, row` → `classFlags` and `info.row`
  written, then `chr_join(chr, avg, eq)` (`src/main/chr.c:205`): equipment from `g_chrDefaultEquip[eq]`, `info.kind =
  chr_calcAppearance(id, classFlags >> 4 & 1, equip)`, level = average of the other recruited characters. The shared
  party-setup code in fields 618/657/661 (entry 0 tag 1) runs `PARTY_JOIN 5, 1, 5, 5, 5` once, guarded by
  `(Gu16[0x13] >> 5) & 1 == 0`, then sets bit 5 (`Gu16[0x13] |= 32`). Bits 0, 1, 4, 5 of b[0x13] are set next to
  PARTY_JOIN 0, 1, 4, 5 the same way (bit k = record k initialised).
- Record values seen: `field_dali_inn` record 5 = Cinna (classFlags 0x15, info 0x000086E5: kind 14, row 8, not
  recruited, equip 00 70 FF FF FF); `field_burmecia_vault` record 5 = Quina (classFlags 0x05, info 0x00005E95: kind 9,
  row 5, recruited, equip 50 75 5A 98 FF), b[0x13..0x14] 0x470F (Dali) vs 0x473F (Burmecia).

## Runtime evidence (PCSX-Redux)

Scratch scripts in `build/3ds10/` of the worktree: `inject.py` (load a state, pokes, stop at evt_update's `jal
evt_updateObjects` 0x8004BC64, OR a mask into `g_msgPadPressed` and/or redirect into `evt_callObjectFunc(obj, lv, tag,
0)` returning to the jal), `teleport.py UID X Z [DIR8]` (the SET_POS_XZ sequence with two `bgi_setActorActive` calls),
`enterq.py` (`tools/rt/enter_field.py` plus a `chr_join` call at main's dispatcher), `chars.py` (character records and
party), `objs.lua` (objects with their locals address), `waitb.py`.

- Auction, disc 2: `inject.py --state r38_auction_bid --poke 0x80077F1C=3200 --btn 0x20000` → price
  (0x80077E7B) 0x001388 = 5000, `Fu8[0x39]` 1 → 0 after the balloon, "5000 Gil!" over Zidane. `--poke
  0x80077F1C=0500` → message 0xCF; `--btn 0x10000` → Ls16 = 0xFFFF, price unchanged; `--poke 0x80077F1C=d007` → price
  0x030D40 = 200000 with 127,972 Gil. `--btn 0x10` (Up) with `Ls8[1]` = 0 → 13 → 14; with `0x80077F19 = 2` → 13 → 113.
- Auction direct: `enter_field.py 909 --disc 2 --poke 0x8007AEF2=0000 --poke 0x8007AEFD=0000` (base `field_iifa_tree`,
  story 6960), 600 vsyncs → control 1, `Fu8[0x39]` = 255; 260 vsyncs later 1 (state `r40_auction_fresh_bidup`);
  `inject.py --call 16,1,3` → both bid windows ("Price: 1000 Gil", "Bid: 01100 Gil"); poke 75 + Cross → "7500 Gil!",
  price 0x1D4C.
- Hippaul direct, disc 3: `enter_field.py 1850 --disc 3 --base-state disc3_pandemonium_lv99_field --poke
  0x8007AEF0=581b0000 --poke 0x8007AEFD=0000`, 600 vsyncs (player uid 15 at (0, 332), control 1; state
  `r40_hippaul_fresh`); `inject.py --call 7,1,3` → "My son plays too many card games." (Hippolady off screen); Cross →
  "Do you mind racing against Hippaul? / I don't mind / I'd rather not"; Cross, Cross → both runners at the start line,
  "When I say GO!..." (`r40_hippaul_howto_direct`); Cross + 320 vsyncs → "GO!" and "Press [Square] and [Circle]
  alternately!".
- Jump rope direct, disc 1: `enter_field.py 103 --base-state field_dali_inn --poke 0x8007AEF0=180b0000 --poke
  0x8007AEFD=0000` (`r40_f103_fresh`, player uid 30 at (-75, -2210)). `--call 8,1,3` alone: "You wanna try?", Cross,
  then the player's scripted walk stops at (-2054, 409) (`Fu8[0x29]` stays 1 after 800 vsyncs). With `teleport.py 30
  -2349 59 192` first: Cross → player at (-2505, 176), `Fu8[0x29]` = 2, "0 times" window and "!" bubble
  (`r40_jumprope_direct_ready`). Arrival 208 (`--poke 0x8007AEF0=180bd000`) + call: stuck at (-2581, 1448).
- Frogs, disc 1: `enter_field.py 657 --base-state field_dali_inn` with story 3880, event.2 0x1060, b[0x18F] 1,
  b[0x311..] 33 66 55 99, party pointers 0x8008B79C = records 0, 1, 2, 5 (record 5 = Cinna): no prompt, `Fu8[0x40]`
  (0x80077EA0) 0. `enterq.py 657 1 field_dali_inn 900` with the same pokes plus `0x8008B55C = 2044383d30ffffffffff05`,
  `0x8008B590 = e55e0000`, `0x8007AF03 = 2f`, `--chrjoin 5,1,5` → record 5: classFlags 05, level 51, kind 9, row 5,
  equip 4F FF FF FF FF; `Fu8[0x40]` = 0x63, "Zidane, can I catch frogs?" (`r40_frog_quina_ask`); Cross x3 + 120 vsyncs
  → "Frogs Captured: 0", `Fbit[0x265]` (fieldVars 0x4C bit 5) set, player uid 22 = model 273 (`r40_frog_quina_running`).
  Walking down out of the pond cleared `Fbit[0x265]` and the window closed.

States created: disc 1 `r40_f103_fresh`, `r40_jumprope_direct_ready`, `r40_frog_quina_ask`, `r40_frog_quina_running`;
disc 2 `r40_auction_fresh_bidup`; disc 3 `r40_hippaul_fresh`, `r40_hippaul_howto_direct`.

## Proposed names (not applied)

- `EVT_GB_CHR_JOINED 0x13` (u16, b[0x13..0x14]: bit k = character record k initialised by its first PARTY_JOIN; the
  shared party-setup scripts test it before `PARTY_JOIN k`). medium-high (bits 0, 1, 4, 5 seen next to the joins).
- `EVT_GB_SAVED_POS 0xF3` (s16 x, s16 y at 0xF5, s16 z at 0xF7, u8 dir at 0xF9: the position field 1850's arrival code
  831 restores). low (one field read; who writes it was not checked).
- `EVT_FUNC_TALK` (3) and `EVT_SYSVAR_GIL` (6, `sys.gil` in the listings) already exist. For the port only (field-local,
  not ff9.h material): `AUCTION_BID_LOCAL 4` (s16) / `AUCTION_DIGIT_LOCAL 1` (s8) / `AUCTION_REPEAT_LOCAL 0` (s8) in the
  locals of field 909's entry-10 object, `AUCTION_PRICE_FV 0x1B` (u24) / `AUCTION_TOP_BIDDER_FV 0x39` /
  `AUCTION_BUSY_FV 0x3C` in fieldVars. high (runtime).
