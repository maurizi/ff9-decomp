#ifndef FX_123_H
#define FX_123_H

/* Effect module fx_123 (dir 13 file 0x123). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0[5];
    SVECTOR unk14; /* centre of the units */
} Fx123Work82;

typedef struct {
    s16 unk0; /* fade-in countdown */
    s16 unk2; /* fade-out countdown */
} Fx123Work83;

typedef struct {
    Pool6CEntry *unk0[6];
    Pool6CEntry *unk18[6];
    Pool6CEntry *unk30[6];
    Pool6CEntry *unk48;
    SVECTOR unk4C; /* caster position */
    SVECTOR unk54;
    SVECTOR unk5C[6]; /* bone positions */
} Fx123Work80; /* 0x8C */

typedef struct {
    u16 unk0; /* bone */
    u16 unk2; /* offset */
} Fx123Bone;

extern Fx123Bone D_801E8F64[6];
extern Unk800F78A4 *D_801E90CC;
extern SVECTOR D_801E9BFC[6];
extern SVECTOR D_801E9C2C[6];

s32 fx_123_op80(s32 mode, Fx123Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_123_op82(s32 mode, Fx123Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
void func_801E8D84(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E8DC8(s32 from, s32 to, s32 t, s32 n);

typedef struct {
    s16 x, y, z;
} Fx123Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx123Point *unk4;
} Fx123Path;
void func_801E8E20(Fx123Path *path, u16 *data);
void func_801E8ECC(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);

/* propagated */
extern SVECTOR D_801E968C[]; /* particle positions */
s32 func_801E77B4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E77E8(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E781C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7850(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7884(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
extern SVECTOR D_801E96B4[]; /* particle positions */
s32 func_801E78B8(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E78EC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7920(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7954(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7988(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E79BC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E79F0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7A24(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n);

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx123Key; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx123Key *unk4;
} Fx123Path_0b4;
s32 func_801E8E38(Fx123Path_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);

/* PsyQ libgte.h macros: candidates for psyq.h */
#define copyVector(d, s) (d)->vx = (s)->vx, (d)->vy = (s)->vy, (d)->vz = (s)->vz
#define addVector(d, s) (d)->vx += (s)->vx, (d)->vy += (s)->vy, (d)->vz += (s)->vz

typedef struct {
    Pool6CEntry *unk0[6];
    u8 pad18[0x30 - 0x18];
    Pool6CEntry *unk30[6];
    Pool6CEntry *unk48[6];
    Pool6CEntry *unk60;
    SVECTOR unk64;
    SVECTOR unk6C[6];
    VECTOR unk9C[6];
    VECTOR unkFC;
    s32 unk10C;
    SVECTOR unk110[6];
    SVECTOR unk140;
    SVECTOR unk148[6];
    s16 unk178;
    u16 unk17A[6];
    u16 unk186;
} Fx123Work81; /* 0x188 */

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
    SVECTOR unk10;
    u16 unk18;
    u16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} Fx123Shot; /* 0x24 */

extern Fx123Shot D_801E8F7C[6];
extern u16 D_801E9054[6];
extern u16 D_801E9060[6];
extern s32 D_801E906C[6];
extern BtlModel D_801E90DC[];
extern Fx123Path_0b4 D_801E965C[6];
extern u8 D_801E96BC[6][0xE0];
extern SVECTOR D_801E9C5C[6];
extern SVECTOR D_801E9C8C[6];

#endif
