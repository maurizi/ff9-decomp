#ifndef FX_187_H
#define FX_187_H

/* Effect module fx_187 (dir 13 file 0x187). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    Pool6CEntry *unk4[2];
    Pool6CEntry *unkC[2];
    SVECTOR unk14; /* target position */
    SVECTOR unk1C;
    SVECTOR unk24; /* bone positions */
    SVECTOR unk2C;
    SVECTOR unk34;
    SVECTOR unk3C;
    SVECTOR unk44;
    SVECTOR unk4C;
    SVECTOR unk54;
    SVECTOR unk5C;
} Fx187Work; /* 0x64 */

extern u16 D_801E7DB8[2]; /* bones */
extern Unk800F78A4 *D_801E7E04;
extern s32 D_801E7E14[];
extern s32 D_801E7F7C[];
extern s32 D_801E80E4[];
extern s32 D_801E8364[];
extern s32 D_801E85E4[];

#endif
