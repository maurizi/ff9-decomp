#ifndef FX_03B_H
#define FX_03B_H

/* Effect module fx_03b (dir 13 file 0x3B). */
#include "btlfx_mod.h"

/* fx_03b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TROUBLE_JUICE_SPLASH         0x1 /* emitter: orange burst of juice blobs */
#define FX_TROUBLE_JUICE_SCRIBBLE_RINGS 0x2 /* emitter: ring with an orange scribble texture + cream gouraud rings */
#define FX_TROUBLE_JUICE_SCREEN_TRAIL   0x3 /* emitter: screen trail, still renders all black (look pending) */
#define FX_TROUBLE_JUICE_SCREEN_TRAIL_2 0xA /* emitter: screen trail, still renders all black (look pending) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* target position */
    SVECTOR pos18; /* above the target */
    SVECTOR pos20; /* target body position */
    SVECTOR pos28;
} Fx03bWork;

extern Unk800F78A4 *fx_03b_env;
extern s32 fx_03b_trails2[];
extern s32 fx_03b_trails[];

#endif
