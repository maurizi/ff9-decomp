#ifndef FX_088_H
#define FX_088_H

/* Effect module fx_088 (dir 13 file 0x88). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* unit 0x10 body position */
    SVECTOR unk10; /* func_800DC818(0x10, 0x17) */
    SVECTOR unk18; /* angles from unk8 to env->targets->centerX */
} Fx088Work80;

typedef struct {
    SVECTOR unk0; /* rotation step total */
    SVECTOR unk8; /* position: env->targets->centerX..Z */
} Fx088Work81;

extern Unk800F78A4 *D_801E7DA0;
extern s32 D_801E7DB0[];

s32 func_801E7984(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta);
s32 func_801E7B00(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang);

#endif
