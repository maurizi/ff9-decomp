# Battle effect data names, slice 1 (fx_060 .. fx_0b0)

Second batch after the pilot in `docs/fxdata.md` (same scheme, same tool): 58 modules, every
`btlfx_getFileData` literal in them named (321 module/index defines). Evidence column: `render` = named by
the `tools/fxdata.py --render` contact sheet, `kind/usage` = a data table named by the export it is passed to
(backdrop layers, motion tracks, strand line, model data), `kind/usage (look pending)` = appearance not
settled by the render (open questions below).

Notes:
- Enemy-only modules use the enemy skill's name (`FX_GNOLL_ATTACK_`, `FX_LAVA_GUN_`, `FX_WATER_GUN_` for
  "Water-gun", `FX_REIS_WIND_` for "Reis's Wind", `FX_LV4_HOLY_` / `FX_LV5_DEATH_`).
- One-target / all-targets twins carry names over through byte-identical entries (fx_062/063, fx_079/07a,
  fx_086/08f, fx_08d/0a2) or the paired call order where the entry kinds agree (fx_087/088, fx_09e/09d).
  fx_061's entries have fx_060's kinds and sizes but differ in bytes; they are named by content.
  fx_090 runs fx_08e's C with its entries in another order: call-order pairing gives wrong kinds there,
  so it is named by kind + render (same lesson as the pilot's fx_03e).
- Several textures read a placeholder page in the tool's VRAM (fx_07f entry 1, fx_086/08f entry 4: Japanese
  developer scribbles on a white page; fx_060's trail strips show staff-like marks). The real UVs probably
  sit on a white or other part of the page; those names say the shape only.

## Open questions (emulator)

Revisited with fxdata d6121033 (fans, `'so'` models, dots, summon / common pages): 24 of the 52 look-pending
entries are now named by the render (evidence `render (fxdata d6121033)`: the fan streaks / rays / cones,
the hammer, ice crystal, reaper and lava-ball models, spark dots). Still pending, one screenshot at the
frame the C starts the entry would settle each:

- Callback-only emitters (nothing drawable in the data): `FX_FLARE_CALLBACK_EMITTER`, `FX_WEB_CALLBACK_EMITTER`.
- Still black: `FX_BIO_ORBIT_TRAIL`, `FX_SLOW_PARTICLES`, `FX_DRAIN_SPRITES`, `FX_METEOR_SPRITES`,
  `FX_METEOR_CONE_SPRITES`, `FX_METEOR_DEBRIS_SPRITES`, `FX_ELECTROCUTE_BOLT_TRAIL(_2)` (sprites),
  `FX_BUZZ_SPRITE`, `FX_SPORE_TRAIL`, `FX_WEB_STRAND_TRAIL`, `FX_LEAF_SWIRL_LEAF_TRAIL(_2,_3,_4)` (dark strip
  with a leaf tip), `FX_LIGHTNING_TRAIL_2` (not rendered).
- Sparkles still drawn as flat squares (sprite texture not resolved): `FX_CURAGA_SPARKLE_BURST`,
  `FX_CURAGA_SPARKLES`, `FX_CURAGA_SPARKLE_SPIRAL`, `FX_CURA_SPARKLE_BURST`.
- Texture outside the file and the common / summon pages (magenta): `FX_DEMI_CAP`, `FX_DEMI_SHOCK_SPHERE`,
  `FX_DEMI_SHOCK_TRAIL`, `FX_SLOW_GLINT` (partly).
- Reads unrelated VRAM, probably filled at run time: `FX_FLARE_TEX_RING(_2)`, `FX_LV4_HOLY_TEX_RING`,
  `FX_LV4_HOLY_TRAIL`, `FX_LV4_HOLY_SPRITE_LINE`, `FX_LV4_HOLY_PARTICLE`, `FX_LV4_HOLY_SPECK_EMITTER`,
  `FX_FROST_ICE_GLINTS`, `FX_BLASTER_TRAIL(_2)`, `FX_CURA_FAN_EMITTER` (cyan stripes: light rays or junk).
- Placeholder-page UVs (shape known, look not): `FX_SILENCE_RING_DISC`, `FX_METEOR_STREAK_SHELL`,
  `FX_CONFUSE_WAVE_TRAILS`, `FX_CONFUSE_ORBIT_TRAILS` (the strip page shows music staves, notes and smoke
  swirls: the real Confuse texture or a placeholder).

## Table

| module | index | name | evidence | what it is |
|---|---|---|---|---|
| fx_060 | 0x1 | `FX_CONFUSE_WAVE_TRAILS` | kind/usage (look pending) | emitter: two wobbling flat trails; the strip page shows music staves with notes and a hazy wave band, real texture or placeholder page |
| fx_060 | 0x2 | `FX_CONFUSE_SWIRLS` | render | emitter: teal and pink spiral swirl particles |
| fx_060 | 0x9 | `FX_CONFUSE_ORBIT_TRAILS` | kind/usage (look pending) | emitter: orbiting 3D + screen trails; the strip page shows music notes, a smoke swirl and a wavy smoke band, real texture or placeholder page |
| fx_061 | 0x1 | `FX_CONFUSE_SWIRLS` | render | emitter: teal and pink spiral swirl particles |
| fx_061 | 0x2 | `FX_CONFUSE_ORBIT_TRAILS` | kind/usage (look pending) | emitter: orbiting 3D + screen trails; the strip page shows music notes, a smoke swirl and a wavy smoke band, real texture or placeholder page |
| fx_061 | 0x3 | `FX_CONFUSE_SWIRLS_2` | render | emitter: teal and pink spiral swirl particles, rising in a cylinder |
| fx_061 | 0x7 | `FX_CONFUSE_WAVE_TRAILS` | kind/usage (look pending) | emitter: two wobbling flat trails; the strip page shows music staves with notes and a hazy wave band, real texture or placeholder page |
| fx_062 | 0x7 | `FX_DEMI_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_062 | 0x8 | `FX_DEMI_SPHERE` | render | model: sphere, white texture |
| fx_062 | 0x9 | `FX_DEMI_BACKDROP_2` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_062 | 0xA | `FX_DEMI_CAP` | kind/usage (look pending) | model: shallow spherical cap, texture still outside the file and the common pages (magenta) |
| fx_062 | 0xB | `FX_DEMI_BOWL` | render | model: gouraud hemisphere bowl |
| fx_062 | 0xC | `FX_DEMI_BOWL_2` | render | model: second gouraud hemisphere bowl |
| fx_062 | 0xD | `FX_DEMI_GLOW_TRAIL` | render | trail: white radial glow strip |
| fx_063 | 0x7 | `FX_DEMI_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) (byte-identical to fx_062 entry 0x7) |
| fx_063 | 0x8 | `FX_DEMI_SPHERE` | render | model: sphere, white texture (byte-identical to fx_062 entry 0x8) |
| fx_063 | 0x9 | `FX_DEMI_CAP` | kind/usage (look pending) | model: shallow spherical cap, texture still outside the file and the common pages (magenta) (byte-identical to fx_062 entry 0xa) |
| fx_063 | 0xA | `FX_DEMI_BACKDROP_2` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) (byte-identical to fx_062 entry 0x9) |
| fx_063 | 0xB | `FX_DEMI_BOWL` | render | model: gouraud hemisphere bowl (byte-identical to fx_062 entry 0xb) |
| fx_063 | 0xC | `FX_DEMI_BOWL_2` | render | model: second gouraud hemisphere bowl (byte-identical to fx_062 entry 0xc) |
| fx_063 | 0xD | `FX_DEMI_GLOW_TRAIL` | render | trail: white radial glow strip (byte-identical to fx_062 entry 0xd) |
| fx_078 | 0x2 | `FX_BIO_GREEN_RING` | render | emitter: rings textured with teal-green swirls |
| fx_078 | 0xC | `FX_BIO_VEC_TRACK` | kind/usage | table: vector track (fxm_vecTrackInit) |
| fx_078 | 0xD | `FX_BIO_ROT_POS_TRACK` | kind/usage | table: rotation + position track (fxm_rotPosTrackInit) |
| fx_078 | 0x14 | `FX_BIO_RED_BUBBLES` | render | emitter: red veined bubbles with a crimson glow |
| fx_078 | 0x15 | `FX_BIO_POS_TRACK` | kind/usage | table: position track (fxm_posTrackInit) |
| fx_078 | 0x16 | `FX_BIO_ORBIT_TRAIL` | kind/usage (look pending) | emitter: orbiting screen trail, still renders black |
| fx_078 | 0x17 | `FX_BIO_GREEN_CLOUD` | render | emitter: murky green cloud + grey glow |
| fx_079 | 0x1 | `FX_DEMI_SHOCK_WISP_BURST` | render | emitter: white gouraud rings + pink-violet smoke wisps |
| fx_079 | 0x2 | `FX_DEMI_SHOCK_FLASH_RING` | render | emitter: white gouraud ring |
| fx_079 | 0x6 | `FX_DEMI_SHOCK_BOWL_2` | render | model: second gouraud hemisphere bowl |
| fx_079 | 0x9 | `FX_DEMI_SHOCK_TRAIL` | kind/usage (look pending) | trail: strip texture still outside the file and the common pages (magenta) |
| fx_079 | 0xA | `FX_DEMI_SHOCK_BOWL` | render | model: gouraud hemisphere bowl |
| fx_079 | 0xB | `FX_DEMI_SHOCK_SPHERE` | kind/usage (look pending) | model: sphere, texture still outside the file and the common pages (magenta) |
| fx_079 | 0xC | `FX_DEMI_SHOCK_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_07a | 0x8 | `FX_DEMI_SHOCK_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_07a | 0x9 | `FX_DEMI_SHOCK_WISP_BURST` | render | emitter: white gouraud rings + blue and violet smoke wisps |
| fx_07a | 0xB | `FX_DEMI_SHOCK_SPHERE` | kind/usage (look pending) | model: sphere, texture still outside the file and the common pages (magenta) (byte-identical to fx_079 entry 0xB) |
| fx_07a | 0xC | `FX_DEMI_SHOCK_TRAIL` | kind/usage (look pending) | trail: strip texture still outside the file and the common pages (magenta) (byte-identical to fx_079 entry 0x9) |
| fx_07a | 0xD | `FX_DEMI_SHOCK_BOWL` | render | model: gouraud hemisphere bowl (byte-identical to fx_079 entry 0xa) |
| fx_07a | 0xE | `FX_DEMI_SHOCK_BOWL_2` | render | model: second gouraud hemisphere bowl (byte-identical to fx_079 entry 0x6) |
| fx_07b | 0x1 | `FX_PROTECT_BLUE_HELIX` | render | emitter: blue flat trail spiralling up + sprite |
| fx_07b | 0x3 | `FX_PROTECT_GOLD_RING` | render | emitter: golden ring particles with blue glints |
| fx_07c | 0x6 | `FX_MINI_LIGHT_BEAM` | render (fxdata d6121033) | emitter: fan drawn as a blue-white horizontal light beam + small blue rings |
| fx_07d | 0x1 | `FX_FLARE_RED_VIOLET_RINGS` | render | emitter: white-red and white-blue-violet gouraud rings |
| fx_07d | 0x2 | `FX_FLARE_ARCH` | render | model: half-ring arch, white texture |
| fx_07d | 0x3 | `FX_FLARE_ORANGE_GLOW` | render | particles: orange glow with a white core |
| fx_07d | 0x5 | `FX_FLARE_PINK_RING` | render | rings: white-pink-red gouraud rings |
| fx_07d | 0x8 | `FX_FLARE_BLUE_LIGHTNING` | render | trail: blue lightning strip |
| fx_07d | 0x9 | `FX_FLARE_WHITE_RING` | render | rings: white-cream gouraud rings |
| fx_07d | 0xB | `FX_FLARE_RED_SPARKS` | render | emitter: red spark sprites + white-red rings |
| fx_07d | 0xF | `FX_FLARE_RING_DISC` | render | model: gouraud flat annulus |
| fx_07d | 0x11 | `FX_FLARE_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_07d | 0x12 | `FX_FLARE_CALLBACK_EMITTER` | kind/usage (look pending) | emitter: orbit, one callback element, nothing drawable in the data |
| fx_07d | 0x13 | `FX_FLARE_FIRE_RING` | render | rings: white-cyan-orange-red gouraud rings |
| fx_07d | 0x14 | `FX_FLARE_WHITE_FLASH` | render (fxdata d6121033) | emitter: two fans drawn as solid white flash quads |
| fx_07d | 0x15 | `FX_FLARE_BACKDROP_2` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_07d | 0x16 | `FX_FLARE_FIRE_DOME` | render | model: dome with a fiery orange texture |
| fx_07d | 0x17 | `FX_FLARE_BACKDROP_3` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_07d | 0x18 | `FX_FLARE_TEX_RING` | kind/usage (look pending) | rings: textured rings; the UV box reads unrelated multicoloured VRAM, filled at run time |
| fx_07d | 0x19 | `FX_FLARE_TEX_RING_2` | kind/usage (look pending) | rings: textured rings; the UV box reads unrelated multicoloured VRAM, filled at run time |
| fx_07d | 0x1A | `FX_FLARE_SPARK_GLOW` | render | emitter: red spark sprites + fading orange glow |
| fx_07d | 0x1B | `FX_FLARE_GOLDEN_GLOWS` | render (fxdata d6121033) | emitter: two fans drawn as small golden glow spots |
| fx_07d | 0x1E | `FX_FLARE_CLOUD_SPHERE` | render (fxdata d6121033) | model: sphere with a grey cloudy texture |
| fx_07e | 0x1 | `FX_SLOW_GLINT` | kind/usage (look pending) | particles: white glint, the rest of the frame still outside the file and the common pages |
| fx_07e | 0x2 | `FX_SLOW_GREEN_RING` | render | rings: white-green gouraud rings |
| fx_07e | 0x3 | `FX_SLOW_BLUE_WAVE` | render | particles: blue chevron wave frames |
| fx_07e | 0x5 | `FX_SLOW_PARTICLES` | kind/usage (look pending) | particles: 8 frames, still render all black |
| fx_07e | 0xA | `FX_SLOW_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_07e | 0xC | `FX_SLOW_CRESCENT` | render | model: one quad textured with a blue-white crescent |
| fx_07f | 0x1 | `FX_SILENCE_RING_DISC` | kind/usage (look pending) | model: flat annulus; the UV box reads a placeholder page (hand-written Japanese text), filled at run time |
| fx_07f | 0x2 | `FX_SILENCE_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_07f | 0x3 | `FX_SILENCE_KANJI` | render | emitter: the kanji "silence" glyph, glowing orange to violet |
| fx_07f | 0x5 | `FX_SILENCE_KANJI_DOME` | render | model: dome textured with the "silence" glyph |
| fx_07f | 0x9 | `FX_SILENCE_PINK_PUFF` | render | emitter: pink-white puff |
| fx_07f | 0xC | `FX_SILENCE_PINK_RING` | render | emitter: white-pink gouraud rings |
| fx_080 | 0x3 | `FX_DISPEL_SEAL_DISCS` | render | emitter: engraved disc particles with pink glow |
| fx_080 | 0x7 | `FX_DISPEL_GOLD_SEAL` | render | emitter: orange engraved seal + white-violet rings |
| fx_080 | 0x9 | `FX_DISPEL_SEAL_SHATTER` | render | emitter: crystal shards, blue glow and silver seal discs |
| fx_080 | 0xA | `FX_DISPEL_FLASH_RINGS` | render | emitter: white-yellow and white-red gouraud rings |
| fx_081 | 0x4 | `FX_LV4_HOLY_BLUE_RING` | render | rings: white-cyan-blue gouraud rings |
| fx_081 | 0x5 | `FX_LV4_HOLY_GLINTS` | render | emitter: white star glints |
| fx_081 | 0x6 | `FX_LV4_HOLY_CYAN_RING` | render | rings: white-cyan-blue gouraud rings, 8 segments |
| fx_081 | 0x9 | `FX_LV4_HOLY_LILAC_RING` | render | rings: white-lilac gouraud rings |
| fx_081 | 0xA | `FX_LV4_HOLY_RAINBOW_ARCH` | render | model: half-ring arch, rainbow cloud texture |
| fx_081 | 0xB | `FX_LV4_HOLY_SPRITE_LINE` | kind/usage (look pending) | sprite line; its frames read a page of unrelated sprites, filled at run time |
| fx_081 | 0xC | `FX_LV4_HOLY_BLUE_SPARKLES` | render | particles: blue sparkle frames |
| fx_081 | 0xF | `FX_LV4_HOLY_BLUE_STREAKS` | render | emitter: blue light streaks + sparkles |
| fx_081 | 0x10 | `FX_LV4_HOLY_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_081 | 0x11 | `FX_LV4_HOLY_SPARKLE_EMITTER` | render | emitter: callback element + blue sparkles and specks |
| fx_081 | 0x12 | `FX_LV4_HOLY_TRAIL` | kind/usage (look pending) | trail: the strip reads unrelated multicoloured VRAM, filled at run time |
| fx_081 | 0x13 | `FX_LV4_HOLY_VIOLET_RAYS` | render (fxdata d6121033) | emitter: two fans drawn as narrow violet-white dithered rays |
| fx_081 | 0x14 | `FX_LV4_HOLY_VIOLET_RAYS_2` | render (fxdata d6121033) | emitter: two fans drawn as narrow violet-white dithered rays |
| fx_081 | 0x15 | `FX_LV4_HOLY_BLUE_SPECKS` | render | emitter: blue speck sprites |
| fx_081 | 0x16 | `FX_LV4_HOLY_VIOLET_RING` | render | rings: white-grey-violet-blue gouraud rings |
| fx_081 | 0x17 | `FX_LV4_HOLY_RING_DISC` | render | model: gouraud flat annulus |
| fx_081 | 0x19 | `FX_LV4_HOLY_DARK_BLUE_RING` | render | emitter: dark blue gouraud rings |
| fx_081 | 0x1A | `FX_LV4_HOLY_BLUE_RAYS` | render (fxdata d6121033) | emitter: two fans drawn as thin vertical blue light rays |
| fx_081 | 0x1C | `FX_LV4_HOLY_SPECK_EMITTER` | kind/usage (look pending) | emitter: dim sprites with blue specks; the frames read unrelated dithered VRAM |
| fx_081 | 0x1D | `FX_LV4_HOLY_PRISM_BURST` | render | emitter: prism-coloured particles + violet / cyan rings |
| fx_081 | 0x1F | `FX_LV4_HOLY_BLUE_RING_2` | render | rings: blue gouraud rings |
| fx_081 | 0x20 | `FX_LV4_HOLY_BACKDROP_2` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_081 | 0x21 | `FX_LV4_HOLY_WHITE_SPHERE` | render | model: sphere, white texture |
| fx_081 | 0x22 | `FX_LV4_HOLY_BACKDROP_3` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_081 | 0x23 | `FX_LV4_HOLY_TEX_RING` | kind/usage (look pending) | rings: textured rings; the UV box reads unrelated multicoloured VRAM, filled at run time |
| fx_081 | 0x24 | `FX_LV4_HOLY_VIOLET_RING_2` | render | rings: white-blue-violet gouraud rings, 4 segments |
| fx_081 | 0x25 | `FX_LV4_HOLY_PARTICLE` | kind/usage (look pending) | particles: one small frame; reads grey stripes that look like unrelated VRAM |
| fx_081 | 0x27 | `FX_LV4_HOLY_FLASH_RINGS` | render | emitter: white-yellow and white-grey gouraud rings |
| fx_082 | 0x1 | `FX_MAGIC_HAMMER_SMOKE` | render | emitter: grey smoke puffs |
| fx_082 | 0x2 | `FX_MAGIC_HAMMER_RAINBOW_STREAK` | render (fxdata d6121033) | emitter: fan drawn as a horizontal streak cycling blue, green, yellow, orange, red, pink |
| fx_082 | 0x3 | `FX_MAGIC_HAMMER_PASTEL_RINGS` | render | emitter: pastel gouraud rings + coloured star specks |
| fx_082 | 0x4 | `FX_MAGIC_HAMMER_STARS` | render | emitter: five-point stars fading blue to red |
| fx_082 | 0x5 | `FX_MAGIC_HAMMER_HAMMER` | render (fxdata d6121033) | 'so' model: the hammer (banded green barrel head with steel caps, short handle) |
| fx_082 | 0x7 | `FX_MAGIC_HAMMER_VIOLET_RING` | render | emitter: white-violet gouraud rings |
| fx_082 | 0x8 | `FX_MAGIC_HAMMER_CYAN_RING` | render | emitter: white-cyan gouraud rings |
| fx_083 | 0x1 | `FX_SCAN_RED_BAND` | render | model: ring band, red texture |
| fx_083 | 0x2 | `FX_SCAN_GLYPH_TEXT` | render | emitter: scrolling coloured glyph text |
| fx_083 | 0x5 | `FX_SCAN_METAL_SPHERE` | render | model: sphere with a riveted metal texture |
| fx_083 | 0x7 | `FX_SCAN_GLYPH_TEXT_2` | render | emitter: scrolling coloured glyph text, orbiting |
| fx_083 | 0xB | `FX_SCAN_ORB_TILES` | render | emitter: orange / blue orbs + yellow and blue tiles |
| fx_083 | 0xC | `FX_SCAN_YELLOW_RING` | render | emitter: dark yellow gouraud rings |
| fx_084 | 0x1 | `FX_DRAIN_SPRITES` | kind/usage (look pending) | emitter: sprites, still render all black |
| fx_084 | 0x2 | `FX_DRAIN_PINK_ORB` | render | emitter: pink veined orb |
| fx_084 | 0x6 | `FX_DRAIN_RED_RING` | render | emitter: dark red textured ring |
| fx_086 | 0x4 | `FX_METEOR_STREAK_SHELL` | kind/usage (look pending) | model: bullet-shaped shell; the UV box reads a placeholder page (hand-written Japanese text), filled at run time |
| fx_086 | 0x6 | `FX_METEOR_SPRITES` | kind/usage (look pending) | emitter: sprites, still render all black |
| fx_086 | 0x7 | `FX_METEOR_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_086 | 0x9 | `FX_METEOR_EXPLOSION` | render | emitter: green and orange explosion frames |
| fx_086 | 0xB | `FX_METEOR_CONE_SPRITES` | kind/usage (look pending) | emitter: cone of sprites, still render all black |
| fx_086 | 0xC | `FX_METEOR_DUST_RING` | render (fxdata d6121033) | model: ring band with a pink-brown rocky dust texture |
| fx_086 | 0xE | `FX_METEOR_CLOUDS` | render | sprite: grey cloud strip |
| fx_086 | 0xF | `FX_METEOR_SKY_CYLINDER` | render | model: cylinder with a blue sky / cloud texture |
| fx_086 | 0x10 | `FX_METEOR_DARK_CLOUDS` | render | sprite: dark cloud strip |
| fx_086 | 0x11 | `FX_METEOR_ROCK` | render | model: rock with a cratered texture (the meteor) |
| fx_086 | 0x12 | `FX_METEOR_ORANGE_RING` | render | rings: white-orange gouraud rings |
| fx_086 | 0x13 | `FX_METEOR_DEBRIS_SPRITES` | kind/usage (look pending) | sprites: 44 frames, still render all black |
| fx_086 | 0x18 | `FX_METEOR_STREAK` | render | model: long thin gouraud spike |
| fx_086 | 0x19 | `FX_METEOR_BOWL` | render | model: gouraud hemisphere bowl |
| fx_086 | 0x1A | `FX_METEOR_RING_DISC` | render | model: gouraud flat annulus |
| fx_086 | 0x1B | `FX_METEOR_FIRE_DOME` | render | model: dome with a fire texture |
| fx_087 | 0x6 | `FX_SPARE_CHANGE_STREAKS` | render | emitter: violet-white light streak trails |
| fx_087 | 0x8 | `FX_SPARE_CHANGE_COINS` | render | emitter: bronze coin sprites + white flashes |
| fx_089 | 0x4 | `FX_REVIVE_LIGHT_SWIRL` | render | emitter: blue-white light swirl + flare |
| fx_089 | 0x5 | `FX_REVIVE_FEATHERS` | render | emitter: white wing / feather sprites + flare |
| fx_08a | 0x6 | `FX_AUTO_LIFE_SPIRAL_TRAILS` | render | emitter: white trails spiralling, red specks, pink wing |
| fx_08a | 0x7 | `FX_AUTO_LIFE_FLASH_RINGS` | render | emitter: white-pink and white-blue gouraud rings |
| fx_08a | 0x9 | `FX_AUTO_LIFE_GOLD_WINGS` | render | emitter: golden wing sprites |
| fx_08a | 0xA | `FX_AUTO_LIFE_GOLD_WINGS_2` | render | emitter: golden wing sprites |
| fx_08b | 0x1 | `FX_AERIAL_SLASH_CRESCENT` | render (fxdata d6121033) | emitter: fan drawn as a pale green-white crescent slash + dim green specks |
| fx_08b | 0x2 | `FX_AERIAL_SLASH_CRESCENT_2` | render (fxdata d6121033) | emitter: fan drawn as a pale green-white crescent slash + dim green specks |
| fx_08b | 0x6 | `FX_AERIAL_SLASH_VIOLET_PUFFS` | render | emitter: violet cloud puffs + green glint |
| fx_08b | 0x8 | `FX_AERIAL_SLASH_BLUE_GUST` | render (fxdata d6121033) | emitter: fan drawn as a blue crescent gust + blue specks |
| fx_08b | 0xE | `FX_AERIAL_SLASH_SPARKLE_CURTAIN` | render | emitter: violet sparkle curtain ring + violet-white rings |
| fx_08b | 0xF | `FX_AERIAL_SLASH_SMOKE_RING` | render | emitter: ring textured with grey smoke |
| fx_08c | 0x1 | `FX_BERSERK_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_08c | 0x3 | `FX_BERSERK_FUNNEL` | render | model: open funnel, white texture |
| fx_08c | 0x8 | `FX_BERSERK_FIRE_RING` | render | emitter: red-orange-yellow gouraud rings |
| fx_08c | 0xB | `FX_BERSERK_STAR_FLASH` | render | emitter: white and red star flashes + violet ring |
| fx_08d | 0x1 | `FX_CURA_STAR_TRAIL` | render | emitter: blue star-sparkle trail + sprites |
| fx_08d | 0x7 | `FX_CURA_STAR_FLARES` | render | sprite line: blue-white star flares |
| fx_08d | 0x8 | `FX_CURA_CRESCENT_BURST` | render | emitter: ring + yellow, white and green crescent sprites |
| fx_08d | 0xA | `FX_CURA_FAN_EMITTER` | kind/usage (look pending) | emitter: fan; its rect reads cyan dithered vertical stripes (light rays or unrelated VRAM) |
| fx_08e | 0x1 | `FX_CURAGA_SPARKLE_BURST` | kind/usage (look pending) | emitter: ring + sparkle sprites, still drawn as coloured squares (texture not resolved) |
| fx_08e | 0x3 | `FX_CURAGA_WHITE_RINGS` | render | emitter: white gouraud rings |
| fx_08e | 0x5 | `FX_CURAGA_GOLD_RING` | render | emitter: white-gold gouraud rings |
| fx_08e | 0x6 | `FX_CURAGA_STAR_GLINTS` | render | emitter: green and violet star glints |
| fx_08e | 0x7 | `FX_CURAGA_STAR_TRAIL` | render | emitter: blue starry trail + sparkles |
| fx_08e | 0xD | `FX_CURAGA_SPARKLE_SPIRAL` | kind/usage (look pending) | emitter: spiralling sparkle sprites, still drawn as pale yellow-green squares |
| fx_08e | 0xE | `FX_CURAGA_LIGHT_FOUNTAIN` | render | model: tall funnel with a green light-fountain texture |
| fx_08e | 0xF | `FX_CURAGA_LIGHT_RING` | render | model: ring with a green light-fountain texture |
| fx_08e | 0x10 | `FX_CURAGA_SPARKLES` | kind/usage (look pending) | emitter: sparkle sprites, still drawn as white / green squares |
| fx_08f | 0x1 | `FX_METEOR_FIRE_RING` | render | rings: cream-red-blue-violet-orange gouraud rings |
| fx_08f | 0x4 | `FX_METEOR_STREAK_SHELL` | kind/usage (look pending) | model: bullet-shaped shell; the UV box reads a placeholder page (hand-written Japanese text), filled at run time (byte-identical to fx_086 entry 0x4) |
| fx_08f | 0x7 | `FX_METEOR_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) (byte-identical to fx_086 entry 0x7) |
| fx_08f | 0xB | `FX_METEOR_CONE_SPRITES` | kind/usage (look pending) | emitter: cone of sprites, still render all black (byte-identical to fx_086 entry 0xB) |
| fx_08f | 0xC | `FX_METEOR_DUST_RING` | render (fxdata d6121033) | model: ring band with a pink-brown rocky dust texture (byte-identical to fx_086 entry 0xC) |
| fx_08f | 0xE | `FX_METEOR_CLOUDS` | render | sprite: grey cloud strip |
| fx_08f | 0xF | `FX_METEOR_SKY_CYLINDER` | render | model: cylinder with a blue sky / cloud texture (byte-identical to fx_086 entry 0xf) |
| fx_08f | 0x10 | `FX_METEOR_DARK_CLOUDS` | render | sprite: dark cloud strip |
| fx_08f | 0x11 | `FX_METEOR_ROCK` | render | model: rock with a cratered texture (the meteor) (byte-identical to fx_086 entry 0x11) |
| fx_08f | 0x13 | `FX_METEOR_DEBRIS_SPRITES` | kind/usage (look pending) | sprites: 44 frames, still render all black (byte-identical to fx_086 entry 0x13) |
| fx_08f | 0x17 | `FX_METEOR_STREAK` | render | model: long thin gouraud spike (byte-identical to fx_086 entry 0x18) |
| fx_08f | 0x18 | `FX_METEOR_BOWL` | render | model: gouraud hemisphere bowl (byte-identical to fx_086 entry 0x19) |
| fx_08f | 0x19 | `FX_METEOR_RING_DISC` | render | model: gouraud flat annulus (byte-identical to fx_086 entry 0x1a) |
| fx_08f | 0x1A | `FX_METEOR_FIRE_DOME` | render | model: dome with a fire texture (byte-identical to fx_086 entry 0x1b) |
| fx_08f | 0x1B | `FX_METEOR_EXPLOSION` | render | emitter: green and orange explosion frames |
| fx_08f | 0x1C | `FX_METEOR_SMOKE_TRAIL` | render | trail: grey smoke strip |
| fx_090 | 0x2 | `FX_CURAGA_STAR_GLINTS` | render | emitter: white star glints + faint coloured arcs |
| fx_090 | 0x3 | `FX_CURAGA_SPARKLE_BURST` | kind/usage (look pending) | emitter: ring + white / green sparkle dots, its last element still drawn as squares (texture not resolved) |
| fx_090 | 0x5 | `FX_CURAGA_STAR_TRAIL` | render | emitter: blue starry trail + sparkles |
| fx_090 | 0xB | `FX_CURAGA_LIGHT_FOUNTAIN` | render | model: tall funnel with a cyan light-fountain texture |
| fx_090 | 0xC | `FX_CURAGA_WHITE_RINGS` | render | emitter: white gouraud rings |
| fx_090 | 0xD | `FX_CURAGA_LIGHT_RING` | render | model: ring with a cyan light-fountain texture |
| fx_090 | 0xE | `FX_CURAGA_TEAL_RING` | render | emitter: white-teal gouraud rings |
| fx_090 | 0xF | `FX_CURAGA_SPARKLES` | kind/usage (look pending) | emitter: sparkle sprites, still drawn as white / green squares |
| fx_090 | 0x10 | `FX_CURAGA_SPARKLE_SPIRAL` | kind/usage (look pending) | emitter: spiralling sparkle sprites, still drawn as pale yellow-green squares |
| fx_091 | 0x1 | `FX_ENTICE_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_091 | 0x2 | `FX_ENTICE_VIOLET_HALO` | render | emitter: violet glow ring + sprites |
| fx_092 | 0x1 | `FX_HYPNOTIZE_MAGENTA_RINGS` | render | emitter: violet-cream and magenta gouraud rings |
| fx_092 | 0x2 | `FX_HYPNOTIZE_HALO_RINGS` | render | emitter: glowing circle particles, violet to orange |
| fx_092 | 0x3 | `FX_HYPNOTIZE_VIOLET_RING` | render | emitter: violet-tan gouraud rings |
| fx_092 | 0x4 | `FX_HYPNOTIZE_SPARKLE_DOTS` | render (fxdata d6121033) | emitter: faint white sparkle dots (btlfx_emitDots) |
| fx_093 | 0x2 | `FX_CANNON_SMOKE` | render | emitter: grey smoke puffs |
| fx_093 | 0x4 | `FX_CANNON_EXPLOSION` | render | emitter: explosion burst + sparks |
| fx_093 | 0x8 | `FX_CANNON_BLAST_RING` | render | emitter: cream-orange gouraud rings |
| fx_093 | 0xC | `FX_CANNON_DUST_RING` | render | emitter: grey dust ring |
| fx_094 | 0x1 | `FX_DELTA_ATTACK_WHITE_RING` | render | emitter: white gouraud rings |
| fx_094 | 0x2 | `FX_DELTA_ATTACK_VIOLET_RING` | render | emitter: homing violet gouraud rings |
| fx_094 | 0x3 | `FX_DELTA_ATTACK_WHITE_RING_2` | render | emitter: white gouraud rings |
| fx_094 | 0x4 | `FX_DELTA_ATTACK_COLOR_RINGS` | render | emitter: yellow / blue / red / gold gouraud rings |
| fx_094 | 0x5 | `FX_DELTA_ATTACK_RAINBOW_STREAK` | render (fxdata d6121033) | emitter: fan drawn as a thin rainbow-coloured horizontal streak |
| fx_095 | 0x20 | `FX_STOP_CLOCK_RING` | render | sprite: silver clock ring with spikes |
| fx_095 | 0x2C | `FX_STOP_FLARE` | render | sprite: blue-red flare, 6 frames |
| fx_096 | 0x1 | `FX_FROST_ICE_GLINTS` | kind/usage (look pending) | emitter: blue particles + sprites; the frames read blue dithered blocks that look like unrelated VRAM |
| fx_096 | 0x6 | `FX_FROST_SNOW_BURST` | render | emitter: ring of ice-flake sprites + white-cyan rings |
| fx_096 | 0xA | `FX_FROST_ICE_CRYSTAL` | render (fxdata d6121033) | 'so' model: faceted ice crystal (pale blue cracked-ice texture) |
| fx_096 | 0xB | `FX_FROST_ICE_SHARD` | render (fxdata d6121033) | 'so' model: flat jagged ice shard (same cracked-ice texture) |
| fx_096 | 0xC | `FX_FROST_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_099 | 0x1 | `FX_BREAK_STONE_CRUST` | render (fxdata d6121033) | 'so' model: jagged brown stone pieces (cracked rock texture) |
| fx_099 | 0x2 | `FX_BREAK_CYAN_RINGS` | render | emitter: white-cyan / lilac gouraud rings + fan |
| fx_099 | 0x3 | `FX_BREAK_DUST_CLOUD` | render | emitter: sparkles, brown dust cloud, pale puffs |
| fx_099 | 0x4 | `FX_BREAK_ROCK_RING` | render | emitter: ring of white rock chunks |
| fx_09a | 0x1 | `FX_LV5_DEATH_GREEN_GLOW` | render | emitter: fan + green glow sprites, orange / violet rings |
| fx_09a | 0x2 | `FX_LV5_DEATH_MODEL_DATA` | kind/usage | table: data block passed with the model to btlfx_registerSummonModel |
| fx_09a | 0x3 | `FX_LV5_DEATH_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_09a | 0x7 | `FX_LV5_DEATH_BLUE_TRAIL` | render | emitter: blue spiralling trail |
| fx_09a | 0x8 | `FX_LV5_DEATH_REAPER` | render (fxdata d6121033) | 'so' model (btlfx_registerSummonModel): the robed skull reaper holding a scythe |
| fx_09a | 0x9 | `FX_LV5_DEATH_RED_BURST` | render | emitter: pink-red sprites + white-red rings |
| fx_09a | 0xE | `FX_LV5_DEATH_GREY_SMOKE` | render | emitter: grey smoke blobs |
| fx_09b | 0x1 | `FX_LAVA_GUN_FIRE_FLASH` | render | emitter: red rings, fiery glow, embers |
| fx_09b | 0x3 | `FX_LAVA_GUN_EXPLOSION` | render | emitter: fire explosion + embers |
| fx_09b | 0x4 | `FX_LAVA_GUN_FIRE_RING` | render | emitter: ring with a fiery texture |
| fx_09b | 0x5 | `FX_LAVA_GUN_RED_RING` | render | emitter: red gouraud rings |
| fx_09b | 0x6 | `FX_LAVA_GUN_LAVA_BALL` | render (fxdata d6121033) | 'so' model: rounded blob with a red-orange molten texture |
| fx_09c | 0x1 | `FX_GNOLL_ATTACK_IMPACT_RINGS` | render | emitter: cream-orange and white gouraud rings + sparkles |
| fx_09c | 0x2 | `FX_GNOLL_ATTACK_IMPACT_RINGS_2` | render | emitter: cream-orange, magenta and white gouraud rings + sparkles |
| fx_09c | 0x7 | `FX_GNOLL_ATTACK_SPARKLE_RINGS` | render | emitter: fans + white-cream-red rings and sparkles |
| fx_09c | 0x8 | `FX_GNOLL_ATTACK_GREY_RING` | render | emitter: grey gouraud rings |
| fx_09e | 0x6 | `FX_BLIND_VORTEX` | render | emitter: silver-blue swirling vortex ring |
| fx_09e | 0x8 | `FX_BLIND_VORTEX_2` | render | emitter: silver-blue swirling vortex ring + fans |
| fx_09f | 0x2 | `FX_PYRO_COLOR_PUFFS` | render | emitter: yellow, violet, cyan and green puffs |
| fx_09f | 0x3 | `FX_PYRO_SMOKE` | render | emitter: dark grey smoke |
| fx_09f | 0x4 | `FX_PYRO_SPARK_ARCS` | render | emitter: yellow-red spark arcs |
| fx_09f | 0x5 | `FX_PYRO_ORANGE_RING` | render | emitter: white-orange gouraud rings |
| fx_09f | 0x7 | `FX_PYRO_FLAMES` | render | emitter: flames + smoke |
| fx_09f | 0xC | `FX_PYRO_FLAMES_2` | render | emitter: spiralling flames + smoke |
| fx_0a0 | 0x1 | `FX_WEB_CALLBACK_EMITTER` | kind/usage (look pending) | emitter: orbit, one callback element, nothing drawable in the data |
| fx_0a0 | 0x2 | `FX_WEB_WHITE_PUFFS` | render | emitter: white puffs |
| fx_0a0 | 0x6 | `FX_WEB_STRAND_LINE` | kind/usage | table: strand line data (fxm_drawTrailLine) |
| fx_0a0 | 0x7 | `FX_WEB_STRAND_TRAIL` | kind/usage (look pending) | trail: dark strip, still nearly black |
| fx_0a1 | 0x1 | `FX_BLASTER_TEX_RING` | render | emitter: textured green-grey ring band |
| fx_0a1 | 0x2 | `FX_BLASTER_PINK_HALO` | render | emitter: green ring band + pink halo rings |
| fx_0a1 | 0x3 | `FX_BLASTER_PINK_HALO_2` | render | emitter: pink halo rings |
| fx_0a1 | 0x4 | `FX_BLASTER_LIGHT_CONE` | render (fxdata d6121033) | emitter: fan drawn as a narrowing light cone fading pink to yellow |
| fx_0a1 | 0x5 | `FX_BLASTER_PINK_STARBURST` | render | emitter: pink starburst + violet petals |
| fx_0a1 | 0x6 | `FX_BLASTER_VIOLET_PETALS` | render | emitter: violet petal sprites |
| fx_0a1 | 0x7 | `FX_BLASTER_TRAIL` | kind/usage (look pending) | emitter: flat orbiting trail; the grey strip reads a page of unrelated smoke shapes |
| fx_0a1 | 0x8 | `FX_BLASTER_GREY_RING` | render | emitter: lilac-grey gouraud rings |
| fx_0a1 | 0xF | `FX_BLASTER_VIOLET_RING` | render | emitter: white-violet gouraud rings |
| fx_0a1 | 0x10 | `FX_BLASTER_TRAIL_2` | kind/usage (look pending) | trail: small strip, mostly black with magenta texels |
| fx_0a2 | 0x1 | `FX_CURA_SPARKLE_BURST` | kind/usage (look pending) | emitter: ring + sparkle sprites, still drawn as green / yellow / white squares (texture not resolved) |
| fx_0a2 | 0x2 | `FX_CURA_STAR_TRAIL` | render | emitter: blue star-sparkle trail spiralling + sprites |
| fx_0a3 | 0x1 | `FX_ELECTROCUTE_BOLT_TRAIL` | kind/usage (look pending) | emitter: small blue bolt trail + sprites that render black or as faint dots |
| fx_0a3 | 0x2 | `FX_ELECTROCUTE_BOLT_TRAIL_2` | kind/usage (look pending) | emitter: small blue bolt trail + sprites that render black, spiralling |
| fx_0a4 | 0x1 | `FX_BUZZ_SPRITE` | kind/usage (look pending) | emitter: one sprite, still renders black |
| fx_0a4 | 0x6 | `FX_BUZZ_SOUND_WAVE` | render | emitter: ring textured with a yellow sine wave |
| fx_0a5 | 0x7 | `FX_SPORE_TRAIL` | kind/usage (look pending) | emitter: orbiting trail + sprite, still render black |
| fx_0a5 | 0x8 | `FX_SPORE_SPORES` | render | emitter: brown spore sprites |
| fx_0a6 | 0x1 | `FX_SWEEP_DUST_RINGS` | render | emitter: gold-brown gouraud rings + dust |
| fx_0a6 | 0x2 | `FX_SWEEP_WIND_TRAIL` | render | emitter: grey wind streak trail |
| fx_0a6 | 0x3 | `FX_SWEEP_DUST_PUFF` | render | emitter: yellow-brown dust puff |
| fx_0a6 | 0x4 | `FX_SWEEP_TAN_RING` | render | emitter: tan-white gouraud rings |
| fx_0a6 | 0x5 | `FX_SWEEP_DUST_CLOUD` | render | emitter: dust cloud puffs |
| fx_0a7 | 0x1 | `FX_WATER_GUN_SPRAY_ARC` | render | emitter: fan + blue water spray arc |
| fx_0a7 | 0x4 | `FX_WATER_GUN_WATER_TRAIL` | render | emitter: water strip trail + droplet sprites |
| fx_0a7 | 0x5 | `FX_WATER_GUN_SPRAY_ARC_2` | render | emitter: blue water spray arc |
| fx_0a7 | 0x6 | `FX_WATER_GUN_WATER_TRAIL_2` | render | emitter: water strip trail |
| fx_0a7 | 0x7 | `FX_WATER_GUN_BUBBLES` | render | emitter: bubble rings |
| fx_0a8 | 0x1 | `FX_REIS_WIND_SPARKLES` | render | emitter: violet, gold, teal and white sparkles |
| fx_0a8 | 0x3 | `FX_REIS_WIND_LIGHT_STREAK` | render (fxdata d6121033) | emitter: fan drawn as a thin green to violet horizontal light streak |
| fx_0a8 | 0x5 | `FX_REIS_WIND_SPARKLES_2` | render | emitter: gold and teal sparkles |
| fx_0a8 | 0x6 | `FX_REIS_WIND_VIOLET_SPARKLES` | render | emitter: violet sparkles |
| fx_0a8 | 0x7 | `FX_REIS_WIND_WIND_TRAIL` | render | emitter: green wind trail + green specks |
| fx_0a8 | 0xC | `FX_REIS_WIND_GREEN_RINGS` | render | emitter: green gouraud rings |
| fx_0a8 | 0xF | `FX_REIS_WIND_GREEN_WISP` | render | emitter: green wisp puffs |
| fx_0a8 | 0x10 | `FX_REIS_WIND_GREEN_CRESCENT` | render | emitter: green crescents |
| fx_0a9 | 0x1 | `FX_LIGHTNING_BOLT_PILLAR` | render | trail: violet lightning pillar strip |
| fx_0a9 | 0x3 | `FX_LIGHTNING_ELECTRIC_ARCS` | render | emitter: blue / green electric arcs + embers |
| fx_0a9 | 0x4 | `FX_LIGHTNING_TRAIL_2` | kind/usage (look pending) | trail: one frame, still not rendered by the tool |
| fx_0a9 | 0x6 | `FX_LIGHTNING_BLUE_RING` | render | rings: white-blue-violet gouraud rings |
| fx_0aa | 0x1 | `FX_SNOWSTORM_SNOW_BURST` | render | emitter: snow-textured ring + snowflake sprites |
| fx_0aa | 0x2 | `FX_SNOWSTORM_SNOW_BURST_2` | render | emitter: snow-textured ring + snowflake sprites |
| fx_0aa | 0x3 | `FX_SNOWSTORM_SNOW_STREAM` | render | emitter: snow streak trail + snowflakes |
| fx_0aa | 0x4 | `FX_SNOWSTORM_SNOW_STREAM_2` | render | emitter: snow streak trail + snowflakes |
| fx_0aa | 0x5 | `FX_SNOWSTORM_ICE_FLAKES` | render | emitter: icy cyan flakes |
| fx_0aa | 0x6 | `FX_SNOWSTORM_MIST` | render | emitter: grey-teal mist wisps |
| fx_0aa | 0x7 | `FX_SNOWSTORM_FROST_RINGS` | render | emitter: teal gouraud rings + snow-textured rings |
| fx_0aa | 0x8 | `FX_SNOWSTORM_FLAKES` | render | emitter: snowflake sprites |
| fx_0aa | 0x9 | `FX_SNOWSTORM_SNOW_TRAIL` | render | emitter: snow streak trail spiralling |
| fx_0ab | 0x1 | `FX_MAELSTROM_RED_PUFFS` | render | emitter: red puffs + sprites |
| fx_0ab | 0x2 | `FX_MAELSTROM_RED_STREAKS` | render | emitter: red wisps and streaks |
| fx_0ab | 0x3 | `FX_MAELSTROM_SWIRL` | render | emitter: dark red spiral swirl |
| fx_0ab | 0x4 | `FX_MAELSTROM_WIND_TRAIL` | render | emitter: grey smoke trail spiralling |
| fx_0ac | 0x1 | `FX_SLEEPING_JUICE_SPLASH_TRAILS` | render | emitter: yellow liquid trails |
| fx_0ac | 0x2 | `FX_SLEEPING_JUICE_RIPPLE` | render | emitter: yellow arc particles |
| fx_0ac | 0x3 | `FX_SLEEPING_JUICE_BUBBLES` | render | emitter: yellow bubble cloud |
| fx_0ac | 0x4 | `FX_SLEEPING_JUICE_GLOW` | render | emitter: yellow glow burst |
| fx_0ad | 0x1 | `FX_AERO_BREATH_SWIRL` | render | emitter: green wind swirl |
| fx_0ad | 0x6 | `FX_AERO_BREATH_WIND_TRAILS` | render | emitter: green wind trails spiralling |
| fx_0ae | 0x1 | `FX_VENOM_BREATH_VENOM_CLOUD` | render | emitter: violet venom blobs + droplets |
| fx_0ae | 0x2 | `FX_VENOM_BREATH_VENOM_SPLASH` | render | emitter: violet venom blobs and droplet splash |
| fx_0af | 0x5 | `FX_ZOMBIE_BREATH_BREATH_TRAILS` | render | emitter: white ghostly puff trails |
| fx_0af | 0x7 | `FX_ZOMBIE_BREATH_GREEN_GLOW` | render | emitter: white puffs + pale green glow |
| fx_0af | 0x8 | `FX_ZOMBIE_BREATH_BREATH_TRAILS_2` | render | emitter: white ghostly puff trails + one more trail |
| fx_0b0 | 0x1 | `FX_LEAF_SWIRL_LEAF_TRAIL` | kind/usage (look pending) | emitter: helix trail, mostly dark strip with a leaf tip |
| fx_0b0 | 0x2 | `FX_LEAF_SWIRL_LEAF_TRAIL_2` | kind/usage (look pending) | emitter: helix trail, mostly dark strip with a leaf tip |
| fx_0b0 | 0x3 | `FX_LEAF_SWIRL_DUST_SPECKS` | render (fxdata d6121033) | emitter: brown dust puffs + small specks (btlfx_emitDots) |
| fx_0b0 | 0x4 | `FX_LEAF_SWIRL_OLIVE_RING` | render | emitter: olive gouraud rings |
| fx_0b0 | 0xA | `FX_LEAF_SWIRL_LEAF_TRAIL_3` | kind/usage (look pending) | emitter: spiral trail, mostly dark strip with a leaf tip |
| fx_0b0 | 0xB | `FX_LEAF_SWIRL_LEAF_TRAIL_4` | kind/usage (look pending) | emitter: spiral trail, mostly dark strip with a leaf tip |
| fx_0b0 | 0xC | `FX_LEAF_SWIRL_WIND_TRAIL` | render | emitter: grey wind trail |
