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
| 0xD | `FX_SWDMAG_THUNDAGA_DISC` | kind/usage (look pending) | model: flat octagon on 15-bit texture page 0x119, which the file does not fill (drawn at run time) (byte-identical to fx_018 entry 0x10); runtime: as Thundaga's disc (fx_018 0x10) its page holds 4-bit lightning textures at run time, read as 15-bit noise |
| 0xE | `FX_SWDMAG_THUNDAGA_GLOW_DOME` | render, byte-identical | model: hemisphere with an orange-to-white streak texture (byte-identical to fx_018 entry 0x11) |
| 0x10 | `FX_SWDMAG_THUNDAGA_BLUE_RING` | render | rings: white-blue gouraud rings |
| 0x12 | `FX_SWDMAG_THUNDAGA_WHITE_RING` | render | rings: white gouraud rings drawn flat |
| 0x14 | `FX_SWDMAG_THUNDAGA_LIGHTNING_BOLT` | render, byte-identical | particles: wide blue-white lightning bolt (byte-identical to fx_018 entry 0x17) |
| 0x18 | `FX_SWDMAG_THUNDAGA_SPARK_BALL` | render, byte-identical | particles: golden plasma ball, 8 frames (byte-identical to fx_018 entry 0x1B) |
| 0x1E | `FX_SWDMAG_THUNDAGA_VIOLET_RING` | render | rings: white-violet gouraud rings |
| 0x20 | `FX_SWDMAG_THUNDAGA_THIN_BOLT` | render, byte-identical | particles: thin cyan lightning bolt (byte-identical to fx_018 entry 0x20) |
| 0x21 | `FX_SWDMAG_THUNDAGA_LIGHTNING_ARCS` | render (fxdata d6121033) | emitter: four fans of cyan / blue zigzag lightning arcs (byte-identical to fx_018 entry 0x21) |
| 0x22 | `FX_SWDMAG_THUNDAGA_LIGHTNING_ARCS_2` | render (fxdata d6121033) | emitter: four fans of cyan / blue zigzag lightning arcs (byte-identical to fx_018 entry 0x22) |
| 0x25 | `FX_SWDMAG_THUNDAGA_IMPACT_FLASH` | render, byte-identical | emitter: four-point star flash (byte-identical to fx_018 entry 0x25) |
| 0x26 | `FX_SWDMAG_THUNDAGA_SPARKLE_SWARM` | runtime | emitter: Thundaga's swarms of blue, lilac and teal twinkle dots (byte-identical to fx_018 entry 0x26) |
| 0x27 | `FX_SWDMAG_THUNDAGA_SPARK_DOTS` | render (fxdata d6121033) | emitter: white and yellow dot sparks (byte-identical to fx_018 entry 0x27) |
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
| 0x1C | `FX_SWDMAG_BLIZZAGA_ICE_SPIKES` | render (fxdata d6121033) | emitter: four fans of white-cyan icicle spikes (byte-identical to fx_097 entry 0x1C) |
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
| 0x1 | `FX_SWDMAG_BIO_PATH` | usage | table: rotation/position keys of the morphing bubble (fxm_rotPosTrackInit) |
| 0x2 | `FX_SWDMAG_BIO_SCALE_TRACK` | usage | table: scale keys of the morphing bubble (fxm_vecTrackInit) |
| 0xF | `FX_SWDMAG_BIO_SCALE_TRACK_2` | usage | table: scale keys of the second bubble pass (fxm_vecTrackInit) |
| 0x10 | `FX_SWDMAG_BIO_PATH_2` | usage | table: rotation/position keys of the second bubble pass |
| 0x11 | `FX_SWDMAG_BIO_BUBBLE_TRAIL` | runtime | emitter: Bio's screen trail of glowing green bubbles |
| 0x12 | `FX_SWDMAG_BIO_TEAL_RING` | render | emitter: teal-green wisp rings |
| 0x13 | `FX_SWDMAG_BIO_TOXIC_CLOUD` | render | emitter: green gas cloud + grey haze |
| 0x14 | `FX_SWDMAG_BIO_RED_ORB` | render | emitter: red cell-mesh balls with a crimson glow |
| 0x15 | `FX_SWDMAG_BIO_SWORD_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0x16 | `FX_SWDMAG_BIO_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |

