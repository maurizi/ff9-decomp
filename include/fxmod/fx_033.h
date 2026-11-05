#ifndef FX_033_H
#define FX_033_H

/* Effect module fx_033 (dir 13 file 0x33). */
#include "btlfx_mod.h"

/* fx_033 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SNORT_SMOKE_STREAKS 0x5 /* emitter: two spiralling screen trails, the grey smoke streaks blowing past the target; runtime (docs/runtime-findings.md effect-data pass 2, medium) */

typedef struct {
    s32 timer0; /* knockback countdown */
    SVECTOR pos4;
    SVECTOR posC; /* target body position */
    SVECTOR pos14; /* target home position */
    SVECTOR pos1C; /* target position while pushed back */
    SVECTOR aim24; /* angles from pos4 to posC */
    Pool6CEntry *emit2C;
} Fx033Work; /* 0x30 */

extern Unk800F78A4 *fx_033_env;
extern s32 fx_033_trails[];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
