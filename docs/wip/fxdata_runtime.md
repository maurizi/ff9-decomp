# Effect data: emulator checklist for the "(look pending)" names

Every effect-data define whose comment still ends in "(look pending)" (217 defines in 115 modules, consistency pass of `docs/fxdata.md`), grouped by the battle setup that plays it, so one emulator session per group covers them. The define comment in `include/fxmod/<module>.h` has the details; `tools/docker.sh python3 tools/fxdata.py <module> --render` draws what the file alone gives.

How to play them (`docs/emulator.md`):

- Party abilities (P1-P7): one battle state with the caster in the party (e.g. `r30b_enc21_start`, disc 1 Lv 50 Zidane / Dagger / Vivi / Steiner, or the disc 3 Malboro states with Eiko / Quina / Amarant), then `build/rt3/run3.py NAME --state S --patch CMD,ABILITY[,TARGETS]` rewrites the first Attack into the ability. One-target and all-targets modules are the same ability with one or all enemies targeted.
- Short summon animations (P7) play instead of the full one when the summon is cast with the short animation setting / on a repeat cast; force the file id at btlfx_start if neither reaches it.
- Enemy skills (E*): `tools/rt/encscan.py <effect ids>` finds the encounters whose battle sequences play the file, `tools/rt/enter_battle.py ENC` enters one, and a `btl_setEnemyCmd` hook (`--fn`) forces the ability.
- Capture: `tools/emu.py` screenshot at the frame the module's op starts the entry (break on the module op, the `btlfx_getFileData(file, idx)` call returns the entry), and a VRAM dump (1024x512, 16-bit) while the effect plays; render the dumped page with the entry's tpage / CLUT to settle the look. Then rename by look and drop "(look pending)".

## Summary by battle group

| group | setup | modules | defines |
|---|---|---|---|
| P1 | Black magic (Vivi), one target and all targets | fx_005 fx_007 fx_018 fx_01a fx_038 fx_048 fx_049 fx_04e fx_05d fx_05e fx_062 fx_063 fx_078 fx_07d fx_07e fx_084 fx_085 fx_086 fx_08f | 35 |
| P2 | White magic (Garnet / Eiko) | fx_00b fx_00e fx_01b fx_02a fx_04c fx_04f fx_057 fx_05a fx_060 fx_061 fx_07f fx_08d fx_090 fx_0a2 | 29 |
| P3 | Blue magic (Quina) | fx_052 fx_081 fx_096 fx_0ec fx_138 fx_188 | 15 |
| P4 | Sword Magic (Steiner, Vivi in the party) | fx_0d6 fx_0d8 fx_0d9 fx_0dd fx_0df | 7 |
| P5 | Other party skills (Zidane Dyne, Amarant Elan, Medeo) | fx_079 fx_07a fx_0f2 fx_0f3 fx_0f4 fx_0f8 fx_179 | 10 |
| P6 | Summons, full animation (Garnet / Eiko) | fx_0b1 fx_0b8 fx_0ba fx_0d3 fx_0e1_0 fx_0e1_1 fx_0e3_0 fx_0e3_1 fx_0fb_1 fx_114 fx_17d_0 fx_17d_2 fx_17d_5 fx_17d_6 fx_17d_8 | 38 |
| P7 | Summons, short animation | fx_17a fx_19f fx_1a8 fx_1bd fx_1be fx_1bf_2 fx_1fd fx_1fe | 18 |
| E1 | enemy: Yan | fx_01f | 2 |
| E2 | enemy: Garland | fx_023 | 1 |
| E3 | enemy: Shell Dragon | fx_033 | 1 |
| E4 | enemy: Chimera | fx_034 fx_0a9 | 2 |
| E5 | enemy: Sand Scorpion / Seeker Bat | fx_037 | 1 |
| E6 | enemy: Carrion Worm | fx_03b | 2 |
| E7 | enemy: Gigan Octopus | fx_043 fx_1ad | 3 |
| E8 | enemy: Axe Beak / Carve Spider | fx_0a0 | 2 |
| E9 | enemy: Torama | fx_0a1 fx_0a3 | 4 |
| E10 | enemy: Dragonfly | fx_0a4 | 1 |
| E11 | enemy: Myconid | fx_0a5 | 1 |
| E12 | enemy: Soulcage | fx_0b0 | 4 |
| E13 | enemy: Griffin | fx_0b4 fx_1f6 | 3 |
| E14 | enemy: Thorn / Zorn | fx_0cb fx_0cc | 2 |
| E15 | enemy: Gimme Cat | fx_0cd fx_0ce | 4 |
| E16 | enemy: Whale Zombie | fx_0f0 | 1 |
| E17 | enemy: Magic Vice / Mimic | fx_112 | 1 |
| E18 | enemy: Prison Cage (Garnet) | fx_11d | 2 |
| E19 | enemy: Maliris | fx_123 | 1 |
| E20 | enemy: Benero / King Leo / Zenero | fx_125 | 2 |
| E21 | enemy: Kuja / Trance Kuja | fx_180 fx_18a fx_1b4 fx_1ec | 9 |
| E22 | enemy: Jabberwock | fx_18c | 1 |
| E23 | enemy: Moonlight Slash (enemy not labelled) | fx_1a2 | 3 |
| E24 | enemy: Kraken | fx_1a6 fx_1ab | 2 |
| E25 | enemy: Nova Dragon | fx_1b2 | 2 |
| E26 | enemy: Necron | fx_1b3 fx_1f0 | 4 |
| E27 | enemy: Silver Dragon | fx_1b6 | 1 |
| E28 | enemy: Gizamaluke | fx_1b8 fx_1b9 | 2 |
| E29 | enemy: Ragtime Mouse | fx_1e6 | 1 |

