#ifndef FX_01A_H
#define FX_01A_H

/* Effect module fx_01a (dir 13 file 0x1A). */
#include "btlfx_mod.h"

/* fx_01a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SLEEP_BACKDROP       0x1 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture; not drawn by the tool) (look pending) */
#define FX_SLEEP_LIGHT_SPIRAL   0x2 /* emitter: helix of white screen trails, white-cyan rings, green glint */
#define FX_SLEEP_SPECK_RING     0x3 /* emitter: particle (magenta: texture still outside the file and the common pages) + ring textured with brown specks (look pending) */
#define FX_SLEEP_LIGHT_SPIRAL_2 0x4 /* emitter: helix of two white screen trails with green / violet glows */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target position */
} Fx01aWork;

extern Unk800F78A4 *fx_01a_env;
extern s32 fx_01a_trails[];
extern s32 fx_01a_trails2[];

#endif
