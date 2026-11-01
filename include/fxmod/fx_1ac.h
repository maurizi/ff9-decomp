#ifndef FX_1AC_H
#define FX_1AC_H

/* Effect module fx_1ac (dir 13 file 0x1ac): enemy Virus Crunch (Chimera). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 timer0[8]; /* countdowns */
    SVECTOR pos20; /* caster position */
    SVECTOR pos28; /* target position */
    SVECTOR pos30; /* bone position */
    SVECTOR rot38; /* target rotation */
    u8 pad40[0x44 - 0x40];
    SVECTOR rot44; /* saved target rotation */
    Pool6CEntry *emit4C[5];
} Fx1acWork; /* 0x60 */

extern Unk800F78A4 *fx_1ac_env;
extern Pool6CEntry *fx_1ac_emitter;
extern s32 fx_1ac_op80Trails[];
extern SVECTOR fx_1ac_emitPos0;
extern SVECTOR fx_1ac_emitPos1;
extern SVECTOR fx_1ac_emitPos2;
extern SVECTOR fx_1ac_emitPos3;
extern SVECTOR fx_1ac_emitPos4;
extern SVECTOR fx_1ac_targetPos;
extern SVECTOR fx_1ac_lastParticlePos;

static s32 fx_1ac_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);

#endif
