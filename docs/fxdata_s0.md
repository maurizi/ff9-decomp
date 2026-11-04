# Battle effect data names: slice 0 (fx_001 .. fx_05f)

Effect-data names (`btlfx_getFileData` indices) for the slice-0 modules, following the scheme in
`docs/fxdata.md` (renders from `tools/fxdata.py --render`, one contact sheet per module, plus the C call
order). One-target / all-targets twins and the four Aerial Slash modules carry names over by byte-identical
entries or by the same call site.

25 modules, 98 named sites: 77 by the render, 21 by kind and use (look pending).

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
- fx_02b 0x5 `FX_AERIAL_SLASH_FAN_EMITTER_3`: emitter: fan + small blue spark quads (fan not rendered) (byte-identical to fx_01d entry 0x5)
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
| fx_02b | 0x3 | `FX_AERIAL_SLASH_DUST_CLOUD` | render | emitter: lilac dust clouds + green glint (byte-identical to fx_01d entry 0x3) |
| fx_02b | 0x5 | `FX_AERIAL_SLASH_FAN_EMITTER_3` | kind/usage (look pending) | emitter: fan + small blue spark quads (fan not rendered) (byte-identical to fx_01d entry 0x5) |
| fx_02b | 0xB | `FX_AERIAL_SLASH_WIND_RINGS` | render | emitter: ring with a violet sparkle-curtain texture + pink-white gouraud rings (byte-identical to fx_01d entry 0xb) |
| fx_02b | 0xC | `FX_AERIAL_SLASH_DUST_RING` | render | emitter: ring with a grey dust texture (byte-identical to fx_01d entry 0xc) |
| fx_045 | 0x1 | `FX_AERIAL_SLASH_STAR_FLASH` | render | emitter: white star flash with a blue halo (same call site as fx_024 entry 0xb) |
| fx_045 | 0x2 | `FX_AERIAL_SLASH_FAN_EMITTER_3` | kind/usage (look pending) | emitter: fan + small blue spark quads (fan not rendered) (byte-identical to fx_01d entry 0x5) |
| fx_045 | 0x3 | `FX_AERIAL_SLASH_FAN_EMITTER` | kind/usage (look pending) | emitter: fan + faint green spark quads (fan not rendered) (same call site as fx_01d entry 0x2) |
| fx_045 | 0x4 | `FX_AERIAL_SLASH_FAN_EMITTER_2` | kind/usage (look pending) | emitter: fan + faint green spark quads (fan not rendered) (same call site as fx_01d entry 0x1) |
| fx_045 | 0x5 | `FX_AERIAL_SLASH_WIND_RINGS` | render | emitter: ring with a violet sparkle-curtain texture + pink-white gouraud rings (byte-identical to fx_01d entry 0xb) |
| fx_045 | 0x6 | `FX_AERIAL_SLASH_DUST_RING` | render | emitter: ring with a grey dust texture (byte-identical to fx_01d entry 0xc) |
| fx_045 | 0x7 | `FX_AERIAL_SLASH_DUST_CLOUD` | render | emitter: lilac dust clouds + green glint (byte-identical to fx_01d entry 0x3) |
