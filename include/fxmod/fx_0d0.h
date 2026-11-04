#ifndef FX_0D0_H
#define FX_0D0_H

/* Effect module fx_0d0 (dir 13 file 0xD0): Power Break. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0d0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_POWER_BREAK_RED_SWIRL    0x7 /* emitter: red-white swirl ring particles */
#define FX_POWER_BREAK_LIGHT_STREAK 0x8 /* emitter: yellow light streak with a flare, red ember sprites */
#define FX_POWER_BREAK_ENERGY_ORB   0xC /* emitter: yellow energy orb particle */
#define FX_POWER_BREAK_SPARKLES     0xD /* emitter: gold sparkle sprites, fan */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* unit 0x10 body position */
    SVECTOR pos18; /* caster body position */
} Fx0d0Work;

extern Unk800F78A4 *fx_0d0_env;
extern s32 fx_0d0_trails[];

#endif
