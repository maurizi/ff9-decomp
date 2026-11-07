#ifndef FX_116_H
#define FX_116_H

/* Effect module fx_116 (dir 13 file 0x116): item Ether. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_116 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ETHER_SPARKLES 0x1 /* emitter: red and violet sparkles orbiting, white-cyan gouraud ring, red glow */

/* fx_116 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_ETHER_FRAMES 0x46 /* op80's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx116Work; /* 0xC */

extern Unk800F78A4 *fx_116_env;

#endif
