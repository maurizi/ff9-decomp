# R32: answers for the 3DS port (button icons, field exits, ATE prompt, main menu)

Answers from the matched C and asm (paths are relative to the repo; line numbers at commit 635f1a61), checked in
PCSX-Redux on disc 1 (states `field_d1_101`, `field_d1_352`). The runtime evidence is also recorded in
`docs/runtime-findings.md` ("R32 3DS requests").

## 1. Button icon sprites

**Short answer.** All button glyphs are `g_uiSprites` entries (main 0x800686D0, 8-byte `UiSprite` = `u8 u, u8 v,
u16 clut, u8 w, u8 h, u16 tpage`). They all use tpage 0x001F (4 bpp, page at VRAM (960,256)) and CLUT 0x3824
(VRAM (576,224)), and they all come from the common TIM `0x2710` that `game_loadCommonImages` uploads at boot.
The ids are 4 Cross, 5 Circle, 6 Square, 7 Triangle, 8 L1, 9 L2, 10 R1, 11 R2, 12 d-pad, 13-16 d-pad arm highlights
(up, down, left, right), 17 START, 18 SELECT. All of this was checked against a VRAM dump.

### Sprite table (g_uiSprites ids 4-18; decoded from the exe, all `tpage 0x001F`, `clut 0x3824`)

| id | glyph | u | v | w x h | VRAM halfword rect (x = 960 + u/4, y = 256 + v) |
|---|---|---|---|---|---|
| 4 | Cross | 32 | 0 | 12x11 | x 968-970, y 256-266 |
| 5 | Circle | 44 | 0 | 12x11 | x 971-973, y 256-266 |
| 6 | Square | 56 | 0 | 12x11 | x 974-976, y 256-266 |
| 7 | Triangle | 68 | 0 | 12x11 | x 977-979, y 256-266 |
| 8 | L1 | 0 | 0 | 12x11 | x 960-962, y 256-266 |
| 9 | L2 | 12 | 0 | 12x11 | x 963-965, y 256-266 |
| 10 | R1 | 0 | 11 | 12x11 | x 960-962, y 267-277 |
| 11 | R2 | 12 | 11 | 12x11 | x 963-965, y 267-277 |
| 12 | d-pad | 24 | 11 | 12x11 | x 966-968, y 267-277 |
| 13 | d-pad up-arm highlight | 24 | 0 | 4x4 | x 966, y 256-259 |
| 14 | down-arm highlight | 28 | 0 | 4x4 | x 967, y 256-259 |
| 15 | left-arm highlight | 28 | 4 | 4x3 | x 967, y 260-262 |
| 16 | right-arm highlight | 24 | 4 | 4x3 | x 966, y 260-262 |
| 17 | START | 168 | 0 | 32x11 | x 1002-1009, y 256-266 |
| 18 | SELECT | 168 | 11 | 34x11 | x 1002-1010, y 267-277 |

- Page: tpage 0x1F means tx = (0x1F & 0xF) * 64 = 960, ty = ((0x1F >> 4) & 1) * 256 = 256, depth (tpage >> 7) & 3 = 0
  (4 bpp), abr 0. Sampling is the standard 4 bpp one: texel `u` is nibble `u & 3` of halfword `960 + u / 4`.
- CLUT 0x3824: x = (0x3824 & 0x3F) * 16 = 576, y = 0x3824 >> 6 = 224. That is 16 colours at (576..591, 224), with
  colour 0 transparent.
- Draw calls: `ui_buildSprite` (`src/main/ui.c:818`) emits a SPRT (colour 0x808080, or semi-transparent 0x606060 when
  `flag`) plus a DR_TPAGE from `get_mode(1, 1, d->tpage)`, and the pen's `x` advances by `w`. `pen_drawSprite`
  (main 0x800513CC, hand-written asm) does the same for the dialogue renderer: tpage from the entry, x advances by `w`.
