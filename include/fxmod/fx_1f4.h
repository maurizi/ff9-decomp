#ifndef FX_1F4_H
#define FX_1F4_H

/* Effect module fx_1f4 (dir 13 file 0x1f4): Vanish; enemy Vanish (Drakan, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1f4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_VANISH_WHITE_STREAK_2 0x1 /* emitter: orbiting screen trail with a white light-streak texture */
#define FX_VANISH_WHITE_STREAK   0x2 /* emitter: orbiting sprite (blank) + screen trail with a white light-streak texture */
#define FX_VANISH_GOLD_GLOW      0x4 /* emitter: golden glow sprite + pale yellow gouraud rings */

/* fx_1f4 choreography (docs/fxdata.md, "Choreography literals"): op80's streaks and the caster's fade (the same
 * sequence as Carbuncle (4)'s op81, fx_1ef, on unit 0 only). */
#define FX_VANISH_STREAK_TRAIL_SLOTS  0x1E /* WHITE_STREAK / WHITE_STREAK_2 trail buffers (fx_1f4_op80Trails0/1): slots of */
#define FX_VANISH_STREAK_TRAIL_STRIDE 0x9C /*   this many bytes */
#define FX_VANISH_STREAK_2_FRAME 0x10 /* WHITE_STREAK_2 and GOLD_GLOW start (WHITE_STREAK at init) */
#define FX_VANISH_FADE_OUT_FRAME 0x18 /* unit 0 fades out (fadeOut0: btlfx_fadeUnitById 0x80 -> 0) */
#define FX_VANISH_FADE_IN_FRAME  0x28 /* and back in (fadeIn4) */
#define FX_VANISH_FADE_FRAMES    0x10 /*   both last this many frames (8 per frame: 0x10 * 8 = 0x80) */
#define FX_VANISH_FRAMES         0x56 /* the task ends at this frame */

typedef struct {
    s32 fadeOut0; /* fade-out countdown */
    s32 fadeIn4; /* fade-in countdown */
    SVECTOR pos8;
} Fx1f4Work;

extern Unk800F78A4 *fx_1f4_env;
extern s32 fx_1f4_op80Trails0[];
extern s32 fx_1f4_op80Trails1[];

#endif
