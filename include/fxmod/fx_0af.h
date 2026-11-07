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

/* fx_0af choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_ZOMBIE_BREATH_STATE_BLOW  0    /* op80's only state: the breath trails blow towards the target */
#define FX_ZOMBIE_BREATH_GLOW_FRAME  0x10 /* FX_ZOMBIE_BREATH_GREEN_GLOW and _BREATH_TRAILS_2 start */
#define FX_ZOMBIE_BREATH_FRAMES      0x3C /* op80's length (the op ends) */
#define FX_ZOMBIE_BREATH_TRAIL_SLOTS  0x1E /* both breath-trail emitters' trail buffers (fx_0af_trails, _trails2): slots of */
#define FX_ZOMBIE_BREATH_TRAIL_STRIDE 0x4C /* this many bytes */

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