- **Upload:** `game_loadCommonImages` (`src/main/gamedata.c:140`) calls `file_loadTim` on DB_TIM items
  `0x40000 | {0, 0x2710, 0x2711, ..., 0x2716}` of the resident common DB (FF9.IMG dir 1, file 0x271A, loaded by
  `game_loadCommonDb`). TIM `0x42710` holds the buttons: 4 bpp, image (960,256) 64x38 halfwords (256x38 texels),
  CLUT block (576,224) 16x16 (16 palettes, rows 224-239). The other TIMs in that DB are `0x40000` (896,256) 64x52 /
  CLUT (624,224), `0x42711` (960,294) 64x26 / (624,240), `0x42712` (960,320) 64x64 / (576,240) 16x16 (the ATE banner
  text, see 3), `0x42713` (960,384) 64x24, `0x42714` (960,408) 24x24, `0x42715` (984,408) 40x24 and `0x42716` (896,320) 32x16.

### Index orders

- `g_buttonSprites` (main 0x8006BC18, `s32[8]`) = `{4, 5, 7, 6, 8, 10, 9, 11}`, i.e. by *action slot*:
  0 Cross, 1 Circle, 2 Triangle, 3 Square, 4 L1, 5 R1, 6 L2, 7 R2.
- `g_msgIconSprites` (main 0x8006E67C, `u8[]`) = `{5, 4, 7, 6, 8, 10, 9, 11, 17, 18, 0, 0, 4, 7, 10, 16}`. The first
  10 entries are indexed by `code - 0x30` (Circle, Cross, Triangle, Square, L1, R1, L2, R2, START, SELECT). Note that
  Circle and Cross are swapped relative to `g_buttonSprites`.
- Slot order matches `config_applyPadMapping` (`src/main/tu_800236C4.c:26`): game button `g_configButtons[i]`
  = {BTN_CROSS, BTN_CIRCLE, BTN_TRIANGLE, BTN_SQUARE, BTN_L1, BTN_R1, BTN_L2, BTN_R2}[i] is bound to the physical
  pad bits `g_configPadButtons[remap(i)]` = {PAD_CROSS 0x4000, PAD_CIRCLE 0x2000, PAD_TRIANGLE 0x1000,
  PAD_SQUARE 0x8000, PAD_L1 0x400, PAD_R1 0x800, PAD_L2 0x100, PAD_R2 0x200}[...].
- Pad remap (`config_remapButton`, `src/main/tu_800236C4.c:53`): when `GameConfig.padCustom` (GameState.config bit 1)
  is 1, `remap(i) = (GameState.padMap >> (i * 4)) & 0xF`, otherwise `i`. Nibble `i` holds the *physical button
  slot* (same 0-7 order) that performs action `i`, so the icon to show for action `i` is
  `g_buttonSprites[remap(i)]`, the button the player has to press. The default (`config_reset`) is the identity
  0x76543210.

### Text codes (all follow the escape byte 0xF9 = TEXT_ESC)

- **0x30-0x39, fixed buttons**: Circle, Cross, Triangle, Square, L1, R1, L2, R2, START, SELECT
  (`TEXT_BTN_*`, ff9.h:800). Menu renderer `text_buildCode` (`src/main/text.c:416`, cases at 459-512) uses the
  `UI_SPRITE_*` id directly. Dialogue renderer `msg_renderText` (`src/main/msg.c:407-420`) draws
  `g_msgIconSprites[c - 0x30]` after `pen_move(0, 1)`, then `pen_move(1, -1)`. Both give the same sprites.
