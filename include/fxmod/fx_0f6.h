#ifndef FX_0F6_H
#define FX_0F6_H

/* Effect module fx_0f6 (dir 13 file 0x0F6): Meo Twister. Sprite-line layers (fx_0f6_lines) and emitters at the
 * target centre. Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
    SVECTOR linePos10;
} Fx0f6Work;

typedef struct {
    u16 block0; /* file */
    u16 done2; /* done */
    u16 start4; /* first frame */
    u16 end6; /* end frame */
    u16 frame8; /* frame */
    u16 alphaA; /* fade */
} Fx0f6Layer;

extern Fx0f6Layer fx_0f6_lines[4];
extern Unk800F78A4 *fx_0f6_env;

#endif
