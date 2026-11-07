#ifndef FX_0EB_H
#define FX_0EB_H

/* Effect module fx_0eb (dir 13 file 0x0EB): enemy Psychokinesis (Nova Dragon, ...). The target is lifted, held and
 * dropped back (btlfx_setUnitPos) among emitters. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0eb effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PSYCHOKINESIS_WHITE_RINGS_2 0x1 /* emitter: white gouraud rings (byte-identical to fx_023 entry 0x3) */
#define FX_PSYCHOKINESIS_WHITE_RINGS 0x2 /* emitter: white gouraud rings (byte-identical to fx_023 entry 0x1) */
#define FX_PSYCHOKINESIS_PSYCHIC_ORBS 0x3 /* emitter: homing violet-to-pink orbs */
#define FX_PSYCHOKINESIS_GREY_RINGS 0x7 /* emitter: white-to-grey gouraud rings, 8 frames */
#define FX_PSYCHOKINESIS_DARK_BUBBLES 0x8 /* emitter: dark bubble screen trails */

/* fx_0eb choreography (docs/fxdata.md, "Choreography literals"): op80's lift, hold and drop of the target, the
 * trail buffer and the op's length. */
#define FX_PSYCHOKINESIS_LIFT_HEIGHT 0x3E8 /* the target is lifted to this height (PSYCHIC_ORBS home on it there) */
#define FX_PSYCHOKINESIS_LIFT_FRAMES 0xC   /* over this many frames from frame 0 (liftTimer34), then held */
#define FX_PSYCHOKINESIS_DROP_FRAME  0x1C  /* the target drops (WHITE_RINGS_2), */
#define FX_PSYCHOKINESIS_DROP_FRAMES 3     /* over this many frames (dropTimer36); on landing GREY_RINGS and DARK_BUBBLES */
#define FX_PSYCHOKINESIS_FRAMES      0x50  /* op80's length (the op ends) */
#define FX_PSYCHOKINESIS_DARK_BUBBLES_TRAIL_SLOTS  0x3C /* FX_PSYCHOKINESIS_DARK_BUBBLES' trail buffer fx_0eb_op80Trails: slots of */
#define FX_PSYCHOKINESIS_DARK_BUBBLES_TRAIL_STRIDE 0x18 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

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
} Fx0ebWork; /* 0x3C */

extern Unk800F78A4 *fx_0eb_env;
extern s32 fx_0eb_op80Trails[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
