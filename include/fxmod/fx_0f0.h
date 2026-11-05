#ifndef FX_0F0_H
#define FX_0F0_H

/* Effect module fx_0f0 (dir 13 file 0x0F0): enemy Zombie Powder (Whale Zombie). Emitters from a caster bone with
 * trail buffers, one homing on the target. Evidence: [data] the party ability / item tables and enemy sequences
 * that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ZOMBIE_POWDER_BLUE_SPECKS  0x1 /* emitter: animated quads, blue specks at run time: the blue glowing dust drifting round the caster; runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_ZOMBIE_POWDER_SPORE_TRAIL  0x2 /* emitter: homing green spores, yellow puffs, sparkles */
#define FX_ZOMBIE_POWDER_YELLOW_PUFFS 0x3 /* emitter: yellow powder puffs */
#define FX_ZOMBIE_POWDER_YELLOW_CLOUD 0x4 /* emitter: yellow powder cloud */
#define FX_ZOMBIE_POWDER_VIOLET_CLOUD 0x5 /* emitter: violet powder cloud */
#define FX_ZOMBIE_POWDER_MIST_TRAIL   0x6 /* emitter: green-grey mist screen trails */
#define FX_ZOMBIE_POWDER_MIST_TRAIL_2 0x7 /* emitter: green-grey mist screen trails */
#define FX_ZOMBIE_POWDER_DUST_WAVE    0x15 /* emitter: yellow-green dust half ring */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR pos24;
} Fx0f0Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC;
} Fx0f0Work81;

extern Unk800F78A4 *fx_0f0_env;
extern s32 fx_0f0_op80Trails0[];
extern s32 fx_0f0_op80Trails1[];
extern s32 fx_0f0_op80Trails2[];

#endif
