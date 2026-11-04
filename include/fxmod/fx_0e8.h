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
} Fx0e8Work;

extern Unk800F78A4 *fx_0e8_env;
extern s32 fx_0e8_op80Trails[];

#endif
