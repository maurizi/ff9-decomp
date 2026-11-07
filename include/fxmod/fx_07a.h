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
#define FX_DEMI_SHOCK_ENERGY_SHELL 0xB /* model: sphere, the swirling blue then red-violet energy shell round the black sphere (byte-identical to fx_079 entry 0xB); runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_DEMI_SHOCK_SPARK_TRAIL 0xC /* trail: 3 px wide strip, the thin pink / orange spark lines from the caster to the sphere (byte-identical to fx_079 entry 0x9); runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */
#define FX_DEMI_SHOCK_BOWL       0xD /* model: gouraud hemisphere bowl (byte-identical to fx_079 entry 0xa) */
#define FX_DEMI_SHOCK_BOWL_2     0xE /* model: second gouraud hemisphere bowl (byte-identical to fx_079 entry 0x6) */

/* fx_07a choreography (docs/fxdata.md, "Choreography literals"; fx_079's names where the slot or value means the
 * same): op80's keyframes and countdowns, the sphere over the targets, the twisted disc's rings and radii. */
#define FX_DEMI_SHOCK_TIMERS     16 /* work->timers0: per-phase countdowns / flags, set on keyframes */
#define FX_DEMI_SHOCK_T_STRANDS  0  /* countdown: the spark strands from the caster's bone to the sphere are drawn (and respawned) */
#define FX_DEMI_SHOCK_T_SPHERE   1  /* flag: the shell and bowls are drawn, facing the camera */
#define FX_DEMI_SHOCK_T_FLY      2  /* countdown: the sphere flies from the caster's bone to its place over the targets */
#define FX_DEMI_SHOCK_T_GROW     4  /* countdown: the shell and bowls grow */
#define FX_DEMI_SHOCK_T_SHRINK   5  /* countdown: they shrink to nothing */
#define FX_DEMI_SHOCK_T_LIFT     6  /* countdown: the targets are lifted, each after its own delay */
#define FX_DEMI_SHOCK_T_DROP     7  /* countdown: they are put back */
#define FX_DEMI_SHOCK_T_BACKDROP 9  /* counter: FX_DEMI_SHOCK_BACKDROP's layer frame */
#define FX_DEMI_SHOCK_T_DISC     10 /* flag: the twisted disc (fx_07a_drawTwistDisc) is drawn under the targets */
#define FX_DEMI_SHOCK_T_DISC_OPEN 11 /* countdown: the disc and its bands open out */
#define FX_DEMI_SHOCK_T_TWIST    12 /* countdown: the disc's rings twist and its texture radius grows */
#define FX_DEMI_SHOCK_T_DISC_CLOSE 13 /* countdown: the disc and its bands close */
#define FX_DEMI_SHOCK_START_FRAME  1   /* after the background capture (frame 0): sphere, strands, backdrop start */
#define FX_DEMI_SHOCK_DISC_FRAME   8   /* the disc opens and twists, the targets are lifted */
#define FX_DEMI_SHOCK_FLASH_RING_FRAME 12 /* FX_DEMI_SHOCK_FLASH_RING starts at the centre */
#define FX_DEMI_SHOCK_SHRINK_FRAME 24  /* the sphere shrinks, the disc closes */
#define FX_DEMI_SHOCK_WISP_FRAME   36  /* FX_DEMI_SHOCK_WISP_BURST starts, the targets are put back */
#define FX_DEMI_SHOCK_FLASH_FRAME  40  /* white screen flash, the disc and the sphere are no longer drawn */
#define FX_DEMI_SHOCK_FRAMES       0x46 /* op80's length (the op ends) */
#define FX_DEMI_SHOCK_STRANDS_FRAMES 0x24 /* FX_DEMI_SHOCK_T_STRANDS' length */
#define FX_DEMI_SHOCK_STRAND_RESPAWN_FRAMES 5 /* strands are respawned only while T_STRANDS has this many frames left */
#define FX_DEMI_SHOCK_STRAND_FADE_FRAMES 4 /* a strand fades out over its last this many frames */
#define FX_DEMI_SHOCK_FLY_FRAMES   8    /* FX_DEMI_SHOCK_T_FLY's length (and its lerp's n) */
#define FX_DEMI_SHOCK_GROW_FRAMES  0x10 /* FX_DEMI_SHOCK_T_GROW / _SHRINK's length (and the lerps' n) */
#define FX_DEMI_SHOCK_LIFT_FRAMES  0x1C /* FX_DEMI_SHOCK_T_LIFT's length; a target's lift lerp runs this minus its delay */
#define FX_DEMI_SHOCK_DROP_FRAMES  4    /* FX_DEMI_SHOCK_T_DROP's length (and its lerp's n) */
#define FX_DEMI_SHOCK_DISC_OPEN_FRAMES 8 /* FX_DEMI_SHOCK_T_DISC_OPEN's length (and the lerps' n) */
#define FX_DEMI_SHOCK_TWIST_FRAMES 0x20 /* FX_DEMI_SHOCK_T_TWIST's length (and the lerps' n) */
#define FX_DEMI_SHOCK_DISC_CLOSE_FRAMES 0x10 /* FX_DEMI_SHOCK_T_DISC_CLOSE's length (and the lerps' n) */
#define FX_DEMI_SHOCK_LIFT_HEIGHT  0x200 /* the targets are lifted this high */
#define FX_DEMI_SHOCK_SPHERE_HEIGHT 0x500 /* the sphere (fx_07a_centerPos) hangs this high over the targets' centre */
#define FX_DEMI_SHOCK_SHELL_SCALE_0 0x5C  /* the models' start and full scales: FX_DEMI_SHOCK_ENERGY_SHELL */
#define FX_DEMI_SHOCK_SHELL_SCALE   0x2A2
#define FX_DEMI_SHOCK_BOWL_SCALE_0  0x50  /* FX_DEMI_SHOCK_BOWL */
#define FX_DEMI_SHOCK_BOWL_SCALE    0x243
#define FX_DEMI_SHOCK_BOWL_2_SCALE_0 0x66 /* FX_DEMI_SHOCK_BOWL_2 */
#define FX_DEMI_SHOCK_BOWL_2_SCALE  0x299
#define FX_DEMI_SHOCK_STRAND_REACH 0x80  /* a strand ends this far outside the shell's scale */
#define FX_DEMI_SHOCK_MODEL_COUNT  3     /* btlfx_initEffModels slots: ENERGY_SHELL, BOWL, BOWL_2 */
#define FX_DEMI_SHOCK_STRANDS      5     /* spark strands (fx_07a_strips), started one frame apart */
#define FX_DEMI_SHOCK_STRAND_POINTS 11   /* points per strand (Fx07aStrip.pts4) */
#define FX_DEMI_SHOCK_SHELL_TPAGE  0x137  /* 15-bit page (0x1C0, 0x100), abr 1 */
#define FX_DEMI_SHOCK_SHELL_CLUT   0xFFFF /* none (15-bit page) */
#define FX_DEMI_SHOCK_FLASH_RGB    0xFF, 0xFF, 0xFF /* opaque white screen tint on FX_DEMI_SHOCK_FLASH_FRAME */
/* Frame 0 renders the background round the targets into this rect (btlfx_drawBgOffscreen); the disc samples it
 * through the 15-bit page DISC_TPAGE (0x1C0, 0x100), its texture centred on (DISC_U, DISC_V). */
