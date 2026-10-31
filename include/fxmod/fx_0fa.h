#ifndef FX_0FA_H
#define FX_0FA_H

/* Effect module fx_0fa (dir 13 file 0x0FA): Detect. A model that rises from the caster and turns to the target,
 * with scrolling textures (btlfx_drawScrollMove). Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

static void fxm_easeBlend4(s32 mode, s32 len, s32 t, SVECTOR *ctrl, SVECTOR *out);

typedef struct {
    s32 unk0[10]; /* countdowns and flags */
    SVECTOR homePos28; /* model home position */
    SVECTOR pos30; /* target position */
    s32 model38; /* model */
    SVECTOR rot3C; /* model rotation */
    SVECTOR pos44; /* model position */
    VECTOR pos4C; /* model position */
    VECTOR scale5C; /* model scale */
    SVECTOR aimRot6C; /* angles towards the model */
    Pool6CEntry *emit74;
} Fx0faWork; /* 0x78 */

typedef struct {
    s32 srcX0;
    s32 srcY4;
} Fx0faFlash;

extern Fx0faFlash fx_0fa_scrollSrc0[];
extern Fx0faFlash fx_0fa_scrollSrc1[];
extern Unk800F78A4 *fx_0fa_env;
extern BtlModel fx_0fa_effModels[];

s32 fx_0fa_op80(s32 mode, Fx0faWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
