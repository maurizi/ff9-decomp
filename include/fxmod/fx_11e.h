#ifndef FX_11E_H
#define FX_11E_H

/* Effect module fx_11e (dir 13 file 0x11E): enemy Virus Fly (Abadon). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_11e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_VIRUS_FLY         0x1 /* 'so' model: brown fly (insect) */
#define FX_VIRUS_FLY_2       0x2 /* 'so' model: brown fly (insect), wings in another pose (same size as entry 0x1) */
#define FX_VIRUS_FLY_FLIES   0x3 /* emitter: homing animated fly sprites flapping their wings, 64 frames */

/* fx_11e choreography (docs/fxdata.md, "Choreography literals"): the fly model's buzz round the target and op80's
 * keys. */
#define FX_VIRUS_FLY_MODEL_SLOTS   8    /* btlfx_initEffModels slots (once for all targets, fx_11e_modelsReady) */
#define FX_VIRUS_FLY_RELEASE_FRAME 3    /* sets BTLFX_SIG_WAIT again (the script goes on to the next target) */
#define FX_VIRUS_FLY_BUZZ_FRAME    0x1E /* the fly model (FLY / FLY_2 alternating) buzzes round the target from this frame */
#define FX_VIRUS_FLY_BUZZ_FRAMES   0x28 /* for this many frames */
#define FX_VIRUS_FLY_LEAVE_FRAME   0x19 /* from this frame of the buzz it flies up and away (it turns round the frame before) */
#define FX_VIRUS_FLY_FRAMES        0x50 /* op80's length (the op ends) */

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);


typedef struct {
    Pool6CEntry *emit0;
    SVECTOR rot4; /* model rotation */
    VECTOR posC; /* model position */
    VECTOR scale1C; /* model scale */
    u8 pad2C[0x30 - 0x2C];
    s32 dir30; /* radius direction */
    s32 dir34; /* angle direction */
    s32 radius38; /* radius */
    s32 angle3C; /* angle */
    s32 timer40; /* radius direction timer */
    s32 timer44; /* angle direction timer */
    SVECTOR pos48; /* target body position */
    SVECTOR pos50;
    s32 unit58; /* target */
    u16 model5C; /* models */
    u16 model5E;
} Fx11eWork; /* 0x60 */
extern s32 fx_11e_modelsReady; /* models registered */
extern Unk800F78A4 *fx_11e_env;
extern BtlModelBuf fx_11e_effModels[];
s32 fx_11e_op80(s32 mode, Fx11eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
