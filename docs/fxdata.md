# Battle effect data (FF9.IMG dir 13): what the btlfx_getFileData indices are

Every effect module reads its file's data entries with `btlfx_getFileData(entry->file, N)` (ovl_0b
`btlfx_pool.c`: `base + base[N]` over the data chunk's offset table). This page gives the naming scheme for
those indices, how `tools/fxdata.py` finds out what each entry is, and the pilot (Fire, Fira, Firaga, Blizzard,
Blizzara, Blizzaga, Thunder, Thundara, Thundaga, Cure: 20 modules, every getFileData literal in them named).

## Naming scheme

- `#define FX_<GAME NAME>_<WHAT> <index>` in the module's own header (`include/fxmod/fx_XXX.h`), used as
  `getFileData(entry->file, FX_BLIZZARA_ICE_CRYSTAL)`. Modules share addresses but not headers, so the
  one-target and all-targets modules of a spell both use the spell's prefix with their own index values.
- `<GAME NAME>`: the player-facing name of what plays the file (ability / spell / summon / item;
  `docs/effect-modules.md` effect column, `tools/effect_labels.py`); for enemy-only effects the enemy skill name
  from the game text (`tools/ff9text.py`).
- `<WHAT>`: what the entry looks like in the render, in plain words: `FLAME_BURST`, `ICE_CRYSTAL`,
  `LIGHTNING_BOLT`, `HEAT_RING`, `SMOKE`. Repeats get `_2`, `_3`; parts of one object `_PIECE_0..2` /
  `_SEG_0..3`. Where the render does not settle it, the name says the kind and what the C does with it
  (`FX_ARK_FAN_EMITTER_2`, `FX_FLARE_CALLBACK_EMITTER`) and the define comment ends with `(look pending)`;
  those are listed under open questions for an emulator look.
- The define comment says the kind and what the render shows; names carried to the all-targets module say
  where from ("byte-identical to fx_006 entry 0x3", "same call site as ...").
- Only indices the C uses get a define. Script-only entries (sounds, cameras: the effect script names them,
  not the C) and entries indexed through data tables (`fx_003_strip16Blocks`, `fx_097_pieceBlocks`) have no
  literal to replace; the tool lists them.
- Shared data: indices 0x40+ (ring start) mean the current file's own data in every file but 0xFB (the only
  flagged chunk with common sectors); 0x80+ are the summon data (`g_btlfxLoader.summon`); no pilot module
  uses either, so `include/btlfx_mod.h` has no shared names yet.

## tools/fxdata.py

`tools/docker.sh python3 tools/fxdata.py fx_004 [fx_04d ...] [--render] [--md OUT] [--json OUT]` (module names
or dir-13 file ids; stdlib only, PNG written with zlib). Per file it:

1. parses the chunk directory (header sector) and replays the texture chunks into a 1024x512 VRAM: type 0 =
   `{next, imgs, nimgs}` + images (CLUTs and small textures) then 64x128 pages to the rects at `next`, type 1 =
   more pages (btlfx_uploadTextures' rect walk), on top of the common effect pages every battle keeps
   (dir 1 file 0x2BF: four pages at (0x300, 0x100), CLUT rows at y 0xF0);
2. splits the type 2 (+10) data chunk at its offset table; entry 0 is the slot's heap. The data ring is replayed
   as btlfx_updateLoader fills it (a flagged chunk restarts it, a type 2 chunk that reaches the ring end wraps the
   cursor), so a type 10 chunk loaded after a wrap lands below its group's data, which indexes it with negative
   offsets (Ark groups 2, 4, 7, Ark short group 2);
3. classifies each entry by content (AKAO sound; `'so'` model; emitter = EffEmit [+EffSub+EffEmit] +
   EffElemHdr + EffElem[] + EffTail as btlfx_spawnEmitter walks it; BtlModelHeader with in-range offsets) and by
   use: which export the module's C passes it to (also through a local variable), and which effect-script op
   names it (0x23/0x29 camera, 0x2D sound, 0x63/0x64 party motion, control 0x0F vibration);
4. with `--render`, draws into `build/fxdata/<file>/`: every emitter element's sprite data (kinds 0-3: up to 8
   frames of 10-byte quads / 3-byte sprites with their tpage, CLUT and colour; trails: the strip texture;
   rings: the UV box or the ring colours), standalone sprite sets (rings, particles, sprites, sprite lines),
   models (front/top/side wireframe + the UV box read with the tpage/CLUT the C passes to
   Hi_RegisterTexEffModel), `vram.png`, and `sheet.png`: one labelled row per image of the entries the C
   uses (`0F E1` = entry 0xF element 1, `B` = its second draw, `M` model, `D` drawn directly). Texels
   nothing uploaded show as a magenta checker (textures from elsewhere, see open questions).

