#ifndef FX_0EB_H
#define FX_0EB_H

/* Effect module fx_0eb (dir 13 file 0x0EB): enemy Psychokinesis (Nova Dragon, ...). The target is lifted, held and
 * dropped back (btlfx_setUnitPos) among emitters. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0eb effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PSYCHOKINESIS_WHITE_RING   0x1 /* emitter: white gouraud rings */
#define FX_PSYCHOKINESIS_WHITE_RING_2 0x2 /* emitter: white gouraud rings */
#define FX_PSYCHOKINESIS_PSYCHIC_ORBS 0x3 /* emitter: homing violet-to-pink orbs */
#define FX_PSYCHOKINESIS_SHOCK_RING   0x7 /* emitter: white-to-grey gouraud rings, 8 frames */
#define FX_PSYCHOKINESIS_DARK_BUBBLES 0x8 /* emitter: dark bubble screen trails */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* target position */
    SVECTOR pos1C; /* target home position */
    SVECTOR pos24;
    SVECTOR emitTarget2C;
    s16 liftTimer34; /* lift countdown */
    s16 dropTimer36; /* drop countdown */
    s16 hold38; /* hold up */
} Fx0ebWork;

extern Unk800F78A4 *fx_0eb_env;
extern s32 fx_0eb_op80Trails[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
