#ifndef FX_1A7_H
#define FX_1A7_H

/* Effect module fx_1a7 (dir 13 file 0x1a7): enemy Mind Blast (Drakan). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1a7 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MIND_BLAST_WAVE_TRAIL   0x1 /* emitter: orbiting screen trail, blue wavy line / gold pattern strip (sprites render black) */
#define FX_MIND_BLAST_GREEN_RINGS  0x2 /* emitter: dark-to-pale green gouraud rings, 33 frames */
#define FX_MIND_BLAST_WAVE_TRAIL_2 0x4 /* emitter: wobbling screen trail, blue wavy line / gold pattern strip (sprites render black) */
#define FX_MIND_BLAST_TEAL_RINGS   0x5 /* emitter: teal-to-pale-green gouraud rings, 24 frames */
#define FX_MIND_BLAST_WAVE_TRAIL_3 0x7 /* emitter: wobbling screen trail, white-violet wavy strip (sprites render black) */
#define FX_MIND_BLAST_VIOLET_ORB   0x18 /* emitter: violet glowing orb with a bright rim (second particle renders black) */

/* fx_1a7 choreography (docs/fxdata.md, "Choreography literals"): op80's charge at the caster's bone, then nine
 * spiralling bolts to the target; op81's nine bolts back from the target and the orb behind the caster; trail
 * buffers, keyframes and lengths. */
#define FX_MIND_BLAST_BONE         0x1C  /* unit 0x10's bone the charge sits on and the bolts leave from */
#define FX_MIND_BLAST_CHARGE_AHEAD 0x190 /* op80: WAVE_TRAIL and GREEN_RINGS follow a point this far ahead of it */
#define FX_MIND_BLAST_CHARGE_FRAMES 0x18 /* while frame < this */
#define FX_MIND_BLAST_WAVE_TRAIL_TRAIL_SLOTS  0x10 /* WAVE_TRAIL's trail buffer fx_1a7_op80Trails0: slots of */
#define FX_MIND_BLAST_WAVE_TRAIL_TRAIL_STRIDE 0x60 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_MIND_BLAST_LAUNCH_FRAME 0x32  /* op80: the bolts start (fx_1a7_op80StartFrames after this frame) */
#define FX_MIND_BLAST_LAUNCH_AHEAD 0xC8  /* from this far ahead of the bone */
#define FX_MIND_BLAST_LAUNCH_HEIGHT 0xC8 /* and this much higher */
#define FX_MIND_BLAST_BOLTS        9     /* bolts per op (emitter file ids, start frames, motions, angles, radii, trails) */
#define FX_MIND_BLAST_BOLT_FRAMES  12    /* a bolt's particles cross in this many frames */
#define FX_MIND_BLAST_BOLT_TRAIL_SLOTS    1    /* op80: each bolt's trail buffer fx_1a7_op80Trails1[i]: slots of */
#define FX_MIND_BLAST_BOLT_TRAIL_STRIDE   0x38 /* this many bytes */
#define FX_MIND_BLAST_FRAMES       0x50  /* op80's length (the op ends) */
#define FX_MIND_BLAST_TRAIL_2_HEIGHT 0xC8 /* op81: WAVE_TRAIL_2 sits this high over the target's body */
#define FX_MIND_BLAST_WAVE_TRAIL_2_TRAIL_SLOTS  0x16 /* its trail buffer fx_1a7_op81Trails0: slots of */
#define FX_MIND_BLAST_WAVE_TRAIL_2_TRAIL_STRIDE 0x38 /* this many bytes */
#define FX_MIND_BLAST_ORB_BEHIND   0x190 /* op81: the bolts fly to (and VIOLET_ORB sits at) this far behind the caster */
#define FX_MIND_BLAST_BACKLASH_FRAME 0x14 /* op81: the bolts start (fx_1a7_op81StartFrames after this frame) */
#define FX_MIND_BLAST_BACKLASH_TRAIL_SLOTS  1    /* op81: each bolt's trail buffer fx_1a7_op81Trails1[i]: slots of */
#define FX_MIND_BLAST_BACKLASH_TRAIL_STRIDE 0xB0 /* this many bytes */
#define FX_MIND_BLAST_ORB_FRAME    0x1C  /* op81: WAVE_TRAIL_3 at the caster, VIOLET_ORB behind it */
#define FX_MIND_BLAST_WAVE_TRAIL_3_TRAIL_SLOTS  0xD  /* WAVE_TRAIL_3's trail buffer fx_1a7_op81Trails2: slots of */
#define FX_MIND_BLAST_WAVE_TRAIL_3_TRAIL_STRIDE 0xB0 /* this many bytes */
#define FX_MIND_BLAST_BACKLASH_FRAMES 0x28 /* op81's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8[FX_MIND_BLAST_BOLTS];
    SVECTOR pos2C; /* target body position */
    SVECTOR pos34;
} Fx1a7Work80; /* 0x3C */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8[FX_MIND_BLAST_BOLTS];
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    SVECTOR pos34; /* caster body position */
    SVECTOR pos3C; /* target body position */
    SVECTOR pos44; /* target position */
    SVECTOR angle4C; /* angles */
    SVECTOR pos54;
    SVECTOR pos5C;
} Fx1a7Work81; /* 0x64 */

