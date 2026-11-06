# R44 main / ovl_0b: layout and tuning literals left for a runtime check

Round R44 (screen layout, tuning, probability bands) for the boot exe (`main`) and the battle overlay (`ovl_0b`,
0x800A7000). The names below are in the headers already, named by what the code does; each row says what would
confirm or correct the part the code can't prove. States and helpers: `docs/emulator.md` (`tools/rt/enter_battle.py`).

| # | symbol / site | binary | current name (header) | what to observe | mode / state |
|---|---|---|---|---|---|
| 1 | `evt_placeBalloon` (0x80038FD8), `src/main/event.c` ~545-570 (`evt_placeBalloon_y`): `below = 1` puts the box at `sy - h - 16`, `below = 0` at `sy + 32`; `tailMode = below << 1 \| right` | main | `BALLOON_GAP_ABOVE` (16), `BALLOON_GAP_BELOW` (32) in `include/ff9.h`. The code's `below` flag places the box *above* the anchor (tail pointing down), so "below" may name the speaker's side. Unproven: the 32 px gap really is the box under the speaker's head, not a correction for the tail sprite | `field_dali_inn`: talk to an NPC (any balloon). Screenshot, and read the open message window's `tailMode` (`MsgWindow`) plus the speaker's projected y (`evt_projectToScreen` output; break at 0x80038FD8 and log `$a2`/`$a3`, then the WinLayout y after return). Expect: tailMode bit 1 set = box above the head | field |
| 2 | `akao_loadBankInSlot` (0x80056048), `src/main/akao_api.c:501-507`: bank id ranges `500-599` (slot << 12), `200-299` (slot * 5 << 12), `300-499` (slot << 14, slot += 4) | main | none (left as numbers). Want a name per range: which sound banks are 200-299 / 300-499 / 500-599 (enemy SE? summon / effect SE? weapon SE?) | `trace` 0x80056048 logging `$a1` (slot) and `*(u32 *)($a0 + id offset)` (AkaoBankHeader.id) through one battle start and one summon (`enter_battle.py`, then cast a summon) and one field load | battle, field |
| 3 | `btl_calcCmd` (0x800B44C0) `BTL_FORMULA_EVT_POWER_DAMAGE` (0x68), `src/ovl_0b/btl_cmd.c` ~2569: `1 << (g_evtGlobalVars.b[0xC0] - 1)` | ovl_0b | `EVT_GB_POWER_DAMAGE_EXP` (0xC0) in `include/ff9.h`, `BTL_EVT_POWER_DAMAGE_CAP_EXP` (14) in `include/ovl_0b.h`. Unknown: which ability / battle uses formula 0x68 and what the battle script counts in byte 0xC0 | grep the ability table (`Battle.abilities`, formula byte) for 0x68 in a few boss battles, or break on the formula's case in `btl_calcCmd` and note the encounter and the attack name; watch writes to 0x8007AEF0 + 0xC0 during that battle | battle |
| 4 | `BTL_FORMULA_PHYSICAL_CLASS8` (0x62), `src/ovl_0b/btl_cmd.c` ~2516: `classFlags & 8` misses | ovl_0b | none for the bit (`CHR_CLASS_*` in `include/ff9.h` has 0x1, 0x2, 0x10). Want `CHR_CLASS_<what>` for 0x8 | dump `Character.classFlags` for all 8+ party characters (GameData.characters, any field state with a full roster) and say which have bit 3 set | field (disc 3/4 state) |
| 5 | `btlui_drawPanelGauges` (0x800F36F8), `src/ovl_0b/btlui.c` ~3360: in Trance the Trance gauge is drawn with `ui_drawGauge` style 1 | ovl_0b | `BTLUI_TRANCE_ACTIVE` (2) / `BTLUI_TRANCE_GAUGE` (1) in `include/ovl_0b.h` (from the field comment "0 no trance gauge, 1 gauge, 2 in trance") | in a battle, poke a party unit's `trance` to 0xFF so it enters Trance; screenshot the panel row before and after, and read `g_btluiPanel->tranceKind[row]` (Unk80105078 + 0x88 + row; the panel is g_btluiPanel, allocated 0x8C) in both. Confirms 1 = gauge filling, 2 = in Trance | battle |

Not asked (named from the code alone, or left as numbers on purpose):
- btl_calcCmd bands (steal slot odds, Six Dragons, short summon, Comet, item heal multipliers, revive HP ranges): the
  code says what each threshold decides; the values stay data. Percent arithmetic (`% 100`, `/ 100`, `* 3 / 4`,
  `>> 3` fractions), stat caps written as `0xFF` / `0x100` (u8 range) and Lucky Seven's 7 / 77 / 777 / 7777 (the
  number is the meaning) are left as numbers.
- AKAO: volumes, pans, pitch scales and LFO phases are fixed point; only the gate gap and the note-op range are named.
- Angles (0x400 / 0x800 / 0xFFF), the analog speed formula, colour channels, VRAM cache coordinates of the battle
  panel header (0xD0 / 0x110, 0x1F8 / 0x1FC) and GPU sizes stay numbers.
