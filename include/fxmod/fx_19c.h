#ifndef FX_19C_H
#define FX_19C_H

/* Effect module fx_19c (dir 13 file 0x19c): Thunder Slash (Seiken). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_19c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THUNDER_SLASH_SPARKS    0x1 /* emitter: cone of blue-green electric wisps, orange sparks and white glints (byte-identical to fx_0bf entry 0x1) */
#define FX_THUNDER_SLASH_LIGHTNING 0x2 /* sprite: blue / red lightning streak, 12 frames (drawn directly; byte-identical to fx_0bf entry 0x2) */

/* fx_19c choreography (docs/fxdata.md, "Choreography literals"): the ops' keyframes and lengths (fx_0bf's names). */
#define FX_THUNDER_SLASH_SPARKS_FRAME  3    /* op80: FX_THUNDER_SLASH_SPARKS start */
#define FX_THUNDER_SLASH_FLASH_FRAME   7    /* op80: one-frame additive grey screen flash */
#define FX_THUNDER_SLASH_FLASH_GREY    0x80 /* op80: its level (work->colorC, r = g = b) */
#define FX_THUNDER_SLASH_FRAMES        0x32 /* op80: the task ends at this frame */
#define FX_THUNDER_SLASH_CAMERA_FRAMES 7    /* op81: the camera looks at unit 0x10 (btlfx_getUnitPos) until this frame */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    s32 colorC;
} Fx19cWork; /* 0x10 */

extern Unk800F78A4 *fx_19c_env;

#endif
