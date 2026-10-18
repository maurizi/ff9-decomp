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
