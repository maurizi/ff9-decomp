# R37: scenario recipes for the 3DS port (world-map vehicles, chocobo digging, Hot and Cold, Moogle choice, Blackjack)

Sources: the world scripts (FF9.IMG dir 3, disassembled with a scratch wrapper around `tools/evtscript/edis.py`,
`build/3ds7/wdis.py`), the field scripts of Chocobo's Forest (2950) and the ending field (3011), the matched C (line
numbers at commit 0d495035), and PCSX-Redux runs on discs 3 and 4. Background: R30b, the constants pass and R37 world
map constants in `docs/runtime-findings.md`.

Notation: `event.N` = `g_evtGlobalVars.b[N]` = main 0x8007AEF0 + N (decimal N, hex in brackets). `story` = h[0]
(b[0..1]). `Gbit[k]` = bit k&7 of b[k>>3]. `gamedata.chocograph` is **not** an event byte: it is
`GameData.chocograph` (+0x844, 0x8008BA94 at runtime; `include/ff9.h:395`), 1-24, 0 = none. Multi-byte event values
are little-endian. World ids are 9000 + FF9.IMG dir-3 file index. Pad names are the PlayStation buttons (Square is the
pink one shown as "dig").

## Short recipes

All world recipes leave `event.2` (b[2..3], the arrival code) at 0 unless stated: with 0 every world script puts the
walker, the chocobo and the vehicle at their default spot (see "Positions" below), so no position bytes are needed.
Add `event.100=1` (b[0x64], the navi mode `wm_naviInit` reads, `tu_800BA528.c:1457`) to get the minimap at entry; the
saves used here had 1, a zeroed harness state starts with the navi map off (Select turns it on).

### 1a. World map riding the chocobo, chocograph selected (dig/hint UI live)

```
kind world
disc 3                     # any disc; 9007 is the disc-3 world (story 10400..11089)
world 9007
story 10400
event.190=5                # b[0xBE] move mode: 1-5 = riding (= chocobo ability level), 0 walk, 7 boat, 8 HG3, 9 Invincible
event.191=5                # b[0xBF] chocobo abilities (1 Field .. 5 Sky/gold); 0 = no chocobo actor at all
event.101=0x06             # b[0x65]: Gbit 0x32A (bit 2) chocobo on the map (else hidden), 0x329 (bit 1) chocobo met
event.187=0x01             # b[0xBB..0xBD] chocographs owned (bit k = chocograph k+1); bit 0 also unlocks the Triangle menu
event.184=0x00             # b[0xB8..0xBA] chocograph treasures dug up (bit k = chocograph k+1); leave bit clear to dig
gamedata.chocograph=1      # selected chocograph (GameData+0x844), set before the world init
```

Result (verified, disc 3): the party enters already riding the gold chocobo (player object = the chocobo, Zidane
attached), free to move, minimap bottom right and the selected chocograph's picture above it (the picture is loaded
once by the world init, `tu_800BA528.c:85`, so set the chocograph first). Presses:
- **Square** = dig. Away from the chocograph spot: a "Kweh?"-style bubble (message 0x3C), closer than the outer radius
  message 0x3D; inside the inner radius "K-KWEHHH!!!" then **"Found a treasure chest!"** (recipe 2).
- **Triangle** = chocobo choice window **"Open Chocograph menu / Feed him a Dead Pepper (Remaining: N) / Cancel"**
  (WM_REQ_MOG_CHOICE = 1 while it is up). Row 2 only with item 252 (Dead Pepper). Row 1 opens ovl_32
  (`menu_open(MENU_CHOCOGRAPH, 0)`, script op 0x75 5): "Select / Cancel", SELECTED picture, CHOCOBO ABILITY icons,
  Inventory / Loc. Found counts, chocograph list and HINT text. Needs `event.187` bit 0, otherwise message 0x35.
- Circle = get off (when a get-off cell is near).

### 1b. Chocobo Hot and Cold running (timer UI)