#define FX_DEMI_SHOCK_BG_VRAM_X    0x1C0
#define FX_DEMI_SHOCK_BG_VRAM_Y    0x17E
#define FX_DEMI_SHOCK_BG_VRAM_W    0x70
#define FX_DEMI_SHOCK_BG_VRAM_H    0x70
#define FX_DEMI_SHOCK_DISC_TPAGE   0x117  /* 15-bit page (0x1C0, 0x100) */
#define FX_DEMI_SHOCK_DISC_U       0x38   /* texture centre u (BG_VRAM_W / 2) */
#define FX_DEMI_SHOCK_DISC_V       (-0x48) /* texture centre v (0xB8 as a byte) */
#define FX_DEMI_SHOCK_DISC_RADIUS  0x5A00 /* fx_07a_discRadius when open (x 1/16 units) */
#define FX_DEMI_SHOCK_OUTER_BAND_RADIUS 0x4A00 /* fx_07a_outerBandRadius: the white-to-clear band outside the disc */
#define FX_DEMI_SHOCK_INNER_BAND_RADIUS 0xB20  /* fx_07a_innerBandRadius: the clear-to-white band inside it (>> 12 units) */
#define FX_DEMI_SHOCK_UV_RADIUS    0x340  /* fx_07a_uvRadiusOut / _In when open: the sampled texture's radius */
#define FX_DEMI_SHOCK_DISC_RINGS   4      /* textured rings from centre to rim (fx_07a_ringTwist[0..4] at their edges) */
#define FX_DEMI_SHOCK_DISC_SEGMENTS 16    /* angular segments (0x100 each) of the disc and of its two bands */
#define FX_DEMI_SHOCK_DISC_Z       0x20   /* the disc is drawn this far behind the centre */

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
