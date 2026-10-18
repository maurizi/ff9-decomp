#ifndef FX_0B3_H
#define FX_0B3_H

/* Effect module fx_0b3 (dir 13 file 0xB3). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0[8]; /* enemy positions */
    s32 unk40; /* unit count */
} Fx0b3Work86;

extern Unk800F78A4 *D_801ED5E0;
extern s32 D_801EB5D4;
extern SVECTOR D_801EE340[16];

typedef struct {
    u16 unk0; /* file id, track 0 */
    u16 unk2; /* file id, track 1 */
    u16 unk4; /* end frame */
    u16 unk6;
} Fx0b3Track87;

extern Fx0b3Track87 D_801EB5E8[];

void func_801E7704(u8 *data, s32 frame, SVECTOR *out);

s32 func_801E9DB8(s32 a);
s32 func_801EA6C8(SVECTOR *v);

#endif
