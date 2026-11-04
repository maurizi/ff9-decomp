#ifndef FX_11A_H
#define FX_11A_H

/* Effect module fx_11a (dir 13 file 0x11A): item Vaccine. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_11a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_VACCINE_GLOW 0x1 /* emitter: white-lilac gouraud ring, green glow sprite + a fan element */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx11aWork;

extern Unk800F78A4 *fx_11a_env;

#endif
