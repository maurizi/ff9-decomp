# Encounters: who starts a battle, and where its background comes from

Answer to ff9-ds request R24. Catalogue tool: `tools/encounter_catalog.py` (reads `disks/us/discN/FF9.IMG`, no
emulator). Everything here is read from the code named below and checked against the four discs; nothing was
observed at run time.

## Summary for the port
- A battle needs two CD ranges in `GameData`: `unk7FC[0]/unk804[0]` (encounter bundle, FF9.IMG dir index 5) and
  `unk7FC[1]/unk804[1]` (battle background, dir index 6). The field or world map that starts the battle fills
  both from **its own dependency table** (below) and then switches to mode 2.
- Every encounter bundle names its background too (its own dependency table has one key `0x0C0000|bg`), and the
  place that starts it always names the same one (0 mismatches on all discs).
- The 408 encounters the port's sweep could not enter (130/79/121/78 on discs 1-4) are exactly the encounters
  **whose background file is not on that disc**: the bundle's `0x0C` entry has sector 0 and length 0, and so has
  the entry in every field/world table on that disc. The game never starts them there. Each of the 408 has its
  background on another disc, where a world-map region starts it (all 408 are world-map encounters: 376 random,
  the rest listed or script-started on the world map). So a fixture should run encounter N on a disc from
  `bg_present_on_discs`, where the bundle supplies the background like every other encounter (no field needed).
- `python3 tools/encounter_catalog.py --encounters --json out.json` gives, per (disc, encounter): background id,
  whether it is on the disc, the discs where it is, and every field / world region / world file that starts it.

## Containers and dependency tables
Most FF9.IMG files are **DB containers** (`db_lookup`, main 0x8001C8B0): byte 0 `0xDB`, byte 1 = number of blocks,
then one u32 per block at +4: `kind << 24 | offset` (offset relative to that word; kind masked with 0x7F). A block:
byte 1 = item count, u16 ids from +4, then (4-aligned) one u32 per item, `offset` relative to that word. A key is
`kind << 16 | id`. Loaded containers are registered with `db_register` (0x8001C750) in `g_dbList`, and
`db_lookupRegistered` (0x8001C7FC) searches all of them (mode 4 = item address, 5 = size, 1 = count, 2 = address
by index, 3 = key by index).

Block kind **0x14** is a **dependency table**: `{u16 ?, u16 n}` then n entries `{u32 key, u32 sector, u32 sectors,
u32 link}`; sector is relative to FF9.IMG and 0/0 means "not on this disc". `dep_find` (main 0x80063088) finds an
entry by key (24 bits). The field/world helpers below scan from the start and stop at the first match, so with the
duplicated keys the tables contain (each battle appears twice: linked to its background and to `0x070000`) the
**first** entry wins; the catalogue does the same.

| key kind | FF9.IMG dir index | what |
|---|---|---|
| 0x05 | 4 (fields), 3 (world maps 9000-9012) | field / world file; link = its area (0x06) |
| 0x06 | 2 | area bundle |
| 0x11 | 5 | encounter bundle (id = battle id); link = its background (0x0C) |
| 0x02 | 7 | enemy model file (id = model id, `btl_loadEnemyModelsStep` loads every 0x02 key); 4th word = sectors of model data before the textures (docs/battle-models.md) |
| 0x0C | 6 | battle background (file id = background id); link points back to a battle |
| 0x07, 0x09 | 9, 11 | linked pairs present in every battle's table (not needed here) |

Dir index 4 file ids are `area << 16 | field` (as `tools/scenario_catalog.py --fields` reads them); dir index 3
holds the world-map "fields" 9000-9012 (area 0x44) plus empty 9100/9101. Dir index 1 file 20000 is a dependency
table only (1637 battle entries); no code that loads it was found.

## Field → battle (ovl_0a)
1. `fld_load` → `fld_loadFile` (0x800A9BDC) reads the field file from `unk7FC[1]` and sets
   `GameData.unk7E4` to its dependency table (block 0x14).
2. A battle request is the event system's return value 3 (`func_8004BC38` → `func_80048F54`): either a script
   opcode (below) or a random encounter. `fld_main` then sets the field scene's exit reason `unk4A = 2`.
3. `fld_exit` (0x800A8E58) case 2: copies `unk7FC/unk804` to `unk7EC/unk7F4` (restored by ovl_0b
   `btl_cleanup`), then `fld_depFindBackground(fld_depFindBattle(id, &unk7FC[0], &unk804[0]), &unk7FC[1],
   &unk804[1])`, i.e. entry `0x110000|id` gives the bundle and its link the background. `GameData.unkE = id`,
   mode 2. (`fieldId` keeps the field to return to.)
4. ovl_0b: `btl_entry` → `btl_main((s16)unkE)`; `btl_loadEncounterFile` reads `unk7FC[0]` (first 0x2800 bytes) and
   replaces `unk7E4` with the bundle's own table; `btl_loadBackgroundStep` reads `unk7FC[1]`.

Case 3 (field → world map) and case 1 (field → field) use `fld_depFindField` (0x5 key) and `fld_depFindArea`
(its 0x6 link) the same way, with the two slots swapped.

## World map → battle (ovl_0c)
Same scheme: `wm_loadFile` (0x800A9C1C) loads the world file from `unk7FC[1]` and sets `unk7E4`; on exit reason 2
(`unk8->unk1B`), `wm_depFindBackground(wm_depFindBattle(id, ...))` fills the two slots, `unkE = id`, mode 2.

