#ifndef FX_102_H
#define FX_102_H

/* Effect module fx_102 (dir 13 file 0x102): enemy Rise (Agares/Gargoyle). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_102 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_RISE_RUNE_CIRCLE 0x9 /* emitter: magic circle of runes cycling violet to yellow + white gouraud rings */
#define FX_RISE_GLOW_RING   0xA /* emitter: glowing ring disc cycling violet to orange */

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
