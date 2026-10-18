#ifndef FX_001_H
#define FX_001_H

/* Effect module fx_001 (dir 13 file 0x01). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    s32 unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    Pool6CEntry *unk18;
    Pool6CEntry *unk1C;
    SVECTOR unk20; /* model rotation */
    VECTOR unk28; /* model position */
    VECTOR unk38; /* model scale */
    s32 unk48; /* model brightness */
    SVECTOR unk4C; /* target body position */
    SVECTOR unk54;
    SVECTOR unk5C; /* caster position */
    s32 unk64; /* fade-in countdown */
    s32 unk68; /* fade-out countdown */
    u16 unk6C; /* models */
    u16 unk6E;
} Fx001Work; /* 0x70 */

extern Unk800F78A4 *D_801E7E20;
extern s16 D_801E7D9C[][2];
extern BtlModel D_801E7E30[];
extern s32 D_801E7FC0[];

void func_801E7D00(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7D44(s32 from, s32 to, s32 t, s32 n);

#endif
