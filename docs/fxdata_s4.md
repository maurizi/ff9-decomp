# Battle effect data names, slice 4 (fx_0f3 .. fx_128)

Same scheme and tool as the pilot in `docs/fxdata.md`: 48 modules, every `btlfx_getFileData` literal in them
named (327 module/index defines: 191 by the render, 57 data tables by the export they are passed to, 79 by
kind with the look pending). Evidence column: `render` = named by the `tools/fxdata.py --render` contact sheet,
`kind/usage` = a data table named by its export (backdrop layers, camera keys, ribbon paths), `kind/usage (look
pending)` = appearance not settled by the render (open questions below).

Notes:
- Zidane's Dyne skills (Scoop Art, Shift Break, Stellar Circle 5, Meo Twister, Solution 9, Grand Lethal) use the
  skill names; Solution 9's red glyph sprites are drawn as runes.
- Enemy-only modules use the enemy skill's name: `FX_SOLUTION_` (fx_100), `FX_RISE_`, `FX_MIST_`,
  `FX_EVERYONES_GRUDGE_`, `FX_MUCUS_`, `FX_HIGH_WIND_`, `FX_ABSORB_`, `FX_VIRUS_FLY_`, `FX_VIRUS_COMBO_`,
  `FX_RAINING_SWORDS_`, `FX_BOOMERANG_`, `FX_WAVE_`, enemy Eat = `FX_EAT_` like Quina's Eat.
- Summons: Madeen (fx_0fb_0, fx_0fb_1; the same name in both groups for byte-identical entries), Odin (fx_105),
  Ifrit (fx_114). Summon-data indices (0x80+) are `FX_<SUMMON>_SUMMON_MODEL_<n>` (n = index - 0x80) as in slice 2;
  the tool does not read the summon data, so their look is pending. Ifrit's summon-data tables are named by the
  track / path init they go to (`FX_IFRIT_SUMMON_VEC_TRACK_4` = entry 0x84 through fxm_vecTrackInit).
- Shared files: fx_117 plays Echo Screen (0x117), Dead Pepper (0x120) and Gysahl Greens (0x121), each with its
  own entry 4 (one define, `FX_ECHO_SCREEN_SPARKLES`, the comment describes all three); fx_124 plays every gem
  item and takes the first one's name (`FX_AMETHYST_`).
- Throw modules (fx_109..fx_110, fx_115): `FX_THROW_<weapon>` is the 'so' model the C draws flying,
  `_AFTERIMAGE` the textured copy drawn as fading afterimages; the impact emitter is byte-identical in eight of
  them.
- Odin's camera key tables are named by shot (the start frame passed to fxm_camKeyAdd: 0, 0xA7, 0xFB, 0x137 =
  shots 0-3) and the slot word (`_3` = env->camera[2], `_1` = [0], `_2` = [1], `_9` = [8], `_4` = [3]).
- Byte-identical entries were found by hashing every entry of the slice (build-time helper, not committed); names
  only carry over through those, call-site matches alone are not used.
- `tools/fxdata.py` crashes on some modules of this slice (`classify`: `u.get('call', '')` is `None` for a use
  without a call); the renders here came from a copy with `(u.get('call') or '')`.

## Open questions (emulator)

All still-pending names of all slices, grouped by battle setup with what to capture, are in
`docs/wip/fxdata_runtime.md` (the emulator checklist); the list below is this slice's original notes.

16 names are still by kind (look pending) after the re-render with `tools/fxdata.py` at d6121033 (summon data, fans, 'so' models and dot sprites drawn). What is left is texture the file does not hold or the tool does not resolve (blank or placeholder pages filled at run time, tpages the C computes, texture lists the tool does not follow, garbled texels). Each wants one screenshot at the frame the C starts the entry, or a VRAM dump while it plays:

| module | index | name | why the look is pending |
|---|---|---|---|
| fx_0f3 | 0x8 | `FX_SCOOP_ART_SCREEN_TRAIL` | emitter: screen trail + sprite, texture on a placeholder page (the render reads garbled tan texels) |
| fx_0f4 | 0xc | `FX_SHIFT_BREAK_CONE_RINGS` | emitter: textured rings in a cone; the render reads garbled multicolour texels (texture not resolved) |
| fx_0f8 | 0xd | `FX_GRAND_LETHAL_SPRITE_RINGS` | emitter: white / cyan / blue / grey gouraud rings with a sprite that renders as a flat violet square (texture not in the file) |
| fx_0f8 | 0xe | `FX_GRAND_LETHAL_SWIRL_RINGS` | emitter: white swirl sprite with textured rings; the rings still read garbled multicolour texels (texture not resolved) |
| fx_0fb_1 | 0x14 | `FX_MADEEN_TRAIL` | trail: strip texture not in the file (magenta in the render) |
| fx_112 | 0x2 | `FX_EAT_ORBIT_TRAIL` | emitter: orbiting screen trail; the strip is still mostly dark in the render |
| fx_114 | 0x12 | `FX_IFRIT_SPOKED_RING` | model: flat ring with spokes (registered through the texture list; the tool does not follow it, so the texture is not drawn) |
| fx_114 | 0x1e | `FX_IFRIT_BOWL` | model: hemisphere bowl (registered through the texture list; the tool does not follow it, so the texture is not drawn) |
| fx_114 | 0x2a | `FX_IFRIT_GRID_PLANE` | model: flat grid plane (tpage 0x117 reads a yellow placeholder page in the render) |
| fx_114 | 0x81 | `FX_IFRIT_CANOPY` | summon data entry 1: textured model, canopy on legs (registered through the texture list; the tool does not follow it, so the texture is not drawn) |
| fx_114 | 0x83 | `FX_IFRIT_CYLINDER` | summon data entry 3: textured model, cylinder on tpage 0x117 (a yellow placeholder page in the render) |
| fx_11d | 0x9 | `FX_ABSORB_TRAIL` | emitter: 3D trail; the render is still black (byte-identical to entry 0xA) |
| fx_11d | 0xa | `FX_ABSORB_TRAIL_2` | emitter: 3D trail; the render is still black (byte-identical to entry 0x9) |
| fx_123 | 0xc | `FX_RAINING_SWORDS_DARK_BURST` | emitter: rings and sprites that still render black, + cyan dot specks |
| fx_125 | 0xd | `FX_POLY_POLYHEDRON` | model: faceted polyhedron, texture not in the file (magenta in the render) (byte-identical to entry 0xE) |
| fx_125 | 0xe | `FX_POLY_POLYHEDRON_2` | model: faceted polyhedron, registered with a tpage the C computes (not resolved by the tool) (byte-identical to entry 0xD) |

## Table

