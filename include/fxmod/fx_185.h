#ifndef FX_185_H
#define FX_185_H

/* Effect module fx_185 (dir 13 file 0x185): Spear (2nd). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_185 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SPEAR_WHITE_RING 0x1 /* emitter: white-lilac gouraud ring + sprites that render transparent, fan */
#define FX_SPEAR_SPEAR      0xB /* 'so' model: the thrown spear (registered after btlfx_setWeaponVisible(caster, 0)) (byte-identical to fx_184 entry 0x2) */

/* fx_185 choreography (docs/fxdata.md, "Choreography literals"): the spears' drop and the ops' lengths. */
#define FX_SPEAR_DROP_HEIGHT (-0x7D0) /* op80: a spear starts this high above the caster, falls on a random point near the targets */
#define FX_SPEAR_FRAMES      4        /* op80's length: the spear lands at this frame (lerp t = frame << 10) */
#define FX_SPEAR_RING_FRAMES 0x1E     /* op81's length (WHITE_RING at the last spear's point) */

typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    SVECTOR rot10; /* model rotation */
    VECTOR pos18; /* model position */
    VECTOR scale28; /* model scale */
    s32 index38; /* index */
    u16 model3C; /* model */
} Fx185Work80;
#define FX_185_SPEARS 0x10 /* instances, one per spear */
#define FX_185_LAST_SPEAR (FX_185_SPEARS - 1) /* clears the models when done */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx185Work81;

extern Unk800F78A4 *fx_185_env;
extern BtlModelBuf fx_185_effModels[];
extern SVECTOR fx_185_targetPos;

#endif
