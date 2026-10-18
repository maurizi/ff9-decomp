#ifndef FX_0EF_H
#define FX_0EF_H

/* Effect module fx_0ef (dir 13 file 0x0EF). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0[5];
    Pool6CEntry *unk14[5];
    u8 pad28[0x38 - 0x28];
    SVECTOR unk38;
    SVECTOR unk40[5];
} Fx0efWork80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8;
} Fx0efWork81;

typedef struct {
    u16 unk0; /* file */
    u16 unk2; /* frame */
    s16 unk4[3]; /* offset */
    s16 padA;
} Fx0efSpawn;

extern Fx0efSpawn D_801E7AC8[5];
extern Unk800F78A4 *D_801E7B4C;

typedef struct {
    s16 x, y, z;
} Fx0efPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx0efPoint *unk4;
} Fx0efPath;
void func_801E7A4C(Fx0efPath *path, u16 *data);
s32 func_801E7A64(Fx0efPath *path, s32 t, SVECTOR *out);

#endif
