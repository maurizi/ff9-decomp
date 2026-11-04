#ifndef FX_019_H
#define FX_019_H

/* Effect module fx_019 (dir 13 file 0x19). */
#include "btlfx_mod.h"

/* fx_019 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MINI_FAN_EMITTER 0x1 /* emitter: fan element + one faint ring (fan not rendered) (look pending) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
} Fx019Work;

extern Unk800F78A4 *fx_019_env;

#endif
