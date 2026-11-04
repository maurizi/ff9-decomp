#ifndef FX_02E_H
#define FX_02E_H

/* Effect module fx_02e (dir 13 file 0x2E). */
#include "btlfx_mod.h"

/* fx_02e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MAGIC_BREAK_SPARKLES    0x1 /* emitter: blue-violet specks + fan */
#define FX_MAGIC_BREAK_RED_ORB     0x2 /* emitter: red sphere with a white flame inside */
#define FX_MAGIC_BREAK_FAN_EMITTER 0x3 /* emitter: one fan element (not rendered) (look pending) */
#define FX_MAGIC_BREAK_RUNE_RING   0x4 /* emitter: violet circuit-pattern ring + red sphere */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* target body position */
    SVECTOR aim20; /* angles from the camera eye to pos18 */
} Fx02eWork; /* 0x28 */

extern Unk800F78A4 *fx_02e_env;

#endif
