#ifndef FX_1A5_H
#define FX_1A5_H

/* Effect module fx_1a5 (dir 13 file 0x1a5): Revive (Elan). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1a5 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_REVIVE_LIGHT_SWIRL 0x1 /* emitter: blue-white light swirl + flare (byte-identical to fx_089 entry 0x4) */
#define FX_REVIVE_FEATHERS    0x2 /* emitter: white wing / feather sprites + flare (byte-identical to fx_089 entry 0x5) */

/* fx_1a5 choreography (docs/fxdata.md, "Choreography literals"; fx_089's names). */
#define FX_REVIVE_FEATHERS_FRAME 0xF  /* FX_REVIVE_FEATHERS starts */
#define FX_REVIVE_SIGNAL_FRAME   0xA  /* the BTLFX_SIG_WAIT signal is raised again (cleared at init) */
#define FX_REVIVE_FRAMES         0x28 /* op80's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
    s32 unit10; /* unit */
} Fx1a5Work; /* 0x14 */

extern Unk800F78A4 *fx_1a5_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
