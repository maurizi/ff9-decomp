#ifndef FX_0CE_H
#define FX_0CE_H

/* Effect module fx_0ce (dir 13 file 0xCE): enemy Screech, Shriek (Gimme Cat, ...). Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* caster body position, raised by 0x80 */
    SVECTOR pos10; /* caster position */
    SVECTOR pos18; /* btlfx_getUnitArgBonePos(0x10, 0) */
    s32 model20; /* model */
    SVECTOR rot24; /* angles from pos18 to pos8 */
    Pool6CEntry *emit2C;
} Fx0ceWork;

extern Unk800F78A4 *fx_0ce_env;
extern BtlModel fx_0ce_effModels[];
extern s32 fx_0ce_trails[];

#endif
