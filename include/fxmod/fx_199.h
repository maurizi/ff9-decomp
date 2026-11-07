#ifndef FX_199_H
#define FX_199_H

/* Effect module fx_199 (dir 13 file 0x199): Darkside (alt). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_199 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DARKSIDE_SPARKLE_BURST 0x1 /* emitter: yellow / blue ring, blue sparkle sprites, fan (byte-identical to fx_0bc entry 0x1) */

/* fx_199 choreography (docs/fxdata.md, "Choreography literals"): the ops' lengths (fx_0bc's names). */
#define FX_DARKSIDE_FRAMES        0x32 /* op80: the task ends at this frame */
#define FX_DARKSIDE_CAMERA_FRAMES 7    /* op81: the camera looks at unit 0x10 (btlfx_getUnitPos) until this frame */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx199Work; /* 0xC */

extern Unk800F78A4 *fx_199_env;

#endif
