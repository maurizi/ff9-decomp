#ifndef FX_0D6_H
#define FX_0D6_H

/* Effect module fx_0d6 (dir 13 file 0xD6): Sword Magic Firaga. Fire / ice drawn as a grid of texel quads
 * (fxm_drawTexelQuads). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0d6 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_FIRAGA_FLAME_PILLAR  0x9 /* model: open cylinder with a flame texture (the pillar) (byte-identical to fx_007 entry 0x8) */
#define FX_SWDMAG_FIRAGA_FLAME_DISC    0xB /* model: flat octagon, flame texture; registered, never drawn (byte-identical to fx_007 entry 0x9) */
#define FX_SWDMAG_FIRAGA_FLAME_BURST   0xC /* emitter: flame explosion (byte-identical to fx_007 entry 0xa) */
#define FX_SWDMAG_FIRAGA_GLOW          0xD /* rings: white-red gouraud glow disc at the target (byte-identical to fx_007 entry 0xb) */
#define FX_SWDMAG_FIRAGA_EMBER_TRAIL   0xE /* emitter: orbiting red glow trail with red ember dots at the start (byte-identical to fx_007 entry 0xc) */
#define FX_SWDMAG_FIRAGA_COLUMN_SPARK  0xF /* rings: small red-white glow drawn at random fire-column segments (byte-identical to fx_007 entry 0xd) */
#define FX_SWDMAG_FIRAGA_FLAME_BURST_2 0x10 /* emitter: flame explosion at the pillar's start (byte-identical to fx_007 entry 0xe) */
#define FX_SWDMAG_FIRAGA_PILLAR_RING   0x11 /* rings: textured flame ring rising with the pillar (byte-identical to fx_007 entry 0xf) */
#define FX_SWDMAG_FIRAGA_EMBERS        0x12 /* emitter: dark red ember sprites (byte-identical to fx_007 entry 0x12) */
#define FX_SWDMAG_FIRAGA_GOLD_SPARKS_2 0x13 /* emitter: golden spark sprites + spark sub-particles (byte-identical to fx_007 entry 0x11) */
#define FX_SWDMAG_FIRAGA_GOLD_SPARKS   0x14 /* emitter: golden spark sprites (byte-identical to fx_007 entry 0x10) */
#define FX_SWDMAG_FIRAGA_SMOKE_BURST   0x15 /* emitter: dark teal-tinted burst (smoke after the blast) (byte-identical to fx_007 entry 0x13) */
#define FX_SWDMAG_FIRAGA_RED_HAZE      0x16 /* emitter: soft dark-red additive blob, Firaga's red heat haze (byte-identical to fx_007 entry 0x15); runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_SWDMAG_FIRAGA_EMBER_CONE 0x17 /* emitter: cone of red and white ember dots (byte-identical to fx_007 entry 0x14) */
#define FX_SWDMAG_FIRAGA_SWORD_GLINT   0x18 /* rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat */
#define FX_SWDMAG_FIRAGA_SWORD_FLASH 0x19 /* rings: white 8-segment rings flashed rotating at the sword (byte-identical to the SWORD_FLASH of every Sword Magic module) */
#define FX_SWDMAG_FIRAGA_FLAME_RIBBON_1 0x1B /* 'so' model: twisted flame ribbon with a fire texture (byte-identical to fx_007 entry 0x17) */
#define FX_SWDMAG_FIRAGA_FLAME_RIBBON_0 0x1C /* 'so' model: twisted flame ribbon with a fire texture, drawn coiled 18 times (byte-identical to fx_007 entry 0x16) */
#define FX_SWDMAG_FIRAGA_FLAME_RIBBON_2 0x1D /* 'so' model: twisted flame ribbon with a fire texture (byte-identical to fx_007 entry 0x18) */
#define FX_SWDMAG_FIRAGA_FLAME_RIBBON_3 0x1E /* 'so' model: twisted flame ribbon with a fire texture (byte-identical to fx_007 entry 0x19) */

