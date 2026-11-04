# Battle effect data, slice 5 (fx_129 .. fx_187)

Names for the `btlfx_getFileData` literals of the modules fx_129 .. fx_138, fx_179 .. fx_187 (including the Ark
summon groups fx_17d_0 .. fx_17d_8), by the scheme and tool in `docs/fxdata.md` (`tools/fxdata.py <module>
--render`, one contact-sheet look per module plus the C call order). Evidence: `render` = what the tool draws;
`usage` = what the C does with it (backdrop tables, key and path tables), no look needed; `kind/usage (look
pending)` = appearance not settled, listed under open questions.

Notes:
- Prefixes are the in-game names (`tools/ff9text.py dump abilities` for player abilities: Pumpkin Head, Doomsday,
  Lucky Seven, Medeo, Madeen, Ark, Countdown, Cherry Blossom, Spear, Earth Shake; enemy skills from
  docs/effect-modules.md): fx_12d is the Antlion's enemy Death (`FX_DEATH_`), fx_131 Golem Revival, fx_132
  Appearance, fx_133 Disappear, fx_135 Bad Breath, fx_137 Flame.
- fx_133 also plays file 0x1B5, whose entry 1 is a test emitter (coloured puffs and the letters of "Dummy");
  the name follows file 0x133.
- `tools/fxdata.py` at 8ff097d8 crashes on uses without a `call` (`u.get('call', '')` on a None value); the
  renders here came from a local copy with `(u.get('call') or '')`.

- Twins: fx_182 (Countdown, Elan) shares only the backdrop table byte for byte with fx_181; its two emitters
  (0x3, 0x6) differ in bytes but render the same as fx_181 0x2 / 0x3 and got the same names. fx_185 (Spear,
  2nd) shares the spear model with fx_184 byte for byte; its ring emitter differs and is named by its render.
  fx_183's 'so' model 0x1D has the spear model's size (0xB18) but other bytes.

## Ark: literals still left

`tools/fxdata.py` at d6121033 renders the summon data (indices 0x80+), so the Ark summon-data literals are named
now (evidence `render (fxdata d6121033)`), as are fx_17d_2's entries 1-3. For some groups the tool's slot split
disagreed with the C (entries the C registers as models or draws as trails or rings came out empty). The cause
(fixed in the consistency pass, `docs/fxdata.md`): a group's type 10 chunk goes where the data ring's cursor is,
and when the group's type 2 chunk reached the end of the ring the cursor wrapped, so the type 10 data sits
*below* the group's data and the table indexes it with negative offsets (Ark groups 2, 4, 7 and Ark short group
2). Those entries are named now:

