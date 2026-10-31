#ifndef FX_0DC_H
#define FX_0DC_H

/* Effect module fx_0dc (dir 13 file 0xDC): Sword Magic Blizzaga. Ice shards (pool items) that break into flying
 * pieces. Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* time */
    SVECTOR pos8; /* caster bone position */
} Fx0dcWork84;

extern Unk800F78A4 *fx_0dc_env;

s32 fx_0dc_op84(s32 mode, Fx0dcWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

/* propagated */

extern s16 fx_0dc_shardDists[];
void fx_0dc_shardPoint(s16 *a, s16 *b, s32 i, s16 *out);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0dcVec3;

/* one shard, a pool item */
typedef struct {
    s16 unk0; /* state */
    s16 flying2; /* flying */
    s32 shard4; /* index */
    s32 frame8; /* frame */
    Fx0dcVec3 posC; /* position << 4 */
    SVECTOR pos18; /* velocity */
    SVECTOR rot20; /* rotation */
    s16 spinX28; /* rotation speed */
    s16 spinY2A;
    s32 model2C; /* models */
    s32 model30;
    Pool6CEntry *emit34;
} Fx0dcItem; /* 0x38 */

typedef struct {
    u8 pad0[0x20];
    s32 model20[22]; /* models */
    s16 split78;
} Fx0dcWork;

extern SVECTOR fx_0dc_shardAngles[];
static s32 fx_0dc_op80PoolTask(s32 mode, Fx0dcItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx0dcWork *work);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 model8; /* models */
    s32 modelC;
    s32 model10;
    s32 model14;
    s32 model18;
    s32 model1C;
    s32 model20[14]; /* shard models */
    SVECTOR pos58; /* target position */
    SVECTOR pos60;
    SVECTOR pos68;
    SVECTOR pos70;
    s16 split78;
    s16 spawnCount7A; /* shards spawned */
    s32 unk7C;
    s32 unk80;
    Pool6CEntry *emit84;
} Fx0dcWork80; /* 0x88 */

extern s32 fx_0dc_shardModelBlocks[14];
extern BtlModel fx_0dc_effModels[];

#endif
