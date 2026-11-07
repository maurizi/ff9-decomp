#ifndef FX_08E_H
#define FX_08E_H

/* Effect module fx_08e (dir 13 file 0x8E): Curaga, one target (party; enemy Curaga of Dummy/Necron). A morphing
 * model (Hi_DrawMorphEffModel) and texture frames copied by MoveImage. Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_08e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CURAGA_SPARKLES 0x1 /* emitter: tiny rings + white/green sparkle sprites (byte-identical to fx_001 entry 0x1, same render) */
#define FX_CURAGA_WHITE_RINGS    0x3 /* emitter: white gouraud rings */
#define FX_CURAGA_GOLD_RING      0x5 /* emitter: white-gold gouraud rings */
#define FX_CURAGA_STAR_FLARES 0x6 /* emitter: yellow-green and violet star flares, faint glow (byte-identical to fx_001 entry 0x6) */
#define FX_CURAGA_BLUE_SWIRL 0x7 /* emitter: blue swirl-textured trail + green/yellow/white sparkle sprites (byte-identical to fx_001 entry 0x7) */
#define FX_CURAGA_SPARKLE_SHOWER 0xD /* emitter: green/yellow/white sparkle sprites (byte-identical to fx_001 entry 0xD, same render) */
#define FX_CURAGA_LIGHT_FOUNTAIN 0xE /* model: tall funnel with a green light-fountain texture */
#define FX_CURAGA_LIGHT_RING     0xF /* model: ring with a green light-fountain texture */
#define FX_CURAGA_SPARKLE_BURST 0x10 /* emitter: white/yellow/green sparkle sprites (byte-identical to fx_001 entry 0x10, same render) */

/* fx_08e choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, the light fountain's phases,
 * fades and texture animation. */
#define FX_CURAGA_SWIRL_FRAME    6    /* FX_CURAGA_BLUE_SWIRL and FX_CURAGA_GOLD_RING start */
#define FX_CURAGA_FOUNTAIN_FRAME 22   /* WHITE_RINGS and SPARKLE_BURST start; the fountain grows, fades in and its texture animates */
#define FX_CURAGA_SPARKLES_FRAME 28   /* FX_CURAGA_SPARKLES start */
#define FX_CURAGA_FADE_OUT_FRAME 36   /* the fountain starts fading out */
#define FX_CURAGA_SHOWER_FRAME   42   /* FX_CURAGA_SPARKLE_SHOWER starts */
#define FX_CURAGA_FRAMES         0x46 /* op80's length (the op ends) */
#define FX_CURAGA_FADE_IN_FRAMES  0x12 /* fadeIn60: the fountain's brightness rises from -0x80 over this many frames */
#define FX_CURAGA_FADE_OUT_FRAMES 0x18 /* fadeOut64: and falls back over this many */
#define FX_CURAGA_FOUNTAIN_GROW_FRAMES  10 /* FX_CURAGA_LIGHT_FOUNTAIN grows (height) over this many frames, then */
#define FX_CURAGA_FOUNTAIN_MORPH_FRAMES 24 /* morphs to FX_CURAGA_LIGHT_RING over this many */
#define FX_CURAGA_SPARKLES_HEIGHT 0x1C2 /* FX_CURAGA_SPARKLES start this far above the target */
#define FX_CURAGA_MODEL_COUNT     2     /* btlfx_initEffModels slots: LIGHT_FOUNTAIN, LIGHT_RING */
#define FX_CURAGA_BLUE_SWIRL_TRAIL_SLOTS  9    /* FX_CURAGA_BLUE_SWIRL's trail buffer fx_08e_trails: slots of */
#define FX_CURAGA_BLUE_SWIRL_TRAIL_STRIDE 0xA0 /* this many bytes */
/* The fountain models sample (tpage, u / v 0) the rect into which each frame MoveImage copies one of the
 * FX_CURAGA_FOUNTAIN_ANIM_FRAMES texture frames listed in fx_08e_texAnimSrc. */
#define FX_CURAGA_FOUNTAIN_TPAGE  0x1B   /* 4-bit page (0x2C0, 0x100) */
#define FX_CURAGA_FOUNTAIN_CLUT   0x3D09 /* CLUT (0x90, 0xF4) */
#define FX_CURAGA_FOUNTAIN_VRAM_X 0x2C0
#define FX_CURAGA_FOUNTAIN_VRAM_Y 0x100
#define FX_CURAGA_FOUNTAIN_VRAM_W 0x20
#define FX_CURAGA_FOUNTAIN_VRAM_H 0x80
#define FX_CURAGA_FOUNTAIN_ANIM_FRAMES 15

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    SVECTOR rot1C; /* model rotation */
    VECTOR pos24; /* model position */
    VECTOR scale34; /* model scale */
    s32 bright44; /* model brightness */
    SVECTOR pos48; /* target body position */
    SVECTOR pos50;
    SVECTOR pos58; /* caster position */
    s32 fadeIn60; /* fade-in countdown */
    s32 fadeOut64; /* fade-out countdown */
    u16 model68; /* models */
    u16 model6A;
} Fx08eWork; /* 0x6C */

extern Unk800F78A4 *fx_08e_env;
extern s16 fx_08e_texAnimSrc[][2];
extern BtlModelBuf fx_08e_effModels[];
extern s32 fx_08e_trails[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