5. lists the shared data the module can index after the file's own entries, with the full index, classified and
   rendered the same way (sheet labels `S85 M`, `S9E E0`; `R` = 0x40+, `L` = 0xC0+):
   - 0x80-0xBF: `g_btlfxLoader.summon->data[0]`, which BTLFX_LDCMD_SUMMON_DATA points at the summon file's
     **type 6 chunk** (btlfx_updateLoader). A group module sees the last type 6 chunk loaded up to its group
     (Ark reloads it in group 3). Every 0x80+ literal in the module C now resolves to an entry (232 use sites,
     207 entries: 107 'so' models, 24 emitters, 24 models, 51 tables, 1 rings set).
   - 0xC0+: the battle load buffer, filled by a type 8 chunk (only file 0x197, Diamond Dust / Shiva (short):
     0xD8 / 0xD9 are 'so' models).
   - 0x40-0x7F: the ring start (first ring-start type 2 chunk, or 0xFB's extra-sector common part), listed only
     when the C uses it (no module does yet).
6. replays the summon textures too: type 4 (BTLFX_LDCMD_SUMMON_TEX: header Unk80100DC054, texCount pages at its
   tpage / y, CLUT rows at (0x100, 0xE6)) and type 9 (BTLFX_LDCMD_COMMON_TEX: mask bits 0/1 = two pages each at
   (0x300 / 0x340, 0x100), bits 2-5 one page each at (0x140 / 0x180, 0x100 / 0x180)).

Drawing: `'so'` models (Unk800E8F50: texture list of (tpage, clut) + uv offsets, a polygon's texture byte picks
one) and BtlModelHeader models are rendered textured from front, top and an oblique angle (flat-shaded, painter's
order, rest pose) next to an oblique wireframe and the UV boxes; a Hi_RegisterTexListModel call whose BtlTexInfo
and offsets are module symbols (fx_0e2's Fenrir model) reads them from `disks/us/fxmod/<module>.bin`. Calls
through a cast export (`((T)ex->Hi_RegisterTexListModel)(...)`) are recognised. Fans (element kind 7 /
btlfx_drawFan, Unk800D2938): textured = the frame's (u, v, w, h) rect tinted with the point colour; gouraud =
btlfx_emitFrameFan's three triangles in the x/z plane with the inner / outer colours, scaled per frame. Kind-3
sprite data with tpage 0 is drawn by btlfx_emitDots as 1-pixel dots (TILE_1) in the colour-table colour: these
were the "spark sprites with textures outside the file" (magenta) of the pilot.

Limits: a tpage the C computes (`tmp`) is not resolved; indices taken from module data tables
(`fx_026_op81ModelBlocks`) are not followed; models are drawn in the rest pose (bone lengths along z only).
Summon modules take about 30 s with `--render` (the textured model views).

Still not resolved by the render (not texture-source problems the file could answer):
- page (0x1C0, 0x100), 4-bit u 0x00-0x8F, v 0x00-0x0F is blank (zero) in fx_005 / fx_018's own upload, and that is where
  `FX_THUNDARA_STAR_GLINTS` and `FX_THUNDAGA_VIOLET_GLOW(_2)` sample: probably filled at run time (a VRAM
  dump while the spell plays would show it).
- `FX_THUNDAGA_DISC` reads 15-bit page 0x119 = (0x240, 0x100), which holds the file's 4-bit pages: the disc
  samples something drawn there at run time (screen copy?), not file data.

## How well appearance worked (pilot)

179 named sites in 20 modules: 143 by the render (flame bursts, smoke, ice crystals and their pieces,
reflection-map shells, lightning bolts, plasma orbs, star flashes, ring colours), 36 by kind and use (look
pending). The pending ones were emitters whose sprites read VRAM the file does not upload (magenta: spark
sprites in every Thunder spell, Firaga's cone sparks), emitters that render fully transparent (Thundaga's
cylinder/cone emitters), fan emitters and `'so'` models the tool did not draw. The fxdata d6121033 revisit
renamed 24 of the 36 by the render (the magenta sparks are 1-pixel dots, `*_SPARK_DOTS`; the fans are
Thundaga's lightning arcs; the `'so'` models are Firaga's flame ribbons); 12 stay pending. The one-target and all-targets
files mostly share entries byte for byte (Firaga, Blizzaga, Thundara, Thundaga) or run the same C with other
indices; fx_03e shows that the same call site can play different data (its "flares" site plays fx_006's other
flare emitter), so names follow the content, not the site.

## Consistency pass (all 380 modules)

Every data entry of every effect file was hashed (8274 entries, the summon data and load buffer included) and
each group of byte-identical entries used by several modules was compared by name and by render (the entry's
PNGs from `fxdata.py --render`, byte for byte, then by eye where only the UV box or a stray texel differed).
Where the renders agree, the `<WHAT>` part now agrees; the game-name prefix still follows the module. The
direction: the spell's own module over its Sword Magic twin; the long summon over its short form; the look name
over a generic or carried pending one (`SO_MODEL`, `MODEL_n`, `FAN_EMITTER`, `EFFECT_MODEL`), which also
settled 19 "(look pending)" names. 161 renames in 61 modules, among them:

- Sword Magic: fx_0d4 .. fx_0e0 take the spells' names (`FX_SWDMAG_WATER_GLOBE_n`, `..._SPLASH_FUNNEL`,
  `FX_SWDMAG_BIO_PATH`, `FX_SWDMAG_FLARE_RED_VIOLET_RINGS`, `FX_SWDMAG_DOOMSDAY_METEOR`, `..._SPARK_DOTS`);
  the shared rotating sword ring is `*_SWORD_FLASH` in all eight.
- Short forms and carried names: Ramuh (fx_19f), Aera (fx_1f6), Fenrir (fx_1fd), Phoenix (fx_1fe), Ultima (fx_1ec),
  Atomos (fx_1be), Ifrit (fx_1bd), Trance (fx_1e9), Bahamut (fx_195), Ark (fx_1bf_1, fx_1bf_2), Carbuncle
  (fx_1ed .. fx_1ef `FX_CARBUNCLE_WHITE_LIGHT_STREAK`), Shiva / Diamond Dust (fx_197 `FX_SHIVA_ICICLE(_2)`).
- Enemy twins: Curaga (fx_08e, and fx_090 which runs the same C) takes enemy Dagger's magic's (fx_001) names;
  Roulette's model is Death's reaper (`FX_ROULETTE_REAPER`); Earthquake's two modules share `ROCK_CHUNK_n`;
  Luna's fan and sparkle circle are `LIGHT_STREAK` / `SPARKLE_CIRCLE` in all seven modules that carry them;
  the Throw impact is `FX_THROW_IMPACT` in all eight weapon modules.
- Generic names with a drawn shape (still pending where the texture is not settled): `FX_ARK_STAR_SPHERE`,
  `FX_LIMIT_GLOVE_PINK_ORB`, `FX_BAHAMUT_TERRAIN` / `CLOUD_RING` / `SMOKE_STRIP` / `SMOKE_SHELL`,
  `FX_LUCKY_SEVEN_RAINBOW_STRIPS`, `FX_MOONLIGHT_SLASH_SPIKED_SHAPE`, `FX_6_LEGS_SLIME_SHELL`. (Names updated after the effect-data runtime passes, which settled all of these but `FX_MOONLIGHT_SLASH_SPIKED_SHAPE`; the 6 Legs pod was renamed in pass 2.)

Left different on purpose (byte-identical, but drawn differently in context: other CLUTs or texture pages):
Magic / Armor / Mental Break orbs (red / blue / pink), Reflect's star (sketch placeholder page) vs Carbuncle's
star crystal, Holy's blue cloud arch vs Holy (Lv4)'s rainbow arch, Aero (breath) swirl vs Rainbow Wind ring,
Psychokinesis' screen trails (black in fx_023's render but white light rays at run time, `FX_PSYCHOKINESIS_LIGHT_RAYS`; dark bubbles in fx_0eb), Bahamut's blast plate (page 0x119) vs
the short form's smoke arc, Madeen's Y frame vs the short form's feather wings, Thievery's lens flares vs Lucky
Seven's nearly empty sprite line. No module defines a name twice or two names for one index; the remaining
generic names (`*_CYL_EMITTER`, `*_CALLBACK_EMITTER`, `*_FAN_EMITTER` of fans the render does not show) are
all still pending. The helper scripts were throw-away (hash every entry with `FxFile.entries`, group by sha1).

## Layer tables (render-to-texture)

The `*_BACKDROP` entries are not screen backdrops. `btlfx_drawLayerTexture` (was `btlfx_drawBackdrop`, ovl_0b
0x800D3030) sets the draw area and offset to the rect in the table header (`BtlBgLayers`: count, w, h, x, y), clears
it with a black TILE, draws the scrolling / waving / fading layers into it and restores the draw environment. Every
table the modules pass (89 call sites) targets an off-screen VRAM rect (x >= 0x180, at most 240 wide), and the
effect's sprites and models sample it: skipping the call turns their textured parts white or garbled
(docs/runtime-findings.md, effect-data pass). Played tables are named by the texture they make
(`FX_SLEEP_MOON_TEXTURE`, `FX_HOLY_STARFIELD_TEXTURE`); the others keep `*_BACKDROP`, with a "layer table" comment.

## Placeholder pages (oddities)

Some files ship test or placeholder textures (list in `docs/wip/fxdata_runtime.md`, "Oddities in the files"). What
the runtime passes found:
- Ark's kana test page (page 0x11B = (0x2C0, 0x100): a white page with the kana labels up / left / right / down,
  uploaded by fx_1bf_2, the short Ark's group 2, and fx_17d_8 in the full one) is never sampled on screen. In the
  short Ark it sits in VRAM from group 2's upload (~f907) to f1300 with nothing reading it; group 2 registers the
  model at f1309, and by f1320 the page holds an animated red-violet nebula. The model draws from ~f1584 with that
  nebula: the wide red glow band along Ark's beam hitting the planet, so entry 0x2 (and fx_17d_8's byte-identical
  0x6) is `FX_ARK_BEAM_HAZE` (docs/runtime-findings.md, effect-data pass 2).
- Ramuh short (fx_19f): the "dummy" placeholder is gone by run time; `FX_RAMUH_BLUE_AURA_COLUMN` samples a
  blue-white marbled texture.
- Ifrit (fx_114 / fx_1bd) tpage 0x117: the yellow placeholder is replaced by grey-white wavy streaks by the time
  `FX_IFRIT_WHITE_HAZE` is fetched; `FX_IFRIT_CYLINDER` is fetched while the placeholder is still there and changes
  nothing visible.

## Open questions (emulator)

All still-pending names of all slices, grouped by battle setup with what to capture, are in
`docs/wip/fxdata_runtime.md` (the emulator checklist); the list below is this slice's original notes.

Each wants one screenshot of the effect at the frame the C starts the entry (save state: battle, cast the
spell; `docs/emulator.md`), or a VRAM dump while it plays to render the missing texture pages:

- Blank page area (0x1C0, 0x100), filled at run time: `FX_THUNDARA_STAR_GLINTS` (fx_005, fx_049).
- Still transparent with the common / summon pages: `FX_THUNDAGA_VIOLET_GLOW(_2)`, `FX_THUNDAGA_SPARKLE_SWARM`
  (fx_018, fx_085).
- `FX_FIRAGA_RED_HAZE` (fx_007, fx_048): one dim dark-red frame; the blend mode decides the look.
- `FX_THUNDAGA_DISC` (fx_018, fx_085): 15-bit texture page 0x119, drawn there at run time.

All four were settled by the effect-data runtime pass (docs/runtime-findings.md, names above updated) except `FX_THUNDAGA_DISC`, still pending.

## Pilot table

