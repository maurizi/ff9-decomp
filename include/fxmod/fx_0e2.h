#ifndef FX_0E2_H
#define FX_0E2_H

/* Effect module fx_0e2 (dir 13 file 0x0E2). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[1]; /* (size - 4) / 8 points */
} Fx0e2Trail;

typedef struct {
    DVECTOR unk0; /* screen xy of one edge end */
    DVECTOR unk4; /* screen xy of the other */
    s32 unk8; /* ot z */
} Fx0e2Edge;

typedef struct {
    SVECTOR pos0; /* unit position */
    SVECTOR rot8; /* unit rotation */
    s16 unk10; /* distance from the centre */
    s16 unk12; /* angle around it */
} Fx0e2Orbit;

extern Unk800F78A4 *fx_0e2_env;
extern Fx0e2Orbit D_801EE8FC[];
extern u8 D_801EFDEC[][0x240]; /* per-unit texture animation */

typedef struct {
    Pool6CEntry *emit0[8];
    Pool6CEntry *emit20[8];
    SVECTOR unk40[8]; /* unit positions */
    SVECTOR unk80[8]; /* effect positions */
    s16 unkC0[8]; /* unit landed */
} Fx0e2Work84; /* 0xD0 */
extern Fx0e2Edge D_801EE62C[60];
extern SVECTOR D_801F0FEC; /* trail origin */
extern SVECTOR D_801F0FF4; /* trail head position */
extern SVECTOR D_801F0FFC; /* trail head rotation */
extern SVECTOR D_801F1004; /* trail aim target */

void func_801E77C0(void *trackA, void *trackB, s32 t, Fx0e2Trail *trail, s32 size, s32 pad);
void func_801E7A0C(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 pad);
void func_801E7BE8(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad);
void func_801E7D5C(void *trackA, void *trackB, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad, s32 t,
                   s32 rotY);
void func_801E7FBC(void *trackA, void *trackB, s32 arg2, s32 arg3, SVECTOR *scale, s32 w, s32 t, s32 rotY, s32 twist);
void func_801E83B0(void *track, s32 arg1, s32 arg2, SVECTOR *scale, s32 w, s32 rotY);
void func_801E86D0(s32 n, s32 c, s32 tpage, s32 clut, u16 v0, u16 v1);
typedef struct {
    Pool6CEntry *emit0[5];
    SVECTOR rot14; /* tower rotation */
    VECTOR pos1C; /* tower position */
    VECTOR scale2C; /* tower scale */
    SVECTOR rot3C; /* piece rotation */
    VECTOR pos44; /* piece position */
    VECTOR scale54; /* piece scale */
    SVECTOR unk64[4]; /* block rotations */
    VECTOR unk84[4]; /* block positions */
    VECTOR unkC4[4]; /* block scales */
    SVECTOR unk104[4]; /* effect positions */
    u16 model124[12]; /* models: 3 tower stages, 5 pieces, 4 blocks */
} Fx0e2Work81; /* 0x13C */

extern u16 D_801ECB68[3];
extern u16 D_801ECB70[5];
extern u32 D_801EE9CC[];
extern SVECTOR D_801F103C[4];

s32 fx_0e2_op81(s32 mode, Fx0e2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_0e2_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_0e2_op84(s32 mode, Fx0e2Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);
static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

typedef struct {
    s16 x, y, z;
} Fx0e2Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx0e2Point *unk4;
} Fx0e2Path;
static void fxm_posPathInit(Fx0e2Path *path, u16 *data);
extern Fx0e2Path D_801EE99C[5]; /* piece tracks */
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *motion180[8]; /* summon motions */
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0e2Summon;

typedef s32 (*Fx0e2RegTexList)(BtlUnit9C8 *, BtlTexInfo *, s16 *);

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8[5];
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
    Pool6CEntry *emit24;
    Pool6CEntry *emit28[5];
    SVECTOR rot3C; /* summon rotation */
    VECTOR pos44; /* summon position */
    VECTOR pos54; /* model position */
    VECTOR scale64; /* scale */
    s32 unk74;
    s32 unk78;
    SVECTOR rot7C;
    VECTOR pos84;
    VECTOR scale94;
    VECTOR unkA4;
    s32 unkB4;
    SVECTOR rotB8;
    VECTOR posC0;
    VECTOR scaleD0;
    SVECTOR rotE0;
    VECTOR posE8;
    VECTOR scaleF8;
    u8 pad108[8];
    SVECTOR unk110;
    SVECTOR unk118;
    SVECTOR unk120;
    s32 unk128;
    s32 unk12C;
    s32 unk130;
    s32 unk134;
    s32 unk138;
    s32 unk13C;
    s32 unk140;
    s32 unk144;
    s32 unk148;
    s16 unk14C;
    s16 unk14E;
    u16 model150[14]; /* models */
} Fx0e2Work80; /* 0x16C */

