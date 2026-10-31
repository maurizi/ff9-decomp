# R32c: answers for the 3DS port (how shopkeepers and save points reach MENU)

Answers from the matched C (paths relative to the repo, line numbers at commit 3e4ea0c9), the field scripts on all
four discs (dir 4 of `FF9.IMG`, DB item `0x50000 | fieldId`) and PCSX-Redux runs on disc 1 (fields 354 and 355).
Script layout and the general instruction encoding are in `docs/r32/3ds-requests.md` section 2.

## Short answers

1. **In field 354 there is no hand-off: the premise is inverted.** Entry 3 is the shopkeeper (model 55, "Shopkeeper
   Eve"), and its own talk function (tag 3) runs `75 00 02 00` = MENU shop 0 (Dali weapon shop, "Weapons"). Entry 11 is
   **Zidane** (model 98, `SET_PLAYER` 0x2C, `playerUid = 11`). Entries 11-19 are the nine party-character entries that
   every field script ends with (the last 9 entries, `evt_execInstr_newactor(r + g_evtScriptCount - 9, 0)`,
   event.c:3411). Entry 7 is the later-story variant of the shopkeeper: entry 0 creates actors 11, 3, 6 when the story
   counter (global u16 0) is < 0x2B52, and 11, 7, 8 from then on. Entry 7's talk function also has MENU shop 0. At
   runtime the object list is ordered 0, 1, 2, **11**, **3**, 10, 9, so the 4th object is the player and the 5th the
   shopkeeper. Read the entry from `EvtObject +6` (`scriptIdx`); `+7` is the uid. A port that labels objects by list
   position will swap them.
   The hand-off does exist in other shops (17 of 58 shop triggers): the counter is a **region** whose talk function
   (tag 3, Cross pressed inside the polygon) calls the shopkeeper actor's function, and that function holds the MENU.
   Example, field 355 (Dali item shop): region 16 tag 3 runs `14 00 02 05 12` = **CALL_SYNC** (op 0x14), level 2,
   object uid 5, function tag 0x12. The shopkeeper (uid 5) then runs its own tag-18 function as itself, with its own
   ip and stack, starting on the next scheduler pass, and that function runs `75 00 02 19` (MENU shop 25). The region
   blocks (`wait = 0xFF`) until that function returns. Call-op layout:
   `op, flags, level, uid, tag`. `flags` bit 0 = level is an expression, bit 1 = uid is an expression. The tag byte is
   always a literal (read with `*ip++`, not through the flags). Details below.
2. **Recipe:** start from every object created by `09 entry uid` (actor) or `08 entry uid` (region). Take its tag 3
   (and tag 8; for regions also tag 2). Disassemble that function properly, not by byte search. A MENU there is the
   answer. For each `10/12/14 flags lv uid tag` in it, map uid to entry and repeat in that function (depth 2 is enough on
   all discs). A disc-wide run of this recipe finds **58 shop triggers in 51 fields** (shop ids 0-39), **57 Moogle save
   points**, and 9 Moogle shops. It misses: 7 Memoria save spheres on disc 4 (2901, 2905, 2909, 2913, 2916, 2919, 2925),
   which are code objects that poll the player's position in their main loop (tag 1); one shop that opens from a region's
   range function (2108, region 6 tag 2); and two dead entries that no op ever creates (1213 entry 5, 1309 entry 6).
   The full table is below.
3. **Yes.** The second byte of every flags-carrying op is the expression mask: bit i set means argument i is an
   expression ending in 0x7F. MENU is `75 flags type arg`: type u8 (bit 0), arg u8 (bit 1), then
   `menu_open(type, arg); evt_onMenuOpen(); return 1`. Neither `75 04 20 00` nor `75 01 05 D9` is a MENU instruction.
   Both are false hits of a byte search. `75 04 20 00` is the last jump offset (0x0475) of a SWITCH table in entry 3
   tag 1, followed by MSG (0x20) with flags 00. `75 01 05 D9` is the text id 0x0175 at the end of `1F 00 02 80 75 01`
   (MSG_WAIT window 2, text 0x175) in entry 13 tag 1, followed by an EXPR `05 D9 ...`. Over all four discs every real
   MENU has flags 00, except the Moogles' `75 02 02 D2 01 7F` (flags 02: the shop id comes from an expression).
4. **There is no per-field shopkeeper or shop table.** Which object opens which shop exists only in the event scripts
   (the MENU arg). The shop id indexes the shop overlay's own tables: ovl_3c `s_shopItems[id]` (0x801FBC44, 32 item
   ids per shop, 0xFF ends) for ids < 0x20, and `s_shopRecipes[64]` filtered by `shopMask` bit `(id - 0x20)` for
   synthesis shops (ids >= 0x20, `SHOP_ID_SYNTH`, include/ovl_3c.h:11). Those tables are in ovl_3c's data, not on a
   per-field basis.

