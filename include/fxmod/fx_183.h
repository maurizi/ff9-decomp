#ifndef FX_183_H
#define FX_183_H

/* Effect module fx_183 (dir 13 file 0x183). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx183Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx183Point *unk4;
} Fx183Path;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* caster rotation */
} Fx183Work80; /* 0x18 */

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster position */
    SVECTOR unk10; /* target position */
    SVECTOR unk18; /* from */
    SVECTOR unk20; /* model rotation */
    VECTOR unk28; /* model position */
    VECTOR unk38; /* model scale */
    u16 unk48; /* model */
} Fx183Work81; /* 0x4C */

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* model rotation */
    VECTOR unkC; /* model position */
    VECTOR unk1C; /* model scales */
    VECTOR unk2C;
    s32 unk3C; /* brightness */
    SVECTOR unk40; /* target position */
    s32 unk48; /* fade-in countdown */
    s32 unk4C; /* fade-out countdown */
    u16 unk50[6]; /* models */
} Fx183Work82; /* 0x5C */

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* target body position */
    s32 unk14; /* unit */
} Fx183Work83; /* 0x18 */

extern u16 D_801E8288[6]; /* model files */
extern u16 D_801E8294[6]; /* path files */
extern Unk800F78A4 *D_801E82E8;
extern BtlModel D_801E82F8[];
extern Fx183Path D_801E8878[6];
extern s32 D_801E88A8[][0xF0];

void func_801E8140(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E8184(s32 from, s32 to, s32 t, s32 n);
void func_801E81DC(Fx183Path *path, u16 *data);
s32 func_801E81F4(Fx183Path *path, s32 t, SVECTOR *rot, SVECTOR *pos);

/* copied */
extern Unk800F78A4 *D_801E82E8;
/* end copied */

#endif
