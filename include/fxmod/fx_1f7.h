#ifndef FX_1F7_H
#define FX_1F7_H

/* Effect module fx_1f7 (dir 13 file 0x1f7): enemy Blue Shockwave (Dummy/Necron). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1f7 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BLUE_SHOCKWAVE_FLASH      0x1 /* emitter: violet glow flash, white-red-blue and pink gouraud rings */
#define FX_BLUE_SHOCKWAVE_LIGHTNING  0x2 /* emitter: violet lightning fans + spark sprites */
#define FX_BLUE_SHOCKWAVE_GLOW_TRAIL 0x3 /* trail: violet glow strip */
#define FX_BLUE_SHOCKWAVE_GLOW       0x5 /* particles: violet glow, 7 frames */

/* fx_1f7 choreography (docs/fxdata.md, "Choreography literals"): op80's glow trails along the caster's bone chains,
 * the glow sparks, the flash and the blue screen pulse. */
#define FX_BLUE_SHOCKWAVE_TRAILS        6    /* GLOW_TRAIL strips (fx_1f7_trails), one per bone chain (fx_1f7_trailBones) */
#define FX_BLUE_SHOCKWAVE_TRAIL_LINKS   16   /*   bone pairs per chain (17 bones), */
#define FX_BLUE_SHOCKWAVE_LINK_STEPS    4    /*   each lerped in this many steps (j << 10) into fx_1f7_trailPoints / sparkPoints */
#define FX_BLUE_SHOCKWAVE_TRAIL_POINTS  0x21 /*   strip points: a window of the chain's points, */
#define FX_BLUE_SHOCKWAVE_TRAIL_START   0x20 /*   starting this far in and moving 4 points back a frame, */
#define FX_BLUE_SHOCKWAVE_TRAIL_FADE    0x10 /*   fading (index110 << 8) over the last this many */
#define FX_BLUE_SHOCKWAVE_TRAIL_PASSES  4    /*   passes per strip (count10C), strip i delayed i * 2 frames */
#define FX_BLUE_SHOCKWAVE_SPARK_CHAINS  2    /* bone chains of the sparks (fx_1f7_sparkBones), */
#define FX_BLUE_SHOCKWAVE_SPARK_LINKS   6    /*   bone pairs each (7 bones) */
#define FX_BLUE_SHOCKWAVE_SPARKS        12   /* GLOW particles (fx_1f7_sparks), spark i delayed i * 3 frames, */
#define FX_BLUE_SHOCKWAVE_SPARK_FRAMES  6    /*   each plays this many frames at a random point, */
#define FX_BLUE_SHOCKWAVE_SPARK_REPEATS 2    /*   this many times */
#define FX_BLUE_SHOCKWAVE_FLASH_FRAME   0x18 /* FLASH and LIGHTNING start at the caster's body */
#define FX_BLUE_SHOCKWAVE_PULSE_FRAME   0x19 /* additive blue screen tint, (timer8 << 6) */
#define FX_BLUE_SHOCKWAVE_PULSE_FRAMES  4    /*   fading over this many frames */
#define FX_BLUE_SHOCKWAVE_FRAMES        0x40 /* the task ends at this frame */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[FX_BLUE_SHOCKWAVE_TRAIL_POINTS];
    s32 count10C; /* passes left */
    s32 index110; /* window start */
    s32 delay114; /* delay */
} Fx1f7Trail; /* 0x118 */

typedef struct {
    s32 index0; /* point */
    s32 offsetX4;
    s32 count8; /* repeats left */
    s32 frameC; /* frame */
    s32 delay10; /* delay */
} Fx1f7Spark; /* 0x14 */

typedef struct {
    s32 active0; /* trails on */
    s32 active4; /* sparks on */
    s32 timer8; /* flash */
    SVECTOR posC; /* caster body position */
} Fx1f7Work; /* 0x14 */

extern Unk800F78A4 *fx_1f7_env;
extern s32 fx_1f7_trailBones[FX_BLUE_SHOCKWAVE_TRAILS][FX_BLUE_SHOCKWAVE_TRAIL_LINKS + 1]; /* bones */
extern s32 fx_1f7_sparkBones[FX_BLUE_SHOCKWAVE_SPARK_CHAINS][FX_BLUE_SHOCKWAVE_SPARK_LINKS + 1]; /* bones */
extern SVECTOR fx_1f7_trailPoints[FX_BLUE_SHOCKWAVE_TRAIL_LINKS * FX_BLUE_SHOCKWAVE_LINK_STEPS];
extern SVECTOR fx_1f7_sparkPoints[FX_BLUE_SHOCKWAVE_SPARK_CHAINS * FX_BLUE_SHOCKWAVE_SPARK_LINKS * FX_BLUE_SHOCKWAVE_LINK_STEPS];
extern Fx1f7Trail fx_1f7_trails[FX_BLUE_SHOCKWAVE_TRAILS];
extern Fx1f7Spark fx_1f7_sparks[FX_BLUE_SHOCKWAVE_SPARKS];

#endif
