#ifndef FX_032_H
#define FX_032_H

/* Effect module fx_032 (dir 13 file 0x32). */
#include "btlfx_mod.h"

/* fx_032 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_GLOWING_EYES_SPARKS       0x1 /* emitter: cone of spark sprites (texture not in the file) (look pending) */
#define FX_GLOWING_EYES_GLOW_RINGS   0x2 /* emitter: olive-yellow and white-cyan gouraud rings */
#define FX_GLOWING_EYES_GLOW_RINGS_2 0x3 /* emitter: olive-yellow and white-cyan gouraud rings */
#define FX_GLOWING_EYES_SMOKE        0x4 /* emitter: grey smoke wisps */
#define FX_GLOWING_EYES_SWIRL        0x5 /* emitter: pale green swirling vortex */

typedef struct {
    SVECTOR pos0; /* target body position */
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR pos20;
    SVECTOR pos28;
    SVECTOR pos30;
    SVECTOR pos38;
    SVECTOR pos40;
    Pool6CEntry *emit48;
    Pool6CEntry *emit4C;
    Pool6CEntry *emit50;
    Pool6CEntry *emit54;
} Fx032Work; /* 0x58 */

extern Unk800F78A4 *fx_032_env;

#endif
