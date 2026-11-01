#ifndef FX_193_H
#define FX_193_H

/* Effect module fx_193 (dir 13 file 0x193): enemy Counter Horn (Antlion). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
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
extern SVECTOR fx_193_op80From0;
extern SVECTOR fx_193_op80To0;
extern SVECTOR fx_193_op80From1;
extern SVECTOR fx_193_op80To1;
extern SVECTOR fx_193_emitPos;
extern s32 fx_193_op80Trails0[];
extern s32 fx_193_op80Trails1[];

static s32 fx_193_op80ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fx_193_op80ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fxm_wobbleParticle(Pool30Entry *e, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 amp, s32 frame, s32 frames);

#endif