## P1: Black magic (Vivi), one target and all targets

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_005 | Thundara (one target) | `FX_THUNDARA_ORB_SPARKS` | 0x1e | sprites | blank page in the file's upload, filled at run time | VRAM dump of page (0x1C0, 0x100) while the effect plays |
| fx_007 | Firaga (one target) | `FX_FIRAGA_CONE_GLOW` | 0x15 | emitter | look decided by the blend mode | screenshot at the frame the op starts the entry |
| fx_018 | Thundaga (one target) | `FX_THUNDAGA_CYL_EMITTER` | 0x7 | emitter | renders fully transparent | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_018`) |
| fx_018 | Thundaga (one target) | `FX_THUNDAGA_CYL_EMITTER_2` | 0xa | emitter | renders fully transparent | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_018`) |
| fx_018 | Thundaga (one target) | `FX_THUNDAGA_DISC` | 0x10 | model | 15-bit page 0x119 drawn at run time (screen copy?) | VRAM dump of (0x240, 0x100) while the effect plays + screenshot at the frame the op starts the entry |
| fx_018 | Thundaga (one target) | `FX_THUNDAGA_CONE_EMITTER` | 0x26 | emitter | renders fully transparent | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_018`) |
| fx_01a | Sleep (one target) | `FX_SLEEP_BACKDROP` | 0x1 | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_01a | Sleep (one target) | `FX_SLEEP_SPECK_RING` | 0x3 | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_01a`) |
| fx_038 | Comet | `FX_COMET_BACKDROP` | 0xc | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_048 | Firaga (all targets) | `FX_FIRAGA_CONE_GLOW` | 0x18 | emitter | look decided by the blend mode | screenshot at the frame the op starts the entry |
| fx_049 | Thundara (all targets) | `FX_THUNDARA_ORB_SPARKS` | 0x1b | sprites | blank page in the file's upload, filled at run time | VRAM dump of page (0x1C0, 0x100) while the effect plays |
| fx_04e | Sleep (all targets) | `FX_SLEEP_SPECK_RING` | 0x7 | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_04e`) |
| fx_04e | Sleep (all targets) | `FX_SLEEP_BACKDROP` | 0x9 | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_05d | Bio (one target) | `FX_BIO_SCREEN_TRAIL` | 0x11 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_05d`) |
| fx_05e | Death | `FX_DEATH_BACKDROP` | 0xb | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_062 | Demi (one target) | `FX_DEMI_CAP` | 0xa | model | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_062`) |
| fx_063 | Demi (all targets) | `FX_DEMI_CAP` | 0x9 | model | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_063`) |
| fx_078 | Bio (all targets) | `FX_BIO_ORBIT_TRAIL` | 0x16 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_078`) |
| fx_07d | Flare | `FX_FLARE_CALLBACK_EMITTER` | 0x12 | emitter | drawn by the module's callback, nothing drawable in the data | screenshot at the frame the op starts the entry |
| fx_07d | Flare | `FX_FLARE_TEX_RING` | 0x18 | rings | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_07d`) |
| fx_07d | Flare | `FX_FLARE_TEX_RING_2` | 0x19 | rings | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_07d`) |
| fx_07e | Slow | `FX_SLOW_GLINT` | 0x1 | particles | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_07e`) |
| fx_07e | Slow | `FX_SLOW_PARTICLES` | 0x5 | particles | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_07e`) |
| fx_084 | Drain | `FX_DRAIN_SPRITES` | 0x1 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_084`) |
| fx_085 | Thundaga (all targets) | `FX_THUNDAGA_CONE_EMITTER` | 0x8 | emitter | renders fully transparent | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_085`) |
| fx_085 | Thundaga (all targets) | `FX_THUNDAGA_DISC` | 0xf | model | 15-bit page 0x119 drawn at run time (screen copy?) | VRAM dump of (0x240, 0x100) while the effect plays + screenshot at the frame the op starts the entry |
| fx_085 | Thundaga (all targets) | `FX_THUNDAGA_CYL_EMITTER` | 0x1b | emitter | renders fully transparent | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_085`) |
| fx_085 | Thundaga (all targets) | `FX_THUNDAGA_CYL_EMITTER_2` | 0x1c | emitter | renders fully transparent | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_085`) |
| fx_086 | Meteor | `FX_METEOR_STREAK_SHELL` | 0x4 | model | placeholder page in the file (filled or replaced at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_086`); screenshot at the frame the op starts the entry |
| fx_086 | Meteor | `FX_METEOR_SPRITES` | 0x6 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_086`) |
| fx_086 | Meteor | `FX_METEOR_CONE_SPRITES` | 0xb | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_086`) |
| fx_086 | Meteor | `FX_METEOR_DEBRIS_SPRITES` | 0x13 | sprites | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_086`) |
| fx_08f | Meteor (alt) | `FX_METEOR_STREAK_SHELL` | 0x4 | model | placeholder page in the file (filled or replaced at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_08f`); screenshot at the frame the op starts the entry |
| fx_08f | Meteor (alt) | `FX_METEOR_CONE_SPRITES` | 0xb | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_08f`) |
| fx_08f | Meteor (alt) | `FX_METEOR_DEBRIS_SPRITES` | 0x13 | sprites | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_08f`) |

