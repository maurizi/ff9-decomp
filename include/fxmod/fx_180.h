#ifndef FX_180_H
#define FX_180_H

/* Effect module fx_180 (dir 13 file 0x180). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx180Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx180Point *unk4;
} Fx180Path;

typedef struct {
    Pool6CEntry *emit0[9];
    SVECTOR pos24; /* caster body position */
    SVECTOR pos2C; /* caster position */
    SVECTOR unk34; /* target position */
    SVECTOR pos3C; /* bone position */
    SVECTOR pos44;
    SVECTOR unk4C;
    SVECTOR unk54; /* camera targets */
    SVECTOR unk5C;
    SVECTOR unk64;
    SVECTOR unk6C;
} Fx180Work80; /* 0x74 */

typedef struct {
    Pool6CEntry *emit0[11];
    SVECTOR unk2C; /* target position */
    SVECTOR pos34;
    SVECTOR pos3C;
    SVECTOR unk44;
    SVECTOR pos4C;
} Fx180Work81; /* 0x54 */

typedef struct {
    Pool6CEntry *emit0[8];
    SVECTOR rot20; /* model rotation */
    VECTOR pos28; /* model position */
    VECTOR scale38; /* model scale */
    s32 unk48; /* brightness */
    SVECTOR pos4C; /* target position */
    SVECTOR pos54;
    SVECTOR pos5C;
    s32 unk64; /* fade countdown */
    u16 model68; /* models */
    u16 model6A;
} Fx180Work82; /* 0x6C */

typedef struct {
    Pool6CEntry *emit0;
    u8 pad4[8 - 4];
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* caster position */
    SVECTOR unk18; /* destination */
    SVECTOR pos20; /* caster position */
    SVECTOR unk28;
    SVECTOR pos30;
    s16 unk38; /* fade countdown */
    s16 unk3A;
} Fx180Work83; /* 0x3C */

extern Unk800F78A4 *fx_180_env;
extern BtlModel fx_180_effModels[];
extern Fx180Path D_801E93F8[0x12];
extern Fx180Path D_801E9488[0x1E];
extern s32 D_801E9578[];
extern s32 D_801E9BB8[];
extern s32 D_801EA398[];
extern s32 D_801EB088[];
extern s32 D_801EBCE8[];
extern s32 D_801EC6E8[];
extern s32 D_801EDE88[];
extern s32 D_801EF3E8[];
extern s32 D_801EFF28[];
extern SVECTOR D_801F0828;
extern SVECTOR D_801F0830;
extern SVECTOR D_801F0838;
extern SVECTOR D_801F0840;

static s32 fx_180_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
static s32 fx_180_op81ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);
static void fxm_posPathInit(Fx180Path *path, u16 *data);
static s32 fxm_posPathGet(Fx180Path *path, s32 t, SVECTOR *out);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startMotion3IfActive(s32 unit);

/* copied */
extern Unk800F78A4 *fx_180_env;
/* end copied */

/* propagated */
static s32 fxm_moveUnitArc(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start);
static u16 *fxm_offsetTableEntry(u8 *base, u16 i);

#endif
