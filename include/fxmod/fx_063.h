#ifndef FX_063_H
#define FX_063_H

/* Effect module fx_063 (dir 13 file 0x63). */
#include "btlfx_mod.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    u16 halfWidth6; /* half-width of the ribbon at this point (scales the edge normal) */
} Fx063Pt;

/* line strip passed to btlfx_drawTrail: a quadratic curve from the head back to the start point */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    Fx063Pt unk4[11];
    SVECTOR jitter5C[9]; /* jitter */
    s32 jitterVelA4[9]; /* jitter speed x */
    s32 jitterVelC8[9]; /* jitter speed z */
    s32 angleEC; /* angle */
    s32 angleF0; /* angle */
    s32 spinF4; /* angle speed */
    s32 spinF8; /* angle speed */
    s32 widthFC; /* ribbon half-width, copied into each point's halfWidth6 */
    s32 timer100; /* timer */
} Fx063Strip; /* 0x104 */

/* per unit: bobbing */
typedef struct {
    s32 timer0; /* timer */
    s32 phase4; /* phase */
    s32 phaseSpeed8; /* phase speed */
    SVECTOR homeC; /* home position */
    SVECTOR pos14; /* position */
} Fx063Unit; /* 0x1C */

typedef struct {
    s32 model0; /* model */
    SVECTOR rot4; /* rotation */
    VECTOR scaleC; /* scale */
} Fx063Model; /* 0x1C */

typedef struct {
    s32 timers0[16]; /* timers */
    Fx063Model models40[3];
    VECTOR pos94; /* position */
    SVECTOR posA4; /* centre the models face the camera from */
    s32 modelAC; /* model */
    SVECTOR rotB0; /* rotation */
    VECTOR posB8; /* position */
    VECTOR scaleC8; /* scale */
    s32 padD8;
} Fx063Work; /* 0xDC */

extern Unk800F78A4 *fx_063_env;
extern BtlModelBuf fx_063_effModels[];
extern u32 fx_063_bgOt[];
extern Fx063Strip fx_063_strips[10];
extern Fx063Unit fx_063_unitBob[];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
