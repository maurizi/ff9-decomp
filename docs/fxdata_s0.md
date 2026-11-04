# Battle effect data names: slice 0 (fx_001 .. fx_05f)

Effect-data names (`btlfx_getFileData` indices) for the slice-0 modules, following the scheme in
`docs/fxdata.md` (renders from `tools/fxdata.py --render`, one contact sheet per module, plus the C call
order). One-target / all-targets twins and the four Aerial Slash modules carry names over by byte-identical
entries or by the same call site.

44 modules, 209 named sites: 146 by the render, 63 by kind and use (look pending).

## Game-name notes

- fx_001: the label is the enemy attack name "Dagger's magic" (Shell Dragon encounter); prefix
  `FX_DAGGERS_MAGIC_`.
- fx_01f: the enemy skill is "!!!" (Yan), which has no identifier form, so the prefix is the module's
  (`FX_01F_`).
- fx_022: one module for "Right tentacle gone", "Left tentacle gone" and "Both tentacles gone" (Kraken);
  prefix `FX_TENTACLE_GONE_`.
- fx_01e: "1,000 Needles" -> `FX_1000_NEEDLES_`.
- Backdrop tables (`btlfx_drawBackdrop`) are named `FX_<NAME>_BACKDROP` by use; the tool does not draw
  them (look pending).

## Open questions (emulator)

Entries named by kind and use. Most are fan emitters (not drawn by the tool), particles / rings whose texture
the file does not upload (magenta), `'so'` models, and backdrop layer tables.

