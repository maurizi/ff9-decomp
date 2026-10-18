#ifndef FX_048_H
#define FX_048_H

/* Effect module fx_048 (dir 13 file 0x48). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* caster position */
    SVECTOR unk8; /* caster position, y = 0 */
    s32 unk10; /* state */
    s32 unk14[4]; /* models */
    s32 unk24; /* model */
    s32 unk28; /* model */
    Pool6CEntry *unk2C;
    Pool6CEntry *unk30;
    s32 unk34; /* frame counter */
} Fx048Work; /* 0x38 */

extern Unk800F78A4 *D_801E8F3C;
extern BtlModel D_801E8F4C[];
extern s32 D_801E93FC[];
extern u16 D_801E968C[];
void func_801E7704(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

s32 fx_048_op80(s32 mode, Fx048Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
