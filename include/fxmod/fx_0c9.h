#ifndef FX_0C9_H
#define FX_0C9_H

/* Effect module fx_0c9 (dir 13 file 0xC9). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster position */
    SVECTOR unk10; /* caster body position */
} Fx0c9Work80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* unit 0x10 body position, raised by 0x50 */
    SVECTOR unk14; /* caster position */
    SVECTOR unk1C; /* moves from unk14 to unkC */
    SVECTOR unk24; /* model rotation */
    VECTOR unk2C; /* model position */
    VECTOR unk3C; /* model scale */
    SVECTOR unk4C;
    u16 unk54; /* model */
} Fx0c9Work81;

extern Unk800F78A4 *D_801E7D38;
extern BtlModel D_801E7D48[];
extern SVECTOR D_801E7E10; /* particle position */
extern SVECTOR D_801E7E18;
extern s32 D_801E7E28[];

s32 func_801E7828(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);

#endif
