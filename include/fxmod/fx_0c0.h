#ifndef FX_0C0_H
#define FX_0C0_H

/* Effect module fx_0c0 (dir 13 file 0xC0): Shock; Shock (Seiken); enemy Shock (Beatrix). Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0c0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SHOCK_SPARKLES_2 0x7 /* emitter: blue sparkle sprites (+ a dark sprite element, fans) */
#define FX_SHOCK_ENERGY_ORB 0x9 /* emitter: blue-white energy orb particle */
#define FX_SHOCK_FLAME_RING 0xB /* emitter: ring textured with blue flames */
#define FX_SHOCK_SPARKLES   0xD /* emitter: blue sparkle sprites, fan */

/* fx_0c0 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe and length. */
#define FX_SHOCK_FLAME_RING_FRAME 0x24 /* op80: FX_SHOCK_FLAME_RING and FX_SHOCK_SPARKLES_2 start at the caster */
#define FX_SHOCK_FRAMES           0x46 /* op80: the task ends at this frame */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* unit 0x10 body position */
    SVECTOR pos18; /* caster body position */
} Fx0c0Work;

extern Unk800F78A4 *fx_0c0_env;

#endif
