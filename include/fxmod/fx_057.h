#ifndef FX_057_H
#define FX_057_H

/* Effect module fx_057 (dir 13 file 0x57). */
#include "btlfx_mod.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx057Vec3;

typedef struct {
    s32 state0;
    s32 frame4; /* frame */
    s32 targetOtZ8; /* target 0's model radius / 6 (btlfx_addRadiusSixth), depth bias of its emitters and rings */
    SVECTOR posC; /* position */
    SVECTOR pos14;
    Fx057Vec3 pos1C;
    s32 model28; /* model */
} Fx057Work81; /* 0x2C */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 model8; /* model */
    s32 modelC; /* model */
    SVECTOR pos10; /* position */
    Fx057Vec3 pos18; /* model position */
    SVECTOR pos24;
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    Pool6CEntry *emit34;
} Fx057Work80; /* 0x38 */

extern SVECTOR fx_057_targetPos;
extern s32 fx_057_trails[];
extern s32 fx_057_fade; /* fade */
extern Pool6CEntry *fx_057_emitter1;
extern BtlModelBuf fx_057_effModels[];
extern Unk800F78A4 *fx_057_env;
extern SVECTOR fx_057_camRest; /* camera rest position */
extern SVECTOR fx_057_headPos; /* particle head position */
extern Pool6CEntry *fx_057_emitter0;
extern Unk80100C78Entry *fx_057_task;

static void fx_057_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *e, SVECTOR *pos, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 t,
                   s32 n);

#endif
