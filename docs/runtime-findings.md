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