/* fx_0d6 choreography (docs/fxdata.md, "Choreography literals", names as fx_048's Firaga, which op80 twins from
 * state FLASH on): op80's states, their lengths and keyframes, the flame column, the VRAM rects of the pillar's
 * flame texture and the floor glow frames; op84's sword-flash states. */
#define FX_SWDMAG_FIRAGA_STATE_FLASH  1 /* red screen flash, FX_SWDMAG_FIRAGA_GLOW and the sparks (the op starts here) */
#define FX_SWDMAG_FIRAGA_STATE_PILLAR 2 /* FLAME_PILLAR, PILLAR_RING and the coiled flame column */
#define FX_SWDMAG_FIRAGA_STATE_TAIL   3 /* draws nothing, waits for the emitters */
#define FX_SWDMAG_FIRAGA_FLASH_FRAMES        8    /* FX_SWDMAG_FIRAGA_STATE_FLASH's length */
#define FX_SWDMAG_FIRAGA_SPARKS_FRAME        4    /* STATE_FLASH: FX_SWDMAG_FIRAGA_EMBERS, GOLD_SPARKS and GOLD_SPARKS_2 start */
#define FX_SWDMAG_FIRAGA_PILLAR_FRAMES       0x2E /* FX_SWDMAG_FIRAGA_STATE_PILLAR's length */
#define FX_SWDMAG_FIRAGA_WHITE_FLASH_FRAMES  4    /* STATE_PILLAR: additive white screen flash */
#define FX_SWDMAG_FIRAGA_RED_TINT_FRAMES     0x20 /* STATE_PILLAR: flickering opaque red screen tint */
#define FX_SWDMAG_FIRAGA_FLAME_BURST_2_FRAME 2    /* STATE_PILLAR: FX_SWDMAG_FIRAGA_FLAME_BURST_2 starts */
#define FX_SWDMAG_FIRAGA_RED_HAZE_FRAME      3    /* STATE_PILLAR: FX_SWDMAG_FIRAGA_RED_HAZE and EMBER_CONE start */
#define FX_SWDMAG_FIRAGA_SMOKE_BURST_FRAME   0xE  /* STATE_PILLAR: FX_SWDMAG_FIRAGA_SMOKE_BURST starts */
#define FX_SWDMAG_FIRAGA_PILLAR_RING_FRAMES  0x1D /* STATE_PILLAR: PILLAR_RING and FLAME_PILLAR are drawn */
#define FX_SWDMAG_FIRAGA_COLUMN_FRAMES       0x24 /* STATE_PILLAR: the FLAME_RIBBON_n column is drawn */
#define FX_SWDMAG_FIRAGA_COLUMN_GROW_FRAMES  0x10 /* the column's radius grows, then swells for the rest of COLUMN_FRAMES */
#define FX_SWDMAG_FIRAGA_COLUMN_RISE_FRAMES  6    /* the column rises to its full height */
#define FX_SWDMAG_FIRAGA_COLUMN_RADIUS       0x2BC /* the column's base radius (segment x / z scale) */
#define FX_SWDMAG_FIRAGA_COLUMN_SEGMENTS     18   /* ribbon pieces coiled up the column (FLAME_RIBBON_0..3 in turn) */
#define FX_SWDMAG_FIRAGA_COLUMN_SPARKS       3    /* at most this many FX_SWDMAG_FIRAGA_COLUMN_SPARK rings per frame */
#define FX_SWDMAG_FIRAGA_TAIL_FRAMES         0x10 /* FX_SWDMAG_FIRAGA_STATE_TAIL's length (the op ends) */
#define FX_SWDMAG_FIRAGA_MODEL_COUNT         6    /* btlfx_initEffModels slots: 4 ribbons, FLAME_PILLAR, FLAME_DISC */
#define FX_SWDMAG_FIRAGA_EMBER_TRAIL_SLOTS   0x12 /* FX_SWDMAG_FIRAGA_EMBER_TRAIL's trail buffer fx_0d6_trails: slots of */
#define FX_SWDMAG_FIRAGA_EMBER_TRAIL_STRIDE  0x24 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
/* FLAME_PILLAR and FLAME_DISC's texture (Hi_RegisterTexEffModel): FLAME_PILLAR samples the rect the file fills with a
 * "ring_1 dummy" placeholder; while the pillar shows, btlfx_drawScrollMove copies the file's flame texture from
 * (SRC_X, SRC_Y) into it, scrolled by the spin. */
