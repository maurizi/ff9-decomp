#ifndef FX_1A3_H
#define FX_1A3_H

/* Effect module fx_1a3 (dir 13 file 0x1a3): enemy Trouble Mucus (Antlion). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1a3 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TROUBLE_MUCUS_SLIME_BLOBS    0x1 /* emitter: yellow slime blobs (byte-identical to fx_106 entry 0x1) */
#define FX_TROUBLE_MUCUS_BUBBLE_TRAIL   0x2 /* emitter: violet bubble screen trail + yellow speck dots (byte-identical to fx_106 entry 0x2) */
#define FX_TROUBLE_MUCUS_BUBBLE_TRAIL_2 0x3 /* emitter: violet bubble screen trail + yellow speck dots (byte-identical to fx_106 entry 0x3) */
#define FX_TROUBLE_MUCUS_YELLOW_RINGS   0x4 /* emitter: yellow-green gouraud rings (byte-identical to fx_106 entry 0x4) */
#define FX_TROUBLE_MUCUS_SPLASH_STREAK  0x5 /* emitter: white streaky splash screen trail (byte-identical to fx_106 entry 0x5) */

/* fx_1a3 choreography (docs/fxdata.md, "Choreography literals"): op80's slime blobs (lerped from in front of the
 * caster to the target), op81's splash at the caster, the trail buffers and lengths. */
#define FX_TROUBLE_MUCUS_AHEAD       0x190 /* op80: the blobs start this far ahead of unit 0x10's argument bone */
#define FX_TROUBLE_MUCUS_BLOBS_FRAME 4     /* op80: the blob particles leave from this particle frame (8-frame ramp) */
#define FX_TROUBLE_MUCUS_SLIME_BLOBS_TRAIL_SLOTS  0x23 /* SLIME_BLOBS' trail buffer fx_1a3_op80Trails: slots of */
#define FX_TROUBLE_MUCUS_SLIME_BLOBS_TRAIL_STRIDE 0x20 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_TROUBLE_MUCUS_BLOBS_FRAMES 0x28 /* op80's length (the op ends) */
#define FX_TROUBLE_MUCUS_BUBBLES_HEIGHT 0x190 /* op81: BUBBLE_TRAIL_2 starts this high over the target's feet */
#define FX_TROUBLE_MUCUS_BUBBLE_TRAIL_2_TRAIL_SLOTS  0x33 /* its trail buffer fx_1a3_op81Trails0: slots of */
#define FX_TROUBLE_MUCUS_BUBBLE_TRAIL_2_TRAIL_STRIDE 0x20 /* this many bytes */
#define FX_TROUBLE_MUCUS_SPLASH_FRAME 4    /* op81: SPLASH_STREAK starts */
#define FX_TROUBLE_MUCUS_SPLASH_STREAK_TRAIL_SLOTS  0x1E /* its trail buffer fx_1a3_op81Trails1: slots of */
#define FX_TROUBLE_MUCUS_SPLASH_STREAK_TRAIL_STRIDE 0x70 /* this many bytes */
#define FX_TROUBLE_MUCUS_FRAMES      0x28  /* op81's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* caster rotation */
    SVECTOR pos10; /* target body position */
    SVECTOR pos18;
    SVECTOR pos20;
} Fx1a3Work80; /* 0x28 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster position */
    SVECTOR pos14;
} Fx1a3Work81; /* 0x1C */

extern Unk800F78A4 *fx_1a3_env;
extern s32 fx_1a3_op80Trails[];
extern s32 fx_1a3_op81Trails0[];
extern s32 fx_1a3_op81Trails1[];
extern SVECTOR fx_1a3_motionFrom; /* from */
extern SVECTOR fx_1a3_motionTo; /* to */
extern SVECTOR fx_1a3_emitPos;

static s32 fx_1a3_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
