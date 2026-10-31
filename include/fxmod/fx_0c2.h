#ifndef FX_0C2_H
#define FX_0C2_H

/* Effect module fx_0c2 (dir 13 file 0xC2): No Mercy (Elan). Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* unit 0x10 bone 0x17 position */
} Fx0c2Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* unit 0x10 position */
    SVECTOR rot14; /* unit 0x10 rotation */
    SVECTOR pos1C; /* target body position */
    SVECTOR rot24; /* target rotation */
    SVECTOR pos2C; /* posC moved 0x12C along rot14 */
    SVECTOR pos34; /* btlfx_getUnitBonePos(0x10, 0x17) */
    SVECTOR rot3C; /* copy of rot44 */
    SVECTOR rot44; /* angles from pos34 to pos1C */
    s32 unit4C; /* target unit */
} Fx0c2Work81;

typedef struct {
    SVECTOR turn0; /* rotation step total */
    SVECTOR pos8; /* position: env->targets->centerX..Z */
} Fx0c2Work82;

typedef struct {
    SVECTOR turn0; /* turn angles */
    SVECTOR pos8; /* unit position */
    s32 unit10; /* unit */
} Fx0c2Work83;

extern SVECTOR fx_0c2_pathFrom; /* particle path start */
extern SVECTOR fx_0c2_pathTo; /* particle path end */
extern SVECTOR fx_0c2_emitPos;

static s32 fx_0c2_op81ParticleMotion(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_turnUnitBy(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta);
static s32 fxm_turnUnitToward(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang);
static void fxm_startHitMotionIfActive(s32 unit);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static void fxm_startHeavyHitMotionIfActive(s32 unit);
static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v);
extern Unk800F78A4 *fx_0c2_env;
static s32 fxm_isMotionDone(s32 unit);

#endif
