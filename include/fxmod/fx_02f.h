#ifndef FX_02F_H
#define FX_02F_H

/* Effect module fx_02f (dir 13 file 0x2F). */
#include "btlfx_mod.h"

/* fx_02f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLAME_FIREBALL     0x5 /* emitter: expanding red-orange fireballs + flame tongues (one per target) */
#define FX_FLAME_FLAME_STREAK 0x6 /* emitter: screen trail with a flame-streak strip */

typedef struct {
    SVECTOR aim0; /* beam angles */
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18; /* target position, on the ground */
    Pool6CEntry *emit20;
} Fx02fWork; /* 0x24 */

extern Unk800F78A4 *fx_02f_env;
extern SVECTOR fx_02f_targetPos[]; /* target body positions */
extern s32 fx_02f_trails[];

#endif
