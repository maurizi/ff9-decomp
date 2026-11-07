#ifndef FX_126_H
#define FX_126_H

/* Effect module fx_126 (dir 13 file 0x126): enemy Boomerang (Ark). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_126 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BOOMERANG_CRESCENTS 0x1 /* emitter: orange, white and blue crescent arcs + debris chips and specks */
#define FX_BOOMERANG_SWIRL     0x2 /* emitter: orange and teal spiral swirls + yellow and pink gouraud rings */
#define FX_BOOMERANG_CURVED_BLADE 0x7 /* 'so' model: gold curved boomerang blade, registered twice */

/* fx_126 choreography (docs/fxdata.md, "Choreography literals"): the two blades' Bezier flight, the hit keys and
 * op80's length. */
#define FX_BOOMERANG_MODEL_SLOTS   2     /* btlfx_initEffModels slots: CURVED_BLADE twice */
#define FX_BOOMERANG_TARGETS       4     /* pos7C / done9C: up to this many targets get the hit emitters */
#define FX_BOOMERANG_REACH         0x6E0 /* the blades' far control point lies this far down -z */
#define FX_BOOMERANG_SWING_WIDTH   0x692 /* their side control points this far left / right */
#define FX_BOOMERANG_FLIGHT_FRAMES 0x18  /* the blades fly out and back (fx_126_quarticBezier) over frames 1 .. this */
#define FX_BOOMERANG_HIT_FRAME     8     /* SWIRL and CRESCENTS on targets 0 and 3 */
#define FX_BOOMERANG_HIT_2_FRAME   10    /* and on targets 1 and 2 */
#define FX_BOOMERANG_FRAMES        0x20  /* op80's length (the op ends) */

typedef struct {
    s32 model0; /* models */
    s32 model4;
    VECTOR pos8; /* model positions */
    VECTOR pos18;
    SVECTOR rot28; /* model rotations */
    SVECTOR rot30;
    VECTOR scale38; /* model scale */
    u8 pad48[0x50 - 0x48];
    SVECTOR pos50; /* curve control points */
    SVECTOR pos58;
    SVECTOR pos60;
    SVECTOR pos68;
    SVECTOR pos70;
    u8 pad78[0x7C - 0x78];
    SVECTOR pos7C[FX_BOOMERANG_TARGETS]; /* unit body positions */
    s32 done9C[FX_BOOMERANG_TARGETS]; /* unit hit */
} Fx126Work; /* 0xAC */

extern Unk800F78A4 *fx_126_env;
extern BtlModelBuf fx_126_effModels[];

s32 fx_126_op80(s32 mode, Fx126Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
void fx_126_quarticBezier(s32 len, s32 t, SVECTOR *p0, SVECTOR *p1, SVECTOR *p2, SVECTOR *p3, SVECTOR *p4, VECTOR *out);

#endif
