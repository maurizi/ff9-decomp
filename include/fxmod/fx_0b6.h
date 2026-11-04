#ifndef FX_0B6_H
#define FX_0B6_H

/* Effect module fx_0b6 (dir 13 file 0xB6): enemy Scorch (Grenade/Vepal). Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_0b6 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SCORCH_FIREBALL    0x1 /* emitter: red-orange swirling fireball particle */
#define FX_SCORCH_FAN_EMITTER 0x2 /* emitter: two fan elements (not rendered) (look pending) */
#define FX_SCORCH_HEAT_RING   0x7 /* emitter: white-pink gouraud rings */

typedef struct {
    SVECTOR pos0;
} Fx0b6Work;

extern Unk800F78A4 *fx_0b6_env;

#endif
