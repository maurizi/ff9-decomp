#ifndef FX_17F_H
#define FX_17F_H

/* Effect module fx_17f (dir 13 file 0x17f): enemy Melt (Zombie). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 scale0[3]; /* target scale */
    SVECTOR posC; /* target position */
    SVECTOR rot14; /* target rotation */
    BtlModelHeader *model1C;
    SVECTOR pos20; /* caster position */
} Fx17fWork;

extern Unk800F78A4 *fx_17f_env;
extern s32 fx_17f_vertexDrops[0x2BC];
extern s32 fx_17f_modelBackup[];

void fx_17f_op80MeltVertex(SVECTOR *v, s32 i);

#endif
