#ifndef FX_103_H
#define FX_103_H

/* Effect module fx_103 (dir 13 file 0x103): enemy Mist (Mistodon). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_103 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MIST_SWIRL 0x2 /* emitter: blue-grey mist puffs orbiting */
#define FX_MIST_CLOUD 0x3 /* emitter: blue-grey mist puffs */

/* fx_103 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_MIST_SPREAD      0x1F4 /* the two CLOUDs (from the caster's two arg bones) aim at points this far left / right of the target */
#define FX_MIST_SWIRL_FRAME 0x10  /* SWIRL starts at the target */
#define FX_MIST_FRAMES      0x50  /* op80's length (the op ends) */

typedef struct {
    SVECTOR pos0;
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR rot20;
    SVECTOR rot28;
    SVECTOR pos30;
    Pool6CEntry *emit38;
    Pool6CEntry *emit3C;
} Fx103Work; /* 0x40 */

extern Unk800F78A4 *fx_103_env;

#endif
