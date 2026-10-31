#ifndef FX_08B_H
#define FX_08B_H

/* Effect module fx_08b (dir 13 file 0x8B): enemy Aerial Slash (Nova Dragon). Emitters and a screen grid flash.
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    s32 unk10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* unit 0x10 position */
    SVECTOR rot20; /* unit 0x10 rotation */
    SVECTOR pos28; /* unit 0x10 bone 5 position */
    SVECTOR pos30; /* unit 0x10 bone 0x22 position */
    SVECTOR pos38; /* pos18 moved 0x708 along rot20 */
    SVECTOR aim40; /* angles from pos28 to aimPos50 */
    SVECTOR aim48; /* angles from pos30 to aimPos50 */
    SVECTOR aimPos50; /* pos18 moved 0x7D0 along rot20 */
    s32 tint58; /* screen tint */
} Fx08bWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* unit position */
    s32 unitC; /* unit */
} Fx08bWork81;

extern Unk800F78A4 *fx_08b_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v);

#endif
