#ifndef FX_00A_H
#define FX_00A_H

/* Effect module fx_00a (dir 13 file 0x0A). */
#include "btlfx_mod.h"

/* fx_00a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_VENOM_POWDER_POWDER_CLOUD 0x2 /* emitter: purple powder clouds + purple droplet sprites */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    u8 pad4[0xC];
    SVECTOR pos10; /* target body position */
    SVECTOR pos18; /* target position */
    u8 pad20[8];
} Fx00aWork; /* 0x28 */

extern Unk800F78A4 *fx_00a_env;

#endif
