#ifndef FX_130_H
#define FX_130_H

/* Effect module fx_130 (dir 13 file 0x130): enemy Propeller Wind (Ark). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_130 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PROPELLER_WIND_TORNADO       0x1 /* 'so' model: blue twisted column with a wind texture (tornado), registered per target */
#define FX_PROPELLER_WIND_TEAL_RING     0x2 /* emitter: orbiting dark-to-teal gouraud rings */
#define FX_PROPELLER_WIND_BLUE_CRESCENT 0x3 /* emitter: blue crescent then ring particles (ring texture partly not in the file) */
#define FX_PROPELLER_WIND_WIND_RING     0x4 /* emitter: orbiting grey wind ring */
#define FX_PROPELLER_WIND_WIND_SWIRL    0x5 /* emitter: three-armed grey wind swirl */

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);
extern Unk800F78A4 *fx_130_env;

typedef struct {
    s32 timer0[8]; /* countdowns */
    SVECTOR pos20; /* centre */
} Fx130Work; /* 0x28 */

typedef struct {
    SVECTOR pos0; /* unit position */
    SVECTOR rot8; /* unit rotation */
    SVECTOR rot10; /* unit rotation at the start */
    s32 timer18; /* start delay */
    s32 timer1C; /* spin time */
    s32 timer20; /* effect delay */
    s32 model24; /* model */
    SVECTOR rot28; /* model rotation */
    VECTOR pos30; /* model position */
    VECTOR scale40; /* model scale */
} Fx130Unit; /* 0x50 */

extern BtlModelBuf fx_130_effModels[];
extern Fx130Unit fx_130_units[4];

s32 fx_130_op80(s32 mode, Fx130Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
