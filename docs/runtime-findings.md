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

## Battle sequence (enemy action script) interpreter  (status: applied except btlseq_opReleaseEffect, btlseq_opSetStance (low-medium))
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

## Status application, ticks and looks  (status: applied except btl_makeGreyCluts (low-medium))
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

## Battle sound  (status: applied except btl_setUnitBrightness (low-medium))
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

## Status bits confirmed at runtime  (status: proposed)
- binary ovl_0b; btl_addStatus a1 traced from btl_addStatuses+0x48 while the patched spell hit:
  Regen 0x00040000 (Vivi), Vanish 0x04000000 (Vivi), Auto-Life 0x00002000 (Vivi), Freeze 0x02000000 (Frost on the
  Malboro). Bad Breath added Blind, Confuse, Poison, Slow and Mini in one call each. Mighty Guard added Shell and Protect
  to all four. Bio, Frog Drop and Mustard Bomb added nothing (Malboro, and Vivi for Mustard Bomb), so Heat stays
  code-only.
- Proposed: `BTL_STATUS_REGEN 0x00040000` (runtime; today only `UNK38_REGEN` exists); mark VANISH and AUTO_LIFE as
  runtime. In t_regen, btl_initDotTimer(Vivi, 0x12) runs from btl_addStatus+0x774 and btl_tickRegen runs 3 frames later
  from btl_updateUnitStatus+0x1AC. Regen effect id 0x20, file 0x20.
- confidence: high.

## Scan, Detect, Reflect, defeat, flee  (status: proposed: upgrades of existing names to [runtime])
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

## Matra Magic: func_800B8FF8  (status: proposed)
- func_800B8FF8 → btl_setTargetHp (`void (BtlCalc *)`): sets the target's HP to the ability parameter (unk8->unk8[5]),
  or kills it (HP 0, death cry, KO motion) when the parameter is 0; fails on KO targets. Runtime: only in u_matra, from
  btl_calcCmd+0xE1C. After it the Malboro got LOW_HP (btl_addStatus 0x200 from btl_updateHpStatus+0xAC), i.e. HP 1.
- confidence: medium-high.

## Summons  (status: proposed)
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

## Trance entry  (status: proposed: existing names now [runtime])
- With the party's trance gauges poked to 0xFE (+0x78) and the Malboro left to act (`g_trance4`, deterministic):
  btl_gainTrance is called 2x from btl_applyCalcResult+0x318 when "Virus Tentacles" hit. Eiko's gauge overflowed, so
  btl_addStatus(Eiko, 0x4000) ran from btl_gainTrance+0x1B4. Effect 0x101 (the trance transformation) followed.
  btl_gainTrance only runs for enemy → party damage, so a party Attack on Vivi doesn't trigger it.
- Trance-only functions in that run: func_800AB3E4 (→ func_800ABBC4(unit, 0x17 / 0x8017), 12 calls from
  btlfx_runScript+0x68 / +0x684, reads CD via cd_readChunks: probably the trance model swap), func_800BD698,
  func_800C0D24 → func_800C2344 → func_800C8D18. Leads only.

## Battle entry (from the ovl_0b entry point)  (status: proposed)
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

## Unit shadows  (status: proposed)
- func_800AC37C → btl_drawShadow (`u8 *(Battle11C028 *, s32 slot, s32, s32 bones, u8 *prim)`, 4 semi-transparent FT4
  quads, macro SHADOW_QUAD in the C) and func_800AD4E4 → btl_calcShadowMatrix (orientation from one bone or between
  two bones). Runtime: both run per unit per tick from btl_updateUnitModel+0x544. Stubbing either removes the dark
  ellipses under all units (abl_model, 0.51 % of pixels; build/rt3/abld_model.png). Stubbing func_800C7B84,
  func_800C7CC8, func_800C9040 or func_800C1A8C changed nothing at idle. high / medium-high.

## Effect emitters and wrappers (ovl_0b D0B18-D5C30)  (status: proposed)
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

## ovl_65: victory rewards vs game over  (status: proposed)
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
