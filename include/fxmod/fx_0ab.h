#ifndef FX_0AB_H
#define FX_0AB_H

/* Effect module fx_0ab (dir 13 file 0xAB): enemy Maelstrom (Feather Circle/Garuda and others). Emitters only.
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ab effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MAELSTROM_RED_PUFFS   0x1 /* emitter: red puffs + sprites */
#define FX_MAELSTROM_RED_STREAKS 0x2 /* emitter: red wisps and streaks */
#define FX_MAELSTROM_SWIRL       0x3 /* emitter: dark red spiral swirl */
#define FX_MAELSTROM_WIND_TRAIL  0x4 /* emitter: grey smoke trail spiralling */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* caster position */
    SVECTOR pos18; /* pos10 raised by 0xC8 */
    SVECTOR pos20; /* pos10 raised by 0x190 */
} Fx0abWork;

extern Unk800F78A4 *fx_0ab_env;
extern s32 fx_0ab_trails[];

#endif