## 1. Details

### Call ops (main `evt_execInstr`, src/main/event.c:842; macros include/ff9.h:3839-3841)
All three are `op, flags, level, uid, tag` (5 bytes when flags = 0):
- level: `evt_getArg8` (flags bit 0). This is the priority the callee runs at. The call only happens when
  `level < target->level`. Idle objects are at level 7 (the main loop), talk functions at 1.
- uid: `evt_getObjectArg` (flags bit 1) → `evt_resolveObjectId`: 0xFF = the running actor itself, 0xFA = the player
  (`EvtState.playerUid`), 0xFB-0xFE = party slots 0-3, anything else = the object with that uid (list search).
- tag: one raw byte (`*g_evtCurObj->ip++`), never an expression.
- **0x10 CALL** (event.c:883): `evt_callObjectFunc(obj, level, tag, 0)`, fire and forget. A busy target (level not
  lower) silently drops the call. Returns 0, so the caller keeps executing in the same frame.
- **0x12 CALL_WAIT_FREE** (event.c:888): if the target is busy, it rewinds ip and retries next frame; otherwise the
  same as 0x10.
- **0x14 CALL_SYNC** (event.c:899): `evt_callObjectFunc(obj, level, tag, 1)`. The caller gets `wait = EVT_WAIT_CALL`
  (0xFF) and sleeps until the callee's RETURN (`evt_returnFromFunc`, event.c:149, clears it). It retries while the
  target is busy.
- 0x16 / 0x18 / 0x1A are the same three for "the object that called me": `op, flags, level, tag`, target uid =
  `CUR_UID()` (the caller uid saved in the top call frame, ff9.h:4178), event.c:910-934.

`evt_callObjectFunc(o, lv, key, save)` (event.c:211) does not run anything itself. It pushes the target's current ip
and `wait | level << 8 | callerLevel << 16 (or 0xFF0000 when save = 0) | callerUid << 24` on the **target's** stack,
then sets the target's ip to the function, its level to lv and its wait to 0. So the function runs as the target
object (its own `g_evtCurObj`, local variables, model, message window anchor). It is not a new thread (that is op
0x43 RUN_THREAD) and it does not run as the player. The target resumes what it was doing when the function returns.

The engine's own talk dispatch is the same call: `evt_checkPlayerAction` (event.c:2770) calls
`evt_callObjectFunc(nearest actor in talk range and within ±0x3FF of the player's facing, 1, 8 (Square) or 3, 0)`.
If no actor takes it, it calls the first region containing the player that has tag 3 (or tag 8).

### Field 354 (Dali weapon shop) bytes
Entry 0 tag 0 at +0x19A: `05 dc 00 7d 52 2b 18 7f` (story < 0x2B52), `02 0c 00`, then `09 0b 00 09 03 00 09 06 00`
(Zidane, shopkeeper, entry 6), else `09 0b 00 09 07 00 09 08 00`. uid 0 means uid = entry (`evt_createObject`,
src/main/evt_object.c:65).

