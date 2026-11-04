#ifndef FX_09F_H
#define FX_09F_H

/* Effect module fx_09f (dir 13 file 0x9F): Pyro (party; enemy Pyro of Benero/King Leo/Zenero). Emitters only.
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_09f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PYRO_COLOR_PUFFS 0x2 /* emitter: yellow, violet, cyan and green puffs */
#define FX_PYRO_SMOKE       0x3 /* emitter: dark grey smoke */
#define FX_PYRO_SPARK_ARCS  0x4 /* emitter: yellow-red spark arcs */
#define FX_PYRO_ORANGE_RING 0x5 /* emitter: white-orange gouraud rings */
#define FX_PYRO_FLAMES      0x7 /* emitter: flames + smoke */
#define FX_PYRO_FLAMES_2    0xC /* emitter: spiralling flames + smoke */

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* caster position */
    SVECTOR posC; /* caster body position */
} Fx09fWork;

extern Unk800F78A4 *fx_09f_env;

#endif
