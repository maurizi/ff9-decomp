#ifndef FX_0DA_H
#define FX_0DA_H

/* Effect module fx_0da (dir 13 file 0xDA): Sword Magic Blizzard. Evidence: [data] the party ability / item tables
 * and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0daWork84;

extern Unk800F78A4 *fx_0da_env;
extern BtlModel fx_0da_effModels[];

typedef struct {
    s32 unk0; /* state */
    s32 model4; /* models */
    s32 model8;
    s32 modelC;
    s32 model10;
    SVECTOR pos14; /* caster body position */
    SVECTOR pos1C; /* caster position */
    s32 unk24; /* frame counter */
} Fx0daWork80;

#endif