Entry 3 (shopkeeper; tag 0 has `2f 00 37 00 3e` = SET_MODEL 55) tag 3, 216 bytes:
```
0000: 05 7a 02 7f                 EXPR sysvar 2 (control enabled)
0004: 03 01 00                    JMP_IF +1 -> 0008
0007: 04                          RETURN
0008: 05 c5 9e 7d 00 00 2c 7f     EXPR bit var = 0
0010: 05 c5 9f 7d 01 00 20 7f     EXPR bit var == 1
0018: 02 13 00                    JMP_IFNOT -> 002e
001b: 2d                          DISABLE_CONTROL
...                               (0x1c-0x2b: menu-button lock / wait)
002e: 27 00 7f                    BG_MASK_FILTER 0x7f
0031: 05 dc 00 7d ae 0b 19 ... 7f story checks (first-visit lines 0x1f0 / 0x251)
0084: 1f 00 06 80 49 01           MSG_WAIT window 6, flags 0x80, text 0x149 ("Welcome!")
008a: 47 00 02                    SET_MOVE_FLAGS 2
008d: 40 00 db 03                 ANIM 0x3db
0091: 54 00 02                    MSG_WAIT_WIN 2
0094: 22 00 04 / 0097: 22 00 03   WAIT 4, WAIT 3
009a: 75 00 02 00                 MENU type 2 (shop), arg 0  <- shop 0
009e: 22 00 03                    WAIT 3
00a1: 42                          ANIM_END
00a2: 40 00 dc 03                 ANIM 0x3dc
00a6: 47 00 07                    SET_MOVE_FLAGS 7
00a9: ... 2e ... aa               restore control (ENABLE_CONTROL, PAD_UNLOCK_MENU)
00d7: 04                          RETURN
```
Entry 7 (later shopkeeper, model 0x48) tag 3 is the same frame, plus `10 00 06 08 19` (CALL level 6, uid 8, tag 0x19).
That makes the NPC in entry 8 turn to the player and say text 0x238. Then MSG_WAIT 0x237, then `75 00 02 00` at +0x49.
The MENU is again in the shopkeeper's own function. Entry 11 (Zidane) has tags 0, 12, 1, 16-20 and no tag 3.

### Field 355 (Dali item shop): a counter region hands off to the shopkeeper
Region entry 16 tag 0: `29 00 06 ...` six-point polygon (579,-181) (568,-381) (360,-589) (-180,-559) (-420,-439)
(-475,-257). Tag 3:
```
0000: 05 7a 02 7f 03 01 00 04     return unless control is enabled
... 2d ...                        DISABLE_CONTROL
002e: 27 00 7f
0031: 05 dc 00 7d 4e 1b 18 7f     story < 0x1B4E ?
0039: 02 15 00                    JMP_IFNOT -> 0051
003c: 05 d9 1d 7d 02 00 2c 7f     field var 0x1d = 2
0044: 12 00 06 fa 0d              CALL_WAIT_FREE level 6, uid 0xFA (player), tag 13 (player turns to the counter)
0049: 14 00 02 05 12              CALL_SYNC level 2, uid 5, tag 18   <- hand-off
004e: 01 0a 00                    JMP -> 005b
0051: 12 00 06 fa 0e              (later story) CALL_WAIT_FREE player tag 14
0056: 14 00 02 08 12              CALL_SYNC level 2, uid 8, tag 18
005b: 04                          RETURN
```
Actor entry 5 (shopkeeper, model 604) tag 18:
```
0051: 05 d6 06 7d 01 00 2c 7f     local var = 1
0061: 51 00 fa 20 / 0065: 50      TURN_TO_OBJ player, WAIT_TURN
0066: 05 dc 00 7d ae 0b 18 7f     story < 0xBAE ?
0071: 1f 00 06 80 ed 00  / 007a: 1f 00 06 80 e1 01   greeting (story-dependent)
0080: 1f 00 06 80 ee 00           MSG_WAIT 0xee
0086: 05 7a 09 7d 00 00 20 7f     sysvar 9 (the choice) == 0 ?
008e: 02 0a 00                    JMP_IFNOT -> 009b
0091: 22 00 03
0094: 75 00 02 19                 MENU shop 25
0098: 22 00 03
...  (restores control) 012c: 04  RETURN
```

## 2. Static recipe and disc-wide results

