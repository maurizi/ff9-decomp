#ifndef FX_0AC_H
#define FX_0AC_H

/* Effect module fx_0ac (dir 13 file 0xAC): enemy Sleeping Juice (Cave Imp/Flan). Emitters only. Evidence: [data]
 * the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ac effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SLEEPING_JUICE_SPLASH_TRAILS 0x1 /* emitter: yellow liquid trails */
#define FX_SLEEPING_JUICE_RIPPLE        0x2 /* emitter: yellow arc particles */
#define FX_SLEEPING_JUICE_BUBBLES       0x3 /* emitter: yellow bubble cloud */
#define FX_SLEEPING_JUICE_GLOW          0x4 /* emitter: yellow glow burst */

/* fx_0ac choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_SLEEPING_JUICE_SPLASH_FRAME  2     /* FX_SLEEPING_JUICE_SPLASH_TRAILS start (and turn towards the target from then on) */
#define FX_SLEEPING_JUICE_SPLASH_REACH  (-0x32) /* from the point this far along the unit's bone */
#define FX_SLEEPING_JUICE_SPLASH_HEIGHT 0x96  /* raised by this much */
#define FX_SLEEPING_JUICE_BUBBLES_FRAME 8     /* BUBBLES, GLOW and RIPPLE start */
#define FX_SLEEPING_JUICE_FRAMES        0x32  /* op80's length (the op ends) */
#define FX_SLEEPING_JUICE_SPLASH_TRAILS_TRAIL_SLOTS  0x3B /* FX_SLEEPING_JUICE_SPLASH_TRAILS' trail buffer fx_0ac_trails: slots of */
#define FX_SLEEPING_JUICE_SPLASH_TRAILS_TRAIL_STRIDE 0xBC /* this many bytes */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* caster position */
    SVECTOR pos20; /* unit 0x10 bone position, raised by 0x96 */
    SVECTOR aim28; /* angles from pos20 to pos10 */
} Fx0acWork;

extern Unk800F78A4 *fx_0ac_env;
extern s32 fx_0ac_trails[];

#endif
