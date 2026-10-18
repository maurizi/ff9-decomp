#ifndef FX_11E_H
#define FX_11E_H

/* Effect module fx_11e (dir 13 file 0x11E). */
#include "btlfx_mod.h"

void func_801E7F70(Unk800F78A4 *env, s32 i, s32 v);


typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* model rotation */
    VECTOR unkC; /* model position */
    VECTOR unk1C; /* model scale */
    u8 pad2C[0x30 - 0x2C];
    s32 unk30; /* radius direction */
    s32 unk34; /* angle direction */
    s32 unk38; /* radius */
    s32 unk3C; /* angle */
    s32 unk40; /* radius direction timer */
    s32 unk44; /* angle direction timer */
    SVECTOR unk48; /* target body position */
    SVECTOR unk50;
    s32 unk58; /* target */
    u16 unk5C; /* models */
    u16 unk5E;
} Fx11eWork; /* 0x60 */
extern s32 D_801E7F84; /* models registered */
extern Unk800F78A4 *D_801E7FD0;
extern BtlModel D_801E7FE0[];
s32 fx_11e_op80(s32 mode, Fx11eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
