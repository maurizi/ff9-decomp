#ifndef FX_198_H
#define FX_198_H

/* Effect module fx_198 (dir 13 file 0x198): enemy Tsunami (Black Waltz 1/Sealion). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    s32 scale8; /* scale */
    SVECTOR posC; /* caster position */
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s32 model1C; /* model */
    s32 unk20;
} Fx198Work; /* 0x24 */

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} Fx198Key;

extern s16 fx_198_quadPathFileIds[0x11]; /* file ids */
extern Unk800F78A4 *fx_198_env;
extern BtlModelBuf fx_198_effModels[];

#endif
