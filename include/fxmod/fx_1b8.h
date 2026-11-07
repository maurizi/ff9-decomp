#ifndef FX_1B8_H
#define FX_1B8_H

/* Effect module fx_1b8 (dir 13 file 0x1b8): enemy Death (Gizamaluke). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1b8 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH_TEAL_GLOWS  0x1 /* emitter: anim quads + sprites, soft teal-white glows drifting on screen (byte-identical to fx_1b9 entry 0x2); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_DEATH_WATER_WAVE  0x2 /* emitter: green-teal water wave particles, 16 frames */
#define FX_DEATH_SWIRL       0x6 /* emitter: orbiting white swirl vortex particles (sprites render black) */
#define FX_DEATH_RIPPLES     0x7 /* emitter: green ripple-circle particles + grey widening ring (byte-identical to fx_1b9 entry 0x1) */
#define FX_DEATH_WATER_SPOUT 0xE /* emitter: green / blue water spout, 10 frames (byte-identical to fx_1b9 entry 0x7) */

/* fx_1b8 choreography (docs/fxdata.md, "Choreography literals"): op80's length, op81's keyframes and length. */
#define FX_DEATH_SWIRL_FRAMES  0x3C /* op80 (SWIRL at unit 0x10): the task ends at this frame */
#define FX_DEATH_SPOUT_FRAME   8    /* op81: TEAL_GLOWS, WATER_SPOUT and WATER_WAVE start at unit 0x10's feet */
#define FX_DEATH_RIPPLES_FRAME 10   /* op81: RIPPLES start */
#define FX_DEATH_FRAMES        0x5A /* op81's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx1b8Work80; /* 0xC */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR pos20;
} Fx1b8Work81; /* 0x28 */

extern Unk800F78A4 *fx_1b8_env;

#endif
