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
  (`FX_THUNDAGA_CYL_EMITTER`, `FX_FLARE_CALLBACK_EMITTER`) and the define comment ends with `(look pending)`;
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
  `FX_THUNDARA_ORB_SPARKS` and `FX_THUNDAGA_CYL_EMITTER(_2)` sample: probably filled at run time (a VRAM
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
- Generic names with a drawn shape (still pending where the texture is not settled): `FX_ARK_NOISE_SPHERE`,
  `FX_LIMIT_GLOVE_SPHERE`, `FX_BAHAMUT_CORE_GRID` / `TILE_RING` / `PILLAR_STRIP` / `BLAST_PLATE`,
  `FX_LUCKY_SEVEN_SLOT_STRIPS`, `FX_MOONLIGHT_SLASH_SPIKED_SHAPE`, `FX_6_LEGS_POD`.

Left different on purpose (byte-identical, but drawn differently in context: other CLUTs or texture pages):
Magic / Armor / Mental Break orbs (red / blue / pink), Reflect's star (sketch placeholder page) vs Carbuncle's
star crystal, Holy's blue cloud arch vs Holy (Lv4)'s rainbow arch, Aero (breath) swirl vs Rainbow Wind ring,
Psychokinesis' screen trails (black in fx_023, dark bubbles in fx_0eb), Bahamut's blast plate (page 0x119) vs
the short form's smoke arc, Madeen's Y frame vs the short form's feather wings, Thievery's lens flares vs Lucky
Seven's nearly empty sprite line. No module defines a name twice or two names for one index; the remaining
generic names (`*_CYL_EMITTER`, `*_CALLBACK_EMITTER`, `*_FAN_EMITTER` of fans the render does not show) are
all still pending. The helper scripts were throw-away (hash every entry with `FxFile.entries`, group by sha1).

## Open questions (emulator)

Each wants one screenshot of the effect at the frame the C starts the entry (save state: battle, cast the
spell; `docs/emulator.md`), or a VRAM dump while it plays to render the missing texture pages:

- Blank page area (0x1C0, 0x100), filled at run time: `FX_THUNDARA_ORB_SPARKS` (fx_005, fx_049).
- Still transparent with the common / summon pages: `FX_THUNDAGA_CYL_EMITTER(_2)`, `FX_THUNDAGA_CONE_EMITTER`
  (fx_018, fx_085).
- `FX_FIRAGA_CONE_GLOW` (fx_007, fx_048): one dim dark-red frame; the blend mode decides the look.
- `FX_THUNDAGA_DISC` (fx_018, fx_085): 15-bit texture page 0x119, drawn there at run time.

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
| fx_005 | 0x1E | `FX_THUNDARA_ORB_SPARKS` | kind/usage (look pending) | sprites: blank, samples page (0x1C0, 0x100) which the file leaves zero, filled at run time |
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
| fx_007 | 0x15 | `FX_FIRAGA_CONE_GLOW` | kind/usage (look pending) | emitter: cone, one dim dark-red particle frame; the blend mode decides the look |
| fx_007 | 0x16 | `FX_FIRAGA_FLAME_RIBBON_0` | render (fxdata d6121033) | 'so' model: flat flame-textured ribbon piece, drawn coiled 18 times as the fire column |
| fx_007 | 0x17 | `FX_FIRAGA_FLAME_RIBBON_1` | render (fxdata d6121033) | 'so' model: flat flame-textured ribbon piece |
| fx_007 | 0x18 | `FX_FIRAGA_FLAME_RIBBON_2` | render (fxdata d6121033) | 'so' model: flat flame-textured ribbon piece |
| fx_007 | 0x19 | `FX_FIRAGA_FLAME_RIBBON_3` | render (fxdata d6121033) | 'so' model: flat flame-textured ribbon piece |
| fx_008 | 0x3 | `FX_CURE_STAR_FLARES` | render | sprite line: blue star flares along a path |
| fx_008 | 0x4 | `FX_CURE_SPARKLE_SWIRL` | render | emitter: star-sparkle trails orbiting the target, sparkles and bubbles |
| fx_009 | 0x3 | `FX_CURE_SPARKLE_SWIRL` | render | emitter: star-sparkle trails spiralling up, sparkles and bubbles |
| fx_018 | 0x7 | `FX_THUNDAGA_CYL_EMITTER` | kind/usage (look pending) | emitter: cylinder particles, still render all transparent with the summon/common pages |
| fx_018 | 0xA | `FX_THUNDAGA_CYL_EMITTER_2` | kind/usage (look pending) | emitter: cylinder particles, still render all transparent with the summon/common pages |
| fx_018 | 0x10 | `FX_THUNDAGA_DISC` | kind/usage (look pending) | model: flat octagon, 15-bit texture page 0x119 holds the file's 4-bit pages, so it samples something drawn there at run time |
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
| fx_018 | 0x26 | `FX_THUNDAGA_CONE_EMITTER` | kind/usage (look pending) | emitter: cone of sprites, still render all transparent with the summon/common pages |
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
| fx_048 | 0x18 | `FX_FIRAGA_CONE_GLOW` | kind/usage (look pending) | emitter: cone, one dim dark-red particle frame; the blend mode decides the look (same call site as fx_007 entry 0x15) |
| fx_048 | 0x19 | `FX_FIRAGA_EMBER_CONE` | render (fxdata d6121033) | emitter: cone of red and grey ember dots (byte-identical to fx_007 entry 0x14) |
| fx_049 | 0x2 | `FX_THUNDARA_IMPACT_FLASH` | render | emitter: four-point star flash, white rings, sparks |
| fx_049 | 0x6 | `FX_THUNDARA_IMPACT_RING` | render | emitter: white-cyan gouraud rings |
| fx_049 | 0x15 | `FX_THUNDARA_ORB_EMITTER` | render | emitter: helix whose callback element draws the orb (0x16, 0x1C-0x1F) |
| fx_049 | 0x18 | `FX_THUNDARA_SPARK_DOTS` | render (fxdata d6121033) | emitter: white spark dots (btlfx_emitDots) |
| fx_049 | 0x19 | `FX_THUNDARA_ORB_FADE` | render | particles: blue energy sphere fading to orange |
| fx_049 | 0x1A | `FX_THUNDARA_ORB` | render | particles: bright blue energy sphere |
| fx_049 | 0x1B | `FX_THUNDARA_ORB_SPARKS` | kind/usage (look pending) | sprites: blank, samples page (0x1C0, 0x100) which the file leaves zero, filled at run time |
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
| fx_085 | 0x8 | `FX_THUNDAGA_CONE_EMITTER` | kind/usage (look pending) | emitter: cone of sprites, still render all transparent with the summon/common pages (byte-identical to fx_018 entry 0x26) |
| fx_085 | 0xE | `FX_THUNDAGA_SPARK_DOTS` | render (fxdata d6121033) | emitter: white spark dots (btlfx_emitDots) (byte-identical to fx_018 entry 0x27) |
| fx_085 | 0xF | `FX_THUNDAGA_DISC` | kind/usage (look pending) | model: flat octagon, 15-bit texture page 0x119 holds the file's 4-bit pages, so it samples something drawn there at run time (byte-identical to fx_018 entry 0x10) |
| fx_085 | 0x10 | `FX_THUNDAGA_LIGHTNING_BOLT` | render | particles: wide blue-white lightning bolt (byte-identical to fx_018 entry 0x17) |
| fx_085 | 0x11 | `FX_THUNDAGA_THIN_BOLT` | render | particles: thin cyan lightning bolt (byte-identical to fx_018 entry 0x20) |
| fx_085 | 0x15 | `FX_THUNDAGA_LIGHTNING_ARCS` | render (fxdata d6121033) | emitter: four fans drawn as cyan and blue zig-zag lightning arcs (byte-identical to fx_018 entry 0x21) |
| fx_085 | 0x18 | `FX_THUNDAGA_SPARK_BALL` | render | particles: golden plasma ball, 8 frames (byte-identical to fx_018 entry 0x1b) |
| fx_085 | 0x1B | `FX_THUNDAGA_CYL_EMITTER` | kind/usage (look pending) | emitter: cylinder particles, still render all transparent with the summon/common pages (byte-identical to fx_018 entry 0x7) |
| fx_085 | 0x1C | `FX_THUNDAGA_CYL_EMITTER_2` | kind/usage (look pending) | emitter: cylinder particles, still render all transparent with the summon/common pages (byte-identical to fx_018 entry 0xa) |
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
