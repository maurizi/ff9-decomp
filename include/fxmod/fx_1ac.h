#ifndef FX_1AC_H
#define FX_1AC_H

/* Effect module fx_1ac (dir 13 file 0x1ac): enemy Virus Crunch (Chimera). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0[8]; /* countdowns */
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
extern SVECTOR D_801E8580;
extern SVECTOR D_801E8588;
extern SVECTOR D_801E8590;
extern SVECTOR D_801E8598;
extern SVECTOR D_801E85A0;
extern SVECTOR D_801E85A8;
extern SVECTOR D_801E85B0;

static s32 fx_1ac_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);

#endif
