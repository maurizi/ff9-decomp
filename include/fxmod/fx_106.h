#ifndef FX_106_H
#define FX_106_H

/* Effect module fx_106 (dir 13 file 0x106): enemy Mucus (Anemone). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_106 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MUCUS_SLIME_BLOBS    0x1 /* emitter: yellow slime blobs */
#define FX_MUCUS_BUBBLE_TRAIL   0x2 /* emitter: violet bubble screen trail + speck sprites (texture not in the file) */
#define FX_MUCUS_BUBBLE_TRAIL_2 0x3 /* emitter: violet bubble screen trail + speck sprites (texture not in the file) */
#define FX_MUCUS_YELLOW_RINGS   0x4 /* emitter: yellow-green gouraud rings */
#define FX_MUCUS_SPLASH_STREAK  0x5 /* emitter: white streaky splash screen trail */

/* fx_106 choreography (docs/fxdata.md, "Choreography literals"): the slime arc of op80's particle callback, the
 * trail buffers and the ops' lengths. */
#define FX_MUCUS_ARC_START_FRAME     4    /* fx_106_op80ParticleMotion: the blobs fly from the caster's mouth to the target from this frame (over 16: << 8) */
#define FX_MUCUS_ARC_HEIGHT          200  /* the arc's peak height */
#define FX_MUCUS_SLIME_TRAIL_SLOTS   0x23 /* SLIME_BLOBS' trail buffer (fx_106_op80Trails): slots */
#define FX_MUCUS_SLIME_TRAIL_STRIDE  0x20 /* bytes per slot */
#define FX_MUCUS_SPIT_FRAMES         0x28 /* op80's length */
#define FX_MUCUS_BUBBLE_TRAIL_SLOTS  0x33 /* op81: BUBBLE_TRAIL_2's trail buffer (fx_106_op81Trails0): slots */
#define FX_MUCUS_BUBBLE_TRAIL_STRIDE 0x20 /* bytes per slot */
#define FX_MUCUS_SPLASH_FRAME        4    /* SPLASH_STREAK starts at the target */
#define FX_MUCUS_SPLASH_TRAIL_SLOTS  0x1E /* its trail buffer (fx_106_op81Trails1): slots */
#define FX_MUCUS_SPLASH_TRAIL_STRIDE 0x70 /* bytes per slot */
#define FX_MUCUS_HIT_FRAMES          0x28 /* op81's length */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* caster rotation */
    SVECTOR pos10; /* target body position */
    SVECTOR pos18;
    SVECTOR pos20;
} Fx106Work80; /* 0x28 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target position */
    SVECTOR pos14;
} Fx106Work81; /* 0x1C */

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    s32 (*motion8)(VECTOR *, s32, SVECTOR *, s32, s32, s32);
} Fx106EffHdr;

extern Unk800F78A4 *fx_106_env;
extern s32 fx_106_op80Trails[];
extern s32 fx_106_op81Trails0[];
extern s32 fx_106_op81Trails1[];
extern SVECTOR fx_106_pathStart; /* path start */
extern SVECTOR fx_106_pathEnd; /* path end */
extern SVECTOR fx_106_emitPos;

static s32 fx_106_op80ParticleMotion(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
