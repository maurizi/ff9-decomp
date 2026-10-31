#ifndef FX_193_H
#define FX_193_H

/* Effect module fx_193 (dir 13 file 0x193). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position */
} Fx193Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* target body position */
    SVECTOR pos18; /* target position */
} Fx193Work81;

extern Unk800F78A4 *fx_193_env;
extern SVECTOR D_801E7CBC;
extern SVECTOR D_801E7CC4;
extern SVECTOR D_801E7CCC;
extern SVECTOR D_801E7CD4;
extern SVECTOR D_801E7CDC;
extern s32 D_801E7CE4[];
extern s32 D_801E80A4[];

static s32 fx_193_op80ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
static s32 fx_193_op80ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
static s32 fxm_wobbleParticle(Pool30Entry *e, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 amp, s32 frame, s32 frames);

#endif