#define FX_SWDMAG_FIRAGA_FLAME_PILLAR_TPAGE  0x37   /* 4-bit page (0x1C0, 0x100) */
#define FX_SWDMAG_FIRAGA_FLAME_PILLAR_CLUT   0x3D06 /* CLUT (0x60, 0xF4) */
#define FX_SWDMAG_FIRAGA_FLAME_PILLAR_U      0x80
#define FX_SWDMAG_FIRAGA_FLAME_PILLAR_V      0x80
#define FX_SWDMAG_FIRAGA_PILLAR_FLAME_VRAM_X 0x1E0
#define FX_SWDMAG_FIRAGA_PILLAR_FLAME_VRAM_Y 0x180
#define FX_SWDMAG_FIRAGA_PILLAR_FLAME_VRAM_W 0x20
#define FX_SWDMAG_FIRAGA_PILLAR_FLAME_VRAM_H 0x80
#define FX_SWDMAG_FIRAGA_PILLAR_FLAME_SRC_X  0x1E0
#define FX_SWDMAG_FIRAGA_PILLAR_FLAME_SRC_Y  0x100
/* The floor glow (states FLASH and PILLAR): a gouraud grid flat at the target whose vertex colours are a 16x16 frame
 * of the file's dark-red glow animation, read back with StoreImage: 8 frames in 2 columns at
 * (0x1C0 + X, 0x100 + Y) of the 15-bit page TPAGE. */
#define FX_SWDMAG_FIRAGA_FLOOR_GLOW_TPAGE    0x117 /* 15-bit page (0x1C0, 0x100) */
#define FX_SWDMAG_FIRAGA_FLOOR_GLOW_X        0
#define FX_SWDMAG_FIRAGA_FLOOR_GLOW_Y        0x30
#define FX_SWDMAG_FIRAGA_FLOOR_GLOW_COLS     2    /* frames per row */
#define FX_SWDMAG_FIRAGA_FLOOR_GLOW_FRAME_MASK 7  /* frame34 & mask: 8 frames */
#define FX_SWDMAG_FIRAGA_FLOOR_GLOW_CELL     0x100 /* grid cell size */
#define FX_SWDMAG_FIRAGA_FLOOR_GLOW_GRID     0x10 /* grid cells (and frame texels) per side */
#define FX_SWDMAG_FIRAGA_FLASH_STATE_IN      0 /* op84: SWORD_GLINT and SWORD_FLASH open up at the sword */
#define FX_SWDMAG_FIRAGA_FLASH_STATE_FLICKER 1 /* op84: they flicker and grow */
#define FX_SWDMAG_FIRAGA_FLASH_STATE_FADE    2 /* op84: SWORD_FLASH turns and they fade */
#define FX_SWDMAG_FIRAGA_FLASH_IN_FRAMES      3 /* FX_SWDMAG_FIRAGA_FLASH_STATE_IN's length */
#define FX_SWDMAG_FIRAGA_FLASH_FLICKER_FRAMES 9 /* FX_SWDMAG_FIRAGA_FLASH_STATE_FLICKER's length */
#define FX_SWDMAG_FIRAGA_FLASH_FADE_FRAMES    9 /* FX_SWDMAG_FIRAGA_FLASH_STATE_FADE's length (the op ends) */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d6Work84;

extern Unk800F78A4 *fx_0d6_env;

typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR pos8; /* caster position */
    s32 state10; /* state */
    s32 model14[4]; /* models */
    s32 model24; /* model */
    s32 model28; /* model */
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    s32 frame34; /* frame counter */
} Fx0d6Work80; /* 0x38 */

extern BtlModelBuf fx_0d6_effModels[];
extern s32 fx_0d6_trails[];
extern u16 fx_0d6_texels[];
s32 fx_0d6_op80(s32 mode, Fx0d6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */

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
static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

#endif
