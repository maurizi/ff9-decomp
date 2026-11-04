#ifndef FX_1F4_H
#define FX_1F4_H

/* Effect module fx_1f4 (dir 13 file 0x1f4): Vanish; enemy Vanish (Drakan, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1f4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_VANISH_WHITE_STREAK_2 0x1 /* emitter: orbiting screen trail with a white light-streak texture */
#define FX_VANISH_WHITE_STREAK   0x2 /* emitter: orbiting sprite (blank) + screen trail with a white light-streak texture */
#define FX_VANISH_GOLD_GLOW      0x4 /* emitter: golden glow sprite + pale yellow gouraud rings */

typedef struct {
    s32 fadeOut0; /* fade-out countdown */
    s32 fadeIn4; /* fade-in countdown */
    SVECTOR pos8;
} Fx1f4Work;

extern Unk800F78A4 *fx_1f4_env;
extern s32 fx_1f4_op80Trails0[];
extern s32 fx_1f4_op80Trails1[];

#endif
