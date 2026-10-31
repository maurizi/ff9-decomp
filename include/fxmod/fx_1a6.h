#ifndef FX_1A6_H
#define FX_1A6_H

/* Effect module fx_1a6 (dir 13 file 0x1a6). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
} Fx1a6Work;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* target body position */
    SVECTOR pos20; /* bone position */
    SVECTOR pos28;
    u8 pad30[0x38 - 0x30];
    SVECTOR unk38; /* angles */
    SVECTOR unk40;
    s32 unk48; /* index */
} Fx1a6Work80; /* 0x4C */

extern u16 D_801E823C[3]; /* files */
extern s32 (*D_801E8244[3])();
extern Unk800F78A4 *fx_1a6_env;
extern s32 D_801E82A8[3][0x5A];
extern s32 D_801E86E8[3][0x1D0];
extern s32 D_801E9CA8[3][0x560];
extern SVECTOR D_801EDD28[3]; /* from */
extern SVECTOR D_801EDD48[3]; /* to */
extern SVECTOR D_801EDD68[3];

s32 func_801E77C4(Pool30Entry *e, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 frame, s32 frames);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startMotion2IfActive(s32 unit);

/* propagated */
s32 fx_1a6_op80ParticleMotion0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_1a6_op80ParticleMotion1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_1a6_op80ParticleMotion2(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);

#endif
