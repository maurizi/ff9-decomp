#ifndef FX_1AD_H
#define FX_1AD_H

/* Effect module fx_1ad (dir 13 file 0x1ad): enemy 6 Legs (Gigan Octopus). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1ad effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_6_LEGS_BACKDROP     0x1 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_6_LEGS_SLIME_SHELL 0x2 /* 'so' model: rounded pod with an opening; page 0x117 holds a green slimy texture at run time: the green slime shell wrapping the target; runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_6_LEGS_BUBBLE_TRAIL 0x4 /* emitter: orbiting 3D trail of glass bubbles (sprites render black) */
#define FX_6_LEGS_GREEN_TRAIL  0x5 /* emitter: screen trail, green striped strip */

/* fx_1ad choreography (docs/fxdata.md, "Choreography literals"): the green trail running down the six legs' bone
 * chains, the bubble trail, the pulsing slime shell on the caster's bone and its scrolled texture, op80's length. */
#define FX_6_LEGS_LEGS        6    /* bone chains (fx_1ad_boneChains rows: a count, then the bones of one leg) */
#define FX_6_LEGS_CHAIN_BONES 8    /* bones per chain */
#define FX_6_LEGS_SHELL_BONE  0xF  /* unit 0x10's bone the slime shell sits on */
#define FX_6_LEGS_SHELL_ROT_BONE 0x10 /* the bone whose angles turn it */
#define FX_6_LEGS_SLIME_VRAM_X 0x200 /* btlfx_drawScrollMove: the shell's 32x64 texture rect (inside page 0x117) */
#define FX_6_LEGS_SLIME_VRAM_Y 0x100
#define FX_6_LEGS_SLIME_VRAM_W 0x20
#define FX_6_LEGS_SLIME_SRC_X  0x240 /* copied from a 4x4 grid of frames from here on (one per tick) */
#define FX_6_LEGS_SLIME_SRC_Y  0x100
#define FX_6_LEGS_GREEN_TRAIL_TRAIL_SLOTS   0x48 /* GREEN_TRAIL's trail buffer fx_1ad_op80Trails0: slots of */
#define FX_6_LEGS_GREEN_TRAIL_TRAIL_STRIDE  0x3C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_6_LEGS_BUBBLE_TRAIL_TRAIL_SLOTS  0x10 /* BUBBLE_TRAIL's trail buffer fx_1ad_op80Trails1: slots of */
#define FX_6_LEGS_BUBBLE_TRAIL_TRAIL_STRIDE 0x6C /* this many bytes */
#define FX_6_LEGS_SHELL_FRAME 0xC  /* the shell draws (and fades in and out over the rest of the op) from this frame */
#define FX_6_LEGS_FRAMES      0x45 /* op80's length (the op ends) */

typedef struct {
    s32 done0; /* done */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* bone position */
    SVECTOR pos10; /* target body position */
    u8 pad18[0x20 - 0x18];
    Pool6CEntry *emit20;
    s32 model24; /* model */
} Fx1adWork; /* 0x28 */

extern u16 fx_1ad_boneChains[FX_6_LEGS_LEGS][1 + FX_6_LEGS_CHAIN_BONES]; /* per row: count, bones */
extern Unk800F78A4 *fx_1ad_env;
extern BtlModelBuf fx_1ad_effModels[];
extern s32 fx_1ad_op80Trails0[];
extern s32 fx_1ad_op80Trails1[];

static s32 fx_1ad_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
