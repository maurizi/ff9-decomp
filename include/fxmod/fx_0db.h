#ifndef FX_0DB_H
#define FX_0DB_H

/* Effect module fx_0db (dir 13 file 0xDB): Sword Magic Blizzara. Ice shards (pool items) that grow out of the
 * caster and split in three. Evidence: [data] the party ability / item tables and enemy sequences that play this
 * file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0db effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BLIZZARA_SWORD_ICE_REFLECTION  0xB /* model: crystal shell drawn with the reflection map (byte-identical to fx_004 entry 0xB) */
#define FX_BLIZZARA_SWORD_FROST_MIST      0xD /* emitter: grey frost mist puffs (byte-identical to fx_004 entry 0xD) */
#define FX_BLIZZARA_SWORD_SHARD_MIST      0xE /* emitter: grey mist trailing each flying shard (pool item) (byte-identical to fx_004 entry 0xE) */
#define FX_BLIZZARA_SWORD_ICE_FLARES_2    0xF /* emitter: cyan ice glints (byte-identical to fx_004 entry 0xF) */
#define FX_BLIZZARA_SWORD_ICE_FLARES      0x10 /* emitter: cyan ice glints, four elements (byte-identical to fx_004 entry 0x10) */
#define FX_BLIZZARA_SWORD_FREEZE_BURST    0x11 /* emitter: white-pink rings + spark sprites (byte-identical to fx_004 entry 0x11) */
#define FX_BLIZZARA_SWORD_FROST_RING      0x12 /* emitter: lilac-blue gouraud rings (byte-identical to fx_004 entry 0x12) */
#define FX_BLIZZARA_SWORD_ICE_CRYSTAL     0x13 /* model: long ice crystal spike (ice texture), splits in three (byte-identical to fx_004 entry 0x13) */
#define FX_BLIZZARA_SWORD_WHITE_RING      0x14 /* emitter: white gouraud ring at the start (byte-identical to fx_004 entry 0x14) */
#define FX_BLIZZARA_SWORD_CRYSTAL_PIECE_0 0x15 /* model: first piece of the split crystal (byte-identical to fx_004 entry 0x15) */
#define FX_BLIZZARA_SWORD_CRYSTAL_PIECE_1 0x16 /* model: second piece of the split crystal (byte-identical to fx_004 entry 0x16) */
#define FX_BLIZZARA_SWORD_CRYSTAL_PIECE_2 0x17 /* model: third piece of the split crystal (byte-identical to fx_004 entry 0x17) */
#define FX_BLIZZARA_SWORD_SHATTER_WAVE    0x19 /* emitter: blue ripple wave (byte-identical to fx_004 entry 0x19) */
#define FX_BLIZZARA_SWORD_BLADE_FLASH     0x1A /* rings: white 8-segment rings flashed rotating at the sword (op84) */
#define FX_BLIZZARA_SWORD_BLADE_GLINT     0x1B /* rings: white 6-segment rings flashed at the sword (op84, first draw) */

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
} Fx0dbWork80;

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
