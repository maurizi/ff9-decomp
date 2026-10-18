#ifndef FX_1A4_H
#define FX_1A4_H

/* Effect module fx_1a4 (dir 13 file 0x1a4). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14;
    SVECTOR unk1C;
    SVECTOR unk24; /* caster position */
    SVECTOR unk2C;
    s32 unk34; /* brightness */
    s32 unk38; /* flag */
    s16 unk3C; /* countdowns */
    s16 unk3E;
    s16 unk40;
} Fx1a4Work80; /* 0x44 */

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* model rotations */
    SVECTOR unk10;
    SVECTOR unk18;
    VECTOR unk20; /* model position */
    VECTOR unk30; /* model scale */
    s32 unk40; /* brightness */
    SVECTOR unk44; /* caster body position */
    s16 unk4C; /* counter */
    s16 unk4E;
    s16 unk50;
    s16 unk52; /* fade countdown */
    u16 unk54; /* models */
    u16 unk56;
    u16 unk58;
} Fx1a4Work81; /* 0x5C */

extern Unk800F78A4 *D_801E8168;
extern BtlModel D_801E8178[];
extern SVECTOR D_801E83D0;
extern SVECTOR D_801E83D8;

void func_801E8084(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E80C8(s32 from, s32 to, s32 t, s32 n);

/* copied */
extern Unk800F78A4 *D_801E8168;
/* end copied */

#endif
