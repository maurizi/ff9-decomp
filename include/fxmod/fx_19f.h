#ifndef FX_19F_H
#define FX_19F_H

/* Effect module fx_19f (dir 13 file 0x19f). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[1];
} Fx19fTrail;

extern Unk800F78A4 *fx_19f_env;

typedef struct {
    s16 x, y, z;
} Fx19fKey;

typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    Fx19fKey unk4[1];
} Fx19fKeys;
typedef struct {
    SVECTOR unk0[17]; /* points of a 17-point trail (its 4-byte header precedes them) */
    u8 pad88[4];
} Fx19fTrailPts; /* 0x8C */

extern Fx19fTrailPts D_801EB3BC[32];

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[17];
} Fx19fTrail17; /* 0x8C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[4];
} Fx19fTrail4; /* 0x24 */

typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    u8 unk10; /* trail index */
    u8 unk11; /* kind */
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 *unk18; /* noise */
} Fx19fBolt;

extern Fx19fTrail17 D_801EB3B8[];
extern Fx19fTrail4 D_801EC538[];
extern SVECTOR D_801EA9AC;
extern s32 D_801EA9B4;
extern s32 D_801EC9B8[64][3]; /* noise */

typedef struct {
    s32 *unk0; /* noise */
    s32 unk4; /* spawn count */
    s32 unk8; /* kind */
} Fx19fWork83;

typedef struct {
    s32 unk0; /* frame */
    u8 pad4[0xC];
    s16 *unk10; /* pool */
} Fx19fInst;

static s32 fx_19f_op83PoolTask(s32 mode, Fx19fBolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

static void fxm_drawJaggedTrail(Unk80100C78Entry *entry, s32 file, s32 ot, Fx19fTrail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip);
static void fxm_drawJaggedTrailBranch(Unk80100C78Entry *entry, s32 file, s32 ot, Fx19fTrail *path, Fx19fTrail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad);

/* word i of an SVECTOR, for copies done as two words */
#define FX19F_W(v, i) (((s32 *)&(v))[i])

typedef struct {
    s16 x, y, z, rx, ry, rz;
} Fx19fKey12; /* 0xC */

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR pos8; /* base position */
    SVECTOR pos10;
    SVECTOR unk18;
    u8 pad20[0x28];
    s32 unk48[3]; /* model position */
    s32 unk54[3]; /* model scale */
    SVECTOR rot60; /* model rotation */
    Pool6CEntry *emit68;
    Pool6CEntry *emit6C;
    s32 model70; /* models */
    s32 model74;
    s32 model78;
    s32 model7C;
    s32 model80;
} Fx19fWork81; /* 0x84 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[13];
} Fx19fTrail13; /* 0x6C */

extern s16 D_801EA9B8[];
extern s32 D_801EA9D0[3];
extern s32 D_801EA9DC[3];
extern s32 D_801EA9E8[3];
extern s32 D_801EA9F4[3];
extern Fx19fTrail D_801EAA58;
extern BtlModel fx_19f_effModels[];
extern Fx19fTrail17 D_801EAF18[6];
extern Fx19fTrail13 D_801EB268[3];
static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx19fTrail *trail, SVECTOR *from, SVECTOR *to, s32 amp, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9);

/* copied */
static void fxm_drawJaggedTrail(Unk80100C78Entry *entry, s32 file, s32 ot, Fx19fTrail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip);
/* end copied */

/* propagated */
static void fxm_drawCenterBand(u32 *ot, s32 tpage, s32 w, s32 r, s32 g, s32 b);

#endif
