#ifndef FX_0FB_0_H
#define FX_0FB_0_H

/* Effect module fx_0fb_0 (dir 13 file 0x0FB). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4[8]; /* unit home positions */
    SVECTOR unk44[8]; /* unit rotations */
    SVECTOR unk84[8]; /* spin speeds */
    SVECTOR unkC4[8]; /* current rotations */
    s32 unk104; /* unit count */
    s32 unk108; /* lift height */
} Fx0fb0Work81; /* 0x10C */

extern s32 D_801EB348; /* set when the units are put back */
extern Unk800F78A4 *D_801EB3AC;

s32 fx_0fb_0_op81(s32 mode, Fx0fb0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
