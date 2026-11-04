#ifndef FX_0B0_H
#define FX_0B0_H

/* Effect module fx_0b0 (dir 13 file 0xB0): enemy Leaf Swirl (Soulcage). Emitters only, two moved from the caster
 * bones to the sides. Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0b0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LEAF_SWIRL_LEAF_TRAIL   0x1 /* emitter: helix trail, mostly dark strip with a leaf tip (look pending) */
#define FX_LEAF_SWIRL_LEAF_TRAIL_2 0x2 /* emitter: helix trail, mostly dark strip with a leaf tip (look pending) */
#define FX_LEAF_SWIRL_LEAVES       0x3 /* emitter: dust puffs + leaf sprites (texture not in the file) (look pending) */
#define FX_LEAF_SWIRL_OLIVE_RING   0x4 /* emitter: olive gouraud rings */
#define FX_LEAF_SWIRL_LEAF_TRAIL_3 0xA /* emitter: spiral trail, mostly dark strip with a leaf tip (look pending) */
#define FX_LEAF_SWIRL_LEAF_TRAIL_4 0xB /* emitter: spiral trail, mostly dark strip with a leaf tip (look pending) */
#define FX_LEAF_SWIRL_WIND_TRAIL   0xC /* emitter: grey wind trail */

typedef struct {
    s32 state0; /* state */
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* pos14 raised by 0xF0 */
    SVECTOR pos14; /* position: env->targets->centerX */
    SVECTOR pos1C; /* unit 0x10 bone fx_0b0_bones[0] */
    SVECTOR pos24; /* unit 0x10 bone fx_0b0_bones[1] */
    SVECTOR pos2C; /* posC, 0x300 to one side */
    SVECTOR pos34; /* posC, 0x300 to the other side */
    SVECTOR pos3C;
    SVECTOR pos44;
} Fx0b0Work;

extern u16 fx_0b0_bones[2]; /* bones */
extern Unk800F78A4 *fx_0b0_env;
extern s32 fx_0b0_trails[];
extern s32 fx_0b0_trails2[];
extern s32 fx_0b0_trails3[];
extern s32 fx_0b0_trails4[];
extern s32 fx_0b0_trails5[];
extern s32 fx_0b0_trails6[];
extern s32 fx_0b0_trails7[];

#endif
