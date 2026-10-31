#ifndef FX_04D_H
#define FX_04D_H

/* Effect module fx_04d (dir 13 file 0x4D). */
#include "btlfx_mod.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx04dPos;

/* pool item: a model that grows, then splits into three pieces that fly apart */
typedef struct {
    s32 piece0; /* index into fx_04d_pieceRots */
    s32 state4; /* state */
    s32 frame8; /* frame */
    Fx04dPos posC[3];
    SVECTOR rot30; /* rotation once split */
    s16 spin38; /* rotation speed */
    s16 spin3A;
    s32 size3C; /* size */
    Pool6CEntry *emit40;
} Fx04dItem; /* 0x44 */

typedef struct {
    s32 state0;
    s32 model4; /* models */
    s32 model8;
    s32 modelC;
    s32 model10;
    s32 model14;
    SVECTOR pos18;
    SVECTOR pos20;
    s16 split28; /* split now */
    s16 count2A;
    s32 frameCount2C; /* incremented every update, never read */
    s32 scroll30;
} Fx04dWork;

extern SVECTOR fx_04d_pieceRots[];
extern Unk800F78A4 *fx_04d_env;
extern BtlModelBuf fx_04d_effModels[];

static s32 fx_04d_op80PoolTask(s32 mode, Fx04dItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx04dWork *work);

#endif
