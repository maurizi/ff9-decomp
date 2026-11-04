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
| fx_0b2 | 0x1 | `FX_TWISTER_VORTEX` | render | particles: tan swirling vortex, drawn directly |
| fx_0b2 | 0x2 | `FX_TWISTER_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop |
| fx_0b2 | 0x4 | `FX_TWISTER_WIND_STREAK` | render | trail: violet-white wind streak (fxm_drawOffsetTrail) |
| fx_0b2 | 0x5 | `FX_TWISTER_DUST_CLOUDS` | render | emitter: spiralling grey-blue dust clouds |
| fx_0b4 | 0xE | `FX_AERO_GREEN_RING` | render | emitter: orbiting green gouraud rings |
| fx_0b4 | 0xF | `FX_AERO_WIND_RING` | render | emitter: orbiting pale green wind-ring particles |
| fx_0b4 | 0x10 | `FX_AERO_GREEN_BURST` | render | emitter: green gust puffs fading to blue |
| fx_0b4 | 0x11 | `FX_AERO_SPRITE_SWIRL` | kind/usage (look pending) | emitter: spiralling sprites, render all black |
| fx_0b4 | 0x19 | `FX_AERO_WIND_TRAIL` | render | emitter: white 3D wind trail spiralling |
| fx_0b5 | 0x1 | `FX_RAINBOW_WIND_RAINBOW_TRAIL` | render | emitter: rainbow-coloured 3D trail spiralling up |
| fx_0b5 | 0x2 | `FX_RAINBOW_WIND_WHIRLWIND` | render | emitter: blue-white whirlwind swirl particles |
| fx_0b5 | 0x5 | `FX_RAINBOW_WIND_RAINBOW_RING` | render | emitter: rainbow ring particle |
| fx_0b6 | 0x1 | `FX_SCORCH_FIREBALL` | render | emitter: red-orange swirling fireball particle |
| fx_0b6 | 0x2 | `FX_SCORCH_FAN_EMITTER` | kind/usage (look pending) | emitter: two fan elements (not rendered) |
| fx_0b6 | 0x7 | `FX_SCORCH_HEAT_RING` | render | emitter: white-pink gouraud rings |
| fx_0b7 | 0x2 | `FX_HEAVY_FAN_EMITTER` | kind/usage (look pending) | emitter: orbiting fan elements (not rendered) |
| fx_0b7 | 0x6 | `FX_HEAVY_GREEN_SWIRL` | render | emitter: green swirl ring particle + pink-white gouraud rings |
| fx_0bb | 0x1 | `FX_MUSTARD_BOMB_BACKDROP` | usage | table: background layers for btlfx_drawBackdrop |
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
| fx_0c5 | 0x8 | `FX_AURA_FAN_EMITTER` | kind/usage (look pending) | emitter: three orbiting fan elements (not rendered) |
| fx_0c6 | 0x5 | `FX_AURA_FAN_EMITTER` | kind/usage (look pending) | emitter: three orbiting fan elements (not rendered) (byte-identical to fx_0c5 entry 0x8) |
| fx_0c6 | 0x8 | `FX_AURA_SWIRL` | render | emitter: yellow / white / lilac swirl particles, blue star flash, sparkles, fans (byte-identical to fx_0c5 entry 0x7) |
| fx_0c7 | 0x1 | `FX_FOCUS_GLOW_RING` | render | emitter: orange gouraud ring, fans |
| fx_0d0 | 0x7 | `FX_POWER_BREAK_RED_SWIRL` | render | emitter: red-white swirl ring particles |
| fx_0d0 | 0x8 | `FX_POWER_BREAK_LIGHT_STREAK` | render | emitter: yellow light streak with a flare, red ember sprites |
| fx_0d0 | 0xC | `FX_POWER_BREAK_ENERGY_ORB` | render | emitter: yellow energy orb particle |
| fx_0d0 | 0xD | `FX_POWER_BREAK_SPARKLES` | render | emitter: gold sparkle sprites, fan |
| fx_0d1 | 0x4 | `FX_MENTAL_BREAK_PINK_GLOW` | render | emitter: pink glow sprite with red ember sprites |
| fx_0d1 | 0x7 | `FX_MENTAL_BREAK_RINGS` | render | emitter: white-violet, white-pink and pink-purple gouraud rings |
| fx_0d1 | 0x9 | `FX_MENTAL_BREAK_GLOW_CROSS` | render | emitter: four pink glows in a cross, orange / blue ring |
| fx_0d1 | 0xD | `FX_MENTAL_BREAK_ENERGY_ORB` | render | emitter: pink energy orb particle |
| fx_0d1 | 0xE | `FX_MENTAL_BREAK_SPARKLES` | render | emitter: pink star sparkle sprites, fan |
| fx_0d4 | 0x1 | `FX_SWDMAG_FIRE_FIREBALL` | render | emitter: fireball sprite with ember sparks, at the target body (byte-identical to fx_000 entry 0x5) |
| fx_0d4 | 0x2 | `FX_SWDMAG_FIRE_FLAME_BURST` | render | emitter: flame explosion (burst, smoke, embers) rising around the target (byte-identical to fx_000 entry 0x6) |
| fx_0d4 | 0x9 | `FX_SWDMAG_FIRE_HEAT_RING` | render | emitter: white-red gouraud rings at the target's feet (byte-identical to fx_000 entry 0x7) |
| fx_0d4 | 0xB | `FX_SWDMAG_FIRE_SWORD_GLINT` | render | rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat |
| fx_0d4 | 0xC | `FX_SWDMAG_FIRE_SWORD_GLINT_2` | render | rings: white gouraud ring flash at the sword, rotated |
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
| fx_0d6 | 0xE | `FX_SWDMAG_FIRAGA_ORBIT_TRAIL` | kind/usage (look pending) | emitter: orbiting trail + sprites at the start (texture not in the file) (byte-identical to fx_007 entry 0xc) |
| fx_0d6 | 0xF | `FX_SWDMAG_FIRAGA_COLUMN_SPARK` | render | rings: small red-white glow drawn at random fire-column segments (byte-identical to fx_007 entry 0xd) |
| fx_0d6 | 0x10 | `FX_SWDMAG_FIRAGA_FLAME_BURST_2` | render | emitter: flame explosion at the pillar's start (byte-identical to fx_007 entry 0xe) |
| fx_0d6 | 0x11 | `FX_SWDMAG_FIRAGA_PILLAR_RING` | render | rings: textured flame ring rising with the pillar (byte-identical to fx_007 entry 0xf) |
| fx_0d6 | 0x12 | `FX_SWDMAG_FIRAGA_EMBERS` | render | emitter: dark red ember sprites (byte-identical to fx_007 entry 0x12) |
| fx_0d6 | 0x13 | `FX_SWDMAG_FIRAGA_GOLD_SPARKS_2` | render | emitter: golden spark sprites + spark sub-particles (byte-identical to fx_007 entry 0x11) |
| fx_0d6 | 0x14 | `FX_SWDMAG_FIRAGA_GOLD_SPARKS` | render | emitter: golden spark sprites (byte-identical to fx_007 entry 0x10) |
| fx_0d6 | 0x15 | `FX_SWDMAG_FIRAGA_SMOKE_BURST` | render | emitter: dark teal-tinted burst (smoke after the blast) (byte-identical to fx_007 entry 0x13) |
| fx_0d6 | 0x16 | `FX_SWDMAG_FIRAGA_CONE_GLOW` | kind/usage (look pending) | emitter: cone, one dark particle frame (byte-identical to fx_007 entry 0x15) |
| fx_0d6 | 0x17 | `FX_SWDMAG_FIRAGA_CONE_SPARKS` | kind/usage (look pending) | emitter: cone of spark sprites (texture not in the file) (byte-identical to fx_007 entry 0x14) |
| fx_0d6 | 0x18 | `FX_SWDMAG_FIRAGA_SWORD_GLINT` | render | rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat |
| fx_0d6 | 0x19 | `FX_SWDMAG_FIRAGA_SWORD_GLINT_2` | render | rings: white gouraud ring flash at the sword, rotated |
| fx_0d6 | 0x1B | `FX_SWDMAG_FIRAGA_COLUMN_SEG_1` | kind/usage (look pending) | 'so' model: fire-column segment (byte-identical to fx_007 entry 0x17) |
| fx_0d6 | 0x1C | `FX_SWDMAG_FIRAGA_COLUMN_SEG_0` | kind/usage (look pending) | 'so' model: fire-column segment, drawn coiled 18 times (byte-identical to fx_007 entry 0x16) |
| fx_0d6 | 0x1D | `FX_SWDMAG_FIRAGA_COLUMN_SEG_2` | kind/usage (look pending) | 'so' model: fire-column segment (byte-identical to fx_007 entry 0x18) |
| fx_0d6 | 0x1E | `FX_SWDMAG_FIRAGA_COLUMN_SEG_3` | kind/usage (look pending) | 'so' model: fire-column segment (byte-identical to fx_007 entry 0x19) |
| fx_0d7 | 0x7 | `FX_SWDMAG_THUNDER_SMOKE` | render | emitter: yellow / blue smoke puffs after the strike (byte-identical to fx_003 entry 0xa) |
| fx_0d7 | 0x8 | `FX_SWDMAG_THUNDER_IMPACT_RING` | render | rings: cyan gouraud rings at the strike (byte-identical to fx_003 entry 0xb) |
| fx_0d7 | 0x9 | `FX_SWDMAG_THUNDER_IMPACT_FLASH` | render | emitter: four-point star flash, white rings, sparks (byte-identical to fx_003 entry 0xc) |
| fx_0d7 | 0xF | `FX_SWDMAG_THUNDER_SWORD_GLINT` | render | rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat |
| fx_0d7 | 0x10 | `FX_SWDMAG_THUNDER_SWORD_GLINT_2` | render | rings: white gouraud ring flash at the sword, rotated |
| fx_0d8 | 0x2 | `FX_SWDMAG_THUNDARA_IMPACT_FLASH` | render | emitter: four-point star flash, white rings, sparks (byte-identical to fx_005 entry 0x17) |
| fx_0d8 | 0x14 | `FX_SWDMAG_THUNDARA_ORB_EMITTER` | render | emitter: helix whose callback element draws the orb (0x19-0x1D here) (byte-identical to fx_005 entry 0x1a) |
| fx_0d8 | 0x17 | `FX_SWDMAG_THUNDARA_IMPACT_RING` | render | emitter: white-cyan gouraud rings (byte-identical to fx_005 entry 0x19) |
| fx_0d8 | 0x18 | `FX_SWDMAG_THUNDARA_SPARKS` | kind/usage (look pending) | emitter: spark sprites (texture not in the file) (byte-identical to fx_005 entry 0x7) |
| fx_0d8 | 0x19 | `FX_SWDMAG_THUNDARA_ORB_FADE` | render | particles: blue energy sphere fading to orange (FX_THUNDARA_ORB_FADE's draw slot in fx_005) |
| fx_0d8 | 0x1A | `FX_SWDMAG_THUNDARA_ORB` | render | particles: bright blue energy sphere (byte-identical to fx_005 entry 0x1d) |
| fx_0d8 | 0x1B | `FX_SWDMAG_THUNDARA_ORB_SPARKS` | kind/usage (look pending) | sprites: sparks around the orb (texture not in the file) (byte-identical to fx_005 entry 0x1e) |
| fx_0d8 | 0x1C | `FX_SWDMAG_THUNDARA_ORB_SPARKS_2` | kind/usage (look pending) | sprites: sparks around the orb (texture not in the file) (byte-identical to fx_005 entry 0x1f) |
| fx_0d8 | 0x1D | `FX_SWDMAG_THUNDARA_LENS_FLARES` | render | particles: orange lens-flare dots around the orb (byte-identical to fx_005 entry 0x16) |
| fx_0d8 | 0x1E | `FX_SWDMAG_THUNDARA_SWORD_GLINT` | render | rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat |
| fx_0d8 | 0x1F | `FX_SWDMAG_THUNDARA_SWORD_GLINT_2` | render | rings: white gouraud ring flash at the sword, rotated |
