#ifndef FX_023_H
#define FX_023_H

/* Effect module fx_023 (dir 13 file 0x23). */
#include "btlfx_mod.h"

/* fx_023 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PSYCHOKINESIS_WHITE_RINGS   0x1 /* emitter: white gouraud rings */
#define FX_PSYCHOKINESIS_GLYPH_ORBS    0x2 /* emitter: pink orbs, each with a kana glyph, homing */
#define FX_PSYCHOKINESIS_WHITE_RINGS_2 0x3 /* emitter: white gouraud rings */
#define FX_PSYCHOKINESIS_GREY_RINGS    0x7 /* emitter: white rings fading to grey */
#define FX_PSYCHOKINESIS_LIGHT_RAYS    0x8 /* emitter: two screen trails, the white light rays bursting from the target (byte-identical to fx_0eb entry 0x8, which renders as dark bubbles from that file's pages); runtime (docs/runtime-findings.md effect-data pass 2) */

/* fx_023 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, the target's lift and drop, and the length. */
#define FX_PSYCHOKINESIS_LIFT_FRAME  5    /* op80: FX_PSYCHOKINESIS_GLYPH_ORBS and WHITE_RINGS start, the target starts rising */
#define FX_PSYCHOKINESIS_LIFT_FRAMES 0x1E /* rise34: the target rises to LIFT_HEIGHT over this many frames, then is held there */
#define FX_PSYCHOKINESIS_DROP_FRAME  40   /* op80: FX_PSYCHOKINESIS_WHITE_RINGS_2 starts, the target drops */
#define FX_PSYCHOKINESIS_DROP_FRAMES 3    /* fall36: back to the ground over this many frames; then GREY_RINGS and LIGHT_RAYS start */
#define FX_PSYCHOKINESIS_LIFT_HEIGHT 1000 /* the height the target is lifted to (the glyph orbs' target) */
#define FX_PSYCHOKINESIS_FRAMES      0x50 /* op80: the task ends at this frame */
#define FX_PSYCHOKINESIS_LIGHT_RAYS_TRAIL_SLOTS  0x3C /* FX_PSYCHOKINESIS_LIGHT_RAYS's trail buffer fx_023_trails: slots of */
#define FX_PSYCHOKINESIS_LIGHT_RAYS_TRAIL_STRIDE 0x18 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* target position */
    SVECTOR pos1C; /* target home position */
    SVECTOR pos24;
    SVECTOR target2C;
    s16 rise34; /* rise countdown */
    s16 fall36; /* fall countdown */
    s16 held38; /* held up */
} Fx023Work; /* 0x3C */

extern Unk800F78A4 *fx_023_env;
extern s32 fx_023_trails[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
