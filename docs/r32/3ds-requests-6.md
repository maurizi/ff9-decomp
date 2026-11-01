# R36: scenario fixtures for the 3DS port (ATE prompt, end-of-disc-1 disc change)

Answers from the field scripts (`tools/evtscript/edis.py`, plus scratch scans in `build/3ds6/`: `opscan.py` = every
instance of one opcode in every field script, `rawscan.py` = raw byte search of FF9.IMG mapped to file ids), the matched
C (line numbers at commit 629d0924), and PCSX-Redux runs on disc 1 (and disc 2 for the tail of the disc change).
Background: `docs/r32/3ds-requests.md` section 3.

Notation: `b[n]` = `g_evtGlobalVars.b[n]` = main 0x8007AEF0 + n = the harness's `event.n`. `h[0]` = story counter
(u16, b[0..1]). Multi-byte script variables are little-endian. `Fbit[...]`/`Fu8[...]` are field-local (cleared per field).

## Short answers

### Fixture 1: ATE available (Lindblum, disc 1)

```
disc 1
field 552
story 3110
event.2=0
event.3=0
event.236=1
event.237=0
```

- Result (verified): "ACTIVE TIME EVENT" / "PRESS [SELECT] TO SEE ATE" is shown at once. Select opens the
  "ACTIVE TIME EVENT / Select event" window with **"Small-Town Knight in a Big City"** and "Cancel". Cross shows the
  title card, a second Cross starts the ATE (field 555, Steiner: "What a huge town! I'm completely lost.").