- **0x60-0x67, remappable action buttons**: action slot `s` = {0x60: 1, 0x61: 0, 0x62: 2, 0x63: 3, 0x64: 4, 0x65: 5,
  0x66: 6, 0x67: 7}. 0x60 is the Circle action, 0x61 Cross, 0x62 Triangle, 0x63 Square, 0x64 L1, 0x65 R1, 0x66 L2,
  0x67 R2. Sprite = `g_buttonSprites[remap(s)]`.
  - `text_buildCode` 0x60 → `text_getButtonSprite(1)`, 0x61 → `(0)`, 0x62..0x67 → `(2..7)`
    (`text_getButtonSprite` = `g_buttonSprites[config_remapButton(n)]`, text.c:1194).
  - `msg_renderText` (msg.c:421-438) computes the same thing another way: `c ^= 1` for 0x60/0x61, then
    `n = custom ? (padMap >> (c * 4)) & 0xF : c & 0xF`, then `if (n < 2) n ^= 1`, and draws `g_msgIconSprites[n]`.
    **Port note:** `padMap >> (c * 4)` with c = 0x60..0x67 shifts by 0x180..0x19C. That only works because MIPS
    `srlv` uses the low 5 bits (0x184 & 31 = 4). In portable C write `(padMap >> ((c & 7) * 4)) & 0xF`.
  - Quirk: `text_getCodeWidth` (text.c:579, cases 608-629) measures 0x60 with slot 0 and 0x61 with slot 1, the other
    way round from the draw. It is harmless because every button sprite is 12 wide.
- **D-pad**: menu renderer `text_buildDpad(x, y, ot, mask, ...)` (text.c:1032) draws the arm highlights from
  `g_dpadArrowSprites` (main 0x8006BC0C, 4 x `{u8 x, u8 y, u8 sprite}` = {4,1,13} up, {4,6,14} down, {1,4,15} left,
  {6,4,16} right; mask bit i = entry i) at `(x + dx, y + dy)`, then sprite 12 at (x, y) (12 wide). Codes: 0x3A up
  (mask 1), 0x3B down (2), 0x3C left (4), 0x3D right (8), 0x3E up+down (3), 0x3F left+right (12). Dialogue renderer
  (msg.c:440-470): 0x3A/0x3B/0x3C/0x3D draw the highlight (13/14/15/16) at pen offsets (+4,+2)/(+4,+7)/(+1,+5)/(+6,+5),
  then the d-pad sprite; 0x2F draws the bare d-pad (12); 0x4B n draws any `g_uiSprites` id n inline.

### Runtime evidence
- `field_d1_101`, field 101 → 112. Dumped VRAM with `tools/rt/vram.lua` (`DumpVram`) and rendered ids 4-18
  through their UiSprite u/v/w/h/clut/tpage. The glyphs read Cross (blue), Circle (red), Square (pink),
  Triangle (green), L1, L2, R1, R2, the d-pad, four 4x4 / 4x3 arm pieces, START and SELECT.
- The VRAM rect (960,256) 64x38 and CLUT (576,224) 16x16 are byte-identical to TIM `0x42710` in dir 1 / file 0x271A
  of disc 1's FF9.IMG (both compared over the whole rect).

## 2. Field exits

**Short answer.** An exit is a *region* object: a convex polygon on the floor (x/z) plus script functions. The
engine runs the region's **function tag 2** on every frame in which the player's (x, z) is inside the polygon
and control is enabled. That function, after its own checks (walking in, story flags), sets the arrival-point
variable and executes **op 0x2B GOTO_FIELD u16 field** (or **0xB6 GOTO_WORLD u16 world**, 9000+). All of it is in
the field's event script, the DB item `0x50000 | fieldId` (DB_FIELD) of the field's FF9.IMG dir-4 file.

### Event script layout (main `evt_init`, `src/main/event.c:3318`, `evt_callObjectFunc_find` event.c:134)
- Header: `+0` "EV", `+3` u8 entry count, `+4` debug name, `+0x2C` location name, `+0x80` entry table of 8-byte
  `ScriptEntry {u16 offset (from +0x80), u16 size, u8 varBytes, u8 flags, u8 pad[2]}`.
- Entry at `+0x80 + offset`: `u16 h` with function count `h >> 8`, then that many `{u16 tag, u16 ofs}`. Code of tag
  T starts at `entry + 2 + ofs`. Tag 0 = init, tag 1 = main loop, tag 2 = "range" (player inside the region /
  touching the actor), tag 3 = Cross action (talk), tag 8 = Square action (the engine tries 8 before 3 on Square).
  `tools/encounter_catalog.py` `scripts()` / `script_functions()` already parse this.

