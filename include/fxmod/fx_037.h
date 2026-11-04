#ifndef FX_037_H
#define FX_037_H

/* Effect module fx_037 (dir 13 file 0x37). */
#include "btlfx_mod.h"

/* fx_037 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DARKNESS_DARK_CRESCENTS 0x1 /* emitter: pale crescent quads tinted violet / green + coloured specks */
#define FX_DARKNESS_SCREEN_TRAIL   0x2 /* emitter: orbiting screen trail (texture not in the file) (look pending) */
#define FX_DARKNESS_WHITE_GLOW     0x3 /* emitter: white radial glow + one particle (texture not in the file) */
#define FX_DARKNESS_DARK_ORBS      0x4 /* emitter: blue orbs turning violet then red */
#define FX_DARKNESS_FAN_EMITTER    0x5 /* emitter: one fan element (not rendered) (look pending) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* target body position */
} Fx037Work;

extern Unk800F78A4 *fx_037_env;
extern s32 fx_037_trails[];
extern s32 fx_037_trails2[];

#endif
