#ifndef FX_183_H
#define FX_183_H

/* Effect module fx_183 (dir 13 file 0x183): Cherry Blossom. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx183Key; /* 0xC */

typedef struct {
    s16 unk0;
    u16 count2; /* keys */
    Fx183Key *keys4;
} Fx183Path;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR rot10; /* caster rotation */
} Fx183Work80; /* 0x18 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster position */
    SVECTOR pos10; /* target position */
    SVECTOR pos18; /* from */
    SVECTOR rot20; /* model rotation */
    VECTOR pos28; /* model position */
    VECTOR scale38; /* model scale */
    u16 model48; /* model */
} Fx183Work81; /* 0x4C */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR rot4; /* model rotation */
    VECTOR posC; /* model position */
    VECTOR scale1C; /* model scales */
    VECTOR scale2C;
    s32 fade3C; /* brightness */
    SVECTOR pos40; /* target position */
    s32 timer48; /* fade-in countdown */
    s32 timer4C; /* fade-out countdown */
    u16 model50[6]; /* models */
} Fx183Work82; /* 0x5C */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target body position */
    s32 unit14; /* unit */
} Fx183Work83; /* 0x18 */

extern u16 fx_183_modelFileIds[6]; /* model files */
extern u16 fx_183_pathFileIds[6]; /* path files */
extern Unk800F78A4 *fx_183_env;
extern BtlModelBuf fx_183_effModels[];
extern Fx183Path fx_183_rotPosPath[6];
extern s32 fx_183_op83Trails[][0xF0];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_rotPosPathInit(Fx183Path *path, u16 *data);

/* copied */
extern Unk800F78A4 *fx_183_env;
/* end copied */

/* propagated */
static s32 fxm_rotPosPathGet(Fx183Path *path, s32 i, SVECTOR *rot, SVECTOR *pos);

#endif
