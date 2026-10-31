#ifndef FX_0F7_H
#define FX_0F7_H

/* Effect module fx_0f7 (dir 13 file 0x0F7): Solution 9. Emitters and backdrops at the target, with four screen
 * sprites (fx_0f7_sparks). Evidence: [data] the party ability / item tables and enemy sequences that play this file
 * id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

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
