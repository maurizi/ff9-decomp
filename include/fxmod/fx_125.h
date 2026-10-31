#ifndef FX_125_H
#define FX_125_H

/* Effect module fx_125 (dir 13 file 0x125): Poly; enemy Poly (Benero/King Leo/Zenero). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    SVECTOR pos4; /* target position */
    SVECTOR posC;
    SVECTOR pos14; /* target body position */
    SVECTOR pos1C;
    s32 model24; /* models */
    s32 model28[2];
} Fx125Work;

extern Unk800F78A4 *fx_125_env;
extern BtlModelBuf fx_125_effModels[];

#endif
