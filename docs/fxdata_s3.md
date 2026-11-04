# Battle effect data, slice 3 (fx_0d9 .. fx_0f2): Sword Magic, summons, enemy skills

Names for the `btlfx_getFileData` literals of these modules, by the scheme and tool in `docs/fxdata.md`.

- **Sword Magic** (Steiner, command "Swd Mag"): the game text names these abilities like the spells (ability
  0x041 'Thundaga', 0x04b 'Bio', ...), so the prefix is `FX_<SPELL>_SWORD_` to keep them apart from the spells'
  own modules. Entries byte-identical to a named spell module (Thundaga fx_018, Blizzard fx_006, Blizzara
  fx_004, Blizzaga fx_097) take that name with the prefix swapped. Every Sword Magic module has op84, which flashes
  two white ring sets at the weapon (`btlfx_getWeaponPos(0x10, 1)`) as the spell starts: the 6-segment set is
  `_BLADE_GLINT`, the rotating 8-segment set `_BLADE_FLASH`.
- Evidence column: `render` = named by the `tools/fxdata.py --render` contact sheet; `usage` = a data table
  named by what the C does with it (settled); `kind/usage (look pending)` = appearance not settled (open
  questions below); `byte-identical` = the entry's bytes equal a named entry of another module.

## Tables

### fx_0d9: Sword Magic Thundaga (FX_THUNDAGA_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0xD | `FX_THUNDAGA_SWORD_DISC` | kind/usage (look pending), byte-identical | model: flat octagon, 15-bit texture page 0x119 (byte-identical to fx_018 entry 0x10) |
| 0xE | `FX_THUNDAGA_SWORD_GLOW_DOME` | render, byte-identical | model: hemisphere with an orange-to-white streak texture (byte-identical to fx_018 entry 0x11) |
| 0x10 | `FX_THUNDAGA_SWORD_BLUE_RING` | render | rings: white-blue gouraud rings |
| 0x12 | `FX_THUNDAGA_SWORD_WHITE_RING` | render | rings: white gouraud rings drawn flat |
| 0x14 | `FX_THUNDAGA_SWORD_LIGHTNING_BOLT` | render, byte-identical | particles: wide blue-white lightning bolt (byte-identical to fx_018 entry 0x17) |
| 0x18 | `FX_THUNDAGA_SWORD_SPARK_BALL` | render, byte-identical | particles: golden plasma ball, 8 frames (byte-identical to fx_018 entry 0x1B) |
| 0x1E | `FX_THUNDAGA_SWORD_VIOLET_RING` | render | rings: white-violet gouraud rings |
| 0x20 | `FX_THUNDAGA_SWORD_THIN_BOLT` | render, byte-identical | particles: thin cyan lightning bolt (byte-identical to fx_018 entry 0x20) |
| 0x21 | `FX_THUNDAGA_SWORD_FAN_EMITTER` | kind/usage (look pending), byte-identical | emitter: four fan elements (not rendered) (byte-identical to fx_018 entry 0x21) |
| 0x22 | `FX_THUNDAGA_SWORD_FAN_EMITTER_2` | kind/usage (look pending), byte-identical | emitter: four fan elements (not rendered) (byte-identical to fx_018 entry 0x22) |
| 0x25 | `FX_THUNDAGA_SWORD_IMPACT_FLASH` | render, byte-identical | emitter: four-point star flash (byte-identical to fx_018 entry 0x25) |
| 0x26 | `FX_THUNDAGA_SWORD_CONE_EMITTER` | kind/usage (look pending), byte-identical | emitter: cone of sprites, render all transparent (byte-identical to fx_018 entry 0x26) |
| 0x27 | `FX_THUNDAGA_SWORD_SPARKS` | kind/usage (look pending), byte-identical | emitter: spark sprites (texture not in the file) (byte-identical to fx_018 entry 0x27) |
| 0x28 | `FX_THUNDAGA_SWORD_BLADE_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0x29 | `FX_THUNDAGA_SWORD_BLADE_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |

