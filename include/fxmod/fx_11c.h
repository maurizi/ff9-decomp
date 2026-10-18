#ifndef FX_11C_H
#define FX_11C_H

/* Effect module fx_11c (dir 13 file 0x11C). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10;
} Fx11cWork80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4[5];
    Pool6CEntry *unk18;
    Pool6CEntry *unk1C;
    SVECTOR unk20; /* target position */
} Fx11cWork81;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    SVECTOR unk18;
    SVECTOR unk20;
    SVECTOR unk28;
} Fx11cWork82;

typedef struct {
    SVECTOR unk0; /* spawn position */
    SVECTOR unk8;
    u16 unk10; /* started */
    u16 unk12; /* age */
    u16 unk14; /* start frame */
    u16 unk16; /* file */
} Fx11cBurst;

extern Fx11cBurst D_801E7EDC[5];
extern Unk800F78A4 *D_801E7F9C;
extern s32 D_801E7FAC[];
extern s32 D_801E86FC[];
extern s32 D_801E8E4C[];

#endif
