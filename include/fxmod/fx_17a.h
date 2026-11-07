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
#define FX_MADEEN_SPIRAL_STREAK 0x11 /* trail: the thin white curved light streaks spiralling round the blast (the tool still resolves the strip's tpage to the placeholder page); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_MADEEN_BACKDROP      0x12 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_MADEEN_BLUE_STREAKS  0x13 /* emitter: orbiting blue light streaks */

/* fx_17a choreography (docs/fxdata.md, "Choreography literals"): op80's states (work->state0; the frame restarts at
 * each) and their lengths, the model textures, the cylinder and its scrolling textures, the spark pool, counts and
 * screen tints. */
#define FX_MADEEN_STATE_GATHER   2    /* RAINBOW_SPHERE shrinks into the centre inside BLUE_RINGS */
#define FX_MADEEN_GATHER_FRAMES  0x12
#define FX_MADEEN_STATE_ARCH     3    /* RAINBOW_ARCH copies open, VIOLET_RING / VIOLET_GLOW, LENS_FLARES, a white flash */
#define FX_MADEEN_ARCH_FRAMES    0x18
#define FX_MADEEN_STATE_COLLAPSE 4    /* the sphere and rings collapse, a white flash then darkening */
#define FX_MADEEN_COLLAPSE_FRAMES 0xA
#define FX_MADEEN_STATE_BLAST    5    /* the blast: cylinder, WINGS, LOOP_RIBBON, WHITE_GLOW ring, SPIRAL_STREAKs, sparks */
#define FX_MADEEN_BLAST_FRAMES   0x33
#define FX_MADEEN_STATE_FADE     6    /* BLUE_RINGS fade with a white flash (the op ends; the models are cleared) */
#define FX_MADEEN_FADE_FRAMES    0x10
#define FX_MADEEN_SPARK_POOL     0x30 /* the pool of FX_17A_PART_SPARK particles (Fx17aPart) */
#define FX_MADEEN_SPARK_FRAMES   0x11 /* a spark's life */
#define FX_MADEEN_MODEL_COUNT    6    /* btlfx_initEffModels slots */
#define FX_MADEEN_RAINBOW_SPHERE_TPAGE 0xBA   /* 8-bit page (0x280, 0x100), additive */
#define FX_MADEEN_RAINBOW_SPHERE_CLUT  0x3FC0 /* CLUT (0, 0xFF) */
#define FX_MADEEN_RAINBOW_SPHERE_U     0
#define FX_MADEEN_RAINBOW_SPHERE_V     0
#define FX_MADEEN_RAINBOW_ARCH_TPAGE   0xBA   /* the same page */
#define FX_MADEEN_RAINBOW_ARCH_CLUT    0x3EC0 /* CLUT (0, 0xFB) */
#define FX_MADEEN_RAINBOW_ARCH_U       0
#define FX_MADEEN_RAINBOW_ARCH_V       0x80
#define FX_MADEEN_WINGS_TPAGE          0x3A   /* 4-bit page (0x280, 0x100), additive */
#define FX_MADEEN_WINGS_CLUT           0x3E81 /* CLUT (0x10, 0xFA) */
#define FX_MADEEN_WINGS_U              0
#define FX_MADEEN_WINGS_V              0x80
#define FX_MADEEN_LOOP_RIBBON_TPAGE    0x138  /* 15-bit page (0x200, 0x100), additive */
#define FX_MADEEN_LOOP_RIBBON_CLUT     0xFFFF /* (no CLUT) */
#define FX_MADEEN_LOOP_RIBBON_U        0
#define FX_MADEEN_LOOP_RIBBON_V        0x80
#define FX_MADEEN_ARCH_COPIES    4    /* STATE_ARCH: RAINBOW_ARCH drawn at rot90[] */
#define FX_MADEEN_CYLINDER_ROWS    20 /* fxm_drawTexCylinder: rows of */
#define FX_MADEEN_CYLINDER_COLUMNS 16 /* quads (texture tiles 4 x 16) */
#define FX_MADEEN_CYLINDER_TPAGE 0x17   /* STATE_BLAST's cylinder: 4-bit page (0x1C0, 0x100), opaque */
#define FX_MADEEN_CYLINDER_CLUT  0x3E80 /* CLUT (0, 0xFA) */
/* Scrolling textures (STATE_BLAST): the cylinder's page gets (SRC_X, SRC_Y) 7 texels a frame; WINGS' page at
 * v 0x80, u 0x80 gets its strip 10 a frame. */
#define FX_MADEEN_CYLINDER_VRAM_X 0x1C0
#define FX_MADEEN_CYLINDER_VRAM_Y 0x100
#define FX_MADEEN_CYLINDER_VRAM_W 0x20
#define FX_MADEEN_CYLINDER_VRAM_H 0x80
#define FX_MADEEN_CYLINDER_SRC_X  0x2A0
#define FX_MADEEN_CYLINDER_SRC_Y  0x100
#define FX_MADEEN_WINGS_VRAM_X    0x2A0
#define FX_MADEEN_WINGS_VRAM_Y    0x180
#define FX_MADEEN_WINGS_VRAM_W    0x10
#define FX_MADEEN_WINGS_VRAM_H    0x40
#define FX_MADEEN_WINGS_SRC_X     0x2B0
#define FX_MADEEN_WINGS_SRC_Y     0x180
#define FX_MADEEN_FLASH_FRAMES   6    /* STATE_COLLAPSE / STATE_BLAST: the white flash fades over this many frames */
#define FX_MADEEN_DARKEN_FRAME   7    /* STATE_COLLAPSE: the screen darkens from here (0x55 a frame) */
#define FX_MADEEN_BLACKOUT_RGB   0xFF, 0xFF, 0xFF /* STATE_COLLAPSE's last frame: subtractive */
#define FX_MADEEN_BLAST_TINT_RGB 0xA0, 0, 0xFF    /* STATE_BLAST frame 0: opaque violet */
#define FX_MADEEN_BLAST_DARKEN_RGB 0xFF, 0xFF, 0  /* frame 1: subtractive */
#define FX_MADEEN_TEX_RING_FRAMES 0x20 /* STATE_BLAST: the two TEXTURED_RINGs */
#define FX_MADEEN_WINGS_FRAMES   0x19 /* STATE_BLAST: WINGS fade out over this many frames */
#define FX_MADEEN_GROW_FRAMES    0x18 /* STATE_BLAST: the glow ring and streaks grow, LENS_FLARES_2 shows */
#define FX_MADEEN_GLOW_PARTICLES 0x80 /* STATE_BLAST: WHITE_GLOW particles round the centre */
#define FX_MADEEN_STREAK_POINTS  0x40 /* Fx17aRing points (SPIRAL_STREAK trail buffers; they sink 0x40 a frame) */
#define FX_MADEEN_SPARKS_FRAME   8    /* STATE_BLAST: 3 sparks a frame from here */
#define FX_MADEEN_GLOW_2_FRAME   0x2B /* STATE_BLAST: VIOLET_GLOW; the brightening screen grid from the next frame */

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
    SVECTOR rot90[FX_MADEEN_ARCH_COPIES];
    Pool6CEntry *emitB0;
} Fx17aWork80; /* 0xB4 */

typedef struct {
    u8 pad0[4];
    SVECTOR points4[FX_MADEEN_STREAK_POINTS]; /* points; vy decays */
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
