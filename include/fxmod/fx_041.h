#ifndef FX_041_H
#define FX_041_H

/* Effect module fx_041 (dir 13 file 0x41). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx041Point;

/* point list read from the effect file */
typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx041Point *unk4;
} Fx041Path;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    SVECTOR unk18; /* target body position */
    SVECTOR unk20; /* target position */
    SVECTOR unk28; /* caster position */
} Fx041Work80; /* 0x30 */

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* target position */
    SVECTOR unk14;
} Fx041Work81; /* 0x1C */

extern Unk800F78A4 *D_801E7F3C;
extern Fx041Path D_801E7F44;
extern SVECTOR D_801E7F4C; /* target position */
extern SVECTOR D_801E7F54; /* last path point */
extern SVECTOR D_801E7F5C;
extern s32 D_801E7F6C[];

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 t, s32 n);
void func_801E7CF0(Fx041Path *path, u16 *data);
s32 func_801E7D08(Fx041Path *path, s32 t, SVECTOR *out);
void func_801E7D6C(s32 unit, s32 motion);
s32 func_801E7DD8(s32 unit);
void func_801E7E5C(s32 unit);
void func_801E7EA8(s32 unit);

#endif
