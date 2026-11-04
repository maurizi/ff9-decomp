#ifndef FX_19A_H
#define FX_19A_H

/* Effect module fx_19a (dir 13 file 0x19a): Minus Strike (alt). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_19a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MINUS_STRIKE_SLASH_ARC 0x4 /* emitter: blue slash arc, blue sparkle sprites, pale yellow ring, fan (byte-identical to fx_0bd entry 0x4) */

extern Unk800F78A4 *fx_19a_env;

/* propagated */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
} Fx19aWork80;
s32 fx_19a_op80(s32 mode, Fx19aWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
