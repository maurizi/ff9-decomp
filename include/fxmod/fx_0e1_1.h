#ifndef FX_0E1_1_H
#define FX_0E1_1_H

/* Effect module fx_0e1_1 (dir 13 file 0x0E1): Rebirth Flame (Phoenix's second attack), load group 1. Screen tints,
 * a sprite line and a backdrop, then emitters under a bobbing camera point with a ripple grid
 * (btlfx_drawRippleGrid). Evidence: [data] the party ability / item tables and enemy sequences that play this file
 * id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    SVECTOR pos8; /* effect origin */
    s32 unk10; /* unused (runtime watch, round 33) */
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR camPos24; /* camera position */
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    s32 swayAmp34; /* sway amplitude */
    s32 fadeOut38; /* fading out */
    s32 alpha3C; /* particle alpha */
} Fx0e11Work; /* 0x40 */

extern SVECTOR fx_0e1_1_origin;
extern SVECTOR fx_0e1_1_camPos;
extern CVECTOR fx_0e1_1_rippleRgb;
extern Unk800F78A4 *fx_0e1_1_env;

s32 fx_0e1_1_op80(s32 mode, Fx0e11Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */
s32 fx_0e1_1_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
