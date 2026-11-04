#ifndef FX_09C_H
#define FX_09C_H

/* Effect module fx_09c (dir 13 file 0x9C): enemy Gnoll Attack (Dummy/Gnoll). Emitters only. Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_09c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_GNOLL_ATTACK_IMPACT_RINGS   0x1 /* emitter: cream-orange and white gouraud rings + sparkles */
#define FX_GNOLL_ATTACK_IMPACT_RINGS_2 0x2 /* emitter: cream-orange, magenta and white gouraud rings + sparkles */
#define FX_GNOLL_ATTACK_SPARKLE_RINGS  0x7 /* emitter: fans + white-cream-red rings and sparkles */
#define FX_GNOLL_ATTACK_GREY_RING      0x8 /* emitter: grey gouraud rings */

typedef struct {
    SVECTOR unk0; /* write-only: pos10 + random */
    SVECTOR unk8; /* write-only: pos10 + random */
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* caster body position */
    Pool6CEntry *emit20;
    u8 pad24[0x2C - 0x24];
    SVECTOR pos2C;
    SVECTOR aim34; /* angles */
    Pool6CEntry *emit3C;
} Fx09cWork;

extern Unk800F78A4 *fx_09c_env;

#endif
