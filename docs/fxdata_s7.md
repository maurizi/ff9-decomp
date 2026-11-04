# Battle effect data names, slice 7 (fx_1bf_0 .. fx_1fe)

Same scheme and tool as the pilot in `docs/fxdata.md`: 30 modules (fx_1bf_0..2, fx_1c9, fx_1cb, fx_1cc,
fx_1e6, fx_1e7, fx_1e9 .. fx_1fe), every `btlfx_getFileData` literal in them named (323 module/index defines:
262 by the render, 39 data tables by the export they are passed to, 22 by kind with the look pending). Renders
come from `tools/fxdata.py` at 8b8d391c (summon data 0x80+, fans, 'so' models, dot sprites). Evidence column:
`render` = named by the contact sheet, `usage` = a data table named by its export (camera keys, paths,
backdrops), `kind/usage (look pending)` = appearance not settled (open questions below).

Notes:
- Prefixes are the game's names (docs/effect-modules.md): enemy skills Sealion appears (fx_1cc), Correct!!!
  (fx_1e6), Wrong!!! (fx_1e7), Ultima (fx_1ec, Kuja's; the same prefix as the player's fx_180), Appearance
  (fx_1f0), Death3 (fx_1f1), Grand Cross (fx_1f2), Neutron Ring (fx_1f3), Aera (fx_1f6), Blue Shockwave (fx_1f7);
  Thunder Slash (fx_1c9) and Stock Break (fx_1cb) are byte-identical to fx_0bf / fx_0cf; Trance (fx_1e9, the
  Trance command outside Trance, prefix of fx_101), Dragon's Crest (`FX_DRAGONS_CREST_`), Six Dragons, Vanish,
  Night; summons Carbuncle (fx_1ed / fx_1ee / fx_1ef = the abilities "Carbuncle" 2-4, fx_1f8..fx_1fb their short
  forms), Fenrir (fx_1fc short, fx_1fd 2nd short), Phoenix (fx_1fe short), Ark (fx_1bf_0..2, the short form).
- Byte-identical entries were found by hashing every entry of every module (build-time helper, not committed);
  names carry over only through those. Where a module runs another module's C with other indices (the Carbuncle
  entrance op80 = fx_0b1's op80, Ark short = fx_17d_4 / 7 / 8, Phoenix short = fx_0d3), the call-site pairing
  was only used together with a matching kind and render ("same call site as ..., same kind").
