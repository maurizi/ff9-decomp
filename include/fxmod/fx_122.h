#ifndef FX_122_H
#define FX_122_H

/* Effect module fx_122 (dir 13 file 0x122): item Tent. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_122 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TENT_CANVAS_TENT 0x1 /* 'so' model: yellow-green striped canvas tent */
#define FX_TENT_SPARKLES 0x5 /* emitter: green and violet sparkles orbiting, white-cyan gouraud ring, glow */

/* fx_122 choreography (docs/fxdata.md, "Choreography literals"): where the tent goes on an enemy, its shake and
 * fades, op80's length. */
#define FX_TENT_OFFSET_COUNT     0x11  /* fx_122_offsets: per-enemy (height, radius) placements */
#define FX_TENT_TALL_HEIGHT      0x258 /* other enemies this tall */
#define FX_TENT_WIDE_RADIUS      0x2EE /* or this wide get the tent at their height, the rest on the ground */
#define FX_TENT_FADE_IN_FRAMES   8     /* the tent fades in from frame 0; SPARKLES start when it is done */
#define FX_TENT_SHAKE_FRAME      0x19  /* the tent shakes from this frame */
#define FX_TENT_SHAKE_END_FRAME  0x28  /* until this one */
#define FX_TENT_FADE_OUT_FRAME   0x40  /* and fades out from this frame */
#define FX_TENT_FADE_OUT_FRAMES  0x10  /* over this many */
#define FX_TENT_FRAMES           0x4F  /* op80's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR rot4; /* model rotation */
    VECTOR posC; /* model position */
    VECTOR scale1C; /* model scale */
    s16 color2C; /* model brightness */
    s16 height2E; /* model height */
    SVECTOR pos30; /* target position */
    SVECTOR rot38; /* target rotation */
    SVECTOR pos40;
    s16 timer48; /* fade-in countdown */
    s16 timer4A; /* fade-out countdown */
    u16 model4C; /* model */
} Fx122Work; /* 0x50 */

typedef struct {
    s16 height0; /* unit id */
    s16 radius2; /* unit radius */
    u16 offset4; /* height offset; bit 15: forward offset instead */
} Fx122Offset;
/* Fx122Offset.offset4 bit 15: the remaining bits move the point back along z instead of up (fx_122 op). Evidence: [code] */
#define FX122_OFS_FORWARD 0x8000

extern Fx122Offset fx_122_offsets[FX_TENT_OFFSET_COUNT];
extern Unk800F78A4 *fx_122_env;
extern BtlModelBuf fx_122_effModels[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
