#ifndef FX_082_H
#define FX_082_H

/* Effect module fx_082 (dir 13 file 0x82): Magic Hammer (party and enemy). A morphing hammer model
 * (Hi_DrawMorphEffModel) and a squash of the caster scale. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

/* fx_082 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MAGIC_HAMMER_SMOKE        0x1 /* emitter: grey smoke puffs */
#define FX_MAGIC_HAMMER_RAINBOW_STREAK 0x2 /* emitter: fan drawn as a horizontal streak cycling blue, green, yellow, orange, red, pink */
#define FX_MAGIC_HAMMER_PASTEL_RINGS 0x3 /* emitter: pastel gouraud rings + coloured star specks */
#define FX_MAGIC_HAMMER_STARS        0x4 /* emitter: five-point stars fading blue to red */
#define FX_MAGIC_HAMMER_HAMMER       0x5 /* 'so' model: the hammer (banded green barrel head with steel caps, short handle) */
#define FX_MAGIC_HAMMER_VIOLET_RING  0x7 /* emitter: white-violet gouraud rings */
#define FX_MAGIC_HAMMER_CYAN_RING    0x8 /* emitter: white-cyan gouraud rings */

/* fx_082 choreography (docs/fxdata.md, "Choreography literals"): op80's states and their lengths, the hammer's
 * place and size; op81's squash. */
#define FX_MAGIC_HAMMER_STATE_GROW  0 /* the hammer appears (grows) above the target, the smoke puffs */
#define FX_MAGIC_HAMMER_STATE_SWING 1 /* it swings down */
#define FX_MAGIC_HAMMER_STATE_HIT   2 /* it shakes on the hit: rainbow streak and stars */
#define FX_MAGIC_HAMMER_STATE_FADE  3 /* it fades out, then the op ends */
#define FX_MAGIC_HAMMER_GROW_FRAMES  7 /* FX_MAGIC_HAMMER_STATE_GROW's length (the (frame << 12) / N grow ramp) */
#define FX_MAGIC_HAMMER_SWING_FRAMES 4 /* FX_MAGIC_HAMMER_STATE_SWING's length (the swing angle ramp is frame << 10) */
#define FX_MAGIC_HAMMER_RINGS_FRAME  3 /* FX_MAGIC_HAMMER_STATE_SWING: FX_MAGIC_HAMMER_PASTEL_RINGS start; the violet / cyan rings at its end */
#define FX_MAGIC_HAMMER_HIT_FRAMES   6 /* FX_MAGIC_HAMMER_STATE_HIT's length (the shake ramp (frame << 12) / N) */
#define FX_MAGIC_HAMMER_FADE_FRAMES  8 /* FX_MAGIC_HAMMER_STATE_FADE's length (the fade ramp is frame << 9) */
#define FX_MAGIC_HAMMER_SCALE   0xC00 /* the hammer's full scale (0x1000 = 1) */
#define FX_MAGIC_HAMMER_REACH   0x226 /* the hammer hangs this far ahead of the target */
#define FX_MAGIC_HAMMER_HEIGHT  0x145 /* and this far above its body position (the camera looks there) */
#define FX_MAGIC_HAMMER_MODEL_COUNT 2 /* btlfx_initEffModels slots: FX_MAGIC_HAMMER_HAMMER twice (morph pair) */
#define FX_MAGIC_HAMMER_SQUASH_FRAMES 0x10 /* op81: the target wobbles (scale) for this many frames, then its scale is restored */

typedef struct {
    s32 state0; /* state */
    s32 unk4; /* write-only (cleared at init) */
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10;
    SVECTOR pos18; /* pos8 moved 0x226 along rot20, raised by 0x145 */
    SVECTOR rot20; /* caster rotation */
    SVECTOR rot28; /* model rotation */
    s32 model30; /* models */
    s32 model34;
} Fx082Work80;

typedef struct {
    s32 scale0[3]; /* caster scale */
} Fx082Work81;

extern Unk800F78A4 *fx_082_env;
extern BtlModelBuf fx_082_effModels[];

#endif
