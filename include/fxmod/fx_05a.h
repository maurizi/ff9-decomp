#ifndef FX_05A_H
#define FX_05A_H

/* Effect module fx_05a (dir 13 file 0x5A). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* target position */
    SVECTOR unk10; /* effect position (raised for big enemies) */
    SVECTOR unk18; /* model rotation */
    VECTOR unk20; /* model position */
    VECTOR unk30; /* model scale */
    s16 unk40; /* model brightness */
    s16 unk42; /* screen tint */
    u16 unk44; /* model */
} Fx05aWork; /* 0x48 */

extern Unk800F78A4 *D_801E7B10;
extern BtlModel D_801E7B20[];
extern s32 D_801E7BF0[];

#endif
