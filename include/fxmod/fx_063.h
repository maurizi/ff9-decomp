#ifndef FX_063_H
#define FX_063_H

/* Effect module fx_063 (dir 13 file 0x63). */
#include "btlfx_mod.h"

/* fx_063 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEMI_BACKDROP   0x7 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) (byte-identical to fx_062 entry 0x7) */
#define FX_DEMI_SPHERE     0x8 /* model: sphere, white texture (byte-identical to fx_062 entry 0x8) */
#define FX_DEMI_CAP        0x9 /* model: shallow spherical cap, texture still outside the file and the common pages (magenta) (byte-identical to fx_062 entry 0xa); runtime (docs/runtime-findings.md effect-data pass): not played; fx_062's same entry is not the black Demi sphere (ablation) (look pending) */
#define FX_DEMI_BACKDROP_2 0xA /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) (byte-identical to fx_062 entry 0x9) */
#define FX_DEMI_BOWL       0xB /* model: gouraud hemisphere bowl (byte-identical to fx_062 entry 0xB) */
#define FX_DEMI_BOWL_2     0xC /* model: second gouraud hemisphere bowl (byte-identical to fx_062 entry 0xC) */
#define FX_DEMI_GLOW_TRAIL 0xD /* trail: white radial glow strip (byte-identical to fx_062 entry 0xD) */

/* fx_063 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and phase timers, the models'
 * sizes and textures, the glow strands and the targets' bobbing (fx_062's names, all-targets values). */
#define FX_DEMI_TIMERS       16 /* work->timers0: per-phase countdowns / flags, set on keyframes */
#define FX_DEMI_T_STRANDS    0  /* countdown: the glow strands are drawn (and respawned) */
#define FX_DEMI_T_SPHERE     1  /* flag: the sphere and bowls are drawn facing the camera */
#define FX_DEMI_T_GROW       2  /* countdown: the sphere and bowls grow */
#define FX_DEMI_T_SHRINK     3  /* countdown: they shrink */
#define FX_DEMI_T_CAP        4  /* flag: FX_DEMI_CAP is drawn on the ground */
#define FX_DEMI_T_CAP_RISE   5  /* countdown: the cap rises (scale y) */
#define FX_DEMI_T_CAP_SINK   6  /* countdown: it sinks */
#define FX_DEMI_T_BOB        7  /* flag: the targets bob (fx_063_unitBob) */
#define FX_DEMI_T_BACKDROP   9  /* counter + 1: FX_DEMI_BACKDROP's layer frame */
#define FX_DEMI_T_BACKDROP_2 10 /* counter + 1: FX_DEMI_BACKDROP_2's layer frame */
#define FX_DEMI_SPHERE_FRAME   1    /* the sphere, cap, strands and bobbing start; the sphere grows, the cap rises */
#define FX_DEMI_SHRINK_FRAME   42   /* the sphere shrinks (and darkens), the cap sinks */
#define FX_DEMI_END_DRAW_FRAME 49   /* the sphere is no longer drawn */
#define FX_DEMI_CAP_END_FRAME  51   /* the cap is no longer drawn */
#define FX_DEMI_FRAMES         0x46 /* op80's length (the op ends) */
#define FX_DEMI_GROW_FRAMES    0x10 /* FX_DEMI_T_GROW / _CAP_RISE's length (and the eased lerps' n) */
#define FX_DEMI_SHRINK_FRAMES  8    /* FX_DEMI_T_SHRINK / _CAP_SINK's length */
#define FX_DEMI_STRANDS_FRAMES 0x30 /* FX_DEMI_T_STRANDS' length */
#define FX_DEMI_STRAND_FADE_FRAMES 4 /* a strand fades out over its last this many frames */
#define FX_DEMI_BOB_PHASE_FRAMES 0x10 /* each target's bob: rise, hover and drop of this many frames each (timer0 counts down from 3 of them + random) */
#define FX_DEMI_LIFT_HEIGHT    0x80  /* the targets are lifted this high */
#define FX_DEMI_SPHERE_SCALE   0x400 /* the models' full scales: FX_DEMI_SPHERE */
#define FX_DEMI_BOWL_SCALE     0x300 /* FX_DEMI_BOWL */
#define FX_DEMI_BOWL_2_SCALE   0x480 /* FX_DEMI_BOWL_2 (x / z) */
#define FX_DEMI_SHRUNK_SCALE   0x200 /* all three shrink to this */
#define FX_DEMI_CAP_WIDTH      0x400 /* FX_DEMI_CAP's scale x / z */
#define FX_DEMI_CENTRE_Z       0x30C  /* the sphere's centre (and camera point): x 0, this z on the targets' side */
#define FX_DEMI_CENTRE_Y       (-0x600) /* at this height (y, up is negative) */
#define FX_DEMI_MODEL_COUNT    4     /* btlfx_initEffModels slots: SPHERE, BOWL, BOWL_2, CAP */
#define FX_DEMI_STRANDS        10    /* glow strands (fx_063_strips) */
#define FX_DEMI_STRAND_POINTS  11    /* points per strand (Fx063Strip.pts4): the two ends and the curve between */
#define FX_DEMI_SPHERE_TPAGE   0x137  /* 15-bit page (0x1C0, 0x100), abr 1 */
#define FX_DEMI_SPHERE_CLUT    0xFFFF /* none (15-bit page) */
#define FX_DEMI_SPHERE_V       0x80
#define FX_DEMI_CAP_TPAGE      0x17A  /* 15-bit page (0x280, 0x100), abr 3 */
#define FX_DEMI_CAP_CLUT       0xFFFF /* none (15-bit page) */
#define FX_DEMI_CAP_V          0x80
/* frame 0: btlfx_drawBgOffscreen renders the battle background round the centre into this rect of FX_DEMI_CAP's page */
#define FX_DEMI_BG_VRAM_X 0x280
#define FX_DEMI_BG_VRAM_Y 0x100
#define FX_DEMI_BG_VRAM_W 0x70
#define FX_DEMI_BG_VRAM_H 0x70

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
    Fx063Pt pts4[FX_DEMI_STRAND_POINTS]; /* ring points (btlfx_pushTrailPoint) */
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
    s32 timers0[FX_DEMI_TIMERS]; /* timers */
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
extern Fx063Strip fx_063_strips[FX_DEMI_STRANDS];
extern Fx063Unit fx_063_unitBob[];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
