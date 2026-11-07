#ifndef FX_0B0_H
#define FX_0B0_H

/* Effect module fx_0b0 (dir 13 file 0xB0): enemy Leaf Swirl (Soulcage). Emitters only, two moved from the caster
 * bones to the sides. Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0b0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LEAF_SWIRL_YELLOW_LEAVES 0x1 /* emitter: helix trail whose run-time strip texture is a yellow maple leaf (4 frames): swirling yellow leaves (not ablated); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_LEAF_SWIRL_YELLOW_LEAVES_2 0x2 /* emitter: helix trail whose run-time strip texture is a yellow maple leaf (4 frames): swirling yellow leaves (not ablated); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_LEAF_SWIRL_DUST_SPECKS  0x3 /* emitter: brown dust puffs + small specks (btlfx_emitDots) */
#define FX_LEAF_SWIRL_OLIVE_RING   0x4 /* emitter: olive gouraud rings */
#define FX_LEAF_SWIRL_YELLOW_LEAVES_3 0xA /* emitter: spiral trail whose run-time strip texture is a yellow maple leaf (4 frames): swirling yellow leaves (not ablated); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_LEAF_SWIRL_YELLOW_LEAVES_4 0xB /* emitter: spiral trail whose run-time strip texture is a yellow maple leaf (4 frames): swirling yellow leaves (not ablated); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_LEAF_SWIRL_WIND_TRAIL   0xC /* emitter: grey wind trail */

/* fx_0b0 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_LEAF_SWIRL_STATE_SWIRL    0     /* op80's only state */
#define FX_LEAF_SWIRL_HEIGHT         0xF0  /* the swirl centre (posC) is this far above the targets' centre */
#define FX_LEAF_SWIRL_SIDE           0x300 /* the two leaf swirls end this far to either side of it (pos2C, pos34) */
#define FX_LEAF_SWIRL_GATHER_FRAMES  0x10  /* the leaves move (eased) from the unit's two bones to the sides over this many frames */
#define FX_LEAF_SWIRL_BURST_FRAME    0xE   /* DUST_SPECKS, OLIVE_RING, WIND_TRAIL and two more leaf spirals start */
#define FX_LEAF_SWIRL_FRAMES         0x37  /* op80's length (the op ends) */
#define FX_LEAF_SWIRL_HELIX_TRAIL_SLOTS  0x96 /* YELLOW_LEAVES and _2's trail buffers (fx_0b0_trails, _trails2): slots */
#define FX_LEAF_SWIRL_SPIRAL_TRAIL_SLOTS 0x12 /* YELLOW_LEAVES_3 and _4's four trail buffers (fx_0b0_trails4 .. 7): slots */
#define FX_LEAF_SWIRL_LEAF_TRAIL_STRIDE  0x14 /* all leaf trails: bytes per slot */
#define FX_LEAF_SWIRL_WIND_TRAIL_TRAIL_SLOTS  0x10 /* FX_LEAF_SWIRL_WIND_TRAIL's trail buffer fx_0b0_trails3: slots of */
#define FX_LEAF_SWIRL_WIND_TRAIL_TRAIL_STRIDE 0x6C /* this many bytes */

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
