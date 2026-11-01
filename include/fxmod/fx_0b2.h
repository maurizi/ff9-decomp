#ifndef FX_0B2_H
#define FX_0B2_H

/* Effect module fx_0b2 (dir 13 file 0xB2): Twister (Blue Magic; the enemy Twister / Whirlwind, Ark's among them).
 * Not Ark's summon, which is fx_17d (file 0x17D; short 0x1BF). Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state 0-3 */
    s32 frame4; /* frame */
    SVECTOR center8; /* centre: env->targets->centerX */
    SVECTOR pos10; /* position */
    SVECTOR rot18;
} Fx0b2Work; /* 0x20 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[25];
} Fx0b2Trail; /* 0xCC */

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx0b2Rgbc; /* primitive colour word */

extern Unk800F78A4 *fx_0b2_env;
extern Fx0b2Trail fx_0b2_trails[3];

static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx0b2Trail *trail, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth);
void fx_0b2_drawFunnel(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 radius0, s32 radius1, s32 bright, s32 tpage, s32 clut, s32 texU,
                   s32 texV, s32 drift, s32 driftAng, s32 driftStep, s32 twist, s32 abr, s32 segs);

#endif
