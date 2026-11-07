#ifndef FX_062_H
#define FX_062_H

/* Effect module fx_062 (dir 13 file 0x62). */
#include "btlfx_mod.h"

/* fx_062 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEMI_BACKDROP   0x7 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_DEMI_SPHERE     0x8 /* model: sphere, white texture */
#define FX_DEMI_BACKDROP_2 0x9 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_DEMI_CAP        0xA /* model: shallow spherical cap, texture still outside the file and the common pages (magenta); runtime (docs/runtime-findings.md effect-data pass): the dome reads white with magenta noise with the run-time page; ablating it leaves the black Demi sphere unchanged, so it is not the sphere (look pending) */
#define FX_DEMI_BOWL       0xB /* model: gouraud hemisphere bowl */
#define FX_DEMI_BOWL_2     0xC /* model: second gouraud hemisphere bowl */
#define FX_DEMI_GLOW_TRAIL 0xD /* trail: white radial glow strip */

/* fx_062 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and phase timers, the models'
 * sizes and textures, the glow strands. */
#define FX_DEMI_TIMERS       16 /* work->timers0: per-phase countdowns / flags, set on keyframes */
#define FX_DEMI_T_STRANDS    0  /* countdown: the glow strands are drawn (and respawned) */
#define FX_DEMI_T_SPHERE     1  /* flag: the sphere and bowls are drawn facing the camera */
#define FX_DEMI_T_GROW       2  /* countdown: the sphere and bowls grow */
#define FX_DEMI_T_SHRINK     3  /* countdown: they shrink */
#define FX_DEMI_T_CAP        4  /* flag: FX_DEMI_CAP is drawn on the ground */
#define FX_DEMI_T_CAP_RISE   5  /* countdown: the cap rises (scale y) */
#define FX_DEMI_T_CAP_SINK   6  /* countdown: it sinks */
#define FX_DEMI_T_LIFT       7  /* countdown: the target is lifted (and shrunk if tall) */
#define FX_DEMI_T_DROP       8  /* countdown: it is put back */
#define FX_DEMI_T_HOVER      9  /* countdown: it hovers, bobbing */
#define FX_DEMI_SPHERE_FRAME   1    /* the sphere and strands start, grow (FX_DEMI_GROW_FRAMES), the target is lifted */
#define FX_DEMI_CAP_FRAME      9    /* the cap appears and rises */
#define FX_DEMI_HOVER_FRAME    0x11 /* the target hovers */
#define FX_DEMI_SHRINK_FRAME   0x2A /* the sphere shrinks, the target is put back, the cap sinks */
#define FX_DEMI_END_DRAW_FRAME 0x3A /* the sphere and cap are no longer drawn */
#define FX_DEMI_FRAMES         0x46 /* op80's length (the op ends) */
#define FX_DEMI_GROW_FRAMES    0x10 /* FX_DEMI_T_GROW / _SHRINK / _LIFT / _DROP: their length (and the eased lerps' n) */
#define FX_DEMI_STRANDS_FRAMES 0x30 /* FX_DEMI_T_STRANDS' length */
#define FX_DEMI_CAP_RISE_FRAMES 8   /* FX_DEMI_T_CAP_RISE / _CAP_SINK's length */
#define FX_DEMI_HOVER_FRAMES   0x19 /* FX_DEMI_T_HOVER's length */
#define FX_DEMI_LIFT_HEIGHT    0x200 /* the target is lifted this high */
#define FX_DEMI_SHRINK_HEIGHT  0x200 /* targets taller than this are also shrunk to half size while lifted */
#define FX_DEMI_SPHERE_SCALE   0x170 /* the models' full scales: FX_DEMI_SPHERE */
#define FX_DEMI_BOWL_SCALE     0x143 /* FX_DEMI_BOWL */
#define FX_DEMI_BOWL_2_SCALE   0x199 /* FX_DEMI_BOWL_2 */
#define FX_DEMI_CAP_WIDTH      0x249 /* FX_DEMI_CAP's scale x / z */
#define FX_DEMI_MODEL_COUNT    4     /* btlfx_initEffModels slots: SPHERE, BOWL, BOWL_2, CAP */
#define FX_DEMI_STRANDS        5     /* glow strands (fx_062_strips) */
#define FX_DEMI_STRAND_POINTS  11    /* points per strand (Fx062Strip.pts4): the two ends and the curve between */
#define FX_DEMI_SPHERE_TPAGE   0x137  /* 15-bit page (0x1C0, 0x100), abr 1 */
#define FX_DEMI_SPHERE_CLUT    0xFFFF /* none (15-bit page) */
#define FX_DEMI_SPHERE_V       0x80
#define FX_DEMI_CAP_TPAGE      0x17A  /* 15-bit page (0x280, 0x100), abr 3 */
#define FX_DEMI_CAP_CLUT       0xFFFF /* none (15-bit page) */
#define FX_DEMI_CAP_V          0x80
/* frame 0: btlfx_drawBgOffscreen renders the battle background round the target into this rect of FX_DEMI_CAP's page */
#define FX_DEMI_BG_VRAM_X 0x280
#define FX_DEMI_BG_VRAM_Y 0x100
#define FX_DEMI_BG_VRAM_W 0x70
#define FX_DEMI_BG_VRAM_H 0x70

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    u16 halfWidth6; /* half-width of the ribbon at this point (scales the edge normal) */
} Fx062Pt;

/* line strip passed to btlfx_drawTrail: a quadratic curve from pos5C over the target to pos64 */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    Fx062Pt pts4[FX_DEMI_STRAND_POINTS]; /* ring points (btlfx_pushTrailPoint) */
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
    s32 width114; /* ribbon half-width, copied into each point's halfWidth6 */
    s32 timer118; /* timer */
} Fx062Strip; /* 0x11C */

typedef struct {
    s32 model0; /* model */
    SVECTOR rot4; /* rotation */
    VECTOR scaleC; /* scale */
} Fx062Model; /* 0x1C */

typedef struct {
    s32 timers0[FX_DEMI_TIMERS]; /* timers */
    Fx062Model models40[3];
    VECTOR pos94; /* position */
    s32 modelA4; /* model */
    SVECTOR rotA8; /* rotation */
    VECTOR posB0; /* position */
    VECTOR scaleC0; /* scale */
    s32 unusedD0; /* allocated by op80, never used */
} Fx062Work; /* 0xD4 */

extern Unk800F78A4 *fx_062_env;
extern BtlModelBuf fx_062_effModels[];
extern u32 fx_062_bgOt[];
extern SVECTOR fx_062_targetBodyPos;
extern SVECTOR fx_062_targetHome;
extern VECTOR fx_062_targetScale;
extern s32 fx_062_targetHeight;
extern Fx062Strip fx_062_strips[FX_DEMI_STRANDS];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
