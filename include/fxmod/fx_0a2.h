#ifndef FX_0A2_H
#define FX_0A2_H

/* Effect module fx_0a2 (dir 13 file 0xA2): Cura, all targets. Emitters only. Evidence: [data] the party ability /
 * item tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md,
 * "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0a2 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CURA_SPARKLE_BURST 0x1 /* emitter: ring + sparkle sprites (drawn as squares: texture not resolved) (look pending) */
#define FX_CURA_STAR_TRAIL    0x2 /* emitter: blue star-sparkle trail spiralling + sprites */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* position: env->targets->centerX..Z */
} Fx0a2Work;

extern Unk800F78A4 *fx_0a2_env;
extern s32 fx_0a2_trails[];

#endif
