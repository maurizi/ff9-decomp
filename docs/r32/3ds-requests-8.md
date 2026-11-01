# R38: field minigame scenario recipes for the 3DS port

Sources: the field scripts (`tools/evtscript/edis.py FIELD --disc N`), the field message tables (static dump, see
"Tools" below), the matched C, and PCSX-Redux runs on discs 1-3 (2026-11-01). Background and notation as in R37
(`docs/r32/3ds-requests-7.md`): `event.N` = `g_evtGlobalVars.b[N]` = main 0x8007AEF0 + N (decimal N, hex in
brackets), `story` = h[0]. Script variables: `G` = global bank (`g_evtGlobalVars`), `F` = field bank
(`EvtState.fieldVars`, `*g_evtState` = 0x80077E60 at runtime, cleared on every field entry), `L` = object locals;
`Fu8[0x29]` = u8 at field-var byte 0x29, `Gbit[k]` = bit k&7 of b[k>>3]. Field pad masks in scripts: low 16 bits are
the raw controller bits (`PAD_*`, `include/ff9.h:2137`: 0x10 Up, 0x20 Right, 0x40 Down, 0x80 Left, 0x1000
Triangle, 0x2000 Circle, 0x4000 Cross, 0x8000 Square, 1 Select, 8 Start), the high ones the game buttons (`BTN_*`:
0x10000 Circle, 0x20000 Cross, 0x80000 Square). `padPressed`/`padHeld`/`padReleased` are expression ops 0x4F/0x59/0x58
(`ff9.h:4279`), which read `msg_getPadPressed`/`msg_getPadHeld`/`msg_getPadReleased` (`src/main/msg.c:1062-1072`).

**One harness rule for every forced field entry: `event.13=0 event.14=0`** (b[0x0D], b[0x0E], the two
environment-sound slot states). Every field init script does `if (b[0x0D] == 2 && its env sound id b[0x09..0x0A] < 0)
b[0x0D] = 9`, and then shows message 0x44 of the area text, **"Error Env Play() Slot=N"** (window 6), when b[0x0D] or
b[0x0E] is 9. A save made in a field that plays an environment sound (state 2) and dropped into a field without one
shows that box, which is what R37 saw in Chocobo's Forest. It is a script message, not an engine error. With both
bytes 0 it never appears (verified in fields 64 and 2950).

None of these minigames uses the field timer (`g_timerRect`, `timer_*` in `src/main/tu_800236C4.c:90-242`) except
Chocobo Hot and Cold. Field scripts with timer ops (0x69/0x7D/0x8D): Lindblum 550-572 (Festival of the Hunt), 2950,
and disc-3 Alexandria Square 1853. Every counter below is a script variable shown through a message window with
`EVT_OP_MSG_NUMBER` (0x66, `event.c`; the number is printed by text code `<40:slot>`).

## Recipes (compact)

All verified in PCSX-Redux unless marked. "Base" is the save used; the harness equivalent is the listed
story/party/items plus `event.13=0 event.14=0`. Presses are in vsyncs (the field runs at 30 fps, one script frame =
2 vsyncs).

### 1. Alexandria jump rope (disc 1)

