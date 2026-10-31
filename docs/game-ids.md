# Game-data ids

`include/ff9_ids.h` (included by `ff9.h`, so by every overlay and effect module) names the ids the code tests:
characters, items, key items, action and support abilities, battle commands, status and element bits. Names come
from the game's own text, decoded by `tools/ff9text.py`; each macro's comment says which table (`[text items]`).
Only ids the code uses get a macro. Don't commit decoded tables.

## Text encoding

`python3 tools/ff9text.py font --png build/font.png` renders the font (FF9.IMG dir index 1, file 0x271A: item
0x0D0000 is the `Font` header and glyph table, TIM item 0x040000 the 4 bpp glyph sheet) with each code under its
glyph; the charmap in `ff9text.py` was read from that sheet. Codes 0x00-0x09 are digits (text_formatNumber writes
digit d as code d), 0x0F space, 0x10-0x29 A-Z, 0x30-0x49 a-z, 0x7E the apostrophe, 0x50-0x7D accented letters,
the rest punctuation and symbols. 0xFF ends a string, 0xF7 is a newline, 0xF9 and 0xFA take one argument byte
(text_buildBlock, msg_emitItemName). Each g_btlMessages text starts with one digit code before the message.

## Tables

`python3 tools/ff9text.py dump TABLE [--macros PREFIX]` (host side: reads `disks/us/SLUS_012.51` and
`disks/us/ovl/*.bin`; `font` also needs FF9.IMG):

| table | ids | source |
|---|---|---|
| `items` | 0x00-0xFF | main `g_itemTable` (0x12-byte records, name offset +0) into `g_itemNames` |
| `keyitems` | 0x00-0x4F | main `g_keyItemTable` (8-byte records) into `g_keyItemNames` |
| `abilities` | 0x00-0xBF | ovl_33 `s_menuAbilityActionTable` (0x10-byte records, name +0xE) into `s_menuAbilityActionNames` |
| `support` | 0xC0-0xFF | main `g_supportAbilities` (8-byte records, name +2) into ovl_33 `s_menuAbilitySupportNames` |
| `commands` | 0x00-0x2F | ovl_0b `g_btlCmdTable` (0xC-byte records, name +0) into `g_btlCmdNames` |
| `cmdabilities` | 0x00-0x2F | the abilities each command lists (`g_btlCmdTable` count/first into `g_btlCmdAbilities`) |
| `messages` | | ovl_0b `g_btlMessages`; entries 1-8 are the element names (btl_getElementName) |
| `chrnames` | | main `g_chrDefaultNames`; character id n is entry `g_chrDefaultSets[n][0]` |

## Naming rules

- Prefixes: `CHR_`, `ITEM_`, `KEY_ITEM_`, `ABIL_` (action abilities), `SA_` (support abilities, as ids),
  `BTL_CMD_`, `BTL_STATUS_` (BtlUnit.status), `CHR_STATUS_` (Character.status, the same low bits), `ELEM_`.
- Support abilities are tested as bits of `Character.supportAbilities` / `BtlUnit.support`: write
  `support[1] & SA_BIT(SA_COUNTER)` (word = (id - 0xC0) >> 5, as chr_testSupportBit).
- A name the table repeats gets the command that lists it (`ABIL_SPARE_CHANGE_ELAN`, `ABIL_CURSE_ELAN`), the
  code's role (`ABIL_SPEAR_TRANCE`), or a number (`ABIL_FENRIR2`).
- Replace a literal only where its domain is proven (an item-id argument, a compare with `cmdNo`/`subNo`, a mask on
  a support or status word). Ids without a text name stay literals unless the code makes them unambiguous
  (`ITEM_ID_KEY_FIRST`, `CHR_COUNT`).
- Status bits: `BTL_STATUS_*` names are runtime/code evidence (docs/runtime-findings.md); bit 0x4 (`VIRUS`) and the
  `CHR_STATUS_*` set come from ovl_38's status help: its line k shows icon argument k, which
  menuStatus_updateHelp sets to the same sprite ids (154..148) that g_statusIconSprites gives status bit k.
