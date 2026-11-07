#ifndef FX_113_H
#define FX_113_H

/* Effect module fx_113 (dir 13 file 0x113): enemy Virus Combo (Mover). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_113 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_VIRUS_COMBO_STREAK_TRAIL 0x1 /* emitter: screen trail spiralling in a cone, grey streak strip */
#define FX_VIRUS_COMBO_BURST        0x2 /* emitter: pink-yellow and white-red gouraud rings + speck sprites (texture not in the file) */
#define FX_VIRUS_COMBO_BURST_2      0x3 /* emitter: pink-yellow and white-red gouraud rings + speck sprites (texture not in the file) */
#define FX_VIRUS_COMBO_BURST_3      0x4 /* emitter: pink-yellow and white-red gouraud rings + speck sprites (texture not in the file) */
#define FX_VIRUS_COMBO_PUFF         0x5 /* emitter: pink-white puffs fading in a cone */

/* fx_113 choreography (docs/fxdata.md, "Choreography literals"): op80's per-hit bursts, op81's trail buffer and the
 * ops' lengths. */
#define FX_VIRUS_COMBO_HITS                3    /* op80's argument picks BURST / BURST_2 / BURST_3 (each at its own offset); others end at once */
#define FX_VIRUS_COMBO_BURST_FRAMES        0x28 /* op80's length */
#define FX_VIRUS_COMBO_STREAK_TRAIL_SLOTS  0xD  /* op81: STREAK_TRAIL's trail buffer (fx_113_op81Trails): slots */
#define FX_VIRUS_COMBO_STREAK_TRAIL_STRIDE 0x50 /* bytes per slot */
#define FX_VIRUS_COMBO_STREAK_FRAMES       0x28 /* op81's length */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
    SVECTOR pos10;
    s32 index18;
} Fx113Work80; /* 0x1C */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx113Work81; /* 0xC */

extern Unk800F78A4 *fx_113_env;
extern s32 fx_113_op81Trails[];

#endif
