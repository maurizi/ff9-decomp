#ifndef FX_090_H
#define FX_090_H

/* Effect module fx_090 (dir 13 file 0x90): Curaga, all targets (party; enemy Curaga of Hilgigars and others). A
 * morphing model (Hi_DrawMorphEffModel) and texture frames copied by MoveImage. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_090 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CURAGA_STAR_FLARES 0x2 /* emitter: white star glints + faint coloured arcs */
#define FX_CURAGA_SPARKLES 0x3 /* emitter: green and gold twinkling glints spreading out; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_CURAGA_BLUE_SWIRL 0x5 /* emitter: blue starry trail + sparkles */
#define FX_CURAGA_LIGHT_FOUNTAIN 0xB /* model: tall funnel with a cyan light-fountain texture */
#define FX_CURAGA_WHITE_RINGS    0xC /* emitter: white gouraud rings */
#define FX_CURAGA_LIGHT_RING     0xD /* model: ring with a cyan light-fountain texture */
#define FX_CURAGA_TEAL_RING      0xE /* emitter: white-teal gouraud rings */
#define FX_CURAGA_SPARKLE_BURST 0xF /* emitter: gold-white four-point star sparkles in a rising column; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_CURAGA_SPARKLE_SHOWER 0x10 /* emitter: green four-point star twinkles; runtime (docs/runtime-findings.md effect-data pass) */

/* fx_090 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, the light fountain's phases,
 * fades and texture animation (fx_08e's names, all-targets values). */
#define FX_CURAGA_FOUNTAIN_FRAME 18   /* WHITE_RINGS and SPARKLE_BURST start; the fountain grows, fades in and its texture animates */
#define FX_CURAGA_SPARKLES_FRAME 22   /* FX_CURAGA_SPARKLES and FX_CURAGA_STAR_FLARES start */
#define FX_CURAGA_FADE_OUT_FRAME 28   /* the fountain starts fading out */
#define FX_CURAGA_SHOWER_FRAME   39   /* FX_CURAGA_SPARKLE_SHOWER starts */
#define FX_CURAGA_FRAMES         0x46 /* op80's length (the op ends) */
#define FX_CURAGA_FADE_IN_FRAMES  0x12 /* fadeIn68: the fountain's brightness rises from -0x80 over this many frames */
#define FX_CURAGA_FADE_OUT_FRAMES 0x1A /* fadeOut6C: and falls back over this many */
#define FX_CURAGA_FOUNTAIN_GROW_FRAMES  6  /* FX_CURAGA_LIGHT_FOUNTAIN grows (height) over this many frames, then */
#define FX_CURAGA_FOUNTAIN_MORPH_FRAMES 26 /* morphs to FX_CURAGA_LIGHT_RING over this many */
#define FX_CURAGA_SPARKLES_HEIGHT 0x320 /* SPARKLES, STAR_FLARES and SPARKLE_SHOWER start this far above the targets' centre */
#define FX_CURAGA_MODEL_COUNT     2     /* btlfx_initEffModels slots: LIGHT_FOUNTAIN, LIGHT_RING */
#define FX_CURAGA_BLUE_SWIRL_TRAIL_SLOTS  0xC  /* FX_CURAGA_BLUE_SWIRL's trail buffer fx_090_trails: slots of */
#define FX_CURAGA_BLUE_SWIRL_TRAIL_STRIDE 0xA0 /* this many bytes */
/* The fountain models sample (tpage, u / v 0) the rect into which each frame MoveImage copies one of the
 * FX_CURAGA_FOUNTAIN_ANIM_FRAMES texture frames listed in fx_090_texAnimSrc. */
#define FX_CURAGA_FOUNTAIN_TPAGE  0x1B   /* 4-bit page (0x2C0, 0x100) */
#define FX_CURAGA_FOUNTAIN_CLUT   0x3D09 /* CLUT (0x90, 0xF4) */
#define FX_CURAGA_FOUNTAIN_VRAM_X 0x2C0
#define FX_CURAGA_FOUNTAIN_VRAM_Y 0x100
#define FX_CURAGA_FOUNTAIN_VRAM_W 0x20
#define FX_CURAGA_FOUNTAIN_VRAM_H 0x80
#define FX_CURAGA_FOUNTAIN_ANIM_FRAMES 15

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4; /* unused */
    s32 unk8; /* unused */
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
    SVECTOR rot24; /* model rotation */
    VECTOR pos2C; /* model position */
    VECTOR scale3C; /* model scale */
    s32 bright4C; /* model brightness */
    SVECTOR pos50; /* position: env->targets->centerX..Z */
    SVECTOR pos58; /* pos50 raised by 0x320 */
    SVECTOR pos60; /* pos50 raised by 0x4B0 */
    s32 fadeIn68; /* fade-in countdown */
    s32 fadeOut6C; /* fade-out countdown */
    u16 model70; /* models */
    u16 model72;
} Fx090Work; /* 0x74 */

extern s16 fx_090_texAnimSrc[][2];
extern Unk800F78A4 *fx_090_env;
extern BtlModelBuf fx_090_effModels[];
extern s32 fx_090_trails[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
