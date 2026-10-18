#ifndef FX_19A_H
#define FX_19A_H

/* Effect module fx_19a (dir 13 file 0x19a). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801E78DC;

/* propagated */

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* caster body position */
} Fx19aWork80;
s32 fx_19a_op80(s32 mode, Fx19aWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
