#ifndef FX_17A_H
#define FX_17A_H

/* Effect module fx_17a (dir 13 file 0x17A): Madeen (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_17a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MADEEN_BLUE_RING     0x1 /* rings: black-to-blue gouraud rings */
#define FX_MADEEN_BLUE_GLOW     0x2 /* particles: blue glow (pool task) */
#define FX_MADEEN_BLUE_RINGS 0x3 /* rings: white-blue gouraud rings */
#define FX_MADEEN_VIOLET_RING   0x4 /* rings: white-cyan-violet gouraud rings, four segments */
#define FX_MADEEN_TEXTURED_RING 0x5 /* rings: ring with a pink-green noise texture */
#define FX_MADEEN_TEAL_RING     0x6 /* rings: black-blue-teal gouraud rings */
#define FX_MADEEN_RAINBOW_ARCH  0x7 /* model: arch with a rainbow aura texture */
#define FX_MADEEN_WINGS         0x8 /* model: three-armed wing shape with a grey feather texture */
#define FX_MADEEN_LOOP_RIBBON 0x9 /* model: figure-eight band (its 15-bit texture page 0x138 holds a handwritten placeholder in the file) */
#define FX_MADEEN_RAINBOW_SPHERE 0xA /* model: sphere wrapped in red-green-blue rainbow stripe bands (byte-identical to fx_0fb_1 summon entry 0x89) */
#define FX_MADEEN_LENS_FLARES_2 0xB /* sprite line: green / red / orange lens-flare rings (byte-identical to fx_0fb_1 entry 0x17) */
#define FX_MADEEN_LENS_FLARES 0xC /* sprite line: green / red / orange lens-flare rings (byte-identical to fx_0fb_1 entry 0xC) */
#define FX_MADEEN_VIOLET_GLOW   0xD /* emitter: three fans, a horizontal violet-white glow */
#define FX_MADEEN_WHITE_GLOW    0xE /* particles: white-blue glow */
#define FX_MADEEN_BLUE_SPARKLES 0xF /* emitter: blue square sparkles and gold sprites */
#define FX_MADEEN_TRAIL         0x11 /* trail: strip texture not in the file (magenta in the render) (look pending) */
#define FX_MADEEN_BACKDROP      0x12 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_MADEEN_BLUE_STREAKS  0x13 /* emitter: orbiting blue light streaks */

typedef struct {
    s16 state0; /* state */
    s16 unk2; /* write-only (cleared at spawn) */
    s16 scale4; /* size */
    s16 unk6; /* unused */
    s16 unk8; /* unused */
    s16 speedA; /* drift */
    SVECTOR posC; /* position */
    SVECTOR unk14; /* write-only: rotation */
    s16 unk1C; /* write-only */
    s16 unk1E; /* write-only */
    s32 unk20; /* write-only (cleared at spawn) */
    s32 unk24; /* write-only (cleared at spawn) */
} Fx17aPart;
#define FX_17A_PART_SPARK 4 /* Fx17aPart.state0 of the drifting sparks (fx_0fb_1 FX_0FB_1_SPARK) */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17aVec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    u8 pad8[4];
    SVECTOR posC; /* position */
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR pos24;
    u8 pad2C[8];
    Fx17aVec3 pos34; /* model position */
    Fx17aVec3 scale40; /* model scale */
    SVECTOR rot4C; /* model rotation */
    u8 pad54[0xC];
    Fx17aVec3 scale60;
    SVECTOR rot6C;
    u8 pad74[0xC];
    s32 model80; /* models */
    s32 model84;
    s32 model88;
    s32 model8C;
    SVECTOR rot90[4];
    Pool6CEntry *emitB0;
} Fx17aWork80; /* 0xB4 */

typedef struct {
    u8 pad0[4];
    SVECTOR points4[0x40]; /* points; vy decays */
    u8 pad204[8];
} Fx17aRing; /* 0x20C */

extern SVECTOR fx_17a_centerPos;
extern u8 fx_17a_cylColors[];
extern Fx17aRing fx_17a_trails[2];
extern BtlModelBuf fx_17a_effModels[];

static s32 fx_17a_op80PoolTask(s32 mode, Fx17aPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

/* propagated */

extern Unk800F78A4 *fx_17a_env;
static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 radius, s32 bright, CVECTOR *rgb, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 dang, s32 abr);

#endif