| module | index | name | evidence | what it is |
|---|---|---|---|---|
| fx_000 | 0x5 | `FX_FIRE_FIREBALL` | render | emitter: fireball sprite with ember sparks, at the target body |
| fx_000 | 0x6 | `FX_FIRE_FLAME_BURST` | render | emitter: flame explosion (burst, smoke, embers) rising around the target |
| fx_000 | 0x7 | `FX_FIRE_HEAT_RING` | render | emitter: white-red gouraud rings at the target's feet |
| fx_002 | 0x4 | `FX_FIRA_FLAME_BURST` | render | emitter: flame explosion with ember sparks, at the target body |
| fx_002 | 0x5 | `FX_FIRA_SMOKE` | render | emitter: grey smoke puffs |
| fx_002 | 0x7 | `FX_FIRA_FLAME_BURST_2` | render | emitter: second flame explosion (embers, red-tinted burst) |
| fx_002 | 0x8 | `FX_FIRA_SMOKE_2` | render | emitter: grey smoke puffs after the second burst |
| fx_002 | 0x9 | `FX_FIRA_FLAME_SWIRL` | render | emitter: flame-strip trails orbiting the target |
| fx_002 | 0xD | `FX_FIRA_GROUND_RING` | render | rings: red gouraud ring drawn flat on the ground |
| fx_002 | 0xE | `FX_FIRA_HEAT_RING` | render | emitter: white-red gouraud rings at the target's feet |
| fx_003 | 0xA | `FX_THUNDER_SMOKE` | render | emitter: yellow / blue smoke puffs after the strike |
| fx_003 | 0xB | `FX_THUNDER_IMPACT_RING` | render | rings: cyan gouraud rings at the strike |
| fx_003 | 0xC | `FX_THUNDER_IMPACT_FLASH` | render | emitter: four-point star flash, white rings, sparks |
| fx_004 | 0xB | `FX_BLIZZARA_ICE_REFLECTION` | render | model: crystal shell drawn with the reflection map |
| fx_004 | 0xD | `FX_BLIZZARA_FROST_MIST` | render | emitter: grey frost mist puffs |
| fx_004 | 0xE | `FX_BLIZZARA_SHARD_MIST` | render | emitter: grey mist trailing each flying shard (pool item) |
| fx_004 | 0xF | `FX_BLIZZARA_ICE_FLARES_2` | render | emitter: cyan ice glints |
| fx_004 | 0x10 | `FX_BLIZZARA_ICE_FLARES` | render | emitter: cyan ice glints, four elements |
| fx_004 | 0x11 | `FX_BLIZZARA_FREEZE_BURST` | render | emitter: white-pink rings + spark sprites |
| fx_004 | 0x12 | `FX_BLIZZARA_FROST_RING` | render | emitter: lilac-blue gouraud rings |
| fx_004 | 0x13 | `FX_BLIZZARA_ICE_CRYSTAL` | render | model: long ice crystal spike (ice texture), splits in three |
| fx_004 | 0x14 | `FX_BLIZZARA_WHITE_RING` | render | emitter: white gouraud ring at the start |
| fx_004 | 0x15 | `FX_BLIZZARA_CRYSTAL_PIECE_0` | render | model: first piece of the split crystal |
| fx_004 | 0x16 | `FX_BLIZZARA_CRYSTAL_PIECE_1` | render | model: second piece of the split crystal |
| fx_004 | 0x17 | `FX_BLIZZARA_CRYSTAL_PIECE_2` | render | model: third piece of the split crystal |
| fx_004 | 0x19 | `FX_BLIZZARA_SHATTER_WAVE` | render | emitter: blue ripple wave |
| fx_005 | 0x7 | `FX_THUNDARA_SPARK_DOTS` | render (fxdata d6121033) | emitter: white spark dots (btlfx_emitDots) |
| fx_005 | 0x16 | `FX_THUNDARA_LENS_FLARES` | render | particles: orange lens-flare dots around the orb |
| fx_005 | 0x17 | `FX_THUNDARA_IMPACT_FLASH` | render | emitter: four-point star flash, white rings, sparks |
| fx_005 | 0x19 | `FX_THUNDARA_IMPACT_RING` | render | emitter: white-cyan gouraud rings |
| fx_005 | 0x1A | `FX_THUNDARA_ORB_EMITTER` | render | emitter: helix whose callback element draws the orb (0x16, 0x1C-0x1F) |
| fx_005 | 0x1C | `FX_THUNDARA_ORB_FADE` | render | particles: blue energy sphere fading to orange |
| fx_005 | 0x1D | `FX_THUNDARA_ORB` | render | particles: bright blue energy sphere |
| fx_005 | 0x1E | `FX_THUNDARA_STAR_GLINTS` | runtime | sprites: three 16x16 star-glint sprites in the run-time page (0x1C0,0x100), twinkling orange to blue around the orbs |
| fx_005 | 0x1F | `FX_THUNDARA_ORB_DOTS` | render (fxdata d6121033) | sprites: white dots scattering around the orb |
| fx_006 | 0x1 | `FX_BLIZZARD_ICE_FLARES` | render | emitter: cyan ice glint / light-streak frames |
| fx_006 | 0x2 | `FX_BLIZZARD_ICE_REFLECTION` | render | model: ice shell drawn with the scrolling reflection map |
| fx_006 | 0x3 | `FX_BLIZZARD_ICE_BLOCK` | render | model: ice block (cracked-ice texture), morph start |
| fx_006 | 0x4 | `FX_BLIZZARD_ICE_CRACKING` | render | model: the block breaking into pieces, morph middle |
| fx_006 | 0x5 | `FX_BLIZZARD_ICE_SHARDS` | render | model: scattered ice shards, morph end |
| fx_006 | 0x6 | `FX_BLIZZARD_ICE_FLARES_2` | render | emitter: cyan ice glints |
| fx_006 | 0xD | `FX_BLIZZARD_FROST_RING` | render | emitter: lilac-blue gouraud rings |
| fx_006 | 0xE | `FX_BLIZZARD_FREEZE_BURST` | render | emitter: white-pink rings + spark sprites as the ice forms |
| fx_006 | 0x10 | `FX_BLIZZARD_SHATTER_WAVE` | render | emitter: blue ripple wave when the ice shatters |
| fx_006 | 0x12 | `FX_BLIZZARD_FROST_SWIRL` | render (fxdata d6121033) | emitter: orbiting pale trail, cyan ice flecks and white dots |
| fx_007 | 0x8 | `FX_FIRAGA_FLAME_PILLAR` | render | model: open cylinder with a flame texture (the pillar) |
| fx_007 | 0x9 | `FX_FIRAGA_FLAME_DISC` | render | model: flat octagon, flame texture; registered, never drawn |
| fx_007 | 0xA | `FX_FIRAGA_FLAME_BURST` | render | emitter: flame explosion |
| fx_007 | 0xB | `FX_FIRAGA_GLOW` | render | rings: white-red gouraud glow disc at the target |
| fx_007 | 0xC | `FX_FIRAGA_EMBER_TRAIL` | render (fxdata d6121033) | emitter: orbiting red glow trail + red ember dots |
| fx_007 | 0xD | `FX_FIRAGA_COLUMN_SPARK` | render | rings: small red-white glow drawn at random fire-column segments |
| fx_007 | 0xE | `FX_FIRAGA_FLAME_BURST_2` | render | emitter: flame explosion at the pillar's start |
| fx_007 | 0xF | `FX_FIRAGA_PILLAR_RING` | render | rings: textured flame ring rising with the pillar |
| fx_007 | 0x10 | `FX_FIRAGA_GOLD_SPARKS` | render | emitter: golden spark sprites |
| fx_007 | 0x11 | `FX_FIRAGA_GOLD_SPARKS_2` | render | emitter: golden spark sprites + spark sub-particles |
| fx_007 | 0x12 | `FX_FIRAGA_EMBERS` | render | emitter: dark red ember sprites |
| fx_007 | 0x13 | `FX_FIRAGA_SMOKE_BURST` | render | emitter: dark teal-tinted burst (smoke after the blast) |
| fx_007 | 0x14 | `FX_FIRAGA_EMBER_CONE` | render (fxdata d6121033) | emitter: cone of red and grey ember dots |
| fx_007 | 0x15 | `FX_FIRAGA_RED_HAZE` | runtime (medium) | emitter: soft dark-red additive blob, the red heat haze filling the fire column |
| fx_007 | 0x16 | `FX_FIRAGA_FLAME_RIBBON_0` | render (fxdata d6121033) | 'so' model: flat flame-textured ribbon piece, drawn coiled 18 times as the fire column |
| fx_007 | 0x17 | `FX_FIRAGA_FLAME_RIBBON_1` | render (fxdata d6121033) | 'so' model: flat flame-textured ribbon piece |
| fx_007 | 0x18 | `FX_FIRAGA_FLAME_RIBBON_2` | render (fxdata d6121033) | 'so' model: flat flame-textured ribbon piece |
| fx_007 | 0x19 | `FX_FIRAGA_FLAME_RIBBON_3` | render (fxdata d6121033) | 'so' model: flat flame-textured ribbon piece |
| fx_008 | 0x3 | `FX_CURE_STAR_FLARES` | render | sprite line: blue star flares along a path |
| fx_008 | 0x4 | `FX_CURE_SPARKLE_SWIRL` | render | emitter: star-sparkle trails orbiting the target, sparkles and bubbles |
| fx_009 | 0x3 | `FX_CURE_SPARKLE_SWIRL` | render | emitter: star-sparkle trails spiralling up, sparkles and bubbles |
| fx_018 | 0x7 | `FX_THUNDAGA_VIOLET_GLOW` | runtime (medium) | emitter: soft violet-magenta glow puffs, the violet light around the thin bolts |
| fx_018 | 0xA | `FX_THUNDAGA_VIOLET_GLOW_2` | runtime (medium) | emitter: the same violet-magenta glow puffs, started later |
| fx_018 | 0x10 | `FX_THUNDAGA_DISC` | kind/usage (look pending) | model: flat octagon, 15-bit texture page 0x119 holds the file's 4-bit pages, so it samples something drawn there at run time; runtime: its page (0x240,0x100) holds 4-bit lightning textures at run time, not a screen copy; ablating the model changes nothing visible |
| fx_018 | 0x11 | `FX_THUNDAGA_GLOW_DOME` | render | model: hemisphere with an orange-to-white streak texture |
| fx_018 | 0x13 | `FX_THUNDAGA_BLUE_RING` | render | rings: white-blue gouraud rings |
| fx_018 | 0x15 | `FX_THUNDAGA_WHITE_RING` | render | rings: white gouraud rings drawn flat |
| fx_018 | 0x17 | `FX_THUNDAGA_LIGHTNING_BOLT` | render | particles: wide blue-white lightning bolt |
| fx_018 | 0x1B | `FX_THUNDAGA_SPARK_BALL` | render | particles: golden plasma ball, 8 frames |
| fx_018 | 0x1E | `FX_THUNDAGA_VIOLET_RING` | render | rings: white-violet gouraud rings |
| fx_018 | 0x20 | `FX_THUNDAGA_THIN_BOLT` | render | particles: thin cyan lightning bolt |
| fx_018 | 0x21 | `FX_THUNDAGA_LIGHTNING_ARCS` | render (fxdata d6121033) | emitter: four fans drawn as cyan and blue zig-zag lightning arcs |
| fx_018 | 0x22 | `FX_THUNDAGA_LIGHTNING_ARCS_2` | render (fxdata d6121033) | emitter: four fans drawn as cyan and blue zig-zag lightning arcs |
| fx_018 | 0x25 | `FX_THUNDAGA_IMPACT_FLASH` | render | emitter: four-point star flash |
| fx_018 | 0x26 | `FX_THUNDAGA_SPARKLE_SWARM` | runtime | emitter: swarms of blue, lilac and teal twinkle dots, the cyan sparkles around the strike |
| fx_018 | 0x27 | `FX_THUNDAGA_SPARK_DOTS` | render (fxdata d6121033) | emitter: white spark dots (btlfx_emitDots) |
| fx_03e | 0x2 | `FX_BLIZZARD_ICE_REFLECTION` | render | model: ice shell drawn with the scrolling reflection map (byte-identical to fx_006 entry 0x2) |
| fx_03e | 0x3 | `FX_BLIZZARD_ICE_BLOCK` | render | model: ice block (cracked-ice texture), morph start (byte-identical to fx_006 entry 0x3) |
| fx_03e | 0x7 | `FX_BLIZZARD_FROST_SWIRL` | render (fxdata d6121033) | emitter: orbiting pale trail, cyan ice flecks and white dots (same call site as fx_006 entry 0x12) |
| fx_03e | 0x8 | `FX_BLIZZARD_SHATTER_WAVE` | render | emitter: blue ripple wave with ice sparkles |
| fx_03e | 0xA | `FX_BLIZZARD_FROST_RING` | render | emitter: lilac-blue gouraud rings |
| fx_03e | 0x15 | `FX_BLIZZARD_ICE_FLARES_2` | render | emitter: cyan ice glints |
| fx_03e | 0x16 | `FX_BLIZZARD_ICE_FLARES` | render | emitter: cyan ice glint / light-streak frames |
| fx_042 | 0x8 | `FX_THUNDER_IMPACT_FLASH` | render | emitter: four-point star flash, white rings, sparks |
| fx_046 | 0x2 | `FX_FIRE_FLAME_BURST` | render | emitter: flame explosion (burst, smoke, embers) over the targets |
| fx_046 | 0x3 | `FX_FIRE_HEAT_RING` | render | emitter: white-red gouraud rings on the ground |
| fx_047 | 0x1 | `FX_FIRA_FLAME_SWIRL` | render | emitter: flame-strip trails orbiting the targets |
| fx_047 | 0x2 | `FX_FIRA_HEAT_RING` | render | emitter: white-red gouraud rings on the ground |
| fx_047 | 0x3 | `FX_FIRA_FLAME_BURST` | render | emitter: flame explosion with ember sparks, above the targets |
| fx_047 | 0x4 | `FX_FIRA_SMOKE` | render | emitter: grey smoke puffs |
| fx_047 | 0x5 | `FX_FIRA_FLAME_BURST_2` | render | emitter: second flame explosion |
| fx_048 | 0x2 | `FX_FIRAGA_FLAME_RIBBON_3` | render (fxdata d6121033) | 'so' model: flat flame-textured ribbon piece (byte-identical to fx_007 entry 0x19) |
| fx_048 | 0x5 | `FX_FIRAGA_FLAME_BURST` | render | emitter: flame explosion (byte-identical to fx_007 entry 0xa) |
| fx_048 | 0x9 | `FX_FIRAGA_FLAME_RIBBON_1` | render (fxdata d6121033) | 'so' model: flat flame-textured ribbon piece (byte-identical to fx_007 entry 0x17) |
| fx_048 | 0xA | `FX_FIRAGA_FLAME_PILLAR` | render | model: open cylinder with a flame texture (the pillar) (byte-identical to fx_007 entry 0x8) |
| fx_048 | 0xB | `FX_FIRAGA_FLAME_RIBBON_2` | render (fxdata d6121033) | 'so' model: flat flame-textured ribbon piece (byte-identical to fx_007 entry 0x18) |
| fx_048 | 0xC | `FX_FIRAGA_FLAME_RIBBON_0` | render (fxdata d6121033) | 'so' model: flat flame-textured ribbon piece, drawn coiled 18 times as the fire column (byte-identical to fx_007 entry 0x16) |
| fx_048 | 0xE | `FX_FIRAGA_FLAME_DISC` | render | model: flat octagon, flame texture; registered, never drawn (byte-identical to fx_007 entry 0x9) |
| fx_048 | 0x11 | `FX_FIRAGA_EMBER_TRAIL` | render (fxdata d6121033) | emitter: orbiting red glow trail + red ember dots (byte-identical to fx_007 entry 0xc) |
| fx_048 | 0x12 | `FX_FIRAGA_COLUMN_SPARK` | render | rings: small red-white glow drawn at random fire-column segments (byte-identical to fx_007 entry 0xd) |
| fx_048 | 0x13 | `FX_FIRAGA_PILLAR_RING` | render | rings: textured flame ring rising with the pillar (byte-identical to fx_007 entry 0xf) |
| fx_048 | 0x14 | `FX_FIRAGA_EMBERS` | render | emitter: dark red ember sprites (byte-identical to fx_007 entry 0x12) |
| fx_048 | 0x15 | `FX_FIRAGA_GOLD_SPARKS_2` | render | emitter: golden spark sprites + spark sub-particles (byte-identical to fx_007 entry 0x11) |
| fx_048 | 0x16 | `FX_FIRAGA_GOLD_SPARKS` | render | emitter: golden spark sprites (byte-identical to fx_007 entry 0x10) |
| fx_048 | 0x17 | `FX_FIRAGA_SMOKE_BURST` | render | emitter: dark teal-tinted burst (smoke after the blast) (byte-identical to fx_007 entry 0x13) |
| fx_048 | 0x18 | `FX_FIRAGA_RED_HAZE` | runtime (medium) | emitter: soft dark-red additive blob, the red heat haze filling the fire column (same call site as fx_007 entry 0x15) |
| fx_048 | 0x19 | `FX_FIRAGA_EMBER_CONE` | render (fxdata d6121033) | emitter: cone of red and grey ember dots (byte-identical to fx_007 entry 0x14) |
| fx_049 | 0x2 | `FX_THUNDARA_IMPACT_FLASH` | render | emitter: four-point star flash, white rings, sparks |
| fx_049 | 0x6 | `FX_THUNDARA_IMPACT_RING` | render | emitter: white-cyan gouraud rings |
| fx_049 | 0x15 | `FX_THUNDARA_ORB_EMITTER` | render | emitter: helix whose callback element draws the orb (0x16, 0x1C-0x1F) |
| fx_049 | 0x18 | `FX_THUNDARA_SPARK_DOTS` | render (fxdata d6121033) | emitter: white spark dots (btlfx_emitDots) |
| fx_049 | 0x19 | `FX_THUNDARA_ORB_FADE` | render | particles: blue energy sphere fading to orange |
| fx_049 | 0x1A | `FX_THUNDARA_ORB` | render | particles: bright blue energy sphere |
| fx_049 | 0x1B | `FX_THUNDARA_STAR_GLINTS` | runtime | sprites: three 16x16 star-glint sprites in the run-time page (0x1C0,0x100), twinkling orange to blue around the orbs |
| fx_049 | 0x1C | `FX_THUNDARA_ORB_DOTS` | render (fxdata d6121033) | sprites: white dots scattering around the orb |
| fx_049 | 0x1F | `FX_THUNDARA_LENS_FLARES` | render | particles: orange lens-flare dots around the orb |
| fx_04d | 0xB | `FX_BLIZZARA_ICE_REFLECTION` | render | model: crystal shell drawn with the reflection map |
| fx_04d | 0xD | `FX_BLIZZARA_ICE_CRYSTAL` | render | model: long ice crystal spike (ice texture), splits in three |
| fx_04d | 0xE | `FX_BLIZZARA_FROST_MIST` | render | emitter: grey frost mist puffs |
| fx_04d | 0xF | `FX_BLIZZARA_FREEZE_BURST` | render | emitter: white-pink rings + spark sprites |
| fx_04d | 0x10 | `FX_BLIZZARA_FROST_RING_2` | render | emitter: pale blue gouraud rings |
| fx_04d | 0x11 | `FX_BLIZZARA_ICE_FLARES` | render | emitter: cyan ice glints, four elements |
| fx_04d | 0x12 | `FX_BLIZZARA_ICE_FLARES_2` | render | emitter: cyan ice glints |
| fx_04d | 0x13 | `FX_BLIZZARA_FROST_RING` | render | emitter: lilac-blue gouraud rings |
| fx_04d | 0x14 | `FX_BLIZZARA_SHARD_MIST` | render | emitter: grey mist trailing each flying shard (pool item) |
| fx_04d | 0x15 | `FX_BLIZZARA_SNOW_BURST` | render | emitter: snow-flake sprites |
| fx_04d | 0x16 | `FX_BLIZZARA_CRYSTAL_PIECE_1` | render | model: second piece of the split crystal |
| fx_04d | 0x17 | `FX_BLIZZARA_CRYSTAL_PIECE_0` | render | model: first piece of the split crystal |
| fx_04d | 0x18 | `FX_BLIZZARA_CRYSTAL_PIECE_2` | render | model: third piece of the split crystal |
| fx_04d | 0x19 | `FX_BLIZZARA_SHATTER_WAVE` | render | emitter: blue ripple wave |
| fx_085 | 0x1 | `FX_THUNDAGA_IMPACT_FLASH` | render | emitter: four-point star flash (byte-identical to fx_018 entry 0x25) |
| fx_085 | 0x6 | `FX_THUNDAGA_GLOW_DOME` | render | model: hemisphere with an orange-to-white streak texture (byte-identical to fx_018 entry 0x11) |
| fx_085 | 0x8 | `FX_THUNDAGA_SPARKLE_SWARM` | runtime | emitter: swarms of blue, lilac and teal twinkle dots, the cyan sparkles around the strike (byte-identical to fx_018 entry 0x26) |
| fx_085 | 0xE | `FX_THUNDAGA_SPARK_DOTS` | render (fxdata d6121033) | emitter: white spark dots (btlfx_emitDots) (byte-identical to fx_018 entry 0x27) |
| fx_085 | 0xF | `FX_THUNDAGA_DISC` | kind/usage (look pending) | model: flat octagon, 15-bit texture page 0x119 holds the file's 4-bit pages, so it samples something drawn there at run time (byte-identical to fx_018 entry 0x10); runtime: same entry as fx_018 0x10, whose page holds 4-bit lightning textures at run time; ablating it there changes nothing visible |
| fx_085 | 0x10 | `FX_THUNDAGA_LIGHTNING_BOLT` | render | particles: wide blue-white lightning bolt (byte-identical to fx_018 entry 0x17) |
| fx_085 | 0x11 | `FX_THUNDAGA_THIN_BOLT` | render | particles: thin cyan lightning bolt (byte-identical to fx_018 entry 0x20) |
| fx_085 | 0x15 | `FX_THUNDAGA_LIGHTNING_ARCS` | render (fxdata d6121033) | emitter: four fans drawn as cyan and blue zig-zag lightning arcs (byte-identical to fx_018 entry 0x21) |
| fx_085 | 0x18 | `FX_THUNDAGA_SPARK_BALL` | render | particles: golden plasma ball, 8 frames (byte-identical to fx_018 entry 0x1b) |
| fx_085 | 0x1B | `FX_THUNDAGA_VIOLET_GLOW` | runtime (medium) | emitter: soft violet-magenta glow puffs, the violet light around the thin bolts (byte-identical to fx_018 entry 0x7) |
| fx_085 | 0x1C | `FX_THUNDAGA_VIOLET_GLOW_2` | runtime (medium) | emitter: the same violet-magenta glow puffs, started later (byte-identical to fx_018 entry 0xa) |
| fx_085 | 0x22 | `FX_THUNDAGA_LIGHTNING_ARCS_2` | render (fxdata d6121033) | emitter: four fans drawn as cyan and blue zig-zag lightning arcs (byte-identical to fx_018 entry 0x22) |
| fx_097 | 0xF | `FX_BLIZZAGA_CRYSTAL_SHEEN_2` | render | model: crystal 2 shape, reflection-map texture |
| fx_097 | 0x10 | `FX_BLIZZAGA_CRYSTAL_SHEEN_3` | render | model: crystal 3 shape, reflection-map texture |
| fx_097 | 0x11 | `FX_BLIZZAGA_CRYSTAL_SHEEN` | render | model: main crystal shape, reflection-map texture |
| fx_097 | 0x12 | `FX_BLIZZAGA_ICE_CRYSTAL_2` | render | model: ice crystal spike (hexagonal, ice texture) |
| fx_097 | 0x13 | `FX_BLIZZAGA_ICE_CRYSTAL_3` | render | model: wide ice crystal spike |
| fx_097 | 0x14 | `FX_BLIZZAGA_ICE_CRYSTAL` | render | model: main ice crystal spike |
| fx_097 | 0x1B | `FX_BLIZZAGA_FROST_RING` | render | rings: blue gouraud ring on the ground |
| fx_097 | 0x1C | `FX_BLIZZAGA_ICE_SPIKES` | render (fxdata d6121033) | emitter: four fans drawn as narrowing cyan, green, violet and blue icicle-shaped light spikes |
| fx_097 | 0x1D | `FX_BLIZZAGA_SNOW_BURST` | render | emitter: rings textured with snow flakes |
| fx_097 | 0x1E | `FX_BLIZZAGA_SHATTER_WAVE` | render | emitter: blue ripple wave |
| fx_097 | 0x1F | `FX_BLIZZAGA_ICE_FLARES_2` | render | emitter: cyan ice glints |
| fx_097 | 0x20 | `FX_BLIZZAGA_ICE_FLARES` | render | emitter: cyan ice glints, four elements |
| fx_097 | 0x21 | `FX_BLIZZAGA_FREEZE_BURST` | render | emitter: white-pink rings + spark sprites |
| fx_097 | 0x22 | `FX_BLIZZAGA_FROST_RING_2` | render | emitter: pale blue gouraud rings |
| fx_097 | 0x23 | `FX_BLIZZAGA_SNOW_RING` | render | rings: textured ring of blue snow flakes |
| fx_097 | 0x24 | `FX_BLIZZAGA_SNOW_RING_2` | render | rings: textured ring of pale snow flakes |
| fx_097 | 0x25 | `FX_BLIZZAGA_SHARD_MIST` | render | emitter: grey mist trailing each shard (pool item) |
| fx_097 | 0x26 | `FX_BLIZZAGA_MIST` | render | emitter: grey frost mist at the crystal base |
| fx_097 | 0x27 | `FX_BLIZZAGA_MIST_HIGH` | render | emitter: grey frost mist higher up |
| fx_097 | 0x28 | `FX_BLIZZAGA_ICE_SPARKLES` | render | emitter: small blue ice sparkles at the shatter |
| fx_097 | 0x29 | `FX_BLIZZAGA_ICE_SPARKLES_2` | render | emitter: small blue ice sparkles while the crystal grows |
| fx_098 | 0xF | `FX_BLIZZAGA_CRYSTAL_SHEEN_2` | render | model: crystal 2 shape, reflection-map texture (byte-identical to fx_097 entry 0xf) |
| fx_098 | 0x10 | `FX_BLIZZAGA_CRYSTAL_SHEEN_3` | render | model: crystal 3 shape, reflection-map texture (byte-identical to fx_097 entry 0x10) |
| fx_098 | 0x11 | `FX_BLIZZAGA_CRYSTAL_SHEEN` | render | model: main crystal shape, reflection-map texture (byte-identical to fx_097 entry 0x11) |
| fx_098 | 0x12 | `FX_BLIZZAGA_ICE_CRYSTAL_2` | render | model: ice crystal spike (hexagonal, ice texture) (byte-identical to fx_097 entry 0x12) |
| fx_098 | 0x13 | `FX_BLIZZAGA_ICE_CRYSTAL_3` | render | model: wide ice crystal spike (byte-identical to fx_097 entry 0x13) |
| fx_098 | 0x14 | `FX_BLIZZAGA_ICE_CRYSTAL` | render | model: main ice crystal spike (byte-identical to fx_097 entry 0x14) |
| fx_098 | 0x1B | `FX_BLIZZAGA_FROST_RING` | render | rings: blue gouraud ring on the ground (byte-identical to fx_097 entry 0x1b) |
| fx_098 | 0x1D | `FX_BLIZZAGA_ICE_SPIKES` | render (fxdata d6121033) | emitter: four fans drawn as narrowing cyan, green, violet and blue icicle-shaped light spikes (byte-identical to fx_097 entry 0x1c) |
| fx_098 | 0x1F | `FX_BLIZZAGA_SHATTER_WAVE` | render | emitter: blue ripple wave (byte-identical to fx_097 entry 0x1e) |
| fx_098 | 0x20 | `FX_BLIZZAGA_ICE_FLARES_2` | render | emitter: cyan ice glints (byte-identical to fx_097 entry 0x1f) |
| fx_098 | 0x21 | `FX_BLIZZAGA_FROST_RING_2` | render | emitter: pale blue gouraud rings (byte-identical to fx_097 entry 0x22) |
| fx_098 | 0x22 | `FX_BLIZZAGA_ICE_FLARES` | render | emitter: cyan ice glints, four elements (byte-identical to fx_097 entry 0x20) |
| fx_098 | 0x23 | `FX_BLIZZAGA_SNOW_RING` | render | rings: textured ring of blue snow flakes (byte-identical to fx_097 entry 0x23) |

