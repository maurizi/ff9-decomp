#ifndef FX_016_H
#define FX_016_H

/* Effect module fx_016 (dir 13 file 0x16). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0[8]; /* target home positions */
    s32 unk40; /* target count */
    s32 unk44; /* jump height */
    s32 unk48[8]; /* target heights */
} Fx016Work81; /* 0x68 */

extern Unk800F78A4 *D_801E8358;
extern s32 D_801E89A8; /* jump scale */

#endif
