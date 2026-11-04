#ifndef FX_093_H
#define FX_093_H

/* Effect module fx_093 (dir 13 file 0x93): enemy Cannon (Armstrong and others). Emitters only. Evidence: [data]
 * the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_093 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CANNON_SMOKE      0x2 /* emitter: grey smoke puffs */
#define FX_CANNON_EXPLOSION  0x4 /* emitter: explosion burst + sparks */
#define FX_CANNON_BLAST_RING 0x8 /* emitter: cream-orange gouraud rings */
#define FX_CANNON_DUST_RING  0xC /* emitter: grey dust ring */

typedef struct {
    SVECTOR pos0; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos8; /* unit 0x10 bone position */
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* caster position */
    SVECTOR aim20; /* angles from pos0 to pos8 */
    Pool6CEntry *emit28;
} Fx093Work;

extern Unk800F78A4 *fx_093_env;

#endif
