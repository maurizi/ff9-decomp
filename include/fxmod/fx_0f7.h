#ifndef FX_0F7_H
#define FX_0F7_H

/* Effect module fx_0f7 (dir 13 file 0x0F7): Solution 9. Emitters and backdrops at the target, with four screen
 * sprites (fx_0f7_sparks). Evidence: [data] the party ability / item tables and enemy sequences that play this file
 * id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f7 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SOLUTION_9_BACKDROP_2 0x2 /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_SOLUTION_9_BACKDROP   0x4 /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_SOLUTION_9_RUNES      0x5 /* sprite: red glowing rune glyphs (large) */
#define FX_SOLUTION_9_BLUE_RINGS 0xB /* emitter: white-blue gouraud rings + textured rings (texture not resolved) */
#define FX_SOLUTION_9_RUNES_2    0xC /* sprite: red glowing rune glyphs (small) */
#define FX_SOLUTION_9_RUNE_RINGS 0xE /* emitter: white-blue gouraud rings + red rune particles, fan elements */
#define FX_SOLUTION_9_HALO_RINGS 0xF /* emitter: white-blue gouraud rings + dark ring with a rainbow rim */

typedef struct {
    s32 timer0; /* countdown */
    SVECTOR pos4; /* target body position */
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
} Fx0f7Work;

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

extern Fx0f7Spark fx_0f7_sparks[4];
extern Unk800F78A4 *fx_0f7_env;

#endif