### fx_0de: Sword Magic Water (FX_WATER_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_SWDMAG_WATER_SPLASH_DROPS` | render | emitter: water blobs, blue glow and glints |
| 0x2 | `FX_SWDMAG_WATER_GLOBE_1` | render | model: water funnel rising, morph stage 1 |
| 0x3 | `FX_SWDMAG_WATER_GLOBE_2` | render | model: water vase shape, morph stage 2 |
| 0x4 | `FX_SWDMAG_WATER_GLOBE_3` | render | model: water bulb closing, morph stage 3 |
| 0x5 | `FX_SWDMAG_WATER_GLOBE_0` | render | model: flat water pool (water texture), morph stage 0 |
| 0x6 | `FX_SWDMAG_WATER_SPLASH_FUNNEL` | render | model: swirling funnel, mist texture, morph end of the spray |
| 0x7 | `FX_SWDMAG_WATER_SPLASH_COLUMN` | render | model: open cylinder, mist texture, morph start of the spray |
| 0x8 | `FX_SWDMAG_WATER_BASE_DISC` | render | model: flat disc, water texture |
| 0x9 | `FX_SWDMAG_WATER_BLUE_RING` | render | emitter: white-blue gouraud rings |
| 0xD | `FX_SWDMAG_WATER_GLOBE` | render | model: closed water sphere, morph end |
| 0xE | `FX_SWDMAG_WATER_SWORD_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0xF | `FX_SWDMAG_WATER_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |

### fx_0df: Sword Magic Flare (FX_FLARE_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x3 | `FX_SWDMAG_FLARE_RED_RINGS` | render | rings: white-pink-red gouraud rings |
| 0x4 | `FX_SWDMAG_FLARE_BLAST_RINGS` | render | rings: white-cyan-orange-red gouraud rings, the blast |
| 0x5 | `FX_SWDMAG_FLARE_FIRE_DOME` | render | model: dome with a fire texture |
| 0x6 | `FX_SWDMAG_FLARE_SPARK_GLOW` | render | emitter: red ember sprites + orange glow |
| 0x8 | `FX_SWDMAG_FLARE_RED_VIOLET_RINGS` | render | emitter: white-crimson / white-violet gouraud rings |
| 0x9 | `FX_SWDMAG_FLARE_GOLDEN_GLOWS` | render (byte-identical twin, consistency pass) | emitter: two fans drawn as small golden glow spots (byte-identical to fx_07d entry 0x1B, same render) |
| 0xB | `FX_SWDMAG_FLARE_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| 0xC | `FX_SWDMAG_FLARE_CLOUD_SPHERE` | render (fxdata d6121033) | model: sphere with a grey smoke texture (page 0x13C) |
| 0xD | `FX_SWDMAG_FLARE_RED_HALO` | render (fxdata d6121033) | model: flat red-orange gouraud halo ring |
| 0xE | `FX_SWDMAG_FLARE_BACKDROP_2` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| 0x10 | `FX_SWDMAG_FLARE_ORANGE_GLOW` | render | particles: orange glow with a white core |
| 0x11 | `FX_SWDMAG_FLARE_EMBERS_2` | render | emitter: red ember sprites |
| 0x12 | `FX_SWDMAG_FLARE_ARCH` | render (fxdata d6121033) | model: grey arch, 15-bit texture page 0x139 (white in the render) |
| 0x13 | `FX_SWDMAG_FLARE_WHITE_RINGS` | render | rings: white-cream gouraud rings |
| 0x18 | `FX_SWDMAG_FLARE_BACKDROP_3` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| 0x19 | `FX_SWDMAG_FLARE_TEX_RING` | kind/usage (look pending) | rings: textured ring; the render reads garbled multicolour texels (texture not in the file); runtime: the ring UV box reads noise in the run-time pages too |
| 0x1A | `FX_SWDMAG_FLARE_TEX_RING_2` | kind/usage (look pending) | rings: textured ring; the render reads garbled multicolour texels (texture not in the file); runtime: noise in the run-time page too |
| 0x1B | `FX_SWDMAG_FLARE_BLUE_LIGHTNING` | render | trail: blue lightning strip (fxm_drawOffsetTrail) |
| 0x1E | `FX_SWDMAG_FLARE_SWORD_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |
| 0x1F | `FX_SWDMAG_FLARE_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |

### fx_0e0: Sword Magic Doomsday (FX_DOOMSDAY_SWORD_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x8 | `FX_SWDMAG_DOOMSDAY_METEOR` | render | model: polyhedral rock (rock texture) |
| 0x9 | `FX_SWDMAG_DOOMSDAY_WHITE_RING` | render | rings: white ring fading through cream to grey, 8 frames |
| 0xA | `FX_SWDMAG_DOOMSDAY_FIRE_RING` | render | rings: white-orange / yellow rings, 16 frames |
| 0xB | `FX_SWDMAG_DOOMSDAY_BIG_METEOR` | render | model: egg-shaped boulder (rock texture) |
| 0xD | `FX_SWDMAG_DOOMSDAY_EMBERS` | render | emitter: spiral of tiny orange sprites |
| 0x11 | `FX_SWDMAG_DOOMSDAY_STAR_TUNNEL` | render | model: three-bladed funnel with a starfield texture |
| 0x12 | `FX_SWDMAG_DOOMSDAY_STAR_ARCH` | render | model: arch / dome with a starfield texture |
| 0x13 | `FX_SWDMAG_DOOMSDAY_RED_DOME` | render (fxdata d6121033) | model: shallow dark-red gouraud dome with a white centre |
| 0x14 | `FX_SWDMAG_DOOMSDAY_BOULDER_2` | render | model: rough egg-shaped boulder (rock texture) |
| 0x19 | `FX_SWDMAG_DOOMSDAY_LIGHT_CONE` | render (fxdata d6121033) | model: open cylinder with a grey-white smoke texture |
| 0x1A | `FX_SWDMAG_DOOMSDAY_LILAC_RINGS` | render | rings: white-lilac gouraud rings at the centre |
| 0x1B | `FX_SWDMAG_DOOMSDAY_BLUE_RINGS` | render | rings: white-cyan-blue gouraud rings at the centre |
| 0x1C | `FX_SWDMAG_DOOMSDAY_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| 0x1D | `FX_SWDMAG_DOOMSDAY_PURPLE_STREAK` | render | trail: purple lightning strip (fxm_drawOffsetTrail) |
| 0x20 | `FX_SWDMAG_DOOMSDAY_PEBBLES` | render | emitter: spiral of small grey pebble sprites |
| 0x21 | `FX_SWDMAG_DOOMSDAY_ROCK_DEBRIS` | render (fxdata d6121033) | emitter: four particle elements, tumbling grey-blue rock debris |
| 0x22 | `FX_SWDMAG_DOOMSDAY_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (op84) |
| 0x23 | `FX_SWDMAG_DOOMSDAY_SWORD_GLINT` | render | rings: white 6-segment rings flashed at the sword (op84, first draw) |

### fx_0e1_0: Rebirth Flame, group 0 (Phoenix; FX_REBIRTH_FLAME_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_REBIRTH_FLAME_BLUE_ORBS` | render | emitter: clusters of blue-white glowing orbs |
| 0x3 | `FX_REBIRTH_FLAME_SWIRL_SPHERE` | render (fxdata d6121033) | 'so' model drawn at the end (Hi_DrawEffModel): sphere with a purple-red-blue swirl texture |
| 0x4 | `FX_REBIRTH_FLAME_STAR_FLARES` | render | emitter: pink-white lens arcs + orange star flares |
| 0x5 | `FX_REBIRTH_FLAME_RED_RINGS` | render | rings: white-grey-violet-red gouraud rings, 24 frames |
| 0x6 | `FX_REBIRTH_FLAME_RED_RINGS_2` | render | emitter: white-grey-violet-red gouraud rings |
| 0x7 | `FX_REBIRTH_FLAME_MIST_TRAIL` | render | emitter: orbiting trail with a white cloudy strip texture |
| 0x8 | `FX_REBIRTH_FLAME_BLUE_RING` | render | emitter: blue gouraud rings |
| 0x9 | `FX_REBIRTH_FLAME_BLUE_BURST` | render | emitter: deep blue rings, sub-emitter of white-blue-indigo rings |
| 0xA | `FX_REBIRTH_FLAME_WHITE_STREAK` | render (fxdata d6121033) | emitter: two fans, a long white-grey light streak |
| 0xC | `FX_REBIRTH_FLAME_RED_SPARKS` | render | emitter: four elements of tiny red spark sprites |
| 0xD | `FX_REBIRTH_FLAME_BLUE_ORBS_2` | render | emitter: faint blue orbs |
| 0x80 | `FX_REBIRTH_FLAME_SCREEN_SPHERE` | runtime | summon data, 'so' model, morph end, then drawn fading: the captured battle scene wrapped on a sphere, the morph target the flat capture bends into |
| 0x81 | `FX_REBIRTH_FLAME_SCREEN_BULGE` | runtime (medium) | summon data, 'so' model, morph middle: the in-between morph shape with the same captured scene texture |
| 0x82 | `FX_REBIRTH_FLAME_SCREEN_PLANE` | runtime | summon data, 'so' model, morph start: flat grid carrying the captured battle scene, the picture before it is bent |
| 0x83 | `FX_REBIRTH_FLAME_GLOW_BAND` | render (fxdata d6121033) | summon data: 'so' model, ring band with a white cloud texture, drawn three times with a fading glow |

