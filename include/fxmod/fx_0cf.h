#ifndef FX_0CF_H
#define FX_0CF_H

/* Effect module fx_0cf (dir 13 file 0xCF): Stock Break. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* position: env->targets->centerX..Z */
} Fx0cfWork80;

typedef struct {
    SVECTOR pos0; /* unit 0x10 body position */
    SVECTOR rot8; /* angles from pos0 to pos18, turned around */
    SVECTOR pos10; /* pos18 moved 0x320 along rot8 */
    SVECTOR pos18; /* position: env->targets->centerX..Z */
    SVECTOR pos20; /* start position */
} Fx0cfWork81;

extern Unk800F78A4 *fx_0cf_env;

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static s32 fxm_moveUnitArc(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start);
static s32 fxm_turnUnitBy(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta);
static s32 fxm_turnUnitToward(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang);

#endif
