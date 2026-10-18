#ifndef FX_180_H
#define FX_180_H

/* Effect module fx_180 (dir 13 file 0x180). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* key count */
    u8 *unk4; /* keys */
} Fx180Path;

typedef struct {
    Pool6CEntry *unk0[9];
    SVECTOR unk24; /* caster body position */
    SVECTOR unk2C; /* caster position */
    SVECTOR unk34; /* target position */
    SVECTOR unk3C; /* bone position */
    SVECTOR unk44;
    SVECTOR unk4C;
    SVECTOR unk54; /* camera targets */
    SVECTOR unk5C;
    SVECTOR unk64;
    SVECTOR unk6C;
} Fx180Work80; /* 0x74 */

typedef struct {
    Pool6CEntry *unk0[11];
    SVECTOR unk2C; /* target position */
    SVECTOR unk34;
    SVECTOR unk3C;
    SVECTOR unk44;
    SVECTOR unk4C;
} Fx180Work81; /* 0x54 */

typedef struct {
    Pool6CEntry *unk0[8];
    SVECTOR unk20; /* model rotation */
    VECTOR unk28; /* model position */
    VECTOR unk38; /* model scale */
    s32 unk48; /* brightness */
    SVECTOR unk4C; /* target position */
    SVECTOR unk54;
    SVECTOR unk5C;
    s32 unk64; /* fade countdown */
    u16 unk68; /* models */
    u16 unk6A;
} Fx180Work82; /* 0x6C */

typedef struct {
    Pool6CEntry *unk0;
    u8 pad4[8 - 4];
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* caster position */
    SVECTOR unk18; /* destination */
    SVECTOR unk20; /* caster position */
    SVECTOR unk28;
    SVECTOR unk30;
    s16 unk38; /* fade countdown */
    s16 unk3A;
} Fx180Work83; /* 0x3C */

extern Unk800F78A4 *D_801E9258;
extern BtlModel D_801E9268[];
extern Fx180Path D_801E93F8[0x12];
extern Fx180Path D_801E9488[0x1E];
extern s32 D_801E9578[];
extern s32 D_801E9BB8[];
extern s32 D_801EA398[];
extern s32 D_801EB088[];
extern s32 D_801EBCE8[];
extern s32 D_801EC6E8[];
extern s32 D_801EDE88[];
extern s32 D_801EF3E8[];
extern s32 D_801EFF28[];
extern SVECTOR D_801F0828;
extern SVECTOR D_801F0830;
extern SVECTOR D_801F0838;
extern SVECTOR D_801F0840;

s32 func_801E78E0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E79DC(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
u8 *func_801E8DA4(u8 *data, u16 i);
s32 func_801E8E00(s32 from, s32 to, s32 t, s32 n);
void func_801E8E58(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 layer);
void func_801E8EF0(Fx180Path *path, u8 *data);
s32 func_801E8F08(Fx180Path *path, s32 t, SVECTOR *out);
void func_801E8F6C(s32 unit, s32 motion);
s32 func_801E8FD8(s32 unit);
void func_801E90A8(s32 unit);
s32 func_801E90F4(s32 unit, SVECTOR *to, s32 t, s32 n, s32 height, SVECTOR *from);

/* copied */
extern Unk800F78A4 *D_801E9258;
/* end copied */

#endif
