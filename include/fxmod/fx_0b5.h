#ifndef FX_0B5_H
#define FX_0B5_H

/* Effect module fx_0b5 (dir 13 file 0xB5): enemy Rainbow Wind (Zemzelett). Evidence: [data] the party ability /
 * item tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round
 * 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0b5 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_RAINBOW_WIND_RAINBOW_TRAIL 0x1 /* emitter: rainbow-coloured 3D trail spiralling up */
#define FX_RAINBOW_WIND_WHIRLWIND     0x2 /* emitter: blue-white whirlwind swirl particles */
#define FX_RAINBOW_WIND_RAINBOW_RING  0x5 /* emitter: rainbow ring particle */

typedef struct {
    SVECTOR pos0; /* caster position */
    SVECTOR pos8; /* caster body position */
} Fx0b5Work;

extern Unk800F78A4 *fx_0b5_env;
extern s32 fx_0b5_trails[];

#endif
