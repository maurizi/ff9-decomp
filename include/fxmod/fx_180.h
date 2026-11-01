#ifndef FX_180_H
#define FX_180_H

/* Effect module fx_180 (dir 13 file 0x180): enemy Ultima (Trance Kuja). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx180Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx180Point *keys4;
} Fx180Path;

typedef struct {
    Pool6CEntry *emit0[9];
    SVECTOR pos24; /* caster body position */
    SVECTOR pos2C; /* caster position */
    SVECTOR pos34; /* target position */
    SVECTOR pos3C; /* bone position */
    SVECTOR pos44;
    SVECTOR unk4C; /* write-only (copy of pos24) */
    SVECTOR camPos54; /* camera targets */
    SVECTOR camPos5C;
    SVECTOR camPos64;
    SVECTOR camPos6C;
} Fx180Work80; /* 0x74 */

typedef struct {
    Pool6CEntry *emit0[11];
    SVECTOR pos2C; /* target position */
    SVECTOR pos34;
    SVECTOR pos3C;
    SVECTOR pos44;
    SVECTOR pos4C;
} Fx180Work81; /* 0x54 */

typedef struct {
    Pool6CEntry *emit0[8];
    SVECTOR rot20; /* model rotation */
    VECTOR pos28; /* model position */
    VECTOR scale38; /* model scale */
    s32 fade48; /* brightness */
    SVECTOR pos4C; /* target position */
    SVECTOR pos54;
    SVECTOR pos5C;
    s32 timer64; /* fade countdown */
    u16 model68; /* models */
    u16 model6A;
} Fx180Work82; /* 0x6C */

typedef struct {
    Pool6CEntry *emit0;
    u8 pad4[8 - 4];
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* caster position */
    SVECTOR pos18; /* destination */
    SVECTOR pos20; /* caster position */
    SVECTOR pos28;
    SVECTOR pos30;
    s16 timer38; /* fade countdown */
    s16 done3A;
} Fx180Work83; /* 0x3C */

extern Unk800F78A4 *fx_180_env;
extern BtlModelBuf fx_180_effModels[];
extern Fx180Path fx_180_posPath0[0x12];
extern Fx180Path fx_180_posPath1[0x1E];
extern s32 fx_180_op80Trails0[];
extern s32 fx_180_op80Trails1[];
extern s32 fx_180_op81Trails0[];
extern s32 fx_180_op81Trails1[];
extern s32 fx_180_op81Trails2[];
extern s32 fx_180_op81Trails3[];
extern s32 fx_180_op82Trails0[];
extern s32 fx_180_op82Trails1[];
extern s32 fx_180_op83Trails[];
extern SVECTOR fx_180_op80PathOrigin;
extern SVECTOR fx_180_op81PathOrigin;
extern SVECTOR fx_180_emitPos0;
extern SVECTOR fx_180_emitPos1;

static s32 fx_180_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
static s32 fx_180_op81ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);
static void fxm_posPathInit(Fx180Path *path, u16 *data);
static s32 fxm_posPathGet(Fx180Path *path, s32 t, SVECTOR *out);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHeavyHitMotionIfActive(s32 unit);

/* copied */
extern Unk800F78A4 *fx_180_env;
/* end copied */

/* propagated */
static s32 fxm_moveUnitArc(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start);
static u16 *fxm_offsetTableEntry(u8 *base, u16 i);

#endif
