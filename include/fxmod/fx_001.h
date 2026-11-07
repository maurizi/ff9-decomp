#ifndef FX_001_H
#define FX_001_H

/* Effect module fx_001 (dir 13 file 0x01). */
#include "btlfx_mod.h"

/* fx_001 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DAGGERS_MAGIC_SPARKLES       0x1 /* emitter: tiny rings + white/green sparkle sprites (at pos54) */
#define FX_DAGGERS_MAGIC_WHITE_RINGS    0x3 /* emitter: white gouraud rings */
#define FX_DAGGERS_MAGIC_GOLD_RINGS     0x5 /* emitter: white-gold gouraud rings */
#define FX_DAGGERS_MAGIC_STAR_FLARES    0x6 /* emitter: yellow-green and violet star flares, faint glow */
#define FX_DAGGERS_MAGIC_BLUE_SWIRL     0x7 /* emitter: blue swirl-textured trail + green/yellow/white sparkle sprites */
#define FX_DAGGERS_MAGIC_SPARKLE_SHOWER 0xD /* emitter: green/yellow/white sparkle sprites (at pos4C) */
#define FX_DAGGERS_MAGIC_LIGHT_FOUNTAIN 0xE /* model: tall spiky funnel with a green light-fountain texture (byte-identical to fx_08e entry 0xE) */
#define FX_DAGGERS_MAGIC_LIGHT_RING     0xF /* model: ring, green light-arc texture */
#define FX_DAGGERS_MAGIC_SPARKLE_BURST  0x10 /* emitter: white/yellow/green sparkle sprites */

/* fx_001 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, the fountain's grow / morph
 * phases and fades, and its scrolling texture. */
#define FX_DAGGERS_MAGIC_BLUE_SWIRL_FRAME      6    /* op80: FX_DAGGERS_MAGIC_BLUE_SWIRL and FX_DAGGERS_MAGIC_GOLD_RINGS start */
#define FX_DAGGERS_MAGIC_FOUNTAIN_FRAME        22   /* WHITE_RINGS / SPARKLE_BURST start, the fountain fades in and grows, its texture animates */
#define FX_DAGGERS_MAGIC_FOUNTAIN_GROW_FRAMES  10   /* FX_DAGGERS_MAGIC_LIGHT_FOUNTAIN grows (scale vy 0 -> 0x800) over this many frames, then */
#define FX_DAGGERS_MAGIC_RING_MORPH_FRAMES     24   /* morphs into FX_DAGGERS_MAGIC_LIGHT_RING over this many */
#define FX_DAGGERS_MAGIC_SPARKLES_FRAME        28   /* FX_DAGGERS_MAGIC_SPARKLES start */
#define FX_DAGGERS_MAGIC_FADE_OUT_FRAME        36   /* the fountain starts fading out */
#define FX_DAGGERS_MAGIC_SPARKLE_SHOWER_FRAME  42   /* FX_DAGGERS_MAGIC_SPARKLE_SHOWER starts */
#define FX_DAGGERS_MAGIC_FRAMES                0x46 /* op80: the task ends at this frame */
#define FX_DAGGERS_MAGIC_FADE_IN_FRAMES        0x12 /* fadeIn64: model brightness -0x80 -> 0 over this many frames */
#define FX_DAGGERS_MAGIC_FADE_OUT_FRAMES       0x18 /* fadeOut68: 0 -> -0x80 over this many */
#define FX_DAGGERS_MAGIC_SPARKLES_HEIGHT       0x1C2 /* FX_DAGGERS_MAGIC_SPARKLES' position (pos54) above the target */
#define FX_DAGGERS_MAGIC_MODEL_COUNT           2    /* btlfx_initEffModels slots: LIGHT_FOUNTAIN, LIGHT_RING */
#define FX_DAGGERS_MAGIC_BLUE_SWIRL_TRAIL_SLOTS  9    /* FX_DAGGERS_MAGIC_BLUE_SWIRL's trail buffer fx_001_trails: slots of */
#define FX_DAGGERS_MAGIC_BLUE_SWIRL_TRAIL_STRIDE 0xA0 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
/* LIGHT_FOUNTAIN and LIGHT_RING share one texture: 4-bit page (0x2C0, 0x100), clut (0x90, 0xF4); from
 * FOUNTAIN_FRAME on, each frame MoveImage copies one of the LIGHT_ANIM_FRAMES rects of fx_001_texAnimXY into the
 * page at (VRAM_X, VRAM_Y), animating the light. */
#define FX_DAGGERS_MAGIC_LIGHT_TPAGE           0x1B
#define FX_DAGGERS_MAGIC_LIGHT_CLUT            0x3D09
#define FX_DAGGERS_MAGIC_LIGHT_VRAM_X          0x2C0
#define FX_DAGGERS_MAGIC_LIGHT_VRAM_Y          0x100
#define FX_DAGGERS_MAGIC_LIGHT_VRAM_W          0x20
#define FX_DAGGERS_MAGIC_LIGHT_VRAM_H          0x80
#define FX_DAGGERS_MAGIC_LIGHT_ANIM_FRAMES     15   /* fx_001_texAnimXY entries, cycled frame % this */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    s32 unk8; /* unused */
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR rot20; /* model rotation */
    VECTOR pos28; /* model position */
    VECTOR scale38; /* model scale */
    s32 rgb48; /* model brightness */
    SVECTOR pos4C; /* target body position */
    SVECTOR pos54;
    SVECTOR pos5C; /* caster position */
    s32 fadeIn64; /* fade-in countdown */
    s32 fadeOut68; /* fade-out countdown */
    u16 model6C; /* models */
    u16 model6E;
} Fx001Work; /* 0x70 */

extern Unk800F78A4 *fx_001_env;
extern s16 fx_001_texAnimXY[][2];
extern BtlModelBuf fx_001_effModels[];
extern s32 fx_001_trails[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
