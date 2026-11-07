#ifndef FX_022_H
#define FX_022_H

/* Effect module fx_022 (dir 13 file 0x22). */
#include "btlfx_mod.h"

/* fx_022 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TENTACLE_GONE_BUBBLES 0x1 /* emitter: iridescent soap bubbles */

/* fx_022 choreography (docs/fxdata.md, "Choreography literals"): op80's length. */
#define FX_TENTACLE_GONE_FRAMES 0x28 /* op80: the task ends at this frame */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    SVECTOR posC;
} Fx022Work;

extern Unk800F78A4 *fx_022_env;

#endif