| module | index | name | evidence | what it is |
|---|---|---|---|---|
| fx_0f3 | 0x2 | `FX_SCOOP_ART_BLUE_RINGS` | render | emitter: white-blue gouraud rings + rings textured with blue swirl marks |
| fx_0f3 | 0x3 | `FX_SCOOP_ART_PINK_TRAIL` | render | trail: red-pink glowing strip drawn along the moving point |
| fx_0f3 | 0x8 | `FX_SCOOP_ART_SCREEN_TRAIL` | kind/usage (look pending) | emitter: screen trail + sprite, texture on a placeholder page (the render reads garbled tan texels) |
| fx_0f4 | 0x2 | `FX_SHIFT_BREAK_WHITE_RINGS` | render | emitter: white gouraud rings in a cone + a textured ring (texture not resolved) |
| fx_0f4 | 0x3 | `FX_SHIFT_BREAK_RAINBOW_HALO` | render | emitter: orbiting particles, dark disc with a rainbow rim |
| fx_0f4 | 0x4 | `FX_SHIFT_BREAK_VIOLET_HALO` | render | emitter: orbiting white swirl marks + blue-violet glow ring |
| fx_0f4 | 0x8 | `FX_SHIFT_BREAK_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_0f4 | 0xA | `FX_SHIFT_BREAK_DOME` | render | model: stepped dome, white texture |
| fx_0f4 | 0xB | `FX_SHIFT_BREAK_DOME_2` | render | model: stepped dome, white texture (byte-identical to entry 0xA) |
| fx_0f4 | 0xC | `FX_SHIFT_BREAK_CONE_RINGS` | kind/usage (look pending) | emitter: textured rings in a cone; the render reads garbled multicolour texels (texture not resolved) |
| fx_0f4 | 0xD | `FX_SHIFT_BREAK_LIGHTNING` | render | emitter: white-cyan gouraud rings + blue lightning streak particles |
| fx_0f4 | 0xE | `FX_SHIFT_BREAK_BACKDROP_2` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_0f5 | 0x7 | `FX_STELLAR_CIRCLE_5_SMOKE` | render | emitter: grey smoke cloud, 20 animated frames |
| fx_0f5 | 0x9 | `FX_STELLAR_CIRCLE_5_CLOUD_DOME` | render (fxdata d6121033) | 'so' model: shallow dome with a grey-violet cloud texture |
| fx_0f6 | 0xD | `FX_MEO_TWISTER_WIND_RINGS` | render | emitter: white / pale-cyan gouraud rings in a cone + a fan element |
| fx_0f6 | 0xE | `FX_MEO_TWISTER_FIRE_ORBS` | render | emitter: red / yellow / green gouraud rings + fiery orb particles |
| fx_0f7 | 0x2 | `FX_SOLUTION_9_BACKDROP_2` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_0f7 | 0x4 | `FX_SOLUTION_9_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_0f7 | 0x5 | `FX_SOLUTION_9_RUNES` | render | sprite: red glowing rune glyphs (large) |
| fx_0f7 | 0xB | `FX_SOLUTION_9_BLUE_RINGS` | render | emitter: white-blue gouraud rings + textured rings (texture not resolved) |
| fx_0f7 | 0xC | `FX_SOLUTION_9_RUNES_2` | render | sprite: red glowing rune glyphs (small) |
| fx_0f7 | 0xE | `FX_SOLUTION_9_RUNE_RINGS` | render | emitter: white-blue gouraud rings + red rune particles, fan elements |
| fx_0f7 | 0xF | `FX_SOLUTION_9_HALO_RINGS` | render | emitter: white-blue gouraud rings + dark ring with a rainbow rim |
| fx_0f8 | 0x1 | `FX_GRAND_LETHAL_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_0f8 | 0x2 | `FX_GRAND_LETHAL_BACKDROP_2` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_0f8 | 0x3 | `FX_GRAND_LETHAL_PINK_FLARES` | render | emitter: pink lens-flare glows and streaks |
| fx_0f8 | 0xD | `FX_GRAND_LETHAL_SPRITE_RINGS` | kind/usage (look pending) | emitter: white / cyan / blue / grey gouraud rings with a sprite that renders as a flat violet square (texture not in the file) |
| fx_0f8 | 0xE | `FX_GRAND_LETHAL_SWIRL_RINGS` | kind/usage (look pending) | emitter: white swirl sprite with textured rings; the rings still read garbled multicolour texels (texture not resolved) |
| fx_0f9 | 0x1 | `FX_FLEE_SPARKLE_BURST` | render | emitter: pale gouraud rings, pale sparkle puffs and orange sparks |
| fx_0fa | 0x1 | `FX_DETECT_METAL_SPHERE` | render | model: 12-sided sphere, bronze riveted-metal texture in the render |
| fx_0fa | 0x2 | `FX_DETECT_BLUE_SPIKES` | render (fxdata d6121033) | emitter: two fans, blue light spikes |
| fx_0fa | 0x3 | `FX_DETECT_GLYPH_TEXT` | render | emitter: blocks of scrolling script glyphs (blue, green, purple, orange) |
| fx_0fb_0 | 0x1 | `FX_MADEEN_SPECKS` | render (fxdata d6121033) | emitter: white dot specks |
| fx_0fb_0 | 0x2 | `FX_MADEEN_ROCK_DUST` | render | emitter: tan dust puff + teal-green rock debris |
| fx_0fb_0 | 0x3 | `FX_MADEEN_CLOUD_STRIP` | render (fxdata d6121033) | sprite: wide grey-white cloud strip drawn scrolling across the screen (byte-identical to fx_0fb_1 entry 0x1) |
| fx_0fb_0 | 0x4 | `FX_MADEEN_BLUE_SPECKS` | render | emitter: small blue sparkle specks |
| fx_0fb_0 | 0x5 | `FX_MADEEN_GOLD_LIGHT_BAND` | render (fxdata d6121033) | emitter: fan, a horizontal gold light band |
| fx_0fb_0 | 0x6 | `FX_MADEEN_ROCKS` | render | emitter: teal-green rock debris, four elements |
| fx_0fb_0 | 0x8 | `FX_MADEEN_BLUE_RING` | render | emitter: white-blue gouraud ring |
| fx_0fb_0 | 0x9 | `FX_MADEEN_VIOLET_TRAIL` | render | trail: violet speckled strip |
| fx_0fb_0 | 0xA | `FX_MADEEN_BLUE_RINGS` | render | rings: white-blue gouraud rings, 16 frames |
| fx_0fb_0 | 0xB | `FX_MADEEN_DUST_CLOUD` | render | emitter: tan dust puffs, 27 animated frames |
| fx_0fb_0 | 0xC | `FX_MADEEN_ROCK_SPIRAL` | render | emitter: teal-green rock debris rising in a spiral |
| fx_0fb_0 | 0xD | `FX_MADEEN_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_0fb_0 | 0xE | `FX_MADEEN_BACKDROP_2` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_0fb_0 | 0xF | `FX_MADEEN_BLUE_RING_2` | render | emitter: white-blue gouraud ring |
| fx_0fb_0 | 0x10 | `FX_MADEEN_BLUE_GLOW` | render | particles: blue glowing dot (byte-identical to fx_0fb_1 entry 0xa) |
| fx_0fb_0 | 0x14 | `FX_MADEEN_WHITE_RINGS` | render | rings: white-blue gouraud rings, five rings |
| fx_0fb_0 | 0x15 | `FX_MADEEN_LIGHT_STREAKS` | render (fxdata d6121033) | emitter: two fans, thin orange and violet light streaks |
| fx_0fb_0 | 0x82 | `FX_MADEEN_PINK_CLOUD_BALL` | render (fxdata d6121033) | summon data entry 2: textured model, ball with a dark pink cloud texture |
| fx_0fb_0 | 0x84 | `FX_MADEEN_CLOUD_RING` | render (fxdata d6121033) | summon data entry 4: textured model, ring band with a white cloud texture |
| fx_0fb_0 | 0x87 | `FX_MADEEN_LEAF_MOUND` | render (fxdata d6121033) | summon data entry 7: textured model, low mound with a green foliage texture |
| fx_0fb_0 | 0x94 | `FX_MADEEN_RAINBOW_ARC` | render (fxdata d6121033) | summon data entry 20: textured model, thin curved ribbon with a red-green-blue stripe (rainbow) texture |
| fx_0fb_1 | 0x1 | `FX_MADEEN_CLOUD_STRIP` | render (fxdata d6121033) | sprite: wide grey-white cloud strip drawn scrolling across the screen (byte-identical to fx_0fb_0 entry 0x3) |
| fx_0fb_1 | 0x2 | `FX_MADEEN_BLUE_STREAKS` | render | emitter: orbiting blue light streaks + blue wing-pattern particles |
| fx_0fb_1 | 0x3 | `FX_MADEEN_LOOP_RIBBON` | render (fxdata d6121033) | model: figure-eight ribbon loop (its texture page 0x138 holds a handwritten placeholder in the file) |
| fx_0fb_1 | 0x5 | `FX_MADEEN_CRYSTAL_SPARKLES` | render | emitter: blue-gold crystal sparkles + blue specks |
| fx_0fb_1 | 0x6 | `FX_MADEEN_TEAL_RINGS` | render | rings: navy-teal gouraud rings |
| fx_0fb_1 | 0x7 | `FX_MADEEN_WHITE_GLOW` | render | particles: white-violet glow |
| fx_0fb_1 | 0x8 | `FX_MADEEN_BLUE_RINGS` | render | rings: white-blue gouraud rings |
| fx_0fb_1 | 0xA | `FX_MADEEN_BLUE_GLOW` | render | particles: blue glowing dot (byte-identical to fx_0fb_0 entry 0x10) |
| fx_0fb_1 | 0xC | `FX_MADEEN_LENS_FLARES` | render | sprite line: green / red / orange lens-flare rings |
| fx_0fb_1 | 0xD | `FX_MADEEN_RAINBOW_RINGS` | render | rings: rings textured with a rainbow patch |
| fx_0fb_1 | 0xE | `FX_MADEEN_BLUE_SPECKS` | render | emitter: small blue specks in a spiral |
| fx_0fb_1 | 0xF | `FX_MADEEN_BLUE_RINGS_2` | render | rings: navy-blue gouraud rings |
| fx_0fb_1 | 0x10 | `FX_MADEEN_PALE_RINGS` | render | rings: white / pale-cyan / grey gouraud rings, 8 frames |
| fx_0fb_1 | 0x11 | `FX_MADEEN_BACKDROP_3` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_0fb_1 | 0x12 | `FX_MADEEN_WHITE_RINGS` | render | rings: white to grey fading gouraud rings, 8 frames |
| fx_0fb_1 | 0x13 | `FX_MADEEN_RAINBOW_ARCH` | render | model: arch, rainbow-cloud texture |
| fx_0fb_1 | 0x14 | `FX_MADEEN_TRAIL` | kind/usage (look pending) | trail: strip texture not in the file (magenta in the render) |
| fx_0fb_1 | 0x15 | `FX_MADEEN_VIOLET_GLOW` | render (fxdata d6121033) | emitter: three fans, a horizontal violet-white glow |
| fx_0fb_1 | 0x16 | `FX_MADEEN_BLUE_RINGS_3` | render | rings: white-blue gouraud rings, 16 frames |
| fx_0fb_1 | 0x17 | `FX_MADEEN_LENS_FLARES_2` | render | sprite line: green / red / orange lens-flare rings |
| fx_0fb_1 | 0x18 | `FX_MADEEN_ROCK_BURST` | render | emitter: teal-green rock debris + tan dust puffs |
| fx_0fb_1 | 0x19 | `FX_MADEEN_ROCK_BURST_2` | render | emitter: teal-green rock debris + tan dust puffs |
| fx_0fb_1 | 0x1A | `FX_MADEEN_VIOLET_RINGS` | render | rings: white / violet / indigo gouraud rings |
| fx_0fb_1 | 0x23 | `FX_MADEEN_Y_FRAME` | render | model: three-armed frame, grey streak texture |
| fx_0fb_1 | 0x82 | `FX_MADEEN_PINK_CLOUD_BALL` | render (fxdata d6121033) | summon data entry 2: textured model, ball with a dark pink cloud texture |
| fx_0fb_1 | 0x87 | `FX_MADEEN_LEAF_MOUND` | render (fxdata d6121033) | summon data entry 7: textured model, low mound with a green foliage texture |
| fx_0fb_1 | 0x89 | `FX_MADEEN_RAINBOW_SPHERE` | render (fxdata d6121033) | summon data entry 9: textured model, sphere wrapped in red-green-blue stripe (rainbow) bands |
| fx_0fb_1 | 0x8C | `FX_MADEEN_BLUE_SWIRL_DISC` | render (fxdata d6121033) | summon data entry 12: textured model, disc with a blue swirl texture |
| fx_0fb_1 | 0x8F | `FX_MADEEN_RAINBOW_SPHERE_2` | render (fxdata d6121033) | summon data entry 15: textured model, sphere wrapped in red-green-blue stripe (rainbow) bands |
| fx_0fb_1 | 0x90 | `FX_MADEEN_RAINBOW_BANDS` | render (fxdata d6121033) | summon data entry 16: textured model, curved red-green-blue stripe (rainbow) bands |
| fx_0fe | 0x4 | `FX_EAT_WHITE_RINGS` | render | emitter: white-lilac and white-cyan gouraud rings in a cone + a fan element (byte-identical to fx_0ff entry 0x4) |
| fx_0ff | 0x4 | `FX_COOK_WHITE_RINGS` | render | emitter: white-lilac and white-cyan gouraud rings in a cone + a fan element (byte-identical to fx_0fe entry 0x4) |
| fx_0ff | 0x8 | `FX_COOK_FLAMES` | render | emitter: flame tongue particles + white-orange gouraud ring |
| fx_100 | 0x1 | `FX_SOLUTION_DUST_PUFF` | render | emitter: olive-tan dust puff particles |
| fx_100 | 0x2 | `FX_SOLUTION_SPIRAL_TRAIL` | render | emitter: 3D trail spiralling in a tube, grey jagged strip |
| fx_100 | 0x3 | `FX_SOLUTION_OLIVE_RINGS` | render | emitter: olive-grey gouraud rings, 8 frames |
| fx_100 | 0x4 | `FX_SOLUTION_DUST_WISPS` | render | emitter: brown dust wisps in a cone |
| fx_100 | 0x5 | `FX_SOLUTION_PINK_STREAK_ORBIT` | render (fxdata d6121033) | emitter: orbiting fan of a pink streak with red dots, white dot specks |
| fx_100 | 0x6 | `FX_SOLUTION_SPECK_TRAIL` | render (fxdata d6121033) | emitter: 3D trail (white blob strip) with white dot specks |
| fx_100 | 0x7 | `FX_SOLUTION_VIOLET_SMOKE` | render | emitter: violet smoke clouds |
| fx_100 | 0x8 | `FX_SOLUTION_VIOLET_RINGS` | render | emitter: violet gouraud rings, 24 frames |
| fx_101 | 0x1 | `FX_TRANCE_BLUE_RING` | render | rings: navy-white gouraud rings |
| fx_101 | 0x3 | `FX_TRANCE_AURA_CYLINDER` | render | model: open cylinder, blue-green cloud texture |
| fx_101 | 0x5 | `FX_TRANCE_THIN_STREAKS` | render (fxdata d6121033) | emitter: two fans, thin orange and blue streaks |
| fx_101 | 0x8 | `FX_TRANCE_VIOLET_TRAIL` | render | trail: violet sparkle strip, 12 frames |
| fx_101 | 0x9 | `FX_TRANCE_LILAC_RINGS` | render | rings: white / lilac / blue gouraud rings |
| fx_101 | 0xA | `FX_TRANCE_WHITE_RINGS` | render | rings: white-blue gouraud rings |
| fx_101 | 0xC | `FX_TRANCE_SPARKLE_SPIRAL` | render | emitter: small blue-gold sparkles rising in a spiral (first element renders black) |
| fx_101 | 0xF | `FX_TRANCE_PINK_STREAKS` | render (fxdata d6121033) | emitter: two orbiting fans, white-pink light streaks |
| fx_101 | 0x10 | `FX_TRANCE_VIOLET_STREAKS` | render (fxdata d6121033) | emitter: two orbiting fans, violet-blue light streaks |
| fx_102 | 0x9 | `FX_RISE_RUNE_CIRCLE` | render | emitter: magic circle of runes cycling violet to yellow + white gouraud rings |
| fx_102 | 0xA | `FX_RISE_GLOW_RING` | render | emitter: glowing ring disc cycling violet to orange |
| fx_103 | 0x2 | `FX_MIST_SWIRL` | render | emitter: blue-grey mist puffs orbiting |
| fx_103 | 0x3 | `FX_MIST_CLOUD` | render | emitter: blue-grey mist puffs |
| fx_104 | 0x1 | `FX_EVERYONES_GRUDGE_PINK_RINGS` | render | emitter: pink gouraud rings + pink glow-ring particles + white-yellow rings |
| fx_104 | 0x3 | `FX_EVERYONES_GRUDGE_SWIRL_TRAIL` | render | emitter: flat trail spiralling, white swirl strip |
| fx_104 | 0x4 | `FX_EVERYONES_GRUDGE_SMOKE` | render | emitter: grey smoke wisps |
| fx_104 | 0x5 | `FX_EVERYONES_GRUDGE_MAGIC_CIRCLE` | render | emitter: mandala magic circle cycling green to blue |
| fx_105 | 0x1 | `FX_ODIN_SHOT2_CAMERA_KEYS_3` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0xFB into env->camera[2]) |
| fx_105 | 0x2 | `FX_ODIN_SHOT0_CAMERA_KEYS_1` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0x0 into env->camera[0]) |
| fx_105 | 0x3 | `FX_ODIN_SPECK_SPIRAL` | render (fxdata d6121033) | emitter: white dot specks rising in a spiral |
| fx_105 | 0x4 | `FX_ODIN_PALE_CLOUD` | render | emitter: pale blue cloud puffs spiralling along an axis (later frames: texture not in the file) |
| fx_105 | 0x5 | `FX_ODIN_SHOT0_CAMERA_KEYS_3` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0x0 into env->camera[2]) |
| fx_105 | 0x6 | `FX_ODIN_SHOT0_CAMERA_KEYS_4` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0x0 into env->camera[3]) |
| fx_105 | 0x7 | `FX_ODIN_SHOT0_CAMERA_KEYS_9` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0x0 into env->camera[8]) |
| fx_105 | 0x8 | `FX_ODIN_SHOT0_CAMERA_KEYS_2` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0x0 into env->camera[1]) |
| fx_105 | 0x9 | `FX_ODIN_SHOT1_CAMERA_KEYS_1` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0xA7 into env->camera[0]) |
| fx_105 | 0xA | `FX_ODIN_SHOT1_CAMERA_KEYS_3` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0xA7 into env->camera[2]) |
| fx_105 | 0xB | `FX_ODIN_PALE_CLOUD_2` | render | emitter: pale blue cloud puffs spiralling (later frames: texture not in the file) |
| fx_105 | 0xD | `FX_ODIN_STONE_RIBBON` | render (fxdata d6121033) | 'so' model: wavy strip with a dark green stone texture (model138[0]) |
| fx_105 | 0xE | `FX_ODIN_WHITE_DEBRIS` | render | emitter: white crumbling debris + specks |
| fx_105 | 0x10 | `FX_ODIN_STAR` | render (fxdata d6121033) | sprite: small dark-grey speck, drawn as the star field (fx_105_stars) |
| fx_105 | 0x11 | `FX_ODIN_MOON` | render | emitter: pale blue full moon sprite (+ light beam) |
| fx_105 | 0x14 | `FX_ODIN_RIBBON_PATH_4` | kind/usage | table: world-space ribbon path (fx_105_drawWorldRibbon) |
| fx_105 | 0x15 | `FX_ODIN_STONE_RIBBON_2` | render (fxdata d6121033) | 'so' model: thin wavy strip with a dark green stone texture (model138[1]) |
| fx_105 | 0x16 | `FX_ODIN_SHOT1_CAMERA_KEYS_2` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0xA7 into env->camera[1]) |
| fx_105 | 0x17 | `FX_ODIN_SHOT2_CAMERA_KEYS_1` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0xFB into env->camera[0]) |
| fx_105 | 0x18 | `FX_ODIN_CAM_RIBBON_PATH` | kind/usage | table: camera-space ribbon path (fx_105_drawCamRibbon) |
| fx_105 | 0x19 | `FX_ODIN_SHOT2_CAMERA_KEYS_2` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0xFB into env->camera[1]) |
| fx_105 | 0x1A | `FX_ODIN_SHOT3_CAMERA_KEYS_1` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0x137 into env->camera[0]) |
| fx_105 | 0x1B | `FX_ODIN_SHOT3_CAMERA_KEYS_3` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0x137 into env->camera[2]) |
| fx_105 | 0x1C | `FX_ODIN_SHOT3_CAMERA_KEYS_2` | kind/usage | table: camera key path (fxm_camKeyAdd from frame 0x137 into env->camera[1]) |
| fx_105 | 0x1D | `FX_ODIN_DEBRIS_RINGS` | render | emitter: grey-green gouraud rings + pale debris chips + specks |
| fx_105 | 0x1E | `FX_ODIN_DEBRIS_CONE` | render | emitter: pale debris chips in a cone + specks |
| fx_105 | 0x1F | `FX_ODIN_DEBRIS_SPIRAL` | render | emitter: white crumbling debris in a spiral + specks |
| fx_105 | 0x20 | `FX_ODIN_RIBBON_PATH` | kind/usage | table: world-space ribbon path (fx_105_drawWorldRibbon) |
| fx_105 | 0x22 | `FX_ODIN_RIBBON_PATH_2` | kind/usage | table: world-space ribbon path (fx_105_drawWorldRibbon) |
| fx_105 | 0x23 | `FX_ODIN_RIBBON_PATH_3` | kind/usage | table: world-space ribbon path (fx_105_drawWorldRibbon) |
| fx_105 | 0x25 | `FX_ODIN_RIBBON_PATH_5` | kind/usage | table: world-space ribbon path (fx_105_drawWorldRibbon) |
| fx_105 | 0x28 | `FX_ODIN_DEBRIS_BURST` | render | emitter: pale debris burst + specks |
| fx_105 | 0x29 | `FX_ODIN_STONE_RIBBON_3` | render (fxdata d6121033) | 'so' model: thin wavy strip with a dark green stone texture (model138[2]) |
| fx_105 | 0x2A | `FX_ODIN_STONE_RIBBON_4` | render (fxdata d6121033) | 'so' model: thin wavy strip with a dark green stone texture (model138[3]) |
| fx_105 | 0x2B | `FX_ODIN_STONE_RIBBON_5` | render (fxdata d6121033) | 'so' model: thin wavy strip with a dark green stone texture (model138[4]) |
| fx_105 | 0x2C | `FX_ODIN_STONE_RIBBON_6` | render (fxdata d6121033) | 'so' model: thin wavy strip with a dark green stone texture (model138[5]) |
| fx_105 | 0x2D | `FX_ODIN_STONE_RIBBON_7` | render (fxdata d6121033) | 'so' model: thin wavy strip with a dark green stone texture (model138[6]) |
| fx_105 | 0x2E | `FX_ODIN_STONE_RIBBON_8` | render (fxdata d6121033) | 'so' model: thin wavy strip with a dark green stone texture (model138[7]) |
| fx_105 | 0x3B | `FX_ODIN_SMOKE_SPIRAL` | render | emitter: grey smoke wisps in a spiral + specks |
| fx_105 | 0x80 | `FX_ODIN_WHITE_FOLD_0` | render (fxdata d6121033) | summon data entry 0: 'so' model, folded white sheet |
| fx_105 | 0x81 | `FX_ODIN_WHITE_FOLD_1` | render (fxdata d6121033) | summon data entry 1: 'so' model, folded white sheet |
| fx_105 | 0x82 | `FX_ODIN_WHITE_FOLD_2` | render (fxdata d6121033) | summon data entry 2: 'so' model, folded white sheet |
| fx_105 | 0x83 | `FX_ODIN_WHITE_FOLD_3` | render (fxdata d6121033) | summon data entry 3: 'so' model, folded white sheet |
| fx_106 | 0x1 | `FX_MUCUS_SLIME_BLOBS` | render | emitter: yellow slime blobs |
| fx_106 | 0x2 | `FX_MUCUS_BUBBLE_TRAIL` | render | emitter: violet bubble screen trail + speck sprites (texture not in the file) |
| fx_106 | 0x3 | `FX_MUCUS_BUBBLE_TRAIL_2` | render | emitter: violet bubble screen trail + speck sprites (texture not in the file) |
| fx_106 | 0x4 | `FX_MUCUS_YELLOW_RINGS` | render | emitter: yellow-green gouraud rings |
| fx_106 | 0x5 | `FX_MUCUS_SPLASH_STREAK` | render | emitter: white streaky splash screen trail |
| fx_109 | 0x1 | `FX_THROW_RACKET` | render (fxdata d6121033) | 'so' model: the thrown racket, drawn on its path (the same model as entry 0xC, 'so' form) |
| fx_109 | 0x5 | `FX_THROW_IMPACT` | render | emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) |
| fx_109 | 0xC | `FX_THROW_RACKET_AFTERIMAGE` | render | model: racket (76 polys, weapon texture), drawn as fading afterimages along the path |
| fx_10a | 0x1 | `FX_THROW_THIEF_SWORD` | render (fxdata d6121033) | 'so' model: the thrown thief sword, drawn on its path (the same model as entry 0xC, 'so' form) |
| fx_10a | 0x5 | `FX_THROW_IMPACT` | render | emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) |
| fx_10a | 0xC | `FX_THROW_THIEF_SWORD_AFTERIMAGE` | render | model: thief sword (60 polys, weapon texture), drawn as fading afterimages along the path |
| fx_10b | 0x4 | `FX_THROW_IMPACT` | render | emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) |
| fx_10b | 0x5 | `FX_THROW_SWORD` | render (fxdata d6121033) | 'so' model: the thrown sword, drawn on its path (the same model as entry 0xC, 'so' form) |
| fx_10b | 0xC | `FX_THROW_SWORD_AFTERIMAGE` | render | model: sword (63 polys, weapon texture), drawn as fading afterimages along the path |
| fx_10c | 0x4 | `FX_THROW_IMPACT` | render | emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) |
| fx_10c | 0x5 | `FX_THROW_SPEAR` | render (fxdata d6121033) | 'so' model: the thrown spear, drawn on its path (the same model as entry 0xC, 'so' form) |
| fx_10c | 0xC | `FX_THROW_SPEAR_AFTERIMAGE` | render | model: spear (67 polys, weapon texture), drawn as fading afterimages along the path |
| fx_10d | 0x1 | `FX_THROW_CLAWS` | render (fxdata d6121033) | 'so' model: the thrown claws, drawn on its path (the same model as entry 0xC, 'so' form) |
| fx_10d | 0x5 | `FX_THROW_IMPACT` | render | emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) |
| fx_10d | 0xC | `FX_THROW_CLAWS_AFTERIMAGE` | render | model: claws (42 polys, weapon texture), drawn as fading afterimages along the path |
| fx_10e | 0x4 | `FX_THROW_ROD` | render (fxdata d6121033) | 'so' model: the thrown rod / flute / staff, drawn on its path (the same model as entry 0xC, 'so' form) |
| fx_10e | 0x5 | `FX_THROW_IMPACT` | render | emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) |
| fx_10e | 0xC | `FX_THROW_ROD_AFTERIMAGE` | render | model: rod / flute / staff (54 polys, weapon texture), drawn as fading afterimages along the path |
| fx_10f | 0x1 | `FX_THROW_IMPACT` | render | emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) |
| fx_10f | 0x2 | `FX_THROW_FORK` | render (fxdata d6121033) | 'so' model: the thrown fork, drawn on its path (the same model as entry 0xC, 'so' form) |
| fx_10f | 0xC | `FX_THROW_FORK_AFTERIMAGE` | render | model: fork (90 polys, weapon texture), drawn as fading afterimages along the path |
| fx_110 | 0x5 | `FX_THROW_IMPACT` | render | emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) |
| fx_110 | 0x6 | `FX_THROW_DAGGER` | render (fxdata d6121033) | 'so' model: the thrown dagger, drawn on its path (the same model as entry 0xC, 'so' form) |
| fx_110 | 0xC | `FX_THROW_DAGGER_AFTERIMAGE` | render | model: dagger (48 polys, weapon texture), drawn as fading afterimages along the path |
| fx_112 | 0x1 | `FX_EAT_SPHERE` | render | model: sphere, white texture |
| fx_112 | 0x2 | `FX_EAT_ORBIT_TRAIL` | kind/usage (look pending) | emitter: orbiting screen trail; the strip is still mostly dark in the render |
| fx_112 | 0x3 | `FX_EAT_SMOKE` | render | emitter: grey smoke clouds, 24 animated frames |
| fx_112 | 0x4 | `FX_EAT_GOLD_CUTLERY` | render | emitter: golden fork, spoon, cup and spinning gold coins |
| fx_113 | 0x1 | `FX_VIRUS_COMBO_STREAK_TRAIL` | render | emitter: screen trail spiralling in a cone, grey streak strip |
| fx_113 | 0x2 | `FX_VIRUS_COMBO_BURST` | render | emitter: pink-yellow and white-red gouraud rings + speck sprites (texture not in the file) |
| fx_113 | 0x3 | `FX_VIRUS_COMBO_BURST_2` | render | emitter: pink-yellow and white-red gouraud rings + speck sprites (texture not in the file) |
| fx_113 | 0x4 | `FX_VIRUS_COMBO_BURST_3` | render | emitter: pink-yellow and white-red gouraud rings + speck sprites (texture not in the file) |
| fx_113 | 0x5 | `FX_VIRUS_COMBO_PUFF` | render | emitter: pink-white puffs fading in a cone |
| fx_114 | 0x3 | `FX_IFRIT_EMBERS` | render | emitter: tiny ember specks orbiting |
| fx_114 | 0x5 | `FX_IFRIT_FIRE_RING` | render | emitter: orange-red to white gouraud ring |
| fx_114 | 0x6 | `FX_IFRIT_BACKDROP` | kind/usage | table: backdrop layers (btlfx_drawBackdrop) |
| fx_114 | 0x7 | `FX_IFRIT_EMBER_TRAIL` | render | emitter: orbiting screen trail (red strip) + ember sprites |
| fx_114 | 0x8 | `FX_IFRIT_ROCK_RINGS` | render | emitter: rings textured with a golden rock pattern, 16 frames |
| fx_114 | 0x9 | `FX_IFRIT_FLAME_LINE` | render (fxdata d6121033) | emitter: two orbiting fans, a thin horizontal orange flame line |
| fx_114 | 0xB | `FX_IFRIT_ORANGE_RINGS` | render | emitter: orange gouraud rings, 16 frames |
| fx_114 | 0x12 | `FX_IFRIT_SPOKED_RING` | kind/usage (look pending) | model: flat ring with spokes (registered through the texture list; the tool does not follow it, so the texture is not drawn) |
| fx_114 | 0x15 | `FX_IFRIT_FIRE_TRAIL` | render | trail: orange strip, drawn along the paths of entry 0x17 |
| fx_114 | 0x16 | `FX_IFRIT_ORANGE_RING_2` | render | emitter: orange to white gouraud ring |
| fx_114 | 0x17 | `FX_IFRIT_TRAIL_PATHS` | kind/usage | table: offset table of position paths (fxm_offsetTableEntry + fxm_posPathInit) for the fire trails |
| fx_114 | 0x19 | `FX_IFRIT_FLAME_SWIRL` | render | emitter: flat trail, flame swirl strip (byte-identical to entries 0x1A-0x1C) |
| fx_114 | 0x1A | `FX_IFRIT_FLAME_SWIRL_2` | render | emitter: flat trail, flame swirl strip (byte-identical to entry 0x19) |
| fx_114 | 0x1B | `FX_IFRIT_FLAME_SWIRL_3` | render | emitter: flat trail, flame swirl strip (byte-identical to entry 0x19) |
| fx_114 | 0x1C | `FX_IFRIT_FLAME_SWIRL_4` | render | emitter: flat trail, flame swirl strip (byte-identical to entry 0x19) |
| fx_114 | 0x1D | `FX_IFRIT_VEC_TRACK` | kind/usage | table: vector track (fxm_vecTrackInit) |
| fx_114 | 0x1E | `FX_IFRIT_BOWL` | kind/usage (look pending) | model: hemisphere bowl (registered through the texture list; the tool does not follow it, so the texture is not drawn) |
| fx_114 | 0x20 | `FX_IFRIT_BLOCK_CLUSTER` | render | model: cluster of block chunks, orange streak texture |
| fx_114 | 0x21 | `FX_IFRIT_FIRE_BURST` | render | emitter: ember specks + orange fire-burst clouds |
| fx_114 | 0x22 | `FX_IFRIT_FLAME_RING` | render | model: flat ring, flame texture |
| fx_114 | 0x23 | `FX_IFRIT_POS_TRACK` | kind/usage | table: position track (fxm_posTrackInit) |
| fx_114 | 0x24 | `FX_IFRIT_VEC_TRACK_2` | kind/usage | table: vector track (fxm_vecTrackInit) |
| fx_114 | 0x25 | `FX_IFRIT_VEC_TRACK_3` | kind/usage | table: vector track (fxm_vecTrackInit) |
| fx_114 | 0x26 | `FX_IFRIT_RED_GLOW` | render (fxdata d6121033) | emitter: orbiting fan, a horizontal orange-red glow |
| fx_114 | 0x27 | `FX_IFRIT_FLAME_DOME` | render | model: dome, flame texture |
| fx_114 | 0x28 | `FX_IFRIT_PIECE_MODELS` | kind/usage | table: offset table of model data, one per piece (fxm_offsetTableEntry) |
| fx_114 | 0x29 | `FX_IFRIT_PIECE_TRACKS` | kind/usage | table: offset table of tracks, one per piece (fxm_offsetTableEntry) |
| fx_114 | 0x2A | `FX_IFRIT_GRID_PLANE` | kind/usage (look pending) | model: flat grid plane (tpage 0x117 reads a yellow placeholder page in the render) |
| fx_114 | 0x2B | `FX_IFRIT_POS_PATH` | kind/usage | table: position path (fxm_posPathInit) |
| fx_114 | 0x2C | `FX_IFRIT_POS_PATH_2` | kind/usage | table: position path (fxm_posPathInit) |
| fx_114 | 0x80 | `FX_IFRIT_FIREBALL` | render (fxdata d6121033) | summon data entry 0: 'so' model, ball with a fire texture and a flame crown |
| fx_114 | 0x81 | `FX_IFRIT_CANOPY` | kind/usage (look pending) | summon data entry 1: textured model, canopy on legs (registered through the texture list; the tool does not follow it, so the texture is not drawn) |
| fx_114 | 0x82 | `FX_IFRIT_FIRE_COLUMN` | render (fxdata d6121033) | summon data entry 2: textured model, cylinder with a fire texture |
| fx_114 | 0x83 | `FX_IFRIT_CYLINDER` | kind/usage (look pending) | summon data entry 3: textured model, cylinder on tpage 0x117 (a yellow placeholder page in the render) |
| fx_114 | 0x84 | `FX_IFRIT_SUMMON_VEC_TRACK_4` | kind/usage | summon data entry 4: vector track (fxm_vecTrackInit) |
| fx_114 | 0x85 | `FX_IFRIT_GOURAUD_BOWL` | render (fxdata d6121033) | summon data entry 5: gouraud model, grey-white gouraud bowl |
| fx_114 | 0x86 | `FX_IFRIT_SUMMON_VEC_TRACK_6` | kind/usage | summon data entry 6: vector track (fxm_vecTrackInit) |
| fx_114 | 0x87 | `FX_IFRIT_SUMMON_VEC_TRACK_7` | kind/usage | summon data entry 7: vector track (fxm_vecTrackInit) |
| fx_114 | 0x88 | `FX_IFRIT_SUMMON_VEC_TRACK_8` | kind/usage | summon data entry 8: vector track (fxm_vecTrackInit) |
| fx_114 | 0x89 | `FX_IFRIT_SUMMON_VEC_TRACK_9` | kind/usage | summon data entry 9: vector track (fxm_vecTrackInit) |
| fx_114 | 0x8A | `FX_IFRIT_SUMMON_POS_PATH_10` | kind/usage | summon data entry 10: position path (fxm_posPathInit) |
| fx_114 | 0x8D | `FX_IFRIT_SUMMON_POS_TRACK_13` | kind/usage | summon data entry 13: position track (fxm_posTrackInit) |
| fx_114 | 0x8E | `FX_IFRIT_SUMMON_POS_TRACK_14` | kind/usage | summon data entry 14: position track (fxm_posTrackInit) |
| fx_114 | 0x8F | `FX_IFRIT_SUMMON_POS_TRACK_15` | kind/usage | summon data entry 15: position track (fxm_posTrackInit) |
| fx_114 | 0x90 | `FX_IFRIT_SUMMON_POS_TRACK_16` | kind/usage | summon data entry 16: position track (fxm_posTrackInit) |
| fx_114 | 0x91 | `FX_IFRIT_SUMMON_POS_TRACK_17` | kind/usage | summon data entry 17: position track (fxm_posTrackInit) |
| fx_114 | 0x92 | `FX_IFRIT_SUMMON_POS_TRACK_18` | kind/usage | summon data entry 18: position track (fxm_posTrackInit) |
| fx_114 | 0x94 | `FX_IFRIT_SUMMON_POS_PATH_20` | kind/usage | summon data entry 20: position path (fxm_posPathInit) |
| fx_114 | 0x95 | `FX_IFRIT_SUMMON_VEC_PATH_21` | kind/usage | summon data entry 21: vector path (fxm_vecPathInit) |
| fx_114 | 0x96 | `FX_IFRIT_SUMMON_POS_PATH_22` | kind/usage | summon data entry 22: position path (fxm_posPathInit) |
| fx_115 | 0x4 | `FX_THROW_PINWHEEL_IMPACT` | render | emitter: white-lilac gouraud ring + sprites (render black) at the hit |
| fx_115 | 0x5 | `FX_THROW_PINWHEEL` | render (fxdata d6121033) | 'so' model: the thrown pinwheel / rising sun (a ring blade), registered twice |
| fx_116 | 0x1 | `FX_ETHER_SPARKLES` | render | emitter: red and violet sparkles orbiting, white-cyan gouraud ring, red glow |
| fx_117 | 0x4 | `FX_ECHO_SCREEN_SPARKLES` | render | emitter, one per file: Echo Screen (0x117) blue-white puff, violet specks, white-cyan gouraud ring, glow; Dead Pepper (0x120) puff, violet and red sparkles, glow; Gysahl Greens (0x121) green / violet / gold sparkles, ring, glow, leaf sprite |
| fx_118 | 0x4 | `FX_EYE_DROPS_SPARKLES` | render | emitter: white-lilac gouraud rings, pale sparkles, blue and green glows |
| fx_119 | 0x4 | `FX_MAGIC_TAG_GLOW` | render | emitter: blue-white glow sprite + a fan element |
| fx_11a | 0x1 | `FX_VACCINE_GLOW` | render | emitter: white-lilac gouraud ring, green glow sprite + a fan element |
| fx_11b | 0x1 | `FX_ANNOYNTMENT_SPARKLES` | render | emitter: blue-white puff, pale sparkles, white-cyan gouraud ring, green glow |
| fx_11c | 0x2 | `FX_HIGH_WIND_WIND_TRAIL` | render | emitter: 3D trail spiralling up, grey wind-swirl strip |
| fx_11c | 0x3 | `FX_HIGH_WIND_TEAL_SWIRL` | render | emitter: teal swirl and crescent particles |
| fx_11c | 0x6 | `FX_HIGH_WIND_WIND_TRAIL_2` | render | emitter: 3D trail spiralling up, grey wind-swirl strip |
| fx_11c | 0x9 | `FX_HIGH_WIND_CLOUD` | render | emitter: teal-white cloud puffs spiralling, 22 animated frames |
| fx_11c | 0xA | `FX_HIGH_WIND_TEAL_RINGS` | render | emitter: teal gouraud rings, 32 frames |
| fx_11c | 0xB | `FX_HIGH_WIND_WIND_TRAIL_3` | render | emitter: 3D trail spiralling up, grey wind-swirl strip |
| fx_11c | 0xC | `FX_HIGH_WIND_WIND_STREAKS` | render | emitter: teal jagged wind-streak particles spiralling |
| fx_11c | 0xD | `FX_HIGH_WIND_CLOUD_2` | render | emitter: teal-white cloud puffs spiralling, 22 animated frames |
| fx_11c | 0xE | `FX_HIGH_WIND_GREY_SWIRL` | render | emitter: grey swirl particle |
| fx_11d | 0x1 | `FX_ABSORB_VIOLET_BLOBS` | render | emitter: violet blob clouds, 19 animated frames |
| fx_11d | 0x3 | `FX_ABSORB_TEAL_RING` | render | emitter: teal-white gouraud ring |
| fx_11d | 0x9 | `FX_ABSORB_TRAIL` | kind/usage (look pending) | emitter: 3D trail; the render is still black (byte-identical to entry 0xA) |
| fx_11d | 0xA | `FX_ABSORB_TRAIL_2` | kind/usage (look pending) | emitter: 3D trail; the render is still black (byte-identical to entry 0x9) |
| fx_11d | 0xB | `FX_ABSORB_RED_GLOW` | render | emitter: red glow orb with a white / cyan core, 11 frames (byte-identical to entry 0xC) |
| fx_11d | 0xC | `FX_ABSORB_RED_GLOW_2` | render | emitter: red glow orb with a white / cyan core, 11 frames (byte-identical to entry 0xB) |
| fx_11d | 0xD | `FX_ABSORB_LENS_FLARE` | render | emitter: rainbow-rimmed lens flare sprites spiralling + a 3D trail (renders black) |
| fx_11e | 0x1 | `FX_VIRUS_FLY` | render (fxdata d6121033) | 'so' model: brown fly (insect) |
| fx_11e | 0x2 | `FX_VIRUS_FLY_2` | render (fxdata d6121033) | 'so' model: brown fly (insect), wings in another pose (same size as entry 0x1) |
| fx_11e | 0x3 | `FX_VIRUS_FLY_FLIES` | render | emitter: homing animated fly sprites flapping their wings, 64 frames |
| fx_11f | 0x1 | `FX_DARK_MATTER_YELLOW_RINGS` | render | emitter: yellow and pink gouraud rings + a fan element |
| fx_11f | 0x2 | `FX_DARK_MATTER_VIOLET_RINGS` | render | emitter: blue and violet gouraud rings |
| fx_11f | 0x3 | `FX_DARK_MATTER_BOLT_TRAIL` | render | emitter: 3D trail with a violet lightning strip (+ a sprite, render black) |
| fx_11f | 0x4 | `FX_DARK_MATTER_BOLT_RINGS` | render | emitter: yellow-white gouraud rings, rings textured with violet lightning, pink rings |
| fx_122 | 0x1 | `FX_TENT_CANVAS_TENT` | render (fxdata d6121033) | 'so' model: yellow-green striped canvas tent |
| fx_122 | 0x5 | `FX_TENT_SPARKLES` | render | emitter: green and violet sparkles orbiting, white-cyan gouraud ring, glow |
| fx_123 | 0x4 | `FX_RAINING_SWORDS_SPECKS` | render (fxdata d6121033) | emitter: cyan dot specks |
| fx_123 | 0x5 | `FX_RAINING_SWORDS_SWORD_RING` | render | model: six swords in a star ring (weapon texture) |
| fx_123 | 0x7 | `FX_RAINING_SWORDS_SWORD` | render | model: single sword (weapon texture) |
| fx_123 | 0x8 | `FX_RAINING_SWORDS_CYAN_FLAMES` | render | emitter: cyan spirit flames + speck sprites |
| fx_123 | 0x9 | `FX_RAINING_SWORDS_CYAN_FLAMES_2` | render | emitter: cyan spirit flame burst + speck sprites |
| fx_123 | 0xA | `FX_RAINING_SWORDS_FLAME_SPIRAL` | render | emitter: cyan spirit flames spiralling + rings |
| fx_123 | 0xB | `FX_RAINING_SWORDS_CYAN_FLAMES_3` | render | emitter: cyan spirit flames + speck sprites |
| fx_123 | 0xC | `FX_RAINING_SWORDS_DARK_BURST` | kind/usage (look pending) | emitter: rings and sprites that still render black, + cyan dot specks |
| fx_123 | 0xD | `FX_RAINING_SWORDS_CYAN_RINGS` | render | emitter: cyan to white gouraud rings, 16 frames |
| fx_123 | 0xE | `FX_RAINING_SWORDS_TEAL_RINGS` | render | emitter: teal gouraud rings + a fan element |
| fx_123 | 0xF | `FX_RAINING_SWORDS_CYAN_RINGS_2` | render | emitter: cyan and teal gouraud rings, 8 frames |
| fx_123 | 0x20 | `FX_RAINING_SWORDS_BLUE_RINGS` | render | emitter: navy to blue gouraud rings, 24 frames |
| fx_124 | 0x1 | `FX_AMETHYST_YELLOW_RINGS` | render | emitter: white-yellow to navy gouraud rings spiralling + sprites (render black); the same file plays every gem item |
| fx_125 | 0x1 | `FX_POLY_DOTS` | render | emitter: tiny yellow / violet dots |
| fx_125 | 0x2 | `FX_POLY_DOTS_2` | render | emitter: tiny yellow / green / blue dots |
| fx_125 | 0x3 | `FX_POLY_GREEN_SPECKS` | render | emitter: small green and cyan bow-tie specks |
| fx_125 | 0x4 | `FX_POLY_COLOR_SPIKES` | render (fxdata d6121033) | emitter: two fans of blue and green light spikes, multicoloured dot sparkles |
| fx_125 | 0x5 | `FX_POLY_STAR_BURST` | render | emitter: orange-to-green glow, white-green gouraud rings, violet star sprites |
| fx_125 | 0x6 | `FX_POLY_STAR_GLOWS` | render | emitter: orange-to-green glows and violet star sprites |
| fx_125 | 0x7 | `FX_POLY_GREEN_SPECKS_2` | render | emitter: small green and cyan bow-tie specks |
| fx_125 | 0x8 | `FX_POLY_CYAN_RINGS` | render | emitter: white-cyan gouraud rings, 24 frames |
| fx_125 | 0xD | `FX_POLY_POLYHEDRON` | kind/usage (look pending) | model: faceted polyhedron, texture not in the file (magenta in the render) (byte-identical to entry 0xE) |
| fx_125 | 0xE | `FX_POLY_POLYHEDRON_2` | kind/usage (look pending) | model: faceted polyhedron, registered with a tpage the C computes (not resolved by the tool) (byte-identical to entry 0xD) |
| fx_126 | 0x1 | `FX_BOOMERANG_CRESCENTS` | render | emitter: orange, white and blue crescent arcs + debris chips and specks |
| fx_126 | 0x2 | `FX_BOOMERANG_SWIRL` | render | emitter: orange and teal spiral swirls + yellow and pink gouraud rings |
| fx_126 | 0x7 | `FX_BOOMERANG_CURVED_BLADE` | render (fxdata d6121033) | 'so' model: gold curved boomerang blade, registered twice |
| fx_127 | 0x1 | `FX_WAVE_WHITE_RINGS` | render | emitter: white gouraud rings in a tube, 28 frames |
| fx_127 | 0x2 | `FX_WAVE_WHITE_RINGS_2` | render | emitter: white gouraud rings in a cylinder, 28 frames |
| fx_127 | 0x3 | `FX_WAVE_WATER_STREAK` | render (fxdata d6121033) | emitter: fan along an axis, a thin white-blue water streak |
| fx_127 | 0x4 | `FX_WAVE_WATER_STREAK_2` | render (fxdata d6121033) | emitter: fan along an axis, a thin white-blue water streak |
| fx_127 | 0x5 | `FX_WAVE_STRIPED_RINGS` | render | emitter: white and dark striped gouraud rings |
| fx_127 | 0x6 | `FX_WAVE_STRIPED_RINGS_2` | render | emitter: white and dark striped gouraud rings, homing |
| fx_127 | 0x7 | `FX_WAVE_WATER_TRAIL` | render | emitter: screen trail, grey streak strip (part of the texture not in the file) |
| fx_127 | 0xA | `FX_WAVE_WHITE_RINGS_3` | render | emitter: white gouraud rings, 38 frames + a fan element |
| fx_127 | 0xD | `FX_WAVE_WHITE_RING` | render | emitter: white gouraud ring |
| fx_127 | 0x12 | `FX_WAVE_WHITE_RINGS_4` | render | emitter: white gouraud rings, 38 frames |
| fx_128 | 0x2 | `FX_DRAGON_BREATH_ROT_POS_PATH` | kind/usage | table: rotation + position path (fxm_rotPosPathInit) |
| fx_128 | 0x3 | `FX_DRAGON_BREATH_VALUE_PATH` | kind/usage | table: value path (fxm_valuePathInit) |
| fx_128 | 0x8 | `FX_DRAGON_BREATH_VIOLET_RINGS` | render | emitter: violet and blue-white gouraud rings |
| fx_128 | 0xB | `FX_DRAGON_BREATH_DRAGON` | render (fxdata d6121033) | 'so' model: teal-green dragon |
| fx_128 | 0x15 | `FX_DRAGON_BREATH_LIGHT_STREAK` | render (fxdata d6121033) | emitter: fan drawn as a thin green to violet horizontal light streak (byte-identical to fx_03d entry 0x10) |
| fx_128 | 0x16 | `FX_DRAGON_BREATH_SPARKLE_CIRCLE` | render | emitter: circle of gold / white sparkle quads + teal specks (byte-identical to fx_03d entry 0x11) |
| fx_128 | 0x17 | `FX_DRAGON_BREATH_BUBBLES` | render | emitter: gold sparkles + teal bubbles |
| fx_128 | 0x18 | `FX_DRAGON_BREATH_BUBBLES_2` | render | emitter: gold sparkles + green bubbles spiralling |
| fx_128 | 0x1F | `FX_DRAGON_BREATH_DRAGON_2` | render (fxdata d6121033) | 'so' model: the second teal-green dragon |
| fx_128 | 0x20 | `FX_DRAGON_BREATH_GREEN_TRAIL` | render | emitter: 3D helix trail, green streak strip |
| fx_128 | 0x21 | `FX_DRAGON_BREATH_ORB` | render | emitter: green-and-orange swirled orb + gold sparkles and specks |
| fx_128 | 0x22 | `FX_DRAGON_BREATH_GREEN_TRAIL_2` | render | emitter: screen helix trail, green streak strip |
| fx_128 | 0x23 | `FX_DRAGON_BREATH_ORB_2` | render | emitter: green-and-orange swirled orb, one frame |
| fx_128 | 0x24 | `FX_DRAGON_BREATH_BUBBLE_RINGS` | render | emitter: green bubbles + specks, white-green gouraud rings |
| fx_128 | 0x25 | `FX_DRAGON_BREATH_POS_PATH` | kind/usage | table: position path (fxm_posPathInit) |
| fx_128 | 0x26 | `FX_DRAGON_BREATH_WHITE_RINGS` | render | emitter: white and pale-cyan gouraud rings, 30 frames |