## Random encounters
`evt_rollEncounter` (main 0x80038D3C), called from the actor update (`func_80048F54`) whenever the player moved:
`g_evtEncounterDist` accumulates the distance walked (starts at -0x5A0 on `evt_init`); each time it passes 0x3C0 it is
reset, `g_evtEncounterChance += g_evtState->unk16EF` (the frequency), and if a random byte is `< chance >> 3` a
battle is picked, the chance reset, and `evt_setNextBattle(id)` called (a repeat of the previous pick,
`g_evtLastEncounter`, is rerolled once). It only runs while `g_evtState->unk16EC` is set (script opcodes 0x2E on /
0x2D off). `g_evtEncounterPending` turns the result into return value 3.

The pick (`func_80038D3C_pick`): table `{u16 battle[4], u8 pattern}`; one random byte compared with the
cumulative thresholds `g_encounterSlotThresholds[pattern & 3]` (main 0x8006E648):

| pattern | thresholds | slot shares (of 256) |
|---|---|---|
| 0 | 0x60 0xA8 0xE0 | 96 72 56 32 |
| 1 | 0x40 0x80 0xC0 | 64 64 64 64 |
| 2 | 0x5A 0xA7 0xF4 | 90 77 77 12 |
| 3 | 0x73 0xD9 0xF4 | 115 102 27 12 |

- **Field** (`g_evtMode` 1): the table is `g_evtEncounterTable` (main 0x8007AE40), filled by script opcode 0x3C.
- **World map** (`g_evtMode` 3): `wm_getEncounterEntry` (ovl_0c 0x800BBBA8). The cell under the player gives area
  (`attr >> 8 & 0x3F`) and topography (`attr & 0xFF) >> 2`); `g_worldAreaZone[area]` (ovl_0c .data 0x800CCFB4,
  64 bytes) gives the zone; zone z owns entries `[start(z), start(z+1))` with `start` = 2 x running sum of
  `g_worldZoneEncounterCount` (0x800CCFF4, 26 bytes; 254 entries in all, `g_worldEncounterZoneStart`). The first
  entry whose topography and fog bit match `g_worldFog` ("Now Fog" in its debug print) is used; with the late data
  set (`g_worldDataSet == 4`) and index < 100 the entry 254 later is returned instead.
  Entries are 10 bytes `{u16 battle[4], u8 topography << 2 | pattern, u8 zone << 1 | fog}` in
  `g_worldEncounterTable` = pack entry 3 of world data item 0x41B (story counter < 0x2B52, `g_worldDataSet = 1`) or
  0x41E (later, set 4); both live in FF9.IMG dir index 1 (file 10002 on discs 1-3, 10006 on disc 4). A pack is
  `{u32 n, u32 offset[n]}` (offsets from the pack, `w_frameExtruct` relocates them). Pack entry 4,
  `g_worldEncounterEventRows`: rows of 12 u16 entry numbers (1-based, 0xFFFF = none); `wm_applyEncounterEvents`
  (0x800BAD98) sets `battle[3] = battle[2]` in row k's entries when bit k of `g_evtGlobalVars.b[0xC2] |
  b[0xC6] << 8` is set (the catalogue reports these bits per region).

## Event-script opcodes (main `evt_execInstr`)
An instruction is `op, flags, args`; flag bit i set = argument i is an expression, else a literal (1 or 2 bytes).

| op | args | effect |
|---|---|---|
| 0x2A | u8 t, u16 b | battle `b & 0x7FFF` (`evt_setNextBattle`); bit 15 → `GameData.unk571`; `t` → `D_8007BD2C` (selects one of two draw environments `D_8006F2E0[]` in `func_80053A10`, probably the transition effect); `unk848 = -1`; returns 3 |
| 0x8C | u8 t, u8 x, u16 b | same, `x` → `GameData.unk848` |
| 0x3C | u8 pattern, u16 b0..b3 | field random table `g_evtEncounterTable` |
| 0x57 | u8 f | encounter frequency `g_evtState->unk16EF` (0 also clears the chance) |
| 0x2E / 0x2D | - | random encounters on / off (`unk16EC`) |
| 0x2B / 0xB6 | u16 id | next field / world map (`evt_setNextLocation`), return 4 / 5 |

Script layout (item kind 5, id = field or world id, in the file's second nested container 0x1B0001): "EV" magic,
byte 3 = entry count, entry table at +0x80 of 8-byte records `{u16 offset (from +0x80), u16 size, ...}`; an entry
starts with `u16 nfunc << 8` and nfunc `{u16 tag, u16 offset}` pairs, code at entry + 2 + offset
(`func_800384D0_find`).

## What the catalogue covers
- Fields: every battle in a field's dependency table, with background. Kind `random` if a literal 0x3C in the
  field's script names it (share = its slots' share in that table; `frequency` = literal 0x57 values right before
  a 0x3C), `scripted` for a literal 0x2A/0x8C. The scan does not decode instructions: it looks for the byte patterns
  with flags 0 and keeps a hit only when every battle id is in the field's table. On all 4 discs every field
  battle was matched to one of them.
- World map: every random-table entry of the data set(s) on the disc (zone, its areas, topography, fog, pattern,
  share), with the background from the world files 9000-9012 that list the battle; world-script 0x2A/0x8C as
  `scripted`; battles only in the world tables as `listed` (30 per disc, 79 on disc 4: e.g. those set through
  expression arguments or only reachable through other data sets).
- Not covered: expression arguments (story-dependent ids), the conditions around an instruction (story flags,
  which entry runs), which world file 9000-9012 is active when, and the field-only gate `D_8007AEAC`.
- Counts (distinct encounters per disc 1/2/3/4): field random 31/78/107/61, field scripted 33/35/35/21, world random
  244/244/244/195, world scripted 14 each; encounters whose background is on the disc: 207/308/292/278, all
  of them started by at least one place.
