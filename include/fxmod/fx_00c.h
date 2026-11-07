#ifndef FX_00C_H
#define FX_00C_H

/* Effect module fx_00c (dir 13 file 0x0C). */
#include "btlfx_mod.h"

/* fx_00c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PANACEA_GREEN_RINGS 0x7 /* emitter: yellow-green and white-violet gouraud rings */
#define FX_PANACEA_SPARKLES    0x8 /* emitter: green star glint, teal glow orb, golden sparkle sprites */

/* fx_00c choreography (docs/fxdata.md, "Choreography literals"): op80's length. */
#define FX_PANACEA_FRAMES 0x28 /* op80: the task ends at this frame */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
    SVECTOR pos10; /* target body position */
} Fx00cWork;

extern Unk800F78A4 *fx_00c_env;

#endif
