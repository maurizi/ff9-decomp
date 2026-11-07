#ifndef FX_007_H
#define FX_007_H

/* Effect module fx_007 (dir 13 file 0x07): Firaga, one target (party and enemy). Fire / ice drawn as a grid of
 * texel quads (fxm_drawTexelQuads). Evidence: [data] the party ability / item tables and enemy sequences that play
 * this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_007 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FIRAGA_FLAME_PILLAR  0x8 /* model: open cylinder with a flame texture (the pillar) */
#define FX_FIRAGA_FLAME_DISC    0x9 /* model: flat octagon, flame texture; registered, never drawn */
#define FX_FIRAGA_FLAME_BURST   0xA /* emitter: flame explosion */
#define FX_FIRAGA_GLOW          0xB /* rings: white-red gouraud glow disc at the target */
#define FX_FIRAGA_EMBER_TRAIL   0xC /* emitter: orbiting red glow trail + red ember dots */
#define FX_FIRAGA_COLUMN_SPARK  0xD /* rings: small red-white glow drawn at random fire-column segments */
#define FX_FIRAGA_FLAME_BURST_2 0xE /* emitter: flame explosion at the pillar's start */
#define FX_FIRAGA_PILLAR_RING   0xF /* rings: textured flame ring rising with the pillar */
#define FX_FIRAGA_GOLD_SPARKS   0x10 /* emitter: golden spark sprites */
#define FX_FIRAGA_GOLD_SPARKS_2 0x11 /* emitter: golden spark sprites + spark sub-particles */
#define FX_FIRAGA_EMBERS        0x12 /* emitter: dark red ember sprites */
#define FX_FIRAGA_SMOKE_BURST   0x13 /* emitter: dark teal-tinted burst (smoke after the blast) */
#define FX_FIRAGA_EMBER_CONE    0x14 /* emitter: cone of red and grey ember dots */
#define FX_FIRAGA_RED_HAZE      0x15 /* emitter: soft dark-red additive blob, the red heat haze filling the fire column; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_FIRAGA_FLAME_RIBBON_0 0x16 /* 'so' model: flat flame-textured ribbon piece, drawn coiled 18 times as the fire column */
#define FX_FIRAGA_FLAME_RIBBON_1 0x17 /* 'so' model: flat flame-textured ribbon piece */
#define FX_FIRAGA_FLAME_RIBBON_2 0x18 /* 'so' model: flat flame-textured ribbon piece */
#define FX_FIRAGA_FLAME_RIBBON_3 0x19 /* 'so' model: flat flame-textured ribbon piece */

/* fx_007 choreography (docs/fxdata.md, "Choreography literals"): op80's states, their lengths and keyframes, the
 * flame column's shape, and the VRAM rects of the pillar's flame texture and the floor glow frames. */
#define FX_FIRAGA_STATE_GLOW   0 /* FX_FIRAGA_GLOW grows at the target */
#define FX_FIRAGA_STATE_FLASH  1 /* red screen flash, FX_FIRAGA_FLAME_BURST and the sparks */
#define FX_FIRAGA_STATE_PILLAR 2 /* FX_FIRAGA_FLAME_PILLAR, FX_FIRAGA_PILLAR_RING and the coiled flame column */
#define FX_FIRAGA_STATE_TAIL   3 /* draws nothing, waits for the emitters */
#define FX_FIRAGA_GLOW_FRAMES         0x18 /* FX_FIRAGA_STATE_GLOW's length; at its end FX_FIRAGA_FLAME_BURST starts */
#define FX_FIRAGA_FLASH_FRAMES        8    /* FX_FIRAGA_STATE_FLASH's length */
#define FX_FIRAGA_SPARKS_FRAME        4    /* FX_FIRAGA_STATE_FLASH: FX_FIRAGA_EMBERS, GOLD_SPARKS and GOLD_SPARKS_2 start */
#define FX_FIRAGA_PILLAR_FRAMES       0x2E /* FX_FIRAGA_STATE_PILLAR's length */
#define FX_FIRAGA_WHITE_FLASH_FRAMES  4    /* FX_FIRAGA_STATE_PILLAR: additive white screen flash */
#define FX_FIRAGA_RED_TINT_FRAMES     0x20 /* FX_FIRAGA_STATE_PILLAR: flickering opaque red screen tint */
#define FX_FIRAGA_FLAME_BURST_2_FRAME 2    /* FX_FIRAGA_STATE_PILLAR: FX_FIRAGA_FLAME_BURST_2 starts */
#define FX_FIRAGA_RED_HAZE_FRAME      3    /* FX_FIRAGA_STATE_PILLAR: FX_FIRAGA_RED_HAZE and FX_FIRAGA_EMBER_CONE start */
#define FX_FIRAGA_SMOKE_BURST_FRAME   0xE  /* FX_FIRAGA_STATE_PILLAR: FX_FIRAGA_SMOKE_BURST starts */
#define FX_FIRAGA_PILLAR_RING_FRAMES  0x1D /* FX_FIRAGA_STATE_PILLAR: FX_FIRAGA_PILLAR_RING and FX_FIRAGA_FLAME_PILLAR are drawn */
#define FX_FIRAGA_COLUMN_FRAMES       0x24 /* FX_FIRAGA_STATE_PILLAR: the FX_FIRAGA_FLAME_RIBBON_n column is drawn */
#define FX_FIRAGA_COLUMN_GROW_FRAMES  0x10 /* the column's radius grows, then swells for the rest of COLUMN_FRAMES */
#define FX_FIRAGA_COLUMN_RISE_FRAMES  6    /* the column rises to its full height */
#define FX_FIRAGA_COLUMN_RADIUS       0x2BC /* the column's base radius (segment x / z scale) */
#define FX_FIRAGA_COLUMN_SEGMENTS     18   /* ribbon pieces coiled up the column (FX_FIRAGA_FLAME_RIBBON_0..3 in turn) */
#define FX_FIRAGA_COLUMN_SPARKS       3    /* at most this many FX_FIRAGA_COLUMN_SPARK rings per frame */
#define FX_FIRAGA_TAIL_FRAMES         0x10 /* FX_FIRAGA_STATE_TAIL's length (the op ends) */
#define FX_FIRAGA_MODEL_COUNT         6    /* btlfx_initEffModels slots: 4 ribbons, FLAME_PILLAR, FLAME_DISC */
#define FX_FIRAGA_EMBER_TRAIL_SLOTS   0x12 /* FX_FIRAGA_EMBER_TRAIL's trail buffer fx_007_trails: slots of */
#define FX_FIRAGA_EMBER_TRAIL_STRIDE  0x24 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
/* FX_FIRAGA_FLAME_PILLAR samples (tpage 0x37 = 4-bit page (0x1C0, 0x100), u / v 0x80) the rect the file fills with a
 * "ring_1 dummy" placeholder; while the pillar shows, btlfx_drawScrollMove copies the file's flame texture from
 * (SRC_X, SRC_Y) into it, scrolled by the spin. */