## P2: White magic (Garnet / Eiko)

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_00b | Esuna | `FX_ESUNA_BACKDROP` | 0x7 | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_00e | Berserk (one target) | `FX_BERSERK_BACKDROP` | 0x3 | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_01b | Float (one target) | `FX_FLOAT_SPECK_RING` | 0x1 | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_01b`) |
| fx_01b | Float (one target) | `FX_FLOAT_GLOW` | 0x3 | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_01b`) |
| fx_01b | Float (one target) | `FX_FLOAT_BACKDROP` | 0x4 | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_02a | Silence (one target) | `FX_SILENCE_BACKDROP` | 0x4 | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_04c | Float (all targets) | `FX_FLOAT_GLOW_2` | 0x1 | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_04c`) |
| fx_04c | Float (all targets) | `FX_FLOAT_GLOW` | 0x2 | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_04c`) |
| fx_04c | Float (all targets) | `FX_FLOAT_BACKDROP` | 0x5 | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_04f | Reflect | `FX_REFLECT_BACKDROP` | 0xa | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_04f | Reflect | `FX_REFLECT_BACKDROP_2` | 0xd | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_057 | Holy | `FX_HOLY_BACKDROP_3` | 0x3 | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_057 | Holy | `FX_HOLY_TRAIL` | 0x6 | trail drawn by the emitter callback | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_057`) |
| fx_057 | Holy | `FX_HOLY_SPRITE_LINE` | 0x17 | sprite line | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_057`) |
| fx_057 | Holy | `FX_HOLY_BACKDROP` | 0x18 | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_057 | Holy | `FX_HOLY_SPARK` | 0x1d | particles | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_057`) |
| fx_057 | Holy | `FX_HOLY_TEXTURED_RING` | 0x1e | rings | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_057`) |
| fx_057 | Holy | `FX_HOLY_BACKDROP_2` | 0x1f | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_05a | Might | `FX_MIGHT_BACKDROP` | 0x7 | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_060 | Confuse (one target) | `FX_CONFUSE_WAVE_TRAILS` | 0x1 | emitter | placeholder page in the file (filled or replaced at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_060`); screenshot at the frame the op starts the entry |
| fx_060 | Confuse (one target) | `FX_CONFUSE_ORBIT_TRAILS` | 0x9 | emitter | placeholder page in the file (filled or replaced at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_060`); screenshot at the frame the op starts the entry |
| fx_061 | Confuse (all targets) | `FX_CONFUSE_ORBIT_TRAILS` | 0x2 | emitter | placeholder page in the file (filled or replaced at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_061`); screenshot at the frame the op starts the entry |
| fx_061 | Confuse (all targets) | `FX_CONFUSE_WAVE_TRAILS` | 0x7 | emitter | placeholder page in the file (filled or replaced at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_061`); screenshot at the frame the op starts the entry |
| fx_07f | Silence (all targets) | `FX_SILENCE_RING_DISC` | 0x1 | model | placeholder page in the file (filled or replaced at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_07f`); screenshot at the frame the op starts the entry |
| fx_08d | Cura (one target) | `FX_CURA_FAN_EMITTER` | 0xa | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_08d`) |
| fx_090 | Curaga (all targets) | `FX_CURAGA_SPARKLES` | 0x3 | emitter | sprite texture not resolved (drawn as flat squares) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_090`); screenshot at the frame the op starts the entry |
| fx_090 | Curaga (all targets) | `FX_CURAGA_SPARKLE_BURST` | 0xf | emitter | sprite texture not resolved (drawn as flat squares) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_090`); screenshot at the frame the op starts the entry |
| fx_090 | Curaga (all targets) | `FX_CURAGA_SPARKLE_SHOWER` | 0x10 | emitter | sprite texture not resolved (drawn as flat squares) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_090`); screenshot at the frame the op starts the entry |
| fx_0a2 | Cura (all targets) | `FX_CURA_SPARKLE_BURST` | 0x1 | emitter | sprite texture not resolved (drawn as flat squares) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0a2`); screenshot at the frame the op starts the entry |

