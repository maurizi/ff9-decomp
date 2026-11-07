#ifndef FX_0CF_H
#define FX_0CF_H

/* Effect module fx_0cf (dir 13 file 0xCF): Stock Break. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0cf effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_STOCK_BREAK_SLASH_ARC 0x1 /* emitter: blue slash arc, fans */

/* fx_0cf choreography (docs/fxdata.md, "Choreography literals"): the ops' lengths and the leap. */
#define FX_STOCK_BREAK_FRAMES      0x28  /* op80 (FX_STOCK_BREAK_SLASH_ARC): the task ends at this frame */
#define FX_STOCK_BREAK_LEAP_FRAMES 0xC   /* op81: unit 0x10 moves (fxm_moveUnitArc) to its landing point in this many frames */
#define FX_STOCK_BREAK_LEAP_REACH  0x320 /* op81: the landing point is this far past the targets' centre, seen from unit 0x10 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* position: env->targets->centerX..Z */
} Fx0cfWork80;

typedef struct {
    SVECTOR pos0; /* unit 0x10 body position */
    SVECTOR rot8; /* angles from pos0 to pos18, turned around */
    SVECTOR pos10; /* pos18 moved 0x320 along rot8 */
    SVECTOR pos18; /* position: env->targets->centerX..Z */
    SVECTOR pos20; /* start position */
    u8 pad28[8]; /* unused: op81 asks for 0x30 bytes */
} Fx0cfWork81;

extern Unk800F78A4 *fx_0cf_env;

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static s32 fxm_moveUnitArc(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start);
static s32 fxm_turnUnitBy(s32 unit, s32 frame, s32 n, SVECTOR *unused, SVECTOR *delta);
static s32 fxm_turnUnitToward(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang);

#endif
