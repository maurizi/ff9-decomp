#ifndef FX_080_H
#define FX_080_H

/* Effect module fx_080 (dir 13 file 0x80): Dispel. Emitters only. Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_080 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DISPEL_SEAL_DISCS   0x3 /* emitter: engraved disc particles with pink glow */
#define FX_DISPEL_GOLD_SEAL    0x7 /* emitter: orange engraved seal + white-violet rings */
#define FX_DISPEL_SEAL_SHATTER 0x9 /* emitter: crystal shards, blue glow and silver seal discs */
#define FX_DISPEL_FLASH_RINGS  0xA /* emitter: white-yellow and white-red gouraud rings */

/* fx_080 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_DISPEL_STATE_SEAL     0    /* op80's only state */
#define FX_DISPEL_SHATTER_FRAME  0x18 /* FX_DISPEL_SEAL_SHATTER starts */
#define FX_DISPEL_LAST_FRAME     0x30 /* op80 ends after this frame */

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* caster body position */
} Fx080Work;

extern Unk800F78A4 *fx_080_env;

#endif
