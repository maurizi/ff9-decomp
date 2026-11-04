#ifndef FX_131_H
#define FX_131_H

/* Effect module fx_131 (dir 13 file 0x131): enemy Golem Revival (Core/Sand Golem). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_131 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_GOLEM_REVIVAL_PINK_GLOW  0x1 /* emitter: pink glow with a green ring, fan */
#define FX_GOLEM_REVIVAL_SAND_CLOUD 0x2 /* emitter: orbiting pale sand clouds */

typedef struct {
    SVECTOR pos0;
    SVECTOR pos8;
    SVECTOR pos10;
    Pool6CEntry *emit18;
} Fx131Work;

extern Unk800F78A4 *fx_131_env;

#endif