#define FX_FIRAGA_PILLAR_FLAME_VRAM_X 0x1E0
#define FX_FIRAGA_PILLAR_FLAME_VRAM_Y 0x180
#define FX_FIRAGA_PILLAR_FLAME_VRAM_W 0x20
#define FX_FIRAGA_PILLAR_FLAME_VRAM_H 0x80
#define FX_FIRAGA_PILLAR_FLAME_SRC_X  0x1E0
#define FX_FIRAGA_PILLAR_FLAME_SRC_Y  0x100
/* The floor glow (states GLOW to PILLAR): a gouraud grid flat at the target whose vertex colours are a 16x16 frame
 * of the file's dark-red glow animation, read back with StoreImage: 8 frames in 2 columns at
 * (0x1C0 + X, 0x100 + Y) of the 15-bit page TPAGE. */
#define FX_FIRAGA_FLOOR_GLOW_TPAGE    0x117 /* 15-bit page (0x1C0, 0x100) */
#define FX_FIRAGA_FLOOR_GLOW_X        0
#define FX_FIRAGA_FLOOR_GLOW_Y        0x30
#define FX_FIRAGA_FLOOR_GLOW_COLS     2    /* frames per row */
#define FX_FIRAGA_FLOOR_GLOW_FRAME_MASK 7  /* frame34 & mask: 8 frames */
#define FX_FIRAGA_FLOOR_GLOW_CELL     0x100 /* grid cell size */
#define FX_FIRAGA_FLOOR_GLOW_GRID     0x10 /* grid cells (and frame texels) per side */

/* PsyQ gte_stszotz: SZ3 / 4 */
#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
/* VZ0 = VZ1 = VZ2 = 0 */
#define gte_ldVZ012_0() __asm__ volatile( \
    "mtc2\t$0, $1;" \
    "mtc2\t$0, $3;" \
    "mtc2\t$0, $5")

/* V0..V2 x and y only (VZ0..VZ2 are loaded once with gte_ldVZ012_0) */
#define gte_ldv3xy(r0, r1, r2) __asm__ volatile( \
    "lwc2\t$0, 0(%0);" \
    "lwc2\t$2, 0(%1);" \
    "lwc2\t$4, 0(%2)" \
    : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldv0xy(r0) __asm__ volatile( \
    "lwc2\t$0, 0(%0)" \
    : : "r"(r0))

extern Unk800F78A4 *fx_007_env;

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

typedef struct {
    SVECTOR pos0; /* target body position */
    SVECTOR pos8; /* target position */
    s32 state10; /* state */
    s32 model14[4]; /* models */
    s32 model24;
    s32 model28;
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    s32 frame34; /* frame */
} Fx007Work80; /* 0x38 */

extern BtlModelBuf fx_007_effModels[];
extern u8 fx_007_trails[];
extern u16 fx_007_texels[];

#endif
