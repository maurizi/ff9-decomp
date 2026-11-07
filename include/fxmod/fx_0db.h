#ifndef FX_0DB_H
#define FX_0DB_H

/* Effect module fx_0db (dir 13 file 0xDB): Sword Magic Blizzara. Ice shards (pool items) that grow out of the
 * caster and split in three. Evidence: [data] the party ability / item tables and enemy sequences that play this
 * file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0db effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_BLIZZARA_ICE_REFLECTION  0xB /* model: crystal shell drawn with the reflection map (byte-identical to fx_004 entry 0xB) */
#define FX_SWDMAG_BLIZZARA_FROST_MIST      0xD /* emitter: grey frost mist puffs (byte-identical to fx_004 entry 0xD) */
#define FX_SWDMAG_BLIZZARA_SHARD_MIST      0xE /* emitter: grey mist trailing each flying shard (pool item) (byte-identical to fx_004 entry 0xE) */
#define FX_SWDMAG_BLIZZARA_ICE_FLARES_2    0xF /* emitter: cyan ice glints (byte-identical to fx_004 entry 0xF) */
#define FX_SWDMAG_BLIZZARA_ICE_FLARES      0x10 /* emitter: cyan ice glints, four elements (byte-identical to fx_004 entry 0x10) */
#define FX_SWDMAG_BLIZZARA_FREEZE_BURST    0x11 /* emitter: white-pink rings + spark sprites (byte-identical to fx_004 entry 0x11) */
#define FX_SWDMAG_BLIZZARA_FROST_RING      0x12 /* emitter: lilac-blue gouraud rings (byte-identical to fx_004 entry 0x12) */
#define FX_SWDMAG_BLIZZARA_ICE_CRYSTAL     0x13 /* model: long ice crystal spike (ice texture), splits in three (byte-identical to fx_004 entry 0x13) */
#define FX_SWDMAG_BLIZZARA_WHITE_RING      0x14 /* emitter: white gouraud ring at the start (byte-identical to fx_004 entry 0x14) */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_0 0x15 /* model: first piece of the split crystal (byte-identical to fx_004 entry 0x15) */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_1 0x16 /* model: second piece of the split crystal (byte-identical to fx_004 entry 0x16) */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_2 0x17 /* model: third piece of the split crystal (byte-identical to fx_004 entry 0x17) */
#define FX_SWDMAG_BLIZZARA_SHATTER_WAVE    0x19 /* emitter: blue ripple wave (byte-identical to fx_004 entry 0x19) */
#define FX_SWDMAG_BLIZZARA_SWORD_FLASH     0x1A /* rings: white 8-segment rings flashed rotating at the sword (op84) */
#define FX_SWDMAG_BLIZZARA_SWORD_GLINT     0x1B /* rings: white 6-segment rings flashed at the sword (op84, first draw) */

/* fx_0db choreography (docs/fxdata.md, "Choreography literals"; as fx_004 Blizzara): op80's states and their
 * lengths, the crystal pool items' phases, the VRAM rect of the reflection map, the model textures; op84's sword
 * flash. */
