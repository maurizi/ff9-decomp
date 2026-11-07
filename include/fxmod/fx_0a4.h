#ifndef FX_0A4_H
#define FX_0A4_H

/* Effect module fx_0a4 (dir 13 file 0xA4): enemy Buzz (Dragonfly and others). Emitters only. Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0a4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BUZZ_INSECT     0x1 /* emitter: one sprite, a small grey winged insect silhouette (2 frames) at run time (not ablated); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_BUZZ_SOUND_WAVE 0x6 /* emitter: ring textured with a yellow sine wave */

/* fx_0a4 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_BUZZ_SOUND_WAVE_FRAME 0x12 /* FX_BUZZ_SOUND_WAVE starts */
#define FX_BUZZ_FRAMES           0x28 /* op80's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
} Fx0a4Work;

extern Unk800F78A4 *fx_0a4_env;

#endif
