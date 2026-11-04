#ifndef FX_07A_H
#define FX_07A_H

/* Effect module fx_07a (dir 13 file 0x7A): Demi Shock (Elan). As fx_079 for all targets, plus a twisted disc over
 * a copy of the background (fx_07a_drawTwistDisc). Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_07a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEMI_SHOCK_FLASH_RING 0x6 /* emitter: white gouraud ring (same call site as fx_079 entry 0x2) */
#define FX_DEMI_SHOCK_BACKDROP   0x8 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_DEMI_SHOCK_WISP_BURST 0x9 /* emitter: white gouraud rings + blue and violet smoke wisps */
#define FX_DEMI_SHOCK_SPHERE     0xB /* model: sphere, texture still outside the file and the common pages (magenta) (byte-identical to fx_079 entry 0xB) (look pending) */
#define FX_DEMI_SHOCK_TRAIL      0xC /* trail: strip texture still outside the file and the common pages (magenta) (byte-identical to fx_079 entry 0x9) (look pending) */
#define FX_DEMI_SHOCK_BOWL       0xD /* model: gouraud hemisphere bowl (byte-identical to fx_079 entry 0xa) */
#define FX_DEMI_SHOCK_BOWL_2     0xE /* model: second gouraud hemisphere bowl (byte-identical to fx_079 entry 0x6) */

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
    s16 halfWidth6; /* half-width of the ribbon at this point (scales the edge normal) */
} Fx07aPt;

/* lightning strip passed to btlfx_drawTrail */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
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

extern BtlModelBuf fx_07a_effModels[];
extern SVECTOR fx_07a_bonePos;
extern SVECTOR fx_07a_centerPos;
extern Fx07aStrip fx_07a_strips[5];
extern s32 fx_07a_discRadiusIn; /* never read (runtime watch, round 33) */
extern u32 fx_07a_bgOt[];

#endif
