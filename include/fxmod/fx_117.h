#ifndef FX_117_H
#define FX_117_H

/* Effect module fx_117 (dir 13 file 0x117): item Echo Screen; item Dead Pepper; item Gysahl Greens. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_117 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ECHO_SCREEN_SPARKLES 0x4 /* emitter, one per file: Echo Screen (0x117) blue-white puff, violet specks, white-cyan gouraud ring, glow; Dead Pepper (0x120) puff, violet and red sparkles, glow; Gysahl Greens (0x121) green / violet / gold sparkles, ring, glow, leaf sprite */

/* fx_117 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_ECHO_SCREEN_FRAMES 0x32 /* op80's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx117Work; /* 0xC */

extern Unk800F78A4 *fx_117_env;

#endif
