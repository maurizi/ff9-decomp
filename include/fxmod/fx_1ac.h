#ifndef FX_1AC_H
#define FX_1AC_H

/* Effect module fx_1ac (dir 13 file 0x1ac): enemy Virus Crunch (Chimera). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1ac effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_VIRUS_CRUNCH_MOTHS      0x1 /* emitter: orbiting gold / green / orange moth-wing quads, 24 frames */
#define FX_VIRUS_CRUNCH_MOTHS_2    0x2 /* emitter: gold / orange / green moth-wing quads, 24 frames */
#define FX_VIRUS_CRUNCH_CYAN_CLOUD 0x3 /* emitter: white-grey gouraud rings + teal-cyan cloud quads */
#define FX_VIRUS_CRUNCH_GREY_TRAIL 0x5 /* emitter: flat trail, grey faceted strip */

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

static s32 fx_1ac_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