Effort for the other modules: the tool run is about 3 s per file; naming took one contact-sheet look plus
the C call order per module (about 10 minutes for a one-target module, a few for its all-targets twin with
the byte-identical/call-order carry-over), so roughly 40-60 hours for the remaining 360 modules, summons
(several groups, 0x80+ data) being the slow part.

## Choreography literals (pilot)

The effect modules' other numbers (18,228 actionable literals in `tools/literal_survey.py --bin fxmod`: compare /
mask / case / arg / assign) are mostly the effect's choreography: how long a task or phase runs, on which frame it
starts the next emitter, which VRAM rect it scrolls, how big a piece is. The pilot names the ones whose meaning
the code proves, in the same 20 modules, grounded in the data names above. `tools/fxlits.py` counts them per
module (`--pilot`, `--all`, `fx_006 --list`, same classifier as literal_survey).

### Scheme

Defines go in the module header, in a block after the effect-data names (`/* fx_XXX choreography ... */`), with
the spell prefix and a comment that says what the code does with the value:

| kind | form | proof in the code |
|---|---|---|
| task / phase length | `FX_<SPELL>_<PIECE>_FRAMES` | `if (frame >= N) { state = next; entry->frame = -1; }`, `frame < N` around a draw, the `(frame << 12) / N` ramp of the same phase (the define replaces both, so they visibly agree) |
| keyframe | `FX_<SPELL>_<EVENT>_FRAME` | `frame == N` that starts an emitter (`FX_FIRA_SMOKE_2_FRAME`), switches a morph or a sub-phase (`FX_FIRA_RING_PULSE_FRAME`) |
| state values | `FX_<SPELL>_STATE_<PHASE>` | the op's `switch (work->stateN)` labels and assignments, named by what the case draws |
| op modes | `FX_<SPELL>_BOLTS_<KIND>` | a `switch` on the op's init argument (Thundaga op83) |
| pool / work sizes | `sizeof(FxXXXWork)`, `sizeof(FxXXXItem)` | `*args = N` (mode 0) and `init->workSize` / `elemSize`; where the struct was short the header gains an `unused` pad (fx_004 work, the Thunder / Thundara / Thundaga bolts) |
| counts | `_COUNT`, `_SLOTS`, `_POINTS`, `_SEGMENTS` | `init->poolCount`, loop bounds, `% N` into the same array, `btlfx_initEffModels` slots, array sizes in the header (`pts4[FX_THUNDAGA_STRIP_POINTS]`) |
| trail buffers | `FX_<SPELL>_<EMITTER>_TRAIL_SLOTS` / `_TRAIL_STRIDE` | `emitter->trailSlots` / `trailStride` of the emitter whose data index names it |
| VRAM rects | `FX_<SPELL>_<TEX>_VRAM_X/Y/W/H`, `_SRC_X/Y` | `btlfx_drawScrollMove` args, checked against the model's tpage + u/v and the file's upload (`fxdata.py` vram: the destination holds a "dummy" placeholder, the source the texture) |
| texel frames | `FX_FIRAGA_FLOOR_GLOW_TPAGE/X/Y/COLS/GRID` | `fxm_drawTexelQuads`' StoreImage rect (2x4 frames of 16x16 texels in the 15-bit page) |
| sizes | `_SCALE`, `_HEIGHT`, `_REACH`, `_RADIUS`, `_WIDTH` | a scale or offset used for one named piece in several places (`FX_BLIZZARD_ICE_SCALE`, all-targets twins' `pos.vy -= FX_BLIZZARA_ICE_HEIGHT`) |
| screen tints | `FX_<SPELL>_<WHAT>_RGB` = `r, g, b` | the three constant colour args of `btlfx_drawScreenTint` (`FX_BLIZZARD_FREEZE_TINT_RGB 0, 0x20, 0x46`); `DARKEN` for the subtractive ones (abr 2) |
| GPU words | `GPU_POLY_G4 \| GPU_ABE`, `PRIM_TAG(POLY_GT4)`, `BTLFX_ABR_OPAQUE` | the existing psyq.h / ovl_0b.h names, in the modules' own primitive helpers |

Derived numbers are written as expressions of the names when the code computes them that way (constant-folded,
byte-identical): `/ (FX_FIRA_GROUND_RING_FRAMES - FX_FIRA_RING_PULSE_FRAME)` for Fira's 21,
`0x1000 / (FX_CURE_STAR_FLARES_FRAMES - FX_CURE_STAR_FLARES_FADE_FRAME)` for Cure's fade step 0x155,
`FX_THUNDAGA_CHARGE_FRAMES - 1` for the darken frame. One-target and all-targets modules use the same names with
their own values (`FX_BLIZZARD_ICE_SCALE` 0xC00 / 0xE00, `FX_THUNDER_BOLT_COUNT` 5 / 0x18).

Examples (old → new):

- fx_006 `if (frame >= 0x10) { work->state0 = 3; ...` → `if (frame >= FX_BLIZZARD_GLEAM_FRAMES) { work->state0 = FX_BLIZZARD_STATE_SHATTER;`
- fx_006 `morph = 0x1000 - ((frame - 2) << 12) / 14` → `... ((frame - FX_BLIZZARD_CRACK_FRAMES) << 12) / FX_BLIZZARD_SHARDS_FRAMES`
- fx_004 `btlfx_drawScrollMove(scroll30, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100)` → `FX_BLIZZARA_REFLECTION_VRAM_X, ..._Y, ..._W, ..._H, FX_BLIZZARA_REFLECTION_SRC_X, ..._SRC_Y`
- fx_007 `pillarOn = frame < 0x1D` / `(frame << 12) / 29` → `FX_FIRAGA_PILLAR_RING_FRAMES`; `i < 18` → `FX_FIRAGA_COLUMN_SEGMENTS`
- fx_003 `frame == 3 || frame == 5 ...`, `boltCountC < 5` → `FX_THUNDER_BOLT_2_FRAME ...`, `FX_THUNDER_BOLT_COUNT`; `it->kind14 = 2` → `FX_003_STRIP_FLAT`
- fx_005 `drawScreenTint(ot, 2, 0x40, 0x40, 0x40)` on `frame == 0xF` → `FX_THUNDARA_DARKEN_RGB` on `FX_THUNDARA_DARKEN_FRAME`
- fx_018 `case 2:` of op83's kind switch → `case FX_THUNDAGA_BOLTS_CHAIN:`; `/ 31` (three places) → `FX_THUNDAGA_CHAIN_BOLTS_FRAMES`
- fx_097 `pieceIdx = 10` → `FX_BLIZZAGA_CRYSTAL_PIECES + FX_BLIZZAGA_SHARD_PIECES`; `prim + 14 * 14` → `FX_BLIZZAGA_BULGE_GRID * FX_BLIZZAGA_BULGE_GRID`
- fx_008 `*args = 0x18` → `sizeof(Fx008Work)`; `trailSlots = 0xB` → `FX_CURE_SPARKLE_SWIRL_TRAIL_SLOTS`

### Counts (actionable literals, `tools/fxlits.py --pilot`)

| spell | one target | before | after | all targets | before | after |
|---|---|---|---|---|---|---|
| Fire | fx_000 | 4 | 2 | fx_046 | 4 | 2 |
| Fira | fx_002 | 18 | 8 | fx_047 | 11 | 5 |
| Firaga | fx_007 | 91 | 52 | fx_048 | 93 | 53 |
| Blizzard | fx_006 | 53 | 29 | fx_03e | 52 | 28 |
| Blizzara | fx_004 | 53 | 36 | fx_04d | 55 | 37 |
| Blizzaga | fx_097 | 174 | 106 | fx_098 | 175 | 105 |
| Thunder | fx_003 | 53 | 31 | fx_042 | 38 | 21 |
| Thundara | fx_005 | 63 | 39 | fx_049 | 63 | 36 |
| Thundaga | fx_018 | 125 | 69 | fx_085 | 118 | 63 |
| Cure | fx_008 | 9 | 1 | fx_009 | 4 | 0 |

1,256 → 723 (533 named, 42%; 439 defines over the 20 headers, twins counted separately); literal_survey's fxmod
total 18,228 → 17,695.

### What stays numeric, and why

- **Texture args of `Hi_RegisterTexEffModel`** (tpage, CLUT, u, v: 108 of the remaining 723): `tools/fxdata.py`
  reads these call args as integer literals (`lit()`) to texture the model renders, so names there would blank
  the renders. A follow-up that lets fxdata.py resolve the module header's defines (and `getTPage`/`getClut`)
  would allow `FX_<SPELL>_<MODEL>_TPAGE` / `_CLUT`; the bulge grid's (`FX_BLIZZAGA_BULGE_TPAGE`), which fxdata
  does not read, is named already.
- **OT depth biases** (`emitter->otZ = 0x80`, `btlfx_addRadiusSixth(0, 0x20)`, `env->otZ = 0x200`: 130):
  draw-order tuning per emitter, no meaning beyond "in front of / behind"; a name would only restate the number.
- **Random spreads** (`btlfx_randCentered(0x190)`, `rand() % 50`, jitter `& 0x1F`: ~95) and the zig-zag
  amplitudes / angle steps of the lightning strips.
- **Fixed-point and angle math**: `0x1000` ramps, `>> 12`, `- 0x80` (offset from the neutral 0x80 colour),
  `rot.vx = 0x400` (90 degrees), easing coefficients, ring / particle scales that appear once
  (`btlfx_drawRings(..., 0x7000, ...)`), colour-channel masks (`& 0x1F`), shade clamps.
- Frame-relative shifts (`t = frame << 10` = a 4-frame ramp) where the matching length is a separate compare:
  the compare gets the name, the shift stays (a division would change the code).
- abr arguments (1 = additive, 2 = subtractive) are the noise values 1 / 2 the survey skips. Since the
  all-module pass (below) every literal one is a `BTLFX_ABR_*` name (ovl_0b.h).
- Values the code does not tie to one meaning: Firaga's column height (0x960 in fx_007, but 0xB86 / 0xA28 in
  fx_048's two branches), the branch start `% 14` / `% 13` (one less than the last point index pair, not
  provably derived), drawRings' fixed data frame (`..., 4, 1, ...`).

