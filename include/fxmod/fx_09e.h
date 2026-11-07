#ifndef FX_09E_H
#define FX_09E_H

/* Effect module fx_09e (dir 13 file 0x9E): Blind, one target (party; enemy Blind of Quale and others). Emitters
 * only. Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_09e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BLIND_VORTEX   0x6 /* emitter: silver-blue swirling vortex ring */
#define FX_BLIND_VORTEX_2 0x8 /* emitter: silver-blue swirling vortex ring + fans */

/* fx_09e choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_BLIND_VORTEX_FRAME 0x18 /* FX_BLIND_VORTEX starts (FX_BLIND_VORTEX_2 at the start) */
#define FX_BLIND_FRAMES       0x38 /* op80's length (the op ends) */

typedef struct {
    SVECTOR pos0; /* caster body position */
} Fx09eWork;

extern Unk800F78A4 *fx_09e_env;

#endif
