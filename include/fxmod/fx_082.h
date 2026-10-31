#ifndef FX_082_H
#define FX_082_H

/* Effect module fx_082 (dir 13 file 0x82): Magic Hammer (party and enemy). A morphing hammer model
 * (Hi_DrawMorphEffModel) and a squash of the caster scale. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 unk4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10;
    SVECTOR pos18; /* pos8 moved 0x226 along rot20, raised by 0x145 */
    SVECTOR rot20; /* caster rotation */
    SVECTOR rot28; /* model rotation */
    s32 model30; /* models */
    s32 model34;
} Fx082Work80;

typedef struct {
    s32 scale0[3]; /* caster scale */
} Fx082Work81;

extern Unk800F78A4 *fx_082_env;
extern BtlModelBuf fx_082_effModels[];

#endif
