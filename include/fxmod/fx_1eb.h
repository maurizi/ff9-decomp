#ifndef FX_1EB_H
#define FX_1EB_H

/* Effect module fx_1eb (dir 13 file 0x1eb). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* key count */
    u8 *unk4; /* keys */
} Fx1ebPath;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* caster rotation */
} Fx1ebWork80; /* 0x18 */

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    SVECTOR unk18; /* model rotation */
    VECTOR unk20; /* model position */
    VECTOR unk30; /* model scale */
    s32 unk40; /* brightness */
    SVECTOR unk44; /* target position */
    s32 unk4C; /* fade countdown */
    u16 unk50;
    u16 unk52[3]; /* models */
    u16 unk58; /* morph target */
} Fx1ebWork82; /* 0x60 */

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
    s32 unkC; /* unit */
} Fx1ebWork83;

extern u16 D_801E8210[3]; /* model files */
extern u16 D_801E8218[3]; /* path files */
extern Unk800F78A4 *D_801E8268;
extern BtlModel D_801E8278[];
extern Fx1ebPath D_801E8728[3];
extern Fx1ebPath D_801E8740;
extern s32 D_801E8748[];

void func_801E806C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E80B0(s32 from, s32 to, s32 t, s32 n);
void func_801E8108(Fx1ebPath *path, u8 *data);
s32 func_801E8120(Fx1ebPath *path, s32 t, SVECTOR *rot, SVECTOR *pos);
void func_801E81B4(Fx1ebPath *path, u8 *data);
s32 func_801E81CC(Fx1ebPath *path, s32 t, s32 *out);

/* copied */
extern Unk800F78A4 *D_801E8268;
/* end copied */

#endif
