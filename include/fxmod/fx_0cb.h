#ifndef FX_0CB_H
#define FX_0CB_H

/* Effect module fx_0cb (dir 13 file 0xCB). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* scale */
    SVECTOR unk4; /* position */
    u8 padC[0x14 - 0xC];
    s16 unk14; /* spin angle */
    s16 unk16; /* spin speed */
} Fx0cbPart;

typedef struct {
    u8 pad0[0x20];
    s32 unk20; /* model */
    s32 unk24; /* model */
} Fx0cbWork;

extern Unk800F78A4 *D_801E8670;

void func_801E7EB8(s16 *a, s16 *b);
s32 func_801E7EFC(s32 mode, Fx0cbPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx0cbWork *work);

#endif
