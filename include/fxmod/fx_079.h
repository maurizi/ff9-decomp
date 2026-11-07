#ifndef FX_079_H
#define FX_079_H

/* Effect module fx_079 (dir 13 file 0x79): Demi Shock. Three models over the target, a lift of the target and 5
 * jagged btlfx_drawTrail strips from a bone of unit 0x10. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

/* fx_079 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEMI_SHOCK_WISP_BURST 0x1 /* emitter: white gouraud rings + pink-violet smoke wisps */
#define FX_DEMI_SHOCK_FLASH_RING 0x2 /* emitter: white gouraud ring */
#define FX_DEMI_SHOCK_BOWL_2     0x6 /* model: second gouraud hemisphere bowl */
#define FX_DEMI_SHOCK_SPARK_TRAIL 0x9 /* trail: 3 px wide strip, the thin pink / orange spark lines from the caster to the sphere (the only trail-like element on screen); runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */
#define FX_DEMI_SHOCK_BOWL       0xA /* model: gouraud hemisphere bowl */
#define FX_DEMI_SHOCK_ENERGY_SHELL 0xB /* model: sphere, the swirling blue then red-violet energy shell round the black sphere; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_DEMI_SHOCK_BACKDROP   0xC /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */

/* fx_079 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and phase timers, the models'
 * sizes and texture, the spark strands (Demi's scheme, fx_062). */
#define FX_DEMI_SHOCK_TIMERS     16 /* work->timers0: per-phase countdowns / flags, set on keyframes */
#define FX_DEMI_SHOCK_T_STRANDS  0  /* countdown: the spark strands from the unit's bone to the sphere are drawn (and respawned) */
#define FX_DEMI_SHOCK_T_SPHERE   1  /* flag: the shell and bowls are drawn round the target, facing the camera */
#define FX_DEMI_SHOCK_T_RAISE    2  /* countdown: the sphere rises (never set) */
#define FX_DEMI_SHOCK_T_GROW     4  /* countdown: the shell and bowls grow */
#define FX_DEMI_SHOCK_T_SHRINK   5  /* countdown: they shrink to nothing */
#define FX_DEMI_SHOCK_T_LIFT     6  /* countdown: the target is lifted (and shrunk if tall) */
#define FX_DEMI_SHOCK_T_DROP     7  /* countdown: it is put back */
#define FX_DEMI_SHOCK_T_HOLD     8  /* countdown: it is held up */
#define FX_DEMI_SHOCK_T_BACKDROP 9  /* counter: FX_DEMI_SHOCK_BACKDROP's layer frame */
#define FX_DEMI_SHOCK_LIFT_FRAME   8   /* the target is lifted */
#define FX_DEMI_SHOCK_FLASH_RING_FRAME 16 /* FX_DEMI_SHOCK_FLASH_RING starts on the target */
#define FX_DEMI_SHOCK_HOLD_FRAME   24  /* the target is held up */
#define FX_DEMI_SHOCK_SHRINK_FRAME 28  /* the sphere shrinks, the target is put back */
#define FX_DEMI_SHOCK_WISP_FRAME   40  /* FX_DEMI_SHOCK_WISP_BURST starts */
#define FX_DEMI_SHOCK_FLASH_FRAME  44  /* white screen flash, the sphere is no longer drawn */
#define FX_DEMI_SHOCK_FRAMES       0x5A /* op80's length (the op ends) */
#define FX_DEMI_SHOCK_STRANDS_FRAMES 0x24 /* FX_DEMI_SHOCK_T_STRANDS' length */
#define FX_DEMI_SHOCK_GROW_FRAMES  0x10 /* FX_DEMI_SHOCK_T_GROW / _SHRINK / _LIFT / _DROP's length (and the lerps' n) */
#define FX_DEMI_SHOCK_RAISE_FRAMES 8    /* FX_DEMI_SHOCK_T_RAISE's lerp n */
#define FX_DEMI_SHOCK_HOLD_FRAMES  4    /* FX_DEMI_SHOCK_T_HOLD's length */
#define FX_DEMI_SHOCK_STRAND_FADE_FRAMES 4 /* a strand fades out over its last this many frames */
#define FX_DEMI_SHOCK_LIFT_HEIGHT  0x200 /* the target (and the unused raise) is lifted this high */
#define FX_DEMI_SHOCK_SHRINK_HEIGHT 0x200 /* targets taller than this are also shrunk to half size while lifted */
#define FX_DEMI_SHOCK_SHELL_SCALE_0 0x5C  /* the models' start and full scales: FX_DEMI_SHOCK_ENERGY_SHELL */
#define FX_DEMI_SHOCK_SHELL_SCALE   0x18A
#define FX_DEMI_SHOCK_BOWL_SCALE_0  0x50  /* FX_DEMI_SHOCK_BOWL */
#define FX_DEMI_SHOCK_BOWL_SCALE    0x143
#define FX_DEMI_SHOCK_BOWL_2_SCALE_0 0x66 /* FX_DEMI_SHOCK_BOWL_2 */
#define FX_DEMI_SHOCK_BOWL_2_SCALE  0x199
#define FX_DEMI_SHOCK_MODEL_COUNT  3     /* btlfx_initEffModels slots: ENERGY_SHELL, BOWL, BOWL_2 */
#define FX_DEMI_SHOCK_STRANDS      5     /* spark strands (fx_079_strips), started one frame apart */
#define FX_DEMI_SHOCK_STRAND_POINTS 11   /* points per strand (Fx079Strip.pts4, offsetY6C, speedYC4) */
#define FX_DEMI_SHOCK_SHELL_TPAGE  0x137  /* 15-bit page (0x1C0, 0x100), abr 1 */
#define FX_DEMI_SHOCK_SHELL_CLUT   0xFFFF /* none (15-bit page) */
#define FX_DEMI_SHOCK_FLASH_RGB    0xFF, 0xFF, 0xFF /* opaque white screen tint on FX_DEMI_SHOCK_FLASH_FRAME */

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    u16 halfWidth6; /* half-width of the ribbon at this point (scales the edge normal) */
} Fx079Pt;

