#ifndef FX_0A5_H
#define FX_0A5_H

/* Effect module fx_0a5 (dir 13 file 0xA5): enemy Spore (Myconid). Emitters, one moved across the targets.
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0a5 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SPORE_GREEN_GLOW 0x7 /* emitter: orbiting trail + sprite, a pale gold strip and a soft green glow at run time; runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */
#define FX_SPORE_SPORES 0x8 /* emitter: brown spore sprites */

/* fx_0a5 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_SPORE_GLOW_FRAME   0x19  /* FX_SPORE_GREEN_GLOW starts and sweeps across the targets from then on */
#define FX_SPORE_SWEEP_X      0x5DC /* the sweep runs from x = this to x = -this (pos18 -> pos20) */
#define FX_SPORE_SWEEP_FRAMES 30    /* the sweep ratio (frame << 12) / this reaches 1 at this frame */
#define FX_SPORE_FRAMES       0x50  /* op80's length (the op ends) */
#define FX_SPORE_GREEN_GLOW_TRAIL_SLOTS  0x80 /* FX_SPORE_GREEN_GLOW's trail buffer fx_0a5_trails: slots of */
#define FX_SPORE_GREEN_GLOW_TRAIL_STRIDE 0x20 /* this many bytes */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR aim10; /* angles from pos8 to env->targets->centerX */
    SVECTOR pos18; /* lerp start: env position at x = 0x5DC */
    SVECTOR pos20; /* lerp end: env position at x = -0x5DC */
    SVECTOR pos28; /* lerp output */
} Fx0a5Work;

extern Unk800F78A4 *fx_0a5_env;
extern s32 fx_0a5_trails[];

#endif
