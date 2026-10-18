#ifndef FX_062_H
#define FX_062_H

/* Effect module fx_062 (dir 13 file 0x62). */
#include "btlfx_mod.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    u16 unk6; /* colour */
} Fx062Pt;

/* line strip passed to btlfx_drawTrail: a quadratic curve from unk5C over unk64 to the target */
typedef struct {
    u8 unk0; /* count - 1 */
    u8 unk1;
    u8 unk2;
    u8 pad3;
    Fx062Pt unk4[11];
    SVECTOR unk5C; /* start */
    SVECTOR unk64; /* end */
    SVECTOR unk6C[9]; /* jitter */
    s32 unkB4; /* angle speed */
    s32 unkB8; /* angle speed */
    s32 unkBC[9]; /* jitter speed x */
    s32 unkE0[9]; /* jitter speed z */
    s32 unk104; /* angle */
    s32 unk108; /* angle */
    s32 unk10C; /* x */
    s32 unk110; /* z */
    s32 unk114; /* colour */
    s32 unk118; /* timer */
} Fx062Strip; /* 0x11C */

typedef struct {
    s32 unk0; /* model */
    SVECTOR unk4; /* rotation */
    VECTOR unkC; /* scale */
} Fx062Model; /* 0x1C */

typedef struct {
    s32 unk0[16]; /* timers */
    Fx062Model unk40[3];
    VECTOR unk94; /* position */
    s32 unkA4; /* model */
    SVECTOR unkA8; /* rotation */
    VECTOR unkB0; /* position */
    VECTOR unkC0; /* scale */
} Fx062Work; /* 0xD0 (0xD4 allocated) */

extern Unk800F78A4 *D_801E882C;
extern BtlModel D_801E883C[];
extern u32 D_801E8B5C[];
extern SVECTOR D_801E8F5C;
extern SVECTOR D_801E8F64;
extern VECTOR D_801E8F6C;
extern s32 D_801E8F78;
extern Fx062Strip D_801E8F7C[5];

s32 func_801E77F0(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
