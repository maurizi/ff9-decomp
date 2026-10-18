#ifndef FX_1FE_H
#define FX_1FE_H

/* Effect module fx_1fe (dir 13 file 0x1fe). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8;
    SVECTOR unk10;
    SVECTOR unk18;
    SVECTOR unk20; /* camera target */
    u8 pad28[0x34 - 0x28];
    s32 unk34; /* model */
    s32 unk38; /* model */
    s32 unk3C; /* model */
} Fx1feWork81; /* 0x40 */

extern BtlModel D_801E9070[];
extern SVECTOR D_801E88F8;
extern SVECTOR D_801E8900;
extern CVECTOR D_801E8908;
extern u8 D_801E8960[];
s32 fx_1fe_op81(s32 mode, Fx1feWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

/* propagated */

extern Unk800F78A4 *D_801E9060;
extern s32 D_801E95E8;
extern s32 D_801E95EC;
void func_801E7CA0(s16 *v);
s32 fx_1fe_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
