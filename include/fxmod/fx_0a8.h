#ifndef FX_0A8_H
#define FX_0A8_H

/* Effect module fx_0a8 (dir 13 file 0xA8): Reis's Wind. Emitters only. Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_0a8 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_REIS_WIND_SPARKLES        0x1 /* emitter: violet, gold, teal and white sparkles */
#define FX_REIS_WIND_LIGHT_STREAK    0x3 /* emitter: fan drawn as a thin green to violet horizontal light streak */
#define FX_REIS_WIND_SPARKLE_CIRCLE 0x5 /* emitter: circle of gold / white sparkle quads + teal specks (byte-identical to fx_03d entry 0x11) */
#define FX_REIS_WIND_VIOLET_SPARKLES 0x6 /* emitter: violet sparkles */
#define FX_REIS_WIND_WIND_TRAIL      0x7 /* emitter: green wind trail + green specks */
#define FX_REIS_WIND_GREEN_RINGS     0xC /* emitter: green gouraud rings */
#define FX_REIS_WIND_GREEN_WISP      0xF /* emitter: green wisp puffs */
#define FX_REIS_WIND_GREEN_CRESCENT  0x10 /* emitter: green crescents */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* unit 0x10 body position */
    SVECTOR rot10; /* unit 0x10 rotation */
} Fx0a8Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    s32 unk14; /* unused */
    Pool6CEntry *emit18;
    s32 unk1C; /* unused */
    SVECTOR pos20; /* position: env->targets->centerX..Z */
} Fx0a8Work81;

extern Unk800F78A4 *fx_0a8_env;
extern s32 fx_0a8_trails[];

#endif
