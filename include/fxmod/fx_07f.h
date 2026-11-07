#ifndef FX_07F_H
#define FX_07F_H

/* Effect module fx_07f (dir 13 file 0x7F): Silence. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_07f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SILENCE_VIOLET_RING 0x1 /* model: disc on a blue-magenta haze page at run time (the file's scribble page is replaced): the glowing violet ring round each target; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_SILENCE_BACKDROP   0x2 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_SILENCE_KANJI      0x3 /* emitter: the kanji "silence" glyph, glowing orange to violet */
#define FX_SILENCE_KANJI_DOME 0x5 /* model: dome textured with the "silence" glyph */
#define FX_SILENCE_PINK_PUFF  0x9 /* emitter: pink-white puff */
#define FX_SILENCE_PINK_RING  0xC /* emitter: white-pink gouraud rings */

/* fx_07f choreography (docs/fxdata.md, "Choreography literals"): op80's state, length and keyframes, and the
 * model textures. */
#define FX_SILENCE_STATE_DOME   0    /* op80's only state: the dome and ring grow and fade */
#define FX_SILENCE_FRAMES       0x29 /* FX_SILENCE_STATE_DOME's length (the op ends); the grow / fade ramp runs over it */
#define FX_SILENCE_TURN_FRAMES  0xC  /* FX_SILENCE_KANJI_DOME turns a quarter round over this many frames, then */
#define FX_SILENCE_KANJI_FRAME  FX_SILENCE_TURN_FRAMES /* FX_SILENCE_KANJI starts */
#define FX_SILENCE_KANJI_HEIGHT 0x180 /* FX_SILENCE_KANJI starts this far above the targets' centre */
#define FX_SILENCE_MODEL_COUNT  2    /* btlfx_initEffModels slots: KANJI_DOME, VIOLET_RING */
#define FX_SILENCE_KANJI_DOME_TPAGE  0xB7   /* 8-bit page (0x1C0, 0x100), abr 1 */
#define FX_SILENCE_KANJI_DOME_CLUT   0x3D40 /* CLUT (0, 0xF5) */
#define FX_SILENCE_VIOLET_RING_TPAGE 0x139  /* 15-bit page (0x240, 0x100), abr 1 */
#define FX_SILENCE_VIOLET_RING_CLUT  0xFFFF /* none (15-bit page) */
#define FX_SILENCE_VIOLET_RING_U     0x40
#define FX_SILENCE_VIOLET_RING_V     0x80

typedef struct {
    s32 state0; /* state */
    s32 bgFrame4; /* frame for btlfx_drawLayerTexture */
    SVECTOR pos8; /* position: env->targets->centerX */
    SVECTOR pos10; /* pos8 raised by 0x180 */
    s32 modelPos18[3]; /* model position (VECTOR without its pad word) */
    SVECTOR rot24; /* model rotation */
    s32 model2C; /* model */
    s32 model30; /* model */
} Fx07fWork;

extern Unk800F78A4 *fx_07f_env;
extern BtlModelBuf fx_07f_effModels[];

#endif
