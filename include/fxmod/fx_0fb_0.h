#ifndef FX_0FB_0_H
#define FX_0FB_0_H

/* Effect module fx_0fb_0 (dir 13 file 0x0FB). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4[8]; /* unit home positions */
    SVECTOR unk44[8]; /* unit rotations */
    SVECTOR unk84[8]; /* spin speeds */
    SVECTOR unkC4[8]; /* current rotations */
    s32 unk104; /* unit count */
    s32 unk108; /* lift height */
} Fx0fb0Work81; /* 0x10C */

typedef struct {
    s32 unk0; /* state */
    s16 unk4; /* kind */
    s16 unk6; /* bob height, then phase */
    s32 unk8; /* frame */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    u16 unk1C; /* spin speed */
    u16 unk1E;
    s32 unk20; /* model */
} Fx0fb0Rock;

extern s32 D_801EB344; /* set when the small rocks go */
extern s32 D_801EB348; /* set when the units are put back */
extern Unk800F78A4 *D_801EB3AC;

s32 func_801E8540(s32 mode, Fx0fb0Rock *r, Unk800F78A4 *env);
s32 fx_0fb_0_op81(s32 mode, Fx0fb0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx0fb0Seg; /* 0x10 */
void func_801E7720(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx0fb0Seg *buf, s32 abr);

typedef struct {
    s32 vx, vy, vz;
} Fx0fbVec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    s32 unk8; /* rocks spawned */
    SVECTOR unkC; /* base position */
    SVECTOR unk14;
    SVECTOR unk1C;
    SVECTOR unk24;
    Fx0fbVec3 unk2C; /* model position */
    Fx0fbVec3 unk38; /* model scale */
    SVECTOR unk44; /* model rotation */
    SVECTOR unk4C;
    SVECTOR unk54;
    SVECTOR unk5C; /* summon position */
    SVECTOR unk64;
    s32 unk6C[27]; /* rock models */
    Fx0fbVec3 unkD8;
    Fx0fbVec3 unkE4;
    SVECTOR unkF0;
    Fx0fbVec3 unkF8;
    Fx0fbVec3 unk104;
    SVECTOR unk110;
    s32 unk118; /* models */
    s32 unk11C;
    s32 unk120;
    s32 unk124;
} Fx0fb0Work80; /* 0x128 */

typedef struct {
    u8 data[0x84];
} Fx0fb0Trail16; /* 4 + 16 points */

typedef struct {
    Fx0fb0Seg seg[12];
} Fx0fb0SegBuf; /* 0xC0 */

typedef struct {
    s16 x;
    s16 z;
} Fx0fb0Ofs;

typedef struct {
    u8 pad0[0x38];
    BtlModel *unk38;
    u8 pad3C[0x18];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180[8]; /* summon motions */
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0fb0Summon;

#define FX0FB_W(v, i) (((s32 *)&(v))[i])
#define SUMMON0FB(env) ((Fx0fb0Summon *)(env)->unk4)

void func_801E7B90(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 col, s32 tpage, s32 clut, s32 u, s32 v, s32 amp,
                   s32 ph, s32 step, s32 abr);
void func_801E809C(SVECTOR *pos, s32 dy, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 ph, s32 step,
                   s32 abr, s32 ph2);
extern SVECTOR D_801EB248;
extern s16 D_801EB250[4][2];
extern s16 D_801EB260[10];
extern u16 D_801EB274[27];
extern s16 D_801EB2AC[25];
extern s16 D_801EB2E0[];
extern CVECTOR D_801EB34C[];
extern CVECTOR D_801EB358[];
extern CVECTOR D_801EB35C[];
extern SVECTOR D_801EB3BC[10];
extern SVECTOR D_801EB3C4[];
extern Fx0fb0SegBuf D_801EB40C[4];
extern Fx0fb0Trail16 D_801EB70C[10];

#endif
