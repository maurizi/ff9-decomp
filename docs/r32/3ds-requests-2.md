# R32b: answers for the 3DS port (what the player points at in each menu screen)

Answers from the matched C (paths relative to the repo, line numbers at commit 893287ee), checked in PCSX-Redux on
disc 1 with the existing states `menu_items`, `menu_equip`, `menu_ability`, `menu_shop_weapons`, `menu_save`,
`field_dali_inn_moogle` and `field_worldmap_ice_exit`. The runtime evidence is also in `docs/runtime-findings.md`
("R32b 3DS requests").

Common facts:
- Every menu overlay loads at 0x801EDF00, and **file offset in the dir-0 overlay = vram - 0x801EDF00** for text and
  data alike (e.g. ovl_3c `s_shopModes` 0x801FBBA0 is at 0xDCA0 of `3c.bin`). Overlay symbols only mean something
  while that overlay is resident.
- Which screen is up: `g_menu` (main 0x80077DB0, `MenuState`, ff9.h:661): `+0 state` (3 = running), `+1 type`
  (0 main menu, 2 shop, 4 save/load, 1 name entry, 3 party select, 5 chocograph, 8 disc change), `+2 cardSlot`,
  `+0xC arg`. For type 0, `g_menuScreen` (0x80077E50, `{u8 active, u8 slideIn, u8 screen, u8 member}`): screen
  1 Item (ovl_37), 2 Ability (ovl_33), 3 Equip (ovl_33), 4 Status, 6 Card, 7 Config.
- `GameState *g_gameState` = *(u32 *)0x8006794C; `GameData *gd` = *(gameState + 0x1C) (0x8008B250 at runtime).
  `gd + 0x3C` `Character characters[9]` (0x90 each), `gd + 0x54C` `Character *party[4]`, `gd + 0x560` `u32 gil`,
  `gd + 0x574` `ItemSlot items.slots[256]` (`{u8 id, u8 count}`, count 0 = empty, id 0xFF = none).
- `MenuList` (ff9.h:825, 0x10 bytes): `+0 s16 count`, `+2 u8 rows`, `+3 visibleRows`, `+4 cols`, `+5 top` (first
  visible row), `+6 cursor` (row on screen), `+7 col`, `+8 scroll` (MENULIST_SCROLL_*: 1 down, 2 up, 4 page,
  8 wrap-col), `+9 scrollPos`. Highlighted entry = `(top + cursor) * cols + col`.
- Text: FF9 encoding (`tools/ff9text.py decode`), 0xFF ends a string, 0xF7 is a newline, 0xF9/0xFA take an argument
  byte.
- The game itself only fills `g_menuHelp.text` (0x80077DF0) while the Select help is open (help callbacks), except
  the Item screen, which recomputes it every frame. A port should compute the pointed-at thing itself from the state
  below and not rely on `g_menuHelp.text`.

## 1. Item screen (ovl_37)

**Short answer.** `s_menuItem` (ovl_37 0x801FAB78, `ItemMenu`, include/ovl_37.h:31). When `state == 2` (LIST) and
`tab == 0` (Use), the row is `slot = (itemList.top + itemList.cursor) * 2 + itemList.col` and the item is
`gd->items.slots[slot]` (count 0 = empty row). With `tab == 2` (Key) the key item id is
`s_menuItem.keyItems[(keyList.top + keyList.cursor) * 2 + keyList.col]` (valid while `< keyCount`). Help text =
`s_menuItemHelp + g_itemTable[id].help`. `s_menuItemHelp` is a 0x33B4-byte blob in the overlay's own data
(0x801F3168, file offset 0x5268 of `37.bin`), FF-terminated strings. ovl_33 (`s_menuEquipItemHelp` 0x801F8990) and
ovl_3c (`s_shopItemHelp` 0x801F87EC) carry byte-identical copies.

### Details
- `ItemMenu` offsets: `+0 tab` (0 Use, 1 Arrange, 2 Key), `+1 state` (ITEM_STATE_*: 0 busy/animating, 1 tab row,
  2 list, 3 picking the target character, 4 Auto/Manual window, 5 manual arrange, 6 key item picture view),
  `+2 slot` (target party slot in state 3), `+6 swapFrom`, `+7 swapPicked`, `+0xC MenuList itemList` (count 0xFF,
  2 columns, 11 visible rows), `+0x1C MenuList keyList`, `+0x54 keyView`, `+0x56 keyViewId` (key item shown in
  state 6), `+0x60 u8 keyItems[]`, `+0xB0 s32 keyCount`.
