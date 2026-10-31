#ifndef FX_1F6_H
#define FX_1F6_H

/* Effect module fx_1f6 (dir 13 file 0x1f6): enemy Aera (Griffin, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 model0; /* model */
    SVECTOR rot4; /* rotation */
    VECTOR posC; /* position */
    VECTOR scale1C; /* scale */
} Fx1f6Model; /* 0x2C */

typedef struct {
    s32 timer0[8]; /* countdowns */
    Fx1f6Model models20[3];
    SVECTOR posA4; /* target position */
    SVECTOR posAC; /* target body position */
} Fx1f6Work; /* 0xB4 */

extern Unk800F78A4 *fx_1f6_env;
extern BtlModel fx_1f6_effModels[];
extern s32 fx_1f6_op80Trails0[];
extern s32 fx_1f6_op80Trails1[];

#endif
