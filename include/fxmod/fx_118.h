#ifndef FX_118_H
#define FX_118_H

/* Effect module fx_118 (dir 13 file 0x118): item Eye Drops. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_118 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_EYE_DROPS_SPARKLES 0x4 /* emitter: white-lilac gouraud rings, pale sparkles, blue and green glows */

/* fx_118 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_EYE_DROPS_FRAMES 0x28 /* op80's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx118Work; /* 0xC */

extern Unk800F78A4 *fx_118_env;

#endif
