#ifndef FX_1A8_H
#define FX_1A8_H

/* Effect module fx_1a8 (dir 13 file 0x1a8): Odin (short); Zantetsuken. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1a8 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ODIN_BLUE_RINGS       0x1 /* emitter: white-cyan-violet and white-cyan-blue gouraud rings */
#define FX_ODIN_FLAME_TRAIL      0x2 /* trail: orange flame strip */
#define FX_ODIN_FAN_STREAK       0x5 /* emitter: two fan elements, a thin tan streak in the render (look pending) */
#define FX_ODIN_SKY_CRYSTAL      0x6 /* 'so' model: polyhedron crystal, blue sky-and-cloud texture */
#define FX_ODIN_FLAME_RIBBON     0x7 /* 'so' model: twisted ribbon, fire texture */
#define FX_ODIN_FLAME_RIBBON_2   0x8 /* 'so' model: twisted ribbon, fire texture */
#define FX_ODIN_FLAME_RIBBON_3   0x9 /* 'so' model: twisted ribbon, fire texture */
#define FX_ODIN_SKY_CRYSTAL_2    0xA /* 'so' model: flattened crystal, blue sky-and-cloud texture */
#define FX_ODIN_CRIMSON_RINGS    0xB /* rings: white-crimson gouraud rings, 16 frames */
#define FX_ODIN_SPECK_SPIRAL     0xC /* emitter: spiral of two speck-sprite elements, render nearly black (look pending) */
#define FX_ODIN_CRIMSON_RINGS_2  0xD /* emitter: white-crimson and white-violet gouraud rings */
#define FX_ODIN_FLAME_RIBBON_4   0x10 /* 'so' model: twisted ribbon, fire texture */
#define FX_ODIN_STONE_RING       0x11 /* 'so' model: ring band, grey-green stone texture */
#define FX_ODIN_SPECKS           0x12 /* emitter: speck sprites, render nearly black (look pending) */
#define FX_ODIN_PINK_SMOKE       0x13 /* emitter: white-pink smoke burst quads, 22 frames */
#define FX_ODIN_CLOUD_TORUS      0x14 /* 'so' model: torus, dark marbled cloud texture */
#define FX_ODIN_GOLD_RING        0x15 /* 'so' model: ring band, yellow-brown texture */
#define FX_ODIN_VORTEX_CONE      0x16 /* 'so' model: spiralling cone, brown-grey texture */
#define FX_ODIN_CALLBACK_EMITTER 0x17 /* emitter: two callback elements (drawn by the module) (look pending) */
#define FX_ODIN_ZANTETSUKEN      0x19 /* 'so' model: thin sword with a hilt, red-violet texture (the Zantetsuken blade) */

extern Unk800F78A4 *fx_1a8_env;
extern s32 fx_1a8_elemModels[2]; /* models */

static void fx_1a8_op80ElemDraw0(s32 pt, s32 owner, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 alpha, s32 blend, s32 frame, s32 n);
static void fx_1a8_op80ElemDraw1(s32 pt, s32 owner, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 alpha, s32 blend, s32 frame, s32 n);
static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 bright, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1a8Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    SVECTOR pos8;
    SVECTOR pos10;
    u8 pad18[0x20 - 0x18];
    SVECTOR pos20; /* ring position */
    s32 model28; /* model */
    SVECTOR rot2C; /* rotation */
    Fx1a8Vec3 scale34; /* scale */
    Fx1a8Vec3 pos40; /* position */
    s32 model4C[4]; /* models */
    s32 model5C;
    s32 model60;
    s32 model64;
    s32 model68;
    u8 pad6C[0x74 - 0x6C];
} Fx1a8Work80; /* 0x74 */

extern SVECTOR fx_1a8_targetPos;
extern u32 fx_1a8_bgOt[];
extern BtlModelBuf fx_1a8_effModels[];
extern u8 fx_1a8_trailPoints[];

#endif
