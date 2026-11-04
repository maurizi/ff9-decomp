# Battle effect data, slice 3 (fx_0d9 .. fx_0f2): Sword Magic, summons, enemy skills

Names for the `btlfx_getFileData` literals of these modules, by the scheme and tool in `docs/fxdata.md`.

- **Sword Magic** (Steiner, command "Swd Mag"): the game text names these abilities like the spells (ability
  0x041 'Thundaga', 0x04b 'Bio', ...), so the prefix is `FX_<SPELL>_SWORD_` to keep them apart from the spells'
  own modules. Entries byte-identical to a named spell module (Thundaga fx_018, Blizzard fx_006, Blizzara
  fx_004, Blizzaga fx_097) take that name with the prefix swapped. Every Sword Magic module has op84, which flashes
  two white ring sets at the weapon (`btlfx_getWeaponPos(0x10, 1)`) as the spell starts: the 6-segment set is
  `_BLADE_GLINT`, the rotating 8-segment set `_BLADE_FLASH`.
- **Summons** (Rebirth Flame, Fenrir, Bahamut): indices 0x80+ read the summon data
  (`g_btlfxLoader.summon->data[0]`), which is the file's own type-6 chunk (group 0), with the same offset table
  layout as a slot. `fxdata.py` does not split it; the helper used here (not committed) appends that chunk as an
  extra slot and runs `fxdata.classify` / `render_entry` on it with the C's 0x80+ uses. Its texture pages come
  with the summon load, so its models render as shape only. All groups of a summon file share it (Bahamut's
  0x85 is the same entry in fx_0e3_0 and fx_0e3_1). Most summon entries are `'so'` models, which the tool does not
  draw; they are named by what the C does with them (morph source/target, drawn stacked as a cone, ...).
