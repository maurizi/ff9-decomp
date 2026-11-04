#ifndef FX_0AF_H
#define FX_0AF_H

/* Effect module fx_0af (dir 13 file 0xAF): enemy Zombie Breath (Dracozombie). Emitters only. Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0af effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ZOMBIE_BREATH_BREATH_TRAILS   0x5 /* emitter: white ghostly puff trails */
#define FX_ZOMBIE_BREATH_GREEN_GLOW      0x7 /* emitter: white puffs + pale green glow */
#define FX_ZOMBIE_BREATH_BREATH_TRAILS_2 0x8 /* emitter: white ghostly puff trails + one more trail */

typedef struct {
    s32 state0; /* state */
    Pool6CEntry *emit4;
    SVECTOR pos8; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos10; /* caster body position */
    SVECTOR aim18; /* angles from pos8 to pos10 */
} Fx0afWork;

extern Unk800F78A4 *fx_0af_env;
extern s32 fx_0af_trails[];
extern s32 fx_0af_trails2[];

#endif