- fx_00b 0x7 `FX_ESUNA_BACKDROP`: backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool)
- fx_00e 0x3 `FX_BERSERK_BACKDROP`: backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool)
- fx_019 0x1 `FX_MINI_FAN_EMITTER`: emitter: fan element + one faint ring (fan not rendered)
- fx_01a 0x1 `FX_SLEEP_BACKDROP`: backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool)
- fx_01a 0x3 `FX_SLEEP_SPECK_RING`: emitter: particle (texture not in the file) + ring textured with brown specks
- fx_01b 0x1 `FX_FLOAT_SPECK_RING`: emitter: particle (texture not in the file) + ring textured with brown specks
- fx_01b 0x3 `FX_FLOAT_GLOW`: emitter: one particle (texture not in the file)
- fx_01b 0x4 `FX_FLOAT_BACKDROP`: backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool)
- fx_01d 0x1 `FX_AERIAL_SLASH_FAN_EMITTER_2`: emitter: fan + faint green spark quads (fan not rendered)
- fx_01d 0x2 `FX_AERIAL_SLASH_FAN_EMITTER`: emitter: fan + faint green spark quads (fan not rendered)
- fx_01d 0x5 `FX_AERIAL_SLASH_FAN_EMITTER_3`: emitter: fan + small blue spark quads (fan not rendered)
- fx_01f 0x3 `FX_01F_WHITE_RINGS`: emitter: textured rings (texture not in the file) + white gouraud rings
- fx_01f 0xD `FX_01F_BACKDROP`: backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool)
- fx_023 0x8 `FX_PSYCHOKINESIS_SCREEN_TRAILS`: emitter: two screen trails, render all black
- fx_024 0xC `FX_AERIAL_SLASH_FAN_EMITTER_3`: emitter: fan + small blue spark quads (fan not rendered) (same call site as fx_01d entry 0x5)
- fx_024 0xD `FX_AERIAL_SLASH_FAN_EMITTER`: emitter: fan + faint green spark quads (fan not rendered) (same call site as fx_01d entry 0x2)
- fx_024 0xE `FX_AERIAL_SLASH_FAN_EMITTER_2`: emitter: fan + faint green spark quads (fan not rendered) (same call site as fx_01d entry 0x1)
- fx_026 0x85 `FX_SHIVA_SO_MODEL`: 'so' model registered at the end of op80 state 1 (summon data 0x80+, not analysed by the tool)
- fx_026 0x86 `FX_SHIVA_TRACK_EMITTER`: emitter started at each op80 pool item's track point (summon data 0x80+, not analysed by the tool)
- fx_026 0x87 `FX_SHIVA_SO_MODEL_2`: 'so' model registered with 0x85 (summon data 0x80+, not analysed by the tool)
- fx_026 0x9E `FX_SHIVA_IMPACT_EMITTER_2`: emitter at the target, op82 frame 39 (summon data 0x80+, not analysed by the tool)
- fx_026 0x9F `FX_SHIVA_IMPACT_EMITTER`: emitter at the target, op82 frame 39 (summon data 0x80+, not analysed by the tool)
- fx_026 0xA0 `FX_SHIVA_BONE19_RINGS`: rings drawn at summon bone 0x19, growing (summon data 0x80+, not analysed by the tool)
- fx_026 0xA1 `FX_SHIVA_ROOT_EMITTER_3`: emitter at summon bone 0, op80 state 2 start (summon data 0x80+, not analysed by the tool)
- fx_026 0xA2 `FX_SHIVA_ROOT_EMITTER`: emitter at summon bone 0, op80 state 1 frame 38 (summon data 0x80+, not analysed by the tool)
- fx_026 0xA3 `FX_SHIVA_ROOT_EMITTER_2`: emitter at summon bone 0, op80 state 1 frame 42 (summon data 0x80+, not analysed by the tool)
- fx_026 0xA4 `FX_SHIVA_ROOT_EMITTER_4`: emitter at summon bone 0, op80 state 2 end (summon data 0x80+, not analysed by the tool)
- fx_026 0xA5 `FX_SHIVA_BONE19_EMITTER_2`: emitter at summon bone 0x19, op80 state 3 (summon data 0x80+, not analysed by the tool)
- fx_026 0xA6 `FX_SHIVA_BONE19_EMITTER`: emitter at summon bone 0x19, op80 state 2 frame 29 (summon data 0x80+, not analysed by the tool)
- fx_026 0xAB `FX_SHIVA_SPIN_MODEL`: 'so' model drawn spinning and widening at the target (op82) (summon data 0x80+, not analysed by the tool)
- fx_02a 0x4 `FX_SILENCE_BACKDROP`: backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool)
- fx_02b 0x5 `FX_AERIAL_SLASH_FAN_EMITTER_3`: emitter: fan + small blue spark quads (fan not rendered) (byte-identical to fx_01d entry 0x5)
- fx_02e 0x3 `FX_MAGIC_BREAK_FAN_EMITTER`: emitter: one fan element (not rendered)
- fx_030 0x1 `FX_POLLEN_POLLEN`: emitter: pollen sprites (texture not in the file)
- fx_030 0x8 `FX_POLLEN_POLLEN_2`: emitter: pollen sprites (texture not in the file)
- fx_032 0x1 `FX_GLOWING_EYES_SPARKS`: emitter: cone of spark sprites (texture not in the file)
- fx_033 0x5 `FX_SNORT_BREATH_TRAILS`: emitter: two spiralling screen trails (texture mostly not in the file)
- fx_034 0xA `FX_COLD_BREATH_ICE_CRYSTAL_2`: model: the same crystal shape, texture page 0x9A not in the file
- fx_036 0xB `FX_BLOWUP_SPARKS`: emitter: spark sprites (texture not in the file), at the targets
- fx_036 0xC `FX_BLOWUP_SPARKS_2`: emitter: spark sprites (texture not in the file), at the targets
- fx_037 0x2 `FX_DARKNESS_SCREEN_TRAIL`: emitter: orbiting screen trail (texture not in the file)
- fx_037 0x5 `FX_DARKNESS_FAN_EMITTER`: emitter: one fan element (not rendered)
- fx_038 0xC `FX_COMET_BACKDROP`: backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool)
- fx_039 0x7 `FX_OSMOSE_SPARKLES`: emitter: blue glint + spark sprites (texture not in the file)
- fx_039 0x8 `FX_OSMOSE_FAN_EMITTER`: emitter: two fan elements (not rendered)
- fx_03a 0x1 `FX_EARTHQUAKE_GROUND_PIECE_8`: 'so' model: ground piece 8 bounced by the quake (fx_03a_models[8])
- fx_03a 0x2 `FX_EARTHQUAKE_GROUND_PIECE_1`: 'so' model: ground piece 1 bounced by the quake (fx_03a_models[1])
- fx_03a 0x3 `FX_EARTHQUAKE_GROUND_PIECE_2`: 'so' model: ground piece 2 bounced by the quake (fx_03a_models[2])
- fx_03a 0x4 `FX_EARTHQUAKE_GROUND_PIECE_3`: 'so' model: ground piece 3 bounced by the quake (fx_03a_models[3])
- fx_03a 0x5 `FX_EARTHQUAKE_GROUND_PIECE_4`: 'so' model: ground piece 4 bounced by the quake (fx_03a_models[4])
- fx_03a 0x6 `FX_EARTHQUAKE_GROUND_PIECE_5`: 'so' model: ground piece 5 bounced by the quake (fx_03a_models[5])
- fx_03a 0x7 `FX_EARTHQUAKE_GROUND_PIECE_6`: 'so' model: ground piece 6 bounced by the quake (fx_03a_models[6])
- fx_03a 0x8 `FX_EARTHQUAKE_GROUND_PIECE_7`: 'so' model: ground piece 7 bounced by the quake (fx_03a_models[7])
- fx_03a 0x9 `FX_EARTHQUAKE_GROUND_PIECE_0`: 'so' model: ground piece 0 bounced by the quake (fx_03a_models[0])
- fx_03a 0xE `FX_EARTHQUAKE_GROUND_PIECE_9`: 'so' model: fx_03a_models[9], registered but not drawn
- fx_045 0x2 `FX_AERIAL_SLASH_FAN_EMITTER_3`: emitter: fan + small blue spark quads (fan not rendered) (byte-identical to fx_01d entry 0x5)
- fx_045 0x3 `FX_AERIAL_SLASH_FAN_EMITTER`: emitter: fan + faint green spark quads (fan not rendered) (same call site as fx_01d entry 0x2)
- fx_045 0x4 `FX_AERIAL_SLASH_FAN_EMITTER_2`: emitter: fan + faint green spark quads (fan not rendered) (same call site as fx_01d entry 0x1)

## Table