```
kind field   disc 1   field 103 (Alexandria/Square)   story 2840   event.2=0   event.13=0 event.14=0
party: any (verified with the Dali save's Zidane/Dagger/Vivi/Steiner, Lv 50)
```
The girls (entry 8, at (-2550, 228)) are created unconditionally. Default arrival puts the player at (-75, -2210).
Presses: hold **Left 80**, hold **Up 66**, hold **Left 5** (player at about (-2349, 59)), **Cross** → "You wanna
try?" (msg 0xE9, window 3, no choice); **Cross** → ~200 vsyncs of walking into place → top-left counter window
**"0 times"** (msg 0xEA, window 0) and a "!" bubble over the player (`PLAYER_BUBBLE`). Then **Cross or Square** (mask
0xA0000) once per rope turn while the "!" shows: each successful jump reprints the window as **"N times"** (msg
0xEB, number slot 2 = the count). Missing a turn ends the game; then the comment window closes, the girl says
**"Come play with us again!"** (msg 0xF6). Comments in window 4 at the thresholds: 11 "It's gonna get faster after
20!" (0xEC), 20 "Getting faster!" (0xED), 40 (0xEE), 90 "Almost 100!" (0xF0), 100 (0xF1), 180 (0xF2), 220 (0xF3), 280
(0xF4), 300 "Jump! Jump!" (0xF5); first-time rewards at 20/50/100/200/300 in window 6 ("Received N Gil!" 0xF8,
"Received the X Card!" 0xF7). States: `r38_f103_entry`, `r38_jumprope_ask`, `r38_jumprope_ready` ("0 times" up),
`r38_jumprope_counting` (4 jumps).

### 2. Hippaul racing (disc 3)

```
kind field   disc 3   field 1850 (Alexandria/Main Street)   story 7000 (any < 7100)   event.13=0 event.14=0
event.491=<Hippaul level 0..100>   (b[0x1EB]; the Pandemonium save had 44)
```
Hippolady (entry 7, (-496, 2091)) and Hippaul (entry 8) exist only while story < 7100 (`Fu8[0x20]`, field 1850 entry
0 tag 0). Default arrival: player at (0, 332). Presses: hold **Up 52**, hold **Left 12**, Up 3, Left 3, **Cross** →
"My son plays too many card games..." → **Cross** → **"Do you mind racing against Hippaul?" / I don't mind / I'd
rather not** (msg 0x90, window 2) → **Cross** ("Okay, let's start!", fade, both at the start line) → **"When I say GO!,
press [Square] and [Circle] alternately!"** (0x93) → **Cross** → "On your mark!" (0x95) → "Get set!" (0x96) → **"GO!"**
(0x97, balloon) plus the top-left window **"Press [Square] and [Circle] alternately!"** (msg 0x94, window 5) for the
whole race. Input: **Circle (0x10000) and Square (0x80000) alternately**; each change of button sets the runner's
speed to 30, which drops by 5 per script frame and moves the runner `speed` units along z; pressing the same button
twice does nothing. Finish line z = 5000. End: "<leader> wins!" (0x98) or "Hippaul wins!" (0x99), then rewards.
Verified: alternating every 4 vsyncs from GO wins in 460 vsyncs at level 44 ("Vivi wins!", level 44 → 45). No timer,
no counter window. States: `r38_hippaul_entry`, `r38_hippaul_ask`, `r38_hippaul_howto`, `r38_hippaul_race` (GO,
window up).

### 3. Frog catching (Quina, Qu's Marsh; disc 1 shown, ponds also on discs 2-4)

