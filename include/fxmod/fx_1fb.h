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

extern Unk800F78A4 *fx_1fb_env;
extern SVECTOR fx_1fb_emitPos0;
extern s32 fx_1fb_fadeTimers[2]; /* fade-out / fade-in countdowns */
extern SVECTOR fx_1fb_emitPos1[4]; /* unit body positions */
extern s32 fx_1fb_op81Trails0[4][0x1FE];
extern s32 fx_1fb_op81Trails1[4][0x1FE];

#endif
