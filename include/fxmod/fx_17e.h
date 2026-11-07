#ifndef FX_17E_H
#define FX_17E_H

/* Effect module fx_17e (dir 13 file 0x17e): enemy Chestnut (Mandragora). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_17e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CHESTNUT_IMPACT_RINGS   0x2 /* emitter: white-pink-red-yellow and white-grey gouraud rings + spark sprites (texture not in the file) */
#define FX_CHESTNUT_SPIKY_BURR     0x9 /* 'so' model: the thrown chestnut, a spiky ball with a brown-orange burr texture */

/* fx_17e choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, its countdown slots
 * (work->timer40[FX_CHESTNUT_T_*]) and the lengths written into them. */
#define FX_CHESTNUT_TIMERS      4    /* work->timer40[] countdowns (cleared at init): */
#define FX_CHESTNUT_T_HOLD      0    /* SPIKY_BURR sits above the caster's HOLD_BONE */
#define FX_CHESTNUT_T_LIFT      1    /* it follows the caster's LIFT_BONE */
#define FX_CHESTNUT_T_THROW     2    /* it flies from there to the target */
#define FX_CHESTNUT_T_SHOW      3    /* it is drawn */
#define FX_CHESTNUT_HOLD_BONE   5
#define FX_CHESTNUT_LIFT_BONE   0xC
#define FX_CHESTNUT_APPEAR_FRAME 1   /* T_SHOW, T_HOLD and the fade-in (work->timer50) start */
#define FX_CHESTNUT_SHOW_FRAMES 0x1A
#define FX_CHESTNUT_FADE_IN_FRAMES 8 /* the burr brightens (additive) from black to full over this many frames */
#define FX_CHESTNUT_HOLD_FRAMES 8
#define FX_CHESTNUT_LIFT_FRAME  10   /* T_LIFT starts (the burr at full brightness) */
#define FX_CHESTNUT_LIFT_FRAMES 0xE
#define FX_CHESTNUT_THROW_FRAME 24   /* T_THROW starts */
#define FX_CHESTNUT_THROW_FRAMES 3
#define FX_CHESTNUT_IMPACT_FRAME 26  /* IMPACT_RINGS at the target */
#define FX_CHESTNUT_FRAMES      0x42 /* op80's length */

typedef struct {
    s32 model0; /* model */
    s32 fade4; /* brightness */
    VECTOR scale8; /* model scale */
    VECTOR pos18; /* model position */
    SVECTOR rot28; /* model rotation */
    SVECTOR pos30;
    SVECTOR pos38; /* target body position */
    s32 timer40[FX_CHESTNUT_TIMERS]; /* countdowns (FX_CHESTNUT_T_*) */
    s32 timer50;
} Fx17eWork;

extern Unk800F78A4 *fx_17e_env;
extern BtlModelBuf fx_17e_effModels[];

#endif