#define FX_SWDMAG_BLIZZARA_STATE_CRYSTALS 0 /* op80: one FX_SWDMAG_BLIZZARA_ICE_CRYSTAL pool item spawns per frame */
#define FX_SWDMAG_BLIZZARA_STATE_GLEAM    1 /* the crystals stand, the reflection map scrolls slower and slower */
#define FX_SWDMAG_BLIZZARA_STATE_SHATTER  2 /* split24 set: the crystals split into their three pieces and fly apart */
#define FX_SWDMAG_BLIZZARA_CRYSTALS_FRAMES    9  /* STATE_CRYSTALS's length (spawns on frames 0..9) */
#define FX_SWDMAG_BLIZZARA_GLEAM_FRAMES       14 /* STATE_GLEAM's length; at its end the freeze burst, frost ring and flares start */
#define FX_SWDMAG_BLIZZARA_FLASH_FRAMES       5  /* STATE_SHATTER: additive blue screen flash fading out over these frames */
#define FX_SWDMAG_BLIZZARA_SHATTER_FRAMES     21 /* STATE_SHATTER's length (the op ends) */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_COUNT      10 /* crystals: pool items, fx_0db_shardAngles entries */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_STATE_GROW  0 /* pool item: the crystal grows out */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_STATE_HOLD  1 /* pool item: it stands until split24 */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_STATE_SPLIT 2 /* pool item: its three pieces fly apart and fade */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_GROW_FRAMES 4 /* pool item: CRYSTAL_STATE_GROW's length */
#define FX_SWDMAG_BLIZZARA_PIECES_FRAMES      20 /* pool item: CRYSTAL_STATE_SPLIT's length */
#define FX_SWDMAG_BLIZZARA_MODEL_COUNT        6  /* btlfx_initEffModels slots (5 registered: crystal, 3 pieces, reflection; the reflection overwrites model10) */
#define FX_SWDMAG_BLIZZARA_ICE_CRYSTAL_TPAGE 0x9A /* Hi_RegisterTexEffModel texture: 8-bit page (0x280, 0x100), abr 0 */
#define FX_SWDMAG_BLIZZARA_ICE_CRYSTAL_CLUT  0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_SWDMAG_BLIZZARA_ICE_CRYSTAL_U     0x80 /* texture origin (u, v) in the page */
#define FX_SWDMAG_BLIZZARA_ICE_CRYSTAL_V     0x0
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_0_TPAGE 0x9A /* Hi_RegisterTexEffModel texture: 8-bit page (0x280, 0x100), abr 0 */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_0_CLUT  0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_0_U     0x80 /* texture origin (u, v) in the page */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_0_V     0x0
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_1_TPAGE 0x9A /* Hi_RegisterTexEffModel texture: 8-bit page (0x280, 0x100), abr 0 */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_1_CLUT  0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_1_U     0x80 /* texture origin (u, v) in the page */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_1_V     0x0
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_2_TPAGE 0x9A /* Hi_RegisterTexEffModel texture: 8-bit page (0x280, 0x100), abr 0 */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_2_CLUT  0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_2_U     0x80 /* texture origin (u, v) in the page */
#define FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_2_V     0x0
#define FX_SWDMAG_BLIZZARA_ICE_REFLECTION_TPAGE 0xBA /* Hi_RegisterTexEffModel texture: 8-bit page (0x280, 0x100), abr 1, the reflection rect below */
#define FX_SWDMAG_BLIZZARA_ICE_REFLECTION_CLUT  0x3D40 /* CLUT at (0x0, 0xF5) */
#define FX_SWDMAG_BLIZZARA_ICE_REFLECTION_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_BLIZZARA_ICE_REFLECTION_V     0x80
/* FX_SWDMAG_BLIZZARA_ICE_REFLECTION samples (ICE_REFLECTION_TPAGE / _U / _V) the rect the file fills with a "dummy"
 * placeholder; every frame btlfx_drawScrollMove copies the reflection texture from (SRC_X, SRC_Y) into it, scrolled
 * vertically by scroll2C. */
#define FX_SWDMAG_BLIZZARA_REFLECTION_VRAM_X 0x280
#define FX_SWDMAG_BLIZZARA_REFLECTION_VRAM_Y 0x180
#define FX_SWDMAG_BLIZZARA_REFLECTION_VRAM_W 0x40
#define FX_SWDMAG_BLIZZARA_REFLECTION_VRAM_H 0x80
#define FX_SWDMAG_BLIZZARA_REFLECTION_SRC_X  0x280
#define FX_SWDMAG_BLIZZARA_REFLECTION_SRC_Y  0x100
#define FX_SWDMAG_BLIZZARA_SWORD_STATE_APPEAR  0 /* op84: SWORD_GLINT / SWORD_FLASH grow in at the weapon */
#define FX_SWDMAG_BLIZZARA_SWORD_STATE_FLICKER 1 /* op84: they flicker and turn */
#define FX_SWDMAG_BLIZZARA_SWORD_STATE_FADE    2 /* op84: they spread and fade (the op ends) */
#define FX_SWDMAG_BLIZZARA_SWORD_APPEAR_FRAMES  3 /* SWORD_STATE_APPEAR's length */
#define FX_SWDMAG_BLIZZARA_SWORD_FLICKER_FRAMES 9 /* SWORD_STATE_FLICKER's length */
#define FX_SWDMAG_BLIZZARA_SWORD_FADE_FRAMES    9 /* SWORD_STATE_FADE's length */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0dbWork84;

extern Unk800F78A4 *fx_0db_env;
extern BtlModelBuf fx_0db_effModels[];

typedef struct {
    s32 state0; /* state */
    s32 model4; /* models */
    s32 model8;
    s32 modelC;
    s32 model10;
    SVECTOR pos14; /* caster body position */
    SVECTOR pos1C; /* caster position */
    s16 split24;
    s16 spawnCount26; /* particles spawned */
    s32 frame28; /* frame counter */
    s32 scroll2C;
    s32 unk30; /* unused */
} Fx0dbWork80; /* 0x34 */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0dbPos;
typedef struct {
    s32 shard0; /* index into fx_0db_shardAngles */
    s32 state4; /* state */
    s32 frame8; /* frame */
    Fx0dbPos posC[3];
    SVECTOR rot30; /* rotation once split */
    s16 spinX38; /* rotation speed */
    s16 spinY3A;
    s32 size3C; /* size */
    Pool6CEntry *emit40;
} Fx0dbItem; /* 0x44 */
extern SVECTOR fx_0db_shardAngles[];
static s32 fx_0db_op80PoolTask(s32 mode, Fx0dbItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx0dbWork80 *work);

#endif
