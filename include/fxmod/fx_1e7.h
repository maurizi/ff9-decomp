#ifndef FX_1E7_H
#define FX_1E7_H

/* Effect module fx_1e7 (dir 13 file 0x1e7): enemy Wrong!!! (False/Ragtime Mouse/True). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1e7Point;

typedef struct {
    s16 unk0;
    u16 count2; /* count */
    Fx1e7Point *keys4;
} Fx1e7Path;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
    s16 unk10;
} Fx1e7Work80; /* 0x14 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4[5];
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR pos20; /* target body position */
    SVECTOR pos28; /* target position */
    s32 unk30;
    s32 flag34; /* waiting for the motion to end */
} Fx1e7Work81; /* 0x38 */

extern u16 fx_1e7_op81EmitFileIds[5]; /* files */
extern u16 fx_1e7_op81PathFileIds[5]; /* path files */
extern s32 (*fx_1e7_op81SpawnPoints[5])(SVECTOR *, SVECTOR *, s32, s32);
extern Unk800F78A4 *fx_1e7_env;
extern s32 fx_1e7_op81Frame; /* op81 frame */
extern Fx1e7Path fx_1e7_posPath;
extern Fx1e7Path fx_1e7_op81Paths[5];
extern s32 fx_1e7_op80Trails0[];
extern s32 fx_1e7_op80Trails1[];
extern SVECTOR fx_1e7_op80Origin;
extern SVECTOR fx_1e7_op81Origin;
extern SVECTOR fx_1e7_op80PathRot; /* angles */
extern SVECTOR fx_1e7_emitPos0;
extern SVECTOR fx_1e7_emitPos1[5];

static s32 fxm_followPathParticle(Pool30Entry *e, SVECTOR *pos, Fx1e7Path *path, s32 frame, s32 frames);
void fx_1e7_getSpawnPos(SVECTOR *out, Fx1e7Path *path, s32 t);
static void fxm_posPathInit(Fx1e7Path *path, u16 *data);
static s32 fxm_posPathGet(Fx1e7Path *path, s32 t, SVECTOR *out);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHeavyHitMotionIfActive(s32 unit);

/* propagated */
extern Fx1e7Path fx_1e7_posPath0[];
static s32 fx_1e7_op80ParticleMotion0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_1e7_op80ParticleMotion1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_1e7_op81SpawnPoint0(SVECTOR *out, s32 arg1, s32 i);
s32 fx_1e7_op81SpawnPoint1(SVECTOR *out, s32 arg1, s32 i);
s32 fx_1e7_op81SpawnPoint2(SVECTOR *out, s32 arg1, s32 i);
s32 fx_1e7_op81SpawnPoint3(SVECTOR *out, s32 arg1, s32 i);
s32 fx_1e7_op81SpawnPoint4(SVECTOR *out, s32 arg1, s32 i);

#endif
