# Battle effect data names, slice 6 (fx_188 .. fx_1be)

Same scheme and tool as the pilot in `docs/fxdata.md`: every `btlfx_getFileData` literal of the 48 modules
fx_188 .. fx_1be named in the module's header (332 module/index defines: 258 by the render, 32 data tables by
their use, 42 by kind with the look pending). Evidence column: `render` = named by the `tools/fxdata.py
--render` contact sheet (tool at 8b8d391c: fans, 'so' models, dot sprites and summon data drawn), `usage` = a data
table named by what the C passes it to (backdrop layers, key / path tables, buffers), `kind/usage (look pending)` =
appearance not settled by the render (open questions below).

Notes:
- Prefixes are the in-game names from `docs/effect-modules.md`. Enemy-only modules use the enemy skill:
  `FX_TRANSFORM_` (Kuja), `FX_SILENT_VOICE_`, `FX_LIGHT_` (Jabberwock), `FX_VIRAL_SMOKE_`, `FX_COUNTER_HORN_`,
  `FX_SHOCKWAVE_`, `FX_TSUNAMI_` (fx_198, Black Waltz 1 / Sealion), `FX_FLAME_SLASH_`, `FX_MOONLIGHT_SLASH_`,
  `FX_TROUBLE_MUCUS_`, `FX_WATER_GUN_`, `FX_MIND_BLAST_`, `FX_VIRUS_POWDER_`, `FX_WATERGA_`, `FX_VIRUS_CRUNCH_`,
  `FX_6_LEGS_`, `FX_GOLEM_DEATH_`, `FX_TIDAL_WAVE_`, `FX_FLARE_STAR_`, `FX_APPEARANCE_`. The bosses' death
  sequences are all called "Death" in the enemy data and use `FX_DEATH_` like slice 5's fx_12d (fx_192, fx_1af
  Valia Pira, fx_1b0 Ozma, fx_1b6 Silver Dragon, fx_1b7 Nova Dragon, fx_1b8 Gizamaluke); Deathguise's "Death0"
  and Necron's "Death1" keep the digit (`FX_DEATH0_`, `FX_DEATH1_`). Player abilities: `FX_LV3_DEF_LESS_`,
  `FX_CLIMHAZZARD_` (fx_18d; Seiken fx_1a1 and Beatrix fx_1bb play the same entry byte for byte), `FX_CHARGE_`,
  `FX_WHATS_THAT_`, `FX_LANCER_`, `FX_REVIVE_` (fx_1a5, Elan).
- Byte-identical entries (every entry of every module hashed against all 380 files): the alt / Seiken sword-art
  modules fx_199, fx_19a, fx_19b, fx_19c, fx_19e repeat fx_0bc, fx_0bd, fx_0be, fx_0bf, fx_0cf entry for entry and
  keep their names; fx_1a3 (Trouble Mucus) repeats fx_106 (Mucus) under its own prefix; fx_1a5 repeats fx_089
  (Revive); fx_18a's cone ring is fx_101's Trance aura cylinder; fx_1b8 / fx_1b9 (Gizamaluke death / appearance)
  share three emitters. Names were carried over only through these hashes, never by call site.
- fx_1b3 (Necron, Death1) reads entry 0x93, i.e. summon data entry 0x13: it only takes the address as the base of
  its trail and split-piece work buffers (0xE00 + 0xF30 + 0x2448 bytes from there), so it is named
  `FX_DEATH1_SCRATCH_BUF`.
- The short summon modules (fx_195 Bahamut, fx_196 Leviathan, fx_197 Shiva, fx_19f Ramuh, fx_1a8 Odin,
  fx_1bd Ifrit, fx_1be Atomos) take the summon's prefix. fx_19f (Ramuh) and fx_1bd (Ifrit) are almost all
  byte-identical to fx_0ba / fx_114 and keep those names and their look-pending status; fx_197 (Shiva) shares
  seven emitters with fx_026, fx_196 two with fx_0b3, fx_1be two with fx_0b8; fx_195 (Bahamut) and fx_1a8 (Odin)
  share nothing with the long files and are named by their own renders. None of them reads summon data (0x80+);
  fx_197 registers two ice-shard 'so' models from the battle load buffer (0xD8, 0xD9; type 8 chunk). Ifrit's two
  camera channels follow the camera-key convention (`FX_IFRIT_SHOT0_CAMERA_KEYS_<slot + 1>`, dst word 1 / 2 =
  env->camera[0] / [1] via BTLFX_CAM_SLOT).

## Open questions (emulator)

All still-pending names of all slices, grouped by battle setup with what to capture, are in
`docs/wip/fxdata_runtime.md` (the emulator checklist); the list below is this slice's original notes.

Each wants one screenshot at the frame the C starts the entry, or a VRAM dump while it plays.

