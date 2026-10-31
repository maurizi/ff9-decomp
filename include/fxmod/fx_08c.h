#ifndef FX_08C_H
#define FX_08C_H

/* Effect module fx_08c (dir 13 file 0x8C): Berserk, all targets (party; enemy Berserk of Armstrong). One texture
 * model drawn twice, and a backdrop. Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 bgFrame4; /* frame for btlfx_drawBackdrop */
    SVECTOR pos8; /* position: env->targets->centerX..Z */
    SVECTOR unk10; /* pos8 raised by 0x100 */
    s32 modelPos18[3]; /* model position (VECTOR without its pad word) */
    s32 model24; /* model */
} Fx08cWork;

extern Unk800F78A4 *fx_08c_env;
extern BtlModelBuf fx_08c_effModels[];

#endif