### fx_0da: Sword Magic Blizzard (FX_BLIZZARD_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_BLIZZARD_SWORD_ICE_FLARES` | render, byte-identical | emitter: cyan ice glint / light-streak frames (byte-identical to fx_006 entry 0x1) |
| 0x2 | `FX_BLIZZARD_SWORD_ICE_REFLECTION` | render, byte-identical | model: ice shell drawn with the scrolling reflection map (byte-identical to fx_006 entry 0x2) |
| 0x3 | `FX_BLIZZARD_SWORD_ICE_BLOCK` | render, byte-identical | model: ice block (cracked-ice texture), morph start (byte-identical to fx_006 entry 0x3) |
| 0x4 | `FX_BLIZZARD_SWORD_ICE_CRACKING` | render, byte-identical | model: the block breaking into pieces, morph middle (byte-identical to fx_006 entry 0x4) |
| 0x5 | `FX_BLIZZARD_SWORD_ICE_SHARDS` | render, byte-identical | model: scattered ice shards, morph end (byte-identical to fx_006 entry 0x5) |
| 0x6 | `FX_BLIZZARD_SWORD_ICE_FLARES_2` | render, byte-identical | emitter: cyan ice glints (byte-identical to fx_006 entry 0x6) |
| 0xD | `FX_BLIZZARD_SWORD_FROST_RING` | render, byte-identical | emitter: lilac-blue gouraud rings (byte-identical to fx_006 entry 0xD) |
| 0xE | `FX_BLIZZARD_SWORD_FREEZE_BURST` | render, byte-identical | emitter: white-pink rings + spark sprites as the ice forms (byte-identical to fx_006 entry 0xE) |
| 0x10 | `FX_BLIZZARD_SWORD_SHATTER_WAVE` | render, byte-identical | emitter: blue ripple wave when the ice shatters (byte-identical to fx_006 entry 0x10) |
| 0x13 | `FX_BLIZZARD_SWORD_BLADE_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |
| 0x14 | `FX_BLIZZARD_SWORD_BLADE_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |

### fx_0db: Sword Magic Blizzara (FX_BLIZZARA_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0xB | `FX_BLIZZARA_SWORD_ICE_REFLECTION` | render, byte-identical | model: crystal shell drawn with the reflection map (byte-identical to fx_004 entry 0xB) |
| 0xD | `FX_BLIZZARA_SWORD_FROST_MIST` | render, byte-identical | emitter: grey frost mist puffs (byte-identical to fx_004 entry 0xD) |
| 0xE | `FX_BLIZZARA_SWORD_SHARD_MIST` | render, byte-identical | emitter: grey mist trailing each flying shard (pool item) (byte-identical to fx_004 entry 0xE) |
| 0xF | `FX_BLIZZARA_SWORD_ICE_FLARES_2` | render, byte-identical | emitter: cyan ice glints (byte-identical to fx_004 entry 0xF) |
| 0x10 | `FX_BLIZZARA_SWORD_ICE_FLARES` | render, byte-identical | emitter: cyan ice glints, four elements (byte-identical to fx_004 entry 0x10) |
| 0x11 | `FX_BLIZZARA_SWORD_FREEZE_BURST` | render, byte-identical | emitter: white-pink rings + spark sprites (byte-identical to fx_004 entry 0x11) |
| 0x12 | `FX_BLIZZARA_SWORD_FROST_RING` | render, byte-identical | emitter: lilac-blue gouraud rings (byte-identical to fx_004 entry 0x12) |
| 0x13 | `FX_BLIZZARA_SWORD_ICE_CRYSTAL` | render, byte-identical | model: long ice crystal spike (ice texture), splits in three (byte-identical to fx_004 entry 0x13) |
| 0x14 | `FX_BLIZZARA_SWORD_WHITE_RING` | render, byte-identical | emitter: white gouraud ring at the start (byte-identical to fx_004 entry 0x14) |
| 0x15 | `FX_BLIZZARA_SWORD_CRYSTAL_PIECE_0` | render, byte-identical | model: first piece of the split crystal (byte-identical to fx_004 entry 0x15) |
| 0x16 | `FX_BLIZZARA_SWORD_CRYSTAL_PIECE_1` | render, byte-identical | model: second piece of the split crystal (byte-identical to fx_004 entry 0x16) |
| 0x17 | `FX_BLIZZARA_SWORD_CRYSTAL_PIECE_2` | render, byte-identical | model: third piece of the split crystal (byte-identical to fx_004 entry 0x17) |
| 0x19 | `FX_BLIZZARA_SWORD_SHATTER_WAVE` | render, byte-identical | emitter: blue ripple wave (byte-identical to fx_004 entry 0x19) |
| 0x1A | `FX_BLIZZARA_SWORD_BLADE_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |
| 0x1B | `FX_BLIZZARA_SWORD_BLADE_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |

