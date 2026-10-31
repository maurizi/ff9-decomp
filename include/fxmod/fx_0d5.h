#ifndef FX_0D5_H
#define FX_0D5_H

/* Effect module fx_0d5 (dir 13 file 0xD5): Sword Magic Fira. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* caster position */
    SVECTOR posC; /* caster body position */
} Fx0d5Work80;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d5Work84;

extern Unk800F78A4 *fx_0d5_env;
extern s32 fx_0d5_trails[];

#endif
