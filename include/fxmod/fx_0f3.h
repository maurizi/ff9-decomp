#ifndef FX_0F3_H
#define FX_0F3_H

/* Effect module fx_0f3 (dir 13 file 0x0F3): Scoop Art. A moving point drawn as a strip (btlfx_drawTrail) with an
 * emitter and screen tints, three path variants. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 variant4; /* variant (0..2) */
    SVECTOR pos8; /* target body position */
    SVECTOR headPos10;
    SVECTOR pos18;
    s32 unk20;
    Pool6CEntry *emit24;
} Fx0f3Work;

extern Unk800F78A4 *fx_0f3_env;
extern u8 fx_0f3_strips[][0xEC];
extern u8 fx_0f3_op80Trails[][0x2B0];

#endif
