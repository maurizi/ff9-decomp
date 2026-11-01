#ifndef FX_1B2_H
#define FX_1B2_H

/* Effect module fx_1b2 (dir 13 file 0x1b2): enemy Tidal Wave (Nova Dragon). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1b2Key6;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1b2Key6 *keys4;
} Fx1b2Track6;

typedef struct {
    s16 x, y, z;
} Fx1b2Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1b2Point *keys4;
} Fx1b2Path;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR rot10; /* model rotation */
    VECTOR pos18; /* model position */
    VECTOR scale28; /* model scale */
    s32 fade38; /* brightness */
    SVECTOR rot3C;
    SVECTOR pos44; /* bone positions */
    SVECTOR pos4C;
    SVECTOR pos54;
    s32 timer5C; /* countdown */
    s32 timer60; /* fade countdown */
    s32 count64;
    s32 index68;
    u16 model6C; /* model */
} Fx1b2Work80; /* 0x70 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster position */
    SVECTOR pos14; /* bone positions */
    SVECTOR pos1C;
} Fx1b2Work81; /* 0x24 */

typedef struct {
    SVECTOR rot0; /* model rotation */
    VECTOR pos8; /* model position */
    VECTOR scale18; /* model scale */
    s32 fade28; /* brightness */
    s32 timer2C; /* fade-in countdown */
    s32 timer30; /* fade-out countdown */
    u16 model34; /* model */
} Fx1b2Work82; /* 0x38 */

extern Unk800F78A4 *fx_1b2_env;
extern BtlModelBuf fx_1b2_effModels[];
extern Fx1b2Path fx_1b2_vecPath;
extern u32 fx_1b2_bgOt[];
extern s32 fx_1b2_op80Trails0[][0x258];
extern s32 fx_1b2_op80Trails1[][0x258];
extern s32 fx_1b2_op81Trails0[];
extern s32 fx_1b2_op81Trails1[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_vecPathInit(Fx1b2Path *path, u16 *data);
static s32 fxm_vecPathGet(Fx1b2Track6 *track, s32 t, VECTOR *out);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHitMotionIfActive(s32 unit);

/* copied */
extern Unk800F78A4 *fx_1b2_env;
/* end copied */

#endif
