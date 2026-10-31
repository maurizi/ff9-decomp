#ifndef FX_0A1_H
#define FX_0A1_H

/* Effect module fx_0a1 (dir 13 file 0xA1): enemy Blaster (Torama and others). A btlfx_drawTrail beam from the
 * caster bone to the target, then emitters. Evidence: [data] the party ability / item tables and enemy sequences
 * that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 unk4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* caster position */
    SVECTOR pos18; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos20; /* btlfx_getUnitArgBonePos(0x10, 0), every frame */
    SVECTOR aim28; /* angles from pos8 to pos18 */
} Fx0a1Work;

extern Unk800F78A4 *fx_0a1_env;
extern u8 fx_0a1_trail[];
extern s32 fx_0a1_trails[];
extern s32 fx_0a1_trails2[];

#endif
