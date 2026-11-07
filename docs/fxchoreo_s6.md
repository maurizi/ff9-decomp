# Choreography literals, slice 6 (fx_188 .. fx_1be)

The effect modules fx_188 .. fx_1be (the short summons Mega Flare, Tsunami, Diamond Dust, Judgement Bolt, Zantetsuken,
Flames of Hell and Worm Hole, the Seiken / alt sword skills, the enemy Death sequences and many enemy skills), named
per docs/fxdata.md "Choreography literals (pilot)". Each header has a `/* fx_XXX choreography */` block after its
effect-data names. Counts: `python3 tools/fxlits.py <module>` (actionable literals: compare / mask / case / arg /
assign), before = the slice's state at the start (main e3ebd8ed).

## Counts

| module | effect | before | after | defines | state |
|---|---|---|---|---|---|
| fx_188 | LV3 Def-less | 33 | 23 | 11 | done |
| fx_18a | enemy Transform (Kuja/Trance Kuja) | 89 | 61 | 27 | done |
| fx_18b | Gizamaluke's Silent Voice | 34 | 23 | 7 | done |
| fx_18c | enemy Light (Jabberwock) | 14 | 3 | 11 | done |
| fx_18d | Climhazzard | 13 | 5 | 6 | done |
| fx_18e | Charge! | 9 | 5 | 3 | done |
| fx_18f | What's That!? | 3 | 0 | 3 | done |
| fx_190 | enemy Viral Smoke (Meltigemini) | 17 | 9 | 6 | done |
| fx_191 | Lancer | 31 | 17 | 11 | done |
| fx_192 | enemy Death | 3 | 1 | 1 | done |
| fx_193 | enemy Counter Horn (Antlion) | 19 | 5 | 10 | done |
| fx_194 | enemy Shockwave (Nova Dragon, ...) | 31 | 21 | 9 | done |
| fx_195 | Bahamut (short); Mega Flare | 174 | 173 | 0 | partial: size |
| fx_196 | Leviathan (short); Tsunami | 97 | 60 | 20 | done via fx_0b3's names |
| fx_197 | Shiva, short summon | 92 | 34 | 48 | done via fx_026's names |
| fx_198 | enemy Tsunami (Black Waltz 1/Sealion) | 32 | 15 | 15 | done |
| fx_199 | Darkside (alt) | 6 | 2 | 2 | done |
| fx_19a | Minus Strike (alt) | 6 | 2 | 2 | done |
| fx_19b | Iai Strike (alt) | 6 | 3 | 2 | done |
| fx_19c | Thunder Slash (Seiken) | 10 | 3 | 5 | done |
| fx_19e | Stock Break (Seiken) | 11 | 6 | 3 | done |
| fx_19f | Judgement Bolt; Ramuh (short) | 212 | 166 | 36 | done via fx_0ba's names (+ sizes, GPU word) |
| fx_1a0 | enemy Flame Slash (Maliris) | 15 | 5 | 8 | done |
| fx_1a1 | Climhazzard (Seiken) | 12 | 5 | 6 | done |
| fx_1a2 | Moonlight Slash (Blank, enemy) | 163 | 132 | 17 | partial: size, pool, textures, trails |
| fx_1a3 | enemy Trouble Mucus (Antlion) | 19 | 7 | 12 | done |
| fx_1a4 | enemy Death0 (Deathguise) | 69 | 33 | 24 | done |
| fx_1a5 | Revive (Elan) | 9 | 5 | 3 | done |
| fx_1a6 | enemy Water-gun (Kraken, ...) | 30 | 14 | 14 | done |
| fx_1a7 | enemy Mind Blast (Drakan) | 34 | 8 | 24 | done |
| fx_1a8 | Odin (short); Zantetsuken | 101 | 100 | 0 | partial: size |
| fx_1a9 | enemy Virus Powder (Ring Leader, ...) | 2 | 1 | 1 | done |
| fx_1ab | enemy Waterga (Kraken, ...) | 25 | 8 | 11 | done |
| fx_1ac | enemy Virus Crunch (Chimera) | 52 | 19 | 20 | done |
| fx_1ad | enemy 6 Legs (Gigan Octopus) | 27 | 13 | 15 | done |
| fx_1af | enemy Death (Valia Pira) | 47 | 18 | 17 | done |
| fx_1b0 | enemy Death (Ozma) | 120 | 44 | 37 | done |
| fx_1b1 | enemy Golem Death (Core/Sand Golem) | 5 | 2 | 3 | done |
| fx_1b2 | enemy Tidal Wave (Nova Dragon) | 66 | 22 | 34 | done |
| fx_1b3 | enemy Death1 (Dummy/Necron) | 339 | 330 | 8 | partial: size, trails |
| fx_1b4 | enemy Flare Star (Kuja/Trance Kuja, ...) | 49 | 22 | 27 | done |
| fx_1b6 | enemy Death (Silver Dragon) | 50 | 25 | 17 | done |
| fx_1b7 | enemy Death (Nova Dragon) | 70 | 35 | 24 | done |
| fx_1b8 | enemy Death (Gizamaluke) | 14 | 8 | 4 | done |
| fx_1b9 | enemy Appearance (Gizamaluke) | 8 | 5 | 2 | done |
| fx_1bb | enemy Climhazzard (Beatrix) | 2 | 0 | 2 | done |
| fx_1bd | Ifrit, short summon | 169 | 158 | 13 | partial: sizes, textures |
| fx_1be | Atomos, short summon | 119 | 118 | 0 | partial: size |
| **total** | | **2558** | **1774** | **581** | |