/* line strip passed to btlfx_drawTrail */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    Fx079Pt pts4[FX_DEMI_SHOCK_STRAND_POINTS];
    s32 endX5C; /* end offset */
    s32 endY60;
    s32 endZ64;
    s32 pad68;
    s32 offsetY6C[FX_DEMI_SHOCK_STRAND_POINTS]; /* y offset */
    s32 unk98[FX_DEMI_SHOCK_STRAND_POINTS]; /* unused */
    s32 speedYC4[FX_DEMI_SHOCK_STRAND_POINTS]; /* y speed */
    s32 unkF0[FX_DEMI_SHOCK_STRAND_POINTS]; /* unused */
    s32 timer11C; /* timer */
} Fx079Strip; /* 0x120 */

typedef struct {
    s32 model0; /* model */
    SVECTOR rot4; /* rotation */
    VECTOR scaleC; /* scale */
} Fx079Model; /* 0x1C */

typedef struct {
    s32 timers0[FX_DEMI_SHOCK_TIMERS]; /* timers */
    Fx079Model models40[3];
    VECTOR pos94; /* position */
    s32 unusedA4; /* allocated by op80, never used */
} Fx079Work; /* 0xA8 */

extern Unk800F78A4 *fx_079_env;
extern BtlModelBuf fx_079_effModels[];
extern SVECTOR fx_079_bonePos;
extern SVECTOR fx_079_bodyPos;
extern SVECTOR fx_079_homePos;
extern VECTOR fx_079_unitScale;
extern SVECTOR fx_079_curBodyPos;
extern s32 fx_079_unitHeight;
extern Pool6CEntry *fx_079_emitter;
extern SVECTOR fx_079_emitStartPos;
extern SVECTOR fx_079_emitPos;
extern Fx079Strip fx_079_strips[FX_DEMI_SHOCK_STRANDS];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
