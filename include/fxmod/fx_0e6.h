#ifndef FX_0E6_H
#define FX_0E6_H

/* Effect module fx_0e6 (dir 13 file 0x0E6): enemy Stone (Mu). A model and a 25-point strip (btlfx_drawTrail) flying
 * from the caster to the target. Evidence: [data] the party ability / item tables and enemy sequences that play
 * this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 model0; /* model */
    VECTOR scale4; /* scale */
    VECTOR pos14; /* position */
    SVECTOR rot24; /* rotation */
    SVECTOR pos2C;
    SVECTOR pos34;
} Fx0e6Work;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[25]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0e6Line;

extern Unk800F78A4 *fx_0e6_env;
extern BtlModelBuf fx_0e6_effModels[];
extern Fx0e6Line fx_0e6_strip;

#endif