## P3: Blue magic (Quina)

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_052 | Limit Glove | `FX_LIMIT_GLOVE_SPHERE` | 0x7 | 'so' model | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_052`) |
| fx_052 | Limit Glove | `FX_LIMIT_GLOVE_BACKDROP` | 0xc | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |
| fx_081 | LV4 Holy | `FX_LV4_HOLY_SPRITE_LINE` | 0xb | sprite line | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_081`) |
| fx_081 | LV4 Holy | `FX_LV4_HOLY_TRAIL` | 0x12 | trail | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_081`) |
| fx_081 | LV4 Holy | `FX_LV4_HOLY_TEXTURED_RING` | 0x23 | rings | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_081`) |
| fx_081 | LV4 Holy | `FX_LV4_HOLY_SPARK` | 0x25 | particles | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_081`) |
| fx_096 | Frost | `FX_FROST_ICE_GLINTS` | 0x1 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_096`) |
| fx_0ec | Doom | `FX_DOOM_PARTICLES` | 0x6 | particles | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0ec`) |
| fx_0ec | Doom | `FX_DOOM_GLOW_SHELL` | 0xd | model | tpage computed by the C (not resolved by the tool) | breakpoint on the model registration (tpage argument) + screenshot at the frame the op starts the entry |
| fx_0ec | Doom | `FX_DOOM_WHITE_SHELL` | 0x10 | model | 15-bit page 0x119 drawn at run time (screen copy?) | VRAM dump of (0x240, 0x100) while the effect plays + screenshot at the frame the op starts the entry |
| fx_0ec | Doom | `FX_DOOM_GOURAUD_SHELL` | 0x13 | model | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0ec`) |
| fx_138 | Lucky Seven | `FX_LUCKY_SEVEN_SPRITE_LINE` | 0xe | sprite line | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_138`) |
| fx_138 | Lucky Seven | `FX_LUCKY_SEVEN_SLOT_STRIPS` | 0x1b | model | texture page not uploaded by the file or the common / summon pages | VRAM dump of page 0x9A = (0x280, 0x100) while the effect plays |
| fx_188 | LV3 Def-less | `FX_LV3_DEF_LESS_PARTICLE` | 0x3 | particles | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_188`) |
| fx_188 | LV3 Def-less | `FX_LV3_DEF_LESS_RING` | 0x14 | rings | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_188`) |

## P4: Sword Magic (Steiner, Vivi in the party)

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_0d6 | Sword Magic Firaga | `FX_SWDMAG_FIRAGA_CONE_GLOW` | 0x16 | emitter | look decided by the blend mode | screenshot at the frame the op starts the entry |
| fx_0d8 | Sword Magic Thundara | `FX_SWDMAG_THUNDARA_ORB_SPARKS` | 0x1b | sprites | blank page in the file's upload, filled at run time | VRAM dump of page (0x1C0, 0x100) while the effect plays |
| fx_0d9 | Sword Magic Thundaga | `FX_SWDMAG_THUNDAGA_DISC` | 0xd | model | 15-bit page 0x119 drawn at run time (screen copy?) | VRAM dump of (0x240, 0x100) while the effect plays + screenshot at the frame the op starts the entry |
| fx_0d9 | Sword Magic Thundaga | `FX_SWDMAG_THUNDAGA_CONE_EMITTER` | 0x26 | emitter | renders fully transparent | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0d9`) |
| fx_0dd | Sword Magic Bio | `FX_SWDMAG_BIO_SCREEN_TRAIL` | 0x11 | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0dd`) |
| fx_0df | Sword Magic Flare | `FX_SWDMAG_FLARE_TEX_RING` | 0x19 | rings | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0df`) |
| fx_0df | Sword Magic Flare | `FX_SWDMAG_FLARE_TEX_RING_2` | 0x1a | rings | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0df`) |

## P5: Other party skills (Zidane Dyne, Amarant Elan, Medeo)

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_079 | Demi Shock | `FX_DEMI_SHOCK_TRAIL` | 0x9 | trail | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_079`) |
| fx_079 | Demi Shock | `FX_DEMI_SHOCK_SPHERE` | 0xb | model | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_079`) |
| fx_07a | Demi Shock (Elan) | `FX_DEMI_SHOCK_SPHERE` | 0xb | model | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_07a`) |
| fx_07a | Demi Shock (Elan) | `FX_DEMI_SHOCK_TRAIL` | 0xc | trail | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_07a`) |
| fx_0f2 | Tidal Flame | `FX_TIDAL_FLAME_DOME` | 0x2 | model | texture page not uploaded by the file or the common / summon pages | VRAM dump of page 0x11A = (0x280, 0x100) while the effect plays |
| fx_0f3 | Scoop Art | `FX_SCOOP_ART_SCREEN_TRAIL` | 0x8 | emitter | placeholder page in the file (filled or replaced at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0f3`); screenshot at the frame the op starts the entry |
| fx_0f4 | Shift Break | `FX_SHIFT_BREAK_CONE_RINGS` | 0xc | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0f4`) |
| fx_0f8 | Grand Lethal | `FX_GRAND_LETHAL_SPRITE_RINGS` | 0xd | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0f8`) |
| fx_0f8 | Grand Lethal | `FX_GRAND_LETHAL_SWIRL_RINGS` | 0xe | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0f8`) |
| fx_179 | Medeo | `FX_MEDEO_SPRITE_BURST` | 0xb | emitter | renders fully transparent | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_179`) |