- Carbuncle: the summon's entrance (op80) in fx_1ed / fx_1ee / fx_1ef takes fx_0b1's names; fx_0b1's `RED_RING`
  is `FX_CARBUNCLE_GLOW_RING` here (these files' ring is white / pale green). Each file's payload (op81) is named
  by its render: Carbuncle 3 blue rings, Carbuncle 2 a clock face with rings, Carbuncle 4 light streaks and a
  gold glow (fx_1f4 Vanish draws the same three emitters, in other bytes). The short forms carry the payload names.
- Sealion appears (fx_1cc) is Blizzaga's crystal data byte for byte (fx_097 / fx_098), named `FX_SEALION_APPEARS_`
  + Blizzaga's `<WHAT>`.
- Ark short, group 2 (fx_1bf_2): entry 2 (`FX_ARK_FLAT_PANEL`, byte-identical to fx_17d_8 entry 6) is textured with a
  placeholder page carrying the kana labels "up / left / right / down" (an orientation test texture).
- fx_1f2 (Grand Cross) and fx_1bf_1 use summon data (0x80+): Grand Cross's planets, moon, earth globe, vortices;
  Ark short's beam, tori, rocks and flame pieces, all named by the new 'so' model renders.
- Some names carried from other slices were "(look pending)" there and the new tool now draws them (fx_1ec
  `FX_ULTIMA_SO_MODEL`: dome with a red spike texture; fx_1f6 `FX_AERA_MODEL*`; fx_1fd `FX_FENRIR_EFFECT_MODEL*`;
  fx_1fe `FX_PHOENIX_MODEL_2/3/4`: flat flame dome, flame dome with a hole, smoke arc). The names were kept for
  the carry-over; renaming them belongs with their source modules (fx_180, fx_0cd, fx_0e2, fx_0d3).

## Open questions (emulator)

| module | index | name | question |
|---|---|---|---|
| fx_1e6 | 0x4 | `FX_CORRECT_SPARKLES` | the tube particle element renders black (texture not in the file) |
| fx_1ec | 0x2, 0x12, 0x27 | `FX_ULTIMA_NOISE_RING(_2/_3)` | ring texture not resolved (noise) |
| fx_1ec | 0xD | `FX_ULTIMA_SO_MODEL` | carried from fx_180; now drawn as a dome with a red spike texture, a better name wanted |
| fx_1f0 | 0x4 | `FX_APPEARANCE_SPRITES` | sprites render empty |
| fx_1f6 | 0x1, 0x3, 0x6, 0x8, 0xC | `FX_AERA_*` | carried from fx_0cd with their pending state |
| fx_1fd | 0x5, 0x6, 0x16 | `FX_FENRIR_EFFECT_MODEL(_2)`, `FX_FENRIR_SPARKS` | carried from fx_0e2 with their pending state |
| fx_1fe | 0x16 | `FX_PHOENIX_EMBERS` | sprites render empty here; named after fx_0d3 entry 0xF (same call site and kind) |

## Table

| module | index | name | evidence | what it is |
|---|---|---|---|---|
| fx_1c9 | 0x1 | `FX_THUNDER_SLASH_SPARKS` | render | emitter: cone of blue-green electric wisps, orange sparks and white glints (byte-identical to fx_0bf entry 0x1) |
| fx_1c9 | 0x2 | `FX_THUNDER_SLASH_LIGHTNING` | render | sprite: blue / red lightning streak, 12 frames (drawn directly) (byte-identical to fx_0bf entry 0x2) |
| fx_1cb | 0x1 | `FX_STOCK_BREAK_SLASH_ARC` | render | emitter: blue slash arc, fans (byte-identical to fx_0cf entry 0x1) |
| fx_1cc | 0x1 | `FX_SEALION_APPEARS_FREEZE_BURST` | render | emitter: white-pink rings + spark sprites (same call site as fx_097 entry 0x21) (byte-identical to fx_098 entry 0x27) |
| fx_1cc | 0x5 | `FX_SEALION_APPEARS_FROST_RING_2` | render | emitter: pale blue gouraud rings (byte-identical to fx_097 entry 0x22) |
| fx_1cc | 0x6 | `FX_SEALION_APPEARS_ICE_FLARES` | render | emitter: cyan ice glints, four elements (byte-identical to fx_097 entry 0x20) |
| fx_1cc | 0x7 | `FX_SEALION_APPEARS_ICE_FLARES_2` | render | emitter: cyan ice glints (byte-identical to fx_097 entry 0x1F) |
| fx_1cc | 0xB | `FX_SEALION_APPEARS_ICE_SPARKLES_2` | render | emitter: small blue ice sparkles while the crystal grows (same call site as fx_097 entry 0x29) (byte-identical to fx_098 entry 0x29) |
| fx_1cc | 0xD | `FX_SEALION_APPEARS_MIST` | render | emitter: grey frost mist at the crystal base (same call site as fx_097 entry 0x26) (byte-identical to fx_098 entry 0x1C) |
| fx_1cc | 0xE | `FX_SEALION_APPEARS_SHARD_MIST` | render | emitter: grey mist trailing each shard (pool item) (same call site as fx_097 entry 0x25) (byte-identical to fx_098 entry 0x26) |
| fx_1cc | 0xF | `FX_SEALION_APPEARS_MIST_HIGH` | render | emitter: grey frost mist higher up (same call site as fx_097 entry 0x27) (byte-identical to fx_098 entry 0x28) |
| fx_1cc | 0x1E | `FX_SEALION_APPEARS_CRYSTAL_SHEEN_2` | render | model: crystal 2 shape, reflection-map texture (byte-identical to fx_097 entry 0xF) |
| fx_1cc | 0x1F | `FX_SEALION_APPEARS_CRYSTAL_SHEEN_3` | render | model: crystal 3 shape, reflection-map texture (byte-identical to fx_097 entry 0x10) |
| fx_1cc | 0x20 | `FX_SEALION_APPEARS_CRYSTAL_SHEEN` | render | model: main crystal shape, reflection-map texture (byte-identical to fx_097 entry 0x11) |
| fx_1cc | 0x24 | `FX_SEALION_APPEARS_SNOW_RING` | render | rings: textured ring of blue snow flakes |
| fx_1cc | 0x25 | `FX_SEALION_APPEARS_FROST_RING` | render | rings: lilac-blue gouraud ring on the ground |
| fx_1cc | 0x26 | `FX_SEALION_APPEARS_SNOW_RING_2` | render | rings: textured ring of pale blue snow flakes |
| fx_1cc | 0x27 | `FX_SEALION_APPEARS_ICE_CRYSTAL_2` | render | model: ice crystal spike (hexagonal, ice texture) (byte-identical to fx_097 entry 0x12) |
| fx_1cc | 0x28 | `FX_SEALION_APPEARS_ICE_CRYSTAL_3` | render | model: wide ice crystal spike (byte-identical to fx_097 entry 0x13) |
| fx_1cc | 0x29 | `FX_SEALION_APPEARS_ICE_CRYSTAL` | render | model: main ice crystal spike (byte-identical to fx_097 entry 0x14) |
| fx_1e6 | 0x1 | `FX_CORRECT_STREAMER` | render | emitter: 3D trail with a thin white streamer texture |
| fx_1e6 | 0x2 | `FX_CORRECT_STREAMER_2` | render | emitter: 3D trail with a thin white streamer texture |
| fx_1e6 | 0x3 | `FX_CORRECT_STREAMER_3` | render | emitter: 3D trail with a thin white streamer texture |
| fx_1e6 | 0x4 | `FX_CORRECT_SPARKLES` | kind/usage (look pending) | emitter: tube of particles (renders black) + small dot sparkles |
| fx_1e6 | 0x5 | `FX_CORRECT_SPIRAL_STREAMER` | render | emitter: spiralling 3D trail, white crescent texture |
| fx_1e6 | 0x6 | `FX_CORRECT_SPIRAL_STREAMER_2` | render | emitter: spiralling 3D trail, white crescent texture |
| fx_1e6 | 0x7 | `FX_CORRECT_SPIRAL_STREAMER_3` | render | emitter: spiralling 3D trail, white crescent texture |
| fx_1e6 | 0x8 | `FX_CORRECT_GOLD_BALL` | render | 'so' model: closed golden ball |
| fx_1e6 | 0x9 | `FX_CORRECT_GOLD_BALL_OPEN` | render | 'so' model: the golden ball split open in two halves |
| fx_1e6 | 0xD | `FX_CORRECT_PATH_KEYS` | usage | table: position path keys for fxm_posPathInit (op 0x80) |
| fx_1e6 | 0xE | `FX_CORRECT_RED_TRAIL` | render | trail: red streak strip drawn along the path |
| fx_1e7 | 0x1 | `FX_WRONG_RED_SMOKE` | render | emitter: red smoke puffs |
| fx_1e7 | 0x7 | `FX_WRONG_RED_BURST` | render | emitter: red-pink explosion burst |
| fx_1e7 | 0x8 | `FX_WRONG_RED_RINGS` | render | emitter: dark-to-light red gouraud rings |
| fx_1e7 | 0x11 | `FX_WRONG_WHITE_TRAIL` | render | emitter: white screen trail (byte-identical to entry 0x12) |
| fx_1e7 | 0x12 | `FX_WRONG_WHITE_TRAIL_2` | render | emitter: white screen trail (byte-identical to entry 0x11) |
| fx_1e7 | 0x13 | `FX_WRONG_PATH_KEYS` | usage | table: position path keys for fxm_posPathInit (fx_1e7_posPath0) |
| fx_1e7 | 0x14 | `FX_WRONG_PATH_KEYS_2` | usage | table: position path keys for fxm_posPathInit (fx_1e7_posPath) |
| fx_1e9 | 0x1 | `FX_TRANCE_BLUE_RING` | render | rings: navy-white gouraud rings |
| fx_1e9 | 0x3 | `FX_TRANCE_AURA_CYLINDER` | render | model: open cylinder, blue-green cloud texture (byte-identical to fx_101 entry 0x3) |
| fx_1e9 | 0x5 | `FX_TRANCE_FAN_EMITTER` | render | emitter: two fan elements, thin blue-white light streaks (byte-identical to fx_101 entry 0x5) |
| fx_1e9 | 0x8 | `FX_TRANCE_VIOLET_TRAIL` | render | trail: violet sparkle strip, 12 frames |
| fx_1e9 | 0x9 | `FX_TRANCE_LILAC_RINGS` | render | rings: white / lilac / blue gouraud rings (byte-identical to fx_101 entry 0x9) |
| fx_1e9 | 0xA | `FX_TRANCE_WHITE_RINGS` | render | rings: white-blue gouraud rings |
| fx_1e9 | 0xF | `FX_TRANCE_PINK_STREAKS` | render | emitter: two orbiting fans, pink-white light streaks |
| fx_1e9 | 0x10 | `FX_TRANCE_VIOLET_STREAKS` | render | emitter: two orbiting fans, violet light streaks |
| fx_1ea | 0x1 | `FX_DRAGONS_CREST_GREEN_EMBLEM` | render | emitter: green horned dragon-crest emblem |
| fx_1ea | 0x2 | `FX_DRAGONS_CREST_WHITE_EMBLEM` | render | emitter: white horned dragon-crest emblem |
| fx_1ea | 0x3 | `FX_DRAGONS_CREST_ROCK_DEBRIS` | render | emitter: spiral of grey rock chunks |
| fx_1ea | 0x4 | `FX_DRAGONS_CREST_ROCK_DEBRIS_2` | render | emitter: grey rock chunks, five elements |
| fx_1ea | 0x5 | `FX_DRAGONS_CREST_WHITE_RINGS` | render | emitter: white gouraud rings, 45 frames |
| fx_1ea | 0x6 | `FX_DRAGONS_CREST_GREEN_RINGS` | render | emitter: dark-to-light green gouraud rings |
| fx_1ea | 0x7 | `FX_DRAGONS_CREST_DUST_RINGS` | render | emitter: grey-brown to white gouraud rings |
| fx_1ea | 0xB | `FX_DRAGONS_CREST_ROT_PATH_KEYS` | usage | table: rotation / position path keys for fxm_rotPosPathInit |
| fx_1ea | 0x13 | `FX_DRAGONS_CREST_FAN_EMITTER` | render | emitter: fan, green light streaks (byte-identical to fx_03d entry 0x10) |
| fx_1ea | 0x14 | `FX_DRAGONS_CREST_SPARKLE_CIRCLE` | render | emitter: circle of gold / white sparkle quads + blue-green specks (byte-identical to fx_03d entry 0x11) |
| fx_1ea | 0x1C | `FX_DRAGONS_CREST_VIOLET_ENERGY` | render | emitter: violet crackling energy shapes + violet sub-sparks |
| fx_1ea | 0x1D | `FX_DRAGONS_CREST_FAN_EMITTER_2` | render | emitter: two fans, white / green light streaks |
| fx_1ea | 0x20 | `FX_DRAGONS_CREST_DRAGON_TRAIL` | render | trail: orange dragon-scale strip |
| fx_1ea | 0x21 | `FX_DRAGONS_CREST_DRAGON_TRAIL_2` | render | trail: orange dragon-scale strip |
| fx_1ea | 0x22 | `FX_DRAGONS_CREST_ROCK_DEBRIS_3` | render | emitter: spiral of grey rock chunks and a grey glow |
| fx_1ea | 0x23 | `FX_DRAGONS_CREST_ROCK_DEBRIS_4` | render | emitter: spiral of grey rock chunks, two elements |
| fx_1eb | 0x1 | `FX_SIX_DRAGONS_DRAGON_ORBS` | render | emitter: orbiting green-gold and red dragon-eye orbs |
| fx_1eb | 0x3 | `FX_SIX_DRAGONS_GREEN_AURA` | render | emitter: green orb rings and rising green-gold aura flames |
| fx_1eb | 0x4 | `FX_SIX_DRAGONS_WHITE_RINGS` | render | emitter: pale blue / white gouraud rings |
| fx_1eb | 0x5 | `FX_SIX_DRAGONS_BLUE_BUBBLES` | render | emitter: blue hemisphere bubbles + small bubble sprites |
| fx_1eb | 0x6 | `FX_SIX_DRAGONS_STONE_DRAGON` | render | 'so' model: winged stone dragon (rock texture) |
| fx_1eb | 0x7 | `FX_SIX_DRAGONS_VALUE_PATH` | usage | table: value path keys for fxm_valuePathInit |
| fx_1eb | 0x8 | `FX_SIX_DRAGONS_STONE_TRAIL` | render | emitter: spiralling 3D trail with a rock texture, small spark sprites |
| fx_1eb | 0x14 | `FX_SIX_DRAGONS_SPARKLE_CIRCLE` | render | emitter: circle of gold / white sparkle quads + blue-green specks (byte-identical to fx_03d entry 0x11) |
| fx_1eb | 0x15 | `FX_SIX_DRAGONS_FAN_EMITTER` | render | emitter: fan, green light streaks (byte-identical to fx_03d entry 0x10) |
| fx_1eb | 0x1D | `FX_SIX_DRAGONS_WHITE_RINGS_2` | render | emitter: white gouraud rings, 45 frames |
| fx_1eb | 0x1E | `FX_SIX_DRAGONS_SMOKE` | render | emitter: green-to-pink smoke puffs |
| fx_1ec | 0x1 | `FX_ULTIMA_RED_FLARE_2` | render | emitter: red lens streak and white-red gouraud rings (byte-identical to fx_180 entry 0x18) |
| fx_1ec | 0x2 | `FX_ULTIMA_NOISE_RING` | kind/usage (look pending) | emitter: ring with a texture not resolved (noise) + pink gouraud ring (byte-identical to fx_180 entry 0x2) |
| fx_1ec | 0x3 | `FX_ULTIMA_ROCK_DEBRIS` | render | emitter: grey rock chunks (byte-identical to fx_180 entry 0x3) |
| fx_1ec | 0x4 | `FX_ULTIMA_GREY_TRAIL` | render | emitter: wobbling 3D trail with a grey streak texture (byte-identical to fx_180 entry 0x4) |
| fx_1ec | 0x6 | `FX_ULTIMA_PINK_SPIKES` | render | emitter: pink spike particles + spark sprites (byte-identical to fx_180 entry 0x14) |
| fx_1ec | 0x7 | `FX_ULTIMA_RED_LENS_FLARE` | render | emitter: red lens streak, white-pink rings, red sparks (byte-identical to fx_180 entry 0x6) |
| fx_1ec | 0x8 | `FX_ULTIMA_COLOR_RING` | render | emitter: yellow, lilac, violet and magenta gouraud rings (byte-identical to fx_180 entry 0x7) |
| fx_1ec | 0x9 | `FX_ULTIMA_ROCK_DEBRIS_2` | render | emitter: spiral of grey rock chunks (byte-identical to fx_180 entry 0x8) |
| fx_1ec | 0xB | `FX_ULTIMA_PATH_SET_2` | usage | table: 30 position paths (fxm_offsetTableEntry, fxm_posPathInit), op 0x81 (byte-identical to fx_180 entry 0x1A) |
| fx_1ec | 0xD | `FX_ULTIMA_SO_MODEL` | kind/usage (look pending) | 'so' model (not drawn by the tool), drawn with FX_ULTIMA_GLOW_DOME (byte-identical to fx_180 entry 0x17) |
| fx_1ec | 0xE | `FX_ULTIMA_GLOW_DOME` | render | model: gouraud octagonal dome (20 polys) (byte-identical to fx_180 entry 0x1) |
| fx_1ec | 0xF | `FX_ULTIMA_GREY_TRAIL_2` | render | emitter: wobbling screen trail with a grey streak texture (byte-identical to fx_180 entry 0xB) |
| fx_1ec | 0x10 | `FX_ULTIMA_BLAST_RINGS` | render | emitter: white-pink, lilac and yellow-red gouraud rings, pink glows (byte-identical to fx_180 entry 0xC) |
| fx_1ec | 0x11 | `FX_ULTIMA_PINK_SPARKS` | render | emitter: sparse pink spark sprites (byte-identical to fx_180 entry 0xE) |
| fx_1ec | 0x12 | `FX_ULTIMA_NOISE_RING_2` | kind/usage (look pending) | emitter: ring with a texture not resolved (noise) + pink-lilac gouraud rings (byte-identical to fx_180 entry 0xF) |
| fx_1ec | 0x13 | `FX_ULTIMA_RED_FLARE` | render | emitter: red lens streak and white-red gouraud rings (byte-identical to fx_180 entry 0x10) |
| fx_1ec | 0x14 | `FX_ULTIMA_FAN_EMITTER` | render | emitter: orbiting fan, pale pink light streaks (byte-identical to fx_180 entry 0x11) |
| fx_1ec | 0x15 | `FX_ULTIMA_PINK_TRAIL` | render | emitter: pink screen trail (byte-identical to fx_180 entry 0x12) |
| fx_1ec | 0x16 | `FX_ULTIMA_PINK_TRAIL_2` | render | emitter: orbiting pink screen trail (byte-identical to fx_180 entry 0x13) |
| fx_1ec | 0x17 | `FX_ULTIMA_PINK_ORB_BURST` | render | emitter: pink orb and blade particles, sparks, white-yellow-red gouraud rings (byte-identical to fx_180 entry 0x16) |
| fx_1ec | 0x18 | `FX_ULTIMA_RED_BURST` | render | emitter: red flame-burst particles |
| fx_1ec | 0x1E | `FX_ULTIMA_PATH_SET` | usage | table: 18 position paths (fxm_offsetTableEntry, fxm_posPathInit), op 0x80 (byte-identical to fx_180 entry 0x22) |
| fx_1ec | 0x20 | `FX_ULTIMA_ROCK_SWIRL` | render | emitter: spiralling 3D trail of rock pieces |
| fx_1ec | 0x21 | `FX_ULTIMA_PINK_RING` | render | emitter: pink gouraud rings + fan (byte-identical to fx_180 entry 0x9) |
| fx_1ec | 0x22 | `FX_ULTIMA_ROCK_TRAIL` | render | emitter: screen trail of rock pieces (byte-identical to fx_180 entry 0xA) |
| fx_1ec | 0x23 | `FX_ULTIMA_ROCK_SWIRL_2` | render | emitter: spiralling 3D trail of rock pieces + sparks |
| fx_1ec | 0x24 | `FX_ULTIMA_PINK_SPIKES_2` | render | emitter: pink spike particles + spark sprites (byte-identical to fx_180 entry 0x15) |
| fx_1ec | 0x25 | `FX_ULTIMA_WHITE_BEAM` | render | emitter: spiralling white beam screen trail, pink sparks (byte-identical to fx_180 entry 0xD) |
| fx_1ec | 0x26 | `FX_ULTIMA_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop (byte-identical to fx_180 entry 0x19) |
| fx_1ec | 0x27 | `FX_ULTIMA_NOISE_RING_3` | kind/usage (look pending) | emitter: ring with a texture not resolved (noise) + lilac gouraud ring |
| fx_1ed | 0x1 | `FX_CARBUNCLE_BLUE_RINGS` | render | emitter: pale blue gouraud rings at each target (op 0x81) |
| fx_1ed | 0x3 | `FX_CARBUNCLE_CAMERA_KEYS_3` | usage | table: camera key path for fxm_camKeyAdd (target 3) (same call site as fx_0b1 entry 0x17, same kind) |
| fx_1ed | 0x4 | `FX_CARBUNCLE_CAMERA_KEYS` | usage | table: camera key path for fxm_camKeyAdd (target 1) (same call site as fx_0b1 entry 0xF, same kind) |
| fx_1ed | 0x5 | `FX_CARBUNCLE_CAMERA_KEYS_2` | usage | table: camera key path for fxm_camKeyAdd (target 2) (same call site as fx_0b1 entry 0x19, same kind) |
| fx_1ed | 0xF | `FX_CARBUNCLE_LENS_FLARE` | render | emitter: orbiting white glow trail with a blue lens streak (same call site as fx_0b1 entry 0x9, same kind) |
| fx_1ed | 0x10 | `FX_CARBUNCLE_ORBIT_GLOW` | render | emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0xA, same kind) |
| fx_1ed | 0x11 | `FX_CARBUNCLE_GLOW_RING` | render | emitter: white glow fans and a white / pale green gouraud ring (same call site as fx_0b1 entry 0xC, same kind) |
| fx_1ed | 0x12 | `FX_CARBUNCLE_RING_BURST_2` | render | emitter: cone of white, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x1, same kind) |
| fx_1ed | 0x13 | `FX_CARBUNCLE_LENS_STREAK` | render | emitter: blue-white lens streak and a gouraud ring (same call site as fx_0b1 entry 0xE, same kind) |
| fx_1ed | 0x14 | `FX_CARBUNCLE_FLAME_GLINT` | render | emitter: orange-blue flame-shaped glints, gouraud rings, fans (same call site as fx_0b1 entry 0x13, same kind) |
| fx_1ed | 0x15 | `FX_CARBUNCLE_RING_BURST_3` | render | emitter: cone of white, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x14, same kind) |
| fx_1ed | 0x16 | `FX_CARBUNCLE_LENS_STREAK_2` | render | emitter: blue-white lens streak, sparkle sprites (same call site as fx_0b1 entry 0x15, same kind) |
| fx_1ed | 0x17 | `FX_CARBUNCLE_ORBIT_GLOW_3` | render | emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x24, same kind) |
| fx_1ed | 0x18 | `FX_CARBUNCLE_ORBIT_GLOW_2` | render | emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x16, same kind) |
| fx_1ed | 0x19 | `FX_CARBUNCLE_RING_BURST` | render | emitter: cone of white-yellow, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x11, same kind) |
| fx_1ed | 0x1A | `FX_CARBUNCLE_CRYSTAL_SPARKLES` | render | emitter: white crystal-flake sprites and blue sparkles, fans (same call site as fx_0b1 entry 0x12, same kind) |
| fx_1ed | 0x1B | `FX_CARBUNCLE_FAN_EMITTER` | render | emitter: one fan, white glow streak (same call site as fx_0b1 entry 0xD, same kind) |
| fx_1ed | 0x1D | `FX_CARBUNCLE_LIGHT_BEAM` | render | trail: blue-white light beam, drawn twice (same call site as fx_0b1 entry 0x10, same kind) |
| fx_1ee | 0x2 | `FX_CARBUNCLE_YELLOW_RED_RINGS` | render | emitter: white-yellow and white-red gouraud rings at each target |
| fx_1ee | 0x13 | `FX_CARBUNCLE_VIOLET_RINGS` | render | emitter: white / pale blue / violet gouraud rings, 24 frames |
| fx_1ee | 0x14 | `FX_CARBUNCLE_GREEN_RING` | render | emitter: white-green gouraud ring |
| fx_1ee | 0x17 | `FX_CARBUNCLE_CLOCK_FACE` | render | emitter: clock face with Roman numerals and a blue ring glow |
| fx_1ee | 0x1E | `FX_CARBUNCLE_CAMERA_KEYS` | usage | table: camera key path for fxm_camKeyAdd (target 1) (same call site as fx_0b1 entry 0xF, same kind) |
| fx_1ee | 0x1F | `FX_CARBUNCLE_CAMERA_KEYS_3` | usage | table: camera key path for fxm_camKeyAdd (target 3) (same call site as fx_0b1 entry 0x17, same kind) |
| fx_1ee | 0x20 | `FX_CARBUNCLE_CAMERA_KEYS_2` | usage | table: camera key path for fxm_camKeyAdd (target 2) (same call site as fx_0b1 entry 0x19, same kind) |
| fx_1ee | 0x2B | `FX_CARBUNCLE_LENS_STREAK` | render | emitter: blue-white lens streak and a gouraud ring (same call site as fx_0b1 entry 0xE, same kind) |
| fx_1ee | 0x2C | `FX_CARBUNCLE_LENS_STREAK_2` | render | emitter: blue-white lens streak, sparkle sprites (same call site as fx_0b1 entry 0x15, same kind) |
| fx_1ee | 0x2E | `FX_CARBUNCLE_ORBIT_GLOW` | render | emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0xA, same kind) |
| fx_1ee | 0x30 | `FX_CARBUNCLE_RING_BURST_2` | render | emitter: cone of white, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x1, same kind) |
| fx_1ee | 0x31 | `FX_CARBUNCLE_RING_BURST_3` | render | emitter: cone of white, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x14, same kind) |
| fx_1ee | 0x32 | `FX_CARBUNCLE_ORBIT_GLOW_2` | render | emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x16, same kind) |
| fx_1ee | 0x33 | `FX_CARBUNCLE_LIGHT_BEAM` | render | trail: blue-white light beam, drawn twice (same call site as fx_0b1 entry 0x10, same kind) |
| fx_1ee | 0x35 | `FX_CARBUNCLE_FLAME_GLINT` | render | emitter: orange-blue flame-shaped glints, gouraud rings, fans (same call site as fx_0b1 entry 0x13, same kind) |
| fx_1ee | 0x36 | `FX_CARBUNCLE_ORBIT_GLOW_3` | render | emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x24, same kind) |
| fx_1ee | 0x38 | `FX_CARBUNCLE_CRYSTAL_SPARKLES` | render | emitter: white crystal-flake sprites and blue sparkles, fans (same call site as fx_0b1 entry 0x12, same kind) |
| fx_1ee | 0x39 | `FX_CARBUNCLE_GLOW_RING` | render | emitter: white glow fans and a white / pale green gouraud ring (same call site as fx_0b1 entry 0xC, same kind) |
| fx_1ee | 0x3A | `FX_CARBUNCLE_RING_BURST` | render | emitter: cone of white-yellow, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x11, same kind) |
| fx_1ee | 0x3B | `FX_CARBUNCLE_LENS_FLARE` | render | emitter: orbiting white glow trail with a blue lens streak (same call site as fx_0b1 entry 0x9, same kind) |
| fx_1ee | 0x3C | `FX_CARBUNCLE_FAN_EMITTER` | render | emitter: one fan, white glow streak (same call site as fx_0b1 entry 0xD, same kind) |
| fx_1ef | 0x3 | `FX_CARBUNCLE_WHITE_STREAK` | render | emitter: orbiting sprite (blank) + screen trail with a white light-streak texture |
| fx_1ef | 0x4 | `FX_CARBUNCLE_LENS_FLARE` | render | emitter: orbiting white glow trail with a blue lens streak (same call site as fx_0b1 entry 0x9, same kind) |
| fx_1ef | 0x5 | `FX_CARBUNCLE_RING_BURST_2` | render | emitter: cone of white, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x1, same kind) |
| fx_1ef | 0x6 | `FX_CARBUNCLE_LENS_STREAK` | render | emitter: blue-white lens streak and a gouraud ring (same call site as fx_0b1 entry 0xE, same kind) |
| fx_1ef | 0x7 | `FX_CARBUNCLE_RING_BURST_3` | render | emitter: cone of white, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x14, same kind) |
| fx_1ef | 0x8 | `FX_CARBUNCLE_LENS_STREAK_2` | render | emitter: blue-white lens streak, sparkle sprites (same call site as fx_0b1 entry 0x15, same kind) |
| fx_1ef | 0x9 | `FX_CARBUNCLE_RING_BURST` | render | emitter: cone of white-yellow, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x11, same kind) |
| fx_1ef | 0xA | `FX_CARBUNCLE_CRYSTAL_SPARKLES` | render | emitter: white crystal-flake sprites and blue sparkles, fans (same call site as fx_0b1 entry 0x12, same kind) |
| fx_1ef | 0xB | `FX_CARBUNCLE_ORBIT_GLOW` | render | emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0xA, same kind) |
| fx_1ef | 0xC | `FX_CARBUNCLE_WHITE_STREAK_2` | render | emitter: orbiting screen trail with a white light-streak texture |
| fx_1ef | 0xD | `FX_CARBUNCLE_GOLD_GLOW` | render | emitter: golden glow sprite + pale yellow gouraud rings |
| fx_1ef | 0xE | `FX_CARBUNCLE_FLAME_GLINT` | render | emitter: orange-blue flame-shaped glints, gouraud rings, fans (same call site as fx_0b1 entry 0x13, same kind) |
| fx_1ef | 0xF | `FX_CARBUNCLE_ORBIT_GLOW_3` | render | emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x24, same kind) |
| fx_1ef | 0x10 | `FX_CARBUNCLE_FAN_EMITTER` | render | emitter: one fan, white glow streak (same call site as fx_0b1 entry 0xD, same kind) |
| fx_1ef | 0x11 | `FX_CARBUNCLE_GLOW_RING` | render | emitter: white glow fans and a white / pale green gouraud ring (same call site as fx_0b1 entry 0xC, same kind) |
| fx_1ef | 0x12 | `FX_CARBUNCLE_LIGHT_BEAM` | render | trail: blue-white light beam, drawn twice (same call site as fx_0b1 entry 0x10, same kind) |
| fx_1ef | 0x13 | `FX_CARBUNCLE_CAMERA_KEYS` | usage | table: camera key path for fxm_camKeyAdd (target 1) (same call site as fx_0b1 entry 0xF, same kind) |
| fx_1ef | 0x14 | `FX_CARBUNCLE_CAMERA_KEYS_3` | usage | table: camera key path for fxm_camKeyAdd (target 3) (same call site as fx_0b1 entry 0x17, same kind) |
| fx_1ef | 0x15 | `FX_CARBUNCLE_CAMERA_KEYS_2` | usage | table: camera key path for fxm_camKeyAdd (target 2) (same call site as fx_0b1 entry 0x19, same kind) |
| fx_1ef | 0x1F | `FX_CARBUNCLE_ORBIT_GLOW_2` | render | emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x16, same kind) |
| fx_1f8 | 0x2 | `FX_CARBUNCLE_ORBIT_GLOW_2` | render | emitter: orbiting particle + sprite, orange speckled arcs (same call site as fx_0b1 entry 0x16, same kind) |
| fx_1f8 | 0x5 | `FX_CARBUNCLE_COLOR_RINGS` | render | emitter: pink / green / pink gouraud rings at each target body (byte-identical to fx_0b1 entry 0x18) |
| fx_1f8 | 0xA | `FX_CARBUNCLE_PINK_RING` | render | emitter: pink gouraud ring and swirl-ring particles at each target body (byte-identical to fx_0b1 entry 0x21) |
| fx_1f8 | 0xB | `FX_CARBUNCLE_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop (byte-identical to fx_0b1 entry 0x22) |
| fx_1f8 | 0xC | `FX_CARBUNCLE_STAR_CRYSTAL` | render | model: four-point star crystal (texture 0x177), one per target (byte-identical to fx_0b1 entry 0x23) |
| fx_1f9 | 0x1 | `FX_CARBUNCLE_BLUE_RINGS` | render | emitter: pale blue gouraud rings at each target (op 0x81) (byte-identical to fx_1ed entry 0x1) |
| fx_1f9 | 0x3 | `FX_CARBUNCLE_ORBIT_GLOW_2` | render | emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x16, same kind) |
| fx_1fa | 0x2 | `FX_CARBUNCLE_YELLOW_RED_RINGS` | render | emitter: white-yellow and white-red gouraud rings at each target (byte-identical to fx_1ee entry 0x2) |
| fx_1fa | 0x13 | `FX_CARBUNCLE_VIOLET_RINGS` | render | emitter: white / pale blue / violet gouraud rings, 24 frames (byte-identical to fx_1ee entry 0x13) |
| fx_1fa | 0x14 | `FX_CARBUNCLE_GREEN_RING` | render | emitter: white-green gouraud ring (byte-identical to fx_1ee entry 0x14) |
| fx_1fa | 0x17 | `FX_CARBUNCLE_CLOCK_FACE` | render | emitter: clock face with Roman numerals and a blue ring glow (byte-identical to fx_1ee entry 0x17) |
| fx_1fa | 0x1E | `FX_CARBUNCLE_ORBIT_GLOW_2` | render | emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x16, same kind) |
| fx_1fb | 0x1 | `FX_CARBUNCLE_ORBIT_GLOW_2` | render | emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x16, same kind) |
| fx_1fb | 0x4 | `FX_CARBUNCLE_WHITE_STREAK` | render | emitter: orbiting sprite (blank) + screen trail with a white light-streak texture (byte-identical to fx_1ef entry 0x3) |
| fx_1fb | 0x5 | `FX_CARBUNCLE_WHITE_STREAK_2` | render | emitter: orbiting screen trail with a white light-streak texture (byte-identical to fx_1ef entry 0xC) |
| fx_1fb | 0x6 | `FX_CARBUNCLE_GOLD_GLOW` | render | emitter: golden glow sprite + pale yellow gouraud rings (byte-identical to fx_1ef entry 0xD) |
| fx_1f0 | 0x1 | `FX_APPEARANCE_GREY_RINGS` | render | emitter: grey / white / blue gouraud rings |
| fx_1f0 | 0x3 | `FX_APPEARANCE_BLUE_SMOKE_COLUMN` | render | 'so' model: open cylinder, blue smoke texture |
| fx_1f0 | 0x4 | `FX_APPEARANCE_SPRITES` | kind/usage (look pending) | emitter: sprites, render empty |
| fx_1f0 | 0x5 | `FX_APPEARANCE_BLUE_SMOKE_TWIST` | render | 'so' model: twisted cylinder, blue smoke texture |
| fx_1f0 | 0x6 | `FX_APPEARANCE_BROWN_SMOKE_COLUMN` | render | 'so' model: open cylinder, brown smoke texture |
| fx_1f0 | 0x7 | `FX_APPEARANCE_BROWN_SMOKE_TWIST` | render | 'so' model: twisted cylinder, brown smoke texture |
| fx_1f1 | 0x2 | `FX_DEATH3_RUNE_CYLINDER` | render | 'so' model: cylinder of blue glowing runes |
| fx_1f1 | 0x4 | `FX_DEATH3_RUNE_CYLINDER_2` | render | 'so' model: cylinder of blue glowing runes (other texture row) |
| fx_1f1 | 0x5 | `FX_DEATH3_MAGIC_CIRCLE` | render | emitter: blue glowing magic circle with a star |
| fx_1f2 | 0x1 | `FX_GRAND_CROSS_ASTEROID` | render | 'so' model: dark cratered rock |
| fx_1f2 | 0x3 | `FX_GRAND_CROSS_TWINKLE` | render | emitter: tiny twinkling dot sprites |
| fx_1f2 | 0x4 | `FX_GRAND_CROSS_TWINKLE_2` | render | emitter: tiny twinkling dot sprites |
| fx_1f2 | 0x5 | `FX_GRAND_CROSS_COLOR_RINGS` | render | emitter: white-lilac, teal-orange and orange-magenta gouraud rings |
| fx_1f2 | 0x6 | `FX_GRAND_CROSS_WHITE_TRAIL` | render | trail: white streak strip |
| fx_1f2 | 0x7 | `FX_GRAND_CROSS_SMOKE_PANEL` | render | 'so' model: curved panel, grey smoke texture |
| fx_1f2 | 0x9 | `FX_GRAND_CROSS_NEBULA_PANEL` | render | 'so' model: curved panel, violet nebula texture |
| fx_1f2 | 0xA | `FX_GRAND_CROSS_LILAC_RINGS` | render | emitter: white-lilac and cyan gouraud rings, magenta smoke |
| fx_1f2 | 0xB | `FX_GRAND_CROSS_PALE_STREAKS` | render | emitter: two fans, pale light streaks |
| fx_1f2 | 0xC | `FX_GRAND_CROSS_WHITE_TRAIL_2` | render | trail: white streak strip |
| fx_1f2 | 0xD | `FX_GRAND_CROSS_PINK_RINGS` | render | emitter: pink-to-teal gouraud rings + smoke |
| fx_1f2 | 0xE | `FX_GRAND_CROSS_BLUE_RINGS` | render | emitter: blue-to-teal gouraud rings + smoke |
| fx_1f2 | 0xF | `FX_GRAND_CROSS_ASTEROID_2` | render | 'so' model: dark cratered rock |
| fx_1f2 | 0x10 | `FX_GRAND_CROSS_BEIGE_RINGS` | render | emitter: white-grey and beige gouraud rings |
| fx_1f2 | 0x11 | `FX_GRAND_CROSS_SMOKE_RINGS` | render | emitter: two rings with a grey smoke texture |
| fx_1f2 | 0x80 | `FX_GRAND_CROSS_ICE_PLANET` | render | summon data: 'so' model, pale blue planet sphere |
| fx_1f2 | 0x81 | `FX_GRAND_CROSS_BANDED_PLANET` | render | summon data: 'so' model, banded brown gas-giant sphere |
| fx_1f2 | 0x82 | `FX_GRAND_CROSS_BANDED_PLANET_2` | render | summon data: 'so' model, banded brown gas-giant sphere |
| fx_1f2 | 0x83 | `FX_GRAND_CROSS_ORANGE_PLANET` | render | summon data: 'so' model, swirled orange planet sphere |
| fx_1f2 | 0x84 | `FX_GRAND_CROSS_DARK_SPHERE` | render | summon data: 'so' model, dark speckled sphere |
| fx_1f2 | 0x85 | `FX_GRAND_CROSS_VORTEX_CONE` | render | summon data: 'so' model, dark cone with an orange vortex |
| fx_1f2 | 0x86 | `FX_GRAND_CROSS_CLOUD_DOME` | render | summon data: 'so' model, grey cloud dome |
| fx_1f2 | 0x87 | `FX_GRAND_CROSS_EARTH_GLOBE` | render | summon data: 'so' model, blue-white earth-like globe |
| fx_1f2 | 0x88 | `FX_GRAND_CROSS_CLOUD_DISC` | render | summon data: 'so' model, dark blue cloud disc |
| fx_1f2 | 0x89 | `FX_GRAND_CROSS_GALAXY_RING` | render | summon data: 'so' model, band ring with a starry nebula texture |
| fx_1f2 | 0x8A | `FX_GRAND_CROSS_RED_PLANET` | render | summon data: 'so' model, red planet sphere |
| fx_1f2 | 0x8B | `FX_GRAND_CROSS_GREY_MOON` | render | summon data: 'so' model, cratered grey moon sphere |
| fx_1f2 | 0x8C | `FX_GRAND_CROSS_VORTEX_HOURGLASS` | render | summon data: 'so' model, purple-orange hourglass vortex |
| fx_1f2 | 0x8D | `FX_GRAND_CROSS_BLUE_SPIKE` | render | summon data: 'so' model, gouraud blue spike |
| fx_1f2 | 0x8E | `FX_GRAND_CROSS_BLUE_HALO` | render | summon data: 'so' model, gouraud blue ring halo |
| fx_1f3 | 0x1 | `FX_NEUTRON_RING_FIRE_DISC` | render | 'so' model: flat ring disc, orange-teal fire texture |
| fx_1f3 | 0x3 | `FX_NEUTRON_RING_LIGHTNING_FLASH` | render | emitter: white glow flashes and violet lightning |
| fx_1f3 | 0x4 | `FX_NEUTRON_RING_LIGHT_STREAKS` | render | emitter: two fans, pale yellow light streaks |
| fx_1f3 | 0x5 | `FX_NEUTRON_RING_ORBIT_TRAIL` | render | emitter: orbiting 3D trail, white streak texture |
| fx_1f3 | 0x7 | `FX_NEUTRON_RING_YELLOW_RINGS` | render | emitter: pale yellow and yellow-green gouraud rings |
| fx_1f3 | 0x8 | `FX_NEUTRON_RING_TEAL_TORUS` | render | 'so' model: teal ring torus |
| fx_1f3 | 0x9 | `FX_NEUTRON_RING_FIRE_BOWL` | render | 'so' model: bowl, orange-teal fire texture |
| fx_1f3 | 0xB | `FX_NEUTRON_RING_COLOR_RINGS` | render | emitter: white-lilac, peach-pink and teal-orange gouraud rings |
| fx_1f3 | 0xD | `FX_NEUTRON_RING_TEAL_ARC` | render | 'so' model: teal arc band |
| fx_1f3 | 0xE | `FX_NEUTRON_RING_LIGHT_STREAKS_2` | render | emitter: two fans, pale yellow light streaks |
| fx_1f3 | 0xF | `FX_NEUTRON_RING_YELLOW_RINGS_2` | render | emitter: yellow / pale yellow gouraud rings |
| fx_1f3 | 0x10 | `FX_NEUTRON_RING_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop |
| fx_1f3 | 0x11 | `FX_NEUTRON_RING_VIOLET_TRAIL` | render | trail: violet noise strip |
| fx_1f3 | 0x12 | `FX_NEUTRON_RING_VIOLET_RINGS` | render | emitter: white-lilac, peach and violet gouraud rings |
| fx_1f3 | 0x13 | `FX_NEUTRON_RING_GREY_DISC` | render | 'so' model: flat ring disc, grey streak texture |
| fx_1f3 | 0x14 | `FX_NEUTRON_RING_GREEN_DISC` | render | 'so' model: flat ring disc, green-yellow gouraud |
| fx_1f4 | 0x1 | `FX_VANISH_WHITE_STREAK_2` | render | emitter: orbiting screen trail with a white light-streak texture |
| fx_1f4 | 0x2 | `FX_VANISH_WHITE_STREAK` | render | emitter: orbiting sprite (blank) + screen trail with a white light-streak texture |
| fx_1f4 | 0x4 | `FX_VANISH_GOLD_GLOW` | render | emitter: golden glow sprite + pale yellow gouraud rings |
| fx_1f5 | 0x1 | `FX_NIGHT_MOON_CLOUDS` | render | emitter: dark clouds and a full moon |
| fx_1f5 | 0x5 | `FX_NIGHT_PINK_GLOW_SWIRL` | render | emitter: helix of white screen trails with pink glows |
| fx_1f5 | 0x6 | `FX_NIGHT_PINK_WISPS` | render | emitter: pink wisp sprites |
| fx_1f6 | 0x1 | `FX_AERA_MODEL_3` | kind/usage (look pending) | 'so' model (byte-identical to fx_0cd entry 0x1) |
| fx_1f6 | 0x3 | `FX_AERA_MODEL_2` | kind/usage (look pending) | 'so' model (byte-identical to fx_0cd entry 0x3) |
| fx_1f6 | 0x4 | `FX_AERA_GREEN_RING` | render | emitter: green gouraud rings (byte-identical to fx_0cd entry 0x4) |
| fx_1f6 | 0x5 | `FX_AERA_WIND_RING` | render | emitter: orbiting white wind-ring particle (byte-identical to fx_0cd entry 0x5) |
| fx_1f6 | 0x6 | `FX_AERA_MODEL` | kind/usage (look pending) | 'so' model (byte-identical to fx_0cd entry 0xD) |
| fx_1f6 | 0x7 | `FX_AERA_GREEN_BURST` | render | emitter: green gust puffs (byte-identical to fx_0cd entry 0x7) |
| fx_1f6 | 0x8 | `FX_AERA_WIND_STREAKS` | kind/usage (look pending) | emitter: spiralling sprites + 3D trail (sprite texture garbled in the render) (byte-identical to fx_0cd entry 0x8) |
| fx_1f6 | 0xA | `FX_AERA_GUST_RINGS` | render | emitter: cyan-green gouraud rings (byte-identical to fx_0cd entry 0xA) |
| fx_1f6 | 0xC | `FX_AERA_SPRITE_BURST` | kind/usage (look pending) | emitter: sprites (texture garbled in the render) (byte-identical to fx_0cd entry 0xC) |
| fx_1f7 | 0x1 | `FX_BLUE_SHOCKWAVE_FLASH` | render | emitter: violet glow flash, white-red-blue and pink gouraud rings |
| fx_1f7 | 0x2 | `FX_BLUE_SHOCKWAVE_LIGHTNING` | render | emitter: violet lightning fans + spark sprites |
| fx_1f7 | 0x3 | `FX_BLUE_SHOCKWAVE_GLOW_TRAIL` | render | trail: violet glow strip |
| fx_1f7 | 0x5 | `FX_BLUE_SHOCKWAVE_GLOW` | render | particles: violet glow, 7 frames |
| fx_1fc | 0x2 | `FX_FENRIR_MORPH_PATH` | usage | table: rotation / position path of the morph models (fx_0d2_morphPath) (byte-identical to fx_0d2 entry 0x9) |
| fx_1fc | 0x6 | `FX_FENRIR_UNIT_PATHS` | usage | table: offset table of per-unit position paths (op83) (byte-identical to fx_0d2 entry 0x8) |
| fx_1fc | 0x9 | `FX_FENRIR_BLUE_RINGS` | render | emitter: white-blue gouraud rings (byte-identical to fx_0d2 entry 0x3) |
| fx_1fc | 0xA | `FX_FENRIR_CAMERA_KEYS_3` | usage | table: camera channel 3 keys (fxm_camChanAdd) |
| fx_1fc | 0xB | `FX_FENRIR_DUST_CLOUD` | render | emitter: tan dust clouds and an orange gouraud ring |
| fx_1fc | 0xC | `FX_FENRIR_CAMERA_KEYS_2` | usage | table: camera channel 2 keys (fxm_camChanAdd) |
| fx_1fc | 0xD | `FX_FENRIR_DUST_DEBRIS` | render | emitter: tan dust clouds and gold rock debris quads |
| fx_1fc | 0xE | `FX_FENRIR_CAMERA_KEYS_1` | usage | table: camera channel 1 keys (fxm_camChanAdd) |
| fx_1fc | 0x13 | `FX_FENRIR_SLICE_PATH_2` | usage | table: rotation / position path (fx_0d2_slicePath2) (byte-identical to fx_0d2 entry 0x28) |
| fx_1fc | 0x17 | `FX_FENRIR_UNIT_OFFSET_PATH` | usage | table: position path (fx_1fc_posPath; as fx_0d2 entry 0x14) |
| fx_1fc | 0x18 | `FX_FENRIR_UNIT_SCALE_PATH` | usage | table: vector path (fx_1fc_vecPath; as fx_0d2 entry 0x15) |
| fx_1fc | 0x1A | `FX_FENRIR_MORPH_CURVE` | usage | table: value path (fx_0d2_morphCurve2) (byte-identical to fx_0d2 entry 0x19) |
| fx_1fc | 0x1B | `FX_FENRIR_MORPH_CURVE_2` | usage | table: alternative value path (fx_0d2_morphCurve2) (byte-identical to fx_0d2 entry 0x1A) |
| fx_1fd | 0x3 | `FX_FENRIR_RIBBON_PATHS_A` | usage | table: position paths of ribbon 0 (byte-identical to fx_0e2 entry 0x9) |
| fx_1fd | 0x4 | `FX_FENRIR_RIBBON_PATHS_B` | usage | table: position paths of ribbon 1 (byte-identical to fx_0e2 entry 0xC) |
| fx_1fd | 0x5 | `FX_FENRIR_EFFECT_MODEL` | kind/usage (look pending) | 'so' model (op84) (byte-identical to fx_0e2 entry 0x11) |
| fx_1fd | 0x6 | `FX_FENRIR_EFFECT_MODEL_2` | kind/usage (look pending) | 'so' model (op84) (byte-identical to fx_0e2 entry 0x1D) |
| fx_1fd | 0x8 | `FX_FENRIR_GREEN_RINGS_2` | render | emitter: white-green / pale green gouraud rings (byte-identical to fx_0e2 entry 0x10) |
| fx_1fd | 0xE | `FX_FENRIR_CAM_TRACK_1` | usage | table: camera key track, channel 1 (fxm_camChanAdd) |
| fx_1fd | 0xF | `FX_FENRIR_CAM_TRACK_2` | usage | table: camera key track, channel 2 (fxm_camChanAdd) |
| fx_1fd | 0x10 | `FX_FENRIR_EMIT_PATHS` | usage | table: five position paths of the op 0x80 trail emitters (fx_1fd_posPath) |
| fx_1fd | 0x11 | `FX_FENRIR_GREEN_RINGS` | render | emitter: pale green gouraud rings, 64 frames (byte-identical to fx_0e2 entry 0x4) |
| fx_1fd | 0x12 | `FX_FENRIR_RIBBON_PATHS_C` | usage | table: position paths of ribbons 2/3 (fxm_offsetTableEntry, fxm_posPathInit) (byte-identical to fx_0e2 entry 0x1) |
| fx_1fd | 0x13 | `FX_FENRIR_GREEN_WIND_TRAIL` | render | emitter: spiralling green wind-streak trail + grey smoke (byte-identical to fx_0e2 entry 0xE) |
| fx_1fd | 0x14 | `FX_FENRIR_SAND_RINGS` | render | emitter: brown-salmon gouraud rings + spark sprites (texture not in the file) (byte-identical to fx_0e2 entry 0xD) |
| fx_1fd | 0x15 | `FX_FENRIR_RIBBON_PATHS_D` | usage | table: position paths of ribbons 2/3 (other branch) (byte-identical to fx_0e2 entry 0xF) |
| fx_1fd | 0x16 | `FX_FENRIR_SPARKS` | kind/usage (look pending) | emitter: spiral of spark sprites (texture not in the file) (byte-identical to fx_0e2 entry 0x12) |
| fx_1fe | 0x2 | `FX_PHOENIX_MODEL_2` | render | 'so' model: flat dome, flame texture (byte-identical to fx_0d3 entry 0xD) |
| fx_1fe | 0xD | `FX_PHOENIX_MODEL_3` | render | 'so' model: dome with a hole, flame texture (byte-identical to fx_0d3 entry 0xC) |
| fx_1fe | 0xF | `FX_PHOENIX_MODEL_4` | render | 'so' model: arc band, smoke texture (byte-identical to fx_0d3 entry 0xE) |
| fx_1fe | 0x10 | `FX_PHOENIX_FIRE_FEATHERS` | render | emitter: orbiting fire-textured trails and flame-feather quads (same call site as fx_0d3 entry 0x10, same kind) |
| fx_1fe | 0x11 | `FX_PHOENIX_PINK_RINGS` | render | emitter: white / pink / copper gouraud rings (byte-identical to fx_0d3 entry 0x11) |
| fx_1fe | 0x12 | `FX_PHOENIX_GOLD_FEATHERS` | render | emitter: golden feather quads (same call site as fx_0d3 entry 0x12, same kind) |
| fx_1fe | 0x15 | `FX_PHOENIX_FLAME_FEATHERS_2` | render | emitter: copper flame-feather quads (cylinder) (same call site as fx_0d3 entry 0x18, same kind) |
| fx_1fe | 0x16 | `FX_PHOENIX_EMBERS` | kind/usage (look pending) | emitter: sprites, render empty (texture not in the file) (same call site as fx_0d3 entry 0xF, same kind) |
| fx_1fe | 0x17 | `FX_PHOENIX_FLAME_FEATHERS` | render | emitter: copper flame-feather quads (same call site as fx_0d3 entry 0x17, same kind) |
| fx_1bf_0 | 0x5 | `FX_ARK_BEAM_LINE` | usage | table: trail data for fxm_drawTrailLine, vertical light line at the strand tips and ring centre |
| fx_1bf_0 | 0xA | `FX_ARK_RED_FLASH_RING` | render | rings: red-white flat flash ring |
| fx_1bf_0 | 0x1C | `FX_ARK_RED_RING` | render | rings: white-pink-red gouraud rings (byte-identical to fx_17d_4 entry 0x3) |
| fx_1bf_0 | 0x1D | `FX_ARK_STRAND_POINTS` | usage | table: offset table of the 18 strand point lists per layer (drawStrands) |
| fx_1bf_1 | 0x1 | `FX_ARK_FAN_EMITTER` | render | emitter: two fans, white / orange light streaks (byte-identical to fx_17d_7 entry 0x2) |
| fx_1bf_1 | 0x3 | `FX_ARK_ROCK_DEBRIS_2` | render | emitter: grey-blue rock chunks (byte-identical to fx_17d_7 entry 0x4) |
| fx_1bf_1 | 0x5 | `FX_ARK_PALE_RING` | render | emitter: white-cream-violet and white-peach-mauve gouraud rings (byte-identical to fx_17d_7 entry 0x6) |
| fx_1bf_1 | 0x6 | `FX_ARK_VIOLET_RING` | render | emitter: white-cream-violet and white-purple-blue gouraud rings (byte-identical to fx_17d_7 entry 0x7) |
| fx_1bf_1 | 0x9 | `FX_ARK_GOLD_GLOW` | render | emitter: golden glow flash (byte-identical to fx_17d_7 entry 0xA) |
| fx_1bf_1 | 0xC | `FX_ARK_PEACH_RING` | render | rings: white-peach-mauve gouraud rings (byte-identical to fx_17d_6 entry 0xB) |
| fx_1bf_1 | 0xE | `FX_ARK_ROCK_DEBRIS` | render | emitter: grey-blue rock chunks (byte-identical to fx_17d_7 entry 0x1) |
| fx_1bf_1 | 0xF | `FX_ARK_ORANGE_RING` | render | emitter: two fans (not rendered) + white-orange gouraud rings (byte-identical to fx_17d_7 entry 0xD) |
| fx_1bf_1 | 0x86 | `FX_ARK_RED_BEAM` | render | summon data: 'so' model, glowing red-orange beam bar |
| fx_1bf_1 | 0x88 | `FX_ARK_GREEN_CRATER` | render | summon data: 'so' model, dark green rippled crater ring |
| fx_1bf_1 | 0x89 | `FX_ARK_FIRE_DISC` | render | summon data: 'so' model, flat ring disc with an orange flame texture |
| fx_1bf_1 | 0x8A | `FX_ARK_SMOKE_BOWL` | render | summon data: 'so' model, bowl with a dark cloud texture |
| fx_1bf_1 | 0x8C | `FX_ARK_GREY_TORUS` | render | summon data: 'so' model, grey gouraud torus |
| fx_1bf_1 | 0x8D | `FX_ARK_ROCK_TORUS` | render | summon data: 'so' model, torus with a brown rock texture |
| fx_1bf_1 | 0x8E | `FX_ARK_GOLD_TORUS` | render | summon data: 'so' model, gold gouraud torus |
| fx_1bf_1 | 0x8F | `FX_ARK_BLUE_SWIRL_BOWL` | render | summon data: 'so' model, bowl with a twisted blue cloud texture |
| fx_1bf_1 | 0x90 | `FX_ARK_ROCK` | render | summon data: 'so' model, rock chunk |
| fx_1bf_1 | 0x91 | `FX_ARK_FIRE_CONE` | render | summon data: 'so' model, open cone with a dark flame texture |
| fx_1bf_1 | 0x92 | `FX_ARK_ROCK_2` | render | summon data: 'so' model, rock chunk |
| fx_1bf_1 | 0x93 | `FX_ARK_ROCK_3` | render | summon data: 'so' model, rock chunk |
| fx_1bf_1 | 0x94 | `FX_ARK_FIRE_ARC` | render | summon data: 'so' model, curved band with an orange flame texture |
| fx_1bf_2 | 0x1 | `FX_ARK_ORANGE_BEAM` | render | 'so' model: glowing red-orange beam bar (byte-identical to fx_17d_8 entry 0x3) |
| fx_1bf_2 | 0x2 | `FX_ARK_FLAT_PANEL` | kind/usage (look pending) | 'so' model: flat band; its texture is a placeholder with the kana labels up / left / right / down (byte-identical to fx_17d_8 entry 0x6) |
| fx_1bf_2 | 0x3 | `FX_ARK_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop (byte-identical to fx_17d_8 entry 0x7) |
| fx_1bf_2 | 0x5 | `FX_ARK_GREY_STAR_DISC` | render | 'so' model: shallow dish with a grey cloud texture (byte-identical to fx_17d_8 entry 0x9) |
| fx_1bf_2 | 0x7 | `FX_ARK_GOLD_SPARKS` | render | emitter: tube of small gold spark sprites (byte-identical to fx_17d_8 entry 0xB) |
| fx_1bf_2 | 0xB | `FX_ARK_STARFIELD` | render | sprite: faint starfield drawn on screen (byte-identical to fx_17d_8 entry 0x10) |
| fx_1bf_2 | 0xD | `FX_ARK_PLANET` | render | 'so' model: hemisphere with an earth-globe texture (byte-identical to fx_17d_8 entry 0x1) |
| fx_1bf_2 | 0xF | `FX_ARK_RED_GLOW_DOME` | render (type 10 split fix) | 'so' model: shallow red-orange gouraud glow dome (byte-identical to fx_17d_8 entry 0x2) |
| fx_1bf_2 | 0x10 | `FX_ARK_DARK_BLUE_RING` | render (type 10 split fix) | 'so' model: flat ring with a dark blue texture (byte-identical to fx_17d_8 entry 0x4) |
| fx_1bf_2 | 0x11 | `FX_ARK_FIRE_RING` | render (type 10 split fix) | rings: white-yellow-red gouraud rings (byte-identical to fx_17d_8 entry 0x5) |
