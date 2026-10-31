#ifndef FX_004_H
#define FX_004_H

/* Effect module fx_004 (dir 13 file 0x04). */
#include "btlfx_mod.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx004Pos;

/* pool item: a model that grows, then splits into three pieces that fly apart */
typedef struct {
    s32 piece0; /* index into fx_004_pieceRots */
    s32 state4; /* state */
    s32 frame8; /* frame */
    Fx004Pos posC[3];
    SVECTOR rot30; /* rotation once split */
    s16 spin38; /* rotation speed */
    s16 spin3A;
    s32 size3C; /* size */
    Pool6CEntry *emit40;
} Fx004Item; /* 0x44 */

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
} Fx004Work;

extern SVECTOR fx_004_pieceRots[];
extern Unk800F78A4 *fx_004_env;
extern BtlModelBuf fx_004_effModels[];

static s32 fx_004_op80PoolTask(s32 mode, Fx004Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx004Work *work);

#endif