extern u16 fx_1a7_op80EmitFileIds[FX_MIND_BLAST_BOLTS]; /* files */
extern u16 fx_1a7_op80StartFrames[FX_MIND_BLAST_BOLTS]; /* start frames */
extern u16 fx_1a7_op81EmitFileIds[FX_MIND_BLAST_BOLTS];
extern u16 fx_1a7_op81StartFrames[FX_MIND_BLAST_BOLTS];
extern s32 fx_1a7_op80Trails0[];
extern s32 fx_1a7_op80Trails1[FX_MIND_BLAST_BOLTS][FX_MIND_BLAST_BOLT_TRAIL_SLOTS * FX_MIND_BLAST_BOLT_TRAIL_STRIDE / 4];
extern s32 fx_1a7_op81Trails0[];
extern s32 fx_1a7_op81Trails1[FX_MIND_BLAST_BOLTS][FX_MIND_BLAST_BACKLASH_TRAIL_SLOTS * FX_MIND_BLAST_BACKLASH_TRAIL_STRIDE / 4];
extern s32 fx_1a7_op81Trails2[];
extern SVECTOR fx_1a7_emitPos;

extern s32 (*fx_1a7_op80ParticleMotions[FX_MIND_BLAST_BOLTS])();
extern s32 (*fx_1a7_op81ParticleMotions[FX_MIND_BLAST_BOLTS])();
extern Unk800F78A4 *fx_1a7_env;
extern s16 fx_1a7_op80Angles[FX_MIND_BLAST_BOLTS]; /* angles */
extern s16 fx_1a7_op81Angles[FX_MIND_BLAST_BOLTS];
extern s16 fx_1a7_op80Radii[FX_MIND_BLAST_BOLTS]; /* radii */
extern s16 fx_1a7_op81Radii[FX_MIND_BLAST_BOLTS];
extern SVECTOR fx_1a7_op80MotionFrom; /* from */
extern SVECTOR fx_1a7_op81MotionFrom;
extern SVECTOR fx_1a7_op80MotionTo; /* to */
extern SVECTOR fx_1a7_op81MotionTo;
s32 fx_1a7_op80ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op80ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op80ParticleMotion2(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op80ParticleMotion3(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op80ParticleMotion4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op80ParticleMotion5(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op80ParticleMotion6(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op80ParticleMotion7(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op80ParticleMotion8(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op81ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op81ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op81ParticleMotion2(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op81ParticleMotion3(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op81ParticleMotion4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op81ParticleMotion5(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op81ParticleMotion6(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op81ParticleMotion7(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a7_op81ParticleMotion8(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fxm_lineOrbitParticle0(Pool30Entry *pt, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames);
static s32 fxm_lineOrbitParticle1(Pool30Entry *pt, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames);

#endif
