#ifndef FX_0C2_H
#define FX_0C2_H

/* Effect module fx_0c2 (dir 13 file 0xC2). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* unit 0x10 bone 0x17 position */
} Fx0c2Work80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* unit 0x10 position */
    SVECTOR unk14; /* unit 0x10 rotation */
    SVECTOR unk1C; /* target body position */
    SVECTOR unk24; /* target rotation */
    SVECTOR unk2C; /* unkC moved 0x12C along unk14 */
    SVECTOR unk34; /* btlfx_getUnitBonePos(0x10, 0x17) */
    SVECTOR unk3C; /* copy of unk44 */
    SVECTOR unk44; /* angles from unk34 to unk1C */
    s32 unk4C; /* target unit */
} Fx0c2Work81;

typedef struct {
    SVECTOR unk0; /* rotation step total */
    SVECTOR unk8; /* position: env->targets->centerX..Z */
} Fx0c2Work82;

typedef struct {
    SVECTOR unk0; /* turn angles */
    SVECTOR unk8; /* unit position */
    s32 unk10; /* unit */
} Fx0c2Work83;

extern SVECTOR D_801E8484; /* particle path start */
extern SVECTOR D_801E848C; /* particle path end */
extern SVECTOR D_801E8494;

s32 func_801E7804(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_turnUnitBy(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta);
static s32 fxm_turnUnitToward(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang);
void func_801E7D6C(s32 unit, s32 motion);
static void fxm_startMotion2IfActive(s32 unit);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static void fxm_startMotion3IfActive(s32 unit);
static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v);
extern Unk800F78A4 *D_801E847C;
static s32 fxm_isMotionDone(s32 unit);

#endif
