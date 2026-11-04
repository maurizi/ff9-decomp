#ifndef FX_044_H
#define FX_044_H

/* Effect module fx_044 (dir 13 file 0x44). */
#include "btlfx_mod.h"

/* fx_044 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ULTRA_SOUND_WAVE_PINK_RINGS   0x1 /* emitter: pink gouraud rings */
#define FX_ULTRA_SOUND_WAVE_VIOLET_HALO  0x2 /* emitter: violet ring-shaped glow */
#define FX_ULTRA_SOUND_WAVE_RAINBOW_RING 0x3 /* emitter: flat trail with a rainbow ring texture */
#define FX_ULTRA_SOUND_WAVE_SWIRL        0x4 /* emitter: rainbow swirl + lilac ring */
#define FX_ULTRA_SOUND_WAVE_WISPS        0x5 /* emitter: wisps cycling orange-green-blue-violet */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position */
    SVECTOR pos10;
    SVECTOR aim18; /* angles from pos10 to pos8 */
} Fx044Work80; /* 0x20 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target body position */
    SVECTOR pos14; /* target position */
    SVECTOR pos1C; /* above the target body */
} Fx044Work81; /* 0x24 */

extern Unk800F78A4 *fx_044_env;
extern s32 fx_044_trails[];

#endif
