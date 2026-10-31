#ifndef FX_12C_H
#define FX_12C_H

/* Effect module fx_12c (dir 13 file 0x12C): enemy Earthquake, Power of the Land (Catoblepas, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0[8]; /* countdowns */
    SVECTOR pos20; /* effect position */
    SVECTOR pos28; /* effect position */
} Fx12cWork; /* 0x30 */

typedef struct {
    s32 unk0; /* model */
    VECTOR unk4; /* scale (vy: bounce height) */
    VECTOR unk14; /* position */
    SVECTOR unk24; /* rotation */
    s32 unk2C; /* bounce speed */
    s32 unk30; /* bounce time */
} Fx12cRock; /* 0x34 */

extern Unk800F78A4 *fx_12c_env;
extern Fx12cRock D_801E7F08[10];
extern s32 D_801E8110; /* bounce strength */
extern BtlModel fx_12c_effModels[];
extern u32 D_801E88E8[];

s32 fx_12c_op80(s32 mode, Fx12cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
