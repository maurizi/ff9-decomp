#ifndef FX_0D1_H
#define FX_0D1_H

/* Effect module fx_0d1 (dir 13 file 0xD1): Mental Break. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0d1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MENTAL_BREAK_PINK_GLOW  0x4 /* emitter: pink glow sprite with red ember sprites */
#define FX_MENTAL_BREAK_RINGS      0x7 /* emitter: white-violet, white-pink and pink-purple gouraud rings */
#define FX_MENTAL_BREAK_GLOW_CROSS 0x9 /* emitter: four pink glows in a cross, orange / blue ring */
#define FX_MENTAL_BREAK_ENERGY_ORB 0xD /* emitter: pink energy orb particle */
#define FX_MENTAL_BREAK_SPARKLES   0xE /* emitter: pink star sparkle sprites, fan */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* unit 0x10 body position */
    SVECTOR pos1C; /* caster body position */
} Fx0d1Work;

extern Unk800F78A4 *fx_0d1_env;

#endif