### fx_0dc: Sword Magic Blizzaga (FX_BLIZZAGA_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0xF | `FX_BLIZZAGA_SWORD_CRYSTAL_SHEEN_2` | render, byte-identical | model: crystal 2 shape, reflection-map texture (byte-identical to fx_097 entry 0xF) |
| 0x10 | `FX_BLIZZAGA_SWORD_CRYSTAL_SHEEN_3` | render, byte-identical | model: crystal 3 shape, reflection-map texture (byte-identical to fx_097 entry 0x10) |
| 0x11 | `FX_BLIZZAGA_SWORD_CRYSTAL_SHEEN` | render, byte-identical | model: main crystal shape, reflection-map texture (byte-identical to fx_097 entry 0x11) |
| 0x12 | `FX_BLIZZAGA_SWORD_ICE_CRYSTAL_2` | render, byte-identical | model: ice crystal spike (hexagonal, ice texture) (byte-identical to fx_097 entry 0x12) |
| 0x13 | `FX_BLIZZAGA_SWORD_ICE_CRYSTAL_3` | render, byte-identical | model: wide ice crystal spike (byte-identical to fx_097 entry 0x13) |
| 0x14 | `FX_BLIZZAGA_SWORD_ICE_CRYSTAL` | render, byte-identical | model: main ice crystal spike (byte-identical to fx_097 entry 0x14) |
| 0x1B | `FX_BLIZZAGA_SWORD_FROST_RING` | render | rings: blue gouraud ring on the ground |
| 0x1C | `FX_BLIZZAGA_SWORD_FAN_EMITTER` | kind/usage (look pending), byte-identical | emitter: four fan elements (not rendered) (byte-identical to fx_097 entry 0x1C) |
| 0x1D | `FX_BLIZZAGA_SWORD_SNOW_BURST` | render, byte-identical | emitter: rings textured with snow flakes (byte-identical to fx_097 entry 0x1D) |
| 0x1E | `FX_BLIZZAGA_SWORD_SHATTER_WAVE` | render, byte-identical | emitter: blue ripple wave (byte-identical to fx_097 entry 0x1E) |
| 0x1F | `FX_BLIZZAGA_SWORD_ICE_FLARES_2` | render, byte-identical | emitter: cyan ice glints (byte-identical to fx_097 entry 0x1F) |
| 0x20 | `FX_BLIZZAGA_SWORD_ICE_FLARES` | render, byte-identical | emitter: cyan ice glints, four elements (byte-identical to fx_097 entry 0x20) |
| 0x21 | `FX_BLIZZAGA_SWORD_FREEZE_BURST` | render, byte-identical | emitter: white-pink rings + spark sprites (byte-identical to fx_097 entry 0x21) |
| 0x22 | `FX_BLIZZAGA_SWORD_FROST_RING_2` | render, byte-identical | emitter: pale blue gouraud rings (byte-identical to fx_097 entry 0x22) |
| 0x23 | `FX_BLIZZAGA_SWORD_SNOW_RING` | render, byte-identical | rings: textured ring of blue snow flakes (byte-identical to fx_097 entry 0x23) |
| 0x24 | `FX_BLIZZAGA_SWORD_SNOW_RING_2` | render, byte-identical | rings: textured ring of pale snow flakes (byte-identical to fx_097 entry 0x24) |
| 0x25 | `FX_BLIZZAGA_SWORD_SHARD_MIST` | render, byte-identical | emitter: grey mist trailing each shard (pool item) (byte-identical to fx_097 entry 0x25) |
| 0x26 | `FX_BLIZZAGA_SWORD_MIST` | render, byte-identical | emitter: grey frost mist at the crystal base (byte-identical to fx_097 entry 0x26) |
| 0x27 | `FX_BLIZZAGA_SWORD_MIST_HIGH` | render, byte-identical | emitter: grey frost mist higher up (byte-identical to fx_097 entry 0x27) |
| 0x28 | `FX_BLIZZAGA_SWORD_BLADE_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |
| 0x29 | `FX_BLIZZAGA_SWORD_BLADE_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0x2A | `FX_BLIZZAGA_SWORD_ICE_SPARKLES_2` | render, byte-identical | emitter: small blue ice sparkles while the crystal grows (byte-identical to fx_097 entry 0x29) |
| 0x2B | `FX_BLIZZAGA_SWORD_ICE_SPARKLES` | render, byte-identical | emitter: small blue ice sparkles at the shatter (byte-identical to fx_097 entry 0x28) |

