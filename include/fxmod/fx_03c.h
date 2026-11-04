#ifndef FX_03C_H
#define FX_03C_H

/* Effect module fx_03c (dir 13 file 0x3C). */
#include "btlfx_mod.h"

/* fx_03c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_WIND_WIND_TRAILS 0x2 /* emitter: two spiralling trails with a grey wind-streak strip */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
    SVECTOR posC; /* target body position */
    SVECTOR pos14;
    u8 pad1C[8];
} Fx03cWork; /* 0x24 */

extern Unk800F78A4 *fx_03c_env;
extern s32 fx_03c_trails[];

#endif