### Instruction encoding (main `evt_execInstr` event.c:842, scheduler `evt_runScripts` asm)
- `op` byte. Ops 0x01/0x02/0x03 JMP/JMP_IFNOT/JMP_IF: `s16` offset relative to the end of the instruction (the IF
  forms pop the last expression result). 0x04 RETURN. 0x05 EXPR: an expression up to its 0x7F terminator. 0x06 /
  0x0B / 0x0D switches (see the opcode table in `docs/runtime-findings.md`).
- **0x07 / 0x08 / 0x09 NEW_CODE / NEW_REGION / NEW_ACTOR: `op, u8 entry, u8 uid`**, with no flags byte. Region =
  `evt_createObject(entry, uid, 0x38, 0x10)`, kind 3, flags 1. The new object starts at its entry's tag 0.
- Every other op: `op, flags, args...`. In `flags`, bit i set means argument i is an expression (bytes up to 0x7F)
  instead of a literal; literals are little-endian with the op's fixed width (u8 / s16). An op that reads no
  arguments has **no** flags byte (the scheduler steps ip back by one when `g_evtArgRead` stays 0). 0xFF is a
  prefix for a second opcode page.
- Expression bytes (`evt_runExpr`, jump table main 0x8006E718): 0x7F end; 0x7D s16 constant (2 bytes); 0x7E 26-bit
  constant (4 bytes); 0x78 2 bytes (big-endian); 0x79, 0x7A (system variable `evt_getSysVar`) and 0x29, 0x5F take 1
  byte; bytes >= 0x80 are variable references `0x80 | 0x40? | 0x20 wide index | type 0x1C | bank 0x03`, followed by a
  1-byte index (2 bytes when 0x20 is set). Banks: 0 = globals (0x8007AEF0), 1 = field-local `EvtState.fieldVars`
  (cleared on every field entry), 2 = object locals. Types: `0x00` bit, `0x08` s24, `0x10` s8, `0x14` u8, `0x18`
  s16, `0x1C` u16. All other expression ops take no inline bytes.

### Region polygon and the inside test
- **Op 0x29 (EVT_OP_SET_PATH, in practice "set region polygon"), `29 flags n {s16 x, s16 z}[n]`** (event.c:1287) writes
  `count = n` (<= 8) and the points into the current object's `Unk80039C38Quad` (`+0x14 s32 count`,
  `+0x18 {s16 x; s16 z}[8]`). Ops 0x76 (`x, z`: count 2, both points = (x,z)), 0x77 (`x, z`: overwrite the last
  point) and 0x78 (duplicate the last point, count++) edit it.
- `evt_isInRegion(obj, x, z)` (main 0x80053124, hand-written GTE asm): for each edge `p[i] -> p[(i+1) % n]`,
  `cross = dz * (x - p[i].x) - dx * (z - p[i].z)` with `dx = p[i+1].x - p[i].x` (16-bit),
  `dz = ((p[i+1].x | p[i+1].z << 16) - (p[i].x | p[i].z << 16)) >> 16` (arithmetic shift of the packed word
  difference, so it is `z1 - z0 - 1` when `(u16)x1 < (u16)x0`, a quirk), and `IR2` saturates to s16, which keeps
  the sign. Inside = all crosses >= 0, or all <= 0, so the test needs a convex polygon, accepts either winding and
  includes the edges.
- Engine call (`evt_checkPlayerAction`, event.c:2770, called from `evt_updateObjects` event.c:2987 for the player
  actor in field mode **only while `EvtState.controlEnabled`**): the first kind-3 object in the object list that has a
  tag-2 function and contains the player's (x, z) gets `evt_callObjectFunc(o, 1, 2, 0)` (level 1, so it does not
  restart while that function is still running at level <= 1). Cross / Square do the same with tag 3 / tag 8
  (Square first tries 8). The region functions then re-check `sysvar 2` (= controlEnabled), e.g. `05 7a 02 7f 03 01 00
  04` = "return unless control is enabled".

