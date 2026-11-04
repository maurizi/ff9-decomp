#ifndef FX_0BB_H
#define FX_0BB_H

/* Effect module fx_0bb (dir 13 file 0xBB): Mustard Bomb; enemy Mustard Bomb (Drakan, ...). Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0bb effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MUSTARD_BOMB_BACKDROP        0x1 /* table: background layers for btlfx_drawBackdrop */
#define FX_MUSTARD_BOMB_FIREBALLS       0x7 /* emitter: orange fireball sprites with light streaks */
#define FX_MUSTARD_BOMB_EXPLOSION_RINGS 0x9 /* emitter: yellow-orange-red and blue gouraud rings (+ a textured ring, texture not in the file) */
#define FX_MUSTARD_BOMB_BLAST_RINGS     0xA /* emitter: cone of yellow / orange / brown gouraud rings */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
} Fx0bbWork80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
} Fx0bbWork81;

extern Unk800F78A4 *fx_0bb_env;

#endif
