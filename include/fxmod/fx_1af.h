#ifndef FX_1AF_H
#define FX_1AF_H

/* Effect module fx_1af (dir 13 file 0x1af). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* counter */
    s32 unk8[3]; /* position */
    SVECTOR unk14; /* rotation */
    s32 unk1C[3]; /* scale */
    SVECTOR unk28;
    SVECTOR unk30;
    SVECTOR unk38;
    s32 unk40[5]; /* models */
} Fx1afWork; /* 0x54 */

extern s16 D_801E7EF4[5]; /* model files */
extern Unk800F78A4 *D_801E7F48;
extern Unk80100C78Entry *D_801E7F4C;
extern BtlModel D_801E7F58[];
extern s32 D_801E8348[];

#endif