### fx_0e1_1: Rebirth Flame, group 1 (FX_REBIRTH_FLAME_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_REBIRTH_FLAME_TEX_EMITTER` | kind/usage (look pending) | emitter: textured elements + tan-white rings; the textured element still reads garbled texels (texture not in the file); runtime: fetched during the fire blast (orange-white flames on screen); its ring elements read noise in the run-time page |
| 0x2 | `FX_REBIRTH_FLAME_RED_RINGS_2` | render, byte-identical | emitter: white-grey-violet-red gouraud rings (byte-identical to fx_0e1_0 entry 0x6) |
| 0x3 | `FX_REBIRTH_FLAME_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| 0x4 | `FX_REBIRTH_FLAME_RED_SPARKS` | render | emitter: spiral of tiny red spark sprites |
| 0x5 | `FX_REBIRTH_FLAME_LENS_FLARES` | render | sprite line: lens-flare rings and glows |
| 0x6 | `FX_REBIRTH_FLAME_STAR_FLARES` | render | emitter: orange star flares + glowing dots |
| 0x7 | `FX_REBIRTH_FLAME_ORANGE_STREAKS` | render (fxdata d6121033) | emitter: two fans, thin orange-white rising streaks |

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
| 0x11 | `FX_FENRIR_WIND_RING` | render (fxdata d6121033) | 'so' model: flat ring with a green wind texture |
| 0x12 | `FX_FENRIR_CYAN_GLINTS` | runtime (medium) | emitter: spiral of white dot sparks; at run time (fx_1fd's byte-identical entry) tiny cyan-white glints round the green whirlwind |
| 0x17 | `FX_FENRIR_GREEN_WIND_STRIP` | render | trail: green wind-streak strip texture |
| 0x1D | `FX_FENRIR_WIND_RING_2` | render (fxdata d6121033) | 'so' model: flat ring with a green wind texture (same shape as WIND_RING) |
| 0x1F | `FX_FENRIR_WIND_VORTEX` | render (fxdata d6121033) | 'so' model: swirl of green wind blades (vortex), drawn with WIND_SHELL |
| 0x21 | `FX_FENRIR_WIND_SHELL` | render (fxdata d6121033) | 'so' model: cone-shaped shell with a green wind texture |
| 0x80 | `FX_FENRIR_ROCK_SPIKE` | render (fxdata d6121033) | summon data: 'so' model, rock spike with a rock texture, drawn sliced as falling blocks (Hi_DrawSliceEffModel) |
| 0x81 | `FX_FENRIR_ROCK_SHARD` | render (fxdata d6121033) | summon data: 'so' model, small rock shard, drawn with ROCK_SPIKES / ROCK_SHARD_2 |
| 0x82 | `FX_FENRIR_WIND_STREAK` | render | summon data: model, long teardrop streak (Hi_RegisterTexListModel; shape only) |
| 0x83 | `FX_FENRIR_WIND_SERPENT` | render | summon data: model, serpent-like streak with a head (shape only) |
| 0x84 | `FX_FENRIR_WIND_BEAST` | render | summon data: model, spiky beast-shaped mass (shape only) |
| 0x85 | `FX_FENRIR_WIND_SPINDLE` | render | summon data: model, spindle with a round end (shape only) |
| 0x87 | `FX_FENRIR_ROCK_SPIKES` | render (fxdata d6121033) | summon data: 'so' model, row of rock spikes |
| 0x92 | `FX_FENRIR_CAM_TRACK_9` | usage | summon data: camera key tracks, channel 9 |
| 0x93 | `FX_FENRIR_ROCK_SHARD_2` | render (fxdata d6121033) | summon data: 'so' model, small rock shard |
| 0x94 | `FX_FENRIR_SPIKE_BURST` | render (fxdata d6121033) | summon data: 'so' model, morph target, grey gouraud burst of spikes |
| 0x95 | `FX_FENRIR_WIND_BALL` | render (fxdata d6121033) | summon data: 'so' model, morph source, ball with a green wind texture |
| 0x96 | `FX_FENRIR_CAM_TRACK_4` | usage | summon data: camera key tracks, channel 4 |
| 0x9B | `FX_FENRIR_CAM_TRACK_START` | usage | summon data: camera key tracks of channels 3, 4, 9, 10 at the start |
| 0x9C | `FX_FENRIR_PIECE_TRACKS` | usage | summon data: rotation/position paths of the pieces (fxm_rotPosPathInit) |
| 0x9D | `FX_FENRIR_STRIP_PATHS_2` | usage | summon data: position paths of strip set 1 |
| 0x9E | `FX_FENRIR_STRIP_PATHS` | usage | summon data: position paths of strip set 0 (head position) |
| 0x9F | `FX_FENRIR_CAM_TRACK_3` | usage | summon data: camera key tracks, channel 3 |
| 0xA0 | `FX_FENRIR_SMOKE` | render | summon data: emitter, grey smoke puffs |
| 0xA2 | `FX_FENRIR_GREEN_RING_BURST` | render | summon data: emitter, cream-white / green gouraud rings |
| 0xA3 | `FX_FENRIR_GREEN_RING_BURST_2` | render | summon data: emitter, olive-cream / green gouraud rings |
| 0xA4 | `FX_FENRIR_WIND_TRAIL` | render (fxdata d6121033) | summon data: emitter, green screen-space trail and white dot sparks |
| 0xA5 | `FX_FENRIR_RED_MOON` | render | summon data: emitter, one big red moon sprite |
| 0xA6 | `FX_FENRIR_RED_MOON_GLOW` | render | summon data: emitter, red moon particle |

### fx_0e3_0: Bahamut, group 0 (FX_BAHAMUT_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_BAHAMUT_WING_TRAIL` | render | trail: white streak strip drawn from Bahamut's bones |
| 0x2 | `FX_BAHAMUT_PURPLE_BOLT` | render | trail: purple energy strip (fxm_drawOffsetTrail1) |
| 0x3 | `FX_BAHAMUT_TEAL_PUFF` | runtime (confirmed) | particles: a dark teal cloud puff |
| 0x5 | `FX_BAHAMUT_BLUE_RINGS` | render | emitter: dark-to-light blue gouraud rings |
| 0x7 | `FX_BAHAMUT_TERRAIN` | runtime (medium) | 'so' model, flat square grid drawn with a fading core colour: grid textured with aerial green mountain terrain, the ground seen from above |
| 0x8 | `FX_BAHAMUT_WHITE_BOLT` | runtime (confirmed) | trail: a thin white lightning bolt over a white cloud |
| 0xA | `FX_BAHAMUT_FIRE_SPARKS` | render | emitter: orange flame glows + spark sprites |
| 0xB | `FX_BAHAMUT_CLOUD_RING` | runtime | 'so' model, flat ring of tiles drawn with a fading colour: flat ring with a band of teal-blue cloud |
| 0xC | `FX_BAHAMUT_WHITE_RINGS` | render | rings: grey-white gouraud rings |
| 0x82 | `FX_BAHAMUT_SMOKE_STRIP` | runtime (medium) | summon data, 'so' model, the pillar (pillarRgb): wavy strip of grey-white streaky smoke |
| 0x83 | `FX_BAHAMUT_BLUE_SPARKS` | render | summon data: emitter, blue-white spark sprites |
| 0x85 | `FX_BAHAMUT_CLOUD_SPHERE` | render (fxdata d6121033) | summon data: 'so' model, sphere with a blue-white cloud texture, drawn fading at the end |
| 0x86 | `FX_BAHAMUT_SMOKE_ARC` | render (fxdata d6121033) | summon data: 'so' model, curved band with a grey smoke texture, stacked nine times as a cone |

