#ifndef FX_090_H
#define FX_090_H

/* Effect module fx_090 (dir 13 file 0x90). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    s32 unk4;
    s32 unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    Pool6CEntry *unk18;
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
    SVECTOR unk24; /* model rotation */
    VECTOR unk2C; /* model position */
    VECTOR unk3C; /* model scale */
    s32 unk4C; /* model brightness */
    SVECTOR unk50; /* position: env->targets->centerX..Z */
    SVECTOR unk58; /* unk50 raised by 0x320 */
    SVECTOR unk60; /* unk50 raised by 0x4B0 */
    s32 unk68; /* fade-in countdown */
    s32 unk6C; /* fade-out countdown */
    u16 unk70; /* models */
    u16 unk72;
} Fx090Work; /* 0x74 */

extern s16 D_801E7DB4[][2];
extern Unk800F78A4 *D_801E7E38;
extern BtlModel D_801E7E48[];
extern s32 D_801E7FD8[];

void func_801E7D18(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7D5C(s32 from, s32 to, s32 t, s32 n);

#endif
