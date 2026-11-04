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
- **Corrected by the "Sound ids" section: 0x65 is cancel/back, Cross plays 0x67.** `func_8002F488(id)` (logging breakpoint, same state): id 0x67 when the cursor moves down, 0x65 when Cross
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

## CD sector loading  (status: applied)
- Redux disc 1, `field_dali_inn_moogle`: Moogle -> Save -> Save loads ovl_39 (resident ~180 frames later).
  The first write to 0x801EDF00 is CD_getsector+0xB0 (a0 0x801EDF00, a1 0x200) from func_800213C0. Logging
  func_800213C0: 29 calls into 0x801EDF00.. (0x801EDF00, 0x801EE700, ... size 0x200 words), all from
  func_8002291C+0x64. Sampling 0x800761D0 per frame: +4 29 -> 28 -> 25 -> ... (sectors left), +0xC and +0x10 grow by
  0x800 per sector, +0x14 = 0. func_800213C0's body is libcd's CdGetSector.
- names: cd_onSectorReady, CdGetSector, g_cdLoad (sectorsLeft, bytesRead, chunkOfs, chunkIndex).
- The first write of g_cdLoad.sectorsLeft (29) is func_8002267C+0xA8; afterwards g_cdLoad.unk944 (the copied request)
  holds +0 = 3, +4 = 0x5FA (start sector), +8 = 0xE800 (bytes, = 29 sectors), +0xC = -1 (use the inline chunk),
  +0x14 = func_800227C8, +0x1C = {0xE800, 0x801EDF00}.
- names: cd_startRequest, CdRequest.sector/size, CdChunk.size/dest.

## math_sin / math_cos, rand_refill equivalence  (status: applied)
- Redux: in `field_dali_inn` walking calls func_8004BB80 ~once a frame (a0 0x1040, 0x820, ...); in
  `battle_iifa_command` an attack makes 438 calls to func_8004BB7C, each with a0 = 0 followed by func_8004BB80 with
  a0 = 0x400 from the same call site (the fall-through). Table D_8006C1C4: [0] 0, [0x200] 2896, [0x400] 4096.
- src_equiv/main/math_rand.c's rand_refill, applied to the 0x20C table bytes captured the frame before a refill in
  `field_burmecia_vault`, gives exactly the bytes after it (0 of 0x20C differ, the 3 bytes past the table included).
- names: math_sin, math_cos, g_sinTable.

## Equivalence checks for src_equiv/main/script_text.c  (status: applied)
- Redux disc 1, `field_burmecia_vault`, holding a direction: broke at the entry of func_80052F24, func_80052EC0,
  func_80052DEC and func_80053124 (keeping only stops where pc == entry, then pc == the return address), recorded
  a0-a3 and v0, and for func_80053124 the polygon bytes. Replaying them through the src_equiv C on the host:
  85 math samples and 29 polygon samples, 0 differences. (All polygon samples were "outside"; an "inside" case is
  still to capture.)

## Game over: modes 7 and 8  (status: applied)
- Redux disc 2, `battle_iifa_start`, all four party units' hp written to 0: mode 2 -> 8, ovl_65 loads; the mode loop
  calls func_801EF284 (frame 6722), which reaches func_801F3488 via func_801F3A7C, and the GAME OVER screen shows.
- Redux disc 1, `field_dali_inn_moogle`: Cross on the Moogle, break at the first evt_execInstr, write 0xF5 over the
  opcode at gExec's ip (0x8010E2DE, was 0x59): mode 1 -> 7, ovl_65 loads at 0x801EDF00 while ovl_0a stays, and the
  same GAME OVER screen shows. Path: evt_execInstr case 0xF5 returns 8 -> func_8004BC38 returns 8 -> fld_main sets
  the field exit reason (FieldScene+0x4A) to 7 -> mode 7 -> fld_gameOver (ovl_0a func_800A80A4) -> gameover_main,
  then mode 0.
- names: battleEnd_main, gameover_main (ovl_65), fld_gameOver (ovl_0a), EVT_OP_GAME_OVER.

## Leads from the ff9-ds port, not yet named  (status: proposed)
Behaviour the DS port (`../ff9-ds`) documented or checked against the original instructions, which wasn't enough
for a name on its own. Port names are leads; the decomp code has to confirm each one.
- main `func_80063118` / `func_800631AC` / `func_800632B4`: per-frame offset animation of a RECT (`WinDef`/`TweenDef`):
  start (copies the delay, optionally rewinds by the summed deltas), step (returns 1 when done, calls the done
  hook), sum of n s8 deltas. ff9-ds `tools/test_window_tween.py` matches the original 80063118 in 400 cases. Who
  owns the RECT (text window or any menu element) is unconfirmed.
- main `func_8001486C` / `func_80014AA8`: libgpu's CPU-to-VRAM and VRAM-to-CPU transfer helpers behind
  `LoadImage`/`StoreImage` (entries +0x20/+0x1C of the table at `D_80066614`); the read side transfers
  `(w*h+1)/2` whole words. ff9-ds `docs/decomp-requests.md` F23/F24. Library code, so no game name; the PsyQ
  names need a signature match.
- ovl_0b `func_800E3EEC`: runs the frame's effect calls on the scratchpad stack and saves/restores the effect RNG
  seed (`g_btlfxRandSeed`). Whether it is the whole battle frame or only the effect step is open.
- ovl_0b effect emitters `func_800D3ED4` … `func_800D5C30` (sprites, strips, ribbons, Gouraud fans, circular lines)
  and particle integrators `func_800DFE4C`/`DFE54`/`DFF5C`/`DFF64`/`E0014`: ff9-ds `port/game/battle_quads.c`,
  `battle_ribbons.c`, `battle_motion.c` with original-instruction tests. Names need each packet format checked.
- ovl_0b `func_800F4A00`: battle HUD drawn through BreakDraw/ContinueDraw with its own embedded draw area/offset
  (E3/E4/E5) and a DR_MOVE from the back buffer; `D_80105160` holds the saved draw state (ff9-ds F10/F11).
- ovl_0b `func_800C9040`: advances texture animations (fixed-point frames; `unk0 & 4` scrolls rows with two
  MoveImages) (ff9-ds F11).
- Mode init functions set `GameState.vsyncMode`: ovl_0a `func_800A8198` 2, ovl_0b `func_800A8CD4` 4, ovl_0c
  `func_800A8B14` 3; ovl_0c `func_800AC5C8` picks 2/0/3 by state and ovl_0b `func_800E8F40` changes it (ff9-ds
  `docs/porting-notes.md`).
- Nullable per-model texture-animation table (`unk50` of field and battle models): read without a NULL check by
  evt_execInstr opcodes 0xC0-0xC2, ovl_0a `func_800AB3CC` and several ovl_0b status/KO routines (ff9-ds F4, F9,
  F20, F26). `g_evtGlobalVars` byte 0x10 enables trance in ovl_0b `func_800BB4A0` (ff9-ds F22).
- Correction to the port: main `func_800236A4` (ff9-ds `port/psx/cd.c` treats it as a read-callback setter) swaps
  the level that gates CdSearchFile's printfs, so it was named `CdSetDebug`.

## Battle campaign 2026-10-17: method, states, helper scripts  (status: reference)
- Redux disc 3. Battles are entered directly from the Lv 99 Pandemonium save (card 1 FILE04, state
  `disc3_pandemonium_lv99_field`) with `build/rt/enter_battle.py ENC --field F --party 0,1,2,6` (a copy of ff9-ds
  `tools/psx_battle_fixture.py` that keeps the save's own party instead of importing fixture blobs, see
  docs/emulator.md "Entering any battle"). Party Zidane/Vivi/Dagger/Eiko, Lv 99, 9999 HP.
- Encounter 899 (Malboro, the Pandemonium random encounter) with `--field 2714`: states `disc3_malboro_lv99_start`,
  `disc3_malboro_lv99_calm` (Zidane's command menu, nothing queued), `_target` (target hand on the Malboro),
  `_ablist` (Vivi's Blk Mag list), `_itemlist` (Item list), `_help` (Select: help window), `disc3_malboro_rewards`
  (EXP/AP screen, ovl_65 resident). Ark (encounter 0) has magic disabled (Oeilvert), so it's not used for spells
  (`disc3_ark_lv99_cmd`, `disc3_ark_lv99_calm` exist).
- The "calm" states rely on a per-vsync poke that keeps the Malboro's ATB at 0 and its HP at 11683
  (`build/rt/freeze.lua`, `Freeze.list={{0x80109268,"0000"},{0x80109264,"a32d"}}`); without it the Malboro acts.
- Coverage: `build/rt/cov.py start --bin ovl_0b --bin main` puts an exec breakpoint on all 2144 game functions with a
  single shared LuaJIT callback (one `ffi.cast` per breakpoint exhausts LuaJIT's callback slots at ~900) and counts
  hits, first frame and up to 6 callers. `build/rt/act.py NAME STATE 'inputs'` loads a state, plays inputs and saves
  `build/rt/cov_NAME.json`; `build/rt/matrix.py a,b,c` prints the per-function hit counts side by side.
  Runs: idle, z_attack, z_steal (Mug), v_fire, d_cure, d_summon (Ramuh, via a btl_setCmd argument patch
  `build/rt/patch.lua`), z_item (Potion), enemy (Malboro acting), victory (Malboro HP 1, Attack), ui_* (60 frames in
  each UI state). The battle logic runs every 4th vsync (375 calls per 1500 frames), the UI on every vsync.
- Ablation: `build/rt/ablate.py STATE '' func_A func_B ...` loads the state, overwrites the function's first two
  words with `jr ra; nop`, runs 8 frames and screenshots. What disappears from the screen is what the function draws.
  Only meaningful for void functions; stubbing a value-returning one (marked below) shows what its result controls.

## func_800E9A74 → btlui_vsync  (status: applied)
- binary: ovl_0b; `void (void)`. Installed with `VSyncCallback(func_800E9A74)` by func_800E9D30; coverage: called
  1500x in 1500 frames from func_80016620+0x48 (the libetc vsync handler), while the battle logic runs 375x.
  Calls the UI pad read, frame begin, all window/HUD drawing and the UI OT submit (below).
- confidence: high.

## func_800E9D30 → btlui_init  (status: applied)
- ovl_0b `void (void)`: initialises the UI blocks (func_800EB05C, func_800F3C08, func_800F4884), sets
  D_80104E68.unk6 and installs btlui_vsync. Called from btl_main when Battle.unkD85 & 7 == 7. confidence: medium (code).

## func_800F4778 → btlui_readPad  (status: applied)
- ovl_0b `void (void)`: pad_update() then func_8002C308, and Start (g_padRepeat & 8) sets D_80104E68.unkB.
  Runtime: called from btlui_vsync+0x34 every vsync; Start at `disc3_malboro_lv99_calm` shows the PAUSE screen.
  This is where the battle menu reads the pad (R16). confidence: high.

## func_800F4D40 → btlui_beginFrame  (status: applied)
- ovl_0b: flips D_801051E0, gfx_selectBuffer(D_801051E0), clears the UI OT (func_800F4D80(g_ot, 0x18)); every vsync
  from btlui_vsync. confidence: medium (code + call order).

## func_800F4A00 → btlui_submitOt  (status: applied)
- ovl_0b: links draw-mode/area/offset packets into g_ot[0x17]/[1] and pushes the UI OT with BreakDraw/ContinueDraw.
  Ablation at `disc3_malboro_lv99_calm`: the whole HUD (command window and party panel) disappears.
  confidence: high.

## func_800EA440 → btlui_drawWindows  (status: applied)
- ovl_0b: when D_80104E68.unk8 == 0 calls btlui_drawHelpWindow, btlui_drawPartyPanel, win_drawAll, func_800EA518,
  then btlui_drawTargetCursors. Ablation: command window and party panel both vanish. confidence: high.

## func_800F3060 → btlui_drawPartyPanel  (status: applied)
- ovl_0b: draws the NAME/HP/MP/ATB panel: highlight, header, then per row btlui_updatePanelRowColor and the four
  column callbacks D_800F9038[] (name, HP, MP, gauges). Ablation: the whole party panel disappears, nothing else.
  D_80105078 is the panel state (unk5C = row count, unk68[] = units, unk60 = y). confidence: high.

## func_800F3390 → btlui_drawPanelHeader  (status: applied)
- ovl_0b: the header rule (func_80025A28) and column titles (ui_drawLabel). Ablation: the "NAME HP MP ATB" bar
  goes, rows stay. confidence: high.

## func_800F34C0 → btlui_drawPanelHighlight  (status: applied)
- ovl_0b: POLY_G4 bar at row D_80105078->unk2 (the unit whose menu is open). Ablation: the yellow bar behind
  "Zidane" goes. confidence: high.

## func_800F35C8 → btlui_drawPanelName  (status: applied)
- ovl_0b `(s32, u32 row)`: the name SPRT of a row (clut from the row color). Ablation: the four names go. high.

## func_800F36F8 → btlui_drawPanelGauges  (status: applied)
- ovl_0b `(BtlUnit *, s32 row)`: both gauge bars of a row via ui_drawGauge. Ablation: the ATB (cyan) and trance
  (red) bars go, the numbers stay. confidence: high.

## func_800F41B8 → btlui_drawPanelHp, func_800F422C → btlui_drawPanelMp  (status: applied)
- ovl_0b `(BtlUnit *, s32 row)`: text_formatNumber(hp, buf, 4, 1) + ui_drawDigits at x 0xD0 (HP); F422C the MP
  column. Ablation: only the HP column (41B8) / only the MP column (422C) disappears. confidence: high.

## func_800F4060 → btlui_updatePanelRowColor  (status: applied)
- ovl_0b `(s32 row)`: D_80105078->unk78[row] = 3 when hp is 0, 4 when hp <= maxHp/6, else 0 (color of the row's
  name and numbers). Runtime at `_calm`: Vivi's hp written to 100 and Dagger's to 0: D_80105078->unk78 (0x800999B4) becomes
  00 04 03 00 and the panel shows Vivi's row yellow, Dagger's red. The shown HP rolls down towards the new value
  (8419, 8429 after 12 frames): btlui_drawPanelHp prints the unit's menu entry unkB4->unk0[0].unk2, a display
  counter, not BtlUnit.hp. confidence: high.

## func_800EC628 → btlui_drawCmdList, func_800EC730 → btlui_drawCmdOverlay  (status: applied)
- ovl_0b `(Window *)`: the command window's content and overlay callbacks (set with win_setDrawContent /
  win_setDrawOverlay by the code at ovl_0b_2.c:9926). Ablation: EC628 removes the four command names (frame stays);
  EC730 removes the hand cursor and the "COMMAND" title. confidence: high.

## func_800EB1CC → btlui_updateMenu, func_800EB114 → btlui_menuInput  (status: applied)
- ovl_0b `void (void)`: EB1CC runs the per-menuState update D_800F8DA8[g_btlUi->menuState]; EB114 picks the next
  ready unit when menuState is 0 and otherwise dispatches input through D_800F8D94[menuState]. Coverage: both every
  vsync; during target selection EB114 calls func_800F11FC each frame (trace, `disc3_malboro_lv99_target`).
  confidence: medium.

## func_800EB2BC → btlui_setMenuState  (status: applied)
- ovl_0b `s32 (s32 state)`: stores g_btlUi->menuState unless state is 5, returns the old value. Runtime
  (`disc3_malboro_lv99_calm`, Steal): called with 3 from func_800EF80C (command chosen -> target select), 4 from
  func_800F0444 (target confirmed) and func_800EC4EC, 1 from func_800EDC3C (next unit's menu). confidence: high.

## func_800F11FC → btlui_targetInput  (status: applied)
- ovl_0b: target-selection input: Circle cancels (func_800F07F0), Cross confirms (func_800F0444), L1/R1 toggles
  single/all (func_800EFF14), L2 toggles GameState.unk24 bit 19, otherwise d-pad (func_800F1554). Runtime at
  `disc3_malboro_lv99_target`: called every vsync from btlui_menuInput; L2 sets 0x80000 in GameState+0x24 and opens
  the ENEMY / PLAYER name window. confidence: high.

## func_800F0444 → btlui_confirmTarget, func_800F07F0 → btlui_cancelTarget  (status: applied)
- ovl_0b: F0444 is the caller of btl_setCmd when the Attack target is confirmed (ra func_800F0444+0x78, both
  campaigns) and sets menuState 4. F07F0 is what Circle and an unusable command fall back to. confidence: high / medium.

## func_800F1554 → btlui_targetMove  (status: applied)
- ovl_0b: d-pad handler table D_800F8E94[] indexed by D_800F8EA4[mode][i] button masks. confidence: medium (code).

## func_800F0904 → btlui_updateTargetWindow  (status: applied)
- ovl_0b: opens (win_open(&D_800F8EF4, 5, 2, 0) with callbacks F1A8C/F0A54/F0BE0) or closes the ENEMY/PLAYER target
  name window when the GameState.unk24 bit 19 option differs from its state. Runtime: L2 in target select shows that
  window. confidence: high.

## func_800F0118 → btlui_isCmdUsable  (status: applied)
- ovl_0b `s32 (void)`: 0 when the selected command can't be used any more (trance ended for a trance command, MP
  below the cost (halved under D_80104FF0.unkD), silenced for a magic ability, item count minus reserved <= 0).
  Runtime: stubbed (returns garbage) at `disc3_malboro_lv99_target`, the target hand disappears because func_800F1300
  then cancels selection. confidence: high.

## func_800F0DF8 → btlui_drawTargetCursors  (status: applied)
- ovl_0b: one 24x14 hand SPRT per entry of D_80105020 (dimmed when several blink). Ablation at `_target`: the hand
  on the Malboro disappears. D_80105020 = target cursor list. confidence: high.

## D_80104FF0 → g_btlTarget  (status: applied)
- ovl_0b: target-selection state (126 refs): unk1 command id, unk1A ability/item, unk20 caster, unk6 multi-target,
  unkC target-window open. Read by btlui_isCmdUsable/func_800F028C/btlui_updateTargetWindow while `_target` runs.
  confidence: medium (name only; fields open).

## func_800EEA98 → btlui_getAbilityState  (status: applied)
- ovl_0b `s32 (s32 ability)`: 0 = not learned (row hidden), 1 = greyed (silenced magic or MP above the caster's),
  2 = usable. Runtime at `disc3_malboro_lv99_ablist` (Vivi shows Fire 6, Fira 12 and two blank rows): stubbed, all
  four rows show, greyed: Fire, Fira, Firaga 24, Sleep 10. Called 4x per vsync (one per visible row). confidence: high.

## func_800EF0D8 → btlui_drawAbilityRow, func_800EF228 → btlui_drawItemRow  (status: applied)
- ovl_0b `(s32, s32 row, s32 index, MenuView *)`: row callbacks of the list window (via D_800F8E48[] in
  func_800EF094). Ablation: ability names/costs vanish (`_ablist`); item icons, names and counts vanish (`_itemlist`).
  confidence: high.

## func_800EF598 → btl_getMpCost  (status: applied)
- ovl_0b `s32 (Ability *)`: mpCost, x4 for abilities with unkB & 4 while D_8007AF02 is set, halved while
  D_80104FE8. Used for the list's cost column and usability. confidence: medium (code; the list shows the plain costs).

## func_800EEED4 → btlui_drawListContent, func_800EEBB8 → btlui_drawListOverlay  (status: applied)
- ovl_0b `(Window *)`: content/overlay callbacks of the ability/item list window. Ablation at `_ablist`: EEED4
  removes the rows, EEBB8 removes the hand cursor. confidence: high.

## func_800EE54C → btlui_listInput  (status: applied)
- ovl_0b: list input: Circle closes, Cross picks (func_800EEE3C), else menuList_up/down/pageUp/pageDown. Coverage:
  only in the ability and item list states. confidence: high.

## func_800EC194 → btlui_getItemReserved, func_800EC114 → btlui_getItemListEntry  (status: applied)
- ovl_0b: D_80104FA8->unk1E0[id] (items already queued by other party members), D_80104FA8->unk4[i]. Runtime at
  `_itemlist`: stubbing EC194 (garbage return) changes every shown count (99 -> 15, 95 -> 09, ...), the count is
  inventory minus this. confidence: high.

## func_800EBA20 → btlui_drawHelpWindow, func_800EBD88 → btlui_updateHelpText, func_800EBE1C → btlui_getHelpRect  (status: applied)
- ovl_0b: the Select help window (D_80104F90 = its state). Ablation at `disc3_malboro_lv99_help`: EBE1C (rect) makes
  the whole window vanish; EBD88 picks the text through D_800F8DC4[menuState]. confidence: high (EBA20 by call
  structure, EBE1C ablation).

## func_800EDB88 → btlui_getCmdHelpText  (status: applied)
- ovl_0b `u8 *(void)`: copies the selected command's description and appends text 0x19 (the "Change / Defend / Skip"
  key line) into D_80104FA0. Ablation (garbage return): the help window shows garbage glyphs. confidence: high.

## func_800CD5C8 → btl_showMessage  (status: applied)
- ovl_0b `void (u32 id, ...)`: formats battle text id+8 from D_800F6CB4, substituting varargs (item names, numbers),
  and passes it to func_800F4F04 (the top message banner). Runtime (`_calm`, Steal): func_800CD5C8(0xC, item name)
  from func_800B7F20+0x78 and the banner shows "Stole Ore!". 31 refs. confidence: high.

## func_800C09A4 → btl_hasStatus  (status: applied)
- ovl_0b `s32 (BtlUnit *, u32 mask)`: 1 when unk38 or status has any bit of mask. Called 720x/60 frames by the UI
  (e.g. btlui_drawCmdOverlay tests 0x4000 = trance for the title). 40 refs. confidence: high (code).

## Window system (main exe), seen through the battle HUD  (status: applied)
- binary: main. D_80077530 = the 10 `Window` slots (0x54 each). All evidence below is ablation at
  `disc3_malboro_lv99_calm` (or the state named) plus the call structure; these are shared by field and menus.
- func_8002C9B0 → win_drawAll (`void`): for each live window: overlay callback (unk44), content callback (unk40,
  clipped), scroll arrows, frame. Ablation: the command window disappears (the panel, drawn elsewhere, stays). high.
- func_8002CB18 → win_drawFrame (`Window *`): ablation: the command window's frame and background go, its text and
  cursor stay. Calls func_80026B30 and func_8002709C. high.
- func_80026B30 → win_drawBorder (`RECT *, otz, ...`) and its packet builder func_800253D4 → win_buildBorder:
  ablation: the window's bevelled border goes, the grey fill stays. high.
- func_8002709C → win_drawFill and its packet builder func_80024EB4 → win_buildFill: ablation: the grey textured fill
  goes, the border stays. high.
- func_8002D9B4 → win_setClip (`RECT *r, otz, style`): DR_AREA to r (offset by style, clamped to the display), or the
  whole display for NULL. Ablation: no visible change (nothing overflows). medium (code).
- func_8002E4BC → win_isVisible (`u8 (Window *)`): open flag and minimum size. medium (code).
- func_8002DCAC → win_open (`Window *(RECT *, otz, style, u32 slot)`) → func_8002EA7C → win_init: runtime: L2 in
  target select calls it (via btlui_updateTargetWindow) and the ENEMY/PLAYER window opens. high.
- func_8002DD00 → win_close (`Window *`): state 2 (closing). func_8002DD54 → win_closeAll. medium (code).
- func_8002CF20 → win_setOpenAnim (`Window *, mode`): open/close step sizes per mode 0/1/3/4. medium (code).
- func_8002E180 → win_updateAll: per vsync, grows opening windows (func_8002D550 → win_stepOpen, calls unk48 and sets
  state 3 when fully open) and shrinks closing ones (func_8002D7E8 → win_stepClose). Coverage: func_8002D550 runs 7x
  as the next unit's command window slides in after an Attack. high.
- func_8002E260 → win_reapClosed: windows in state 2 go to 0 and get their unk4C callback. medium (code).
- func_8002E2EC → win_setDrawContent (unk40), func_8002E2F4 → win_setDrawOverlay (unk44), func_8002E2E4 →
  win_setOnOpened (unk48), func_8002E2FC → win_setOnClosed (unk4C): plain setters; the battle code registers
  btlui_drawCmdList as content and btlui_drawCmdOverlay as overlay (ablation results above). high.
- func_8002E718 → win_drawScrollArrows (`RECT *, otz, MenuList *`): sprites 0xB8/0xB9 (up/down), blinking. Called by
  win_drawAll when visible rows < total rows; ablation at `_ablist`: the arrows at the list's right edge go. high.
- func_8002ECA0 → menuList_getView (`MenuView *(MenuList *, MenuView *)`): visible rows/first row/y offset (with
  scroll animation). Ablation at `_ablist`: the rows disappear. high.
- func_8002EEF0 → menuList_up, func_8002EF58 → menuList_down (pad 0x10 / 0x40, cursor or scroll, sound 0x67),
  func_8002F1CC → menuList_pageUp, func_8002F284 → menuList_pageDown (0x100000 / 0x200000, sound 0x417). Called only
  by btlui_listInput in the list states (coverage). medium (code).
- func_8002DF18 → win_drawHelpFrame (`RECT *, otz, style, xoff`): title tab, border (func_80026BDC) and the teal fill
  (func_800274CC → win_drawHelpFill, a semi-transparent TILE 0x78B0A8). Ablation at `_help`: DF18 removes the help
  window's frame and fill; 274CC leaves only the border. high.

## Sprite / text drawing (main exe)  (status: applied)
- func_800275DC → ui_drawSprite (`s32 (x, y, otz, id, flag)`, sprite table D_800686D0) and func_800270EC →
  ui_drawHand (`(x, y, otz)`, sprite 1). Ablation: 270EC removes only the command window's hand; 275DC removes the hand
  and the scroll arrows. high.
- func_80029058 → text_draw (`(x, y, otz, u8 *s, pal)`): ablation: the command names go. high.
- func_8002B524 → ui_drawLabel (`(x, y, otz, str, mode)`) with packet builder func_80029B4C → ui_buildLabel: ASCII
  A-Z a-z 0-9 & . _ in the small title font; ablation: "COMMAND", "NAME", "HP", "MP", "ATB" go. high.
- func_8002B5B4 → ui_drawDigits (`(x, y, otz, u8 *s, color)`) with builder func_8002B644: ablation: the HP and MP
  numbers go. func_8002B56C → ui_drawSmallDigits (builder func_80029D60): ablation at `_ablist`: the MP cost
  column goes. high.
- func_8002B5FC → text_drawBlock (builder func_8002920C): ablation at `_help`: the help text goes. high.
- func_80025ED4 → ui_drawGauge (`(x, y, otz, cur, max, style, blink)`): ablation: the ATB and trance bars go. high.
- func_80025A28 → ui_drawRule (`(x, y, otz, style, w, abr)`): ablation: the panel's header bar goes. high.
- func_80026C34 → ui_drawItemIcon (`(x, y, otz, itemId, flag)`): ablation at `_itemlist`: the potion icons go. high.
- func_8002AD38 → text_append (`u8 *(u8 *dst, u8 *src)`): appends src after dst's 0xFF terminator (skipping
  multi-byte codes). Ablation at `_help`: the "Change Defend Skip" key line is missing. high.

## Battle flow (ovl_0b)  (status: applied)
- func_800A8600 → btl_main (`void (s32)`): battle mode loop: btl_beginFrame, CD/UI checks, btl_update, btlui_init
  once (Battle.unkD85 & 7 == 7), func_80023A4C / func_80032098 / preload_update, btl_endFrame, until GameState.unk0 &
  0x301; then func_800A94F8 (cleanup, coverage: only at the end of the victory run). Never hit by coverage started
  mid-battle: it is the long-running caller (every per-tick function's ra chain ends in it). confidence: high.
- func_800AEA48 → btl_beginFrame: ClearOTagR of the frame's OTs. func_800AEE68 → btl_endFrame: soft reset check
  (L1+L2+R1+R2+Start+Select = 0xF09 sets GameState.unk0 bit 0), DrawOTag, buffer flip. Both 375x per 1500 frames
  (one per battle tick = every 4th vsync). confidence: high (code + rate).
- func_800AF2E0 → btl_update (`s32 (void)`): switch on Battle.unkD80 (phase): 1/3 entrance (func_800AF578), 4
  combat (btl_updateCombat), 5 victory (btl_victory), 6 exit. Runtime watch on Battle+0xD80 (0x80109580) in the
  victory run: 4 during combat, -> 5 written by func_800C0EC8+0x294 when the Malboro died, -> 6 by
  func_800AF800+0x5F4 ~30 frames later. confidence: high.
- func_800B0468 → btl_updateCombat (`(GameData *, Battle *)`): per unit btl_projectUnitPos, btl_updateUnitModel,
  func_800B3460 (ATB/menu), btl_updateHpStatus, func_800BFAF4; then func_800B1764 and func_800B05C0 (scene draw).
  375x/1500 frames in every combat run. confidence: high.
- func_800AF800 → btl_victory: phase 5; coverage only in the victory run (8 hits); sets phase 6. high.
- func_800C0EC8 → btl_checkSideDefeated (`(BtlUnit *)`): returns unless every unit on the unit's side is KO/stone/etc.
  Enemy side down: phase 5 (victory). Party side down: Rebirth-flame chance via item 0xF9, else message 5 and the
  defeat path. Runtime: the write of 5 above. high.
- func_800BD93C → btl_updateHpStatus (`(BtlUnit *)`): every tick per unit: removes LOW_HP (0x200) when hp*6 > maxHp
  (clamping hp), adds KO at hp 0, adds LOW_HP otherwise, clamps mp. Coverage: 5 calls per tick (one per unit); trace
  shows btl_removeStatus(unit, 0x200) from it every tick. high.
- func_800BF558 → btl_removeStatus (`s32 (BtlUnit *, u32 mask)`): clears status bits with per-status side effects
  (KO revive resets the motion, Berserk/Confuse reset ATB, ...); returns 1 if nothing to do. Runtime (`_calm`, Malboro
  acting): (Malboro, 0x8000) from func_800B1520 when its command is set (Defend ends when the unit acts), plus the
  per-tick (unit, 0x200). The physical-hit path (func_800B7954) removes 0x400/0x20000 (Confuse/Sleep) and 0x4000000.
  23 refs. high.
- func_800B1520 → btl_setEnemyCmd (`(s32 unitId, s16 targets, u32 cmdNo, u32 ability)`): Runtime: (0x10 = Malboro,
  1 = Zidane, 0x2F, 7) from evt_execInstr+0x5D38 and the Malboro casts Osmose (banner "Osmose") on Zidane. Enemy
  AI runs on the event-script VM. Confused/berserk units get random targets. high.
- func_800BED10 → btl_getUnitById (`BtlUnit *(u16 bit)`): unit whose unkB0 target bit matches (0x10 = Malboro above).
  medium (code).
- func_800BEC64 → btl_getUnitMask (`u16 (u32 side)`): OR of unkB0 for 0 party, 1 enemies, 2 all; called every tick by
  the event VM (func_80048E80). medium.
- func_800BDED4 → btl_getUnitAttr (`u32 (BtlUnit *, s32 attr)`) and func_800BE1E0 → btl_setUnitAttr: event-script
  unit variables (0x23 maxHp, 0x24 hp, 0x25 maxMp, 0x26 mp, 0x27 maxAtb, 0x29 level, 0x2A-0x2F status words,
  0x30-0x33 element bytes unk70-unk73). Runtime: after Osmose the Malboro's AI calls btl_setUnitAttr(Malboro, 0x30,
  8) and (Malboro, 0x33, 0x20) from func_8004B7A4 (event VM). high.
- func_800B065C → btl_tryCounter (`s32 (BtlUnit *target, BtlUnit *attacker, BtlCmd *)`): Counter (support bit, chance
  from unk33, doubled by another bit) queues cmd 0x31 Attack 0xB0 against the attacker; Return Magic queues 0x32
  with the same ability. Runtime: called from func_800B7954 when Osmose hit Zidane; in the Ark run the Ark lost HP
  while nobody attacked (Zidane's counters). medium.
- func_800B97A0 → btl_killUnit: hp 0, death flag 0x100000, death motion. medium (code; called by btl_damageHp at 0 hp).
- func_800B3EF8 → btl_isCmdQueued (`u8 (BtlUnit *, u8 cmdNo)`), func_800B4040 → btl_cancelUnitCmds (`s32 (BtlUnit *)`):
  queue search / removal of a unit's commands. medium (code).
- func_800BDC04 → btl_healHp (`(BtlUnit *, s32)`): coverage only in the Cure and Potion runs. func_800BDC64 →
  btl_damageMp and func_800BDCBC → btl_healMp: only in the Osmose run, called (Zidane, n) and (Malboro, n) from
  func_800B7954. high.
- func_800B7F20 → btl_calcSteal: only in the Steal run; shows "Stole Ore!" through btl_showMessage(0xC, name). high.
- func_800B4234 → btl_projectUnitPos (`(BtlUnit *)`): RTPS of the unit's bone into unkB8/unkBA (screen x/y), sets
  unkBC. Coverage: 15 calls in 60 frames only while the target hand is shown (and during actions). medium.
- func_800BCAA4 → btl_startHitMotion: hit-reaction motion, unk68 |= 0x20000; from btl_damageHp. medium (code).

## Damage numbers / popups  (status: applied)
- func_800CC044 → btl_spawnPopups (`(BtlUnit *)`): from the unit's unk7E flags and unk80/unk82 (hp/mp change) starts
  popups: kind 0 HP number (func_800CC31C → btl_popupHp, 0xC0 = heal color), kind 1 MP (func_800CC37C → btl_popupMp),
  kind 2 text (func_800CC3E0 → btl_popupText: Miss/Guard/Critical, D_800F6A54), then clears unk80/unk82.
  Runtime: the only reader of the Malboro's unk80 (0x801092BC) after Zidane's Attack (3233 damage) is
  func_800CC044+0xF4, called by func_800E8E84 (effect script). high.
- func_800CC4F0 → btl_drawPopups (`u8 *(u8 *prim)`): walks the 16-entry popup ring (Battle+0x1358), projects each,
  draws through D_800F6A7C[kind]. Ablation at `disc3_malboro_dmgnum` (state saved while "3233" floats over the
  Malboro): the number disappears. high.
- func_800CE1B4 → btlbg_draw: ablation at `disc3_malboro_dmgnum`: the whole battle background goes black (units,
  number stay). It is btlbg_drawRecord's caller. high.
- D_800F78A4 → g_btlfxEnv (applied): effect environment (unk0 effect header, unkC BtlUnitList, unk14 camera matrix,
  unk18/1C disp/draw env, unk20 OT, unk24 next primitive), 285 refs. medium (code, name only).

## ATB, per-unit status ticks, command execution  (status: applied)
- func_800EAAFC → btl_updateAtb: every vsync (from btlui_updateMenu); for each live unit atb += speed (BtlUnit28.unk6)
  up to maxAtb; a full player opens its menu (func_800EB358) or auto-acts under Berserk/Confuse (func_800B3B5C), a
  full enemy runs its AI script 0x2F (func_8004AB7C). Runtime: write watch on Zidane's atb (0x80108F28) after his
  Attack: written at func_800EAAFC+0xC0/+0xE0 (fill, clamp) and func_800B2988+0xA8 (reset after acting). high.
- func_800EA038 → btl_isAtbRunning (`u8 (void)`): false before the UI is up; with the Wait option (GameState.unk24
  bit 3) only while menuState < 2. Gates btl_updateAtb, status ticks and command start. medium (code).
- func_800E9FCC → btlui_getActiveUnit: g_btlUi->cmdUnit when a menu is open, else NULL. medium (code).
- func_800B3460 → btl_drawActiveMarker (`(GameData *, Battle *, BtlUnit *)`): the spinning yellow pyramid above the
  unit whose menu is open. Ablation at `_calm`: the yellow marker above Eiko disappears. high.
- func_800BFAF4 → btl_updateUnitStatus (`(BtlUnit *)`): per tick per unit: tint, KO handling (func_800BCEF0, 364 hits
  in the victory run while the Malboro dies), Poison/Venom/Regen damage ticks, trance timers, then
  btl_tickStatusTimers. medium (code + coverage).
- func_800C0DAC → btl_tickStatusTimers: counts down unk46[16] for the timed statuses (bits 16-31), removing them when
  they expire (Doom-style bit 0x8000000 kills, bit 31 adds Petrify). medium (code).
- func_800BFF20 → btl_updateUnitTint: unit color tint for statuses (0xC00000 flashing, trance color per character
  from an 8-entry table, Stop/Freeze 0x3000840 via func_800C8A3C). medium (code).
- func_800B1764 → btl_processCmdQueue (`(Battle *)`): takes the first runnable command from Battle.cmdQueue into
  curCmd (btl_removeCmd) and steps it through Battle.unk11BA phases (checks, btl_payMpCost, execution). Coverage:
  every tick; the queue functions in the action runs follow from it. medium-high.
- func_800B05C0 → btl_drawScene: enemy battle sequences (func_800CA7CC = btlseq_update; campaign 2 corrected the
  earlier "camera" reading), btlbg_draw, effects (func_800E3EEC), btl_drawPopups,
  func_800F4E70. high (call structure + the ablations of its callees).

## btl_calcCmd formula steps  (status: applied except btl_rollSummonLength? (low); code-derived; runtime = which runs execute them)
- binary ovl_0b, all `(BtlCalc *)`. btl_calcCmd(caster, target, cmd, formula) switches on the ability's formula:
  runtime formula numbers: Attack 1, Osmose 0xF (Malboro), self-targeted Malboro reaction 0x40 (trace of
  btl_calcCmd's a3). BtlCalc fields as used: unkE base power, unk10 defence, unk12 evade %, unk14 multiplier,
  unk16 hit %, unk18/unk1B result flags, unk1E final value.
- Coverage columns: A = Attack/Mug/killing blow runs only, M = Fire and Ramuh only, H = Cure and Potion only,
  E = Osmose only.
- func_800B9418 → btl_checkFrozenShatter (A): a hit on a Freeze (0x02000000) target kills it (hp 0, message 0x13).
- func_800B9AE4 → btl_initHitRates (A): hit 100, evade = target unk6D.
- func_800B9C14 → btl_hasSureHit (A): caster support bit 0x200; when set the hit roll is skipped.
- func_800B8390 → btl_rollHit (A): halves hit for caster Blind-type status 0x410 / target Defend 0x8000 / support
  0x400, zero evade for some statuses, rolls hit and evade; 0 = miss (sets result flag 1).
- func_800B87D0 → btl_setPhysParams (`(BtlCalc *, u8 kind)`) (A): power from weapon (unk74[5]), defence = target
  unk6C, multiplier = strength + random by kind 1-5,7 (mixes with magic/spirit/speed).
- func_800B8A24 → btl_applyKillerBonus (A): x1.5 when a caster support bit matches the enemy's category bits, and the
  MP-attack bonus (support 0x1000 with mp > 0, sets flag 4).
- func_800BA2AC → btl_applyCasterStatusMod (A): x1.5 under 0x4800 (trance/berserk), 1 under 0x10000000 (mini).
- func_800BA3FC → btl_applyTargetStatusMod (A): halves under target 0x808000 (Defend/Protect), x1.5 under 0x10020000.
- func_800BA534 → btl_applyRowMod (A): x1.5 when attacking from behind (facing equal) or in a back attack, halves
  for row distance >= 2 unless long-range.
- func_800BA660 → btl_applyWeaponElementBoost (A) and func_800BA8F0 → btl_applyAbilityElementBoost (M): x1.5 when the
  caster's element-boost bits (unk79) match the element.
- func_800BA6A4 → btl_applyWeaponElement (A) and func_800B8D58 → btl_applyAbilityElement (M): target absorb / immune
  (unk70, result flag 0x40 = "Guard" popup) / half (unk72) / weak (unk73) bytes vs the element; fire/ice elements
  remove Freeze/Heat-type statuses.
- func_800B8BE8 → btl_rollCritical (A): chance from spirit/4, x2 and flag 4 (the "Critical" popup).
- func_800B8E9C → btl_finishPhysDamage (A): (power - defence) * multiplier capped at 9999 into unk1E.
- func_800BAE44 → btl_payMpAttack (A): takes maxMp/8 from the caster when the MP-attack flag was set.
- func_800B81C4 → btl_gainTrance (`(BtlCalc *, u8)`) (Attack, Mug, Fire, Ramuh runs; returns early unless an enemy hits a party member): enemy hits on a party member add
  random(unk33) to the trance gauge (unk78), at 0xFF btl_addStatus(target, 0x4000 = trance). Runtime (`disc3_ark_lv99_cmd`, Ark's
  attack hits all four): func_800B81C4 called 4x from func_800B7954+0x318, and the one for Eiko (0x8010916C) calls
  btl_addStatus(Eiko, 0x4000) from func_800B81C4+0x1B4; Eiko glows in trance afterwards. high.
- func_800B9F80 → btl_setMagicParams (M, H, E): multiplier = magic + random, power = ability power (unk8[5]),
  defence = target magic defence (unk6E).
- func_800BA468 → btl_applyCasterMiniMagic (halved under 0x10000000), func_800BA4AC → btl_applyTargetShell (M, E:
  halved under target 0x400000).
- func_800BAA28 → btl_finishMagicDamage (M): (power - defence) * multiplier, x2/3 for spread casts, cap 9999.
- func_800BABAC → btl_rollAbilityStatus (M): adds the ability's status set (unk8[9]) with chance unk8[7].
- func_800BA994 → btl_calcHeal (H): power * multiplier capped at 9999, heal flags.
- func_800BA4F0 → btl_applyHealBoost (H, Cure only): x1.5 for a caster support bit.
- func_800B9DA4 → btl_checkTargetAlive (E): fails on target 0x101; func_800B9E20 → btl_checkNotSelf (E).
- func_800B989C → btl_rollSummonLength? (`(BtlCmd *)`) (Ramuh only): random chance (0xE6/256 with mp > 2*cost, else
  0xAA/256) sets cmd flag unk10.b.unk0_10. Which animation the flag selects is not confirmed. low.
- func_800B7954 → btl_applyCalcResult: applies BtlCalc to the target (counter check, miss/guard flags, status side
  effects, btl_damageHp / btl_healHp / MP changes); every action run. high (it's the btl_damageHp caller of record).
- confidence: medium for all of the step names (code); the coverage only places them in the right formula.

## Ability-name banner  (status: applied)
- State `disc3_malboro_fire_banner` (Vivi casting Fire, "Fire" banner at the top).
- func_800F4E70 → btl_drawBanner (`u8 *(u8 *prim)`): while D_801051F0.unk8C is set draws the banner and counts
  unk80 up to unk84 (duration), then clears it. Ablation: the banner disappears. Called by btl_drawScene. high.
- func_800F5064 → btl_buildBanner (`u8 *(u8 *)`): centred text (func_8002920C) plus frame. Ablation: banner gone. high.
- func_800F511C → btl_buildBannerFrame (`u8 *(u8 *, RECT *)`): border + fill. Ablation: only "Fire" remains. high.
- func_800F4F04 → btl_setBanner (`(s32 text, s32 priority)`): replaces the banner text if priority >= the current
  one. Runtime: btl_showMessage passes the formatted "Stole Ore!" text to it with priority 1. high.
- D_801051F0 → g_btlBanner (text at +0, unk80 timer, unk84 duration, unk8C active, unk90 priority). medium.

## Effect-script unit motion helpers  (status: applied)
- ovl_0b, unit index argument (btl_getUnit; 0xFF = 0 in D8A64). Runtime: at `disc3_malboro_fire_banner` (Fire cast
  starting) func_800D85AC runs once from func_800E52B4, calling the other three; func_800D87D0 runs 4x.
- func_800D8A64 → btlfx_isPartyUnit (`s32 (s32 unit)`): unk68 & 1. 22 refs.
- func_800D8740 → btlfx_getUnitMotion (`s32 (s32 unit, s32 slot)`): unkA4[slot] (the unit's motion table).
- func_800D86E0 → btlfx_setUnitMotion (`(s32 unit, s32 motion)`): party units only, sets unk9C->unkC and restarts
  the frame counter (unkA0->unk3A = 0).
- func_800D85AC → btlfx_playUnitMotion (`(s32 unit, s32 slot)`): the two above for party units. 20 refs.
- func_800D87D0 → btlfx_getMotionLength (`s32 (s32 unit)`): unk9C->unkC->unk2, the frame count that
  func_800BD31C compares unkA0->unk3A against to detect the end of a motion. 18 refs.
- confidence: medium (code; runtime only shows they run when a spell starts).

## Battle campaign 2 (2026-10-17): method, runs, helper scripts  (status: reference)
- Same setup as campaign 1 (Redux disc 3, `disc3_malboro_lv99_calm`, Malboro ATB/HP frozen by `build/rt/freeze.lua`).
  Every action is Zidane's Attack rewritten by `tools/rt/patch.lua` into another command/ability (command ids from the
  ovl_0b command table D_800FD56C: 4 Defend, 5 Flee, 7 Change, 0xD Focus, 0xF Throw, 3 Jump, 8 Eat, 0x11 Wht Mag,
  0x16 Blk Mag, 0x18 Blu Mag, 0x19 Skill; ability ids from the battle ability table Battle+0x16CC, names decoded from
  D_800F909C: 0x1C Sleep, 0x20 Slow, 0x24 Stop, 0x25 Poison, 0x0E Silence, 0x0F Mini, 0x11 Confuse, 0x12 Berserk,
  0x13 Blind, 0x2B Death, 0x2C Break, 0x51 Doom, 0x0D Haste, 0x0C Protect, 0x0B Shell, 0x10 Reflect, 0x14 Float,
  0x04 Regen, 0x0A Esuna, 0x64 Auto-Life, 0x60 Vanish, 0x03 Curaga, 0x1D Blizzard).
- Scratch helpers (not tools; `build/rt2/`): `spell.py`/`spell2.py NAME CMD ABIL [--tgt] [--trace f,..] [--nocov]`
  (patched action + coverage and/or argument traces), `who.py`/`callers.py FUNC..` (hit counts per run and the distinct
  callers recorded by `cov.lua`; `MODULE` = a return address inside the CD-loaded effect module at 0x801E7700-0x801F1700),
  `tsum.py` (summary of the trace runs), `abil.py`/`cmds.py` (ability/command table dumps), plus prepared but not yet run:
  `oplog.lua`/`oplog.py` (log effect-script and battle-sequence opcodes), `ablate2.py` (stub a function mid-action),
  `entry.py` (coverage from btl_main's first call), `batch3.sh` (defeat, trance, Reflect bounce, L1+R1 flee).
- Coverage runs added (`build/rt/cov_*.json`, 600-1200 frames from the patched action): c_defend, c_change, c_flee
  (Flee command 5), c_focus, c_throw, c_jump, c_eat, s_sleep, s_slow, s_stop, s_poison, s_silence, s_mini, s_confuse,
  s_berserk, s_blind, s_death, s_break, s_doom, p_haste, p_protect, p_shell, p_reflect, p_float, p_regen, p_esuna,
  p_autolife, p_vanish, p_curaga (party targets: Vivi, Curaga all four), b_blizzard. Together with campaign 1's runs:
  49 runs. Trace runs (`build/rt2/trace_t_*.json`, no coverage): t_sleep ... t_float, 16 spells (the emulator hung during
  t_regen, see the end of this section).
- Emulator note: during t_regen Redux logged "Lua Breakpoint invoker threw an exception, deleting breakpoint" followed
  by "Unknown error in event listener" and the web server stopped answering (the modal-dialog case in docs/emulator.md).
  `tools/emu.py stop` cannot stop it in that state; the instance has to be killed before the next session.

## Battle STATUS bits (BtlUnit.status)  (status: applied — BTL_STATUS_* in include/ovl_0b.h; 0x4 left unnamed)
- binary ovl_0b. Runtime: trace of btl_addStatus while the patched spell hits (t_* runs, all from func_800C0ACC+0x48):
  Break 0x00000001 (petrify), Silence 0x8, Blind 0x10, Confuse 0x400, Berserk 0x800, Stop 0x1000, Poison 0x10000,
  Sleep 0x20000, Haste 0x80000, Slow 0x100000, Float 0x200000 (on Vivi), Shell 0x400000, Protect 0x800000,
  Mini 0x10000000, Reflect 0x20000000. Known before: KO 0x100, low HP 0x200, Trance 0x4000, Defend 0x8000 (every action
  in the t_* runs starts with btl_removeStatus(Zidane, 0x8000) from btl_setCmd+0x1F4), Freeze 0x02000000.
- From code (consistent with the above): 0x2 = Venom (the per-tick handler drains HP and MP, func_800BDD24 +
  func_800BDE78), 0x20 = Trouble (func_800BD774 splashes half the damage on allies), 0x40 = Zombie (heals hurt, drain
  reversed), 0x80 = immune to instant death/gravity (formulas fail on it), 0x2000 = Auto-Life (KO queues revive cmd
  0x3D), 0x01000000 = Heat (acting while it is set adds KO in btl_processCmdQueue), 0x04000000 = Vanish (hides the
  meshes, func_800BD414), 0x08000000 = Doom, 0x40000000 = Jump (in the air), 0x80000000 = gradual petrify.
  Bits 0x8000000|0x80000000 are the two shown with a countdown number (func_800CD210).
- confidence: high for the traced bits, medium for the code-derived ones (0x4 unidentified).

## Battle sequence (enemy action script) interpreter  (status: applied; btlseq_opReleaseEffect and btlseq_opSetStance applied later with [code] evidence in the ovl_0b cleanup pass)
- binary ovl_0b. Correction to campaign 1: func_800CA7CC is not camera code. It runs up to four sequence slots
  (Battle11C4.unk68[4], BtlScript) and dispatches one-byte opcodes through D_800F5858[35] = {onEnter, step} pairs.
- func_800CA7CC → btlseq_update (`void (void)`): once per battle tick from btl_drawScene+0x3c (375 calls / 1500 frames
  in `idle`). Runtime: its opcode handlers are only ever called from func_800CA7CC+0x110 (onEnter) / +0x134 (step), and
  only in runs where an enemy acted (`enemy` = Malboro's Osmose, `attack` = Ark's attack, `c_jump`); player actions
  never execute a handler. high.
- D_800F5858 → g_btlseqOps (`BtlScriptOp[35]`). high.
- func_800CA530 → btl_startEnemySeq (`(BtlCmd *)`): called from btl_startCmdEffect+0x30c (the cmd 0x2F/0x35-0x37 =
  enemy AI command case) in the enemy runs only; starts a slot when the battle data has a sequence for subNo. high.
- func_800CA5AC → btlseq_start (`Battle122C *(BtlCmd *)`): allocates one of the 4 slots (pc = sequence table[subNo]);
  runtime caller func_800CA530+0x44 only. high.
- func_800CA694 → btlseq_startDirect (`(u16 unitMask, s16 targets, u16 seqId)`): event-script opcode 0xE5 (main
  evt_execInstr) runs a sequence without a command. medium (code).
- func_800CA774 → btlseq_isBusy (`s32 (void)`): 1 while any slot is active. medium (code).
- func_800CA3CC → btlseq_init: resolves the sequence offsets and loads the animation list from the battle data,
  clears the 4 slots. medium (code; battle load only).
- Opcode handlers (op: onEnter / step). Runtime column = runs that executed it.
  - op00 func_800CAAD0 → btlseq_opEnd: waits for the animation/effect/popups, then btl_onCmdEffectDone and frees the
    slot (enemy 122 calls, attack 26). medium-high. op18 func_800CAC24 → btlseq_opEndNoAnimWait. medium.
  - op01 func_800CAC98 → btlseq_opWait (sets the counter from pc[1]) / func_800CACAC → btlseq_stepWait. medium-high.
  - op02 func_800CACD8 → btlseq_opExecCmd: btl_execCmd on every unit in the target mask (enemy: 1 call = Osmose's
    damage). high.
  - op03/op1E func_800CAD6C → btlseq_opMoveToTarget, func_800CAEC4 → btlseq_stepMove; op04 func_800CAF90 →
    btlseq_opMoveHome; op13 func_800CB008 → btlseq_opMoveTo (absolute), func_800CB090 → btlseq_stepMoveSetHome;
    op1B func_800CB0F0 → btlseq_opMoveBy (relative). medium (code: lerp of BtlUnit.unk8C over pc[1] frames).
  - func_800CB17C → btlseq_findUnit (first unit in a mask), func_800CB1D4 → btlseq_getTargetCenter (average x/z of the
    target units; runtime from func_800CBA58). medium.
  - op05 func_800CB2E0 → btlseq_opPlayAnim (enemy 4, attack 2). medium-high.
  - op06 func_800CB3A4 → btlseq_opDelayedEffect (effect id, arg, delay; started by btlseq_update when unk25 runs out).
    medium.
  - op07 func_800CB4E8 → btlseq_opWaitAnim (enemy 60 calls). medium-high.
  - op08/op1A func_800CB3EC → btlseq_opEffect: the enemy run's only btl_playCmdEffect call came from
    func_800CB3EC+0xC8. high.
  - op09 func_800CB560 → btlseq_opWaitEffectHit (waits for slot flag 2, set when the effect raises its hit flag).
    medium. op0B func_800CB5C4 → btlseq_opWaitEffect. medium. op0A func_800CB594 → btlseq_opReleaseEffect
    (g_btlfxState.unkF = 1). low-medium.
  - op0C func_800CBBB0 → btlseq_opScale / func_800CBC38 → btlseq_stepScale (unit attr 0x37). medium.
  - op0D func_800CB618 → btlseq_opHideMeshes, op0F func_800CB68C → btlseq_opShowMeshes (both once in the Ark run;
    they call func_800BD414 / func_800BD464). medium.
  - op0E/op21 func_800CB700 → btlseq_opShowName (banner: the ability name for 0x80, else enemy text; 1 call in the
    Osmose run, whose banner showed "Osmose"). medium-high.
  - op10 func_800CB848 → btlseq_opSetCamera (Battle.unk131C). medium. op12/op20 func_800CB884 →
    btlseq_opPlayCamera (50 % chance unless the camera option is off, then btlcam_playSeq). medium.
  - op11 func_800CB808 → btlseq_opSetStance (unk68 bit 18 = which idle animation pair). low-medium.
  - op14 func_800CBA58 → btlseq_opTurn. medium.
  - op15 func_800CBD28 → btlseq_opPlayTexAnim, op16 func_800CBDC8 → btlseq_opLoopTexAnim, op17 func_800CBE68 →
    btlseq_opStopTexAnim. medium.
  - op19 func_800CBEB4 → btlseq_opDelayedSound (btl_playUnitSound after a delay). medium.
  - op1D func_800CBF28 → btlseq_opFadeOut (btl_setUnitFade). medium.

## Command execution phases  (status: applied)
- binary ovl_0b; btl_processCmdQueue steps Battle.curCmd through unk11BA = 0 check, 1 start effect, 2 wait, 3 finish.
- func_800C28F4 → btl_startCmdEffect (`(BtlCmd *)`): phase 1, once per action in every action run (caller
  btl_processCmdQueue+0x328). Picks the effect: Attack 0x64 + weapon type, Steal per character, Item/Throw by item
  class, enemy commands → btl_startEnemySeq, Eidolon ids; Defend (4) and Change (7) only show the name and set a motion
  (runtime: c_defend/c_change reach it but never btl_playCmdEffect). high.
- func_800C2598 → btl_playCmdEffect (`(BtlCmd *, s32 effectId, s16 *params)`): fills the effect target list
  (Battle.unk116C, reflect retargeting via func_800B3188), resets the effect system and calls btlfx_start(effectId).
  Runtime: in all 16 traced spells a1 equals the effect id btlfx_start receives (Sleep 0x1A, Slow 0x7E, Stop 0x95,
  Poison 0x15, Silence 0x2A, Mini 0x19, Confuse 0x60, Berserk 0x0E, Blind 0x9E, Doom 0xEC, Break 0x99, Haste 0x59,
  Protect 0x7B, Shell 0x3F, Reflect 0x4F, Float 0x1B). high.
- func_800B1B48 → btl_checkCmdUsable: phase 0 checks with failure messages (0x17, 0x18 no item left, 0x1B Dagger can't
  concentrate); Jump sets status 0x40000000. medium-high (code + called once per action).
- func_800B23B4 → btl_validateTargets: drops dead targets, re-targets a random living unit. medium.
- func_800B253C → btl_updatePartyCmd: phase 2 per tick for party actions; plays the Defend kneel and the Change
  row-walk (moves unk8C z by 400 over the motion, then btl_execCmd). medium.
- func_800B3C00 → btl_onCmdEffectDone: phase 3 (also from btlseq_opEnd and btl_updatePartyCmd); a reflected spell
  (cmd unk10 bits 7-8 = 2) replays the effect on the bounce targets. medium.
- func_800B2988 → btl_finishCmd: resets the actor's ATB (campaign 1: the write at func_800B2988+0xA8), drains the trance
  gauge per action, ends Jump. high.
- func_800BD31C → btl_waitHitMotions (`s32 (BtlCmd *)`): 0 while any unit is still in its hit-reaction motion
  (unk68 bit 17); finishes them via the two functions below. medium.
- func_800B3188 → btl_applyReflect (`u16 (BtlCmd *)`): for reflectable abilities, targets with status 0x20000000 bounce
  the spell to random units of the other side (unk1C[]). medium (code; once per spell from btl_playCmdEffect+0xD8).
- func_800B4478 → btl_orTargets, func_800B3CEC → btl_clearCmd, func_800B3E7C → btl_isCmdPending (queued or current),
  func_800B3D54 → btl_queueAutoCmd (slot unk8; runtime: the counter-attack from btl_tryCounter+0x190),
  func_800B4114 → btl_removeUnitCmds, func_800B41A4 → btl_removeQueuedCmd(unit, cmdNo), func_800B4434 →
  btl_releaseItem (un-reserves a queued item; runtime c_throw), func_800B2DD4 → btl_flushCmdQueue, func_800B2F24 →
  btl_clearCmdQueue (runtime: victory and the "all enemies stopped/petrified" runs), func_800B0EF8 →
  btl_initCmdQueue, func_800B10F8 → btl_initUnitCmds (6 slots per party unit, 3 per enemy). medium.
- func_800B3B5C → btl_autoAttack (Confuse: random side, Berserk: enemies; Attack 0xB0). medium (code).
- func_800CDEF0 → btl_showCmdName (ability/item name banner; runtime from btl_startCmdEffect in 37 runs, including
  Defend and Change). high. func_800EA0B4 → btl_getAbilityName, func_800CD75C → btl_getItemName,
  func_800CD70C → btl_getElementName (bit index → text), func_800CD78C → btl_getUnitName. medium-high.

## Status application, ticks and looks  (status: applied; btl_makeGreyCluts applied later with [code] evidence in the ovl_0b cleanup pass)
- func_800C0ACC → btl_addStatuses (`u32 (BtlUnit *, u32 mask)`): btl_addStatus per bit, keeps the best result. Runtime:
  every traced status spell adds its bit from func_800C0ACC+0x48; called from btl_rollAbilityStatus and func_800BACB4.
  high.
- func_800C0B6C → btl_removeStatuses (`s32 (BtlUnit *, u32 mask)`): runtime Esuna (p_esuna, via func_800BAD64), victory.
  high.
- func_800C0C00 → btl_initDotTimer (`(BtlUnit *, s32 bit)`): Venom/Poison/Regen tick timers from spirit; runtime only
  in s_poison and p_regen. high.
- func_800BDD24 → btl_tickPoison (maxHp/16 damage): s_poison only. high. func_800BDDA0 → btl_tickRegen (maxHp/16 heal,
  damage for undead): p_regen only. high. func_800BDE78 → btl_tickVenomMp (maxMp/16). medium.
- func_800CC1A0 → btl_spawnTickPopups (numbers for the ticks above): s_poison/p_regen only. high.
- func_800BD8F0 → btl_resetAtbSpeed (unk2E = 8/10/14 by the battle-speed option): runtime from btl_addStatus in
  p_haste and s_slow only. high.
- func_800C02D4 → btl_updateStatusColor (Zombie/Berserk/Heat/Freeze tint into unkC1). medium.
- func_800C0430 → btl_setStoneClut (`(BtlUnit *, s32 on)`): shifts the model CLUT rows (petrify); runtime s_break. medium.
- func_800C0544 → btl_makeGreyCluts (StoreImage, grey, LoadImage). low-medium.
- func_800C0A70 → btl_interruptUnit (cancel commands, ATB 0). medium. func_800C09EC → btl_clearUnitStatus. medium.
- func_800BD414 → btl_hideMeshes (`(BtlUnit *, u16 mask)`), func_800BD464 → btl_showMeshes: runtime p_vanish
  (btl_addStatus+0x49c → hide) and the Ark sequence. medium-high. func_800BD4DC → btl_hideWeapon, func_800BD53C →
  btl_showWeapon (c_jump via func_800D8C08). medium.
- func_800E8408 → btlfx_reapplyMini, func_800E8460 → btlfx_reapplyVanish (after effects restore party models). medium.
- func_800CCDB8 → btl_drawStatusIcons: per tick from btl_drawPopups+0x284; for each unit and each entry of the 12-entry
  table D_800F6A88 (status mask, exclusion mask, sprite anim, bone) draws the status animation at a bone. medium
  (code + call site; ablation planned). D_800F6A88 → g_btlStatusIcons. medium.
- func_800CD210 → btl_drawStatusCountdown: Doom / gradual-petrify counter digits over units (from btl_drawPopups+0x1c0).
  medium.
- func_800BD774 → btl_spreadTroubleDamage. medium (code).
- func_800C1704 → btl_checkEnemiesKO: runtime s_death (the Malboro died from Death) and victory, from btl_addStatus's KO
  case; flushes the queue when no enemy is left. medium-high. func_800C15DC → btl_checkPartyDisabled. medium.
- func_800BCEF0 → btl_updateKo: KO state machine (death motion, sounds 0x79/0x78, fade-out, rewards, unit removed);
  runtime 364 calls while the Malboro died (victory), also s_death/s_stop/s_break. high.
- func_800BC78C → btl_setIdleMotion, func_800BCC64 → btl_endPartyHitMotion (attack run), func_800BCDBC →
  btl_endEnemyHitMotion (8 damage runs). medium.
- func_800BC21C → btl_updateUnitModel (`(BtlUnit *, s32 *ot)`): per unit per tick from btl_updateCombat, btl_victory
  and func_800AF578 (rotation/translation matrix, motion end handling). medium.

## Ability formula steps (btl_calcCmd)  (status: applied)
- binary ovl_0b, `(BtlCalc *)` unless noted. Runtime = runs whose coverage contains the function.
- func_800B8664 → btl_rollStatusHit (hit % then evade %, else miss flag): every s_* run. high.
- func_800B9AFC → btl_initStatusHitRate (level + accuracy + magic/4 - target level; evade = magic evade): every s_* run.
  high.
- func_800B9C30 → btl_halveHitForShell (target 0x400000), func_800B9C74 → btl_halveHitForSpread: s_* runs. medium-high.
- func_800BACB4 → btl_applyAbilityStatus (status set of the ability via btl_addStatuses; 0 → Guard flag, 1 → miss): all
  s_* and p_* status runs. high.
- func_800BAD64 → btl_cureAbilityStatus: p_esuna only. high. func_800BADD4 → btl_cureItemStatus. medium.
- func_800BA858 → btl_halveForSpread (magic multiplier halved for multi-target casts): Fire, Blizzard, Cure, Curaga.
  medium-high.
- func_800BA7D0 → btl_missIfAirborne (ground-only abilities miss Float or flying enemies): Fire, Blizzard, Ramuh runs.
  medium.
- func_800B918C → btl_rollFlee (escape chance from average levels; success queues cmd 0x38) and func_800BAEA8 →
  btl_canFlee (battle allows escape and someone can act): c_flee only. high.
- func_800B9E50 → btl_guardIfDeathImmune (Guard when target status 0x80): s_death. medium. func_800B9D34 →
  btl_missIfDeathImmune: c_eat. medium.
- func_800BA1B4 → btl_setEnemyAttackParams (power = ability, defence = target, multiplier = strength + random): Ark's
  attack (`attack` run, formula 0x64). medium-high. func_800B9E88 → btl_setAbilityPhysParams (same for player physical
  abilities, formulas 8 / 0x62). medium. func_800BA094 → btl_setWeaponAbilityParams (weapon power * ability / 10).
  medium.
- func_800B9994 → btl_canAddHitStatus (weapon status on hit; from btl_applyCalcResult in the Attack runs). medium.
- func_800BAAE4 → btl_calcPercentDamage (Demi-type: maxHp * rate / 100, 2/3 when spread). medium (code).
- func_800B9A5C → btl_setDrainFlags, func_800B9CDC → btl_checkLevelMultiple (LV-n spells), func_800B9D6C →
  btl_checkTargetIsPlayer, func_800BAF3C → btl_markTrouble (target Trouble 0x20 → popup flag 0x100). medium (code).

## Escape, rewards, end of battle, enemy AI hooks  (status: applied)
- func_800C12CC → btl_addEnemyRewards (exp, gil, item drops 1/256, 32/256, 96/256, card): runtime victory only, from
  btl_updateKo+0x22c when the Malboro died. high. D_800FDF30 → g_btlRewards. medium. func_800C1594 →
  btl_clearRewards, func_800C1524 → btl_initState. medium.
- func_800C1794 → btl_saveParty (victory, btl_update+0x260) and func_800C1854 → btl_saveCharacter (4 calls in victory =
  the 4 party members; writes HP/MP/trance back). high.
- func_800C20B8 → btl_playVictoryMusic (from btl_victory; sound commands 0x8601/0x700). medium.
- func_800C19F8 → btl_tryEscape (message 4 when the battle forbids it, else flee formula 0x38). medium.
- func_800C1184 → btl_rollAmbush (0 back attack, 1 first strike, 2 normal; support bits 0x200/0x400 change the odds).
  medium (code).
- func_8004AB7C (main) → evt_battleTrigger (`s32 (s32 tag, s32 unitBits, s32 attacker, s32 ability)`): runs an enemy's
  event-script tag: 0x2F turn (AI), 0x35 hit/counter, 0x36 death, 0x37 any hit. Runtime callers: btl_updateAtb+0x15c
  (enemy ATB full) and btl_applyCalcResult+0x598/+0x5ac (2 calls after every spell that hit the Malboro, 3 in the
  enemy run, 0 in Defend/Change). medium-high.
- func_800BE784 → btl_getBattleVar (event-script battle variables 32-42: scene flags, gil, phase, attacker, targets...;
  runtime from the event VM func_80037D6C+0x3cc) and func_800BE99C → btl_setBattleVar (32 force victory, 33-35 end
  modes, 38 add gil, ...; event opcode 0x4A). medium-high / medium.
- func_800B0A6C → btl_applyAutoAbilities (Auto-Reflect/Float/Haste/Regen/Life and immunity supports). medium (code).
  func_800B0CF8 → btl_partyHasSupport. medium. func_800B0D4C → btl_tryAutoPotion (runtime from btl_gainTrance+0x6c in
  the Ark run). medium. func_800B081C → btl_findCover (Cover/Protect-type supports). medium (code).
- Unit helpers: func_800C1E88 → btl_hasEnemyFlag (enemy category bits: 0x10 undead, 0x80 flying ...), func_800C1EDC →
  btl_getModelSlot, func_800C1F00 → btl_countActiveUnits, func_800C1F7C → btl_pickRandomUnit, func_800C1CA0 →
  btl_getUnitsWithStatus, func_800C1D8C → btl_getUnitBySlot, func_800C1938 → btl_unlinkUnit, func_800C1990 →
  btl_insertUnit, func_800BED6C → btl_findUnitByBit. medium (code).
- Scan/Detect: func_800CD808 → btl_startScan, func_800CD83C → btl_updateScan (LV, HP/MP, weakness lines),
  func_800CDC68 → btl_startDetect, func_800CDD68 → btl_updateDetect. medium (code; runs planned).

## Battle sound  (status: applied; btl_setUnitBrightness applied after battle campaign 3 raised it to medium)
- func_800AD90C → btl_soundCmd (`s32 (s32 op, s32 id, s32, s32 pan, s32 vol)`): the battle's GameData.unk83C sound hook
  (music 0/7, 0xC streams, 0xD000 sound effects); runtime: menu snd_playSe reaches it through the hook (caller
  snd_playSe+0x38). medium-high.
- func_800C22CC → btl_playUnitSound (`(BtlUnit *, u16 id, u8 vol)`, panned at the unit): runtime death sounds from
  btl_updateKo. high. func_800C2200 → btl_playHitSound (weapon hit sound; z_attack via func_800D8988). medium-high.
  func_800C2250 → btl_playDeathCry (once per unit, from btl_killUnit). medium-high.
- func_800C24E8 → btl_setUnitFade (`(BtlUnit *, s32 level)`): 17 calls while the Malboro faded out. high.
- func_800C1A8C → btl_setUnitBrightness. low-medium.

## Effect system (spell/summon effects)  (status: applied)
- binary ovl_0b. An effect id starts a 3-byte-opcode script (directory at 0x801E7700) run by an interpreter that spawns
  "tasks" (up to 11, callbacks (mode 0 size query, 1 start, 2 step)); opcodes < 0x20 are control, 0x20-0x2F resident
  ops (D_800F8D10), 0x50-0x7F unit/choreography ops (D_800F72E0), >= 0x80 ops exported by the CD-loaded module.
- func_800D62A0 → btlfx_start (`s32 (s32 effectId)`): trace values above; -1 when busy. high. D_800FEC40 →
  g_btlfxState (unk0 state, unk2 resolved id, unk18 script pc, unk1C header flags). medium-high.
- func_800D616C → btlfx_reset. medium. func_800E3DA8 → btlfx_resetAll (once per effect from btl_playCmdEffect). medium.
- func_800D650C → btlfx_runScript: per frame from func_800E3EEC; states 0-4 (save VRAM, read header, load file,
  run ops). high (code + runtime: btlfx_spawnTask's only script caller is func_800D650C+0x808).
- func_800D6360 → btlfx_findScript, func_800D64C8 → btlfx_callScript (weapon sub-script, op 0x7C), func_800D6340 →
  btlfx_retryOp, func_800D63B0 → btlfx_swapVramPages. medium.
- func_800D6E70 → btlfx_spawnTask (`(s32 op, s32 file, s32 a, s32 b)`): 14-44 calls per action. high.
  func_800D7094 → btlfx_updateTasks: per frame. high. func_800D6D3C → btlfx_initTasks, func_800D6D8C →
  btlfx_allocTaskMem. medium-high. D_80100C78 → g_btlfxTasks. medium. D_800F8D10 → g_btlfxResidentOps, D_800F72E0 →
  g_btlfxUnitOps. medium-high.
- func_800DA4F4 → btlfx_loadFile (`s32 (s32 id)`): once per spell from btlfx_runScript+0x1f4; file ids: Sleep 0x4E,
  Slow 0x7E, Stop 0x95, Poison 0xCA, Silence 0x7F, Mini 0x7C, Confuse 0x61, Berserk 0x0E, Blind 0x9D, Doom 0xEC,
  Break 0x99, Haste 0x59, Protect 0x7B, Shell 0x3F, Reflect 0x4F, Float 0x1B. high. func_800DA92C → btlfx_updateLoader
  (per frame). medium. D_80100DC0 → g_btlfxLoader. medium. func_800DA3A4 → btlfx_findFileSlot, func_800DA3D8 →
  btlfx_getFileData (`s32 (s32 slot, s32 idx)`; also called by module code), func_800DA480 → btlfx_getFileDataSize.
  medium-high.
- Particle/element pools exported to modules: func_800DA104 → btlfx_poolSize, func_800DA138 → btlfx_poolInit,
  func_800DA1B8 → btlfx_poolClear, func_800DA1F4 → btlfx_poolSpawn, func_800DA2AC → btlfx_poolUpdate (MODULE callers in
  Haste, Slow, Stop, Doom, Reflect, Ramuh). medium.
- Effect heap: func_800E3B34 → btlfx_heapInit, func_800E3B70 → btlfx_heapAlloc, func_800E3BF8 → btlfx_heapWrapIfFull,
  func_800E3C50 → btlfx_heapFree (empty). medium.
- func_800E4134 → btlfx_reloadCommonData (re-reads file 0x2BF into 0x801E7700 and the VRAM pages after a summon; runtime
  d_summon only). medium.
- func_800E701C → btlfx_setTargets, func_800E72B8 → btlfx_clearTargets (g_btlfxEnv->unkC). medium-high.
  func_800E70D0 → btlfx_beginPrims, func_800E7154 → btlfx_endPrims (prim buffer pointer; used by btl_drawStatusIcons).
  medium.
- func_800E8DC8 → btlfx_execCmd (btl_execCmd on one target or all) and func_800E8E84 → btlfx_showPopups
  (btl_spawnPopups); both from func_800E5DA8 → btlfx_opApplyResult (op 0x25), which runs in every action run. high.
- func_800E4984 → btlfx_startCasterTask (op 0x27 = func_800E4C88 → btlfx_opCast, the caster's cast motion; every
  spell), func_800E5538 → btlfx_opReturnHome (op 0x28; uses func_800E43D8 → btlfx_moveUnitHome), func_800E60EC →
  btlfx_opFadeBackground (op 0x2A), func_800E6270 → btlfx_opFadeUnits (op 0x24; d_summon only; func_800E813C →
  btlfx_fadeUnitGroup, func_800E7FC0 → btlfx_fadeUnit, func_800E7EC0 → btlfx_setUnitBrightness), func_800E5F24 →
  btlfx_opPlayCamera (op 0x29; calls btlcam_play), func_800E5EAC → btlfx_opAddCameraChoice (op 0x23), func_800E45D0 →
  btlfx_opSetFrameRate (op 0x2C). medium.
- func_800E8F40 → btl_setVsyncMode (`g_gameState->vsyncMode = arg`). high (code).
- Primitive helpers: func_800D0858 → btlfx_addPrim (`(u32 len, s32 z, void *)` into the effect OT by depth),
  func_800D08C4 → btlfx_addPrimAbr, func_800D07B4 → btlfx_linkPrimAbr, func_800D090C → btlfx_allocDrMode,
  func_800D2C78 → btlfx_addSpriteQuad, func_800D0A0C → btlfx_drawScreenTint (8 TILEs over the screen; module-called in
  summon, Death, Doom, Stop). medium.
- Unit helpers for effects (unit index, 0x10 = caster): func_800E73EC → btlfx_getUnitHomePos, func_800E750C →
  btlfx_getUnitPos, func_800E769C → btlfx_getUnitBodyPos, func_800E76DC → btlfx_setUnitPos, func_800E77D8 →
  btlfx_getUnitRot, func_800E7824 → btlfx_setUnitRot, func_800E7A44 → btlfx_getUnitScale, func_800E7BA0 →
  btlfx_getStrikePos (point in front of the target at its radius; Attack runs), func_800E72C8 → btlfx_getUnitRadius,
  func_800E788C → btlfx_isEnemyUnit, func_800E7650 → btlfx_applyFloatHeight. medium (code + MODULE callers).
- func_800D8658 → btlfx_resetUnitMotion, func_800D8774 → btlfx_setMotionFrame, func_800D87FC → btlfx_getMotionFrame,
  func_800D8820 → btlfx_turnUnit, func_800D8914 → btlfx_playSwingSound, func_800D8988 → btlfx_playHitSound,
  func_800D8A94 → btlfx_isSameSide, func_800D8ACC → btlfx_isSameUnit, func_800D8B70 → btlfx_getUnitWeapon,
  func_800D8BB8 → btlfx_setUnitVisible, func_800D8C08 → btlfx_setWeaponVisible (MODULE caller in c_jump),
  func_800D8C4C → btlfx_isUnitActive, func_800D8C70 → btlfx_getUnitStatus. medium.
- Choreography ops (onStart / step), runtime in the physical-command runs (z_attack, z_steal, victory, c_eat, c_focus,
  c_jump): op 0x50 func_800D73AC → btlfx_opSetParamsA, op 0x51 func_800D73E4 → btlfx_opSetParamsB, op 0x55
  func_800D7414/func_800D8CB4 → btlfx_opSwingSound/btlfx_stepSwingSound, op 0x56 func_800D74B8/func_800D8CD8 →
  btlfx_opHitSound/btlfx_stepHitSound, op 0x5A func_800D75EC/func_800D8D68 → btlfx_opPlayMotion/btlfx_stepPlayMotion,
  op 0x5B func_800D7660/func_800D8D94 → btlfx_opPlayMotionWait/btlfx_stepPlayMotionWait, op 0x62 func_800D78EC/
  func_800D9038 → btlfx_opResetMotion/btlfx_stepResetMotion, op 0x6A func_800D7B2C/func_800D9260 →
  btlfx_opRunHome/btlfx_stepRunHome, op 0x6C func_800D7C9C/func_800D9488 → btlfx_opJumpForward/btlfx_stepJumpForward,
  op 0x6D func_800D7D64/func_800D955C → btlfx_opJumpHome/btlfx_stepJumpHome, op 0x70 func_800D7F80/func_800D9990 →
  btlfx_opRunToTarget/btlfx_stepRunToTarget, op 0x71 func_800D805C/func_800D9AB0 → btlfx_opFaceTarget/
  btlfx_stepFaceTarget, op 0x72 func_800D8124/func_800D9D58 → btlfx_opFaceHome/btlfx_stepFaceHome, op 0x7C
  func_800D84C4/func_800DA030 → btlfx_opCallWeaponScript/btlfx_stepCallWeaponScript; func_800D71C8 →
  btlfx_moveUnitArc, func_800D72BC → btlfx_moveUnitLinear. medium.
- D_800F7510 → g_btlfxModuleExports: 216 function pointers then data pointers (g_btlfxState, g_btlfxLoader,
  g_btlfxTasks) that the CD-loaded effect modules call; runtime: functions whose recorded callers are inside the module
  area (Hi_DrawEffModel, btlfx_poolSpawn, btlfx_getFileData, func_800D3030, ...) are all in this table. medium-high.

## Battle camera  (status: applied)
- binary ovl_0b (ovl_0b_o1.c). func_800C3770 → btlcam_update: first call of btl_update every tick; func_800C37BC →
  btlcam_step builds eye/target, applies func_800C5530 → btlcam_applyTargetView (target-selection view),
  func_800C4A3C → btlcam_applySway, func_800C493C → btlcam_applyShake and func_800C52B4 → btlcam_lookAt (view matrix
  into GameData.unk18). All per tick in every run. medium-high (code + call graph).
- func_800C316C → btlcam_loadScript, func_800C2F9C → btlcam_play (runtime: from btlfx_opPlayCamera and func_800E9290
  once or twice per spell), func_800C2F1C → btlcam_playScene (battle-start/victory scene camera; runtime btl_victory),
  func_800C2FFC → btlcam_playSeq (from btlseq_opPlayCamera), func_800C2E04 → btlcam_init, func_800C2E4C →
  btlcam_beginEffect (runtime: from btlfx_runScript), func_800C30F4 → btlcam_endEffect, func_800C57A4 →
  btlcam_getUnitAnchors. medium. D_800FE3B8 → g_btlCamera. medium.

## Battle setup (code)  (status: applied)
- No battle-entry coverage yet (entry.py prepared). From code: func_800BAFEC → btl_initParty, func_800BB4A0 →
  btl_initPartyUnit (`(Character *, BtlUnit *, u16 slot, u16 n)`), func_800BB1F4 → btl_initEnemies, func_800BBD3C →
  btl_resetUnit, func_800BBF5C → btl_initWeapon, func_800BC0D0 → btl_initEquipElements, func_800C9EE0 →
  btl_initEnemyUnit, func_800CA0A0 → btl_setEnemyStats, func_800B03B0 → btl_startBattle (phase 2),
  func_800CE05C → btlbg_init, func_800CEE74 → btlbg_getHeader. medium (code).

## Battle campaign 3 (2026-10-18): method, runs, helper scripts  (status: reference)
- Redux disc 3, Lv 99 party from `disc3_malboro_lv99_calm` unless noted. Every action is Zidane's Attack rewritten at
  btl_setCmd (0x800B1250) into another command/ability, as in campaign 2. Run outputs: `build/rt3/NAME.json` (traces,
  start frame) and `build/rt/cov_NAME.json` (coverage, same format as campaigns 1-2), screenshots `build/rt3/NAME_XX.png`.
- The Malboro is kept passive by two one-time pokes after the state load instead of the per-vsync Freeze listener:
  BtlUnit.unk2E (ATB speed) = 0 at 0x8010926A and HP = 60000 at 0x80109264. With speed 0 its ATB stayed 0 for 900 frames.
- Hangs: the "Unknown error in event listener" + modal hang happened twice in this campaign as well (both during long
  summons, with coverage on). `redux.log` shows a 37-line Lua stack dump ("1: (Table)" ... "37: (Table)") right
  before the message, and no Lua error text. A Lua breakpoint removed twice is harmless (tested). After switching the
  LuaJIT compiler off (`jit.off(); jit.flush()` once per session) the remaining ~45 runs (all other summons, Blue magic,
  Dyne, ablations, ovl_65), about 60k frames with breakpoints, ran without a hang. So the likely cause is JIT-compiled
  Lua calling into C that re-enters Lua, but it isn't proven. See docs/emulator.md.
- Coverage blind spot in campaigns 1-2: `tools/rt/cov.py` skips everything in `config/library_funcs.us.txt`, including
  the 46 ovl_0b functions marked "hand-written asm" (the effect emitters D3ED4-D5C30, the particle integrators
  DFE4C-E0014, btl_drawMesh, the btlfx_rand/lerp helpers, ...). They always looked unreached. They do run: a direct trace
  of func_800D3ED4 during Death shows 960 calls from func_800D1D74+0x264. `build/rt3/run3.py` covers them.
- Scripts (scratch, `build/rt3/`): `r3.lua` (hang-safe breakpoints: two ffi callbacks created once and never freed,
  pcall in every callback, dispatch by address because Redux passes its own cause string "Lua Breakpoint", no
  breakpoint changes inside callbacks; coverage pruning happens between run chunks), `run3.py` (state, patch, traces,
  watches, coverage, opcode logs, `--cov-after ADDR` to start coverage only after an overlay is loaded, `--save-at`),
  `entry3.py` (enter an encounter from the field and cover from the ovl_0b entry), `ablate3.py` + `abldiff.py` (stub a
  function mid-action and diff the screenshot against the control; the stub needs at least ~20 frames before the change
  shows, 4 frames showed nothing), `summ.sh`/`blu.sh`/`fxmap.sh` (batches), `tsum.py`, `newhits.py`, `order.py`,
  `who.py`, `unreached.py`.
- Runs: t_regen, t_vanish, t_autolife, t_bio, t_frogdrop, t_frost, t_mustard, t_mustard_vivi, t_badbreath, t_mightyguard,
  t_defend, t_fire, t_cure, t_attack, t_scan, t_detect, g_defeat, g_trance4 (trance entry), g_reflect, g_flee (L1+R1),
  e_malboro (battle entry), su_{shiva,ifrit,atomos,odin,leviathan,bahamut,ark,fenrir,carbuncle,phoenix,madeen} (full
  summons), e_eidolon, u_* (16 Blue magic + 4 Skill), d_freeenergy / d_grandlethal (Dyne), fx_* (15 black/white spells,
  effect range only), o65_victory / o65_defeat (ovl_65), op_enemy (sequence opcodes), ablations abl_{holy2,fire,death,
  icon,model}.

## Status bits confirmed at runtime  (status: applied: BTL_STATUS_REGEN replaces UNK38_REGEN, unk38 uses the same bits)
- binary ovl_0b; btl_addStatus a1 traced from btl_addStatuses+0x48 while the patched spell hit:
  Regen 0x00040000 (Vivi), Vanish 0x04000000 (Vivi), Auto-Life 0x00002000 (Vivi), Freeze 0x02000000 (Frost on the
  Malboro). Bad Breath added Blind, Confuse, Poison, Slow and Mini in one call each. Mighty Guard added Shell and Protect
  to all four. Bio, Frog Drop and Mustard Bomb added nothing (Malboro, and Vivi for Mustard Bomb), so Heat stays
  code-only.
- Proposed: `BTL_STATUS_REGEN 0x00040000` (runtime; today only `UNK38_REGEN` exists); mark VANISH and AUTO_LIFE as
  runtime. In t_regen, btl_initDotTimer(Vivi, 0x12) runs from btl_addStatus+0x774 and btl_tickRegen runs 3 frames later
  from btl_updateUnitStatus+0x1AC. Regen effect id 0x20, file 0x20.
- confidence: high.

## Scan, Detect, Reflect, defeat, flee  (status: applied, incl. btlui_pollEscape, Battle.escapeHeld and btl_setUnitBrightness)
- btl_startScan: once from btl_calcCmd+0x2220 on the Malboro; btl_updateScan: 175 ticks from btl_drawPopups+0x298. The
  banner showed "Malboro LV: 57", then "HP: 11683/11687 MP: 2316/2334". btl_startDetect: from btl_calcCmd+0x2230;
  btl_updateDetect: from btl_drawPopups+0x2A0. The banner showed "Carries Vaccine.", then "Carries Ore.". high.
- btl_applyReflect / btl_orTargets: Fire on a Malboro with Reflect set in its permanent-status word (BtlUnit.unk38, byte
  0x80109277 |= 0x20). Setting it in `status` doesn't work: btl_tickStatusTimers removes it 3 frames later because the
  timer is 0. btl_orTargets(Zidane.unk1C) runs from btl_applyReflect+0x278, then btl_calcCmd(Zidane, Vivi), and Vivi took
  the Fire (9848 HP). high.
- btl_checkSideDefeated: with all party HP poked to 0 it is called 4x from btl_updateKo+0x414 and shows message 5
  ("Annihilated..." banner). Then the GAME OVER screen appears with ovl_65 resident. high.
- func_800EB924 → btlui_pollEscape (`void (void)`): while L1+R1 are held it sets Battle.unkD83 = 1 and calls
  btl_tryEscape(0) each frame, and btl_tryEscape(1) every 62nd frame (runtime: 122 calls with 0, 2 with 1; the second one
  ran btl_calcCmd(0,0,0,0x38) → btl_canFlee + btl_rollFlee → btl_setCmd(…, 0x38) and the party escaped). high.
- Battle.unkD83 → escapeHeld (u8): btl_updateUnitModel compares it with unit.unk7B and calls btl_setIdleMotion when it
  changes (the run-away stance). medium.
- func_800C1A8C (btl_setUnitBrightness?, low-medium in campaign 2): runtime from func_800B0060+0x124 during the battle
  intro, with Battle.unkD86/unkD87 ramping +4 per tick to 0x20 (party/enemy fade-in), and 64x from btl_victory+0x330 while
  the party ran away. Raises it to medium.

## Matra Magic: func_800B8FF8  (status: applied)
- func_800B8FF8 → btl_setTargetHp (`void (BtlCalc *)`): sets the target's HP to the ability parameter (unk8->unk8[5]),
  or kills it (HP 0, death cry, KO motion) when the parameter is 0; fails on KO targets. Runtime: only in u_matra, from
  btl_calcCmd+0xE1C. After it the Malboro got LOW_HP (btl_addStatus 0x200 from btl_updateHpStatus+0xAC), i.e. HP 1.
- confidence: medium-high.

## Summons  (status: applied; the leads were named from code in the ovl_0b cleanup pass: btl_reloadParty, btlfx_loadSoundBank/btlfx_waitSoundBank, btlfx_countVisibleParty, btlfx_loadNextBlock)
- func_800B989C → btl_rollShortSummon (`void (BtlCmd *)`; was "btl_rollSummonLength?"): for command 0x10/0x12/0x14
  (from btl_checkCmdUsable+0x264) it sets cmd.unk10 bit 0x400 with chance 0xE6/256 (MP > 2x cost) or 0xAA/256. That bit
  makes btl_startCmdEffect play ability->unkC (short version) instead of ability->unk0_8. Support bit unkC4[1] 0x08000000
  (Boost?) skips the roll. Runtime: Ifrit without that bit → effect 0x1BD, ~300 frames. With the bit poked into Zidane's
  support word (0x8008B31B |= 8) → effect 0x114, the full ~1500-frame summon. high.
- Full summon effect ids (btlfx_start a0 = btlfx_loadFile a0): Shiva 0x26, Ifrit 0x114, Atomos 0xB8, Odin 0x105,
  Leviathan 0xB3, Bahamut 0xE3, Ark 0x17D, Fenrir 0xD2, Carbuncle (ability 0x44) 0xB1, Phoenix 0xD3, Madeen 0xFB. Short:
  Shiva 0x197, Ifrit 0x1BD, Atomos 0x1BE. Eidolon command 0x12 (Dagger's trance) with Shiva also plays 0x26.
- func_800E89D0 → btlfx_restorePartyLooks (`void (void)`): from btlfx_runScript+0x4C once at the end of every summon.
  For each party unit it calls func_800C0D24 + btlfx_reapplyMini + btlfx_reapplyVanish; after effect 0x1BF it calls
  btl_setIdleMotion on the caster. medium-high.
- func_800E8A80 → btlfx_restoreUnitLook (`void (s32 unit)`): the same for one unit; runtime once in the trance run
  (btlfx_runScript+0x7C). medium.
- func_800E86E0 → btlfx_copyWords (`(s32 *dst, s32 *src, u32 bytes)`, rounds up to words): once per summon from
  btlfx_runScript+0x78C. medium.
- Leads, not named: func_800AB410 → func_800A9BE4(0x207 / 0x8207) runs 48-57x per summon from btlfx_runScript+0x3C/+0x600;
  func_800DA634 / func_800DA6B4 (start/wait for main func_800562E0 / func_80055DCC + func_80056388, probably the summon's
  sound bank) in the summons with their own sounds; func_800E8AD0 counts party units with unk68 & 0x400001 == 1, per frame
  from btlfx_updateLoader during summons; func_800DA5C4 (Ark, Bahamut, Madeen) restarts g_btlfxLoader.

## Trance entry  (status: reference: btl_gainTrance was already [runtime]; leads named from code in the ovl_0b cleanup pass: btl_swapPartyModelStep, btl_loadPartyMotions, btl_reapplyStatusLook, btl_applyBgLight, btl_setMeshRgb; func_800AB3E4 stays unnamed)
- With the party's trance gauges poked to 0xFE (+0x78) and the Malboro left to act (`g_trance4`, deterministic):
  btl_gainTrance is called 2x from btl_applyCalcResult+0x318 when "Virus Tentacles" hit. Eiko's gauge overflowed, so
  btl_addStatus(Eiko, 0x4000) ran from btl_gainTrance+0x1B4. Effect 0x101 (the trance transformation) followed.
  btl_gainTrance only runs for enemy → party damage, so a party Attack on Vivi doesn't trigger it.
- Trance-only functions in that run: func_800AB3E4 (→ func_800ABBC4(unit, 0x17 / 0x8017), 12 calls from
  btlfx_runScript+0x68 / +0x684, reads CD via cd_readChunks: probably the trance model swap), func_800BD698,
  func_800C0D24 → func_800C2344 → func_800C8D18. Leads only.

## Battle entry (from the ovl_0b entry point)  (status: applied except btl_loadSoundBanks (low-medium); leads named from code in the ovl_0b cleanup pass: btl_loadModelsStep, btl_registerModel, btl_initEnemyState, btl_saveEnemyCluts, btl_savePartyCluts; main's call to btl_entry stays func_800A859C, see names_from_port.txt)
- Run e_malboro: `build/rt3/entry3.py e_malboro 899 2714 1200` (enter_battle from `disc3_pandemonium_lv99_field`,
  break at 0x800A859C, then coverage). Order by first-hit frame is in `build/rt3/order.py e_malboro --all`.
- func_800A859C → btl_entry: game_main's mode-2 case calls it right after game_loadModeOverlay(2) (the break hit there).
  It calls func_800A8CD4, loops btl_main until GameState.unk0 & 0x101, then func_800A8DE0. high.
- func_800A8CD4 → btl_initMode: battle buffers, vsyncMode 4, installs GameData.unk838 = func_800AB810 and unk83C =
  btl_soundCmd, then func_800A90B0 ... func_800B031C (all at +0..+43). medium-high.
- func_800A8DE0 → btl_exitMode (code), func_800A94F8 → btl_cleanup (end of btl_main: frees the battle files, copies
  the saved encounter sectors GameData.unk7EC/unk7F4 back into unk7FC/unk804 and restores fieldId / unk7DC; these are the
  fields tools/rt/enter_battle.py saves). medium (code).
- func_800A8E24 → btl_allocBattle (Battle struct at D_80067944 + 0x61800), func_800A8E6C → btl_clearBattle. medium.
- func_800A90B0 → btl_loadCommonData: file 0x271B of directory 1, from which it copies Battle.unk154C, Battle.abilities
  (the ability table) and Battle.unk22CC. medium.
- func_800A8F8C → btl_loadSoundBanks (two banks from D_800F51E8 via func_80055E14). low-medium.
- func_800A923C → btl_load (`void (s32 encounter)`, first call of btl_main): buffers, encounter file, event script,
  background, model slots, btlui reset, btl_startBattle. medium-high.
- func_800A9768 → btl_allocBuffers, func_800A98A0 → btl_initReturnInfo (stores the previous mode GameState.unkA and the
  field id or world position to return to). medium.
- func_800AA08C → btl_loadEncounterFile: synchronous CD read of GameData.unk7FC[0]/unk804[0], the encounter sector
  range enter_battle.py writes. medium-high.
- func_800A99AC → btl_initEncounter: finds the encounter's 0x11xxxx data blocks, evt_init on the AI script,
  func_800C94B8. medium.
- func_800AA140 → btl_loadBackgroundStep (`s16 (Battle11C0 *, s32 enc, state *)`): CD read state machine for
  unk7FC[1] (the background file enter_battle.py writes); btl_load polls it until 0 (54473 calls). medium-high.
- func_800A9A7C → btl_setupBackground (texture pages, tex anims; sets Battle.unk0 |= 1, which turns on btlbg_draw).
  medium.
- func_800E9E10 → btlui_reset (clears the 0x11C UI block, sets the battle text bank, VSyncCallback(NULL)). medium.
- func_800B031C → btl_initBattleState (btl_initState, effect common data, btlcam_init, ...). medium.
- func_800B0060 → btl_updateIntro (`void (GameData *, Battle *)`): 105 ticks from btl_update+0x70 at battle start.
  It steps the model loader, then btl_initEnemies / btl_initParty, fades enemies then party in (unkD87 / unkD86 += 4
  up to 0x20), and draws units, background and banner. medium-high.
- Leads: func_800AB580 (4-step loader over func_800AA24C/AA504/AA958/AABC4; sets Battle.unk0 |= 4, which starts
  btl_initParty) → btl_loadPartyModelsStep?, func_800AB810 (GameData.unk838 hook, 5 calls: builds a model entry from
  loaded data) → btl_setupModel?, func_800BB974 (after btl_initEnemies), func_800C0C7C / func_800C0CD0 (before
  btl_initEnemies / btl_initParty).

## Unit shadows  (status: applied)
- func_800AC37C → btl_drawShadow (`u8 *(Battle11C028 *, s32 slot, s32, s32 bones, u8 *prim)`, 4 semi-transparent FT4
  quads, macro SHADOW_QUAD in the C) and func_800AD4E4 → btl_calcShadowMatrix (orientation from one bone or between
  two bones). Runtime: both run per unit per tick from btl_updateUnitModel+0x544. Stubbing either removes the dark
  ellipses under all units (abl_model, 0.51 % of pixels; build/rt3/abld_model.png). Stubbing func_800C7B84,
  func_800C7CC8, func_800C9040 or func_800C1A8C changed nothing at idle. high / medium-high.

## Effect emitters and wrappers (ovl_0b D0B18-D5C30)  (status: applied for medium and up, incl. the btlfx_mod.h members; the ovl_0b cleanup pass added D198C btlfx_drawSprite, D1BC4 btlfx_drawSpriteAtOt, D2938 btlfx_drawFan, D2DBC btlfx_drawBackdropSprite, D0B18 btlfx_drawRippleGrid, D23D8 btlfx_drawRings, D3B6C btlfx_drawSpriteLine from code; the inner emitters D40CC, D4228, D437C, D45FC, D57D0, D59D0 carry btlfx_emit* names)
- Hit counts per effect (fx_* runs, effect range only): an emitter runs once per wrapper call that passes the
  wrapper's depth/count checks, so its count equals the wrapper's or is slightly lower. D1680 = D4478,
  D1D74 = D3ED4, D198C = D45FC, D1FFC = D40CC + D4228 + D437C, D23D8 ⊃ D4768 + D4A30, D2938 = D57D0 + D59D0,
  D1494 → D0E14 = D50A0 (Death, Fira, Holy), D0FFC = D4D6C (Firaga) or D5568 (Thundaga), D12C4 = D5C30 (Comet,
  Demi, Doomsday, Flare, Osmose, Thundaga). The packet codes are from the C of each wrapper.
- Ablations (stub mid-effect, 24-30 frames, diff vs control; build/rt3/abld_*.png):
  - func_800D1494 → btlfx_drawTrail (`(u8 *p, s32, s32 *, s32 frame, s32 scale, s32, s32 tp, u8 *out, s32 mode)`,
    calls btlfx_pushTrailPoint, then one of three ribbon modes): Holy's blue light ribbons vanish (15.9 % of pixels),
    and so do Death's red streaks. medium-high.
  - func_800D0E14 → btlfx_drawTrail3D (GT4 0x3C/0x3E, 0x34 bytes per segment; via func_800D50A0 →
    btlfx_emitTrailRibbon3D), func_800D0FFC → btlfx_drawTrailFlat (GT4 via func_800D4D6C → btlfx_emitTrailRibbonFlat,
    or LINE_G2 0x50/0x52 via func_800D5568 → btlfx_emitTrailLines), func_800D12C4 → btlfx_drawTrailScreen (GT4 via
    func_800D5C30 → btlfx_emitTrailRibbonScreen). The ff9-ds notes F13/F14/F16 describe the same split. medium.
  - func_800D1680 → btlfx_drawParticles (FT4 0x2C/0x2E, 40 bytes each; inner func_800D4478 → btlfx_emitParticles):
    Fire's flame particles (3.7 %) and Holy's white sparkles (3.0 %) vanish. medium.
  - func_800D1D74 → btlfx_drawAnimQuads (animated FT4 set, frame = arg4 mod frame count; inner func_800D3ED4 →
    btlfx_emitAnimQuads): Death's dark smoke cloud vanishes (12.5 %). medium.
  - func_800D1FFC → btlfx_drawSprites (3D points drawn as TILE 0x68 / SPRT_8 0x74 / SPRT_16 0x7C / SPRT 0x64 + one
    DR_MODE; inner func_800D437C (dots), func_800D4228 (8x8 / 16x16), func_800D40CC (free size)): with Vivi poisoned,
    stubbing it or btl_drawStatusIcons removes the same Poison bubbles over Vivi (abl_icon, 0.14 % each). Its caller
    btl_drawStatusIcons+0x3E4 is the status-animation path. medium-high for the wrapper, medium for the three inner names.
  - func_800D198C → btlfx_drawSprite (SPRT 0x64/0x66 at a projected point, OT bucket from depth; inner func_800D45FC →
    btlfx_emitSprite), func_800D1BC4 → btlfx_drawSpriteAtOt (same with an explicit OT index). Called from
    btl_drawStatusIcons+0x384 and func_800E0168. Stubbing it at idle or with Poison changed nothing visible. medium-low.
  - func_800D3030 → btlfx_drawBackdrop (`(BtlBgLayers *, s32)`: layered screen backdrop via btlfx_addSpriteQuad and
    func_800D2DBC): Holy's dark starry sky vanishes and the battle background shows through (64.5 %). medium.
    func_800D2DBC → btlfx_drawBackdropSprite (SPRT / GT4 at x, y with a frame; Demi, Holy, Carbuncle, Reflect). low-medium.
  - func_800D2938 → btlfx_drawFan (textured FT4 fan func_800D57D0 / Gouraud G3 fan func_800D59D0 + DR_MODE, as in ff9-ds
    F13; Death, Flare, Holy, Osmose, Thundaga, Blizzaga). medium-low (no ablation).
  - func_800D0B18 (POLY_G4 0x38 grid of arg2 x arg3; Doomsday, Leviathan, Phoenix, Madeen) → btlfx_drawGouraudGrid.
    low-medium. func_800D23D8 (FT4 w x n grid, inner D4768 / D4A30; runs in every action from +124, small glow near the
    caster in Fire) → btlfx_drawQuadGrid. low. func_800D3B6C (lerps two points, draws via func_800D1BC4; Holy, Cure,
    Phoenix, Bahamut, Madeen, Potion) and func_800DE11C (Hi_DrawSliceEffModel's inner loop; Death, Doom, Break, Fenrir,
    Frog Drop): leads.

## Battle sequence opcodes seen (op_enemy)  (status: reference)
- 4000 frames of the Malboro acting freely: 69 opcodes from 6 btlseq_start calls. Typical stream:
  `10 00 / 08 39 / 05 03 / 0e 81 / 07 05 / 05 04 / 09 07 / 07 05 / 05 05 / 01 06 / 0a 00 / 00`, i.e. setCamera, effect
  0x39, anim, showName 0x81, waitAnim, anim, waitEffectHit, waitAnim, anim, wait 6, op 0A, end. Op 0A
  (btlseq_opReleaseEffect) always followed an effect + wait-hit + wait (3 calls). Op 11 (btlseq_opSetStance) appeared as
  `11 01 / 02 00 / 00` in one action and `11 00 / 02 00 / 00` in a later one: it is set before op 02 (execCmd) and
  cleared again in the next sequence. Both names stay low-medium.

## ovl_65: victory rewards vs game over  (status: applied for battleEnd_update, battleEnd_init, rewards_update; the low-medium state handlers stay unnamed)
- Runs o65_victory (Malboro HP 1, Attack, coverage from main's `jal battleEnd_main` at 0x8001285C) and o65_defeat
  (party HP 0; no button taps, so it stays on GAME OVER). Screens: +100 / +200 "Received EXP and AP!" (counting), +300
  "Received Item(s) and Gil", +400 Received Gil counted down to 0.
- func_801F3A7C → battleEnd_update (per frame from battleEnd_main+0x70): calls gameover_main on defeat, func_801F0F04
  on victory (487 calls). medium. func_801EF45C → battleEnd_init (both runs, once). medium.
- func_801F0F04 → rewards_update (victory only, every frame). It calls func_801F1160 (every frame: func_801EFB98,
  EFC68, EFF50, F0044, F0138, F0264, likely the window draws) and func_801F11B8 (every frame; calls a state handler
  through a pointer at +0x40). medium for rewards_update, low for the rest.
- State handlers (victory only, first frame after coverage start / calls): func_801F143C +11 x109 (EXP/AP count-up),
  func_801F14C0 +120 x109 (after the first X), func_801F1518 +229 x141 (Items/Gil screen), func_801F076C +370 x101
  (gil transfer). The X taps came every 120 frames, so the transitions line up with them. low-medium: rewards_stateExp,
  rewards_stateExpDone, rewards_stateItems, rewards_stateGil.
- Defeat only: gameover_main (already named), func_801F3818 and func_801F3A20 (both once from gameover_main). Leads.

## Field/menu campaign (2026-10-24): method, states, helper scripts  (status: reference)
- Redux disc 1. Field states `field_dali_inn` (Lv 50 save, card 2 FILE02) and `field_dali_inn_moogle`; menu states
  `menu_main` / `menu_items` (Burmecia vault Lv 99 save). New states (all disc 1, listed in docs/emulator.md):
  `menu_status`, `menu_equip`, `menu_ability`, `menu_card`, `menu_config` (opened from `menu_main` with the pad),
  and, from `field_dali_inn`, `menu_shop_weapons`, `menu_shop_synth`, `menu_name_entry`, `menu_party_select`,
  `menu_save`, `menu_chocograph`, `menu_chocograph2`, `menu_disc_change` (forced with `force_menu.py`, below).
- Menu types: `menu_open(type, arg)` loads overlay `D_8006CA40[type]` (s32[9]: ff, 3a, 3c, 3b, 39, 32, 3d, ff, 3e)
  to 0x801EDF00 and runs `D_8006C9D4[type]` = {init, update, exit}. `build/rtf/force_menu.py STATE TYPE ARG FRAMES
  [SAVE]` (scratch) loads a field state, presses Triangle, breaks at `menu_open`, rewrites a0/a1 and lets the menu
  load (optional `POKES=addr=hex,...` are written first). Results (screenshots `build/rtf/fm_*.png`): type 1 = name
  entry (ovl_3a), 2 = shop (ovl_3c; arg = shop id, 0 = "Weapons" Buy/Sell/Exit, >= 0x20 = synthesis), 3 = party select
  (ovl_3b; arg = request pointer, `&D_8007AE70` gave four Zidanes), 4 = save/load (ovl_39; arg 2 = field save, 1 =
  title Continue), 5 = Chocograph (ovl_32), 8 = "Change to DISC" (ovl_3e; arg = (disc & 3) << 14 | field id, 0x3A98
  from ovl_14). So `config/overlays.us.txt` can now say: ovl_36 = Card (Tetra Master collection, main-menu screen 6),
  ovl_3c = shop (buy/sell/synthesis, menu type 2), ovl_3e = disc change screen (menu type 8), ovl_3d = menu type 6
  (no code). Types 6 and 7 were not tried.
- Main-menu screens (menu type 0): `D_8006E510` (s32[8]: ff, 37, 33, 33, 38, ff, 36, 34) is the overlay of each
  screen (main, Item, Ability, Equip, Status, Order, Card, Config) and `D_8006E530` the field table of {init, update,
  exit, needsMember} per screen (`D_8006E5B0` the world-map one, picked by `func_80035D3C` when GameState.mode == 3).
  The command screen itself lives in ovl_0a 0x800C8124-0x800C9700 (ovl_0c has its own copy).
- Coverage: `tools/rt/cov.py start --bin main --bin ovl_0a [--bin ovl_XX]`, then `tools/rt/act.py NAME STATE SEQ`
  (wrapper `build/rtf/mc.sh`), outputs `build/rt/cov_NAME.json`. `build/rtf/fl.py mat|cmp BINS RUNS` (scratch) is
  `tools/rt/matrix.py`/`cmp.py` for any binary, printing each function's source file. Runs: field `f_idle`, `f_walk`,
  `f_idle2`, `f_moog` (Cross on the Moogle: MOGNET window), `f_moogdown` (+ Down x2), `f_menu` (Triangle); main menu
  `m_idle`, `m_down`, `m_item`, `m_abil`, `m_equip`, `m_status`, `m_order`, `m_card`, `m_config`, `m_help` (Select),
  `m_close` (Circle), `m_abilsel`; item `i_*`; status `s_*`; shop `sh_*`; save `sv_*`; card `c_*`.
- Ablation: `tools/rt/ablate.py STATE PRESEQ FUNCS --frames 60`, montages `build/rt/abl_*.png`. In menus 20-30
  frames were sometimes not enough for the change to show (ovl_3c 0x801F4000 and ovl_37 0x801F262C only vanished
  at 30/60 frames), so 60 was used; a stub with no visible change at 60 frames named nothing.
- Two scripts driving the emulator at once corrupt each other's runs (happened once; that run was repeated).
  With LuaJIT off there was no hang in ~30k frames.

## Main menu, command screen (ovl_0a 0x800C8124-0x800C9630)  (status: proposed)
State `menu_main` (Item..Config list, party panels, TIME & GIL, LOCATION). `D_800CB188` (Unk800CB188): +0 command
cursor (0-6), +1 state (0 commands, 1 pick a member, 2 Order), +2 Order cursor, +3 Order target, +4 Order "first
member picked", +8 WinDef[6] panel tweens. Read: `menu_main` 00 00, `menu_status` 03 01, after an Order swap 04 02 01 01.
- D_800CB188 → s_menuMain (cursor, state, orderCursor, orderTarget, orderPicked, panels). high.
- func_800C93D4 → menuMain_init: init of `D_8006E530[0]`; once when the menu opens (f_menu). high.
- func_800C94B0 → menuMain_update: `D_8006E530[0]` update, once per frame in every m_* run. high.
- func_800C94E0 → menuMain_exit: `D_8006E530[0]` exit; once when Item/Ability/... is entered (m_item, ...). high.
- func_800C951C → menuMain_input: dispatches `D_800C9E14[state]` (cov: 800C8284 in state 0, 800C8354 in m_abilsel,
  800C842C in m_order). high.
- func_800C8284 → menuMain_inputCommand: Up/Down move the cursor (m_down), Cross → 800C923C, Circle closes the
  menu (m_close: func_800362C8). high.
- func_800C923C → menuMain_selectCommand: once per Cross in m_item ... m_order; Order → state 2, screens that need
  a member → state 1, others → func_800361CC(screen). high.
- func_800C8354 → menuMain_inputMember: state-1 handler (m_abilsel: 86 hits, Down moved the hand to Vivi). high.
- func_800C842C → menuMain_inputOrder: state-2 handler (m_order: 72 hits). high.
- func_800C85B8 → menuMain_reorder (`(slotA, slotB)`): run o_row (Order, Cross, Cross on Zidane) called it with
  (0, 0): Zidane's panel shifts (Character+0x34 bit 9 toggled, 0x0E10 → 0x0C10). Run o_swap (Cross Zidane, Down,
  Cross): (0, 1), party[0]/party[1] swapped, Vivi now first (`build/rt/o_row.png`, `o_swap.png`). high.
- func_800C86C8 → menuMain_swapPortraits: called from 800C85B8 only for the real swap (o_swap: (0, 1)); swaps the two
  portraits in VRAM (StoreImage/MoveImage/LoadImage). high.
- func_800C9580 → menuMain_draw: calls the draw functions below. high (cov + calls).
- func_800C9630 → menuMain_drawParty: ablation (`abl_mm.png`): the four party panels go. high.
- func_800C883C → menuMain_drawMember (`(slot)`): ablation: portrait, name, Lv/HP/MP, stones go; frames stay. high.
- func_800C8AE8 → menuMain_drawPortraitShade: TILE 0x2E x 0x30; ablation: the dark rectangle behind the portraits
  goes. medium.
- func_800C8C08 → menuMain_drawTimeGil: ablation: TIME & GIL window goes. high.
- func_800C8D40 → menuMain_drawLocation: ablation: LOCATION window goes. high.
- func_800C8E2C → menuMain_drawCursor: ablation: the hand goes. high.
- func_800C9014 → menuMain_updateHelp: the help callback set by menuMain_init; runs only with the Select help open
  (m_help: 73 hits). high.
- func_800C95B8 → menuMain_updatePanels: steps the six panel tweens while D_80077E50.unk1 is set. medium.
- func_800C8124 → menuMain_initPanels: sets the six panel rects and starts their slide-in (f_menu: once). medium.
- func_800C92BC → menuMain_onSlideInDone: clears D_80077E50.unk1; f_menu: once, from the tween code (ra 0x80063294). medium.
- func_800C9510 → menuMain_setCursor (`(u8)`): called by func_80036560 with the screen being left. medium (code).
- func_800C90D0 → menuMain_setExpHelp, func_800C92C8 → menuMain_setEquipHelp: help text for the Status (EXP/Next)
  and Equip (five slots) cases of 800C9014. medium (code).
- CharBits.unk9 (Character+0x34 bit 9) is the battle row toggled by Order; with it set the portrait is drawn 14
  pixels further left. All four members of this save had it set. Name it `frontRow` only if a battle check agrees
  (btl_cmd.c compares it with the unit's row); medium as "row".

## Main menu screen manager (main tu_80035D3C)  (status: proposed)
`D_80077E50` (Unk80077E50): +0 menu running (1 in all menu states), +1 panels sliding in, +2 current screen (read:
menu_main 0, menu_items 1, menu_status 4, menu_card 6), +3 selected member, +4 frame counter (+1 per frame).
`D_80077E28` is the screen-switch state (+7 target screen, +0x14 command-window RECT, +0x1C screen table).
- D_80077E50 → g_menuScreen (active, slideIn, screen, member, frame). high.
- D_8006E510 → g_menuScreenOvl, D_8006E530 → g_menuScreensField, D_8006E5B0 → g_menuScreensWorld. high.
- D_8006CA40 → g_menuTypeOvl, D_8006C9D4 → g_menuTypes (the menu_open tables above). high.
- func_80035D3C → menuScreen_init: init of menu type 0 (`D_8006C9D4[0]`), prints the ff9db.h path; once in f_menu. high.
- func_8003613C → menuScreen_update: type-0 update, every frame in all m_* runs; command window, switch step,
  the screen's update, frame counter. high.
- func_800361A8 → menuScreen_exit: type-0 exit (m_close: once). high.
- func_80035E40 → menuScreen_drawCommands: ablation at `menu_main`: the Item..Config window goes; inside a screen it
  draws only that screen's name (the "Item"/"Status" box at the top right). high.
- func_800361CC → menuScreen_open (`(screen)`): once per screen entered (m_item ... m_config); fade out, load the
  overlay (80036470), then the screen's init. high.
- func_80036270 → menuScreen_switch (`(u8 screen)`): Square on the Ability screen (`menu_ability`) called it with 3
  and the Equip screen came up without an overlay load (`build/rt/ab_sq.png`). high.
- func_80036560 → menuScreen_onSwitchFaded: called from the fade update right after 80036270 in that run: old exit,
  new init, fade in. high.
- func_80035F94 → menuScreen_updateOpen: waits for the title animation and the overlay read, then calls the new
  screen's init. medium.
- func_80036470 → menuScreen_loadOverlay: cd_readAsync of `D_8006E510[screen]` to 0x801EDF00 (once per open). high.
- func_80036444 → menuScreen_requestOverlay: calls 80036470 when the screen has an overlay. medium.
- func_80036634 → menuScreen_onOverlayRead (read callback), func_80036528 → menuScreen_onOpenFaded (fade callback
  set by 800361CC): once per open each. high.
- func_800362C8 → menuScreen_close: Circle on the command list (m_close). high.
- func_80036300 → menuScreen_needsMember (`u8 (screen)`): table flag +0xC (1 for Ability, Equip, Status);
  800C923C goes to state 1 with it (m_abil/m_status: 1 call). high.
- func_8003631C → menuScreen_getCommandHelp: help text of command i (m_help: 73 calls). high.
- func_800362B8 → menuScreen_getCurrent: returns g_menuScreen.screen. medium (code).
- func_800363BC → menuScreen_startTitleAnim, func_800360B0 → menuScreen_stepTitleScroll, func_80036344 →
  menuScreen_stepTitleResize: the command window shrinks to the one-name title box while a screen opens (80036344:
  12 calls per open, h 0x74 ↔ 0x1A). medium.

## Help window and menu cursor (main tu_80035108)  (status: proposed)
`D_80077DE8` (Unk80077DE8, the most-referenced main global): +0/+2 hand x/y, +4 hand OT index, +8 help text, +0xC
help callback, +0x10 help bounds, +0x18 help open, +0x19 opening, +0x1A text shown, +0x1B help locked, +0x1C help
follows the hand, +0x1D hand drawn above the help, +0x20 moogle-icon animation.
- D_80077DE8 → g_menuHelp. high (every screen writes the hand position there before func_800355E4).
- func_800355E4 → help_drawCursor: sprite 1 (the hand) at g_menuHelp x/y/otz; each screen's cursor ablation
  (menuMain_drawCursor, ovl_37 801F13F8, ...) removes the hand by skipping this call. high.
- func_80035490 → help_update: every menu frame; Select (0x8000000) toggles the help (m_help). high.
- func_80035680 → help_toggle: m_help: once, on Select (sound 0x2AA). high.
- func_80035108 → help_draw: m_help only (73 hits): moogle icon, text, frame. high.
- func_80035214 → help_layout: m_help only; places the box next to the hand inside the bounds. high.
- func_80035744 → help_onIconAnimDone: m_help: once (the moogle animation's done callback shows the text). medium.
- func_80035430 → help_init: once per menu open (f_menu). high.
- func_8003556C → help_setCallback, func_80035580 → help_setBounds (NULL = whole screen), func_8003561C →
  help_isOpen, func_8003562C → help_isLocked, func_8003563C → help_setLocked, func_80035648 → help_getFollow,
  func_80035658 → help_setFollow, func_80035664 → help_getCursorOnTop, func_80035674 → help_setCursorOnTop:
  accessors of the fields above, called from every screen's init/draw. medium (code; field meanings from 80035108/80035214).

## Sprite animation player (main tu_800357E0)  (status: proposed)
Drives the moogle icon of the help window; only m_help reaches it.
- func_80035C2C → sprAnim_draw (m_help: 73, one per help frame), func_800357E0 → sprAnim_drawCell (SPRT),
  func_8003595C → sprAnim_drawScaledCell (POLY_FT4), func_80035B90 → sprAnim_update (every menu frame),
  func_80035B24 → sprAnim_init, func_80035B64 → sprAnim_setPlaying, func_80035B74 → sprAnim_setOnDone,
  func_80035B7C → sprAnim_reset (the last three once when the help opens). medium (cov; not ablated).

## Screen fade (main tu_8002F7E8 + func_8002F6C8)  (status: proposed)
`D_80077C78` read every 2 frames after Triangle in `field_dali_inn`: +0 1, +1 1, +2 active, +8 = 0xC, +0xC counts
1..0xC, +0x10 = 0x80032238 (callback), +0x24 0xFFFFFF, +0x28 rising 0x18 per frame (the screen darkens); then a
0x10-frame fade from 0xFFFFFF back to 0 as the menu appears.
- D_80077C78 → g_fade (active +2, frames +8, frame +0xC, onDone +0x10, otz +0x1C, startColor +0x20, endColor
  +0x24, color +0x28). high.
- func_8002FC8C → fade_start (`(type, subtract, frames, color)`). high.
- func_8002FAAC → fade_out (`(frames)`, 0 → 0xFFFFFF), func_8002FA50 → fade_in (`(frames)`, 0xFFFFFF → 0),
  func_8002F9F0 → fade_outDefault (12), func_8002F9D0 → fade_inDefault (16): the menu-open sequence above. high.
  func_8002FB64 → fade_outAdditive, func_8002FB08 → fade_inAdditive (same with flag 0). medium (code).
- func_8002F8A8 → fade_setOnDone, func_8002F810 → fade_isBusy (every menu input handler returns while it is set),
  func_8002F8E4 → fade_setOtz (returns the old value; screens set 3, the shop 7), func_8002F904 → fade_getOtz. high.
- func_8002F6C8 → fade_update: every frame in field and menus; calls onDone when the count reaches the length
  (traces: 0x80036560 and 0x80032238 called from func_8002F6C8+0x110). high. It is the last function of menulist.c
  by the current split; the TU boundary may be at 0x8002F6C8 (the data-cut window is 0x8002F6C8-0x8002F800).
- func_8002F7E8 → fade_setActive, func_8002F800 → fade_isActive, func_8002FC38 → fade_init. medium (code).

## Window helpers (main window.c)  (status: proposed)
- func_8002DEE4 → win_drawBox (`(RECT *, otz, style)`): ablation at `menu_main`: every window frame and fill goes,
  the contents stay. high.
- func_8002CD4C → win_drawBoxEx (`(RECT *, otz, style, abr)`): the body of 8002DEE4 (style 0 panel, 1 ruled list,
  2 window, 3/4 help). medium (code).
- func_8002E040 → win_drawListBox (`(RECT *, otz, style, MenuList *)`): ablation at `menu_items`: the item list's
  frame, fill and scroll arrows go, the rows stay. high.
- func_8002DC54 → win_openFree (`(RECT *, otz, style)`): first free slot + win_init; the item Arrange window, shop
  quantity window and card Discard window open through it (cov i_arr, sh_buyx, c_x). high.
- func_8002E304 → win_setTitle (`(Window *, label)`), func_8002E588 → win_drawTitle: ablating 8002E588 in the Moogle
  window removes the "MOGNET" label; msg_open passes its labels through 8002E304. high.
- func_8002DE60 → win_closeAllNow: all live windows to state 0 with their onClosed callback (m_close: once, from
  func_80032384). medium.

## UI drawing and inventory helpers (main ui.c / text.c / chr.c / tu_800654F8)  (status: proposed)
- func_80026FC4 → ui_drawStatusIcons (`(x, y, otz, u8 bits, flag)`): Character+0x38 poked to 0x7F / 0x01 / 0x04 for
  Zidane / Vivi / Freya at `menu_main`: 7, 1 and 1 icons appear after the names (`build/rtf/stat.png`). high.
- Character.unk38 → Character.status (the persistent status bits those icons show; ovl_33 treats `& 9` as "can't
  use abilities"). high.
- func_800283E0 → ui_drawPortrait (`(x, y, otz, slot, h, ko)`): ablation at `menu_main` and in the item target
  window: the portraits go. high.
- func_800273C0 → ui_drawStones (`(x, y, otz, cur, max)`): ablation: the gem icon and "37/99" go. high.
- func_80026EA0 → ui_drawMemberSummary (`(x, y, otz, slot, mode)`): ablation in the item target window: portraits and
  stats go. func_80026F8C → ui_drawMemberStats: ablation: name/Lv/HP/MP go, portraits stay. high.
- func_80024428 → ui_drawMemberHeader: ablation at `menu_status`: the portrait/name/Lv/HP/MP block goes (status,
  equip and ability screens share it). high.
- func_80028D08 → ui_startMemberSwitch (`(state, from, to, dir)`): R1 on the status screen (s_r1) called it with
  (hdr, 0, 1, 2) and the header slid to Vivi. func_80028CF8 → ui_initMemberSwitch (once per screen init),
  func_80028D2C → ui_updateMemberSwitch (every frame). medium-high.
- func_80027834 → ui_buildSprite (`(x, y, ot, id, flag, *w, pkt)`): ablation at `menu_status`: the equipment-slot
  icons and the "Lv" of "Next Lv" go. high.
- func_80026C7C → ui_buildItemIcon: packet builder of ui_drawItemIcon (its stub corrupts the item list). medium.
- func_800269F4 → ui_drawEtchedLine (`(x, y, otz, len)`: three 1-pixel lines, vertical when len < 0). medium (code;
  its stub showed no visible change on the status screen).
- func_80027D84 → item_add (`(id, n)`): equipping Mage Masher over The Ogre (e_eq) called it with (8, 1) and the old
  weapon went back to the inventory; buying a Dagger called it with the quantity (sh_buyxx: stock 6 → 7). high.
- func_80032E84 → chr_calcEquipStats (`(in, out)`): 53 calls while the Equip item list is open (e_eq), feeding the
  stat comparison; func_8003384C → chr_updateStats: once per equip change (e_eq). medium.
- func_800654F8 → item_useOnMember (`(user, target, item, effect, all)`): Potion on Zidane (i_use: HP 100 → 200);
  the Ability screen's field abilities go through it too (ovl_33 801EED10). high.

## Card inventory (main tu_80062344)  (status: proposed)
`D_80083248` at `menu_card`: s16 wins 10, losses 3, draws 3 (the screen: "Win 10 Lose 3 Draw 3"), then 0x69 cards of
6 bytes from +6 (id, arrows, attack, type, pdef, mdef; 0xFF = empty slot). 23 slots used, 11 distinct ids, id 0 four
times: the screen shows "Stock: 23", "Type: 11" and "1/4 Goblin" on card 0.
- D_80083248 → g_cards (wins, losses, draws, cards[0x69]). high.
- func_80062498 → card_getWins, func_800624D8 → card_getLosses, func_8006251C → card_getDraws (the INFO window
  prints them, capped at 99999); func_800624A8 → card_addWin, func_800624E8 → card_addLoss, func_8006252C →
  card_addDraw (+1, capped at 9999). high (values) / medium for the add functions (code).
- func_800625F8 → card_countAll (23 = "Stock"), func_80062638 → card_countKinds (11 = "Type"), func_80062560 →
  card_countOf(id) (4 for Goblin, "1/4"). high.
- func_800625A0 → card_find (`(id, n)`: n-th copy), func_800626D0 → card_remove (`(id, n)`, called by the Discard
  confirmation 0x801F2578), func_8006276C → card_getName, func_80062794 → card_getNameWidth. medium-high.
- func_8006238C → card_add (`(id)`: random stats via 80062830, -1 when 100 cards), func_80062420 → card_addWithStats,
  func_80062830 → card_rollStats, func_80062344 → card_initAll, func_80062724 → card_clearAll, func_80062B30 →
  card_getInfoTable, func_80062824 → card_getInventory, func_800627BC → card_getRarity (id → 0..6, the grid icon
  class). medium (code).

## Item screen (ovl_37)  (status: proposed)
State `menu_items` (state 2, Use tab). `D_801FAB78` (ItemMenu): +0 tab (0 Use, 1 Arrange, 2 Key), +1 state, +2 target
member, +5 arrange choice, +6/+7 manual-arrange first slot. State handlers `D_801F307C` (read): [1] 801F0D18, [2]
801F22C8, [3] 801F0E10, [4] 801F23C4, [5] 801F0F50, [6] 801F2488. Tab table `D_801F3040` {onSelect, drawRow, list}:
Use/Arrange 801F10C4, 801F2870; Key 801F253C, 801F29D0.
- D_801FAB78 → s_menuItem, D_801F307C → s_menuItemInput, D_801F3040 → s_menuItemTabs. high.
- func_801F0C18 → menuItem_init, func_801F2160 → menuItem_update, func_801F2190 → menuItem_exit (screen-1 entry
  of g_menuScreensField: 0x801F0C18/0x801F2160/0x801F2190). high.
- func_801F21E8 → menuItem_input (dispatch). func_801F0D18 → menuItem_inputTabs (state 1; i_tab: Circle from the
  list). func_801F22C8 → menuItem_inputList (state 2; i_idle). func_801F0E10 → menuItem_inputTarget (state 3; i_use
  61 frames). func_801F23C4 → menuItem_inputArrange (state 4; i_arr). func_801F0F50 → menuItem_inputManualArrange
  (state 5; i_arrman 114). func_801F2488 → menuItem_inputKeyItem (state 6; i_keyshow). high.
- i_use (Zidane's HP poked to 100, Cross on Potion, Cross on Zidane): func_801F24DC → menuItem_selectRow (calls the
  tab's onSelect) → func_801F10C4 → menuItem_selectItem → func_801F1778 → menuItem_openTarget; then
  func_801F2590 → menuItem_useOnTarget (`(id)`, 0xEC) → item_remove(0xEC, 1), HP 100 → 200. high.
- func_801F2CC4 → menuItem_onTargetOpened, func_801F2C58 → menuItem_closeTarget (i_tgtcancel: Circle),
  func_801F2CD4 → menuItem_onTargetClosed. high.
- func_801F2D58 → menuItem_openArrange (i_arr: win_openFree + callbacks), func_801F2E40 → menuItem_drawArrangeWindow
  (its content: two text lines, Auto/Manual), func_801F2EBC → menuItem_onArrangeOpened (state 4),
  func_801F2ECC → menuItem_onArrangeClosed, func_801F2E18 → menuItem_closeArrange. high / medium for the last two.
- func_801F187C → menuItem_sortAuto: Arrange → first choice; sorts the inventory by item-table +0x10, then id. medium (code).
- func_801F253C → menuItem_selectKeyItem, func_801F2F38 → menuItem_showKeyItem (i_keyshow: once each),
  func_801F2FA0 → menuItem_hideKeyItem, func_801F1D88 → menuItem_updateKeyItemAnim, func_801F1E88 →
  menuItem_drawKeyItem (the slide-in key item card). medium-high.
- Ablation at `menu_items` (`abl_items.png`, `abl_items2.png`, `abl_t.png`): func_801F262C → menuItem_drawTabs
  (Use/Arrange/Key bar), func_801F11E4 → menuItem_drawList (the list window), func_801F13F8 → menuItem_drawCursor,
  func_801F2870 → menuItem_drawRow, func_801F2240 → menuItem_draw (everything but the title box),
  func_801F27A0 → menuItem_drawTargetWindow (with the target window open). high.
- func_801F29D0 → menuItem_drawKeyRow (Key tab only: i_key / i_keyshow). high.
- func_801F26E0 → menuItem_drawScrollRows (rows while the list scrolls), func_801F1B78 → menuItem_updateHelp,
  func_801F2280 → menuItem_updateAnims, func_801F2CEC → menuItem_collectKeyItems, func_801F2FC4 →
  menuItem_getSlot (`(MenuList *)`), func_801F2EDC → menuItem_loadImages. medium (code + cov).
- Item table (`D_80069018`, Unk80069018) +4 = price: the shop shows Dagger 320 and charges 320 (below); sell
  price = +4 >> 1 (0x801F7BC4). high.

## Status screen (ovl_38)  (status: proposed)
State `menu_status`. `D_801F120C` (StatusMenu): +0 state, +1 open ability pages, +2 member. Handlers `D_801EFBA4`:
[1] 801EE568, [2] 801EF8A0.
- D_801F120C → s_menuStatus. high.
- func_801EE440 → menuStatus_init, func_801EF69C → menuStatus_update, func_801EF6CC → menuStatus_exit (screen 4
  entry). high.
- func_801EF708 → menuStatus_input, func_801EE568 → menuStatus_inputMain (s_idle), func_801EF8A0 →
  menuStatus_inputAbilities (s_x). high.
- func_801EF568 → menuStatus_setMember (`(slot, dir)`): R1 (s_r1) → (1, 2), the screen switches to Vivi; func_801EFAF8
  → menuStatus_onMemberSwitched (the switch-animation callback, s_r1 once). high.
- func_801EF494 → menuStatus_openAbilityPage (Cross, s_x: page NO1 opens; again, NO2), func_801EF9D4 →
  menuStatus_closeAbilityPages (Circle, s_xo), func_801EFA80 → menuStatus_onPageOpened, func_801EFABC →
  menuStatus_onPagesClosed. high.
- Ablation (`abl_st.png`): func_801EE650 → menuStatus_drawHeader (portrait/name/Lv/HP/MP, stones, status icons),
  func_801EE740 → menuStatus_drawCommands (COMMAND window), func_801EE9E8 → menuStatus_drawStats (Speed .. Magic
  Eva), func_801EED18 → menuStatus_drawExp (Trance, EXP, Next Lv), func_801EEF00 → menuStatus_drawEquipment,
  func_801EF904 → menuStatus_drawFrame. high.
- func_801EF760 → menuStatus_draw, func_801EF0A0 → menuStatus_drawAbilityPages, func_801EF258 →
  menuStatus_drawAbilityRow (name + AP stars; 504 calls with one page open, 1360 with two). high.
- func_801EF80C → menuStatus_updateAnims, func_801EFB08 → menuStatus_updateHelp. medium.
- func_80027170 → ui_buildApGauge (main; the star/AP gauge of every ability row). medium.

## Ability screen (ovl_33 tu_801EE7C0)  (status: proposed)
State `menu_ability`. `D_801FBF84` (EquipMenu, despite the name): +0 tab (0 Use, 1 Equip), +1 state, +2 member,
+3 target. Handlers `D_801F33A8`: [1] 801EEA50, [2] 801F070C, [3] 801EEBE0.
- D_801FBF84 → s_menuAbility. high.
- func_801EE7C0 → menuAbility_init, func_801F05FC → menuAbility_update, func_801F062C → menuAbility_exit (screen 2
  entry). high.
- func_801F0664 → menuAbility_input, func_801EEA50 → menuAbility_inputTabs (m_abil 181; Square → menuScreen_switch(3)),
  func_801F070C → menuAbility_inputList (ab_sup 86), func_801EEBE0 → menuAbility_inputTarget. high / medium for
  the last (code).
- func_801EFE2C → menuAbility_toggleSupport: run ab_sup (Right, Cross, Cross at `menu_ability`): called once,
  func_8003493C(0, 0xD7, 0) followed, Zidane's stones 37 → 42 and Character+0x88 bit 0x00800000 cleared. high.
  func_8003493C → chr_setSupportAbility (`(charId, abilityId, on)`). high. Character.unk88 → supportAbilities. high.
- func_801EFCB4 → menuAbility_selectAction (Use tab, opens the target window), func_801EED10 →
  menuAbility_useOnTarget (item_useOnMember, pays MP), func_801F0DDC → menuAbility_closeTarget, func_801F0E3C →
  menuAbility_onTargetOpened, func_801F0E4C → menuAbility_onTargetClosed. medium (code).
- func_801EFB8C → menuAbility_getActionState (`(id, c)`: 0 hidden, 2 greyed, 3 usable; 1584 calls in m_abil),
  func_801F0CC4 → menuAbility_getSupportState (4 = equipped; ab_sup 1863 calls), func_801F1018 → menuAbility_getMpCost.
  medium-high.
- func_801EF900 → menuAbility_setMember (`(slot, dir)`: builds the two lists; m_abil once), func_801F0ED4 →
  menuAbility_onMemberSwitched, func_801F1048 / func_801F0410 → menuAbility_initActionCursor /
  menuAbility_initSupportCursor, func_801F0EE4 → menuAbility_loadImages. medium.
- Ablation (`abl_ab.png`): func_801F07DC → menuAbility_drawTabs, func_801EEEA8 → menuAbility_drawPlayer (PLAYER
  window), func_801EEFDC → menuAbility_drawInfo (INFO), func_801EF270 → menuAbility_drawCommand (COMMAND window),
  func_801EF3A8 → menuAbility_drawList, func_801F0934 → menuAbility_drawRow, func_801EF644 → menuAbility_drawCursor.
  high. func_801F06BC → menuAbility_draw, func_801F09CC → menuAbility_drawActionRow (Use tab: m_abil only),
  func_801F0AB8 → menuAbility_drawSupportRow (Equip tab: ab_sup only), func_801EF560 → menuAbility_drawTargetWindow,
  func_801EE978 → menuAbility_updateAnims. medium-high.

## Equip screen (ovl_33 tu_801F1150)  (status: proposed)
State `menu_equip`. `D_801FC840` (AbilityMenu, despite the name): +0 tab (Equip/Optimize/Off), +1 state, +2 slot,
+3 member. Handlers `D_801F7420`: [1] 801F12C8, [2] 801F1454, [3] 801F2DB4.
- D_801FC840 → s_menuEquip. high.
- func_801F1150 → menuEquip_init, func_801F2C0C → menuEquip_update, func_801F2C3C → menuEquip_exit (screen 3
  entry). high.
- func_801F2C78 → menuEquip_input, func_801F12C8 → menuEquip_inputTabs (m_equip 181), func_801F1454 →
  menuEquip_inputSlot (e_eq 81), func_801F2DB4 → menuEquip_inputList (e_eq 53). high.
- Run e_eq (Cross, Cross on the weapon slot, Down, Cross): func_801F2894 → menuEquip_buildItemList, func_801F301C →
  menuEquip_openList, func_801F30B0 → menuEquip_onListOpened, func_801F30D8 → menuEquip_equipSelected (item_add(8,1),
  item_remove(7,1): weapon 8 → 7), func_801F32E0 → menuEquip_applyEquip (`(Character *)`), func_801F26C4 →
  menuEquip_dropUnlearnedSupport, func_801F307C → menuEquip_closeList, func_801F30C0 → menuEquip_onListClosed
  (each once). high.
- func_801F24EC → menuEquip_optimize, func_801F31B0 → menuEquip_removeSlot (Off). medium (code).
- func_801F3278 → menuEquip_setMember (`(slot, dir)`), func_801F3264 → menuEquip_onMemberSwitched. medium.
- Ablation in the weapon list (`abl_eq.png`): func_801F2E34 → menuEquip_drawTabs, func_801F15E0 → menuEquip_drawInfo
  (INFO with stats), func_801F1A94 → menuEquip_drawAbilities (ABILITY window), func_801F1F08 → menuEquip_drawList
  (WEAPON list window), func_801F239C → menuEquip_drawCursor, func_801F20A0 → menuEquip_drawStatCompare (the
  "99 → 32" columns), func_801F2F3C → menuEquip_drawListRow. high. func_801F189C → menuEquip_drawEquipment
  (EQUIPMENT window; hidden under the list during the ablation) medium-high, func_801F2CD0 → menuEquip_draw,
  func_801F3344 → menuEquip_drawStatLabel. medium.

## Card screen (ovl_36)  (status: proposed)
State `menu_card`. `D_801F347C` (CardMenu): +0 state, +1 row, +2 column, +7 collector rank, +8 count per card id,
+0x6C shown copy per id, +0x18F0 collector points {kinds, arrows, types, total} = {110, 1, 5, 116} (the screen:
"Collectors Lv: 116p Beginner"). Handlers `D_801F30F4`: [1] 801F0AE8, [2] 801F2578.
- D_801F347C → s_menuCard. high.
- func_801F08F4 → menuCard_init, func_801F23DC → menuCard_update, func_801F240C → menuCard_exit (screen 6). high.
- func_801F24A0 → menuCard_input, func_801F0AE8 → menuCard_inputGrid (c_move/c_r1), func_801F2578 →
  menuCard_inputDiscard (c_x/c_xo). high.
- func_801F2C5C → menuCard_selectCard (`(id)`: Down in c_move → (1)), func_801F1778 → menuCard_loadCardImage (the
  selected card's image decoded with 801F3034 and uploaded; c_move once), func_801F3034 → menuCard_decodeRle,
  func_801F2FE0 → menuCard_flipCopy (`(id, dir)`: R1 in c_r1 → (0, 1)), func_801F2B94 → menuCard_updateFlip. high.
- func_801F0CC8 → menuCard_openDiscard (Cross, c_x: window with "Discard?" and Yes/No), func_801F2F34 →
  menuCard_drawDiscardWindow, func_801F2F14 → menuCard_onDiscardOpened, func_801F2F24 → menuCard_onDiscardClosed
  (c_xo). high.
- func_801F1C88 → menuCard_refresh (counts per id via card_countOf, collector points, rank), func_801F2138 →
  menuCard_calcCollectorPoints (out = 110, 1, 5, 116 at `menu_card`). high.
- Ablation (`abl_card.png`): func_801F0DB0 → menuCard_drawGrid (CARD window), func_801F0F9C → menuCard_drawInfo
  (INFO: rank, Win/Lose/Draw), func_801F2664 → menuCard_drawCardInfo (CARD INFO window), func_801F166C →
  menuCard_drawCursor, func_801F2860 → menuCard_drawCell, func_801F12B8 → menuCard_drawCardStack, func_801F2730 →
  menuCard_drawCard, func_801F2A04 → menuCard_drawArrows, func_801F2AB8 → menuCard_drawStats (the "8PFF" string),
  func_801F1AE8 → menuCard_drawFace. high. func_801F1508 → menuCard_drawCellHighlight (pulsing quads at the cursor
  cell), func_801F1D6C → menuCard_drawSprite, func_801F2508 → menuCard_draw. medium.
- func_801F1EA0 / func_801F1FE8 → menuCard_saveVram / menuCard_restoreVram (StoreImage on init, LoadImage on exit),
  func_801F2CD4 → menuCard_initImageCache, func_801F2D30 → menuCard_findCachedImage, func_801F2D88 →
  menuCard_showCachedImage. medium (code).

## Config screen (ovl_34)  (status: proposed)
State `menu_config`. Each row toggled with Right then Left (script in this campaign), GameState+0x24 before/after
(base 0x00019AC0): row 0 Sound bit 0 (1 = Mono), 1 Controller bit 1 (Custom), 2 Cursor bit 2 (Memory), 3 ATB bit 3
(Wait), 4 Battle Camera bit 4 (Fixed), 5 Battle Control bit 5 (Custom), 6 Movement bit 6 (1 = Run, the save's
setting), 7 Battle Speed bits 8-10 (2), 8 Field Message bits 14-16 (6), 9 HERE Icon bit 17 (1 = OFF), 10 Window
Color bit 18 (1 = Classic), 11 Target Window bit 19 (Always), 12 Vibration bit 7 (no change: no analog pad).
Row names from the screen (`build/rt/cfg_scroll.png` for rows 4-12).
- GameState.unk24 → GameState.config; ConfigBits b0 → soundMono, b1 → padCustom, b2 → cursorMemory, b3 → atbWait,
  b4 → battleCameraFixed, b5 → battleControlCustom, b6 → movementRun, b7 → vibration, b8 → battleSpeed,
  b14 → messageSpeed, b17 → hereIconOff, b18 → windowClassic, b19 → targetWindowAlways. high. b11 (bits 11-13) and
  b20 (per-character controller port for Battle Control, 801EEEF8) stay as they are. `(unk24 >> 18) & 1`, read by
  all window drawing as "abr", is the Window Color option. GameState.unk28 = the custom pad mapping (4 bits per
  action, drawn by 801EF110). medium.
- D_801F1558 → s_menuConfig. high.
- func_801EEA1C → menuConfig_init, func_801F00A8 → menuConfig_update, func_801F00D8 → menuConfig_exit (screen 7). high.
- func_801F00F8 → menuConfig_input, func_801EECB4 → menuConfig_inputList (m_config 185), func_801EF50C →
  menuConfig_setOption (`(u8 value)`: the bit writes above), func_801EFA54 → menuConfig_stepSlider (slider rows
  change after 5 frames), func_801F02CC / func_801F0380 → menuConfig_pageUp / menuConfig_pageDown. high.
- func_801EF938 → menuConfig_openSubWindow (Cross on Controller/Battle Control when Custom), func_801EED9C →
  menuConfig_inputPadMap, func_801EEEF8 → menuConfig_inputBattleControl, func_801EF110 → menuConfig_drawPadMap,
  func_801EF2B4 → menuConfig_drawBattleControl, func_801F0BE8 → menuConfig_drawSubWindows. medium (code).
- Ablation (`abl_cfg.png`): func_801EEB68 → menuConfig_drawRows, func_801EF3F8 → menuConfig_drawCursor,
  func_801F0C78 → menuConfig_drawScrollArrows, func_801F0550 → menuConfig_drawRow, func_801F0D20 →
  menuConfig_drawChoice (the two option words), func_801EFC20 → menuConfig_drawSlider, func_801F05D0 →
  menuConfig_drawSoundRow (the Sound row goes), func_801F08F8 → menuConfig_drawBattleSpeedRow (the Battle Speed row
  goes). high. The other row drawers of the same table (`D_801F14BC`; row i prints title `D_801F13F0[i]` and reads
  its own bit; rows matched to the screen): func_801F0640 → menuConfig_drawControllerRow, func_801F06B4 →
  menuConfig_drawCursorRow, func_801F0728 → menuConfig_drawAtbRow, func_801F079C → menuConfig_drawCameraRow,
  func_801F0810 → menuConfig_drawBattleControlRow, func_801F0884 → menuConfig_drawMovementRow, func_801F0974 →
  menuConfig_drawMessageSpeedRow, func_801F09F0 → menuConfig_drawHereIconRow, func_801F0A64 →
  menuConfig_drawWindowColorRow, func_801F0AD8 → menuConfig_drawTargetWindowRow, func_801F0B4C →
  menuConfig_drawVibrationRow. high.
- func_801F0184 → menuConfig_draw, func_801F025C → menuConfig_updateAnims, func_801EFB38 → menuConfig_updateHelp,
  func_801F0DD8 → menuConfig_drawSliderRow, func_801F0FE8 → menuConfig_drawText. medium.

## Shop (ovl_3c)  (status: proposed)
States `menu_shop_weapons` (menu type 2, shop 0, "Weapons") and `menu_shop_synth` (shop 0x20, "Synth"). `D_801FC2D0`
(shop): +0 kind (0 items, 1 equipment, 2 synthesis; 1 here), +1 Buy/Sell/Exit cursor, +2 entry count, +3 in a
sub-screen, +8 shop id, +0x10 help text. `D_801FBBA0[kind][entry]` = {init, update, exit} (read): kinds 0/1: Buy
801F4B48/801F5940/801F5988, Sell 801F7770/801F832C/801F835C; kind 2: Synth 801F5F88/801F7390/801F73C0. Buy state
`D_801FC2F8`, Synth `D_801FC338`, Sell `D_801FC438` (+0 state 1 list, 2 quantity window). Run sh_buyxx (Buy, Cross,
Cross on Dagger 320 G): funds 6028798 → 6028478 and "Stock" 6 → 7 (`build/rtf/mont.png` of that run).
- D_801FC2D0 → s_shop, D_801FBBA0 → s_shopModes, D_801FC2F8 → s_shopBuy, D_801FC338 → s_shopSynth,
  D_801FC438 → s_shopSell. high.
- func_801F45E4 → shop_init, func_801F4688 → shop_update, func_801F4720 → shop_exit (`D_8006C9D4[2]`). high.
- func_801F40AC → shop_inputMenu (Buy/Sell/Exit: sh_idle, sh_down), func_801F4960 → shop_selectMenu (once per Cross),
  func_801F4AC0 → shop_enterMode (fade callback: runs the mode's init), func_801F4B38 → shop_onModeEntered,
  func_801F43F8 → shop_leaveMode (Circle in a list), func_801F4854 → shop_onModeLeft, func_801F4A14 → shop_close
  (sh_exit: Circle on the menu). high / medium for the leave pair (code).
- func_801F4A90 → shop_getKind (`(id)`: id >= 0x20 → 2, else func_801F59A8 → shop_isEquipmentShop: 1 when every
  item is equipment). high (shop 0 → 1, 0x20 → 2).
- func_801F442C → shop_setHelp (`(text id)`), func_801F49C4 → shop_updateHelp, func_801F4458 → shop_getId,
  func_801F4468 → shop_collectMembers (party list for the stat comparison), func_801F48F4 → shop_countEntries,
  func_801F42A8 → shop_sumArmorStat, func_801F4A44 → shop_loadImages. medium (code + cov).
- Ablation (`abl_shop*.png`): func_801F4000 → shop_drawMessage (the top help line; needs 60 frames),
  func_801F4748 → shop_drawTitle ("Weapons" box), func_801F4198 → shop_drawMenu (Buy/Sell/Exit window),
  func_801F47F8 → shop_drawMenuCursor, func_801F44F4 → shop_drawStatIcon. high. func_801F47C8 → shop_drawMenuAll. medium.
- Buy: func_801F4B48 → shopBuy_init, func_801F5940 → shopBuy_update, func_801F5988 → shopBuy_exit,
  func_801F5A1C → shopBuy_input, func_801F5AE4 → shopBuy_inputList (sh_buy), func_801F57C8 → shopBuy_openQuantity
  (Cross on Dagger, sh_buyx), func_801F4C68 → shopBuy_inputQuantity (Left/Right ±1, Up/Down ±10), func_801F5E4C →
  shopBuy_confirm (item_add, gil -= price × n; sh_buyxx), func_801F5D14 → shopBuy_closeQuantity, func_801F5D40 /
  func_801F5D6C → shopBuy_onQuantityOpened / shopBuy_onQuantityClosed, func_801F5D98 → shopBuy_loadList (the shop's
  item list `D_801FBC44[id]`), func_801F5A74 → shopBuy_draw, func_801F5AAC → shopBuy_updateList, func_801F5B64 →
  shopBuy_drawScrollRows. high (cov) / medium for draw/update wrappers.
  Ablation: func_801F4E1C → shopBuy_drawInfo (INFO: funds/stock/equipped), func_801F506C → shopBuy_drawList,
  func_801F51B8 → shopBuy_drawParty (ATTACK window: members and stat arrows), func_801F56E4 → shopBuy_drawCursor,
  func_801F5BCC → shopBuy_drawRow. high. func_801F551C → shopBuy_drawQuantityWindow (window content; sh_buyx 63). high.
- Sell (sh_sell, sh_sellx: list of the inventory, Cross opens QUANTITY + INFO windows): func_801F7770 → shopSell_init,
  func_801F832C → shopSell_update, func_801F835C → shopSell_exit, func_801F837C → shopSell_input, func_801F8434 →
  shopSell_inputList, func_801F80D0 → shopSell_openQuantity, func_801F7848 → shopSell_inputQuantity, func_801F8224 →
  shopSell_confirm (item_remove, gil += price/2 × n, cap 9999999; code), func_801F8680 → shopSell_closeQuantity,
  func_801F86D0 / func_801F86FC → shopSell_onQuantityOpened / shopSell_onQuantityClosed, func_801F79FC →
  shopSell_drawList, func_801F7BC4 → shopSell_drawQuantityWindow, func_801F7DA8 → shopSell_drawInfoWindow,
  func_801F7FE0 → shopSell_drawCursor, func_801F856C → shopSell_drawRow, func_801F84D8 → shopSell_drawScrollRows,
  func_801F83D4 → shopSell_draw, func_801F83FC → shopSell_updateList, func_801F8728 → shopSell_updateHelp.
  high for those seen in sh_sell/sh_sellx (init, update, input, inputList, openQuantity, inputQuantity, drawList,
  the two window contents, drawCursor, drawRow), medium for the rest (same structure as Buy).
- Synthesis (`menu_shop_synth`, sh_synth/sh_synthx: "What would you like to synthesize?", Required Item list):
  func_801F5F88 → shopSynth_init, func_801F7390 → shopSynth_update, func_801F73C0 → shopSynth_exit, func_801F73E0 →
  shopSynth_input, func_801F74A8 → shopSynth_inputList, func_801F6F00 → shopSynth_openQuantity, func_801F60A8 →
  shopSynth_inputQuantity, func_801F7614 → shopSynth_confirm (item_add product, item_remove both ingredients, gil;
  code), func_801F7590 → shopSynth_closeQuantity, func_801F75BC / func_801F75E8 → shopSynth_onQuantityOpened /
  shopSynth_onQuantityClosed, func_801F7250 → shopSynth_loadRecipes (recipes whose shop mask has bit id - 0x20),
  func_801F625C → shopSynth_drawInfo, func_801F65C8 → shopSynth_drawList, func_801F6714 → shopSynth_drawParty,
  func_801F6A7C → shopSynth_drawQuantityWindow, func_801F6C50 → shopSynth_drawCursor, func_801F6D34 →
  shopSynth_drawRow, func_801F7528 → shopSynth_drawScrollRows, func_801F7438 → shopSynth_draw, func_801F7470 →
  shopSynth_updateList, func_801F76C8 → shopSynth_updateHelp. high for the ones covered in sh_synth/sh_synthx
  (init, update, input, inputList, openQuantity, inputQuantity, the draw functions, onQuantityOpened), medium for
  the rest (code; parallel to Buy).

## Save / load (ovl_39)  (status: proposed)
State `menu_save` (menu type 4, arg 2: "Select MEMORY CARD slot." with slot 1 / slot 2). `D_801FB1A8`: +0 mode
(menu arg), +1 state (1 slot select, 4 file list, 3 confirm), +2 sub-state, +4[2] card status per slot, +0x24 file
MenuList, +0x34 file entries. Run sv_yesno (Cross on slot 2, Down to the empty FILE03, Cross) went straight to
"Saving..." (no confirmation for an empty block) and wrote a real save: memory card 2 block 6 "FF9/FILE03/99:56"
is new from this campaign. Runs sv_idle, sv_up, sv_slot2, sv_slot2down.
- D_801FB1A8 → s_menuSave. high.
- func_801F4414 → menuSave_init, func_801F5280 → menuSave_update, func_801F52C0 → menuSave_exit (`D_8006C9D4[4]`). high.
- func_801F4664 → menuSave_inputSlot (sv_idle/sv_up), func_801F58E0 → menuSave_selectSlot (Cross: sv_slot2),
  func_801F55D8 → menuSave_inputFileList (sv_slot2down), func_801F5960 → menuSave_selectFile (sv_yesno: once, then
  func_801F94D0 → menuSave_saveFile(2, 2) in save mode; func_801F9788 is the load-mode branch). high.
- func_801F63D8 → menuSave_openCheck: opens the CHECK window ("Do not remove MEMORY CARDs ...", "Checking...") and
  starts reading the card (sv_slot2 once). medium-high.
- Ablation (`abl_sv.png`, `abl_sv2.png`): func_801F4760 → menuSave_drawSlotWindow, func_801F4538 →
  menuSave_drawHelp (help line + slot name box), func_801F53E0 → menuSave_drawTitle ("Save" box), func_801F4840 →
  menuSave_drawFileList (FILE window), func_801F49C4 → menuSave_drawFile ("FILE01" etc. + the entry), func_801F4B18 →
  menuSave_drawFileInfo (portraits, name, Lv, time, gil, location), func_801F4E00 → menuSave_drawCursor,
  func_801F573C → menuSave_drawScrollArrows. high.
- func_801F500C → menuSave_updateHelp, func_801F5114 → menuSave_setFileHelp, func_801F5460 → menuSave_drawState,
  func_801F54C8 → menuSave_input, func_801F5550 → menuSave_updateState, func_801F5A68 → menuSave_drawBookIcon /
  func_801F5E68 → menuSave_drawBook (the animated book of the CHECK window), func_801F5BE0 / func_801F5D00 →
  menuSave_saveVram / menuSave_restoreVram. medium (code + cov).
- func_801F5E90 → menuSave_openConfirm (`(text, yes, no)`), func_801F5FE4 → menuSave_drawConfirm, func_801F60DC →
  menuSave_drawConfirmCursor, func_801F61BC → menuSave_inputConfirm: the Yes/No dialog; not reached (empty block
  saves without asking). medium (code).

## Name entry (ovl_3a)  (status: proposed)
State `menu_name_entry` (menu type 1, arg 0 = Zidane: "Pick a name.", Alphabet/Symbol/Delete/Confirm/Default).
`D_801F4BBC`: +0 in the character grid, +1 command cursor, +2 page (Alphabet/Symbol), +3 grid cursor, +4 length,
+7 name buffer, +0x11 default name, +0x130 character.
- D_801F4BBC → s_menuName. high.
- func_801F3874 → menuName_init, func_801F4748 → menuName_update, func_801F4770 → menuName_exit (`D_8006C9D4[1]`). high.
- Ablation (`abl_name.png`, `abl_name2.png`): func_801F3FAC → menuName_drawPortrait, func_801F4110 →
  menuName_drawCommands, func_801F4238 → menuName_drawNameWindow (name + character grid), func_801F459C →
  menuName_drawCursor, func_801F48A0 → menuName_drawTitle ("Name" box), func_801F490C → menuName_drawHelp. high.
- func_801F3A98 → menuName_input, func_801F3BBC → menuName_inputGrid, func_801F47E8 → menuName_inputCommands,
  func_801F3D9C → menuName_confirm (Cross: add a character / page / Delete / Confirm copies the name to the
  Character / Default), func_801F47A0 → menuName_draw, func_801F49A0 → menuName_loadPortrait. medium (code).

## Party select (ovl_3b)  (status: proposed)
State `menu_party_select` (menu type 3; the bogus request filled all slots with Zidane, the layout is still the
real one: PARTY column, HELP "Form Party", INFO with the reserve grid).
- D_801F4748 → s_menuParty. high.
- func_801F3FEC → menuParty_init, func_801F405C → menuParty_update, func_801F4084 → menuParty_exit (`D_8006C9D4[3]`). high.
- Ablation (`abl_party.png`): func_801F3B3C → menuParty_drawParty, func_801F3CF0 → menuParty_drawInfo,
  func_801F3E58 → menuParty_drawCursor, func_801F4190 → menuParty_drawTitle, func_801F4204 → menuParty_drawHelp,
  func_801F43FC → menuParty_drawPortrait. high. func_801F4360 → menuParty_drawMember, func_801F4150 → menuParty_draw. medium.
- func_801F40AC → menuParty_input, func_801F37A8 → menuParty_inputParty, func_801F38D8 → menuParty_inputReserve,
  func_801F3A60 → menuParty_pick (first Cross picks, second swaps), func_801F42A4 → menuParty_confirm (Circle:
  party_setMember for each slot, close). medium (code).

## Chocograph (ovl_32)  (status: proposed)
`menu_chocograph` (none owned) and `menu_chocograph2`: before opening, g_evtGlobalVars bytes 0xB8.. = 03 01 00,
0xBB.. = 07 00 00, 0xBF = 03 were poked; the screen then shows "Inventory: 3", "Loc. Found: 3", three chocobo
abilities (Field, Reef, Mt.) and the list Streamside / Between mountains / Uncultivated land (`ch_list.png`). So
g_evtGlobalVars b[0xBB..0xBD] = chocographs owned (bit per chocograph), b[0xB8..0xBA] = treasure found, b[0xBF] =
chocobo ability count, and GameData.unk844 = the selected chocograph (1-24, 0 none). high.
- D_801F9850 → s_menuChocograph. high.
- func_801F7AC0 → menuChocograph_init, func_801F8FF8 → menuChocograph_update, func_801F904C → menuChocograph_exit. high.
- Ablation (`abl_choco.png`): func_801F9164 → menuChocograph_drawTitle, func_801F91D8 → menuChocograph_drawTabs
  (Select/Cancel), func_801F7EB4 → menuChocograph_drawSelected, func_801F8010 → menuChocograph_drawList,
  func_801F8130 → menuChocograph_drawCounts, func_801F92C0 → menuChocograph_drawAbilities, func_801F8240 →
  menuChocograph_drawHint, func_801F8404 → menuChocograph_drawCursor, func_801F93FC → menuChocograph_drawRow. high.
- func_801F907C → menuChocograph_input, func_801F7C8C → menuChocograph_inputTabs, func_801F7DC0 →
  menuChocograph_inputList (Cross selects when the ability count suffices), func_801F90D4 → menuChocograph_draw,
  func_801F85EC → menuChocograph_readFlags (the evt vars above), func_801F84F0 → menuChocograph_updateHelp,
  func_801F95E8 → menuChocograph_getRequiredAbility, func_801F88F4 → menuChocograph_drawPicture, func_801F9518 →
  menuChocograph_drawIcon. medium (code).

## Disc change (ovl_3e)  (status: proposed)
`menu_disc_change` (menu type 8, arg 0x3A98): the "Change to DISC" art over the whole screen.
- func_801F605C → discChange_init, func_801F65A4 → discChange_update, func_801F65D4 → discChange_exit
  (`D_8006C9D4[8]`; exit jumps to the arg's field unless it is 15000). high.
- func_801F6124 → discChange_drawBackground: ablation: the whole picture goes (`abl_disc.png`). high.
- func_801F6368 → discChange_checkDisc (CdControlB/CdGetDiskType; 7 = right disc), func_801F673C →
  discChange_loadPicture, func_801F6254 → discChange_drawPanel (states 3-4), func_801F6654 / func_801F66AC /
  func_801F66F4 → discChange_input / discChange_draw / discChange_updateState (tables `D_801F6B60`, `D_801F6B74`,
  `D_801F6B88`). medium (code).

## Message windows (main msg.c)  (status: proposed)
`field_dali_inn_moogle`, Cross on the Moogle: MOGNET window "Gumo / Can I help you, kupo? / Save / Tent / Mognet /
Cancel". After each Down (frames 100, 120, 140): D_8007B958 0, 1, 2, 3; D_8007B95C 4; D_8007B968 2; D_8007B96C 3;
D_8007B970 2.
- D_8007B958 → g_msgChoice (cursor), D_8007B95C → g_msgChoiceCount, D_8007B968 → g_msgChoiceLine (first choice
  line in the text), D_8007B96C → g_msgChoiceCancel (Cancel = choice 3), D_8007B970 → g_msgChoiceWindow (window id).
  high. D_8007B9C0 → g_msgWindows (10 entries), D_8007BB78 → g_msgFocus (top window). medium.
- func_8004F5D4 → msg_drawWindowContent (the Window content callback set by msg_open): ablation removes the
  text, the frame stays (`abl_msg.png`). high.
- func_8004D940 → msg_drawText (`(msgwin)`), func_8004C8C4 → msg_renderText: ablation: the text goes. high.
- func_8004D2C0 → msg_advanceText: stubbed 8 frames after opening, no character ever appears (`abl_msg2.png`). high.
- func_8004F660 → msg_drawChoiceCursor (the Window overlay callback): stubbed from the start, no hand. medium-high.
- func_8004DAD0 → msg_input (`(msgwin, pad)`): Up/Down move g_msgChoice with sound 0x67, Cross confirms (cov:
  f_moogdown). medium-high.
- func_8004E248 → msg_getLayout (`(id, *x, *y, *z)`): parses the message's leading 0xF9 codes (size, choices,
  position); f_moog: once before msg_open, from func_80039660 (the event layout helper called by evt_execInstr). high.
- func_8004C2C0 → msg_measureLine (f_moog: once, from msg_getLayout; returns a line's width). medium.
- func_8004E7FC → msg_update (every field frame: pad edges → the focused window's input callback), func_8004C0E8 →
  msg_readPad, func_8004E8D4 → msg_getPadHeld, func_8004E8EC → msg_getPadPressed, func_8004E904 →
  msg_getPadReleased. medium (code; called every frame).
- func_8004E958 → msg_closeAll (menu open: once, from evt), func_8004BFB0 → msg_init (code: resets the windows and picks the text bank per mode), func_8004E16C →
  msg_resetChoices, func_8004EFB0 → msg_getEntry (`(id, *size)`), func_8004F058 → msg_getText, func_8004ED94 →
  msg_getWindow, func_8004EA3C → msg_getFocus, func_8004EA30 → msg_setFocus. medium (code).
- func_8004FF98 → msg_startHereIcon (`(reason)`; 0 = stop) and func_8004F74C → msg_drawHereIcon: the arrow over the
  player, gated by Config "HERE Icon" (bit 17) and g_evtState.controlEnabled. medium (code; the icon was not seen
  in the runs).

## Event system (main event.c)  (status: proposed)
- Unk8007AECC (g_evtState) +0x16EC → controlEnabled: at `field_dali_inn_moogle` 1 while walking, 0 during the
  Moogle dialog, 1 again after it closed. high. (+0x16ED is the known playerUid.)
- func_80048A5C → evt_checkPlayerAction: every field frame; on Cross it called func_80038618(player, 4) → the Moogle
  (0x80078588) and then func_800384D0(moogle, 1, 3, 0) (run t_talk). Function 3 of an object is "talk", 2
  "touch/proximity", 8 the Square-button variant (0x80000 = card game challenge). high.
- func_80038618 → evt_findNearestObject (`(o, flags, *dist)`): flags 4 = in front, talk range; 0 = collision
  (func_800475D8 calls it while walking). high.
- func_800384D0 → evt_callObjectFunc (`(o, level, func, save)`): runs script function `func` of object o at
  priority `level` (t_talk: moogle func 3; evt_execInstr+0x73C: player func 0x16). high.
- func_80038278 → evt_returnFromFunc: pops the object's call stack when a function ends (t_talk: once, from the
  expression code). medium.
- func_80037D6C → evt_getSysVar (`(id)`): system variables of the expression evaluator (0 random, 2 controlEnabled,
  6 gil, 20 play time, 24 battle id, ...); called from evt_runExpr (t_talk: 35 calls with id 2). high.
- func_800475D8 → evt_movePlayer: pad → player direction/speed; Config Movement bit (b6) picks run (60) or walk
  (30), Circle inverts it. Runs every field frame (f_idle/f_walk). high.
- func_80038990 → evt_moveObjectTo (`(x, y, z, flags)`): script-driven walk toward a point with collision. medium (code).
- func_80048F54 → evt_updateObjects (all objects each frame), func_8004883C → evt_updateIdleTimer (random idle
  animation timer of the player). medium.
- func_8004AE00 → evt_getPlayer (the object whose uid is playerUid, i.e. Zidane 0x8007805C in Dali), func_8004AE84 → evt_isControlEnabled,
  func_8004AE9C → evt_getObjectList, func_8004B040 → evt_getObject (`(uid)`), func_8004A9E8 → evt_onMenuOpen (called
  by fld_main right after menu_open: closes message windows). medium-high.
- func_8004B68C → evt_getObjectProp (`(o, prop)`: 0-2 position, 3 direction, 4 flags, 5 uid, ...). medium (code).

## Field frame (ovl_0a fld_movie.c)  (status: proposed)
- func_800AE788 → fld_beginFrame: clears this frame's OT (ClearOTag/ClearOTagR); ablation at `field_dali_inn`: the
  background is replaced by stale white layers (`abl_f1.png`). high.
- func_800AE930 → fld_endFrame: soft reset (L1+L2+R1+R2+Start+Select), VSync, PutDispEnv/DrawOTag, buffer flip and the
  four-frame deferred flag queue (D_800C9EF0). medium-high (the btl_endFrame counterpart).
- func_800AB3CC → fld_drawModels: ablation: Zidane and the Moogle vanish, the background stays. high.
- func_800AB110 → fld_collectVisibleModels (`(list)` → count): its stub (no return value) also removed every model. medium.
- func_800BE158 → bg_drawOverlay (`(overlay, ...)`): 16 calls per field frame at `field_dali_inn`; its stub leaves
  only the models on a white screen (`abl_f3.png`). high. func_800BFEE0 → bg_draw: once per frame, updates the
  background (800BF0C4, 800BF660, 800BEDE8, 800BDC44) and calls 800BE158 for each visible overlay of the current
  camera (`D_800CA068.unk75`). medium-high. Both are in tu_800B61A4.c, which then looks like the field background
  module (D_800C9DA4 = its state, `.unk10` the loaded background header).
- func_800B61A4: its stub garbles the models' geometry (Zidane's mesh stretched across the room); leads only.
  No visible change in a still frame for 800B1E8C, 800B2D10, 800B3254, 800B3294, 800B3300, 800B3684, 800B4894,
  800BD4D4, 800BDC44, 800BEDE8, 800C1368, 800C3EC0, 800C44F4, 800C4EC8, 800C60F0, 800C7100, 800B7F14, 800BA560,
  800BF0C4, 800BF660, 800C3608 (`abl_f2-4.png`); 800B3378's stub turned the Moogle white (lighting). Not named.

## What the campaign did not reach  (status: reference)
- Field: walking events, NPC dialogue with speech tails (func_8002D398, the tail setter, is code-only), the "!"
  bubble (func_8004FBD8 / func_8004F9A0 did nothing visible next to the Moogle) and the HERE icon (not shown after
  Select); ATE prompt (func_8004FE0C); field → world exit; Chocobo minigames; the opcode handlers of evt_execInstr.
- Menus: the item Auto sort, Tetra Master challenge (Square), the save/load Yes/No dialog and loading a file,
  memory card wrappers 0x801FA0C8-0x801FAF74 of ovl_39 (all `_card_*` callers), menu types 6/7 (ovl_3d has no code),
  the world-map copies of the main menu (ovl_0c), ovl_65 beyond the battle campaign.
- Party select and Chocograph were opened outside their story context, so their logic (picking members against a
  real request, using a chocograph) was not exercised; the names there are mostly from ablation + code.

## Field fixture campaign (2026-10-28): method, states, helper scripts  (status: reference)
- Redux, disc 1 (disc 2 for the opcode survey). Any field is entered with `build/rtf2/enter_field.py FIELD [--disc N]
  [--base-state S] [--frames N] [--save NAME] [--poke ADDR=HEX]`, adapted from ff9-ds `tools/psx_field_fixture.py`
  but keeping the save's own party and event variables (no host export): from a paused field it sets
  GameState flag 2 + byte 10, breaks at main's dispatcher 0x80012834, writes the field's area/field extents (FF9.IMG
  dirs 2/4) into GameData.unk7FC/unk804, fieldId (+0xC), unk3A (area) and mode 1, then lets fld_main load it. The
  scripts run with the base save's story flags, so many fields play their cutscene or a variant of it.
- Opcode survey: `build/rtf2/op.lua` (one exec breakpoint on evt_execInstr, ONE shared callback that pcall()s a
  predefined function: no closure per hit) records the opcode under `g_evtCurObj->ip` (FF xx as 0xFFxx), the uid
  and up to 4 pcs per opcode; `build/rtf2/opscan.py FIELDS --frames 900` enters each field and saves
  `build/rtf2/ops_<disc>_<field>.json` + a screenshot; `build/rtf2/scanall.sh DISC 30` restarts Redux every 30
  fields. All 222 disc-1 fields were entered (900 VBlanks each; `build/rtf2/opsum_d1.txt` = per-opcode totals),
  plus the disc-2 fields listed below. Redux crashed twice during the long scans (`redux.log`: "1: (Table)" ...
  "Unknown error in event listener", then "X connection broken"), after ~65 fields each, with LuaJIT off; restarting
  every 30 fields avoided it.
- Object recording: `build/rtf2/rec.lua` (GPU::Vsync listener, pcall'd, `Rec.uninstall()` removes it) copies memory
  regions every VBlank inside Redux; `build/rtf2/evtrec.py NAME (--state S | --field F) --frames N [--seq ...]`
  records the whole `g_evtStateData` block (0x17C8 bytes) plus globals and `build/rtf2/evtan.py NAME [--uid U]
  [--globals] [--list]` prints every byte that changes per object. 1000 VBlanks take ~5 s (the old
  per-frame HTTP reads took >10 min). `build/rtf2/track.py STATE ADDR SIZE SEQ` is the per-frame HTTP version.
- Ablation with input after the stub: `build/rtf2/ablate2.py STATE SEQ FUNC...` (diff pixel counts vs no stub).
- Static exits: `build/rtf2/exits.py DISC` lists the fields whose scripts hold a literal `B6 00 <world id>` (world
  map exit) with the world ids in their dependency tables (uses tools/encounter_catalog.py's parsers).
- New states (disc 1, listed in `docs/emulator.md`; all made with enter_field.py from `field_dali_inn`, so they carry
  the Lv 50 Dali save's party and story flags): `field_burmecia_rain` (763, rain), `field_burmecia_bubble`
  (757, "?" over Zidane), `field_burmecia_btl47` (battle 47 forced from 763), `field_burmecia_rewards` (its rewards
  screen, ovl_65), `field_opening_dark` (50, "Sure is dark..." balloon), `field_dali_359` (Dali entrance, arrival
  cutscene), `field_ice_300` (Ice Cavern exit; walking down leaves for the world map), `field_worldmap_ice_exit`
  (world map 9000 outside the Ice Cavern, ovl_0c), `field_alex_100` (Alexandria street at night), `field_d1_101`
  (Alexandria street with control, scrolls), `field_d1_352` (Dali, scrolls), `field_d1_404`, `field_d1_501`
  (cutscenes with non-zero direction offsets), `field_d1_456` (animated flag and scrolling clouds), `field_d1_502`
  (cargo ship cutscene with a scripted pan).

## Event VM: scheduler and object header (main evt_expr_asm.c / event.c / evt_object.c)  (status: applied — evt_runScripts, g_evtActor, type EvtObject with its header fields; unk11 left unnamed)
Runs: `talk` = `evtrec.py talk --state field_dali_inn_moogle --seq 'w4,x:2,w120'` (Cross on the Moogle),
`f100b` = `evtrec.py f100b --field 100 --frames 1200` (Alexandria street at night: 23 objects created, three
copies of one NPC script), `f153` (field 153, camera switch), `f259` (Evil Forest, a scripted jump), plus the opcode
log of field 100 (`opscan.py 100 --log 3000`, `build/rtf2/ops_1_100.json`).
- func_80052870 → evt_runScripts (`(Unk16D8 *list)` → event result): the scheduler. Trace at `field_dali_inn`: one
  call per game frame (10 in 20 VBlanks) from evt_updateObjects+0x284. Its op jump table 0x8006E918 (read from RAM)
  handles 0x01 jump, 0x02 jump if the popped value is 0, 0x03 jump if non-zero, 0x04 return (evt_returnFromFunc),
  0x05 expression (evt_runExpr), 0x06/0x0B/0x0D switch forms, 0x1C delete object (0xFF = self), 0x22 wait N frames
  (object +0xD = N-1, max 0xFD), 0x30/0x6C func_80051878(0/1), 0x31/0x6D func_80051920(0/1), 0x32 pen_setCell;
  every other opcode goes to evt_execInstr. Before an object runs it sets g_evtCurObj, g_evtObjVars (= o + varsOfs*4)
  and D_8007B704. high.
- D_8007B704 → g_evtActor: the object the running script moves/animates/talks through. The scheduler stores the
  object itself, or for a kind-1 thread (op 0x43, uid = parent + 0x40) `evt_getObject(uid - 0x40)` (0x80052904).
  In f153 it equals g_evtCurObj on every frame (no threads); in field 56 (`evtrec.py f56 --field 56 --g
  0x8007B704:4,0x8007B70C:4`) they differ on 40 VBlanks: g_evtCurObj = 0x800782AC (thread uid 73 = 9 + 0x40, kind 1)
  while D_8007B704 = 0x800781AC (its parent); field 62 likewise with threads 77/82/84. high.
- Unk8004AB64 → proposed type name `EvtObject`; header fields (all kinds):
  - unk0 → ip. The logged opcodes are read through it; it walks the script bytes (`build/rtf2/ops_1_100.json` log:
    `2F 00 08 00 3D`, `1D 03 D9 00 7F D9 04 7F`, `36 01 D9 06 7F`, `33 00 94 00`, ...). high.
  - unk4 → level: running priority. 7 while idle; the Moogle runs its talk function at level 1 (7 → 1 on Cross), the
    player runs the turn-to-talker function at level 4, both back to 7 when it returns (`talk`). evt_callObjectFunc
    only calls into an object whose level is above the requested one. high.
  - unk5 → kind: 2 = code object (op 0x07, 0x14-byte header), 3 = region (op 0x08, 0x38), 4 = actor (op 0x09, 0xAC),
    1 = thread (op 0x43). f100b: uids 0-4 and 14 are kind 2 with 84-88-byte blocks, 16-18 kind 3 (120), the actors
    kind 4 (236-252). high.
  - unk6 → scriptIdx: equals the uid except for copies (uids 128/129/130 in field 100 are entry 10, created by the
    logged `09 0A 80`, `09 0A 81`, `09 0A 82`). high.
  - unk7 → uid. high.
  - unk8 → varsOfs: header size in words, 5 / 14 / 43 for kinds 2 / 3 / 4 (0x14/0x38/0xAC); g_evtObjVars = o +
    unk8*4. high.
  - unk9 → stackOfs: varsOfs + local-variable words (43 → 44, 46, 47 for actors with variables). high.
  - unkA → sp: call stack depth in words, 0 → 2 per call (both objects in `talk`), back to 0 on return. high.
  - unkD → wait: frames left of op 0x22 (f100b uid 3 counts 19 → 0, uid 4 29 → 0), 0xFE = waiting for its message
    window (ops 0x1F/0x95/0x54), 0xFF = waiting for a function it called with save=1. high (frame counter), code
    (FE/FF).
  - unkE → state: 2 while a new object runs its init function (the first ~130 VBlanks of every object of f100b),
    then 1; 3 = suspended. Run `ret`: at `field_burmecia_rain` an encounter was forced (g_evtEncounterChance 0x7FF,
    g_evtEncounterDist 0x3C1, one step: battle 47 from the field's table 47/47/38/79, state `field_burmecia_btl47`),
    holding L1+R1 for 600 frames ended the battle, the rewards screen (0 EXP, state `field_burmecia_rewards`) was
    closed with Cross, and back in field 763 every object had state 3 with unk12 = 1 for ~124 VBlanks while object 0
    ran function 10 at level 0 (sp 2), then all went back to 1 (evt_returnFromFunc copies unk12 back). high.
    unk12 → savedState. high.
  - unkF → flags: 0x80 = turning (0x05 → 0x85 → 0x05 for the player and the Moogle in `talk`; ops 0x50/0xBC wait for
    it to clear). Code (evt_findNearestObject): 0x02 / 0x04 collision masks against the player / other actors (two
    actors pass through each other when both have the bit), 0x08 talk mask, 0x10 not pushable (D_8007AEB4); op 0x93
    sets the low six bits. medium-high for 0x80, code for the others.
  - unk10 → windowId: 0xFF; 2 while the Moogle's message window 2 is open (`talk`), set by ops 0x1F/0x20/0x54. high.
  - unk11 → set by op 0x3B (meaning unknown).

## Field actor fields (EvtObject kind 4)  (status: applied — EvtObject members; lookParams became lookParam0/1, lookAt/balloonOfs/jump* got X/Y/Z suffixes, unk26-2E savedWalk*; the turn-left/right anims stay ActorPos unk20/rotY of prevPos)
Runs: `t_walk` = `track.py field_dali_inn 0x8007805C 0xAC 'w20,right:40,w20,right+o:40,w30'` (Zidane; this
save's Movement option is Run, so Right runs and Right+Circle walks), `t_talk`/`t_talkm` (Cross on the Moogle,
player 0x8007805C / Moogle 0x80078588), `t_idle` (idle timer poked to 3), `f100b` (NPC copies uid 128-130),
`f259` (field 259, uid 12 jumps three times). The field runs at 30 fps, so values hold for two VBlanks.
- unk34 → modelId: 98 for Zidane, 220 for the Moogle, 0xFFFF for model-less objects (op 0x2F's u16). high.
- unk3B → modelSlot: 0 for Zidane, 9, 10, 11 ... for NPCs (fld_setupActorModel: slots 0-8 belong to the nine
  characters, NPCs take 9 + GameData.unk39). high.
- unk36 → anim: 200 idle, 38 running, 25 walking, back to 200 (t_walk); 55 when the idle animation plays. high.
- unk38 → animFrames: 60 / 18 / 20 / 82 for those four anims. high.
- unk3A → animFrame (0 .. animFrames-1, wraps), unk39 → animPrevFrame (its previous value every step). high.
- unk64 → animSubFrame: the 4-bit fraction of the 12.4 frame position (4, 6, ... 14, 0 idle at speed 18). high.
- unk5D → animSpeed: 16 while moving, 18 idle (Zidane); NPC uid 128 switches among 28/32/36/40 at each idle loop.
  high. unk7C → idleAnimSpeeds[4]: uid 128 = {28,32,36,40}, Zidane {14,16,18,20} (op 0x86 `86 00 0E 10 12 14`). high.
- unk69 → defaultAnimSpeed (op 0x3E, copied to animSpeed on each anim change; 16 everywhere seen). medium.
- unk54 → standAnim, unk56 → walkAnim, unk58 → runAnim: Zidane (200, 25, 38), the anims shown idle / walking /
  running in t_walk (ops 0x33/0x34/0x35). high.
- unk5C → speed: 60 running, 30 walking (evt_movePlayer stores the pad speed). high.
- unk5E → runSpeed: 31; speed >= runSpeed shows runAnim, below it walkAnim (op 0xA6). high.
- unk74 → idleAnim: 55 (op 0x52). With the idle timer poked to 3 (`write 0x80079560 3 --size 2` at field_dali_inn),
  the timer reached 0, the stand anim finished its loop and anim became 55 (82 frames), unk62 0x82, timer reloaded
  to 0x2DC (t_idle). high.
- unk62 → animFlags: 0x80 = scripted anim playing (ops 0x40/0x41), 0x02 = idle anim (t_idle 0x82), 0x40 reverse,
  0x10/0x20 loop/bounce (op 0x3F), 0x04 hold last frame. high for 0x80/0x02, code for the rest. unk63 → animLoops
  (op 0x3F / 0x98 count, 0xFF = forever). medium. unk60/unk61 → animStart/animEnd (op 0x3D). medium.
- unk40 → prevPos: the position one game frame earlier (+0x40 follows +0x14 two VBlanks later in t_walk). high.
- unk52 → turnTarget: 0x6BB for both actors when they face each other in `talk` (= their final rotY). high.
- unk5A → turnPrevAnim: the anim to restore after a turn (200 Zidane, 0xB58 the Moogle's stand anim). high.
- unk80 → turnStartDir (0xDA0 = the Moogle's rotY before turning), unk82 → turnStep (141 per frame),
  unk6C → turnRate (0x7FFF = turn without a turn animation). high / high / medium.
- unk73 → turnSpeed (op 0x99, default 16). medium (code). unk5F → moveTurnRate (op 0x55; per-frame turn step x8
  while walking to a point). medium (code).
- unk40.unk20 / unk40.rotY (+0x4C/+0x4E) double as turnLeftAnim / turnRightAnim (ops 0x7A/0x7B; 0x87B for uid 128).
  medium (code + values).
- unk93 → talkTarget: uid of the object the player last talked to (0xFF → 8, the Moogle, on Cross). high.
- unk78 → moveDistSq: squared distance to the walk target, shrinking every frame of a scripted walk and reset to
  0x7FFFFFFF at the end (f259 uid 12: high half 35, 32, ... 0, then 0x7FFF). high. unk50 bit 0x10 = getting closer,
  0x20 = facing locked (code).
- unk50 → moveFlags: bit 8 while jumping (f259: 3 → 11 → 3), bits 0-2 op 0x47, 0x40/0x80 world-map camera marks.
  medium.
- Jump (ops 0x94, 0xE2, 0xDC; f259 uid 12, three jumps): unk68 → jumpFrame (0 .. 12), unk92 → jumpFrames (12),
  unk94 / unkAA / unk96 → jumpTargetX / Y / Z ((-420, -1264, 1451), (260, -1159, 1444), (49, 16, 1086)),
  unkA4 / unkA6 / unkA8 → jumpStartX / Y / Z (the position before each jump), pos follows a parabola between them;
  unk76 → jumpAnim (10444 / 10446), unk90 / unk91 → jumpAnimStart / jumpAnimEnd (9, 25). high.
- unk3C → collisionRadius, unk3D → talkRadius (x4 world units; op 0x4B args 2/3). With the Moogle's and Zidane's
  unk3D = 0 Cross no longer starts the dialog (level stays 7, control stays 1); with the Moogle's unk3C = 0x40 Zidane
  can't move at all next to it (distance stays 203 in every direction; `build/rtf2/coll.py`). high.
- unk98 → collidedObject (= the Moogle while pushing into it), unk9C → collisionDepth (negative, -899 normal,
  -100571 with the big radius). high / medium.
- unk32 → height (-(op 0x2F's u8 << 2); -148 for uid 128), unk2F → scale (64 = 1.0; op 0x9F's 2nd arg); both place
  the HERE icon / bubble anchor (evt_setDrawPosAtObject). medium.
- unk2D → collisionEllipse (op 0xDF; stretches the radius along the facing). medium (code).
- unk3E → balloonWindow (window anchored on the actor, evt_followBalloon; 0xFF none). medium (code).
- unk84/86/88 → lookAt (smoothed look-at point), unk6A/unk6B → lookParams (op 0x8B). medium (code; changed while
  the Moogle and Zidane face each other in t_talk).
- unk8A/8C/8E → balloonOfs (copied from D_8007AE5C..64, msg_getLayout's 0xF9 0x5F offsets). medium (code).
- unkA0 → animSyncSource (op 0xB5: copy another actor's anim and frame). medium (code).
- unk26 / unk28 / unk2A / unk2E → saved walkmesh radius/4, triangle, flags and floor (evt_updateObjects saves them
  when the field exits to a battle, evt_createModels restores them). medium (code).
- unk30 → pitch (rotation x while flying to a point, op 0xA2). medium (code).
- unk6E → talkTurnScript: 0xA7 = turn toward the talker before the talk function runs (op 0x91; func_80047AB8 uses
  it as a 2-byte script `A7 <dir>`), 4 = don't. medium (code).

## Event state block (Unk8007AECC at g_evtStateData 0x80077E60) and event globals  (status: applied — type EvtState (its node type EvtAllocNode), every field, globals and functions below, GameData npcCount/npcModelSlots/locationName/cardOpponent)
- Unk8007AECC → proposed type name `EvtState`.
- unk0[0x50] → fieldVars: variable bank 1 (evt_getVarBank 1 = *g_evtState). Bytes 0x00-0x27 change while field
  100's and the Moogle's scripts run (f100b: +0x00..+0x06, +0x12/13, +0x20..+0x26; talk: +0x10, +0x13, +0x27);
  evt_init clears them. high.
- unk50 → objPool (0x1400 bytes; every object of f100b lies in 0x80077EB0-0x80078AD0), unk1450[32] → allocNodes,
  unk15D0 → allocUsed, unk15D4 → allocFree, unk15D8[32] → listNodes, unk16D8 → objects (the list evtan.py walks),
  unk16DC → objectsTail, unk16E0 → listFree. high (structure; names medium).
- unk16E4[4] → partyUids: [19,22,20,21] at Dali (g_evtScriptCount 28; uid - 19 = character 0 Zidane, 3 Steiner,
  1 Vivi, 2 Dagger, the Dali party), [18,21,19,20] in field 100 (27 scripts). high.
- unk16E8 → analogDirOfs, unk16EA → padDirOfs: added to the stick / d-pad direction by evt_movePlayer. 0 in the
  Dali/Alexandria fields, (0x400, 0x1E0) in field 404 and (0x300, 0x3E0) in field 501 (op 0x67 arguments << 4).
  With padDirOfs poked to 0x400 at `field_dali_inn`, Right walked Zidane toward the camera (z falling, rotY 0)
  instead of to the right (rotY 0xC00). high.
- unk16EC controlEnabled / unk16ED playerUid: already named (field/menu campaign); op 0x2C sets playerUid, 0x2D/0x2E
  clear/set controlEnabled (talk: 1 → 0 → 1).
- unk16EE → mode (1 at Dali and in field 100) and unk16F4 → locationId (351 at Dali, 100 in field 100): the mode and
  field the block belongs to; evt_init restores g_evtSavedState when both match. high.
- unk16F0 → scriptBase: 0x8010D938 at Dali (event data; call frames store ip - scriptBase). high.
- unk16EF → encounterRate (op 0x57; added to g_evtEncounterChance per roll): 0x40 in Burmecia field 763, whose
  g_evtEncounterTable is {47, 47, 38, 79, pattern 1}; forcing the chance gave battle 47 (run `ret`). high.
- unk16F8 → walkOnly: poked to 1 at `field_dali_inn`, Right played the walk anim 25 instead of running (op 0x6A sets,
  0xF0 clears). high.
- unk16FC → battleObjectsTail (evt_init in battle; func_8004AEBC returns it to btl_init). medium (code).
- unk1700 → idleTimer: counts down once per frame while the player has control (talk: 191 → 187, then frozen while
  the dialog has control), reloaded with (rand + 200) * 2 or * 4; at 0 the player's idleAnim plays (t_idle).
  -1 = disabled (op 0x90), 0 = off (op 0xEE). high.

Globals (main):
- D_8007AEC4 → g_evtStepCount: +2 per game frame while running, +1 walking (t_walk 0x18E70 → ...); sysvar 7. high.
- D_8007AE20 → g_evtTalkAngle: angle from the player's facing to the object talked to (-188 for the Moogle in
  t_talk); sysvar 4. high.
- D_8007AEC0 → g_evtCamera, D_8007AEC8 → g_evtCameraChanged: in field 153 the script's fldbg_setCamera(1) turned
  AEC0 0 → 1 and AEC8 was 1 for 34 VBlanks, then 0 (f153). The bubble, HERE icon and balloons skip frames with
  AEC8 set. high.
- D_8007BBC8 → g_evtMsgCount: message windows opened since control was taken (0 → 1 → 2 in the Moogle talk; msg_open
  increments, op 0x2D clears). D_8007B710 → g_evtPlayerAnimCount: player anim changes in the same period (10 → 0 at
  op 0x2D, increments in field 153's cutscene every frame). msg_startHereIcon(1) at op 0x2E shows the HERE icon only
  when the Config HERE Icon option is On and (g_evtMsgCount >= 3 or g_evtPlayerAnimCount != 0): Moogle talk with
  HERE icon On and nothing poked: no icon (count 1); with 0x8007BBC8 poked to 3 or 0x8007B710 to 1 before the dialog
  closed, g_hereIconTimer went 59, 56, ... (icon shown); with the save's own Config (HERE icon Off, bit 17 set) never.
  (`build/rtf2/here2.py`). high.
- D_8007AEA8 → g_evtPlayerBlocked (set to 2 when a collision pushed the player back, counts down; suppresses the
  stand anim), D_8007AEB0 → g_evtPushFrames (frames pushed into a pushable actor; 1, 2 per contact in the Moogle
  run, at 25 it turns to -25 and collisions are skipped while negative), D_8007AEB4 → g_evtPushable (!(unkF & 0x10)
  of the nearest actor; 1 for the Moogle). medium-high (runtime counts + code).
- D_8007AEA4 → g_evtPosSetThisFrame (op 0x1D sets it, cleared each update; blocks a pending encounter that frame).
  medium (runtime: 1 for 4 VBlanks at field 153's placement ops).
- D_8007AE8C → g_evtLocationName (event data + 0x2C: the field's location name) and GameData.unk80C → locationName:
  the main menu's LOCATION box shows "Dali/Inn" from it; with unk80C poked to 0 the box printed whatever is at RAM 0
  ("Honolulu conversion code"). D_8007AE90 → g_evtLocationNameBuf (op 0xB0 copies a message there, 0xB1 restores).
  high for unk80C, medium for the two globals.
- D_8007BBD4 → g_msgReady: 0 while msg_init runs at a field load, 1 afterwards (f55 record). medium.
- D_8007BBD8 → g_msgSyncTag: set by op 0xE3 and by the text codes F9 0F xx / F9 46 while a message prints, read by
  scripts as sysvar 8. medium (code; not seen changing in the f55 dialog).
- D_80067948 → g_modeHeap: bump allocator of the running mode. fld_main sets it to g_loadAreaEnd + 0x28000, skips
  0x2000 and carves the FieldMap there: at `field_burmecia_rain` FieldMap = 0x800D1000 = 0x800A7000 + 0x2A000 and
  D_80067948 = 0x80107A20 past the loaded field data (g_allocTop stays 0x800A6528); battle (btl_main) and world map
  (wm_main, w_memoryman) use it the same way. medium-high.
- GameData.unk38 → npcCount, unk39 → npcModelSlots: at field 351 (Dali) func_8004AAF0 returned 8 (fld_load+0x258)
  = the eight kind-4 objects with scriptIdx < g_evtScriptCount - 9; unk39 = 7 (one of them has no model). high.
- GameData.unk8B2 → cardOpponent (func_80021680 stores op 0xAE's argument; ovl_0d tm_game reads it as the
  opponent). medium (code).

Functions (main):
- func_8004AAF0 → evt_countNpcActors (above; called by fld_load, wm_main and btl_main with 0). high.
- func_8004AB64 → evt_getObjectChrIndex (`(o)` → o->scriptIdx + 9 - g_evtScriptCount: 0-8 for the character
  scripts, used by fld_setupActorModel to give characters model slots 0-8). medium-high (code + slot numbers).
- func_8004B120 → evt_getPartyChrIndex (`(slot)`: partyUids[slot] → character index; msg name codes 0x18+).
  medium (code; the Dali uids map to the Dali party as above).
- func_8004B3D0 → evt_createRegion (out-of-line op 0x08: kind 3, flags 1). high (structure).
- func_8004AFF0 → evt_getObjectModelData (`(o)` → the model slot's +8 object: mesh show/hide, attach, scale,
  texture anims; 0 for model-less objects). medium (code).
- func_8004B158 → evt_callWorldCellTrigger (`(x, z, attr)` → object 0's function 0x8000|z<<8|x<<2|attr, from
  ovl_0c w_movement). medium (code).
- func_8004B1EC → evt_getWorldCamEyeObj, func_8004B1FC → evt_getWorldCamTargetObj (return D_8007AEB8/AEBC, the
  objects with world-map flags 0x40/0x80). medium (code).
- func_8004AEBC → evt_getBattleObjects (unk16FC). medium (code).
- func_8002170C → evt_updateActorWalkmesh (bgi_updateActor for field actors with a model; called per actor by
  evt_updateObjects). medium (code).
- func_80021680 → evt_setCardOpponent. medium (code).
- func_80062750 → card_setGameResult, func_8006275C → card_getGameResult (sysvar 18), func_8006237C →
  card_resetGameResult, D_800834C8 → g_cardGameResult (0 win, 1 loss/quit, 2 draw; tm_checkGameEnd). medium (code).
- func_80012AD4 → mbg_getStatus (bits: 1 starting, 2 playing, 4 24-bit, 0x10 paused from g_mbgState/Frame/Paused/
  Rgb24; fld_movie and ovl_15 wait for bit 2). medium (code).
- func_8002F514 → snd_startSe (`(id)`: sound command 0xD000 with pan 0x80, volume 0x7F) and func_8002F55C →
  snd_stopSe (command 0x5100): the EXP/Gil count loop in ovl_65 and sound 0x44A in ovl_37 start/stop in pairs.
  medium (code).
- func_8005392C → gamedata_resetEventFlags (clears GameData.unk568/56A/56C, battleResult, unk56F, rainCount, unk571).
  medium (code).

## Message windows, speech tails, bubble, HERE icon, ATE prompt  (status: applied — MsgWindow and Window members)
Run `msg359`: `evtrec.py msg359 --state field_dali_359 --seq 'x:2,w200,x:2,w200,x:2,w200' --g 0x8007B9C0:0x1B8`
(the Dali arrival cutscene: windows 1 and 2 open, three pages, Cross every 200 VBlanks).
- MsgWindow.unkA → id: 1 / 2 for g_msgWindows[1] / [2] (msg_open stores the index). high.
- MsgWindow.unk24 → typeAccum: text timing in 1/8 characters (+speed per frame, >>3 = characters; 8, 64, 72, ...
  per page). medium-high. MsgWindow.unk2A → pendingChars: 1 for two VBlanks at each page start, else 0. medium.
- MsgWindow.unk26 → waitFrames: 0xFF while the text waits for Cross (255 for ~250 VBlanks per page), my Cross makes
  it 1 → 0 and the next page starts (msg_input); other values count down text-code pauses. high.
- MsgWindow.unk1C → textWidth (76 / 132 / 110 = the window width w each page; the text is centred with it). medium.
- MsgWindow.unk1A → tailOffset (46 / 53 / 55) and MsgWindow.unk2B → tailMode (3 / 1 / 2): msg_open copies the
  layout's tail into them and passes them to win_setTail. medium (runtime values + code).
- Window (g_windows[10], 0x54 bytes) at `field_opening_dark` ("Sure is dark..." over Zidane): slot 2 = rect
  (128, 144, 94, 23), unk1A = 3 (style 3 = balloon with tail), unk1B = 2 (slot), unk1C = 3 (open), unk28 = 2 (anim
  mode 2 = tail), unk29 = 2 (tail mode), unk2A = 38 (tail x). Poking unk2A = 70 moved the tail 32 px to the right
  (`build/rtf2/tail_z.png`); poking unk29 to 0-3 changed nothing visible on an open window (it only feeds the open
  steps). Proposed: Window.unk1A → style, unk1B → slot, unk1C → state, unk28 → animMode, unk29 → tailMode,
  unk2A → tailX; the setters give unk10 openFrames, unk12 closeFrames, unk1D translucent, unk1F shadow,
  unk20/22/24 titleX/titleY/title, unk23 visible, unk30 list, unk40 drawContent, unk44 drawOverlay, unk48 onOpened,
  unk4C onClosed (code). high for 1A/1C/2A, medium for the rest.
- Player bubble (`field_burmecia_bubble`, field 757, near a "?" spot): op 0x68 calls msg_showPlayerBubble(0) every
  frame (trace: 5 calls in 10 VBlanks from evt_execInstr+0x9284) and a "?" balloon floats over Zidane. Patching the
  store in msg_showPlayerBubble to always write 1 (`write 0x8005007C 0xac43bbf4`) shows "!" instead; stubbing it
  removes the balloon (`build/rtf2/bubble_m.png`). Confirms msg_showPlayerBubble / msg_updatePlayerBubble /
  msg_drawPlayerBubble and g_bubbleVariant (0 "?", 1 "!") at runtime (they were [code]). high.
- HERE icon: Select at `field_burmecia_rain` draws the white pointing hand over Zidane (`build/rtf2/here.png`); the
  automatic one is the g_evtMsgCount / g_evtPlayerAnimCount rule above (needs Config HERE Icon On; the Dali save has
  it Off, config 0xF39A40 bit 17, which is why the field/menu campaign never saw it). Confirms msg_drawHereIcon /
  msg_startHereIcon / g_hereIconTimer. high.
- ATE prompt: g_atePromptMode (0x8007BC10) poked at `field_burmecia_rain`: 1 = "PRESS SELECT TO SEE ATE" at the bottom
  left (blinking with the sprite-string variant every 44 frames), 2 = blinking "ACTIVE TIME EVENT"
  (`build/rtf2/ate_m.png`, `ate2_m.png`). Op 0xD7 runs in the Dali fields 350-359, 450 and 505 (13 disc-1 fields).
  Confirms msg_drawAtePrompt / msg_setAtePrompt at runtime. high.
- Field 359 entered out of story order shows a script error window "Error Env Play() / Slot=0" (a debug message in
  the field's text, `build/rtf2/f359.png`): a lead for the field's environment-sound handling, not followed.

## Field rain (main msg.c tail, tu_80050D38.c, asm_80053384.c; ovl_0a tu_800B0124.c)  (status: applied — rain_*, wrain_*, g_fieldRain*, g_worldRain*, fld_enableRain, fld_drawRain, FLD_SCN_RAIN_*)
Nine Burmecia fields (750-768) run op 0xD8 in their first 900 VBlanks (`opscan.py`; rain visible in 751 and 763). At
`field_burmecia_rain` (763): `D_8007BC20` = {free list 0x800FBD7C, active list, 24, 896, DR_TPAGE x2, ..., 0}.
- Trace at the field-763 entry (`enter_field.py 763 --frames 0`, then `trace ... --frames 700`): op 0xD8 →
  func_800B3444(0, 0, 1) → func_80050BE0(0x800FBC5C) once, then func_80050CA8(0x18, 0x38); func_800B3684 runs every
  game frame from fld_main+0x41C.
- Ablation at `field_burmecia_rain`, 30 frames (`build/rtf2/abl_rain.png`): stubbing func_80050AAC, func_8005338C,
  func_80053498 or func_800B3684 removes the falling streaks; nothing else changes.
- func_800B3444 → fld_enableRain (`(0, 0, on)`: allocates the drop buffer once (FLD_SCN_OBJFX_ALLOC, at the scene's
  heapTop, 0x800FBC5C in field 763, else at D_80067948), sets/clears FLD_SCN_OBJFX_ON; evt_updateObjects calls it with
  0 when the field exits to a battle). high. FLD_SCN_OBJFX_ALLOC/ON → FLD_SCN_RAIN_ALLOC/FLD_SCN_RAIN_ON.
- func_800B3684 → fld_drawRain (every frame, calls func_80050AAC when ON). high.
- func_80050BE0 → rain_init (`(buf)`: 222 drop nodes into the free list, tpage, returns the end of its two primitive
  buffers). high.
- func_80050CA8 → rain_set (`(count, speed)`: drops per frame (max 0x82) and fall speed * 16; count 0 with a speed
  stops spawning and lets the drops fall out). high.
- func_80050AAC → rain_draw (spawns, then moves and draws the active drops with the field's view matrix). high.
- func_8005338C → rain_spawn (`(height)`: takes `count` nodes from the free list at random x/z, y = height). high.
- func_80053498 → rain_drawDrops (moves each drop, emits its line primitive, frees drops below the floor). high.
- func_8005345C → rain_stepDrop (asm helper of rain_drawDrops). medium.
- D_8007BC20 → g_fieldRain {free, active, count, speed, tpage[2], ..., stopping}. high. D_8007BC18 → g_fieldRainPrims:
  the line-primitive buffer after the 222 nodes, two halves 0x22B0 apart picked by the draw buffer (rain_draw).
  medium (code).
- World-map copies with identical code (op 0xD8 in evt_modelOpWorld): func_80050D38 → wrain_draw, func_80050E34 →
  wrain_init, func_80050EFC → wrain_set, func_80053658 → wrain_spawn, func_80053764 → wrain_drawDrops,
  func_80053728 → wrain_stepDrop, D_8007BC50 → g_worldRain, D_8007BC48 → g_worldRainPrims. medium (code; not
  run in a rainy world map).
- GameData.rainCount (already named) = op 0xD8's first argument (24 in field 763).

## Field exits, menus from scripts, jumps  (status: reference; applied — FLD_OBJ_NO_SHADOW, [runtime] on the four fldbg_* entries)
- Field → field: at `field_burmecia_rain` holding Down 150 frames: op 0x2B → evt_setNextLocation(0x2FA) →
  game_setNextScene(0x2FA); 300 frames later field 762 is loaded (fieldId 762). At `field_ice_300` Up: 0x2B(301).
- Field → world map: `build/rtf2/exits.py 1` finds 24 disc-1 fields whose scripts hold `B6 00 <9000-9012>`. At
  `field_ice_300` (field 300, Ice Cavern exit) holding Down 240 frames: op 0xB6 → evt_setNextLocation(0x2328 =
  9000) (ra evt_execInstr+0x3DE8); GameState.mode becomes 3, ovl_0c replaces ovl_0a, wm_main(0x2328) runs from
  wm_entry+0x30 and GameData.unk7DC = 9000; the world map with the mini-map comes up outside the cavern
  (`build/rtf2/world1.png`, state `field_worldmap_ice_exit`).
- Moogle save point: `field_dali_inn_moogle`, Cross, Cross ("Save, kupo?" → Save / Don't save), Cross: op 0x75 →
  menu_open(4, 0) + evt_onMenuOpen (ra evt_execInstr+0xA4F0/0xA4F8) and ovl_39 loads (the save screen; nothing was
  saved). So the script-opened save screen uses arg 0 (the forced `menu_save` state used 2).
- Scripted jump: field 259 (Evil Forest) uid 12 runs 0x94 / 0x9C / 0xE2 / 0xDC (see the jump fields above).
- Camera switch: field 153 calls fldbg_setCamera(0) at load and (1) from op 0x7E (traces in fields 61, 63, 153).
- Background layers (`build/rtf2/ablate2.py STATE SEQ FUNCS`, pixel diffs against an unstubbed run):
  fldbg_followPlayer at `field_d1_101` / `field_d1_352` while walking Left 40 frames: the background no longer
  scrolls with the player (41668 / 50249 pixels differ, `build/rtf2/ab_follow.png`). fldbg_updateOverlayAnims at
  `field_d1_456` (a windy lookout with a flag and a moon vane, 60 frames): only the flag and the vane differ (frozen
  frames; 1093 pixels, `build/rtf2/ab_456_diffs.png`). fldbg_updateOverlayScroll there: the drifting cloud layer
  stops (599 pixels along the cloud edges). fldbg_updateScroll at `field_d1_502` (cutscene with a scripted pan,
  60 frames): the whole view stays at the old scroll (22837 pixels, `build/rtf2/ab_502_scroll.png`).
  fldbg_updateFollowOverlays, bgi_updateAnims and bgi_updateFloorMoves changed nothing in any field tried (100, 101,
  300, 352, 404, 456, 502). So fldbg_followPlayer, fldbg_updateOverlayAnims, fldbg_updateOverlayScroll and
  fldbg_updateScroll can carry [runtime]. high.
- FieldObjSlot.flags (ovl_0a, game_getModel slot) 0x10 → FLD_OBJ_NO_SHADOW: at `field_dali_inn` setting it on
  Zidane's slot (0x80094110, flags 0x101 → 0x111) removed the dark shadow under his feet after 20 frames (92 pixels
  differ there, `build/rtf2/shadow_m.png`; fld_drawShadow returns early for it). Ops 0x7F / 0x80 clear / set it
  (EVT_OP_SHADOW_ON / SHADOW_OFF; 0x80 runs in 72 disc-1 fields), the jump op 0xE2 sets it and the jump end
  clears it. 0x20 (FLD_OBJ_SHADOW_FIXED, ops 0x83/0x84) moved the shadow slightly (14 pixels). high for 0x10.

## Event opcode table  (status: applied for the opcodes seen running plus the scheduler ops and 0x75/0xB6/0x83/0x84 — EVT_OP_* in ff9.h; placeholder names (DEBUG_*, NOP_*, SOUND3-5, MSG_8E, ...) and ops not seen stay code leads)
How the runtime column was made: every disc-1 field (222) and 74 disc-2 fields were entered with `opscan.py` (900
VBlanks each, no input) and every opcode fetched by evt_execInstr was counted (`build/rtf2/ops_*.json`,
`build/rtf2/optable.py 1 2` regenerates this table). 150 distinct opcodes ran. "N calls / F fields" = totals over
those entries; scheduler opcodes are handled in evt_runScripts before evt_execInstr and are not counted (their
evidence is the scheduler entry above). Object arguments: 0xFF = the current actor (g_evtActor), 0xFA = the player,
0xFB-0xFE = party slots 0-3 (func_8004B4C4 → evt_resolveObjectId; at runtime the Moogle's talk function runs `10 00 04
FA 16` = CALL level 4, the player, function 0x16, and the player's function 0x16 is `51 00 08 10`, `50` = turn to
object 8 (the Moogle) at speed 16 and wait: `build/rtf2/oplog.py field_dali_inn_moogle 'w4,x:2,w60'`).
Disc-1 examples: 0xB3
(fld_spriteCommand) 118855 calls in 109 fields, 0x59 (layer colour) 51953 in 99, 0x80 (shadow off) 26069 in 72, 0x23
(walk to x/z) 11884 in 65; 0x68 (bubble) only in 357, 405, 757 and 801; 0xD7 (ATE prompt) in the Dali fields 350-359,
450 and 505; 0xD8 (rain) in nine Burmecia fields 750-768; 0xE2 (jump) in 259, 454, 2950; 0x2A (battle) in 258; 0x48
(add item) in 261.

| op | proposed EVT_OP_ | args | effect (handler) | runtime |
|---|---|---|---|---|
| 01 | JMP | s16 ofs | scheduler: ip += ofs | scheduler op (not hooked) |
| 02 | JMP_IFNOT | s16 ofs | scheduler: pop expression result, jump when 0 | scheduler op (not hooked) |
| 03 | JMP_IF | s16 ofs | scheduler: jump when non-zero | scheduler op (not hooked) |
| 04 | RETURN |  | scheduler: evt_returnFromFunc | scheduler op (not hooked) |
| 05 | EXPR | expr | scheduler: evt_runExpr (assignments, variable writes) | scheduler op (not hooked) |
| 06 | SWITCH | n, default, {value, ofs}[n] | scheduler: switch on the popped value | scheduler op (not hooked) |
| 07 | NEW_CODE | entry, uid | evt_createObject kind 2 (0x14-byte header) | 693 calls / 255 fields; f100b uids 0-4/14: kind 2, header 5 words |
| 08 | NEW_REGION | entry, uid | evt_createObject kind 3 (0x38), flags 1 | 612 calls / 226 fields; f100b uids 16-18: kind 3, header 14 words |
| 09 | NEW_ACTOR | entry, uid | evt_createObject kind 4 (0xAC) with actor defaults | 1202 calls / 294 fields; `09 0A 80..82` makes uids 128-130 from entry 10 (field 100) |
| 0B | SWITCH_TABLE | u8 n, base, default, ofs[n] | scheduler: dense jump table indexed by value - base | scheduler op (not hooked) |
| 0D | SWITCH_TABLE16 | u16 n, base, default, ofs[n] | scheduler: the same with a 16-bit count | scheduler op (not hooked) |
| 10 | CALL | level, obj, func | evt_callObjectFunc(obj, level, func, 0) | 66 calls / 33 fields |
| 12 | CALL_WAIT_FREE | level, obj, func | retries until obj.level > level, then calls | 361 calls / 16 fields |
| 14 | CALL_SYNC | level, obj, func | calls with save=1 (caller waits, wait=0xFF) | 350 calls / 14 fields |
| 16 | CALL_CALLER | level, func | calls a function of the object that called this function (uid in the top call frame) | not seen |
| 18 | CALL_CALLER_WAIT_FREE | level, func |  | not seen |
| 1A | CALL_CALLER_SYNC | level, func |  | not seen |
| 1B | ? | flag | GameData.unk56F bit 3 | not seen |
| 1C | DELETE_OBJ | obj (0xFF self) | scheduler: evt_deleteObject | scheduler op (not hooked) |
| 1D | SET_POS_XZ | x, z | pos/prevPos = (x, -0x8000, z): placed on the floor | 1239 calls / 294 fields; every actor init |
| 1E | BG_SCROLL_LIMITS | cam, 4 x s16 | fldbg_setScrollLimits | 868 calls / 16 fields |
| 1F | MSG_WAIT | win, flags, msg | msg_open + wait (obj.wait = 0xFE) | 40 calls / 36 fields; talk: the Moogle +0x10 = 2 |
| 20 | MSG | win, flags, msg | msg_open, no wait | 90 calls / 78 fields |
| 21 | MSG_CLOSE | win | msg_close | 644 calls / 5 fields |
| 22 | WAIT | u8 frames | scheduler: obj.wait = n-1 (max 0xFD) | scheduler op (not hooked); f100b uid 3/4: +0xD counts 19..0, 29..0 |
| 23 | MOVE_XZ | x, z | evt_moveObjectTo(x, 0, z, 0), repeats until there | 15034 calls / 81 fields; f259: +0x78 dist^2 shrinks to 0 |
| 24 | MOVE_TO_OBJ | obj | evt_moveObjectTo the object | 67 calls / 1 fields |
| 25 | MOVE_RESET |  | loop count 0xFF, stops the walk state | 457 calls / 86 fields |
| 26 | SET_WALK_SPEED | u8 | actor.speed | 211 calls / 89 fields |
| 27 | BG_MASK_FILTER | u8 | bgi_setMaskFilter | 1114 calls / 293 fields |
| 28 | MOVIE | cmd, u16, u8 | fld_movieCommand | 8 calls / 3 fields |
| 29 | SET_PATH | n, {x,z}[n] | points used by 0xA4 | 610 calls / 226 fields |
| 2A | BATTLE | swirl, u16 battle | evt_setNextBattle, returns 3 | 1 calls / 1 fields |
| 2B | GOTO_FIELD | u16 field | evt_setNextLocation, returns 4 | 1 calls / 1 fields; field_burmecia_rain, walking down: 0x2B(0x2FA) -> field 762; field_ice_300 up: 0x2B(301) |
| 2C | SET_PLAYER |  | playerUid = current object | 294 calls / 288 fields; state +0x16ED = 19 (Zidane) |
| 2D | DISABLE_CONTROL |  | controlEnabled = 0, HERE icon off, msg/anim counters cleared | 2 calls / 2 fields; talk: +0x16EC 1 -> 0 |
| 2E | ENABLE_CONTROL |  | controlEnabled = 1, msg_startHereIcon(1), idle timer re-rolled | 173 calls / 173 fields; HERE icon test (here2.py) |
| 2F | SET_MODEL | u16 model, u8 height | actor.modelId/modelSlot/height | 1193 calls / 294 fields; Zidane model 98 |
| 30 | DEBUG_30 |  | scheduler: func_80051878(0) | scheduler op (not hooked) |
| 31 | DEBUG_31 |  | scheduler: func_80051920(0) | scheduler op (not hooked) |
| 32 | PEN_CELL | a, b | scheduler: pen_setCell(a, b) | scheduler op (not hooked) |
| 33 | SET_STAND_ANIM | u16 | actor.standAnim | 3253 calls / 294 fields; t_walk |
| 34 | SET_WALK_ANIM | u16 | actor.walkAnim | 1058 calls / 294 fields; t_walk |
| 35 | SET_RUN_ANIM | u16 | actor.runAnim | 1027 calls / 294 fields; t_walk |
| 36 | SET_DIR | u8 dir | rotY = dir << 4 | 6696 calls / 294 fields |
| 37 | SET_ROT_XZ | u8, u8 | rotX, rotZ | 535 calls / 10 fields |
| 38 | BTL_ENEMY_CMD | u8 | battle: btl_setEnemyCmd (death/counter/turn) | not seen |
| 39 | MESH_HIDE | obj, mesh | model unk14 &= ~bit | 51 calls / 2 fields |
| 3A | MESH_SHOW | obj, mesh | model unk14 |= bit | 45 calls / 8 fields |
| 3B | SET_OBJ_11 | u8 | current object +0x11 | not seen |
| 3C | ENCOUNTER_TABLE | u8, 4 x u16 | g_evtEncounterTable | 47 calls / 47 fields |
| 3D | SET_ANIM_RANGE | start, end | actor anim frame range | 1513 calls / 26 fields |
| 3E | SET_ANIM_SPEED | u8 | actor.defaultAnimSpeed | 15 calls / 8 fields |
| 3F | SET_ANIM_LOOP | mode, count | anim flags (loop/bounce) and loop count | 55 calls / 32 fields |
| 40 | ANIM | u16 anim | evt_execInstr_anim (play once) | 1981 calls / 78 fields |
| 41 | WAIT_ANIM |  | retries while the anim plays (flags 0x80) | 8294 calls / 66 fields; f100b 4954 retries |
| 42 | ANIM_END |  | evt_onAnimEnd | 2 calls / 2 fields |
| 43 | RUN_THREAD | func | kind-1 object uid+0x40 running a function of this object | 27 calls / 18 fields |
| 44 | WAIT_THREAD |  | retries while the uid+0x40 thread exists | 271 calls / 2 fields |
| 45 | KILL_THREAD |  | deletes it | 8 calls / 5 fields |
| 46 | NOP_46 |  | returns 0 | not seen |
| 47 | SET_MOVE_FLAGS | u8 | actor flags 0-2 | 470 calls / 151 fields |
| 48 | ADD_ITEM | u16 id, u8 n | item / key item / card | 1 calls / 1 fields |
| 49 | REMOVE_ITEM | u16 id, u8 n |  | not seen |
| 4A | BTL_SET_VAR | u8, u16 | battle | not seen |
| 4B | SET_RADIUS | walkmesh r, collision r, talk r | bgi radius, actor.collisionRadius, actor.talkRadius | 1191 calls / 294 fields; radius pokes (talk/collision tests) |
| 4C | ATTACH | obj, parent, bone | model attach | 31 calls / 17 fields |
| 4D | DETACH | obj |  | 3 calls / 3 fields |
| 4E | DEBUG_STRING | string | func_80052D0C on the inline string | not seen |
| 4F | YIELD |  | returns 6 (rest of the list runs next frame) | not seen |
| 50 | WAIT_TURN |  | retries while turning (flags 0x80) | 1832 calls / 53 fields |
| 51 | TURN_TO_OBJ | obj, speed | evt_turnObject toward obj | 34 calls / 19 fields |
| 52 | SET_IDLE_ANIM | u16 | actor.idleAnim | 301 calls / 271 fields; idle test: anim 55 |
| 53 | MSG_KEEP |  | msg_keepWindowsOnInit | not seen |
| 54 | MSG_WAIT_WIN | win | waits for window | 80 calls / 75 fields |
| 55 | SET_MOVE_TURN | u8 | actor turn rate while walking (0 = 0xFF) | 142 calls / 53 fields |
| 56 | TURN | dir, speed | evt_turnObject | 132 calls / 43 fields |
| 57 | ENCOUNTER_RATE | u8 | state.encounterRate (0 clears the chance) | 47 calls / 47 fields |
| 58 | BG_ORIGIN | 3 x s16 | fldbg_moveOrigin | not seen |
| 59 | BG_LAYER_COLOR | layer, r, g, b | fldbg_setOverlayColor | 71002 calls / 134 fields; field 100: 4900 calls (per-frame colour animation) |
| 5A | BG_LAYER_MOVE |  | fldbg_moveOverlay | 368 calls / 29 fields |
| 5B | BG_LAYER_SHOW | layer, on | fldbg_showOverlay | 1876 calls / 66 fields |
| 5C | BG_LAYER_LOOP |  | fldbg_setOverlayLoop | 1415 calls / 18 fields |
| 5D | BG_LAYER_PARALLAX |  | fldbg_setOverlayParallax | 44 calls / 14 fields |
| 5E | BG_LAYER_POS |  | fldbg_setOverlayPos | 162 calls / 1 fields |
| 5F | BG_ANIM_FRAME |  | fldbg_setAnimFrame | 95 calls / 62 fields |
| 60 | BG_ANIM_PLAY |  | fldbg_playAnim | 120 calls / 41 fields |
| 61 | BG_ANIM_SPEED |  | fldbg_setAnimSpeed | 1016 calls / 102 fields |
| 62 | CHR_ROW | chr, back | character row bit (Character +0x34 bit 9) | not seen |
| 63 | BG_ANIM_DELAY |  | fldbg_setAnimStepDelay | 2 calls / 2 fields |
| 64 | BG_ANIM_FLAGS |  | fldbg_setAnimFlags | 189 calls / 98 fields |
| 65 | BG_ANIM_RANGE |  | fldbg_playAnimRange | 226 calls / 21 fields |
| 66 | MSG_NUMBER | slot, u16 | msg_setNumber (value printed by a text code) | 69 calls / 68 fields |
| 67 | SET_DIR_OFFSETS | analog, pad | state.analogDirOfs/padDirOfs = arg << 4 | 1496 calls / 296 fields; fields 404 (0x400, 0x1E0) and 501 (0x300, 0x3E0); 0 elsewhere |
| 68 | PLAYER_BUBBLE | variant | msg_showPlayerBubble: 0 "?", 1 "!" | 1436 calls / 5 fields; field 757 calls it every frame (a0 0, "?"); variant 1 patched -> "!" |
| 69 | TIMER_SET | u16 s | timer_setSeconds | not seen |
| 6A | WALK_ONLY |  | state.walkOnly = 1 | 1 calls / 1 fields; the flag it sets, poked: walk anim only |
| 6B | CLEAR_COLOR | r, g, b | game_setClearColor | 6 calls / 6 fields |
| 6C | DEBUG_6C |  | scheduler: func_80051878(1) | scheduler op (not hooked) |
| 6D | DEBUG_6D |  | scheduler: func_80051920(1) | scheduler op (not hooked) |
| 6F | BG_SCROLL_TO | x, y, frames, mode | fldbg_scrollTo | 1046 calls / 37 fields |
| 70 | BG_SCROLL_TO_PLAYER |  | fldbg_scrollToPlayer | 2 calls / 2 fields |
| 71 | BG_FOLLOW |  | fldbg_setFollow | not seen |
| 72 | BG_FOLLOW_HEIGHT | s16 | fldbg_setFollowHeight | 2845 calls / 24 fields |
| 73 | BG_LIMITS_ON |  | fldbg_enableScrollLimits | not seen |
| 74 | BG_LIMITS_OFF |  | fldbg_disableScrollLimits | 1 calls / 1 fields |
| 75 | MENU | type, arg | menu_open + evt_onMenuOpen, returns 1 | not seen; Moogle "Save" -> menu_open(4, 0), ovl_39 loads |
| 76 | PATH_START | x, z |  | not seen |
| 77 | PATH_POINT | x, z |  | not seen |
| 78 | PATH_ADD |  |  | not seen |
| 79 | NOP_79 |  | returns 0 | not seen |
| 7A | SET_TURN_LEFT_ANIM | u16 | actor prevPos.rotX field reused as turn-left anim | 902 calls / 294 fields |
| 7B | SET_TURN_RIGHT_ANIM | u16 |  | 902 calls / 294 fields |
| 7C | MSG_CHOICE_MASK | u16, u8 | msg_setChoiceMask | not seen |
| 7D | TIMER_RUN | u8 | timer_setRunning | not seen |
| 7E | BG_CAMERA | u8 cam | fldbg_setCamera | 19 calls / 15 fields; field 153: setCamera(1) -> D_8007AEC0 0 -> 1 |
| 7F | SHADOW_ON |  | clears FieldObjSlot flag 0x10 (evt_modelOp*) | 5 calls / 4 fields |
| 80 | SHADOW_OFF |  | sets FieldObjSlot flag 0x10: no shadow | 31833 calls / 96 fields; flag poke removes Zidane's shadow |
| 81 | SHADOW_SIZE | u8, u8 | evt_modelOpField | 1 calls / 1 fields |
| 82 | SHADOW_OFS | s16, s16 |  | not seen |
| 83 | SHADOW_FIXED | u8 | flag 0x20 + placement | not seen |
| 84 | SHADOW_UNFIX |  |  | not seen |
| 85 | SHADOW_85 | u8 | placement byte 0 | not seen |
| 86 | SET_IDLE_SPEEDS | 4 x u8 | actor.idleAnimSpeeds | 930 calls / 294 fields; Zidane 0E 10 12 14; uid 128 28/32/36/40 seen in +0x5D |
| 87 | SET_DIR_OBJ | obj, dir | 0x36 on another object | 121 calls / 2 fields |
| 88 | MODEL_CMD | cmd, 3 x s16 | fld_modelCommand | 5935 calls / 274 fields |
| 89 | SOUND_POS | x, y, z, u8 | fld_getSoundPanVolume -> sysvars 22/23 | 10 calls / 6 fields |
| 8A | SOUND_POS_OBJ | obj, u8 |  | 381 calls / 4 fields |
| 8B | SET_LOOK | u8, u8 | look-at params, flags |= 3 | 1032 calls / 294 fields |
| 8C | BATTLE_EX | swirl, u8, u16 | battle with a music override | not seen |
| 8D | TIMER_ENABLE | u8 | timer_enable | not seen |
| 8E | MSG_8E |  | func_8004ED3C | 70 calls / 68 fields |
| 8F | OBJ_COLOR | obj, r, g, b | fld_setObjectColor (retries while fading) | 746 calls / 8 fields |
| 90 | IDLE_OFF |  | state.idleTimer = -1 | 6 calls / 6 fields |
| 91 | SET_TALK_TURN | u8 | actor turns toward the talker (0xA7) or not | 48 calls / 23 fields |
| 92 | BG_FOLLOW_LAYER | 7 args | fldbg_addFollowOverlay | 1 calls / 1 fields |
| 93 | SET_OBJ_FLAGS | u8 | flags 0-5 of +0xF | 5699 calls / 161 fields |
| 94 | SET_JUMP_ANIM | u16 anim, start, end | actor.jumpAnim / jumpAnimStart / jumpAnimEnd | 90 calls / 58 fields; f259: 10444, 9..25 |
| 95 | MSG_OBJ_WAIT | obj, win, flags, msg | message anchored on obj | 10 calls / 10 fields |
| 96 | MSG_OBJ | obj, win, flags, msg |  | 1 calls / 1 fields |
| 97 | RESTART_OBJ | obj | sp 0, state 2, return: the object re-runs its init function | not seen |
| 98 | SET_MOVE_COUNT | u8 | actor loop/move count | 117 calls / 8 fields |
| 99 | SET_TURN_SPEED | u8 | actor.turnSpeed | 9 calls / 5 fields |
| 9A | BGI_TRI_ACTIVE | u16 tri, on | bgi_setTriangleActive | 8933 calls / 24 fields |
| 9B | TURN_TO_POINT | x, z |  | 4 calls / 3 fields |
| 9C | ANIM_FWD |  | jump anim forward | 9 calls / 5 fields |
| 9D | ANIM_BACK |  |  | 7 calls / 4 fields |
| 9E | PLAYER_CALL_16 |  | pushes a call frame on the player | not seen |
| 9F | MODEL_SCALE | obj, x, y, z | model scale (x64), actor.scale | 463 calls / 67 fields |
| A0 | MOVE_TO_PATH_POINT |  | moves to sysvars 10/11 | not seen |
| A1 | SET_POS | x, y, z | pos/prevPos | 8773 calls / 180 fields |
| A2 | MOVE_XYZ | x, y, z | evt_moveObjectTo flags 2 (flying) | 977 calls / 5 fields |
| A3 | NOP_A3 | 4 x u8 | arguments read, nothing done | not seen |
| A4 | PATH_NEAREST |  | nearest point of the path segment to the player -> sysvars 10/11 | not seen |
| A5 | MOVE_XZ_NOTURN | x, z | evt_moveObjectTo flags 1 | 1208 calls / 6 fields |
| A6 | SET_RUN_SPEED | u8 | actor.runSpeed | not seen |
| A7 | TURN_DEFAULT | dir | evt_turnObject with turnSpeed | not seen |
| A8 | BGI_ACTOR_ACTIVE | u8 | bgi_setActorActive | 211 calls / 100 fields |
| A9 | OBJ_SCREEN_POS | obj | evt_projectToScreen -> sysvars 12/13 | 236 calls / 227 fields |
| AA | PAD_UNLOCK_MENU |  | g_padPortMask[0] &= ~0x40000 | 174 calls / 174 fields |
| AB | PAD_LOCK_MENU |  | |= 0x40000 | 1 calls / 1 fields |
| AC | DISC_CHANGE | u16 disc<<14|field | fld_requestDiscChange | not seen |
| AD | SET_POS_OBJ | obj, x, y, z |  | 120 calls / 1 fields |
| AE | CARD_GAME | u16 opponent | func_80021680 -> GameData.unk8B2, returns 7 (Tetra Master) | not seen |
| B0 | SET_LOCATION | u16 msg | location name = message text | not seen |
| B1 | RESET_LOCATION |  | location name = the field's | not seen |
| B2 | PARTY_SELECT | u8, u16 | D_8007AE70 request, menu_open(3) | not seen |
| B3 | SPRITE_CMD | 5 args | fld_spriteCommand | 121704 calls / 134 fields |
| B4 | SET_RECRUITED | u16 mask | chr_setRecruited per bit | 2 calls / 2 fields |
| B5 | SYNC_ANIM | obj | copy another actor's anim (unkA0) | not seen |
| B6 | GOTO_WORLD | u16 world | evt_setNextLocation, returns 5 | not seen; field_ice_300 walking down: 0xB6(9000) -> mode 3, ovl_0c, wm_main(0x2328) |
| B7 | WM_CAM_EYE |  |  | not seen |
| B8 | WM_CAM_TARGET |  |  | not seen |
| B9 | PAD_MASK_SET | port, u16 |  | not seen |
| BA | PAD_MASK_CLEAR | port, u16 |  | not seen |
| BB | TURN_OBJ | obj, dir, speed |  | not seen |
| BC | WAIT_TURN_OBJ | obj |  | not seen |
| BD | ANIM_OBJ | obj, anim |  | 1 calls / 1 fields |
| BE | WAIT_ANIM_OBJ | obj |  | 41 calls / 1 fields |
| BF | SET_POS_XZ_OBJ | obj, x, z |  | 1 calls / 1 fields |
| C0 | TEX_ANIM_PLAY | obj, n | model texture anim n on | 44 calls / 20 fields |
| C1 | TEX_ANIM_LOOP | obj, n |  | not seen |
| C2 | TEX_ANIM_STOP | obj, n |  | not seen |
| C3 | BG_LAYER_VIEWPORT |  | fldbg_setOverlayViewport | 869 calls / 13 fields |
| C4 | WM_PARAM |  | w_frameSetParameter | not seen |
| C5 | SOUND | u16 cmd, u16 id | GameData.unk83C(cmd, id) | 2644 calls / 286 fields |
| C6 | SOUND3 | cmd, id, u24 |  | 214 calls / 196 fields |
| C7 | SOUND4 |  |  | 34 calls / 25 fields |
| C8 | SOUND5 |  |  | 2225 calls / 210 fields |
| C9 | BG_VIEWPORT |  | fldbg_setViewport | 594 calls / 11 fields |
| CA | BG_ANIM_RESET |  | fldbg_resetAnim | 3 calls / 1 fields |
| CB | BGI_FLOOR_ACTIVE | floor, on | bgi_setFloorActive | 41 calls / 25 fields |
| CC | MODEL_FLAGS_SET | u16 |  | 4 calls / 3 fields |
| CD | MODEL_FLAGS_CLEAR | u16 |  | 2 calls / 2 fields |
| CE | ADD_GIL | u24 |  | not seen |
| CF | REMOVE_GIL | u24 |  | not seen |
| D0 | BTL_BANNER | u16 msg | btl_setBanner | not seen |
| D1 | CLEAR_GLOBALS |  | evt_clearGlobalVars | not seen |
| D2 | NOP_D2 |  | returns 0 | not seen |
| D3 | NOP_D3 |  | returns 0 | not seen |
| D4 | MODEL_OFS | 3 x s16 | model +0x74..0x78 | not seen |
| D5 | SAVE_ACTOR_FLAGS |  |  | not seen |
| D6 | RESTORE_ACTOR_FLAGS |  |  | not seen |
| D7 | ATE_PROMPT | mode | msg_setAtePrompt | 27 calls / 24 fields; Dali fields 350-359, 450, 505; mode 1 "PRESS SELECT TO SEE ATE", 2 blinking "ACTIVE TIME EVENT" (poke) |
| D8 | RAIN | count, speed | func_800B3444(0,0,1) + func_80050CA8, GameData.rainCount | 11 calls / 10 fields; Burmecia 750-768: rain (24, 56) in 763 (field_burmecia_rain) |
| D9 | CURE_STATUS | chr, bits | chr_cureStatusBit | 36 calls / 3 fields |
| DA | SPRITE_CMD2 |  | fld_spriteCommand | 2 calls / 1 fields |
| DB | CHR_BIT10 | chr, bit | Character +0x34 bit 10 | 1 calls / 1 fields |
| DC | JUMP_STEP |  | one jump frame, retries until done | 95 calls / 5 fields; f259: +0x68 0..12 |
| DD | PARTY_REMOVE | chr |  | 4 calls / 2 fields |
| DE | SET_CHR_NAME | chr, msg |  | not seen |
| DF | SET_COLL_ELLIPSE | u8 | actor +0x2D | not seen |
| E0 | INC_GD568 |  |  | not seen |
| E1 | BTL_END_REWARDS |  | rewards_requestClose | not seen |
| E2 | JUMP | x, y, z, frames | starts a jump (jumpTarget, jumpFrames) | 9 calls / 5 fields; f259 uid 12 |
| E3 | MSG_SYNC_TAG | u8 | D_8007BBD8 (sysvar 8) | 61 calls / 60 fields |
| E4 | BG_LAYER_SKEW |  |  | 197 calls / 3 fields |
| E5 | BTLSEQ_START |  |  | not seen |
| E6 | BG_LAYER_FLAG1 |  |  | 7 calls / 5 fields |
| E7 | BG_ANIM_SHOW_FRAME |  | fldbg_showAnimFrame | 1887 calls / 20 fields |
| E8 | MOVE_XYZ3 | x, y, z | evt_moveObjectTo flags 3 | 165 calls / 1 fields |
| E9 | PARTY_REFRESH |  | evt_execInstr_party | not seen |
| EA | GET_SCROLL |  | fldbg_getScroll -> sysvars 12/13 | 1058 calls / 3 fields |
| EB | MSG_CLOSE_ALL |  |  | not seen |
| EC | FADE | 6 args | game_startFade | 232 calls / 230 fields |
| ED | BG_VIEWPORT_RATE |  |  | 3 calls / 3 fields |
| EE | IDLE_ON |  | state.idleTimer = 0 | not seen |
| EF | HERE_ICON | u8 | msg_startHereIcon | 16 calls / 14 fields |
| F0 | RUN_ALLOWED |  | state.walkOnly = 0 | not seen |
| F1 | SET_HP |  |  | 18 calls / 2 fields; applied earlier |
| F2 | SET_MP |  |  | 18 calls / 2 fields; applied earlier |
| F3 | RESET_AP |  |  | not seen |
| F4 | LEARN_ABILITY |  |  | not seen |
| F5 | GAME_OVER |  |  | not seen; applied earlier |
| F6 | VIB_PLAY |  |  | not seen |
| F7 | VIB_SET_PLAYING | u8 | vib_setPlaying | 1 calls / 1 fields |
| F8 | VIB_TRACK | 3 x u8 | vib_selectTrack | 2 calls / 1 fields |
| F9 | VIB_TRACK_ENABLE | 3 x u8 | vib_setTrackEnabled | not seen |
| FA | VIB_RATE | u16 | vib_setRate | not seen |
| FB | VIB_LOOP | u8 | vib_setLoopMode | not seen |
| FC | VIB_RANGE | 2 x u8 | vib_setRange | not seen |
| FD | GD8BC_CALL | u8, u16 | GameData.unk8BC(a, b) | 13 calls / 13 fields |
| FE | PARTY_JOIN |  | chr_join | 1 calls / 1 fields |
| FF 00 | BGI_FLOOR_MOVE_START | u8, u8 | bgi_startFloorMove | 16 calls / 1 fields; field 404 (corrected by docs/r32/3ds-requests-9.md: that table lumped every FF sub-op under FF 00; field 404 uses FF 0A BGI_ANIM_SHOW_FRAME, floor moves FF 00-08 occur only in fields 1609, 1610, 1652, 1658, 1759, 2952) |
| FF 01 | BGI_FLOOR_MOVE_STOP |  | bgi_stopFloorMove | not seen |
| FF 02 | BGI_FLOOR_MOVE_PLAYING |  | bgi_setFloorMovePlaying | not seen |
| FF 03 | BGI_FLOOR_MOVE_MODE |  | bgi_setFloorMoveMode | not seen |
| FF 04 | BGI_FLOOR_MOVE_FLOOR |  | bgi_setFloorMoveFloor | not seen |
| FF 05 | BGI_FLOOR_MOVE_SPEED |  | bgi_setFloorMoveSpeed | not seen |
| FF 06 | BGI_FLOOR_MOVE_CURVE |  | bgi_setFloorMoveCurve | not seen |
| FF 07 | BGI_FLOOR_MOVE_RANGE |  | bgi_setFloorMoveRange | not seen |
| FF 08 | BGI_FLOOR_MOVE_AXIS |  | bgi_setFloorMoveAxis | not seen |
| FF 09 | BGI_ANIM_FRAME |  | bgi_setAnimFrame | not seen |
| FF 0A | BGI_ANIM_SHOW_FRAME |  | bgi_showAnimFrame | not seen |
| FF 0B | BGI_ANIM_PLAYING |  | bgi_setAnimPlaying | not seen |
| FF 0C | BGI_ANIM_MODE |  | bgi_setAnimMode | not seen |
| FF 0D | BGI_ANIM_RATE |  | bgi_setAnimRate | not seen |
| FF 0E | BGI_ANIM_VALUES |  | bgi_setAnimFrameValues | not seen |
| FF 0F | BGI_ANIM_VALUE |  | bgi_setAnimFrameValue | not seen |
| FF 10 | BGI_ANIM_RANGE |  | bgi_playAnimRange | not seen |
| FF 11 | BGI_ANIM_RESET |  | bgi_resetAnim | not seen |

## What the field campaign did not reach  (status: reference)
- Minigames: jump rope, Hippaul's race and Chocobo Hot & Cold run as field scripts in specific story states; entering
  their fields with the Lv 50/99 saves' flags plays other variants, so their opcodes (timer 0x69/0x7D/0x8D, pad
  masks 0xB9/0xBA) were seen only as counts. Tetra Master from a field (op 0xAE → mode 9, card_*GameResult) was not
  started; the NPC challenge (Square, function 8) needs a card-playing NPC in its story state.
- Not seen in any field entry: 0x16/0x18/0x1A (call the caller; the Moogle uses 0x10 with object 0xFA instead),
  0xA4/0x76-0x78 path ops, 0x9E, 0xAC (disc change), 0xB2 (party select), 0xDE (rename), 0xF3/0xF4, the FF01-FF11
  walkmesh ops and the vibration ops other than F7/F8. 0x2B and 0xB6 only ran when the player walked out (0x2B also
  once in disc-2 field 1400's cutscene).
- World-map rain (wrain_*), world-map camera marks (ops 0xB7/0xB8, func_8004B1EC/1FC) and the world-map cell
  trigger (func_8004B158) were not run; `field_worldmap_ice_exit` is a starting point for them.
- fldbg_updateFollowOverlays, bgi_updateAnims and bgi_updateFloorMoves showed no visible change when stubbed in the
  fields tried; the walkmesh animations need a field where a floor visibly moves. tu_800B61A4's remaining unnamed
  functions (800C0D38, 800C1CC0, 800C1D08, 800C1DAC/B4/BC) are empty or return 1; ovl_0a 800B58A8/B0 and 800B5928/30
  are the empty update/leave hooks of the disc-change save and disc modes (g_fieldDiscChangeModes[2]/[3]);
  800C7450/7458 return/store 1; 800C77DC/800C7914 return (count, pointer) pairs D_800C9DDE/D_800CAA98 and
  D_800C9DE0/D_800CAAB8 of the walkmesh; 800C0FF4 stores arg² (or 3600) into D_800CA0E0; 8019AB98 is called by
  fld_movie before mbg_play with (2, 4, movie volume) and lives in the FMV overlay's space.
- g_msgSyncTag (D_8007BBD8) never changed in the dialog recorded; the F9 0F / F9 46 text codes that set it were not
  found in a running message.


## R30 runtime pass (2026-10-30): rows of docs/wip/r30_runtime_{main,ovl_0b,overlays}.md  (status: **applied**)
Method: Redux disc 3 (`disc3_malboro_lv99_calm` with the two calm pokes, `ds_zidane_trance_entry`, encounters entered with
`tools/rt/enter_battle.py`) and disc 1 (field/menu/world-map states). Action runs use `build/rt3/run3.py` (hang-safe
breakpoints, `--patch CMD,ABILITY[,TARGETS]` rewrites the first Attack); effect-op arguments were logged by a small Lua hook
(scratch `r30.lua`, reads the op's argument words through the stack pointer, `g_btlfxState.effectId` 0x800FEC42 and
`g_btlfxOpArgs` 0x80100D88). Encounter flags were read from the encounter files (FF9.IMG dir 5, header block 16 +4, the
same u16 `btl_initSceneFlags` reads; checked at runtime against `encounter->flags` / `Battle.scene.flags` for encounters
899 and 878). New Redux states (disc 3): `r30_veteran_cmd` (encounter 878 Veteran, Zidane's command menu),
`r30_malboro_dagger_cmd`, `r30_malboro_eiko_cmd` (Malboro calm state, Dagger's / Eiko's menu up after Triangle presses).

### ovl_0b

- **func_800D7768 (unit op 0x5E) + func_800D8EA4 → btlfx_opPlayAttackMotion / btlfx_stepPlayAttackMotion; func_800D77E4
  (0x5F) + func_800D8F1C → btlfx_opPlayAttackMotionHold / btlfx_stepPlayAttackMotionHold; func_800D7870 (0x61) +
  func_800D8FAC → btlfx_opPlayAttackReturnMotion / btlfx_stepPlayAttackReturnMotion. CONFIRMED (structure), motion slots
  named from screenshots.** Zidane Attack (effect 0x64, `disc3_malboro_lv99_calm`): op 0x71 face target (args unit 0x10,
  target 0), 0x7F, then 0x5E (unit 0x10 = caster) steps from t+101 to t+141 frames after the menu input, the frames in
  which Zidane runs in and slashes (impact particles from btlfx_opDrawOnUnit at t+121, damage 3233). 0x61 (caster, target 0
  = the Malboro) steps t+153..t+169, exactly the back-flip home after the hit (screenshots every 8 frames). Steal (effect
  0xC8) runs 0x71 and 0x61 too. 0x5F was seen once, in effect 0xDF (Zidane's command patched to 8/0xBD) with
  `g_btlfxOpArgs` 03 00 bc 02, so trimFrames 3. So motion slot 0x17 = the attack motion (run in and strike), 0x19 = the
  jump back after attacking an enemy (0x61 skips it for party targets). Names [runtime], medium.
- **func_800D8520 (unit op 0x7F) + func_800DA064 → btlfx_opSyncOpposed / btlfx_stepSyncOpposed. CONFIRMED (no visible
  effect).** In effect 0x64 it ran with (0x10, 0) caster vs the Malboro: the task lived exactly one step (one mode-2 call,
  same frame as the following op), as the code says (done at once on the same side, one frame later otherwise). Nothing
  drawn. [runtime] low-medium (a one-frame sync when caster and target are on opposite sides).
- **func_800E4530 → btlfx_getCommonSprite. REFUTED (not per-effect data).** The table at 0x801E9700 is 0xFF-filled at rest
  and gets filled ~35 frames after the Attack input by `StoreImage` called from btlfx_runScript+0xE0/+0x184 (watch on
  0x801E9700 hit libgpu func_80014AA8 = the StoreImage DMA, destination 0x801E7700). The contents (offset table e0 294 348
  3fc ...) occur on disc 3 only in dir 1 file 0x2BF (sector 9) and file 0x2C0 (sector 0), i.e. the common effect file that
  btlfx_reloadCommonData puts into VRAM. So it's the common effect data read back from VRAM at effect start, and
  func_800E4530(n) returns entry n of its self-relative table (btlfx_opDrawOnUnit called it with 0x12, 0x16, 0x0F during
  Attack). medium.
- **func_800E83EC (export 0x34C, sets g_evtGlobalVars.b[0xC7] 0x10). REFUTED "summon seen", NOT REACHED.** Effect ids
  0x1B3/0x1A2/0x1EC are enemy abilities in story battles, from the encounter files (ability record effect field):
  encounter 21 Sealion/Black Waltz ability 2 "Tsunami" (0x1B3), 337 Blank ability 1 "Moonlight Slash" (0x1A2),
  891/937 Kuja/Trance Kuja ability 5 "Ultima" (0x1EC). No ability (party, item or enemy) uses 0x180 or 0x18A, and none of
  the five is a summon. Not run (no state reaches those bosses). Leave unnamed until a field script reading var 0xC7 is
  found.
- **D_800F78AC → g_btlfxSoundPaused. code-proven, runtime NOT REACHED.** btlfx_copyDisplayToDraw only runs from the
  hand-written btlfx_runFrame when `env->...->retry` is set (the script waits on the loader, `btlfx_retryOp`); it then
  pauses sound group 2 (akao_pause(2)) once and btlfx_runFrame clears the flag and calls akao_resume(2) on the next frame
  that isn't frozen. No freeze happened in Shiva (short and with firstUseFxSeen cleared), Ark, Madeen, Doomsday runs
  (akao_pause 0 calls, no write to 0x800F78AC): Redux's CD never stalled the loader. [code] (sound paused while the effect
  waits for its data).
- **btl_rollMagicFail (applied): CONFIRMED.** Battle ability table (Battle+0x16CC, names from D_800F909C): 0x2E is
  "Meteor", MP 42 (neighbours 0x2D Water 22, 0x2F Flare 40, 0x30 Doomsday 72).
- **btl_randomizeDefense (applied): CONFIRMED, the bit is support ability 0xDA "Gamble Defense".** Support bit n of
  support[0] = ability 0xC0+n (Character.supportAbilities, g_supportAbilities name offsets into ovl_65's
  s_rewardsSupportNames): 0x04000000 = 0xDA "Gamble Defense" (1 stone), 0x02000000 = 0xD9 "Add Status" (matches
  btl_calcCmd's weapon-status use). Runtime: with the Malboro's support word (g_btlEnemySupport 0x800F5244) given bit
  0x04000000, Zidane's Attack takes the rand path in btl_randomizeDefense (a0 replaced by the rand pointer at +0xDC),
  without it the function returns untouched. No party member of the Lv 99 save has it equipped.
- **btl_applyInvertedPower (applied): CONFIRMED, scene flag 0x400 = encounter flag 0x100, set only for Ipsen's Castle
  encounters.** Encounters with flag 0x100 (all discs): 246/253 Cerberus, 871 Taharka, 872-874 Tonberry, 878-880 Veteran,
  881-883 Gargoyle+Agares. Runtime: `enter_battle.py 878` (Veteran): encounter flags 0x100, scene.flags 0x4C2; Zidane's
  Attack hit btl_applyInvertedPower with BtlCalc.power 71, which became 1 (60 - 71 clamped), state `r30_veteran_cmd`.
- **g_btlfxCmdCameras (applied): CONFIRMED with details.** btlfx_playCmdCamera never runs for Attack, Steal, Focus,
  Jump or Items. Arguments (camera code, high nibble = category): Dagger summons Shiva/Ark 0x3018 and Eiko's Madeen 0x3318
  (category 1, so entry [2] for character 2 Dagger, [3] for Eiko), Zidane's Doomsday 0x3328 (category 2, scene data 4,
  enemy target), trance Zidane's Free Energy 0x3300 (category 0, in Trance, enemy target, entry [1]), the Malboro's
  ability (enemy camera). Category 0xF0 (trance entry 0x30F8, Focus 0x33F0) plays nothing. [2]/[3] are the summon cameras
  of Dagger / the other summoner.
- **Unk800D805C.unk3 (op 0x71 third op argument) → snapToFront. CONFIRMED.** Effect 0xDF runs 0x71 with
  `g_btlfxOpArgs` 0c 00 01 00 (frames 12, turnAway 0, unk3 1). Against the Malboro it stepped 12 frames (14 hits);
  with Vivi as target (patch targets 2) it finished after one step (3 hits) and set rotY 0x800 instead of turning.
  Unk800D7C9C.unk2 / Unk800D955C.unk2 (ops 0x6C/0x6D): Focus (effect 0xC7) runs both with arguments (target 0, unk2 0);
  no run had unk2 = 1, INCONCLUSIVE.
- **g_btluiState.unk5 / unk9 / unk14 / unk20 (0x80104E6D/71/7C/88): CONFIRMED unused.** Read and write watchpoints over 700
  frames of menu, target selection and an Attack: no access to any of them (the hits at 0x80104E7D are the next field,
  btlui_readPad). Leave unnamed.

### main

- **GameData.unk810 → debugLocationName. CONFIRMED.** Points at a Shift-JIS full-width developer name: Dali inn
  "Ｄａｌｉ／Ｉｎｎｌａｇｅ　Ｒｏａｄｅｌｌ" (field data + 4), world map "ｗｏｒｌｄ　Ｃｅｎｔｒａｌｔｅｍ　Ｓｈ..." (right before the
  area name). No reads while opening the menu in the Dali inn or in the save screen (`menu_save`). [runtime] high.
- **GameData.unk571 → darkBattle. CONFIRMED.** World map: teleporting the player to the weather-2 zone (pos 0x50B00,
  -0x3A000 from `field_worldmap_ice_exit`) sets g_worldWeatherMode 2 (dark sky), the next random encounter wrote
  unk571 = 1 on leaving the world map, the battle background was a night sky and characters[3] (Steiner) kind became 8.
  Forcing encounter 47 from `field_burmecia_rain` with unk571 0 or 1: kind 7 vs 8, and the only pixels that differ in the
  whole battle frame are Steiner's (bbox 265,67-283,101, darker armour). So bit 15 of EVT_OP_BATTLE's id / the world-map
  weather selects Steiner's dark-scene model variant. [runtime] medium (name the field after the battle condition).
- **GameData.unk9, unk836, unk849, unk8CC[8]: CONFIRMED unused.** Read watchpoints in a battle Attack, field walking
  (`field_d1_101`) and the menu (`menu_main`): no reads (the one hit near unk849 was +0x84C kanjiFont, text_measure).
- **g_menu.unk1C / func_80032A34, g_menu.unk3 / unk4: CONFIRMED unused.** Menu open/close from `field_dali_inn`, a shop
  (`menu_shop_weapons` Buy then exit) and the save screen: func_80032A34 0 calls, unk1C written once by menu_doOpen,
  unk3/unk4 never written. (0x80077DB5 is written by menu_doOpen and menu_finish.)
- **D_80071E30 → g_mbgIsBackground. CONFIRMED.** g_mbgMovieLists in RAM (`intro_test`): unk5 = 1 for every `.MBG` entry
  (MBG101-MBG119, rgb24 0) and 0 for every `.STR` FMV. So mbg_getStatus bit 8 = the playing movie is a field movie
  background. [runtime] high.
- **D_8007AF00 → g_evtTranceEnabled, D_8007AF01 → g_evtDaggerFalters, D_8007AF02 → g_evtSummonMpX4, D_8007AFB1 →
  g_evtBattleNoHud, D_8007AFD3 → g_evtNoActionAbilities. CONFIRMED.** b[0x10] = 1 in the disc 3 saves; its code uses are
  the trance gauge (btl_initPartyUnit trance, ovl_38 Trance bar). b[0x11]: the unit kind tested is flags & 0x1F == 5, which
  at runtime is Dagger (character id 2) in the Malboro battle, and btl_initPartyUnit also blocks trance for character 2
  (the 1/4 failure didn't roll in 8 tries, the rand state is the save state's). b[0x12] = 1: Dagger's Summon list shows
  Ramuh 88 MP instead of 22 (abilities with unkB bit 4 = Dagger's eidolons Shiva..Ark, not Eiko's). b[0xC1] = 1 before
  entering encounter 899: the battle opens with no HUD and no ATB. b[0xE3] = 1 before opening Ability (`menu_main`): the
  Use tab is greyed. [runtime] high (names describe the effect, not the story reason).
- **D_8007B9A0, D_8007B9A4 (msg_beginDraw): CONFIRMED write-only.** In battle msg_beginDraw never runs (Attack, Steal);
  in the field it runs every frame from fld_beginFrame and no code reads 0x8007B9A0 or 0x8007B9A4 (60 frames with a speech
  balloon). The battle values (1, both pointers = D_8007B9B0) are left from the battle's mode switch. Leave unnamed.
- **D_8007B994 / D_8007B998, D_8007B94C: CONFIRMED unused.** With a message on screen and Cross presses: D_8007B994 is only
  read by msg_update (never written, 0), D_8007B998 only written by msg_update, D_8007B94C (0xB2) never read.
- **D_8007F8DC (D_8007F8D8[1]): CONFIRMED never written after init.** Field → world map transition: akao_startSeq ran once
  and read 0x8007F8DC (value 0), no writes to 0x8007F8D8-0x8007F8DF. So the 0x20 flag goes to sequences whose
  instrumentSet is 0. D_8007F8E0, D_80080A20, D_800809EC, D_8007AE04: no reads (title, field → world map).
  D_8007954E (EvtState+0x16EE) is only read by the whole-struct copy `g_evtSavedState = g_evtStateData` in evt_init.
- **ModelSlot.unk4 → modelKey. CONFIRMED.** `field_d1_101`: chrModels[1] 0x20008, npcModels[0..4] 0x20020, 0x21574,
  0x2003F; the bound actor's modelId (+0x34) is 8, 0x20, 0x1574, 0x3F. So unk4 = 0x20000 | modelId (the model's db key,
  type 2), -1 for empty slots. [runtime] high.
- **EvtObject.unkC: CONFIRMED unused.** 16 in every actor of `field_d1_101`; read watchpoints on three actors over 400
  frames of walking and talking: none.
- **D_8006A774 → alias of g_usableItemTable. CONFIRMED.** btlui_selectItem indexes it with the raw item id (Potion 0xEC,
  Hi-Potion 0xED as subNo of btlui_startTargetSelect), so D_8006A774 + 0xE0 * 12 = 0x8006B1F4 = g_usableItemTable. Use
  `g_usableItemTable[id - ITEM_FIRST_USABLE]` there. **D_8006CB88 → g_abilityRows[1] (Vivi's row). CONFIRMED.** It is
  0x8006CB28 + 0x60, entries (ability, AP): Fire 25, Fira 50, Firaga 75, Sleep 20, Blizzard 25 ... and Vivi's info.row is 1.
- **D_8006E658 (op 0x9E script): NOT REACHED** (op 0x9E ran in none of the 222 fields of the opcode survey either).

### overlays

- **ovl_14 func_800AB1E4 → opening_setWorldCamKeepNew, func_800AB200 → opening_setWorldCamKeepLoad, GameData.unk890 →
  worldCamKeep. CONFIRMED (load path).** Title → Continue → load: func_800AB200 called once from opening_resetGameData,
  writes 2 to GameData+0x890. On the world map (`field_worldmap_ice_exit`) it is already 0 (wm_findVehicles clears it).
  New Game path not run. [runtime] medium.
- **ovl_14 func_800A73B8: CONFIRMED uncalled** (title, Continue, load: 0 calls).
- **ovl_0c func_800C45D8: INCONCLUSIVE.** Walking out of the Ice Cavern it ran once from wm_init with story counter
  0xB18 and b[0xBE] 0, returning 0 (music fades in). No state with the story counter in 0x2580-0x26B5 was tried.
- **ovl_0c func_800B8548 → wm_isDarkWeather. CONFIRMED.** See GameData.unk571 above: weather mode 2 (the zone around
  0x50B/-0x3A0, `wm_getWeatherByPos`) darkens the world map, and the flag it returns turns into the dark battle.
- **ovl_0c D_800CE3D8, D_800CE270: NOT REACHED** (no actor of type 1, 2 or 11 near the Ice Cavern exit, no writes in
  380 frames).
- **ovl_0c D_800D4960 / D_800D49F0: CONFIRMED unread** (380 frames walking, menu open/close).
- **ovl_0c D_800CCE49, D_800CCE4A, D_800CCE55: CONFIRMED never written.** **D_800D32FC: REFUTED** - it is
  g_worldAmbientSeDist[1] (0x800D32F8 + 4), written by wm_updateMap every 3 frames (value 718 at the Ice Cavern exit), so
  wm_findGetoffPos's test is "within 0x16 of ambient sound source 1". Replace the D_ name with the array element.
- **ovl_0c D_800D30C4, D_800CE284, D_800CE1E1, D_800CF47E, D_800CE3F8: CONFIRMED unread** (the hits at 0x800D30C5/C6 are
  the next variables). **D_800CF478 → g_worldAreaNames (world data pack entry 0): READ by main evt_onMenuOpen+0x2C when
  the menu opens on the world map**, i.e. it is the table the world map's LOCATION name comes from. [runtime] medium.
- **ovl_34 D_801F14F8 → s_menuConfigPadArrow. CONFIRMED.** Controller → Custom page: the glyph between the NEW and NORMAL
  button icons is a left arrow.
- **Unk8006D328 (g_armorStatBonus) unk0..unk3 → defense, evade, magicDef, magicEvade. CONFIRMED (game data).** Wrist
  0 5 0 3, Glass Armlet 0 10 0 7, Silver Gloves 0 10 0 5, Feather Hat 0 0 7 0, Magus Hat 0 0 10 0, Iron Helm 0 0 7 0,
  Silk Shirt 7 0 0 0, Bronze Vest 9 0 1 0, Linen Cuirass 10 0 0 0 (names from g_itemTable/g_itemNames in RAM): armlets
  give evade and magic evade, hats magic defense, body armour defense. The Lv 99 save's status values are all capped at 99,
  so the screen check wasn't possible. medium-high.
- **FieldScene.unk5F, FieldPalAnim.unk4: NOT REACHED.** No fld_soundCommand kind 2 sub 12 call and no
  fldbg_startClutFade call in `field_burmecia_rain`, `field_d1_456`, `field_alex_100`, `field_opening_dark` (300-400
  frames each).
- **BattleResult.unkB → hasPostEvent. CONFIRMED.** Scene flag bit 13 = encounter flag 0x800, set for story battles
  (Beatrix 4/73/299, Baku 295, Black Waltz 296, Prison Cage 302, Masked Man 336, Blank 337, King Leo 338, Zorn and Thorn,
  Garland, Kuja, Necron, the Mu/Fang/Trick Sparrow tutorial-area bundles 12-35, ...). Runtime: g_btlRewards.unkB
  (0x800FDF3B) = 1 in `ds_port_first_battle` (encounter flags 0x835), 0 in the random Ironite battle 47 and in the
  fixture battle (flags 0x21). [runtime] high.
- **FieldBgState.unk74 / unk78 / unk9C / unkA0-A3: CONFIRMED unread** (300 frames of scrolling in `field_d1_101`).
  FieldMap.unk14/18, title-menu and Tetra Master fields not run.

## R30b runtime pass (2026-10-30): rows left open by R30  (status: **applied**)
Method: Redux disc 1, states generated instead of played to (recipes in `docs/emulator.md` "Generating states").
Encounters entered with `tools/rt/enter_battle.py ENC --disc 1 --field 351 --base-state field_dali_inn --frames 900`
(new states `r30b_enc21_start`, `r30b_enc337_start`), world map with `build/rtf2/wm_enter.py` (`r30b_wm_story2600`,
`r30b_wm_moogle`). Runs use `build/rt3/run3.py` (outputs `build/rt3/r30b_*.json` and screenshots). Static scans:
`build/rtf2/encscan.py`, `fxscan.py`, `varscan.py` (scratch, described in emulator.md).

### ovl_0b

- **func_800E83EC → btlfx_signalScriptCue [runtime] (the effect tells the battle AI script that its cue point played,
  the AI waits for b[0xC7] bit 0x10 and then fades out and ends the battle). CONFIRMED.**
  Correction to R30: the btlseq effect ops in the encounter files (`encscan.py`) give 0x1A2 in encounter 337 (disc 1,
  the Prima Vista Steiner fight), 0x1EC and 0x18A in 891 (disc 3, Kuja), 0x180 in 937 and 0x1B3 in 938 (disc 4).
  Encounter 21's "Tsunami" (Sealion ability 2, forced through a `btl_setEnemyCmd` hook) plays effect 0x198, which never
  calls the export. Readers (`varscan.py byte:0xC7`): b[0xC7] is a per-battle flag byte of the battle AI scripts
  (FF9.IMG dir 5). Bit 0x10 is used only by encounter 337 (entry 1) and 891 (entry 2): their init function (tag 0)
  clears it (`05 d4c7 7defff 3d 7f`, b[0xC7] &= 0xFFEF), and their turn function (tag 1) runs BTLSEQ_START (op 0xE5),
  then `while (!(b[0xC7] & 0x10)) wait 1`, then FADE (op 0xEC, 30 frames), waits and sets battle var 0x21 = 5.
  Other encounters use other bits of the byte (336, 920/921, 84, 114/115, 57/211, 191). Runtime, `r30b_enc337_start`:
  forcing the ability (`--fn 'btl_setEnemyCmd=... r.a3=1'`) played effect 0x1A2 and the export ran once at the effect's
  last phase frame 0x4B (~1200 frames after btlfx_start, from 0x801E96B0 in fx_1a2_op80), but nothing read the byte
  because the AI was not in that branch. With the AI's own branch (`--pre` enemy 0x10 HP = 500, `run3.py r30b_e337f`):
  the AI started effect 0x1A2 itself (btlfx_start from the btlseq, no btl_setEnemyCmd), evt_readVar read 0x8007AFB7
  (u8, var type 0x28) every 4 frames (268 reads) until the export set the bit at f23856, then the screen faded to
  black and the game returned to the field (ovl_0a resident). high.
  Suggested header constant: `EVT_GB_BATTLE_AI_FLAGS 0xC7` (per-battle flags of the battle AI scripts, bit 0x10 = effect
  cue, set by btlfx_signalScriptCue).
- **Unk800D7C9C.unk2 / Unk800D955C.unk2 (effect ops 0x6C/0x6D, btlfx_opJumpForward / btlfx_opJumpHome) → skipIfParty
  [runtime] (with 1 the caster stays in place when the target is a party unit). CONFIRMED.** `fxscan.py 6c,6d`: unk2 is
  1 in 25 effect files (Steiner's Sword Arts 0x5C/0x2E/0xD0/0xD1, Freya's 0xA8/0x128/0x53/0x1EB/0x183/0x1EA, 0xFE/0xFF,
  0x116-0x124, ...) and 0 in Focus 0xC7 (common script too) and 0x3D. Runtime, `r30b_enc337_start`, Steiner's Attack
  patched into Sword Art Armor Break (cmd 0x1E, ability 0x91, effect 0x5C): against the enemy (targets 0x10) op 0x6C
  stepped 9 times (Steiner leaps to the enemy) and 0x6D 8 times, against Zidane (targets 1) both ops returned in
  mode 1 with no step (Steiner hit Zidane from his own place, screenshots `build/rt3/r30b_jf_0x10_*`, `r30b_jf_0x1_*`).
  high.
- **D_800F78AC g_btlfxSoundPaused (applied): CONFIRMED at runtime.** Forced stall: Dagger's Shiva (`--patch
  0x10,0x31,0x10`, short form effect 0x197) with `g_btlfxLoader.summonDataState` (0x80100E12) held at 1 for 90 effect
  frames once it reached 2 (Lua on btlfx_runScript entry), so control op 06 01 retried. btlfx_copyDisplayToDraw ran 72
  times (every stalled frame, f27000-f27284), akao_pause(2) once at f27000 with g_btlfxSoundPaused written 1 by
  btlfx_copyDisplayToDraw+0xC0, and btlfx_runFrame+0xD8 wrote 0 and called akao_resume(2) at f27288, the first frame
  after the stall (`build/rt3/r30b_stall.json`). high.

### ovl_0c

- **func_800C45D8 → wm_keepEntryMusic [runtime] (returns 1 when wm_init must not fade the music that is playing on
  entry). CONFIRMED.** `wm_enter.py` pokes at the dispatcher break: story 0x0B18 / b[0xBE] 0 returns 0 and wm_init sends
  SND_MUSIC_VOL_INTPL (0, 0x78, 0) at once, a 120-frame fade to volume 0 (wm_musicPlay's own call is (0, 0x3C, v), a fade
  in to v). Story 0x2600 (inside 0x2580-0x26A1), 0x26A5 (inside 0x26A2-0x26B5), b[0xBE] (= g_worldMoveMode) 8 and 9 all
  return 1, and no fade-out is sent: the next sound commands are song 0x5F (wm_soundCommand(0, 0x5F)) and its fade in
  (0, 0x3C, 0x7D). Correction to R30: 0 means the carried-over music fades out, not in. high.
- **D_800CE3D8 → g_worldWalkerActor [runtime] (the party leader on foot, actorType 1 or 2). CONFIRMED (type 1).**
  World scripts (`op 0x3B` literal actorType then `0x2F` model, all discs): type 1 = model 310 in dir-3 world files 0, 3,
  5, 7-12, type 2 = model 309 only in world file 2, type 11 = model 307 in most of them (file 0 is the one loaded at
  the Ice Cavern exit, GameData+0x800 = its sector). At the Ice Cavern exit
  (`field_worldmap_ice_exit`, `build/rtf2/actors.lua`) uid 11 is type 1, model 310, the Zidane sprite the pad moves, and
  D_800CE3D8 = g_worldPlayer = that actor (0x800782C8). Type 2 not seen: world file 2 loads with `wm_enter.py --world 9002`
  but its init script stalls with the Lv 50 save. Write-only in ovl_0c (wm_initMovement clears it, wm_findVehicles sets
  it). medium-high.
- **D_800CE270 → g_worldMoogleActor [runtime] (the world-map moogle, actorType 11). CONFIRMED.** uid 6 (type 11, model
  307) exists hidden (flags 0x0E) at the Ice Cavern exit and D_800CE270 stays 0. Pressing Square calls the moogle: it
  flies in (flags 0x07) and wm_updateActors+0x110 writes its address (0x800780BC) every frame from then on (state
  `r30b_wm_moogle`). Forcing flags bit 0 and placing it next to Zidane shows a moogle model. Write-only in ovl_0c. high.

### ovl_0a / ovl_14

- **FieldScene.unk5F: NOT REACHABLE, write-only.** Its only writers are fld_soundCommand category 2 kind 0 (cleared when
  a sound starts), kind 2 sub 12 (= arg2) and fld_main's init. No code reads it, no C caller passes a sub-12 command,
  and no field or world script (all discs, ops 0xC5-0xC8 with literal arguments) sends one: the category-2 kind-2
  commands on disc are only sub 1 (0x6201, sound volume) and sub 10 (0x620A, movie sound volume). Leave unnamed.
- **FieldPalAnim.unk4: NOT REACHABLE.** fldbg_startClutFade, its only writer, has no caller in any binary, and the
  background update loop never handles entry type 0x80 (fldbg_update / fldbg_scanEntries test only 0x10 and 0x40). It
  is 1 for mode 2 (add with a flag) and 0 for add/subtract. Leave unnamed (dead code).
- **FieldBgState.unkA0 is g_fieldBgClutRows (0x800CA108). CONFIRMED.** Watching it across a field entry
  (`enter_field.py 101`, `run3.py r30b_bg_entry --noload`): fldbg_init clears unkA0-A3, then fldbg_buildOverlaySprites
  (+0x28 / +0x264) reads and updates 0x800CA108 eight times each, the symbol g_fieldBgClutRows. Use one name (the struct
  field `clutRows`, or keep the separate symbol and leave the field padding). unkA1-A3, unk74, unk9C and FieldMap
  unk14/unk18 are only stored by fldbg_init / fld_initMap: 600 frames each of `field_d1_502` (scripted pan),
  `field_opening_dark` and `field_d1_404` (cutscenes) gave no access at all. Leave unnamed.
- **TitleMenu.unk0 / unk10 (g_titleMenu 0x800AB320 / +0x10): CONFIRMED unused** from `title_screen` through Down, Up
  and Continue into the memory-card screen (ovl_39 loaded): no access. SceneMenu (ovl_15) and TmState (ovl_0d) not run
  (no state reaches the ending, blackjack or a card game).

## Sound ids  (status: applied — SE_* in include/snd_ids.h)
- binary: main (snd_playSe 0x8002F488) with ovl_0a, ovl_33, ovl_37, ovl_0b callers.
- method: `tools/rt/act.py NAME STATE 'inputs' --nocov --trace 8002F488` (log breakpoint on snd_playSe, a0 = SE id).
- `menu_main` (disc 1): Down → 0x67 (menuMain_inputCommand); Cross on Ability → 0x67 (menuMain_selectCommand);
  Circle out of the member selection → 0x65 (menuMain_inputMember); Select → 0x2AA (help_toggle+0x50, help opens);
  Select again → 0x65 (help_toggle+0x90, help closes); Circle on the command list → 0x65 (menuMain_inputCommand).
  So 0x65 is the cancel/back sound. The older "Pad globals and snd_playSe" entry (0x65 "when Cross opens the
  character selection") was a misreading: Cross plays 0x67.
- `menu_items`: Cross on Potion → 0x67 (menuItem_openTarget); Cross on a full-HP Zidane → 0x66
  (menuItem_useOnTarget); Circle → 0x65; Down x5 → 0x67 each (menuList_down); Cross on a greyed Leather Hat → 0x66
  (menuItem_selectItem). 0x66 is the refusal buzzer.
- `menu_equip`: Cross on the tab and slot → 0x67; Cross on a new weapon → 0x6B (menuEquip_equipSelected);
  R1 → 0x417 (menuEquip_inputSlot, next member).
- `disc3_malboro_lv99_calm` (Malboro ATB/HP pokes): Right / Left → 0x417 (btlui_slideRight/Left); Select → 0x2AA,
  again → 0x65 (btlui_toggleHelp); Cross Attack → 0x67 (btlui_confirmCmd), Cross on the target → 0x67
  (btlui_confirmTarget), 4 frames later the next member's menu → 0x414 (btlui_openCmdMenu+0x124).
- names: SE_CANCEL 0x65, SE_BUZZER 0x66, SE_CURSOR 0x67, SE_SET 0x6B, SE_HELP_OPEN 0x2AA, SE_WINDOW_OPEN 0x414,
  SE_PAGE 0x417. Code-only (no runtime check): SE_USE_ITEM 0x6A, SE_MESSAGE 0x416, SE_COMPLETE 0x4ED,
  SE_GAME_START 0xC18, SE_CHOCOBO_CALL 0x44A, SE_ENCOUNTER_1-3, SE_ENEMY_DEATH_1/2, SE_FLEE_1-3.
- confidence: high for the runtime ones.

## Battle flags  (status: applied — BTL_UNIT_*, BTL_POPUP_* etc. in include/ovl_0b.h; the rest of those names are [code])
- binary: ovl_0b; BtlUnit.flags at unit+0x68, koState +0x7D, popupFlags +0x7E; units[0] at 0x80108EFC (stride 0xD0).
- method: `tools/emu.py start --disc 2 --state battle_iifa_command` (Vivi's command menu up), `read` the flags words,
  `press cross` (Attack → target select), `press cross` (confirm), reads every 10-20 frames.
- party at rest: Zidane 0x51010101, Vivi 0xD101E023, Eiko 0xD101EC4D, Amarant 0xD101E16F; enemy (units[4]) 0x11013280.
  Bit 0 set on the party only; bits 1-4 = character id (0 Zidane, 1 Vivi, 6 Eiko, 7 Amarant) = BTL_UNIT_INDEX;
  bits 5-7 = 0,1,2,3 for the party and 4 for the enemy = BTL_UNIT_SLOT; bits 8-9 = 1 for Zidane/Amarant,
  0 for Vivi/Eiko, 2 for the enemy = BTL_UNIT_ROW; bits 16 (ATB_ACTIVE), 24 (SHADOW), 28 (TARGETABLE) on every unit;
  bit 30 (TRANCE_ENABLED) on the party only. Bits 10-15 and 31 are set on some units but no C code reads or writes
  them (left unnamed).
- Cross on Attack: the enemy gets 0x20000000 (BTL_UNIT_TARGETED, cursor on it); confirming clears it. About 80 frames
  later the enemy has 0x20000 (BTL_UNIT_HIT_MOTION) and popupFlags 1 (BTL_POPUP_HP, damage number), then 0x80000
  (BTL_UNIT_CMD_WAITING: its own command queued); all three are clear again 40-60 frames later. confidence: high.

## Constants runtime pass (2026-10-31): rows of docs/wip/constants_runtime_{main,overlays}.md  (status: **applied**)
Applied: CFG_SHIFT_BTL_MSG_SPEED, AKAO_MODE_STEREO/MONO, AKAO_LEGATO/_TIED/_SE, TEXT_PAL_*, DIGITS_*, UI_LABEL_*,
UI_SPRITE_<button>, TEXT_BTN_* (plus TEXT_BTN_START 0x38 / TEXT_BTN_SELECT 0x39, which text_buildCode maps to the
START / SELECT sprites) and MODEL_SLOT_LOADED/NO_SHADOW in ff9.h; FLD_MAP_ACTIVE (named instead of FLD_MAP_NORMAL)
and FLD_OBJ_NO_IDLE_TEXANIM in ovl_0a.h; WM_ACTOR_NONE/BOAT/AIRSHIP/AIRSHIP2, WmActorType.rideMode (was unk4) and
WM_MOVE_CHOCOBO_FIELD/REEF/MOUNTAIN/OCEAN/SKY, WM_MOVE_BOAT/HILDA_GARDE/INVINCIBLE in ovl_0c.h (renamed from
WM_MOVE_CHOCOBO/STEER/AIRSHIP/AIRSHIP2). Left literal: GameState.mode 5/6, GameData.flags 0x40, FieldEvtCtx.flags
0x800, FieldActorExt.flags 0x02000000, BgiFloorMove/BgiAnim 0x2, BgEntry.type 0x40.
Follow-up: ModelSlot (ff9.h) is the same record as ovl_0a's FieldEvtCtx / FieldObjSlot, ovl_0c's WmObjSlot and ovl_0b's
BtlObj / Unk8E0Ld; the types (and FLD_OBJ_* / WM_SLOT_* / MODEL_SLOT_*) are not merged yet.
Redux, LuaJIT off. Value polling used a per-VBlank Lua listener (`GPU::Vsync`, records value changes of
GameState.mode 0x8008B008, FieldMap.flags, GameData.flags 0x8008B250); read PCs came from a Lua `Read` breakpoint
collecting distinct PCs, mapped through `mips-linux-gnu-objdump -d build/us/*.elf`. GameState is 0x8008B000,
GameData 0x8008B250, FieldMap (`ctx.field`) 0x800D1000 in every disc-1 field state.
New states: disc 3 `cpass_eiko_trance_menu` (Malboro fight, Eiko in Trance, her command menu up, so the TRANCE
title is drawn), `cpass_wm_hadris` (card 1 FILE03 loaded: world map at Hadris Basin next to Ipsen's Castle, gold
chocobo uid 5 and the type-9 airship uid 6 on the map), `cpass_wm_airship9` (just boarded the type-9 airship, move
mode 8).

### main
- **GameState.mode 5 / 6: NOT REACHED (no writer).** Polled the mode byte through `field_burmecia_btl47` (battle, 2) →
  victory (8) → field (1), `field_ice_300` walked out to the world (3, written by wm code at 0x800A9134),
  `title_screen`/`intro_test` (0), `field_d1_502` (FMV field, 1), `r30b_wm_moogle` (3), `menu_main` (1). 5 and 6 never
  appear. The only writers of GameState.mode in C store 0,1,2,3,4,7,8,9 or a saved/return mode (ovl_39 restore,
  ovl_65 `s->unk6`, wm_main's exit kind). Proposal: keep them unnamed, or `GS_MODE_UNUSED5` / `GS_MODE_UNUSED6` with
  the existing comment (field-like / battle-like leftovers). medium.
- **GameState.config bits 11-13: CONFIRMED battle message speed, but REFUTED "a Config row".** `menu_config`
  (disc 1): config 0x00019AC0; Right on Sound → 0x19AC1; Left on Field Message → 0x15AC0 (bits 14-16, 6 → 5). The
  NTSC-U Config screen has no Battle Message row (rows: Sound, Controller, Cursor, ATB, Battle Camera, Battle Control,
  Movement, Battle Speed, Field Message, HERE Icon, Window Color, Target Window, Vibration; screenshots), and ovl_34
  writes only CFG_SHIFT_BTL_SPEED and CFG_SHIFT_MSG_SPEED. Bits 11-13 are only set by config_reset (3) and read by
  btl_initBanner (g_btlBannerDurations). Proposed `CFG_SHIFT_BTL_MSG_SPEED 11` (3 bits, fixed at 3, no Config row).
  high.
- **AkaoFlags.mode 1 / 2: CONFIRMED.** `menu_config`: g_akaoFlags (0x80083158) words bankUpload 1, mode 1, dirty 0; Right on the Sound
  row (Mono) → mode 2; Left (Stereo) → 1. Proposed `AKAO_MODE_STEREO 1`, `AKAO_MODE_MONO 2`. high.
- **Text palettes 0-9: CONFIRMED (colours).** g_textPalettes = 3827 38A7 3927 39A7 3A27 3AA7 3B27 3BA7 3F26 3FA6.
  Method: `menu_main`, write g_textPalettes[0] := palette n, 4 frames, screenshot (every palette-0 text and the
  menu digits redraw in palette n). Colours (fill / outline): 0 white with dark outline, 1 white without the dark
  outline, 2 grey (148,148,148; the "not available" grey of `ok ? 0 : 2` callers), 3 red, 4 yellow, 5 cyan,
  6 purple, 7 green, 8 pale steel blue (131,148,172; help text: help_draw and the key-item description use it),
  9 orange-red (172,82,57) with a lavender outline (the help-window red). Proposed `TEXT_PAL_WHITE 0`,
  `TEXT_PAL_PLAIN 1` (no outline), `TEXT_PAL_GREY 2`, `TEXT_PAL_RED 3`, `TEXT_PAL_YELLOW 4`, `TEXT_PAL_CYAN 5`,
  `TEXT_PAL_PURPLE 6`, `TEXT_PAL_GREEN 7`, `TEXT_PAL_HELP 8`, `TEXT_PAL_HELP_RED 9`. high for the colours, medium
  for the HELP pair's role.
- **ui_drawDigits / text_buildDigits styles: CONFIRMED.** `r30b_enc337_start` (party HP/MP panel), write
  g_numberClutRows[0] (0x8006BBB4) := 0x40 / 0x80 / 0xC0: the style-0 digits turn red / yellow / green. Table
  0,0,0,0x40,0x80,0,0,0xC0, so styles 0, 1, 5, 6 are white, 3 red, 4 yellow, 7 green, and 2 is white drawn dim and
  semi-transparent (code). Callers: 3 = KO (hp 0), 4 = low HP/MP (BTL_STATUS_LOW_HP, mp < max/6; ui_drawStones uses 4
  for 0 stones). Proposed `DIGITS_WHITE 0`, `DIGITS_DIM 2`, `DIGITS_RED 3` (KO), `DIGITS_YELLOW 4` (low),
  `DIGITS_GREEN 7`. high.
- **ui_drawLabel mode 1: CONFIRMED (gradient mask).** Only caller: btlui_drawTranceLabel, the command-window title
  of a unit in Trance. `cpass_eiko_trance_menu`: the "TRANCE" title letters show the pulsing red-to-yellow gradient
  (screenshots 3 frames apart change colour), i.e. mode 1 draws the glyphs black/semi-transparent with the CLUT 0x40
  rows down inside the mask-bit packets so only the gradient shows through the letters. Proposed `UI_LABEL_NORMAL 0`,
  `UI_LABEL_MASK 1`, `UI_LABEL_DIM 2`. high (look), medium (mechanism wording).
- **Button glyphs, text codes 0x30-0x37 / ui_drawSprite ids: CONFIRMED.** `menu_main`, Lua exec breakpoint on
  ui_drawSprite rewriting a3 (sprite id) of the stone/clock icons to 4..19, screenshots: 4 Cross, 5 Circle, 6 Square,
  7 Triangle, 8 L1, 9 L2, 10 R1, 11 R2, 12 d-pad, 13-16 small arrows, 17 START, 18 SELECT. g_buttonSprites = 4,5,7,6.
  So text codes 0x30 Circle, 0x31 Cross, 0x32 Triangle, 0x33 Square, 0x34 L1, 0x35 R1, 0x36 L2, 0x37 R2. Proposed
  `UI_SPRITE_CROSS 4`, `_CIRCLE 5`, `_SQUARE 6`, `_TRIANGLE 7`, `_L1 8`, `_L2 9`, `_R1 10`, `_R2 11`, `_DPAD 12`,
  `_START 17`, `_SELECT 18`, and `TEXT_BTN_CIRCLE 0x30`, `TEXT_BTN_CROSS 0x31`, `TEXT_BTN_TRIANGLE 0x32`,
  `TEXT_BTN_SQUARE 0x33`, `TEXT_BTN_L1 0x34`, `TEXT_BTN_R1 0x35`, `TEXT_BTN_L2 0x36`, `TEXT_BTN_R2 0x37`. high.
- **ModelSlot.flags 1 / 0x10: CONFIRMED, and ModelSlot is the same record as ovl_0a's FieldEvtCtx.** Slots
  (chrModels 0x80094110, stride 0x14): every slot with a model has bit 1 (field 0x101, battle 1, world 1), empty ones
  0 with modelKey -1. A read breakpoint on chrModels[1].flags (0x80094124, Zidane) in `field_d1_101` is hit by
  fld_drawModels (andi 0x2, 0x1000), fld_drawShadow (andi 0x10), fld_calcShadowMatrix (0x20), fld_playAnimSfx (0x40),
  fld_applyLights (0x4, 0x100, 0x400): the FLD_OBJ_* bits. So 1 = FLD_OBJ_LOADED and 0x10 = FLD_OBJ_NO_SHADOW (the
  jump op hides the shadow for the jump). Proposed: one shared set (move FLD_OBJ_* to ff9.h as `MODEL_SLOT_*`, or use
  FLD_OBJ_LOADED / FLD_OBJ_NO_SHADOW in event.c/btl/wm), and make ModelSlot and FieldEvtCtx one type. high.
- **GameData.flags 0x40: INCONCLUSIVE (write-only).** Set by fld_startEncounter (gameFlags queue 0x43; polled:
  GameData.flags 0x3F000000 → 0x3F000043 → 0x3F00007F → 0x3F00005F during the encounter swirl, cleared 0x35000000
  before the battle) and by a 24-bit field movie (0x44); cleared by fld_movieFinish / the swirl end. A read
  breakpoint on 0x8008B250 over 1000 frames of `field_burmecia_rain` (walk, encounter, swirl, battle start) found
  no instruction testing 0x40 (all readers test 0x1/0x2/0x4/0x8/0x10/0x1C/0x20/0x100); forcing the bit in
  `field_d1_101` changed nothing visible (walking, menu). Proposed `GD_FIELD_SUSPENDED 0x40` (set while the field
  scene is replaced by the encounter swirl or a full-screen movie), low; leaving it a literal is also fine.
- **AkaoChannel.legato 4: CONFIRMED SE-only.** `r30b_enc337_start`, log breakpoints on akao_opD0 / akao_opCC /
  akao_opDA over 10 Cross presses (Steiner attacks): opD0 2 hits, a0 = 0x8007F248 / 0x8007F37C = g_akaoSeChannels[12]
  and [13] (0x8007E3D8 + n*0x134); opCC 7 hits on SE channels; opDA 0. With the code (1 set by CC/DA, 2 = previous
  note tied, copied from bit 1 after each note, `& 5` suppresses the 2-tick gate cut): proposed `AKAO_LEGATO 1`,
  `AKAO_LEGATO_TIED 2`, `AKAO_LEGATO_SE 4`. high.

### overlays
- **ovl_0a FieldMap.flags 0x4: REFUTED "loading"; it is "normal field running / no encounter transition".** It is
  set (0x4) in every disc-1 field state checked (10 states, incl. ones with control and random encounters).
  Polling FieldMap.flags in `field_burmecia_rain` while walking until a random encounter: 4 → 0x191D (fld_startEncounter's
  set-queue 0x1919) → 0x1919 one frame later (its clear-queue `mapFlags[1][3] = 4`) → 0 at the exit → mode 2. So the
  bit is cleared exactly when the encounter swirl starts, and fld_main then runs fld_updateEncounter (the swirl/battle
  preload, not the random-encounter roll), fld_endFrame draws with DrawOTag instead of DrawOTagEnv (no clear: the
  swirl reuses the last frame) and the sound loader keeps 0x3000 bytes back (the swirl buffer). Proposed
  `FLD_MAP_NORMAL 0x4` (or `FLD_MAP_NO_ENCOUNTER_FX`): set by fld_load, cleared to start the encounter transition.
  high.
- **ovl_0a FieldEvtCtx.flags 0x1000: CONFIRMED "no idle texture animation restart".** `field_d1_101`, exec breakpoint
  at fld_drawModels 0x800AB984 (the `|= TEXANIM_ACTIVE` of texture animation 2), 600 frames: NPC models 0x80129AA4 /
  BD4 / C6C / D04 restarted 300 / 124 / 199 / 255 times. With 0x1000 or'ed into the slot of model 0x80129AA4
  (npcModels[0] flags 0x101 → 0x1101): that model 0 times, the others unchanged. Proposed `FLD_OBJ_NO_IDLE_TEXANIM
  0x1000` (texture animation 2, probably the blink; not verified visually). high (mechanism).
- **ovl_0a FieldEvtCtx.flags 0x800: NOT REACHED.** fld_setSlotColor was called 0 times in 600 frames each of
  `field_d1_502`, `field_d1_404`, `field_d1_501`, `field_dali_359`, `field_opening_dark`, `field_alex_100`. From the
  code: its only caller is EVT_OP_OBJ_COLOR, which re-executes while the function returns 1; the first call sets the
  colour and 0xC00, the second clears 0x800 and returns 0, so the op always takes two frames. Proposed
  `FLD_OBJ_COLOR_WAIT 0x800` (one-frame wait latch of EVT_OP_OBJ_COLOR), medium (code).
- **ovl_0a FieldActorExt.flags 0x02000000: NOT REACHED** (no mirror scene in the available states). Leave unnamed.
- **ovl_0a BgiFloorMove.flags 0x2: CONFIRMED write-only.** g_fieldBgiFloorMoves[0].flags reads 0x0002 in `field_d1_404`
  and `field_d1_456`; a read breakpoint over 300 frames each is hit only by bgi_updateFloorMoves+0x40 (0x800C7140),
  which tests 0x1 and 0xC. 0x2 is the reset value and is never tested (BgiAnim's 2 likewise has no reader in C/asm).
  Proposed: leave a literal, or `BGI_ANIM_RESET 0x2` with "never tested". medium.
- **ovl_0a BgEntry.type 0x40: NOT REACHED** (as in R30b).
- **ovl_0c actorType 0: CONFIRMED "not a world actor".** `r30b_wm_story2600` (disc 1) and `cpass_wm_hadris` (disc 3),
  `tools/rt/actors.lua`: every kind-2 (code-only) script object has actorType 0; every kind-4 actor has 1, 7, 9, 11,
  12, 13 or 14. Proposed `WM_ACTOR_NONE 0`. high.
- **ovl_0c actorType 9: CONFIRMED airship (Hilda Garde 3 on disc 3); 8 boat and 10 second airship by data.**
  `cpass_wm_hadris`: uid 6 type 9 model 320 is g_worldVehicleActor (0x800781D8). Teleported Zidane next to it,
  Cross: g_worldMoveMode = b[0xBE] = 8 and the ship takes off (y -2070 and rising; `cpass_wm_airship9`). The disc-3
  save (17:04, before Pandemonium) only has the Hilda Garde 3, so 9 = Hilda Garde 3. g_worldActorTypeInfo
  (0x800CCCC0): type 8 flying 0 (with the code: floorY forced 0, no shadow, so a sea vehicle), types 9 and 10
  flying 1; their unk4 bytes are 7, 8, 9 = the move modes of WM_MOVE_STEER / AIRSHIP / AIRSHIP2 (and types 3-7 have
  unk4 1-5 = the chocobo modes), so unk4 looks like the actor type's ride mode. Boarding is decided by the world
  script, not by actorType (poking uid 6 to type 10 still gave mode 8; to type 8 no boarding). Proposed
  `WM_ACTOR_BOAT 8` (Blue Narciss, medium), `WM_ACTOR_AIRSHIP 9` (Hilda Garde 3, high), `WM_ACTOR_AIRSHIP2 10`
  (Invincible, medium: disc 4 not reached, a forced Treno → world exit stalled in wm init).
- **ovl_0c g_worldMoveMode 1-5: CONFIRMED = chocobo ability count.** `cpass_wm_hadris`, moved the gold chocobo
  (uid 5, type 7) and Zidane away from the castle entrance, Cross to ride: mode 5 with b[0xBF] (chocobo ability count)
  = 5; with b[0xBF] poked to 1, 2, 3, 4 before riding: modes 1, 2, 3, 4 (actorType stays 7). The move mode table's
  allowed-cell masks grow with the mode: 1 0x0010667F (= walking), 2 0x20F8667F, 3 0x20FE667F, 4 and 5 0x23FF667F,
  6 flying. Proposed `WM_MOVE_CHOCOBO_FIELD 1`, `WM_MOVE_CHOCOBO_REEF 2`, `WM_MOVE_CHOCOBO_MOUNTAIN 3`,
  `WM_MOVE_CHOCOBO_OCEAN 4`, `WM_MOVE_CHOCOBO_SKY 5` (gold chocobo on the ground; 6 stays WM_MOVE_CHOCOBO_FLY). high.
- **ovl_0c g_worldMoveMode 7: NOT REACHED** (no Blue Narciss state). Data: WM_CTRL_STEER, speed 0xF0, loop SE 24,
  ride mode of actor type 8. Proposed `WM_MOVE_BOAT 7`, medium (data).
- **ovl_0c g_worldMoveMode 8 / 9: 8 CONFIRMED (Hilda Garde 3), 9 inferred.** Mode 8 observed above. Modes 8 and 9
  share the flight parameters and differ in loop SE (23 / 22) and get-off distance (0x280 / 0x2D0). Proposed
  `WM_MOVE_HILDA_GARDE 8` (or keep WM_MOVE_AIRSHIP), `WM_MOVE_INVINCIBLE 9` (medium).

## Round 31 runtime pass (2026-10-31): rows of docs/wip/round31_runtime_{main_overlays,ovl_0b,fxmod}.md  (status: **applied**)
Applied: the sweep is `tools/effect_labels.py` (`effect_modules.py catalogue` fills the effect column from it; KNOWN
0xB2 = Twister); BTL_ENEMY_HUMANOID .. AERIAL (FLYING renamed after Scan's text) and SA_*_KILLER / SA_MAN_EATER;
Unk800D8124.partyMode + BTLFX_FACE_*; BTLFX_HDR_RESIDENT comment; btl_loadEncounterFile / btl_initEncounter take
Battle11C0 * (Battle11C0.unk10 = script; Unk800AA08C stays as btl_loadBackgroundStep's step state);
VRAM_BTLFX_SCRATCH comment; BTLFX_SIG_MOTION/MOVE/TURN/WAIT, the modules' Unk800F78A40.signals[8] (ovl_0b's own
Unk800FEC40 keeps signals.b[4] + the four named done flags, which op 1 reaches as b[4-7]); BTL_MOTION_HIT /
BTL_MOTION_HIT_HEAVY, fxm_startHitMotionIfActive / fxm_startHeavyHitMotionIfActive; fx_0d2 / fx_0b3 comments;
CharBits.victoryPose (CHR_INFO_VICTORY_POSE), GameData.inWorldFrame, AKAO_PAUSE_FORCE_TICK, CHR_CLASS_MALE; module
identity header comments. Left unnamed: AkaoStreamState.unk20/unk30, tailMode 0x20, FieldSpriteSet.unk0/unk4,
classFlags 0x4 / 0x8.
Redux, LuaJIT off, the hang-safe runner `build/rt3/run3.py` copied to `build/r31rt/run3.py` (outputs there).
`--patch CMD,AB,TGT` rewrites Zidane's first Attack. Disc 3 states `disc3_malboro_lv99_calm` (calm pokes) and
`disc3_malboro_entry_1100`, disc 1 field states. VRAM was read with `PCSX.GPU.getVRAM()` (a 1 MB Slice, byte-indexable;
`build/r31rt/vram.lua` `DumpVram(path)` writes it to a file in ~1 s, `vram2png.py` / `sheet.py` crop and tile it).
No new save states.

### Effect identity sweep (docs/effect-modules.md effect column filled, 379 of 380 modules)
- **Effect id = dir-13 file id: CONFIRMED, with two remaps in btlfx_resolveFirstUse.** `btlfx_start(id)` stores
  `btlfx_resolveFirstUse(id)` and phase 1 calls `btlfx_loadFile(effectId)`. Runtime: Fire on the Malboro
  (`--patch 0x16,0x19,0x10`) gives btlfx_start(0) but btlfx_loadFile(0x46), because g_btlfxLargeTargetFx
  (0x800F727C) swaps the one-target id for the all-targets one when unit 0's scaled radius is 0x385 or more. Its pairs
  0x2A>0x7F, 0x19>0x7C, 0x60>0x61, 0x9E>0x9D, 0>0x46, 2>0x47, 7>0x48, 0x1A>0x4E, 6>0x3E, 4>0x4D, 0x97>0x98, 3>0x42,
  5>0x49, 0x18>0x85, 0x15>0xCA, 0x5D>0x78 explain the old runtime list (Sleep 0x4E, Silence 0x7F, ... on the Malboro).
  g_btlfxFirstUsePairs gives short>full summon ids (0x197>0x26 Shiva ... 0x1BF>0x17D Ark). Malboro AI Osmose
  (`disc3_malboro_entry_1100`, natural) btlfx_start/loadFile 0x39, Bad Breath (btl_setEnemyCmd a3 forced to 0) 0x135,
  both as the static sweep predicts.
- **Static sweep** (`build/r31rt/label.py`, `encfx.py`, `sweep.py`): ovl_33 s_menuAbilityActionTable is the battle
  ability table layout (BtlCmd8, effect bits 8-16, altEffect +0xC; it matches every runtime id above). Target type 3
  abilities play `effect` for several targets and `altEffect` for one (btl_startCmdEffect), others play altEffect for
  short summons and the Seiken commands. Items from g_usableItemTable.effect, fixed ids from btl_startCmdEffect
  (Attack 0x64 + appearance kind, enemy Attack 0x77, Steal 0xC8/0x111/0x13/0x14, Throw 0x109-0x115 by weapon icon,
  Trance 0x101/0x1E9). Enemy ids from every encounter bundle on all four discs (kind 0x10 header, enemy ability i ->
  sequence i of the kind 0x11 bundle, btlseq ops 0x06 / 0x08 / 0x1A parsed with the op lengths of g_btlseqOps,
  names from the kind 6 text block, enemy names then ability names). 0 parse errors.
- Only fx_0fd has no reference. The table is in docs/effect-modules.md, and as a dict for `tools/effect_modules.py`
  KNOWN (the catalogue regenerates the doc) in `build/r31rt/known_effects.py`.
- Proposed module identity comments (headers of src/fxmod/fx_XXX): take the effect column. Examples the round asked
  about, jagged strip = lightning bolt, fx_003 Thunder one target, fx_042 Thunder all, fx_005 Thundara one, fx_049
  Thundara all, fx_018 Thundaga one, fx_085 Thundaga all, fx_0d7 / fx_0d8 / fx_0d9 Sword Magic Thunder / Thundara /
  Thundaga. Texel grid = fire and ice (fx_007 Firaga one, fx_048 Firaga all, fx_097 / fx_098 Blizzaga one / all,
  fx_0d6 Sword Magic Firaga, fx_0d3 Phoenix, fx_1fe Phoenix short, fx_0e1 Rebirth Flame, fx_1cc Black Waltz 1
  "Sealion appears", fx_114 / fx_1bd Ifrit full / short). Ribbon trail = fx_05e Death, fx_09a LV5 Death, fx_0ed
  Roulette, fx_0fb Madeen (and Terra Homing), fx_105 Odin, fx_1a2 Blank's Moonlight Slash. Streak field = fx_086
  Meteor, fx_08f Meteor (alt), fx_17d Ark. fx_18b Gizamaluke's Silent Voice, fx_112 Mimic's Eat.
- confidence: high (static data, two runtime spot checks per source kind).

### ovl_0b
- **BtlEnemyType.category bits: CONFIRMED (data).** btl_updateScan prints g_btlScanTexts[j] for each set bit j, and
  those texts are 0 Humanoid, 1 Beast, 2 Demon, 3 Dragon, 4 Undead, 5 Stone, 6 Insect, 7 Aerial. The Killer supports
  (`ff9text.py dump support`) 0xCD Bird Killer, 0xCE Bug, 0xCF Stone, 0xD0 Undead, 0xD1 Dragon, 0xD2 Devil, 0xD3 Beast,
  0xD4 Man Eater are support bits 13-20, which btl_applyKillerBonus pairs with 0x80 .. 0x01, the same families.
  Encounter records (+0x41) agree, Zuu 0x80, Hornet 0xC0, Gargoyle 0xA0, Skeleton 0x10, Grand Dragon 0x08, Hecteyes
  0x14, Fang 0x02, Soldier 0x01, Malboro 0. Proposed `BTL_ENEMY_HUMANOID 0x01`, `BTL_ENEMY_BEAST 0x02`,
  `BTL_ENEMY_DEMON 0x04`, `BTL_ENEMY_DRAGON 0x08`, `BTL_ENEMY_STONE 0x20`, `BTL_ENEMY_INSECT 0x40` (keep UNDEAD 0x10,
  FLYING 0x80 or rename AERIAL), and `SA_BIRD_KILLER 0xCD` .. `SA_MAN_EATER 0xD4` in ff9_ids.h. high.
- **Unk800D8124.unk2: CONFIRMED, party-target mode of op 0x72.** Its value comes from unit op 0x50 b (btlfx_opSetParamsA,
  g_btlfxOpArgs.unk1). Throw dagger (0x110, op args frames 4, mode 1) at Vivi (`--patch 0x0F,0x01,0x02`)
  btlfx_stepFaceHome runs once and snaps the rotation, at the Malboro (`--patch 0x0F,0x01,0x10`) it runs 4 frames.
  Scripts (file scripts + common scripts dump) use mode 0 in items and some abilities, mode 1 in Throw, Sword Magic,
  Sword Arts, Flair, Seiken, mode 2 only in the resident racket attacks 0x68 / 0x6A / 0x6F and 0x27. Proposed
  `partyMode` with `BTLFX_FACE_ALWAYS 0` (turn back over the frames), `BTLFX_FACE_SNAP_PARTY 1` (party target snaps
  back at once), `BTLFX_FACE_SKIP_PARTY 2` (party target, the op ends without turning). high.
- **BTLFX_HDR_RESIDENT: CONFIRMED, the physical-hit effects.** The header table is a VRAM copy (0x300,0x140) at
  0x801E7700 when btlfx_loadFile runs (dumped 0x400 bytes there, `build/r31rt/fxhdr.bin`). 118 ids carry 0x8000, all
  with header 0x83FF and none has a dir-13 file. The party Attacks 0x64-0x76, enemy/Marcus Attack 0x77, Steal / Mug
  0xC8 / 0x111 / 0x13 / 0x14, Jump 0x189, the enemy hits 0x139-0x178 and 0x1CD-0x1E5 (Tail, Strike, Claws, Fang, Head
  Attack, Virus Tentacles ...), and 0x10 / 0x12 / 0x25 / 0x27. Others look like 0x3340 (Bad Breath), 0x320 (Osmose),
  0x3320 (Fire all). Proposed comment on BTLFX_HDR_RESIDENT "weapon and physical-hit scripts (Attack, Steal, Jump,
  enemy melee), header 0x83FF, no module file". high.
- **Unk800AA08C.unk20: CONFIRMED, it is Battle11C0.encounter.** `disc3_malboro_lv99_calm`, RAM search for the kind
  0x10 item of encounter 899 (07 01 01 08 = unk0, groupCount 1, typeCount 1, abilityCount 8) found it at 0x8010ED9C,
  and the only pointer to it at 0x8010AECC, whose +4 points at the kind 0x11 item (0x8010E6F4, Battle11C0.bundle) and
  -0xC at the kind 6 text item (0x8010E490, Battle11C0.texts). So Unk800AA08C is a second view of Battle11C0
  (offset 0x14 `area` = texts, the DB_AREA key in a battle bundle is the name text block, 0x20 = encounter, 0x24
  sceneBuf = bundle). Proposed: drop Unk800AA08C for Battle11C0 in btl_loadEncounterFile / btl_initEncounter, or name
  unk20 `encounter` (Battle11C020 *). high.

### fxmod
- **VRAM_BTLFX_SCRATCH: CONFIRMED screen captures.** Slow (`vramfx.py slow 0x16 0x20 0x10`, file 0x7E) and Stop (0x95),
  12 + 8 + 10 VRAM dumps 20-30 frames apart. Slow, (0x1C0,0x180) 128x128 holds the rendered screen around the target
  (Malboro mouth and chains visible), refreshed every frame, and is drawn back as the swirling clock face. The clock
  texture itself sits at about (0x260,0x140) (file data). Stop, a ~192x192 copy of the screen including the HUD
  ("Zidane 9999") is written to (0x1C0,0x100) once and stays unchanged while the frozen grey image is shown. Proposed
  `VRAM_BTLFX_SCRATCH_X 0x1C0`, `_Y 0x100` as named, comment "screen capture of the target area drawn back as a
  texture (Slow per frame at y 0x180, Stop once)". fx_18b (Silent Voice) and fx_112 (Eat) not run. high.
- **fxm_setSignal(env, 7, v): CONFIRMED, script signal 7 (control op 1 waits on it), not a unit-op flag.** Mighty Guard
  (`--patch 0x18,0x54,0x0F`, 0x51), watch on 0x800FEC57. Writes from the module (0x801E7998, 8x), reads only from
  btlfx_runScript+0x43C / +0x460 (control op 01 01 07, a3 = 7), 108 reads. The scripts of fx_01d, fx_024, fx_02b,
  fx_045, fx_051 have `01 01 07` and fx_053 `01 01 04` / `01 01 05`. g_btlfxState.signals (0x10) is 4 bytes but op 1
  indexes up to 7, so b[4]-b[7] are motionDone / moveDone / turnDone / waitDone. Proposed: make `signals` an 8-byte
  `u8 b[8]` union covering those four, with `BTLFX_SIG_MOTION 4`, `BTLFX_SIG_MOVE 5`, `BTLFX_SIG_TURN 6`,
  `BTLFX_SIG_WAIT 7`, and `fxm_setSignal` stays (its 7 = BTLFX_SIG_WAIT, set by the module). high.
- **Motion slots 2 and 3: CONFIRMED hit and knock-down.** `disc3_malboro_lv99_calm`, Zidane's model->motion
  (0x80093BC4) poked to motions[0] / [2] / [3] (motion table 0x80108808), screenshots 8 and 18 frames later
  (`build/r31rt/mot_zoom.png`). [2] a short flinch, head back, [3] knocked off his feet backwards. Matches
  btl_startHitMotion (party [3] for abilities with flags2 0x81, enemies [3] = death motion or weak-stance hit). The
  module callers are enemy attacks on the party (motion 2 in fx_041 Whirl Slash, fx_09b Lava Gun, fx_100 Solution,
  fx_1a6 Water-gun, fx_1b2 Tidal Wave, motion 3 in fx_180 / fx_1ec Ultima, fx_1e7 Ragtime Mouse "Wrong!!!"). No
  module call was observed live (fx_0c2's copies are unused). Proposed `BTL_MOTION_HIT 2`, `BTL_MOTION_HIT_HEAVY 3`
  (knock-down), `fxm_startHitMotionIfActive` / `fxm_startHeavyHitMotionIfActive`. high (party), medium (enemy wording).
- **fxm_camChan* during Fenrir: CONFIRMED.** Fenrir full (`--patch 0x14,0x42,0x10`, support poke 0x8008B31B |= 8,
  btlfx_start 0xD2), write watch on g_btlCamera 0x800FE3B8 +0x50. fx_0d2's fxm_camChanUpdate stores (0x801EA33C /
  350 / 364 = vx/vy/vz) write SVECTOR slots 0-3 (points[0-3]) and 8, 9 (pointRots[2], pointRots[3]) from frame 9227 to
  10111 (~150-1100 writes each), fx_0d2_op80 (0x801E82F8) writes slot 7 (pointRots[1]). So the key nibble n addresses
  ((SVECTOR *)g_btlCamera)[n - 1] over points[6] + pointRots[4] (fx_0d2 uses 1-4, 9, 10). Proposed comment "key
  destination = g_btlCamera point n-1 (points then pointRots)". high.
- **Leviathan (fx_0b3) scroll textures: CONFIRMED, not a capture.** Full Leviathan (`--patch 0x10,0x3C,0x10` with the
  support poke, btlfx_start 0xB3), 24 VRAM dumps 60 frames apart. (0x1C0,0x100) 32x128 halfwords is indexed texel data
  that from frame ~400 on is a vertically wrapped copy of the (0x1E0,0x100) source rows (row 0 = source row 0, 104,
  80, ...), the (0x1C0,0x180) half stayed equal to the source in this window. Proposed comment "scrolling water texture,
  rows of the 0x1E0 source rotated into 0x1C0 each frame". high.

### main and overlays
- **CharBits.unkA (info bit 10): CONFIRMED victory pose.** All 9 characters have it in the disc-3 Lv 99 save. Malboro
  HP poked to 1, Attack, victory (`vic_on` / `vic_off`): with the bit Zidane jumps into his victory pose, with it
  cleared (0x8008B2C1 &= 0xFB, Zidane's info at GameData+0x3C+0x34) he stands in his idle stance on the same frames
  (`build/r31rt/vic_sheet2.png`). Proposed `victoryPose` (`CHR_INFO_VICTORY_POSE 0x400`). Event op 0xDB not traced.
  high.
- **GameData.unk892: INCONCLUSIVE (mechanism confirmed).** `cpass_wm_hadris` (vehicles found), watch 30 frames, 8
  writes each at wm_frameMain+0x38 (1) and +0x480 (0), reads 1 at the vsync pause and 0 with the menu open over the
  world map. It is 1 exactly while a world frame runs. WM_EVT_FIELD reaches the clear, so a normal world > field >
  world trip returns with 0 and resets the camera yaw. It stays 1 only when the frame doesn't finish, e.g. a save
  written during a world frame (ovl_39 stores it). Proposed `inWorldFrame` (medium) with a comment on the camera
  guard. A field entry from a vehicle was not tried.
- **Character.classFlags 0x1 / 0x4 / 0x8: NOT REACHED.** No enemy ability (all encounters, all discs, record +4) or
  party ability uses formula 0x62, only Lamia's Entice uses 0x61 (misses 0x2 female). Data only, 0x1 is on every
  row without 0x2 (Quina included), 0x8 only on Zidane's row, 0x4 on every other row. Proposed `CHR_CLASS_MALE 0x1`
  (medium), leave 0x4 / 0x8 literal (formula 0x62 is unused data).
- **g_akaoPauseFlags 4: CONFIRMED never set.** Only writers are akao_cmdPauseMusic/ResumeMusic (1), PauseSe/ResumeSe
  (2) and akao_init (= 0), no overlay asm references it. Write watch over a Fire cast (500 frames) 0 writes, value 0.
  Proposed `AKAO_PAUSE_FORCE_TICK 4` (tested by both tempo loops, never set, a debug step), medium.
- **AkaoStreamState.unk20 / unk30: NOT REACHED.** Read watches on 0x80083188 / 0x80083198 / 0x80083168 over 600 frames of
  `movie_debug`, `movie_vlc_1792187258632121515`, `field_d1_502`, `field_opening_dark`, 0 hits (no stream ran).
- **tailMode 0x20: NOT REACHED.** evt_getMsgLayout traced over 900 frames (Cross every 60) in 9 disc-1 states, 34 calls,
  flags 0x00 / 0x10 / 0x80 only. msg_open's own flags tests are 4, 8, 0x40.
- **FieldSpriteSet.unk0 / unk4: CONFIRMED write-only.** Set headers found by pattern (01 00 10 10, unk4 = header + 0x14)
  at 0x801275B0 (`field_dali_inn`) and 0x800F4430 (`field_d1_456`), read watches on +0 (2), +4 (4) and the 0x20-byte
  table, 600 frames each, 0 hits, while the same watch on +8 / +3 gets ~1000 hits from fld_drawSprites. Leave unnamed
  or `unused0` / `unusedIds`.

## R32 3DS requests: button sprites, field exits, ATE prompt, direct menu screens  (status: proposed; details in docs/r32/3ds-requests.md)
- binary: main, ovl_0a; data and script-format facts (names proposed at the end of docs/r32/3ds-requests.md)
- evidence (disc 1):
  - `field_d1_101`, VRAM dump (`tools/rt/vram.lua`): g_uiSprites 4-18 render as Cross, Circle, Square, Triangle, L1, L2,
    R1, R2, d-pad, 4 arm highlights, START, SELECT. VRAM (960,256) 64x38 and CLUT (576,224) 16x16 are byte-identical
    to common-DB TIM 0x42710 (dir 1 file 0x271A, game_loadCommonImages).
  - `field_d1_101`: region objects uid 15/16/17 (kind 3) hold the op-0x29 polygons. With Zidane teleported into
    uid 17, evt_checkPlayerAction calls evt_callObjectFunc(uid 17, 1, 2) every field frame; after walking up,
    evt_setNextLocation(0x70) from op 0x2B and field 112 loads. The global s16 at 0x8007AEF2 (arrival point) is
    written in the GOTO frame, then read and reset by the new field's init.
  - `field_d1_352`: poking g_evtGlobalVars.b[0xEE] (0x8007AFDE) = 1 and pressing Select runs the script's
    expression op 0x4F (0x800525C8) and msg_open(0x62, 1, .., 0x40) ("ACTIVE TIME EVENT" / Select event / Cancel).
    Poking g_atePromptMode = 1 draws "PRESS [SELECT] TO SEE ATE" at (16, 200).
  - `field_d1_352`: Triangle, break at ovl_0a menuMain_update, then s_menuMain.cursor = 5, g_menuScreen.slideIn = 0
    and pc = menuScreen_open(a0 = 6). The Card screen opens (ovl_36 resident, g_menuScreen.screen 6), Circle returns
    to the main menu with the cursor on Card, and a second Circle closes the menu.
- confidence: high for all four (code plus runtime). The ATE variables (b[0xEE], u16 at 0xEC, s16 at 0xF1) are
  script conventions seen in 47 disc-1 fields, not engine state.

## R32b 3DS requests: pointed-at item/ability/block per menu screen, Mognet, navi map names  (status: proposed; details in docs/r32/3ds-requests-2.md)
- binary: ovl_37, ovl_33, ovl_3c, ovl_39, ovl_0c, main (msg); existing names, plus the event-global/text-code names
  proposed at the end of docs/r32/3ds-requests-2.md
- evidence (disc 1):
  - `menu_items`: s_menuItem tab 0 state 2, itemList cursor 0 → g_menuHelp.text = s_menuItemHelp+0x3081 (Potion).
    Right, Down x2: cursor 2 col 1 → slot 5 = {0xF9, 2} Phoenix Pinion, help s_menuItemHelp+0x3229 = g_itemTable[0xF9].help.
    The help blobs of ovl_37/ovl_33/ovl_3c (0x33B4 bytes) are byte-identical.
  - `menu_equip`: Cross, Down, Cross, Down → s_menuEquip state 3, equipSlot 1, list cursor 1, items[1] = 0x72 Feather
    Hat. `until menuEquip_drawInfo+0x1DC` (return of chr_calcEquipStats): in.equip = 08 72 5a 98 ff, out stats
    32/50/47/39, defs 24/9/10/8/7 = the on-screen "after" column.
  - `menu_ability`: Cross, Right, Down → tab 0 state 2, lists[0] cursor 1 col 1 → actions[3].id 0x68 Soul Blade;
    trace shows chr_getApNeeded(0, 0x68) from menuAbility_drawInfo+0x1C8 each frame.
  - `menu_shop_weapons`: Buy, Down x2 → s_shop kind 1 cursor 0 inMode 1, s_shopBuy.items[2] = 0x10 Broadsword (hand on
    it). Sell, Right, Down → s_shopSell list slot 3 = Elixir.
  - `menu_save`: Down, Cross, 320 frames → s_menuSave state 4, list top 3 cursor 2 → files[5] (FILE06 Crystal World
    21:28:08): playTime 4637282 frames = 77288 s at 60/s, gil 9999999.
  - `field_dali_inn_moogle`: Mognet is script only (ovl_0a resident). msg_open log: (0xC,5), (8,2) moogle menu,
    (0x2C,2) "Read mail", (0x2E,3,flags 0x10) the letter "From Mois to Gumo"; letter list cursor = g_msgChoice.
    New state `build3ds2_mog_choice` (moogle menu open, Mognet highlighted).
  - `field_worldmap_ice_exit`, Select: navi full map. R1/L1 cycle g_worldNaviPlace; b[0x67] (0x8007AF57) = place + 1;
    each change calls msg_open(3, 4, .., 4), message 3 = F9 45 F9 0C F9 06 00 FF, inserting g_msgTextBanks[0] entry
    g_msgNumbers[0] (= b[0x67]). Writing g_worldNaviCursorX/Y near Ice Cavern snaps to it (b[0x67] 4); an unknown
    place gives 0. Markers measured at (mapX + 40.5, mapY + 9.5) ± 0.5 px for 7 places. New state `build3ds2_wm_navi`
    (full map open, cursor on Ice Cavern).
- confidence: high (code plus runtime for each).

## Round 33 runtime pass (2026-10-31): rows of docs/wip/r33_runtime_{fxmod_hi,fxmod_lo_A..D,main_overlays,ovl_0b}.md  (status: **applied**)
Setup: Redux disc 3, LuaJIT off, hang-safe runner `build/rt3/run3.py` from `disc3_malboro_lv99_calm` (calm pokes).
Any effect id is played by rewriting Zidane's Attack in `btl_playCmdEffect` (`--fn 'btl_playCmdEffect=r.a1=ID'`).
Two pokes after the load steer btlfx_resolveFirstUse: `GameData.firstUseFxSeen` is GameData+0x55E (0x8008B7AE in
this save, GameData = *(GameState+0x1C) = 0x8008B250), 0xFFFF plays the short summon ids, 0 the full ones, and
`g_btlfxLargeTargetFx` (0x800F727C) first byte = 0xFF switches off the Malboro's one-target to all-targets remap.
`build/r33rt/fxw.py NAME ID --op ADDR --w 'op:off:size' --d 'addr:size'` runs the effect twice: pass 1 logs the
op handler's mode-1 `work` pointer (the task heap gives the same address on a replay), pass 2 puts Read watchpoints
(or `:w` Write) on work+off / data and classifies every access by module function and opcode. A Read watchpoint
only fires on loads (fx_1cc: the single hit is the `-=` in mode 1). Logs `build/r33rt/batch*.log`, screenshots
`build/rt3/r33_*.png`, sheets `build/r33rt/s_*.png`. One Redux restart (a hang during a Ramuh write watch, redux.log
"Unknown error in event listener"). No new save states.
Where the code alone settles a row (ovl_0b asm, effect scripts, module data images in `disks/us/fxmod/`), the row
says "code" or "static" and gives the evidence.

### Patterns covering many modules
- **Work `unk0` set to 0 in mode 1 and tested == 0 every frame (fx_000 fx_002 fx_00a fx_00b fx_00e fx_00f fx_016 fx_01b
  fx_01e fx_01f fx_029 fx_02a fx_038 fx_046 fx_047 fx_04a fx_04c fx_058 fx_059 fx_05f fx_060 fx_061): REFUTED (nobody
  else writes it).** Representative fx_046 (Fire on the Malboro, the large-target remap of 0), work 0x800FF4EC, Read
  and Write watch over the whole effect. Accesses are btlfx_heapAlloc+0x60 (the zero fill of btlfx_allocTaskMem), the
  `sw zero` at fx_046_op80+0x44 in mode 1 and the `lw` at op80+0x108 every frame (40 hits), nothing else. The work
  block is private task heap memory (btlfx_spawnTask passes it only to the handler), so the same holds for every
  module of the list by code inspection. Proposed `stop0`, a dead early-out flag that is always 0. medium (the role
  is a guess, the "only 0" part is high).
- **Strip point halfword +6 (`Fx026Pt.unk6`, `Fx062Strip.unk114`/`Fx063Strip.unkFC` copies, `Fx079Pt.unk6`,
  `Fx07aPt.unk6`, the fx_1f7 0x80 / fx_1f2 work unk38 0x200 / fx_1f3 0xC0 pads): CONFIRMED width (code).**
  btlfx_pushTrailPoint stores `z | (v << 17)`, i.e. point+6 = 2 * the frame entry's byte 2 scaled by `scale`.
  btlfx_emitTrailRibbonScreen loads `lh 0x6` of the point into IR0 and runs `gpf` on the unit normal of the segment
  (rsqrt-normalised screen delta), so it is the distance of the strip edges from the centre line. btlfx_emitTrailRibbon3D
  (`lh 0x6` + gpf) and btlfx_emitTrailRibbonFlat (`lhu 0x6` + gpf) do the same, btlfx_emitTrailLines ignores it.
  Proposed `halfWidth6` (half-width of the ribbon at this point, scales the edge normal). high.
- **Trail ring header bytes (every `FxXXXTrail` / `Bolt` buffer, fx_134 `Fx134Bolt` 0x18, 0, 0x18): CONFIRMED (code).**
  btlfx_drawTrail frame -1 writes out[0] = out[1] = length, out[2] = 0. btlfx_pushTrailPoint: head = out[1] - 1,
  wrapping to the length passed (slots 0..length), point at out + 4 + head * 8, out[1] = head, out[2] = count + 1
  until it equals length. The emitters wrap with out[0] + 1. Proposed `lastSlot0` (highest slot index, the ring holds
  lastSlot0 + 1 points), `head1` (slot of the newest point), `count2` (points pushed, capped at the length). high.
  fx_134's 0x18, 0, 0x18 is a full 24-point ring with the newest point in slot 0.
- **Particle `unk12` passed to btlfx_stepModuleParticle (fx_026 `Fx026Shard.unk12` 0x1000, fx_0a9 `Fx0a9Part.unk12`
  0xE00 +- 0x20, fx_1e9 / fx_101 / fx_18a `Spark.unk12` 0xE00 + randCentered(0x200)): CONFIRMED drag (code).** The
  step (src_equiv/ovl_0b/math.c BtlfxBody) puts +0x12 in IR0 and runs `gpf` on the velocity, velocity = velocity *
  drag >> 12 every frame, 0x1000 keeps the speed. Proposed `drag12` (velocity factor per frame, 0x1000 none). high.
- **Summon slot `unk54` (fx_026 `Fx026Summon.unk54`, fx_0b1 / fx_0b3 / fx_09a, fx_0e1_0 `Fx0e10Summon.unk54`, the
  `Fx105Summon` copies): CONFIRMED (code).** env->loader is &g_btlfxLoader (Unk80100DC0), +0x54 is
  `g_btlfxLoader.summon` (Unk80100DC054 *, the summon file in Battle.partyBufs[0]) whose +0x180 is `motions[]`, and +0x60
  is `summonModel`. Proposed `summon54` everywhere (one shared view of Unk80100DC0 would do). high.
- **Module data reloaded per cast (fx_197 `fx_197_overlayOn`): CONFIRMED reloaded.** Diamond Dust (0x197, short ids
  forced) cast by Zidane, `btlfx_spawnTask` traced with a dump of 0x801E91C4: 1 at the first spawn, 0 after frame 20
  of op81, still 0 after the cast. Second cast from the next menu (`--noload`), first spawn again reads 1, so the
  effect file (module image included) is read again for every cast and the "only cleared" flag is per cast. high.
  Name stays.

### fxmod fx_100..fx_1ff (r33_runtime_fxmod_hi.md)
- **fx_105 `fx_105_quads`: CONFIRMED wind streaks.** Odin (0x105, full). fx_105_drawQuads runs at effect frames
  627-768. Patching it to `jr ra` after the module loads (`--fn` on fx_105_op80 writing 0x03E00008 at 0x801E9F28)
  removes the bright white-blue translucent streaks that rush past behind Odin during the charge
  (`build/r33rt/s_odin_abl.png`, top row normal, bottom ablated). The update moves them along z and wraps them.
  Proposed `fx_105_windStreaks` (translucent streak quads rushing past during Odin's charge). medium.
- **fx_105 `fx_105_screenSprites`: CONFIRMED twinkling stars.** btlfx_drawSpriteAtOt from the module at frames
  385-663, the small flickering points over the night sky and the moon (`build/rt3/r33_odin_08.png`), drifting left
  after timer30 < 0x28. Proposed `fx_105_stars` (screen sprites twinkling in the night sky, drift left and wrap). medium.
- **fx_114 `Fx114Work80.unk164`: CONFIRMED loop flag (code).** Hi_DrawSummonModel arg 4 bit 0 = wrap the summon motion
  to frame 0 at its end, else hold the last frame. Proposed `loopMotion164` (1 loops Ifrit's motion before frame 0x30,
  0 holds its last frame). high. Same for fx_0e2 `Fx0e2Work80.unk12C` (read 131 times by fx_0e2_op80+0x1010 for that
  argument during Fenrir, below).
- **fx_138 `D_801E8408` (3 `Fx138Ring`) and work `unk88`..`unk8E`: CONFIRMED unused.** Lucky Seven (0x138), Read watch on
  0x801E8408 (0x48 bytes) and work 0x800FF4EC+0x88 (8 bytes), 0 hits over 600 frames. No slot reels on screen
  (`build/r33rt/s_138.png`). The ring models (`model96`) are registered and never drawn. fx_138_randSets rows are a
  permutation of 0x14 / 0x28 / 0x50 plus 1 or 2 (reel stop frames and a result kind, by the look). Proposed
  `fx_138_reels` (three slot-reel records set up from a fx_138_randSets row, never read), work `reelResult8A`
  (fourth value of that row), `unk88` / `unk8C` / `unk8E` stay (only initialised). low for the reel reading.
- **fx_17d_4 `D_801EB25C`: CONFIRMED no other reader (static).** No access to 0x801EB25C in the asm of any fx_17d_N
  group, any other module or ovl_0b, and groups 0/2/4/6/8 replace each other at 0x801E7700. Proposed
  `fx_17d_4_unused` or leave. high.
- **fx_17d_6 / fx_17d_7 `Work83.unk8` (`kind8`): CONFIRMED values (static).** File 0x17D script: `05 06` (group 6) ...
  `83 00 00` at +0x1A7, then `05 07` (group 7) ... `83 00 00` at +0x213 and `83 01 00` at +0x21C. Op 0x83 mode 1 gets
  `args[0]` = byte a. So fx_17d_6_op83 only ever gets kind 0 (its `kind8 != 0` early return is dead), fx_17d_7_op83
  gets 0 then 1. Names stay (`kind8`). high.
- **fx_186 `Fx186Rock.unk6`: INCONCLUSIVE (code only).** State 0 uses it as the hop amplitude (sin), state 1 adds
  unk6 / 5 to y and then 12 to unk6 (a falling velocity). Proposed `lift6` (hop height in state 0, then y velocity
  with gravity 12 in state 1). low, not watched.
- **fx_18a `fx_18a_op80Done`: CONFIRMED no reader.** Kuja's Transform (0x18A) played in full, Read watch on 0x801E89D8,
  0 hits. Static: no access outside fx_18a_op80. Proposed keep the name, comment "set at the end, nothing reads it". high.
- **fx_197 `fx_197_overlayOn`**: see the reload pattern above.
- **fx_1cc work `unk70`: CONFIRMED unused.** Sealion appears (0x1CC), work 0x800FFC7C, Read watch on +0x70 (8 bytes):
  one hit, fx_1cc_op80+0x2F0 `lhu` of unk70.vy, the `-= 0x80` in mode 1. Proposed `unused70`. high.
- **fx_1e9 `Fx1e9Spark.unk12` (and fx_101 / fx_18a): drag**, see the patterns above.
- **fx_1ec `Fx1ecWork80.unk3C` / `unk44`: CONFIRMED unused.** Ultima (0x1EC), work 0x800FF4EC, Read watch on
  +0x3C..+0x4B, 0 hits. Proposed `unused3C` / `unused44`. high.
- **Trail point pad / trail header**: see the patterns above (width, lastSlot/head/count).
- **fx_1f2 `fx_1f2_unitTrails[4]` cleared as 16: CONFIRMED the array is 16 long (static).** The symbol runs from
  0x801EBB90 to fx_1f2_effModels at 0x801ED090, 0x1500 bytes = 16 * 0x150, all zero in the module image. Only entries
  below targetCount are drawn. Proposed declare `Fx1f2Trail fx_1f2_unitTrails[16]`. high.

### fxmod slice A fx_000..fx_063
- **work unk0 pattern**: REFUTED, see above.
- **fx_018 `Fx018Work83.kind18`: CONFIRMED (static).** File 0x18 (Thundaga) script runs op 0x83 three times,
  `83 00 00` (+0x2A), `83 01 00` (+0x39), `83 02 00` (+0x3F): the three bolt stages of one cast, the kind is the op's
  byte a. No other file loads module 0x18. Name stays. high.
- **fx_026 `Fx026Pt.unk6`**: width (pattern). **`Fx026Shard.unk12`**: drag. **`Fx026Summon.unk54`**: summon54.
- **fx_026 `Fx026Work80.unk64`: CONFIRMED unused.** Shiva (0x26 full), work 0x800FF57C, Read watch on +0x64, 0 hits.
- **fx_034 helpers vs fx_096 / fx_1b3: REFUTED as copies.** `tools/fxmod_dups.py --func fx_034 <f>` puts
  fx_034_explodeModel / pushPrims / splitModel / calcPrimCentres each in a group of one, so no shared fxm_ names.
- **fx_03a `Fx03aWork.unk10[4]`: CONFIRMED unused.** Earthquake (0x3A), Read watch on work+0x10 (16 bytes), 0 hits.
- **fx_04c `((u8 *)env->state)[0x12]`: CONFIRMED script signal 2 (static).** It is Unk800F78A40.signals[2]. File 0x4C
  script runs `80`, `81`, `01 00 0c`, `82`, then `01 01 02` (op 1 a=1 waits for signal b=2) before the fade. Proposed
  write it as `env->state->signals[2]` (op82 raises it when every target has been visited, the script waits for it). high.
- **fx_04f `unk8`, fx_00b / fx_02a `unkC`: CONFIRMED unused.** Reflect (0x4F), Esuna (0xB), Silence (0x2A, remap off),
  Read watches on those words, 0 hits each.
- **fx_062 / fx_063 strip unk114 / unkFC**: width (pattern). Proposed `width114` / `widthFC`.
- **fx_057 `otZ8`: CONFIRMED (code).** btlfx_addRadiusSixth(0, 0) = target 0's model radius / 6. Proposed `targetOtZ8`
  (target 0's model radius over 6, the depth bias of its emitters and rings). high.
- **fx_05d `unk40` / `unk58`: CONFIRMED unused.** Bio (0x5D, remap off), Read watches, 0 hits.
- **fx_059 `posZ4`: REFUTED direction (code).** btlfx_pushViewRotation sets the view to a rotation with translation
  (0, 0, ReadGeomScreen()), so z is distance into the screen from the projection plane. posZ4 starts at -0x20 and
  grows 0xC8 per frame, so the spiral head recedes from the viewer. Proposed `depthZ4` (spiral head distance into the
  screen in the view-rotation frame, grows 0xC8 per frame). medium.
- **fx_004 / fx_04d `unk2C`: CONFIRMED plain frame counter.** Blizzara (0x4 with the remap off, and 0x4D), Read watch on
  work+0x2C, 46 hits each, all the `lw` of the `++` at op80+0x660 / +0x664. Proposed `frameCount2C`
  (incremented every update, never read). high.

### fxmod slice B fx_078..fx_0b4
- **fx_079 / fx_07a `unk6`**: width (pattern), timer * 4 makes the strip thin out as it ends. Proposed `halfWidth6`.
- **fx_07a `D_801E9504`: CONFIRMED unused.** Demi Shock (Elan, 0x7A), Read watch, 0 hits. Proposed `fx_07a_unusedRadius`.
- **fx_0a9 `Fx0a9Part.unk12`**: drag (pattern), 0xE00 slows the scattered bolt points about 12% per frame.
- **fx_0b3 `fx_0b3_sharedModel`: CONFIRMED sea and sky.** Leviathan (0xB3 full). The model is drawn every effect frame
  372-1727 by the module. Zeroing its scale VECTOR in a `--fn` on Hi_DrawEffModel (only when a3 = the id at 0x801EE270)
  leaves black where the ocean surface and the sky dome were (frames 400 and 600 fully black, at 1000 the sky behind
  Leviathan is gone) (`build/r33rt/s_levi.png`, top normal, bottom ablated). Proposed `fx_0b3_seaModel` (ocean surface
  and sky dome backdrop of Leviathan's scenes). high.
- **fx_0b3 `Fx0b3Track.unk0`, fx_0b4 `Fx0b4Path.unk0`: REFUTED (stride hypothesis), always 0.** `build/r33rt/k33.lua`
  hooked btlfx_getFileData and recorded every block used. Aero (0xB4) blocks 1-6 are 388 bytes = 4 + 0x20 keys * 0xC,
  header 0000 0020. Leviathan's key blocks have strides of 3, 6 and 9 halfwords (blocks 9-13, 17-26, 32-34, 39, 41-46,
  63) and all start with 0000. Proposed `pad0` (always 0, the count follows). high.
- **fx_0b1 / fx_0b3 / fx_09a `unk54`**: summon54 (pattern).
- **fx_097 / fx_098 `unk70`: CONFIRMED unused.** Blizzaga 0x97 (remap off) and 0x98, Read watch on work+0x70: one hit
  each, the `lhu` of the `-= 0x80` in mode 1 (fx_097_op80+0x2CC, fx_098_op80+0x2EC). Proposed `unused70`.

### fxmod slice C fx_0b5..fx_0e0
- **fx_0df `fx_0df_recordElemDraw` (0x801E7EB8): CONFIRMED never called.** Flare Sword (0xDF, Zidane as caster, no
  partner), Exec breakpoint 0 hits. fx_0df_recentNext (0x801EA410) is only written once (`sw` at fx_0df_op80+0xD8 in
  mode 1), never incremented. No word 0x801E7EB8 in the module image or the whole effect file 0xDF (266240 bytes,
  disc 3). So fx_0df_recentPos stays as loaded and op83 picks from it. Proposed keep the name with a comment
  "unreferenced element draw callback, never runs". medium (a run with a real Vivi partner was not tried).
- **fx_0e0 `D_801EB1D8` / `D_801EB1DC`: CONFIRMED dead (static + runtime).** Both 0 in the module image. Doomsday Sword
  (0xE0) played in full (`build/r33rt/s_0e0.png`), Read watch on both words, 0 hits. fx_0e0_op80 spawns its pool items
  only with state0 = 3, so the state 0/1 branch that reads them never runs. Proposed `fx_0e0_captureRadius` and
  `fx_0e0_endEarly` (dead, 0 in data). high.
- **fx_0ba `D_801EC550`: CONFIRMED "before the orb" flag (static + runtime).** 1 in the module image (file offset
  0x4E50). Ramuh (0xBA full): 925 reads at fx_0ba_op82+0x854 / +0x904 and fx_0ba_op83+0x8DC, all within the first ~350
  effect frames, none after the orb. The write watch run hung Redux, so the clear itself was not caught (static, only
  fx_0ba_op80 frame 0x3C writes it). Proposed `fx_0ba_preOrb` (1 from the data until the orb appears, bolts spawn
  every frame and draw impact rings while set). medium.

### fxmod slice D fx_0e1_0..fx_0ff
- **fx_0e1_1 `Fx0e11BattleState.unk22`: CONFIRMED partyReload.** Unk800F78A40 +0x22 is Unk800FEC40.partyReload
  (control op 11). Rebirth Flame (0xE1), write watch on 0x800FEC62 over 2800 frames: btlfx_reset+0x10 and
  btlfx_start+0x4C clear it, btlfx_runScript+0x608 sets it (op `0b 00 00` at script +0x78), btlfx_runScript+0x48 clears it
  200 frames later when btl_reloadParty finishes. fx_0e1_1_op80 waits for it after frame 0x35 to start the fade.
  Proposed name the module view's +0x21 / +0x22 `ending` / `partyReload` like Unk800FEC40. high.
- **fx_0e1_1 `Fx0e11Work.unk10`, fx_0e1_0 `Fx0e10Work80.unk30`: CONFIRMED unused.** Read watches over the whole Rebirth
  Flame, 0 hits.
- **fx_0e1_0 `fx_0e1_0_scaleXY` (0x801E88F0): CONFIRMED never called.** Exec breakpoint over 2800 frames, 0 hits.
- **fx_0e1_0 `unk54`**: summon54 (pattern).
- **fx_0e7 `Fx0e7Work81.unk68`: CONFIRMED stays -1.** Thievery (0xE7), work 0x800FF508. Write watch on +0x68: only the
  heap zero fill and the `-1` store at fx_0e7_op81+0x18C. Read 32 times at op81+0x414 (the `>= 0` test). Proposed
  `fadeInTimer68` (a fade-in countdown this op never starts). medium.
- **fx_0f3 `Fx0f3Work.unk20`: CONFIRMED unused.** Scoop Art (0xF3), first of three op80 tasks, Read watch, 0 hits.
- **fx_0fc identity: INCONCLUSIVE (name NOT REACHED).** The only users are encounters 220 and 238 (all discs), whose
  names don't decode with the US table (`<F6>„<A7><BC>`, ability 2 `<FAD5><FC>=`). Entering 220 from the Pandemonium
  field hung the loader (pc 0xD80). Played on the Malboro, fx_0fc_op80 moves target 0 back along +z 0xC0 per frame and
  fades it out at frame 0x12, the Malboro slides away and vanishes (`build/r33rt/s_0fc.png`). Proposed module label
  "enemy retreat (slides back and fades out)", from encounters 220 / 238.
- **fx_0ed `Fx0edWork80.unk20`: CONFIRMED unused.** Roulette (0xED), Read watch, 0 hits.
- **fx_0e3_0 `Fx0e30Work80.unk48` / `unk68` / `unk70` / `unk7C`: CONFIRMED unused.** Bahamut (0xE3 full), Read watches
  on +0x48 and +0x68..+0x87, 0 hits.
- **fx_0e3_1 `Fx0e31Work80.unk48`: CONFIRMED swing base.** Read 33 times by btlfx_pointAhead+0x58/0x64/0x70 (11 effect
  frames), the call that turns it into camTarget40 = camera point 3. Proposed `swingBase48` (point ahead of the hand bone,
  origin of the camera target swing). high. **`unk6C`**: Read watch, 0 hits, unused.
- **fx_0e2 Fenrir: `unk12C` CONFIRMED loopMotion (read every frame as Hi_DrawSummonModel's flags), `unkA4`, `unk128`,
  `unk130`, `unk134` CONFIRMED unused** (Read watches on work+0xA4 and +0x128..+0x137, the only hits are on +0x12C).
  `Fx0e2Ribbon*` / `Fx0e2Path80` fields not watched.
- **fx_0ec `Fx0ecVertex.unk6`, fx_0fb_0 / fx_0fb_1 rock fields, fx_0fa: NOT REACHED** (no single address to watch,
  time spent elsewhere).

### main and overlays (r33_runtime_main_overlays.md)
- **#1 `GameData.flags` 0x40: REFUTED "only cleared" (code).** It is set through g_fieldFlagQueue: fld_movieUpdate
  queues 0x44 (with GD_HIDE_MODELS) for a field movie whose movieRgb24 is nonzero, fld_startEncounter queues 0x43
  (with GD_MENU_DISABLED and GD_MENU_OPEN), fld_endFrame applies the queue. The two clears are the end of those
  (fld_movieFinish, fld_updateEncounter). Runtime only showed it clear (Pandemonium field 0x33, Malboro battle 0x31,
  0 writes in 300 battle frames). No reader found in C. Proposed `GD_FLD_TRANSITION 0x40` (set while an encounter
  starts or a colour-keyed field movie plays), low.
- **#2 `FieldMap.flags` 0x100: REFUTED "only cleared" (code).** fld_startEncounter queues mapFlags 0x1919 (0x100 with
  NO_WINDOWS, NO_BGI_ANIM, NO_VIBRATION, NO_FADE, NO_PRELOAD) and fld_updateEncounter clears the same set. Reader not
  found. Proposed leave literal or `FLD_MAP_ENCOUNTER_0x100`, low.
- **#3 `FieldBgState.flags` 0xC00: CONFIRMED never set.** Pandemonium field, g_fieldBgState (0x800CA068) = 0x60, write
  watch 300 frames 0 writes, no C setter. Leave literal.
- **#4 soundLoadBank 0x84, #5 AkaoStreamState.unk20 / unk30, #6 FieldActorExt 0x02000000: NOT REACHED** (disc 1 field
  work, not attempted this pass).
- **#7 GameState.mode 5 / 6**: no runtime action (left literal).

### ovl_0b (r33_runtime_ovl_0b.md)
- **#1 `EffEmit.kind`: CONFIRMED partly.** btlfx_spawnEmitter traced with a 0x14-byte dump of a2 over 12 effects. Kinds
  seen ballistic 0 (Fire, Thundaga, Holy, Comet, Firaga, Thundara, Haste, Earth Shake), orbit 1 (Holy, Firaga, Cure,
  Aero and the enemy cast emitter 0x800F7DD8), spiral 2 (Fire, Firaga, Earthquake, Aero), helix 4 (the party cast
  emitter 0x800F7F14 at every spell start, Thundara), module 0xFF (Aero). The helix one is the sparkle swirl winding
  up around Zidane while he casts (`build/r33rt/s_kinds.png`), matching "helix". Wobble 3 and homing 5 not seen.
- **#2 floorMode other than 0/1: NOT REACHED.** Every ballistic emitter of those 12 effects has floorMode 0.
- **#3 `BtlCmd8.forceCamera`: CONFIRMED scripted actions (static).** Parsing every encounter bundle (Battle11C020
  header, groups 0x38, type records 0x74, then BtlCmd8 0x10 each, 562 encounters, 3525 abilities): bit 30 is set on 227
  abilities in 102 encounters, all scripted or story actions ("Happy"/"More" of the friendly monsters, Ragtime Mouse
  "Read Quiz", death and appearance sequences, "Camera use", "Death camera", Beatrix Stock Break / Climhazzard,
  Sealion appears, Kuja Transform / Ultima). Name fits. high.
- **#4 `BtlCmd8` bits 17-29 / 31: CONFIRMED data only (static).** Bits 17-28 are 1 in all 3525 enemy abilities (a 0xFFF
  field), bit 29 is set on 17 (Epitaph "Battlemes0", Tonberry "Knife" / "None", Gargoyle / Agares "Rise"), bit 31 on
  none. Battle16CC / g_usableItemTableById not checked.
- **#5 `skipParty` (ops 0x6C / 0x6D byte b): CONFIRMED (static).** `tools/rt/fxscan.py 6c,6d`: b = 1 in the items
  (Elixir, Ether, Echo Screen, Eye Drops, Magic Tag, Vaccine, Annoyntment, Dark Matter, Dead Pepper, Gysahl Greens, Tent,
  gems), the Break skills, Reis's Wind, White Draw, Shock, Eat, Cook, Dragon Breath, Cherry Blossom, Dragon's Crest, Six
  Dragons. b = 0 in Luna and Focus. So it is set by abilities that can target allies, where the caster stays home
  instead of jumping at a party member. high for the data, the on-screen difference was not filmed.
- **#6 `func_800EA1D0` callers: CONFIRMED none.** No `jal 0x800EA1D0`, no word 0x800EA1D0 and no `addiu` with low half
  0xA1D0 in any overlay, the exe or any effect module. Exec breakpoint over 12 effect runs, 0 hits. So
  g_btluiState.unk9 / unk14 are written by dead code only. high.
- **#7 `FileEntry16.idHi`: NOT REACHED.**
- **#8 `WmSrcFT4.unk14`, `BtlBoneDef.unk2`: INCONCLUSIVE (asm).** btl_drawMesh has a `lbu 0x14` (0x800C6070) in a
  0x18-stride loop that tests its 0x2 bit (WMSRC_DOUBLE_SIDED), which looks like a different source type's flags byte, not
  FT4's unk14. btl_calcBoneMatrices reads byte 2 of the header (`lbu 0x2` after `lw 0x8(s4)`), not of a bone. A
  per-loop mapping of the source types is needed.

## R32c 3DS requests: shopkeeper talk, call hand-off and MENU (status: proposed; answers in docs/r32/3ds-requests-3.md)
- binary: main (event script interpreter `evt_execInstr`, `evt_callObjectFunc`, `evt_checkPlayerAction`, `menu_open`)
- evidence (disc 1, `tools/rt/enter_field.py 354` / `355` from `field_dali_inn`, controlEnabled `EvtState+0x16EC` poked
  to 1, exec breakpoints on evt_execInstr / evt_callObjectFunc / menu_open, scratch `build/3ds3/optrace.lua`; new states
  `3ds3_f354`, `3ds3_f354_counter`, `3ds3_f355_counter`):
  - 354 (Dali weapon shop): `playerUid` 11 = entry 11 (model 98, Zidane); the shopkeeper is uid/entry 3 (model 55).
    Cross → `evt_callObjectFunc(uid 3, 1, 3, 0)` from evt_checkPlayerAction; uid 3 runs MSG_WAIT 0x149 "Welcome!",
    then `75 00 02 00` in its own tag 3 (+0x9A) → `menu_open(2, 0)`, ovl_3c "Weapons". No call op in between.
  - 355 (Dali item shop): Cross inside region uid 16 → its tag 3 runs `12 00 06 fa 0d` (player tag 13) and
    `14 00 02 05 12` → `evt_callObjectFunc(uid 5, lv 2, tag 18, save 1)`; the region then sits at wait 0xFF
    (EVT_WAIT_CALL) while uid 5 runs its own tag 18 at level 2, which executes `75 00 02 19` → `menu_open(2, 25)`
    (Potion, Phoenix Down, ...).
  - Static: a disassembler built from evt_execInstr's cases decodes every field script on the 4 discs. MENU flags are
    always 00 except the Moogles' `75 02 02 d2 01 7f` (shop id from the Moogle's local s8 var 1). Byte-search hits such
    as `75 04 20 00` (SWITCH offset + MSG op) and `75 01 05 d9` (text id 0x175 + EXPR) are not MENU instructions.
- proposed: `EVT_OP_CALL_CALLER 0x16`, `EVT_OP_CALL_CALLER_WAIT_FREE 0x18`, `EVT_OP_CALL_CALLER_SYNC 0x1A`;
  `MENU_TYPE_MAIN 0 / NAME 1 / SHOP 2 / PARTY 3 / SAVE 4 / CHOCOGRAPH 5 / DISC 8` for `menu_open`'s first argument.
- confidence: high for the call semantics and MENU encoding (C plus runtime); the 0x16-0x1A ops were not run.

## R32d 3DS requests: Tetra Master phases, cursors, board and panels (status: proposed; answers in docs/r32/3ds-requests-4.md)
- binary: ovl_0d (tm_mainLoop and its task handlers), main (`g_cards`, `card_*`, `evt_setCardOpponent`), ovl_36 (Card screen)
- entry recipe: no save reaches a card game, so `build/3ds4/enter_tm.py OPP FRAMES` (scratch) forces one from a paused
  field: field cleanup to main's dispatcher 0x80012834 (as `tools/rt/enter_battle.py`), then `GameData+0x8B2`
  (cardOpponent) = OPP and `GameState.mode` = 9. From `field_dali_inn` (disc 1) with OPP 12 the "Card Selection" screen
  comes up and the game later returns to the Dali inn (mode 1, prevMode 9). The real trigger is op `AE 00 lo hi` in an
  NPC's Square function (tag 8); disc 1 has 41 of them, opponent ids 0-34 (`build/3ds4/cardscan.py`, scratch).
- `TmState` (tm_mainLoop local) is at 0x801FF9E0: `trace tm_stockInput tm_drawStock --frames 2` gave a0/a2 =
  0x801FF9E0, and `unk5C8` there is {50, 30, 200, 100} (tm_mainLoop's constants). Same in every task and in the second
  game. Offsets of include/ovl_0d.h TmState agree with every field read (cursor, boardX/Y, stockRow 0x478, getCursor
  0x5B0, targets 0x48C, numTargets 0x4AC, autoPlay 0x438, board 0x268).
- cursor writes followed by one Cross: stockRow=3 → card id 3 picked; cursor=4 → PLAYER_SET with card 4; boardX/Y=1/0 →
  card placed at (1,0); BATTLE_SELECT (numTargets 2, targets = cards 6 and 5) boardX/Y=0/1 → BATTLE dst (0,1);
  getCursor=1 → second captured card traded ("Received Lizard Man"); cursor=1 in TRY_AGAIN → END_FADE.
- task sequence seen: 18 CHOICE → 19 → 21 FIELD_FADE → 33 ENEMY_APPEAR → 28 COIN → 30/8 → 0 PLAYER_SELECT → 1 → 7/5/6 …
  → 3 RESULT → 14 CARD_MOVE → 34 GET_SELECT → 15 → 13 → 16 TRADE → 17 → 22 AWAY_INFO (stock 101) → 23 → 24 → 11
  TRY_AGAIN → 27 END_FADE.
- GET SELECT runs after a win with 2+ (not all) enemy cards captured (here 2), not only after a perfect game: the
  `tm_taskGetSelect` symbol comment ("after a perfect win") should be corrected (C: tm_taskCardMove's
  `countOwned != 10 && countCaptured != 1`).
- card pictures: VRAM dump during the match rendered with x = (id/30)*128 + (id%30%6)*21 halfwords, y = 0x100 +
  (id%30/6)*51, 8-bit, CLUT (0x180, 0x19C+id) gave correct faces for ids 0, 1, 3, 5, 31, 99 (`build/3ds4/cardimg.py`).
- data read: `g_tmArrowSprites` (8 x {x, y, smallX, smallY, u, v}), `g_tmOpponents` starts {0,0},{0,1},{0,2},{0,3},{1,0}…
  (opponent 12 → deck 3, AI 0 = TmState.deck/aiLevel), `g_tmSeIds` = 1860, 1853, 1854, 1855, 1856, 1857, 1858, 1859,
  1852, 1861, 1862, 1863, 1864, 2331, 2332, 2333.
- states (disc 1): `3ds4_tm_card_select`, `3ds4_tm_confirm`, `3ds4_tm_player_select`, `3ds4_tm_turn2`,
  `3ds4_tm_battle_select` (board poked), `3ds4_tm_result_lose`, `3ds4_tm_result_win` (cards poked), `3ds4_tm_get_select`,
  `3ds4_tm_over100`, `3ds4_tm_rematch`.
- proposed: fix the `tm_taskGetSelect` comment; `TM_SEL_YES 0` / `TM_SEL_NO 1`; note the TmState runtime address
  0x801FF9E0 in include/ovl_0d.h's TmState comment.
- confidence: high (C plus runtime for every cursor and confirm path listed; GAME_QUIT, EXTENSION and CARD_LESS5 were not run).

## R34 3DS request: Mognet (status: proposed; answers in docs/r32/3ds-requests-5-mognet.md)
- binary: main (`msg_input`, `msg_update`, `msg_getChoiceIndex`, `msg_close`, `evt_update`, `evt_findNearestObject`,
  `evt_callObjectFunc`), field event scripts (58 Moogle talk functions, tag 3); disc 1, field 351 (Dali inn Moogle)
- confirm without a press: at `evt_updateObjects` entry (`until evt_updateObjects`), `write g_msgChoice ROW` and
  `write g_msgPadPressed 0x20000 --size 4` → the script's `WAIT 1 / JMP_IF !padPressed(0xB0000)` loop accepts it.
  From `build3ds2_mog_choice`: Mognet menu (mask 6, "You have a letter from Kumop to Mogki" in window 5), then
  row 0 → letter list (mask 0x8001 "Trapped In Ice! / Cancel"), row 0 → window 3 msgId 0x2E flags 0x10 (the letter).
  States `build3ds5m_mognet_menu`, `build3ds5m_letter_list`, `build3ds5m_letter_open`.
- `msg_close(3)` run in place of one `msg_update` (pc hijack at its entry, ra kept) closes the letter; the script
  (op 0x54 wait) fades back and continues. Hijacking at `evt_updateObjects` entry with ra = pc instead loops forever
  (its own ra is lost); use a function whose return the caller ignores.
- `evt_findNearestObject(player 0x8007805C, 4, NULL)` (same hijack): 0x80078588 = the Moogle (kind 4, scriptIdx 8,
  modelId 220, talkRadius 22) at `field_dali_inn_moogle`, NULL at `field_dali_inn`; `evt_callObjectFunc(moogle, 1, 3, 0)`
  opens the Moogle menu (mask 0x47).
- Mognet globals (g_evtGlobalVars.b): 0x400 version, 0x408 give counter (2 → 3 on Give), 0x40A + 4k carried slots
  {used, letter, from, to} (save: 01 06 08 0A), 0x417 taken bits / 0x41F received bits (MSB-first per byte; letter 5
  bit set again after the poked delivery), 0x428/0x437 list scratch (5 / 6). Static survey of all talk functions in
  the answer doc (46 letters, ids 3-48, Moogle ids = text bank msg 0 names).
- proposed (not applied): `EVT_SYSVAR_MSG_CHOICE 9`, `EVT_SYSVAR_CONTROL_ENABLED 2`; `EVT_GB_MOGNET_VERSION 0x400`,
  `EVT_GB_MOGNET_GIVE_COUNT 0x408`, `EVT_GB_MOGNET_SLOTS 0x40A`, `EVT_GB_MOGNET_TAKEN 0x417`, `EVT_GB_MOGNET_RECEIVED
  0x41F`, `EVT_GB_MOGNET_LIST_LETTER 0x428`, `EVT_GB_MOGNET_LIST_SENDER 0x437`; expression op names
  `EVT_EXPR_PAD_PRESSED 0x4F`, `EVT_EXPR_SYSVAR 0x7A`, `EVT_EXPR_CONST16 0x7D`, `EVT_EXPR_CONST26 0x7E`,
  `EVT_EXPR_OBJPROP 0x78`, `EVT_EXPR_END 0x7F`.
- confidence: high for the choice/confirm mechanics, the carried-slot and bit layouts (script + runtime); medium for
  `b[0x408]`'s purpose (only incremented on Give and printed by field 3100) and for the auto-letter / hand-out
  condition tables (static extraction, one runtime case).

## R35 3DS models: runtime checks  (status: answered; doc docs/r32/3ds-requests-5-models.md "Runtime checks")
- binary: ovl_0b (btl_setUnitBrightness, btl_applyBgLight, btl_initBattleState, btl_calcBoneMatrices,
  btl_uploadModelTextures), main (chr_calcAppearance), field event objects
- evidence (scratch scripts in build/r35rt/ of the R35 worktree):
  - colour: `load disc3_malboro_lv99_calm` → FT4/FT3 source rgb and packets of every party body/weapon = 0x62627E with
    `g_btlbgHeader` light 0x707090 (= × 28/32). `enter_battle.py 899 --field 2714 --base-state
    disc3_pandemonium_lv99_field --party 0,1,2,6 --frames 60`, then `trace btl_setUnitBrightness btl_applyBgLight
    --frames 900`: levels 0..0x1C step 4 then 0x20 (no-op), applyBgLight only from btl_initWeapon (4 calls); rgb
    afterwards still 0x62627E. `r30b_enc21_start` (disc 1): light 0x708898, rgb 0x627785.
  - Garnet: battle_pose renders of 281/283/288/287 and field 185/76 (long: 281, 283, 185; short: 288, 287, 76);
    memory card saves parsed (story u16 at save block +448, characters at +2512): card 2 Garnet kind 4 at story
    6960, kind 5 at 10980 and 11770; card 1 Lv 99 saves kind 3/4 at every story (stale/edited).
    Poke kind 5 (+ darkBattle 1) in `disc3_pandemonium_lv99_field`, `enter_battle.py 899 --party 0,2,3,6`: Garnet unit
    model 288, Steiner kind 8, model 286.
  - field anim: `field_dali_inn` Zidane (uid 19) anim/stand 0xC8, walk 0x19, run 0x26, idle 0x37; `press
    left+circle --hold 30` → anim 0x19, `press left --hold 40` → 0x26.
  - weapon: weapon model +0x74 attach (0,0,0), +4 parentBone = Character.weaponBone, bones[0] == body
    bones[parentBone] (Pandemonium battle and r30b_enc21_start).
  - VRAM: `lua -f tools/rt/vram.lua`, `DumpVram`: weapon TIM rows found at (0xC0 + 64·slot, 0x1D0); RAM uvs v 208-255,
    file uvs v − 0x80.
- names: the R35 type renames (BtlModel, BtlModelHeader, BtlMotion, BtlTexWord, ModelTexWord/ModelTexPos,
  FieldLightSet, FieldModelLight, one WeaponEntry) and EVT_SYSVAR_* are applied in the R35 commits.
- confidence: high for 1, 3, 5, 6 and the long/short split of 2; medium for "10300 = the haircut" (bracketed by
  saves at 6960 and 10980 only); high for Zidane in 4 (other characters' default animIds not checked).

## R37 world map constants (2026-11-01)  (status: **applied** in the R37 commit, ovl_0c.h WM_WEATHER_*, WM_TOPO_*, WM_FX_*, WM_BLOCKFX_*, WM_CAM_*)
Method: Redux disc 3, states `cpass_wm_hadris` (walker, story 0x28A0, world data set 1) and `cpass_wm_airship9` (Hilda
Garde 3 in flight). Scratch scripts in `build/r37/` (worktree agent-a812c85200c396bb2): `wmlib.py` (player = `*0x800CE3DC`,
actorType = obj+17, topography/area from `g_worldActorTypeInfo[type].cellAttr`, active effect list from
`g_worldEffectActive`, teleport = write obj+20/+28, `noenc()` = EvtState+0x16EF encounterRate and g_evtEncounterChance
to 0 before every step), `tp.py`/`walk.py` (teleport, walk, sample, screenshot), `ablate_fx.py` (same input twice from a
state, the second time with wm_drawEffects patched to `jr ra`; image diff = what the effects draw), `force_weather`
(break at wm_setWeather after `g_worldWeatherAutoMode = -1`, set a0 = mode, a2 = 0x1000), `rectopo.lua` + `sweep.py`
(per-vsync topography log while the airship flies; 287 changes recorded in `build/r37/topo.csv`; a second sweep hung
Redux with "Unknown error in event listener", killed). New states: `r37_wm_sand`, `r37_wm_shore`, `r37_wm_air_sand`,
`r37_wm_waterfall`, `r37_wm_gulug`, `r37_wm_gulug2` (disc 3).

- **wm_setWeather mode 0-3 → WM_WEATHER_CLEAR / SUNSET / DARK / DARK_PURPLE. CONFIRMED.** Forced each mode at the
  Hadris basin start (`cpass_wm_hadris`, `wx.py`): 0 normal daylight, 1 orange sky and orange-lit ground (sunset),
  2 dark blue night-like light, 3 purple-lit ground and dark blue sea. `wm_getWeatherByPos` gives 1 within 0x56 of
  (0x508, -0x3AD) << 8 and within 0xB4 of (0x1F4, -0x316) << 8, 2 within 0x3B of (0x50B, -0x3A0) << 8 (inside the first
  circle), 3 within 0x65 of (0x2FE, -0x140) << 8 only with data set 4; teleporting to (0x1F400, -0x31600) gave auto mode
  1, to (0x50B00, -0x3A000) mode 2, 0x4C00 further out mode 1. wm_isDarkWeather (darkBattle) = modes 2 and 3. high.
- **Topography 0x24 (and 0x25, 0x26) → WM_TOPO_FOREST(_2/_3). CONFIRMED for 0x24.** Teleported the walker to
  (309727, -161838): topography 0x24, Zidane sinks to his shoulders among dark-green trees, effect type 16 spawns two
  every other frame (`t24c.png`: green leaves around him). 0x25/0x26 not stood on; the code treats 0x24-0x26 alike
  everywhere (wm_getTerrainHeightOffset class 0 = 400 for the walker class, wm_findGetoffPos, the effect, the event
  battle parameter). high (0x24), medium (0x25/0x26 by code).
- **Effect type 16 → WM_FX_FOREST_LEAVES. CONFIRMED** (above). high.
- **Topography 0x22 → WM_TOPO_BEACH, effect 7 → WM_FX_BEACH_SPLASH. CONFIRMED (medium).** (331345, -82379): grey sand at
  the water's edge, y about 0 (sea level); walking spawns type 7 every 4 frames, seen as a few light-blue specks at the
  feet while in the surf (`t22c.png`); in `r37_wm_shore` the ablation diff is a 4x4 speck. 0x1E (also y 0, next to the
  sea) and 0x23 spawn the same effect but were not looked at, so they stay numbers. 0x20 is also grey beach sand next
  to water (`t26b.png`) but no code tests it. medium.
- **Topography 0x29 → WM_TOPO_SAND, effect 27 → WM_FX_SAND_DUST. CONFIRMED (medium).** (301500, -58000) area 39: light
  tan sandy plain; walking spawns type 27 every 4 frames; ablation in `r37_wm_sand` shows a faint pale puff behind the
  walker (diff bbox 11x11). The airship forced to y -1500 over it spawns type 19 (code: also the flying gold chocobo
  spawns 14 there); 14/18/19 share one draw case, so 19 = WM_FX_AIRSHIP_SAND and 14 = WM_FX_CHOCOBO_SAND by spawn
  site; they were alive but behind the camera, not seen. 6 = WM_FX_CHOCOBO_SPRAY by the same rule as
  WM_FX_AIRSHIP_SPRAY (code). medium.
- **Topography 0x31 → WM_TOPO_MOUNTAIN. CONFIRMED.** Airship over (231369, -78456): a large brown mountain, the airship
  climbs from -2700 to -3682 over it; data: the Mountain chocobo (WM_MOVE_CHOCOBO_MOUNTAIN) adds 0x31 and 0x32 to the
  allowed cells. In the sweep 0x31 is the commonest land value next to 0x3A (fly-only peaks). high.
- **Topography 0x34 → WM_TOPO_NO_ENCOUNTER (code).** wm_movePlayer counts no step on it (no encounter roll), wm_frameMain
  drops a script battle while the player stands on it, wm_findGetoffPos refuses it, the ground lookup skips the poly
  cache. Not stood on. medium (code).
- **Topography 0x35 → WM_TOPO_SHALLOWS (code + data).** wm_findGetoffPos lets the boat be left only from 0x35; it is
  boat water and Reef-chocobo water; the smallest water sink offset (150). In the sweep it is the commonest
  value of area 0 next to the coasts. medium.
- **Effect type 20 → WM_FX_WATERFALL_MIST, wm_updateBlockFx 5 → WM_BLOCKFX_WATERFALLS. CONFIRMED.** Block 5's emitter
  table (data pack entry 0x2D, `blockfx.py`) emits type 20 at six points around (321000, -950, -184000), always on.
  `r37_wm_waterfall` (player at (322000, -192000)): two waterfalls ahead; with effects drawn there is white spray at the
  foot of each fall, with wm_drawEffects patched out none (`wf_on_c.png` / `wf_off_c.png`). high.
- **wm_updateBlockFx 10 → WM_BLOCKFX_WATER_SHRINE (string/code).** On only while g_worldFormFlags[WM_FORM_HOKORA_MIZU]
  ("HOKORA(MIZU)", water shrine) is set; its table emits type 29 around (57000, 1150, -155500). Not seen.
- **Effect 10 → WM_FX_BOAT_SIDE_WAKE (code).** wm_controlSteer spawns two beside WM_FX_BOAT_WAKE on odd frames. No boat
  state. medium.
- **wm_cameraFollow dir → WM_CAM_HOLD / LOWER / RAISE (code).** 1 when the wanted height is more than the dead zone
  larger (y grows downward: airship heights are negative, its spray needs y >= -0x800), 2 when smaller. high.
- Not named: effect types 2/3 (block 2 at Mount Gulug's summit (129057, -2400, -29156) with type 3, rising, growing,
  subtractive blend: probably smoke, never on screen in `r37_wm_gulug*`), 4, 8, 17, 23-26, 29, 32; block fx 2, 3, 4,
  6, 7; topographies 3/0x12/0x15/0x16/0x1C (chocobo call cells, wm_canCallChocobo; no chocobo track visited), 0x17,
  0x1E, 0x23, 0x2D/0x2E and the water depths 0x30/0x33/0x36-0x39/0x3D (depth order from g_worldTerrainHeightOffsets
  only). Snow: Mount Gulug's slopes are topography 0x1B/0x07/0x21 (`fx3.png`, `fx3b.png`), untested by code.

## R36: ATE fixture and end-of-disc-1 disc change  (status: reference; details in docs/r32/3ds-requests-6.md)
- binary: main / ovl_0a / ovl_3e; no new symbols (all functions on both paths already named).
- ATE: `tools/rt/enter_field.py 552 --base-state field_dali_inn --frames 900 --poke 0x8007AEF0=260c
  --poke 0x8007AEF2=0000 --poke 0x8007AFDC=0100` (story 3110, b[2..3] = 0, u16 b[0xEC] = 1) → "ACTIVE TIME EVENT"
  prompt; `press select` → ATE window "Small-Town Knight in a Big City / Cancel"; Cross, Cross → field 555 (Steiner).
  The standard init block tests only u16 b[0xEC] != 0 and s16 b[2] not 1/2, and starts the Select-handler object
  inside that branch, so b[0xEC] has to be set before the field init. high.
- Disc change: field 70 with s16 b[2] = 1 (story 3900) plays movie 0x11C and runs `AC 0x4320`. Hit
  `fld_requestDiscChange(1, 0x320)` from `evt_execInstr+0xA598` ~3800 frames in, then "DISC CHANGE / save?" prompt,
  then ovl_3e "Change to DISC 2". With disc 1 the check gives DISC_CHECK_WRONG_DISC (6), "INSERT CORRECT DISC" for 180
  frames, then the picture again. The same state under `--disc 2` reaches field 800 (Alexandria gate). high.
- Direct `menu_open(8, 0x4320)` from the field loop (disc 2, field 800): after a successful check
  `GameState.flags` stays `GS_RELOAD_FILE_TABLES` (0x100000), and `fld_exit` runs every ~140 frames (black screen). The
  field loop ends whenever the flag is set (fld_main.c:68), and in field mode only `fld_discChangeLeave` →
  `file_loadDirTables` clears it. high.
- States: disc 1 `r36_lindblum_552_ate`, `r36_lindblum_552_ate_window`, `r36_disc1_end_fmv`, `r36_disc1_discchange_op`,
  `r36_disc1_discchange_prompt`, `r36_disc1_change_to_disc2`; disc 2 `r36_x_change_to_disc2` (the disc-1 state, for
  the swap), `r36_disc2_field800_start`, `r36_x_direct_open_disc2`.

## Verification pass (code-only names), 2026-11-01  (status: proposed fixes below; nothing renamed)
Method: PCSX-Redux, disc 3 (`disc3_malboro_lv99_calm`, `disc3_pandemonium_lv99_field`, `cpass_wm_hadris`,
`cpass_wm_airship9`, `r37_wm_air_sand`) and disc 1 (`field_ice_300`, `field_dali_inn`, `field_d1_101`,
`ds_port_room_revealed`, `ds_port_light_candle_choice`, `ds_port_tantalus_greeting`, `field_dali_inn` → field 50).
Scratch in `build/verify/`: `run4.py` (= `build/rt3/run3.py` plus `--dedup f=v0`, output `build/verify/NAME.json`),
`f2.sh`/`f3.sh` (cast via `--patch`, optionally rewrite btl_calcCmd's a3 = formula, dump the BtlCalc at
btl_applyCalcResult: +0xE power, +0x10 defense, +0x14 multiplier, +0x18 flags, +0x1A/+0x1B popup bytes, +0x1C caster
and +0x1E target HP change), `formscan.py` (enemy ability records of every encounter, all discs: header {?, groups,
types, abilities}, 8 + groups * 0x38 + types * 0x74, then 16-byte BtlCmd8), `abtab.py` (ovl_33 action table),
`kinds.py`/`kind2.sh` (Character.info.kind; GameData.characters at +0x3C, info at +0x34), `wmsweep.py`/`topofind.py`/
`wmhunt.py` (world topography by flight and by scanning the loaded WmBlock polygons), `msgdump.py` (message table with
header: entry u16 +0 = start incl. header codes, u16 +4 = text), `msgwatch.py`/`winwatch.py` (MsgWindow state per
frame). BtlUnit: +0x24 max, +0x28 hp (the "calm" HP poke at 0x80109264 hits hp, not atb). No save states created.

### 1. BTL_FORMULA_* (ovl_0b btl_calcCmd)  — 21 CONFIRMED at runtime, 0 WRONG
Static check first: the parenthesised users in `ff9_ids.h` match `s_menuAbilityActionTable` (`abtab.py`) for every
player ability, and the enemy records (`formscan.py`) use 0x08 (1813 records: Strike, Tail, Counter...), 0x58 Melt /
Blowup, 0x59 HP Switching (Wyerd), 0x5B Cannon (Armstrong), 0x5D Sandstorm / Maelstrom, 0x5E Absorb Magic and 0x5F
Absorb Strength (Tiamat), 0x60 Trance Full / Transfer, 0x61 Entice (Lamia), 0x63 Flare Star (Ozma, Kuja), 0x64 enemy
attacks, 0x65 Steal (rate = item 236..), 0x66 Mug, 0x67 Shell/Protect/Haste/Reflect, 0x68 Everyone's Grudge
(Tonberry), 0x69 Grand Cross (Necron), 0x6A Snort (Yan) / Swallow, 0x6D Mini / Ultra Sound Wave. Every enemy name fits
its formula. **0x5A, 0x5C, 0x62, 0x6B have no record in any encounter of any disc** (no user at all; the header's
"0x58-0x6B are enemy abilities'" is too broad for these four).
Runtime (Zidane's command rewritten with `--patch`, `disc3_malboro_lv99_calm`, Malboro max HP 11687, def/mdef 10):
- 0x01 WEAPON_STR: Attack, power 71 - def 10 = 61 x mult 53 = 3233 (the "3233" seen before). CONFIRMED.
- 0x09 MAGIC: Fire (14) vs mdef 10, x 109 = 436. CONFIRMED.
- 0x0A MAGIC_HEAL: Cure on Zidane: 16 x 109 = 1744 healed, no defense, popup byte 3 (heal). CONFIRMED.
- 0x10 DRAIN: Drain (32 - 10) x 106 = 2332 to both casterHpChange and targetHpChange. CONFIRMED.
- 0x11 PERCENT_MAX_HP: Demi 30 -> 3506 = 30% of max 11687, also with the Malboro's hp poked to 5000. CONFIRMED.
- 0x5B PERCENT_CUR_HP (a3 rewritten): same Demi with hp 5000 -> 1500. CONFIRMED (and the pair is told apart).
- 0x1A FIXED_DAMAGE: 1,000 Needles -> 1000. CONFIRMED.
- 0x1D HP_LOST: Minus Strike with Zidane hp 1000 / max 9999 -> 8999. CONFIRMED.
- 0x45 ITEM_HEAL: Potion -> 150 (power 10 x 15, no Chemist). CONFIRMED. 0x4D 9999_DAMAGE: Dark Matter -> 9999. CONFIRMED.
- 0x08 PHYSICAL: forced Malboro "Virus Tentacles" (power 66) via btl_setEnemyCmd a3 = 2: (66 - 10) x 40 = 2240;
  vs Zidane (evade 99) flags MISS. CONFIRMED.
- 0x58 SELF_DESTRUCT (a3 rewritten, Zidane hp 1234): target takes 1234, Zidane hp 0. CONFIRMED.
- 0x59 SWAP_HP: Zidane 1234 <-> Malboro: Zidane 9999 (capped at max), Malboro 1234. CONFIRMED.
- 0x5D LOW_HP_STATUS: Malboro hp -> 4, flags 0x80 (NO_DAMAGE). CONFIRMED.
- 0x5E ABSORB_MAGIC: Zidane magic 0x63 -> 0xC6, Malboro 0x15 -> 0x0A. CONFIRMED.
- 0x61 STATUS_NOT_FEMALE (Sleep's status set): on Dagger flags MISS, on Vivi not. CONFIRMED.
- 0x68 EVT_POWER_DAMAGE: g_evtGlobalVars.b[0xC0] (0x8007AFB0) = 5 -> 16 damage. CONFIRMED (Everyone's Grudge).
- 0x6A REMOVE_UNIT: on Vivi: Vivi gone from field and HUD (`build/rt3/v_remove_00.png`). CONFIRMED.
- 0x0B ADD_STATUS (Sleep, no effect on the Malboro), 0x3A STEAL (btl_calcSteal message 0x0C), 0x38 FLEE (Flee
  command): case reached with the right ability. CONFIRMED (identity).
- Comment nit only: 0x38's comment says "btl_tryEscape"; its case calls btl_canFlee + btl_rollFlee (btl_tryEscape is
  0x39's). Not a name problem.

### 2. CHR_KIND_* — 5 CONFIRMED (model / weapon), 5 more consistent by character id, 0 WRONG
`kinds.py` on the disc-3 save: Zidane kind 0 with Orichalcon (dagger icon) = ZIDANE_DAGGER; Vivi 2; Eiko 11 with
Mythril Racket = EIKO_RACKET; Steiner 7, Freya 12, Quina 9, Amarant 13, Beatrix (slot 8) 18 match their ids.
GARNET_LONG / GARNET_SHORT: `kind2.sh` re-entered field 2714 with Dagger's kind poked, then battle 899: kind 3 gives
unit modelId 0x119 and a waist-long black hair model, kind 5 modelId 0x120 and short hair (`build/verify/k2_crop.png`).
CONFIRMED. (Poking the kind after the field has loaded changes modelId but not the drawn model: the field preloads
the battle party models.) Side note: this disc-3 save (story 10980) stores Dagger as kind 4 (LONG_RACKET), i.e. the
kind is only recomputed by chr_calcAppearance on equip/join, so a save made around the haircut can keep the old kind.

### 3. TEXT_* control codes — 15 CONFIRMED, 1 comment WRONG
Messages forced by rewriting the id in evt_getMsgLayout (a1) and msg_open (a0) on the next natural message of
`ds_port_tantalus_greeting`; clearing the balloon flag (evt_getMsgLayout a2, msg_open a3) for the fixed positions.
- TEXT_WIN_CENTER 5C (msg 4 "Received ..!"): x 104 = 160 - 113/2, y 50 = 74 - 48/2. CONFIRMED.
- TEXT_WIN_BOTTOM_RIGHT 50 (msg 12 "Skip FMV"): x 249 = 308 - 59, y 188 = 208 - 20. CONFIRMED.
- TEXT_WIN_TOP 55 (msg 37): x 37 = 160 - 246/2, y 12. TEXT_WIN_TOP_LEFT 53 (msg 39): (12, 12). CONFIRMED.
- TEXT_WIN_FIXED_TOP_RIGHT 5A (msg 53) without balloon: position left as the layout had it (96, 22). CONFIRMED.
- Balloon side: msg 39 as a balloon gets tailMode 3 (= 0x53 & 3). CONFIRMED.
- TEXT_MSG_ITEM_NAME 0E with g_msgNumbers[0] = 0xEC: "Received Potion!", Potion in yellow (`v_item_grid.png`). CONFIRMED.
- TEXT_CHAR_NAME+0 (10): "Hey, Zidane!" (msg 42). CONFIRMED.
- TEXT_CHOICES 07 (msg 36 header 07 02 02): g_msgChoiceCount 2, g_msgChoiceCancel 1 (Cancel). CONFIRMED.
- TEXT_LINE_WIDTHS 0D (msg 36: choice 1 width 108, choice 2 width 55): window textWidth 117 from the wider. CONFIRMED.
- TEXT_TAB 71: both choice lines start with it and are indented past the hand cursor. CONFIRMED.
- TEXT_SKIP 0C: msg 4 (starts with 0C) is complete on its first frame; msg 44 (no 0C) types 2 chars per frame over 17
  frames. CONFIRMED.
- TEXT_WAIT_CLOSE 01 (msg 39, `01 14`): window flags gain MSGW_NO_SKIP, closes 40 frames after opening with no input.
  CONFIRMED.
- TEXT_SPEED 02 / TEXT_SPEED_DEFAULT 42 (msg 34 "Sure is dark...", field 50): write watch on MsgWindow.speed: msg_open,
  then msg_advanceText+0x178 (case 02) and +0x1AC (case 42). CONFIRMED.
- TEXT_CHOICE_START 08: name fine, **comment WRONG**: msg_advanceText sets g_msgChoiceLine = g_msgLine, the line that
  holds the code, not the next line. msg 36 puts 08 at the start of line 0 and line 0 is the first choice
  (g_msgChoiceLine read 0 in `ds_port_light_candle_choice`). Fix the comment in ff9.h and docs/text-codes.md: "the
  choice lines start at the current line".
- Format note for docs/text-codes.md: a message table entry holds two offsets (u16 +0 = start of the header codes,
  u16 +4 = start of the text, read by msg_getText / msg_open), and messages ended by TEXT_HOLD or closed by the script
  have no TEXT_END, the next message's header follows directly.

### 4. World map (R37 medium names) — 6 CONFIRMED, 1 INCONCLUSIVE
- WM_TOPO_BEACH 0x22: airship flight from `cpass_wm_airship9` (`wmsweep.py`): 0x22 cells at the grey sand rim between
  turquoise water (0x35) and land at (272204, -93321). CONFIRMED.
- WM_TOPO_SHALLOWS 0x35: the airship's start cell; on screen the light turquoise strip along the coast
  (`airship9.png`), deeper sea is 0x36-0x39. CONFIRMED.
- WM_TOPO_NO_ENCOUNTER 0x34: **INCONCLUSIVE**. No cell with topography 0x34 was found: 1500-sample flight plus two
  `wmhunt.py` flights scanning every loaded block's polygons (~300 000 polygon samples, 34 different topographies,
  incl. 0x25/0x26 forest, 0x2D/0x2E, 0x3E). 0x34 may not occur in the disc-3 world data; keep the code name.
- WM_FX_BOAT_SIDE_WAKE 10: `cpass_wm_airship9` with g_worldMoveMode 7 (0x800CE3C4 = 7, g_worldMoveModeInfo =
  0x800CCC34, b[0xBE] = 7) runs wm_controlSteer: spawns 15 and 10 (two 10s every other spawn). Moving only the type-10
  spawns off-screen (`--fn` a1 = -30000) removes bright white-blue spray puffs in a line beside the vessel; moving the
  15s removes only a faint flat trail (`v_boat_sep.png`). CONFIRMED.
- WM_FX_AIRSHIP_SAND 19: `r37_wm_air_sand` with the airship teleported to (301500, -1500, -58000) (sand): 6 spawns of
  19; removing them removes a pale sand-coloured haze under and behind the airship (`v_as_zoom.png`). CONFIRMED.
- WM_FX_CHOCOBO_SAND 14 / WM_FX_CHOCOBO_SPRAY 6: same flight with the player's actorType poked to 7 (gold chocobo):
  14 over the sand (same pale haze), 6 over the water (a small light blue-white speck). CONFIRMED (6 medium: tiny).
- WM_FX_BEACH_SPLASH 7, WM_FX_SAND_DUST 27, WM_TOPO_SAND 0x29: R37's runtime evidence stands, not re-run.

### 5. EVT_UPD_*, EVT_GETPROP_*, SND_LOAD_* — 13 CONFIRMED (3 EVT_UPD, 4 GETPROP, 6 SND_LOAD), rest not reached
Breakpoint on the instruction after `jal evt_update` (field fld_main 0x800A7C54, world 0x800B846C), logging v0 changes:
- EVT_UPD_WORLD 5: `field_ice_300`, hold Down: 5 at +81, the world map loads. CONFIRMED.
- EVT_UPD_FIELD 4: walking out of `field_dali_inn` (Up, Left) and `field_d1_101` (Left, Right): 4, then the next field.
  CONFIRMED.
- EVT_UPD_BATTLE 3: `ds_port_room_revealed` with Cross every 20 frames: 3 at +834, encounter SEs 0x27C/B/A at +843
  (Baku battle); world map `cpass_wm_hadris` random encounter: 3, then the same SEs 3 frames later. CONFIRMED.
- EVT_UPD_END_FRAME / CARD_GAME / GAME_OVER: not reached.
- EVT_GETPROP_X/Y/Z/ROT: world map walk: props 0-3 on the player object, which carries pos at +0x14/+0x18/+0x1C and
  rotY at +0x20 (the R37 teleport offsets). CONFIRMED. In battle only unit attributes are asked (props 35, 36, 48, 51,
  69), and EVT_SETPROP is only used with 48 and 51 there; FLAGS/UID/LEVEL/ANIM_FRAME and EVT_SETPROP_ROT/INSERT not
  reached.
- SND_LOAD_*: write watch on GameData.soundLoadState (0x8008BAA1) through the world-map encounter: wm_soundCommand
  writes 1, 2, 3, 4 (76 frames later), 5, then 0; the battle music load (0x700, id 0) sits between. Order and timing
  match WAIT_FADE, START, BANK, SEQ_START, SEQ, IDLE. CONFIRMED. The victory fanfare (music 5) loads with no state
  change (the id-5 shortcut). CONFIRMED.

### 6. Round 33 medium names, BTLFX_FIELD/LAUNCH/FALLOFF — 1 WRONG, 1 INCONCLUSIVE, rest confirmed by code
- **fxmod `stop0`: WRONG meaning.** fx_046 (Fire): poking work+0 = 1 from the op's mode-2 call (`--fn` on 0x801E7704)
  makes the op run 165 times instead of 63: with stop0 != 0 the op skips its frame logic and its own `return 1` at frame
  0x3C, so the task stays alive (until the script's effect ends) instead of stopping. In fx_0de the same field makes
  the op return 0 at once. So nonzero = "hold / idle, never finish by itself", never set by anything. Proposed
  `hold0` (comment: "nonzero: the op idles and never ends by itself; always 0").
- fx_0e7 `fadeInTimer68`: INCONCLUSIVE (name speculative). R33's "stays -1" holds; no code decrements it: when >= 0 it
  only switches both models to blend mode 1 with the current rgb3C (op81). It is not a countdown in this module. A
  neutral name such as `blend68` (">= 0: draw the models blended; never set") fits the code better.
- fx_059 `depthZ4` (Haste): code reading confirms (view-space z of the spiral head, -0x20 + 0xC8 per frame); no new run.
- fx_105 `windStreaks` / `stars`, fx_0ba `preOrb`: R33's ablation and read-watch evidence stand; not re-run.
- BTLFX_FIELD_* / BTLFX_FALLOFF_*: the C (btlfx_updateParticles, byte-matching) does exactly what each name says
  (REPEL pos - fieldPos, ATTRACT the reverse, UP v.vy = -0x1000, FADE_* only change alpha). Runtime: none of Fire,
  Thundaga, Cure, Haste, Bio, Water, Flare, Holy reaches the field switch (0 calls to btlfx_distance /
  btlfx_toLocalPolar from it; return addresses 0x800E0AA8.. mapped with objdump), so no effect was watched. Code-only,
  names fine. BTLFX_LAUNCH_*: not observed, code-only.

### 7. SND_TYPE_ / SND_CMD_ / SND_PARAM_ — 14 CONFIRMED (decoded fields fit what the handler does; audio not listened to)
Traces of fld_soundCommand, wm_soundCommand, btl_soundCmd and the akao_* calls:
- SE play `0xD000` (type 1, PLAY): Malboro battle attack SEs -> akao_playSe(handle, a2, a3, 0x7F); encounter SEs
  0x27A-0x27C on field and world; field footsteps. SND_TYPE_SE, SND_CMD_PLAY CONFIRMED.
- `0x9203` (SE, SET, PITCH) after each `field_ice_300` footstep, value -6..+1: SND_CMD_SET + SND_PARAM_PITCH CONFIRMED.
- `0x9601` (SE, FADE_ALL, VOLUME, 0x30 frames, 0) then `0x5501` (SE, SET_ALL, VOLUME, 0) when leaving for the world
  map; `0x5180` (SE STOP, SND_RESIDENT) id 0xC22: SND_CMD_FADE_ALL, SET_ALL, STOP, SND_RESIDENT CONFIRMED.
- `0x2100` (STREAM, STOP) on the same exit; `0xA000` id 8 (STREAM, PLAY) and `0x6201` (STREAM, SET, VOLUME 100) in the
  opening: SND_TYPE_STREAM CONFIRMED.
- Encounter: `0x800` music SUSPEND (field song 0x88 / world 0x45), `0x700` music LOAD id 0, `0x18C0`/`0x1880` SE
  SUSPEND, then `0x0` music PLAY: SND_CMD_SUSPEND, LOAD, PLAY, SND_TYPE_MUSIC CONFIRMED.
- Victory: `0x8601` (music FADE_ALL VOLUME, 30 frames, 0) -> akao_fadeMusicVolume(0, 0x1E, 0); `0x700` id 5 ->
  akao_stopMusic(old handle); `0x0` id 5 -> akao_playMusic: CONFIRMED.
- `0xC00` GET_STATE polled every frame while soundLoadState != 0. CONFIRMED.
- Not seen: SND_TYPE_BANK, FADE, FADE_FROM, RESUME, SET_CONDITION, FADE_ALL_FROM, PARAM_TEMPO/TEMPO_PITCH/PAN/
  CONDITION/PLAY_COUNT/ALL_SLOTS/MOVIE_VOLUME. Bits 14-15 (0xC000) are set in every SE play word; no handler reads
  them (they are outside the documented fields).

### WRONG / to fix
| item | now | finding | proposed |
|---|---|---|---|
| fxmod work +0 (22 modules) | `stop0` | nonzero makes the op idle and never finish itself (task kept alive); always 0 | `hold0` |
| `TEXT_CHOICE_START` comment (ff9.h, docs/text-codes.md) | "choice lines start at the next line" | g_msgChoiceLine = current g_msgLine | "start at the current line" |
| `BTL_FORMULA_*` header note (ff9_ids.h) | "0x58-0x6B are enemy abilities'" | 0x5A, 0x5C, 0x62, 0x6B have no user in any encounter (all discs) | say "unused" for these four |
| `BTL_FORMULA_FLEE` comment | "btl_tryEscape" | case calls btl_canFlee + btl_rollFlee | "btl_rollFlee" |
| fx_0e7 work +0x68 | `fadeInTimer68` | never decremented; >= 0 only enables blend mode 1 | `blend68` (or keep, low confidence) |
| WM_TOPO_NO_ENCOUNTER 0x34 | code name | no 0x34 cell found on disc 3 | keep, INCONCLUSIVE |

## R37b: world-map vehicles, chocobo digging, Hot and Cold, Moogle choice, Blackjack  (status: reference; details in docs/r32/3ds-requests-7.md)
- binary: ovl_0c / ovl_15 / main; no new function names. Method: Redux discs 3 and 4; world maps entered with the scratch
  `build/3ds7/wm_force.py WORLD` (field exit to main's dispatcher, then GameData sceneSector[1]/sceneSectors[1],
  **GameData.worldId (+0x7DC) = WORLD, worldAreaId = 68**, pokes, GameState.mode 3). `wm_load` looks the world script
  up by worldId: without that write only the save's own world loaded and other files ran with no script (object uid 0,
  ip 0); this explains R30b's stalled `wm_enter.py --world 9002`.
- **g_worldMogChoice (0x800CF440, WM_REQ_MOG_CHOICE 35): CONFIRMED "a world-script choice window is open".** It is 1
  while the moogle's "Can I help you, kupo? Save / Tent / Cancel" window is up (walking, Square), and also during the
  chocobo Triangle window ("Open Chocograph menu / Feed him a Dead Pepper / Cancel"), the airship "Go to <place>? Yes /
  No" prompt (full navi map, Cross on a place, move modes 8/9 only) and the navi "Select navigation method" window
  (full map, Triangle); 0 after each closes. wm_frameMain passes it to pad_setFlags(0x8000, busy). Keep the name,
  widen the comment. high.
- Chocobo dig: GameData.chocograph (+0x844) = 1, b[0xB8] bit 0 clear, riding (b[0xBE] = 5), world 9007 entered at
  (264058, -255513) (b[2] = 1, b[0x53..0x5B]): Square → "K-KWEHHH!!!", "Found a treasure chest!", 2 Elixir, 3 Hi-Potion,
  4 Ether, 2 Germinas Boots. The 24 dig spots and radii are in the world chocobo entry 5 tag 14 (table in the R37 doc).
  The chocograph picture above the minimap is loaded once at world init (tu_800BA528.c:85). high.
- Vehicles at entry: b[0xBE] 1-5 rides the chocobo (9007), 7 the Blue Narciss (9003, story >= 9400), 8 the Hilda Garde 3
  (9007, story >= 10400), 9 the Invincible (9008, story >= 11100; works on disc 4, froze in the world init on disc 3).
  With b[2..3] = 0 the world scripts put walker/vehicle/chocobo at their default spots; b[0x40]/[0x4A]/[0x53] hold the
  stored walker/vehicle/chocobo positions (x s24, y s16, z s24, dir u8). The boat has no UI of its own (no "Go to"
  prompt; Triangle opens the main menu). high.
- Hot and Cold: field 2950 with b[2..3] = 2 and b[0xBE] = 5 (plus Gbit 0x58F, 0x32A) starts the minigame: How to play
  page, Select → "1:00" timer box, Square → "Kweh." / "Kweh!?" hints. A debug "Error Env Play() Slot=0" box showed in
  every forced entry (environment sound not preloaded). The points window was not reached. medium-high.
- Blackjack: GameState.mode 4 on disc 4, ending_playMovie (0x800A7378) returned at once → THE END; code R2 L1 R2 R2 Up X
  Right O Down Triangle L2 R1 R2 L1 Square Square (g_endingCodePos 16) + hold Start → wager screen (Bankroll 1000,
  Wager 10) → Cross → Stand/Hit/Double/Split. The real trigger is field 3011 entry 7 GOTO_FIELD 16000. high.
- States: disc 3 `r37b_wm_chocobo_ride`, `r37b_wm_chocobo_choice`, `r37b_wm_dig_found`, `r37b_wm_chocograph1_spot`,
  `r37b_wm_hg3_entry`, `r37b_wm_airship_navi_full`, `r37b_wm_airship_navi_place`, `r37b_wm_airship_goto_prompt`,
  `r37b_wm_boat_entry`, `r37b_wm_moogle_choice`, `r37b_hotcold_howto`, `r37b_hotcold_running`, `r37b_forest_mene_back`;
  disc 4 `r37b_wm_invincible_entry`, `r37b_the_end`, `r37b_bj_wager`, `r37b_bj_action`.

## R38: field minigames (jump rope, Hippaul race, frogs, Canary sword fight, auction, Stellazzio, Hot and Cold)  (status: reference; details in docs/r32/3ds-requests-8.md)
- binary: field scripts (FF9.IMG dir 4) + main event system; no code symbols renamed.
- evidence (PCSX-Redux, `tools/rt/enter_field.py FIELD --disc N --poke ...`, then pad presses):
  - "Error Env Play() Slot=N" is message 0x44 of each area's text, opened by the field init script when
    b[0x0D] / b[0x0E] == 9; init sets 9 when the byte was 2 (env sound playing) and the new field's env sound id
    (b[0x09] / b[0x0B], s16) is < 0. Poking 0x8007AEFD = 0000 at the dispatcher removed the box in fields 64 and 2950.
  - Jump rope, disc 1 field 103 from `field_dali_inn`: Cross at the girl → "You wanna try?" → "0 times" window
    (msg 0xEA); auto-jumper (`build/3ds8/jumper.py`, press Cross while EvtState.fieldVars s16 +0x25 < 72) → "12
    times" + "It's gonna get faster after 20!"; no press → fieldVars +0x29 = 5, "Come play with us again!", top-3
    board b[0x1F0..0x1F8] 21/13/10 → 21/13/12. States `r38_f103_entry`, `r38_jumprope_ask`, `r38_jumprope_ready`,
    `r38_jumprope_counting`.
  - Canary, disc 1 field 64 (event.2 = 0): prompts "Press X!" over Blank; `build/3ds8/canary.py` presses the button
    for fieldVars +0x2E (0 Left 1 Right 2 Triangle 3 Down 4 Cross 5 Up 6 Circle 7 Square); results "Of the 100 nobles
    watching, 18 were impressed." = (fv s16 +0x1E + fv s16 +0x20) / 29 = (474 + 66) / 29. States `r38_canary_entry`,
    `r38_canary_howto`, `r38_canary_fighting`, `r38_canary_results`.
  - Frogs, disc 1 field 657 from `field_burmecia_vault` (Quina in party) with event.2 = 0x1060, b[0x18F] = 1,
    b[0x311..0x314] = 33 66 55 99: Quina's prompt → "Frogs Captured: 0" window, 8 frog actors (uids 0x80+). States
    `r38_frog_ask`, `r38_frog_running`, `r38_frog_pond1`.
  - Disc 2 (`field_iifa_tree`): field 909 auction runs on entry; Cross in region 16 → "Bid Status" + "Bid: 01300 Gil"
    windows (`r38_auction_entry`, `r38_auction_bid`). Field 911: Stella offers Libra (key item 0x36), Yes → b[0x163]
    = 0x40, key-item byte GameData+0x780 0x3C → 0x0C (`r38_stella_entry`, `r38_stella_choice`).
  - Disc 3 (`disc3_pandemonium_lv99_field`): field 1850 with story 7000 → Hippolady's race; `build/3ds8/mash.py`
    (alternate Circle/Square every 4 vsyncs) → "Vivi wins!", b[0x1EB] 44 → 45 (`r38_hippaul_entry`, `_ask`,
    `_howto`, `_race`). Field 2950 Hot and Cold without the error box (`r38_hotcold_entry`, `r38_hotcold_running`);
    a dig on treasure uid 0x87 animates but never reaches msg 0x12D; EvtState.playerUid became 4.
- proposed: `EVT_GB_ENV_SOUND0_STATE 0x0D` / `EVT_GB_ENV_SOUND1_STATE 0x0E` / `_ID 0x09` / `0x0B` (high), plus the
  minigame bytes listed at the end of docs/r32/3ds-requests-8.md.
- confidence: high for the recipes marked verified; Hot and Cold points window and the frog Keep prompt unverified.

## R40: auction bid variables, direct minigame entry, Quina's record  (status: reference; details in docs/r32/3ds-requests-10.md)
- binary: field scripts (FF9.IMG dir 4) + main event system; no code symbols renamed.
- evidence (PCSX-Redux; scratch `build/3ds10/inject.py` stops at evt_update's `jal evt_updateObjects` 0x8004BC64 and
  ORs a mask into g_msgPadPressed or redirects into `evt_callObjectFunc(obj, lv, tag, 0)` with ra = the jal):
  - Field 909 bid window = entry 10 tag 31: bid / 100 in the entry-10 object's local s16 +4 (0x80077F1C), digit
    0..2 in s8 +1 (0x80077F19), repeat counter s8 +0. From `r38_auction_bid`: poke 50 + Cross → price fieldVars +0x1B =
    5000, top bidder +0x39 = 0, balloon "5000 Gil!"; poke 5 → msg 0xCF; Circle → local = -1, no bid; poke 2000 →
    accepted with 127,972 Gil (the confirm has no funds check). Up with digit 0/2 → +1/+100.
  - Direct entry by the talk function: field 1850 `evt_callObjectFunc(uid 7, 1, 3, 0)` → Hippolady's dialog and the
    race (GO reached); field 909 `evt_callObjectFunc(region uid 16, 1, 3, 0)` once fieldVars +0x39 is 1..5 → bid
    windows from the default arrival point; field 103 `evt_callObjectFunc(uid 8, 1, 3, 0)` → "You wanna try?", but
    the player's scripted walk sticks at (-2054, 409) unless the player is first placed at (-2349, 59) the way op
    0x1D does it (ovl_0a bgi_setActorActive(slot, 0), pos/prevPos, bgi_setActorActive(slot, 1)); then "0 times".
    A raw x/z write without the bgi pair stops the actor (R38); with it the player walks normally.
  - Frog prompt (field 657) needs `evt_isInParty(5)`: a party member with info.id 5 and info.kind < 14. With
    `field_dali_inn`'s record 5 (Cinna: classFlags 0x15, kind 14) in party 0,1,2,5 → no prompt; classFlags 5, row 5,
    then `chr_join(5, 1, 5)` (= PARTY_JOIN 05 01 05 05 05) → kind 9, prompt, "Frogs Captured: 0" with Quina (model 273)
    as the player. b[0x13..0x14] bit k = record k initialised by its first PARTY_JOIN (fields 618/657/661 entry 0 tag 1).
- states: disc 1 `r40_f103_fresh`, `r40_jumprope_direct_ready`, `r40_frog_quina_ask`, `r40_frog_quina_running`;
  disc 2 `r40_auction_fresh_bidup`; disc 3 `r40_hippaul_fresh`, `r40_hippaul_howto_direct`.
- proposed: `EVT_GB_CHR_JOINED 0x13` (u16 bits, medium-high), `EVT_GB_SAVED_POS 0xF3` (x/y/z s16 + dir u8, read by
  field 1850 arrival 831; low).
- confidence: high (every recipe above verified from a fresh field entry, screenshots in build/3ds10/).

## F41: does Tetra Master read main's code range (0x800127A0-0x800665E0)?  (status: reference, ff9-ds docs/decomp-requests.md F41)
- binary: ovl_0d (Tetra Master) + main; no symbols renamed.
- question: the port keeps buffers in main's MIPS text range (psx_spare_alloc) and its host TM match diverges from frame
  ~5,190 when a 1 KB table shifts the buffer at 0x80013400 up. Does the PS1 read code bytes there (libgpu MoveImage ..
  PutDrawEnv, 0x80013410-0x800138A0) as data?
- answer: **no**. On the PS1 no game or library code makes a CPU data access (read or write) anywhere in
  0x800127A0-0x800665E0 during Tetra Master, and the GPU DMA sources (OT chains, DrawPrim and LoadImage pointers) never
  point into it. The only reads in the range come from the BIOS interrupt handler: at 0x00000CCC (`lw v0,0(v1)`, v1 = EPC)
  it fetches the interrupted instruction to check for a GTE command (bits 25-31 == 0x4A, then EPC += 4). Those reads land
  wherever VBlank interrupts the CPU (libgpu/libetc wait loops at 0x800135xx-0x800137xx), so they have nothing to do with
  game state, and the port doesn't run them. The F41 divergence is port-side: something in the port reaches the spare
  buffers through a pointer, an overrun of the neighbouring spare buffer, or render-dependent frame pacing that shifts
  which game frame the random presses land on. ovl_0d and main need no hook.
- evidence (PCSX-Redux disc 1, interpreter + debugger, `jit.off()`; scratch `build/f41/`):
  - `rd.lua`: one `PCSX.addBreakpoint(0x800127A0, 'Read', 0x53E40)` whose callback skips PCs below 0x10000 (BIOS) and
    logs every other PC once. `chunks.py STATE SEED N 100 read` loads a state, plays 100 random presses (U/D/L/R/X x3,
    hold 2, wait 13 = 1500 frames) per chunk, saves, and restarts Redux (long runs hung it once with the known "Unknown
    error in event listener").
    - `3ds4_tm_turn2`, seed 22, 5 chunks (7,500 frames: placements, enemy turns/AI, card battles with the digit rolls,
      match end, rematch, a second full match, back to Card Selection showing W/L/D 1/2/2): 0 game-code hits, ~6,800
      BIOS EPC hits per chunk.
    - `3ds4_tm_confirm`, seed 24, 3 chunks (deck confirm, coin toss, first turns): 0. `3ds4_tm_card_select` seeds 7/3
      (500 presses each, mostly Card Selection) and `field_dali_inn` + `tools/rt/enter_tm.py 12 300` (match setup from a
      field, deck build) seed 11: 0. Seed 3 also watched 0x000127A0 and 0xA00127A0; Redux matches breakpoints by physical
      address (the three ranges tripled the BIOS count), so KUSEG/KSEG1 mirror reads are covered.
    - Same watch with 'Write' (`3ds4_tm_card_select`, seed 5): 0 hits, BIOS included.
    - Positive control: the same callback on the TmState (0x801FF9E0, width 0x40), 10 presses -> dozens of ovl_0d PCs
      (0x800A8A58, 0x800B0344, 0x800AD6D8 byte reads, ...), so the filter does catch game reads of every width.
    - First break with the bridge (`addBp(0x80013400,'Read',0x400,'break')`): pc 0x00000CCC, ra 0x00000CC0, v1 =
      0x80013708 (= EPC, inside DrawOTag), stack frame of tm_mainLoop. BIOS 0xC90-0xCF8: k0 = TCB, save at/v0/v1/ra,
      `jal 0xEA0` (cause/EPC), `andi v0,0x3C; bnez` (exceptions skip), `lw v0,0(v1)`, `srl 24; andi 0xFE; li at,0x4A`.
  - `dma.lua` (exec breakpoints on LoadImage 0x80013350, DrawPrim 0x8001363C, DrawOTag 0x80013698; DrawOTag's callback
    walks the chain from a0 to the 0xFFFFFF terminator): `3ds4_tm_turn2` seed 23, 4 chunks = 6,000 DrawOTag calls,
    27.4 M OT nodes, no node and no source pointer in 0x800127A0-0x800665E0 (no LoadImage/DrawPrim calls during play).
- for the port: the PS1 bytes there never reach game logic, so serving the original code bytes would change nothing.
  Suggested port checks: which buffer sat at 0x80013400 (a table of 0x8000xxxx words), and whether the buffer below it
  writes past its end or a native reader indexes past it; whether the port's VBlanks per drawn frame (OT repeat/partial
  redraw from ot_bands hashes, ui_diff) depend on spare contents, which would move the random presses to other game frames
  (compare TmState.timer, 0x801FF9E0 + timer offset, at each press under both layouts); or poison the old location after
  the move (or mprotect it on the host) to catch the stale reader.
- proposed decomp change: none (no PSX_RAM / PSX_NULLABLE-style hook; there is no read site to wrap).
- state: disc 1 `f41_tm_midmatch` (1,500 frames after `3ds4_tm_turn2`, mid-match, battle on screen).
- confidence: high for the PS1 side (about 25,000 watched frames over the match phases the port's repro runs).

## F42: TmState fields read before Tetra Master initialises them (stack residue)  (status: applied 6efe5833; ff9-ds 7522c90 defines it as 0)
- binary: ovl_0d. Owner of the 0x5D0-byte local at 0x801FF9E0: **tm_mainLoop** (`addiu sp,-0x5F0`, `TmState tm` at
  sp+0x10; tm_main only has a 0x18 frame). No symbols renamed.
- question: which TmState fields does ovl_0d read before anything writes them, what does the PS1 hold there on
  different entry paths, and where does that residue come from?
- answer (static walk of tm_mainLoop → tm_setup → tm_taskInit → CHOICE ... plus a runtime check, below). Four fields, and only
  **animFrame** reaches anything observable:
  - `animFrame` (+0x480): **never assigned a start value** anywhere (only `tm.animFrame++` in tm_mainLoop and the
    `savedFrame = animFrame` / `animFrame = savedFrame` pair in PLAYER_SELECT/PLAYER_SET), so it runs on from
    the residue for the whole session, rematches included. Its only reader is tm_drawCursor: `u = ((animFrame / 6) & 7) * 24`
    (and the same with savedFrame), i.e. which of the 8 frames of the hand cursor's animation is drawn. It does not
    feed rand(), task logic, AI or the primitive count; on the PS1 it changes only the cursor sprite's U.
  - `timer` (+0x2E8): incremented from residue every frame, but no code reads it before some `timer = 0`
    (tm_taskChoice2 Yes → FIELD_FADE, tm_startEndFade, tm_taskTurn, ...): its CHOICE-phase readers (tm_drawStock's
    `timer & 0x20` blink, tm_taskCardAway2) are only reached after a trade, and tm_drawBackground/tm_drawBoard read
    it only in FIELD_FADE. Not observable.
  - `turn` (+0x434): read every frame by tm_mainLoop's two `tm_debugPrint` checks from the first frame until
    tm_taskEnemyAppear sets it (coin toss). tm_debugPrint is empty, so not observable.
  - `comboDirs` (+0x50C, u8): tm_drawArrows loads it for every visible card each frame, but uses the bits only when
    `blink` is set, i.e. in FLASH_ARROW for comboCard, a task that only tm_setFlips enters, and tm_setFlips writes
    comboDirs and comboCard first. Not observable.
  - Partially initialised but never read early: `cards[]` (tm_initHands leaves small/fromX/fromY/toX/toY unset, but
    visible = 0, and drawCards skips invisible cards), `board[][].card` (guarded by state), `targets[]` (numTargets),
    `lostCard` (hasLostCard), `flips[]` (tm_clearFlips), `landed[]`, `comboCard`, `pad*`.
- runtime: `build/f42/shadow.lua` (scratch) puts one Write and one Read breakpoint over 0x801FF9E0+0x5D0. Before
  tm_mainLoop's first instruction (exec bp 0x800A7E10, checked against the opcode 0x27BDFA10 and sp == 0x801FFFC0, since
  ovl_0a also runs code at that address) it records the last writer (pc, ra) of every byte. From then on it marks
  bytes as written and logs every read of a byte that hasn't been written (the `++` load/store pair 0x800A8A34-0x800A8A4C
  doesn't count as an initialising write). `build/f42/run.py` loads, optionally forces TM like enter_tm.py, picks 5
  cards (stockRow/stockCol pokes + Cross) and plays random presses (`jit.off()`, interpreter). Runs:
  1. `field_dali_inn` + forced opponent 12 (enter_tm path), 200 + 5 picks + 400 presses (one won game, rematch, second
     game into BATTLE_SELECT): uninitialised reads only at +0x434 (pc 0x800A8974, 243x), +0x480 (pc 0x800B1730 =
     tm_drawCursor, value 0x0045018F at the first draw), +0x50C (pc 0x800B1538 = tm_drawArrows, 2320x). Nothing else.
  2. **Real NPC path**: `3ds3_f354_counter` (Dali 354, at Shopkeeper Eve's counter), Square → "You want to play a card
     game?" Yes (state `f42_dali354_card_prompt` is that prompt), Cross through her lines → field exit → ovl_0d (opponent 12, AE 00 0C 00).
     Same three offsets, nothing else; one lost game, rematch, back to Card Selection.
  3. Second real entry in the same session: quit from Card Selection (Square, Up, Cross; state `f42_dali354_after_quit`
     back in the field), talk again, Yes.
- values at tm_mainLoop entry (s32, little-endian residue):

  | path | animFrame +0x480 | last writer | timer +0x2E8 | turn +0x434 | comboDirs +0x50C |
  |---|---|---|---|---|---|
  | enter_tm 12 from `field_dali_inn` | 0x00450042 | none since the state was made (the inn draws no shadows at this depth) | 0x7FE4DAA1 | 2 | 0x5C |
  | NPC (Dali 354 Eve), 1st | 0x0000FFD6 | ovl_0a fld_drawShadow 0x800ACC14 / 0x800ACC54 | 0xFFC300B6 | 2 | 0x5C |
  | NPC, 2nd entry after quitting | 0x0000001D | same | 0x00000020 | 2 | 0x5C |

  The `3ds4_tm_*` states (enter_tm from `field_dali_inn`) all inherit 0x00450042 (`3ds4_tm_card_select` holds
  0x00450091, `3ds4_tm_rematch` 0x00450605, i.e. +frames). Reading 0x801FFE60 in ~100 disc 1 states gave values
  across the whole range (0, 2, 0xFF, 0xFFxx, 0x0044001D, 0x8001F33C, 0xFFFFE86F, ...), so **it is not constant**.
- stack history: the field main loop runs at the same depth (game_main's dispatcher calls the mode's main from a fixed sp),
  and TmState+0x480..+0x487 is ovl_0a `fld_drawShadow`'s local `MATRIX mtx`, first row: the `sh` stores at 0x800ACC14
  (+0x480 = m[0][0]), 0x800ACC54 (+0x482 = m[0][1]), 0x800ACC94 (+0x484), 0x800ACC18 (+0x486 = m[1][0]) are
  gte_MulMatrix0(viewMatrix, shadowMtx) writing its result. So animFrame starts as `(u16)m[0][0] | m[0][1] << 16` of the
  **last character shadow the field drew before it exited** (m[0][1] was 0 both times): it depends on the camera and the
  last shadowed actor's rotation and changes from frame to frame (0xFFD6 at the first entry, 0xFFD7 one dialogue later
  from the same state). If the field draws no shadows (Dali inn), the value is older. Nothing between the field exit
  and tm_mainLoop (field cleanup, overlay load, tm_init's CD/SPU work) writes +0x480 or +0x2E8. `turn` = 2 and
  `comboDirs` = 0x5C are rewritten on every entry by tm_init's own work: libetc VSync's local (pc 0x80015C70, called from
  CD_sync) and the low byte of a pointer saved by libspu `_spu_t` (0x800174A4, ra in `_spu_Fw`). So those two are
  effectively constant.
- for the port: animFrame's PS1 start value isn't a constant. A real game gives the cursor a random-looking phase (a GTE
  shadow-matrix element). For a bit-exact replay of a PS1 session, take it from the PS1 state (0x801FFE60, or the
  table above for the enter_tm/3ds4 fixtures: 0x00450042). Since only the cursor sprite's U depends on it, a match that
  diverges when animFrame changes means the port feeds rendering back into game logic (frame pacing / VBlank count per
  frame from draw cost or hashes, or input timing). That is the same coupling F41 suspected. On the PS1 the RNG
  (rand() once per frame + the game's own calls) and every task decision are independent of it.
- proposed decomp change (not applied; src/ untouched):
  - `include/ff9.h`, next to PSX_NULLABLE:
    ```c
    /* A local the original code reads before writing it, so on the PS1 it starts with stack residue (the value is
     * documented at the use). Empty here; a port defines it to give the variable a deterministic start value. */
    #ifndef PSX_STACK_RESIDUE
    #define PSX_STACK_RESIDUE(lv)
    #endif
    ```
  - `src/ovl_0d/tm_game.c` tm_mainLoop, after the four `tm.unk5C8` stores and before `tm_setup(&tm)`:
    `PSX_STACK_RESIDUE(tm.animFrame);` with a comment at `TmState tm;`: "Never fully initialised: animFrame (only
    tm_drawCursor's frame) starts as stack residue, on the PS1 the first row of ovl_0a fld_drawShadow's `mtx` from the
    last shadow the field drew (varies, e.g. 0x0000FFD6 / 0x0000001D at Dali 354, 0x00450042 in the enter_tm fixtures);
    timer, turn and comboDirs also start as residue but are not observable (F42)." The ovl_0d.h field comment for
    animFrame could say "never initialised (stack residue, F42)".
  - Byte-identical: `build/f42/probe.c` (tm_mainLoop with the macro line, macro empty) scores `tm_mainLoop:860/860*`
    with `ccprobe --bin ovl_0d`. Only animFrame needs the hook. timer/turn/comboDirs can be left alone, or wrapped the
    same way if the port wants a clean-memory build.
- states (disc 1): `f42_dali354_card_prompt` (Eve's "You want to play a card game?" prompt, Yes highlighted),
  `f42_dali354_after_quit` (back in Dali 354 after quitting a real match).
- confidence: high for the field list and the no-effect claims (decompiled ovl_0d, no INCLUDE_ASM left, plus the tracker
  over ~12,000 frames on 3 entries); high for the residue source (byte-level last-writer log).

## BTLCAM_OP runtime check: battle camera script command flags (ovl_0b btlcam_step)  (status: applied; renames EASE_SPEED, BOB/IDLE_BOB, SET_HOME_VIEW, PAUSE_CMD/PAUSE_SEQ, EASE_IN/OUT swapped)
- binary: ovl_0b; names under test: `BTLCAM_OP_*`, `BTLCAM_CUE_END_INTRO` (include/ovl_0b.h), fields of `g_btlCamera`
  (0x800FE3B8) and `g_btlCamScript` (0x800FE488). Battle (21, Black Waltz 1 + Sealion) base 0x80108800: phase +0xD80,
  subPhase +0xD81, loadFlags +0xD85, cameraNo +0x131C, scene.flags +0x132C, scene.sceneData +0x133C.
- setup: `start --disc 1`, `frames 1800`, `tools/rt/enter_battle.py 21 --disc 1 --field 351 --base-state field_dali_inn`,
  ~200 frames. New disc-1 states: `btlcam_enc21_preintro` (ovl_0b resident, intro camera not started, script frame 0),
  `btlcam_enc21_combat` (phase 4, after the scripted intro), `btlcam_enc21_cmd` (Zidane's command menu, Sealion and
  Black Waltz HP poked to 1), `btlcam_enc21_attack` (cmd state + Cross Cross; an enemy action with an effect camera
  follows).
- tools (scratch, build/btlcam_rt/): `rec.lua` (one exec breakpoint on btlcam_step's entry 0x800C37BC and one on
  btlcam_lookAt 0x800C52B4; logs script frame, pc, endIntro, paused, phase/subPhase/loadFlags, projDist, base, GameData
  projDist (+0xA), eye/target per step), `stream.py` / `streamB.py` (hand-written command lists), `run.py` / `runB.py`
  (load `btlcam_enc21_preintro`, break at the second btlcam_step (script frame 2, pc 0x8010F0C6 = intro variant 0 after
  its frame-1 commands), overwrite the rest of the intro list (room up to 0x8010F2BE) with the test list, record),
  `keep.py` (KEEP_CAMERA), `freq.py` (scanner statistics). The camera steps once per 4 VBlanks here.
- **Q1 intro / victory cues: confirmed.** `until 0x800C3B58` (the `sb` of endIntro in btlcam_step) from
  `btlcam_enc21_preintro`: hit at script frame 0x93 = 147 (variant 0, Battle.cameraNo 0), phase 2, loadFlags 0x7F.
  With cameraNo poked to 1 (`write 0x80109B1C 1`) before the intro: hit at frame 96, variant 1. Same-frame clear:
  `watch 0x800FE41F` shows the set (btlcam_step+0x39C) and the clear (+0x750) in the same step, and phase goes 2 -> 1
  (BTL_PHASE_SCRIPT, battle 21 has BTL_SCENE_SCRIPT_START); with scene.flags bit 2 cleared it goes to ENTRANCE and
  combat (runB). The frame-147 command also sets saveView (1 -> 2 at the next path evaluation). The RETURN at 148 loads
  the idle script (pc 0x8010F2E6). subPhase: `watch 0x80109581` over 1500 frames of intro + combat start: one write,
  btl_setBattleVar from the event script, value stays 0; recorder log over the whole fight: 0 until the end. Victory
  (Sealion and Black Waltz HP poked to 1, ATB speeds +0x2E to 0, Zidane/Vivi Attack): writes are
  btl_checkSideDefeated (BTL_END_VICTORY = 0), btl_victory (0, phase 5 -> 6), then btlcam_step+0x3C4 (cue) and
  btl_update+0x230 one VBlank later. Recorder: victory camera (scene 2, variant 2 here) at script frame 31 sees
  subPhase 0, at frame 32 subPhase 2 and exitTimer counting 1, 2, ... (btl_update: subPhase 1 -> fade_out(0x20) and 2).
  So the 0x4000 cue with argument 2 starts the fade-out at frame 31, as predicted.
- **Q2 frequencies** (`btlcam_scan.py --json`, 5937 scripts: 562 encounters' scene data on all discs (a battle that a
  disc copies twice counts twice), 835 effect-camera lookups on disc 1, 15 common; columns: commands / scripts / where):
  0x40 ACCEL 1428/1060 (intro 169, cmd cams 733, enemy 142, effects 16; values 0, 0x28-0x8C, 0x6F);
  0x80 SAVE_VIEW 1319/578 (every intro (562), always in the same command as the 0x4000 END_INTRO cue; 13 AI cameras,
  3 effects); 0x100 PAUSE 760/745 (only party/enemy-target command cameras (scenes 3/4: 522 + 219), 1 effect, 3 common);
  0x200 SWAY 167/72 (intro 67, args 0xF1 / 0x91 / 0 / 0x0001); 0x400 IDLE_SWAY 19/8 (scene 296's intro, victory and
  AI cameras, 0x22/0x23/0x13 and 0 to stop it); 0x800 ZOOM 14223/3045 (everywhere); 0x1000 SHAKE 339/114 (enemy-type
  cameras 19, effects 95); 0x2000 PICK_VARIANT 3315/1586 (command cameras 1124, enemy-type 452, common 10; never
  intro/victory/effect); 0x4000 CUE 2986/1123 (intro 1303x arg 1, victory 1683x arg 2, nothing else);
  0x8000 RETURN 5133/3408 (args 21 00 00 5079x; a1 00 00 22x; 61 0f 02, 61 0a 02, 21 13 00, 21 10 01, ...);
  0x80000 KEEP_CAMERA 23/23 (22 effect cameras (18 of whose variants end without RETURN), scene 891/6 variant 2);
  **0x100000 PAUSE_HI 1268/695 (enemy-type cameras, scenes 5+: 661; effects 32; AI 2)**; bits 0x10000-0x40000 and
  0x200000: never used.
- **SHAKE (0x1000): confirmed, axes byte refined.** Test list: `10: 0x1000 [40,0,20,0]`, `40: [40,0,20,1]`,
  `70: [40,0,20,2]` with both paths holding. Only the look-at point (btlcam_lookAt a2) moves; the eye never does. Axes 0:
  x, y and z jitter (x 2084..2336, y -2240..-2064 around 2132/-2200), decaying to rest over exactly 20 steps
  (frames 10-28). Axes 1: y fixed, x/z jitter. Axes 2: x/z fixed, y jitters. So args = start amplitude, end amplitude,
  frames, axes (bit 0 no vertical, bit 1 no horizontal), as in the header; the shake moves the target, not the eye.
- **ZOOM (0x800): confirmed.** `100: [10, 2, 0x300]`: projDist and GameData.projDist go 200 -> 257, 314, ... 768 in
  10 steps (linear), base 512 unchanged. `115: [10, 1, 0x100]`: base 512 -> 256 in 10 steps, projDist stays 768.
  `130: [5, 2, 0]`: projDist 768 -> 256 (= the new base) in 5 steps. Bytes: frames, which (bit 0 base, bit 1 current),
  u16 distance (0 = base). Frame-1 commands with frames 1 are just "set the projection distance".
- **PAUSE (0x100) / PAUSE_HI (0x100000): confirmed; both stop the script frame.** `270: 0x100`: frame stays 270 for
  60 VBlanks (paused = 1); after `write paused 0` it counts on. `275: 0x100000` (c0 bit 14): same. What resumes it
  in a real fight: a write log on paused (0x800FE40A, `Bridge.addBp(...,'Write',1,'log')`) during the
  btlcam_enc21_attack action: set by btlcam_step at the enemy camera's (scene 6 = enemy type 1, variant 0) frame 16
  (its 0x100000 command), cleared ~5 VBlanks later by btlseq_opWaitAnim+0x58 (the enemy sequence's
  wait-for-animation-end), also cleared by btlcam_play. Code: btlfx_op's cast handler clears it when the cast ends
  (not observed: no party spell was cast). 0x100 is only in party command cameras and 0x100000 only in enemy/sequence
  and effect cameras, so the two bits look like per-tool spellings of one op; the names can stay, or become
  `PAUSE_CMD` / `PAUSE_SEQ` if you want the usage in the name (low value).
- **SWAY (0x200) / IDLE_SWAY (0x400): confirmed, but it's a vertical bob.** `140: 0x200 [0xF2]`: eye.y and target.y
  both move by the same amount (x, z unchanged), sinusoidal, about +-190 units, period 32 steps; `180: [0x81]`:
  about +-103, period 64 steps. `220: SWAY 0 + IDLE_SWAY 0xF4`: the idle value takes over (period 16 steps,
  same +-190 amplitude) because the script's own sway is 0; `260: IDLE_SWAY 0`: back at rest. Amplitude = high nibble
  scaled by the horizontal eye-target distance, speed = low nibble (phase += n per step, 64/n steps per cycle).
  g_btlCamera.idleSway survives btlcam_playScene/btlcam_play (only g_btlCamScript is cleared), so IDLE_SWAY sets a bob
  that keeps running under later cameras that set none. Proposed rename: `BTLCAM_OP_BOB` / `BTLCAM_OP_IDLE_BOB`
  (fields `bob` / `idleBob`, `btlcam_applySway` -> `btlcam_applyBob`), or keep the names and say "vertical" in the
  comment. Confidence high for the behaviour, naming is taste.
- **ACCEL (0x40): works, name should change.** Same 20-frame anchored move (path 0 mode 3) K0 -> K1, eye progress per
  step (fraction of the distance): ease 0: 0.05 per step (linear); ease 1, accel 0: 0.003, 0.010, 0.023, ... 0.903, 1.0
  (= s^2/400, starts at rest and speeds up); ease 1, accel 0x8C: 0.035, 0.072, 0.110, ... (starts at 0x8C/4096 =
  0.034 per frame); ease 2, accel 0: 0.098, 0.190, ... 0.990, 0.998, 1.0 (slows to rest); ease 2, accel 0x8C: ...
  0.929, 0.965, 1.0 (ends at 0.035 per frame). So the value is the linear-speed term of the eased weight
  (per-frame weight / 4096): start speed for ease mode 1, end speed for ease mode 2; 0x1000/frames would make it
  linear. Proposed: `BTLCAM_OP_EASE_SPEED`, `g_btlCamScript.easeSpeed`. Also, `Unk800C412C.easeModes`'s comment has the
  mode names the wrong way round in the usual sense: mode 1 starts slow (ease-in, accelerate), mode 2 ends slow
  (ease-out, decelerate). The LERP-mode eye (run A, mode 2 paths) shows the same weights, only bent by the polar orbit.
- **SAVE_VIEW (0x80): confirmed, the meaning is narrower.** `265: 0x2080`: then points[4] = (-183, -2419, 3080) = the
  eye and points[5] = (2132, -2200, 5383) = the target (unswayed, before shake), saveView = 2. The idle camera (scene 1
  at 0x8010F2DA: one command, frame 1, flags 9, keys with anchors 0x19 / 0x1A and zero offsets) is "eye at points[4],
  target at points[5]", i.e. the view SAVE_VIEW stored. Every intro ends with 0x4080 (save + END_INTRO), so the idle
  camera is wherever the intro stopped. That is also the target-select view (btlcam_applyTargetView copies
  points[4]/[5]). Proposed: `BTLCAM_OP_SET_HOME_VIEW` (or keep SAVE_VIEW and comment "the idle camera's view").
- **PICK_VARIANT (0x2000): confirmed.** effectPick poked to 7 and g_btlCamera.variant to 2, then `265: 0x2080`:
  effectPick = 2. Used by command and enemy cameras, so the effect camera btlfx_opPlayCamera mode 3 plays follows the
  command camera's variant (otherwise random 0/1 from btlcam_beginEffect).
- **RETURN (0x8000): confirmed, the byte meanings refined.** Arg bytes: pc[0] bits 5-6 = path mode of both paths for the
  way back (other bits ignored: 0x21 and 0xA1 are both HOLD), pc[1] = frames, pc[2] = easeModes. `21 00 00` (run A):
  mode 1, no frames: the idle camera's keys apply on the same step (a snap; with the view saved by the intro the
  picture doesn't move). `61 0f 02` (runB, intro ended at K1, saved view K0): mode 3 (anchored lerp) for 15 steps with
  ease 2: eye 1113,-1537,-1011 -> 642,-1651,-49 -> 528 ... -180,-2416,3063 -> -183,-2419,3080 (slows down at the end),
  then paths drop to HOLD. `locked = 1` keeps the idle script's first command from replacing that mode/timing; it
  only supplies the destination keys. The script frame restarts at 1 in the same step (btlcam_update steps again).
- **KEEP_CAMERA (0x80000): confirmed.** `keep.py`: from `btlcam_enc21_attack`, break at the second btlcam_play
  (effect camera 0x801F6E04 from btlfx_opPlayCamera), OR 0x2000 into each variant's first c0. Unpatched: the idle
  script is reloaded (pc 0x8010F2E6) at the effect camera's own RETURN and again at btlcam_endEffect (f24033, script
  frame 1). Patched: keepCamera = 1 from the first step, the same RETURN reload happens (RETURN ignores the flag), but
  at btlcam_endEffect keepCamera drops to 0 with no reload (script frame runs on 15, 16, ...). 18 of the 22 effect
  camera variants with the flag end without RETURN, so for them the effect's last view stays up after the effect.
  The name fits.
- CUE / END_INTRO: as in Q1; a cue other than 1 increments subPhase (victory cameras use 2). Name fits.
- confidence: high for SHAKE, ZOOM, PAUSE (both bits), SWAY/IDLE_SWAY behaviour, ACCEL behaviour, SAVE_VIEW,
  PICK_VARIANT, RETURN, KEEP_CAMERA, CUE (all measured from patched or natural scripts); the cast-end release of PAUSE
  is code-only.

## R41 runtime pass: pending constant lists  (status: applied: motion slots, ATE / NEW sprites, WM_TOPO_WATER*/SEA*, WM_SE_GROUP_*, TEXT_PAD_*, g_worldDataArea, *_strandTrailTail, btlfx_stepForward; 0x30 / 0x33 and the w_musicGetID SE numbers stay numbers)
Lists: docs/wip/literals_runtime.md, docs/wip/weak_names_runtime.md, docs/wip/constants_runtime_main.md,
docs/wip/constants_runtime_overlays.md (2026-11-04). Redux, LuaJIT off. Battle runs: `build/rt3/run3.py` from
`disc3_malboro_lv99_calm` (`--patch CMD,AB,TGT` rewrites Zidane's first Attack; outputs `build/rt3/r41_*.json` / `.png`).
Sprite identification: a Lua exec breakpoint on `pen_drawSprite` (a0) and `ui_drawSprite` (a3) rewriting one sprite id
to a button glyph (4 Cross, 5 Circle, 6 Square), then a screenshot. New state: disc 3 `r41_wm_near_airship` (walker
teleported under the Hilda Garde 3, Hadris; boarding with Cross did not trigger there).

### constants_runtime_main.md / constants_runtime_overlays.md: already settled
Both lists were answered by "Constants runtime pass (2026-10-31)" (applied) and the leftovers are now named from that
evidence or deliberately left: FLD_OBJ_COLOR_WAIT, FLD_EXT_BUILT_UNREAD, GD_FLD_TAKEOVER, WM_ACTOR_*, WM_MOVE_* (boat /
Invincible confirmed in R37b). Still literal: GameState.mode 5 / 6 (no writer stores them) and BgEntry.type 0x40 (no
writer reached). Nothing new was run for these lists.

### literals_runtime.md: battle motion slots (ovl_0b BtlUnit.motions)
Method: `disc3_malboro_lv99_calm` + calm pokes, Zidane's `model->motion` (0x80093BC4) := `motions[n]` (table
0x80108808) and `obj->frame` (0x80077F3E) := 0, screenshots 3/9/19/34/64 vsyncs later (without the frame reset
btl_updateUnitModel's frame >= frameCount check sends short motions straight back to idle through btl_setIdleMotion).
Frame counts (BtlMotion+2) of Zidane's slots: 4 = 1, 8 = 15, 0x12 = 30, 0x13 = 10, 0x1A = 10, 0x1B = 12, 0x1C = 18,
0x1D = 7, 0x1E = 8, 0x1F = 18, 0x20 = 6. Natural sequences: `run3.py r41_blkmag --patch 0x16,0x19,0x10 --trace
btlfx_playUnitMotion` and `run3.py r41_item --patch 0x0E,0xEC,0x01` (Potion on Vivi, "150" heal shown).
- **slot 8 → `BTL_MOTION_KO_FALL`. CONFIRMED.** Zidane kneels, slumps and ends lying on the floor (frame 64); slot 4
  (BTL_MOTION_KO) is the 1-frame lying pose that follows. Matches btl_updateKo (8 from weak stance, after
  IDLE_TO_WEAK otherwise) and the revive check (`KO` or `8` → GET_UP). high.
- **slot 0x12 → `BTL_MOTION_VICTORY`, 0x13 → `BTL_MOTION_VICTORY_LOOP`. CONFIRMED.** 0x12: crouch, leap, stand with the
  dagger raised (30 frames); 0x13: short loop holding the dagger overhead (10 frames). Code: 0x12 when Character.info
  bit 10 (`info.b.victoryPose`) at the victory, 0x13 each time the current motion ends in BTL_PHASE_EXIT. high.
- **slot 0x20 (32) → `BTL_MOTION_READY_TO_IDLE`. CONFIRMED.** Played by btlfx_opReturnHome once the caster is home
  (f3984 black magic, f4504 item) and by btl_cmd for a READY unit whose command is dropped or flushed; 6 frames, short
  lowering of the guard back to the idle stance. high.
- **0x1A / 0x1B / 0x1C / 0x1F: CONFIRMED** (names stay). Black magic: 0x1D f3532 (btlfx_stepBack), 0x1A f3560,
  0x1B f3599, 0x1C f3651 (btlfx_opCast), 9 READY f3723, 0x1E f3952 (btlfx_moveUnitHome), 0x20 f3984. Item: 0x1F f4296,
  9 f4368, 0x20 f4504. high.
- **slot 0x1D → `BTL_MOTION_STEP_FORWARD`, 0x1E → `BTL_MOTION_STEP_HOME`. CONFIRMED; and btlfx_stepBack is misnamed.**
  btlfx_stepBack plays 0x1D and moves the caster from home to btlfx_pointAhead(home, rot, 400), i.e. 400 units towards
  the enemies (screenshots `build/rt3/r41_blkmag_11..13.png`: Zidane stands left of his home, then walks back right
  during 0x1E). Proposed rename `btlfx_stepBack` → `btlfx_stepForward`. Note these are the same slots as
  `BTL_MOTION_CHANGE_ROW + row` (29 + 0 = 0x1D, 29 + 1 = 0x1E): the Change command reuses them, so CHANGE_ROW can be
  defined as BTL_MOTION_STEP_FORWARD. high.

### literals_runtime.md: menu sprites (main g_uiSprites)
- **ATE prompt 0x53 / 0x54 / 0x80-0x82 / 0x1F-0x21: CONFIRMED.** `r36_lindblum_552_ate` (disc 1, g_atePromptMode 1):
  even 44-tick phase "PRESS [SELECT] TO SEE ATE", odd phase "ACTIVE TIME EVENT" in blue/white. Swaps: 0x53 → Cross
  replaces "PRESS", 0x54 → Circle replaces "TO SEE ATE"; 0x81 → Cross replaces "TIME" (0x80 "ACTIVE", 0x82 "EVENT").
  Mode 2: grey "ACTIVE TIME EVENT" blinking; 0x20 → Cross replaces "TIME", so 0x1F / 0x20 / 0x21 = grey ACTIVE / TIME /
  EVENT. Proposed `UI_SPRITE_ATE_ACTIVE 0x80`, `_ATE_TIME 0x81`, `_ATE_EVENT 0x82`, `UI_SPRITE_ATE_PRESS 0x53`,
  `UI_SPRITE_ATE_TO_SEE 0x54`, `UI_SPRITE_ATE_ACTIVE_GREY 0x1F` (+1 TIME, +2 EVENT) (or the R41 main list's
  `UI_SPRITE_ATE_TITLE` / `_TITLE_GREY` bases + 0..2). high.
- **UI_SPRITE_HELP_TAG 0xB7: CONFIRMED.** `disc3_malboro_lv99_help`: the "HELP" label on the help window's top edge
  (same style as the "COMMAND" tag); swapped to Cross the label becomes a Cross glyph, once per battle frame. Name fits.
  high.
- **UI_SPRITE_NEW 0xB5 / UI_SPRITE_NEW_BLINK 0xB6: CONFIRMED, names slightly off.** `menu_items` → Key: every unseen key
  item (GameData+0x774 2-bit pairs, bit 1 = seen) shows a yellow "!" badge followed by the word "NEW". Swaps: 0xB5 →
  Cross replaces the "!" badge, 0xB6 → Cross replaces "NEW". The "NEW" word alternates white / grey (blink argument
  `g_menuFrame % 48 >= 16`). Proposed `UI_SPRITE_NEW_MARK 0xB5` ("!" badge) and `UI_SPRITE_NEW_TEXT 0xB6` ("NEW", drawn
  with the blink style), or keep the names with the corrected comments. high.

### literals_runtime.md: world map (ovl_0c)
- **Topographies 0x36-0x39: water depths, PARTLY CONFIRMED; 0x30 / 0x33 not found.** `r37b_wm_boat_entry`, sailing up
  then up-left, the boat type's cellAttr (g_worldActorTypeInfo[8] + 8) went 0x35 (31 samples) → 0x36 (5) → 0x39 (4),
  i.e. shallows → deeper water → open sea; screenshot: the Blue Narciss on open sea off a grassy coast. In `cpass_wm_hadris`
  `build/verify/topofind.py` found 0x36 / 0x37 polygons at y 0 forming the lake inside the Hadris crater (walker
  teleported there stands on the blue water surface) and 0x38 / 0x39 at y 0 along the coast (0x39 is the most common
  topography of the loaded blocks: 1594 polygons). g_worldTerrainHeightOffsets read at runtime (s16 [class][9], index
  forest, 0x35, 0x36, 0x37, 0x38, 0x39, 0x33, 0x30, other): class 1 (walker) 400, 150, 250, 200, 450, 450, 300, 300, 0;
  class 2 0, 300 x7, 0. So all eight are water the walker sinks into (0x38/0x39 deepest). Proposed
  `WM_TOPO_WATER 0x36` (inland lake / water beyond the shallows), `WM_TOPO_WATER_2 0x37`, `WM_TOPO_SEA_2 0x38`,
  `WM_TOPO_SEA 0x39`, and 0x30 / 0x33 `WM_TOPO_WATER_3` / `_4` (code + table only). medium; the two
  missing ones need a scan of other areas (topofind from more teleport spots).
- **w_musicGetID SE numbers: PARTLY.** Observed: 39 (SE_CURSOR) from the moogle's choice window (r37b_wm_moogle_choice,
  w_frameSetParameter case 20); 38 = 0x5DB on group 0 (wm_seFadeVolume every 6 frames while the Hilda Garde 3 flies,
  r37b_wm_hg3_entry); 24 (boat loop) below. No script requested 0, 2, 3, 4, 30-35, 37, 40-42 in walking, chocobo
  choice, airship navi / "Go to" prompt, dig and boat runs (300 frames each). They are world-script requests
  (w_frameSetParameter "SE Play"); naming them needs the scripted events that play them plus `audio record`.
  Still unknown.
- **SE group masks 0x8000 / 0x800: CONFIRMED for the vehicle loops.** `r37b_wm_boat_entry` sailing, trace
  wm_soundCommand: 50x (0xD301, 0x5D9, 0x8000) and 50x (0xD301, 0x5DA, 0x4000) from wm_seFadeVolume: the two sounds
  of a two-sound entry get 0x8000 and 0x4000 (the mask shifted right per sound). The airship water loop (38) uses
  group 0. No mode change stops a loop by group: w_movementChange only stops the boat loop by id (old mode
  WM_MOVE_BOAT), and wm_stopAllSe stops numbers 0-6 and 20-25 each with its own mask (code). Proposed
  `WM_SE_GROUP_VEHICLE 0x8000` (+ 0x4000 for the second sound) and `WM_SE_GROUP_AMBIENT 0x800` (+ 0x400), medium for the
  ambient one (code only).

### literals_runtime.md: battle UI TEXT_PAD_BTN + n
- **CONFIRMED by runtime data: the offset follows TEXT_BTN_* order, not PAD_SLOT_*.** Disc 1 `menu_items`:
  g_msgIconSprites (0x8006E67C) bytes 05 04 07 06 08 0A 09 0B 11 12 (Circle, Cross, Triangle, Square, L1, R1, L2, R2,
  START, SELECT), g_buttonSprites (0x8006BC18) 4 5 7 6 8 10 9 11, GameState.config 0x19AC0 (CFG_BIT_CONTROLLER clear),
  padMap 0x76543210. With the default config TEXT_PAD_BTN + 0..7 draws Circle, Cross, Triangle, Square, L1, R1, L2, R2
  (msg_renderText swaps 0/1 into PAD_SLOT order and back; text_buildCode passes PAD_SLOT_CIRCLE for +0). So the cases
  can't read `TEXT_PAD_BTN + PAD_SLOT_*`; use `TEXT_PAD_BTN + (TEXT_BTN_x - TEXT_BTN_CIRCLE)` or names
  `TEXT_PAD_CIRCLE 0x60` .. `TEXT_PAD_R2 0x67`. A message using the codes was not rendered. high (tables), medium
  (rendering).

### weak_names_runtime.md
1. **ovl_0c g_worldLowBuffer → `g_worldDataArea` (0x80100800 = end of the world heap).** g_worldHeapTop reads 0x800DC000
   and wm_heapInit reserves 0x24800, which ends exactly at 0x80100800. `cpass_wm_airship9` flying: write watch over
   0x80100800-0x80138800 (`r41_low`, 97745 hits in 400 frames) by wm_drawMesh / wm_calcBoneMatrices (via
   wm_drawActorModel) and wm_depthCueClut (wm_setActorFog) in 0x8011E800-0x80125BE0; read watch (`r41_lowr`) by
   wm_projectMesh / wm_drawMesh (actor model meshes at 0x80108C7C-0x8010A3C8) and evt_runExpr / evt_popAssign (the world
   event script bytes 0x8011761D-0x8011C8F3). 0x80100800 itself starts with 4-bit image data. So the range holds the
   loaded world data (actor models, the world script, images). Proposed `g_worldDataArea` (pointer still never read).
   medium (who loads it there not traced).
2. **ovl_0c g_worldCameraInitFlag / g_worldMoveInitWord / g_worldSysInitFlag: CONFIRMED write-only.** Disc 1, walked
   out of `field_ice_300` onto world 9000 with log breakpoints (write and read) on all three, ~1200 frames incl.
   walking: writes only from CD_getsector (the overlay load, f5897) and once each from wm_initCamera+0x88,
   wm_initMovement+0xA4, wm_systemInit+0xF0 (f6120); 0 reads. Also 0 reads / writes in 300-frame runs of
   `cpass_wm_hadris`, `cpass_wm_airship9`, `r37b_wm_chocobo_ride` (walk, Triangle menu, Circle). Names fit. high.
3. **main g_menu.result: CONFIRMED only cleared.** Write watch on 0x80077DCC while closing `menu_main`,
   `menu_shop_weapons`, `menu_name_entry`, `menu_save`, `menu_chocograph`, `menu_card`, `menu_party_select` (Circle x3,
   Cross, Circle; 300 frames each): 0 writes. From `field_dali_inn`, Triangle → main menu: 1 write, menu_doOpen+0x64
   storing 0. menu_getResult 0 calls. Name fits. high.
4. **ovl_15 g_messagesFrameWord / g_theEndInitWord: CONFIRMED write-only.** Disc 4, `tools/rt/bj_force.py` variant
   (scratch) with write/read log breakpoints: THE END path: g_theEndInitWord written once (ending_theEndInit+0xC0,
   f2086), 0 reads in 1500 frames. Forcing ending_messages (ending_main state 1, "never reached" naturally) by
   setting pc = 0x800AD7DC at the ending_playMovie break: ending_messagesInit once, ending_messagesSwap 1015 times,
   g_messagesFrameWord 1016 writes (messagesInit+0x3C, messagesSwap+0xFC), 0 reads. high.
5. **fx_0b8_scrollInitWord / fx_1be_scrollInitWord: CONFIRMED write-only.** Atomos full (`--patch 0x10,0x37,0x10`,
   support poke 0x8008B31B |= 8, btlfx_start 0xB8): 0x801EC1B0 written once (module 0x801E7960) and read 0 times in
   2000 frames. Atomos short (btlfx_start 0x1BE): 0x801E95D0 written once (0x801E7A44), 0 reads in 1200 frames. high.
6. **fx_17d_4_buf400 / fx_1bf_0_buf400: REFUTED "unused": the start of each is live trail storage.** Group residency
   from `btlfx_runFrame` dumps of the module header word. Ark full (`r41_ark4`, 4000 frames): fx_17d_4 resident
   f28649-f30157; in that window writes to 0x801EB25C+: op80 mode 1 byte 0 (0x801E8258, once), btlfx_pushTrailPoint
   (ra 0x800D1574) 13x into 0x801EB26C-0x801EB2D0, and module code 0x801E7A8C / 0x801E7A98 (fx_17d_4_drawStrands,
   btlfx_drawTrail state of the last strands: strandTrails[17] = 0x801EB1EC + layer * 36 runs into it) 676x into
   0x801EB25C-0x801EB284. Ark short (first-use bits GameData+0x55E set first, `r41_arks2`): fx_1bf_0 resident
   f32147-f32857: op80 byte 0 once, module 0x801E7A70 / 0x801E7A7C 676x into 0x801EA1FC-0x801EA224. The rest of the
   0x400 bytes was written only while other groups were resident (their own model buffers). Proposed
   `fx_17d_4_strandTrailTail` / `fx_1bf_0_strandTrailTail` (spill area of the strand trail buffers, not a separate
   buffer), remove "no other access" from the comments. high.
   Side note: Ark without the first-use bits plays the full 0x17D modules even when btlfx_start gets 0x1BF.
7. **ovl_0b btlui_perfMark: CONFIRMED.** `disc3_malboro_lv99_calm`, `trace btlui_perfMark --frames 60`: 120 calls, 60
   with a0 0xBA from btlui_vsync+0xBC and 60 with 0xBB from btlui_vsync+0xFC; no other arguments. It is called from
   the VSync callback, so twice per vsync (bracketing the UI frame), not per battle frame. high.
8. **main pad_onStartPressed: CONFIRMED.** `menu_shop_weapons`, Cross into Buy, Start: 1 call per press from
   shopBuy_update+0x20 [ovl_3c] (2 presses → 2 hits); holding Start 150 frames → 1 hit (no auto-repeat). From
   `field_dali_inn` (Start in the field, main menu, item screen): 0 hits. high.
9. **ovl_0b nop exports 0x090/0x094/0x0A4/0x29C/0x300/0x308/0x30C: CONFIRMED never reached** in black magic (effect 0),
   Leviathan full (0xB3), Fenrir full (0xD2), Throw (0x110), the Malboro's own attacks (0x64, 0x18) and Atomos / Ark
   (above, those runs did not trace them). Exec breakpoints on all seven: 0 hits. high.

## R41b runtime pass: R41 question files  (status: proposed)
Lists: docs/wip/r41_runtime_ovl_0b.md (Q1-Q3), docs/wip/r41_runtime_main.md (rows 2-5; row 1, the ATE sprites, was
answered by "R41 runtime pass" above), docs/wip/r41_runtime_overlays.md (Q1-Q11). Redux, LuaJIT off, 2026-11-04.
Scratch in `build/r41b/` (`tp.py` world-map teleport + topography/area readout, `toposweep.py` airship grid sweep with
R37's `build/verify/topofind.py`, `bankabl.py`, `bgscene.sh`, `title.py`, `abl.py`, `areas.py`) and `build/rt3/r41b_*`.
World-map area names come from the data pack at runtime (`g_worldAreaNames` at 0x800CF478 is a *pointer*; area n =
string 2n). New states: disc 4 `r41b_wm_disc4_memoria_orb`, `r41b_wm_disc4_windmill`; disc 1 `r41b_wm_d1_story1000`
(world 9000, story 0x1000) and `r41b_tmp` (scratch, overwritten by abl.py).

### ovl_0b
- **Q1 BTLBG_SCENE_*: scene ids CONFIRMED, locations from the field that starts each battle.** `build/r41b/bgscene.sh ENC
  FIELD DISC BASE` (= `tools/rt/enter_battle.py ... --frames 1300`, read `g_btlbgId` 0x800FE50C, two screenshots 60
  frames apart, `build/r41b/bg_ENC_{a,b}.png`). Field names = the field script's name (`tools/evtscript/evtdis.py F
  --disc N`, Shift-JIS):
  | id | enc (field) | g_btlbgId | field name | seen | proposed |
  |---|---|---|---|---|---|
  | 7 | 931 Nova Dragon (2756, disc 4) | 0x07 | "Red Rose/Bridge" | pink-white sky, no floor | `BTLBG_SCENE_NOVA_DRAGON` (or `_RED_ROSE_SKY`) |
  | 0x44 | 84 Armodullahan (1423, disc 2) | 0x44 | "Fossil Roo/Passage" | stone tunnel with arches (the cart chase) | `BTLBG_SCENE_FOSSIL_ROO` |
  | 0x6E | 924 (2902, disc 4) | 0x6E | "Memoria/Stairs of Time" | stone balustrade / colonnade | `BTLBG_SCENE_MEMORIA_STAIRS` |
  | 0x70 | 925 (2906, disc 4) | 0x70 | "Memoria/Oblivion" | tall hall with windows and balconies | `BTLBG_SCENE_MEMORIA_OBLIVION` |
  | 0xA8 | 191 Hades (2914, disc 4; also 952/960 from 2921) | 0xA8 | "Memoria/Birth", "Memoria/To the Origin" | dark void, red crystal mass on a floating platform | `BTLBG_SCENE_MEMORIA_ORIGIN` |
  | 0xAB | 937 Trance Kuja (2927, disc 4) | 0xAB | "Crystal World/Origin" | tilted round platform with red spikes in space | `BTLBG_SCENE_CRYSTAL_WORLD` |
  Which part moves was not isolated (the battles ran without calm pokes, so screenshots are mid-action); the motion
  names can stay as comments. high (ids/locations), low (moving part).
- **Q2 BTL_SOUND_BASE_BANK 0x201 → `BTL_SOUND_SE_BANK` (the battle's SE instrument bank). CONFIRMED.** Log bps on
  akao_loadBankRetry (0x80055D80) and akao_loadBankInSlot during `enter_battle.py 899 --field 2714 --party 0,1,2,6`
  (disc 3): 1 hit each per battle load. Offline: DIR_SOUND_BANKS file 0x201 is AKAO bank id 208, SPU 0x4F100, 0x13F80
  bytes, slot 192, 32 instruments, i.e. g_akaoInstruments 192-223 = SE instrument numbers 0xC0-0xDF that
  akao_remapSeInstrument maps; the field SE banks (files 0x0/0x14/0x27/0x3A/0x4F/0x63/0x78/0x8E) load into the same
  SPU address and slot. Runtime (`run3.py r41b_inst3 / r41b_inst4` from `disc3_malboro_lv99_calm`, trace
  akao_setChannelInstrument + akao_playSe): instrument 192 is keyed by the battle SE data at 0x80198758 (Zidane's
  attack swing, from btl_soundCmd+0x4CC), 0x8019BAD0 and 0x80199AB0; the menu cursor (data 0x800A5E8C) uses 133 (bank
  0x84 below), enemy/spell sounds 128-175, the music only slot-0 instruments 0-9. An audio ablation (stubbing
  akao_loadBankRetry, `bankabl.py`) was inconclusive: music dominates the mix. high (what it is), medium (which SE).
- **Q3 BTLFX_SNDSET_CAST_EIKO: CONFIRMED with a correction: kind 10 only.** `run3.py` from `disc3_malboro_lv99_calm`,
  trace btlfx_playSoundSet / btlfx_startCastEmitter: Eiko (Triangle x3 to her menu, `--patch 0x11,0x01,0x01` Cure on
  Zidane) → set **0** because her Character.info.kind is 11 (CHR_KIND_EIKO_RACKET; weapon 0x36); with the kind poked
  to 10 (byte 0x8008B620 = 0xA6) → set **1**, sounds 0x17/0x18/0x19 (set 0 = 0x13/0x15/0x16). Zidane casting Fire
  (`--patch 0x16,0x19,0x10`, kind BLACK) → 0; Shiva (`0x10,0x31,0x10`, kind SUMMON) → 2; the Malboro's own cast
  (no calm, btlfx_start 0x39, kind ENEMY) → 0. g_btlfxUnitKinds (0x800F8D70) = 0A 00 00 00 01 00 00 00 0D 0E 0B 0C 12.
  Unit argument is BTLFX_UNIT_CASTER (0x10) in every case. Keep the name; comment: "caster kind CHR_KIND_EIKO
  (not EIKO_RACKET)". Cast-texture screenshots not taken (Cure: white burst with sparkles at Eiko,
  `build/rt3/r41b_eikocure_02.png`). high.

### main / ovl_0d
- **Row 2 tm_soundCmd musicBank[0] = 0xE2 → `TM_MUSIC_BANK` (the Tetra Master song's instrument bank). CONFIRMED.**
  Field 2714: GameData.musicId[0] 0x54, musicBank[0] 0xF1. `enter_tm.py 12 900` with a log bp on db_lookupRegistered:
  keys 0x900FA, 0x900E2 (tm_loadSoundBlocks, the two DB_SOUND_BANK entries of the TM data file, uploaded with
  akao_uploadBank) and 0x70042 (MUSIC_TETRA_MASTER); afterwards musicId[0] 0x42, musicBank[0] 0xE2. Offline: bank 0xE2
  = AKAO id 62, SPU 0x3100, slot 0, 96 instruments (a music bank, like the field's 0xF1); 0xFA = id 701, SPU 0x63100,
  slot 160, 32 instruments (TM sound effects, `TM_SE_BANK`). high.
- **Row 3 TM files 6/7/8/0xA. CONFIRMED by decoding them.** Log bp on tm_loadTim during `enter_tm.py 12 300`; the TIMs
  were still in RAM (file 6 at 0x80169200, 7 0x80179A00, 8 0x80182200, 0xA 0x8018EA00) and were rendered to
  `build/r41b/tmfileN.png`. 6 (8 bpp 256x256): "WIN" / "LOSE" / "PERFECT" / "DRAW" banners with their shadows, "Card
  Select", "Discard", "Selection", Japanese "discard cards" line, combo digits and "COMBO" → `TM_FILE_RESULT_TEXT`.
  7 (8 bpp 256x128): big blue and orange score numerals 1-10, the card backs (blue, orange, two grey stone, the ornate
  one), small arrow glyphs and a hex stat font → `TM_FILE_SCORE_DIGITS` (or `_PARTS`). 8 (8 bpp 256x128): the arrow
  pointers, "NEW Card", "Last ONE!", the coin (both faces and edge frames: the coin toss), a stat font, "R2 NAME",
  L1 / R1 → `TM_FILE_COIN`. 0xA (4 bpp 256x32): card-type icons, digits, "Select!" → `TM_FILE_SELECT_PARTS`. high.
- **Row 4 TM_AI_WEAK..EXPERT: CONFIRMED order.** g_tmOpponents (0x80104000) in a match: levels 0/1/2/3 used 139/22/33/62
  times. Opponents 0-43 cycle decks 0-10 at every level, 44-73 decks 11-20 at levels 1-3, 74-93 decks 21-30 at 2-3,
  94-123 decks 31-59 and 123/127 only at 3; 124-126 = decks 60-62 at 1/2/2; 128-255 = decks 0-63 again at level 0. The
  higher the deck, the higher the minimum level, so 0 = easiest, 3 = hardest. Story opponents not mapped. high.
- **Row 5 EVT_FUNC_RESUME 10 / EVT_FUNC_MENU 4: CONFIRMED; MENU is world-map only.** Trace evt_callObjectFunc:
  (a) field 2714, Triangle → menu → Circle: evt_onMenuOpen runs (from fld_main+0x118) but calls nothing (it only calls
  on g_evtMode == WORLD); (b) world (`cpass_wm_hadris`), Triangle: evt_callObjectFunc(obj0 0x80077EB0, 1, 4, 0) from
  evt_onMenuOpen+0xF8, v0 = 1 (the world script has the tag; its body tests variables 0xD5:0x18-0x1A against 100);
  (c) battle won from `disc3_malboro_lv99_calm` (Malboro HP 1, Attack, Cross taps): back in field 2714 evt_init →
  evt_callObjectFunc(obj0, 0, 10, 0) from evt_init+0x734. Field 2714's entry 0 tag 10 (evtdis) re-enables control
  (op 2E), restores state bytes (op 5B 0..0x16) and shows two conditional messages. The Tetra Master return was not
  run. Names fit; EVT_FUNC_MENU comment: "world map only". high.

### ovl_0c (world map)
- **Q1 SE numbers: 25 / 26 CONFIRMED; the other numbers still unknown.** After a teleport, trace wm_sePlay: source 2
  (299201, -93859, area 60 Triquai Plains) → a0 = 0x19 (WM_SE_RUMBLE_LOOP; the camera-shake source), source 1 (130781,
  -227819, area 45 "Gusty Valley") → 0x1A (WM_SE_VORTEX_LOOP), source 0 (Cleyra) silent because WM_FORM_CLAY is set at
  story 0x28A0. `audio record` at source 1 vs far away: much more low-frequency and hiss energy (wind). Proposed
  `WM_SE_WIND_LOOP` for 26 (both sources are wind/sandstorm). 0-4, 6, 7, 20, 21, 30-35, 37, 40-42 not requested in this
  pass.
- **Q2 WM_TOPO_PUSHBACK 0x17 → `WM_TOPO_GUSTY_VALLEY` (or `WM_TOPO_WIND`). CONFIRMED.** `toposweep.py` found 0x17 only
  in cells (15-16, 26-27) next to source 1. Walker teleported to (131264, -222500) in `cpass_wm_hadris`: topography
  0x17, area 45 = "Gusty Valley", z then rises every frame (-222500 → -221172 → -220236 over 100 frames: pushed away
  from the source toward +z), sunset-lit desert canyon (`build/r41b/push3.png`, `src1_w-*.png`). The ambient loop
  there is 26. high.
- **Q3 chocobo call cells: they differ by ground, not by region. CONFIRMED.** Walker on each found cell
  (`build/r41b/choc_N.png`): all show chocobo footprints. 0x03 (areas 24 Edgecry Coast, 46 Everlang Island, 13
  Eesistern Coast, 7 Eunorus Plains, 50 Salvage Archipelago) = tracks on grass; 0x12 (32 Lucid Plains, 31 Kognish
  Shores, 37 Falstead Coast) = tracks on brown earth; 0x15 (42 Blairsurpass Plains) = tracks on brown earth (Forgotten
  Continent side); 0x1C (53 Mitmakis Ice Field) = tracks on snow; 0x16 not present in the disc 3 world. Proposed
  `WM_TOPO_CHOCOBO_TRACKS_GRASS 0x03`, `_EARTH 0x12`, `_EARTH_2 0x15`, `_TRACKS_4 0x16`, `_SNOW 0x1C`. high (grass,
  snow), medium (0x12 vs 0x15), low (0x16).
- **Q4 WM_TOPO_NO_LANDING 0x2D / 0x2E: rocky plateau/canyon tops; the circle is the upside-down castle.** 0x2D in cells
  (4-9, 14-17), areas 43 Yuanshao Peninsula / 42; 0x2E in cells (11-15, 19-23), areas 41 Seaways Canyon / 45 Gusty
  Valley. Walker on 0x2D (47367, -132597): brown cliff-top plateau by the sea with the castle model below in a crater
  (`w2d_1.png`); on 0x2E (105130, -166230): sunset rock plateau (`w2e_1.png`). Circle (49120, -121408): the walker stands
  next to an upside-down castle in a crater ("Enter with X"; the field entered was 2500 "I. Castle/Entrance", Ipsen's
  Castle, but every teleport-then-Enter in this pass gave 2500, so that id is not position evidence; the model is).
  Circle on the airship (`land.py`): Circle over 0x2D / 0x2E cells did land, on a nearby 0x31 cell (wm_findGetoffPos
  searches around), so the refusal itself was not isolated. Proposed: keep `WM_TOPO_NO_LANDING(_2)`, comments
  "Forgotten Continent rock plateaus (0x2D around Ipsen's Castle, 0x2E Seaways Canyon / Gusty Valley)"; circle
  `WM_IPSEN_CASTLE_X/Z/R` if named. medium.
- **Topographies 0x30 / 0x33 (literals_runtime leftover): 0x30 waterfall, 0x33 river. CONFIRMED (medium).** Sweep: 0x30
  in 9 cells, 0x33 in 7. Near (322000, -192000) (`r37_wm_waterfall`, two waterfalls on screen) the 0x30 polygons run
  from y -6736 down to -684 at (316-331k, -176k..-190k): the falls and their basin; another flat 0x30 pool at y -3900
  (98000, -262000). 0x33 near (265653, -251608): a thin strip of 27 polygons rising from y -10 to -115 inland and
  ending at 0x35/0x36/0x37 water (a river mouth), and 42 polygons (313-319k, -188k..-204k) below the waterfalls
  (y -661..-100). Neither can be walked on (the walker never snaps). Proposed `WM_TOPO_WATERFALL 0x30`, `WM_TOPO_RIVER
  0x33`.
- **Q5 WM_STORY_TITLE_0..3: CONFIRMED.** `build/r41b/title.py V` (wm_enter.py with the story poke, write log on
  g_worldTitleId, 12 screenshots): 0x960 → title 0 "Mist Continent" (shown after the "Normal Navigation" help window
  closes), 0x1766 → 1 "Outer Continent", 0x2585 → 2 "Forgotten Continent", 0x26A2 → 3 "Lost Continent" (each written
  by wm_loadWorldData 0x800BAA54.., cleared by wm_drawTitle 0x800C107C ~950 frames later). Proposed
  `WM_STORY_TITLE_MIST`, `_OUTER`, `_FORGOTTEN`, `_LOST` (the continent names). high.
- **Q6 fog = the Mist. CONFIRMED (medium).** Same spot: story 0x1765 dark blue-grey haze over the Mist Continent map,
  0x1766 clear daylight (`title_6517_11.png` / `title_6617_11.png`; the data set changes at the same value, so
  fog and map switch together); disc 3 (0x28A0) clear; disc 4 (`r37b_wm_invincible_entry`, 0x2B5C) thick purple haze.
  0x2B53 could not be entered on disc 1 (the world init stalled). Proposed `WM_STORY_MIST_GONE 0x1766`,
  `WM_STORY_MIST_BACK 0x2B53`.
- **Q7 block effects.** Emitter tables read from the data pack (`g_worldDataPack` 0x800CF428 -> entry 0x28 + id):
  - `WM_BLOCKFX_DISC4_CORE 6` → **`WM_BLOCKFX_MEMORIA_ORB`. CONFIRMED.** `r37b_wm_invincible_entry` (disc 4), Invincible
    teleported to (196602, -82002): a huge purple glowing sphere with swirling shells over a mound, "???" location,
    "Enter with Circle", area 34 Pualei Plains (Iifa Tree) (`build/r41b/core_d.png`, state `r41b_wm_disc4_memoria_orb`).
    high.
  - `WM_BLOCKFX_SPINNER 7` → **`WM_BLOCKFX_WINDMILL`. CONFIRMED (visual).** Disc 4, Invincible at (283000, -210000):
    a round stone hut with wooden windmill sails in the mountains, area 2 Nolrich Heights (`spin_e.png`, state
    `r41b_wm_disc4_windmill`). Its emitter table: one type-1 entry at (282172, -6779, -205870). Rotation itself not
    filmed. high (object), medium (turning).
  - `WM_BLOCKFX_STORY 3`: emitters at (317649, -100, -71480), (312305, -50, -69732), (318280, -126, -66560), effect
    type 4, chance 32/4096 per frame. `cpass_wm_hadris` with story poked to 0x2500: wm_spawnEffect(…, type 4) from
    wm_updateBlockFx 3 times in 600 frames; area 39 = Kiera Desert (sand dunes, a pyramid-shaped rock). The effect was
    not visible in screenshots. Proposed `WM_BLOCKFX_KIERA_DESERT` (place) ; the effect stays type 4. medium.
  - `WM_BLOCKFX_CLAY 4`: not reached. Disc 1 world 9000 at story 0x1000 (`r41b_wm_d1_story1000`), walker near
    (229007, -207000), area 12 Vube Desert: no loaded block has an fxId there (wm_updateBlockFx / wm_drawBlockFx 0
    calls; ablation diff empty). Emitter table entry 4 is empty in the disc 3 pack. Still unknown.
  - `WM_BLOCKFX_GULUG_SUMMIT 2` (type 3 at (129057, -2400, -29156), chance 1024): not re-run (R37 could not see it).
- **Q8 WM_MUSIC_RESIDENT 0x64 → `WM_MUSIC_CHOCOBO`. CONFIRMED.** `r37b_wm_chocobo_ride`: GameData.musicId = {0x45,
  0x64}, musicBank {0x13B, 0x17B} (slot 1 = the resident song, playing while riding); g_worldSongMusicIds[1]
  (WM_SONG_CHOCOBO) = 0x64. high.
- **Q9 g_worldSongMusicIds is u8[6] = 45 64 70 2D 5F 60. CONFIRMED** on disc 1 (world 9000, story 0x1000), disc 3
  (`cpass_wm_hadris`, playing 0x45; `cpass_wm_airship9` playing 0x70) and disc 4 (`r37b_wm_invincible_entry`, playing
  0x2D). Proposed snd_ids.h: `MUSIC_WORLD_MAP 0x45`, `MUSIC_CHOCOBO 0x64`, `MUSIC_AIRSHIP 0x70`, `MUSIC_WORLD_DISC4
  0x2D`, `0x5F` / `0x60` = WM_SONG_FIELD1 / 2 (not heard). The ids are the same in the overlay image (no runtime fill).
  high.
- **Q10 EVT_GB_WM_FILE: = world file index (worldId - 9000). CONFIRMED.** `tools/rt/wm_force.py 9000..9012 --disc 3`:
  b[0x66] (0x8007AF56) = 0..0xC after each init, written by the world script (write bp: evt_writeVar from
  evt_runExpr). Known files: 0 = disc 1 Mist-continent world (Ice Cavern exit), 3 = boat era (`r37b_wm_boat_entry`,
  story 0x24B8), 7 = disc 3 free world (`cpass_*`), 8 = disc 4 (`r37b_wm_invincible_entry`). Rain spot (247888,
  -186968): `cpass_wm_hadris` with b[0x66] poked to 8, walker at (247888, -192000): rain falls in front of a round
  stone building in a grassy basin (area 9 Daines-horse Basin, `rain8.png`); with 7 no rain (`rain7.png`).
  Proposed `WM_FILE_DISC1 0`, `WM_FILE_DISC4 8`. medium (which story spans the two files cover).

### ovl_0a
- **Q11 FLD_SOUND_BANK_NO_UPLOAD 0x84 → `SND_BANK_COMMON` (the resident system SE bank). CONFIRMED.** main
  `game_loadCommonAkao` uploads DB_SOUND_BANK | 0x84 once; offline the bank is AKAO id 0, SPU 0x71100, slot 128,
  128 instruments; at runtime (battle traces above) the menu cursor SE keys instrument 133 and battle/spell SE use
  128-175, all inside this bank. So the field never re-uploads it because it is always resident. Not seen requested by
  a field in this pass. high (identity), medium (field use).
