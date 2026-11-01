#ifndef FX_1A7_H
#define FX_1A7_H

/* Effect module fx_1a7 (dir 13 file 0x1a7): enemy Mind Blast (Drakan). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8[9];
    SVECTOR pos2C; /* target body position */
    SVECTOR pos34;
} Fx1a7Work80; /* 0x3C */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8[9];
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    SVECTOR pos34; /* caster body position */
    SVECTOR pos3C; /* target body position */
    SVECTOR pos44; /* target position */
    SVECTOR angle4C; /* angles */
    SVECTOR pos54;
    SVECTOR pos5C;
} Fx1a7Work81; /* 0x64 */

extern u16 fx_1a7_op80EmitFileIds[9]; /* files */
extern u16 fx_1a7_op80StartFrames[9]; /* start frames */
extern u16 fx_1a7_op81EmitFileIds[9];
extern u16 fx_1a7_op81StartFrames[9];
extern s32 fx_1a7_op80Trails0[];
extern s32 fx_1a7_op80Trails1[9][0xE];
extern s32 fx_1a7_op81Trails0[];
extern s32 fx_1a7_op81Trails1[9][0x2C];
extern s32 fx_1a7_op81Trails2[];
extern SVECTOR fx_1a7_emitPos;

extern s32 (*fx_1a7_op80ParticleMotions[9])();
extern s32 (*fx_1a7_op81ParticleMotions[9])();
extern Unk800F78A4 *fx_1a7_env;
extern s16 fx_1a7_op80Angles[9]; /* angles */
extern s16 fx_1a7_op81Angles[9];
extern s16 fx_1a7_op80Radii[9]; /* radii */
extern s16 fx_1a7_op81Radii[9];
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
static s32 fxm_lineOrbitParticle0(Pool30Entry *e, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames);
static s32 fxm_lineOrbitParticle1(Pool30Entry *e, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames);

#endif