```
kind field   disc 1   field 657 (Marsh/Pond)   story 3880   party must include Quina (character 5)
event.2=4192   (0x1060: pond 1 (0x1000) + arrival 0x060)    event.399=1   (b[0x18F] > 0)
event.785=0x33 event.786=0x66 event.787=0x55 event.788=0x99   (b[0x311..0x314]: pond-1 frog populations, 8 nibbles)
event.13=0 event.14=0
```
Base used: `field_burmecia_vault` (card 1 FILE01, Lv 99; party Zidane/Vivi/Freya/Quina). On entry Quina asks
**"Zidane, can I catch frogs?"** (msg 0x163) → **Cross** → **"Frogs, again?" / Sure, just for a little while / Do it
later** (0x164) → **Cross** → "Thanks!" (0x166) → **Cross** → Quina is the player and the top window **"Frogs
Captured: N"** (msg 0x183, window 6, N = `GameData.frogCount`, system variable 16) stays up while frog mode is on
(field bit `Fbit[0x265]`). Frogs (entries 5-9: baby, adult, male, female, golden) swim in the pond; Cross next to one
→ **"Keep <frog>? / Keep it / Let it go"** (msg 0x182), Keep → op 0xE0 ADD_FROG (`frogCount++`, `event.c:2138`) and
the window reprints. Without the high-nibble pond code in event.2 (0x1000/0x2000/0x4000/0x8000 select pond 1-4 and
its population bytes b[0x311..0x320]) no frogs spawn. The harness has no frog-count parameter: frogCount is
`GameData+0x568` (s16). Verified up to the counter window with frogs swimming; the Keep prompt was not reached (the
frogs stayed in the water, and moving a frog's script position did not make it talkable). States: `r38_frog_ask`,
`r38_frog_running` (no frogs: event.2 = 0x60), `r38_frog_pond1` (frogs up).

### 4. "I Want to Be Your Canary" sword fight (disc 1)

```
kind field   disc 1   field 64 (A. Castle/Public Seats)   event.2=0   event.13=0 event.14=0
story: any (verified 2840; the script only checks story < 1900 for background animation)
```
Any arrival code other than 315/322/327 starts the play (`Fu8[0x18] = 1`, field 64 entry 0 tag 0). On entry: Zidane
"Expect no quarter from me!" (0x6A) and Blank "En garde!" → **Cross** → instruction window **"To follow Blank's lead,
enter the correct commands from the following choices: [d-pad] [Triangle Circle Cross Square]"** (msg 0x6F, window 6)
→ **Cross** → ~30 vsyncs later the first prompt. Prompts: a balloon over Blank **"Press X!"** (msgs 0x70-0x77,
window 1) for prompt `Fu8[0x2e]`: 0 Left (0x70), 1 Right, 2 Triangle, 3 Down, 4 Cross, 5 Up, 6 Circle, 7 Square
(0x77). 50 prompts; each must be answered within `Fu8[0x34]` script frames (50, or 30 once Fs16[0x24] is set).
Results: **"Of the 100 nobles watching, N were impressed."** (0x78/0x7A, window 5), then "Queen Brahne was not
impressed." (0x79, max combo < 50) or "quite impressed" (0x7B), then by N: < 25 "The audience is booing... Perform the
fight scene again? Yes/No" (0x7C), < 50 (0x7D), < 75 (0x7E), else "They demand an encore!" (0x7F); No → "They shower
you with N Gil!" (0x80). There is **no on-screen crowd meter** on the PS1: the "meter" is `Fs8[0x26]` (-1..2), the
fight's position on the stage, which only moves the camera (BG_SCROLL_TO 0xA2/0xE4/0x122/0x140). Verified: 50 correct
prompts in ~2400 vsyncs; a shortened run gave "18 were impressed" and the booing prompt. States:
`r38_canary_entry`, `r38_canary_howto`, `r38_canary_fighting` (prompts running), `r38_canary_results`.

### 5. Treno auction house bid window (disc 2)

```
kind field   disc 2   field 909 (Treno/Auction Site)   story 6960 (gate: >= 4435)   event.13=0 event.14=0   gil >= price + 100
```
Base: `field_iifa_tree` (disc 2, story 6960, 127972 gil). The auction runs by itself on entry (auctioneer balloons
"1000 Gil!", NPC bids; items cycle). Player at (-14, -1638). Presses: hold **Up 75**, hold **Right 10**, hold **Up
30** (player about (49, -14), inside region 16 in front of the stage), **Cross** while an item is up → two windows:
**"Bid Status / Funds: N Gil / Item: <item> / Price: N Gil"** (msg 0xC9, window 1; 0xCA when the price is >= 10000)
and **"Bid: 01300 Gil / [Up/Down]: Adjust number / [Left/Right]: Select digit / [Cross]: Confirm / [Circle]: Cancel"**
(msg 0xCB, window 6), with the selected digit highlighted by a one-digit window (0xCE/0xCD/0xCC, windows 3/4/5).
Input: Left/Right (0x80/0x20) select the digit, Up/Down (0x10/0x40; holding auto-repeats) change it,
Cross/Square close and bid, Circle cancels. Bids are in units of 100 Gil, at least price + 100, at most 99,900 and the
funds. Errors: "You must bid at least 100 Gil higher than the current bid." (0xCF), "Not enough funds." (0xD0, also
when gil < price + 100, without opening the window). State: `r38_auction_entry`, `r38_auction_bid` (both windows up:
Mini-Cid, price 1200).

### 6. Stellazzio handover (Treno, Queen Stella; disc 2)

```
kind field   disc 2   field 911 (Treno/Queen's House)   story 6960   event.13=0 event.14=0
item.304=1   (key item 0x30 Aries; Stellazzio are event item ids 304-316 = key items 0x30-0x3C, Aries..Ophiuchus)
event.355=0 event.356=0   (b[0x163..0x164]: coins already given, bit k = item 304+k)
```
Player at (-13696, 10231). Presses: hold **Up 60** (player about (-14373, 10266), inside region 6 facing Stella),
**Cross** → (first visit only: "I am collecting star-shaped coins called Stellazzio." 0x8B, Gbit 0xB17) → **"Oh, you
have the <coin>. Give it to me in return for a reward."** (0x8D) → **Cross** → **"Will you give me your <coin>? / Yes
/ No"** (0x8E, window 0) → Yes → "Oh, you are most kind!" (0x90), the coin is removed, bit set in b[0x163], the
reward depends on how many bits are set (13-way switch: gil via msg 0x3A, items via 0x38/0x39), "<Quack!...>" (0x91),
the servant brings it. With all 12 others given, the 13th gives 0x92-0x94. No counter window (the servant reports
"We have received N Stellazzio from you." 0x9B on Cross). Verified with the Iifa save, which held Virgo and Libra:
the script offered Libra (it offers the highest owned id), Yes → b[0x163] = 0x40, Libra gone. States:
`r38_stella_entry`, `r38_stella_choice` (Yes/No up).

### 7. Chocobo Hot and Cold points window (disc 3) - partly

R37 recipe 1b plus `event.13=0 event.14=0`: the "Error Env Play() Slot=0" box is gone and the How-to page shows at
once (`r38_hotcold_entry`); **Select** → timer **1:00** (`r38_hotcold_running`). The 9 treasure objects are uids
0x80-0x88 (kind 4, model 241, e.g. 0x87 at (-235, -4088), 3 Up presses from the start). Choco moved onto 0x87 (21
units away), **Square** → the dig animation starts, the timer keeps running, but no points window (msg 0x12D) and no
second dig: `Fs8[0x23]` (dig request) stays 0 and Square is ignored afterwards; `EvtState.playerUid` became 4 (the
hidden controller object of entry 4). So the missing sound slot was not the cause of R37's hang. Not solved; next
step would be to trace entry 4 tag 1 from 0x29D (distance tests `distToObj(0x80+k)`, `CALL 2, 0x80+k, 0x27/0x28`) to
the treasure's tag 41, which prints 0x12D at 0xD54/0xDA6/0xE14 and reads Cross/Square to continue.

### 8. Others (static pointers only)

- Nero brothers' shell game: disc 3 Alexandria (area 90, fields 1850-1866): "Hey, it's Zidane! Let's play our game!"
  (0xEC-0xEE), "First, pay us N Gil. / Pay / Don't pay" (0xEF-0xF1), "Which one is Zenero/Benero/Genero?" (0xF6-0xFB).
- Treno card tournament: area 70 (fields 900-916; 903 Card Stadium), "Step inside, and the tournament begins!" (0x6D).
- Gargant: area 359 (fields 950-955, disc 2, Gargan Roo, Tot's lever lines 0x6C-0x81); area 289 (1200-1205, "ride the
  gargant downstairs").
- The Ragtime Mouse quiz and the Ipsen mirror puzzle were not located in this pass.

## Details

### Jump rope (field 103, entries 8, 9, 29-37)

- Entry 8 (girl, uid 8) tag 3 = Cross talk: sets `Fu8[0x29]` (game state) 0 → 1, msg 0xE9, walks to the rope, then
  `CALL_WAIT_FREE 4, 0x1E, 12` runs the **player's** tag 12 (entries 29-37 are the player-character variants; the
  player object is uid 0x1E). Then it waits on the count `Fs24[0x2B]` crossing each threshold and prints the
  comments; speed changes at 20 and 50 (`Fu8[0x2E] += 2`, jump height `Fu8[0x2F]--`), 100 (`+= 1`), 200
  (`Fu8[0x30] = 1`: speed alternates +-4 per jump), 300 (speed 14, height 6). First-time rewards guarded by Gbit
  0xECF/0xECE/0xECD/0xECC/0xECB (b[0x1D9] bits 7..3). At 1000 jumps and no item 282: ADD_ITEM 0x11A (key item 0x1A).
- Entry 9 tag 1 (rope, runs every frame): `Fs16[0x25]` = rope phase, += `Fu8[0x2E]` per frame, wraps at 160.
  While state 4 and phase < 80 and not yet jumped: "!" bubble while phase < 72, and `padPressed(0xA0000)` sets
  `Fu8[0x2A] = 1` (jumped). A second press while `Fu8[0x2A] == 1` clears it again (double press = miss).
- Player tag 12: state 2, msg 0xEA, waits for the first press (state 3), then each time the phase crosses 88
  (`Fs16[0x27] < 88 && Fs16[0x25] >= 88`): if jumped → state 4, `Fs24[0x2B]++`, MSG_NUMBER 2 = count, msg 0xEB, jump
  animation; else state 5 (fail). Tag 13 stores the top three scores in Gs24[0x1F0]/[0x1F3]/[0x1F6] (b[0x1F0..0x1F8];
  "Jump-rope Champions" board msg 0x15C in field 106 reads them).
- Runtime (`r38_jumprope_ready`, `build/3ds8/jumper.py`): 12 jumps → "12 times" + "It's gonna get faster after 20!";
  no press → state 5, "Come play with us again!", the save's board 21/13/10 became 21/13/12.

### Hippaul race (field 1850, disc 3)

- Entry 7 (Hippolady) tag 3: choice 0x90; sets positions, msg 0x93, calls tags 15/16 (mark/set) of the player
  (`0xFA`) and Hippaul (uid 8), msg 0x97 + window 5 msg 0x94, `Fu8[0x26]` (finishers) = 0, `Fu8[0x27]` (winner) = 0,
  `CALL 4, 0xFA, 17` and `CALL 4, 8, 17`, waits for `Fu8[0x26] >= 2`, then rewards by Hippaul's level b[0x1EB]:
  levels 10, 20, ..., 70 each give a one-time reward (Gbit 0x1DA7..0x1DA1 = b[0x3B4] bits 7..1; "My son's in better shape, too. Here, take this." 0x9B), level 80 item 283 (if not owned); otherwise "Good job!" (0x9A).
- Player tag 17 (entries 15 onward, one per party leader): `Fu8[0x23]` = last button (1 Circle, 2 Square); a press of the other
  button sets `Fu8[0x24] = 30`; per frame `z += Fu8[0x24]`, `Fu8[0x24] -= 5`; `Fu16[0x2D]`/`[0x2F]` count frames each
  button is held, `Fu16[0x31]` counts presses made while both are equal. At z >= 5000: winner 1 if first.
- Hippaul tag 17: speed starts 1280/128 per frame, accelerates by `3 + level/5` up to `(20 + level) * 70` (level < 20)
  or `2800 + (level - 20) * 5`; finish z 4970. If Hippaul loses, level += 1, or rounds up to the next multiple of 5
  when the player won by more than 30 frames (and `Fu16[0x31] < 52`); max 100.

### Frog catching (field 657)

- Entry 0 tag 0: `Fu8[0x40] = 99` (frog mode request) if `isInParty(5)` (`evt_isInParty`, `event.c:3855`) and
  `(event.2 & 0xFF0) == 0x60` or event.2 == 8370, and b[0x18F] > 0. Pond select = event.2 & 0xF000 (0x1000..0x8000 →
  populations b[0x311..0x314], [0x315..0x318], [0x319..0x31C], [0x31D..0x320], one nibble per spawn slot; masks
  b[0x30C..0x30F]; regrowth time from play time b[0x1AA..] vs b[0x300..]).
- Entry 0 tag 1: msgs 0x163/0x164, choice 0 → `Fbit[0x265] = 1` (frog mode). Entry 3 tag 1 shows/hides window 6
  msg 0x183 with `MSG_NUMBER 1, sys.16` while `Fbit[0x265]`.
- Frog entries 6-9 tag 3 (Cross): choice 0x182; Keep → op 0xE0, MSG_NUMBER 1 again, clears the frog's bit in
  b[0x1A2] and its population nibble.

### Sword fight (field 64)

- `Fu8[0x18]` is the scene state (entry 2 tag 1 switch); state 2 = the fight (entry 13 tag 1 case 2: resets the
  scores, msg 0x6A, waits for Cross/Square, msg 0x6F).
- Entry 20 tag 1 picks each prompt (`Fu8[0x2E]`, never the same as the previous one `Fu8[0x2C]`, some excluded by
  stage position), sets `Fu8[0x2F] = 1` (waiting), `Fu8[0x34] = 50` (or 30), opens 0x70+prompt.
- Entry 3 tag 1 (every frame while state 3): `padPressed(mask[k])` for the 8 buttons; the right one → `Fu8[0x2F] = 2`
  (success sound 0x67), a wrong one → 3 (sound 0x65); Start held → 3. `Fu8[0x34]--` per frame; at 0 → 3.
- Score: success → `Fs16[0x1E] += Fu8[0x34]` (frames left), `Fs16[0x20] += Fs16[0x28]` (combo), combo++; failure →
  combo = 0. `Fs16[0x2A]` = best combo. After 50 prompts (`Fs16[0x22]`) entry 4 tag 1 case 6: impressed =
  `(Fs16[0x1E] + Fs16[0x20]) / 29` clamped 1..100 (best kept in b[0x1DB]); gil = `(Fs16[0x1E]/5 + Fs16[0x20] +
  2*Fs16[0x2A] + 2*Fs16[0x28]) / 2 + 1`, 10000 when impressed = 100; best combo >= 50 sets Gbit 0xEE7 (b[0x1DC]
  bit 7, "Queen Brahne was quite impressed").
- Runtime (`build/3ds8/canary.py` answers each prompt from `Fu8[0x2E]`): 10 prompts in 600 vsyncs, combo 10; 50 with
  one wrong answer at 20: time score 2065, combo score 596, best combo 29.

### Auction (field 909)

- Entry 10 tag 1 (from story 4435): calls the auctioneer's tag 29 (present an item) and a random bidder's tag 30
  (entries 11-15, which set `Fu8[0x39]` = 1..5 = current top bidder and raise the price `Fs24[0x1B]`).
- Region 16 tag 3 (Cross, facing the auctioneer, while `Fu8[0x39] != 0`): `CALL_SYNC 2, 10, 31`.
- Entry 10 tag 31: MSG_NUMBER 0 = item (`Fs16[0x2D]`), 1 = price, 4 = gil; if gil >= price + 100 the bid entry loop
  (`Ls16[0x4]` = bid / 100, `Ls8[0x1]` = digit 0..2, `Ls8[0x0]` = hold-repeat counter). Accepted bid → `Fs24[0x1B]`,
  `Fu8[0x39] = 0` (player is the top bidder).

### Stellazzio (field 911)

- Region 6 tag 3 (Cross facing Stella) → entry 3 tag 17: scans items 304..316 (`itemCount`), keeps the last one held
  in `Fu16[0x19]`; choice 0x8E; Yes → REMOVE_ITEM, `Gu16[0x163] |= 1 << (id - 304)`, counts the bits (13 max) and
  switches on the count for the reward.

### Tools (scratch, build/3ds8/ in the worktree; worth promoting: fmsg.py, textgrep.py, fieldnames.py)

- `fieldnames.py --disc N`: field id → location name (event script header +0x2C, FF9 text), all fields of a disc.
- `fmsg.py FIELD [--disc N] [--ids 0x12D,..] [--grep RX]`: **static dump of a field's message table**. The table is
  DB item `0x60000 | area` of FF9.IMG dir 2 file `area` (`fld_findAreaItem`, `src/ovl_0a/fld_main.c:298`); area per
  field from `field_models.py --fields` (cached in `area_dN.txt`). Messages: `{u32 count; {u16 start, ?, u16 text, ?}
  [count]}`, offsets from +4.
- `textgrep.py RX --disc N`: grep every area's messages.
- `padscan.py --disc N`: per field, count of pad expression ops, timer ops and MSG_NUMBER ops (finds minigames).
- `jumper.py`, `canary.py`, `mash.py`: auto-players for 1, 4, 2 (read the field vars, press via `Bridge.hold`).
- `objaddr.lua`: uid → EvtObject address (`tools/emu.py lua -f`).
- Teleporting the player by writing EvtObject +20/+28 breaks its walking (it stops moving), so the recipes use
  presses. Field arrival codes (event.2) select the start position per field (switch at the top of the player entry's
  tag 0).

## Proposed names (script variables; byte offsets into g_evtGlobalVars; not applied)

- `EVT_GB_ENV_SOUND0_ID 0x09` (s16), `EVT_GB_ENV_SOUND1_ID 0x0B` (s16), `EVT_GB_ENV_SOUND0_STATE 0x0D`,
  `EVT_GB_ENV_SOUND1_STATE 0x0E` (0 none, 1 start, 2 playing, 9 error → msg "Error Env Play()"). high (every field
  init, runtime).
- `EVT_GB_JUMPROPE_BEST 0x1F0` (3 x s24, top three), `EVT_GBIT_JUMPROPE_REWARD 0xECB` (..0xECF). high.
- `EVT_GB_CANARY_BEST 0x1DB` (u8, nobles impressed), `EVT_GBIT_CANARY_QUEEN 0xEE7`. high.
- `EVT_GB_HIPPAUL_LEVEL 0x1EB` (u8 0..100), `EVT_GBIT_HIPPAUL_REWARD 0x1DA1` (..0x1DA7). high.
- `EVT_GB_FROG_UNLOCK 0x18F` (u8, > 0 enables Quina's frog prompt), `EVT_GB_FROG_POP 0x311` (4 ponds x 4 bytes,
  nibble per slot), `EVT_GB_FROG_LEFT 0x1A2` (u16, frogs on screen). medium (names from the code paths).
- `EVT_GB_STELLAZZIO_GIVEN 0x163` (u16, bit k = item 304 + k). high.
- Field-bank (per field, not global): jump rope `Fu8[0x29]` state, `Fs24[0x2B]` count, `Fs16[0x25]` rope phase;
  canary `Fu8[0x2E]` prompt, `Fu8[0x2F]` answer state, `Fu8[0x34]` frames left; race `Fu8[0x26]`/`[0x27]`; auction
  `Fs24[0x1B]` price, `Fu8[0x39]` top bidder. Field 657 `Fbit[0x265]` frog mode.
