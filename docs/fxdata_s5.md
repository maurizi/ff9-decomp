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
| fx_12b | 0x9 | `FX_PHOTON_FAN_EMITTER` | kind/usage (look pending) | emitter: orbiting fan element (not rendered) |
| fx_12b | 0xA | `FX_PHOTON_ENERGY_ORB` | render | emitter: golden-green energy sphere |
| fx_12b | 0xB | `FX_PHOTON_YELLOW_RING` | render | emitter: dark-to-bright yellow gouraud rings |
| fx_12b | 0xC | `FX_PHOTON_GOLD_RING` | render | emitter: dull gold gouraud rings |
| fx_12b | 0x21 | `FX_PHOTON_PATH_KEYS_2` | usage | table: position path keys for fxm_posPathInit (second shard path) |
| fx_12b | 0x22 | `FX_PHOTON_PATH_KEYS` | usage | table: position path keys for fxm_posPathInit (first shard path) |
| fx_12b | 0x23 | `FX_PHOTON_SPARKS` | kind/usage (look pending) | emitter: spark sprites (texture not in the file) |
| fx_12b | 0x24 | `FX_PHOTON_LIGHT_TRAIL` | render | emitter: white glow screen trail |
| fx_12b | 0x25 | `FX_PHOTON_LIGHT_TRAIL_3` | render | emitter: white glow screen trail |
| fx_12b | 0x26 | `FX_PHOTON_LIGHT_TRAIL_2` | render | emitter: white glow screen trail |
| fx_12b | 0x27 | `FX_PHOTON_LIGHT_TRAIL_4` | render | emitter: white glow screen trail |
| fx_12b | 0x28 | `FX_PHOTON_STAR_FLASH_3` | render | emitter: white star glow fading to yellow |
| fx_12b | 0x29 | `FX_PHOTON_STAR_FLASH_4` | render | emitter: white star glow fading to yellow |
| fx_12b | 0x37 | `FX_PHOTON_STAR_FLASH` | render | emitter: white star glow fading to yellow |
| fx_12b | 0x38 | `FX_PHOTON_STAR_FLASH_2` | render | emitter: white star glow fading to yellow |
| fx_12c | 0x1 | `FX_EARTHQUAKE_GROUND_SLAB_8` | kind/usage (look pending) | 'so' model: bouncing ground piece, fx_12c_bounceModels[8] (drawn over the offscreen background copy) |
| fx_12c | 0x2 | `FX_EARTHQUAKE_GROUND_SLAB_1` | kind/usage (look pending) | 'so' model: bouncing ground piece, fx_12c_bounceModels[1] (drawn over the offscreen background copy) |
| fx_12c | 0x3 | `FX_EARTHQUAKE_GROUND_SLAB_2` | kind/usage (look pending) | 'so' model: bouncing ground piece, fx_12c_bounceModels[2] (drawn over the offscreen background copy) |
| fx_12c | 0x4 | `FX_EARTHQUAKE_GROUND_SLAB_3` | kind/usage (look pending) | 'so' model: bouncing ground piece, fx_12c_bounceModels[3] (drawn over the offscreen background copy) |
| fx_12c | 0x5 | `FX_EARTHQUAKE_GROUND_SLAB_4` | kind/usage (look pending) | 'so' model: bouncing ground piece, fx_12c_bounceModels[4] (drawn over the offscreen background copy) |
| fx_12c | 0x6 | `FX_EARTHQUAKE_GROUND_SLAB_5` | kind/usage (look pending) | 'so' model: bouncing ground piece, fx_12c_bounceModels[5] (drawn over the offscreen background copy) |
| fx_12c | 0x7 | `FX_EARTHQUAKE_GROUND_SLAB_6` | kind/usage (look pending) | 'so' model: bouncing ground piece, fx_12c_bounceModels[6] (drawn over the offscreen background copy) |
| fx_12c | 0x8 | `FX_EARTHQUAKE_GROUND_SLAB_7` | kind/usage (look pending) | 'so' model: bouncing ground piece, fx_12c_bounceModels[7] (drawn over the offscreen background copy) |
| fx_12c | 0x9 | `FX_EARTHQUAKE_GROUND_SLAB_0` | kind/usage (look pending) | 'so' model: bouncing ground piece, fx_12c_bounceModels[0] (drawn over the offscreen background copy) |
| fx_12c | 0xE | `FX_EARTHQUAKE_GROUND_SLAB_9` | kind/usage (look pending) | 'so' model: bouncing ground piece, fx_12c_bounceModels[9] (drawn over the offscreen background copy) |
| fx_12c | 0x10 | `FX_EARTHQUAKE_ROCK_DEBRIS_2` | render | emitter: spiral of small rock debris frames |
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
| fx_130 | 0x1 | `FX_PROPELLER_WIND_TARGET_MODEL` | kind/usage (look pending) | 'so' model registered per target (not drawn by the tool) |
| fx_130 | 0x2 | `FX_PROPELLER_WIND_TEAL_RING` | render | emitter: orbiting dark-to-teal gouraud rings |
| fx_130 | 0x3 | `FX_PROPELLER_WIND_BLUE_CRESCENT` | render | emitter: blue crescent then ring particles (ring texture partly not in the file) |
| fx_130 | 0x4 | `FX_PROPELLER_WIND_WIND_RING` | render | emitter: orbiting grey wind ring |
| fx_130 | 0x5 | `FX_PROPELLER_WIND_WIND_SWIRL` | render | emitter: three-armed grey wind swirl |
| fx_131 | 0x1 | `FX_GOLEM_REVIVAL_PINK_GLOW` | render | emitter: pink glow with a green ring, fan |
| fx_131 | 0x2 | `FX_GOLEM_REVIVAL_SAND_CLOUD` | render | emitter: orbiting pale sand clouds |
| fx_132 | 0x1 | `FX_APPEARANCE_SMOKE_PUFF` | render | emitter: pale yellow-green smoke puffs |
| fx_132 | 0x2 | `FX_APPEARANCE_SPARKLE_HELIX` | kind/usage (look pending) | emitter: helix of small sprites (texture not in the file) |
| fx_133 | 0x1 | `FX_DISAPPEAR_SMOKE_PUFF` | render | emitter: pale smoke puffs (file 0x133; file 0x1B5 has coloured puffs and a "Dummy" letter sprite here) |
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
| fx_134 | 0x13 | `FX_DOOMSDAY_GLOW_DISC` | render | model: gouraud disc (46 polys) |
| fx_134 | 0x14 | `FX_DOOMSDAY_BIG_METEOR_2` | render | model: large rock ellipsoid (324 polys, 15-bit rock texture) |
| fx_134 | 0x15 | `FX_DOOMSDAY_EMBERS_2` | render | emitter: small yellow ember sprites |
| fx_134 | 0x16 | `FX_DOOMSDAY_FLAME_BURST` | render | emitter: orange flame puffs |
| fx_134 | 0x18 | `FX_DOOMSDAY_BLUE_PLANET` | render | sprite: pale blue planet |
| fx_134 | 0x19 | `FX_DOOMSDAY_LIGHT_CONE` | render | model: open cone with a grey streak texture |
| fx_134 | 0x1A | `FX_DOOMSDAY_LILAC_RING` | render | rings: white-lilac gouraud rings |
| fx_134 | 0x1B | `FX_DOOMSDAY_BLUE_RING` | render | rings: white-cyan-blue gouraud rings, four segments |
| fx_134 | 0x1E | `FX_DOOMSDAY_PURPLE_STREAK` | render | trail: purple streak (fxm_drawOffsetTrail) |
| fx_134 | 0x1F | `FX_DOOMSDAY_SPACE_LAYER` | kind/usage (look pending) | sprite: purple shapes and noise (space background layer; texture mix unclear) |
| fx_134 | 0x20 | `FX_DOOMSDAY_RED_PLANET` | render | sprite: small red planet |
| fx_134 | 0x21 | `FX_DOOMSDAY_GREY_SPARKS` | render | emitter: small grey spark sprites |
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
| fx_138 | 0xE | `FX_LUCKY_SEVEN_SPRITE_LINE` | kind/usage (look pending) | sprite line: small coloured sprites, drawn frames 0-0xC |
| fx_138 | 0x1B | `FX_LUCKY_SEVEN_SLOT_MODEL` | kind/usage (look pending) | model: 15 polys, tpage 0x9A (texture not in the file), drawn with the three reels |
| fx_138 | 0x2A | `FX_LUCKY_SEVEN_RAINBOW_TRAIL` | render | emitter: orbiting rainbow trail with glow sprites and rainbow rings |
| fx_179 | 0x1 | `FX_MEDEO_WHITE_RING` | render | emitter: white and lilac gouraud rings |
| fx_179 | 0x2 | `FX_MEDEO_METEOR` | render | model: rock (128 polys, red-brown rock texture) |
| fx_179 | 0x4 | `FX_MEDEO_COLOR_SMOKE` | render | emitter: spiral of yellow, purple, cyan and green smoke puffs |
| fx_179 | 0x5 | `FX_MEDEO_GLOW_TRAIL` | render | emitter: blue and green glow screen trails |
| fx_179 | 0x7 | `FX_MEDEO_SPARKLES` | render | emitter: small green and blue spark sprites |
| fx_179 | 0x8 | `FX_MEDEO_STAR_FLASH` | render | emitter: blue and green star flashes |
| fx_179 | 0x9 | `FX_MEDEO_FAN_EMITTER` | kind/usage (look pending) | emitter: three fan elements (not rendered) + sprites that render transparent |
| fx_179 | 0xA | `FX_MEDEO_COLOR_SMOKE_2` | render | emitter: spiral of yellow, purple, cyan and green smoke puffs |
| fx_179 | 0xB | `FX_MEDEO_SPRITE_BURST` | kind/usage (look pending) | emitter: two sprite elements, render all transparent |
| fx_17a | 0x1 | `FX_MADEEN_BLUE_RING` | render | rings: black-to-blue gouraud rings |
| fx_17a | 0x2 | `FX_MADEEN_BLUE_GLOW` | render | particles: blue glow (pool task) |
| fx_17a | 0x3 | `FX_MADEEN_WHITE_RING` | render | rings: white-blue gouraud rings |
| fx_17a | 0x4 | `FX_MADEEN_VIOLET_RING` | render | rings: white-cyan-violet gouraud rings, four segments |
| fx_17a | 0x5 | `FX_MADEEN_TEXTURED_RING` | render | rings: ring with a pink-green noise texture |
| fx_17a | 0x6 | `FX_MADEEN_TEAL_RING` | render | rings: black-blue-teal gouraud rings |
| fx_17a | 0x7 | `FX_MADEEN_RAINBOW_ARCH` | render | model: arch with a rainbow aura texture |
| fx_17a | 0x8 | `FX_MADEEN_WINGS` | render | model: three-armed wing shape with a grey feather texture |
| fx_17a | 0x9 | `FX_MADEEN_INFINITY_LOOP` | kind/usage (look pending) | model: figure-eight band (15-bit texture page 0x138, not resolved) |
| fx_17a | 0xA | `FX_MADEEN_RAINBOW_DOME` | render | model: banded dome with a rainbow strip texture |
| fx_17a | 0xB | `FX_MADEEN_GLOW_LINE` | render | sprite line: green, red, orange and blue ring glows |
| fx_17a | 0xC | `FX_MADEEN_GLOW_LINE_2` | render | sprite line: green, red, orange and blue ring glows |
| fx_17a | 0xD | `FX_MADEEN_FAN_EMITTER` | kind/usage (look pending) | emitter: three fan elements (not rendered) |
| fx_17a | 0xE | `FX_MADEEN_WHITE_GLOW` | render | particles: white-blue glow |
| fx_17a | 0xF | `FX_MADEEN_BLUE_SPARKLES` | render | emitter: blue square sparkles and gold sprites |
| fx_17a | 0x11 | `FX_MADEEN_TRAIL` | kind/usage (look pending) | trail: strip texture not in the file |
| fx_17a | 0x12 | `FX_MADEEN_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop |
| fx_17a | 0x13 | `FX_MADEEN_BLUE_STREAKS` | render | emitter: orbiting blue light streaks |
| fx_17e | 0x2 | `FX_CHESTNUT_IMPACT_RINGS` | render | emitter: white-pink-red-yellow and white-grey gouraud rings + spark sprites (texture not in the file) |
| fx_17e | 0x9 | `FX_CHESTNUT_CHESTNUT_MODEL` | kind/usage (look pending) | 'so' model (not drawn by the tool), probably the thrown chestnut |
| fx_17f | 0x1 | `FX_MELT_PINK_SMOKE` | render | emitter: pink smoke puffs |
| fx_17f | 0x2 | `FX_MELT_GOO_BLOB` | render | emitter: purple goo blobs |
| fx_17f | 0x3 | `FX_MELT_GOO_BLOB_2` | render | emitter: small dark purple goo blobs |
| fx_17f | 0x4 | `FX_MELT_GOO_RIPPLE` | render | emitter: pink ripple disc |
| fx_17f | 0xB | `FX_MELT_GOO_SPLASH` | render | emitter: glossy purple goo splash |

## Open questions (emulator)

Each wants one screenshot of the effect at the frame the C starts the entry (battle save state, cast or provoke the skill; `docs/emulator.md`), or a VRAM dump while it plays:

| module | index | name | why the look is pending |
|---|---|---|---|
| fx_12b | 0x9 | `FX_PHOTON_FAN_EMITTER` | emitter: orbiting fan element (not rendered) |
| fx_12b | 0x23 | `FX_PHOTON_SPARKS` | emitter: spark sprites (texture not in the file) |
| fx_12c | 0x1 | `FX_EARTHQUAKE_GROUND_SLAB_8` | 'so' model: bouncing ground piece, fx_12c_bounceModels[8] (drawn over the offscreen background copy) |
| fx_12c | 0x2 | `FX_EARTHQUAKE_GROUND_SLAB_1` | 'so' model: bouncing ground piece, fx_12c_bounceModels[1] (drawn over the offscreen background copy) |
| fx_12c | 0x3 | `FX_EARTHQUAKE_GROUND_SLAB_2` | 'so' model: bouncing ground piece, fx_12c_bounceModels[2] (drawn over the offscreen background copy) |
| fx_12c | 0x4 | `FX_EARTHQUAKE_GROUND_SLAB_3` | 'so' model: bouncing ground piece, fx_12c_bounceModels[3] (drawn over the offscreen background copy) |
| fx_12c | 0x5 | `FX_EARTHQUAKE_GROUND_SLAB_4` | 'so' model: bouncing ground piece, fx_12c_bounceModels[4] (drawn over the offscreen background copy) |
| fx_12c | 0x6 | `FX_EARTHQUAKE_GROUND_SLAB_5` | 'so' model: bouncing ground piece, fx_12c_bounceModels[5] (drawn over the offscreen background copy) |
| fx_12c | 0x7 | `FX_EARTHQUAKE_GROUND_SLAB_6` | 'so' model: bouncing ground piece, fx_12c_bounceModels[6] (drawn over the offscreen background copy) |
| fx_12c | 0x8 | `FX_EARTHQUAKE_GROUND_SLAB_7` | 'so' model: bouncing ground piece, fx_12c_bounceModels[7] (drawn over the offscreen background copy) |
| fx_12c | 0x9 | `FX_EARTHQUAKE_GROUND_SLAB_0` | 'so' model: bouncing ground piece, fx_12c_bounceModels[0] (drawn over the offscreen background copy) |
| fx_12c | 0xE | `FX_EARTHQUAKE_GROUND_SLAB_9` | 'so' model: bouncing ground piece, fx_12c_bounceModels[9] (drawn over the offscreen background copy) |
| fx_130 | 0x1 | `FX_PROPELLER_WIND_TARGET_MODEL` | 'so' model registered per target (not drawn by the tool) |
| fx_132 | 0x2 | `FX_APPEARANCE_SPARKLE_HELIX` | emitter: helix of small sprites (texture not in the file) |
| fx_134 | 0x1F | `FX_DOOMSDAY_SPACE_LAYER` | sprite: purple shapes and noise (space background layer; texture mix unclear) |
| fx_138 | 0xE | `FX_LUCKY_SEVEN_SPRITE_LINE` | sprite line: small coloured sprites, drawn frames 0-0xC |
| fx_138 | 0x1B | `FX_LUCKY_SEVEN_SLOT_MODEL` | model: 15 polys, tpage 0x9A (texture not in the file), drawn with the three reels |
| fx_179 | 0x9 | `FX_MEDEO_FAN_EMITTER` | emitter: three fan elements (not rendered) + sprites that render transparent |
| fx_179 | 0xB | `FX_MEDEO_SPRITE_BURST` | emitter: two sprite elements, render all transparent |
| fx_17a | 0x9 | `FX_MADEEN_INFINITY_LOOP` | model: figure-eight band (15-bit texture page 0x138, not resolved) |
| fx_17a | 0xD | `FX_MADEEN_FAN_EMITTER` | emitter: three fan elements (not rendered) |
| fx_17a | 0x11 | `FX_MADEEN_TRAIL` | trail: strip texture not in the file |
| fx_17e | 0x9 | `FX_CHESTNUT_CHESTNUT_MODEL` | 'so' model (not drawn by the tool), probably the thrown chestnut |