### Estimate for all 380 modules

The pilot is 5,877 of the modules' 83,673 C lines (7%) and 1,256 of the 18,228 literals (6.9%). At the same
42%, about 7,500 literals would get names. The rest splits roughly as here: a tenth depth biases, a twelfth model
texture args (nameable after the fxdata.py change, another ~1,500), random spreads and math. Effort: the
mechanical part (sizeof, PRIM_TAG / GPU codes, trail slots, state enums, the `frame >= N` / `/ N` pairs, tints)
goes quickly once a module's data names exist; reading the choreography took about 15-30 minutes per one-target
module and a few minutes for its twin (same names, the value diff). That is roughly 60-90 hours for the other
360 modules, the summons (several groups, longer state machines) taking a third of it; in parallel slices
like the effect-data naming (8 x ~45 modules) a few sessions each.

## Choreography (all modules)

After the pilot, the remaining 360 modules were named in eight slices, then the biggest state machines got
deep passes that read them one state at a time. The scheme is the pilot's ([Scheme](#scheme)): defines in a
`/* fx_XXX choreography */` block after the module's effect-data names, the spell's game-name prefix, a comment
saying what the code does with the value, derived numbers written as expressions of the names, and twins
(one / all targets, short / long summons, Sword Magic) sharing names with their own values. Later additions to the
scheme: `Hi_Register*Model` texture args as `FX_<SPELL>_<MODEL>_TPAGE / _CLUT / _U / _V` (fxdata.py's `lit()`
resolves them, so the renders stay textured), countdown-slot indices `FX_<SPELL>_T_<PHASE>` for the modules that
keep their phases in `timers[N]` arrays, and names carried over between twins by token alignment.

