#ifndef FX_11C_H
#define FX_11C_H

/* Effect module fx_11c (dir 13 file 0x11C): enemy High Wind (Abadon). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10;
} Fx11cWork80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4[5];
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR pos20; /* target position */
} Fx11cWork81;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18;
    SVECTOR pos20;
    SVECTOR pos28;
} Fx11cWork82;

typedef struct {
    SVECTOR pos0; /* spawn position */
    SVECTOR unk8;
    u16 unk10; /* started */
    u16 unk12; /* age */
    u16 unk14; /* start frame */
    u16 unk16; /* file */
} Fx11cBurst;

extern Fx11cBurst D_801E7EDC[5];
extern Unk800F78A4 *fx_11c_env;
extern s32 fx_11c_op80Trails0[];
extern s32 fx_11c_op80Trails1[];
extern s32 fx_11c_op82Trails[];

#endif
