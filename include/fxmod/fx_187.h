#ifndef FX_187_H
#define FX_187_H

/* Effect module fx_187 (dir 13 file 0x187): enemy Fire Blades (Soulcage). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    Pool6CEntry *emit4[2];
    Pool6CEntry *emitC[2];
    SVECTOR pos14; /* target position */
    SVECTOR pos1C;
    SVECTOR pos24; /* bone positions */
    SVECTOR pos2C;
    SVECTOR pos34;
    SVECTOR pos3C;
    SVECTOR pos44;
    SVECTOR pos4C;
    SVECTOR pos54;
    SVECTOR pos5C;
} Fx187Work; /* 0x64 */

extern u16 fx_187_bones[2]; /* bones */
extern Unk800F78A4 *fx_187_env;
extern s32 fx_187_op80Trails0[];
extern s32 fx_187_op80Trails1[];
extern s32 fx_187_op80Trails2[];
extern s32 fx_187_op80Trails3[];
extern s32 fx_187_op80Trails4[];

#endif
