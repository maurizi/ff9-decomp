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

/* line strip passed to btlfx_drawTrail: a quadratic curve from pos5C over the target to pos64 */
typedef struct {
    u8 unk0; /* count - 1 */
    u8 unk1;
    u8 unk2;
    u8 pad3;
    Fx062Pt unk4[11];
    SVECTOR pos5C; /* start */
    SVECTOR pos64; /* end */
    SVECTOR jitter6C[9]; /* jitter */
    s32 spinB4; /* angle speed */
    s32 spinB8; /* angle speed */
    s32 jitterVelBC[9]; /* jitter speed x */
    s32 jitterVelE0[9]; /* jitter speed z */
    s32 angle104; /* angle */
    s32 angle108; /* angle */
    s32 x10C; /* x */
    s32 z110; /* z */
    s32 unk114; /* colour */
    s32 timer118; /* timer */
} Fx062Strip; /* 0x11C */

typedef struct {
    s32 model0; /* model */
    SVECTOR rot4; /* rotation */
    VECTOR scaleC; /* scale */
} Fx062Model; /* 0x1C */

typedef struct {
    s32 timers0[16]; /* timers */
    Fx062Model models40[3];
    VECTOR pos94; /* position */
    s32 modelA4; /* model */
    SVECTOR rotA8; /* rotation */
    VECTOR posB0; /* position */
    VECTOR scaleC0; /* scale */
} Fx062Work; /* 0xD0 (0xD4 allocated) */

extern Unk800F78A4 *fx_062_env;
extern BtlModel fx_062_effModels[];
extern u32 fx_062_bgOt[];
extern SVECTOR fx_062_targetBodyPos;
extern SVECTOR fx_062_targetHome;
extern VECTOR fx_062_targetScale;
extern s32 fx_062_targetHeight;
extern Fx062Strip fx_062_strips[5];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
