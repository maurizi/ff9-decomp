# Runtime findings

Evidence from the running game (`tools/emu.py`, see `docs/emulator.md`) about what symbols do.
Append entries; confirmed ones become names in `config/symbols*.txt` and are marked **applied**.

Format:

```
## D_XXXXXXXX → proposed_name  (status: proposed | applied | rejected)
- binary: main | ovl_XX; type: u32 / struct ... / function signature
- evidence: commands run, at which save state / scene, and what they showed
- confidence: high / medium / low, and what would confirm it
```

## D_800678EC → vsync counter  (status: confirmed, left unnamed: libetc-internal, no game code uses it)
- binary: main; type: u32
- evidence: increases by 1 per frame (`snap`/`diff` one frame apart, intro FMV). `watch D_800678EC --frames 10`:
  exactly 10 writes, all at `func_80016620+0x2C` (`sw $v0`), called from `func_80016114+0xDC`. Read ~5000×/frame
  at `func_80015DD0+0x74`, which `VSync` calls (a busy-wait loop), and also read inside `VSync` itself.
- confidence: medium. Probably the libetc vsync counter that `VSync()` polls, which would make `func_80016620`
  the vsync callback and `func_80016114` the interrupt dispatcher. Check them against the PsyQ libetc source.

## Overlay usage seen at runtime  (status: applied — config/overlays.us.txt purpose notes)
- `ovl_14` (+ `ovl_64`) is resident during the boot logos / intro (Squaresoft logo, opening FMV), so it's probably the
  opening/logo sequencer, not a minigame.
- `ovl_39` is resident on the title screen's Continue → memory-card slot and file select screens, which confirms "save / load".
- `ovl_0a` is resident in the field (Burmecia vault after loading a save), which confirms "field".
- The top-level main menu (Triangle in the field) loads no 0x801EDF00 overlay, so its code is in the exe or `ovl_0a`.
  Opening Item loads `ovl_37`, which confirms "menu: items".
- evidence: `tools/emu.py ovl` at save states `title_screen`, `field_burmecia_vault`, `menu_main`, `menu_items`.

## Battle save states / ovl_0b residency  (status: applied — config/overlays.us.txt purpose notes)
- `ovl_0b` (0x800A7000, `.text` 0x800A859C-0x800F51E8) is resident in battle (100% .text match, replacing `ovl_0a`),
  which confirms "battle". It's already resident on the black frame right after the encounter swirl.
