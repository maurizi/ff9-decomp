#ifndef FX_028_H
#define FX_028_H

/* Effect module fx_028 (dir 13 file 0x28). */
#include "btlfx_mod.h"

/* fx_028 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ELIXIR_SPARKLES      0x2 /* emitter: white / gold sparkle shards */
#define FX_ELIXIR_GREEN_GLOW    0x3 /* emitter: violet / green glows, green star glints, green light pillar, pink glints */
#define FX_ELIXIR_BUBBLES       0x4 /* emitter: green / pink star sparks + green, white, yellow, pink bubble sprites */
#define FX_ELIXIR_GREEN_BLOSSOM 0x5 /* emitter: green flower-shaped glow + green specks */
#define FX_ELIXIR_PINK_BLOSSOM  0x6 /* emitter: pink flower-shaped glow + bubble specks */
#define FX_ELIXIR_SPARKLE_RINGS 0x7 /* emitter: rings of red / green dots + green-pink crosses */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* target body position */
} Fx028Work;

extern Unk800F78A4 *fx_028_env;

#endif
