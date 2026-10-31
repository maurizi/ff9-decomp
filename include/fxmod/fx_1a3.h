#ifndef FX_1A3_H
#define FX_1A3_H

/* Effect module fx_1a3 (dir 13 file 0x1a3). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* caster rotation */
    SVECTOR pos10; /* target body position */
    SVECTOR pos18;
    SVECTOR pos20;
} Fx1a3Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster position */
    SVECTOR pos14;
} Fx1a3Work81;

extern Unk800F78A4 *fx_1a3_env;
extern s32 fx_1a3_op80Trails[];
extern s32 fx_1a3_op81Trails0[];
extern s32 fx_1a3_op81Trails1[];
extern SVECTOR D_801E93F4; /* from */
extern SVECTOR D_801E93FC; /* to */
extern SVECTOR fx_1a3_emitPos;

static s32 fx_1a3_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);

#endif