- Disc 2 states: `field_iifa_outer_root` (field, on the outer root below the Iifa Tree trunk, random-encounter screen),
  `battle_iifa_start` (party HP/MP/ATB HUD up, no command menu), `battle_iifa_command` (Vivi's command menu open).
- Reproduce (deterministic): `start --disc 2`, `load field_iifa_tree`, `frames 5`, `press up --hold 150 --wait 5`,
  `press up+right --hold 120 --wait 5`, `press cross --wait 90`, `press down --hold 200 --wait 30`,
  `press down --hold 200 --wait 60`, `press down+left --hold 200 --wait 60` (= `field_iifa_outer_root`), then
  3x `press down+left --hold 200 --wait 10` → `ovl_0b` appears. `frames 465` → HUD (`battle_iifa_start`); the first
  run reached Vivi's command menu ~490 frames after ovl_0b showed up (`battle_iifa_command`).
  The `field_iifa_tree` screen itself never produced an encounter; walking down leaves it for the outer root screen.
- confidence: high (ovl output and screenshots).

## D_800678EC / D_8006F320 tick rates  (status: applied — D_8006F320 is g_akaoTickCount)
- binary: main
- evidence (DuckStation, intro, GDB reads 2 s apart via the duckstation MCP client): D_800678EC +121
  (~60/s, one per vblank, as above); D_8006F320 +485 (~240/s; the window included the pauses for reads).
  akao_init programs root counter 2 with target 0x44E8 = 33.8688 MHz / 8 / 17640 = 240 Hz, and
  akao_tick increments D_8006F320 once per tick. D_80077DC0 ("frame counter?") stayed 0 here, so it's unconfirmed.
- confidence: high for both rates.

## Pad globals and snd_playSe  (status: applied)
- binary: main
- evidence (PCSX-Redux, `menu_main`, values read every frame via the bridge): holding Cross gives `0x24000`
  (0x4000 = hardware X, 0x20000 = the game's confirm bit). Holding Down for 40 frames:
  0x80077528 is set on frame 1, then on frames 16, 21, 26, 31, 36 (auto-repeat: 15 frames, then every 5);
  0x80077524 is set on every held frame; 0x80077520 only on frame 1. Per port, 0x80073990[0] only on the press
  frame and 0x800739A0[0] while held. The old header comments had 0x80077528 and 0x80077520 the wrong way round.
- `func_8002F488(id)` (logging breakpoint, same state): id 0x67 when the cursor moves down, 0x65 when Cross
  opens the character selection. It sends sound command 0xD000 with that id.
- names: g_padRepeat, g_padHeld, g_padPressed, g_padPortPressed, g_padPortHeld, snd_playSe.
- confidence: high.

## Runtime check of code-derived names  (status: applied — [runtime] added to their symbol entries)
- binary: main (+ ovl_37)
- g_gameState / g_allocTop (DuckStation, after boot): 0x8006794C holds 0x8008B000; that block's DISPENV/DRAWENV/
  file-table pointers are 0x8008B044/0x8008B06C/0x8008B124/0x8008B214 as func_8001CB78 writes them, GameData follows
  at 0x8008B250, and 0x80067940 has since advanced to 0x800A6528.
- g_ot / g_primPtr / g_drawBufs (Redux, `field_burmecia_vault`, read each frame): g_ot alternates 0x8008BB58 /
  0x8008FB98 = g_drawBufs ordering tables; g_primPtr sits just past the matching packet buffer
  (0x800941C4 / 0x800981C4). gfx_selectBuffer is only called from ovl_0b (battle) and wasn't seen: still code-only.
- g_randPtr / g_randTable / g_randTableEnd / rand_refill (Redux, field): the pointer advances inside 0x8007B724-
  0x8007B92D (~4 bytes per 3 frames); on reaching 0x8007B92C rand_refill ran once (from 0x80049E9C), the table
  bytes changed and the pointer restarted at the table start. rand_init didn't run (boot / new game path).
- text_setBank / text_get / g_textBank / g_itemMenuText (Redux, `menu_main`, Cross on Item): ovl_37 loads,
  text_setBank(0x801EDF04) once, then text_get(0), (1), (2) every frame - the three tab labels Use/Arrange/Key.
- akao_tick / g_akaoTickCount / akao_command / snd_playSe (Redux, `menu_main`, Down): 80 akao_tick calls in 20 frames,
  g_akaoTickCount +80; the cursor move calls snd_playSe(0x67) and in the same frame akao_command(0x24) from 0x8005544C.
- confidence: high, except gfx_selectBuffer and rand_init (not observed).

## GameState.playTime (+0x2C) and GameData.gil (+0x560)  (status: applied)
- evidence (Redux, `menu_main`, snapshots 120 frames apart): the menu shows TIME 05:08:20 -> 05:08:22 and gil 9999999.
  0x8008B02C (g_gameState+0x2C) went 1110045 -> 1110165 (+120; 1110165 / 60 = 18502 s = 5:08:22). The only other
  values in that range didn't change. 0x8008B7B0 (GameData+0x560) holds 9999999.
- confidence: high.

## Character records, party and the exp table  (status: applied)
- evidence (Redux, `field_burmecia_vault_lv12`, Triangle opens the main menu): screen shows Zidane Lv12 HP 555/575
  MP 60/62 stones 1/23, Vivi Lv13 266/394 79/82 4/20, Freya Lv14 630/630 60/60 0/24, Quina Lv12 483/483 65/65 7/19.
  GameData+0x54C holds 0x8008B28C/0x8008B31C/0x8008B4CC/0x8008B55C = records 0, 1, 4, 5 of the 0x90-byte array at
  GameData+0x3C. In each record +0xB = level, +0xC = exp, +0x10/+0x12 = current HP/MP, +0x17 = stones left,
  +0x18/+0x1A = max HP/MP, +0x1F = max stones, all matching the screen.
- 0x8006DF5C: [0, 16, 47, ..., 2745, 3588, 4612, 5840]; each member has table[level-1] <= exp < table[level]
  (Zidane 3076 at Lv12, Vivi 3600 at Lv13, Freya 5593 at Lv14, Quina 3312 at Lv12).
- names: Character.level/exp/hp/mp/stones/maxHp/maxMp/maxStones, GameData.characters/party, g_levelExp.
- confidence: high.

## GameState.mode (+0x8) and GameData.fieldId (+0xC)  (status: applied)
- evidence (Redux): GameState+0x8 is 1 in field_burmecia_vault(_lv12), field_dali_inn, menu_main, menu_items and 0 on
  title_screen. GameData+0xC is 764 in both Burmecia vault saves, 351 in the Dali inn; walking out of the inn's left
  doorway into the street changed it to 350 (screenshots before/after). The write came from ovl_0a
  func_800A89C4+0x168 and func_800A8E58+0x4BC; ovl_0a.c:490 copies the old value to field->unkA before storing the
  new one (candidate "previous field", unchecked).
- GameState+0xA (3 in Dali, 1 in Burmecia, 0 on the title) didn't change on the field-to-field exit; it may be the
  previous mode but stays unnamed until a mode change is observed.
- confidence: high for mode (0/1 values) and fieldId.

## gfx_selectBuffer in battle  (status: applied — [runtime])
- evidence (Redux disc 2, `battle_iifa_start`; disc 2's states live under redux-home/SLUS01295): called once per frame
  with i = 0,1,0,1,...; afterwards g_ot = g_drawBufs.unk14[i] (0x800941C4 / 0x80094224) and g_primPtr lies inside
  packet buffer i (0x80094284 / 0x80096284).

## Battle units: hp/mp/atb and the max block  (status: applied)
- evidence (Redux disc 2, `battle_iifa_start` / `battle_iifa_command`): HUD shows Zidane 1308/127, Vivi 1225/163,
  Eiko 1283/119, Amarant 563/97. The 0xD0-byte units (Vivi's at 0x80108FCC, Zidane's 0xD0 before it) hold these at
  +0x28 (hp) and +0x2A (mp). Over 30 frames +0x2C rises for every unit and Vivi's stops at 6560 when her command
  menu becomes ready; Character+0x1C of Vivi is 6560 (Zidane's 5440). Each unit's +0x24 points at its party member's
  Character.maxHp (0x8008B2A4 = characters[0]+0x18 ...), and the values there match the party max HP/MP.
- With Vivi's menu open, D_80104F88 (0x80098664, 0x12D8 bytes allocated at battle start) has +0x12A0 = Vivi's
  unit, +0x12C8[0] = Vivi's unit, +0x12B4 0 -> 1, +0x12B5 = 4 (party size). Not named yet.
- names: BtlUnit.hp/mp/atb/max, BtlUnit24.maxHp/maxMp/maxAtb, Character.maxAtb.
- confidence: high.

## Battle command menu and UI block  (status: applied)
- binary: ovl_0b. Redux disc 2, `battle_iifa_command` (Vivi's command menu open).
- 0x80104FB0 (g_btlCmdMenu): +0 counts 0,1,2,3 over three Down presses (Attack -> Blk Mag -> Focus -> Item, screenshot)
  and was the only byte in RAM doing so. Cross on Attack, then Cross on the target, issues Vivi's command (needs ~40
  frames before the screen changes) and Eiko's menu slides in: +0x18 goes 0x80108FCC (Vivi's unit) -> 0x8010909C
  (the next unit), +0x2 goes 1 -> 6 (Vivi = characters[1], Eiko = characters[6]), +0x4[4] goes {1,22,13,14} ->
  {1,19,20,14} (Vivi: Attack/Blk Mag/Focus/Item, Eiko: Attack/Wht Mag/Summon/Item).
- 0x80104F88 -> 0x80098664 (g_btlUi): +0x12A0 tracks the same unit as g_btlCmdMenu.unit, +0x12B4 goes 0 (no menu,
  battle_iifa_start) -> 1 (menu) -> 3 (after Cross on Attack: target select) -> 4 (Vivi done, Eiko's menu coming),
  +0x12B5 = 4 = party size.
- names: g_btlCmdMenu.cursor/charIndex/cmdIds/unit, g_btlUi.cmdUnit/menuState/partyCount.
- confidence: high for the observed values; the menuState value meanings beyond these four are open.

## Battle commands (BtlCmd) and the battle struct  (status: applied)
- binary: ovl_0b. Redux disc 2, `battle_iifa_command`; Vivi's first command slot BtlUnit.unk4 = 0x80109608.
- Attack on the monster: the slot goes from 00000000 cc8f1080 00000000 0000 00 00 ... to
  ... cca91080 1000 01 b0 04 ...: +0x8 = 0x8010A9CC, +0xC = 0x10, +0xE = 1, +0xF = 176 (0xB0).
- Focus (row 2) instead: +0x8 = 0x8010AA8C, +0xC = 0x2 (Vivi's own unit bit), +0xE = 13, +0xF = 188.
  +0xE matches g_btlCmdMenu.cmdIds of the chosen row (Attack 1, Focus 13). +0x8 moves by (188-176)*0x10, i.e.
  &table[+0xF]; GameData.unk8E4 = 0x80108800 (the battle struct) and 0x80108800+0x16CC = 0x80109ECC is that
  table's base, as the code's `cmd->unk8 = &battle->unk16CC[cmd->unkF]` says. Battle+0x6FC = 0x80108EFC is
  Zidane's unit (Vivi's is the next, +0xD0).
- names: BtlCmd.ability/targets/cmdNo/subNo, Battle.units/abilities.
- confidence: high. targets = unit bitmask (party from bit 0, enemies from bit 4) rests on two values (0x10 enemy, 0x2 Vivi).

## Battle command queue  (status: applied)
- binary: ovl_0b. Redux disc 2, `battle_iifa_command`, Attack on the monster (logging breakpoints + write watches on
  Vivi's slot 0x80109608).
- frame 1365: func_800B1250(0x80109608, 1, 0xB0, 0x10) from func_800F0444 writes cmdNo; then
  func_800B3090 -> func_800B440C(0x80109608, 0x80109874) links it into the list at Battle+0x1074 and sets the
  slot's +0x10 to 0x04. frame 1392 (the attack plays): func_800B3F98 -> func_800B4320(0x80109874, ...) unlinks it
  and the state byte goes back to 0. frame 1488: another command (0x80109770, presumably the monster's) is queued
  the same way.
- names: btl_setCmd, btl_enqueueCmd, btl_linkCmd, btl_removeCmd, btl_unlinkCmd, Battle.cmdQueue.
- note: config/functions.us.txt records some functions at their migrated rodata's address (e.g. func_800B1250 at
  0x800A7184); tools/rename.py now checks function names, not addresses.

## Damage path of an Attack  (status: applied)
- binary: ovl_0b. Redux disc 2, `battle_iifa_command`, Vivi attacks the monster (units[4] at 0x8010923C).
- monster HP (unit+0x28) 1473 -> 1347. The only write: func_800BDB20+0x98, called (0x8010923C, 126, 1) from
  func_800B7954, under func_800B44C0(Vivi 0x80108FCC, monster, Vivi's cmd 0x80109608, 1) from func_800B2804(monster).
  Battle+0x1088 = 0x80109608 (Vivi's command) at that point.
- names: btl_damageHp, btl_execCmd, btl_calcCmd, Battle.curCmd.

## MP cost  (status: applied)
- binary: ovl_0b. Redux disc 2, `battle_iifa_command`: Blk Mag -> Fire (list shows Fire 6, Fira 12, Sleep 10) on the
  monster. btl_setCmd(slot, 22, 25, 0x10). Vivi's mp 163 -> 157 in one step; among 51 writes to it (the rest are the
  every-4-frames rewrite in func_800BD93C) the only other one is func_800B9550+0x58, called from func_800B4390.
  The ability entries (Battle.abilities, 0x10 each) have +0xA = 6 (25 Fire), 12 (26 Fira), 24 (27), 10 (28 Sleep).
- names: btl_payMpCost, btl_consumeMp, ability mpCost.
- ability +0xE (same entries): 0xA1, 0xA6, 0xAB, 0xB2, 0xB8 for Fire, Fira, Firaga, Sleep, ...: each step is the
  previous name's length + 1, so it's the name's text offset. Named nameOfs.

## Field player object: pos, facing, playerUid  (status: applied)
- evidence (Redux, `field_dali_inn`): event context (0x80077E60) +0x16ED = 19; the active-object list's object with
  uid 19 is 0x8007805C (Zidane). Walking right then down: +0x14 (pos.vx) -262 -> 211 -> 271, +0x1C (pos.vz)
  1419 -> 1312 -> 891, +0x18 (vy) unchanged, +0x22 (rotation) 0 -> 3072 -> 4095.
- names: Unk8004AB64.pos, ActorPos.rotY, Unk8007AECC.playerUid (the event context struct).

## Regen status, its timer, and the menu list cursor  (status: applied)
- Redux disc 2, `battle_iifa_command`: Vivi attacks, then Eiko: Wht Mag -> list (Cure 6, Cura 10, [empty row], Regen 14).
  Up presses move 0x8007760E (window 2 at 0x800775D8 +0x36 = MenuList+6) 2 -> 1 -> 0; the on-screen hand had misled
  me: my earlier "Sleep" attempts sat on the empty row 2, which is why Cross did nothing. With it at 3 (Regen),
  Cross + Cross casts Regen on Eiko (banner "Regen", MP 119 -> 105).
- Afterwards Eiko's unit (units[2]) has +0x38 = 0x00040000, the other three 0; her +0x44 jumped 0x3C -> 0x4AA
  and then fell by 0xD2 per 60 frames (the Regen countdown).
- names: MenuList.cursor; the Regen bit is UNK38_REGEN in BtlUnit.unk38 (that word's role is still open, see below).

## Sleep status  (status: applied)
- Redux disc 2, `battle_iifa_command`: Vivi, Blk Mag, row 3 (Sleep, MenuList.cursor 3) on the monster.
  btl_setCmd(slot, 22, 0x1C, 0x10); the cast animation takes ~400 frames; btl_calcCmd(Vivi, monster, ..., 0xB) at
  frame 3542; then the monster (units[4]) has +0x3C = 0x00020000 and +0x48 = 8046 (a timer; Regen's was +0x44).
- names: BtlUnit.status, STATUS_SLEEP (0x20000).

## Which status word is which: KO test  (status: applied)
- Redux disc 2, `battle_iifa_command`: monster (units[4]) hp written to 1, then Vivi's Attack kills it. Logged
  func_800BEDBC(monster, 0x200) right after the write (hp 1: low HP; not kept), then (monster, 0x100) every 4 frames
  once hp is 0. +0x34/+0x38/+0x3C went 4400E004/0/0 -> 4400E004/0/00000100.
- func_800BEDBC returns early if +0x34 has the bit, and otherwise ORs it into +0x3C. So +0x3C is the current status
  word (Sleep and KO observed there) and +0x34 the immunity mask (the monster's is non-zero). +0x38 holds the
  Regen spell's 0x40000 and the support-ability bits set at battle start; what separates it from +0x3C is open, so
  it stays unk38.
- names: BtlUnit.status (was briefly "status2"), statusImmune, STATUS_KO, STATUS_LOW_HP (code: hp*6 < maxHp),
  STATUS_SLEEP, UNK38_REGEN, btl_addStatus.

## Item inventory  (status: applied)
- Redux disc 1, `menu_items` (screen: Potion 46, Ether 12, Remedy 3, Tent 9, Hi-Potion 10, Phoenix Down 14, ...,
  Gysahl Greens 99). GameData+0x574 starts {236,46} {237,10} {238,12} {240,14} {247,3} {249,2} {253,9} ...
  {251,99}: {item id, count} byte pairs, 0x100 of them (0x574..0x774).
- names: GameData.items (ItemInventory.slots[], ItemSlot.id/count). ovl_37's sort buffer and swap temp and
  ovl_33's inventory copy hold the same pairs and are now ItemSlot too.

## Field load / loop, event init, menu open  (status: applied)
- Redux disc 1, `field_dali_inn`, walking out of the inn (down, then left): func_800A89C4(0x15E = 350) from
  func_800A7B10+0x14 (ovl_0a); afterwards GameData.fieldId = 350 and the street is on screen. func_800A89C4 then calls
  func_8004A06C(0x80107234) (main) at frame 499.
- Same state, Triangle: func_80032120(0, 0) from func_800A7B10+0x110 and the main menu appears (location
  "Dali/Inn"); func_80032120 just calls func_80032140, which fills D_80077DB0 for screen 0.
- names: fld_load, fld_main (ovl_0a), evt_init, menu_open, menu_doOpen, g_menu (main).

## Character stats  (status: applied)
- Redux disc 1, `field_burmecia_vault_lv12`, Triangle -> Status -> Zidane. Screen: Speed 24, Strength 26, Magic 22,
  Spirit 25, Attack 18, Defense 9, Evade 10, Magic Def 10, Magic Eva 7, Trance bar about half, EXP 3076, Next 512.
  Record 0x8008B28C: +0x20 = 121, +0x24..+0x27 = 24 26 22 25, +0x28..+0x2B = 9 10 10 7, +0x30..+0x33 = 24 25 22 24
  (Strength and Spirit 1 below the shown values: equipment). Attack isn't in the record (weapon).
- equipment: +0x39..+0x3D = 3 115 90 152 255; screen rows Mythril Dagger, Steepled Hat, Glass Armlet, Bronze Vest, (empty).
- names: Character.trance, speed/strength/magic/spirit, defense/evade/magicDef/magicEva, equip[5], baseSpeed/baseStrength/
  baseMagic/baseSpirit.

## Message window  (status: applied)
- Redux disc 1, `field_dali_inn` -> walk up-left (saved as `field_dali_inn_moogle`) -> Cross on the Moogle:
  func_8004DCEC(3, 2, 0x8007B940, 8) from func_80039C38+0x262C and the MOGNET window opens ("Gumo / Can I help you,
  kupo? / Save / Tent / Mognet / Cancel"). func_80039C38's cases 0x1F, 0x20 and 0x95 make that call with a window
  id, flags and a 2-byte message number from the script.
- names: msg_open, evt_execInstr.

## Item remove / count  (status: applied)
- Redux disc 1, `field_dali_inn_moogle`: Cross (Moogle menu) -> Tent: prompt "Rest inside a tent, kupo? (Tent(s)
  Remaining=69)"; func_80027F34(0xFD) runs on that frame (from func_80037ACC). Rest: ~240 frames later the Tent slot
  (GameData.items.slots[10], id 253) goes 69 -> 68, written by func_80027E5C(0xFD, 1) called from evt_execInstr.
- names: item_remove, item_getCount. The inventory helpers now use ItemSlot (code unchanged).

## Script opcodes 0xF1 / 0xF2 (set HP / MP)  (status: applied)
- Redux disc 1, `field_dali_inn_moogle`, Zidane's HP written to 100 and MP to 5, then Tent -> Rest: HP -> 1806,
  MP -> 113. The writes come from evt_execInstr+0xB38C and +0xB4C8, the interpreter's only HP and MP stores, in
  cases 0xF1 and 0xF2 (character from getv, value from a 2-byte arg, capped at maxHp/maxMp).
- names: EVT_OP_SET_HP, EVT_OP_SET_MP.
