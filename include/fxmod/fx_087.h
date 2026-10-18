#ifndef FX_087_H
#define FX_087_H

/* Effect module fx_087 (dir 13 file 0x87). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* unit 0x10 body position */
    SVECTOR unk14; /* caster body position */
    SVECTOR unk1C; /* func_800DC818(0x10, 0x17) */
    u8 pad24[0x2C - 0x24];
    SVECTOR unk2C; /* angles from unkC to unk14 */
} Fx087Work;

extern Unk800F78A4 *D_801E7958;
extern s32 D_801E7968[];

#endif
