#ifndef FX_0E8_H
#define FX_0E8_H

/* Effect module fx_0e8 (dir 13 file 0x0E8): enemy Escape (Magic Vice, ...). The caster stretches, shrinks and fades
 * out (btlfx_setUnitScale, btlfx_fadeUnitById) among emitters. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

/* fx_0e8 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ESCAPE_RAINBOW_STREAKS 0x1 /* emitter: cyan-yellow / magenta-violet flame streaks */
#define FX_ESCAPE_WIND_TRAIL      0x2 /* emitter: spiralling trail with a grey streak texture */
#define FX_ESCAPE_SMOKE           0x3 /* emitter: grey-teal smoke puffs */
#define FX_ESCAPE_CYAN_RING       0x4 /* emitter: pale cyan gouraud rings fading */
#define FX_ESCAPE_FLASH_RINGS     0x5 /* emitter: white-red and cream rings */

/* fx_0e8 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, the caster's shrink and the
 * trail buffer. */
#define FX_ESCAPE_FLASH_HEIGHT  0x12C /* FX_ESCAPE_FLASH_RINGS (frame 0) start this far above the caster */
#define FX_ESCAPE_TRAIL_FRAME   0xC   /* WIND_TRAIL, SMOKE and CYAN_RING start at the caster */
#define FX_ESCAPE_SHRINK_FRAME  0x12  /* RAINBOW_STREAKS start, the caster stretches up and thins */
#define FX_ESCAPE_SHRINK_FRAMES 5     /* over this many frames; on the last it is faded out (btlfx_fadeUnitById) */
#define FX_ESCAPE_FRAMES        0x2D  /* op80's length (the op ends) */
#define FX_ESCAPE_WIND_TRAIL_TRAIL_SLOTS  0x14 /* FX_ESCAPE_WIND_TRAIL's trail buffer fx_0e8_op80Trails: slots of */
#define FX_ESCAPE_WIND_TRAIL_TRAIL_STRIDE 0xD0 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14;
    u8 pad1C[0x2C - 0x1C];
    VECTOR scale2C; /* unit scale */
    SVECTOR pos3C;
} Fx0e8Work; /* 0x44 */

extern Unk800F78A4 *fx_0e8_env;
extern s32 fx_0e8_op80Trails[];

#endif
