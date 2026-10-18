#ifndef FX_1BF_0_H
#define FX_1BF_0_H

/* Effect module fx_1bf_0 (dir 13 file 0x1bf). */
#include "btlfx_mod.h"

/* copied */
extern SVECTOR D_801E87BC[3];
extern s16 D_801E879C[16];
extern Unk800F78A4 *D_801E8830;
/* end copied */

/* propagated */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[2];
} Fx1bf0Line;

extern Fx1bf0Line D_801E881C;
void func_801E7704(u8 *file, SVECTOR *a, SVECTOR *b, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern s16 D_801E86C4[18 * 4]; /* per strand: file, start, stride, width */
extern Fx1bf0Line *D_801E8754[18];
void func_801E77C0(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 w, s32 a, s32 fade, s32 flag);
extern s16 D_801E87B4[];

typedef struct {
    s16 x, y, z;
} Fx1bf0Pt;
void func_801E7B70(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot);

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8;
    SVECTOR unk10;
    u8 pad18[0x28 - 0x18];
} Fx1bf0Work; /* 0x28 */

extern SVECTOR D_801E86BC;
extern BtlModel D_801EA5FC[];
extern u8 D_801EA1FC[];
s32 fx_1bf_0_op80(s32 mode, Fx1bf0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
