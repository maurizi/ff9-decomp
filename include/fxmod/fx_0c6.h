#ifndef FX_0C6_H
#define FX_0C6_H

/* Effect module fx_0c6 (dir 13 file 0xC6): Aura (Elan). Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0c6 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_AURA_CRESCENT_SWOOSHES 0x5 /* emitter: three orbiting fans, blue / pink / green crescent swooshes (byte-identical to fx_0c5 entry 0x8) */
#define FX_AURA_SWIRL       0x8 /* emitter: yellow / white / lilac swirl particles, blue star flash, sparkles, fans (byte-identical to fx_0c5 entry 0x7) */

/* fx_0c6 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and length. */
#define FX_AURA_SWIRL_FRAME   0x12 /* op80: FX_AURA_SWIRL starts */
#define FX_AURA_RELEASE_FRAME 0xC  /* op80: sets BTLFX_SIG_WAIT again (the script goes on to the next target) */
#define FX_AURA_FRAMES        0x3C /* op80: the task ends at this frame */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* unit position */
    s32 unit10; /* unit */
} Fx0c6Work;

extern Unk800F78A4 *fx_0c6_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
