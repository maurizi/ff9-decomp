#ifndef FX_17A_H
#define FX_17A_H

/* Effect module fx_17a (dir 13 file 0x17A). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0; /* state */
    s16 unk2;
    s16 unk4; /* size */
    s16 unk6;
    s16 unk8;
    s16 unkA; /* drift */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C;
    s16 unk1E;
    s32 unk20;
    s32 unk24;
} Fx17aPart;

s32 func_801E7BC0(s32 mode, Fx17aPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