### fx_0e3_1: Bahamut, group 1 (FX_BAHAMUT_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_BAHAMUT_SMOKE_RING` | render (fxdata d6121033) | 'so' model: ring band with a grey-brown smoke texture |
| 0x4 | `FX_BAHAMUT_EMBERS` | render | emitter: spiral of tiny orange sprites |
| 0x5 | `FX_BAHAMUT_FIRE_BURST` | render | emitter: red-white fire flashes |
| 0x6 | `FX_BAHAMUT_BLUE_SPARKS` | render | emitter: blue-white spark sprites falling |
| 0x7 | `FX_BAHAMUT_LENS_FLARE` | runtime | sprite line: lens-flare set (rainbow halo, yellow-green disc, white glow, orange ring, blue disc) |
| 0x8 | `FX_BAHAMUT_FIRE_RING` | render (fxdata d6121033) | 'so' model: ring band with a fire texture |
| 0x9 | `FX_BAHAMUT_FIRE_SPARKS` | render | emitter: orange flame glows + spark sprites |
| 0xA | `FX_BAHAMUT_PURPLE_BOLT` | render | trail: purple energy strip (fxm_drawOffsetTrail2) |
| 0xB | `FX_BAHAMUT_CHARGE_RINGS` | render | rings: white-orange-blue gouraud rings while charging |
| 0xC | `FX_BAHAMUT_BLUE_SPARKS_2` | render | emitter: blue-white spark sprites falling |
| 0xD | `FX_BAHAMUT_FLAME_STRIP` | render (fxdata d6121033) | 'so' model: flat gouraud strip, purple-red-yellow gradient, drawn with BLAST_MODEL |
| 0xE | `FX_BAHAMUT_ORANGE_STREAKS` | render (fxdata d6121033) | emitter: two fans, thin orange-white rising streaks |
| 0xF | `FX_BAHAMUT_SMOKE_SHELL` | runtime (medium) | 'so' model: half-cylinder shell on a page of grey cloudy smoke at run time |
| 0x11 | `FX_BAHAMUT_WHITE_BOLT_2` | runtime (confirmed) | trail: the white lightning strip |
| 0x12 | `FX_BAHAMUT_SPIN_RING` | render | rings: white-blue gouraud rings, spinning |
| 0x14 | `FX_BAHAMUT_BLUE_RINGS` | render | rings: dark-to-light blue gouraud rings |
| 0x15 | `FX_BAHAMUT_WHITE_BOLT` | runtime (confirmed) | trail: the white lightning strip (byte-identical to fx_0e3_0 entry 0x8) |
| 0x16 | `FX_BAHAMUT_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| 0x17 | `FX_BAHAMUT_ENERGY_ORBS` | render | emitter: cyan-green orbs + violet lightning strip |
| 0x18 | `FX_BAHAMUT_SPARKLE_RING` | render | sprites: ring of blue-white sparkles, 16 frames |
| 0x19 | `FX_BAHAMUT_FIRE_RING_BURST` | render | emitter: red-white fire flashes + crimson rings |
| 0x1A | `FX_BAHAMUT_VIOLET_RINGS` | render | rings: white-cyan-violet gouraud rings |
| 0x1B | `FX_BAHAMUT_PINK_FLAT_RING` | render | rings: white-salmon gouraud rings drawn flat |
| 0x1C | `FX_BAHAMUT_HALO_RINGS` | render | rings: white-crimson-blue gouraud rings |
| 0x1D | `FX_BAHAMUT_PURPLE_GLOW` | render (fxdata d6121033) | emitter: fan, a purple-blue glow |
| 0x1F | `FX_BAHAMUT_WHITE_FLASH_RING` | render | rings: white gouraud rings, the flash |
| 0x20 | `FX_BAHAMUT_EMBERS_2` | render | emitter: spiral of tiny orange sprites |
| 0x21 | `FX_BAHAMUT_BLUE_SLIVER` | render (fxdata d6121033) | 'so' model: thin blue-white gouraud sliver, drawn at the blast centre |
| 0x23 | `FX_BAHAMUT_LIGHTNING_BOLT` | render | trail: violet lightning strip (fxm_drawOffsetTrail3) |
| 0x25 | `FX_BAHAMUT_FIRE_DOME` | render (fxdata d6121033) | 'so' model: dome with a fire texture, drawn for the burst and the flash |
| 0x26 | `FX_BAHAMUT_RED_STRIP` | render (fxdata d6121033) | 'so' model: flat red gouraud strip with a bright centre line |
| 0x29 | `FX_BAHAMUT_PURPLE_GLOW_2` | render (fxdata d6121033) | emitter: fan, a purple-blue glow |
| 0x81 | `FX_BAHAMUT_GREY_CONE` | render (fxdata d6121033) | summon data: 'so' model, grey gouraud cone (dark base to white tip), drawn around the orb |
| 0x84 | `FX_BAHAMUT_SMOKE_ARC_2` | render (fxdata d6121033) | summon data: 'so' model, curved band with a grey smoke texture |
| 0x85 | `FX_BAHAMUT_CLOUD_SPHERE` | render (fxdata d6121033) | summon data: 'so' model, sphere with a blue-white cloud texture, drawn fading at the end (same entry as fx_0e3_0's) |

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
| 0x5 | `FX_SACRIFICE_WHITE_FLARE` | render (fxdata d6121033) | emitter: two fans of white light flare streaks, faint sprites with cyan-white rings |
| 0x9 | `FX_SACRIFICE_LENS_FLARES` | render | sprite line: lens-flare circles and rings |

### fx_0eb: enemy Psychokinesis (Nova Dragon, ...; FX_PSYCHOKINESIS_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_PSYCHOKINESIS_WHITE_RINGS_2` | render | emitter: white gouraud rings (byte-identical to fx_023 entry 0x3) |
| 0x2 | `FX_PSYCHOKINESIS_WHITE_RINGS` | render | emitter: white gouraud rings (byte-identical to fx_023 entry 0x1) |
| 0x3 | `FX_PSYCHOKINESIS_PSYCHIC_ORBS` | render | emitter: homing violet-to-pink orbs |
| 0x7 | `FX_PSYCHOKINESIS_GREY_RINGS` | render | emitter: white-to-grey gouraud rings, 8 frames |
| 0x8 | `FX_PSYCHOKINESIS_DARK_BUBBLES` | render | emitter: dark bubble screen trails |

