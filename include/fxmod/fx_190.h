#ifndef FX_190_H
#define FX_190_H

/* Effect module fx_190 (dir 13 file 0x190): enemy Viral Smoke (Meltigemini). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 count0; /* count */
    SVECTOR rot4; /* angles */
    SVECTOR rotC; /* angles */
    SVECTOR rot14[4];
    SVECTOR pos34; /* caster position */
    SVECTOR pos3C; /* target position */
    SVECTOR pos44;
    SVECTOR pos4C;
    Pool6CEntry *emit54[4];
    Pool6CEntry *emit64[4];
} Fx190Work; /* 0x74 */

extern Unk800F78A4 *fx_190_env;

#endif
