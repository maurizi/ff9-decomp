#ifndef FX_0E1_0_H
#define FX_0E1_0_H

/* Effect module fx_0e1_0 (dir 13 file 0x0E1). */
#include "btlfx_mod.h"

/* the effect environment block as these callbacks read it */
typedef struct {
    u8 pad0[0x60];
    s32 unk60; /* summon model */
} Fx0e10EnvB;

extern s32 D_801E9D84[];
extern s32 D_801E9DB8;
extern s32 D_801E9DBC;
extern Unk800F78A4 *D_801E9E38;

s32 func_801E8844(SVECTOR *out);
void func_801E88F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 *v);

#endif
