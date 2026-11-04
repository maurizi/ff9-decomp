#ifndef FX_0BF_H
#define FX_0BF_H

/* Effect module fx_0bf (dir 13 file 0xBF): Thunder Slash. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

/* fx_0bf effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THUNDER_SLASH_SPARKS    0x1 /* emitter: cone of blue-green electric wisps, orange sparks and white glints */
#define FX_THUNDER_SLASH_LIGHTNING 0x2 /* sprite: blue / red lightning streak, 12 frames (drawn directly) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
    s32 tintC; /* screen tint */
} Fx0bfWork80;

typedef struct {
    SVECTOR pos0; /* unit 0x10 position */
} Fx0bfWork81;

extern Unk800F78A4 *fx_0bf_env;

#endif
