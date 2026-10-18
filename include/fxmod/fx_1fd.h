#ifndef FX_1FD_H
#define FX_1FD_H

/* Effect module fx_1fd (dir 13 file 0x1fd). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1fdPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1fdPoint *unk4;
} Fx1fdPath;

typedef struct {
    Pool6CEntry *unk0[5];
    SVECTOR unk14[5];
} Fx1fdWork80; /* 0x3C */

extern u16 D_801E9DF8[5]; /* files */
extern BtlModel D_801E9E5C[];
extern Fx1fdPath D_801EBB7C[5];
extern s32 D_801EBBAC[5][0x78];
extern SVECTOR D_801ED70C[5];

typedef struct {
    Pool6CEntry *unk0[8];
    Pool6CEntry *unk20[8];
    SVECTOR unk40[8]; /* unit positions */
    SVECTOR unk80[8];
    s16 unkC0[8]; /* landed */
} Fx1fdWork84; /* 0xD0 */

typedef struct {
    u8 pad0[8];
    SVECTOR unk8; /* rotation */
    u8 pad10[0x14 - 0x10];
} Fx1fdPose; /* 0x14 */

extern Fx1fdPose D_801EBADC[8];
extern s32 D_801EC50C[8][0x90];

s32 func_801E9984(s32 unit, s32 frame, s32 n, SVECTOR *pos);
u16 *func_801E9710(u8 *data, u16 i);
void func_801E97C4(Fx1fdPath *path, u16 *data);
s32 func_801E97DC(Fx1fdPath *path, s32 t, SVECTOR *out);
void func_801E9A50(void);
void func_801E9A9C(s32 arg0, s32 arg1, s32 arg2, u16 *data);
void func_801E9AF8(s32 arg0);

/* copied */
extern Unk800F78A4 *D_801E9E4C;
/* end copied */

#endif
