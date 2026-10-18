#ifndef FX_0DC_H
#define FX_0DC_H

/* Effect module fx_0dc (dir 13 file 0x0DC). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* time */
    SVECTOR unk8; /* caster bone position */
} Fx0dcWork84;

extern Unk800F78A4 *D_801E9354;

s32 fx_0dc_op84(s32 mode, Fx0dcWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
