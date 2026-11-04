#ifndef FX_01C_H
#define FX_01C_H

/* Effect module fx_01c (dir 13 file 0x1C). */
#include "btlfx_mod.h"

/* fx_01c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_AQUA_BREATH_WATER_DROPS 0x4 /* emitter: water droplet clusters */
#define FX_AQUA_BREATH_WAVE_RING   0x5 /* emitter: ring with a blue wave texture + blue glow */
#define FX_AQUA_BREATH_BUBBLES     0x6 /* emitter: water-droplet trail + bubble quads */
#define FX_AQUA_BREATH_BLUE_RING   0x7 /* emitter: light blue / white gouraud ring */
#define FX_AQUA_BREATH_BUBBLE      0x8 /* emitter: one large bubble + small glints */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* caster body position */
    SVECTOR pos1C; /* target (body) position */
    SVECTOR aim24; /* angles from pos14 to pos1C */
} Fx01cWork; /* 0x2C */

extern Unk800F78A4 *fx_01c_env;
extern s32 fx_01c_trails[];

#endif
