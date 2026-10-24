#ifndef FX_0D5_H
#define FX_0D5_H

/* Effect module fx_0d5 (dir 13 file 0xD5). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4; /* caster position */
    SVECTOR unkC; /* caster body position */
} Fx0d5Work80;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d5Work84;

extern Unk800F78A4 *D_801E7E30;
extern s32 D_801E7E40[];

#endif
