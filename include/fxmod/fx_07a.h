#ifndef FX_07A_H
#define FX_07A_H

/* Effect module fx_07a (dir 13 file 0x7A): Demi Shock (Elan). As fx_079 for all targets, plus a twisted disc over
 * a copy of the background (fx_07a_drawTwistDisc). Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_07a_env;
extern s32 fx_07a_discRadius;
extern s32 fx_07a_uvRadiusIn;
extern s32 fx_07a_uvRadiusOut;
extern s32 fx_07a_outerBandRadius;
extern s32 fx_07a_innerBandRadius;
extern s32 fx_07a_ringTwist[];

static s32 fxm_easeLerpSvec(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out);
static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);
void fx_07a_drawTwistDisc(SVECTOR *pos, s32 ot, s32 tpage, s32 clut, s32 abr);

typedef struct {
    s16 vx, vy, vz;
    s16 unk6; /* width */
} Fx07aPt;

/* lightning strip passed to btlfx_drawTrail */
typedef struct {
    u8 unk0; /* count - 1 */
    u8 unk1;
    u8 unk2;
    u8 pad3;
    Fx07aPt pts4[11];
    s32 endX5C; /* end offset */
    s32 endY60;
    s32 endZ64;
    s32 pad68;
    s32 offsetY6C[20]; /* jitter */
    s32 speedYBC[20]; /* jitter speed */
    s32 timer10C; /* timer */
} Fx07aStrip; /* 0x110 */

typedef struct {
    s32 model0; /* model */
    SVECTOR rot4; /* rotation */
    VECTOR scaleC; /* scale */
} Fx07aModel; /* 0x1C */

typedef struct {
    s32 timers0[16]; /* timers */
    Fx07aModel models40[3];
    VECTOR pos94; /* position */
    SVECTOR homePosA4[8]; /* unit home positions */
    s32 delayE4[8]; /* unit delays */
    s32 pad104;
} Fx07aWork; /* 0x108 */

extern BtlModel fx_07a_effModels[];
extern SVECTOR fx_07a_bonePos;
extern SVECTOR fx_07a_centerPos;
extern Fx07aStrip fx_07a_strips[5];
extern s32 D_801E9504;
extern u32 fx_07a_bgOt[];

#endif