```
kind field
disc 3                     # Chocobo's Forest exists on every disc
field 2950
story 10400                # anything; the forest only adds story-gated scenes
event.2=2                  # b[2..3] = 2: "came back to play" (Mene's menu sets it before GOTO_FIELD 2950)
event.190=5                # riding (1..6); the game only starts on a chocobo
event.191=5                # >= 1
event.101=0x06             # Gbit 0x32A chocobo exists
event.177=0x80             # b[0xB1] bit 7 = Gbit 0x58F: first forest visit done (else the meet-Choco intro plays)
```

Result (verified, disc 3, from a Lv 99 save): the field loads straight into the minigame. A debug box "Error Env Play()
Slot=0" appeared in every forced entry (the forest's environment sound was not preloaded, which the real
GOTO_FIELD from Mene does with op 0xFD PRELOAD); Cross clears it. Then the page **"- How to play Chocobo Hot & Cold -
(D-pad): Move Choco / (Square): Dig / ... SELECT: Start  X: Next page"** (first time per field load). **Select** starts
the game: timer box top left **"1:00"** counting down (EVT_OP_TIMER_SET (60 / Fs8[0x24]) + 1, TIMER_ENABLE 1, TIMER_RUN
1; field 2950 entry 0 tag 22), Choco moves freely, **Square** digs: "Kweh." far, "Kweh!?" closer (8 hidden treasure
objects, entry 8 copies 0x80-0x87, placed per game). A dig on a spot plays the dig animation; the points window
(message 0x12D with the points number) was not reached in this run (the dig loop kept digging; probably the missing
environment sound). At 0:00 message 0x115 then the field reloads with `event.2=3` (results talk).
Real path: Mene's menu (field 2950 entry 9 tag 59) choice 0 needs riding (event.190 1..6), Gbit 0x43F (tutorial seen,
event.135 bit 7) and 60 gil; it takes 60 gil, sets event.2=2 and GOTO_FIELD 2950/2951/2952 by b[0x86] (1 Forest,
2 Lagoon, 3 Air Garden).

### 2. Chocobo digging: dig prompt / treasure found

Recipe 1a plus a position on the chocograph's spot (needs a non-zero arrival code so the scripts keep the stored
chocobo position):

```
kind world
disc 3
world 9007
story 10400
event.2=1                  # non-zero: use the stored positions below instead of the defaults
event.190=5
event.191=5
event.101=0x06
event.187=0x01
event.184=0x00             # chocograph 1 not dug yet
gamedata.chocograph=1
event.83=0x7a event.84=0x07 event.85=0x04      # b[0x53..0x55] chocobo x = 264058 (s24)
event.86=0x18 event.87=0xfc                    # b[0x56..0x57] y = -1000 (s16; snaps to the ground)
event.88=0xe7 event.89=0x19 event.90=0xfc      # b[0x58..0x5A] z = -255513 (s24)
event.91=0                                     # b[0x5B] direction
```

Press: **Square**. Result (verified): "K-KWEHHH!!!" (~40 frames), then **"Found a treasure chest!"**; a chest appears
under Choco; each Cross shows one item: "Found 2 Elixir!", "Found 3 Hi-Potion!", "Found 4 Ether!", "Found 2 Germinas
Boots!" (chocograph 1 "Streamside"); event.184 bit 0 becomes set.
Dig spots (world chocobo entry 5 tag 14, switch on `gamedata.chocograph`): x, z, outer hint radius, find radius:
1 (264058, -255513, 5000, 1400), 2 (211893, -276644, 5000, 900), 3 (326027, -214042, 5000, 1300),
4 (216927, -183851, 4000, 1000), 5 (269481, -90418, 4000, 900), 6 (122636, -60499, 4000, 1300),
7 (118881, -110209, 5000, 1100), 8 (72759, -169520, 5000, 1000), 9 (212248, -113183, 5000, 1100),
10 (196847, -246483, 4000, 1200), 11 (281226, -269032, 4000, 1000), 12 (341553, -273315, 4000, 1100),
13 (271789, -211289, 5000, 900), 14 (295609, -178947, 5000, 600), 15 (93785, -216446, 5000, 700),
16 (43790, -134953, 5000, 800), 17 (142941, -193693, 5000, 1200), 18 (43915, -206088, 8000, 1400),
19 (118343, -31787, 5000, 1100), 20 (345962, -140711, 8000, 1600), 21 (336288, -55950, 3400, 700),
22 (290461, -54813, 4300, 800), 23 (176301, -102275, 3800, 450), 24 (111343, -124738, 3900, 600).
The find test is `distTo(x, z) <= find radius` (message 0x3E, then 0x3F "Found a treasure chest!" and the chest
objects), between the radii message 0x3D, else 0x3C. Other dig outcomes (`event.190` 1-5, Square): no chocograph
selected → messages 0x36-0x3B (status lines; 0x38-0x3B depend on b[0xB8] == 0xFFFFFF and Gbit 0x354), already dug
(bit set) → the same status lines.

