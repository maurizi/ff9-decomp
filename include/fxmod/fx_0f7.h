#ifndef FX_0F7_H
#define FX_0F7_H

/* Effect module fx_0f7 (dir 13 file 0x0F7): Solution 9. Emitters and layer textures at the target, with four screen
 * sprites (fx_0f7_sparks). Evidence: [data] the party ability / item tables and enemy sequences that play this file
 * id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f7 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SOLUTION_9_BACKDROP_2 0x2 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_SOLUTION_9_BACKDROP   0x4 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_SOLUTION_9_RUNES      0x5 /* sprite: red glowing rune glyphs (large) */
#define FX_SOLUTION_9_BLUE_RINGS 0xB /* emitter: white-blue gouraud rings + textured rings (texture not resolved) */
#define FX_SOLUTION_9_RUNES_2    0xC /* sprite: red glowing rune glyphs (small) */
#define FX_SOLUTION_9_RUNE_RINGS 0xE /* emitter: white-blue gouraud rings + red rune particles, fan elements */
#define FX_SOLUTION_9_HALO_RINGS 0xF /* emitter: white-blue gouraud rings + dark ring with a rainbow rim */

/* fx_0f7 choreography (docs/fxdata.md, "Choreography literals"): op80's emitter keys and the rune sprites' timing. */
#define FX_SOLUTION_9_RUNES_FRAMES      0x30 /* the rune sprites (fx_0f7_sparks) drift for this many frames (timer0) from frame 0, when RUNE_RINGS start */
#define FX_SOLUTION_9_RUNE_FADE_FRAMES  8    /* each rune fades in over its first, and all out over the last, this many frames (the >> 3) */
#define FX_SOLUTION_9_RUNE_COUNT        4    /* fx_0f7_sparks */
#define FX_SOLUTION_9_HALO_RINGS_FRAME  0x20 /* HALO_RINGS start */
#define FX_SOLUTION_9_BLUE_RINGS_FRAME  0x28 /* RUNE_RINGS stop and BLUE_RINGS start (BACKDROP_2 replaces BACKDROP while each runs) */
#define FX_SOLUTION_9_FRAMES            0x48 /* op80's length (the op ends) */

typedef struct {
    s32 timer0; /* countdown */
    SVECTOR pos4; /* target body position */
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
} Fx0f7Work; /* 0x18 */

typedef struct {
    s32 delay0; /* delay */
    s32 kind4; /* kind */
    s32 ot8;
    s32 alphaC; /* alpha */
    s32 x10;
    s32 y14;
    s32 speedX18;
    s32 age1C; /* age */
} Fx0f7Spark;

extern Fx0f7Spark fx_0f7_sparks[FX_SOLUTION_9_RUNE_COUNT];
extern Unk800F78A4 *fx_0f7_env;

#endif
