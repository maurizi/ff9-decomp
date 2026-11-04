#ifndef FX_08A_H
#define FX_08A_H

/* Effect module fx_08a (dir 13 file 0x8A): Auto-Life. Emitters only. Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_08a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_AUTO_LIFE_SPIRAL_TRAILS 0x6 /* emitter: white trails spiralling, red specks, pink wing */
#define FX_AUTO_LIFE_FLASH_RINGS   0x7 /* emitter: white-pink and white-blue gouraud rings */
#define FX_AUTO_LIFE_GOLD_WINGS    0x9 /* emitter: golden wing sprites */
#define FX_AUTO_LIFE_GOLD_WINGS_2  0xA /* emitter: golden wing sprites */

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* caster body position */
} Fx08aWork;

extern Unk800F78A4 *fx_08a_env;
extern s32 fx_08a_trails[];

#endif
