# Battle effect data, slice 2 (fx_0b1 .. fx_0d8)

Names for the `btlfx_getFileData` literals of the modules fx_0b1 .. fx_0d8, by the scheme and tool in
`docs/fxdata.md` (`tools/fxdata.py <module> --render`, one contact-sheet look per module plus the C call order).
Evidence: `render` = what the tool draws; `usage` = what the C does with it (backdrop tables, key tables), no
look needed; `kind/usage (look pending)` = appearance not settled, listed under open questions.

Notes:
- Swd Mag (fx_0d4 Fire .. fx_0d8 Thundara): the files carry the spells' entries byte for byte (fx_000, fx_002,
  fx_007, fx_003, fx_005), so the pilot names carry over with the prefix `FX_SWDMAG_<spell>_` ("Swd Mag" is the
  command's in-game name, the abilities are named like the spells). Each adds two white ring flashes at the
  sword (`btlfx_getWeaponPos`), `..._SWORD_GLINT(_2)`.
- Twins: fx_0c2 (No Mercy, Elan), fx_0c4 (Curse, Elan), fx_0c6 (Aura, Elan) run the same entries as fx_0c1,
  fx_0c3, fx_0c5 under other indices.
- Skill names checked with `tools/ff9text.py dump abilities` (player skills); enemy skill names from
  docs/effect-modules.md.

## Table

| module | index | name | evidence | what it is |
|---|---|---|---|---|
| fx_0b1 | 0x1 | `FX_CARBUNCLE_RING_BURST_2` | render | emitter: cone of white-red, lilac, cyan and blue-grey gouraud rings, fan |
| fx_0b1 | 0x9 | `FX_CARBUNCLE_LENS_FLARE` | render | emitter: orbiting white glow trail with a blue lens streak |
| fx_0b1 | 0xA | `FX_CARBUNCLE_GREEN_MOTES` | runtime (medium) | emitter: faint pale round glow, the green motes drifting round the emerald crystal |
| fx_0b1 | 0xC | `FX_CARBUNCLE_RED_RING` | render | emitter: white-red gouraud rings, fans |
| fx_0b1 | 0xD | `FX_CARBUNCLE_WHITE_LIGHT_STREAK` | render (fxdata d6121033) | emitter: fan, a white horizontal light streak |
| fx_0b1 | 0xE | `FX_CARBUNCLE_LENS_STREAK` | render | emitter: blue-white lens streak, white-red ring |
| fx_0b1 | 0xF | `FX_CARBUNCLE_CAMERA_KEYS` | usage | table: camera key path for fxm_camKeyAdd (target 1) |
| fx_0b1 | 0x10 | `FX_CARBUNCLE_LIGHT_BEAM` | render | trail: blue-white light beam, drawn twice (fx_0b1_beams) |
| fx_0b1 | 0x11 | `FX_CARBUNCLE_RING_BURST` | render | emitter: cone of white-yellow, lilac, cyan and blue-grey gouraud rings, fan |
| fx_0b1 | 0x12 | `FX_CARBUNCLE_CRYSTAL_SPARKLES` | render | emitter: white crystal-flake sprites and blue sparkles, fans |
| fx_0b1 | 0x13 | `FX_CARBUNCLE_FLAME_GLINT` | render | emitter: orange-blue flame-shaped glints, red gouraud rings, fans |
| fx_0b1 | 0x14 | `FX_CARBUNCLE_RING_BURST_3` | render | emitter: cone of white-red, lilac, cyan and blue-grey gouraud rings, fan |
| fx_0b1 | 0x15 | `FX_CARBUNCLE_LENS_STREAK_2` | render | emitter: blue-white lens streak, white-red ring, orange sprites |
| fx_0b1 | 0x16 | `FX_CARBUNCLE_ORANGE_MOTES` | runtime | emitter: small orange glow, the orange motes swirling after the burst |
| fx_0b1 | 0x17 | `FX_CARBUNCLE_CAMERA_KEYS_3` | usage | table: camera key path for fxm_camKeyAdd (target 3) |
| fx_0b1 | 0x18 | `FX_CARBUNCLE_COLOR_RINGS` | render | emitter: pink / green / pink gouraud rings at each target body |
| fx_0b1 | 0x19 | `FX_CARBUNCLE_CAMERA_KEYS_2` | usage | table: camera key path for fxm_camKeyAdd (target 2) |
| fx_0b1 | 0x21 | `FX_CARBUNCLE_PINK_RING` | render | emitter: pink gouraud ring and swirl-ring particles at each target body |
| fx_0b1 | 0x22 | `FX_CARBUNCLE_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_0b1 | 0x23 | `FX_CARBUNCLE_STAR_CRYSTAL` | render | model: four-point star crystal (texture 0x177), one per target |
| fx_0b1 | 0x24 | `FX_CARBUNCLE_ORANGE_MOTES_2` | runtime | emitter: the same small orange glow motes |
| fx_0b2 | 0x1 | `FX_TWISTER_VORTEX` | render | particles: tan swirling vortex, drawn directly |
| fx_0b2 | 0x2 | `FX_TWISTER_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_0b2 | 0x4 | `FX_TWISTER_WIND_STREAK` | render | trail: violet-white wind streak (fxm_drawOffsetTrail) |
| fx_0b2 | 0x5 | `FX_TWISTER_DUST_CLOUDS` | render | emitter: spiralling grey-blue dust clouds |
| fx_0b3 | 0x2 | `FX_LEVIATHAN_WATER_SPLASH` | render | emitter: blue water-splash sprites and droplets |
| fx_0b3 | 0x3 | `FX_LEVIATHAN_SUMMON_KEYS` | usage | table: key track (position, rotation, scale) of the summoned body (fxm_keyGetVec3) |
| fx_0b3 | 0x4 | `FX_LEVIATHAN_WATER_WISP` | render (fxdata d6121033) | emitter: orbiting fan, a twisting light-blue water wisp |
| fx_0b3 | 0x5 | `FX_LEVIATHAN_WATER_DROPS` | render | emitter: water droplet sprites |
| fx_0b3 | 0x6 | `FX_LEVIATHAN_BUBBLES` | render (fxdata d6121033) | emitter: dark blue bubbles and rainbow-rimmed bubble rings |
| fx_0b3 | 0x7 | `FX_LEVIATHAN_BLUE_RING` | render | emitter: blue gouraud ring + water-page particle |
| fx_0b3 | 0x8 | `FX_LEVIATHAN_WATER_SURFACE` | render (fxdata d6121033) | 'so' model registered by op80: flat square plane with the water texture |
| fx_0b3 | 0xB | `FX_LEVIATHAN_MORPH_POS_KEYS` | usage | table: key track, position of the morph models (op80) |
| fx_0b3 | 0xC | `FX_LEVIATHAN_MODEL_SCALE_KEYS_3` | usage | table: key track, scale of an effect model (op84) |
| fx_0b3 | 0x13 | `FX_LEVIATHAN_SUMMON_POS_KEYS` | usage | table: key track, position of the summon model (op83) |
| fx_0b3 | 0x15 | `FX_LEVIATHAN_SUMMON_POS_KEYS_2` | usage | table: key track, position of the summon model (op84) |
| fx_0b3 | 0x16 | `FX_LEVIATHAN_SUMMON_POS_KEYS_3` | usage | table: key track, position of the summon model (op85) |
| fx_0b3 | 0x1E | `FX_LEVIATHAN_WATER_GLINT` | render (fxdata d6121033) | emitter: orbiting particle, a blue diagonal water glint |
| fx_0b3 | 0x1F | `FX_LEVIATHAN_WATER_SPLASH_2` | render | emitter: blue water-splash sprites and droplets |
| fx_0b3 | 0x20 | `FX_LEVIATHAN_MORPH_KEYS` | usage | table: key track, position and scale of the morph models (op81) |
| fx_0b3 | 0x23 | `FX_LEVIATHAN_SPRAY` | render | emitter: blue water spray clouds |
| fx_0b3 | 0x24 | `FX_LEVIATHAN_FOAM` | render | animquads: blue-white foam, drawn along the wave edge |
| fx_0b3 | 0x25 | `FX_LEVIATHAN_WATER_DROPS_2` | render | emitter: water droplet sprites |
| fx_0b3 | 0x26 | `FX_LEVIATHAN_FOAM_2` | render | animquads: blue-white foam cloud |
| fx_0b3 | 0x27 | `FX_LEVIATHAN_ORBIT_POS_KEYS_2` | usage | table: key track, position of the orbit particle emitter (op84) |
| fx_0b3 | 0x2D | `FX_LEVIATHAN_MODEL_SCALE_KEYS` | usage | table: key track, scale of an effect model (op83) |
| fx_0b3 | 0x2E | `FX_LEVIATHAN_ORBIT_POS_KEYS` | usage | table: key track, position of the orbit particle emitter (op81) |
| fx_0b3 | 0x2F | `FX_LEVIATHAN_WATER_STREAK` | render | emitter: blue water streak (screen trail) |
| fx_0b3 | 0x30 | `FX_LEVIATHAN_RAINBOW_RING` | render | emitter: blue / green / yellow / red gouraud ring |
| fx_0b3 | 0x31 | `FX_LEVIATHAN_WATER_STREAK_2` | render | emitter: blue water streak (3D trail) and spray |
| fx_0b3 | 0x3F | `FX_LEVIATHAN_MODEL_SCALE_KEYS_2` | usage | table: key track, scale of an effect model (op84) |
| fx_0b3 | 0x86 | `FX_LEVIATHAN_SPLASH_CROWN` | render (fxdata d6121033) | summon data entry 6: 'so' model, crown of light-blue water spikes |
| fx_0b3 | 0x87 | `FX_LEVIATHAN_WATER_COLUMN` | render (fxdata d6121033) | summon data entry 7: 'so' model, cylinder with the water texture |
| fx_0b3 | 0x88 | `FX_LEVIATHAN_SPLASH_RING` | render (fxdata d6121033) | summon data entry 8: 'so' model, low ring of water spikes |
| fx_0b3 | 0x89 | `FX_LEVIATHAN_SEA` | render (fxdata d6121033) | summon data entry 9: 'so' model, the sea (wide water disc with a rim and sky texture), drawn as fx_0b3_seaModel |
| fx_0b3 | 0x8A | `FX_LEVIATHAN_WAVE_SPIRAL` | render (fxdata d6121033) | summon data entry 10: 'so' model, flat spiral of wave ribbons |
| fx_0b3 | 0x8B | `FX_LEVIATHAN_WAVE_COIL` | render (fxdata d6121033) | summon data entry 11: 'so' model, stacked wave rings (coil) |
| fx_0b3 | 0x8C | `FX_LEVIATHAN_WHIRLPOOL` | render (fxdata d6121033) | summon data entry 12: 'so' model, cone-shaped wave spiral (whirlpool) |
| fx_0b3 | 0x8D | `FX_LEVIATHAN_WATER_SPOUT` | render (fxdata d6121033) | summon data entry 13: 'so' model, funnel-shaped water spout |
| fx_0b3 | 0x8E | `FX_LEVIATHAN_FOAM_STRIP` | render (fxdata d6121033) | summon data entry 14: 'so' model, long strip with a white foam texture |
| fx_0b3 | 0x8F | `FX_LEVIATHAN_BLUE_FLASH` | render (fxdata d6121033) | summon data entry 15: 'so' model, small quad with a blue light flash |
| fx_0b4 | 0xE | `FX_AERO_GREEN_RING` | render | emitter: orbiting green gouraud rings |
| fx_0b4 | 0xF | `FX_AERO_WIND_RING` | render | emitter: orbiting pale green wind-ring particles |
| fx_0b4 | 0x10 | `FX_AERO_GREEN_BURST` | render | emitter: green gust puffs fading to blue |
| fx_0b4 | 0x11 | `FX_AERO_GREEN_GLINTS` | runtime (low, look pending) | emitter: spiralling sprites, green four-point star glints at run time (not ablated) |
| fx_0b4 | 0x19 | `FX_AERO_WIND_TRAIL` | render | emitter: white 3D wind trail spiralling |
| fx_0b5 | 0x1 | `FX_RAINBOW_WIND_RAINBOW_TRAIL` | render | emitter: rainbow-coloured 3D trail spiralling up |
| fx_0b5 | 0x2 | `FX_RAINBOW_WIND_WHIRLWIND` | render | emitter: blue-white whirlwind swirl particles |
| fx_0b5 | 0x5 | `FX_RAINBOW_WIND_RAINBOW_RING` | render | emitter: rainbow ring particle |
| fx_0b6 | 0x1 | `FX_SCORCH_FIREBALL` | render | emitter: red-orange swirling fireball particle |
| fx_0b6 | 0x2 | `FX_SCORCH_FLAME_STREAKS` | render (fxdata d6121033) | emitter: two fans of white-yellow to red flame streaks fading out |
| fx_0b6 | 0x7 | `FX_SCORCH_HEAT_RING` | render | emitter: white-pink gouraud rings |
| fx_0b7 | 0x2 | `FX_HEAVY_RED_STREAKS` | render (fxdata d6121033) | emitter: two orbiting fans of small red streaks |
| fx_0b7 | 0x6 | `FX_HEAVY_GREEN_SWIRL` | render | emitter: green swirl ring particle + pink-white gouraud rings |
| fx_0b8 | 0x2 | `FX_ATOMOS_BLUE_SPECKS` | runtime (medium) | emitter: sparse tiny blue specks flying round the gate as it opens |
| fx_0b8 | 0x3 | `FX_ATOMOS_VORTEX_STREAK` | render | trail: violet vortex streak |
| fx_0b8 | 0x7 | `FX_ATOMOS_ORANGE_STREAKS` | render (fxdata d6121033) | emitter: two fans, thin orange light streaks |
| fx_0b8 | 0x8 | `FX_ATOMOS_BLUE_SPECKS_2` | runtime (medium) | emitter: the same tiny blue specks, during the vortex |
| fx_0b8 | 0x9 | `FX_ATOMOS_VIOLET_RINGS` | render | emitter: violet streak and violet / dark blue gouraud rings |
| fx_0b8 | 0xA | `FX_ATOMOS_PINK_CLOUD` | render | emitter: violet-blue and white-red rings, pink cloud particles |
| fx_0b8 | 0xB | `FX_ATOMOS_FAINT_GLOW` | kind/usage (look pending) | emitter: spiralling glow particles; the render is almost black (a faint white glow); runtime: with the run-time page its particle reads a grey zigzag band, nothing like the white mist on screen |
| fx_0b8 | 0xC | `FX_ATOMOS_ROCK_DEBRIS` | render | emitter: grey-blue rock debris particles |
| fx_0b8 | 0xD | `FX_ATOMOS_BLAST_RINGS` | render | emitter: white / yellow / orange / brown and purple gouraud rings |
| fx_0b8 | 0xE | `FX_ATOMOS_VORTEX_SPARKLES` | render | emitter: orbiting violet vortex trails and gold sparkles |
| fx_0b8 | 0xF | `FX_ATOMOS_UNIT_RING` | render | emitter: white-grey gouraud rings at each unit |
| fx_0b8 | 0x10 | `FX_ATOMOS_SPHERE` | render | model: sphere, texture 0x17C (not in the file) |
| fx_0b8 | 0x11 | `FX_ATOMOS_RING_FUNNEL` | render | model: gouraud ring funnel (flat torus) |
| fx_0b8 | 0x12 | `FX_ATOMOS_CLOUD_FUNNEL` | render | model: ring funnel with a grey cloud texture |
| fx_0b8 | 0x13 | `FX_ATOMOS_ORANGE_RING` | render | model: ring with an orange cloud texture |
| fx_0b8 | 0x1F | `FX_ATOMOS_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_0b8 | 0x20 | `FX_ATOMOS_VIOLET_RING` | render | emitter: white-violet and white-blue gouraud rings |
| fx_0b8 | 0x21 | `FX_ATOMOS_PURPLE_RINGS` | render | emitter: white / magenta / purple and pale yellow / grey gouraud rings |
| fx_0b8 | 0x22 | `FX_ATOMOS_BLUE_SPECKS_3` | runtime (medium) | emitter: the same blue specks round the black hole |
| fx_0b8 | 0x80 | `FX_ATOMOS_GREY_RING` | render (fxdata d6121033) | summon data entry 0: 'so' model, thin white-grey gouraud ring |
| fx_0b8 | 0x81 | `FX_ATOMOS_CRATER_MOUND` | render (fxdata d6121033) | summon data entry 1: 'so' model, grey gouraud crater mound |
| fx_0b8 | 0x82 | `FX_ATOMOS_WHITE_PILLAR` | render (fxdata d6121033) | summon data entry 2: 'so' model, white octagonal gouraud pillar |
| fx_0b8 | 0x83 | `FX_ATOMOS_GROUND_CHUNK` | render (fxdata d6121033) | summon data entry 3: 'so' model, chunk of rocky ground (earth cylinder with grass top) |
| fx_0b8 | 0x84 | `FX_ATOMOS_PINE_TREES` | render (fxdata d6121033) | summon data entry 4: 'so' model, row of pine-tree cards |
| fx_0b8 | 0x85 | `FX_ATOMOS_PURPLE_DOME` | render (fxdata d6121033) | summon data entry 5: textured model, purple dome with a cyan star pattern |
| fx_0b8 | 0x86 | `FX_ATOMOS_GOLD_RING` | render (fxdata d6121033) | summon data entry 6: 'so' model, gold-brown gouraud ring |
| fx_0b8 | 0x87 | `FX_ATOMOS_PINE_TREES_2` | render (fxdata d6121033) | summon data entry 7: 'so' model, row of pine-tree cards |
| fx_0b8 | 0x88 | `FX_ATOMOS_LAVA_CRATER` | render (fxdata d6121033) | summon data entry 8: 'so' model, crater mound with a fire / lava texture |
| fx_0b8 | 0x89 | `FX_ATOMOS_CLOUD_DISC` | render (fxdata d6121033) | summon data entry 9: textured model, disc with a dark cloud texture |
| fx_0b8 | 0x8A | `FX_ATOMOS_RUBBLE_BAND` | render (fxdata d6121033) | summon data entry 10: 'so' model, ring band with a grey rubble / smoke texture |
| fx_0b8 | 0x8B | `FX_ATOMOS_WHITE_DROP` | render (fxdata d6121033) | summon data entry 11: textured model, thin white teardrop (needle) |
| fx_0b8 | 0x8C | `FX_ATOMOS_WHITE_DROP_2` | render (fxdata d6121033) | summon data entry 12: textured model, white teardrop |
| fx_0b8 | 0x8D | `FX_ATOMOS_WHITE_DROP_3` | render (fxdata d6121033) | summon data entry 13: textured model, slim white teardrop |
| fx_0b8 | 0x8E | `FX_ATOMOS_WHITE_DROP_4` | render (fxdata d6121033) | summon data entry 14: textured model, wide white teardrop |
| fx_0b9 | 0x1 | `FX_SANDSTORM_SAND_TRAIL_2` | render | emitter: screen trail with a grey sand streak strip (byte-identical to fx_04b entry 0x3) |
| fx_0b9 | 0x2 | `FX_SANDSTORM_SAND_TRAIL` | render | emitter: homing trail with a grey sand streak strip (byte-identical to fx_04b entry 0x2) |
| fx_0b9 | 0x3 | `FX_SANDSTORM_SAND_CLOUD` | render | emitter: grainy grey sand clouds |
| fx_0ba | 0x4 | `FX_RAMUH_STAFF` | render (fxdata d6121033) | 'so' model: gold gouraud staff with a hooked head (Ramuh's staff) |
| fx_0ba | 0x5 | `FX_RAMUH_LIGHTNING_RIBBON` | render (fxdata d6121033) | trail: ribbon of blue-white electric glow drawn between key points |
| fx_0ba | 0x7 | `FX_RAMUH_LENS_FLASH_KEYS` | usage | table: per-frame size keys of the lens flash (FX_RAMUH_LENS_FLASH) |
| fx_0ba | 0xA | `FX_RAMUH_BLUE_GLOW_RINGS` | render | emitter: blue glow quad and fading light-blue gouraud rings |
| fx_0ba | 0xD | `FX_RAMUH_LIGHTNING_RIBBON_2` | render (fxdata d6121033) | trail: second blue-white electric ribbon drawn between key points |
| fx_0ba | 0x14 | `FX_RAMUH_BOLT_SPARK_BURST` | runtime (medium) | emitter: flashing white gouraud rings and blue spark sprites; at run time (fx_19f's byte-identical entry) small blue lightning-bolt glyphs in a glow, the cyan flash and bolt sparks round the strike |
| fx_0ba | 0x15 | `FX_RAMUH_LENS_FLASH` | render | particles: white-violet lens flash |
| fx_0ba | 0x16 | `FX_RAMUH_LIGHTNING_BOLT` | render (fxdata d6121033) | trail: 8-frame blue-white lightning bolt |
| fx_0ba | 0x18 | `FX_RAMUH_THIN_ROD` | render (fxdata d6121033) | 'so' model: thin dark rod with a red tip |
| fx_0ba | 0x19 | `FX_RAMUH_MOTION_KEYS_2` | usage | table: per-frame position / rotation keys (op81, second phase) |
| fx_0ba | 0x1A | `FX_RAMUH_MOTION_KEYS` | usage | table: per-frame position / rotation keys (op81, first phase) |
| fx_0ba | 0x1B | `FX_RAMUH_BLUE_RING` | render | rings: blue gouraud rings |
| fx_0ba | 0x1D | `FX_RAMUH_CYAN_RING` | render | rings: white / cyan / blue gouraud rings |
| fx_0ba | 0x1E | `FX_RAMUH_ORANGE_LIGHTNING` | render (fxdata d6121033) | trail: 8-frame orange lightning bolt (offset trail) |
| fx_0ba | 0x1F | `FX_RAMUH_ELECTRIC_BOWL` | render (fxdata d6121033) | 'so' model: shallow bowl with blue electric flames around a white glow |
| fx_0ba | 0x20 | `FX_RAMUH_CLOUD_PLANE` | render (fxdata d6121033) | 'so' model: flat square plane with a dark blue storm-cloud texture |
| fx_0ba | 0x21 | `FX_RAMUH_GREY_RING` | render | rings: grey gouraud rings |
| fx_0ba | 0x23 | `FX_RAMUH_BOLT_POINTS` | usage | table: ten points around the summon, picked at random (op83) |
| fx_0ba | 0x24 | `FX_RAMUH_BOLT_POINTS_2` | usage | table: second set of ten points around the summon (op83) |
| fx_0ba | 0x26 | `FX_RAMUH_GREEN_DOME` | render (fxdata d6121033) | 'so' model: teal-green gouraud hemisphere |
| fx_0ba | 0x27 | `FX_RAMUH_WHITE_RING` | render | rings: white gouraud ring |
| fx_0ba | 0x29 | `FX_RAMUH_SPARK_RING` | render | rings: pale cyan gouraud rings (pool items) |
| fx_0ba | 0x2C | `FX_RAMUH_BOLT_SPARKS` | runtime (medium) | emitter: one particle; not fetched in a full Ramuh (a branch that cast did not take); fx_19f's byte-identical entry is a blue lightning-bolt glyph sprite at run time, the small bolt sparks around the pillar |
| fx_0ba | 0x2E | `FX_RAMUH_CAMERA_KEYS` | usage | table: per-frame camera point 0 (op82) |
| fx_0ba | 0x2F | `FX_RAMUH_CAMERA_KEYS_2` | usage | table: per-frame camera point 1 (op82) |
| fx_0ba | 0x32 | `FX_RAMUH_POOL_LIGHTNING` | render (fxdata d6121033) | trail: 8-frame blue-white lightning of the op85 pool items |
| fx_0ba | 0x80 | `FX_RAMUH_CLOUD_RING` | render (fxdata d6121033) | summon data entry 0: 'so' model, ring mound with the storm-cloud texture |
| fx_0ba | 0x81 | `FX_RAMUH_CLOUD_DISC` | render (fxdata d6121033) | summon data entry 1: 'so' model, flat disc with the storm-cloud texture |
| fx_0ba | 0x82 | `FX_RAMUH_CLOUD_PLANE_2` | render (fxdata d6121033) | summon data entry 2: 'so' model, flat square plane with the storm-cloud texture |
| fx_0ba | 0x83 | `FX_RAMUH_CAPPED_COLUMN` | render (fxdata d6121033) | summon data entry 3: 'so' model, dark blue column with a mushroom-shaped cap (its texture reads as a "dummy" placeholder) |
| fx_0ba | 0x84 | `FX_RAMUH_STORM_CLOUDS` | render (fxdata d6121033) | summon data entry 4: emitter, orbiting animated blue cloud puffs |
| fx_0ba | 0x85 | `FX_RAMUH_SUMMON_KEYS_5` | usage | summon data entry 5: per-frame key table (op80) |
| fx_0ba | 0x86 | `FX_RAMUH_BLUE_FLASH` | render (fxdata d6121033) | summon data entry 6: emitter, orbiting fan of a horizontal blue light flash |
| fx_0bb | 0x1 | `FX_MUSTARD_BOMB_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_0bb | 0x7 | `FX_MUSTARD_BOMB_FIREBALLS` | render | emitter: orange fireball sprites with light streaks |
| fx_0bb | 0x9 | `FX_MUSTARD_BOMB_EXPLOSION_RINGS` | render | emitter: yellow-orange-red and blue gouraud rings (+ a textured ring, texture not in the file) |
| fx_0bb | 0xA | `FX_MUSTARD_BOMB_BLAST_RINGS` | render | emitter: cone of yellow / orange / brown gouraud rings |
| fx_0bc | 0x1 | `FX_DARKSIDE_SPARKLE_BURST` | render | emitter: yellow / blue ring, blue sparkle sprites, fan |
| fx_0bd | 0x4 | `FX_MINUS_STRIKE_SLASH_ARC` | render | emitter: blue slash arc, blue sparkle sprites, pale yellow ring, fan |
| fx_0be | 0x4 | `FX_IAI_STRIKE_SLASH_ARCS` | render | emitter: blue and pink slash arcs |
| fx_0be | 0x8 | `FX_IAI_STRIKE_SPARKLE_BURST` | render | emitter: violet / blue ring, blue and gold sparkle sprites, fan |
| fx_0bf | 0x1 | `FX_THUNDER_SLASH_SPARKS` | render | emitter: cone of blue-green electric wisps, orange sparks and white glints |
| fx_0bf | 0x2 | `FX_THUNDER_SLASH_LIGHTNING` | render | sprite: blue / red lightning streak, 12 frames (drawn directly) |
| fx_0c0 | 0x7 | `FX_SHOCK_SPARKLES_2` | render | emitter: blue sparkle sprites (+ a dark sprite element, fans) |
| fx_0c0 | 0x9 | `FX_SHOCK_ENERGY_ORB` | render | emitter: blue-white energy orb particle |
| fx_0c0 | 0xB | `FX_SHOCK_FLAME_RING` | render | emitter: ring textured with blue flames |
| fx_0c0 | 0xD | `FX_SHOCK_SPARKLES` | render | emitter: blue sparkle sprites, fan |
| fx_0c1 | 0x7 | `FX_NO_MERCY_BLUE_RING` | render | emitter: white / dark blue / cyan gouraud ring, fans |
| fx_0c1 | 0xB | `FX_NO_MERCY_DUST_PLUME` | render | emitter: rising tan dust column and blue smoke |
| fx_0c1 | 0xC | `FX_NO_MERCY_FLASH_BURST` | render | emitter: white star flash with a lens flare, grey-blue smoke, sparkles |
| fx_0c1 | 0xD | `FX_NO_MERCY_IMPACT_BURST` | render | emitter: white / blue / orange rings, white glow, grey-blue smoke |
| fx_0c2 | 0xA | `FX_NO_MERCY_FLASH_BURST` | render | emitter: white star flash with a lens flare, grey-blue smoke, sparkles (same call site as fx_0c1 entry 0xc) |
| fx_0c2 | 0xB | `FX_NO_MERCY_IMPACT_BURST` | render | emitter: white / blue / orange rings, white glow, grey-blue smoke (same call site as fx_0c1 entry 0xd) |
| fx_0c2 | 0xC | `FX_NO_MERCY_BLUE_RING` | render | emitter: white / dark blue / cyan gouraud ring, fans (byte-identical to fx_0c1 entry 0x7) |
| fx_0c2 | 0xD | `FX_NO_MERCY_DUST_PLUME` | render | emitter: rising tan dust column and blue smoke (same call site as fx_0c1 entry 0xb) |
| fx_0c3 | 0x6 | `FX_CURSE_SPIRIT_WISPS` | render | emitter: blue and magenta wisps (+ two rings, texture not in the file) |
| fx_0c3 | 0x8 | `FX_CURSE_FLAME_PILLAR` | render | emitter: orange-green flame sprite and grey smoke wisps |
| fx_0c4 | 0x6 | `FX_CURSE_FLAME_PILLAR` | render | emitter: orange-green flame sprite and grey smoke wisps (same call site as fx_0c3 entry 0x8) |
| fx_0c4 | 0x8 | `FX_CURSE_SPIRIT_WISPS` | render | emitter: blue and magenta wisps (+ two rings, texture not in the file) (same call site as fx_0c3 entry 0x6) |
| fx_0c5 | 0x7 | `FX_AURA_SWIRL` | render | emitter: yellow / white / lilac swirl particles, blue star flash, sparkles, fans |
| fx_0c5 | 0x8 | `FX_AURA_CRESCENT_SWOOSHES` | render (fxdata d6121033) | emitter: three orbiting fans, blue / pink / green crescent swooshes |
| fx_0c6 | 0x5 | `FX_AURA_CRESCENT_SWOOSHES` | render (fxdata d6121033) | emitter: three orbiting fans, blue / pink / green crescent swooshes (byte-identical to fx_0c5 entry 0x8) |
| fx_0c6 | 0x8 | `FX_AURA_SWIRL` | render | emitter: yellow / white / lilac swirl particles, blue star flash, sparkles, fans (byte-identical to fx_0c5 entry 0x7) |
| fx_0c7 | 0x1 | `FX_FOCUS_GLOW_RING` | render | emitter: orange gouraud ring, fans |
| fx_0c9 | 0x1 | `FX_JEWEL_GEM` | render | model: faceted gem (polyhedron, white texture) |
| fx_0c9 | 0x2 | `FX_JEWEL_GEM_SPARKLES` | render | emitter: stone / crystal glint particles and star sparkles (+ sprites, texture not in the file) |
| fx_0c9 | 0x3 | `FX_JEWEL_COMET_STREAKS` | render (fxdata d6121033) | emitter: two fans, white-violet comet streaks |
| fx_0c9 | 0x4 | `FX_JEWEL_LIGHT_BURST` | render | emitter: white / lilac / cyan rings and violet light streaks |
| fx_0c9 | 0x5 | `FX_JEWEL_ORE_GLINT` | render | emitter: green ore glint, lilac / cyan ring, star sparkles |
| fx_0c9 | 0x6 | `FX_JEWEL_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_0c9 | 0x10 | `FX_JEWEL_GLOW_RING` | render | emitter: white-cyan gouraud ring, fans |
| fx_0ca | 0x1 | `FX_POISON_BUBBLES` | render | emitter: purple poison blobs and bubbles |
| fx_0ca | 0x2 | `FX_POISON_SWIRL` | render | emitter: orbiting purple poison trail |
| fx_0cb | 0x5 | `FX_LIGHT_FLARE_GLOW_RINGS` | render | rings: white / cyan / orange / red gouraud rings |
| fx_0cb | 0x8 | `FX_LIGHT_FLARE_RED_HALO` | render (fxdata d6121033) | 'so' model: red-orange gouraud halo ring, drawn fading with the fireball |
| fx_0cb | 0xC | `FX_LIGHT_FLARE_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) (with the screen tint) |
| fx_0cb | 0xF | `FX_LIGHT_FLARE_BACKDROP_2` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) (with the rings) |
| fx_0cb | 0x11 | `FX_LIGHT_FLARE_GLOW_RING` | runtime | rings: textured ring drawn twice, tilted: the yellow-green glow ring round the target |
| fx_0cb | 0x12 | `FX_LIGHT_FLARE_FIREBALL` | render (fxdata d6121033) | 'so' model: sphere with a fire texture, drawn fading |
| fx_0cc | 0x2 | `FX_METEORITE_IMPACT_RING` | render | rings: white-pink gouraud ring at the impact |
| fx_0cc | 0x3 | `FX_METEORITE_IMPACT_SMOKE` | render | particles: pink-white smoke puff at the impact |
| fx_0cc | 0x7 | `FX_METEORITE_RED_GLOW` | render | particles: red glow around the falling rock |
| fx_0cc | 0x9 | `FX_METEORITE_EMBERS` | runtime (low, look pending) | emitter: sprites following the falling rock, small orange specks at run time |
| fx_0cc | 0xC | `FX_METEORITE_IMPACT_FLASH` | render | particles: white radial flash at the impact |
| fx_0cc | 0xF | `FX_METEORITE_ROCK` | render (fxdata d6121033) | 'so' model: dark brown rock (polyhedron), drawn spinning down |
| fx_0cd | 0x1 | `FX_AERA_GREEN_WIND_RING` | render (fxdata d6121033) | 'so' model: ring of green wind streaks (grass-fringe cylinder) |
| fx_0cd | 0x3 | `FX_AERA_GREY_ARC` | render (fxdata d6121033) | 'so' model: open grey arc band |
| fx_0cd | 0x4 | `FX_AERA_GREEN_RING` | render | emitter: green gouraud rings |
| fx_0cd | 0x5 | `FX_AERA_WIND_RING` | render | emitter: orbiting white wind-ring particle |
| fx_0cd | 0x7 | `FX_AERA_GREEN_BURST` | render | emitter: green gust puffs |
| fx_0cd | 0x8 | `FX_AERA_GREEN_WIND_BLADES` | runtime | emitter: spiralling sprites + 3D trail, the translucent green wind blades sweeping round the target |
| fx_0cd | 0xA | `FX_AERA_GUST_RINGS` | render | emitter: cyan-green gouraud rings |
| fx_0cd | 0xC | `FX_AERA_SPRITE_BURST` | kind/usage (look pending) | emitter: sprites; the render still reads garbled yellow texels (texture not resolved); runtime: its run-time sprite is a tiny green glint; ablating it changes < 210 px |
| fx_0cd | 0xD | `FX_AERA_GREEN_TORNADO` | render (fxdata d6121033) | 'so' model: twisted green column with a smoke texture (tornado) |
| fx_0ce | 0x1 | `FX_SCREECH_TRAIL` | kind/usage (look pending) | emitter: 3D trail; the render still reads garbled pink stripes (texture not resolved); runtime: ablating it changes only scattered pink squiggle pixels (max 1234 px) |
| fx_0ce | 0x2 | `FX_SCREECH_PURPLE_MIST` | render | emitter: dark purple mist rings and brown-pink gouraud rings |
| fx_0ce | 0x7 | `FX_SCREECH_BLADE_RING` | render (fxdata d6121033) | 'so' model: ring of pale green curved blades |
| fx_0ce | 0xB | `FX_SCREECH_SOUND_RINGS` | runtime (medium) | emitter: textured rings, the pale expanding sound-wave rings over the target |
| fx_0ce | 0xC | `FX_SCREECH_LILAC_RING` | render | emitter: lilac glow ring particle |
| fx_0cf | 0x1 | `FX_STOCK_BREAK_SLASH_ARC` | render | emitter: blue slash arc, fans |
| fx_0d0 | 0x7 | `FX_POWER_BREAK_RED_SWIRL` | render | emitter: red-white swirl ring particles |
| fx_0d0 | 0x8 | `FX_POWER_BREAK_LIGHT_STREAK` | render | emitter: yellow light streak with a flare, red ember sprites |
| fx_0d0 | 0xC | `FX_POWER_BREAK_ENERGY_ORB` | render | emitter: yellow energy orb particle |
| fx_0d0 | 0xD | `FX_POWER_BREAK_SPARKLES` | render | emitter: gold sparkle sprites, fan |
| fx_0d1 | 0x4 | `FX_MENTAL_BREAK_PINK_GLOW` | render | emitter: pink glow sprite with red ember sprites |
| fx_0d1 | 0x7 | `FX_MENTAL_BREAK_RINGS` | render | emitter: white-violet, white-pink and pink-purple gouraud rings |
| fx_0d1 | 0x9 | `FX_MENTAL_BREAK_GLOW_CROSS` | render | emitter: four pink glows in a cross, orange / blue ring |
| fx_0d1 | 0xD | `FX_MENTAL_BREAK_ENERGY_ORB` | render | emitter: pink energy orb particle |
| fx_0d1 | 0xE | `FX_MENTAL_BREAK_SPARKLES` | render | emitter: pink star sparkle sprites, fan |
| fx_0d2 | 0x1 | `FX_FENRIR_ORANGE_LIGHT_LINE` | render (fxdata d6121033) | emitter: orbiting fan, a thin horizontal orange-pink light line |
| fx_0d2 | 0x2 | `FX_FENRIR_DUST_CLOUD` | render | emitter: tan dust clouds and an orange gouraud ring |
| fx_0d2 | 0x3 | `FX_FENRIR_BLUE_RINGS` | render | emitter: white-blue gouraud rings |
| fx_0d2 | 0x4 | `FX_FENRIR_ORANGE_RING` | render | emitter: orange gouraud ring |
| fx_0d2 | 0x5 | `FX_FENRIR_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_0d2 | 0x8 | `FX_FENRIR_UNIT_PATHS` | usage | table: offset table of per-unit position paths (op83) |
| fx_0d2 | 0x9 | `FX_FENRIR_MORPH_PATH` | usage | table: rotation / position path of the morph models (fx_0d2_morphPath) |
| fx_0d2 | 0x14 | `FX_FENRIR_UNIT_OFFSET_PATH` | usage | table: position path (fx_0d2_unitOffsetPath) |
| fx_0d2 | 0x15 | `FX_FENRIR_UNIT_SCALE_PATH` | usage | table: vector path (fx_0d2_unitScalePath) |
| fx_0d2 | 0x19 | `FX_FENRIR_MORPH_CURVE` | usage | table: value path (fx_0d2_morphCurve2) |
| fx_0d2 | 0x1A | `FX_FENRIR_MORPH_CURVE_2` | usage | table: alternative value path (fx_0d2_morphCurve2) |
| fx_0d2 | 0x1B | `FX_FENRIR_ROCK_STREAK` | render | trail: brown rock-textured streak, drawn along 9 paths |
| fx_0d2 | 0x1D | `FX_FENRIR_CAMERA_KEYS_2` | usage | table: camera channel 2 keys from frame 0xDE (fxm_camChanAdd) |
| fx_0d2 | 0x1E | `FX_FENRIR_CAMERA_KEYS_3` | usage | table: camera channel 3 keys from frame 0xDD |
| fx_0d2 | 0x1F | `FX_FENRIR_CAMERA_KEYS_1` | usage | table: camera channel 1 keys from frame 0xDE |
| fx_0d2 | 0x20 | `FX_FENRIR_CAMERA_KEYS_4` | usage | table: camera channel 4 keys from frame 0xDD |
| fx_0d2 | 0x21 | `FX_FENRIR_DUST_DEBRIS` | render | emitter: tan dust clouds and rock debris quads |
| fx_0d2 | 0x25 | `FX_FENRIR_ROCK_STREAK_2` | render | trail: brown rock-textured streak, drawn along 11 paths |
| fx_0d2 | 0x28 | `FX_FENRIR_SLICE_PATH_2` | usage | table: rotation / position path (fx_0d2_slicePath2) |
| fx_0d2 | 0x80 | `FX_FENRIR_SUMMON_STREAK_PATHS_0` | usage | summon data entry 0: offset table of the rock-streak position paths |
| fx_0d2 | 0x81 | `FX_FENRIR_SUMMON_STREAK_PATHS_1` | usage | summon data entry 1: offset table of the rock-streak position paths |
| fx_0d2 | 0x82 | `FX_FENRIR_SUMMON_EMIT_PATH_2` | usage | summon data entry 2: position path of the emitters (fx_0d2_emitPath) |
| fx_0d2 | 0x83 | `FX_FENRIR_COLOR_RINGS` | render (fxdata d6121033) | summon data entry 3: emitter, white-teal and white-blue gouraud rings |
| fx_0d2 | 0x84 | `FX_FENRIR_SUMMON_RING_PATH_4` | usage | summon data entry 4: rotation / position path (fx_0d2_ringPath) |
| fx_0d2 | 0x85 | `FX_FENRIR_WINGED_CREATURE` | render (fxdata d6121033) | summon data entry 5: 'so' model, brown furred creature with spread wings and a green eye |
| fx_0d2 | 0x86 | `FX_FENRIR_ROCK_TERRACE` | render (fxdata d6121033) | summon data entry 6: 'so' model, stepped ground terrace with a sand-rock texture |
| fx_0d2 | 0x8D | `FX_FENRIR_ROCK_TERRACE_2` | render (fxdata d6121033) | summon data entry 13: 'so' model, stepped ground terrace with a sand-rock texture (same size as entry 6) |
| fx_0d2 | 0x8E | `FX_FENRIR_ROCK_CRAG` | render (fxdata d6121033) | summon data entry 14: 'so' model, rough brown rock crag |
| fx_0d2 | 0x8F | `FX_FENRIR_SUMMON_MORPH_CURVE_15` | usage | summon data entry 15: value path (fx_0d2_morphCurve) |
| fx_0d2 | 0x90 | `FX_FENRIR_SUMMON_SLICE_PATHS_16` | usage | summon data entry 16: offset table of six rotation / position paths (fx_0d2_slicePaths) |
| fx_0d2 | 0x91 | `FX_FENRIR_SUMMON_SLICE_PATHS_17` | usage | summon data entry 17: offset table of six rotation / position paths (fx_0d2_slicePaths) |
| fx_0d2 | 0x92 | `FX_FENRIR_SUMMON_PATH_18` | usage | summon data entry 18: position path of the summon (fx_0d2_summonPath) |
| fx_0d2 | 0x93 | `FX_FENRIR_SUMMON_CAMERA_KEYS_19` | usage | summon data entry 19: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0x94 | `FX_FENRIR_SUMMON_CAMERA_KEYS_20` | usage | summon data entry 20: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0x95 | `FX_FENRIR_SUMMON_CAMERA_KEYS_21` | usage | summon data entry 21: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0x96 | `FX_FENRIR_SUMMON_CAMERA_KEYS_22` | usage | summon data entry 22: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0x97 | `FX_FENRIR_SUMMON_CAMERA_KEYS_23` | usage | summon data entry 23: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0x98 | `FX_FENRIR_SUMMON_CAMERA_KEYS_24` | usage | summon data entry 24: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0x99 | `FX_FENRIR_SUMMON_CAMERA_KEYS_25` | usage | summon data entry 25: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0x9A | `FX_FENRIR_SUMMON_CAMERA_KEYS_26` | usage | summon data entry 26: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0x9B | `FX_FENRIR_SUMMON_CAMERA_KEYS_27` | usage | summon data entry 27: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0x9C | `FX_FENRIR_SUMMON_CAMERA_KEYS_28` | usage | summon data entry 28: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0x9D | `FX_FENRIR_SUMMON_CAMERA_KEYS_29` | usage | summon data entry 29: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0x9E | `FX_FENRIR_GOLD_HORN` | render (fxdata d6121033) | summon data entry 30: 'so' model, gold gouraud bent sheet (horn-shaped) |
| fx_0d2 | 0x9F | `FX_FENRIR_SUMMON_PATH_31` | usage | summon data entry 31: position path of the summon (fx_0d2_summonPath) |
| fx_0d2 | 0xA0 | `FX_FENRIR_SUMMON_CAMERA_KEYS_32` | usage | summon data entry 32: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0xA1 | `FX_FENRIR_SUMMON_CAMERA_KEYS_33` | usage | summon data entry 33: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0xA2 | `FX_FENRIR_SUMMON_CAMERA_KEYS_34` | usage | summon data entry 34: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0xA3 | `FX_FENRIR_SUMMON_CAMERA_KEYS_35` | usage | summon data entry 35: camera channel keys (fxm_camChanAdd, op80) |
| fx_0d2 | 0xA4 | `FX_FENRIR_MOON` | render (fxdata d6121033) | summon data entry 36: emitter, a pink-orange full-moon sprite |
| fx_0d2 | 0xA5 | `FX_FENRIR_STAR_FLARE` | render (fxdata d6121033) | summon data entry 37: emitter, a cyan-white star flare particle |
| fx_0d2 | 0xA6 | `FX_FENRIR_DUST_CLOUDS` | render (fxdata d6121033) | summon data entry 38: emitter, orbiting dust clouds and gold debris specks |
| fx_0d2 | 0xA7 | `FX_FENRIR_SUMMON_SLICE_PATH_39` | usage | summon data entry 39: rotation / position path (fx_0d2_slicePath) |
| fx_0d3 | 0x1 | `FX_PHOENIX_RED_RINGS` | render | emitter: grey and white-red gouraud rings |
| fx_0d3 | 0x2 | `FX_PHOENIX_FIRE_TRAILS` | render | emitter: fire-textured trails and a swirling ember ring |
| fx_0d3 | 0x3 | `FX_PHOENIX_GLOW_RINGS` | render | emitter: pale yellow / grey gouraud rings (+ textured rings, texture not in the file) |
| fx_0d3 | 0x4 | `FX_PHOENIX_PEACH_GLOW` | render (fxdata d6121033) | emitter: two fans, a narrow peach half-disc glow |
| fx_0d3 | 0x5 | `FX_PHOENIX_RAINBOW_FAN` | runtime | emitter: fan of a rainbow gradient strip, the prismatic light rays behind Phoenix |
| fx_0d3 | 0x6 | `FX_PHOENIX_FEATHERS` | render (fxdata d6121033) | emitter: two fans, grey and tan feathers with light streaks |
| fx_0d3 | 0x7 | `FX_PHOENIX_LENS_FLARE` | runtime | sprite line: lens-flare set (rainbow halo ring, yellow-green disc, white glow, red ring, blue disc) across Phoenix |
| fx_0d3 | 0xB | `FX_PHOENIX_FIRE_TRAILS_2` | render | emitter: orbiting fire-textured trails |
| fx_0d3 | 0xC | `FX_PHOENIX_FIRE_DOME` | render (fxdata d6121033) | 'so' model: dome with a fire texture |
| fx_0d3 | 0xD | `FX_PHOENIX_FIRE_DISC` | render (fxdata d6121033) | 'so' model: flat disc with a fire texture |
| fx_0d3 | 0xE | `FX_PHOENIX_SMOKE_ARC` | render (fxdata d6121033) | 'so' model: thin arc with a grey smoke texture |
| fx_0d3 | 0xF | `FX_PHOENIX_EMBERS` | render | emitter: glowing ember sprites |
| fx_0d3 | 0x10 | `FX_PHOENIX_FIRE_FEATHERS` | render | emitter: orbiting fire trails and flame-feather quads |
| fx_0d3 | 0x11 | `FX_PHOENIX_PINK_RINGS` | render | emitter: white / pink / copper gouraud rings |
| fx_0d3 | 0x12 | `FX_PHOENIX_GOLD_FEATHERS` | render | emitter: golden feather quads |
| fx_0d3 | 0x13 | `FX_PHOENIX_FEATHERS_2` | render (fxdata d6121033) | emitter: two fans, grey and tan feathers with light streaks |
| fx_0d3 | 0x14 | `FX_PHOENIX_RAINBOW_FAN_2` | runtime | emitter: the same rainbow gradient fan, later rays |
| fx_0d3 | 0x15 | `FX_PHOENIX_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_0d3 | 0x16 | `FX_PHOENIX_FLAME_SWIRL` | render (fxdata d6121033) | 'so' model: cone of swirling brown-orange flame tongues (fx_0d3_sharedModel) |
| fx_0d3 | 0x17 | `FX_PHOENIX_FLAME_FEATHERS` | render | emitter: flame-feather quads |
| fx_0d3 | 0x18 | `FX_PHOENIX_FLAME_FEATHERS_2` | render | emitter: flame-feather quads (cylinder) |
| fx_0d3 | 0x19 | `FX_PHOENIX_GOLD_FEATHERS_2` | render | emitter: golden feather sprites |
| fx_0d3 | 0x1A | `FX_PHOENIX_EMBERS_2` | render | emitter: glowing ember sprites |
| fx_0d3 | 0x1B | `FX_PHOENIX_COPPER_RINGS` | render | emitter: orbiting copper / brown gouraud rings |
| fx_0d3 | 0x1C | `FX_PHOENIX_EMBERS_3` | render | emitter: glowing ember sprites and red sparks |
| fx_0d3 | 0x1D | `FX_PHOENIX_FLAME_FEATHERS_3` | render | emitter: flame-feather quads |
| fx_0d3 | 0x26 | `FX_PHOENIX_FLAME_SWIRL_2` | render (fxdata d6121033) | 'so' model: cone of swirling brown-orange flame tongues (other texture page) |
| fx_0d4 | 0x1 | `FX_SWDMAG_FIRE_FIREBALL` | render | emitter: fireball sprite with ember sparks, at the target body (byte-identical to fx_000 entry 0x5) |
| fx_0d4 | 0x2 | `FX_SWDMAG_FIRE_FLAME_BURST` | render | emitter: flame explosion (burst, smoke, embers) rising around the target (byte-identical to fx_000 entry 0x6) |
| fx_0d4 | 0x9 | `FX_SWDMAG_FIRE_HEAT_RING` | render | emitter: white-red gouraud rings at the target's feet (byte-identical to fx_000 entry 0x7) |
| fx_0d4 | 0xB | `FX_SWDMAG_FIRE_SWORD_GLINT` | render | rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat |
| fx_0d4 | 0xC | `FX_SWDMAG_FIRE_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (byte-identical to the SWORD_FLASH of every Sword Magic module) |
| fx_0d5 | 0x1 | `FX_SWDMAG_FIRA_FLAME_BURST` | render | emitter: flame explosion with ember sparks, at the target body (byte-identical to fx_002 entry 0x4) |
| fx_0d5 | 0x2 | `FX_SWDMAG_FIRA_SMOKE` | render | emitter: grey smoke puffs (byte-identical to fx_002 entry 0x5) |
| fx_0d5 | 0x4 | `FX_SWDMAG_FIRA_FLAME_BURST_2` | render | emitter: second flame explosion (embers, red-tinted burst) (byte-identical to fx_002 entry 0x7) |
| fx_0d5 | 0x5 | `FX_SWDMAG_FIRA_SMOKE_2` | render | emitter: grey smoke puffs after the second burst (byte-identical to fx_002 entry 0x8) |
| fx_0d5 | 0x6 | `FX_SWDMAG_FIRA_FLAME_SWIRL` | render | emitter: flame-strip trails orbiting the target (byte-identical to fx_002 entry 0x9) |
| fx_0d5 | 0xD | `FX_SWDMAG_FIRA_GROUND_RING` | render | rings: red gouraud ring drawn flat on the ground (byte-identical to fx_002 entry 0xd) |
| fx_0d5 | 0xE | `FX_SWDMAG_FIRA_SWORD_GLINT` | render | rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat |
| fx_0d5 | 0xF | `FX_SWDMAG_FIRA_SWORD_GLINT_2` | render | rings: white gouraud ring flash at the sword, rotated |
| fx_0d5 | 0x12 | `FX_SWDMAG_FIRA_HEAT_RING` | render | emitter: white-red gouraud rings at the target's feet (byte-identical to fx_002 entry 0xe) |
| fx_0d6 | 0x9 | `FX_SWDMAG_FIRAGA_FLAME_PILLAR` | render | model: open cylinder with a flame texture (the pillar) (byte-identical to fx_007 entry 0x8) |
| fx_0d6 | 0xB | `FX_SWDMAG_FIRAGA_FLAME_DISC` | render | model: flat octagon, flame texture; registered, never drawn (byte-identical to fx_007 entry 0x9) |
| fx_0d6 | 0xC | `FX_SWDMAG_FIRAGA_FLAME_BURST` | render | emitter: flame explosion (byte-identical to fx_007 entry 0xa) |
| fx_0d6 | 0xD | `FX_SWDMAG_FIRAGA_GLOW` | render | rings: white-red gouraud glow disc at the target (byte-identical to fx_007 entry 0xb) |
| fx_0d6 | 0xE | `FX_SWDMAG_FIRAGA_EMBER_TRAIL` | render (fxdata d6121033) | emitter: orbiting red glow trail with red ember dots at the start (byte-identical to fx_007 entry 0xc) |
| fx_0d6 | 0xF | `FX_SWDMAG_FIRAGA_COLUMN_SPARK` | render | rings: small red-white glow drawn at random fire-column segments (byte-identical to fx_007 entry 0xd) |
| fx_0d6 | 0x10 | `FX_SWDMAG_FIRAGA_FLAME_BURST_2` | render | emitter: flame explosion at the pillar's start (byte-identical to fx_007 entry 0xe) |
| fx_0d6 | 0x11 | `FX_SWDMAG_FIRAGA_PILLAR_RING` | render | rings: textured flame ring rising with the pillar (byte-identical to fx_007 entry 0xf) |
| fx_0d6 | 0x12 | `FX_SWDMAG_FIRAGA_EMBERS` | render | emitter: dark red ember sprites (byte-identical to fx_007 entry 0x12) |
| fx_0d6 | 0x13 | `FX_SWDMAG_FIRAGA_GOLD_SPARKS_2` | render | emitter: golden spark sprites + spark sub-particles (byte-identical to fx_007 entry 0x11) |
| fx_0d6 | 0x14 | `FX_SWDMAG_FIRAGA_GOLD_SPARKS` | render | emitter: golden spark sprites (byte-identical to fx_007 entry 0x10) |
| fx_0d6 | 0x15 | `FX_SWDMAG_FIRAGA_SMOKE_BURST` | render | emitter: dark teal-tinted burst (smoke after the blast) (byte-identical to fx_007 entry 0x13) |
| fx_0d6 | 0x16 | `FX_SWDMAG_FIRAGA_RED_HAZE` | runtime (medium) | emitter: soft dark-red additive blob, Firaga's red heat haze (byte-identical to fx_007 entry 0x15) |
| fx_0d6 | 0x17 | `FX_SWDMAG_FIRAGA_EMBER_CONE` | render (fxdata d6121033) | emitter: cone of red and white ember dots (byte-identical to fx_007 entry 0x14) |
| fx_0d6 | 0x18 | `FX_SWDMAG_FIRAGA_SWORD_GLINT` | render | rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat |
| fx_0d6 | 0x19 | `FX_SWDMAG_FIRAGA_SWORD_FLASH` | render | rings: white 8-segment rings flashed rotating at the sword (byte-identical to the SWORD_FLASH of every Sword Magic module) |
| fx_0d6 | 0x1B | `FX_SWDMAG_FIRAGA_FLAME_RIBBON_1` | render (fxdata d6121033) | 'so' model: twisted flame ribbon with a fire texture (byte-identical to fx_007 entry 0x17) |
| fx_0d6 | 0x1C | `FX_SWDMAG_FIRAGA_FLAME_RIBBON_0` | render (fxdata d6121033) | 'so' model: twisted flame ribbon with a fire texture, drawn coiled 18 times (byte-identical to fx_007 entry 0x16) |
| fx_0d6 | 0x1D | `FX_SWDMAG_FIRAGA_FLAME_RIBBON_2` | render (fxdata d6121033) | 'so' model: twisted flame ribbon with a fire texture (byte-identical to fx_007 entry 0x18) |
| fx_0d6 | 0x1E | `FX_SWDMAG_FIRAGA_FLAME_RIBBON_3` | render (fxdata d6121033) | 'so' model: twisted flame ribbon with a fire texture (byte-identical to fx_007 entry 0x19) |
| fx_0d7 | 0x7 | `FX_SWDMAG_THUNDER_SMOKE` | render | emitter: yellow / blue smoke puffs after the strike (byte-identical to fx_003 entry 0xa) |
| fx_0d7 | 0x8 | `FX_SWDMAG_THUNDER_IMPACT_RING` | render | rings: cyan gouraud rings at the strike (byte-identical to fx_003 entry 0xb) |
| fx_0d7 | 0x9 | `FX_SWDMAG_THUNDER_IMPACT_FLASH` | render | emitter: four-point star flash, white rings, sparks (byte-identical to fx_003 entry 0xc) |
| fx_0d7 | 0xF | `FX_SWDMAG_THUNDER_SWORD_GLINT` | render | rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat |
| fx_0d7 | 0x10 | `FX_SWDMAG_THUNDER_SWORD_GLINT_2` | render | rings: white gouraud ring flash at the sword, rotated |
| fx_0d8 | 0x2 | `FX_SWDMAG_THUNDARA_IMPACT_FLASH` | render | emitter: four-point star flash, white rings, sparks (byte-identical to fx_005 entry 0x17) |
| fx_0d8 | 0x14 | `FX_SWDMAG_THUNDARA_ORB_EMITTER` | render | emitter: helix whose callback element draws the orb (0x19-0x1D here) (byte-identical to fx_005 entry 0x1a) |
| fx_0d8 | 0x17 | `FX_SWDMAG_THUNDARA_IMPACT_RING` | render | emitter: white-cyan gouraud rings (byte-identical to fx_005 entry 0x19) |
| fx_0d8 | 0x18 | `FX_SWDMAG_THUNDARA_SPARK_DOTS` | render (fxdata d6121033) | emitter: white dot sparks (byte-identical to fx_005 entry 0x7) |
| fx_0d8 | 0x19 | `FX_SWDMAG_THUNDARA_ORB_FADE` | render | particles: blue energy sphere fading to orange (FX_THUNDARA_ORB_FADE's draw slot in fx_005) |
| fx_0d8 | 0x1A | `FX_SWDMAG_THUNDARA_ORB` | render | particles: bright blue energy sphere (byte-identical to fx_005 entry 0x1d) |
| fx_0d8 | 0x1B | `FX_SWDMAG_THUNDARA_STAR_GLINTS` | runtime | sprites: Thundara's star-glint sprites (run-time page 0x1C0,0x100) (byte-identical to fx_005 entry 0x1e) |
| fx_0d8 | 0x1C | `FX_SWDMAG_THUNDARA_ORB_DOTS` | render (fxdata d6121033) | sprites: white dot sparks around the orb (byte-identical to fx_005 entry 0x1f) |
| fx_0d8 | 0x1D | `FX_SWDMAG_THUNDARA_LENS_FLARES` | render | particles: orange lens-flare dots around the orb (byte-identical to fx_005 entry 0x16) |
| fx_0d8 | 0x1E | `FX_SWDMAG_THUNDARA_SWORD_GLINT` | render | rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat |
| fx_0d8 | 0x1F | `FX_SWDMAG_THUNDARA_SWORD_GLINT_2` | render | rings: white gouraud ring flash at the sword, rotated |

## Open questions (emulator)

All still-pending names of all slices, grouped by battle setup with what to capture, are in
`docs/wip/fxdata_runtime.md` (the emulator checklist); the list below is this slice's original notes.

20 names are still by kind (look pending) after the re-render with `tools/fxdata.py` at d6121033 (summon data, fans, 'so' models and dot sprites drawn). What is left is texture the file does not hold or the tool does not resolve (blank or placeholder pages filled at run time, tpages the C computes, texture lists the tool does not follow, garbled texels). Each wants one screenshot at the frame the C starts the entry, or a VRAM dump while it plays:

| module | index | name | why the look is pending |
|---|---|---|---|
| fx_0b1 | 0xa | `FX_CARBUNCLE_GREEN_MOTES` | emitter: faint pale round glow, the green motes drifting round the emerald crystal (runtime (medium)) |
| fx_0b1 | 0x16 | `FX_CARBUNCLE_ORANGE_MOTES` | emitter: small orange glow, the orange motes swirling after the burst (runtime) |
| fx_0b1 | 0x24 | `FX_CARBUNCLE_ORANGE_MOTES_2` | emitter: the same small orange glow motes (runtime) |
| fx_0b4 | 0x11 | `FX_AERO_GREEN_GLINTS` | emitter: spiralling sprites, green four-point star glints at run time (not ablated) (runtime (low, look pending)) |
| fx_0b8 | 0x2 | `FX_ATOMOS_BLUE_SPECKS` | emitter: sparse tiny blue specks flying round the gate as it opens (runtime (medium)) |
| fx_0b8 | 0x8 | `FX_ATOMOS_BLUE_SPECKS_2` | emitter: the same tiny blue specks, during the vortex (runtime (medium)) |
| fx_0b8 | 0xb | `FX_ATOMOS_FAINT_GLOW` | emitter: spiralling glow particles; the render is almost black (a faint white glow); runtime: with the run-time page its particle reads a grey zigzag band, nothing like the white mist on screen |
| fx_0b8 | 0x22 | `FX_ATOMOS_BLUE_SPECKS_3` | emitter: the same blue specks round the black hole (runtime (medium)) |
| fx_0ba | 0x2c | `FX_RAMUH_BOLT_SPARKS` | emitter: one particle; not fetched in a full Ramuh (a branch that cast did not take); fx_19f's byte-identical entry is a blue lightning-bolt glyph sprite at run time, the small bolt sparks around the pillar (runtime (medium)) |
| fx_0cb | 0x11 | `FX_LIGHT_FLARE_GLOW_RING` | rings: textured ring drawn twice, tilted: the yellow-green glow ring round the target (runtime) |
| fx_0cc | 0x9 | `FX_METEORITE_EMBERS` | emitter: sprites following the falling rock, small orange specks at run time (runtime (low, look pending)) |
| fx_0cd | 0x8 | `FX_AERA_GREEN_WIND_BLADES` | emitter: spiralling sprites + 3D trail, the translucent green wind blades sweeping round the target (runtime) |
| fx_0cd | 0xc | `FX_AERA_SPRITE_BURST` | emitter: sprites; the render still reads garbled yellow texels (texture not resolved); runtime: its run-time sprite is a tiny green glint; ablating it changes < 210 px |
| fx_0ce | 0x1 | `FX_SCREECH_TRAIL` | emitter: 3D trail; the render still reads garbled pink stripes (texture not resolved); runtime: ablating it changes only scattered pink squiggle pixels (max 1234 px) |
| fx_0ce | 0xb | `FX_SCREECH_SOUND_RINGS` | emitter: textured rings, the pale expanding sound-wave rings over the target (runtime (medium)) |
| fx_0d3 | 0x5 | `FX_PHOENIX_RAINBOW_FAN` | emitter: fan of a rainbow gradient strip, the prismatic light rays behind Phoenix (runtime) |
| fx_0d3 | 0x7 | `FX_PHOENIX_LENS_FLARE` | sprite line: lens-flare set (rainbow halo ring, yellow-green disc, white glow, red ring, blue disc) across Phoenix (runtime) |
| fx_0d3 | 0x14 | `FX_PHOENIX_RAINBOW_FAN_2` | emitter: the same rainbow gradient fan, later rays (runtime) |
| fx_0d6 | 0x16 | `FX_SWDMAG_FIRAGA_RED_HAZE` | emitter: soft dark-red additive blob, Firaga's red heat haze (byte-identical to fx_007 entry 0x15) (runtime (medium)) |
| fx_0d8 | 0x1b | `FX_SWDMAG_THUNDARA_STAR_GLINTS` | sprites: Thundara's star-glint sprites (run-time page 0x1C0,0x100) (byte-identical to fx_005 entry 0x1e) (runtime) |
