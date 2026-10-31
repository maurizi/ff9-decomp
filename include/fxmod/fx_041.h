#ifndef FX_041_H
#define FX_041_H

/* Effect module fx_041 (dir 13 file 0x41). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx041Point;

/* point list read from the effect file */
typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx041Point *unk4;
} Fx041Path;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* target body position */
    SVECTOR pos20; /* target position */
    SVECTOR pos28; /* caster position */
} Fx041Work80; /* 0x30 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target position */
    SVECTOR pos14;
} Fx041Work81; /* 0x1C */

extern Unk800F78A4 *fx_041_env;
extern Fx041Path D_801E7F44;
extern SVECTOR D_801E7F4C; /* target position */
extern SVECTOR D_801E7F54; /* last path point */
extern SVECTOR D_801E7F5C;
extern s32 D_801E7F6C[];

static s32 fx_041_op80ParticleMotion(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 t, s32 n);
static void fxm_posPathInit(Fx041Path *path, u16 *data);
static s32 fxm_posPathGet(Fx041Path *path, s32 t, SVECTOR *out);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startMotion2IfActive(s32 unit);
static void fxm_startMotion3IfActive(s32 unit);

#endif
