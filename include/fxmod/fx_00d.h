#ifndef FX_00D_H
#define FX_00D_H

/* Effect module fx_00d (dir 13 file 0x0D). */
#include "btlfx_mod.h"

/* fx_00d effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_STONA_LIGHT_TRAIL 0x2 /* emitter: flat orbiting trail, yellow-red strip texture */
#define FX_STONA_BLUE_ORBS   0x3 /* emitter: blue light orbs dissolving into golden specks */
#define FX_STONA_GOLD_RINGS  0x9 /* emitter: gold-brown gouraud rings + yellow light streak */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target body position */
} Fx00dWork;

extern Unk800F78A4 *fx_00d_env;
extern s32 fx_00d_trails[];

#endif
