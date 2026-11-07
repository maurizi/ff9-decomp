#ifndef FX_1FB_H
#define FX_1FB_H

/* Effect module fx_1fb (dir 13 file 0x1fb): Carbuncle (4) (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1fb effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CARBUNCLE_ORBIT_GLOW_2   0x1 /* emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x16, same kind) */
#define FX_CARBUNCLE_WHITE_STREAK   0x4 /* emitter: orbiting sprite (blank) + screen trail with a white light-streak texture (byte-identical to fx_1ef entry 0x3) */
#define FX_CARBUNCLE_WHITE_STREAK_2 0x5 /* emitter: orbiting screen trail with a white light-streak texture (byte-identical to fx_1ef entry 0xC) */
#define FX_CARBUNCLE_GOLD_GLOW      0x6 /* emitter: golden glow sprite + pale yellow gouraud rings (byte-identical to fx_1ef entry 0xD) */

/* fx_1fb choreography (docs/fxdata.md, "Choreography literals"): the short form's op80 (one orbit glow) and
 * op81's streaks and target fade, which is fx_1ef's op81 (same names, own values where they differ). */
#define FX_CARBUNCLE_GLOW_FRAMES    0x40 /* op80: ORBIT_GLOW_2 plays in front of the camera; the task ends at this frame */
#define FX_CARBUNCLE_STREAK_TRAIL_SLOTS  0x1E /* op81: WHITE_STREAK / WHITE_STREAK_2 trail buffers (fx_1fb_op81Trails0/1): slots of */
#define FX_CARBUNCLE_STREAK_TRAIL_STRIDE 0x9C /*   this many bytes */
#define FX_CARBUNCLE_STREAK_2_FRAME 16   /* op81: WHITE_STREAK_2 and GOLD_GLOW start at each target (WHITE_STREAK at frame 0) */
#define FX_CARBUNCLE_T_FADE_OUT     0    /* fx_1fb_fadeTimers: the targets fade out (btlfx_fadeUnitById 0x80 -> 0) while set */
#define FX_CARBUNCLE_T_FADE_IN      1    /*   the targets fade back in while set */
#define FX_CARBUNCLE_FADE_FRAMES    0x10 /*   both last this many frames (8 per frame: 0x10 * 8 = 0x80) */
#define FX_CARBUNCLE_FADE_OUT_FRAME 24   /* op81: the fade out starts */
#define FX_CARBUNCLE_FADE_IN_FRAME  40   /* op81: the fade in starts */
#define FX_CARBUNCLE_OP81_FRAMES    0x38 /* op81: the task ends at this frame */

extern Unk800F78A4 *fx_1fb_env;
extern SVECTOR fx_1fb_emitPos0;
extern s32 fx_1fb_fadeTimers[FX_CARBUNCLE_T_FADE_IN + 1]; /* fade-out / fade-in countdowns */
extern SVECTOR fx_1fb_emitPos1[4]; /* unit body positions */
extern s32 fx_1fb_op81Trails0[4][0x1FE];
extern s32 fx_1fb_op81Trails1[4][0x1FE];

#endif
