#ifndef FX_183_H
#define FX_183_H

/* Effect module fx_183 (dir 13 file 0x183): Cherry Blossom. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_183 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CHERRY_BLOSSOM_PETAL        0xD /* emitter: spiralling pink petal */
#define FX_CHERRY_BLOSSOM_WHITE_RING   0xE /* emitter: yellow-white gouraud rings + sprites (texture not in the file) */
#define FX_CHERRY_BLOSSOM_PETAL_STORM  0xF /* emitter: drifting pink petals and a large petal */
#define FX_CHERRY_BLOSSOM_PETAL_TRAIL  0x10 /* emitter: orbiting screen trail of petals */
#define FX_CHERRY_BLOSSOM_PINK_RING    0x11 /* emitter: pink and white-red gouraud rings */
#define FX_CHERRY_BLOSSOM_LIGHT_STREAK 0x17 /* emitter: fan drawn as a thin green to violet horizontal light streak (byte-identical to fx_03d entry 0x10) */
#define FX_CHERRY_BLOSSOM_SPARKLE_CIRCLE 0x18 /* emitter: circle of gold / white sparkle quads + teal specks (byte-identical to fx_03d entry 0x11) */
#define FX_CHERRY_BLOSSOM_SPEAR        0x1D /* 'so' model: a spear (the Spear model's shape and size, other bytes) */
#define FX_CHERRY_BLOSSOM_PETAL_VORTEX 0x1E /* emitter: pink swirl vortex + sprites (texture not in the file) */

typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx183Key; /* 0xC */

typedef struct {
    s16 unk0; /* not read by the module */
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

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_rotPosPathInit(Fx183Path *path, u16 *data);

/* copied */
extern Unk800F78A4 *fx_183_env;
/* end copied */

/* propagated */
static s32 fxm_rotPosPathGet(Fx183Path *path, s32 t, SVECTOR *rot, SVECTOR *pos);

#endif
