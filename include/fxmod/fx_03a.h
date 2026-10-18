#ifndef FX_03A_H
#define FX_03A_H

/* Effect module fx_03a (dir 13 file 0x3A). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* bounce countdown */
    s32 unk4; /* power-up countdown */
    s32 unk8; /* power-down countdown */
    s32 unkC; /* blur countdown */
    s32 unk10[4];
    SVECTOR unk20;
    u8 pad28[8];
} Fx03aWork; /* 0x30 */

/* bouncing model */
typedef struct {
    s32 unk0; /* model */
    VECTOR unk4; /* scale */
    VECTOR unk14; /* position */
    SVECTOR unk24; /* rotation */
    s32 unk2C; /* bounce speed */
    s32 unk30; /* bounce time */
} Fx03aModel; /* 0x34 */

extern Unk800F78A4 *D_801E7DA8;
extern Fx03aModel D_801E7DB8[10];
extern s32 D_801E7FC0; /* bounce power */
extern BtlModel D_801E7FC8[];
extern u32 D_801E8798[];

#endif