- Tool note: `fxdata.py` crashes in `classify` (`u.get('call', '').startswith`) on a use record whose call is
  `None` (seen with fx_0e2's summon-data uses and in the fx_0e4..fx_0f2 run); the runs here wrapped `c_usage` to map `None` to `''`.
- Enemy-only modules use the enemy skill name from docs/effect-modules.md (Stone, Escape, Psychokinesis, Mirror,
  Red Clipper, Zombie Powder).
- Evidence column: `render` = named by the `tools/fxdata.py --render` contact sheet; `usage` = a data table
  named by what the C does with it (settled); `kind/usage (look pending)` = appearance not settled (open
  questions below); `byte-identical` = the entry's bytes equal a named entry of another module.

## Tables

### fx_0d9: Sword Magic Thundaga (FX_THUNDAGA_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0xD | `FX_SWDMAG_THUNDAGA_DISC` | kind/usage (look pending), byte-identical | model: flat octagon, 15-bit texture page 0x119 (byte-identical to fx_018 entry 0x10) |
| 0xE | `FX_SWDMAG_THUNDAGA_GLOW_DOME` | render, byte-identical | model: hemisphere with an orange-to-white streak texture (byte-identical to fx_018 entry 0x11) |
| 0x10 | `FX_SWDMAG_THUNDAGA_BLUE_RING` | render | rings: white-blue gouraud rings |
| 0x12 | `FX_SWDMAG_THUNDAGA_WHITE_RING` | render | rings: white gouraud rings drawn flat |
| 0x14 | `FX_SWDMAG_THUNDAGA_LIGHTNING_BOLT` | render, byte-identical | particles: wide blue-white lightning bolt (byte-identical to fx_018 entry 0x17) |
| 0x18 | `FX_SWDMAG_THUNDAGA_SPARK_BALL` | render, byte-identical | particles: golden plasma ball, 8 frames (byte-identical to fx_018 entry 0x1B) |
| 0x1E | `FX_SWDMAG_THUNDAGA_VIOLET_RING` | render | rings: white-violet gouraud rings |
| 0x20 | `FX_SWDMAG_THUNDAGA_THIN_BOLT` | render, byte-identical | particles: thin cyan lightning bolt (byte-identical to fx_018 entry 0x20) |
| 0x21 | `FX_SWDMAG_THUNDAGA_FAN_EMITTER` | kind/usage (look pending), byte-identical | emitter: four fan elements (not rendered) (byte-identical to fx_018 entry 0x21) |
| 0x22 | `FX_SWDMAG_THUNDAGA_FAN_EMITTER_2` | kind/usage (look pending), byte-identical | emitter: four fan elements (not rendered) (byte-identical to fx_018 entry 0x22) |
| 0x25 | `FX_SWDMAG_THUNDAGA_IMPACT_FLASH` | render, byte-identical | emitter: four-point star flash (byte-identical to fx_018 entry 0x25) |
| 0x26 | `FX_SWDMAG_THUNDAGA_CONE_EMITTER` | kind/usage (look pending), byte-identical | emitter: cone of sprites, render all transparent (byte-identical to fx_018 entry 0x26) |
| 0x27 | `FX_SWDMAG_THUNDAGA_SPARKS` | kind/usage (look pending), byte-identical | emitter: spark sprites (texture not in the file) (byte-identical to fx_018 entry 0x27) |
| 0x28 | `FX_SWDMAG_THUNDAGA_SWORD_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0x29 | `FX_SWDMAG_THUNDAGA_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |

### fx_0da: Sword Magic Blizzard (FX_BLIZZARD_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_SWDMAG_BLIZZARD_ICE_FLARES` | render, byte-identical | emitter: cyan ice glint / light-streak frames (byte-identical to fx_006 entry 0x1) |
| 0x2 | `FX_SWDMAG_BLIZZARD_ICE_REFLECTION` | render, byte-identical | model: ice shell drawn with the scrolling reflection map (byte-identical to fx_006 entry 0x2) |
| 0x3 | `FX_SWDMAG_BLIZZARD_ICE_BLOCK` | render, byte-identical | model: ice block (cracked-ice texture), morph start (byte-identical to fx_006 entry 0x3) |
| 0x4 | `FX_SWDMAG_BLIZZARD_ICE_CRACKING` | render, byte-identical | model: the block breaking into pieces, morph middle (byte-identical to fx_006 entry 0x4) |
| 0x5 | `FX_SWDMAG_BLIZZARD_ICE_SHARDS` | render, byte-identical | model: scattered ice shards, morph end (byte-identical to fx_006 entry 0x5) |
| 0x6 | `FX_SWDMAG_BLIZZARD_ICE_FLARES_2` | render, byte-identical | emitter: cyan ice glints (byte-identical to fx_006 entry 0x6) |
| 0xD | `FX_SWDMAG_BLIZZARD_FROST_RING` | render, byte-identical | emitter: lilac-blue gouraud rings (byte-identical to fx_006 entry 0xD) |
| 0xE | `FX_SWDMAG_BLIZZARD_FREEZE_BURST` | render, byte-identical | emitter: white-pink rings + spark sprites as the ice forms (byte-identical to fx_006 entry 0xE) |
| 0x10 | `FX_SWDMAG_BLIZZARD_SHATTER_WAVE` | render, byte-identical | emitter: blue ripple wave when the ice shatters (byte-identical to fx_006 entry 0x10) |
| 0x13 | `FX_SWDMAG_BLIZZARD_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |
| 0x14 | `FX_SWDMAG_BLIZZARD_SWORD_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |

### fx_0db: Sword Magic Blizzara (FX_BLIZZARA_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0xB | `FX_SWDMAG_BLIZZARA_ICE_REFLECTION` | render, byte-identical | model: crystal shell drawn with the reflection map (byte-identical to fx_004 entry 0xB) |
| 0xD | `FX_SWDMAG_BLIZZARA_FROST_MIST` | render, byte-identical | emitter: grey frost mist puffs (byte-identical to fx_004 entry 0xD) |
| 0xE | `FX_SWDMAG_BLIZZARA_SHARD_MIST` | render, byte-identical | emitter: grey mist trailing each flying shard (pool item) (byte-identical to fx_004 entry 0xE) |
| 0xF | `FX_SWDMAG_BLIZZARA_ICE_FLARES_2` | render, byte-identical | emitter: cyan ice glints (byte-identical to fx_004 entry 0xF) |
| 0x10 | `FX_SWDMAG_BLIZZARA_ICE_FLARES` | render, byte-identical | emitter: cyan ice glints, four elements (byte-identical to fx_004 entry 0x10) |
| 0x11 | `FX_SWDMAG_BLIZZARA_FREEZE_BURST` | render, byte-identical | emitter: white-pink rings + spark sprites (byte-identical to fx_004 entry 0x11) |
| 0x12 | `FX_SWDMAG_BLIZZARA_FROST_RING` | render, byte-identical | emitter: lilac-blue gouraud rings (byte-identical to fx_004 entry 0x12) |
| 0x13 | `FX_SWDMAG_BLIZZARA_ICE_CRYSTAL` | render, byte-identical | model: long ice crystal spike (ice texture), splits in three (byte-identical to fx_004 entry 0x13) |
| 0x14 | `FX_SWDMAG_BLIZZARA_WHITE_RING` | render, byte-identical | emitter: white gouraud ring at the start (byte-identical to fx_004 entry 0x14) |
| 0x15 | `FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_0` | render, byte-identical | model: first piece of the split crystal (byte-identical to fx_004 entry 0x15) |
| 0x16 | `FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_1` | render, byte-identical | model: second piece of the split crystal (byte-identical to fx_004 entry 0x16) |
| 0x17 | `FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_2` | render, byte-identical | model: third piece of the split crystal (byte-identical to fx_004 entry 0x17) |
| 0x19 | `FX_SWDMAG_BLIZZARA_SHATTER_WAVE` | render, byte-identical | emitter: blue ripple wave (byte-identical to fx_004 entry 0x19) |
| 0x1A | `FX_SWDMAG_BLIZZARA_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |
| 0x1B | `FX_SWDMAG_BLIZZARA_SWORD_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |

### fx_0dc: Sword Magic Blizzaga (FX_BLIZZAGA_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0xF | `FX_SWDMAG_BLIZZAGA_CRYSTAL_SHEEN_2` | render, byte-identical | model: crystal 2 shape, reflection-map texture (byte-identical to fx_097 entry 0xF) |
| 0x10 | `FX_SWDMAG_BLIZZAGA_CRYSTAL_SHEEN_3` | render, byte-identical | model: crystal 3 shape, reflection-map texture (byte-identical to fx_097 entry 0x10) |
| 0x11 | `FX_SWDMAG_BLIZZAGA_CRYSTAL_SHEEN` | render, byte-identical | model: main crystal shape, reflection-map texture (byte-identical to fx_097 entry 0x11) |
| 0x12 | `FX_SWDMAG_BLIZZAGA_ICE_CRYSTAL_2` | render, byte-identical | model: ice crystal spike (hexagonal, ice texture) (byte-identical to fx_097 entry 0x12) |
| 0x13 | `FX_SWDMAG_BLIZZAGA_ICE_CRYSTAL_3` | render, byte-identical | model: wide ice crystal spike (byte-identical to fx_097 entry 0x13) |
| 0x14 | `FX_SWDMAG_BLIZZAGA_ICE_CRYSTAL` | render, byte-identical | model: main ice crystal spike (byte-identical to fx_097 entry 0x14) |
| 0x1B | `FX_SWDMAG_BLIZZAGA_FROST_RING` | render | rings: blue gouraud ring on the ground |
| 0x1C | `FX_SWDMAG_BLIZZAGA_FAN_EMITTER` | kind/usage (look pending), byte-identical | emitter: four fan elements (not rendered) (byte-identical to fx_097 entry 0x1C) |
| 0x1D | `FX_SWDMAG_BLIZZAGA_SNOW_BURST` | render, byte-identical | emitter: rings textured with snow flakes (byte-identical to fx_097 entry 0x1D) |
| 0x1E | `FX_SWDMAG_BLIZZAGA_SHATTER_WAVE` | render, byte-identical | emitter: blue ripple wave (byte-identical to fx_097 entry 0x1E) |
| 0x1F | `FX_SWDMAG_BLIZZAGA_ICE_FLARES_2` | render, byte-identical | emitter: cyan ice glints (byte-identical to fx_097 entry 0x1F) |
| 0x20 | `FX_SWDMAG_BLIZZAGA_ICE_FLARES` | render, byte-identical | emitter: cyan ice glints, four elements (byte-identical to fx_097 entry 0x20) |
| 0x21 | `FX_SWDMAG_BLIZZAGA_FREEZE_BURST` | render, byte-identical | emitter: white-pink rings + spark sprites (byte-identical to fx_097 entry 0x21) |
| 0x22 | `FX_SWDMAG_BLIZZAGA_FROST_RING_2` | render, byte-identical | emitter: pale blue gouraud rings (byte-identical to fx_097 entry 0x22) |
| 0x23 | `FX_SWDMAG_BLIZZAGA_SNOW_RING` | render, byte-identical | rings: textured ring of blue snow flakes (byte-identical to fx_097 entry 0x23) |
| 0x24 | `FX_SWDMAG_BLIZZAGA_SNOW_RING_2` | render, byte-identical | rings: textured ring of pale snow flakes (byte-identical to fx_097 entry 0x24) |
| 0x25 | `FX_SWDMAG_BLIZZAGA_SHARD_MIST` | render, byte-identical | emitter: grey mist trailing each shard (pool item) (byte-identical to fx_097 entry 0x25) |
| 0x26 | `FX_SWDMAG_BLIZZAGA_MIST` | render, byte-identical | emitter: grey frost mist at the crystal base (byte-identical to fx_097 entry 0x26) |
| 0x27 | `FX_SWDMAG_BLIZZAGA_MIST_HIGH` | render, byte-identical | emitter: grey frost mist higher up (byte-identical to fx_097 entry 0x27) |
| 0x28 | `FX_SWDMAG_BLIZZAGA_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |
| 0x29 | `FX_SWDMAG_BLIZZAGA_SWORD_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0x2A | `FX_SWDMAG_BLIZZAGA_ICE_SPARKLES_2` | render, byte-identical | emitter: small blue ice sparkles while the crystal grows (byte-identical to fx_097 entry 0x29) |
| 0x2B | `FX_SWDMAG_BLIZZAGA_ICE_SPARKLES` | render, byte-identical | emitter: small blue ice sparkles at the shatter (byte-identical to fx_097 entry 0x28) |

### fx_0dd: Sword Magic Bio (FX_BIO_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_SWDMAG_BIO_BUBBLE_PATH` | usage | table: rotation/position keys of the morphing bubble (fxm_rotPosTrackInit) |
| 0x2 | `FX_SWDMAG_BIO_BUBBLE_SCALE` | usage | table: scale keys of the morphing bubble (fxm_vecTrackInit) |
| 0xF | `FX_SWDMAG_BIO_BUBBLE_SCALE_2` | usage | table: scale keys of the second bubble pass (fxm_vecTrackInit) |
| 0x10 | `FX_SWDMAG_BIO_BUBBLE_PATH_2` | usage | table: rotation/position keys of the second bubble pass |
| 0x11 | `FX_SWDMAG_BIO_SCREEN_TRAIL` | kind/usage (look pending) | emitter: orbiting screen-space trail, texture not in the file |
| 0x12 | `FX_SWDMAG_BIO_GREEN_WISPS` | render | emitter: teal-green wisp rings |
| 0x13 | `FX_SWDMAG_BIO_GREEN_CLOUD` | render | emitter: green gas cloud + grey haze |
| 0x14 | `FX_SWDMAG_BIO_RED_CELLS` | render | emitter: red cell-mesh balls with a crimson glow |
| 0x15 | `FX_SWDMAG_BIO_SWORD_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0x16 | `FX_SWDMAG_BIO_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |

### fx_0de: Sword Magic Water (FX_WATER_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_SWDMAG_WATER_DROPLETS` | render | emitter: water blobs, blue glow and glints |
| 0x2 | `FX_SWDMAG_WATER_WATER_BALL_1` | render | model: water funnel rising, morph stage 1 |
| 0x3 | `FX_SWDMAG_WATER_WATER_BALL_2` | render | model: water vase shape, morph stage 2 |
| 0x4 | `FX_SWDMAG_WATER_WATER_BALL_3` | render | model: water bulb closing, morph stage 3 |
| 0x5 | `FX_SWDMAG_WATER_WATER_BALL_0` | render | model: flat water pool (water texture), morph stage 0 |
| 0x6 | `FX_SWDMAG_WATER_SPRAY_FUNNEL` | render | model: swirling funnel, mist texture, morph end of the spray |
| 0x7 | `FX_SWDMAG_WATER_SPRAY_COLUMN` | render | model: open cylinder, mist texture, morph start of the spray |
| 0x8 | `FX_SWDMAG_WATER_WATER_SURFACE` | render | model: flat disc, water texture |
| 0x9 | `FX_SWDMAG_WATER_BLUE_RING` | render | emitter: white-blue gouraud rings |
| 0xD | `FX_SWDMAG_WATER_WATER_BALL` | render | model: closed water sphere, morph end |
| 0xE | `FX_SWDMAG_WATER_SWORD_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0xF | `FX_SWDMAG_WATER_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |

### fx_0df: Sword Magic Flare (FX_FLARE_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x3 | `FX_SWDMAG_FLARE_RED_RINGS` | render | rings: white-pink-red gouraud rings |
| 0x4 | `FX_SWDMAG_FLARE_BLAST_RINGS` | render | rings: white-cyan-orange-red gouraud rings, the blast |
| 0x5 | `FX_SWDMAG_FLARE_FIRE_DOME` | render | model: dome with a fire texture |
| 0x6 | `FX_SWDMAG_FLARE_EMBERS` | render | emitter: red ember sprites + orange glow |
| 0x8 | `FX_SWDMAG_FLARE_SHOCK_RINGS` | render | emitter: white-crimson / white-violet gouraud rings |
| 0x9 | `FX_SWDMAG_FLARE_FAN_EMITTER` | kind/usage (look pending) | emitter: two fan elements (not rendered) |
| 0xB | `FX_SWDMAG_FLARE_BACKDROP` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0xC | `FX_SWDMAG_FLARE_SPHERE` | kind/usage (look pending) | model: sphere, texture page 0x13C not in the file |
| 0xD | `FX_SWDMAG_FLARE_GOURAUD_RING` | kind/usage (look pending) | model: flat gouraud annulus (colours not rendered) |
| 0xE | `FX_SWDMAG_FLARE_BACKDROP_2` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0x10 | `FX_SWDMAG_FLARE_GLOW` | render | particles: orange glow with a white core |
| 0x11 | `FX_SWDMAG_FLARE_EMBERS_2` | render | emitter: red ember sprites |
| 0x12 | `FX_SWDMAG_FLARE_ARCH` | kind/usage (look pending) | model: arch, 15-bit texture page 0x139 |
| 0x13 | `FX_SWDMAG_FLARE_WHITE_RINGS` | render | rings: white-cream gouraud rings |
| 0x18 | `FX_SWDMAG_FLARE_BACKDROP_3` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0x19 | `FX_SWDMAG_FLARE_TEX_RING` | kind/usage (look pending) | rings: textured ring, texture not in the file |
| 0x1A | `FX_SWDMAG_FLARE_TEX_RING_2` | kind/usage (look pending) | rings: textured ring, texture not in the file |
| 0x1B | `FX_SWDMAG_FLARE_BLUE_BOLT` | render | trail: blue lightning strip (fxm_drawOffsetTrail) |
| 0x1E | `FX_SWDMAG_FLARE_SWORD_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0x1F | `FX_SWDMAG_FLARE_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |

### fx_0e0: Sword Magic Doomsday (FX_DOOMSDAY_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x8 | `FX_SWDMAG_DOOMSDAY_ROCK` | render | model: polyhedral rock (rock texture) |
| 0x9 | `FX_SWDMAG_DOOMSDAY_DUST_RING` | render | rings: white ring fading through cream to grey, 8 frames |
| 0xA | `FX_SWDMAG_DOOMSDAY_FIRE_RING` | render | rings: white-orange / yellow rings, 16 frames |
| 0xB | `FX_SWDMAG_DOOMSDAY_BOULDER` | render | model: egg-shaped boulder (rock texture) |
| 0xD | `FX_SWDMAG_DOOMSDAY_EMBERS` | render | emitter: spiral of tiny orange sprites |
| 0x11 | `FX_SWDMAG_DOOMSDAY_STAR_FUNNEL` | render | model: three-bladed funnel with a starfield texture |
| 0x12 | `FX_SWDMAG_DOOMSDAY_STAR_DOME` | render | model: arch / dome with a starfield texture |
| 0x13 | `FX_SWDMAG_DOOMSDAY_GOURAUD_DOME` | kind/usage (look pending) | model: shallow gouraud dome (colours not rendered) |
| 0x14 | `FX_SWDMAG_DOOMSDAY_BOULDER_2` | render | model: rough egg-shaped boulder (rock texture) |
| 0x19 | `FX_SWDMAG_DOOMSDAY_CYLINDER` | kind/usage (look pending) | model: open cylinder, texture not in the file |
| 0x1A | `FX_SWDMAG_DOOMSDAY_LILAC_RINGS` | render | rings: white-lilac gouraud rings at the centre |
| 0x1B | `FX_SWDMAG_DOOMSDAY_BLUE_RINGS` | render | rings: white-cyan-blue gouraud rings at the centre |
| 0x1C | `FX_SWDMAG_DOOMSDAY_BACKDROP` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0x1D | `FX_SWDMAG_DOOMSDAY_PURPLE_BOLT` | render | trail: purple lightning strip (fxm_drawOffsetTrail) |
| 0x20 | `FX_SWDMAG_DOOMSDAY_PEBBLES` | render | emitter: spiral of small grey pebble sprites |
| 0x21 | `FX_SWDMAG_DOOMSDAY_PARTICLE_EMITTER` | kind/usage (look pending) | emitter: four particle elements, texture not in the file |
| 0x22 | `FX_SWDMAG_DOOMSDAY_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |
| 0x23 | `FX_SWDMAG_DOOMSDAY_SWORD_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |

### fx_0e1_0: Rebirth Flame, group 0 (Phoenix; FX_REBIRTH_FLAME_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_REBIRTH_FLAME_BLUE_ORBS` | render | emitter: clusters of blue-white glowing orbs |
| 0x3 | `FX_REBIRTH_FLAME_SO_MODEL` | kind/usage (look pending) | 'so' model drawn at the end (Hi_DrawEffModel, not rendered) |
| 0x4 | `FX_REBIRTH_FLAME_STAR_FLARES` | render | emitter: pink-white lens arcs + orange star flares |
| 0x5 | `FX_REBIRTH_FLAME_RED_RINGS` | render | rings: white-grey-violet-red gouraud rings, 24 frames |
| 0x6 | `FX_REBIRTH_FLAME_RED_RINGS_2` | render | emitter: white-grey-violet-red gouraud rings |
| 0x7 | `FX_REBIRTH_FLAME_MIST_TRAIL` | render | emitter: orbiting trail with a white cloudy strip texture |
| 0x8 | `FX_REBIRTH_FLAME_BLUE_RING` | render | emitter: blue gouraud rings |
| 0x9 | `FX_REBIRTH_FLAME_BLUE_BURST` | render | emitter: deep blue rings, sub-emitter of white-blue-indigo rings |
| 0xA | `FX_REBIRTH_FLAME_FAN_EMITTER` | kind/usage (look pending) | emitter: two fan elements (not rendered) |
| 0xC | `FX_REBIRTH_FLAME_RED_SPARKS` | render | emitter: four elements of tiny red spark sprites |
| 0xD | `FX_REBIRTH_FLAME_BLUE_ORBS_2` | render | emitter: faint blue orbs |
| 0x80 | `FX_REBIRTH_FLAME_MORPH_2` | kind/usage (look pending) | summon data: 'so' model, morph end, then drawn fading |
| 0x81 | `FX_REBIRTH_FLAME_MORPH_1` | kind/usage (look pending) | summon data: 'so' model, morph middle |
| 0x82 | `FX_REBIRTH_FLAME_MORPH_0` | kind/usage (look pending) | summon data: 'so' model, morph start |
| 0x83 | `FX_REBIRTH_FLAME_GLOW_MODEL` | kind/usage (look pending) | summon data: 'so' model drawn three times with a fading glow |

### fx_0e1_1: Rebirth Flame, group 1 (FX_REBIRTH_FLAME_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_REBIRTH_FLAME_TEX_EMITTER` | kind/usage (look pending) | emitter: textured elements (texture not in the file) + tan-white rings |
| 0x2 | `FX_REBIRTH_FLAME_RED_RINGS_2` | render, byte-identical | emitter: white-grey-violet-red gouraud rings (byte-identical to fx_0e1_0 entry 0x6) |
| 0x3 | `FX_REBIRTH_FLAME_BACKDROP` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0x4 | `FX_REBIRTH_FLAME_RED_SPARKS` | render | emitter: spiral of tiny red spark sprites |
| 0x5 | `FX_REBIRTH_FLAME_LENS_FLARES` | render | sprite line: lens-flare rings and glows |
| 0x6 | `FX_REBIRTH_FLAME_STAR_FLARES` | render | emitter: orange star flares + glowing dots |
| 0x7 | `FX_REBIRTH_FLAME_FAN_EMITTER` | kind/usage (look pending) | emitter: fan elements (not rendered) |

### fx_0e2: Fenrir (2nd file; FX_FENRIR_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_FENRIR_RIBBON_PATHS_C` | usage | table: position paths of ribbons 2/3 (fxm_offsetTableEntry, fxm_posPathInit) |
| 0x4 | `FX_FENRIR_GREEN_RINGS` | render | emitter: pale green gouraud rings, 64 frames |
| 0x9 | `FX_FENRIR_RIBBON_PATHS_A` | usage | table: position paths of ribbon 0 |
| 0xA | `FX_FENRIR_CAM_TRACK_1` | usage | table: camera key track, channel 1 (fxm_camChanAdd) |
| 0xB | `FX_FENRIR_CAM_TRACK_2` | usage | table: camera key track, channel 2 (fxm_camChanAdd) |
| 0xC | `FX_FENRIR_RIBBON_PATHS_B` | usage | table: position paths of ribbon 1 |
| 0xD | `FX_FENRIR_SAND_RINGS` | render | emitter: brown-salmon gouraud rings + spark sprites (texture not in the file) |
| 0xE | `FX_FENRIR_GREEN_WIND_TRAIL` | render | emitter: spiralling green wind-streak trail + grey smoke |
| 0xF | `FX_FENRIR_RIBBON_PATHS_D` | usage | table: position paths of ribbons 2/3 (other branch) |
| 0x10 | `FX_FENRIR_GREEN_RINGS_2` | render | emitter: white-green / pale green gouraud rings |
| 0x11 | `FX_FENRIR_EFFECT_MODEL` | kind/usage (look pending) | 'so' model (op84, not rendered) |
| 0x12 | `FX_FENRIR_SPARKS` | kind/usage (look pending) | emitter: spiral of spark sprites (texture not in the file) |
| 0x17 | `FX_FENRIR_GREEN_WIND_STRIP` | render | trail: green wind-streak strip texture |
| 0x1D | `FX_FENRIR_EFFECT_MODEL_2` | kind/usage (look pending) | 'so' model (op84, not rendered) |
| 0x1F | `FX_FENRIR_SHELL_MODEL_2` | kind/usage (look pending) | 'so' model drawn with SHELL_MODEL (not rendered) |
| 0x21 | `FX_FENRIR_SHELL_MODEL` | kind/usage (look pending) | 'so' model (not rendered) |
| 0x80 | `FX_FENRIR_BLOCK_MODEL` | kind/usage (look pending) | summon data: 'so' model drawn sliced as falling blocks (Hi_DrawSliceEffModel) |
| 0x81 | `FX_FENRIR_CLUSTER_MODEL_0` | kind/usage (look pending) | summon data: 'so' model, drawn with CLUSTER_MODEL_1/2 |
| 0x82 | `FX_FENRIR_WIND_STREAK` | render | summon data: model, long teardrop streak (Hi_RegisterTexListModel; shape only) |
| 0x83 | `FX_FENRIR_WIND_SERPENT` | render | summon data: model, serpent-like streak with a head (shape only) |
| 0x84 | `FX_FENRIR_WIND_BEAST` | render | summon data: model, spiky beast-shaped mass (shape only) |
| 0x85 | `FX_FENRIR_WIND_SPINDLE` | render | summon data: model, spindle with a round end (shape only) |
| 0x87 | `FX_FENRIR_CLUSTER_MODEL_1` | kind/usage (look pending) | summon data: 'so' model |
| 0x92 | `FX_FENRIR_CAM_TRACK_9` | usage | summon data: camera key tracks, channel 9 |
| 0x93 | `FX_FENRIR_CLUSTER_MODEL_2` | kind/usage (look pending) | summon data: 'so' model |
| 0x94 | `FX_FENRIR_MORPH_MODEL_1` | kind/usage (look pending) | summon data: 'so' model, morph target |
| 0x95 | `FX_FENRIR_MORPH_MODEL_0` | kind/usage (look pending) | summon data: 'so' model, morph source |
| 0x96 | `FX_FENRIR_CAM_TRACK_4` | usage | summon data: camera key tracks, channel 4 |
| 0x9B | `FX_FENRIR_CAM_TRACK_START` | usage | summon data: camera key tracks of channels 3, 4, 9, 10 at the start |
| 0x9C | `FX_FENRIR_PIECE_TRACKS` | usage | summon data: rotation/position paths of the pieces (fxm_rotPosPathInit) |
| 0x9D | `FX_FENRIR_STRIP_PATHS_2` | usage | summon data: position paths of strip set 1 |
| 0x9E | `FX_FENRIR_STRIP_PATHS` | usage | summon data: position paths of strip set 0 (head position) |
| 0x9F | `FX_FENRIR_CAM_TRACK_3` | usage | summon data: camera key tracks, channel 3 |
| 0xA0 | `FX_FENRIR_SMOKE` | render | summon data: emitter, grey smoke puffs |
| 0xA2 | `FX_FENRIR_GREEN_RING_BURST` | render | summon data: emitter, cream-white / green gouraud rings |
| 0xA3 | `FX_FENRIR_GREEN_RING_BURST_2` | render | summon data: emitter, olive-cream / green gouraud rings |
| 0xA4 | `FX_FENRIR_WIND_TRAIL_EMITTER` | kind/usage (look pending) | summon data: emitter, screen trail + sprites (texture not in the file) |
| 0xA5 | `FX_FENRIR_RED_MOON` | render | summon data: emitter, one big red moon sprite |
| 0xA6 | `FX_FENRIR_RED_MOON_GLOW` | render | summon data: emitter, red moon particle |

### fx_0e3_0: Bahamut, group 0 (FX_BAHAMUT_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_BAHAMUT_WING_TRAIL` | render | trail: white streak strip drawn from Bahamut's bones |
| 0x2 | `FX_BAHAMUT_PURPLE_BOLT` | render | trail: purple energy strip (fxm_drawOffsetTrail1) |
| 0x3 | `FX_BAHAMUT_TEAL_PUFF` | kind/usage (look pending) | particles: teal puff (pool task; texture partly outside the file) |
| 0x5 | `FX_BAHAMUT_BLUE_RINGS` | render | emitter: dark-to-light blue gouraud rings |
| 0x7 | `FX_BAHAMUT_CORE_MODEL` | kind/usage (look pending) | 'so' model drawn with a fading core colour |
| 0x8 | `FX_BAHAMUT_WHITE_BOLT` | kind/usage (look pending) | trail: faint white strip (fxm_drawOffsetTrail0) |
| 0xA | `FX_BAHAMUT_FIRE_SPARKS` | render | emitter: orange flame glows + spark sprites |
| 0xB | `FX_BAHAMUT_GLOW_MODEL` | kind/usage (look pending) | 'so' model drawn with a fading colour |
| 0xC | `FX_BAHAMUT_WHITE_RINGS` | render | rings: grey-white gouraud rings |
| 0x82 | `FX_BAHAMUT_PILLAR_MODEL` | kind/usage (look pending) | summon data: 'so' model, the pillar (pillarRgb) |
| 0x83 | `FX_BAHAMUT_BLUE_SPARKS` | render | summon data: emitter, blue-white spark sprites |
| 0x85 | `FX_BAHAMUT_FADE_MODEL` | kind/usage (look pending) | summon data: 'so' model drawn fading at the end |
| 0x86 | `FX_BAHAMUT_CONE_RING_MODEL` | kind/usage (look pending) | summon data: 'so' model stacked nine times as a cone |

### fx_0e3_1: Bahamut, group 1 (FX_BAHAMUT_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_BAHAMUT_COLUMN_MODEL` | kind/usage (look pending) | 'so' model, the column |
| 0x4 | `FX_BAHAMUT_EMBERS` | render | emitter: spiral of tiny orange sprites |
| 0x5 | `FX_BAHAMUT_FIRE_BURST` | render | emitter: red-white fire flashes |
| 0x6 | `FX_BAHAMUT_BLUE_SPARKS` | render | emitter: blue-white spark sprites falling |
| 0x7 | `FX_BAHAMUT_FLARE_LINE` | kind/usage (look pending) | sprite line (texture not in the file) |
| 0x8 | `FX_BAHAMUT_FLARE_PILLAR_MODEL` | kind/usage (look pending) | 'so' model, the pillar |
| 0x9 | `FX_BAHAMUT_FIRE_SPARKS` | render | emitter: orange flame glows + spark sprites |
| 0xA | `FX_BAHAMUT_PURPLE_BOLT` | render | trail: purple energy strip (fxm_drawOffsetTrail2) |
| 0xB | `FX_BAHAMUT_CHARGE_RINGS` | render | rings: white-orange-blue gouraud rings while charging |
| 0xC | `FX_BAHAMUT_BLUE_SPARKS_2` | render | emitter: blue-white spark sprites falling |
| 0xD | `FX_BAHAMUT_BLAST_MODEL_2` | kind/usage (look pending) | 'so' model drawn with BLAST_MODEL |
| 0xE | `FX_BAHAMUT_FAN_EMITTER` | kind/usage (look pending) | emitter: fan elements (not rendered) |
| 0xF | `FX_BAHAMUT_BLAST_MODEL` | kind/usage (look pending) | 'so' model |
| 0x11 | `FX_BAHAMUT_WHITE_BOLT_2` | kind/usage (look pending) | trail: faint white strip (fxm_drawOffsetTrail1) |
| 0x12 | `FX_BAHAMUT_SPIN_RING` | render | rings: white-blue gouraud rings, spinning |
| 0x14 | `FX_BAHAMUT_BLUE_RINGS` | render | rings: dark-to-light blue gouraud rings |
| 0x15 | `FX_BAHAMUT_WHITE_BOLT` | kind/usage (look pending), byte-identical | trail: faint white strip (fxm_drawOffsetTrail0; byte-identical to fx_0e3_0 entry 0x8) |
| 0x16 | `FX_BAHAMUT_BACKDROP` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0x17 | `FX_BAHAMUT_ENERGY_ORBS` | render | emitter: cyan-green orbs + violet lightning strip |
| 0x18 | `FX_BAHAMUT_SPARKLE_RING` | render | sprites: ring of blue-white sparkles, 16 frames |
| 0x19 | `FX_BAHAMUT_FIRE_RING_BURST` | render | emitter: red-white fire flashes + crimson rings |
| 0x1A | `FX_BAHAMUT_VIOLET_RINGS` | render | rings: white-cyan-violet gouraud rings |
| 0x1B | `FX_BAHAMUT_PINK_FLAT_RING` | render | rings: white-salmon gouraud rings drawn flat |
| 0x1C | `FX_BAHAMUT_HALO_RINGS` | render | rings: white-crimson-blue gouraud rings |
| 0x1D | `FX_BAHAMUT_FAN_EMITTER_2` | kind/usage (look pending) | emitter: fan element (not rendered) |
| 0x1F | `FX_BAHAMUT_WHITE_FLASH_RING` | render | rings: white gouraud rings, the flash |
| 0x20 | `FX_BAHAMUT_EMBERS_2` | render | emitter: spiral of tiny orange sprites |
| 0x21 | `FX_BAHAMUT_ORB_MODEL` | kind/usage (look pending) | 'so' model drawn at the blast centre |
| 0x23 | `FX_BAHAMUT_LIGHTNING_BOLT` | render | trail: violet lightning strip (fxm_drawOffsetTrail3) |
| 0x25 | `FX_BAHAMUT_BURST_MODEL` | kind/usage (look pending) | 'so' model drawn for the burst and the flash |
| 0x26 | `FX_BAHAMUT_BLAST_MODEL_3` | kind/usage (look pending) | 'so' model |
| 0x29 | `FX_BAHAMUT_FAN_EMITTER_3` | kind/usage (look pending) | emitter: fan element (not rendered) |
| 0x81 | `FX_BAHAMUT_ORB_SHELL_MODEL` | kind/usage (look pending) | summon data: 'so' model drawn around the orb |
| 0x84 | `FX_BAHAMUT_DISC_MODEL` | kind/usage (look pending) | summon data: 'so' model, disc |
| 0x85 | `FX_BAHAMUT_FADE_MODEL` | kind/usage (look pending) | summon data: 'so' model drawn fading at the end (same entry as fx_0e3_0's) |

### fx_0e4: Frog Drop (FX_FROG_DROP_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x5 | `FX_FROG_DROP_STARS` | render | emitter: yellow stars orbiting (dizzy stars) |
| 0x6 | `FX_FROG_DROP_DUST_RING` | render | rings: white-grey gouraud rings |
| 0x7 | `FX_FROG_DROP_IMPACT_DEBRIS` | render | emitter: ring of white debris sprites + white-cyan-blue rings |

### fx_0e5: Soul Blade (FX_SOUL_BLADE_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x6 | `FX_SOUL_BLADE_LIGHT_FLASH` | render | emitter: lilac / blue-white rings + white light streak |
| 0x8 | `FX_SOUL_BLADE_SPIRIT_WISPS` | render | emitter: blue rings, white sparkles, golden smoke wisps |
| 0x9 | `FX_SOUL_BLADE_LENS_FLARES` | render | sprite line: lens-flare circles and rings |

### fx_0e6: enemy Stone (Mu; FX_STONE_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x2 | `FX_STONE_ROCK_SHELL` | render | model: ball with a rock texture |
| 0x8 | `FX_STONE_RED_RINGS` | render | emitter: white-pink-red gouraud rings + spark sprites (texture not in the file) |
| 0x9 | `FX_STONE_ROCK_STRIP` | render | trail: strip with a rock texture |

### fx_0e7: Thievery (FX_THIEVERY_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_THIEVERY_CHEST_LID` | render | model: treasure chest lid (Hi_RegisterTexListModel; shape only) |
| 0x2 | `FX_THIEVERY_CHEST_BOX` | render | model: treasure chest box (shape only) |
| 0x3 | `FX_THIEVERY_LENS_FLARES` | render | sprite line: lens-flare circles and rings |
| 0x4 | `FX_THIEVERY_GOLD_GLITTER` | render | emitter: gold glitter + small glows |
| 0x5 | `FX_THIEVERY_WHITE_PUFF` | render | emitter: white smoke puff |
| 0xC | `FX_THIEVERY_GOLD_RINGS` | render | emitter: fans + white-yellow-orange gouraud rings |
| 0xD | `FX_THIEVERY_GOLD_BURST` | render | emitter: golden puffs + yellow-orange / cyan-blue rings |

### fx_0e8: enemy Escape (Magic Vice, ...; FX_ESCAPE_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_ESCAPE_RAINBOW_STREAKS` | render | emitter: cyan-yellow / magenta-violet flame streaks |
| 0x2 | `FX_ESCAPE_WIND_TRAIL` | render | emitter: spiralling trail with a grey streak texture |
| 0x3 | `FX_ESCAPE_SMOKE` | render | emitter: grey-teal smoke puffs |
| 0x4 | `FX_ESCAPE_CYAN_RING` | render | emitter: pale cyan gouraud rings fading |
| 0x5 | `FX_ESCAPE_FLASH_RINGS` | render | emitter: white-red and cream rings |

### fx_0e9: Annoy (FX_ANNOY_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x2 | `FX_ANNOY_PINK_WISPS` | render | emitter: rising pink wisps, orange shimmer, sparks, light streak |
| 0x6 | `FX_ANNOY_ORANGE_SHIMMER` | render | emitter: three orange shimmering quad animations |

### fx_0ea: Sacrifice (FX_SACRIFICE_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_SACRIFICE_RED_PETALS` | render | emitter: spiral of red petals + red glitter |
| 0x2 | `FX_SACRIFICE_LIGHT_FLASH` | render | emitter: lilac / blue-white rings + white light streaks |
| 0x5 | `FX_SACRIFICE_FAN_EMITTER` | kind/usage (look pending) | emitter: fans + faint sprites with cyan-white rings |
| 0x9 | `FX_SACRIFICE_LENS_FLARES` | render | sprite line: lens-flare circles and rings |

### fx_0eb: enemy Psychokinesis (Nova Dragon, ...; FX_PSYCHOKINESIS_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_PSYCHOKINESIS_WHITE_RING` | render | emitter: white gouraud rings |
| 0x2 | `FX_PSYCHOKINESIS_WHITE_RING_2` | render | emitter: white gouraud rings |
| 0x3 | `FX_PSYCHOKINESIS_PSYCHIC_ORBS` | render | emitter: homing violet-to-pink orbs |
| 0x7 | `FX_PSYCHOKINESIS_SHOCK_RING` | render | emitter: white-to-grey gouraud rings, 8 frames |
| 0x8 | `FX_PSYCHOKINESIS_DARK_BUBBLES` | render | emitter: dark bubble screen trails |

### fx_0ec: Doom (FX_DOOM_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_DOOM_GOLD_FRAME` | render | model: ornate gold frame, rises sliced |
| 0x3 | `FX_DOOM_GOLD_PANEL` | render | model: carved gold panel, rises sliced |
| 0x4 | `FX_DOOM_REAPER_PICTURE` | render | model: panel with the picture of Death |
| 0x6 | `FX_DOOM_PARTICLES` | kind/usage (look pending) | particles (texture not in the file) |
| 0x7 | `FX_DOOM_GOLD_LATTICE` | render | model: capsule with a gold lattice texture |
| 0x8 | `FX_DOOM_PINK_RINGS` | render | emitter: pink-white rings (+ particles, texture not in the file) |
| 0x9 | `FX_DOOM_CAMERA_POINT` | usage | table: SVECTOR whose pad sets the camera point height |
| 0xB | `FX_DOOM_RED_RING` | render | emitter: glowing red ring sprite |
| 0xC | `FX_DOOM_PINK_SMOKE` | render | emitter: pink-violet smoke |
| 0xD | `FX_DOOM_GLOW_SHELL` | kind/usage (look pending) | model: capsule, additive, texture page computed (drawn twice) |
| 0xE | `FX_DOOM_CYAN_SPARKLES` | render | emitter: cyan sparkles in a tube |
| 0xF | `FX_DOOM_REAPER_PICTURE_2` | render | model: second panel with the picture of Death |
| 0x10 | `FX_DOOM_WHITE_SHELL` | kind/usage (look pending) | model: capsule, 15-bit texture page 0x119 |
| 0x11 | `FX_DOOM_BACKDROP` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0x13 | `FX_DOOM_GOURAUD_SHELL` | kind/usage (look pending) | model: gouraud capsule (colours not rendered) |

### fx_0ed: Roulette (FX_ROULETTE_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x2 | `FX_ROULETTE_SUMMON_MODEL_DATA` | usage | model data passed to btlfx_registerSummonModel with ROULETTE_SUMMON_MODEL |
| 0x4 | `FX_ROULETTE_BACKDROP` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0x6 | `FX_ROULETTE_TARGET_RING` | render | sprite: white ring outline on the chosen target |
| 0x7 | `FX_ROULETTE_SUMMON_MODEL` | kind/usage (look pending) | 'so' summon model (btlfx_registerSummonModel, not rendered) |
| 0x8 | `FX_ROULETTE_SO_MODEL` | kind/usage (look pending) | 'so' model (not rendered) |
| 0xA | `FX_ROULETTE_BLUE_BEAM` | render | emitter: spiralling blue light-beam trail |
| 0xB | `FX_ROULETTE_GLOW_RINGS` | render | emitter: green glows + white-salmon-violet rings |
| 0xC | `FX_ROULETTE_RED_SPARKS` | render | emitter: pink-red spark sprites + white-red rings |
| 0x10 | `FX_ROULETTE_WHITE_GLOW` | render | emitter: white glow puffs |
| 0x11 | `FX_ROULETTE_BLUE_BUBBLES` | render | emitter: blue bubbles |
| 0x1D | `FX_ROULETTE_BLUE_BEAM_2` | render | emitter: spiralling blue light-beam trail |

### fx_0ee: enemy Mirror (FX_MIRROR_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_MIRROR_WHITE_RINGS` | render | emitter: white rings + green glow |
| 0x2 | `FX_MIRROR_WHITE_RINGS_2` | render | emitter: white rings + green glow |
| 0x3 | `FX_MIRROR_GREEN_SWIRL` | render | emitter: white rings + green swirl particle |
| 0x5 | `FX_MIRROR_FAN_EMITTER` | kind/usage (look pending) | emitter: two fan elements (not rendered) |

### fx_0ef: enemy Red Clipper (Stilva; FX_RED_CLIPPER_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x8 | `FX_RED_CLIPPER_RED_SLASH` | render | emitter: red slash texture + yellow rings + sparks (texture not in the file) |
| 0xE | `FX_RED_CLIPPER_PINK_SMOKE` | render | emitter: pink smoke puffs |
| 0xF | `FX_RED_CLIPPER_RED_SMOKE` | render | emitter: brown-red smoke puffs |

### fx_0f0: enemy Zombie Powder (Whale Zombie; FX_ZOMBIE_POWDER_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_ZOMBIE_POWDER_ANIM_EMITTER` | kind/usage (look pending) | emitter: animated quads, render all black |
| 0x2 | `FX_ZOMBIE_POWDER_SPORE_TRAIL` | render | emitter: homing green spores, yellow puffs, sparkles |
| 0x3 | `FX_ZOMBIE_POWDER_YELLOW_PUFFS` | render | emitter: yellow powder puffs |
| 0x4 | `FX_ZOMBIE_POWDER_YELLOW_CLOUD` | render | emitter: yellow powder cloud |
| 0x5 | `FX_ZOMBIE_POWDER_VIOLET_CLOUD` | render | emitter: violet powder cloud |
| 0x6 | `FX_ZOMBIE_POWDER_MIST_TRAIL` | render | emitter: green-grey mist screen trails |
| 0x7 | `FX_ZOMBIE_POWDER_MIST_TRAIL_2` | render | emitter: green-grey mist screen trails |
| 0x15 | `FX_ZOMBIE_POWDER_DUST_WAVE` | render | emitter: yellow-green dust half ring |

### fx_0f1: Free Energy (FX_FREE_ENERGY_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x2 | `FX_FREE_ENERGY_RUNE_BURST` | render | emitter: white rune glyphs with blue-white rings, then a fire burst |

### fx_0f2: Tidal Flame (FX_TIDAL_FLAME_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x2 | `FX_TIDAL_FLAME_DOME` | kind/usage (look pending) | model: hemisphere, texture page 0x11A not in the file |
| 0x3 | `FX_TIDAL_FLAME_BACKDROP` | kind/usage (look pending) | backdrop layers (btlfx_drawBackdrop, not rendered) |
| 0x8 | `FX_TIDAL_FLAME_RUNE_BURST` | render | emitter: white rune glyphs with cyan-blue rings, embers, blue light streak |
| 0x9 | `FX_TIDAL_FLAME_FLAME_CLOUD` | render | emitter: embers + orange flame cloud |

## Open questions (emulator)

Each wants one screenshot of the effect at the frame the C starts the entry, or a VRAM dump while it
plays (textures not in the file), as in `docs/fxdata.md`.

- fx_0d9: `FX_SWDMAG_THUNDAGA_DISC` (model: flat octagon, 15-bit texture page 0x119 (byte-identical to fx_018 entry 0x10)); `FX_SWDMAG_THUNDAGA_FAN_EMITTER` (emitter: four fan elements (not rendered) (byte-identical to fx_018 entry 0x21)); `FX_SWDMAG_THUNDAGA_FAN_EMITTER_2` (emitter: four fan elements (not rendered) (byte-identical to fx_018 entry 0x22)); `FX_SWDMAG_THUNDAGA_CONE_EMITTER` (emitter: cone of sprites, render all transparent (byte-identical to fx_018 entry 0x26)); `FX_SWDMAG_THUNDAGA_SPARKS` (emitter: spark sprites (texture not in the file) (byte-identical to fx_018 entry 0x27))
- fx_0dc: `FX_SWDMAG_BLIZZAGA_FAN_EMITTER` (emitter: four fan elements (not rendered) (byte-identical to fx_097 entry 0x1C))
- fx_0dd: `FX_SWDMAG_BIO_SCREEN_TRAIL` (emitter: orbiting screen-space trail, texture not in the file)
- fx_0df: `FX_SWDMAG_FLARE_FAN_EMITTER` (emitter: two fan elements (not rendered)); `FX_SWDMAG_FLARE_BACKDROP` (backdrop layers (btlfx_drawBackdrop, not rendered)); `FX_SWDMAG_FLARE_SPHERE` (model: sphere, texture page 0x13C not in the file); `FX_SWDMAG_FLARE_GOURAUD_RING` (model: flat gouraud annulus (colours not rendered)); `FX_SWDMAG_FLARE_BACKDROP_2` (backdrop layers (btlfx_drawBackdrop, not rendered)); `FX_SWDMAG_FLARE_ARCH` (model: arch, 15-bit texture page 0x139); `FX_SWDMAG_FLARE_BACKDROP_3` (backdrop layers (btlfx_drawBackdrop, not rendered)); `FX_SWDMAG_FLARE_TEX_RING` (rings: textured ring, texture not in the file); `FX_SWDMAG_FLARE_TEX_RING_2` (rings: textured ring, texture not in the file)
- fx_0e0: `FX_SWDMAG_DOOMSDAY_GOURAUD_DOME` (model: shallow gouraud dome (colours not rendered)); `FX_SWDMAG_DOOMSDAY_CYLINDER` (model: open cylinder, texture not in the file); `FX_SWDMAG_DOOMSDAY_BACKDROP` (backdrop layers (btlfx_drawBackdrop, not rendered)); `FX_SWDMAG_DOOMSDAY_PARTICLE_EMITTER` (emitter: four particle elements, texture not in the file)
- fx_0e1_0: `FX_REBIRTH_FLAME_SO_MODEL` ('so' model drawn at the end (Hi_DrawEffModel, not rendered)); `FX_REBIRTH_FLAME_FAN_EMITTER` (emitter: two fan elements (not rendered)); `FX_REBIRTH_FLAME_MORPH_2` (summon data: 'so' model, morph end, then drawn fading); `FX_REBIRTH_FLAME_MORPH_1` (summon data: 'so' model, morph middle); `FX_REBIRTH_FLAME_MORPH_0` (summon data: 'so' model, morph start); `FX_REBIRTH_FLAME_GLOW_MODEL` (summon data: 'so' model drawn three times with a fading glow)
- fx_0e1_1: `FX_REBIRTH_FLAME_TEX_EMITTER` (emitter: textured elements (texture not in the file) + tan-white rings); `FX_REBIRTH_FLAME_BACKDROP` (backdrop layers (btlfx_drawBackdrop, not rendered)); `FX_REBIRTH_FLAME_FAN_EMITTER` (emitter: fan elements (not rendered))
- fx_0e2: `FX_FENRIR_EFFECT_MODEL` ('so' model (op84, not rendered)); `FX_FENRIR_SPARKS` (emitter: spiral of spark sprites (texture not in the file)); `FX_FENRIR_EFFECT_MODEL_2` ('so' model (op84, not rendered)); `FX_FENRIR_SHELL_MODEL_2` ('so' model drawn with SHELL_MODEL (not rendered)); `FX_FENRIR_SHELL_MODEL` ('so' model (not rendered)); `FX_FENRIR_BLOCK_MODEL` (summon data: 'so' model drawn sliced as falling blocks (Hi_DrawSliceEffModel)); `FX_FENRIR_CLUSTER_MODEL_0` (summon data: 'so' model, drawn with CLUSTER_MODEL_1/2); `FX_FENRIR_CLUSTER_MODEL_1` (summon data: 'so' model); `FX_FENRIR_CLUSTER_MODEL_2` (summon data: 'so' model); `FX_FENRIR_MORPH_MODEL_1` (summon data: 'so' model, morph target); `FX_FENRIR_MORPH_MODEL_0` (summon data: 'so' model, morph source); `FX_FENRIR_WIND_TRAIL_EMITTER` (summon data: emitter, screen trail + sprites (texture not in the file))
- fx_0e3_0: `FX_BAHAMUT_TEAL_PUFF` (particles: teal puff (pool task; texture partly outside the file)); `FX_BAHAMUT_CORE_MODEL` ('so' model drawn with a fading core colour); `FX_BAHAMUT_WHITE_BOLT` (trail: faint white strip (fxm_drawOffsetTrail0)); `FX_BAHAMUT_GLOW_MODEL` ('so' model drawn with a fading colour); `FX_BAHAMUT_PILLAR_MODEL` (summon data: 'so' model, the pillar (pillarRgb)); `FX_BAHAMUT_FADE_MODEL` (summon data: 'so' model drawn fading at the end); `FX_BAHAMUT_CONE_RING_MODEL` (summon data: 'so' model stacked nine times as a cone)
- fx_0e3_1: `FX_BAHAMUT_COLUMN_MODEL` ('so' model, the column); `FX_BAHAMUT_FLARE_LINE` (sprite line (texture not in the file)); `FX_BAHAMUT_FLARE_PILLAR_MODEL` ('so' model, the pillar); `FX_BAHAMUT_BLAST_MODEL_2` ('so' model drawn with BLAST_MODEL); `FX_BAHAMUT_FAN_EMITTER` (emitter: fan elements (not rendered)); `FX_BAHAMUT_BLAST_MODEL` ('so' model); `FX_BAHAMUT_WHITE_BOLT_2` (trail: faint white strip (fxm_drawOffsetTrail1)); `FX_BAHAMUT_WHITE_BOLT` (trail: faint white strip (fxm_drawOffsetTrail0; byte-identical to fx_0e3_0 entry 0x8)); `FX_BAHAMUT_BACKDROP` (backdrop layers (btlfx_drawBackdrop, not rendered)); `FX_BAHAMUT_FAN_EMITTER_2` (emitter: fan element (not rendered)); `FX_BAHAMUT_ORB_MODEL` ('so' model drawn at the blast centre); `FX_BAHAMUT_BURST_MODEL` ('so' model drawn for the burst and the flash); `FX_BAHAMUT_BLAST_MODEL_3` ('so' model); `FX_BAHAMUT_FAN_EMITTER_3` (emitter: fan element (not rendered)); `FX_BAHAMUT_ORB_SHELL_MODEL` (summon data: 'so' model drawn around the orb); `FX_BAHAMUT_DISC_MODEL` (summon data: 'so' model, disc); `FX_BAHAMUT_FADE_MODEL` (summon data: 'so' model drawn fading at the end (same entry as fx_0e3_0's))
- fx_0ea: `FX_SACRIFICE_FAN_EMITTER` (emitter: fans + faint sprites with cyan-white rings)
- fx_0ec: `FX_DOOM_PARTICLES` (particles (texture not in the file)); `FX_DOOM_GLOW_SHELL` (model: capsule, additive, texture page computed (drawn twice)); `FX_DOOM_WHITE_SHELL` (model: capsule, 15-bit texture page 0x119); `FX_DOOM_BACKDROP` (backdrop layers (btlfx_drawBackdrop, not rendered)); `FX_DOOM_GOURAUD_SHELL` (model: gouraud capsule (colours not rendered))
- fx_0ed: `FX_ROULETTE_BACKDROP` (backdrop layers (btlfx_drawBackdrop, not rendered)); `FX_ROULETTE_SUMMON_MODEL` ('so' summon model (btlfx_registerSummonModel, not rendered)); `FX_ROULETTE_SO_MODEL` ('so' model (not rendered))
- fx_0ee: `FX_MIRROR_FAN_EMITTER` (emitter: two fan elements (not rendered))
- fx_0f0: `FX_ZOMBIE_POWDER_ANIM_EMITTER` (emitter: animated quads, render all black)
- fx_0f2: `FX_TIDAL_FLAME_DOME` (model: hemisphere, texture page 0x11A not in the file); `FX_TIDAL_FLAME_BACKDROP` (backdrop layers (btlfx_drawBackdrop, not rendered))

78 look-pending names.
