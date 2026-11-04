#ifndef FX_128_H
#define FX_128_H

/* Effect module fx_128 (dir 13 file 0x128): Dragon Breath. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_128 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DRAGON_BREATH_ROT_POS_PATH  0x2 /* table: rotation + position path (fxm_rotPosPathInit) */
#define FX_DRAGON_BREATH_VALUE_PATH    0x3 /* table: value path (fxm_valuePathInit) */
#define FX_DRAGON_BREATH_VIOLET_RINGS  0x8 /* emitter: violet and blue-white gouraud rings */
#define FX_DRAGON_BREATH_MODEL         0xB /* 'so' model (btlfx_registerEffectModel), probably a dragon (look pending) */
#define FX_DRAGON_BREATH_FAN_EMITTER   0x15 /* emitter: one fan element spiralling (not rendered) (look pending) */
#define FX_DRAGON_BREATH_GOLD_SPARKLES 0x16 /* emitter: gold and white sparkles + teal specks */
#define FX_DRAGON_BREATH_BUBBLES       0x17 /* emitter: gold sparkles + teal bubbles */
#define FX_DRAGON_BREATH_BUBBLES_2     0x18 /* emitter: gold sparkles + green bubbles spiralling */
#define FX_DRAGON_BREATH_MODEL_2       0x1F /* 'so' model (btlfx_registerEffectModel), probably the second dragon (look pending) */
#define FX_DRAGON_BREATH_GREEN_TRAIL   0x20 /* emitter: 3D helix trail, green streak strip */
#define FX_DRAGON_BREATH_ORB           0x21 /* emitter: green-and-orange swirled orb + gold sparkles and specks */
#define FX_DRAGON_BREATH_GREEN_TRAIL_2 0x22 /* emitter: screen helix trail, green streak strip */
#define FX_DRAGON_BREATH_ORB_2         0x23 /* emitter: green-and-orange swirled orb, one frame */
#define FX_DRAGON_BREATH_BUBBLE_RINGS  0x24 /* emitter: green bubbles + specks, white-green gouraud rings */
#define FX_DRAGON_BREATH_POS_PATH      0x25 /* table: position path (fxm_posPathInit) */
#define FX_DRAGON_BREATH_WHITE_RINGS   0x26 /* emitter: white and pale-cyan gouraud rings, 30 frames */

/* particle as the path callbacks see it */
typedef struct {
    VECTOR pos0; /* position */
    u8 pad10[0x1F - 0x10];
    u8 index1F; /* index (1-based) */
} Fx128Particle;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR rot10; /* caster rotation */
} Fx128Work80;

extern Unk800F78A4 *fx_128_env;
extern SVECTOR fx_128_lerpFrom; /* path start */
extern SVECTOR fx_128_lerpTo; /* path end */
extern SVECTOR fx_128_lerpToRand[]; /* path ends */

static s32 fx_128_op81ParticleMotion0(Fx128Particle *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fx_128_op81ParticleMotion1(Fx128Particle *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

typedef struct {
    s16 x, y, z;
} Fx128Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx128Point *keys4;
} Fx128Path;
static void fxm_rotPosPathInit(Fx128Path *path, u16 *data);
static void fxm_posPathInit(Fx128Path *path, u16 *data);
static s32 fxm_posPathGet(Fx128Path *path, s32 t, SVECTOR *out);
static void fxm_valuePathInit(Fx128Path *path, u16 *data);

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

/* propagated */

typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx128Key; /* 0xC */

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* keys */
    Fx128Key *keys4;
} Fx128Path_0b4;
static s32 fxm_rotPosPathGet(Fx128Path_0b4 *path, s32 t, SVECTOR *rot, SVECTOR *pos);

typedef struct {
    u16 unk0; /* not read by the module */
    u16 count2; /* count */
    s16 *values4;
} Fx128Keys;
static s32 fxm_valuePathGet(Fx128Keys *keys, s32 t, s32 *out);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx128Vec3;

typedef struct {
    Pool6CEntry *emit0[9];
    SVECTOR rot24; /* model rotation */
    Fx128Vec3 pos2C; /* model position */
    s32 pad38;
    Fx128Vec3 scale3C; /* model scale */
    s32 pad48;
    s32 fade4C; /* model colour */
    SVECTOR pos50; /* caster position */
    SVECTOR rot58; /* caster rotation */
    SVECTOR pos60; /* target body position */
    SVECTOR pos68; /* target position */
    SVECTOR pos70;
    SVECTOR pos78;
    SVECTOR rot80;
    SVECTOR pos88;
    SVECTOR pos90;
    s32 timer98; /* fade-in countdown */
    s32 timer9C; /* fade-out countdown */
    u16 modelA0[8]; /* models */
} Fx128Work81; /* 0xB0 */

extern u16 fx_128_modelFileIds[];
extern u16 fx_128_pathFileIds[];
extern BtlModelBuf fx_128_effModels[];
extern Fx128Path_0b4 fx_128_rotPosPath0;
extern Fx128Path_0b4 fx_128_rotPosPath1[];
extern Fx128Path fx_128_posPath;
extern Fx128Keys fx_128_valuePath;
extern SVECTOR fx_128_emitPos0;
extern SVECTOR fx_128_emitPos1;
extern u8 fx_128_op81Trails0[];
extern u8 fx_128_op81Trails1[];

#endif
