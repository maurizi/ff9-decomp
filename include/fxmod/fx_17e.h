#ifndef FX_17E_H
#define FX_17E_H

/* Effect module fx_17e (dir 13 file 0x17e): enemy Chestnut (Mandragora). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_17e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CHESTNUT_IMPACT_RINGS   0x2 /* emitter: white-pink-red-yellow and white-grey gouraud rings + spark sprites (texture not in the file) */
#define FX_CHESTNUT_CHESTNUT_MODEL 0x9 /* 'so' model (not drawn by the tool), probably the thrown chestnut (look pending) */

typedef struct {
    s32 model0; /* model */
    s32 fade4; /* brightness */
    VECTOR scale8; /* model scale */
    VECTOR pos18; /* model position */
    SVECTOR rot28; /* model rotation */
    SVECTOR pos30;
    SVECTOR pos38; /* target body position */
    s32 timer40[4]; /* countdowns */
    s32 timer50;
} Fx17eWork;

extern Unk800F78A4 *fx_17e_env;
extern BtlModelBuf fx_17e_effModels[];

#endif
