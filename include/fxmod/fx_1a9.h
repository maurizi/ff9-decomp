#ifndef FX_1A9_H
#define FX_1A9_H

/* Effect module fx_1a9 (dir 13 file 0x1a9): enemy Virus Powder (Ring Leader, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1a9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_VIRUS_POWDER_PUFFS 0x3 /* emitter: violet, blue and orange powder puffs bursting into specks + pink dots */

extern Unk800F78A4 *fx_1a9_env;
extern SVECTOR fx_1a9_emitPos0;

#endif
