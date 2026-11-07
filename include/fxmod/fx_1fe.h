#ifndef FX_1FE_H
#define FX_1FE_H

/* Effect module fx_1fe (dir 13 file 0x1fe): Phoenix, short summon. Fire / ice drawn as a grid of texel quads
 * (fxm_drawTexelQuads). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_1fe effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PHOENIX_FIRE_DISC 0x2 /* 'so' model: flat disc with a fire texture (byte-identical to fx_0d3 entry 0xD) */
#define FX_PHOENIX_FIRE_DOME 0xD /* 'so' model: dome with a hole, fire texture (byte-identical to fx_0d3 entry 0xC) */
#define FX_PHOENIX_SMOKE_ARC 0xF /* 'so' model: thin arc band with a grey smoke texture (byte-identical to fx_0d3 entry 0xE) */
#define FX_PHOENIX_FIRE_FEATHERS    0x10 /* emitter: orbiting fire-textured trails and flame-feather quads (same call site as fx_0d3 entry 0x10, same kind) */
#define FX_PHOENIX_PINK_RINGS       0x11 /* emitter: white / pink / copper gouraud rings (byte-identical to fx_0d3 entry 0x11) */
#define FX_PHOENIX_GOLD_FEATHERS    0x12 /* emitter: golden feather quads (same call site as fx_0d3 entry 0x12, same kind) */
#define FX_PHOENIX_FLAME_FEATHERS_2 0x15 /* emitter: copper flame-feather quads (cylinder) (same call site as fx_0d3 entry 0x18, same kind) */
#define FX_PHOENIX_EMBERS           0x16 /* emitter: soft orange glowing mote sprites, the small embers drifting after the white-out (same call site as fx_0d3 entry 0xF, same kind); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_PHOENIX_FLAME_FEATHERS   0x17 /* emitter: copper flame-feather quads (same call site as fx_0d3 entry 0x17, same kind) */

/* fx_1fe choreography (docs/fxdata.md, "Choreography literals"): op81's three states, Phoenix's (fx_0d3) op81 states
 * FEATHERS, BURST and TAIL with fx_0d3's names and the short form's own values, and the scrolled textures. */
#define FX_PHOENIX_MODEL_COUNT    7    /* op81: btlfx_initEffModels slots */
#define FX_PHOENIX_STATE_FEATHERS 1    /* op81 (the first state): FIRE_FEATHERS rise, the camera with them */
#define FX_PHOENIX_STATE_BURST    2    /* op81: PINK_RINGS, FLAME_FEATHERS_2, GOLD_FEATHERS, FIRE_DISC / FIRE_DOME, the SMOKE_ARC
                                        * wave, screen tints */
#define FX_PHOENIX_STATE_TAIL     6    /* op81: white fade-out, EMBERS, FLAME_FEATHERS */
#define FX_PHOENIX_FEATHERS_STATE_FRAMES 0x25 /* FX_PHOENIX_STATE_FEATHERS' length (the camera's rise ramp) */
#define FX_PHOENIX_FIRE_FEATHERS_TRAIL_SLOTS  0x40 /* FX_PHOENIX_FIRE_FEATHERS' trail buffer: slots of */
#define FX_PHOENIX_FIRE_FEATHERS_TRAIL_STRIDE 0x1C /* this many bytes */
#define FX_PHOENIX_BLUE_FRAME      5    /* FX_PHOENIX_STATE_BURST: one frame of additive blue screen tint, */
#define FX_PHOENIX_BLUE_RGB        0, 0, 0x64
#define FX_PHOENIX_DARKEN_FRAME    6    /*   then one of subtractive tint, */
#define FX_PHOENIX_DARKEN_RGB      0, 0x80, 0xFF
#define FX_PHOENIX_FLASH_FRAME     7    /*   then an additive yellow-white flash fading over */
#define FX_PHOENIX_FLASH_FRAMES    8    /*   this many frames */
#define FX_PHOENIX_GOLD_FEATHERS_FRAME 7 /*  GOLD_FEATHERS start; FIRE_DISC and FIRE_DOME grow to the state's end, the SMOKE_ARC
                                          *  wave rises */
#define FX_PHOENIX_WAVE_FRAMES     20   /*   over this many frames, */
#define FX_PHOENIX_SMOKE_ARCS      7    /*   SMOKE_ARC drawn this many times up the wave */
#define FX_PHOENIX_WHITE_IN_FRAME  0x18 /*   the additive white fade-in starts (8 frames) */
#define FX_PHOENIX_BURST_FRAMES    0x20 /* FX_PHOENIX_STATE_BURST's length */
#define FX_PHOENIX_WHITE_OUT_FRAMES 0x18 /* FX_PHOENIX_STATE_TAIL: the white fades out over this many frames */
#define FX_PHOENIX_FLAME_FEATHERS_3_FRAME 0x20 /* FX_PHOENIX_STATE_TAIL: FX_PHOENIX_FLAME_FEATHERS start (fx_0d3's FLAME_FEATHERS_3) */
#define FX_PHOENIX_TAIL_FRAMES     0x57 /* FX_PHOENIX_STATE_TAIL's length (the op ends) */
/* Scrolling textures (fx_0d3's rects): FIRE_DOME (tpage 0x9B = 8-bit page (0x2C0, 0x100)), FIRE_DISC (tpage 0x1A =
 * 4-bit page (0x280, 0x100)). */
#define FX_PHOENIX_DOME_VRAM_X  0x2C0
#define FX_PHOENIX_DOME_VRAM_Y  0x100
#define FX_PHOENIX_DOME_VRAM_W  0x40
#define FX_PHOENIX_DOME_VRAM_H  0x80
#define FX_PHOENIX_DOME_SRC_X   0x2C0
#define FX_PHOENIX_DOME_SRC_Y   0x180
#define FX_PHOENIX_DISC_VRAM_X  0x280
#define FX_PHOENIX_DISC_VRAM_Y  0x100
#define FX_PHOENIX_DISC_VRAM_W  0x20
#define FX_PHOENIX_DISC_VRAM_H  0x80
#define FX_PHOENIX_DISC_SRC_X   0x2A0
#define FX_PHOENIX_DISC_SRC_Y   0x100

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR unk8; /* write-only (copy of fx_1fe_basePos0) */
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR camTarget20; /* camera target */
    u8 pad28[0x34 - 0x28];
    s32 model34; /* model */
    s32 model38; /* model */
    s32 model3C; /* model */
} Fx1feWork81; /* 0x40 */

extern BtlModelBuf fx_1fe_effModels[];
extern SVECTOR fx_1fe_basePos0;
extern SVECTOR fx_1fe_basePos1;
extern CVECTOR fx_1fe_rippleColor;
extern u8 fx_1fe_op81Trails[];
s32 fx_1fe_op81(s32 mode, Fx1feWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */

extern Unk800F78A4 *fx_1fe_env;
extern s32 fx_1fe_waveAmp;
extern s32 fx_1fe_wavePhase;
static void fxm_waveVertexX(s16 *vert);
s32 fx_1fe_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

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
