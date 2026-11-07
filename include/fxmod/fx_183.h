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

/* fx_183 choreography (docs/fxdata.md, "Choreography literals"): the ops' keyframes and lengths, the spear's drop,
 * op82's six path-driven models and their fades, the trail buffer. */
#define FX_CHERRY_BLOSSOM_STREAK_FRAMES  0x28 /* op80's length (LIGHT_STREAK and SPARKLE_CIRCLE at the caster) */
#define FX_CHERRY_BLOSSOM_MODEL_COUNT    7    /* btlfx_initEffModels slots (SPEAR and op82's six models) */
#define FX_CHERRY_BLOSSOM_DROP_HEIGHT    (-0x7D0) /* op81: the spear starts this high above the caster, falls on the targets' centre */
#define FX_CHERRY_BLOSSOM_IMPACT_FRAME   4    /* op81: PINK_RING and PETAL at the targets (lerp t = frame << 10 reaches them) */
#define FX_CHERRY_BLOSSOM_SPEAR_FRAMES   5    /* op81: the spear is drawn for this many frames */
#define FX_CHERRY_BLOSSOM_SPEAR_OP_FRAMES 0x3C /* op81's length */
#define FX_CHERRY_BLOSSOM_PATH_MODELS    6    /* op82: models moved by key paths (fx_183_rotPosPath, model50; the first at scale 0x1000, */
#define FX_CHERRY_BLOSSOM_PETAL_SCALE    0x1800 /* the others at this scale) */
#define FX_CHERRY_BLOSSOM_PATH_FRAMES    0x1A /* op82: the path models are drawn for this many frames, */
#define FX_CHERRY_BLOSSOM_FADE_IN_FRAMES 4    /* fading in from black (work->timer48, from frame 0), */
#define FX_CHERRY_BLOSSOM_FADE_OUT_FRAME 0x12 /* and out from this frame */
#define FX_CHERRY_BLOSSOM_FADE_OUT_FRAMES 8   /* (work->timer4C) */
#define FX_CHERRY_BLOSSOM_STORM_FRAME    0xF  /* op82: PETAL_STORM at the targets' centre */
#define FX_CHERRY_BLOSSOM_FRAMES         0x50 /* op82's length (the models are cleared) */
#define FX_CHERRY_BLOSSOM_PETAL_TRAIL_TRAIL_SLOTS  0xC  /* op83: PETAL_TRAIL's trail buffer fx_183_op83Trails[target]: slots of */
#define FX_CHERRY_BLOSSOM_PETAL_TRAIL_TRAIL_STRIDE 0x50 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_CHERRY_BLOSSOM_VORTEX_FRAMES  0x28 /* op83's length (PETAL_TRAIL, WHITE_RING, PETAL_VORTEX at one target) */

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
    u16 model50[FX_CHERRY_BLOSSOM_PATH_MODELS]; /* models */
} Fx183Work82; /* 0x5C */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target body position */
    s32 unit14; /* unit */
} Fx183Work83; /* 0x18 */

extern u16 fx_183_modelFileIds[FX_CHERRY_BLOSSOM_PATH_MODELS]; /* model files */
extern u16 fx_183_pathFileIds[FX_CHERRY_BLOSSOM_PATH_MODELS]; /* path files */
extern Unk800F78A4 *fx_183_env;
extern BtlModelBuf fx_183_effModels[];
extern Fx183Path fx_183_rotPosPath[FX_CHERRY_BLOSSOM_PATH_MODELS];
extern s32 fx_183_op83Trails[][FX_CHERRY_BLOSSOM_PETAL_TRAIL_TRAIL_SLOTS * FX_CHERRY_BLOSSOM_PETAL_TRAIL_TRAIL_STRIDE / 4];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_rotPosPathInit(Fx183Path *path, u16 *data);

/* copied */
extern Unk800F78A4 *fx_183_env;
/* end copied */

/* propagated */
static s32 fxm_rotPosPathGet(Fx183Path *path, s32 t, SVECTOR *rot, SVECTOR *pos);

#endif