### fx_0dd: Sword Magic Bio (FX_BIO_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_BIO_SWORD_BUBBLE_PATH` | usage | table: rotation/position keys of the morphing bubble (fxm_rotPosTrackInit) |
| 0x2 | `FX_BIO_SWORD_BUBBLE_SCALE` | usage | table: scale keys of the morphing bubble (fxm_vecTrackInit) |
| 0xF | `FX_BIO_SWORD_BUBBLE_SCALE_2` | usage | table: scale keys of the second bubble pass (fxm_vecTrackInit) |
| 0x10 | `FX_BIO_SWORD_BUBBLE_PATH_2` | usage | table: rotation/position keys of the second bubble pass |
| 0x11 | `FX_BIO_SWORD_SCREEN_TRAIL` | kind/usage (look pending) | emitter: orbiting screen-space trail, texture not in the file |
| 0x12 | `FX_BIO_SWORD_GREEN_WISPS` | render | emitter: teal-green wisp rings |
| 0x13 | `FX_BIO_SWORD_GREEN_CLOUD` | render | emitter: green gas cloud + grey haze |
| 0x14 | `FX_BIO_SWORD_RED_CELLS` | render | emitter: red cell-mesh balls with a crimson glow |
| 0x15 | `FX_BIO_SWORD_BLADE_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0x16 | `FX_BIO_SWORD_BLADE_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |

### fx_0de: Sword Magic Water (FX_WATER_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_WATER_SWORD_DROPLETS` | render | emitter: water blobs, blue glow and glints |
| 0x2 | `FX_WATER_SWORD_WATER_BALL_1` | render | model: water funnel rising, morph stage 1 |
| 0x3 | `FX_WATER_SWORD_WATER_BALL_2` | render | model: water vase shape, morph stage 2 |
| 0x4 | `FX_WATER_SWORD_WATER_BALL_3` | render | model: water bulb closing, morph stage 3 |
| 0x5 | `FX_WATER_SWORD_WATER_BALL_0` | render | model: flat water pool (water texture), morph stage 0 |
| 0x6 | `FX_WATER_SWORD_SPRAY_FUNNEL` | render | model: swirling funnel, mist texture, morph end of the spray |
| 0x7 | `FX_WATER_SWORD_SPRAY_COLUMN` | render | model: open cylinder, mist texture, morph start of the spray |
| 0x8 | `FX_WATER_SWORD_WATER_SURFACE` | render | model: flat disc, water texture |
| 0x9 | `FX_WATER_SWORD_BLUE_RING` | render | emitter: white-blue gouraud rings |
| 0xD | `FX_WATER_SWORD_WATER_BALL` | render | model: closed water sphere, morph end |
| 0xE | `FX_WATER_SWORD_BLADE_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0xF | `FX_WATER_SWORD_BLADE_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |

