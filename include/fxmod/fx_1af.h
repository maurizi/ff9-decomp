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

/* fx_1af choreography (docs/fxdata.md, "Choreography literals"): op80 swaps the caster for five slice models that
 * are cut away from the bottom up while it drifts and jitters, with flashes and ash / smoke emitters; the states,
 * tints, keyframes and length. */
#define FX_DEATH_MODELS       5    /* btlfx_initEffModels slots, slice models (fx_1af_modelFileIds) */
#define FX_DEATH_STATE_START  0    /* work->state0: hide the caster, register the models, first flash */
#define FX_DEATH_STATE_DISSOLVE 1  /* the slice models dissolve */
#define FX_DEATH_FLASH_RGB    0x80, 0x80, 0xAA /* additive screen flash (STATE_START) */
#define FX_DEATH_FLASH_2_RGB  0x40, 0x40, 0x55 /* additive flash on STATE_DISSOLVE's first frame */
#define FX_DEATH_FLICKER_RGB  0x2A, 0x2A, 0x38 /* additive flicker on the FLICKER frames */
#define FX_DEATH_FLICKER_FRAME   1
#define FX_DEATH_FLICKER_2_FRAME 0x14
#define FX_DEATH_FLICKER_3_FRAME 0x28
#define FX_DEATH_JOLT_FRAMES  4    /* the models turn and drift faster while frame < this */
#define FX_DEATH_ASH_FRAME    0xA  /* SPECKS, ASH_COLUMN, ASH_COLUMN_2, SMOKE */
#define FX_DEATH_ASH_2_FRAME  0x1E /* ASH_COLUMN_3, ASH_COLUMN_4 */
#define FX_DEATH_SMOKE_FRAME  0x45 /* DARK_SMOKE, SPECKS_2, OLIVE_RINGS */
#define FX_DEATH_RINGS_FRAME  0x4B /* PINK_RINGS */
#define FX_DEATH_PINK_RINGS_TRAIL_SLOTS  0x2C /* PINK_RINGS' trail buffer fx_1af_op80Trails: slots of */
#define FX_DEATH_PINK_RINGS_TRAIL_STRIDE 0x4C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_DEATH_DISSOLVE_FRAMES 0xB4 /* STATE_DISSOLVE's length (the slice height ramps over it; the op ends) */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* counter */
    s32 pos8[3]; /* position */
    SVECTOR rot14; /* rotation */
    s32 scale1C[3]; /* scale */
    SVECTOR pos28;
    SVECTOR pos30;
    SVECTOR pos38;
    s32 model40[FX_DEATH_MODELS]; /* models */
} Fx1afWork; /* 0x54 */

extern s16 fx_1af_modelFileIds[FX_DEATH_MODELS]; /* model files */
extern Unk800F78A4 *fx_1af_env;
extern Unk80100C78Entry *fx_1af_task;
extern BtlModelBuf fx_1af_effModels[];
extern s32 fx_1af_op80Trails[];

#endif
