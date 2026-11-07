#ifndef FX_0F3_H
#define FX_0F3_H

/* Effect module fx_0f3 (dir 13 file 0x0F3): Scoop Art. A moving point drawn as a strip (btlfx_drawTrail) with an
 * emitter and screen tints, three path variants. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f3 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SCOOP_ART_BLUE_RINGS   0x2 /* emitter: white-blue gouraud rings + rings textured with blue swirl marks */
#define FX_SCOOP_ART_PINK_TRAIL   0x3 /* trail: red-pink glowing strip drawn along the moving point */
#define FX_SCOOP_ART_GOLD_SPARK   0x8 /* emitter: screen trail + sprite, a pale-gold gradient strip and a soft white-gold glow at run time: the yellow spark at the tip of the pink pillar; runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */

/* fx_0f3 choreography (docs/fxdata.md, "Choreography literals"): op80's two states (the pink strip sweeps through
 * the target along one of three paths, then fades), their keys and the gold spark's trail buffer. */
#define FX_SCOOP_ART_STATE_SWEEP        0    /* op80 state: the strip head moves along the variant's path */
#define FX_SCOOP_ART_STATE_FADE         1    /* the strip stays and fades (cos ramp of its width) */
#define FX_SCOOP_ART_SWEEP_FRAMES       0xC  /* SWEEP lasts this many frames (the (frame << 12) / N path ramp) */
#define FX_SCOOP_ART_DARKEN_FRAME       6    /* variant 0 darkens the screen on this frame */
#define FX_SCOOP_ART_DARKEN_RGB         0x40, 0x40, 0x40 /* its subtractive tint */
#define FX_SCOOP_ART_FLASH_FRAME        7    /* BLUE_RINGS start and the screen flashes on this frame */
#define FX_SCOOP_ART_FLASH_RGB          0x5D, 0x5D, 0x5D /* the additive flash */
#define FX_SCOOP_ART_FADE_FRAMES        0x1D /* FADE lasts this many frames (op80 ends) */
#define FX_SCOOP_ART_SPARK_TRAIL_SLOTS  4    /* GOLD_SPARK's trail buffer: slots */
#define FX_SCOOP_ART_SPARK_TRAIL_STRIDE 0xAC /* bytes per slot */
#define FX_SCOOP_ART_STRIP_SIZE         0xEC /* bytes of one variant's PINK_TRAIL strip buffer (fx_0f3_strips) */

typedef struct {
    s32 state0; /* state */
    s32 variant4; /* variant (0..2) */
    SVECTOR pos8; /* target body position */
    SVECTOR headPos10;
    SVECTOR pos18;
    s32 unk20; /* unused (runtime watch, round 33) */
    Pool6CEntry *emit24;
} Fx0f3Work; /* 0x28 */

extern Unk800F78A4 *fx_0f3_env;
extern u8 fx_0f3_strips[][FX_SCOOP_ART_STRIP_SIZE];
extern u8 fx_0f3_op80Trails[][FX_SCOOP_ART_SPARK_TRAIL_SLOTS * FX_SCOOP_ART_SPARK_TRAIL_STRIDE];

#endif
