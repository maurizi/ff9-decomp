#ifndef FX_0D7_H
#define FX_0D7_H

/* Effect module fx_0d7 (dir 13 file 0xD7). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* func_800E7D3C(0x10, 1) */
} Fx0d7Work84;

extern Unk800F78A4 *D_801E88C0;
extern u8 D_801E88D0[][0x84];
extern u8 D_801E8F00[][0x48];

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
    u8 unk10; /* index */
    u8 unk11; /* state */
    u8 unk12;
    u8 unk13;
    s16 unk14; /* kind */
    s16 unk16;
    s32 *unk18;
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
} Fx0d7Part; /* pool entry data */

/* fx_003 copies (asm) */
void func_801E7704(Unk80100C78Entry *entry, u8 *buf, s32 *data, void *part, SVECTOR *pos, s32 arg5, s32 arg6, s32 arg7,
                   s32 arg8, s32 frame, s32 s, s32 r, s32 kind);
void func_801E7BF8(Unk80100C78Entry *entry, u8 *buf, u8 *buf2, s32 *data, s32 arg4, s32 s, s32 b, s32 kind);
s32 func_801E7CEC(s32 mode, Fx0d7Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
