#ifndef FX_0DA_H
#define FX_0DA_H

/* Effect module fx_0da (dir 13 file 0xDA). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0daWork84;

extern Unk800F78A4 *D_801E8220;
extern BtlModel D_801E8230[];

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* models */
    s32 unk8;
    s32 unkC;
    s32 unk10;
    SVECTOR unk14; /* caster body position */
    SVECTOR unk1C; /* caster position */
    s32 unk24; /* frame counter */
} Fx0daWork80;

#endif
