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

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
    SVECTOR pos10;
    s32 index18;
} Fx113Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx113Work81;

extern Unk800F78A4 *fx_113_env;
extern s32 fx_113_op81Trails[];

#endif
