#ifndef FX_0DE_H
#define FX_0DE_H

/* Effect module fx_0de (dir 13 file 0x0DE). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0[8]; /* unit home positions */
    s32 unk40; /* unit count */
    s32 unk44; /* jump height */
    s32 unk48[8]; /* unit heights */
} Fx0deWork81; /* 0x68 */

extern Unk800F78A4 *D_801E8698;
extern s32 D_801E8CE8; /* jump scale */

s32 fx_0de_op81(s32 mode, Fx0deWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
