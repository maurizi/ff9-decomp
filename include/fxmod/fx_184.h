#ifndef FX_184_H
#define FX_184_H

/* Effect module fx_184 (dir 13 file 0x184): Spear. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_184 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SPEAR_WHITE_RING 0x1 /* emitter: white-lilac gouraud ring + sprites that render transparent, fan */
#define FX_SPEAR_SPEAR      0x2 /* 'so' model: the thrown spear (registered after btlfx_setWeaponVisible(caster, 0)) */

/* fx_184 choreography (docs/fxdata.md, "Choreography literals"): the spear's drop, op82's jump back (states and
 * step counts) and the ops' lengths. */
#define FX_SPEAR_DROP_HEIGHT (-0xFA0) /* op80: the spear starts this high above the caster and falls on the target */
#define FX_SPEAR_FRAMES      4        /* op80's length: the spear lands at this frame (lerp t = frame << 10) */
#define FX_SPEAR_RING_FRAMES 0x1E     /* op81's length (WHITE_RING at the target on the ground) */
#define FX_SPEAR_JUMP_HEIGHT (-2000)  /* op82: the caster starts this high above its place, */
#define FX_SPEAR_LAND_DISTANCE 0x320  /* lands this far (+ the target's radius) in front of the target, */
#define FX_SPEAR_STATE_LAND  0        /* work->state30: moves there in JUMP_FRAMES steps, */
#define FX_SPEAR_STATE_WAIT  1        /* then waits WAIT_FRAMES (the op ends) */
#define FX_SPEAR_JUMP_FRAMES 6
#define FX_SPEAR_WAIT_FRAMES 0x10
#define FX_SPEAR_WEAPON_FRAME 6       /* op82: the caster's weapon is shown again */

typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR pos8; /* target body position */
    SVECTOR pos10; /* from */
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    u16 model40; /* model */
} Fx184Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx184Work81;

typedef struct {
    SVECTOR pos0;
    SVECTOR unk8; /* write-only (pos0 on the ground) */
    SVECTOR pos10; /* target position */
    SVECTOR pos18; /* destination */
    SVECTOR rot20; /* angles */
    SVECTOR pos28;
    s32 state30; /* state */
    s32 duration34; /* steps */
    s32 frame38; /* step */
} Fx184Work82;

extern Unk800F78A4 *fx_184_env;
extern BtlModelBuf fx_184_effModels[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

/* propagated */
static s32 fxm_moveUnitArc(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start);

#endif