### fx_0ec: Doom (FX_DOOM_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_DOOM_GOLD_FRAME` | render | model: ornate gold frame, rises sliced |
| 0x3 | `FX_DOOM_GOLD_PANEL` | render | model: carved gold panel, rises sliced |
| 0x4 | `FX_DOOM_REAPER_PICTURE` | render | model: panel with the picture of Death |
| 0x6 | `FX_DOOM_VIOLET_HALO` | runtime | particles: violet ring halo |
| 0x7 | `FX_DOOM_GOLD_LATTICE` | render | model: capsule with a gold lattice texture |
| 0x8 | `FX_DOOM_PINK_RINGS` | render | emitter: pink-white rings (+ particles, texture not in the file) |
| 0x9 | `FX_DOOM_CAMERA_POINT` | usage | table: SVECTOR whose pad sets the camera point height |
| 0xB | `FX_DOOM_RED_RING` | render | emitter: glowing red ring sprite |
| 0xC | `FX_DOOM_PINK_SMOKE` | render | emitter: pink-violet smoke |
| 0xD | `FX_DOOM_GLOW_SHELL` | kind/usage (look pending) | model: capsule, additive, texture page computed by the C (not resolved by the tool), drawn twice; runtime: tpage computed by the C, not resolved |
| 0xE | `FX_DOOM_CYAN_SPARKLES` | render | emitter: cyan sparkles in a tube |
| 0xF | `FX_DOOM_REAPER_PICTURE_2` | render | model: second panel with the picture of Death |
| 0x10 | `FX_DOOM_SWIRL_SHELL` | runtime (medium) | model: shell on a grey marbled swirl page at run time |
| 0x11 | `FX_DOOM_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| 0x13 | `FX_DOOM_GOURAUD_SHELL` | kind/usage (look pending) | model: gouraud capsule; the render draws it black (colours set by the C at run time?); runtime: still black with the run-time page |

### fx_0ed: Roulette (FX_ROULETTE_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x2 | `FX_ROULETTE_MODEL_DATA` | usage | model data passed to btlfx_registerSummonModel with ROULETTE_SUMMON_MODEL |
| 0x4 | `FX_ROULETTE_SMOKE_TEXTURE` | runtime (medium) | layer table: render-to-texture (btlfx_drawLayerTexture): 2 layers into VRAM (0x200,0x1A0) 64x64, rising purple smoke (byte-identical to fx_05e entry 0xB, played there) |
| 0x6 | `FX_ROULETTE_TARGET_RING` | render | sprite: white ring outline on the chosen target |
| 0x7 | `FX_ROULETTE_REAPER` | render (fxdata d6121033) | 'so' summon model (btlfx_registerSummonModel): the robed skull reaper holding a scythe (byte-identical to fx_05e entry 0x6, same render) |
| 0x8 | `FX_ROULETTE_REAPER_2` | render (fxdata d6121033) | 'so' model: the robed skull reaper with the scythe in another pose (same textures, 354 polys; byte-identical to fx_05e entry 0x1C, same render) |
| 0xA | `FX_ROULETTE_BLUE_TRAIL` | render | emitter: spiralling blue light-beam trail |
| 0xB | `FX_ROULETTE_GREEN_ORB` | render | emitter: green glows + white-salmon-violet rings |
| 0xC | `FX_ROULETTE_RED_RINGS` | render | emitter: pink-red spark sprites + white-red rings |
| 0x10 | `FX_ROULETTE_SMOKE` | render | emitter: grey-white smoke puffs (byte-identical to fx_05e entry 0x10, same render) |
| 0x11 | `FX_ROULETTE_SOUL_ORBS` | render | emitter: blue bubbles |
| 0x1D | `FX_ROULETTE_BLUE_TRAIL_2` | render | emitter: spiralling blue light-beam trail |

### fx_0ee: enemy Mirror (FX_MIRROR_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_MIRROR_WHITE_RINGS` | render | emitter: white rings + green glow |
| 0x2 | `FX_MIRROR_WHITE_RINGS_2` | render | emitter: white rings + green glow |
| 0x3 | `FX_MIRROR_GREEN_SWIRL` | render | emitter: white rings + green swirl particle |
| 0x5 | `FX_MIRROR_LIGHT_SPIKES` | render (fxdata d6121033) | emitter: two fans, white-violet light spikes |