- `menuItem_getSlot(MenuList *l)` (src/ovl_37/ovl_37.c:830) is the exact index the game uses for help. During a
  wrap-to-next-row scroll (`scroll & 8`) it already returns the destination: `(top + cursor -/+ 1) * 2 + (col ^ 1)`
  (-1 if `scroll & 2`). Otherwise `(top + cursor) * 2 + col`.
- `menuItem_updateHelp` (ovl_37.c:344): state 1 → tab help text `text_get(tab + 5)`; state 2 + tab 0 or state 5
  (manual arrange, list `s_menuItemTabs[1].list` = itemList) → `p = &gd->items.slots[getSlot]`, if `p->count`:
  `text = &s_menuItemHelp[g_itemTable[p->id].help]`; state 2 + Key tab → `&s_menuItemKeyHelp[g_keyItemTable[keyItems[i]].help]`;
  state 3 → "Select character."; state 4 → Auto/Manual help.
- `ItemInfo` = `g_itemTable` (main 0x80069018, 0x12-byte records, ff9.h:551): `+0 u16 name` (into `g_itemNames`
  0x8006A218), `+2 u16 help` (into the overlay's help blob), `+4 u16 price`, `+6 u16 equipMask`, `+8 icon`,
  `+9 iconPalette`, `+0xA rank`, `+0xB statBonus`, `+0xC u8 abilities[3]`, `+0xF slots` (bit 0 usable from the menu,
  0x08 accessory, 0x10 body, 0x20 head, 0x40 arm, 0x80 weapon), `+0x10 sortKey`.
- Key items: `g_keyItemTable` (main 0x8006B374, 8 bytes: `+0 s16 name` into `g_keyItemNames` 0x8006ADAC,
  `+2 s16 help` into `s_menuItemKeyHelp` ovl_37 0x801F651C, `+6 s16 desc` into `s_menuItemKeyDesc` ovl_37 0x801F79F8).
- Getting the bytes: either read RAM while ovl_37 is resident, or read `37.bin` from FF9.IMG dir 0 at
  `0x5268 + help` (help blob ends at 0x801F651C, i.e. 0x861C). Example: item 0xEC Potion, help 0x3081 = "Restores
  150 HP.\nThe effect decreases\nif used in the field.".

### Runtime evidence (`menu_items`, disc 1)
- `read 0x801FAB78 0x30`: tab 0, state 2, itemList = `ff00 80 0b 02 00 00 00` (count 0xFF, rows 0x80, 11 visible,
  2 cols, top 0, cursor 0, col 0); `g_menuHelp.text` = `s_menuItemHelp+0x3081` (Potion, the highlighted row).
- `press right`, `press down` x2: itemList cursor 2, col 1 → slot 5; `gd->items.slots[5]` (0x8008B7CE) = `{0xF9, 2}`
  (Phoenix Pinion, the screenshot's highlighted row), `g_menuHelp.text` = `s_menuItemHelp+0x3229` =
  `g_itemTable[0xF9].help`.

## 2. Equip screen (ovl_33)

**Short answer.** `s_menuEquip` (ovl_33 0x801FC840, `EquipMenu`, include/ovl_33.h:96). `equipSlot` (+2) is the slot
under the cursor (0 weapon, 1 head, 2 arm, 3 body, 4 accessory), `state` (+1) 1 = tab row, 2 = picking a slot,
3 = candidate list open and taking input (0 = busy while the list window slides); `listOpen` (+0x260) is 1 from the
open until the close animation ends. Highlighted candidate: `s_menuEquip.items[list.top + list.cursor].id`
(`items` at +0x3C, `ItemSlot[]`, list at +4, one column; empty list when `list.rows == 0`). Before/after stats:
main `chr_calcEquipStats(EquipStatsIn *in, EquipStatsOut *out)` (0x80032E84, src/main/chr.c:68), fed with the
member's current data and the candidate in `in.equip[equipSlot]`.

### Details
- `EquipMenu`: `+0 tab` (0 Equip, 1 Optimize, 2 Off), `+1 state`, `+2 equipSlot`, `+3 slot` (party slot shown;
  Character = `gd->party[slot]`), `+4 MenuList list`, `+0x3C ItemSlot items[256]` (built by
  `menuEquip_buildItemList`, menu_equip.c:483: inventory items with `g_itemTable[id].equipMask & charMask` and
  `.slots & slotMask[equipSlot]`, sorted by `rank` then id), `+0x23C WinDef listWin`, `+0x260 listOpen`,
  `+0x261 savedState`.
- `menuEquip_drawInfo` (menu_equip.c:114) is the comparison:
  - current: `stats = {speed, strength, magic, spirit}` (Character +0x24..+0x27), `defs = {g_weaponTable[equip[0]].attack,
    defense, evade, magicDef, magicEva}` (Character +0x28..+0x2B; `g_weaponTable` main 0x8006E0E8, 12-byte records,
    attack at +5 per the symbol comment).
  - new (state 3 and `list.rows != 0`): `in.base` = the 8 bytes at Character +0x2C (`u16 baseMaxHp, u16 baseMaxMp,
    u8 baseSpeed, baseStrength, baseMagic, baseSpirit`), `in.support` = Character +0x88 (`u32[2]` support bits),
    `in.hp/in.mp` = Character +0x10/+0x12, `in.equip` = Character +0x39 (`u8[5]`), then
    `in.equip[equipSlot] = candidate id`; `chr_calcEquipStats(&in, &out)`.
  - Off tab (`tab 2, state 2, equipSlot != 0`): the same with `in.equip[equipSlot] = 0xFF` (removal preview).
- `EquipStatsIn` (ff9.h:3385, 0x1C): `+0 u8 base[8]`, `+8 u32 support[2]`, `+0x10 u16 hp`, `+0x12 u16 mp`,
  `+0x14 u8 equip[5]`. `EquipStatsOut` (ff9.h:3393, 0x16): `+0 u16 hp`, `+2 mp`, `+4 maxHp`, `+6 maxMp`,
  `+8 u8 stats[4]` (speed, strength, magic, spirit, capped by `g_chrStatCaps`), `+0xC u16 defs[5]` (attack, defense,
  evade, magicDef, magicEva, capped at 999). It returns `out`. It only reads tables (`g_weaponTable`, `g_armorDefs`,
  `g_equipStatBonus`, `g_itemTable`) and the support bits for Auto HP/MP+ (`chr_testSupportBit`), so a port can
  call it any time with a stack in/out.
- Colours (`menuEquip_drawStatCompare`, menu_equip.c:314): new > cur green, new < cur red, equal white.
- Help (`menuEquip_updateHelp`, menu_equip.c:526): slot state → `s_menuEquipItemHelp + g_itemTable[c->equip[equipSlot]].help`
  ("Nothing is equipped." for 0xFF); list state → the candidate's help.

### Runtime evidence (`menu_equip`, Zidane Lv 99)
- Cross, Down: `s_menuEquip` = `00 02 01 00` (tab 0, state 2, equipSlot 1 = head). Cross, Down: `00 03 01 00`, list
  `0400 04 06 01 00 01` (4 candidates, cursor 1), items `70 03 72 02 73 04 75 01` → candidate 0x72 Feather Hat, the
  highlighted row in the screenshot; `listOpen` = 1.
- `trace chr_calcEquipStats`: once per frame from `menuEquip_drawInfo+0x1DC`. `until menuEquip_drawInfo+0x1DC`:
  in = `ac11 2d01 20 32 2f 25 | 00008000 40402200 | 0f27 e703 | 08 72 5a 98 ff` (head replaced by 0x72), out =
  stats 20 32 2f 27, defs 0x18 9 0xA 8 7 = Speed 32, Strength 50, Magic 47, Spirit 39, Attack 24, Defense 9,
  Evade 10, Magic Def 8, Magic Eva 7, exactly the right-hand column on screen.

## 3. Ability screen (ovl_33)

**Short answer.** `s_menuAbility` (ovl_33 0x801FBF84, `AbilityMenu`, include/ovl_33.h:69). `tab` (+0) 0 = action
("Use") list, 1 = support ("Equip") list; `state` (+1) 2 = list has the cursor (1 tab row, 3 target window,
4 target window sliding, 5 member switch, 0 busy). Highlighted index `n = (lists[tab].top + lists[tab].cursor) * 2
+ lists[tab].col` (lists at +8 and +0x18), ability id = `actions[n].id` (+0x668, 4-byte `{u8 id, u8 cmd, pad}`)
on tab 0, `supports[n]` (+0x728, u8) on tab 1 (ids 0xC0-0xFF). AP: `chr_getAp(chrId, id)` (main 0x80034584,
returns -1 if not in the character's row) and `chr_getApNeeded(chrId, id)` (main 0x800347D8, u8, 0 if absent), with
`chrId = Character.info & 0xF` (word at +0x34). The INFO window draws the gauge only if
`chr_canLearnAbilities(c)` (main 0x80034A30) and the ability's state is non-zero.

### Details
- `menuAbility_drawInfo` (src/ovl_33/menu_ability.c:225): returns early (no AP shown) for state 1/5/0 or an empty
  list; `id` as above; `ok = menuAbility_getActionState(id, c)` (menu_ability.c:476) or `getSupportState` (:846):
  0 hidden (not learnable/equipped and no AP yet), 2 greyed, 3 usable, 4 support equipped. Then
  `ui_drawApGauge(.., chr_getAp(c->info.b.id, id), chr_getApNeeded(c->info.b.id, id), flag, 0)`; flag = `!(actionTable[id].flags & 2)`
  for actions, 1 for supports. Then the equipped items that teach it: `equipSlots[id]` (+0x68, bit per equip slot)
  and `equipItems[id][slot]` (+0x168, `u8[256][5]`).
- `chr_getAp` / `chr_getApNeeded` (src/main/chr.c:610/645): slot = index of `id` in `g_abilityRows[row]`
  (main 0x8006CB28, `AbilityEntry {u8 id, u8 ap}[16][48]`, row = `(Character.info >> 12) & 0x1F`, < 16); AP =
  `characters[chrId].ap[slot]` (Character +0x58), needed = `g_abilityRows[row][slot].ap`. "Learned" = ap >= needed.
- Counts: `actionCount` +0x769, `supportCount` +0x76A; `slot` (+2) = party slot shown; `target` (+3, | 0x80 = all) in
  state 3.
- Names / help (all in ovl_33): actions `s_menuAbilityActionTable` (0x801F34EC, 0x10-byte records, name offset +0xE
  into `s_menuAbilityActionNames` 0x801F40EC, MP +0xA, flags +0xB), help `s_menuAbilityActionHelp` (0x801F4734) +
  `s_menuAbilityActionHelpOfs[id].helpOffset` (0x801F6240, 4-byte records); supports `g_supportAbilities` (main
  0x8006D128, 8 bytes per id - 0xC0: `+1 stones`, `+2 u16 name` into `s_menuAbilitySupportNames` 0x801F6540,
  `+4 u16 help` into `s_menuAbilitySupportHelp` 0x801F67D4).

### Runtime evidence (`menu_ability`, Zidane Lv 99)
- Cross, Right, Down: `s_menuAbility` = tab 0, state 2, lists[0] `0800 04 04 02 00 01 01` (8 abilities, cursor 1,
  col 1) → n = 3; `actions` = 65 66 67 68 69 6a 6b 6c → id 0x68, the highlighted "Soul Blade" in the screenshot.
  `trace chr_getAp chr_getApNeeded`: `chr_getApNeeded(0, 0x68)` from `menuAbility_drawInfo+0x1C8` every frame.

## 4. Shop (ovl_3c)

**Short answer.** `s_shop` (ovl_3c 0x801FC2D0): `kind` (+0) 0 items, 1 equipment (Buy/Sell/Exit), 2 synthesis
(Synth/Exit); `cursor` (+1) is the menu option; `inMode` (+3) = 1 while a mode runs. Mode = `s_shopModes[kind][cursor]`
(0x801FBBA0): kind 0/1 cursor 0 Buy, 1 Sell; kind 2 cursor 0 Synth. Highlighted item:
- Buy: `s_shopBuy` (0x801FC2F8): `items[list.top + list.cursor]` with `u8 *items` at +0x3C (= `s_shopItems[shopId]`,
  0x801FBC44, 32 ids per shop, 0xFF ends), list at +0x28 (1 column). `state` (+0) 1 list, 2 quantity window.
- Sell: `s_shopSell` (0x801FC438): `gd->items.slots[(list.top + list.cursor) * 2 + list.col]` (list at +4, 2 columns
  over the whole inventory, count 0 = empty). `state` (+0) 1 list, 2 quantity.
- Synth: `s_shopSynth` (0x801FC338): `recipes[list.top + list.cursor]` (+0x3C, 6-byte `SynthRecipe {u16 price,
  u8 ingredients[2], u8 product, u8 shopMask}`), product = the item; list at +0x28.

### Details
- `shop_update` (src/ovl_3c/ovl_3c.c:135) runs `s_shopModes[kind][cursor][1]` while `inMode`, otherwise the top menu.
  `shop_init` (:119): `id = g_menu.arg` (+8), `kind = shop_getKind(id)` (>= 0x20 synthesis).
- Help: `shopBuy_updateHelp` (:646), `shopSynth_updateHelp` (:1101), `shopSell_updateHelp` (:1429) all use
  `s_shopItemHelp + g_itemTable[id].help` (0x801F87EC, the same blob as ovl_37's). They run only while Select help
  is open.
- `s_shopModes` data: row 0 = {shopBuy_init/update/exit}, {shopSell_*}, NULL; row 1 the same; row 2 {shopSynth_*}, NULL, NULL.

### Runtime evidence (`menu_shop_weapons`, shop 0)
- Top menu: `s_shop` = `01 00 03 00` (kind 1, cursor 0, 3 options). Cross (Buy), Down x2: `01 00 03 01` (inMode 1),
  `s_shopBuy` state 1, count 0x10, list `1000 10 07 01 00 02` (cursor 2), items → 0x801FBC44 =
  `01 02 10 11 39 46 ...`, items[2] = 0x10 Broadsword (screenshot: hand on Broadsword).
- Circle, Down, Cross (Sell), Right, Down: `s_shop.cursor` = 1, `s_shopSell` list `ff00 80 0b 02 00 01 01` → slot 3,
  Elixir in the screenshot.

## 5. Save screen (ovl_39)

**Short answer.** `s_menuSave` (ovl_39 0x801FB1A8, `SaveMenu`, include/ovl_39.h:348). `state` (+1): 1 = picking the
memory card slot, with the slot in `g_menu.cardSlot` (0x80077DB2, 0 = slot 1, 1 = slot 2); 2 checking, 3 yes/no,
4 = picking a block (file list), 5 saving/loading, 6 "Data saved." Highlighted block = `list.top + list.cursor`
(list at +0x24, 15 rows, 3 visible, 1 column) = card file row 0-14 ("FILE01".."FILE15"). The summary is
`s_menuSave.files[row]` (+0x34, 0x98-byte `SaveFile`), copied from each save's `SaveHeader` (the 0x80 bytes after the
0x100-byte card header) by `menuSave_copyHeader` when the card is checked; `menuSave_buildHeader` writes it when
saving.

### Details
- `SaveMenu`: `+0 mode` (0 save, 1 load, 2 next-disc save), `+1 state`, `+2 phase` (2 = input runs), `+3 file`
  (row selected with Cross), `+4 cardStatus[2]`, `+0x10 newestFile`, `+0x24 MenuList list`, `+0x34 SaveFile files[15]`.
- `SaveFile` (ovl_39.h:329): `+0 s32 version` (0x30005 valid), `+4 fileName` ("BASLUS-0125100000-NN", NN = row),
  `+0x1A state` (1 empty block, 2 other game, 3 corrupt, 4 FF9 save), `+0x1B index`, `+0x1C discs` (bit n = disc
  n+1), `+0x1D level` (leader), `+0x1E name[10]` (leader, FF-terminated, cut at 7), `+0x28 location[0x1C]` (text),
  `+0x44 cursor`, `+0x48 windowColor`, `+0x4C counter`, `+0x50 s32 playTime` (frames at 60/s), `+0x54 s32 gil`,
  `+0x58 SaveMember party[4]` (0x10 each: `+0 present`, `+1 portrait` (0xFF none, `ui_drawPortraitId` id), `+2 name[10]`,
  `+0xC level`).
- Loader: `menuSave_openCheck` → `menuSave_readDir` (src/ovl_39/ovl_39.c:832: FF9 files go to the row from their name
  digits, other games' blocks fill empty rows from the end) → for each FF9 file `menuSave_readHeader` (:1109) reads
  0x180 bytes (`CardFileHeader` 0x100 + `SaveHeader` 0x80) into `s_menuSaveCheck.buf` and `menuSave_onHeaderRead`
  (:887) → `menuSave_copyHeader(&files[file], buf + 0x100)` (:1122). `menuSave_findNewest` marks wrong versions as
  corrupt and `menuSave_enterFileList` (:909) puts the cursor on the newest save.
- `SaveHeader` (on the card at file offset 0x100, ovl_39.h:230): `+0 version`, `+4 discs`, `+5 level`, `+6 name[10]`,
  `+0x10 location[0x1C]`, `+0x2C playTime`, `+0x30 gil`, `+0x34 party[4]`, `+0x74 windowColor`, `+0x78 counter`,
  `+0x7C cursor`. Built by `menuSave_buildHeader` (:1834): leader = the player actor's model slot character,
  location = `gd->locationName` cut to 0x1C bytes.
- Drawing: `menuSave_drawFileInfo` (:148): "/DISC" + digits, portraits, name, "Lv", time
  (`text_formatTime(playTime)`), gil, the location box.

### Runtime evidence (`menu_save`, card 1)
- Down (slot 1, `g_menu.cardSlot` 1 → 0), Cross, 320 frames: `s_menuSave` = mode 2, state 4, list
  `0f00 0f 03 01 03 02` (top 3, cursor 2) → row 5, newestFile 5; the screenshot has the hand on FILE06 (Crystal World,
  21:28:08). `files[5].playTime` = 4637282 = 77288 s × 60 = 21:28:08; gil 9999999.
- `files[0]` (0x801FB1DC): version 0x30005, "BASLUS-0125100000-00", state 4, discs 1, level 99, name "Zidane", ...

## 6. Mognet

**Short answer.** There is no Mognet overlay or screen: it is all **field event script** with ordinary message
windows (only ovl_0a is resident). Talking to a moogle opens msg 0x08 (window 2, "Can I help you, kupo? / Save / Tent /
Mognet / Cancel"); Mognet → msg 0x2C ("What do you want to do? Read mail / Cancel"), then a choice window with one row
per letter ("Which letter do you want to read? / Trapped in Ice! / Cancel"). The cursor of every choice window is
`g_msgChoice` (main 0x8007B958, s32 row), with `g_msgChoiceCount` (0x8007B95C) rows and `g_msgChoiceEnabled`
(0x8007B960) as the enabled-row mask. The letter itself is another message of the **field's message table**
(`g_msgTable`, 0x8007B954; e.g. msg 0x2E in window 3, flags 0x10 frameless, "From Mois to Gumo / Yo! Man, I got...").
Which message a row opens is decided by the moogle's script, so the port can only learn it when the script opens it:
hook `msg_open(msg, windowId, layout, flags)` (main 0x8004DCEC) or read the open window's `MsgWindow.msgId`.

### Details
- `g_msgWindows` (main 0x8007B9C0, 10 × 0x2C `MsgWindow`, ff9.h:1732): `+0xA s16 id`, `+0x1E s16 msgId`,
  `+0x28 flags` (MSGW_*, 2 = has a choice). `g_msgChoiceWindow` (0x8007B970) = window id of the open choice.
- Message text: `msg_getText(id)` (main 0x8004F058, src/main/msg.c:1254): in field/world mode `table = g_msgTable`,
  `text = (u8 *)table->offsets + (u16)((u32 *)table->offsets)[id * 2 + 1]` (`table + 4` is the offset array, pairs of
  u32 per message), in battle `[id]`. Messages use text codes; the first messages of a table that start with
  `F9 04` are text banks (see 7).
- Scripts set the choices with op 0x7C (MSG_CHOICE_MASK) and read the result through a system variable after the
  window closes (as for the ATE window in 3ds-requests.md section 3).

### Runtime evidence (`field_dali_inn_moogle` → state `build3ds2_mog_choice` with the Mognet row highlighted)
- Cross on the moogle: `g_msgChoiceCount` 4; Down x2: `g_msgChoice` 2 (Mognet). Log breakpoint on msg_open, then
  Cross, Cross, Cross: calls `(0xC, 5, 0x8007B940, 8)`, `(8, 2, .., 8)`, `(0x2C, 2, .., 8)`, `(0x2E, 3, .., 0x10)`,
  all from `evt_execInstr` (ra 0x8003C264); `ovl` shows only ovl_0a. Screens: "Read mail / Cancel", "Which letter
  do you want to read? / Trapped in Ice!", then the letter on a darkened screen. `g_msgWindows[3].msgId` = 0x2E; the
  text at `msg_getText(0x2E)` (0x800D3293) starts `F9 02 ... "From"`.

## 7. World map navigation map (ovl_0c, Select view)

**Short answer.** Place names are not drawn by the navi code. `wm_naviUpdate` stores the snapped place as
`g_evtGlobalVars.b[0x67]` (0x8007AF57) = **place index + 1** (0 = cursor on no known place), and the world script
shows it: `msg_open(3, 4, .., 4)`, message 3 of the world message table = `F9 45 F9 0C F9 06 00 FF`; code `F9 06 00`
inserts entry `g_msgNumbers[0]` (= b[0x67], set by the script) of text bank 0 (`g_msgTextBanks[0]`, main 0x8007BB80
= world message 0 + 2). Bank 0 holds 61 names: entry k = place k-1 (0 "Dummy", 1 "Alexandria Harbor", 2 "Alexandria",
3 "Evil Forest", 4 "Ice Cavern", ...). Screen position of a place = `(mapX + 40, mapY + 9)`: its marker is a 4×4
quad at `(mapX + 39, mapY + 8)` (centre about +41, +10). `g_worldNaviPlaces[page][i]` with page =
`g_worldNaviPage` (0x800D3E74, 1 once the story counter is >= 0x1766); a place is shown if known
(`wm_naviIsPlaceKnown(i)`: bit `i & 15` of `g_evtGlobalVars.h[0x2E + i / 16]`, 0x8007AF4C..0x8007AF52, and
mapX | mapY != 0), and on page 1 places 7-10 and 15 are skipped.

### Details
- `g_worldNaviMode` (0x800D3E60): 1 mini map, 2 full map (Select). `g_worldNaviActive` 0x800D3E80,
  `g_worldNaviVisible` 0x800D3E64 (0 while Circle is held with the cursor on), `g_worldNaviCursorDraw` 0x800D3E77
  (script request WM_REQ_CURSOR_DRAW), `g_worldNaviCursorOn` (script WM_REQ_NAVI_CURSOR; stick input only when set).
- `WmNaviPlace` (include/ovl_0c.h:705, 0xC bytes): `+0 s16 mapX`, `+2 s16 mapY` (full-map pixels, 0..240 × 0..200),
  `+4 s32 worldX`, `+8 s32 worldZ` (autopilot target). `g_worldNaviPlaces` = 0x800CD10C, `[2][64]`.
- Cursor: `g_worldNaviCursorX/Y` (0x800CD104/0x800CD108, map pixels << 12, wrap at 240/200). The hand is drawn at
  `((cx >> 12) + 18, (cy >> 12) + 6)` (`pen_drawSprite(1)`), so its fingertip is on `(cx >> 12) + 40, (cy >> 12) + 9`.
- `wm_naviUpdate` (src/ovl_0c/tu_800BA528.c:1534): per frame, for every known place (page-1 exceptions as above)
  within `g_worldNaviSnapDist` (0x800CD70C = 14000, ~3.4 px, in << 12 units) on both axes the cursor moves 1/8 of the
  way to it; when within 1 px on both axes, `g_worldNaviPlace` (0x800D3E89) = i and `b[0x67] = i + 1`, otherwise
  `b[0x67] = 0`. L1/R1: with b[0x67] = 0 it jumps to the nearest known place; when on a place it cycles through the
  known places (`(place ± 1) & 63`).
- Drawing (`wm_naviDraw` :1470, mode 2): `wm_naviDrawPlaces` (:2037) calls `wm_naviDrawMarker(mapX, mapY, 1, 1, c0,
  c1, 16, 40, 9, 1)`; `wm_naviDrawMarker(x, y, hw, hh, colA, colB, pulse, dx, dy, textured)` (:1670) with
  `textured` puts two semi-transparent 4×4 POLY_FT4s at `(x + dx - 1, y + dy - 1)`, uv (0,250)-(4,254), tpage 704;
  the colour pulses between colA and colB with `cos(frame * pulse)`. The map itself: `wm_naviDrawFullMap` tiles at
  x 0x28..0x118, y 9..0xD1 (40..280 × 9..209); the frame at (32,1)-(288,217). Vehicles and the player arrow use the
  same +40/+9 after `wm_naviWorldToMap(x, z, .., page * 2 + 1)` (:2150): page 0 `mapX = (x - 0x30000) / 819 * 14 / 16
  + 31`, `mapY = (-0x22000 - z) / 819 * 14 / 16 + 12`; page 1 `mapX = x / 1638`, `mapY = -z / 1638`.
- So a tap at screen (sx, sy) in the PS1's 320-wide frame → map `(sx - 40, sy - 9)`; nearest known i by distance to
  `(mapX, mapY)`. To make the game select it, write `g_worldNaviCursorX/Y = mapX << 12, mapY << 12`; the next
  `wm_naviUpdate` snaps and sets b[0x67], and the script redraws the name.
- Name bank layout: `g_msgTextBanks[0]` points at `u8 count` followed by `u16 offsets[count]` relative to the byte
  after the count; a name ends at 0xF7 (or 0xFF). It is world-script text (`g_msgTable` world table, message 0),
  loaded with the world map.

### Runtime evidence (`field_worldmap_ice_exit` → state `build3ds2_wm_navi`, full map open)
- Select: `g_worldNaviMode` 1 → 2, `g_worldNaviCursorDraw` 1; screenshot: the full map with "Ice Cavern" in a window
  at the top left; `g_worldNaviPlace` 3, b[0x67] 4.
- Log breakpoint on msg_open, R1: place 2, b[0x67] 3, "Evil Forest", call `msg_open(3, 4, 0x8007B940, 4)`; every
  later R1/L1 made the same call with msg 3. `g_msgNumbers[0]` (0x8007BBA8) follows b[0x67] (15 for place 14, 4 for
  place 3). World `g_msgTable` 0x8011973C, msg 3 at 0x80119DFA = `F9 45 F9 0C F9 06 00 FF`; `g_msgTextBanks[0]` =
  0x80119A12; its 61 entries decode as listed.
- `write g_worldNaviCursorX/Y = 0x98000 / 0x53000` (2 px off Ice Cavern, mapX 150, mapY 81): after 40 frames the
  cursor is 0x96024 / 0x51024 (pulled onto the place), `g_worldNaviPlace` 3, b[0x67] 4, name "Ice Cavern". The same
  with Alexandria Harbor's coordinates (unknown place, bit 0 of h[0x2E] = 0x780E clear): b[0x67] 0, no name window.
- Marker positions measured on the 320×218 screenshot (6× zoom) for the known places 1, 2, 3, 11, 12, 13, 14:
  (213.3, 64.2), (196.3, 77.2), (190.8, 90.3), (180.3, 102.5), (167.2, 90.3), (162.5, 93.7), (153.3, 80.3), all within
  half a pixel of `(mapX + 40.5, mapY + 9.5)`.

## Proposed names (not applied)
- `EVT_GB_NAVI_PLACE 0x67` (u8 global, place index + 1 the full-map cursor is snapped to, 0 = none; read by the world
  script for the name, by `w_movement` for the autopilot destination). high.
- `EVT_GH_NAVI_KNOWN 0x2E` (`g_evtGlobalVars.h[0x2E..0x31]`, 64 known-place bits for the navi map,
  `wm_naviIsPlaceKnown`). high.
- `EVT_GB_NAVI_MODE 0x64` (default `g_worldNaviMode`, read by `wm_naviInit`). medium.
- `TEXT_CODE_BANK_TEXT 0x06` (text code `F9 06 c`: insert entry `g_msgNumbers[c & 7]` of `g_msgTextBanks[(c >> 4) & 3]`)
  and `TEXT_CODE_BANK 0x04` (a message starting `F9 04` is a text bank, `msg_resetChoices`). high.
- `menuSave_*`/`s_menuItem*` etc. are already named; no new symbols were needed for questions 1-5.