- What the script tests (all 47 standard ATE fields, run once by the field's init, entry 0 tag 0):
  `u16 b[0xEC..0xED]` (available-ATE mask) != 0, **and** `s16 b[2..3]` (the scene return code) not 1 (Lindblum
  fields: also not 2). 1/2 mean "coming back from an ATE", which hides the prompt. Story value and field bit 0x99 are
  **not** preconditions: the init sets field bit 0x99 itself just before the test. Story 3110 is the value at which the
  real game sets the mask (field 553, leaving through region entry 7: `if (h[0] == 3110 && !Gbit[0xA57]) b[0xEC] = 1`).
  In field 552, story < 3180 (except 3175) gives the normal street actors, 3175 and >= 3180 other sets, and
  3160..3179 also changes the music.
- Mask bit k = choice row k in the field's own ATE list (msg 0x74 in field 552). Row 0 in 552 = "Small-Town Knight in
  a Big City". Another bit gives another title (552 has rows 0..3, going to fields 555, 561, 609, 850), but each ATE field
  checks its own story conditions, so use bit 0 (= 1).
- Simpler forcing method: **poke `event.236` (b[0xEC]) nonzero before the field's init runs** (= the recipe), in any of
  the 47 fields with the standard block (disc 1: 200-208, 250-255, 350-358, 550-573, ...; list in R32 section 3).
  Leave `event.2/3` at 0. Poking after the init doesn't work, because the init also starts the Select-handler object
  (`NEW_CODE`) inside the same branch (552: `07 07 00` at +0x205). Poking b[0xEE]=1 afterwards (R32's method) only
  works in fields whose handler always runs (Dali 352).
- Alexandria (fields 100-117) tests a field-local u8 instead of b[0xEC], so it can't be forced from the global
  variables alone. Use Lindblum (or field 250: story 2260, `event.236=16`, from field 250's own region code).

### Fixture 2: end of disc 1 (disc change to disc 2)

```
disc 1
field 70
story 3900
event.2=1
event.3=0
```

- Field 70 is the disc-1 ending FMV field. Its init (entry 0 tag 0) switches on `s16 b[2..3]`. 0 plays movie 0x103 and
  goes to field 50 (the game opening). **1 plays movie 0x11C (the Burmecia/Kuja silver-dragon ending), sets b[2..3] = 0
  and executes `AC 00 20 43` = op 0xAC DISC_CHANGE 0x4320 = disc index 1 (disc 2), location 0x320 = field 800.**
  The only way into it in the real game is field 768 entry 13 (story 3900), which sets `b[2..3] = 1` and goes to field 70.
  Field 70 itself doesn't test the story. 3900 is what disc 2's field 800 expects (its opening cutscene played).
- Real flow (verified): FMV (ovl_64, about 3800 frames), then `fld_requestDiscChange(1, 800)` → a "DISC CHANGE / Do you
  want to save your game? Yes / No" window (ovl_0a, `fld_runDiscChange`) → No → **"Change to DISC 2"** picture (ovl_3e,
  menu type 8, arg 0x4320). With only disc 1 inserted: Cross (or a lid close) checks the disc, "INSERT CORRECT DISC /
  Caution!" shows for 180 frames, then the picture again, indefinitely. That is as far as disc 1 alone gets. With disc 2
  inserted (Redux started on disc 2, same state): the menu closes, the dir tables reload and **field 800 on disc 2**
  starts (Alexandria castle gate, "Tall Guard: There's been a lot of trouble lately.").
- **Why a direct `menu_open(8, …)` loops.** The real flow doesn't run menu type 8 from the field loop. Op 0xAC calls
  `fld_requestDiscChange`, which sets `FLD_MAP_DISC_CHANGE` on the field and ends the field loop. Then `fld_main`
  runs `fld_runDiscChange()`, a private loop: the save prompt, then menu type 8. **Its leave function then calls
  `file_loadDirTables()` (which clears `GS_RELOAD_FILE_TABLES`) and `fld_refindSectors()`** (the new disc's sectors for
  `scene->nextId`, which ovl_3e set through `evt_setNextLocation`). Opening menu type 8 directly, from inside the field
  loop, skips both. `discChange_exit` sets `GS_RELOAD_FILE_TABLES`, and the field loop's test for that flag
  (`fld_main.c:68`) ends the field every frame it is set. Nothing in field mode clears it outside
  `fld_discChangeLeave`, so the field exits, reloads and exits again. Verified: direct open from field 800 with disc 2
  inserted, Cross → `GameState.flags` stays 0x00100000, black screen, `fld_exit` called every ~140 frames.
- **Minimal state:** none beyond the recipe. The missing piece is the code path, not a variable. To trigger the
  disc change without the script, call `fld_requestDiscChange(1, 800)` (= execute op `AC 0x4320`) from a disc-1 field.
  Don't call `menu_open(8, …)`. If the port must open menu 8 directly, it has to do what `fld_discChangeLeave` does
  when it closes: set `FLD_MAP_REFIND_SECTORS`, call `file_loadDirTables()` and `fld_refindSectors()`. Then let the
  field exit as `FLD_EXIT_FIELD` to `scene->nextId`.

## Details

### ATE block in a field init (field 552 entry 0 tag 0, +0x1C6)

```
01c6  if (Gu16[0xEC]) {
01cd    if (Gs16[0x2] == 1 || Gs16[0x2] == 2) { D7 ATE_PROMPT 0; Fbit[0x9B]=0; Gs8[0xEE]=0; Gs16[0xF1]=0 }
01fd    else {
          Gs16[0xEF] = 552            // the field to come back to after the ATE
0205      NEW_CODE 7                  // Select handler object (entry 7 tag 1)
0208      Fbit[0x99] = 1
0210      if (Fbit[0x99] == 1) {
021b        if (Gu16[0xEC] != 0) {
0226          D7 ATE_PROMPT 1; Fbit[0x9B]=1; Gs8[0xEE]=1; Gs16[0xF1]=Gu16[0xEC]
0240          if (Gu16[0xFB] != (Gu16[0xFB] | Gu16[0xEC])) Fbit[0x98] = 1   // new ATE: jingle (C8 0xD000 0x546)
0255          Gu16[0xFB] |= Gu16[0xEC]                                      // ATEs announced so far
            } else { D7 0; Fbit[0x9B]=0; Gs8[0xEE]=0; Gs16[0xF1]=0 }
      } } }
```

Select handler (552 entry 7 tag 1): `if (controlEnabled == 1 && Gs8[0xEE] == 1 && padPressed(SELECT))` → Fbit[0x9A]=1,
DISABLE_CONTROL, PAD_LOCK_MENU, `7C MSG_CHOICE_MASK (Gs16[0xF1] | 0x8000)`, `1F MSG_WAIT 1, 0x40, 0x74` (window 1,
ATE title flag), switch on `sys.msgChoice`: 0 → title msg 0x75, `GOTO_FIELD 555`; 1 → 561; 2 → 609; 3 → 850.
Each target ATE field clears its own bit when played (555: `Gu16[0xEC] &= ~1`; 609: `&= ~4`; 112/117: `&= ~2`).

Writers of b[0xEC] on disc 1 (raw scan `05 dc ec`, which also finds fields the varscan parser skips):
`=` 1 (553, story 3110), 3 (201), 8 (253), 16 (250, story 2260), 0 / `|=` 8 (565), `|=` 4 (208, 566), `^=` 8 (850).
The other hits are the standard test (`& 0xFFEF`/`0xFFEB`/`0xFFFE` in the inits of 200-208
and 555) and the clears in the ATE fields.

### Disc change path

- `evt_execInstr` `case EVT_OP_DISC_CHANGE` (`src/main/event.c:1976`): `fld_requestDiscChange((a >> 14) & 3, a & 0x3FFF)`.
- `fld_requestDiscChange` (`src/ovl_0a/fld_disc.c:3`, ovl_0a 0x800B5004): `g_fieldDiscChange` (0x800CA058)
  .discMask/.disc/.fieldId/.areaId = -1, field flags |= `FLD_MAP_DISC_CHANGE` (0x100000), GameState flags |=
  `GS_FLD_NEW_FIELD`.
- `fld_main` (`src/ovl_0a/fld_main.c:113`): after the loop, `if (fld_isDiscChangePending()) fld_runDiscChange();` then
  `fld_exit()`.
- `fld_runDiscChange` (`fld_disc.c:28`): modes PROMPT (`fld_discPrompt*`, "Do you want to save your game?"; Yes →
  `menu_open(MENU_SAVE, 2)`, back to the prompt), SWAP (`fld_discChangeOpenMenu8`, `fld_disc.c:223`:
  `menu_open(MENU_DISC, (disc & 3) << 14 | fieldId)`), DONE. `fld_discChangeLeave` (`fld_disc.c:54`): field flags
  |= `FLD_MAP_REFIND_SECTORS`, `file_loadDirTables()`, `fld_refindSectors()` (`fld_main.c:604`, looks up
  `scene->nextId` in the new disc's dir 4/2/9 tables).
- ovl_3e `discChange_init` / `discChange_exit` (`src/ovl_3e/ovl_3e.c:147`): exit calls
  `evt_setNextLocation(nextLocation)` (→ `game_setNextScene` → field `scene->nextId`) unless 15000, and sets
  `GS_RELOAD_FILE_TABLES` (0x100000). `discChange_checkDisc` → `DISC_CHECK_WRONG_DISC` (6) when `GameState.disc`
  (+0xB, read back by `file_initImg`) lacks `1 << disc`.
- `file_loadDirTables` (`src/main/file.c:196`/`:289`) returns at once unless `GS_RELOAD_FILE_TABLES` is set, and clears it.
  Its callers are `game_initState`, ovl_14 `opening.c:65` and `fld_discChangeLeave`, so nothing in field mode clears it.
  A directly opened menu 8 therefore loops the field (`fld_main.c:68`).
- Other op 0xAC users: disc 2/3 field 1800 entries 10/11 (`AC 0x8714` = disc 3, field 1812) and disc 3 field 2750
  (`AC 0xCBEC` = disc 4, field 3052). Disc 2 also has a field 70 with the same `AC 0x4320`.

## Runtime evidence (PCSX-Redux, disc 1, base state `field_dali_inn`)

- ATE: `tools/rt/enter_field.py 552 --base-state field_dali_inn --frames 900 --poke 0x8007AEF0=260c --poke
  0x8007AEF2=0000 --poke 0x8007AFDC=0100` → prompt drawn bottom left (`build/3ds6/f552.png`). b[0xEC..0xF2] = `01 00 01
  28 02 01 00` (EE = 1, EF = 552, F1 = 1), `g_atePromptMode` (0x8007BC10) = 1. `press select --wait 90` → ATE window
  (`f552_sel.png`); `press cross` → title card; `press cross` → fieldId 555, Steiner's ATE (`f552_ate2.png`).
  States `r36_lindblum_552_ate`, `r36_lindblum_552_ate_window`.
- Disc change: `enter_field.py 70 --base-state field_dali_inn --frames 300 --poke 0x8007AEF0=3c0f --poke 0x8007AEF2=0100`
  → FMV (ovl_64 resident; Kuja's silver dragon at +3000 frames). `until 0x800B5004 --frames 6000` → hit at +3800 frames:
  `fld_requestDiscChange(a0=1, a1=0x320)`, ra = `evt_execInstr+0xA598`. +120 frames: "DISC CHANGE / Do you want to save
  your game? Yes / No" (`f70c.png`). Down, Cross: ovl_3e resident, "Change to DISC 2" (`f70d.png`), `s_discChange`
  (0x801F6BBC) = `00 01 00 00 01 00 20 03` (lid closed, wanted disc 1 = disc 2, nextLocation 800). Cross: about 150 frames of
  checking, then state 3 / check 6, "INSERT CORRECT DISC / Caution!" (`f70i.png`). After 180 frames, back to the picture.
  States (disc 1): `r36_disc1_end_fmv`, `r36_disc1_discchange_op` (at the fld_requestDiscChange breakpoint),
  `r36_disc1_discchange_prompt`, `r36_disc1_change_to_disc2`.
- Disc 2 tail: the `r36_disc1_change_to_disc2` state copied to the disc-2 state dir as `r36_x_change_to_disc2`, Redux
  started with `--disc 2`. Cross → `GameState.flags` 0, `disc` 2, fieldId 800. +900 frames: Alexandria castle gate,
  "Tall Guard" speech (`d2b.png`), story still 3900. State (disc 2) `r36_disc2_field800_start`.
- Direct open (the port's case): from `r36_disc2_field800_start`, break at `menu_update` in the field loop and redirect
  the call to `menu_open(8, 0x4320)` (`build/3ds6/direct_open.py`; state `r36_x_direct_open_disc2`). Cross → disc check OK,
  the menu closes, then `GameState.flags` = 0x00100000 for good, a black screen, and `trace fld_init fld_exit --frames 300` shows
  `fld_exit` from `fld_main+0x580` twice (frames 5569, 5708): the field reload loop.
- Side note: the older `menu_disc_change` state (opened with arg 0x3A98), copied to disc 2 and patched to want disc 2,
  still read `GameState.disc` = 1 after the check (wrong disc). The disc-1 states made in this session swapped fine. Use
  the `r36_*` states for disc-swap experiments.

## Proposed names (not applied)

No new symbols: every function on both paths is already named. Script-variable labels for docs/tools
(global bank, `g_evtGlobalVars`):
- `s16 b[2..3]` → scene return code (ATE / FMV-field entry code; 1 = back from an ATE in standard fields, selects the
  movie in field 70). high (field 70, 552, 553 scripts).
- `u16 b[0xEC..0xED]` → ATE available mask (bit = row of the field's ATE list; ATE fields clear their bit). high.
- `s8 b[0xEE]` → ATE prompt on (Select handler armed). high (R32).
- `s16 b[0xEF..0xF0]` → ATE host field (field to return to). medium (set to the field id by every init; the return was not traced).
- `s16 b[0xF1..0xF2]` → ATE choice mask (copy of b[0xEC] fed to MSG_CHOICE_MASK). high.
- `u16 b[0xFB..0xFC]` → ATEs announced (new bits trigger the jingle). medium.
