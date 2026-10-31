#ifndef FX_0A5_H
#define FX_0A5_H

/* Effect module fx_0a5 (dir 13 file 0xA5): enemy Spore (Myconid). Emitters, one moved across the targets.
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR aim10; /* angles from pos8 to env->targets->centerX */
    SVECTOR pos18; /* lerp start: env position at x = 0x5DC */
    SVECTOR pos20; /* lerp end: env position at x = -0x5DC */
    SVECTOR pos28; /* lerp output */
} Fx0a5Work;

extern Unk800F78A4 *fx_0a5_env;
extern s32 fx_0a5_trails[];

#endif