### Field change
- **0x2B GOTO_FIELD `2B flags u16 field`** → `evt_setNextLocation(field)`; `evt_execInstr` returns 4, and `fld_main`
  (`src/ovl_0a/fld_main.c:42-50`) sets `GS_FLD_NEW_FIELD` (field 0x3E80 is special: exit reason 4 + `GS_FLD_LEAVE`).
  **0xB6 GOTO_WORLD `B6 flags u16 world`** returns 5 → `FLD_EXIT_WORLD`. Also 0xAE card game (7) and 0xF5 game over (8).
- Arrival point: every exit sets the global **s16 at g_evtGlobalVars + 2 (0x8007AEF2)** right before the GOTO, e.g.
  field 101 region 17: `05 d8 02 7d c9 00 2c 7f 2b 00 70 00` ("var = 201; GOTO_FIELD 112"), and field 352
  region 14: `05 d8 02 7d 06 00 2c 7f 2b 00 5f 01` (6; field 351). The destination's init reads it to place the
  player, then resets it (field 101 / 112 set 10000).
- Static extraction recipe: in each entry, find `08 entry uid` (NEW_REGION) in any function; the region entry's tag 0
  holds `29 00 n points` (a literal polygon) and its tag 2 holds `2B 00 lo hi` / `B6 00 lo hi` (plus the `d8 02` arrival
  value before it). Byte-pattern level is good enough for the literal cases. Tag-2 functions often contain several
  targets behind story-flag branches (field 300 region 2: field 182 and world maps 9000-9012), and some exits use
  expression arguments (flags != 0), which need the expression parser. Results for three disc-1 fields:
  - 101: region 15 (3295,-65)(3326,-987)(2238,-1444)(2872,261) → 100; region 16 (-3500,-600)(-3500,-200)(-2621,444)(-2254,-1444)
    → 102; region 17 (110,233)(-100,233)(-183,-855)(206,-855) → 112.
  - 352: region 14 (-202,-1476)(188,-1476)(218,-876)(8,-756)(-228,-872) → 351.
  - 300: region 1 → 301; region 2 → 182 or a world map (story-dependent).

### Runtime evidence (`field_d1_101`, disc 1)
- `tools/rt/actors.lua` lists region objects uid 15/16/17 (kind 3, flags 01). Their +0x14 count is 4 and +0x18
  holds the points above (e.g. uid 15 word -4256545 = 0xFFBF0CDF = (3295, -65)).
- After teleporting Zidane (uid 19, EvtObject 0x80077FAC) to (0, -300) inside region 17, `trace evt_callObjectFunc`
  shows `a0 = 0x800787C0` (uid 17), `a1 = 1`, `a2 = 2` once per field frame (every 2 VBlanks) from
  `evt_checkPlayerAction+0x3F8`. The region function waits for the player to walk: after `press up --hold 90`,
  `evt_setNextLocation(0x70)` is called from `evt_execInstr+0x3D50` (op 0x2B), then `GameData.fieldId` = 112 and the
  screen shows the pub interior.
- Read / write breakpoints on 0x8007AEF2 over the same exit: a write by `evt_writeVar` in the frame of the GOTO,
  then in the new field's init a read (`evt_readVar`) and a write (reset to 10000).

## 3. Active Time Event prompt

**Short answer.** The engine knows nothing about ATEs. The prompt is a display mode set by script **op 0xD7
ATE_PROMPT u8 mode** (`msg_setAtePrompt`), and `msg_drawAtePrompt` draws it at (16, 200). "An ATE can be viewed now"
is a **script variable: global byte `g_evtGlobalVars.b[0xEE]` (0x8007AFDE) == 1**, which the field script sets
together with `D7 00 01`. Select is polled **by the field script**: a script object's loop tests
`controlEnabled == 1 && b[0xEE] == 1 && pressed(SELECT)` and then opens the "ACTIVE TIME EVENT" choice window
(msg 0x62, window flag 0x40), whose choices are masked by the available-ATE bits.