| module | index | name | evidence | what it is |
|---|---|---|---|---|
| fx_001 | 0x1 | `FX_DAGGERS_MAGIC_SPARKLES` | render | emitter: tiny rings + white/green sparkle sprites (at pos54) |
| fx_001 | 0x3 | `FX_DAGGERS_MAGIC_WHITE_RINGS` | render | emitter: white gouraud rings |
| fx_001 | 0x5 | `FX_DAGGERS_MAGIC_GOLD_RINGS` | render | emitter: white-gold gouraud rings |
| fx_001 | 0x6 | `FX_DAGGERS_MAGIC_STAR_FLARES` | render | emitter: yellow-green and violet star flares, faint glow |
| fx_001 | 0x7 | `FX_DAGGERS_MAGIC_BLUE_SWIRL` | render | emitter: blue swirl-textured trail + green/yellow/white sparkle sprites |
| fx_001 | 0xD | `FX_DAGGERS_MAGIC_SPARKLE_SHOWER` | render | emitter: green/yellow/white sparkle sprites (at pos4C) |
| fx_001 | 0xE | `FX_DAGGERS_MAGIC_LIGHT_SPIRES` | render | model: spiky spire shapes, green light-arc texture |
| fx_001 | 0xF | `FX_DAGGERS_MAGIC_LIGHT_RING` | render | model: ring, green light-arc texture |
| fx_001 | 0x10 | `FX_DAGGERS_MAGIC_SPARKLE_BURST` | render | emitter: white/yellow/green sparkle sprites |
| fx_00a | 0x2 | `FX_VENOM_POWDER_POWDER_CLOUD` | render | emitter: purple powder clouds + purple droplet sprites |
| fx_00b | 0x7 | `FX_ESUNA_BACKDROP` | kind/usage (look pending) | backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool) |
| fx_00b | 0x8 | `FX_ESUNA_LIGHT_SHOWER` | render | emitter: rainbow halo + blue ripple, falling golden streaks with white glints, lens flare |
| fx_00c | 0x7 | `FX_PANACEA_GREEN_RINGS` | render | emitter: yellow-green and white-violet gouraud rings |
| fx_00c | 0x8 | `FX_PANACEA_SPARKLES` | render | emitter: green star glint, teal glow orb, golden sparkle sprites |
| fx_00d | 0x2 | `FX_STONA_LIGHT_TRAIL` | render | emitter: flat orbiting trail, yellow-red strip texture |
| fx_00d | 0x3 | `FX_STONA_BLUE_ORBS` | render | emitter: blue light orbs dissolving into golden specks |
| fx_00d | 0x9 | `FX_STONA_GOLD_RINGS` | render | emitter: gold-brown gouraud rings + yellow light streak |
| fx_00e | 0x3 | `FX_BERSERK_BACKDROP` | kind/usage (look pending) | backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool) |
| fx_00e | 0x4 | `FX_BERSERK_CONE` | render | model: open cone (frustum), plain white texture |
| fx_00e | 0xA | `FX_BERSERK_FIRE_RINGS` | render | emitter: dark red-orange-yellow gouraud rings |
| fx_00e | 0xB | `FX_BERSERK_RED_FLASH` | render | emitter: white star flash, red star flash, violet circle |
| fx_00f | 0x6 | `FX_FULL_LIFE_DARK_RINGS` | render | emitter: faint gold-blue gouraud rings |
| fx_00f | 0x7 | `FX_FULL_LIFE_LIGHT_TRAILS` | render | emitter: white light trails with faint sparkles, pink feather strip |
| fx_00f | 0x9 | `FX_FULL_LIFE_WINGS` | render | emitter: white angel-wing sprites |
| fx_00f | 0xA | `FX_FULL_LIFE_HALO_RINGS` | render | emitter: white-lilac gouraud rings, leaf-shaped halo outline |
| fx_011 | 0x7 | `FX_LIFE_FEATHER_TRAILS` | render | emitter: two orbiting trails with a feather texture |
| fx_011 | 0x8 | `FX_LIFE_HALO_RINGS` | render | emitter: white-lilac gouraud rings, leaf-shaped halo outline |
| fx_011 | 0x9 | `FX_LIFE_WINGS` | render | emitter: white angel-wing sprites |
| fx_015 | 0x1 | `FX_POISON_PURPLE_SMOKE` | render | emitter: orbiting trail with purple smoke clouds |
| fx_015 | 0x8 | `FX_POISON_TOXIC_BURST` | render | emitter: purple clouds + purple droplet sprites |
| fx_016 | 0x1 | `FX_WATER_SPLASH_DROPS` | render | emitter: water droplet clusters, blue glow, small bubbles |
| fx_016 | 0x2 | `FX_WATER_GLOBE_1` | render | model: flared water bowl, second morph stage |
| fx_016 | 0x3 | `FX_WATER_GLOBE_2` | render | model: water vase shape, third morph stage |
| fx_016 | 0x4 | `FX_WATER_GLOBE_3` | render | model: bulging water bowl, fourth morph stage |
| fx_016 | 0x5 | `FX_WATER_GLOBE_0` | render | model: flat water dish, first morph stage of the growing water globe |
| fx_016 | 0x7 | `FX_WATER_SPLASH_FUNNEL` | render | model: flared funnel, misty water texture |
| fx_016 | 0x8 | `FX_WATER_SPLASH_COLUMN` | render | model: open cylinder, misty water texture; morphs to the funnel as the globe bursts |
| fx_016 | 0x9 | `FX_WATER_BASE_DISC` | render | model: flat disc (water texture) drawn under the globe |
| fx_016 | 0xA | `FX_WATER_BLUE_RING` | render | emitter: white-blue gouraud ring |
| fx_016 | 0xE | `FX_WATER_GLOBE` | render | model: full water sphere, last morph stage, then fades |
| fx_017 | 0x1 | `FX_DEVILS_CANDLE_BLUE_STARBURST` | render | emitter: grey flower flash, blue starburst, blue-violet streak sprites |
| fx_017 | 0x2 | `FX_DEVILS_CANDLE_FLAME` | render | emitter: smoky yellow-grey flame quads |
| fx_017 | 0x7 | `FX_DEVILS_CANDLE_FLAME_2` | render | emitter: smoky yellow-grey flame quads |
| fx_019 | 0x1 | `FX_MINI_FAN_EMITTER` | kind/usage (look pending) | emitter: fan element + one faint ring (fan not rendered) |
| fx_01a | 0x1 | `FX_SLEEP_BACKDROP` | kind/usage (look pending) | backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool) |
| fx_01a | 0x2 | `FX_SLEEP_LIGHT_SPIRAL` | render | emitter: helix of white screen trails, white-cyan rings, green glint |
| fx_01a | 0x3 | `FX_SLEEP_SPECK_RING` | kind/usage (look pending) | emitter: particle (texture not in the file) + ring textured with brown specks |
| fx_01a | 0x4 | `FX_SLEEP_LIGHT_SPIRAL_2` | render | emitter: helix of two white screen trails with green / violet glows |
| fx_01b | 0x1 | `FX_FLOAT_SPECK_RING` | kind/usage (look pending) | emitter: particle (texture not in the file) + ring textured with brown specks |
| fx_01b | 0x2 | `FX_FLOAT_LIFT_RINGS` | render | emitter: speckled ring, white-lilac gouraud rings, two fans |
| fx_01b | 0x3 | `FX_FLOAT_GLOW` | kind/usage (look pending) | emitter: one particle (texture not in the file) |
| fx_01b | 0x4 | `FX_FLOAT_BACKDROP` | kind/usage (look pending) | backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool) |
| fx_01c | 0x4 | `FX_AQUA_BREATH_WATER_DROPS` | render | emitter: water droplet clusters |
| fx_01c | 0x5 | `FX_AQUA_BREATH_WAVE_RING` | render | emitter: ring with a blue wave texture + blue glow |
| fx_01c | 0x6 | `FX_AQUA_BREATH_BUBBLES` | render | emitter: water-droplet trail + bubble quads |
| fx_01c | 0x7 | `FX_AQUA_BREATH_BLUE_RING` | render | emitter: light blue / white gouraud ring |
| fx_01c | 0x8 | `FX_AQUA_BREATH_BUBBLE` | render | emitter: one large bubble + small glints |
| fx_01d | 0x1 | `FX_AERIAL_SLASH_FAN_EMITTER_2` | kind/usage (look pending) | emitter: fan + faint green spark quads (fan not rendered) |
| fx_01d | 0x2 | `FX_AERIAL_SLASH_FAN_EMITTER` | kind/usage (look pending) | emitter: fan + faint green spark quads (fan not rendered) |
| fx_01d | 0x3 | `FX_AERIAL_SLASH_DUST_CLOUD` | render | emitter: lilac dust clouds + green glint |
| fx_01d | 0x5 | `FX_AERIAL_SLASH_FAN_EMITTER_3` | kind/usage (look pending) | emitter: fan + small blue spark quads (fan not rendered) |
| fx_01d | 0xB | `FX_AERIAL_SLASH_WIND_RINGS` | render | emitter: ring with a violet sparkle-curtain texture + pink-white gouraud rings |
| fx_01d | 0xC | `FX_AERIAL_SLASH_DUST_RING` | render | emitter: ring with a grey dust texture |
| fx_01e | 0x4 | `FX_1000_NEEDLES_NEEDLE_BURST` | render | emitter: orange specks, pink starbursts, white shard sprites, sparks (texture not in the file) |
| fx_01e | 0x8 | `FX_1000_NEEDLES_NEEDLE_TRAILS` | render | emitter: flat trails with a white needle-streak strip |
| fx_01f | 0x1 | `FX_01F_PINK_VORTEX` | render | emitter: white-violet-maroon gouraud rings + pink swirling orb |
| fx_01f | 0x3 | `FX_01F_WHITE_RINGS` | kind/usage (look pending) | emitter: textured rings (texture not in the file) + white gouraud rings |
| fx_01f | 0x4 | `FX_01F_RED_GLOW` | render | emitter: dark red glow with a white spot |
| fx_01f | 0xB | `FX_01F_PINK_RINGS` | render | emitter: white-lilac-maroon and white-cream-pink gouraud rings |
| fx_01f | 0xC | `FX_01F_RED_GLOW_2` | render | emitter: dark red glow with a white spot (orbiting) |
| fx_01f | 0xD | `FX_01F_BACKDROP` | kind/usage (look pending) | backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool) |
| fx_020 | 0x2 | `FX_REGEN_GLINTS` | render | emitter: fan + blue glint, teal-white ring, blue / green star glints |
| fx_020 | 0x8 | `FX_REGEN_LIGHT_TRAIL` | render | emitter: white-blue light trail + faint sparkle sprites |
| fx_021 | 0x1 | `FX_OPEN_BOOK_DUST_PUFF` | render | emitter: white-grey dust puffs |
| fx_022 | 0x1 | `FX_TENTACLE_GONE_BUBBLES` | render | emitter: iridescent soap bubbles |
| fx_023 | 0x1 | `FX_PSYCHOKINESIS_WHITE_RINGS` | render | emitter: white gouraud rings |
| fx_023 | 0x2 | `FX_PSYCHOKINESIS_GLYPH_ORBS` | render | emitter: pink orbs, each with a kana glyph, homing |
| fx_023 | 0x3 | `FX_PSYCHOKINESIS_WHITE_RINGS_2` | render | emitter: white gouraud rings |
| fx_023 | 0x7 | `FX_PSYCHOKINESIS_GREY_RINGS` | render | emitter: white rings fading to grey |
| fx_023 | 0x8 | `FX_PSYCHOKINESIS_SCREEN_TRAILS` | kind/usage (look pending) | emitter: two screen trails, render all black |
| fx_024 | 0x7 | `FX_AERIAL_SLASH_DUST_CLOUD` | render | emitter: lilac dust clouds + green glint (byte-identical to fx_01d entry 0x3) |
| fx_024 | 0xB | `FX_AERIAL_SLASH_STAR_FLASH` | render | emitter: white star flash with a blue halo |
| fx_024 | 0xC | `FX_AERIAL_SLASH_FAN_EMITTER_3` | kind/usage (look pending) | emitter: fan + small blue spark quads (fan not rendered) (same call site as fx_01d entry 0x5) |
| fx_024 | 0xD | `FX_AERIAL_SLASH_FAN_EMITTER` | kind/usage (look pending) | emitter: fan + faint green spark quads (fan not rendered) (same call site as fx_01d entry 0x2) |
| fx_024 | 0xE | `FX_AERIAL_SLASH_FAN_EMITTER_2` | kind/usage (look pending) | emitter: fan + faint green spark quads (fan not rendered) (same call site as fx_01d entry 0x1) |
| fx_024 | 0xF | `FX_AERIAL_SLASH_WIND_RINGS` | render | emitter: ring with a violet sparkle-curtain texture + pink-white gouraud rings (same look as fx_01d entry 0xb) |
| fx_024 | 0x10 | `FX_AERIAL_SLASH_DUST_RING` | render | emitter: ring with a grey dust texture (same look as fx_01d entry 0xc) |
| fx_026 | 0x2 | `FX_SHIVA_STREAK_PATH` | kind/usage | track: path of the ice streak strips (op82 pool items) |
| fx_026 | 0x4 | `FX_SHIVA_ICE_CRESCENTS` | render | emitter: blue ice crescent sprites + small shard sprites |
| fx_026 | 0x5 | `FX_SHIVA_ICE_SHARD_BURST` | render | emitter: four ice-crescent sprite elements + cyan-white gouraud rings |
| fx_026 | 0x6 | `FX_SHIVA_ICE_CHUNKS` | render | emitter: falling ice chunk sprites, faint sparkles |
| fx_026 | 0x7 | `FX_SHIVA_STAR_GLINTS` | render | emitter: blue glows, white / green star glints, blue glow domes |
| fx_026 | 0xB | `FX_SHIVA_PATH` | kind/usage | track: keyframed position (camera point 2) in op80 state 1 |
| fx_026 | 0xE | `FX_SHIVA_PATH_2` | kind/usage | track: keyframed position (camera point 2) in op80 state 2 |
| fx_026 | 0x11 | `FX_SHIVA_PATH_3` | kind/usage | track: keyframed position in op80 state 3 |
| fx_026 | 0x17 | `FX_SHIVA_ICE_SPRAY` | render | emitter: white ice chunk sprites |
| fx_026 | 0x19 | `FX_SHIVA_ICE_CHUNKS_2` | render | emitter: falling ice chunk sprites, faint sparkles |
| fx_026 | 0x1A | `FX_SHIVA_SHATTER_RINGS` | render | emitter: blue gouraud rings + ice chunk sprites, sparks (pool item per piece) |
| fx_026 | 0x1F | `FX_SHIVA_BLUE_TRAIL` | render | emitter: spiralling trail, blue strip texture |
| fx_026 | 0x20 | `FX_SHIVA_ICE_DEBRIS` | render | emitter: small ice chunk sprites |
| fx_026 | 0x21 | `FX_SHIVA_ICE_DEBRIS_2` | render | emitter: small ice chunk sprites |
| fx_026 | 0x22 | `FX_SHIVA_CYAN_RING` | render | emitter: cyan-white gouraud ring |
| fx_026 | 0x34 | `FX_SHIVA_ICE_STREAK_3` | render | trail: blue ice strip (third streak kind) |
| fx_026 | 0x35 | `FX_SHIVA_ICE_STREAK_2` | render | trail: blue ice strip (second streak kind) |
| fx_026 | 0x36 | `FX_SHIVA_ICE_STREAK` | render | trail: blue ice strip |
| fx_026 | 0x85 | `FX_SHIVA_SO_MODEL` | kind/usage (look pending) | 'so' model registered at the end of op80 state 1 (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0x86 | `FX_SHIVA_TRACK_EMITTER` | kind/usage (look pending) | emitter started at each op80 pool item's track point (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0x87 | `FX_SHIVA_SO_MODEL_2` | kind/usage (look pending) | 'so' model registered with 0x85 (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0x9D | `FX_SHIVA_STREAK_PATH_2` | kind/usage | track: path of the ice streak strips (op82 pool items) (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0x9E | `FX_SHIVA_IMPACT_EMITTER_2` | kind/usage (look pending) | emitter at the target, op82 frame 39 (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0x9F | `FX_SHIVA_IMPACT_EMITTER` | kind/usage (look pending) | emitter at the target, op82 frame 39 (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0xA0 | `FX_SHIVA_BONE19_RINGS` | kind/usage (look pending) | rings drawn at summon bone 0x19, growing (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0xA1 | `FX_SHIVA_ROOT_EMITTER_3` | kind/usage (look pending) | emitter at summon bone 0, op80 state 2 start (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0xA2 | `FX_SHIVA_ROOT_EMITTER` | kind/usage (look pending) | emitter at summon bone 0, op80 state 1 frame 38 (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0xA3 | `FX_SHIVA_ROOT_EMITTER_2` | kind/usage (look pending) | emitter at summon bone 0, op80 state 1 frame 42 (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0xA4 | `FX_SHIVA_ROOT_EMITTER_4` | kind/usage (look pending) | emitter at summon bone 0, op80 state 2 end (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0xA5 | `FX_SHIVA_BONE19_EMITTER_2` | kind/usage (look pending) | emitter at summon bone 0x19, op80 state 3 (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0xA6 | `FX_SHIVA_BONE19_EMITTER` | kind/usage (look pending) | emitter at summon bone 0x19, op80 state 2 frame 29 (summon data 0x80+, not analysed by the tool) |
| fx_026 | 0xAB | `FX_SHIVA_SPIN_MODEL` | kind/usage (look pending) | 'so' model drawn spinning and widening at the target (op82) (summon data 0x80+, not analysed by the tool) |
| fx_028 | 0x2 | `FX_ELIXIR_SPARKLES` | render | emitter: white / gold sparkle shards |
| fx_028 | 0x3 | `FX_ELIXIR_GREEN_GLOW` | render | emitter: violet / green glows, green star glints, green light pillar, pink glints |
| fx_028 | 0x4 | `FX_ELIXIR_BUBBLES` | render | emitter: green / pink star sparks + green, white, yellow, pink bubble sprites |
| fx_028 | 0x5 | `FX_ELIXIR_GREEN_BLOSSOM` | render | emitter: green flower-shaped glow + green specks |
| fx_028 | 0x6 | `FX_ELIXIR_PINK_BLOSSOM` | render | emitter: pink flower-shaped glow + bubble specks |
| fx_028 | 0x7 | `FX_ELIXIR_SPARKLE_RINGS` | render | emitter: rings of red / green dots + green-pink crosses |
| fx_029 | 0x7 | `FX_WHITE_WIND_GLOW` | render | emitter: blue specks + blue-white star glow |
| fx_02a | 0x1 | `FX_SILENCE_GLYPH` | render | emitter: kana glyph sprite + dotted mouth shape (one element's texture not in the file) |
| fx_02a | 0x3 | `FX_SILENCE_GLYPH_GLOW` | render | emitter: blurred kana glyph + specks |
| fx_02a | 0x4 | `FX_SILENCE_BACKDROP` | kind/usage (look pending) | backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool) |
| fx_02a | 0xA | `FX_SILENCE_RUNE_SEAL` | render | emitter: grey stone ring carved with runes |
| fx_02b | 0x3 | `FX_AERIAL_SLASH_DUST_CLOUD` | render | emitter: lilac dust clouds + green glint (byte-identical to fx_01d entry 0x3) |
| fx_02b | 0x5 | `FX_AERIAL_SLASH_FAN_EMITTER_3` | kind/usage (look pending) | emitter: fan + small blue spark quads (fan not rendered) (byte-identical to fx_01d entry 0x5) |
| fx_02b | 0xB | `FX_AERIAL_SLASH_WIND_RINGS` | render | emitter: ring with a violet sparkle-curtain texture + pink-white gouraud rings (byte-identical to fx_01d entry 0xb) |
| fx_02b | 0xC | `FX_AERIAL_SLASH_DUST_RING` | render | emitter: ring with a grey dust texture (byte-identical to fx_01d entry 0xc) |
| fx_02c | 0x7 | `FX_BAD_BREATH_GAS_CLOUD` | render | emitter: yellow-grey and purple gas clouds + green / pink glows |
| fx_02c | 0x8 | `FX_BAD_BREATH_GAS_BURST` | render | emitter: yellow-grey and purple gas clouds, glows, green gas burst |
| fx_02d | 0x2 | `FX_ANGELS_SNACK_OLIVE_RINGS` | render | emitter: dark olive and white-yellow-grey-violet gouraud rings |
| fx_02d | 0x7 | `FX_ANGELS_SNACK_GOLD_FLARES` | render | emitter: golden lens flares, pink star glints, sparkles |
| fx_02e | 0x1 | `FX_MAGIC_BREAK_SPARKLES` | render | emitter: blue-violet specks + fan |
| fx_02e | 0x2 | `FX_MAGIC_BREAK_RED_ORB` | render | emitter: red sphere with a white flame inside |
| fx_02e | 0x3 | `FX_MAGIC_BREAK_FAN_EMITTER` | kind/usage (look pending) | emitter: one fan element (not rendered) |
| fx_02e | 0x4 | `FX_MAGIC_BREAK_RUNE_RING` | render | emitter: violet circuit-pattern ring + red sphere |
| fx_02f | 0x5 | `FX_FLAME_FIREBALL` | render | emitter: expanding red-orange fireballs + flame tongues (one per target) |
| fx_02f | 0x6 | `FX_FLAME_FLAME_STREAK` | render | emitter: screen trail with a flame-streak strip |
| fx_030 | 0x1 | `FX_POLLEN_POLLEN` | kind/usage (look pending) | emitter: pollen sprites (texture not in the file) |
| fx_030 | 0x2 | `FX_POLLEN_DUST_PUFF` | render | emitter: grey dust puff + dim specks |
| fx_030 | 0x3 | `FX_POLLEN_POLLEN_PUFF` | render | emitter: yellow-brown pollen puff + grey puff |
| fx_030 | 0x8 | `FX_POLLEN_POLLEN_2` | kind/usage (look pending) | emitter: pollen sprites (texture not in the file) |
| fx_031 | 0x1 | `FX_GRADUAL_PETRIFY_GOLD_RINGS` | render | emitter: white-cyan and white-gold gouraud rings |
| fx_031 | 0x2 | `FX_GRADUAL_PETRIFY_GOLD_RINGS_2` | render | emitter: white-gold gouraud rings |
| fx_031 | 0x3 | `FX_GRADUAL_PETRIFY_STONE_DUST` | render | emitter: grey dust clouds + grey streaks |
| fx_031 | 0x9 | `FX_GRADUAL_PETRIFY_STONE_DUST_2` | render | emitter: grey dust clouds |
| fx_032 | 0x1 | `FX_GLOWING_EYES_SPARKS` | kind/usage (look pending) | emitter: cone of spark sprites (texture not in the file) |
| fx_032 | 0x2 | `FX_GLOWING_EYES_GLOW_RINGS` | render | emitter: olive-yellow and white-cyan gouraud rings |
| fx_032 | 0x3 | `FX_GLOWING_EYES_GLOW_RINGS_2` | render | emitter: olive-yellow and white-cyan gouraud rings |
| fx_032 | 0x4 | `FX_GLOWING_EYES_SMOKE` | render | emitter: grey smoke wisps |
| fx_032 | 0x5 | `FX_GLOWING_EYES_SWIRL` | render | emitter: pale green swirling vortex |
| fx_033 | 0x5 | `FX_SNORT_BREATH_TRAILS` | kind/usage (look pending) | emitter: two spiralling screen trails (texture mostly not in the file) |
| fx_034 | 0x1 | `FX_COLD_BREATH_ICE_CRYSTAL` | render | model: ice crystal shape with a cracked-ice texture |
| fx_034 | 0x2 | `FX_COLD_BREATH_FROST_BREATH` | render | emitter: violet-white frost mist streams |
| fx_034 | 0x3 | `FX_COLD_BREATH_ICE_CHUNKS` | render | emitter: grey ice lumps + teal glows |
| fx_034 | 0xA | `FX_COLD_BREATH_ICE_CRYSTAL_2` | kind/usage (look pending) | model: the same crystal shape, texture page 0x9A not in the file |
| fx_035 | 0x3 | `FX_DEVILS_CANDLE_FLAME` | render | emitter: teal / tan flame quads |
| fx_035 | 0x8 | `FX_DEVILS_CANDLE_FLAME_2` | render | emitter: teal / tan flame quads |
| fx_035 | 0x9 | `FX_DEVILS_CANDLE_FIRE` | render | emitter: red-orange-yellow flame tongues + red glow |
| fx_036 | 0x1 | `FX_BLOWUP_EXPLOSION` | render | emitter: fire explosion + red gouraud rings |
| fx_036 | 0x2 | `FX_BLOWUP_EXPLOSION_SMOKE` | render | emitter: fire explosion + grey smoke puffs |
| fx_036 | 0x3 | `FX_BLOWUP_BIG_EXPLOSION` | render | emitter: three fire explosions |
| fx_036 | 0x4 | `FX_BLOWUP_RED_RING` | render | emitter: fan + white-red gouraud ring |
| fx_036 | 0xB | `FX_BLOWUP_SPARKS` | kind/usage (look pending) | emitter: spark sprites (texture not in the file), at the targets |
| fx_036 | 0xC | `FX_BLOWUP_SPARKS_2` | kind/usage (look pending) | emitter: spark sprites (texture not in the file), at the targets |
| fx_036 | 0xD | `FX_BLOWUP_SMOKE` | render | emitter: grey smoke puffs |
| fx_037 | 0x1 | `FX_DARKNESS_DARK_CRESCENTS` | render | emitter: pale crescent quads tinted violet / green + coloured specks |
| fx_037 | 0x2 | `FX_DARKNESS_SCREEN_TRAIL` | kind/usage (look pending) | emitter: orbiting screen trail (texture not in the file) |
| fx_037 | 0x3 | `FX_DARKNESS_WHITE_GLOW` | render | emitter: white radial glow + one particle (texture not in the file) |
| fx_037 | 0x4 | `FX_DARKNESS_DARK_ORBS` | render | emitter: blue orbs turning violet then red |
| fx_037 | 0x5 | `FX_DARKNESS_FAN_EMITTER` | kind/usage (look pending) | emitter: one fan element (not rendered) |
| fx_038 | 0x1 | `FX_COMET_BLUE_GLOW` | render | particles: blue glow with a white core, drawn on each falling rock |
| fx_038 | 0x2 | `FX_COMET_ROCK` | render | model: rock (grey stone texture) |
| fx_038 | 0x3 | `FX_COMET_ROCK_DEBRIS` | render | emitter: orange / white striped debris sprites |
| fx_038 | 0x4 | `FX_COMET_TAIL` | render | trail: violet-white strip behind each rock |
| fx_038 | 0x5 | `FX_COMET_FIRE_GLOW` | render | particles: orange-red fiery glow |
| fx_038 | 0x9 | `FX_COMET_IMPACT_SMOKE` | render | emitter: white-pink smoke burst at the impact |
| fx_038 | 0xA | `FX_COMET_GLOW_DISH` | render | gouraud model: wide shallow dish |
| fx_038 | 0xB | `FX_COMET_SHOCK_RING` | render | model: flat ring, white texture |
| fx_038 | 0xC | `FX_COMET_BACKDROP` | kind/usage (look pending) | backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool) |
| fx_038 | 0xD | `FX_COMET_BLUE_RING` | render | rings: dark blue / blue gouraud rings at the impact |
| fx_039 | 0x7 | `FX_OSMOSE_SPARKLES` | kind/usage (look pending) | emitter: blue glint + spark sprites (texture not in the file) |
| fx_039 | 0x8 | `FX_OSMOSE_FAN_EMITTER` | kind/usage (look pending) | emitter: two fan elements (not rendered) |
| fx_039 | 0x9 | `FX_OSMOSE_DRAIN_ORB` | render | emitter: white-violet-cyan gouraud rings + screen trail, flies between caster and target |
| fx_039 | 0xA | `FX_OSMOSE_ABSORB_FLASH` | render | emitter: white-lilac / white-violet-cyan rings + purple light streaks |
| fx_03a | 0x1 | `FX_EARTHQUAKE_GROUND_PIECE_8` | kind/usage (look pending) | 'so' model: ground piece 8 bounced by the quake (fx_03a_models[8]) |
| fx_03a | 0x2 | `FX_EARTHQUAKE_GROUND_PIECE_1` | kind/usage (look pending) | 'so' model: ground piece 1 bounced by the quake (fx_03a_models[1]) |
| fx_03a | 0x3 | `FX_EARTHQUAKE_GROUND_PIECE_2` | kind/usage (look pending) | 'so' model: ground piece 2 bounced by the quake (fx_03a_models[2]) |
| fx_03a | 0x4 | `FX_EARTHQUAKE_GROUND_PIECE_3` | kind/usage (look pending) | 'so' model: ground piece 3 bounced by the quake (fx_03a_models[3]) |
| fx_03a | 0x5 | `FX_EARTHQUAKE_GROUND_PIECE_4` | kind/usage (look pending) | 'so' model: ground piece 4 bounced by the quake (fx_03a_models[4]) |
| fx_03a | 0x6 | `FX_EARTHQUAKE_GROUND_PIECE_5` | kind/usage (look pending) | 'so' model: ground piece 5 bounced by the quake (fx_03a_models[5]) |
| fx_03a | 0x7 | `FX_EARTHQUAKE_GROUND_PIECE_6` | kind/usage (look pending) | 'so' model: ground piece 6 bounced by the quake (fx_03a_models[6]) |
| fx_03a | 0x8 | `FX_EARTHQUAKE_GROUND_PIECE_7` | kind/usage (look pending) | 'so' model: ground piece 7 bounced by the quake (fx_03a_models[7]) |
| fx_03a | 0x9 | `FX_EARTHQUAKE_GROUND_PIECE_0` | kind/usage (look pending) | 'so' model: ground piece 0 bounced by the quake (fx_03a_models[0]) |
| fx_03a | 0xE | `FX_EARTHQUAKE_GROUND_PIECE_9` | kind/usage (look pending) | 'so' model: fx_03a_models[9], registered but not drawn |
| fx_03a | 0x10 | `FX_EARTHQUAKE_RUBBLE` | render | emitter: yellow striped rubble quads |
| fx_045 | 0x1 | `FX_AERIAL_SLASH_STAR_FLASH` | render | emitter: white star flash with a blue halo (same call site as fx_024 entry 0xb) |
| fx_045 | 0x2 | `FX_AERIAL_SLASH_FAN_EMITTER_3` | kind/usage (look pending) | emitter: fan + small blue spark quads (fan not rendered) (byte-identical to fx_01d entry 0x5) |
| fx_045 | 0x3 | `FX_AERIAL_SLASH_FAN_EMITTER` | kind/usage (look pending) | emitter: fan + faint green spark quads (fan not rendered) (same call site as fx_01d entry 0x2) |
| fx_045 | 0x4 | `FX_AERIAL_SLASH_FAN_EMITTER_2` | kind/usage (look pending) | emitter: fan + faint green spark quads (fan not rendered) (same call site as fx_01d entry 0x1) |
| fx_045 | 0x5 | `FX_AERIAL_SLASH_WIND_RINGS` | render | emitter: ring with a violet sparkle-curtain texture + pink-white gouraud rings (byte-identical to fx_01d entry 0xb) |
| fx_045 | 0x6 | `FX_AERIAL_SLASH_DUST_RING` | render | emitter: ring with a grey dust texture (byte-identical to fx_01d entry 0xc) |
| fx_045 | 0x7 | `FX_AERIAL_SLASH_DUST_CLOUD` | render | emitter: lilac dust clouds + green glint (byte-identical to fx_01d entry 0x3) |
