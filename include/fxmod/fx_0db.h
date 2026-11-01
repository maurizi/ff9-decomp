#ifndef FX_0DB_H
#define FX_0DB_H

/* Effect module fx_0db (dir 13 file 0xDB): Sword Magic Blizzara. Ice shards (pool items) that grow out of the
 * caster and split in three. Evidence: [data] the party ability / item tables and enemy sequences that play this
 * file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

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
