#ifndef FX_1FB_H
#define FX_1FB_H

/* Effect module fx_1fb (dir 13 file 0x1fb): Carbuncle (4) (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_1fb_env;
extern SVECTOR fx_1fb_emitPos0;
extern s32 fx_1fb_fadeTimers[2]; /* fade-out / fade-in countdowns */
extern SVECTOR fx_1fb_emitPos1[4]; /* unit body positions */
extern s32 fx_1fb_op81Trails0[4][0x1FE];
extern s32 fx_1fb_op81Trails1[4][0x1FE];

#endif
