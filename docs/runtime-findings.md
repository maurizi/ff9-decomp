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
| FF 00 | BGI_FLOOR_MOVE_START | u8, u8 | bgi_startFloorMove | 16 calls / 1 fields; field 404 |
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

