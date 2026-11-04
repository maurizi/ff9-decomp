#ifndef FX_04E_H
#define FX_04E_H

/* Effect module fx_04e (dir 13 file 0x4E: Sleep, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_04e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SLEEP_LIGHT_SPIRAL   0x6 /* emitter: helix of white screen trails, white-cyan rings, green glint (same call site and look as fx_01a entry 0x2) */
#define FX_SLEEP_SPECK_RING     0x7 /* emitter: particle (magenta: texture still outside the file and the common pages) + ring textured with brown specks (same call site and look as fx_01a entry 0x3) (look pending) */
#define FX_SLEEP_LIGHT_SPIRAL_2 0x8 /* emitter: helix of two white screen trails with green / violet glows (same call site and look as fx_01a entry 0x4) */
#define FX_SLEEP_BACKDROP       0x9 /* backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool) (same call site and look as fx_01a entry 0x1) (look pending) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* position: env->targets->centerX..Z */
} Fx04eWork;

extern Unk800F78A4 *fx_04e_env;
extern s32 fx_04e_trails[];
extern s32 fx_04e_trails2[];

#endif