### 3. Airship, free flight, navi map and destination UI

Hilda Garde 3:
```
kind world
disc 3
world 9007                 # the airship actor exists in 9007 only from story 10400
story 10400
event.190=8
```
Invincible:
```
kind world
disc 4                     # verified; disc 3 hung in the world init (story >= 11090 loads the disc-4 world data)
world 9008                 # actor needs story >= 11100
story 11100
event.190=9
```

Result (verified both): the airship is the player at its default spot (238344, -1055, -253512), hovering, minimap on
(event.100 = 1, the navi mode the world init reads). Presses:
- **Select** cycles the navi map: minimap → full map ("navi full", a parchment world map with place dots and the ship
  cursor) → off → minimap.
- In the full map: D-pad moves the cursor, **R1/L1** jump to the nearest/next known place; the place name shows top
  left (e.g. "Qu's Marsh", "Lindblum Harbor"; event.103 = place + 1). **Cross** on a place: **"Go to Qu's Marsh? / Yes /
  No"** (airship modes 8/9 only); Yes = autopilot (WM_REQ_AUTO_PILOT_ON), the map closes and the ship flies there.
- In the full map **Triangle**: **"Select navigation method: / Airship Navigation / Autopilot Navigation / Cancel"**.

### 4. Boat (Blue Narciss)

```
kind world
disc 3
world 9003                 # the disc-2/3 world (story 5990..10399); the boat actor needs story >= 9400
story 9400
event.190=7
```

Result (verified): the Blue Narciss is the player at (227812, 280, -254931) by a beach, Zidane aboard. It has **no UI
of its own**: Select shows the same navi map (minimap / full map with place names), but Cross on a place does nothing
(the "Go to ...?" prompt is airship-only), Triangle opens the normal main menu, Square does nothing. D-pad
left/right steer (WM_CTRL_STEER).

### 5. World-map Moogle choice (WM_REQ_MOG_CHOICE)

```
kind world
disc 3                     # any world map with the type-11 moogle actor (all main world files)
world 9007
story 10400
event.190=0                # on foot
```

Press **Square** (walking, no event running). Result (verified): the moogle "Moguo" flies in from above (~250 frames)
and the window **"Moguo / "Can I help you, kupo?" / Save / Tent / Cancel"** opens. Save → **"Moguo / "Save, kupo?" /
Save / Don't save"** → MENU 4 (save screen). Tent uses item 253.
WM_REQ_MOG_CHOICE itself (request 35 of op 0xC4, `w_frame.c:359`, `ovl_0c.h:43`) only sets `g_worldMogChoice`
(0x800CF440); while it is non-zero `wm_frameMain` reports busy to `pad_setFlags(0x8000, busy)` (`w_frame.c:139-142`).
The world script sets it to 1 around every script choice window, not only the moogle's: the moogle menu (entry 6 tag
25, cleared when the moogle leaves, tag 38), the chocobo Triangle menu (1a), the airship "Go to ...?" prompt (3) and
the navi "Select navigation method" window. Runtime: 1 in all four screens above, 0 after they close.

### 6. Blackjack (ovl_15)

