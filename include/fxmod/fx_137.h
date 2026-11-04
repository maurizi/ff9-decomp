#ifndef FX_137_H
#define FX_137_H

/* Effect module fx_137 (dir 13 file 0x137): enemy Flame (Cerberus, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_137 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLAME_FLAME_BURST 0x5 /* emitter: flame explosion with a rising flame pillar, at each target */
#define FX_FLAME_FIREBALL    0x6 /* emitter: fireball explosion at the mouth */

typedef struct {
    SVECTOR rot0;
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18;
    Pool6CEntry *emit20;
} Fx137Work;

extern Unk800F78A4 *fx_137_env;
extern SVECTOR fx_137_targetPos[];

#endif
