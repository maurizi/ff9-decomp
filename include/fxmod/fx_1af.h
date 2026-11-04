#ifndef FX_1AF_H
#define FX_1AF_H

/* Effect module fx_1af (dir 13 file 0x1af): enemy Death (Valia Pira). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1af effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH_SPECKS       0x2 /* emitter: faint grey speck sprites, 32 frames */
#define FX_DEATH_ASH_COLUMN   0x3 /* emitter: column of dark olive ash flakes + dark smoke puffs */
#define FX_DEATH_ASH_COLUMN_2 0x4 /* emitter: column of dark olive ash flakes + dark smoke puffs */
#define FX_DEATH_ASH_COLUMN_3 0x5 /* emitter: column of dark olive ash flakes + dark smoke puffs */
#define FX_DEATH_ASH_COLUMN_4 0x6 /* emitter: column of dark olive ash flakes + dark smoke puffs */
#define FX_DEATH_DARK_SMOKE   0x7 /* emitter: dark smoke puffs, 16 frames */
#define FX_DEATH_SPECKS_2     0x8 /* emitter: faint grey speck sprites, 32 frames */
#define FX_DEATH_OLIVE_RINGS  0x9 /* emitter: pale yellow-olive gouraud rings */
#define FX_DEATH_SMOKE        0xA /* emitter: grey-tan smoke puffs, 16 frames */
#define FX_DEATH_PINK_RINGS   0xD /* emitter: white-pink-magenta gouraud rings + 3D trail of white smoke streaks */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* counter */
    s32 pos8[3]; /* position */
    SVECTOR rot14; /* rotation */
    s32 scale1C[3]; /* scale */
    SVECTOR pos28;
    SVECTOR pos30;
    SVECTOR pos38;
    s32 model40[5]; /* models */
} Fx1afWork; /* 0x54 */

extern s16 fx_1af_modelFileIds[5]; /* model files */
extern Unk800F78A4 *fx_1af_env;
extern Unk80100C78Entry *fx_1af_task;
extern BtlModelBuf fx_1af_effModels[];
extern s32 fx_1af_op80Trails[];

#endif
