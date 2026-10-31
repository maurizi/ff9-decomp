#ifndef FX_02E_H
#define FX_02E_H

/* Effect module fx_02e (dir 13 file 0x2E). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* target body position */
    SVECTOR unk20; /* angles to unk18 */
} Fx02eWork; /* 0x28 */

extern Unk800F78A4 *fx_02e_env;

#endif
