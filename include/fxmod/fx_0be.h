#ifndef FX_0BE_H
#define FX_0BE_H

/* Effect module fx_0be (dir 13 file 0xBE): Iai Strike. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0be effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_IAI_STRIKE_SLASH_ARCS    0x4 /* emitter: blue and pink slash arcs */
#define FX_IAI_STRIKE_SPARKLE_BURST 0x8 /* emitter: violet / blue ring, blue and gold sparkle sprites, fan */

/* fx_0be choreography (docs/fxdata.md, "Choreography literals"): the ops' lengths. */
#define FX_IAI_STRIKE_FRAMES        0x1E /* op80: the task ends at this frame */
#define FX_IAI_STRIKE_CAMERA_FRAMES 0xC  /* op81: the camera looks at unit 0x10 (btlfx_getUnitPos) until this frame */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
} Fx0beWork80;

typedef struct {
    SVECTOR pos0; /* unit 0x10 position */
} Fx0beWork81;

extern Unk800F78A4 *fx_0be_env;

#endif
