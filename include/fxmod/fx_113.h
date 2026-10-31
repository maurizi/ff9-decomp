#ifndef FX_113_H
#define FX_113_H

/* Effect module fx_113 (dir 13 file 0x113): enemy Virus Combo (Mover). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
    SVECTOR pos10;
    s32 index18;
} Fx113Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx113Work81;

extern Unk800F78A4 *fx_113_env;
extern s32 fx_113_op81Trails[];

#endif