## P6: Summons, full animation (Garnet / Eiko)

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_0b1 | Carbuncle | `FX_CARBUNCLE_ORBIT_GLOW` | 0xa | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0b1`) |
| fx_0b1 | Carbuncle | `FX_CARBUNCLE_ORBIT_GLOW_2` | 0x16 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0b1`) |
| fx_0b1 | Carbuncle | `FX_CARBUNCLE_ORBIT_GLOW_3` | 0x24 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0b1`) |
| fx_0b8 | Atomos | `FX_ATOMOS_SPRITES` | 0x2 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0b8`) |
| fx_0b8 | Atomos | `FX_ATOMOS_SPRITES_2` | 0x8 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0b8`) |
| fx_0b8 | Atomos | `FX_ATOMOS_FAINT_GLOW` | 0xb | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0b8`) |
| fx_0b8 | Atomos | `FX_ATOMOS_SPRITES_3` | 0x22 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0b8`) |
| fx_0ba | Ramuh | `FX_RAMUH_SPARK_PARTICLE` | 0x2c | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0ba`) |
| fx_0d3 | Phoenix | `FX_PHOENIX_FAN_EMITTER_2` | 0x5 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0d3`) |
| fx_0d3 | Phoenix | `FX_PHOENIX_SPRITE_LINE` | 0x7 | sprite line | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0d3`) |
| fx_0d3 | Phoenix | `FX_PHOENIX_FAN_EMITTER_5` | 0x14 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0d3`) |
| fx_0e1_0 | Rebirth Flame [group 0] | `FX_REBIRTH_FLAME_MORPH_2` | 0x80 | summon data | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0e1_0`) |
| fx_0e1_0 | Rebirth Flame [group 0] | `FX_REBIRTH_FLAME_MORPH_1` | 0x81 | summon data | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0e1_0`) |
| fx_0e1_0 | Rebirth Flame [group 0] | `FX_REBIRTH_FLAME_MORPH_0` | 0x82 | summon data | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0e1_0`) |
| fx_0e1_1 | Rebirth Flame [group 1] | `FX_REBIRTH_FLAME_TEX_EMITTER` | 0x1 | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0e1_1`) |
| fx_0e3_0 | Bahamut [group 0] | `FX_BAHAMUT_TEAL_PUFF` | 0x3 | particles | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0e3_0`) |
| fx_0e3_0 | Bahamut [group 0] | `FX_BAHAMUT_CORE_GRID` | 0x7 | 'so' model drawn with a fading core colo | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0e3_0`) |
| fx_0e3_0 | Bahamut [group 0] | `FX_BAHAMUT_WHITE_BOLT` | 0x8 | trail | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0e3_0`) |
| fx_0e3_0 | Bahamut [group 0] | `FX_BAHAMUT_TILE_RING` | 0xb | 'so' model drawn with a fading colour | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0e3_0`) |
| fx_0e3_0 | Bahamut [group 0] | `FX_BAHAMUT_PILLAR_STRIP` | 0x82 | summon data | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0e3_0`) |
| fx_0e3_1 | Bahamut [group 1] | `FX_BAHAMUT_FLARE_LINE` | 0x7 | sprite line | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0e3_1`) |
| fx_0e3_1 | Bahamut [group 1] | `FX_BAHAMUT_BLAST_PLATE` | 0xf | 'so' model | 15-bit page 0x119 drawn at run time (screen copy?) | VRAM dump of (0x240, 0x100) while the effect plays + screenshot at the frame the op starts the entry |
| fx_0e3_1 | Bahamut [group 1] | `FX_BAHAMUT_WHITE_BOLT_2` | 0x11 | trail | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0e3_1`) |
| fx_0e3_1 | Bahamut [group 1] | `FX_BAHAMUT_WHITE_BOLT` | 0x15 | trail | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0e3_1`) |
| fx_0fb_1 | Madeen | `FX_MADEEN_TRAIL` | 0x14 | trail | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0fb_1`) |
| fx_114 | Ifrit | `FX_IFRIT_SPOKED_RING` | 0x12 | model | texture list not followed by the tool | VRAM dump of the listed pages while the effect plays (the pages its frames sample: `fxdata.py fx_114`); screenshot at the frame the op starts the entry |
| fx_114 | Ifrit | `FX_IFRIT_BOWL` | 0x1e | model | texture list not followed by the tool | VRAM dump of the listed pages while the effect plays (the pages its frames sample: `fxdata.py fx_114`); screenshot at the frame the op starts the entry |
| fx_114 | Ifrit | `FX_IFRIT_GRID_PLANE` | 0x2a | model | placeholder page in the file (filled or replaced at run time) | VRAM dump of page 0x117 = (0x1c0, 0x100) while the effect plays; screenshot at the frame the op starts the entry |
| fx_114 | Ifrit | `FX_IFRIT_CANOPY` | 0x81 | summon data entry 1 | texture list not followed by the tool | VRAM dump of the listed pages while the effect plays (the pages its frames sample: `fxdata.py fx_114`); screenshot at the frame the op starts the entry |
| fx_114 | Ifrit | `FX_IFRIT_CYLINDER` | 0x83 | summon data entry 3 | placeholder page in the file (filled or replaced at run time) | VRAM dump of page 0x117 = (0x1c0, 0x100) while the effect plays; screenshot at the frame the op starts the entry |
| fx_17d_0 | Ark [group 0] | `FX_ARK_NOISE_SPHERE` | 0x1c | 'so' model registered at the start of op | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_17d_0`) |
| fx_17d_2 | Ark [group 2] | `FX_ARK_THIN_ARCH` | 0x2 | 'so' model | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_17d_2`) |
| fx_17d_5 | Ark [group 5] | `FX_ARK_TRAIL` | 0x5 | trail | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_17d_5`) |
| fx_17d_5 | Ark [group 5] | `FX_ARK_TRAIL_2` | 0x9 | trail | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_17d_5`) |
| fx_17d_5 | Ark [group 5] | `FX_ARK_SPRITE_LINE` | 0xd | sprite line | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_17d_5`) |
| fx_17d_6 | Ark [group 6] | `FX_ARK_FAN_EMITTER_2` | 0xc | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_17d_6`) |
| fx_17d_6 | Ark [group 6] | `FX_ARK_SPRITE_LINE` | 0xd | sprite line | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_17d_6`) |
| fx_17d_8 | Ark [group 8] | `FX_ARK_FLAT_PANEL` | 0x6 | 'so' model | placeholder page in the file (filled or replaced at run time) | VRAM dump of page 0x11B = (0x2c0, 0x100) while the effect plays; screenshot at the frame the op starts the entry |

## P7: Summons, short animation

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_17a | Madeen (short) | `FX_MADEEN_TRAIL` | 0x11 | trail | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_17a`) |
| fx_19f | Judgement Bolt | `FX_RAMUH_LIGHTNING_RIBBON` | 0x6 | trail | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_19f`) |
| fx_19f | Judgement Bolt | `FX_RAMUH_LIGHTNING_RIBBON_2` | 0xf | trail | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_19f`) |
| fx_19f | Judgement Bolt | `FX_RAMUH_SPARK_BURST` | 0x16 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_19f`) |
| fx_19f | Judgement Bolt | `FX_RAMUH_LIGHTNING_BOLT` | 0x18 | trail | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_19f`) |
| fx_19f | Judgement Bolt | `FX_RAMUH_DOME_COLUMN` | 0x22 | 'so' model | placeholder page in the file (filled or replaced at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_19f`); screenshot at the frame the op starts the entry |
| fx_19f | Judgement Bolt | `FX_RAMUH_SPARK_PARTICLE` | 0x2a | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_19f`) |
| fx_1a8 | Odin (short) | `FX_ODIN_FAN_STREAK` | 0x5 | emitter | fan / streak too faint in the render to name | screenshot at the frame the op starts the entry |
| fx_1a8 | Odin (short) | `FX_ODIN_SPECK_SPIRAL` | 0xc | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1a8`) |
| fx_1a8 | Odin (short) | `FX_ODIN_SPECKS` | 0x12 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1a8`) |
| fx_1a8 | Odin (short) | `FX_ODIN_CALLBACK_EMITTER` | 0x17 | emitter | drawn by the module's callback, nothing drawable in the data | screenshot at the frame the op starts the entry |
| fx_1bd | Flames of Hell | `FX_IFRIT_GRID_PLANE` | 0xd | model | placeholder page in the file (filled or replaced at run time) | VRAM dump of page 0x117 = (0x1c0, 0x100) while the effect plays; screenshot at the frame the op starts the entry |
| fx_1bd | Flames of Hell | `FX_IFRIT_BOWL` | 0x13 | model | texture list not followed by the tool | VRAM dump of the listed pages while the effect plays (the pages its frames sample: `fxdata.py fx_1bd`); screenshot at the frame the op starts the entry |
| fx_1bd | Flames of Hell | `FX_IFRIT_SPOKED_RING` | 0x2c | model | texture list not followed by the tool | VRAM dump of the listed pages while the effect plays (the pages its frames sample: `fxdata.py fx_1bd`); screenshot at the frame the op starts the entry |
| fx_1be | Atomos (short) | `FX_ATOMOS_SPHERE` | 0xc | 'so' model | page 0x117 = (0x1c0, 0x100) holds no texture in the file (run-time page) | VRAM dump of page 0x117 = (0x1c0, 0x100) while the effect plays; screenshot at the frame the op starts the entry |
| fx_1bf_2 | Ark (short) | `FX_ARK_FLAT_PANEL` | 0x2 | 'so' model | placeholder page in the file (filled or replaced at run time) | VRAM dump of page 0x11B = (0x2c0, 0x100) while the effect plays; screenshot at the frame the op starts the entry |
| fx_1fd | Fenrir (2nd) (short) | `FX_FENRIR_SPARKS` | 0x16 | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1fd`) |
| fx_1fe | Phoenix (short) | `FX_PHOENIX_EMBERS` | 0x16 | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1fe`) |