Two passes over all 380 modules: every literal abr argument (of the draw exports btlfx_drawScreenTint /
drawRings / drawParticles / drawTrail / drawSprite(s) / drawSpriteAtOt / drawAnimQuads / drawRippleGrid /
addPrimAbr / linkPrimAbr / Hi_Modify*ModelAbr, and of the modules' own helpers whose parameter is `abr`) is a
`BTLFX_ABR_*` name, and every literal unit selector passed to the btlfx unit exports (getUnitPos / BodyPos /
BonePos / BoneMatrix / HomePos / Rot / Scale, setUnit*, addRadiusSixth, fadeUnitById, getWeaponPos, isEnemyUnit,
clearUnitStatus, resetIdleMotion's second argument, btl_getUnit, ...) or to the modules' unit helpers
(fxm_turnUnitBy / fxm_moveUnitArc / fxm_startUnitMotion, ...) is a `BTLFX_UNIT_*` name (ovl_0b.h; 0x10 caster,
0x11 partner). Target index 0 (the first target) stays a plain 0; no other selector value occurs as a literal.

**Totals**: `tools/literal_survey.py --bin fxmod` actionable literals (compare / mask / case / arg / assign)
18,228 before the pilot → 17,695 after it → **8,762** now (720 / 361 / 76 / 4,465 / 3,140).

| doc | modules | what it covers | actionable literals (fxlits) |
|---|---|---|---|
| [fxchoreo_s0.md](fxchoreo_s0.md) | fx_001 .. fx_05f (70) | spells and early abilities, Shiva | 1,379 → 599 |
| [fxchoreo_s1.md](fxchoreo_s1.md) | fx_060 .. fx_0b0 (58) | status spells, Demi, Meteor, Flare, the Cura / Curaga twins | 1,826 → 1,120 |
| [fxchoreo_s2.md](fxchoreo_s2.md) | fx_0b1 .. fx_0d8 (39) | Carbuncle, Leviathan, Atomos, Ramuh, Fenrir, Phoenix, Sword Magic fx_0d4 .. 0d8 | 2,330 → 1,391 |
| [fxchoreo_s3.md](fxchoreo_s3.md) | fx_0d9 .. fx_0f2 (28) | Sword Magic twins, Rebirth Flame, Fenrir 2nd, Bahamut, Doom, Roulette | 2,329 → 1,568 |
| [fxchoreo_s4.md](fxchoreo_s4.md) | fx_0f3 .. fx_128 (48) | late abilities, Madeen, Odin, Ifrit, Throw, items, enemy skills | 1,980 → 1,244 |
| [fxchoreo_s5.md](fxchoreo_s5.md) | fx_129 .. fx_187 (36) | Earthquake, Doomsday, Lucky Seven, Ark's nine groups, Ultima; first abr / caster passes | 2,412 → 1,336 |
| [fxchoreo_s6.md](fxchoreo_s6.md) | fx_188 .. fx_1be (48) | short summons, sword skills, enemy Death sequences | 2,558 → 1,774 |
| [fxchoreo_s7.md](fxchoreo_s7.md) | fx_1bf_0 .. fx_1fe (30) | short Ark / Carbuncle / Fenrir / Phoenix, Dragon's Crest, Six Dragons, Trance, late enemy skills | 2,155 → 1,243 |
| [fxchoreo_deep_a.md](fxchoreo_deep_a.md) | fx_07a, 07d, 086, 134, 0e2, 105 | Demi Shock, Flare, Meteor, Doomsday, Fenrir 2nd, Odin | 1,345 → 730 |
| [fxchoreo_deep_b.md](fxchoreo_deep_b.md) | fx_114, 0fb_0/1, 0e3_0/1 | Ifrit, Madeen, Bahamut (long forms) | 1,150 → 712 |
| [fxchoreo_deep_c.md](fxchoreo_deep_c.md) | fx_17d_0, 3 .. 8 | Ark (long form) | 653 → 428 |
| [fxchoreo_deep_d.md](fxchoreo_deep_d.md) | fx_195, 1bd, 1be, 1a2, 1b3 | Mega Flare, Flames of Hell, Worm Hole, Moonlight Slash, Death1 | 911 → 497 |
| [fxchoreo_deep_e.md](fxchoreo_deep_e.md) | fx_1a8 | Zantetsuken (short Odin) | 100 → 27 |

What stays numeric is the pilot's list ([What stays numeric, and why](#what-stays-numeric-and-why)): otZ biases,
random spreads, fixed-point / angle math and easing, one-off scales and offsets, bone ids, sound entries,
camera-key track numbers, screen-grid / ripple-grid geometry and scroll speeds.

### Open questions (all choreography docs)

**VRAM rects not confirmed** (which model samples the rect; an fxdata `--vram` look or an emulator VRAM dump
would settle each):

- fx_026 Shiva `BARRIER_VRAM_*`: the barrier pieces sampling op81's second scroll is by elimination (s0).
- fx_03a Earthquake `BG_VRAM_*`: which draw samples drawBgOffscreen's (0x1C0, 0x100) 0x100 x 0x100 target (s0).
- fx_038 Comet `SHOCK_RING_U` -0x40 on a 15-bit page: which texel column the ring reads (s0); fx_04f Reflect's
  star / panel pages against the two render targets, inferred from the pages (s0).
- fx_0b8 Atomos `LAVA_2_*` 0x18 x 0x60 (s2); fx_0e1_0 Rebirth Flame `BAND` (0x2A0, 0x180), summon-data model (s3);
  Doomsday's STARS rect, source the common page (s3); Stellar Circle 5's third rect (0x280, 0x100), presumably
  CLOUD_DOME (s4); Earth Shake's smoke rect and every Ark scroll rect ('so' models without a literal tpage; s5,
  deep C).
- **Mega Flare / Bahamut scroll rect vs cylinder v**: fx_0e3_1's (0x340, 0x100, 0x20, 0xC0) and fx_195's (0x200,
  0x100, 0x20, 0xC0) lie in the cylinder's 4-bit page but cover v 0 .. 0xBF while the cylinder samples from v 0xC0
  (deep B, deep D). Also fx_0fb_1 Madeen's two rects (0x360, 0x180) and (0x2A0, 0x180) (deep B).

**Keyframes and meanings that need the emulator** (a trace of the op, Hi_SetSummonMotFrame or a screenshot):

- Countdown / flag slots: fx_0b8 Atomos `work->flags0[]` (about 60 literals, s2); fx_1f2 Grand Cross timer4 ..
  timer58 and the keys that only set them (0x4F, 0x8F, 0xCF, 0x13F, 0x19F, 0x1A7; s7).
- Summon state machines: fx_0e2 / fx_0e3_0 / fx_0e3_1 frame keys (s3, partly done in deep A / B); fx_0fb_1 Madeen's
  25 held rocks (`kk %= 7` rows); fx_114 Ifrit's `countA >= 10 ? 9` trail clamp (deep B); fx_134 Doomsday's orb
  sub-phases, STATE_METEOR spawn timing and the mirror strip (s5, deep A); fx_1cc Sealion's keys in states 0 .. 6
  and its pool task, fx_1fd Fenrir short's op82 ribbon path keys (s7); the six partial modules of slice 6.
- Ark: fx_17d_0 pieces 16 .. 18 (hidden at STATE_ARK 0x28) and piece 16's scale track; fx_17d_3
  `VOLLEY_3_FRAMES` as motion 5's freeze frame; bone 0x21 `FX_ARK_MUZZLE_BONE`; fx_17d_6 `BOLT_RUSH_FRAME`
  against op80's STATE_BURST; fx_17d_7's ring shrink (28) vs op length (0x1E); the bolt / branch trail tables
  (deep C, s5).
- Bones: Odin's 0x3E / 0x43 / 0x4C / 0x5C / 0x60 (fx_105 debris spirals, deep A); whether fx_0ed Roulette's
  RIBBON_BONE 9 is the scythe (s3).
- Death1 fx_1b3: the split models' texture lists (tpages 0x8A .. 0x8F) and caster pages, never rendered (deep D).
- Unnamed arguments: `btlfx_registerEffectModel`'s second argument in every module (deep E); whether fx_126's
  0xFF there is an abr (s4); fx_1fa's LOAD file ids (s7).

**Possible original-game bugs** (commented at the site, code unchanged):

- **Demi Shock disc close** (fx_07a): T_DISC_CLOSE lerps the outer band and disc radius on `timers0[T_TWIST]`,
  already 0, so both snap shut on the first close frame; only the inner band shrinks over the 16 frames (deep A).
- **Odin wind fade-in unreachable** (fx_105): timer24's `>= 0x31` fade-in branch never runs, since the count starts
  at FX_ODIN_WIND_FRAMES 0x30; the streaks start at full brightness (deep A).
- **Odin light band darkens** (fx_105): the "light band" is drawn subtractive (abr 2) with white at the bottom, so
  it darkens the bottom of the screen (deep A).
- **Zantetsuken bounce ignores target height** (fx_1a8): BLAST sets the blade's `pos40.vy` outright, bouncing round
  world y 0; **flame ribbons over-dark**: their brightness ramp is 12 frames but they draw for 16, so on frames
  13 .. 15 the RGB offset goes below -0x80 (deep E).
- **Zantetsuken crystal shards show the captured background** (fx_1a8): drawBgOffscreen writes the battle background
  into exactly the rect where the file uploads the cloud picture the SKY_CRYSTAL models sample (deep E).
- **Worm Hole grid fade** (fx_1be): T_GRID_OUT is loaded at 0x2C but only counts while the grids draw (from 0x40),
  so the grids fade out over their first 16 frames, overriding the fade-in, then show at full brightness (deep D).