784 literals named (31%; 730 of the 1,493 in the 42 finished modules, 49%). The six
partial modules (Mega Flare fx_195, Zantetsuken fx_1a8, Moonlight Slash fx_1a2, Death1 fx_1b3, Flames of Hell
fx_1bd, Worm Hole fx_1be: 1,065 literals) got only the mechanical names; their op state machines need a reading pass.

## Notes

- **abr pass first**: every literal abr of btlfx_drawScreenTint / drawParticles / drawRings / drawTrail / addPrimAbr /
  linkPrimAbr / Hi_ModifyEffModelAbr over the whole slice is a `BTLFX_ABR_*` name (131 sites in 18 modules); the
  five non-literal ones (fx_19f tpage, fx_1a2 / fx_1b2 / fx_1b3 / fx_1bd variables) stay.
- **Short summons reuse the long forms' names** (tool: build/twin.py in the worktree, token alignment of each common
  function, names taken where the long form has a choreography define and the short form a literal at the same
  token, with the short form's own value): fx_197 (fx_026 Shiva, 66 sites), fx_196 (fx_0b3 Leviathan, 36), fx_19f
  (fx_0ba Ramuh, 46). Where the long form writes `A + B` and the short one a folded literal (Shiva's flash fade,
  Ramuh's FLASH / YELLOW windows) the short form now writes the same expression. Values that differ from the long form:
  Shiva BARRIER_POOL_COUNT 7 (0x4A), FLASH_FADE stays 16; Leviathan WATER_CLUT 0x3D04 (0x3D07); Ramuh STRIKE_FRAMES
  0x2B (51), SKY_FRAME 0x21 (41), COLUMN_VRAM_X / SRC_X 0x260 / 0x240 (0x160 / 0x140: here u 0x80 / 0 of
  BLUE_AURA_COLUMN's page 0x19). fx_1be (Atomos) shares little code with fx_0b8 (another op80, its own grid helper):
  the transfer found only 8 sites, among them a lava VRAM rect with other coordinates, so it was not applied. fx_1bd's
  long form fx_114 has no choreography names yet. Sword skills reuse fx_0bc / fx_0bd / fx_0be / fx_0bf / fx_0cf's
  names (camera 7 frames here, 0xC there; Iai Strike's op80 ends on its first update), Revive fx_089's.
- **Countdown slots** in work arrays got slot names plus their lengths: fx_18c `fx_18c_timers[FX_LIGHT_T_RISE /
  _HOVER / _DROP]`, fx_1ac `timer0[FX_VIRUS_CRUNCH_T_JUMP / _RETURN / _MOTHS + n / _CLOUD]`.
- **Model textures**: every literal Hi_RegisterTexEffModel in the slice is named (fx_18a AURA, fx_198 BOWL, fx_1b2 CURL /
  SURGE, fx_1a2 BAND / BLADE / ARMOR, fx_1bd HAZE / BLOCK / FLAME, fx_196 / fx_197 / fx_19f through the long forms'
  names). fxdata.py's lit() resolves them: before / after md5 of all 857 model PNGs of the slice's renders
  (`tools/fxdata.py <48 modules> --render`) are identical.
- **VRAM rects** checked against the model tpage + uv: fx_1a4 COLUMN (tpage 0x17 u 0x80 v 0x80 -> (0x1E0, 0x180)) and
  COIL (0x18 v 0x80 -> (0x200, 0x180)); fx_1b2 BG_VRAM (0x240, 0x100) is exactly SURGE's page 0x119 (op83 renders the
  background there, op82's surge samples it); fx_1ad SLIME (0x200, 0x100) and fx_18a AURA (0x1C0, 0x120) lie inside the
  sampling models' pages (0x117 at run time / 0xB7).
- **Enemy Death sequences** (fx_1af, fx_1b0, fx_1b6, fx_1b7) share one scheme: `FX_DEATH_STATE_CHARGE / _SHATTER`,
  `FX_DEATH_MODELS`, `FX_DEATH_PIECES` (split-model face pieces: 0x7BC / 0x708 / 0x6A4), `FX_DEATH_PIECE_FRAMES`, the
  tints as `_RGB` triples, `/ (3 * sizeof(SVECTOR))` for Hi_GetSplitMdlVertex's byte count per face.
- **GPU words**: fx_18b's radial screen grid (GPU_POLY_GT4 | GPU_ABE, SCREEN_W / SCREEN_H_NTSC clamps, PRIM_TAG(POLY_GT4
  / DR_MOVE), sizeof(DR_MOVE)), fx_196's water mesh (PRIM_TAG(POLY_FT4 / F4 / GT4 / G4)), fx_19f's PRIM_TAG(POLY_G4).
  fx_1b4's `0xE6000001` (GP0 E6 mask-bit setting) stays: no shared name exists.
- **Derived values** as expressions: `FX_6_LEGS_FRAMES - FX_6_LEGS_SHELL_FRAME` (57), `FX_WATERGA_FADE_OUT_FRAME +
  _FADE_OUT_FRAMES` (0x3F), `FX_TSUNAMI_BOWL_FRAMES - _BOWL_FADE_FRAME`, `FX_SHOCKWAVE_FLASH_FRAME + _FLASH_FRAMES
  (- 1)`, `FX_TRANSFORM_FRAMES - _BURST_FRAME` (40), `FX_TRANSFORM_TRAIL_POINTS - 1` (14), `FX_DEATH0_FLAMES_FADE_FRAME
  + _FADE_FRAMES` (0x42), `FX_DEATH_CHARGE_FRAMES - 1` (white flash frames), `FX_DEATH_PULSE_FRAME + n *
  _PULSE_INTERVAL`, Viral Smoke's `n * FX_VIRAL_SMOKE_PUFF_INTERVAL`, trail buffer bounds
  `[SLOTS * STRIDE / 4]`, `FX_TIDAL_WAVE_INTERVAL - 1` as the wave mask.
- **Kept numeric** (per the scheme): otZ biases, random spreads, angle / fixed-point math and easing curves (the
  fxm_easeLerp curve ids), one-off scales and offsets, screen-grid / ripple-grid arguments, sound entry indices,
  unit ids, btlfx_moveSummonToHeap sizes, the split buffers' offsets (0x19000 / 0xC5A8 ...: not the struct sizes).
- **Open**: the six partial modules' state machines; whether fx_1be's lava rect is the same texture as fx_0b8's.
