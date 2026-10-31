#ifndef FX_1EC_H
#define FX_1EC_H

/* Effect module fx_1ec (dir 13 file 0x1ec). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1ecPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1ecPoint *unk4;
} Fx1ecPath;

typedef struct {
    Pool6CEntry *emit0[9];
    SVECTOR pos24; /* caster body position */
    SVECTOR unk2C; /* target position */
    SVECTOR pos34;
    SVECTOR unk3C;
    SVECTOR unk44;
    SVECTOR unk4C; /* camera targets */
    SVECTOR unk54;
    SVECTOR unk5C;
} Fx1ecWork80; /* 0x64 */

typedef struct {
    Pool6CEntry *emit0[11];
    SVECTOR unk2C; /* target position */
    SVECTOR pos34;
    SVECTOR pos3C;
    SVECTOR unk44;
    SVECTOR pos4C;
} Fx1ecWork81; /* 0x54 */

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
    s16 unk68; /* screen fade countdown */
    s16 unk6A; /* screen flash */
    u16 model6C; /* models */
    u16 model6E;
} Fx1ecWork82; /* 0x70 */

extern BtlModel fx_1ec_effModels[];
extern s32 D_801E91B4[];
extern s32 D_801E97F4[];
extern s32 D_801E9FD4[];
extern s32 D_801EACC4[];
extern s32 D_801EB924[];
extern s32 D_801EC324[];
extern s32 D_801EDAC4[];
extern s32 D_801EF024[];
extern SVECTOR D_801EFB74;
extern SVECTOR D_801EFB7C;

static s32 fx_1ec_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
static s32 fx_1ec_op81ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);
static void fxm_posPathInit(Fx1ecPath *path, u16 *data);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startMotion3IfActive(s32 unit);

/* copied */
extern Unk800F78A4 *fx_1ec_env;
extern Fx1ecPath D_801E9034[0x12];
static s32 fxm_posPathGet(Fx1ecPath *path, s32 t, SVECTOR *out);
extern SVECTOR D_801EFB64;
extern Fx1ecPath D_801E90C4[0x1E];
extern SVECTOR D_801EFB6C;
static void fxm_startUnitMotion(s32 unit, s32 motion);
/* end copied */

/* propagated */
static u16 *fxm_offsetTableEntry(u8 *base, u16 i);

#endif