## E1: enemy Yan

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_01f | enemy !!! (Yan) | `FX_01F_WHITE_RINGS` | 0x3 | emitter | placeholder page in the file (filled or replaced at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_01f`); screenshot at the frame the op starts the entry |
| fx_01f | enemy !!! (Yan) | `FX_01F_BACKDROP` | 0xd | backdrop | backdrop layer table, not drawn by the tool | screenshot of the backdrop / screen tint while the op draws it |

## E2: enemy Garland

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_023 | enemy Psychokinesis (Garland) | `FX_PSYCHOKINESIS_SCREEN_TRAILS` | 0x8 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_023`) |

## E3: enemy Shell Dragon

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_033 | enemy Snort (Shell Dragon, ...) | `FX_SNORT_BREATH_TRAILS` | 0x5 | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_033`) |

## E4: enemy Chimera

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_034 | enemy Cold Breath (Chimera, ...) | `FX_COLD_BREATH_ICE_CRYSTAL_2` | 0xa | model | texture page not uploaded by the file or the common / summon pages | VRAM dump of page 0x9A = (0x280, 0x100) while the effect plays |
| fx_0a9 | enemy Lightning (Chimera, ...) | `FX_LIGHTNING_TRAIL_2` | 0x4 | trail | not drawn by the tool | screenshot at the frame the op starts the entry |

## E5: enemy Sand Scorpion / Seeker Bat

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_037 | enemy Darkness (Sand Scorpion/Seeker Bat, ...) | `FX_DARKNESS_SCREEN_TRAIL` | 0x2 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_037`) |

## E6: enemy Carrion Worm

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_03b | enemy Trouble Juice (Carrion Worm) | `FX_TROUBLE_JUICE_SCREEN_TRAIL` | 0x3 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_03b`) |
| fx_03b | enemy Trouble Juice (Carrion Worm) | `FX_TROUBLE_JUICE_SCREEN_TRAIL_2` | 0xa | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_03b`) |

## E7: enemy Gigan Octopus

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_043 | enemy Ink (Gigan Octopus, ...) | `FX_INK_INK_TRAIL` | 0x1 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_043`) |
| fx_043 | enemy Ink (Gigan Octopus, ...) | `FX_INK_INK_SPRAY` | 0x2 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_043`) |
| fx_1ad | enemy 6 Legs (Gigan Octopus) | `FX_6_LEGS_POD` | 0x2 | 'so' model | page 0x117 = (0x1c0, 0x100) holds no texture in the file (run-time page) | VRAM dump of page 0x117 = (0x1c0, 0x100) while the effect plays; screenshot at the frame the op starts the entry |

## E8: enemy Axe Beak / Carve Spider

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_0a0 | enemy Web, String (Axe Beak/Carve Spider, ...) | `FX_WEB_CALLBACK_EMITTER` | 0x1 | emitter | drawn by the module's callback, nothing drawable in the data | screenshot at the frame the op starts the entry |
| fx_0a0 | enemy Web, String (Axe Beak/Carve Spider, ...) | `FX_WEB_STRAND_TRAIL` | 0x7 | trail | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0a0`) |

## E9: enemy Torama

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_0a1 | enemy Blaster (Torama, ...) | `FX_BLASTER_TRAIL` | 0x7 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0a1`) |
| fx_0a1 | enemy Blaster (Torama, ...) | `FX_BLASTER_TRAIL_2` | 0x10 | trail | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0a1`) |
| fx_0a3 | enemy Electrocute (Torama, ...) | `FX_ELECTROCUTE_BOLT_TRAIL` | 0x1 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0a3`) |
| fx_0a3 | enemy Electrocute (Torama, ...) | `FX_ELECTROCUTE_BOLT_TRAIL_2` | 0x2 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0a3`) |

## E10: enemy Dragonfly

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_0a4 | enemy Buzz (Dragonfly, ...) | `FX_BUZZ_SPRITE` | 0x1 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0a4`) |

