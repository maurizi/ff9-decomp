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

/* fx_130 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, its countdown slots
 * (work->timer0[FX_PROPELLER_WIND_T_*]) and lengths, and each target's tornado spin (Fx130Unit). */
#define FX_PROPELLER_WIND_UNITS        4    /* fx_130_units entries (one TORNADO model per target; btlfx_initEffModels slots) */
#define FX_PROPELLER_WIND_TIMERS       8    /* work->timer0[] countdowns (cleared at init): */
#define FX_PROPELLER_WIND_T_TORNADOES  0    /* the tornadoes spin the targets while it runs */
#define FX_PROPELLER_WIND_T_CRESCENTS  1    /* BLUE_CRESCENT starts at each target when its delay runs out */
#define FX_PROPELLER_WIND_SPIN_FRAME   8    /* both start */
#define FX_PROPELLER_WIND_TORNADOES_FRAMES 0x38
#define FX_PROPELLER_WIND_CRESCENTS_FRAMES 0x20
#define FX_PROPELLER_WIND_TEAL_RING_FRAME  0x2A /* TEAL_RING starts (WIND_SWIRL and WIND_RING at frame 0) */
#define FX_PROPELLER_WIND_FRAMES       0x50 /* op80's length */
#define FX_PROPELLER_WIND_STAGGER_FRAMES 6  /* target i's tornado and crescent start i * this many frames later */
#define FX_PROPELLER_WIND_SPIN_FRAMES  0x20 /* Fx130Unit.timer1C: a tornado's life; the target turns two half turns (eased in, out) */
#define FX_PROPELLER_WIND_GROW_FRAMES  8    /* the tornado widens (and brightens) over its first, narrows over its last this many frames */

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);
extern Unk800F78A4 *fx_130_env;

typedef struct {
    s32 timer0[FX_PROPELLER_WIND_TIMERS]; /* countdowns (FX_PROPELLER_WIND_T_*) */
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
extern Fx130Unit fx_130_units[FX_PROPELLER_WIND_UNITS];

s32 fx_130_op80(s32 mode, Fx130Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
