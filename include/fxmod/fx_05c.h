#ifndef FX_05C_H
#define FX_05C_H

/* Effect module fx_05c (dir 13 file 0x5C). */
#include "btlfx_mod.h"

/* fx_05c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ARMOR_BREAK_BLUE_TRIANGLE 0x7 /* emitter: blue-cyan-white / orange gouraud rings, white-grey rings, blue triangle */
#define FX_ARMOR_BREAK_BLUE_ORB      0xB /* emitter: blue sphere with a white flame inside */
#define FX_ARMOR_BREAK_SPARKLES      0xC /* emitter: white cross sparkles + fan */

/* fx_05c choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe and length. */
#define FX_ARMOR_BREAK_TRIANGLE_FRAME 0x24 /* op80: FX_ARMOR_BREAK_BLUE_TRIANGLE starts at the target */
#define FX_ARMOR_BREAK_FRAMES         0x5A /* op80: the triangle is stopped and the task ends at this frame */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster body position */
    SVECTOR pos14; /* target body position */
} Fx05cWork;

extern Unk800F78A4 *fx_05c_env;

#endif
