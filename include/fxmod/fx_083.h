#ifndef FX_083_H
#define FX_083_H

/* Effect module fx_083 (dir 13 file 0x83): Scan (party and enemy). Four orbiting ring models, pillar models and
 * scrolling texture frames (btlfx_drawScrollMove). Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_083 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SCAN_RED_BAND     0x1 /* model: ring band, red texture */
#define FX_SCAN_GLYPH_TEXT   0x2 /* emitter: scrolling coloured glyph text */
#define FX_SCAN_METAL_SPHERE 0x5 /* model: sphere with a riveted metal texture */
#define FX_SCAN_GLYPH_TEXT_2 0x7 /* emitter: scrolling coloured glyph text, orbiting */
#define FX_SCAN_ORB_TILES    0xB /* emitter: orange / blue orbs + yellow and blue tiles */
#define FX_SCAN_YELLOW_RING  0xC /* emitter: dark yellow gouraud rings */

/* fx_083 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and phase timers, the ring and
 * pillar counts, the model textures and the VRAM rects they sample. */
#define FX_SCAN_TIMERS        10 /* work->timers0: per-phase countdowns, set on keyframes */
#define FX_SCAN_T_RINGS_OPEN  0  /* the rings' radius offset eases in */
#define FX_SCAN_T_RINGS_CLOSE 1  /* and back out */
#define FX_SCAN_T_RINGS       2  /* the METAL_SPHERE rings are drawn */
#define FX_SCAN_T_DESCEND     3  /* the rings move from the unit's height to the target's centre */
#define FX_SCAN_T_GLYPH_ANIM  4  /* the sphere texture's first animation (fx_083_texAnimSrc1) */
#define FX_SCAN_T_GLYPH_FLICKER 5 /* a random flicker (fx_083_texAnimSrc2) */
#define FX_SCAN_T_PILLARS     6  /* the RED_BAND pillars are drawn */
#define FX_SCAN_T_PILLAR_SPAWN 7 /* a pillar is added every 8 frames while it runs */
#define FX_SCAN_T_GLYPHS      8  /* the four GLYPH_TEXT emitters orbit with the rings */
#define FX_SCAN_PILLARS_FRAME    0x8  /* FX_SCAN_T_PILLARS starts (fading in) */
#define FX_SCAN_GLYPH_ANIM_FRAME 0x10 /* FX_SCAN_T_GLYPH_ANIM starts */
#define FX_SCAN_DESCEND_FRAME    0x14 /* the rings descend, the pillars build up, the four FX_SCAN_GLYPH_TEXT start */
#define FX_SCAN_ORBS_FRAME       0x30 /* ORB_TILES, YELLOW_RING and GLYPH_TEXT_2 start */
#define FX_SCAN_CLOSE_FRAME      0x34 /* the rings close */
#define FX_SCAN_PILLAR_FADE_FRAME 0x37 /* the pillars fade out one per frame, the top one (4) first */
#define FX_SCAN_FRAMES           0x5C /* op80's length (the op ends) */
#define FX_SCAN_RINGS_OPEN_FRAMES 0x10 /* FX_SCAN_T_RINGS_OPEN / _CLOSE's length */
#define FX_SCAN_RINGS_FRAMES     0x50 /* FX_SCAN_T_RINGS' length */
#define FX_SCAN_DESCEND_FRAMES   0x20 /* FX_SCAN_T_DESCEND / _PILLAR_SPAWN's length */
#define FX_SCAN_GLYPH_ANIM_FRAMES 8   /* FX_SCAN_T_GLYPH_ANIM's length */
#define FX_SCAN_FLICKER_FRAMES   4    /* FX_SCAN_T_GLYPH_FLICKER's length */
#define FX_SCAN_PILLARS_FRAMES   0x3C /* FX_SCAN_T_PILLARS' length */
#define FX_SCAN_GLYPHS_FRAMES    0x30 /* FX_SCAN_T_GLYPHS' length */
#define FX_SCAN_PILLAR_FADE_IN_FRAMES 0x10 /* fade13C: the first pillar fades in over this many frames */
#define FX_SCAN_PILLAR_FADE_FRAMES 8  /* pillarFadeD8: each pillar fades out over this many */
#define FX_SCAN_RINGS            4    /* METAL_SPHERE rings (and glyph emitters) round the target */
#define FX_SCAN_PILLARS          5    /* RED_BAND pillar segments */
#define FX_SCAN_MODEL_COUNT      0x14 /* btlfx_initEffModels slots */
#define FX_SCAN_RING_SCALE       0x333 /* the rings' scale (0x1000 = 1) */
/* FX_SCAN_METAL_SPHERE samples (SPHERE_TPAGE + u / v 0x80) the rect the glyph animations copy texture frames into */
#define FX_SCAN_SPHERE_TPAGE 0xFA   /* 8-bit page (0x280, 0x100), abr 3 */
#define FX_SCAN_SPHERE_CLUT  0x3DC0 /* CLUT (0, 0xF7) */
#define FX_SCAN_SPHERE_U     0x80
#define FX_SCAN_SPHERE_V     0x80
#define FX_SCAN_GLYPH_VRAM_X 0x2C0
#define FX_SCAN_GLYPH_VRAM_Y 0x180
#define FX_SCAN_GLYPH_VRAM_W 0x40
#define FX_SCAN_GLYPH_VRAM_H 0x40
/* FX_SCAN_RED_BAND samples (BAND_TPAGE + u 0xC0) the rect each frame's btlfx_drawScrollMove fills from (SRC_X, SRC_Y),
 * scrolled vertically */