How the game gets there: the last field of the game, **field 3011** (disc 4) entry 7 tag 1, does `GOTO_FIELD 16000`
(FLD_NEXT_ENDING, `ovl_0a.h:221`) → `fld_main` exit reason FLD_EXIT_ENDING (`fld_main.c:43-45`, `:473`) →
`GameState.mode = GS_MODE_ENDING` (4) → main dispatcher loads ovl_15 → `ending_main` (`ending.c:3`): FMV059 (the
credits movie, `ending_playMovie`, `blackjack.c:3`, only played when disc 4 is in: `blackjack.c:16`) → **"THE END"**
(`ending_theEnd`, `the_end.c:3`). There the 16-button code (`g_endingSecretCode`, pad bits, `the_end.c:97-111`):
**R2 L1 R2 R2 Up X Right Circle Down Triangle L2 R1 R2 L1 Square Square**, then **hold Start** (a confirm sound plays
after the 16th button) → 90-frame white fade → `bj_main` (`blackjack.c:631`). A wrong button resets the count.

Direct recipe (verified on disc 4):
```
disc 4
from any loaded field: GameState.mode = 4 (GS_MODE_ENDING) at the field exit (port: run the ending mode directly)
(optional) skip FMV059: return 0 from ending_playMovie(4, 0x12) at once (it is ~10 minutes)
then presses: R2 L1 R2 R2 Up Cross Right Circle Down Triangle L2 R1 R2 L1 Square Square, hold Start ~10 frames, wait ~200 frames
```
No story or event values are read on the way (the ending ignores the save data). Result: **wager screen**: a card
shoe top, "BANKROLL 1000 / WAGER 10" with the hand cursor on the wager, "STAND / HIT / DOUBLE / SPLIT" greyed below.
Right/Left change the wager by 10 (10..1000, at most the bankroll; `bj_wagerInput`, `bj_wager.c:34`), **Cross** deals.
~150 frames later the **action screen**: dealer hand top (one card face down), player hand bottom with its total
("16"), cursor on **STAND / HIT / DOUBLE / SPLIT** (`bj_actionInput`, `bj_action.c:35`; DOUBLE/SPLIT grey when not
allowed). Hit to 26 showed "LOSE" over the hand and Bankroll 990. When the shoe's cut card comes out the round ends
and `ending_main` goes back to THE END.

## Details

### World entry used for the runtime checks

`build/3ds7/wm_force.py WORLD --disc N --poke ADDR=HEX ...` (scratch): load a field state, force the field exit to main's
dispatcher 0x80012834 (as `tools/rt/enter_field.py`), write the world file's sector/length into
`GameData.sceneSector[1]/sceneSectors[1]` (+0x800/+0x808), **`GameData.worldId` (+0x7DC) = WORLD and `worldAreaId`
(+0x7DE) = 68**, apply the pokes, set GameState.mode 3. `wm_load(worldId)` (`wm_main.c:230`) looks the event script
up by **worldId**, not by the sector: without the worldId write only the save's own world (9007 in the disc-3 save)
loaded, and 9000/9003 started with no script (only object uid 0, ip 0). This is also why R30b's
`wm_enter.py --world 9002` stalled. Every world file's dir-3 entry links area 68.

### Which world id the game loads

Field exits pick the world file by story and the exit's arrival code (field 2950's exit code, entry 20 tag ~0x186):
story < 5990: 9000 (default) / 9001 / 9002 / 9009-9011; 5990..10399: **9003** (default), 9004, 9005 (also
9615..9790 for some exits), 9006, 9009, 9012; 10400..11089: **9007**; >= 11090: **9008**; 9009 and 9012 are special
(9009 sets the 5-ability chocobo and many story bits). World init (`wm_systemInit`, `w_frame.c:28`) uses the disc-4 data
set from story 0x2B52 (11090).
Actors per world (entry 0 tag 0 of each world script): chocobo entry 5 if event.191 != 0 (9000, 9003, 9005, 9007, 9008,
9009); Blue Narciss (type 8) in 9003 from story 9400 (9006 has eight type-8 boats, a scripted scene); Hilda Garde 3
(type 9) in 9007 from story 10400; Invincible (type 10) in 9008 from story 11100; moogle (type 11) in all main files.

### Positions (world scripts)

