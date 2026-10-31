#ifndef FX_100_H
#define FX_100_H

/* Effect module fx_100 (dir 13 file 0x100). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14; /* caster position */
    SVECTOR unk1C; /* target position */
    SVECTOR unk24;
} Fx100Work80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* target position */
    SVECTOR unk14;
} Fx100Work81;

extern Unk800F78A4 *fx_100_env;
extern s32 D_801E7D6C[];
extern s32 D_801E827C[];

static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startMotion2IfActive(s32 unit);
static void fxm_startMotion3IfActive(s32 unit);

#endif
