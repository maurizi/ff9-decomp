#ifndef FX_0F4_H
#define FX_0F4_H

/* Effect module fx_0f4 (dir 13 file 0x0F4). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14; /* model rotation */
    VECTOR unk1C; /* model position */
    VECTOR unk2C; /* model scale */
    s32 unk3C; /* model brightness */
    SVECTOR unk40; /* model 2 rotation */
    VECTOR unk48; /* model 2 scale */
    SVECTOR unk58; /* centre */
    SVECTOR unk60;
    s16 unk68; /* fade-in countdown */
    s16 unk6A; /* fade-out countdown */
    u16 unk6C; /* model */
    u16 unk6E; /* model 2 */
} Fx0f4Work; /* 0x70 */

extern Unk800F78A4 *D_801E7DC0;
extern BtlModel D_801E7DD0[];

s32 fx_0f4_op80(s32 mode, Fx0f4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

void func_801E7CDC(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7D20(s32 from, s32 to, s32 t, s32 n);

#endif
