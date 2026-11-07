#ifndef FX_0A6_H
#define FX_0A6_H

/* Effect module fx_0a6 (dir 13 file 0xA6): enemy Sweep (Stroper). Emitters only. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0a6 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWEEP_DUST_RINGS 0x1 /* emitter: gold-brown gouraud rings + dust */
#define FX_SWEEP_WIND_TRAIL 0x2 /* emitter: grey wind streak trail */
#define FX_SWEEP_DUST_PUFF  0x3 /* emitter: yellow-brown dust puff */
#define FX_SWEEP_TAN_RING   0x4 /* emitter: tan-white gouraud rings */
#define FX_SWEEP_DUST_CLOUD 0x5 /* emitter: dust cloud puffs */

/* fx_0a6 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_SWEEP_HEIGHT      0xC8 /* DUST_RINGS, WIND_TRAIL, DUST_PUFF and DUST_CLOUD start this far above the targets' centre */
#define FX_SWEEP_CLOUD_FRAME 6    /* FX_SWEEP_DUST_CLOUD starts (the others at frame 0) */
#define FX_SWEEP_FRAMES      0x28 /* op80's length (the op ends) */
#define FX_SWEEP_WIND_TRAIL_TRAIL_SLOTS  0x12 /* FX_SWEEP_WIND_TRAIL's trail buffer fx_0a6_trails: slots of */
#define FX_SWEEP_WIND_TRAIL_TRAIL_STRIDE 0x60 /* this many bytes */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* position: env->targets->centerX..Z, raised by 0xC8 */
    SVECTOR pos1C; /* position: env->targets->centerX..Z */
} Fx0a6Work;

extern Unk800F78A4 *fx_0a6_env;
extern s32 fx_0a6_trails[];

#endif
