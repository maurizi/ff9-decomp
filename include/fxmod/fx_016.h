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

typedef struct {
    s32 unk0;
    SVECTOR unk4; /* target body */
    SVECTOR unkC; /* target */
    s32 unk14; /* models */
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34; /* scale */
} Fx016Work80; /* 0x38 */

typedef struct {
    u16 unk0;
    u16 unk2;
} Fx016Pair;

extern BtlModel D_801E8368[];
extern Fx016Pair D_801E82B0[];
extern Unk800F78A4 *D_801E8358;
extern s32 D_801E89A8; /* jump scale */

#endif
