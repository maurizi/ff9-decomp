#ifndef FX_0B2_H
#define FX_0B2_H

/* Effect module fx_0b2 (dir 13 file 0xB2): Twister (Blue Magic; the enemy Twister / Whirlwind, Ark's among them).
 * Not Ark's summon, which is fx_17d (file 0x17D; short 0x1BF). Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state 0-3 */
    s32 unk4; /* frame */
    SVECTOR unk8; /* centre: env->targets->centerX */
    SVECTOR pos10; /* position */
    SVECTOR unk18;
} Fx0b2Work; /* 0x20 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[25];
} Fx0b2Trail; /* 0xCC */

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx0b2Rgbc; /* primitive colour word */

extern Unk800F78A4 *fx_0b2_env;
extern Fx0b2Trail D_801E90B0[3];

static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx0b2Trail *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9, s32 arg10);
void func_801E78FC(SVECTOR *arg0, SVECTOR *arg1, VECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13, s32 arg14, s32 arg15);

#endif
