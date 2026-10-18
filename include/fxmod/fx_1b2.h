#ifndef FX_1B2_H
#define FX_1B2_H

/* Effect module fx_1b2 (dir 13 file 0x1b2). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1b2Key6;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1b2Key6 *unk4;
} Fx1b2Track6;

typedef struct {
    s16 x, y, z;
} Fx1b2Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1b2Point *unk4;
} Fx1b2Path;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    SVECTOR unk10; /* model rotation */
    VECTOR unk18; /* model position */
    VECTOR unk28; /* model scale */
    s32 unk38; /* brightness */
    SVECTOR unk3C;
    SVECTOR unk44; /* bone positions */
    SVECTOR unk4C;
    SVECTOR unk54;
    s32 unk5C; /* countdown */
    s32 unk60; /* fade countdown */
    s32 unk64;
    s32 unk68;
    u16 unk6C; /* model */
} Fx1b2Work80; /* 0x70 */

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* caster position */
    SVECTOR unk14; /* bone positions */
    SVECTOR unk1C;
} Fx1b2Work81; /* 0x24 */

typedef struct {
    SVECTOR unk0; /* model rotation */
    VECTOR unk8; /* model position */
    VECTOR unk18; /* model scale */
    s32 unk28; /* brightness */
    s32 unk2C; /* fade-in countdown */
    s32 unk30; /* fade-out countdown */
    u16 unk34; /* model */
} Fx1b2Work82; /* 0x38 */

extern Unk800F78A4 *D_801E84EC;
extern BtlModel D_801E84FC[];
extern Fx1b2Path D_801E8754;
extern u32 D_801E875C[];
extern s32 D_801E8B5C[][0x258];
extern s32 D_801EA77C[][0x258];
extern s32 D_801EC39C[];
extern s32 D_801EC89C[];

void func_801E816C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E81B0(s32 from, s32 to, s32 t, s32 n);
void func_801E8208(Fx1b2Path *path, u16 *data);
s32 func_801E8220(Fx1b2Track6 *track, s32 t, VECTOR *out);
void func_801E831C(s32 unit, s32 motion);
s32 func_801E8388(s32 unit);
void func_801E840C(s32 unit);

/* copied */
extern Unk800F78A4 *D_801E84EC;
/* end copied */

#endif