| module | index | name | what is known |
|---|---|---|---|
| fx_188 | 0x3 | `FX_LV3_DEF_LESS_PARTICLE` | particles: one frame, renders black (drawn per pool item); runtime: still black with the run-time page |
| fx_188 | 0x14 | `FX_LV3_DEF_LESS_RING` | rings: one textured ring (texture not resolved); runtime: noise with the run-time page |
| fx_18a | 0xA | `FX_TRANSFORM_RED_SWEEP` | emitter: fan elements, the red light streaks sweeping in from the left (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently) (runtime (medium)) |
| fx_18a | 0xC | `FX_TRANSFORM_RED_SPECKS` | emitter: spiral of sprites + sub-sprites, small dark-red glowing specks at run time (ablation changes part of the white-out) (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently) (runtime (low, look pending)) |
| fx_18c | 0x1 | `FX_LIGHT_VIOLET_STREAKS` | emitter: one particle element, a thin violet streak sprite at run time: the violet light streaks under the target (runtime (medium)) |
| fx_195 | 0xD | `FX_BAHAMUT_ORANGE_STREAKS` | emitter: two fan elements, a thin tan streak in the render |
| fx_19f | 0x5 | `FX_RAMUH_STAFF` | 'so' model (byte-identical to fx_0ba entry 0x4) |
| fx_19f | 0x6 | `FX_RAMUH_LIGHTNING_RIBBON` | trail: ribbon drawn between key points, crackling blue-white plasma strip at run time: the blue-white electric ribbons arcing across (byte-identical to fx_0ba entry 0x5) (runtime) |
| fx_19f | 0xF | `FX_RAMUH_LIGHTNING_RIBBON_2` | trail: second ribbon drawn between key points, the same blue-white plasma strip at run time (byte-identical to fx_0ba entry 0xD) (runtime (medium)) |
| fx_19f | 0x16 | `FX_RAMUH_BOLT_SPARK_BURST` | emitter: white gouraud rings + small blue lightning-bolt glyph sprites in a glow, the cyan screen flash and bolt sparks round the strike (byte-identical to fx_0ba entry 0x14) (runtime (medium)) |
| fx_19f | 0x18 | `FX_RAMUH_LIGHTNING_BOLT` | trail: 8-frame trail, a blue forked lightning strip at run time (byte-identical to fx_0ba entry 0x16) (runtime (medium)) |
| fx_19f | 0x1A | `FX_RAMUH_THIN_ROD` | 'so' model (byte-identical to fx_0ba entry 0x18) |
| fx_19f | 0x20 | `FX_RAMUH_ORANGE_LIGHTNING` | trail: 8-frame offset trail (texture garbled in the render, orange; byte-identical to fx_0ba entry 0x1E) |
| fx_19f | 0x21 | `FX_RAMUH_ELECTRIC_BOWL` | 'so' model (byte-identical to fx_0ba entry 0x1F) |
| fx_19f | 0x22 | `FX_RAMUH_BLUE_AURA_COLUMN` | 'so' model: dome-capped column; its page holds a blue-white marbled texture at run time (no 'dummy' placeholder): the huge glassy blue energy shroud round Ramuh's staff (runtime) |
| fx_19f | 0x27 | `FX_RAMUH_GREEN_DOME` | 'so' model (byte-identical to fx_0ba entry 0x26) |
| fx_19f | 0x2A | `FX_RAMUH_BOLT_SPARKS` | emitter: one particle, a blue lightning-bolt glyph sprite at run time: the small bolt sparks around the pillar (byte-identical to fx_0ba entry 0x2C) (runtime (medium)) |
| fx_1a2 | 0x6 | `FX_MOONLIGHT_SLASH_SPIKED_SHAPE` | model: 20-poly spiked shape (Hi_RegisterTexListModel, texture not drawn); runtime: untextured spiky model, fetched early; nothing visible in a forced cast with Zidane as the caster (a real enemy cast may differ) |
| fx_1a2 | 0x8 | `FX_MOONLIGHT_SLASH_BLUE_GLOWS` | emitter: three sprite elements, 24 frames, soft blue glows at run time (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently) (runtime (low, look pending)) |
| fx_1a2 | 0x17 | `FX_MOONLIGHT_SLASH_COLOR_GLOWS` | emitter: three sprite elements, 24 frames, blue, violet and green glows at run time (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently) (runtime (low, look pending)) |
| fx_1a6 | 0x3 | `FX_WATER_GUN_SPRAY` | emitter: one particle element, the white water splash spray over the target (runtime) |
| fx_1a8 | 0x5 | `FX_ODIN_GOLD_RAYS` | emitter: two fan elements, a thin pale-gold streak at run time, matching the gold rays of the slash explosion (runtime (low, look pending)) |
| fx_1a8 | 0xC | `FX_ODIN_BLACK_DEBRIS` | emitter: spiral of two speck-sprite elements, dark grey specks at run time: the black debris flying out of the explosion (runtime) |
| fx_1a8 | 0x12 | `FX_ODIN_RED_SPARKS` | emitter: speck sprites, tiny red sparks at run time (not separable on screen) (runtime (low, look pending)) |
| fx_1a8 | 0x17 | `FX_ODIN_CALLBACK_EMITTER` | emitter: two callback elements (drawn by the module); runtime: started with the slash explosion; ablating it only shifts the random pink smoke puffs |
| fx_1ab | 0x4 | `FX_WATERGA_WHITE_FOAM` | emitter: cone of textured rings, the white foam inside the blue water sphere (runtime (medium)) |
| fx_1ad | 0x2 | `FX_6_LEGS_SLIME_SHELL` | 'so' model: rounded pod with an opening; page 0x117 holds a green slimy texture at run time: the green slime shell wrapping the target (runtime (medium)) |
| fx_1b2 | 0x2 | `FX_TIDAL_WAVE_WATER_CURL` | model: curved sheet like a breaking wave; page 0x117 holds blue-white water at run time: the white curling wave crest (runtime) |
| fx_1b2 | 0xC | `FX_TIDAL_WAVE_SURGE` | model: flat soft-edged disc, the big grey water surge sweeping over the party (runtime (medium)) |
| fx_1b3 | 0x6 | `FX_DEATH1_GOLD_RAYS` | emitter: two fan elements, the pale-gold light rays fanning out (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently) (runtime) |
| fx_1b3 | 0x39 | `FX_DEATH1_SCREEN_TRAIL` | emitter: orbiting screen trails (strip texture not resolved) + speck sprites; runtime: not fetched within 1000 frames of a forced cast (Zidane as the caster) |
| fx_1b3 | 0x3A | `FX_DEATH1_SCREEN_TRAIL_2` | emitter: orbiting screen trails (strip texture not resolved) + speck sprites; runtime: not fetched within 1000 frames of a forced cast (Zidane as the caster) |
| fx_1b4 | 0xB | `FX_FLARE_STAR_RED_RAYS` | emitter: two fan elements, the red light rays radiating from the target over the violet field (runtime) |
| fx_1b6 | 0x2 | `FX_DEATH_SPIRAL_TRAIL` | emitter: spiralling 3D trail, strip mostly black with a faint white streak; runtime: started as the screen goes white; its run-time strip is a white streak |
| fx_1b8 | 0x1 | `FX_DEATH_TEAL_GLOWS` | emitter: anim quads + sprites, soft teal-white glows drifting on screen (byte-identical to fx_1b9 entry 0x2) (runtime (medium)) |
| fx_1b9 | 0x2 | `FX_APPEARANCE_TEAL_GLOWS` | emitter: anim quads + sprites, soft teal-white glows (byte-identical to fx_1b8 entry 0x1) (runtime (medium)) |
| fx_1bd | 0xD | `FX_IFRIT_WHITE_HAZE` | model: grid with grey-white wavy streaks, the grey streaky haze over the floor under the target, turning orange as the lava cracks appear (byte-identical to fx_114 entry 0x2A) (runtime) |
| fx_1bd | 0x12 | `FX_IFRIT_RED_GLOW` | emitter: one orbiting fan element (not rendered; byte-identical to fx_114 entry 0x26) |
| fx_1bd | 0x13 | `FX_IFRIT_HEAT_DOME` | model: hemisphere bowl, the bright yellow-white translucent glare dome over the fire dome (byte-identical to fx_114 entry 0x1E) (runtime) |
| fx_1bd | 0x2C | `FX_IFRIT_SPOKED_RING` | model: flat ring with spokes (registered through the texture list, texture not drawn; byte-identical to fx_114 entry 0x12); runtime: ablating it alone changes only a few hundred scattered px |
| fx_1be | 0x2 | `FX_ATOMOS_ORANGE_STREAKS` | emitter: fan elements (not rendered; byte-identical to fx_0b8 entry 0x7) |
| fx_1be | 0xC | `FX_ATOMOS_RED_CLOUD_SPHERE` | 'so' model: segmented sphere; page 0x117 holds a dark red cloudy texture at run time: the dark red cloud sphere inside the violet gate (runtime) |

## Table

