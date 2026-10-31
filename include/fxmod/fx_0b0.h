#ifndef FX_0B0_H
#define FX_0B0_H

/* Effect module fx_0b0 (dir 13 file 0xB0): enemy Leaf Swirl (Soulcage). Emitters only, two moved from the caster
 * bones to the sides. Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* pos14 raised by 0xF0 */
    SVECTOR pos14; /* position: env->targets->centerX */
    SVECTOR pos1C; /* unit 0x10 bone fx_0b0_bones[0] */
    SVECTOR pos24; /* unit 0x10 bone fx_0b0_bones[1] */
    SVECTOR pos2C; /* posC, 0x300 to one side */
    SVECTOR pos34; /* posC, 0x300 to the other side */
    SVECTOR pos3C;
    SVECTOR pos44;
} Fx0b0Work;

extern u16 fx_0b0_bones[2]; /* bones */
extern Unk800F78A4 *fx_0b0_env;
extern s32 fx_0b0_trails[];
extern s32 fx_0b0_trails2[];
extern s32 fx_0b0_trails3[];
extern s32 fx_0b0_trails4[];
extern s32 fx_0b0_trails5[];
extern s32 fx_0b0_trails6[];
extern s32 fx_0b0_trails7[];

#endif
