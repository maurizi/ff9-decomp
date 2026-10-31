#ifndef FX_00A_H
#define FX_00A_H

/* Effect module fx_00a (dir 13 file 0x0A). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    u8 pad4[0xC];
    SVECTOR unk10; /* target body position */
    SVECTOR pos18; /* target position */
    u8 pad20[8];
} Fx00aWork; /* 0x28 */

extern Unk800F78A4 *fx_00a_env;

#endif
