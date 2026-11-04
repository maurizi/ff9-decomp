#ifndef FX_0F8_H
#define FX_0F8_H

/* Effect module fx_0f8 (dir 13 file 0x0F8): Grand Lethal. Emitters aimed at the target centre, four emitter spawns
 * around it and backdrops. Evidence: [data] the party ability / item tables and enemy sequences that play this file
 * id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f8 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_GRAND_LETHAL_BACKDROP     0x1 /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_GRAND_LETHAL_BACKDROP_2   0x2 /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_GRAND_LETHAL_PINK_FLARES  0x3 /* emitter: pink lens-flare glows and streaks */
#define FX_GRAND_LETHAL_SPRITE_RINGS 0xD /* emitter: white / cyan / blue / grey gouraud rings with a sprite that renders as a flat violet square (texture not in the file) (look pending) */
#define FX_GRAND_LETHAL_SWIRL_RINGS  0xE /* emitter: white swirl sprite with textured rings; the rings still read garbled multicolour texels (texture not resolved) (look pending) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    SVECTOR centerC;
    SVECTOR aimRot14;
} Fx0f8Work80;

typedef struct {
    Pool6CEntry *emit0[4];
    Pool6CEntry *emit10[4];
    Pool6CEntry *emit20;
    Pool6CEntry *emit24;
    SVECTOR pos28;
    SVECTOR emitPos30[4];
} Fx0f8Work81;

typedef struct {
    u16 block0; /* file */
    u16 frame2; /* frame */
} Fx0f8Spawn;

extern Fx0f8Spawn fx_0f8_spawns[4];
extern Unk800F78A4 *fx_0f8_env;

#endif
