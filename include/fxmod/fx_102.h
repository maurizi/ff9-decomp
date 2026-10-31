#ifndef FX_102_H
#define FX_102_H

/* Effect module fx_102 (dir 13 file 0x102): enemy Rise (Agares/Gargoyle). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4[8];
    Pool6CEntry *emit24;
    SVECTOR pos28;
    SVECTOR pos30[8];
} Fx102Work;

extern u16 fx_102_emitFileIds[8];
extern Unk800F78A4 *fx_102_env;

#endif