## E11: enemy Myconid

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_0a5 | enemy Spore (Myconid) | `FX_SPORE_TRAIL` | 0x7 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0a5`) |

## E12: enemy Soulcage

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_0b0 | enemy Leaf Swirl (Soulcage) | `FX_LEAF_SWIRL_LEAF_TRAIL` | 0x1 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0b0`) |
| fx_0b0 | enemy Leaf Swirl (Soulcage) | `FX_LEAF_SWIRL_LEAF_TRAIL_2` | 0x2 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0b0`) |
| fx_0b0 | enemy Leaf Swirl (Soulcage) | `FX_LEAF_SWIRL_LEAF_TRAIL_3` | 0xa | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0b0`) |
| fx_0b0 | enemy Leaf Swirl (Soulcage) | `FX_LEAF_SWIRL_LEAF_TRAIL_4` | 0xb | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0b0`) |

## E13: enemy Griffin

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_0b4 | enemy Aero (Griffin, ...) | `FX_AERO_SPRITE_SWIRL` | 0x11 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0b4`) |
| fx_1f6 | enemy Aera (Griffin, ...) | `FX_AERA_WIND_STREAKS` | 0x8 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1f6`) |
| fx_1f6 | enemy Aera (Griffin, ...) | `FX_AERA_SPRITE_BURST` | 0xc | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1f6`) |

## E14: enemy Thorn / Zorn

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_0cb | enemy Light Flare (Thorn/Zorn) | `FX_LIGHT_FLARE_TEXTURED_RING` | 0x11 | rings | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0cb`) |
| fx_0cc | enemy Meteorite (Thorn/Zorn) | `FX_METEORITE_TRAIL_EMITTER` | 0x9 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0cc`) |

## E15: enemy Gimme Cat

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_0cd | enemy Aera (Gimme Cat, ...) | `FX_AERA_WIND_STREAKS` | 0x8 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0cd`) |
| fx_0cd | enemy Aera (Gimme Cat, ...) | `FX_AERA_SPRITE_BURST` | 0xc | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0cd`) |
| fx_0ce | enemy Screech, Shriek (Gimme Cat, ...) | `FX_SCREECH_TRAIL` | 0x1 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0ce`) |
| fx_0ce | enemy Screech, Shriek (Gimme Cat, ...) | `FX_SCREECH_DARK_RINGS` | 0xb | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0ce`) |

## E16: enemy Whale Zombie

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_0f0 | enemy Zombie Powder (Whale Zombie) | `FX_ZOMBIE_POWDER_ANIM_EMITTER` | 0x1 | emitter | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_0f0`) |

## E17: enemy Magic Vice / Mimic

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_112 | enemy Eat (Magic Vice/Mimic) | `FX_EAT_ORBIT_TRAIL` | 0x2 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_112`) |

## E18: enemy Prison Cage (Garnet)

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_11d | enemy Absorb (Garnet/Prison Cage) | `FX_ABSORB_TRAIL` | 0x9 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_11d`) |
| fx_11d | enemy Absorb (Garnet/Prison Cage) | `FX_ABSORB_TRAIL_2` | 0xa | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_11d`) |

## E19: enemy Maliris

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_123 | enemy Raining Swords (Maliris) | `FX_RAINING_SWORDS_DARK_BURST` | 0xc | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_123`) |

## E20: enemy Benero / King Leo / Zenero

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_125 | Poly | `FX_POLY_POLYHEDRON` | 0xd | model | texture page not uploaded by the file or the common / summon pages | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_125`) |
| fx_125 | Poly | `FX_POLY_POLYHEDRON_2` | 0xe | model | tpage computed by the C (not resolved by the tool) | breakpoint on the model registration (tpage argument) + screenshot at the frame the op starts the entry |

## E21: enemy Kuja / Trance Kuja

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_180 | enemy Ultima (Trance Kuja) | `FX_ULTIMA_NOISE_RING` | 0x2 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_180`) |
| fx_180 | enemy Ultima (Trance Kuja) | `FX_ULTIMA_NOISE_RING_2` | 0xf | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_180`) |
| fx_180 | enemy Ultima (Trance Kuja) | `FX_ULTIMA_NOISE_RING_3` | 0x1f | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_180`) |
| fx_18a | enemy Transform (Kuja/Trance Kuja) | `FX_TRANSFORM_FAN_STREAK` | 0xa | emitter | fan / streak too faint in the render to name | screenshot at the frame the op starts the entry |
| fx_18a | enemy Transform (Kuja/Trance Kuja) | `FX_TRANSFORM_SPRITE_SPIRAL` | 0xc | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_18a`) |
| fx_1b4 | enemy Flare Star (Kuja/Trance Kuja, ...) | `FX_FLARE_STAR_FAN_EMITTER` | 0xb | emitter | fan / streak too faint in the render to name | screenshot at the frame the op starts the entry |
| fx_1ec | enemy Ultima (Kuja/Trance Kuja) | `FX_ULTIMA_NOISE_RING` | 0x2 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1ec`) |
| fx_1ec | enemy Ultima (Kuja/Trance Kuja) | `FX_ULTIMA_NOISE_RING_2` | 0x12 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1ec`) |
| fx_1ec | enemy Ultima (Kuja/Trance Kuja) | `FX_ULTIMA_NOISE_RING_3` | 0x27 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1ec`) |

## E22: enemy Jabberwock

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_18c | enemy Light (Jabberwock) | `FX_LIGHT_PARTICLES` | 0x1 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_18c`) |

