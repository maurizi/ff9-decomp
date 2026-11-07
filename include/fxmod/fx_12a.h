#ifndef FX_12A_H
#define FX_12A_H

/* Effect module fx_12a (dir 13 file 0x12A): enemy Jet Fire (Tiamat). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_12a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_JET_FIRE_RED_GLOW       0x1 /* emitter: red-white glow ring */
#define FX_JET_FIRE_WHITE_RING     0x2 /* emitter: white-grey gouraud tube rings */
#define FX_JET_FIRE_JET_STREAM     0x3 /* emitter: cyan wispy jet frames */
#define FX_JET_FIRE_CYAN_WISP_RING 0x4 /* emitter: ring with a cyan wisp texture */
#define FX_JET_FIRE_PALE_WISP_RING 0xB /* emitter: ring with a pale wisp texture */
#define FX_JET_FIRE_WIND_GUST      0xC /* emitter: cyan wisp sprites */

/* fx_12a choreography (docs/fxdata.md, "Choreography literals"): the ops' keyframes and lengths. */
#define FX_JET_FIRE_JET_FRAME    6    /* op80: the JET_STREAMs and CYAN_WISP_RINGs start at the caster's two bones, aimed at the targets */
#define FX_JET_FIRE_FRAMES       0x3C /* op80's length */
#define FX_JET_FIRE_RING_FRAMES  0x28 /* op81's length (PALE_WISP_RING at the targets' centre) */
#define FX_JET_FIRE_GUST_WAIT_FRAME 4 /* op82 (WIND_GUST at one target): BTLFX_SIG_WAIT is set again (the script goes on) */
#define FX_JET_FIRE_GUST_FRAMES  0x28 /* op82's length */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR pos20; /* bone positions */
    SVECTOR pos28;
    SVECTOR pos30;
    SVECTOR pos38;
    SVECTOR pos40; /* aim targets */
    SVECTOR pos48;
    SVECTOR rot50; /* aim angles */
    SVECTOR rot58;
} Fx12aWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx12aWork81;

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4; /* unused */
    SVECTOR pos8;
    s32 unit10; /* target index */
} Fx12aWork82;

extern Unk800F78A4 *fx_12a_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
