#ifndef FX_1EB_H
#define FX_1EB_H

/* Effect module fx_1eb (dir 13 file 0x1eb): Six Dragons. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1eb effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SIX_DRAGONS_DRAGON_ORBS    0x1 /* emitter: orbiting green-gold and red dragon-eye orbs */
#define FX_SIX_DRAGONS_GREEN_AURA     0x3 /* emitter: green orb rings and rising green-gold aura flames */
#define FX_SIX_DRAGONS_WHITE_RINGS    0x4 /* emitter: pale blue / white gouraud rings */
#define FX_SIX_DRAGONS_BLUE_BUBBLES   0x5 /* emitter: blue hemisphere bubbles + small bubble sprites */
#define FX_SIX_DRAGONS_STONE_DRAGON   0x6 /* 'so' model: winged stone dragon (rock texture) */
#define FX_SIX_DRAGONS_VALUE_PATH     0x7 /* table: value path keys for fxm_valuePathInit */
#define FX_SIX_DRAGONS_STONE_TRAIL    0x8 /* emitter: spiralling 3D trail with a rock texture, small spark sprites */
#define FX_SIX_DRAGONS_SPARKLE_CIRCLE 0x14 /* emitter: circle of gold / white sparkle quads + blue-green specks (byte-identical to fx_03d entry 0x11) */
#define FX_SIX_DRAGONS_FAN_EMITTER    0x15 /* emitter: fan, green light streaks (byte-identical to fx_03d entry 0x10) */
#define FX_SIX_DRAGONS_WHITE_RINGS_2  0x1D /* emitter: white gouraud rings, 45 frames */
#define FX_SIX_DRAGONS_SMOKE          0x1E /* emitter: green-to-pink smoke puffs */

typedef struct {
    s16 x, y, z;
} Fx1ebPoint;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1ebPoint *keys4;
} Fx1ebPath;

typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx1ebKey; /* 0xC */

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* keys */
    Fx1ebKey *keys4;
} Fx1ebKeyPath;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR rot10; /* caster rotation */
} Fx1ebWork80; /* 0x18 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    s32 rgb40; /* brightness */
    SVECTOR pos44; /* target position */
    s32 timer4C; /* fade countdown */
    u16 alpha50;
    u16 model52[3]; /* models */
    u16 model58; /* morph target */
} Fx1ebWork82; /* 0x60 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    s32 unitC; /* unit */
} Fx1ebWork83;

extern u16 fx_1eb_modelFileIds[3]; /* model files */
extern u16 fx_1eb_pathFileIds[3]; /* path files */
extern Unk800F78A4 *fx_1eb_env;
extern BtlModelBuf fx_1eb_effModels[];
extern Fx1ebKeyPath fx_1eb_rotPosPath[3];
extern Fx1ebPath fx_1eb_valuePath;
extern s32 fx_1eb_op82Trails[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_rotPosPathInit(Fx1ebKeyPath *path, u16 *data);
static void fxm_valuePathInit(Fx1ebPath *path, u16 *data);

/* copied */
extern Unk800F78A4 *fx_1eb_env;
/* end copied */

/* propagated */
static s32 fxm_rotPosPathGet(Fx1ebKeyPath *path, s32 t, SVECTOR *rot, SVECTOR *pos);
static s32 fxm_valuePathGet(Fx1ebPath *keys, s32 t, s32 *out);

#endif