## E23: enemy Moonlight Slash (enemy not labelled)

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_1a2 | enemy Moonlight Slash | `FX_MOONLIGHT_SLASH_SPIKED_SHAPE` | 0x6 | model | texture list not followed by the tool | VRAM dump of the listed pages while the effect plays (the pages its frames sample: `fxdata.py fx_1a2`); screenshot at the frame the op starts the entry |
| fx_1a2 | enemy Moonlight Slash | `FX_MOONLIGHT_SLASH_SPRITES` | 0x8 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1a2`) |
| fx_1a2 | enemy Moonlight Slash | `FX_MOONLIGHT_SLASH_SPRITES_2` | 0x17 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1a2`) |

## E24: enemy Kraken

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_1a6 | enemy Water-gun (Kraken, ...) | `FX_WATER_GUN_PARTICLE` | 0x3 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1a6`) |
| fx_1ab | enemy Waterga (Kraken, ...) | `FX_WATERGA_RINGS` | 0x4 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1ab`) |

## E25: enemy Nova Dragon

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_1b2 | enemy Tidal Wave (Nova Dragon) | `FX_TIDAL_WAVE_CURL` | 0x2 | model | page 0x117 = (0x1c0, 0x100) holds no texture in the file (run-time page) | VRAM dump of page 0x117 = (0x1c0, 0x100) while the effect plays; screenshot at the frame the op starts the entry |
| fx_1b2 | enemy Tidal Wave (Nova Dragon) | `FX_TIDAL_WAVE_DISC` | 0xc | model | 15-bit page 0x119 drawn at run time (screen copy?) | VRAM dump of (0x240, 0x100) while the effect plays + screenshot at the frame the op starts the entry |

## E26: enemy Necron

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_1b3 | enemy Death1 (Dummy/Necron) | `FX_DEATH1_FAN_EMITTER` | 0x6 | emitter | fan / streak too faint in the render to name | screenshot at the frame the op starts the entry |
| fx_1b3 | enemy Death1 (Dummy/Necron) | `FX_DEATH1_SCREEN_TRAIL` | 0x39 | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1b3`) |
| fx_1b3 | enemy Death1 (Dummy/Necron) | `FX_DEATH1_SCREEN_TRAIL_2` | 0x3a | emitter | UVs read unrelated texels (texture filled at run time) | VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1b3`) |
| fx_1f0 | enemy Appearance (Dummy/Necron) | `FX_APPEARANCE_SPRITES` | 0x4 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1f0`) |

## E27: enemy Silver Dragon

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_1b6 | enemy Death (Silver Dragon) | `FX_DEATH_SPIRAL_TRAIL` | 0x2 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1b6`) |

## E28: enemy Gizamaluke

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_1b8 | enemy Death (Gizamaluke) | `FX_DEATH_DARK_QUADS` | 0x1 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1b8`) |
| fx_1b9 | enemy Appearance (Gizamaluke) | `FX_APPEARANCE_DARK_QUADS` | 0x2 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1b9`) |

## E29: enemy Ragtime Mouse

| module | game name | define | index | kind | why pending | capture |
|---|---|---|---|---|---|---|
| fx_1e6 | enemy Correct!!! (False/Ragtime Mouse/True) | `FX_CORRECT_SPARKLES` | 0x4 | emitter | renders black / empty (texels blank in the upload or a blend mode) | screenshot at the frame the op starts the entry; VRAM dump while the effect plays (the pages its frames sample: `fxdata.py fx_1e6`) |

## Oddities in the files (placeholder and test data)

Texture pages and entries the files ship with that are not the final look. The emulator pass should check whether
the game overwrites them at run time (VRAM dump while the effect plays) or really draws them.

| where | what the file holds | entries that sample it |
|---|---|---|
| fx_07f (Silence, all targets) entry 0x1; fx_086 / fx_08f (Meteor) entry 0x4 | a white page with hand-written Japanese developer scribbles | `FX_SILENCE_RING_DISC`, `FX_METEOR_STREAK_SHELL` |
| fx_17d_8 (Ark, group 8) entry 0x6, fx_1bf_2 (Ark short, group 2) entry 0x2 (byte-identical) | page 0x11B: a white page with the kana labels up / left / right / down around its edges (an orientation test texture) | `FX_ARK_FLAT_PANEL` |
| fx_060 / fx_061 (Confuse) trail strips | a page of music staves with notes, smoke swirls and a hazy wave band: the real Confuse texture or a placeholder | `FX_CONFUSE_WAVE_TRAILS`, `FX_CONFUSE_ORBIT_TRAILS` |
| fx_133 (enemy Disappear / Close Book): file 0x1B5's entry 0x1 | a test emitter: coloured puffs and sprites spelling "Dummy" (file 0x133's entry 0x1 is the real pale dust puff) | `FX_DISAPPEAR_DUST_PUFF` (the module plays both files) |
| fx_0fb_1 / fx_17a (Madeen) page 0x138 | a hand-written placeholder in the file | `FX_MADEEN_LOOP_RIBBON` (named by shape) |
| fx_19f (Ramuh short) | texture pages holding a "dummy" placeholder | `FX_RAMUH_DOME_COLUMN` |
| fx_04f (Reflect) entry 0x6 | a sketch-like placeholder page (the byte-identical Carbuncle star crystal samples a white page) | `FX_REFLECT_STAR` (named by shape) |
| fx_114 / fx_1bd (Ifrit) tpage 0x117 | a yellow placeholder page in the file's upload | `FX_IFRIT_GRID_PLANE`, `FX_IFRIT_CYLINDER` |
| fx_01f (enemy !!!, Yan) entry 0x3 | its textured rings' UV box reads a placeholder page | `FX_01F_WHITE_RINGS` |