### fx_0df: Sword Magic Flare (FX_FLARE_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x3 | `FX_FLARE_SWORD_RED_RINGS` | render | rings: white-pink-red gouraud rings |
| 0x4 | `FX_FLARE_SWORD_BLAST_RINGS` | render | rings: white-cyan-orange-red gouraud rings, the blast |
| 0x5 | `FX_FLARE_SWORD_FIRE_DOME` | render | model: dome with a fire texture |
| 0x6 | `FX_FLARE_SWORD_EMBERS` | render | emitter: red ember sprites + orange glow |
| 0x8 | `FX_FLARE_SWORD_SHOCK_RINGS` | render | emitter: white-crimson / white-violet gouraud rings |
| 0x9 | `FX_FLARE_SWORD_FAN_EMITTER` | kind/usage (look pending) | emitter: two fan elements (not rendered) |
| 0xB | `FX_FLARE_SWORD_BACKDROP` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0xC | `FX_FLARE_SWORD_SPHERE` | kind/usage (look pending) | model: sphere, texture page 0x13C not in the file |
| 0xD | `FX_FLARE_SWORD_GOURAUD_RING` | kind/usage (look pending) | model: flat gouraud annulus (colours not rendered) |
| 0xE | `FX_FLARE_SWORD_BACKDROP_2` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0x10 | `FX_FLARE_SWORD_GLOW` | render | particles: orange glow with a white core |
| 0x11 | `FX_FLARE_SWORD_EMBERS_2` | render | emitter: red ember sprites |
| 0x12 | `FX_FLARE_SWORD_ARCH` | kind/usage (look pending) | model: arch, 15-bit texture page 0x139 |
| 0x13 | `FX_FLARE_SWORD_WHITE_RINGS` | render | rings: white-cream gouraud rings |
| 0x18 | `FX_FLARE_SWORD_BACKDROP_3` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0x19 | `FX_FLARE_SWORD_TEX_RING` | kind/usage (look pending) | rings: textured ring, texture not in the file |
| 0x1A | `FX_FLARE_SWORD_TEX_RING_2` | kind/usage (look pending) | rings: textured ring, texture not in the file |
| 0x1B | `FX_FLARE_SWORD_BLUE_BOLT` | render | trail: blue lightning strip (fxm_drawOffsetTrail) |
| 0x1E | `FX_FLARE_SWORD_BLADE_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0x1F | `FX_FLARE_SWORD_BLADE_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |

### fx_0e0: Sword Magic Doomsday (FX_DOOMSDAY_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x8 | `FX_DOOMSDAY_SWORD_ROCK` | render | model: polyhedral rock (rock texture) |
| 0x9 | `FX_DOOMSDAY_SWORD_DUST_RING` | render | rings: white ring fading through cream to grey, 8 frames |
| 0xA | `FX_DOOMSDAY_SWORD_FIRE_RING` | render | rings: white-orange / yellow rings, 16 frames |
| 0xB | `FX_DOOMSDAY_SWORD_BOULDER` | render | model: egg-shaped boulder (rock texture) |
| 0xD | `FX_DOOMSDAY_SWORD_EMBERS` | render | emitter: spiral of tiny orange sprites |
| 0x11 | `FX_DOOMSDAY_SWORD_STAR_FUNNEL` | render | model: three-bladed funnel with a starfield texture |
| 0x12 | `FX_DOOMSDAY_SWORD_STAR_DOME` | render | model: arch / dome with a starfield texture |
| 0x13 | `FX_DOOMSDAY_SWORD_GOURAUD_DOME` | kind/usage (look pending) | model: shallow gouraud dome (colours not rendered) |
| 0x14 | `FX_DOOMSDAY_SWORD_BOULDER_2` | render | model: rough egg-shaped boulder (rock texture) |
| 0x19 | `FX_DOOMSDAY_SWORD_CYLINDER` | kind/usage (look pending) | model: open cylinder, texture not in the file |
| 0x1A | `FX_DOOMSDAY_SWORD_LILAC_RINGS` | render | rings: white-lilac gouraud rings at the centre |
| 0x1B | `FX_DOOMSDAY_SWORD_BLUE_RINGS` | render | rings: white-cyan-blue gouraud rings at the centre |
| 0x1C | `FX_DOOMSDAY_SWORD_BACKDROP` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0x1D | `FX_DOOMSDAY_SWORD_PURPLE_BOLT` | render | trail: purple lightning strip (fxm_drawOffsetTrail) |
| 0x20 | `FX_DOOMSDAY_SWORD_PEBBLES` | render | emitter: spiral of small grey pebble sprites |
| 0x21 | `FX_DOOMSDAY_SWORD_PARTICLE_EMITTER` | kind/usage (look pending) | emitter: four particle elements, texture not in the file |
| 0x22 | `FX_DOOMSDAY_SWORD_BLADE_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |
| 0x23 | `FX_DOOMSDAY_SWORD_BLADE_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |

