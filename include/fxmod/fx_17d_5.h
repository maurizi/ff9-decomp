#ifndef FX_17D_5_H
#define FX_17D_5_H

/* Effect module fx_17d_5 (dir 13 file 0x17d): Ark, full summon (load group 5). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    s32 vx, vy, vz;
} Fx17d5Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 count8; /* sparks spawned */
    SVECTOR posC; /* spark origin */
    SVECTOR target14; /* spark target */
    SVECTOR angle1C; /* spark angles */
    SVECTOR pos24;
    u8 pad2C[0x3C - 0x2C];
    SVECTOR pos3C; /* camera */
    Fx17d5Vec3 pos44; /* glow position */
    SVECTOR rot50; /* glow rotation */
    Fx17d5Vec3 scale58; /* glow scale */
    Fx17d5Vec3 pos64; /* summon position */
    Fx17d5Vec3 scale70; /* summon scale */
    SVECTOR rot7C; /* summon rotation */
    s32 model84; /* models */
    s32 model88;
    s32 model8C;
    s32 model90;
    s32 model94;
} Fx17d5Work80; /* 0x98 */

typedef struct {
    s16 state0; /* state */
    s16 speed2; /* speed */
    s16 turn4; /* turn rate */
    s16 index6; /* index */
    SVECTOR pos8; /* position */
    s16 angle10; /* angles (with angle12); also the kind (0-2) at spawn */
    s16 angle12;
    s16 flag14; /* homing done */
    s16 unk16; /* unused */
    u8 *trail18; /* trail */
    s16 pos1C; /* target */
    s16 targetY1E; /* target y (pos1C: target x) */
    s16 targetZ20; /* target z */
    s16 width22; /* trail width */
} Fx17d5Spark; /* 0x24 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x1A4];
        BtlMotion *motion1A4;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx17d5Summon;

extern SVECTOR fx_17d_5_targetPos; /* aim point */
extern Unk800F78A4 *fx_17d_5_env;
extern BtlModelBuf fx_17d_5_effModels[];
extern u8 fx_17d_5_sparkTrails[][0xCC]; /* spark trails */

static s32 fx_17d_5_op80PoolTask(s32 mode, Fx17d5Spark *w, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx17d5Work80 *parent);

#endif
