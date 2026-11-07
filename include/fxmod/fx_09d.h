#ifndef FX_09D_H
#define FX_09D_H

/* Effect module fx_09d (dir 13 file 0x9D): Blind. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_09d effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BLIND_VORTEX   0x7 /* emitter: silver-blue swirling vortex ring (same call site as fx_09e entry 0x6) */
#define FX_BLIND_VORTEX_2 0x8 /* emitter: silver-blue swirling vortex ring + fans (same call site as fx_09e entry 0x8) */

/* fx_09d choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_BLIND_VORTEX_FRAME 0x18 /* FX_BLIND_VORTEX starts (FX_BLIND_VORTEX_2 at the start) */
#define FX_BLIND_FRAMES       0x38 /* op80's length (the op ends) */
#define FX_BLIND_VORTEX_Y     (-0x190) /* the vortices turn at this height (y, up is negative) over the targets' centre */

typedef struct {
    SVECTOR pos0; /* position: env->targets->centerX, -0x190, centerZ */
} Fx09dWork;

extern Unk800F78A4 *fx_09d_env;

#endif