## Open questions (emulator)

Each wants one screenshot of the effect at the frame the C starts the entry, or a VRAM dump while it
plays (textures not in the file), as in `docs/fxdata.md`.

- fx_0d9: `FX_THUNDAGA_SWORD_DISC` (model: flat octagon, 15-bit texture page 0x119 (byte-identical to fx_018 entry 0x10)); `FX_THUNDAGA_SWORD_FAN_EMITTER` (emitter: four fan elements (not rendered) (byte-identical to fx_018 entry 0x21)); `FX_THUNDAGA_SWORD_FAN_EMITTER_2` (emitter: four fan elements (not rendered) (byte-identical to fx_018 entry 0x22)); `FX_THUNDAGA_SWORD_CONE_EMITTER` (emitter: cone of sprites, render all transparent (byte-identical to fx_018 entry 0x26)); `FX_THUNDAGA_SWORD_SPARKS` (emitter: spark sprites (texture not in the file) (byte-identical to fx_018 entry 0x27))
- fx_0dc: `FX_BLIZZAGA_SWORD_FAN_EMITTER` (emitter: four fan elements (not rendered) (byte-identical to fx_097 entry 0x1C))
- fx_0dd: `FX_BIO_SWORD_SCREEN_TRAIL` (emitter: orbiting screen-space trail, texture not in the file)
- fx_0df: `FX_FLARE_SWORD_FAN_EMITTER` (emitter: two fan elements (not rendered)); `FX_FLARE_SWORD_BACKDROP` (backdrop layers (btlfx_drawBackdrop, not rendered)); `FX_FLARE_SWORD_SPHERE` (model: sphere, texture page 0x13C not in the file); `FX_FLARE_SWORD_GOURAUD_RING` (model: flat gouraud annulus (colours not rendered)); `FX_FLARE_SWORD_BACKDROP_2` (backdrop layers (btlfx_drawBackdrop, not rendered)); `FX_FLARE_SWORD_ARCH` (model: arch, 15-bit texture page 0x139); `FX_FLARE_SWORD_BACKDROP_3` (backdrop layers (btlfx_drawBackdrop, not rendered)); `FX_FLARE_SWORD_TEX_RING` (rings: textured ring, texture not in the file); `FX_FLARE_SWORD_TEX_RING_2` (rings: textured ring, texture not in the file)
- fx_0e0: `FX_DOOMSDAY_SWORD_GOURAUD_DOME` (model: shallow gouraud dome (colours not rendered)); `FX_DOOMSDAY_SWORD_CYLINDER` (model: open cylinder, texture not in the file); `FX_DOOMSDAY_SWORD_BACKDROP` (backdrop layers (btlfx_drawBackdrop, not rendered)); `FX_DOOMSDAY_SWORD_PARTICLE_EMITTER` (emitter: four particle elements, texture not in the file)

20 look-pending names.
