#ifndef FX_1A6_H
#define FX_1A6_H

/* Effect module fx_1a6 (dir 13 file 0x1a6): enemy Water-gun (Kraken, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
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
    SVECTOR angle38; /* angles */
    SVECTOR angle40;
    s32 index48; /* index */
} Fx1a6Work80; /* 0x4C */

extern u16 fx_1a6_emitFileIds[3]; /* files */
extern s32 (*fx_1a6_op80ParticleMotions[3])();
extern Unk800F78A4 *fx_1a6_env;
extern s32 fx_1a6_op80Trails0[3][0x5A];
extern s32 fx_1a6_op80Trails1[3][0x1D0];
extern s32 fx_1a6_op80Trails2[3][0x560];
extern SVECTOR fx_1a6_motionFrom[3]; /* from */
extern SVECTOR fx_1a6_motionTo[3]; /* to */
extern SVECTOR fx_1a6_emitPos0[3];

static s32 fxm_lerpParticle(Pool30Entry *e, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 frame, s32 frames);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHitMotionIfActive(s32 unit);

/* propagated */
s32 fx_1a6_op80ParticleMotion0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_1a6_op80ParticleMotion1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_1a6_op80ParticleMotion2(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);

#endif
