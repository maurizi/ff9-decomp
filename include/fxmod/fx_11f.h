#ifndef FX_11F_H
#define FX_11F_H

/* Effect module fx_11f (dir 13 file 0x11F): item Dark Matter. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10;
} Fx11fWork;

extern Unk800F78A4 *fx_11f_env;
extern s32 fx_11f_op80Trails[];

#endif
