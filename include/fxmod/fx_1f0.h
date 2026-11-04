#ifndef FX_1F0_H
#define FX_1F0_H

/* Effect module fx_1f0 (dir 13 file 0x1f0): enemy Appearance (Dummy/Necron). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1f0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_APPEARANCE_GREY_RINGS         0x1 /* emitter: grey / white / blue gouraud rings */
#define FX_APPEARANCE_BLUE_SMOKE_COLUMN  0x3 /* 'so' model: open cylinder, blue smoke texture */
#define FX_APPEARANCE_SPRITES            0x4 /* emitter: sprites, render empty (look pending) */
#define FX_APPEARANCE_BLUE_SMOKE_TWIST   0x5 /* 'so' model: twisted cylinder, blue smoke texture */
#define FX_APPEARANCE_BROWN_SMOKE_COLUMN 0x6 /* 'so' model: open cylinder, brown smoke texture */
#define FX_APPEARANCE_BROWN_SMOKE_TWIST  0x7 /* 'so' model: twisted cylinder, brown smoke texture */

typedef struct {
    s32 delay0; /* start delay */
    s32 timer4; /* flight countdown */
    SVECTOR startPos8; /* start position */
    SVECTOR homePos10; /* home position */
    SVECTOR emitPos18;
    SVECTOR pos20; /* current position */
    Pool6CEntry *emit28;
} Fx1f0Unit; /* 0x2C */

typedef struct {
    s32 timer0[16]; /* timers */
    SVECTOR ctrlPts40[4]; /* curve control points */
    s32 model60; /* models */
    s32 model64;
    s32 model68;
    s32 model6C;
    VECTOR pos70; /* model position */
    VECTOR scale80; /* model scale */
    VECTOR scale90;
    SVECTOR rotA0; /* model rotation */
    SVECTOR rotA8;
    s32 spinB0;
    s32 spinB4;
} Fx1f0Work; /* 0xB8 */

extern s32 fx_1f0_unitDelays[];
extern Unk800F78A4 *fx_1f0_env;
extern Fx1f0Unit fx_1f0_units[4];
extern BtlModelBuf fx_1f0_effModels[];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

/* propagated */
static void fxm_easeBlend4(s32 mode, s32 len, s32 t, SVECTOR *ctrl, SVECTOR *out);

#endif