| object | stored at (event bytes) | default when event.2 == 0 (9007) |
|---|---|---|
| walker | b[0x40..0x48] = event.64-72 (x s24, y s16, z s24, dir u8) | (238344, -985, -253512), dir 77 |
| vehicle (boat/airship) | b[0x4A..0x52] = event.74-82 | 9007/9008: (238344, -985, -253512), dir 77; 9003 boat: (227812, 200, -254931), dir 186 |
| chocobo | b[0x53..0x5B] = event.83-91 | (243177, -1039, -251993), dir 77 |

The scripts copy the player's position back into these bytes every frame (e.g. chocobo entry 5 tag 1), so a saved
game resumes where it was. With event.190 1-6 / 7 / 8 / 9 the chocobo / boat / HG3 / Invincible init does `SET_PLAYER`
and attaches Zidane (op 0x4C), so the party starts riding.

### World script pad handling (entry 3 tag 1 of the main world files)

Script pad masks are `g_padPressed` bits (`ff9.h:2125`): 0x10000 Circle, 0x20000 Cross, 0x80000 Square,
0x1000000 Triangle, 0x2000000 Select, 0x800000 R2. Walking: Square = call the moogle (Fu8[0x1E] = 1, entry 2 then
CALLs the moogle's tag 25), Cross next to the chocobo = ride (chocobo entry 5 tag 2). Riding: Circle = get off
(WM_REQ_GET_GETOFF_POS), Square = dig (Fu8[0x25] = 3/4/5), Triangle = chocobo menu. Any mode: Select = navi mode
cycle (WM_REQ_NAVI_MODE 1/2, b[0x64]). Full navi map: Triangle = navigation method window, Cross on a place with
event.190 8/9 = "Go to ...?" (message 4), Select = close.

### Runtime evidence (PCSX-Redux)

- Disc 3, `cpass_wm_hadris` (card 1 FILE03, world 9007, story 0x28A0): moved the gold chocobo next to Zidane, Cross →
  event.190 = 5 (`r37b_wm_chocobo_ride`). Triangle → chocobo window, `g_worldMogChoice` = 1 (`r37b_wm_chocobo_choice`);
  Cross → ovl_32 resident, Chocograph menu with 24 owned. Chocograph 1 poked (GameData+0x844 = 1, event.184 = 0xFE),
  chocobo teleported to (264058, -255513), Square → "K-KWEHHH!!!", "Found a treasure chest!" (`r37b_wm_dig_found`),
  Cross x4 → the four items.
- `wm_force.py 9007 --disc 3 --poke 0x8007AEF0=a0280100 --poke 0x8007AFAE=0505 --poke 0x8007AF55=06 --poke
  0x8008BA94=01000000 --poke 0x8007AFA8=feffffffffff --poke 0x8007AF43=7a070418fce719fc00` from
  `disc3_pandemonium_lv99_field`: riding at the spot, the stream chocograph picture top right; Square → K-KWEHHH!!! →
  Found a treasure chest! (state `r37b_wm_chocograph1_spot`). Same with event.2 = 0: riding at (243177, -1023,
  -251993).
- `wm_force.py 9007 ... --poke 0x8007AEF0=a0280000 --poke 0x8007AFAE=08` → HG3 player, mode 8 (`r37b_wm_hg3_entry`);
  Select → full map; R1 → "Qu's Marsh" (event.103 = 0x16); Cross → "Go to Qu's Marsh?" (`g_worldMogChoice` 1). From
  `cpass_wm_airship9`: R1 → "Lindblum Harbor", Cross → prompt, Yes → map closes, ship flies (autopilot)
  (`r37b_wm_airship_navi_full`, `_navi_place`, `_goto_prompt`).
- `wm_force.py 9003 ... --poke 0x8007AEF0=b8240000 --poke 0x8007AFAE=07` → uid 6 type 8 = player, mode 7, Blue Narciss
  at the beach (`r37b_wm_boat_entry`); Select/R1 → "Lindblum Dragon's Gate" shown, Cross → nothing, `g_worldMogChoice`
  stays 0; Triangle → main menu.
- Disc 4, `wm_force.py 9008 --disc 4 --poke 0x8007AEF0=5c2b0000 --poke 0x8007AFAE=09` from `ds_perf_disc4_treno` →
  Invincible (type 10) player, mode 9, fog-dark disc-4 world (`r37b_wm_invincible_entry`); full map, R1, Cross → "Go to
  Qu's Marsh?"; full map Triangle → "Select navigation method:" (`g_worldMogChoice` 1). The same on disc 3 froze in the
  world init (ovl_0c and ovl_64 resident, old frame on screen).
- Moogle: `cpass_wm_hadris`, Square → `g_worldMogChoice` 1 at once, the window after ~250 frames
  (`r37b_wm_moogle_choice`); Cross → "Save, kupo?".
- Hot and Cold: `tools/rt/enter_field.py 2950 --disc 3 --poke 0x8007AFAE=05 --poke 0x8007AEF2=0200` (Pandemonium save:
  event.177 bit 7 and event.101 bit 2 already set) → "Error Env Play() Slot=0" box, Cross → How to play page
  (`r37b_hotcold_howto`), Select → timer 1:00 (`r37b_hotcold_running`), Square → "Kweh." / "Kweh!?" bubbles.
  Field vars there: Fs8[0x22] = 1 (game on), Fs8[0x23] dig request, Fu16[0x2D] = 30000 at time-out. With
  event.2 = 0 instead, riding: Zidane "Where's Mene...?", then Mene's Lagoon story scene (`r37b_forest_mene_back`).
- Blackjack: disc 4, `build/3ds7/bj_force.py` (from `ds_perf_disc4_treno`: field exit, mode 4 at the dispatcher, break
  at `ending_playMovie` (0x800A7378, args 4, 0x12) and return 0) → "THE END" + "(c)2000 SQUARE" (`r37b_the_end`);
  the 16 presses → `g_endingCodePos` (0x800AE3EC) = 16; hold Start → after ~200 frames the wager screen, Bankroll
  1000 / Wager 10 (`r37b_bj_wager`); Cross → dealt 6+K = 16 against K + hole card, cursor on STAND (`r37b_bj_action`);
  Down, Cross (Hit) → 26, "LOSE", Bankroll 990.

States: disc 3 (`SLUS01296`): `r37b_wm_chocobo_ride`, `r37b_wm_chocobo_choice`, `r37b_wm_dig_found`,
`r37b_wm_chocograph1_spot`, `r37b_wm_hg3_entry`, `r37b_wm_airship_navi_full`, `r37b_wm_airship_navi_place`,
`r37b_wm_airship_goto_prompt`, `r37b_wm_boat_entry`, `r37b_wm_moogle_choice`, `r37b_hotcold_howto`,
`r37b_hotcold_running`, `r37b_forest_mene_back`; disc 4 (`SLUS01297`): `r37b_wm_invincible_entry`, `r37b_the_end`,
`r37b_bj_wager`, `r37b_bj_action`.

### Proposed labels (not applied)

- `g_worldMogChoice` (0x800CF440): keep the name (Square's "Mog choice"), but its comment should say "a world-script
  choice window is open (moogle, chocobo menu, airship destination, navi method); wm_frameMain reports busy".
  high (runtime, four windows).
- Event bytes for docs/headers: `EVT_GB_WM_MOVE_MODE 0xBE` (= g_worldMoveMode, already noted in R30b),
  `EVT_GB_CHOCOBO_LEVEL 0xBF`, `EVT_GB_CHOCOGRAPH_FOUND 0xB8` (u24), `EVT_GB_CHOCOGRAPH_OWNED 0xBB` (u24),
  `EVT_GB_WM_WALKER_POS 0x40`, `EVT_GB_WM_VEHICLE_POS 0x4A`, `EVT_GB_WM_CHOCOBO_POS 0x53` (x s24, y s16, z s24, dir u8),
  `EVT_GB_NAVI_MODE 0x64` (wm_naviInit reads it), `EVT_GB_WM_FILE 0x66` (world file index, set by each world init),
  Gbit 0x32A chocobo on the world map. high (scripts + runtime) except the Gbit 0x329 meaning (medium).