### fx_0ef: enemy Red Clipper (Stilva; FX_RED_CLIPPER_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x8 | `FX_RED_CLIPPER_RED_SLASH` | render | emitter: red slash texture + yellow rings + sparks (texture not in the file) |
| 0xE | `FX_RED_CLIPPER_PINK_SMOKE` | render | emitter: pink smoke puffs |
| 0xF | `FX_RED_CLIPPER_RED_SMOKE` | render | emitter: brown-red smoke puffs |

### fx_0f0: enemy Zombie Powder (Whale Zombie; FX_ZOMBIE_POWDER_)

| index | name | evidence | what it is |
|---|---|---|---|
| 0x1 | `FX_ZOMBIE_POWDER_BLUE_SPECKS` | runtime (medium) | emitter: animated quads, blue specks at run time: the blue glowing dust drifting round the caster |
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
| 0x2 | `FX_TIDAL_FLAME_FIRE_DOME` | runtime | model: hemisphere; page 0x11A holds flames at run time: the fire dome behind the target |
| 0x3 | `FX_TIDAL_FLAME_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| 0x8 | `FX_TIDAL_FLAME_RUNE_BURST` | render | emitter: white rune glyphs with cyan-blue rings, embers, blue light streak |
| 0x9 | `FX_TIDAL_FLAME_FLAME_CLOUD` | render | emitter: embers + orange flame cloud |

## Open questions (emulator)

All still-pending names of all slices, grouped by battle setup with what to capture, are in
`docs/wip/fxdata_runtime.md` (the emulator checklist); the list below is this slice's original notes.

25 names are still by kind (look pending) after the re-render with `tools/fxdata.py` at d6121033 (summon data, fans, 'so' models and dot sprites drawn). What is left is texture the file does not hold or the tool does not resolve (blank or placeholder pages filled at run time, tpages the C computes, texture lists the tool does not follow, garbled texels). Each wants one screenshot at the frame the C starts the entry, or a VRAM dump while it plays:

| module | index | name | why the look is pending |
|---|---|---|---|
| fx_0d9 | 0xd | `FX_SWDMAG_THUNDAGA_DISC` | model: flat octagon on 15-bit texture page 0x119, which the file does not fill (drawn at run time) (byte-identical to fx_018 entry 0x10); runtime: as Thundaga's disc (fx_018 0x10) its page holds 4-bit lightning textures at run time, read as 15-bit noise |
| fx_0d9 | 0x26 | `FX_SWDMAG_THUNDAGA_SPARKLE_SWARM` | emitter: Thundaga's swarms of blue, lilac and teal twinkle dots (byte-identical to fx_018 entry 0x26) (runtime) |
| fx_0dd | 0x11 | `FX_SWDMAG_BIO_BUBBLE_TRAIL` | emitter: Bio's screen trail of glowing green bubbles (runtime) |
| fx_0df | 0x9 | `FX_SWDMAG_FLARE_GOLDEN_GLOWS` | emitter: two fans; the render shows only yellow glow dots on a dark strip (texture mostly outside the file) |
| fx_0df | 0x19 | `FX_SWDMAG_FLARE_TEX_RING` | rings: textured ring; the render reads garbled multicolour texels (texture not in the file); runtime: the ring UV box reads noise in the run-time pages too |
| fx_0df | 0x1a | `FX_SWDMAG_FLARE_TEX_RING_2` | rings: textured ring; the render reads garbled multicolour texels (texture not in the file); runtime: noise in the run-time page too |
| fx_0e1_0 | 0x80 | `FX_REBIRTH_FLAME_SCREEN_SPHERE` | summon data, 'so' model, morph end, then drawn fading: the captured battle scene wrapped on a sphere, the morph target the flat capture bends into (runtime) |
| fx_0e1_0 | 0x81 | `FX_REBIRTH_FLAME_SCREEN_BULGE` | summon data, 'so' model, morph middle: the in-between morph shape with the same captured scene texture (runtime (medium)) |
| fx_0e1_0 | 0x82 | `FX_REBIRTH_FLAME_SCREEN_PLANE` | summon data, 'so' model, morph start: flat grid carrying the captured battle scene, the picture before it is bent (runtime) |
| fx_0e1_1 | 0x1 | `FX_REBIRTH_FLAME_TEX_EMITTER` | emitter: textured elements + tan-white rings; the textured element still reads garbled texels (texture not in the file); runtime: fetched during the fire blast (orange-white flames on screen); its ring elements read noise in the run-time page |
| fx_0e3_0 | 0x3 | `FX_BAHAMUT_TEAL_PUFF` | particles: a dark teal cloud puff (runtime (confirmed)) |
| fx_0e3_0 | 0x7 | `FX_BAHAMUT_TERRAIN` | 'so' model, flat square grid drawn with a fading core colour: grid textured with aerial green mountain terrain, the ground seen from above (runtime (medium)) |
| fx_0e3_0 | 0x8 | `FX_BAHAMUT_WHITE_BOLT` | trail: a thin white lightning bolt over a white cloud (runtime (confirmed)) |
| fx_0e3_0 | 0xb | `FX_BAHAMUT_CLOUD_RING` | 'so' model, flat ring of tiles drawn with a fading colour: flat ring with a band of teal-blue cloud (runtime) |
| fx_0e3_0 | 0x82 | `FX_BAHAMUT_SMOKE_STRIP` | summon data, 'so' model, the pillar (pillarRgb): wavy strip of grey-white streaky smoke (runtime (medium)) |
| fx_0e3_1 | 0x7 | `FX_BAHAMUT_LENS_FLARE` | sprite line: lens-flare set (rainbow halo, yellow-green disc, white glow, orange ring, blue disc) (runtime) |
| fx_0e3_1 | 0xf | `FX_BAHAMUT_SMOKE_SHELL` | 'so' model: half-cylinder shell on a page of grey cloudy smoke at run time (runtime (medium)) |
| fx_0e3_1 | 0x11 | `FX_BAHAMUT_WHITE_BOLT_2` | trail: the white lightning strip (runtime (confirmed)) |
| fx_0e3_1 | 0x15 | `FX_BAHAMUT_WHITE_BOLT` | trail: the white lightning strip (byte-identical to fx_0e3_0 entry 0x8) (runtime (confirmed)) |
| fx_0ec | 0x6 | `FX_DOOM_VIOLET_HALO` | particles: violet ring halo (runtime) |
| fx_0ec | 0xd | `FX_DOOM_GLOW_SHELL` | model: capsule, additive, texture page computed by the C (not resolved by the tool), drawn twice; runtime: tpage computed by the C, not resolved |
| fx_0ec | 0x10 | `FX_DOOM_SWIRL_SHELL` | model: shell on a grey marbled swirl page at run time (runtime (medium)) |
| fx_0ec | 0x13 | `FX_DOOM_GOURAUD_SHELL` | model: gouraud capsule; the render draws it black (colours set by the C at run time?); runtime: still black with the run-time page |
| fx_0f0 | 0x1 | `FX_ZOMBIE_POWDER_BLUE_SPECKS` | emitter: animated quads, blue specks at run time: the blue glowing dust drifting round the caster (runtime (medium)) |
| fx_0f2 | 0x2 | `FX_TIDAL_FLAME_FIRE_DOME` | model: hemisphere; page 0x11A holds flames at run time: the fire dome behind the target (runtime) |
