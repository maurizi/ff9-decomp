#ifndef FX_0A3_H
#define FX_0A3_H

/* Effect module fx_0a3 (dir 13 file 0xA3): enemy Electrocute (Torama and others). Emitters only. Evidence: [data]
 * the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0a3 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ELECTROCUTE_BLUE_BOLTS   0x1 /* emitter: blue bolt trail with blue glow sprites, the blue lightning crackling round the target; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_ELECTROCUTE_BLUE_BOLTS_2 0x2 /* emitter: the same blue bolt trail + sprites, spiralling: part of the blue lightning round the target; runtime (docs/runtime-findings.md effect-data pass 2, medium) */

/* fx_0a3 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_ELECTROCUTE_REACH          0xC8 /* the bolts start this far ahead of the unit (pos10) */
#define FX_ELECTROCUTE_BOLTS_2_FRAME  0xA  /* FX_ELECTROCUTE_BLUE_BOLTS_2 starts */
#define FX_ELECTROCUTE_FRAMES         0x1E /* op80's length (the op ends) */
#define FX_ELECTROCUTE_BLUE_BOLTS_TRAIL_SLOTS    0x2C /* FX_ELECTROCUTE_BLUE_BOLTS' trail buffer fx_0a3_trails: slots of */
#define FX_ELECTROCUTE_BLUE_BOLTS_TRAIL_STRIDE   0x28 /* this many bytes */
#define FX_ELECTROCUTE_BLUE_BOLTS_2_TRAIL_SLOTS  0x14 /* FX_ELECTROCUTE_BLUE_BOLTS_2's trail buffer fx_0a3_trails2: slots of */
#define FX_ELECTROCUTE_BLUE_BOLTS_2_TRAIL_STRIDE 0x28 /* this many bytes */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* unit 0x10 body position, moved 0x190 along rot18 */
    SVECTOR pos10; /* unit 0x10 position, moved 0xC8 along rot18 */
    SVECTOR rot18; /* unit 0x10 rotation */
} Fx0a3Work;

extern Unk800F78A4 *fx_0a3_env;
extern s32 fx_0a3_trails[];
extern s32 fx_0a3_trails2[];

#endif