| module | index | name | evidence | what it is |
|---|---|---|---|---|
| fx_188 | 0x3 | `FX_LV3_DEF_LESS_PARTICLE` | kind/usage (look pending) | particles: one frame, renders black (drawn per pool item); runtime: still black with the run-time page |
| fx_188 | 0x4 | `FX_LV3_DEF_LESS_VIOLET_ORBS` | render | emitter: violet glowing orb sprites, 12 frames (per pool item) |
| fx_188 | 0xE | `FX_LV3_DEF_LESS_VIOLET_ORB` | render | particles: violet glowing orb (per pool item) |
| fx_188 | 0xF | `FX_LV3_DEF_LESS_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_188 | 0x10 | `FX_LV3_DEF_LESS_SMOKE_RINGS` | render | emitter: rings textured with orange smoke |
| fx_188 | 0x11 | `FX_LV3_DEF_LESS_SMOKE_RINGS_2` | render | emitter: rings textured with orange smoke |
| fx_188 | 0x12 | `FX_LV3_DEF_LESS_BLUE_RINGS` | render | emitter: white-blue and white-green gouraud rings |
| fx_188 | 0x13 | `FX_LV3_DEF_LESS_ORANGE_TRAILS` | render | emitter: orbiting 3D trails, orange glowing streak |
| fx_188 | 0x14 | `FX_LV3_DEF_LESS_RING` | kind/usage (look pending) | rings: one textured ring (texture not resolved); runtime: noise with the run-time page |
| fx_188 | 0x15 | `FX_LV3_DEF_LESS_CRYSTAL_SHARDS` | render | emitter: orbiting pink / blue / orange crystal shards, 16 frames |
| fx_18a | 0x1 | `FX_TRANSFORM_AURA_CYLINDER` | render | model: open cone ring, magenta-violet cloud texture (byte-identical to fx_101 FX_TRANCE_AURA_CYLINDER) |
| fx_18a | 0x3 | `FX_TRANSFORM_VIOLET_RINGS` | render | rings: white-lilac-violet gouraud rings |
| fx_18a | 0x4 | `FX_TRANSFORM_VIOLET_RINGS_2` | render | rings: white-violet gouraud rings |
| fx_18a | 0x6 | `FX_TRANSFORM_VIOLET_TRAIL` | render | trail: violet speckled strip, 12 frames (per pool item) |
| fx_18a | 0x9 | `FX_TRANSFORM_BLUE_RING` | render | rings: navy-blue gouraud ring |
| fx_18a | 0xA | `FX_TRANSFORM_RED_SWEEP` | runtime (medium) | emitter: fan elements, the red light streaks sweeping in from the left (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently) |
| fx_18a | 0xC | `FX_TRANSFORM_RED_SPECKS` | runtime (low, look pending) | emitter: spiral of sprites + sub-sprites, small dark-red glowing specks at run time (ablation changes part of the white-out) (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently) |
| fx_18a | 0xE | `FX_TRANSFORM_PINK_STREAKS` | render | emitter: orbiting fans, pink-white light streaks |
| fx_18a | 0xF | `FX_TRANSFORM_RED_STREAKS` | render | emitter: orbiting fans, red light streaks with pink orbs |
| fx_18a | 0x12 | `FX_TRANSFORM_VIOLET_RING` | render | rings: violet gouraud ring |
| fx_18b | 0x1 | `FX_SILENT_VOICE_SWIRLS` | render | emitter: violet and blue spiral swirl particles + white-blue-violet gouraud rings |
| fx_18c | 0x1 | `FX_LIGHT_VIOLET_STREAKS` | runtime (medium) | emitter: one particle element, a thin violet streak sprite at run time: the violet light streaks under the target |
| fx_18c | 0x2 | `FX_LIGHT_GREEN_HALO` | render | emitter: yellow-green crescent halo particles + violet-white gouraud rings |
| fx_18d | 0x1 | `FX_CLIMHAZZARD_FLAME_RINGS` | render | emitter: rings textured with a yellow-red flame wall + white / pale-yellow gouraud rings, 16 frames |
| fx_18e | 0x1 | `FX_CHARGE_BLUE_GLOW` | render | emitter: orbiting blue / cyan glow fans + white star glints + white rings |
| fx_18f | 0x1 | `FX_WHATS_THAT_LIGHT_STREAKS` | render | emitter: navy / pale-blue / grey gouraud rings + three white light-streak fans |
| fx_190 | 0x1 | `FX_VIRAL_SMOKE_BROWN_PUFFS` | render | emitter: orange-brown smoke puffs, 24 frames |
| fx_190 | 0x2 | `FX_VIRAL_SMOKE_GREEN_PUFFS` | render | emitter: green smoke puffs, 24 frames |
| fx_190 | 0x4 | `FX_VIRAL_SMOKE_PUFF_HELIX` | render | emitter: helix of green and orange-brown smoke puffs |
| fx_191 | 0x2 | `FX_LANCER_GOLD_TRAIL` | render | emitter: screen trail, orange-gold streak strip (byte-identical to entry 0x9) |
| fx_191 | 0x4 | `FX_LANCER_PATH` | usage | table: position path (fxm_posPathInit) |
| fx_191 | 0x5 | `FX_LANCER_PATH_2` | usage | table: position path (fxm_posPathInit) |
| fx_191 | 0x9 | `FX_LANCER_GOLD_TRAIL_2` | render | emitter: screen trail, orange-gold streak strip (byte-identical to entry 0x2) |
| fx_191 | 0xF | `FX_LANCER_RED_SPARKS` | render | emitter: white / pink gouraud rings + red spark dots |
| fx_191 | 0x10 | `FX_LANCER_ORANGE_RINGS` | render | emitter: orange gouraud rings, 18 frames |
| fx_191 | 0x11 | `FX_LANCER_GREEN_RINGS` | render | emitter: mint-green gouraud rings, 18 frames |
| fx_191 | 0x12 | `FX_LANCER_CYAN_SPARKS` | render | emitter: white / pale-green gouraud rings + cyan spark dots |
| fx_192 | 0x1 | `FX_DEATH_GREEN_SMOKE` | render | emitter: grey-green smoke puffs in a spiral, 24 frames |
| fx_193 | 0x1 | `FX_COUNTER_HORN_RED_TRAIL` | render | emitter: 3D trail, red-white flame streak (byte-identical to entry 0xF) |
| fx_193 | 0x2 | `FX_COUNTER_HORN_FLAME_RINGS` | render | emitter: rings textured with red-pink flame streaks |
| fx_193 | 0x3 | `FX_COUNTER_HORN_FLAME_RINGS_2` | render | emitter: rings textured with red-pink flame streaks |
| fx_193 | 0x4 | `FX_COUNTER_HORN_DUST_CLOUD` | render | emitter: peach dust cloud bursting and fading, 12 frames |
| fx_193 | 0xE | `FX_COUNTER_HORN_DARK_RING` | render | emitter: dark red textured ring |
| fx_193 | 0xF | `FX_COUNTER_HORN_RED_TRAIL_2` | render | emitter: 3D trail, red-white flame streak (byte-identical to entry 0x1) |
| fx_194 | 0x1 | `FX_SHOCKWAVE_DUST_WAVE` | render | emitter: rings textured with grey dust streaks + white glow flash; sub: white rock-debris ring |
| fx_194 | 0x2 | `FX_SHOCKWAVE_VIOLET_STREAKS` | render | emitter: violet vertical streak sprite (three more sprites render black) |
| fx_194 | 0x3 | `FX_SHOCKWAVE_GREY_STREAK_RINGS` | render | emitter: rings textured with grey vertical streaks |
| fx_194 | 0x6 | `FX_SHOCKWAVE_WHITE_GLOW` | render | emitter: white glow particles fading in, 21 frames |
| fx_195 | 0x2 | `FX_BAHAMUT_PINK_FLAT_RING` | render | rings: white / salmon gouraud rings, 14 frames |
| fx_195 | 0x7 | `FX_BAHAMUT_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_195 | 0x8 | `FX_BAHAMUT_VIOLET_RINGS` | render | rings: white-cyan-violet-indigo gouraud rings, 24 frames |
| fx_195 | 0x9 | `FX_BAHAMUT_HALO_RINGS` | render | rings: white-crimson-blue gouraud rings, 16 frames |
| fx_195 | 0xA | `FX_BAHAMUT_BLUE_RINGS` | render | rings: navy-to-blue gouraud rings, 12 frames |
| fx_195 | 0xB | `FX_BAHAMUT_EMBERS` | render | emitter: spiral of tiny orange sprites |
| fx_195 | 0xC | `FX_BAHAMUT_BLUE_SPARKS` | render | emitter: tiny blue-violet spark sprites falling |
| fx_195 | 0xD | `FX_BAHAMUT_ORANGE_STREAKS` | render (byte-identical twin, consistency pass) | emitter: two fans, thin orange-white rising streaks (byte-identical to fx_0e3_1 entry 0xE, same render) |
| fx_195 | 0xE | `FX_BAHAMUT_BLUE_SPARKS_2` | render | emitter: tiny blue-violet spark sprites falling |
| fx_195 | 0xF | `FX_BAHAMUT_RED_GLOWS` | render | emitter: spiral of red glowing orbs |
| fx_195 | 0x10 | `FX_BAHAMUT_EMBERS_2` | render | emitter: spiral of tiny orange sprites |
| fx_195 | 0x11 | `FX_BAHAMUT_FIRE_CYLINDER` | render | 'so' model: open cone cylinder, fire texture |
| fx_195 | 0x13 | `FX_BAHAMUT_FIRE_RING` | render | 'so' model: flared ring, fire texture |
| fx_195 | 0x14 | `FX_BAHAMUT_SMOKE_RING` | render | 'so' model: flared ring, grey-violet smoke texture |
| fx_195 | 0x15 | `FX_BAHAMUT_FLAME_STRIP` | render | 'so' model: gouraud plane, red-to-yellow glow bands |
| fx_195 | 0x16 | `FX_BAHAMUT_SMOKE_ARC` | render | 'so' model: curved half-tube, grey smoke texture |
| fx_195 | 0x17 | `FX_BAHAMUT_VIOLET_FAN` | render | emitter: fan with a violet lightning pattern, 13 frames |
| fx_195 | 0x18 | `FX_BAHAMUT_PURPLE_BOLT` | render | trail: violet energy strip (fxm_drawOffsetTrail0) |
| fx_195 | 0x19 | `FX_BAHAMUT_PURPLE_BOLT_2` | render | trail: violet energy strip (fxm_drawOffsetTrail1) |
| fx_195 | 0x1A | `FX_BAHAMUT_RED_STRIP` | render | 'so' model: gouraud plane, red beam line |
| fx_195 | 0x1C | `FX_BAHAMUT_BLUE_SLIVER` | render | 'so' model: curved gouraud blade, white-blue |
| fx_196 | 0x5 | `FX_LEVIATHAN_WATER_DROPS_2` | render | emitter: water droplet sprites (byte-identical to fx_0b3 entry 0x25) |
| fx_196 | 0x6 | `FX_LEVIATHAN_FOAM` | render | animquads: blue-white foam, drawn along the wave edge (byte-identical to fx_0b3 entry 0x24) |
| fx_196 | 0x7 | `FX_LEVIATHAN_WATER_BUBBLES` | render | emitter: orbiting blue water bubbles + water droplet sprites |
| fx_196 | 0x8 | `FX_LEVIATHAN_FOAM_2` | render | emitter: blue-white foam anim quads, 16 frames |
| fx_197 | 0x4 | `FX_SHIVA_STAR_GLINTS` | render | emitter: blue glows, white / green star glints, blue glow domes (byte-identical to fx_026 entry 0x7) |
| fx_197 | 0xB | `FX_SHIVA_SHATTER_RINGS` | render | emitter: blue gouraud rings + ice chunk sprites, sparks (pool item per piece; byte-identical to fx_026 entry 0x1A) |
| fx_197 | 0xC | `FX_SHIVA_ICE_DEBRIS_2` | render | emitter: small ice chunk sprites (byte-identical to fx_026 entry 0x21) |
| fx_197 | 0xD | `FX_SHIVA_BLUE_TRAIL` | render | emitter: spiralling trail, blue strip texture (byte-identical to fx_026 entry 0x1F) |
| fx_197 | 0xE | `FX_SHIVA_ICE_DEBRIS` | render | emitter: small ice chunk sprites (byte-identical to fx_026 entry 0x20) |
| fx_197 | 0xF | `FX_SHIVA_CYAN_RING` | render | emitter: cyan-white gouraud ring (byte-identical to fx_026 entry 0x22) |
| fx_197 | 0x1A | `FX_SHIVA_ICE_CHUNKS` | render | emitter: falling ice chunk sprites, faint sparkles (byte-identical to fx_026 entry 0x6) |
| fx_197 | 0xD8 | `FX_SHIVA_ICICLE` | render | 'so' model: long ice shard, pale cracked-ice texture (0xC0+ = battle load buffer, entry 0x18) |
| fx_197 | 0xD9 | `FX_SHIVA_ICICLE_2` | render | 'so' model: long ice shard, dark cracked-ice texture (0xC0+ = battle load buffer, entry 0x19) |
| fx_198 | 0x6 | `FX_TSUNAMI_WATER_SPLASH` | render | anim quads: blue-white water bubbles and foam, 30 frames |
| fx_198 | 0x15 | `FX_TSUNAMI_BOWL` | render | model: hemisphere bowl, plain white texture |
| fx_198 | 0x16 | `FX_TSUNAMI_BOWL_KEYS` | usage | table: per-frame rotation keys of the bowl model |
| fx_198 | 0x17 | `FX_TSUNAMI_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_198 | 0x18 | `FX_TSUNAMI_SPLASH_ROW_KEYS` | usage | table: per-frame position keys of the row of water splashes |
| fx_199 | 0x1 | `FX_DARKSIDE_SPARKLE_BURST` | render | emitter: yellow / blue ring, blue sparkle sprites, fan (byte-identical to fx_0bc entry 0x1) |
| fx_19a | 0x4 | `FX_MINUS_STRIKE_SLASH_ARC` | render | emitter: blue slash arc, blue sparkle sprites, pale yellow ring, fan (byte-identical to fx_0bd entry 0x4) |
| fx_19b | 0x4 | `FX_IAI_STRIKE_SLASH_ARCS` | render | emitter: blue and pink slash arcs (byte-identical to fx_0be entry 0x4) |
| fx_19b | 0x8 | `FX_IAI_STRIKE_SPARKLE_BURST` | render | emitter: violet / blue ring, blue and gold sparkle sprites, fan (byte-identical to fx_0be entry 0x8) |
| fx_19c | 0x1 | `FX_THUNDER_SLASH_SPARKS` | render | emitter: cone of blue-green electric wisps, orange sparks and white glints (byte-identical to fx_0bf entry 0x1) |
| fx_19c | 0x2 | `FX_THUNDER_SLASH_LIGHTNING` | render | sprite: blue / red lightning streak, 12 frames (drawn directly; byte-identical to fx_0bf entry 0x2) |
| fx_19e | 0x1 | `FX_STOCK_BREAK_SLASH_ARC` | render | emitter: blue slash arc, fans (byte-identical to fx_0cf entry 0x1) |
| fx_19f | 0x5 | `FX_RAMUH_STAFF` | render (byte-identical twin, consistency pass) | 'so' model: gold gouraud staff with a hooked head (Ramuh's staff) (byte-identical to fx_0ba entry 0x4) |
| fx_19f | 0x6 | `FX_RAMUH_LIGHTNING_RIBBON` | runtime | trail: ribbon drawn between key points, crackling blue-white plasma strip at run time: the blue-white electric ribbons arcing across (byte-identical to fx_0ba entry 0x5) |
| fx_19f | 0x8 | `FX_RAMUH_LENS_FLASH_KEYS` | usage | table: per-frame size keys of the lens flash (FX_RAMUH_LENS_FLASH; byte-identical to fx_0ba entry 0x7) |
| fx_19f | 0xC | `FX_RAMUH_BLUE_GLOW_RINGS` | render | emitter: blue glow quad and fading light-blue gouraud rings (byte-identical to fx_0ba entry 0xA) |
| fx_19f | 0xF | `FX_RAMUH_LIGHTNING_RIBBON_2` | runtime (medium) | trail: second ribbon drawn between key points, the same blue-white plasma strip at run time (byte-identical to fx_0ba entry 0xD) |
| fx_19f | 0x16 | `FX_RAMUH_BOLT_SPARK_BURST` | runtime (medium) | emitter: white gouraud rings + small blue lightning-bolt glyph sprites in a glow, the cyan screen flash and bolt sparks round the strike (byte-identical to fx_0ba entry 0x14) |
| fx_19f | 0x17 | `FX_RAMUH_LENS_FLASH` | render | particles: white-violet lens flash (byte-identical to fx_0ba entry 0x15) |
| fx_19f | 0x18 | `FX_RAMUH_LIGHTNING_BOLT` | runtime (medium) | trail: 8-frame trail, a blue forked lightning strip at run time (byte-identical to fx_0ba entry 0x16) |
| fx_19f | 0x1A | `FX_RAMUH_THIN_ROD` | render (byte-identical twin, consistency pass) | 'so' model: thin dark rod with a red tip (byte-identical to fx_0ba entry 0x18) |
| fx_19f | 0x1B | `FX_RAMUH_MOTION_KEYS_2` | usage | table: per-frame position / rotation keys (op81, second phase; byte-identical to fx_0ba entry 0x19) |
| fx_19f | 0x1C | `FX_RAMUH_MOTION_KEYS` | usage | table: per-frame position / rotation keys (op81, first phase; byte-identical to fx_0ba entry 0x1A) |
| fx_19f | 0x1D | `FX_RAMUH_BLUE_RING` | render | rings: blue gouraud rings (byte-identical to fx_0ba entry 0x1B) |
| fx_19f | 0x1F | `FX_RAMUH_CYAN_RING` | render | rings: white / cyan / blue gouraud rings (byte-identical to fx_0ba entry 0x1D) |
| fx_19f | 0x20 | `FX_RAMUH_ORANGE_LIGHTNING` | render (byte-identical twin, consistency pass) | trail: 8-frame orange lightning bolt (offset trail) (byte-identical to fx_0ba entry 0x1E, same render) |
| fx_19f | 0x21 | `FX_RAMUH_ELECTRIC_BOWL` | render (byte-identical twin, consistency pass) | 'so' model: shallow bowl with blue electric flames around a white glow (byte-identical to fx_0ba entry 0x1F) |
| fx_19f | 0x22 | `FX_RAMUH_BLUE_AURA_COLUMN` | runtime | 'so' model: dome-capped column; its page holds a blue-white marbled texture at run time (no 'dummy' placeholder): the huge glassy blue energy shroud round Ramuh's staff |
| fx_19f | 0x23 | `FX_RAMUH_GREY_RING` | render | rings: grey gouraud ring |
| fx_19f | 0x27 | `FX_RAMUH_GREEN_DOME` | render (byte-identical twin, consistency pass) | 'so' model: teal-green gouraud hemisphere (byte-identical to fx_0ba entry 0x26) |
| fx_19f | 0x2A | `FX_RAMUH_BOLT_SPARKS` | runtime (medium) | emitter: one particle, a blue lightning-bolt glyph sprite at run time: the small bolt sparks around the pillar (byte-identical to fx_0ba entry 0x2C) |
| fx_19f | 0x2B | `FX_RAMUH_CAMERA_KEYS` | usage | table: per-frame camera point 0 (op82; byte-identical to fx_0ba entry 0x2E) |
| fx_19f | 0x2C | `FX_RAMUH_CAMERA_KEYS_2` | usage | table: per-frame camera point 1 (op82; byte-identical to fx_0ba entry 0x2F) |
| fx_1a0 | 0x1 | `FX_FLAME_SLASH_RED_RINGS` | render | emitter: red gouraud rings, 16 frames + dark red speck sprites |
| fx_1a0 | 0x3 | `FX_FLAME_SLASH_FIRE_BURST` | render | emitter: fire explosion sprites + red spark sprites |
| fx_1a0 | 0x4 | `FX_FLAME_SLASH_FLAME_FLASH` | render | emitter: white-hot flash turning red and fading + grey smoke |
| fx_1a0 | 0x13 | `FX_FLAME_SLASH_FLAME_RINGS` | render | emitter: rings textured with a dark flame wall + red gouraud rings + red specks |
| fx_1a1 | 0x1 | `FX_CLIMHAZZARD_FLAME_RINGS` | render | emitter: rings textured with a yellow-red flame wall + white / pale-yellow gouraud rings, 16 frames (byte-identical to fx_18d entry 0x1) |
| fx_1a2 | 0x1 | `FX_MOONLIGHT_SLASH_LIGHT_BAND` | render | model: cylinder band, blue light-streak texture |
| fx_1a2 | 0x2 | `FX_MOONLIGHT_SLASH_LIGHT_BAND_2` | render | model: cylinder band, blue light-streak texture |
| fx_1a2 | 0x3 | `FX_MOONLIGHT_SLASH_LIGHT_RING` | render | model: flat ring of blue light streaks |
| fx_1a2 | 0x5 | `FX_MOONLIGHT_SLASH_MOON` | render | emitter: blue full-moon particles, 30 frames + blue gouraud rings |
| fx_1a2 | 0x6 | `FX_MOONLIGHT_SLASH_SPIKED_SHAPE` | kind/usage (look pending) | model: 20-poly spiked shape (Hi_RegisterTexListModel, texture not drawn); runtime: untextured spiky model, fetched early; nothing visible in a forced cast with Zidane as the caster (a real enemy cast may differ) |
| fx_1a2 | 0x7 | `FX_MOONLIGHT_SLASH_CYAN_RINGS` | render | emitter: thin cyan streak fan + white-cyan gouraud rings |
| fx_1a2 | 0x8 | `FX_MOONLIGHT_SLASH_BLUE_GLOWS` | runtime (low, look pending) | emitter: three sprite elements, 24 frames, soft blue glows at run time (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently) |
| fx_1a2 | 0x9 | `FX_MOONLIGHT_SLASH_CRESCENT_TRAIL` | render | emitter: spiralling 3D trail, blue crescent strip |
| fx_1a2 | 0xA | `FX_MOONLIGHT_SLASH_CRESCENT_TRAIL_2` | render | emitter: 3D trail, blue crescent strip |
| fx_1a2 | 0xB | `FX_MOONLIGHT_SLASH_BLADE` | render | model: curved blade, blue-white feathered streak texture |
| fx_1a2 | 0xC | `FX_MOONLIGHT_SLASH_BLADE_2` | render | model: curved blade, blue-white feathered streak texture |
| fx_1a2 | 0xD | `FX_MOONLIGHT_SLASH_BLADE_3` | render | model: curved blade, blue-white feathered streak texture |
| fx_1a2 | 0xE | `FX_MOONLIGHT_SLASH_ROT_KEYS` | usage | table: per-frame rotation keys |
| fx_1a2 | 0xF | `FX_MOONLIGHT_SLASH_CYAN_STREAKS` | render | emitter: three fans, thin cyan light streaks |
| fx_1a2 | 0x10 | `FX_MOONLIGHT_SLASH_ARMOR` | render | model: grey-brown armour plating (129 polys) |
| fx_1a2 | 0x11 | `FX_MOONLIGHT_SLASH_ARMOR_2` | render | model: grey-brown armour plating (24 polys) |
| fx_1a2 | 0x12 | `FX_MOONLIGHT_SLASH_WHITE_RINGS` | render | rings: white gouraud rings, 10 frames |
| fx_1a2 | 0x13 | `FX_MOONLIGHT_SLASH_WHITE_RINGS_2` | render | rings: white gouraud rings, 10 frames |
| fx_1a2 | 0x14 | `FX_MOONLIGHT_SLASH_CYAN_SPARKLES` | render | emitter: cyan sparkle dots, 12 frames |
| fx_1a2 | 0x15 | `FX_MOONLIGHT_SLASH_CYAN_SPARKLES_2` | render | emitter: cyan sparkle dots + faint green-grey particles |
| fx_1a2 | 0x17 | `FX_MOONLIGHT_SLASH_COLOR_GLOWS` | runtime (low, look pending) | emitter: three sprite elements, 24 frames, blue, violet and green glows at run time (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently) |
| fx_1a3 | 0x1 | `FX_TROUBLE_MUCUS_SLIME_BLOBS` | render | emitter: yellow slime blobs (byte-identical to fx_106 entry 0x1) |
| fx_1a3 | 0x2 | `FX_TROUBLE_MUCUS_BUBBLE_TRAIL` | render | emitter: violet bubble screen trail + yellow speck dots (byte-identical to fx_106 entry 0x2) |
| fx_1a3 | 0x3 | `FX_TROUBLE_MUCUS_BUBBLE_TRAIL_2` | render | emitter: violet bubble screen trail + yellow speck dots (byte-identical to fx_106 entry 0x3) |
| fx_1a3 | 0x4 | `FX_TROUBLE_MUCUS_YELLOW_RINGS` | render | emitter: yellow-green gouraud rings (byte-identical to fx_106 entry 0x4) |
| fx_1a3 | 0x5 | `FX_TROUBLE_MUCUS_SPLASH_STREAK` | render | emitter: white streaky splash screen trail (byte-identical to fx_106 entry 0x5) |
| fx_1a4 | 0x1 | `FX_DEATH0_ROCK_DEBRIS` | render | emitter: brown rock chips + orange spark dots |
| fx_1a4 | 0x2 | `FX_DEATH0_PINK_RINGS` | render | emitter: spiral of white-pink-magenta gouraud rings + tan dust cloud |
| fx_1a4 | 0x3 | `FX_DEATH0_DUST_BURST` | render | emitter: tan dust clouds + grey smoke |
| fx_1a4 | 0x4 | `FX_DEATH0_FIRE_DEBRIS` | render | emitter: brown rock chips + spark dots + red fire flames |
| fx_1a4 | 0x5 | `FX_DEATH0_COLOUR_STREAKS` | render | emitter: fans of thin blue / orange / green streaks with violet and teal smoke + pale-blue rings |
| fx_1a4 | 0x7 | `FX_DEATH0_FLAME_COLUMN` | render | 'so' model: tall cylinder, flame texture |
| fx_1a4 | 0xC | `FX_DEATH0_DUST_SPIRAL` | render | emitter: spiral of tan dust clouds + grey smoke |
| fx_1a4 | 0xD | `FX_DEATH0_DUST_SPIRAL_2` | render | emitter: spiral of tan dust clouds + grey smoke |
| fx_1a4 | 0xE | `FX_DEATH0_FLAME_COIL` | render | 'so' model: helix coil, flame texture |
| fx_1a4 | 0xF | `FX_DEATH0_FLAME_COIL_2` | render | 'so' model: helix coil, flame texture |
| fx_1a5 | 0x1 | `FX_REVIVE_LIGHT_SWIRL` | render | emitter: blue-white light swirl + flare (byte-identical to fx_089 entry 0x4) |
| fx_1a5 | 0x2 | `FX_REVIVE_FEATHERS` | render | emitter: white wing / feather sprites + flare (byte-identical to fx_089 entry 0x5) |
| fx_1a6 | 0x1 | `FX_WATER_GUN_BUBBLES` | render | emitter: white bubble-ring anim quads, 22 frames |
| fx_1a6 | 0x3 | `FX_WATER_GUN_SPRAY` | runtime | emitter: one particle element, the white water splash spray over the target |
| fx_1a6 | 0x5 | `FX_WATER_GUN_SPLASH` | render | emitter: blue water-splash crescents + white splash |
| fx_1a6 | 0x6 | `FX_WATER_GUN_WATER_JET` | render | emitter: screen trail with a water strip + white splash particles (sprites render black) |
| fx_1a7 | 0x1 | `FX_MIND_BLAST_WAVE_TRAIL` | render | emitter: orbiting screen trail, blue wavy line / gold pattern strip (sprites render black) |
| fx_1a7 | 0x2 | `FX_MIND_BLAST_GREEN_RINGS` | render | emitter: dark-to-pale green gouraud rings, 33 frames |
| fx_1a7 | 0x4 | `FX_MIND_BLAST_WAVE_TRAIL_2` | render | emitter: wobbling screen trail, blue wavy line / gold pattern strip (sprites render black) |
| fx_1a7 | 0x5 | `FX_MIND_BLAST_TEAL_RINGS` | render | emitter: teal-to-pale-green gouraud rings, 24 frames |
| fx_1a7 | 0x7 | `FX_MIND_BLAST_WAVE_TRAIL_3` | render | emitter: wobbling screen trail, white-violet wavy strip (sprites render black) |
| fx_1a7 | 0x18 | `FX_MIND_BLAST_VIOLET_ORB` | render | emitter: violet glowing orb with a bright rim (second particle renders black) |
| fx_1a8 | 0x1 | `FX_ODIN_BLUE_RINGS` | render | emitter: white-cyan-violet and white-cyan-blue gouraud rings |
| fx_1a8 | 0x2 | `FX_ODIN_FLAME_TRAIL` | render | trail: orange flame strip |
| fx_1a8 | 0x5 | `FX_ODIN_GOLD_RAYS` | runtime (low, look pending) | emitter: two fan elements, a thin pale-gold streak at run time, matching the gold rays of the slash explosion |
| fx_1a8 | 0x6 | `FX_ODIN_SKY_CRYSTAL` | render | 'so' model: polyhedron crystal, blue sky-and-cloud texture |
| fx_1a8 | 0x7 | `FX_ODIN_FLAME_RIBBON` | render | 'so' model: twisted ribbon, fire texture |
| fx_1a8 | 0x8 | `FX_ODIN_FLAME_RIBBON_2` | render | 'so' model: twisted ribbon, fire texture |
| fx_1a8 | 0x9 | `FX_ODIN_FLAME_RIBBON_3` | render | 'so' model: twisted ribbon, fire texture |
| fx_1a8 | 0xA | `FX_ODIN_SKY_CRYSTAL_2` | render | 'so' model: flattened crystal, blue sky-and-cloud texture |
| fx_1a8 | 0xB | `FX_ODIN_CRIMSON_RINGS` | render | rings: white-crimson gouraud rings, 16 frames |
| fx_1a8 | 0xC | `FX_ODIN_BLACK_DEBRIS` | runtime | emitter: spiral of two speck-sprite elements, dark grey specks at run time: the black debris flying out of the explosion |
| fx_1a8 | 0xD | `FX_ODIN_CRIMSON_RINGS_2` | render | emitter: white-crimson and white-violet gouraud rings |
| fx_1a8 | 0x10 | `FX_ODIN_FLAME_RIBBON_4` | render | 'so' model: twisted ribbon, fire texture |
| fx_1a8 | 0x11 | `FX_ODIN_STONE_RING` | render | 'so' model: ring band, grey-green stone texture |
| fx_1a8 | 0x12 | `FX_ODIN_RED_SPARKS` | runtime (low, look pending) | emitter: speck sprites, tiny red sparks at run time (not separable on screen) |
| fx_1a8 | 0x13 | `FX_ODIN_PINK_SMOKE` | render | emitter: white-pink smoke burst quads, 22 frames |
| fx_1a8 | 0x14 | `FX_ODIN_CLOUD_TORUS` | render | 'so' model: torus, dark marbled cloud texture |
| fx_1a8 | 0x15 | `FX_ODIN_GOLD_RING` | render | 'so' model: ring band, yellow-brown texture |
| fx_1a8 | 0x16 | `FX_ODIN_VORTEX_CONE` | render | 'so' model: spiralling cone, brown-grey texture |
| fx_1a8 | 0x17 | `FX_ODIN_CALLBACK_EMITTER` | kind/usage (look pending) | emitter: two callback elements (drawn by the module); runtime: started with the slash explosion; ablating it only shifts the random pink smoke puffs |
| fx_1a8 | 0x19 | `FX_ODIN_ZANTETSUKEN` | render | 'so' model: thin sword with a hilt, red-violet texture (the Zantetsuken blade) |
| fx_1a9 | 0x3 | `FX_VIRUS_POWDER_PUFFS` | render | emitter: violet, blue and orange powder puffs bursting into specks + pink dots |
| fx_1ab | 0x1 | `FX_WATERGA_WATER_RING` | render | 'so' model: flared ring, blue water texture |
| fx_1ab | 0x2 | `FX_WATERGA_WATER_SPHERE` | render | 'so' model: sphere, blue water-caustic texture |
| fx_1ab | 0x3 | `FX_WATERGA_WATER_SPLASH` | render | emitter: cone of dark-blue water splash sprites, 24 frames |
| fx_1ab | 0x4 | `FX_WATERGA_WHITE_FOAM` | runtime (medium) | emitter: cone of textured rings, the white foam inside the blue water sphere |
| fx_1ab | 0x5 | `FX_WATERGA_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_1ab | 0x6 | `FX_WATERGA_BACKDROP_2` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_1ab | 0x7 | `FX_WATERGA_BACKDROP_3` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_1ab | 0x8 | `FX_WATERGA_WATER_SPLASH_2` | render | emitter: sphere of dark-blue water splash sprites, 24 frames |
| fx_1ac | 0x1 | `FX_VIRUS_CRUNCH_MOTHS` | render | emitter: orbiting gold / green / orange moth-wing quads, 24 frames |
| fx_1ac | 0x2 | `FX_VIRUS_CRUNCH_MOTHS_2` | render | emitter: gold / orange / green moth-wing quads, 24 frames |
| fx_1ac | 0x3 | `FX_VIRUS_CRUNCH_CYAN_CLOUD` | render | emitter: white-grey gouraud rings + teal-cyan cloud quads |
| fx_1ac | 0x5 | `FX_VIRUS_CRUNCH_GREY_TRAIL` | render | emitter: flat trail, grey faceted strip |
| fx_1ad | 0x1 | `FX_6_LEGS_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_1ad | 0x2 | `FX_6_LEGS_SLIME_SHELL` | runtime (medium) | 'so' model: rounded pod with an opening; page 0x117 holds a green slimy texture at run time: the green slime shell wrapping the target |
| fx_1ad | 0x4 | `FX_6_LEGS_BUBBLE_TRAIL` | render | emitter: orbiting 3D trail of glass bubbles (sprites render black) |
| fx_1ad | 0x5 | `FX_6_LEGS_GREEN_TRAIL` | render | emitter: screen trail, green striped strip |
| fx_1af | 0x2 | `FX_DEATH_SPECKS` | render | emitter: faint grey speck sprites, 32 frames |
| fx_1af | 0x3 | `FX_DEATH_ASH_COLUMN` | render | emitter: column of dark olive ash flakes + dark smoke puffs |
| fx_1af | 0x4 | `FX_DEATH_ASH_COLUMN_2` | render | emitter: column of dark olive ash flakes + dark smoke puffs |
| fx_1af | 0x5 | `FX_DEATH_ASH_COLUMN_3` | render | emitter: column of dark olive ash flakes + dark smoke puffs |
| fx_1af | 0x6 | `FX_DEATH_ASH_COLUMN_4` | render | emitter: column of dark olive ash flakes + dark smoke puffs |
| fx_1af | 0x7 | `FX_DEATH_DARK_SMOKE` | render | emitter: dark smoke puffs, 16 frames |
| fx_1af | 0x8 | `FX_DEATH_SPECKS_2` | render | emitter: faint grey speck sprites, 32 frames |
| fx_1af | 0x9 | `FX_DEATH_OLIVE_RINGS` | render | emitter: pale yellow-olive gouraud rings |
| fx_1af | 0xA | `FX_DEATH_SMOKE` | render | emitter: grey-tan smoke puffs, 16 frames |
| fx_1af | 0xD | `FX_DEATH_PINK_RINGS` | render | emitter: white-pink-magenta gouraud rings + 3D trail of white smoke streaks |
| fx_1b0 | 0x2 | `FX_DEATH_ROCKS` | render | emitter: spiralling 3D trail + grey rock boulders |
| fx_1b0 | 0x3 | `FX_DEATH_LIGHTNING_TRAIL` | render | emitter: 3D trail, blue streak strip and blue lightning |
| fx_1b0 | 0x4 | `FX_DEATH_FIRE_RINGS` | render | emitter: white-yellow-orange-red gouraud rings |
| fx_1b0 | 0x5 | `FX_DEATH_DUST_RINGS` | render | emitter: rings textured with tan dust, 24 frames |
| fx_1b0 | 0x6 | `FX_DEATH_COLOUR_RINGS` | render | emitter: pale-yellow / cyan / orange / magenta gouraud rings |
| fx_1b0 | 0x9 | `FX_DEATH_BLUE_FLARE` | render | emitter: blue cone-beam fan + blue lens-flare streak + yellow-red and pink gouraud rings |
| fx_1b0 | 0xC | `FX_DEATH_ROCK_TRAIL` | render | emitter: orbiting screen trail, grey rock strip |
| fx_1b0 | 0xD | `FX_DEATH_SPECKS` | render | emitter: spiral of faint white speck sprites, 39 frames |
| fx_1b0 | 0xE | `FX_DEATH_DUST_RINGS_2` | render | emitter: pale-yellow / cyan / magenta gouraud rings + rings textured with tan dust |
| fx_1b0 | 0x11 | `FX_DEATH_PINK_RINGS` | render | rings: pale-pink to crimson gouraud rings, 24 frames |
| fx_1b1 | 0x1 | `FX_GOLEM_DEATH_DUST_CLOUD` | render | emitter: tan dust cloud quads, 17 frames (sprites render black) |
| fx_1b1 | 0x5 | `FX_GOLEM_DEATH_DUST_CLOUD_2` | render | emitter: orbiting tan dust cloud quads, 17 frames (sprites render black) |
| fx_1b2 | 0x1 | `FX_TIDAL_WAVE_PATH` | usage | table: vector path (fxm_vecPathInit) |
| fx_1b2 | 0x2 | `FX_TIDAL_WAVE_WATER_CURL` | runtime | model: curved sheet like a breaking wave; page 0x117 holds blue-white water at run time: the white curling wave crest |
| fx_1b2 | 0x3 | `FX_TIDAL_WAVE_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_1b2 | 0x7 | `FX_TIDAL_WAVE_WATER_ORBS` | render | emitter: orbiting blue water-caustic orbs + grey-blue shard sprites |
| fx_1b2 | 0x8 | `FX_TIDAL_WAVE_RED_TRAIL` | render | emitter: helix flat trail, red caustic strip |
| fx_1b2 | 0x9 | `FX_TIDAL_WAVE_WATER_TRAIL` | render | emitter: spiralling screen trail, grey-blue water streak strip |
| fx_1b2 | 0xB | `FX_TIDAL_WAVE_PALE_RINGS` | render | emitter: white / cream / olive gouraud rings |
| fx_1b2 | 0xC | `FX_TIDAL_WAVE_SURGE` | runtime (medium) | model: flat soft-edged disc, the big grey water surge sweeping over the party |
| fx_1b2 | 0xD | `FX_TIDAL_WAVE_FOAM` | render | emitter: orbiting grey-white foam cloud quads, 17 frames |
| fx_1b3 | 0x2 | `FX_DEATH1_DARK_NEBULA` | render | emitter: dark violet nebula particles + pale-yellow light-streak fans |
| fx_1b3 | 0x4 | `FX_DEATH1_LIGHTNING_TRAILS` | render | emitter: orbiting 3D trails with violet lightning strips + dark violet nebula particles |
| fx_1b3 | 0x5 | `FX_DEATH1_TEAL_RINGS` | render | emitter: white / grey / violet and teal / orange gouraud rings |
| fx_1b3 | 0x6 | `FX_DEATH1_GOLD_RAYS` | runtime | emitter: two fan elements, the pale-gold light rays fanning out (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently) |
| fx_1b3 | 0x7 | `FX_DEATH1_YELLOW_STREAKS` | render | emitter: pale-yellow light-streak fans + white-violet and cream-crimson gouraud rings |
| fx_1b3 | 0x8 | `FX_DEATH1_EXPLOSIONS` | render | emitter: fire explosion quads, three elements, + speck sprites |
| fx_1b3 | 0x9 | `FX_DEATH1_ROCKS` | render | emitter: grey rock particles + magenta patterned particles |
| fx_1b3 | 0xA | `FX_DEATH1_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_1b3 | 0xB | `FX_DEATH1_TAN_STREAKS` | render | emitter: tan light-streak fans + white-violet gouraud rings with a silver nebula texture |
| fx_1b3 | 0xC | `FX_DEATH1_GREEN_SMOKE` | render | emitter: teal grid quads + pale-green smoke puffs + speck sprites |
| fx_1b3 | 0xD | `FX_DEATH1_ROCK_DEBRIS` | render | emitter: grey rock quads, three elements |
| fx_1b3 | 0xE | `FX_DEATH1_ROCK_DEBRIS_2` | render | emitter: orbiting grey rock quads |
| fx_1b3 | 0x10 | `FX_DEATH1_TEAL_RINGS_2` | render | emitter: white / grey / violet and teal / orange gouraud rings |
| fx_1b3 | 0x11 | `FX_DEATH1_RIPPLE_RINGS` | render | emitter: rings textured with orange and grey ripple patterns |
| fx_1b3 | 0x12 | `FX_DEATH1_LIGHTNING_TRAILS_2` | render | emitter: orbiting 3D trails with violet lightning strips + white / navy / cream gouraud rings |
| fx_1b3 | 0x13 | `FX_DEATH1_ORANGE_RINGS` | render | emitter: white-orange-plum gouraud rings, 40 frames |
| fx_1b3 | 0x14 | `FX_DEATH1_TAN_STREAKS_2` | render | emitter: two tan light-streak fans |
| fx_1b3 | 0x15 | `FX_DEATH1_LIGHTNING` | render | trail: violet lightning strip |
| fx_1b3 | 0x39 | `FX_DEATH1_SCREEN_TRAIL` | kind/usage (look pending) | emitter: orbiting screen trails (strip texture not resolved) + speck sprites; runtime: not fetched within 1000 frames of a forced cast (Zidane as the caster) |
| fx_1b3 | 0x3A | `FX_DEATH1_SCREEN_TRAIL_2` | kind/usage (look pending) | emitter: orbiting screen trails (strip texture not resolved) + speck sprites; runtime: not fetched within 1000 frames of a forced cast (Zidane as the caster) |
| fx_1b3 | 0x93 | `FX_DEATH1_SCRATCH_BUF` | usage | summon data entry 0x13 (0x93 - 0x80): its address is the base of the module's trail and split-piece work buffers |
| fx_1b4 | 0xA | `FX_FLARE_STAR_PINK_SPARKS` | render | emitter: pink / orange glyph-like spark sprites + pink and lilac gouraud rings |
| fx_1b4 | 0xB | `FX_FLARE_STAR_RED_RAYS` | runtime | emitter: two fan elements, the red light rays radiating from the target over the violet field |
| fx_1b4 | 0xC | `FX_FLARE_STAR_FLAME_TRAIL` | render | emitter: white fan bars + screen trail with a red flame strip |
| fx_1b4 | 0xD | `FX_FLARE_STAR_YELLOW_RINGS` | render | emitter: pale-yellow / tan gouraud rings |
| fx_1b4 | 0xE | `FX_FLARE_STAR_YELLOW_RINGS_2` | render | emitter: pale-yellow / pink gouraud rings |
| fx_1b4 | 0xF | `FX_FLARE_STAR_PINK_RINGS` | render | emitter: thin pink streak fan + white-pink gouraud rings |
| fx_1b4 | 0x10 | `FX_FLARE_STAR_PINK_SPARKS_2` | render | emitter: pink glyph-like spark sprites, 33 frames |
| fx_1b4 | 0x11 | `FX_FLARE_STAR_RED_RINGS` | render | emitter: golden sparkle ring + lilac and crimson gouraud rings |
| fx_1b4 | 0x12 | `FX_FLARE_STAR_SMOKE_TRAIL` | render | emitter: spiralling 3D trail, grey smoke strip |
| fx_1b4 | 0x13 | `FX_FLARE_STAR_RED_ORB` | render | emitter: red glowing orb particles + violet-pink rings + red streak trail |
| fx_1b4 | 0x1C | `FX_FLARE_STAR_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_1b4 | 0x29 | `FX_FLARE_STAR_PINK_RINGS_2` | render | emitter: white-pink-magenta and yellow-olive gouraud rings |
| fx_1b4 | 0x2A | `FX_FLARE_STAR_ORANGE_GLOW` | render | emitter: fading orange glow + red flare streaks |
| fx_1b4 | 0x2B | `FX_FLARE_STAR_RED_SHARDS` | render | emitter: red shard sprites, 31 frames |
| fx_1b6 | 0x2 | `FX_DEATH_SPIRAL_TRAIL` | kind/usage (look pending) | emitter: spiralling 3D trail, strip mostly black with a faint white streak; runtime: started as the screen goes white; its run-time strip is a white streak |
| fx_1b6 | 0x3 | `FX_DEATH_YELLOW_RINGS` | render | emitter: cream and yellow gouraud rings |
| fx_1b6 | 0x4 | `FX_DEATH_RED_LIGHTNING` | render | emitter: orbiting red lightning fan + yellow-to-brown gouraud rings |
| fx_1b6 | 0x5 | `FX_DEATH_BUBBLE_RINGS` | render | emitter: rings textured with white bubble spots + pale-yellow gouraud rings |
| fx_1b6 | 0x7 | `FX_DEATH_STAR_SPECKS` | render | emitter: cone of faint teal glows + white star dots, 40 frames |
| fx_1b6 | 0x8 | `FX_DEATH_CYAN_RINGS` | render | emitter: cyan gouraud rings |
| fx_1b7 | 0x2 | `FX_DEATH_VIOLET_RINGS` | render | emitter: violet / magenta and cream / tan gouraud rings |
| fx_1b7 | 0x3 | `FX_DEATH_RED_LIGHTNING_TRAIL` | render | emitter: wobbling 3D trail, red lightning strip |
| fx_1b7 | 0x4 | `FX_DEATH_YELLOW_GLOW` | render | emitter: orbiting fan, yellow-green glow streak, 17 frames |
| fx_1b7 | 0x5 | `FX_DEATH_ORANGE_RINGS` | render | emitter: pale-yellow / orange gouraud rings |
| fx_1b7 | 0x6 | `FX_DEATH_FLARE_STREAK` | render | emitter: red-orange lens-flare streak + white-yellow-red gouraud rings |
| fx_1b7 | 0x7 | `FX_DEATH_STAR_SPECKS` | render | emitter: spiral of faint teal glows + white star dots, 39 frames |
| fx_1b7 | 0x8 | `FX_DEATH_OLIVE_RINGS` | render | emitter: pale-yellow / olive gouraud rings |
| fx_1b7 | 0x9 | `FX_DEATH_SMOKE_TRAIL` | render | emitter: spiralling 3D trail, grey smoke strip |
| fx_1b7 | 0xA | `FX_DEATH_BUBBLE_RINGS` | render | emitter: rings textured with olive bubble spots + pink / pale-cyan gouraud rings |
| fx_1b7 | 0xB | `FX_DEATH_BLUE_BEAM` | render | emitter: blue cone-beam fan + grey gouraud rings |
| fx_1b7 | 0xC | `FX_DEATH_BLUE_STREAK` | render | emitter: one fan, thin blue streak |
| fx_1b8 | 0x1 | `FX_DEATH_TEAL_GLOWS` | runtime (medium) | emitter: anim quads + sprites, soft teal-white glows drifting on screen (byte-identical to fx_1b9 entry 0x2) |
| fx_1b8 | 0x2 | `FX_DEATH_WATER_WAVE` | render | emitter: green-teal water wave particles, 16 frames |
| fx_1b8 | 0x6 | `FX_DEATH_SWIRL` | render | emitter: orbiting white swirl vortex particles (sprites render black) |
| fx_1b8 | 0x7 | `FX_DEATH_RIPPLES` | render | emitter: green ripple-circle particles + grey widening ring (byte-identical to fx_1b9 entry 0x1) |
| fx_1b8 | 0xE | `FX_DEATH_WATER_SPOUT` | render | emitter: green / blue water spout, 10 frames (byte-identical to fx_1b9 entry 0x7) |
| fx_1b9 | 0x1 | `FX_APPEARANCE_RIPPLES` | render | emitter: green ripple-circle particles + grey widening ring (byte-identical to fx_1b8 entry 0x7) |
| fx_1b9 | 0x2 | `FX_APPEARANCE_TEAL_GLOWS` | runtime (medium) | emitter: anim quads + sprites, soft teal-white glows (byte-identical to fx_1b8 entry 0x1) |
| fx_1b9 | 0x6 | `FX_APPEARANCE_WATER_WAVE` | render | emitter: green-teal water wave particles, 16 frames |
| fx_1b9 | 0x7 | `FX_APPEARANCE_WATER_SPOUT` | render | emitter: green / blue water spout, 10 frames (byte-identical to fx_1b8 entry 0xE) |
| fx_1bb | 0x1 | `FX_CLIMHAZZARD_FLAME_RINGS` | render | emitter: rings textured with a yellow-red flame wall + white / pale-yellow gouraud rings, 16 frames (byte-identical to fx_18d entry 0x1) |
| fx_1bd | 0x6 | `FX_IFRIT_VEC_TRACK` | usage | table: vector track (fxm_vecTrackInit; byte-identical to fx_114 entry 0x1D) |
| fx_1bd | 0x7 | `FX_IFRIT_VEC_TRACK_3` | usage | table: vector track (fxm_vecTrackInit; byte-identical to fx_114 entry 0x25) |
| fx_1bd | 0x8 | `FX_IFRIT_POS_TRACK` | usage | table: position track (fxm_posTrackInit; byte-identical to fx_114 entry 0x23) |
| fx_1bd | 0x9 | `FX_IFRIT_VEC_TRACK_2` | usage | table: vector track (fxm_vecTrackInit; byte-identical to fx_114 entry 0x24) |
| fx_1bd | 0xA | `FX_IFRIT_ORANGE_RINGS` | render | emitter: orange gouraud rings, 16 frames (byte-identical to fx_114 entry 0xB) |
| fx_1bd | 0xB | `FX_IFRIT_FLAME_RING` | render | model: flat ring, flame texture (byte-identical to fx_114 entry 0x22) |
| fx_1bd | 0xD | `FX_IFRIT_WHITE_HAZE` | runtime | model: grid with grey-white wavy streaks, the grey streaky haze over the floor under the target, turning orange as the lava cracks appear (byte-identical to fx_114 entry 0x2A) |
| fx_1bd | 0xE | `FX_IFRIT_FLAME_DOME` | render | model: dome, flame texture (byte-identical to fx_114 entry 0x27) |
| fx_1bd | 0x12 | `FX_IFRIT_RED_GLOW` | render (byte-identical twin, consistency pass) | emitter: orbiting fan, a horizontal orange-red glow (byte-identical to fx_114 entry 0x26, same render) |
| fx_1bd | 0x13 | `FX_IFRIT_HEAT_DOME` | runtime | model: hemisphere bowl, the bright yellow-white translucent glare dome over the fire dome (byte-identical to fx_114 entry 0x1E) |
| fx_1bd | 0x14 | `FX_IFRIT_SHOT0_CAMERA_KEYS_2` | usage | table: camera key channel (fxm_camChanAdd from frame 0 into env->camera[1]) |
| fx_1bd | 0x15 | `FX_IFRIT_SHOT0_CAMERA_KEYS_1` | usage | table: camera key channel (fxm_camChanAdd from frame 0 into env->camera[0]) |
| fx_1bd | 0x16 | `FX_IFRIT_TRAIL_PATHS` | usage | table: offset table of position paths (fxm_offsetTableEntry + fxm_posPathInit) for the fire trails (byte-identical to fx_114 entry 0x17) |
| fx_1bd | 0x17 | `FX_IFRIT_FIRE_TRAIL` | render | trail: orange strip, drawn along the paths of FX_IFRIT_TRAIL_PATHS (byte-identical to fx_114 entry 0x15) |
| fx_1bd | 0x18 | `FX_IFRIT_ORANGE_RING_2` | render | emitter: orange to white gouraud ring (byte-identical to fx_114 entry 0x16) |
| fx_1bd | 0x26 | `FX_IFRIT_FIRE_BURST` | render | emitter: ember specks + orange fire-burst clouds (byte-identical to fx_114 entry 0x21) |
| fx_1bd | 0x28 | `FX_IFRIT_PIECE_TRACKS` | usage | table: offset table of tracks, one per piece (fxm_offsetTableEntry; byte-identical to fx_114 entry 0x29) |
| fx_1bd | 0x29 | `FX_IFRIT_PIECE_MODELS` | usage | table: offset table of model data, one per piece (fxm_offsetTableEntry; byte-identical to fx_114 entry 0x28) |
| fx_1bd | 0x2B | `FX_IFRIT_BLOCK_CLUSTER` | render | model: cluster of block chunks, orange streak texture (byte-identical to fx_114 entry 0x20) |
| fx_1bd | 0x2C | `FX_IFRIT_SPOKED_RING` | kind/usage (look pending) | model: flat ring with spokes (registered through the texture list, texture not drawn; byte-identical to fx_114 entry 0x12); runtime: ablating it alone changes only a few hundred scattered px |
| fx_1be | 0x1 | `FX_ATOMOS_PURPLE_RINGS` | render | emitter: white / magenta / purple and pale yellow / grey gouraud rings (byte-identical to fx_0b8 entry 0x21) |
| fx_1be | 0x2 | `FX_ATOMOS_ORANGE_STREAKS` | render (byte-identical twin, consistency pass) | emitter: two fans, thin orange light streaks (byte-identical to fx_0b8 entry 0x7, same render) |
| fx_1be | 0x4 | `FX_ATOMOS_VIOLET_RING` | render | emitter: white-violet and white-blue gouraud rings |
| fx_1be | 0x5 | `FX_ATOMOS_ROCK_DEBRIS` | render | emitter: orbiting grey-blue rock debris particles, four elements |
| fx_1be | 0x6 | `FX_ATOMOS_BLUE_SPECKS` | render | emitter: tiny blue speck sprites |
| fx_1be | 0x7 | `FX_ATOMOS_FIRE_RINGS` | render | emitter: violet-to-navy and white-crimson gouraud rings + rings textured with a pink / gold / blue fire circle |
| fx_1be | 0x8 | `FX_ATOMOS_NAVY_RINGS` | render | emitter: lilac / navy gouraud rings + small violet sprite |
| fx_1be | 0x9 | `FX_ATOMOS_VORTEX_TORUS` | render | 'so' model: torus, dark violet cloud texture |
| fx_1be | 0xA | `FX_ATOMOS_ORANGE_RING` | render | 'so' model: flat ring, orange cloud texture |
| fx_1be | 0xC | `FX_ATOMOS_RED_CLOUD_SPHERE` | runtime | 'so' model: segmented sphere; page 0x117 holds a dark red cloudy texture at run time: the dark red cloud sphere inside the violet gate |
| fx_1be | 0xD | `FX_ATOMOS_VORTEX_DOME` | render | 'so' model: dome, violet and cyan cloud texture |
| fx_1be | 0xE | `FX_ATOMOS_RING_FUNNEL` | render | 'so' model: gouraud ring funnel, white glow at the hole |
| fx_1be | 0xF | `FX_ATOMOS_BACKDROP` | usage | layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) |
| fx_1be | 0x10 | `FX_ATOMOS_GREY_RINGS` | render | emitter: white-to-grey gouraud rings, 8 frames |