#define FX_SCAN_BAND_TPAGE  0xB8   /* 8-bit page (0x200, 0x100), abr 1 */
#define FX_SCAN_BAND_CLUT   0x3D40 /* CLUT (0, 0xF5) */
#define FX_SCAN_BAND_U      0xC0
#define FX_SCAN_BAND_VRAM_X 0x260
#define FX_SCAN_BAND_VRAM_Y 0x100
#define FX_SCAN_BAND_VRAM_W 0x20
#define FX_SCAN_BAND_VRAM_H 0xF0
#define FX_SCAN_BAND_SRC_X  0x240
#define FX_SCAN_BAND_SRC_Y  0x100

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx083Vec3;

typedef struct {
    s32 timers0[FX_SCAN_TIMERS]; /* countdowns */
    s32 model28[FX_SCAN_RINGS]; /* ring models */
    SVECTOR ringRot38[FX_SCAN_RINGS]; /* ring rotations */
    VECTOR ringPos58[FX_SCAN_RINGS]; /* ring positions */
    Fx083Vec3 ringScale98; /* ring scale */
    s32 unkA4; /* unused */
    s32 ringOffsetA8; /* ring radius offset */
    s32 ringHeightAC; /* ring height */
    s32 modelB0[FX_SCAN_PILLARS]; /* pillar models */
    s32 pillarOnC4[FX_SCAN_PILLARS]; /* pillar active */
    s32 pillarFadeD8[FX_SCAN_PILLARS]; /* pillar fade */
    VECTOR pillarPosEC[FX_SCAN_PILLARS]; /* pillar positions */
    s32 fade13C; /* fade */
    SVECTOR rot140; /* pillar rotation */
    Fx083Vec3 pillarScale148; /* pillar scale */
    s32 unk154; /* unused */
    SVECTOR center158; /* center */
    SVECTOR pos160; /* caster position */
    s32 radius168; /* radius */
    s32 height16C; /* height */
    Pool6CEntry *emit170[FX_SCAN_RINGS];
    SVECTOR emitStartPos180[FX_SCAN_RINGS];
    SVECTOR emitPos1A0[FX_SCAN_RINGS];
    s32 orbit1C0; /* orbit angle */
} Fx083Work; /* 0x1C4 */

typedef struct {
    s32 x0;
    s32 y4;
} Fx083Pair;

extern Fx083Pair fx_083_texAnimSrc1[];
extern Fx083Pair fx_083_texAnimSrc2[];
extern Unk800F78A4 *fx_083_env;
extern BtlModelBuf fx_083_effModels[];

#endif