| module | index | name | evidence | what it is |
|---|---|---|---|---|
| fx_17d_2 | 0x5 | `FX_ARK_FLAME_SPIRAL_CONE` | render | 'so' model: deep closed cone with a spiral fire texture |
| fx_17d_2 | 0x7 | `FX_ARK_BLUE_ENERGY_SHELL` | render | 'so' model: sphere band with a blue-red energy texture |
| fx_17d_2 | 0x8 | `FX_ARK_GREEN_GLOW_SPRITE` | render | sprite: small green-blue swirl glow drawn on screen |
| fx_17d_2 | 0x9 | `FX_ARK_STATIC_SPRITE` | render | sprite: grey static-noise tiles drawn on screen |
| fx_17d_2 | 0xA | `FX_ARK_FLAME_SPIRAL_DISH` | render | 'so' model: shallow dish with a spiral fire texture |
| fx_17d_2 | 0xB | `FX_ARK_SMOKE_CONE` | render | 'so' model: deep closed cone with a grey smoke texture |
| fx_17d_4 | 0x1D | `FX_ARK_SMOKE_TORUS` | render | 'so' model: flat torus with a grey smoke texture (type 10 data below the group's slot) |
| fx_17d_4 | 0x1F | `FX_ARK_SMOKE_CYLINDER` | render | 'so' model: open cylinder with a grey smoke texture (type 10 data below the group's slot) |
| fx_17d_4 | 0x20 | `FX_ARK_BEAM_LINE` | usage | table: trail data for fxm_drawTrailLine, vertical light line at the strand tips and ring centre (same use as fx_1bf_0's FX_ARK_BEAM_LINE) |
| fx_17d_4 | 0x21 | `FX_ARK_FLAME_BOWL` | render | 'so' model: shallow bowl with an orange-white fire texture (byte-identical to fx_1bf_0 entry 0x6) |
| fx_17d_7 | 0x11 | `FX_ARK_PEACH_RING` | render | rings: white-peach-mauve gouraud rings (byte-identical to fx_17d_6 entry 0xB, fx_1bf_1 entry 0xC) |

Indices reached through module tables (`fx_17d_0_pieceFiles`, `fx_17d_0_texModelFileIds`, `fx_17d_4_strandInfo`,
`fx_17d_4_pointFileIds`, `fx_17d_4_particleFileIds`, `fx_12b_pathFileIds*`, `fx_138_*FileIds`,
`fx_183_*FileIds`, `fx_186_modelFileIds*`) have no literal and are not named.

## Table

| module | index | name | evidence | what it is |
|---|---|---|---|---|
| fx_129 | 0x1 | `FX_PUMPKIN_HEAD_SMOKE_BURST_2` | render | emitter: pale smoke burst with a dark red glow, later bounces |
| fx_129 | 0x3 | `FX_PUMPKIN_HEAD_SMOKE_BURST` | render | emitter: pale smoke burst with a dark red glow, first bounce |
| fx_129 | 0x7 | `FX_PUMPKIN_HEAD_PUMPKIN` | render | model: round pumpkin head with a bow tie, drawn with the caster's texture list |
| fx_12a | 0x1 | `FX_JET_FIRE_RED_GLOW` | render | emitter: red-white glow ring |
| fx_12a | 0x2 | `FX_JET_FIRE_WHITE_RING` | render | emitter: white-grey gouraud tube rings |
| fx_12a | 0x3 | `FX_JET_FIRE_JET_STREAM` | render | emitter: cyan wispy jet frames |
| fx_12a | 0x4 | `FX_JET_FIRE_CYAN_WISP_RING` | render | emitter: ring with a cyan wisp texture |
| fx_12a | 0xB | `FX_JET_FIRE_PALE_WISP_RING` | render | emitter: ring with a pale wisp texture |
| fx_12a | 0xC | `FX_JET_FIRE_WIND_GUST` | render | emitter: cyan wisp sprites |
| fx_12b | 0x8 | `FX_PHOTON_SMOKE_PUFF` | render | emitter: yellow-to-blue smoke puffs at each shard, + sub sprites (texture not in the file) |
| fx_12b | 0x9 | `FX_PHOTON_LIGHT_STREAK` | render (fxdata d6121033) | emitter: orbiting fan, a yellow-to-cyan light streak |
| fx_12b | 0xA | `FX_PHOTON_ENERGY_ORB` | render | emitter: golden-green energy sphere |
| fx_12b | 0xB | `FX_PHOTON_YELLOW_RING` | render | emitter: dark-to-bright yellow gouraud rings |
| fx_12b | 0xC | `FX_PHOTON_GOLD_RING` | render | emitter: dull gold gouraud rings |
| fx_12b | 0x21 | `FX_PHOTON_PATH_KEYS_2` | usage | table: position path keys for fxm_posPathInit (second shard path) |
| fx_12b | 0x22 | `FX_PHOTON_PATH_KEYS` | usage | table: position path keys for fxm_posPathInit (first shard path) |
| fx_12b | 0x23 | `FX_PHOTON_SPARKS` | render (fxdata d6121033) | emitter: white dot sparks |
| fx_12b | 0x24 | `FX_PHOTON_LIGHT_TRAIL` | render | emitter: white glow screen trail |
| fx_12b | 0x25 | `FX_PHOTON_LIGHT_TRAIL_3` | render | emitter: white glow screen trail |
| fx_12b | 0x26 | `FX_PHOTON_LIGHT_TRAIL_2` | render | emitter: white glow screen trail |
| fx_12b | 0x27 | `FX_PHOTON_LIGHT_TRAIL_4` | render | emitter: white glow screen trail |
| fx_12b | 0x28 | `FX_PHOTON_STAR_FLASH_3` | render | emitter: white star glow fading to yellow |
| fx_12b | 0x29 | `FX_PHOTON_STAR_FLASH_4` | render | emitter: white star glow fading to yellow |
| fx_12b | 0x37 | `FX_PHOTON_STAR_FLASH` | render | emitter: white star glow fading to yellow |
| fx_12b | 0x38 | `FX_PHOTON_STAR_FLASH_2` | render | emitter: white star glow fading to yellow |
| fx_12c | 0x1 | `FX_EARTHQUAKE_ROCK_CHUNK_8` | render (fxdata d6121033) | 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[8] |
| fx_12c | 0x2 | `FX_EARTHQUAKE_ROCK_CHUNK_1` | render (fxdata d6121033) | 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[1] |
| fx_12c | 0x3 | `FX_EARTHQUAKE_ROCK_CHUNK_2` | render (fxdata d6121033) | 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[2] |
| fx_12c | 0x4 | `FX_EARTHQUAKE_ROCK_CHUNK_3` | render (fxdata d6121033) | 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[3] |
| fx_12c | 0x5 | `FX_EARTHQUAKE_ROCK_CHUNK_4` | render (fxdata d6121033) | 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[4] |
| fx_12c | 0x6 | `FX_EARTHQUAKE_ROCK_CHUNK_5` | render (fxdata d6121033) | 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[5] |
| fx_12c | 0x7 | `FX_EARTHQUAKE_ROCK_CHUNK_6` | render (fxdata d6121033) | 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[6] |
| fx_12c | 0x8 | `FX_EARTHQUAKE_ROCK_CHUNK_7` | render (fxdata d6121033) | 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[7] |
| fx_12c | 0x9 | `FX_EARTHQUAKE_ROCK_CHUNK_0` | render (fxdata d6121033) | 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[0] |
| fx_12c | 0xE | `FX_EARTHQUAKE_ROCK_CHUNK_9` | render (fxdata d6121033) | 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[9] |
| fx_12c | 0x10 | `FX_EARTHQUAKE_RUBBLE` | render | emitter: spiral of small rock debris / rubble frames (byte-identical to fx_03a entry 0x10) |
| fx_12c | 0x11 | `FX_EARTHQUAKE_DUST_RING` | render | emitter: ring with a sand-dust texture on the ground |
| fx_12c | 0x12 | `FX_EARTHQUAKE_ROCK_DEBRIS` | render | emitter: small rock debris frames thrown up |
| fx_12d | 0x1 | `FX_DEATH_SAND_RING` | render | emitter: ring with a sand texture |
| fx_12d | 0x2 | `FX_DEATH_SAND_DUST` | render | emitter: pale sand-dust sprites |
| fx_12d | 0x3 | `FX_DEATH_SAND_CLOUD` | render | emitter: spiralling pale dust clouds |
| fx_12d | 0x4 | `FX_DEATH_SAND_SWIRL` | render | emitter: spiralling 3D trail with a grey sand texture |
| fx_12d | 0xB | `FX_DEATH_PINK_GLOW_TRAIL` | render | emitter: spiralling 3D trail of pink glows |
| fx_12d | 0xE | `FX_DEATH_ANTLION` | render | model: the Antlion (1142 polys), drawn with the caster's texture pages while the unit is hidden |
| fx_12f | 0x1 | `FX_PAPER_STORM_PAPER_SHEETS` | render | emitter: cone of fluttering paper sheets |
| fx_12f | 0x3 | `FX_PAPER_STORM_ORANGE_BURST` | render | emitter: orange speckle burst + red flat rings at each target |
| fx_12f | 0x4 | `FX_PAPER_STORM_PAPER_SWIRL` | render | emitter: paper sheets spiralling up |
| fx_12f | 0x8 | `FX_PAPER_STORM_GREEN_BURST` | render | emitter: green spiky burst, sprites, yellow and red rings at each target |
| fx_130 | 0x1 | `FX_PROPELLER_WIND_TORNADO` | render (fxdata d6121033) | 'so' model: blue twisted column with a wind texture (tornado), registered per target |
| fx_130 | 0x2 | `FX_PROPELLER_WIND_TEAL_RING` | render | emitter: orbiting dark-to-teal gouraud rings |
| fx_130 | 0x3 | `FX_PROPELLER_WIND_BLUE_CRESCENT` | render | emitter: blue crescent then ring particles (ring texture partly not in the file) |
| fx_130 | 0x4 | `FX_PROPELLER_WIND_WIND_RING` | render | emitter: orbiting grey wind ring |
| fx_130 | 0x5 | `FX_PROPELLER_WIND_WIND_SWIRL` | render | emitter: three-armed grey wind swirl |
| fx_131 | 0x1 | `FX_GOLEM_REVIVAL_PINK_GLOW` | render | emitter: pink glow with a green ring, fan |
| fx_131 | 0x2 | `FX_GOLEM_REVIVAL_SAND_CLOUD` | render | emitter: orbiting pale sand clouds |
| fx_132 | 0x1 | `FX_APPEARANCE_SMOKE_PUFF` | render | emitter: pale yellow-green smoke puffs |
| fx_132 | 0x2 | `FX_APPEARANCE_SPARKLE_HELIX` | render (fxdata d6121033) | emitter: helix of white and yellow dot sparkles |
| fx_133 | 0x1 | `FX_DISAPPEAR_DUST_PUFF` | render | emitter: pale smoke puffs (file 0x133; file 0x1B5 has coloured puffs and a "Dummy" letter sprite here) |
| fx_134 | 0x1 | `FX_DOOMSDAY_METEOR_TRAIL` | render | emitter: helix screen trail with a rock texture, red sparks |
| fx_134 | 0x3 | `FX_DOOMSDAY_NEBULA` | render | sprite: purple nebula bands on black (space background layer) |
| fx_134 | 0x4 | `FX_DOOMSDAY_STARFIELD` | render | sprite: blue stars on black (space background layer) |
| fx_134 | 0x5 | `FX_DOOMSDAY_GREEN_GLOW` | render | emitter: green glowing cloud particles |
| fx_134 | 0x6 | `FX_DOOMSDAY_RED_RING` | render | rings: white-pink-red gouraud rings |
| fx_134 | 0x8 | `FX_DOOMSDAY_METEOR` | render | model: rock polyhedron (32 polys, rock texture) |
| fx_134 | 0x9 | `FX_DOOMSDAY_WHITE_RING` | render | rings: white rings fading to grey (pool task) |
| fx_134 | 0xA | `FX_DOOMSDAY_FIRE_RING` | render | rings: white-orange-yellow gouraud rings (pool task) |
| fx_134 | 0xB | `FX_DOOMSDAY_BIG_METEOR` | render | model: large rock ellipsoid (324 polys, 15-bit rock texture) |
| fx_134 | 0xC | `FX_DOOMSDAY_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop |
| fx_134 | 0xD | `FX_DOOMSDAY_EMBERS` | render | emitter: small orange ember sprites (pool task) |
| fx_134 | 0x11 | `FX_DOOMSDAY_STAR_TUNNEL` | render | model: tube with a star texture, a warp tunnel |
| fx_134 | 0x12 | `FX_DOOMSDAY_STAR_ARCH` | render | model: arch with a star texture |
| fx_134 | 0x13 | `FX_DOOMSDAY_RED_DOME` | render | model: shallow dark-red gouraud dome with a white centre (46 polys; byte-identical to fx_0e0 entry 0x13) |
| fx_134 | 0x14 | `FX_DOOMSDAY_BIG_METEOR_2` | render | model: large rock ellipsoid (324 polys, 15-bit rock texture) |
| fx_134 | 0x15 | `FX_DOOMSDAY_EMBERS_2` | render | emitter: small yellow ember sprites |
| fx_134 | 0x16 | `FX_DOOMSDAY_FLAME_BURST` | render | emitter: orange flame puffs |
| fx_134 | 0x18 | `FX_DOOMSDAY_BLUE_PLANET` | render | sprite: pale blue planet |
| fx_134 | 0x19 | `FX_DOOMSDAY_LIGHT_CONE` | render | model: open cone with a grey streak texture |
| fx_134 | 0x1A | `FX_DOOMSDAY_LILAC_RING` | render | rings: white-lilac gouraud rings |
| fx_134 | 0x1B | `FX_DOOMSDAY_BLUE_RING` | render | rings: white-cyan-blue gouraud rings, four segments |
| fx_134 | 0x1E | `FX_DOOMSDAY_PURPLE_STREAK` | render | trail: purple streak (fxm_drawOffsetTrail) |
| fx_134 | 0x1F | `FX_DOOMSDAY_SPACE_LAYER` | render (fxdata d6121033) | sprite: purple-blue starry nebula strip (space background layer) |
| fx_134 | 0x20 | `FX_DOOMSDAY_RED_PLANET` | render | sprite: small red planet |
| fx_134 | 0x21 | `FX_DOOMSDAY_PEBBLES` | render | emitter: spiral of small grey pebble sprites (byte-identical to fx_0e0 entry 0x20) |
| fx_134 | 0x22 | `FX_DOOMSDAY_ROCK_DEBRIS` | render | emitter: grey rock chunks |
| fx_134 | 0x30 | `FX_DOOMSDAY_BACKDROP_2` | usage | table: background layers for btlfx_drawBackdrop |
| fx_134 | 0x31 | `FX_DOOMSDAY_BACKDROP_3` | usage | table: background layers for btlfx_drawBackdrop |
| fx_135 | 0x7 | `FX_BAD_BREATH_GAS_CLOUD` | render | emitter: cone of olive and purple gas blobs with green and pink glows |
| fx_135 | 0x8 | `FX_BAD_BREATH_GAS_BURST` | render | emitter: olive and purple gas blobs, glows and a yellow-green flash at the target |
| fx_136 | 0x1 | `FX_ABSORB_SPARKLE` | render | emitter: cyan swirl collapsing into a star sparkle |
| fx_136 | 0x3 | `FX_ABSORB_SMOKE_SWIRL` | render | emitter: curling white smoke |
| fx_137 | 0x5 | `FX_FLAME_FLAME_BURST` | render | emitter: flame explosion with a rising flame pillar, at each target |
| fx_137 | 0x6 | `FX_FLAME_FIREBALL` | render | emitter: fireball explosion at the mouth |
| fx_138 | 0x1 | `FX_LUCKY_SEVEN_FIRE_BURST` | render | emitter: white-blue gouraud rings, fire bursts and white / pink glows |
| fx_138 | 0xE | `FX_LUCKY_SEVEN_SPRITE_LINE` | kind/usage (look pending) | sprite line: small coloured sprites, drawn frames 0-0xC; the render is almost empty |
| fx_138 | 0x1B | `FX_LUCKY_SEVEN_SLOT_STRIPS` | kind/usage (look pending) | model: 15 polys (three slanted strips), tpage 0x9A not in the file (magenta in the render), drawn with the three reels |
| fx_138 | 0x2A | `FX_LUCKY_SEVEN_RAINBOW_TRAIL` | render | emitter: orbiting rainbow trail with glow sprites and rainbow rings |
| fx_179 | 0x1 | `FX_MEDEO_WHITE_RING` | render | emitter: white and lilac gouraud rings |
| fx_179 | 0x2 | `FX_MEDEO_METEOR` | render | model: rock (128 polys, red-brown rock texture) |
| fx_179 | 0x4 | `FX_MEDEO_COLOR_SMOKE` | render | emitter: spiral of yellow, purple, cyan and green smoke puffs |
| fx_179 | 0x5 | `FX_MEDEO_GLOW_TRAIL` | render | emitter: blue and green glow screen trails |
| fx_179 | 0x7 | `FX_MEDEO_SPARKLES` | render | emitter: small green and blue spark sprites |
| fx_179 | 0x8 | `FX_MEDEO_STAR_FLASH` | render | emitter: blue and green star flashes |
| fx_179 | 0x9 | `FX_MEDEO_COLOR_LIGHT_BARS` | render (fxdata d6121033) | emitter: three fans of blue / gold / green capsule-shaped light bars, + sprites that render transparent |
| fx_179 | 0xA | `FX_MEDEO_COLOR_SMOKE_2` | render | emitter: spiral of yellow, purple, cyan and green smoke puffs |
| fx_179 | 0xB | `FX_MEDEO_SPRITE_BURST` | kind/usage (look pending) | emitter: two sprite elements; the render is still all transparent |
| fx_17a | 0x1 | `FX_MADEEN_BLUE_RING` | render | rings: black-to-blue gouraud rings |
| fx_17a | 0x2 | `FX_MADEEN_BLUE_GLOW` | render | particles: blue glow (pool task) |
| fx_17a | 0x3 | `FX_MADEEN_BLUE_RINGS` | render | rings: white-blue gouraud rings |
| fx_17a | 0x4 | `FX_MADEEN_VIOLET_RING` | render | rings: white-cyan-violet gouraud rings, four segments |
| fx_17a | 0x5 | `FX_MADEEN_TEXTURED_RING` | render | rings: ring with a pink-green noise texture |
| fx_17a | 0x6 | `FX_MADEEN_TEAL_RING` | render | rings: black-blue-teal gouraud rings |
| fx_17a | 0x7 | `FX_MADEEN_RAINBOW_ARCH` | render | model: arch with a rainbow aura texture |
| fx_17a | 0x8 | `FX_MADEEN_WINGS` | render | model: three-armed wing shape with a grey feather texture |
| fx_17a | 0x9 | `FX_MADEEN_LOOP_RIBBON` | render (fxdata d6121033) | model: figure-eight band (its 15-bit texture page 0x138 holds a handwritten placeholder in the file) |
| fx_17a | 0xA | `FX_MADEEN_RAINBOW_SPHERE` | render | model: sphere wrapped in red-green-blue rainbow stripe bands (byte-identical to fx_0fb_1 summon entry 0x89) |
| fx_17a | 0xB | `FX_MADEEN_LENS_FLARES_2` | render | sprite line: green / red / orange lens-flare rings (byte-identical to fx_0fb_1 entry 0x17) |
| fx_17a | 0xC | `FX_MADEEN_LENS_FLARES` | render | sprite line: green / red / orange lens-flare rings (byte-identical to fx_0fb_1 entry 0xC) |
| fx_17a | 0xD | `FX_MADEEN_VIOLET_GLOW` | render (fxdata d6121033) | emitter: three fans, a horizontal violet-white glow |
| fx_17a | 0xE | `FX_MADEEN_WHITE_GLOW` | render | particles: white-blue glow |
| fx_17a | 0xF | `FX_MADEEN_BLUE_SPARKLES` | render | emitter: blue square sparkles and gold sprites |
| fx_17a | 0x11 | `FX_MADEEN_TRAIL` | kind/usage (look pending) | trail: strip texture not in the file (magenta in the render) |
| fx_17a | 0x12 | `FX_MADEEN_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop |
| fx_17a | 0x13 | `FX_MADEEN_BLUE_STREAKS` | render | emitter: orbiting blue light streaks |
| fx_17d_0 | 0x19 | `FX_ARK_CAMERA_KEYS_2` | usage | table: per-frame position keys for camera point 1 (op 0x82) |
| fx_17d_0 | 0x1A | `FX_ARK_CAMERA_KEYS` | usage | table: per-frame position keys for camera point 0 (op 0x82) |
| fx_17d_0 | 0x1B | `FX_ARK_MORPH_KEYS` | usage | table: per-frame morph weights, first morph pair |
| fx_17d_0 | 0x1C | `FX_ARK_NOISE_SPHERE` | kind/usage (look pending) | 'so' model registered at the start of op 0x80: a sphere, its texture reads as white noise in the render |
| fx_17d_0 | 0x1D | `FX_ARK_RED_ROCK_LAYER` | render | sprite: red-brown rock / cloud texture drawn on screen |
| fx_17d_0 | 0x1E | `FX_ARK_MORPH_PATH` | usage | table: per-frame position + rotation keys for the first morph models |
| fx_17d_0 | 0x1F | `FX_ARK_MORPH_PATH_2` | usage | table: per-frame position + rotation keys for the second morph models |
| fx_17d_0 | 0x20 | `FX_ARK_MORPH_KEYS_2` | usage | table: per-frame morph weights, second morph pair |
| fx_17d_0 | 0x21 | `FX_ARK_MORPH_KEYS_3` | usage | table: per-frame morph weights for the second morph models |
| fx_17d_0 | 0x22 | `FX_ARK_GREY_TRAIL` | render | trail: grey noise strip (pool task) |
| fx_17d_0 | 0x23 | `FX_ARK_VIOLET_RING` | render | emitter: white-violet-blue and white-cream-purple gouraud rings |
| fx_17d_0 | 0x25 | `FX_ARK_CAMERA_ROLL_KEYS` | usage | table: per-frame keys whose third value is camera point 0 roll (op 0x82) |
| fx_17d_1 | 0x5 | `FX_ARK_FIRE_RING` | render | rings: white-yellow-red gouraud rings |
| fx_17d_1 | 0x8 | `FX_ARK_ORANGE_SPARKLES` | render | emitter: orange square sparkles |
| fx_17d_1 | 0x9D | `FX_ARK_SPARKLE_TRAIL` | render (fxdata d6121033) | summon data: emitter, spiralling screen trail with orange square sparkles |
| fx_17d_1 | 0x9E | `FX_ARK_STREAK_SHELL` | render (fxdata d6121033) | summon data: 'so' model, barrel-shaped shell with a grey streak texture |
| fx_17d_1 | 0x9F | `FX_ARK_ORANGE_SPARKLES_2` | render (fxdata d6121033) | summon data: emitter, orange square sparkles |
| fx_17d_2 | 0x1 | `FX_ARK_FIRE_RING` | render (fxdata d6121033) | rings: white-yellow-red gouraud rings (as fx_17d_1's) |
| fx_17d_2 | 0x2 | `FX_ARK_THIN_ARCH` | kind/usage (look pending) | 'so' model: thin arch, its texture reads as noise in the render |
| fx_17d_2 | 0x3 | `FX_ARK_ORANGE_SPARKLES` | render (fxdata d6121033) | emitter: spiral of orange square sparkles (as fx_17d_1's) |
| fx_17d_3 | 0x1 | `FX_ARK_SMOKE_TRAIL` | render | emitter: spiralling screen trail with a blue-grey smoke texture, orange embers |
| fx_17d_3 | 0x2 | `FX_ARK_CLOUD_TRAIL` | render | emitter: two spiralling screen trails with a grey cloud texture |
| fx_17d_3 | 0x4 | `FX_ARK_SMOKE_TRAIL_2` | render | emitter: spiralling screen trail with a blue-grey smoke texture, orange embers |
| fx_17d_3 | 0x6 | `FX_ARK_SMOKE_TRAIL_3` | render | emitter: spiralling screen trail with a blue-grey smoke texture, orange embers |
| fx_17d_3 | 0x9 | `FX_ARK_DUST_CLOUD` | render | emitter: grey dust clouds and grey sparks |
| fx_17d_3 | 0xE | `FX_ARK_SMOKE_CLOUDS` | render (fxdata d6121033) | emitter: two fans of grey-purple smoke clouds fading out |
| fx_17d_3 | 0x80 | `FX_ARK_WING_PLATE` | render (fxdata d6121033) | summon data: 'so' model, V-shaped plate with a grey metal texture |
| fx_17d_3 | 0x81 | `FX_ARK_GREY_CYLINDER` | render (fxdata d6121033) | summon data: 'so' model, cylinder with a grey patterned texture |
| fx_17d_3 | 0x82 | `FX_ARK_WING_PLATE_2` | render (fxdata d6121033) | summon data: 'so' model, flat plate with a grey metal texture and a dark centre bar |
| fx_17d_3 | 0x83 | `FX_ARK_TEAL_CYLINDER` | render (fxdata d6121033) | summon data: 'so' model, cylinder with a dark teal patterned texture |
| fx_17d_4 | 0x1 | `FX_ARK_GREEN_TRAIL` | render | emitter: orbiting 3D trails with a green-blue texture |
| fx_17d_4 | 0x3 | `FX_ARK_RED_RING` | render | rings: white-pink-red gouraud rings |
| fx_17d_4 | 0x5 | `FX_ARK_RED_FLASH_RING` | render | rings: red-white flat flash ring |
| fx_17d_4 | 0x17 | `FX_ARK_MAGENTA_RING` | render | emitter: grey noise rings and white-magenta-purple gouraud rings |
| fx_17d_4 | 0x84 | `FX_ARK_STREAK_BOWL` | render (fxdata d6121033) | summon data: 'so' model, bowl ring with a grey streak texture |
| fx_17d_4 | 0x86 | `FX_ARK_SWIRL_DOME` | render (fxdata d6121033) | summon data: 'so' model, dome with a grey swirl texture |
| fx_17d_4 | 0x8A | `FX_ARK_STRAND_POINTS` | usage | summon data: offset table of the strand point lists per layer, read by fx_17d_4_drawStrands (byte-identical to fx_1bf_0 entry 0x1D) |
| fx_17d_5 | 0x1 | `FX_ARK_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop |
| fx_17d_5 | 0x2 | `FX_ARK_CREAM_RING` | render | emitter: cream-white and white-grey-violet gouraud rings |
| fx_17d_5 | 0x3 | `FX_ARK_LILAC_RING` | render | emitter: dark noise particle + lilac-white gouraud ring |
| fx_17d_5 | 0x4 | `FX_ARK_ORANGE_RING` | render | rings: white-orange-brown gouraud rings (pool task) |
| fx_17d_5 | 0x5 | `FX_ARK_TRAIL` | kind/usage (look pending) | trail: strip texture not resolved (noise and green in the render) |
| fx_17d_5 | 0x8 | `FX_ARK_PLANET` | render | particles: green-tinted planet with cloud swirls, fading |
| fx_17d_5 | 0x9 | `FX_ARK_TRAIL_2` | kind/usage (look pending) | trail: strip texture not resolved (noise and green in the render) |
| fx_17d_5 | 0xA | `FX_ARK_PLANET_FRAGMENTS` | render | emitter: sprites of planet-surface fragments |
| fx_17d_5 | 0xB | `FX_ARK_RED_FLASH_RING` | render | rings: red-white flat flash ring |
| fx_17d_5 | 0xD | `FX_ARK_SPRITE_LINE` | kind/usage (look pending) | sprite line: textures partly not resolved (almost empty in the render) |
| fx_17d_5 | 0x85 | `FX_ARK_RIBBED_SPHERE` | render (fxdata d6121033) | summon data: 'so' model, sphere with a dark ribbed texture |
| fx_17d_5 | 0x87 | `FX_ARK_SCALE_BOWL` | render (fxdata d6121033) | summon data: 'so' model, bowl with a red-orange scale texture |
| fx_17d_5 | 0x88 | `FX_ARK_SCALE_DOME` | render (fxdata d6121033) | summon data: 'so' model, hemisphere with a red-orange scale texture |
| fx_17d_5 | 0x91 | `FX_ARK_YELLOW_RING` | render (fxdata d6121033) | summon data: 'so' model, yellow-white gouraud ring |
| fx_17d_5 | 0x9B | `FX_ARK_TAN_GLOW_DISC` | render (fxdata d6121033) | summon data: 'so' model, shallow tan-white gouraud glow disc |
| fx_17d_6 | 0x1 | `FX_ARK_PLANET_LIMB` | render (fxdata d6121033) | emitter: fan, the blue edge of a planet (Earth-like limb) fading out |
| fx_17d_6 | 0x2 | `FX_ARK_GOLD_SPARKLES` | render | emitter: gold square sparkles |
| fx_17d_6 | 0x4 | `FX_ARK_FLAME_BURST` | render | emitter: red-orange flame burst sprites (partly white: texture mix) |
| fx_17d_6 | 0x5 | `FX_ARK_CLOUD_TRAIL` | render | emitter: orbiting 3D trails with a grey cloud texture |
| fx_17d_6 | 0x6 | `FX_ARK_ORANGE_RING` | render | rings: white-orange-brown gouraud rings |
| fx_17d_6 | 0xA | `FX_ARK_RED_RING` | render | rings: red to dark red gouraud rings |
| fx_17d_6 | 0xB | `FX_ARK_PEACH_RING` | render | rings: white-peach-mauve gouraud rings |
| fx_17d_6 | 0xC | `FX_ARK_FAN_EMITTER_2` | kind/usage (look pending) | emitter: two fans; the render shows small olive / orange garbled patches |
| fx_17d_6 | 0xD | `FX_ARK_SPRITE_LINE` | kind/usage (look pending) | sprite line: green textures, partly not resolved (almost empty in the render) |
| fx_17d_6 | 0xE | `FX_ARK_GREY_RING` | render | emitter: grey-white gouraud ring |
| fx_17d_6 | 0x8B | `FX_ARK_ORANGE_DISC` | render (fxdata d6121033) | summon data: 'so' model, flat disc with an orange-brown speckle texture |
| fx_17d_6 | 0x8D | `FX_ARK_GREY_SWIRL_DISC` | render (fxdata d6121033) | summon data: 'so' model, flat disc with a grey swirl texture |
| fx_17d_7 | 0x1 | `FX_ARK_ROCK_DEBRIS` | render | emitter: grey-blue rock chunks |
| fx_17d_7 | 0x2 | `FX_ARK_LIGHT_STREAKS` | render (fxdata d6121033) | emitter: two fans, thin pale orange light streaks |
| fx_17d_7 | 0x4 | `FX_ARK_ROCK_DEBRIS_2` | render | emitter: grey-blue rock chunks |
| fx_17d_7 | 0x6 | `FX_ARK_PALE_RING` | render | emitter: white-cream-violet and white-peach-mauve gouraud rings |
| fx_17d_7 | 0x7 | `FX_ARK_VIOLET_RING` | render | emitter: white-cream-violet and white-purple-blue gouraud rings |
| fx_17d_7 | 0xA | `FX_ARK_GOLD_GLOW` | render | emitter: golden glow flash |
| fx_17d_7 | 0xD | `FX_ARK_ORANGE_RING` | render | emitter: two fans (not rendered) + white-orange gouraud rings |
| fx_17d_7 | 0x8C | `FX_ARK_ORANGE_BEAM` | render (fxdata d6121033) | summon data: 'so' model, long orange gouraud beam |
| fx_17d_7 | 0x8E | `FX_ARK_TEAL_RING` | render (fxdata d6121033) | summon data: 'so' model, flat ring with a dark teal texture |
| fx_17d_7 | 0x8F | `FX_ARK_FLAME_TEX_RING` | render (fxdata d6121033) | summon data: 'so' model, flat ring with an orange flame texture |
| fx_17d_7 | 0x90 | `FX_ARK_SMOKE_BOWL` | render (fxdata d6121033) | summon data: 'so' model, bowl ring with a grey-brown smoke texture |
| fx_17d_7 | 0x92 | `FX_ARK_GREY_TORUS` | render (fxdata d6121033) | summon data: 'so' model, grey gouraud torus |
| fx_17d_7 | 0x93 | `FX_ARK_EMBER_RING` | render (fxdata d6121033) | summon data: 'so' model, flat ring with an orange speckle texture |
| fx_17d_7 | 0x94 | `FX_ARK_GOLD_RING` | render (fxdata d6121033) | summon data: 'so' model, gold gouraud torus |
| fx_17d_7 | 0x95 | `FX_ARK_BLUE_CLOUD_RING` | render (fxdata d6121033) | summon data: 'so' model, twisted ring band with a blue cloud texture |
| fx_17d_7 | 0x96 | `FX_ARK_ROCK` | render (fxdata d6121033) | summon data: 'so' model, rock chunk |
| fx_17d_7 | 0x97 | `FX_ARK_FIRE_CONE` | render (fxdata d6121033) | summon data: 'so' model, cone skirt with a flame texture |
| fx_17d_7 | 0x98 | `FX_ARK_ROCK_2` | render (fxdata d6121033) | summon data: 'so' model, rock chunk |
| fx_17d_7 | 0x99 | `FX_ARK_ROCK_3` | render (fxdata d6121033) | summon data: 'so' model, rock chunk |
| fx_17d_7 | 0x9A | `FX_ARK_FIRE_ARC` | render (fxdata d6121033) | summon data: 'so' model, curved tube with an orange fire texture |
| fx_17d_8 | 0x1 | `FX_ARK_PLANET` | render (fxdata d6121033) | 'so' model: globe with an Earth-like planet texture, work model slot 0 |
| fx_17d_8 | 0x2 | `FX_ARK_RED_GLOW_DOME` | render (fxdata d6121033) | 'so' model: shallow red-orange gouraud glow dome, work model slot 1 |
| fx_17d_8 | 0x3 | `FX_ARK_ORANGE_BEAM` | render (fxdata d6121033) | 'so' model: long orange gouraud beam (same size as fx_17d_7's), work model slot 3 |
| fx_17d_8 | 0x4 | `FX_ARK_DARK_BLUE_RING` | render (fxdata d6121033) | 'so' model: flat ring with a dark blue texture, work model slot 5 |
| fx_17d_8 | 0x5 | `FX_ARK_FIRE_RING` | render | rings: white-yellow-red gouraud rings |
| fx_17d_8 | 0x6 | `FX_ARK_FLAT_PANEL` | kind/usage (look pending) | 'so' model: flat panel on page 0x11B, which holds a handwritten placeholder in the file (filled at run time), work model slot 4 |
| fx_17d_8 | 0x7 | `FX_ARK_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop |
| fx_17d_8 | 0x9 | `FX_ARK_GREY_STAR_DISC` | render (fxdata d6121033) | 'so' model: shallow disc with a grey radial texture, work model slot 2 |
| fx_17d_8 | 0xB | `FX_ARK_GOLD_SPARKS` | render | emitter: tube of small gold spark sprites |
| fx_17d_8 | 0x10 | `FX_ARK_STARFIELD` | render | sprite: faint starfield drawn on screen |
| fx_17e | 0x2 | `FX_CHESTNUT_IMPACT_RINGS` | render | emitter: white-pink-red-yellow and white-grey gouraud rings + spark sprites (texture not in the file) |
| fx_17e | 0x9 | `FX_CHESTNUT_SPIKY_BURR` | render (fxdata d6121033) | 'so' model: the thrown chestnut, a spiky ball with a brown-orange burr texture |
| fx_17f | 0x1 | `FX_MELT_PINK_SMOKE` | render | emitter: pink smoke puffs |
| fx_17f | 0x2 | `FX_MELT_GOO_BLOB` | render | emitter: purple goo blobs |
| fx_17f | 0x3 | `FX_MELT_GOO_BLOB_2` | render | emitter: small dark purple goo blobs |
| fx_17f | 0x4 | `FX_MELT_GOO_RIPPLE` | render | emitter: pink ripple disc |
| fx_17f | 0xB | `FX_MELT_GOO_SPLASH` | render | emitter: glossy purple goo splash |
| fx_180 | 0x1 | `FX_ULTIMA_GLOW_DOME` | render | model: gouraud octagonal dome (20 polys) |
| fx_180 | 0x2 | `FX_ULTIMA_NOISE_RING` | kind/usage (look pending) | emitter: ring whose texture still reads as noise, + pink gouraud ring |
| fx_180 | 0x3 | `FX_ULTIMA_ROCK_DEBRIS` | render | emitter: grey rock chunks |
| fx_180 | 0x4 | `FX_ULTIMA_GREY_TRAIL` | render | emitter: wobbling 3D trail with a grey streak texture |
| fx_180 | 0x6 | `FX_ULTIMA_RED_LENS_FLARE` | render | emitter: red lens streak, white-pink rings, red sparks |
| fx_180 | 0x7 | `FX_ULTIMA_COLOR_RING` | render | emitter: yellow, lilac, violet and magenta gouraud rings |
| fx_180 | 0x8 | `FX_ULTIMA_ROCK_DEBRIS_2` | render | emitter: spiral of grey rock chunks |
| fx_180 | 0x9 | `FX_ULTIMA_PINK_RING` | render | emitter: pink gouraud rings + fan |
| fx_180 | 0xA | `FX_ULTIMA_ROCK_TRAIL` | render | emitter: screen trail of rock pieces |
| fx_180 | 0xB | `FX_ULTIMA_GREY_TRAIL_2` | render | emitter: wobbling screen trail with a grey streak texture |
| fx_180 | 0xC | `FX_ULTIMA_BLAST_RINGS` | render | emitter: white-pink, lilac and yellow-red gouraud rings, pink glows |
| fx_180 | 0xD | `FX_ULTIMA_WHITE_BEAM` | render | emitter: spiralling white beam screen trail, pink sparks |
| fx_180 | 0xE | `FX_ULTIMA_PINK_SPARKS` | render | emitter: sparse pink spark sprites |
| fx_180 | 0xF | `FX_ULTIMA_NOISE_RING_2` | kind/usage (look pending) | emitter: ring whose texture still reads as noise, + pink-lilac gouraud rings |
| fx_180 | 0x10 | `FX_ULTIMA_RED_FLARE` | render | emitter: red lens streak and white-red gouraud rings |
| fx_180 | 0x11 | `FX_ULTIMA_CRIMSON_STREAK` | render (fxdata d6121033) | emitter: orbiting fan, a white-to-crimson light streak |
| fx_180 | 0x12 | `FX_ULTIMA_PINK_TRAIL` | render | emitter: pink screen trail |
| fx_180 | 0x13 | `FX_ULTIMA_PINK_TRAIL_2` | render | emitter: orbiting pink screen trail |
| fx_180 | 0x14 | `FX_ULTIMA_PINK_SPIKES` | render | emitter: pink spike particles + spark sprites |
| fx_180 | 0x15 | `FX_ULTIMA_PINK_SPIKES_2` | render | emitter: pink spike particles + spark sprites |
| fx_180 | 0x16 | `FX_ULTIMA_PINK_ORB_BURST` | render | emitter: pink orb and blade particles, sparks, white-yellow-red gouraud rings |
| fx_180 | 0x17 | `FX_ULTIMA_SPIKE_CROWN` | render (fxdata d6121033) | 'so' model: ring crown of red-pink spikes, drawn with FX_ULTIMA_GLOW_DOME |
| fx_180 | 0x18 | `FX_ULTIMA_RED_FLARE_2` | render | emitter: red lens streak and white-red gouraud rings |
| fx_180 | 0x19 | `FX_ULTIMA_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop |
| fx_180 | 0x1A | `FX_ULTIMA_PATH_SET_2` | usage | table: 30 position paths (fxm_offsetTableEntry, fxm_posPathInit), op 0x81 |
| fx_180 | 0x1D | `FX_ULTIMA_ROCK_SWIRL` | render | emitter: spiralling 3D trail of rock pieces, pink glows |
| fx_180 | 0x1E | `FX_ULTIMA_ROCK_SWIRL_2` | render | emitter: spiralling 3D trail of rock pieces, pink glows |
| fx_180 | 0x1F | `FX_ULTIMA_NOISE_RING_3` | kind/usage (look pending) | emitter: ring whose texture still reads as noise, + lilac gouraud ring |
| fx_180 | 0x20 | `FX_ULTIMA_ROCK_TRAIL_2` | render | emitter: spiralling screen trail of rock pieces |
| fx_180 | 0x21 | `FX_ULTIMA_PURPLE_HAZE` | render (fxdata d6121033) | emitter: pink-purple smoke puffs |
| fx_180 | 0x22 | `FX_ULTIMA_PATH_SET` | usage | table: 18 position paths (fxm_offsetTableEntry, fxm_posPathInit), op 0x80 |
| fx_181 | 0x1 | `FX_COUNTDOWN_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop |
| fx_181 | 0x2 | `FX_COUNTDOWN_GOLD_SHARDS` | render | emitter: golden shard flashes, a dark teal sprite and white-blue gouraud rings |
| fx_181 | 0x3 | `FX_COUNTDOWN_LENS_FLASH` | render | emitter: pink lens streak, golden shard flashes, rings (ring texture not resolved) |
| fx_182 | 0x1 | `FX_COUNTDOWN_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop (byte-identical to fx_181 entry 0x1) |
| fx_182 | 0x3 | `FX_COUNTDOWN_GOLD_SHARDS` | render | emitter: golden shard flashes, a dark teal sprite and white-blue gouraud rings (renders as fx_181 entry 0x2) |
| fx_182 | 0x6 | `FX_COUNTDOWN_LENS_FLASH` | render | emitter: pink lens streak, golden shard flashes, rings (ring texture not resolved; renders as fx_181 entry 0x3) |
| fx_183 | 0xD | `FX_CHERRY_BLOSSOM_PETAL` | render | emitter: spiralling pink petal |
| fx_183 | 0xE | `FX_CHERRY_BLOSSOM_WHITE_RING` | render | emitter: yellow-white gouraud rings + sprites (texture not in the file) |
| fx_183 | 0xF | `FX_CHERRY_BLOSSOM_PETAL_STORM` | render | emitter: drifting pink petals and a large petal |
| fx_183 | 0x10 | `FX_CHERRY_BLOSSOM_PETAL_TRAIL` | render | emitter: orbiting screen trail of petals |
| fx_183 | 0x11 | `FX_CHERRY_BLOSSOM_PINK_RING` | render | emitter: pink and white-red gouraud rings |
| fx_183 | 0x17 | `FX_CHERRY_BLOSSOM_LIGHT_STREAK` | render (fxdata d6121033) | emitter: fan drawn as a thin green to violet horizontal light streak (byte-identical to fx_03d entry 0x10) |
| fx_183 | 0x18 | `FX_CHERRY_BLOSSOM_SPARKLE_CIRCLE` | render | emitter: circle of gold / white sparkle quads + teal specks (byte-identical to fx_03d entry 0x11) |
| fx_183 | 0x1D | `FX_CHERRY_BLOSSOM_SPEAR` | render (fxdata d6121033) | 'so' model: a spear (the Spear model's shape and size, other bytes) |
| fx_183 | 0x1E | `FX_CHERRY_BLOSSOM_PETAL_VORTEX` | render | emitter: pink swirl vortex + sprites (texture not in the file) |
| fx_184 | 0x1 | `FX_SPEAR_WHITE_RING` | render | emitter: white-lilac gouraud ring + sprites that render transparent, fan |
| fx_184 | 0x2 | `FX_SPEAR_SPEAR` | usage | 'so' model: the thrown spear (registered after btlfx_setWeaponVisible(caster, 0)) |
| fx_185 | 0x1 | `FX_SPEAR_WHITE_RING` | render | emitter: white-lilac gouraud ring + sprites that render transparent, fan |
| fx_185 | 0xB | `FX_SPEAR_SPEAR` | usage | 'so' model: the thrown spear (registered after btlfx_setWeaponVisible(caster, 0)) (byte-identical to fx_184 entry 0x2) |
| fx_186 | 0xB | `FX_EARTH_SHAKE_ROCK_ARCH` | render (fxdata d6121033) | 'so' model: arch with a rock texture, registered at the start |
| fx_186 | 0xF | `FX_EARTH_SHAKE_GREY_RING` | render | rings: white-to-grey gouraud rings at each target |
| fx_186 | 0x1A | `FX_EARTH_SHAKE_ROCK_DEBRIS` | render | emitter: small rock debris frames |
| fx_186 | 0x1B | `FX_EARTH_SHAKE_RUBBLE_STRIP` | render (fxdata d6121033) | 'so' model: flat strip of rubble with a grey smoke texture, registered at the start |
| fx_186 | 0x1D | `FX_EARTH_SHAKE_ROCK_DEBRIS_2` | render | emitter: small rock debris frames, brighter |
| fx_186 | 0x1E | `FX_EARTH_SHAKE_LILAC_RING` | render | rings: white-lilac gouraud rings |
| fx_186 | 0x1F | `FX_EARTH_SHAKE_CYAN_RING` | render | rings: white-cyan gouraud rings |
| fx_187 | 0x1 | `FX_FIRE_BLADES_FLAME_BURST` | render | emitter: flame burst + sprites (texture not in the file) |
| fx_187 | 0x2 | `FX_FIRE_BLADES_FLAME_BURST_2` | render | emitter: flame burst + sprites (texture not in the file) |
| fx_187 | 0x3 | `FX_FIRE_BLADES_FIRE_BLADE` | render | emitter: spiralling yellow blade screen trail with red flame particles |
| fx_187 | 0x4 | `FX_FIRE_BLADES_FIRE_BLADE_2` | render | emitter: spiralling yellow blade screen trail with red flame particles |
| fx_187 | 0x5 | `FX_FIRE_BLADES_SPARKS` | render (fxdata d6121033) | emitter: red dot sparks |
| fx_187 | 0x6 | `FX_FIRE_BLADES_SPARKS_2` | render (fxdata d6121033) | emitter: red dot sparks |
| fx_187 | 0x7 | `FX_FIRE_BLADES_FIRE_BLADE_3` | render | emitter: helix of yellow blade screen trail with red flame particles |
| fx_187 | 0x8 | `FX_FIRE_BLADES_FIRE_BLADE_4` | render | emitter: helix of yellow blade screen trail with red flame particles |
| fx_187 | 0x9 | `FX_FIRE_BLADES_SPARKS_3` | render (fxdata d6121033) | emitter: helix of red dot sparks |
| fx_187 | 0xA | `FX_FIRE_BLADES_SPARKS_4` | render (fxdata d6121033) | emitter: helix of red dot sparks |
| fx_187 | 0xB | `FX_FIRE_BLADES_FLAME_BURST_3` | render | emitter: orbiting flame burst + sprites (texture not in the file) |
| fx_187 | 0xC | `FX_FIRE_BLADES_RED_RING` | render | emitter: dark red gouraud rings |
| fx_187 | 0x13 | `FX_FIRE_BLADES_FLAME_TRAIL` | render | emitter: spiralling 3D trail with an orange flame texture |

## Open questions (emulator)

All still-pending names of all slices, grouped by battle setup with what to capture, are in
`docs/wip/fxdata_runtime.md` (the emulator checklist); the list below is this slice's original notes.

15 names are still by kind (look pending) after the re-render with `tools/fxdata.py` at d6121033 (summon data, fans, 'so' models and dot sprites drawn). What is left is texture the file does not hold or the tool does not resolve (blank or placeholder pages filled at run time, tpages the C computes, texture lists the tool does not follow, garbled texels). Each wants one screenshot at the frame the C starts the entry, or a VRAM dump while it plays:

| module | index | name | why the look is pending |
|---|---|---|---|
| fx_138 | 0xe | `FX_LUCKY_SEVEN_SPRITE_LINE` | sprite line: small coloured sprites, drawn frames 0-0xC; the render is almost empty |
| fx_138 | 0x1b | `FX_LUCKY_SEVEN_SLOT_STRIPS` | model: 15 polys (three slanted strips), tpage 0x9A not in the file (magenta in the render), drawn with the three reels |
| fx_179 | 0xb | `FX_MEDEO_SPRITE_BURST` | emitter: two sprite elements; the render is still all transparent |
| fx_17a | 0x11 | `FX_MADEEN_TRAIL` | trail: strip texture not in the file (magenta in the render) |
| fx_17d_0 | 0x1c | `FX_ARK_NOISE_SPHERE` | 'so' model registered at the start of op 0x80: a sphere, its texture reads as white noise in the render |
| fx_17d_2 | 0x2 | `FX_ARK_THIN_ARCH` | 'so' model: thin arch, its texture reads as noise in the render |
| fx_17d_5 | 0x5 | `FX_ARK_TRAIL` | trail: strip texture not resolved (noise and green in the render) |
| fx_17d_5 | 0x9 | `FX_ARK_TRAIL_2` | trail: strip texture not resolved (noise and green in the render) |
| fx_17d_5 | 0xd | `FX_ARK_SPRITE_LINE` | sprite line: textures partly not resolved (almost empty in the render) |
| fx_17d_6 | 0xc | `FX_ARK_FAN_EMITTER_2` | emitter: two fans; the render shows small olive / orange garbled patches |
| fx_17d_6 | 0xd | `FX_ARK_SPRITE_LINE` | sprite line: green textures, partly not resolved (almost empty in the render) |
| fx_17d_8 | 0x6 | `FX_ARK_FLAT_PANEL` | 'so' model: flat panel on page 0x11B, which holds a handwritten placeholder in the file (filled at run time), work model slot 4 |
| fx_180 | 0x2 | `FX_ULTIMA_NOISE_RING` | emitter: ring whose texture still reads as noise, + pink gouraud ring |
| fx_180 | 0xf | `FX_ULTIMA_NOISE_RING_2` | emitter: ring whose texture still reads as noise, + pink-lilac gouraud rings |
| fx_180 | 0x1f | `FX_ULTIMA_NOISE_RING_3` | emitter: ring whose texture still reads as noise, + lilac gouraud ring |
