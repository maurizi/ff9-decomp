#ifndef FX_190_H
#define FX_190_H

/* Effect module fx_190 (dir 13 file 0x190): enemy Viral Smoke (Meltigemini). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_190 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_VIRAL_SMOKE_BROWN_PUFFS 0x1 /* emitter: orange-brown smoke puffs, 24 frames */
#define FX_VIRAL_SMOKE_GREEN_PUFFS 0x2 /* emitter: green smoke puffs, 24 frames */
#define FX_VIRAL_SMOKE_PUFF_HELIX  0x4 /* emitter: helix of green and orange-brown smoke puffs */

/* fx_190 choreography (docs/fxdata.md, "Choreography literals"): four puff pairs from two caster bones, the helix,
 * op80's length. */
#define FX_VIRAL_SMOKE_GREEN_BONE    0x1C /* unit 0x10's bone the GREEN_PUFFS start from */
#define FX_VIRAL_SMOKE_BROWN_BONE    0x2C /* the one the BROWN_PUFFS start from */
#define FX_VIRAL_SMOKE_PUFF_COUNT    4    /* puff pairs (emitter / aim arrays), started */
#define FX_VIRAL_SMOKE_PUFF_INTERVAL 0x12 /* this many frames apart from frame 0, each aimed at the targets' centre plus its own rot14 offset */
#define FX_VIRAL_SMOKE_HELIX_FRAME   0xF  /* PUFF_HELIX at the targets' centre */
#define FX_VIRAL_SMOKE_FRAMES        0x60 /* op80's length (the op ends) */

typedef struct {
    s32 count0; /* count */
    SVECTOR rot4; /* angles */
    SVECTOR rotC; /* angles */
    SVECTOR rot14[FX_VIRAL_SMOKE_PUFF_COUNT];
    SVECTOR pos34; /* caster position */
    SVECTOR pos3C; /* target position */
    SVECTOR pos44;
    SVECTOR pos4C;
    Pool6CEntry *emit54[FX_VIRAL_SMOKE_PUFF_COUNT];
    Pool6CEntry *emit64[FX_VIRAL_SMOKE_PUFF_COUNT];
} Fx190Work; /* 0x74 */

extern Unk800F78A4 *fx_190_env;

#endif
