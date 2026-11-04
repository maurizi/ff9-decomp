#ifndef FX_11F_H
#define FX_11F_H

/* Effect module fx_11f (dir 13 file 0x11F): item Dark Matter. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_11f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DARK_MATTER_YELLOW_RINGS 0x1 /* emitter: yellow and pink gouraud rings + a fan element */
#define FX_DARK_MATTER_VIOLET_RINGS 0x2 /* emitter: blue and violet gouraud rings */
#define FX_DARK_MATTER_BOLT_TRAIL   0x3 /* emitter: 3D trail with a violet lightning strip (+ a sprite, render black) */
#define FX_DARK_MATTER_BOLT_RINGS   0x4 /* emitter: yellow-white gouraud rings, rings textured with violet lightning, pink rings */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10;
} Fx11fWork;

extern Unk800F78A4 *fx_11f_env;
extern s32 fx_11f_op80Trails[];

#endif