typedef struct {
    Fx0e2Path unk0;
    Fx0e2Path unk8;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
} Fx0e2Ribbon80;

typedef struct {
    Fx0e2Path unk0;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
} Fx0e2Path80;

typedef struct {
    s32 unk0[2][4]; /* positions */
    s32 unk20[3]; /* start position */
    u8 pad2C[4];
    s16 unk30[3]; /* velocity */
    s16 pad36;
} Fx0e2Drop; /* 0x38 */

typedef struct {
    u8 data[0x98];
} Fx0e2Trail98;

typedef struct {
    u8 data[0x1E0];
} Fx0e2Tex1E0;

static void fxm_rotPosPathInit(Fx0e2Path *path, u16 *data);
static s32 fxm_posPathGet(Fx0e2Path *path, s32 t, SVECTOR *out);
extern Fx0e2Drop D_801ECAC0[3];
extern u16 D_801ECB7C[3];
extern u16 D_801ECB84[4];
extern u16 D_801ECB8C[5];
extern u16 D_801ECB98[5];
extern BtlTexInfo D_801ECBA4[];
extern s16 D_801ECBB4[];
extern BtlModel fx_0e2_effModels[];
extern Fx0e2Ribbon80 D_801EE51C;
extern Fx0e2Path D_801EE524;
extern s16 D_801EE52E;
extern Fx0e2Path80 D_801EE53C;
extern Fx0e2Path80 D_801EE55C;
extern Fx0e2Trail98 D_801EEDCC[5];
extern Fx0e2Tex1E0 D_801EF0CC[5];
extern u8 D_801EFA2C[];

typedef struct {
    Fx0e2Path unk0;
    Fx0e2Path unk8;
    SVECTOR unk10; /* scale */
    s16 unk18;
    s16 unk1A; /* length */
    s16 unk1C; /* brightness */
    s16 unk1E; /* width */
    s16 unk20;
    s16 unk22;
    u8 pad24[0xC];
} Fx0e2Ribbon; /* 0x30 */

typedef struct {
    Fx0e2Path unk0;
    SVECTOR unk8; /* scale */
    s16 unk10;
    s16 unk12; /* length */
    s16 unk14; /* brightness */
    s16 unk16; /* width */
    s16 unk18;
} Fx0e2Ribbon1;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR rotC; /* model rotations */
    SVECTOR rot14;
    VECTOR pos1C; /* model position */
    VECTOR scale2C; /* model scale */
    s32 unk3C; /* model brightness */
    SVECTOR pos40; /* effect positions */
    SVECTOR pos48;
    SVECTOR unk50; /* caster position */
    s32 unk58; /* fade counters */
    s32 unk5C;
    s32 unk60;
    s16 unk64;
    s16 unk66;
    u16 model68; /* models */
    u16 model6A;
} Fx0e2Work82; /* 0x6C */

extern Fx0e2Ribbon D_801EE57C;
extern Fx0e2Ribbon D_801EE5AC;
extern Fx0e2Ribbon D_801EE5DC;
extern Fx0e2Ribbon1 D_801EE60C;
extern SVECTOR D_801F100C[5];
extern SVECTOR D_801F105C;
extern SVECTOR D_801F1064;
extern SVECTOR D_801F106C;
extern SVECTOR D_801F107C[5];
extern SVECTOR D_801F10AC[5];

/* propagated */

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx0e2Key; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx0e2Key *unk4;
} Fx0e2Path_0b4;
static s32 fxm_rotPosPathGet(Fx0e2Path_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);
static s32 fxm_offsetTableEntry(u8 *base, u16 i);
static s32 fxm_moveUnitHome(s32 a0, s32 t, s32 n, SVECTOR *b);

typedef struct {
    s16 *unk0;
    s16 unk4; /* start frame */
    s16 unk6; /* frames left, -1 = free */
    s16 unk8; /* channel */
    u16 unkA; /* destination slots, 4 bits each */
    u16 unkC;
    u16 unkE;
} Fx0e2Track; /* 0x10 */
extern Fx0e2Track D_801F10DC[];
extern s32 D_801F14DC[];
static void fxm_camChanReset(void);
static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data);
static void fxm_camChanUpdate(s32 ch);

#endif