### Prompt drawing (`src/main/msg.c`)
- `g_atePromptMode` (0x8007BC10) and `g_atePromptTimer` (0x8007BC14). `msg_setAtePrompt(m)` (msg.c:1659) sets the
  mode and timer = 44 when the mode changes. `evt_init` resets it to 0 on every field entry.
- `msg_drawAtePrompt` (msg.c:1594), once per field frame (30 fps): nothing if mode 0; nothing without player control
  unless bit 2 (4) is set; timer++; hidden while `GD_HIDE_ALL` (menus). `pen_setPos(16, 200)`.
  - `(mode & 3) != 2` (modes 1 / 5): alternates every 44 frames between sprites 0x80, 0x81, 0x82 with 2 px gaps
    ("ACTIVE TIME EVENT", blue, CLUT row 255) and `pen_move(-2,0)`, 0x53, `pen_move(2,0)`, 0x12, 0x54 ("PRESS
    [SELECT] TO SEE ATE").
  - `(mode & 3) == 2` (mode 6): blinks (15 frames on, 15 off) sprites 0x1F, 0x20, 0x21, the same text in grey (CLUT
    row 251).
  - Sprites: 0x80/0x1F (u 64, v 64, 40x11), 0x81/0x20 (64, 75, 28x11), 0x82/0x21 (64, 86, 35x11), CLUT 0x3FE4 / 0x3EE4,
    texels from TIM 0x42712; 0x53 (152, 22, 26x10), 0x12 (168, 11, 34x11 = SELECT), 0x54 (178, 22, 48x10), CLUT 0x3824,
    TIM 0x42710. All tpage 0x1F.
- Literal uses on disc 1: D7(0) 261, D7(1) 220, D7(5) 25, D7(6) 8, D7(4) 1.

### Script side (disc-1 fields 200-208, 250-255, 350-358, 550-573, ...: 47 fields with this exact test)
- Main init / loop (e.g. field 202 entry 0 tag 1 +0x11E): `if (fieldbit 0x99 == 1) { if (u16 g[0xEC] != 0) {
  D7(1); fieldbit 0x9B = 1; s8 g[0xEE] = 1; s16 g[0xF1] = g[0xEC]; ... } else { D7(0); fieldbit 0x9B = 0; g[0xEE] = 0;
  g[0xF1] = 0 } }` (`05 c5 99 7d 01 00 20 7f 02 5f 00 05 dc ec 7d 00 00 21 7f 02 39 00 d7 00 01 05 c5 9b 7d 01 00 2c 7f
  05 d0 ee 7d 01 00 2c 7f 05 d8 f1 dc ec 2c 7f ...`). Alexandria fields (100-117) use a field-local u8 (`d5 18`) instead of
  g[0xEC] as the "available" value but set the same g[0xEE] / D7.
- Select handler (field 202 entry 2 tag 1 +0x2C; field 352 entry 1 tag 1 +0x2C):
  `05 7a 02 7d 01 00 20 d0 ee 7d 01 00 20 27 7d 01 00 4f 27 7f 02 a3 04` = if `sysvar2 == 1 && g.s8[0xEE] == 1 &&
  padPressed(1)`. Expression op **0x4F** = `(msg_getPadPressed() & mask) != 0` (0x58 released, 0x59 held; mask
  0x0001 = PAD_SELECT, 0x0008 = PAD_START). It then sets fieldbit 0x9A, disables control,
  `7C 01 [g.s16[0xF1] | 0x8000] 00` (MSG_CHOICE_MASK, the enabled ATE choices), `1F 00 01 40 62 00` (MSG_WAIT window 1,
  flags 0x40 = "Active Time Event" title `g_msgAteTitle`, msg 0x62), then `0B` switches on sysvar 9 (the choice) to
  run the chosen ATE.
- Unrelated engine use of Select: `msg_drawHereIcon` (msg.c:1446) shows the "here" hand over the player on Select
  whenever control is enabled.

### Runtime evidence (`field_d1_352`, Dali, disc 1)
- `write 0x8007AFDE 1`, then `press select`: a log breakpoint on the 0x4F handler (0x800525C8) hit once and
  `msg_open(0x62, 1, ..., 0x40)` was called. The screenshot shows the "ACTIVE TIME EVENT" window with
  "Select event / Cancel" (no ATE choices, since the mask var was 0). The same Select without the poke does nothing.
- `write 0x8007BC10 1` draws "PRESS [SELECT] TO SEE ATE" at the bottom left (x 16, y 200). The sprite sheet rendered
  from the VRAM dump reads "ACTIVE TIME EVENT" (blue and grey) / "PRESS" / "SELECT" / "TO SEE ATE".

## 4. Main menu cursor and opening a sub-screen directly

**Short answer.** `s_menuMain.cursor` (ovl_0a 0x800CB188 in the field, ovl_0c 0x800D5438 on the world map; byte
+0) is the command row 0-6 = Item, Ability, Equip, Status, Order, Card, Config. Command row r opens screen r + 1
(`MENU_SCREEN_*`). You can't preselect a screen before opening, because `menuScreen_init` forces screen 0 and runs
`menuMain_init`. Go straight to a screen *after* the main menu has initialised by calling
`menuScreen_open(MENU_SCREEN_X)`, the call `menuMain_selectCommand` / `menuMain_inputMember` make. It loads the
screen's overlay, fades and runs its init. For a clean return, set `s_menuMain.cursor = X - 1` and
`g_menuScreen.slideIn = 0` first (and `g_menuScreen.member` for the member screens). Tested with the card screen:
it opens, Circle returns to the main menu with the cursor on Card, and a second Circle closes the menu back to
the field.

### Details
- Opening: field Triangle → `menu_open(0, 0)` (`src/ovl_0a/fld_main.c:34`; world map `src/ovl_0c/w_frame.c:90`) →
  `menu_doOpen` (`src/main/menu.c:28`, needs `!GD_MENU_DISABLED`, `g_menu.open == 0`, state closed) → menu type 0 =
  `{menuScreen_init, menuScreen_update, menuScreen_exit}` (`g_menuTypes` 0x8006C9D4) once the fade-out ends.
- `menuScreen_init` (`src/main/menu_screen.c:3`): `g_menuScreen` (0x80077E50 `{u8 active, u8 slideIn, u8 screen,
  u8 member, s32 frame, s32 savedOtz}`) = active 1, slideIn 1, screen 0, member = first party member; resets
  `g_menuScreenSwitch`; `screens` = `g_menuScreensField` (0x8006E530) or `g_menuScreensWorld`; runs
  `screens[0].init` = `menuMain_init`.
- `menuMain_init` (`src/ovl_0a/menu_main.c:377`): if `slideIn` is set, `cursor = orderCursor = 0` (so every fresh
  open starts on Item), state = command list; panels slide in only while `slideIn`.
- Screen table (`g_menuScreensField`, `{init, update, exit, u8 needsMember}`) and overlays (`g_menuScreenOvl`
  0x8006E510): 0 main (none, in ovl_0a / ovl_0c), 1 Item (ovl 0x37), 2 Ability (0x33, needsMember), 3 Equip (0x33,
  needsMember), 4 Status (0x38, needsMember), 5 Order (no screen: NULL entry, handled inside menu_main), 6 Card
  (0x36), 7 Config (0x34). Overlays load at 0x801EDF00 (`RAM_SUB_OVL`).
- `menuScreen_open(n)` (`menu_screen.c:114`): ignored when `screens[n].update` is NULL (Order). Otherwise target = n,
  busy, title-bar resize and scroll animation, `menuScreen_loadOverlay` (async CD read of dir-0 overlay
  `g_menuScreenOvl[n]` unless already loaded), `onFaded = screens[current].exit`, fade out (otz 3). When everything
  settles, `menuScreen_updateOpen` sets `g_menuScreen.screen = n`, fades in, flushes the I-cache and calls
  `screens[n].init`. Sub-screens return with `menuScreen_open(0)` (Item, Card, Config, Status, save) or
  `menuScreen_switch` (Ability <-> Equip, no reload; `menuScreen_onSwitchFaded` also calls
  `menuMain{Field,World}_setCursor(screen - 1)`).
- Safe direct path: (1) `menu_open(0, 0)`; (2) after `menuScreen_init` has run (`g_menuScreen.active == 1`, e.g. at
  the first `menuMain_update`), set `s_menuMain.cursor = n - 1`, `g_menuScreen.slideIn = 0`, for n = 2/3/4 also
  `g_menuScreen.member` (0x80077E53, a party slot with a character), then call `menuScreen_open(n)` instead of that
  frame's `menuMain_update`. With `slideIn` left at 1, the return to the main menu slides the panels in again and
  resets the cursor to Item, which is harmless but differs from the normal path. Calling an overlay screen's init
  directly without `menuScreen_open` is not safe: its code is only in RAM after the overlay read.

### Runtime evidence (`field_d1_352`, disc 1)
- `press triangle`, `until 0x800C94B0` (ovl_0a `menuMain_update`, hit 23 frames later; `g_menuScreen` = 01 01 00 00),
  `write 0x800CB188 5`, `write 0x80077E51 0`, then `setReg a0 = 6`, `pc = 0x800361CC` (`menuScreen_open`), 150 frames:
  `g_menuScreen` = 01 00 06 00, `ovl` shows ovl_36 resident at 0x801EDF00, and the screenshot is the Card screen
  (Collectors Lv, card grid, "Card" title). Circle: back on the main menu with the hand on "Card" (cursor 5).
  Circle again: `g_menuScreen.active` = 0 and the Dali field is back with control.
- Same without clearing `slideIn`: Card opens just the same, but the return slides the panels in and the cursor is on
  Item (`menuMain_init` reset it).

## Proposed names (not applied)
- `EVT_OP_SET_PATH` (0x29) → `EVT_OP_SET_REGION` (or a comment): its only engine readers are the region inside test
  (`evt_isInRegion`) and op 0xA4; every region's tag-0 function starts with it. `Unk80039C38Quad` → `EvtRegion`
  (`count`, `pts`), `Unk80039C38Pt` → `EvtRegionPt`.
- 0x76 / 0x77 / 0x78 → `EVT_OP_REGION_SET_POINT` / `EVT_OP_REGION_MOVE_LAST` / `EVT_OP_REGION_DUP_LAST`.
- Event tag constants: `EVT_FUNC_INIT 0`, `EVT_FUNC_LOOP 1`, `EVT_FUNC_RANGE 2`, `EVT_FUNC_TALK 3`,
  `EVT_FUNC_SQUARE 8` (the keys passed to `evt_callObjectFunc` / `evt_checkPlayerAction_find`).
- Expression ops: `EVT_EXPR_PAD_PRESSED 0x4F`, `EVT_EXPR_PAD_RELEASED 0x58`, `EVT_EXPR_PAD_HELD 0x59`,
  `EVT_EXPR_END 0x7F`, `EVT_EXPR_CONST16 0x7D`, `EVT_EXPR_CONST32 0x7E`, `EVT_EXPR_SYSVAR 0x7A`.
- Globals (comments in ff9.h's story-flag list): `g_evtGlobalVars.b[0xEE]` "ATE available (Select opens the ATE
  menu)", `.h` at +0xEC "available-ATE bits", +0xF1 "ATE choice mask", +2 (s16) "arrival point for the next field".
- `g_atePromptMode` values: `ATE_PROMPT_OFF 0`, `ATE_PROMPT_SELECT 1` ("Press SELECT to see ATE"), `ATE_PROMPT_BLINK 2`,
  `ATE_PROMPT_ALWAYS 4` (bit: also without player control).