Recipe (implemented as scratch `build/3ds3/evtdis.py` + `menuscan.py`; the argument widths come from parsing
`evt_execInstr`'s cases, corrected by hand for if/else and fallthrough cases, see "Disassembler notes"):
1. Disassemble every function of the field script linearly. Ops 0x01-0x03 `op s16`; 0x04 `op`; 0x05 `op expr`; 0x06
   `op n u16 default {u16 value, u16 ofs}[n]`; 0x0B `op u8 n, s16 base, u16 default, u16 ofs[n]`; 0x0D the same with u16
   n; 0x07/0x08/0x09 `op entry uid` (no flags); 0x1C/0x22 `op flags u8`; 0x30/0x31/0x6C/0x6D `op`; 0x32 `op flags u8 u8`;
   every other op `op flags args` (or just `op` when it reads no arguments); 0xFF prefixes a second page
   (`FF op2 flags args`). An argument with its flag bit set is an expression up to 0x7F (0x7D +2, 0x7E +4, 0x78 +2,
   0x79/0x7A/0x29/0x5F +1, bytes >= 0x80 +1 or +2 with 0x20). A byte search for 0x75 has false hits in switch tables
   and in 16-bit text ids (see answer 3), and the disc has one more (field 1602 entry 9: `06 04 75 00 04 00 ...`, a
   SWITCH default offset 0x0075 followed by case value 4).
2. uid → entry: `07/08/09 entry uid` with uid 0 meaning uid = entry. The player: 0xFA, or the entry that runs
   `2C` (SET_PLAYER); the party character entries are the script's last nine. 0xFF = the calling object itself.
3. Triggers: for each entry created by `09` (actor) take tag 3 and tag 8; for each entry created by `08` (region)
   take tag 3, tag 8 and tag 2. In each, collect `75` (MENU), and follow `10/12/14 flags lv uid tag` into
   (entry(uid), tag) (and `16/18/1A` back to the caller). A MENU found through a call belongs to the called object.
   That object is the one to mark (e.g. 355: the region at the counter triggers it, actor uid 5 owns the MENU).
4. MENU type 2: arg = shop id (`menu_open(2, id)`), < 0x20 buy/sell, >= 0x20 synthesis. Type 4 = save (arg 0).
   Other types seen: 0 main menu, 1 name entry (arg = character), 5 chocograph.

Complications:
- **Story branches.** Most talk functions hold several story variants, and fields create different shopkeeper entries
  by story (354: entry 3 or 7; 355: uid 5 or 8). The MENU belongs to whichever is instantiated. At runtime, check the
  object list (`EvtState+0x16D8` list, `EvtObject+6` entry) rather than guessing the story state.
- **Expression operands.** Only the Moogles use one: `75 02 02 d2 01 7f`, shop id = the Moogle's local s8 variable 1
  (var ref 0xD2 = local bank, s8, index 1). The Moogle's init (tag 0) sets it to -1 (no shop) or to a literal shop id,
  and its talk menu shows "Shop" only when it is >= 0 (`05 d2 01 7d 00 00 1b 7f` = `var1 >= 0`). Literal values: 764 →
  2, 1418 / 1421 → 27, 2203 → 22, 2250 → 14, 2360 → 29, 2504 → 16, 2655 / 2706 → 18; all other Moogles -1.
- **Entry indexes vs uids**: uid = entry unless the creating op gives a non-zero uid (e.g. field 100 `09 0A 80..82`
  makes uids 128-130 from entry 10). CALL ops name uids, so map them through the creating ops. Region hand-offs target
  actors by uid (5, 8, ...) and occasionally the field's main object (1503: region 10 → uid 0, entry 0 tag 19).
- **Not reachable from a talk trigger**: the Memoria save spheres (disc 4 fields 2901, 2905, 2909, 2913, 2916, 2919,
  2925) are `07` code objects whose main loop (tag 1) tests "control enabled and player inside an x/z box" plus a
  button, then runs MENU 4 at +0x141 (+0x143 in 2925). Field 2108's region 6 opens shop 36 from its range function
  (tag 2), which the recipe covers only by also scanning regions' tag 2. Two entries are never created: 1213 entry 5
  (a Moogle) and 1309 entry 6 (shop 6).

### Results (all four discs, field scripts deduplicated by content)
Moogles (actor, MENU save + optional Moogle shop in its own tag 3), 57 fields: 115, 150, 206, 253, 300, 306, 351, 407,
554, 602, 611, 662, 706, 764, 802, 810, 853, 904, 950, 1008, 1056, 1102, 1106, 1109, 1152, 1205, 1252, 1304, 1352,
1418, 1421, 1458, 1509, 1553, 1652, 1663, 1759, 1803, 1865, 1904, 2104, 2152, 2161, 2203, 2220, 2250, 2259, 2304,
2354, 2360, 2456, 2504, 2655, 2706, 2714, 2801, 3057. Plus the 7 Memoria save spheres above (not found by the recipe).

Shops (58 triggers, 51 fields; "region R tag 3 -> N" = hand-off to object uid N, which owns the MENU):

| field | disc | trigger -> MENU owner | shop id |
|---|---|---|---|
| 108 | 1 | actor 2 tag 3 | 25 (0x19) |
| 206 | 1 | actor 12 tag 3 | 25 (0x19) |
| 354 | 1 | actor 3 tag 3 | 0 |
| 354 | 1 | actor 7 tag 3 | 0 |
| 355 | 1 | region 16 tag 3 -> 5 | 25 (0x19) |
| 355 | 1 | region 16 tag 3 -> 8 | 25 (0x19) |
| 560 | 1 | region 8 tag 3 -> 4 | 32 (0x20) synth |
| 561 | 1 | region 8 tag 3 -> 4 | 26 (0x1A) |
| 562 | 1 | region 9 tag 3 -> 4 | 1 |
| 602 | 1 | actor 6 tag 3 | 26 (0x1A) |
| 802 | 1 | actor 3 tag 3 | 26 (0x1A) |
| 803 | 2 | actor 2 tag 3 | 25 (0x19) |
| 806 | 1 | actor 2 tag 3 | 25 (0x19) |
| 810 | 2 | actor 6 tag 3 | 3 |
| 902 | 2 | region 6 tag 3 -> 2 | 33 (0x21) synth |
| 910 | 2 | actor 2 tag 3 | 4 |
| 912 | 2 | region 13 tag 3 -> 6 | 26 (0x1A) |
| 1054 | 2 | actor 5 tag 3 | 27 (0x1B) |
| 1055 | 2 | actor 5 tag 3 | 5 |
| 1104 | 2 | actor 15 tag 3 | 27 (0x1B) |
| 1106 | 2 | actor 4 tag 3 | 5 |
| 1307 | 2 | actor 10 tag 3 | 27 (0x1B) |
| 1308 | 2 | region 5 tag 3 -> 3 | 34 (0x22) synth |
| 1309 | 2 | actor 3 tag 3 | 6 |
| 1352 | 2 | actor 4 tag 3 | 7 |
| 1414 | 2 | actor 3 tag 3 | 7 |
| 1455 | 2 | actor 5 tag 3 | 35 (0x23) synth |
| 1456 | 2 | actor 2 tag 3 | 9 |
| 1460 | 2 | actor 4 tag 3 | 28 (0x1C) |
| 1503 | 2 | region 10 tag 3 -> 0 (field code object) | 8 |
| 1509 | 2 | actor 3 tag 3 | 27 (0x1B) |
| 1509 | 2 | region 8 tag 3 -> 3 | 27 (0x1B) |
| 1600 | 2 | actor 6 tag 3 | 10 (0x0A) |
| 1700 | 4 | actor 2 tag 3 | 10 (0x0A) |
| 1857 | 3 | actor 3 tag 3 | 28 (0x1C) |
| 1858 | 3 | actor 3 tag 3 | 11 (0x0B) |
| 1859 | 3 | actor 2 tag 3 | 36 (0x24) synth |
| 1902 | 3 | region 5 tag 3 -> 2 | 36 (0x24) synth |
| 1910 | 3 | actor 4 tag 3 | 12 (0x0C) |
| 1912 | 3 | region 14 tag 3 -> 6 | 28 (0x1C) |
| 1916 | 3 | actor 4 tag 3 | 12 (0x0C) |
| 1917 | 4 | actor 4 tag 3 | 12 (0x0C) |
| 2108 | 3 | actor 4 tag 3 | 36 (0x24) synth |
| 2108 | 3 | region 6 tag 2 (range) -> 4 | 36 (0x24) synth |
| 2109 | 3 | region 10 tag 3 -> 5 | 13 (0x0D) |
| 2303 | 3 | region 11 tag 3 -> 2 | 21 (0x15) and 15 (0x0F) (story) |
| 2452 | 3 | actor 5 tag 3 | 11 (0x0B) |
| 2453 | 3 | actor 3 tag 3 | 36 (0x24) synth |
| 2801 | 3 | actor 10 tag 3 | 37 (0x25) synth |
| 2802 | 3 | actor 13 tag 3 | 17 (0x11) while story < 0x2B5C, else 19 (0x13) |
| 2803 | 3 | actor 17 tag 3 | 29 (0x1D) and 30 (0x1E) (story) |
| 2914 | 4 | region 13 tag 3 -> 14 | 39 (0x27) synth |
| 3054 | 4 | actor 4 tag 3 | 38 (0x26) synth |
| 3055 | 4 | actor 2 tag 3 | 20 (0x14) |
| 3059 | 4 | actor 2 tag 3 | 30 (0x1E) |

The disc column is the disc whose copy of the script was scanned. Fields repeat on later discs, sometimes with
changes. Other MENU uses on disc: type 0 (main menu) in 55 places (tag-1 code of 550-572 and others), type 1 (name
entry) 17, type 5 (chocograph) 37 (2950-2955 chocobo scripts, reached through the player's functions). Shop ids 2,
14, 16, 18 and 22 occur only as Moogle shops; 23, 24 and 31 occur nowhere.

### Disassembler notes
Argument widths were generated from the C, then corrected for: 0x29 (`n`, then n pairs of s16), 0x4B (3 bytes; the
C reads them in either branch), 0xDE (`u8, u16`), 0xFE (5 bytes), 0x4E (debug string: NUL-terminated text, a
variable reference, 1 byte), 0x4F (flags + 1 raw byte), the fallthrough cases 0xBB → 0x56 (`obj, dir, speed`), 0xBC →
0x50 (`obj`), 0xAD / 0x87 / 0xBD / 0xBE / 0xBF, and the field-only ops in `evt_modelOpField` (src/main/msg.c:1666:
0x7F, 0x80, 0x84 no args; 0x81 bb; 0x82 ww; 0x83 b; 0x85 b; 0xD8 bb). With these, every function of every field
script on the four discs decodes to its end with jump targets on instruction boundaries. The only leftovers are
0x00 padding bytes after the last RETURN.

## 3. MENU encoding
`case EVT_OP_MENU` (event.c:1969): `a = evt_getArg8_inline(); menu_open(a, evt_getArg8_inline()); evt_onMenuOpen();
return 1;`. `evt_getArg8_inline` (ff9.h:4180) takes bit 0 of `g_evtArgFlags`: when it is set, it evaluates an
expression and pops it; otherwise it reads one byte. Then it shifts the flags right by one. So MENU is `75 F T A`:
F bit 0 → T (menu type) is an expression, F bit 1 → A (argument) is one. `menu_open` (src/main/menu.c:24) ignores
types >= 9. Types: 0 main menu, 1 name entry (arg = character), 2 shop (arg = shop id), 3 party select (opened by op
0xB2, not MENU), 4 save, 5 chocograph, 8 disc change (opened by op 0xAC EVT_OP_DISC_CHANGE, not MENU). Returning 1 ends the object's turn
for this frame (the scheduler stops running this object until the next field frame). Flag bits beyond the op's argument count
are ignored (e.g. a stray bit 4 in `64 10 22 00`, BG_ANIM_FLAGS with two literal args, field 201).

## 4. Shop tables
None per field. See answer 4: the field → shop id link is the MENU literal (or the Moogle's local variable). The
item lists are in ovl_3c (`s_shopItems`, `s_shopRecipes`), indexed by that id.

## Runtime evidence (disc 1, PCSX-Redux)
- Field 354 entered from `field_dali_inn` with `tools/rt/enter_field.py 354` (story counter 2840 = 0xB18, so entries
  11/3 are created). The field gave no control after the forced entry, so `EvtState+0x16EC` (controlEnabled) was
  poked to 1. Then left / down+left to the counter (states `3ds3_f354`, `3ds3_f354_counter`). Object list
  (`build/3ds3/objs.lua`): `playerUid=11`; uid 11 entry 11 kind 4 model 98; uid 3 entry 3 kind 4 model 55; regions
  10 and 9. Exec breakpoints on evt_execInstr (logging uid/entry and the op bytes at ip), evt_callObjectFunc and
  menu_open (`build/3ds3/optrace.lua`). Cross gave `evt_callObjectFunc(o=0x8007809C uid 3, lv 1, tag 3, save 0)` from
  evt_checkPlayerAction (ra 0x80048B30). uid 3 then ran `27 00 7f`, `1f 00 06 80 49 01` ("Shopkeeper Eve: Welcome!").
  After the next Cross: `47 00 02`, `40 00 db 03`, `54 00 02`, then at f3923 `exec uid=3 entry=3 ip=0x8010B237 ops=75
  00 02 00` and `menu_open(type=2, arg=0)` (ra 0x80044128). ovl_3c was resident and the screen showed
  Buy/Sell/Exit, "Weapons". No call op ran between the Cross press and the MENU.
- Field 355 (`enter_field.py 355`, control poked on, walked up into region 16, state `3ds3_f355_counter`): Cross →
  `evt_callObjectFunc(o=0x800780F4 uid 16 (region), lv 1, tag 3)` (ra 0x80048C84, the region branch of
  evt_checkPlayerAction). Region 16 ran `12 00 06 fa 0d` → `evt_callObjectFunc(player uid 18, lv 6, tag 13, save 0)`,
  then `14 00 02 05 12` → `evt_callObjectFunc(o=0x8007825C uid 5, lv 2, tag 18, save 1)` (ra 0x8003A098). In the same
  frame uid 5 ran its tag-18 code at level 2 (`51 00 fa 20`, `50`), and the player ran tag 13 at level 6 (`51 00 05
  10`). The region object then read `level 1, sp 2, wait 0xFF` (EVT_WAIT_CALL), and the shopkeeper `level 2, sp 2`.
  Two Cross presses later: `exec uid=5 entry=5 ip=0x800FE6C1 ops=75 00 02 19`, `menu_open(type=2, arg=25)`, and the
  Dali item shop opened (Potion, Phoenix Down, ...).
- Emulator stopped afterwards; no breakpoints or processes left.

## Proposed names (not applied)
- `EVT_OP_CALL_CALLER 0x16`, `EVT_OP_CALL_CALLER_WAIT_FREE 0x18`, `EVT_OP_CALL_CALLER_SYNC 0x1A` (`level, func`;
  target = the uid in the top call frame, `CUR_UID()`). They are already in the opcode table of
  `docs/runtime-findings.md` but have no ff9.h macros; event.c:910/915/925 use bare numbers. Confidence high (code),
  not seen at runtime.
- Menu types for `menu_open` / `g_menu.type`: `MENU_TYPE_MAIN 0`, `MENU_TYPE_NAME 1`, `MENU_TYPE_SHOP 2`,
  `MENU_TYPE_PARTY 3`, `MENU_TYPE_SAVE 4`, `MENU_TYPE_CHOCOGRAPH 5`, `MENU_TYPE_DISC 8` (runtime: shop 2 and save 4
  here and in R32b; the others from the R32b / field-menu campaign states). Confidence high for 0/2/4, medium for
  the rest.
- `EVT_VAR_LOCAL_S8 0xD0` style constants are not proposed; the var-ref byte layout is documented in
  3ds-requests.md section 2.

## Tools (promoted)
`tools/evtscript/evtdis.py FIELD [--disc N] [--entry E] [--tag T]` disassembles a field's event script (argument widths
from evt_execInstr, `optab.json`), and `tools/evtscript/menuscan.py` runs the recipe above over all discs
(`MENUSCAN_JSON=out.json` writes the summary). Both find FF9.IMG like the other tools (`encounter_catalog.find_image`).
Applied names: `EVT_OP_CALL_CALLER` / `_WAIT_FREE` / `_SYNC` (0x16/0x18/0x1A), `MENU_MAIN` .. `MENU_DISC` (ff9.h).
