#ifndef FX_0D3_H
#define FX_0D3_H

/* Effect module fx_0d3 (dir 13 file 0xD3): Phoenix, full summon. Fire / ice drawn as a grid of texel quads
 * (fxm_drawTexelQuads). Also fxm_drawTexRibbonTrail. Evidence: btlfx_start and btlfx_loadFile get the same id for
 * summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_0d3 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PHOENIX_RED_RINGS        0x1 /* emitter: grey and white-red gouraud rings */
#define FX_PHOENIX_FIRE_TRAILS      0x2 /* emitter: fire-textured trails and a swirling ember ring */
#define FX_PHOENIX_GLOW_RINGS       0x3 /* emitter: pale yellow / grey gouraud rings (+ textured rings, texture not in the file) */
#define FX_PHOENIX_PEACH_GLOW       0x4 /* emitter: two fans, a narrow peach half-disc glow */
#define FX_PHOENIX_RAINBOW_FAN      0x5 /* emitter: fan of a rainbow gradient strip, the prismatic light rays behind Phoenix; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_PHOENIX_FEATHERS         0x6 /* emitter: two fans, grey and tan feathers with light streaks */
#define FX_PHOENIX_LENS_FLARE       0x7 /* sprite line: lens-flare set (rainbow halo ring, yellow-green disc, white glow, red ring, blue disc) across Phoenix; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_PHOENIX_FIRE_TRAILS_2    0xB /* emitter: orbiting fire-textured trails */
#define FX_PHOENIX_FIRE_DOME        0xC /* 'so' model: dome with a fire texture */
#define FX_PHOENIX_FIRE_DISC        0xD /* 'so' model: flat disc with a fire texture */
#define FX_PHOENIX_SMOKE_ARC        0xE /* 'so' model: thin arc with a grey smoke texture */
#define FX_PHOENIX_EMBERS           0xF /* emitter: glowing ember sprites */
#define FX_PHOENIX_FIRE_FEATHERS    0x10 /* emitter: orbiting fire trails and flame-feather quads */
#define FX_PHOENIX_PINK_RINGS       0x11 /* emitter: white / pink / copper gouraud rings */
#define FX_PHOENIX_GOLD_FEATHERS    0x12 /* emitter: golden feather quads */
#define FX_PHOENIX_FEATHERS_2       0x13 /* emitter: two fans, grey and tan feathers with light streaks */
#define FX_PHOENIX_RAINBOW_FAN_2    0x14 /* emitter: the same rainbow gradient fan, later rays; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_PHOENIX_BACKDROP         0x15 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_PHOENIX_FLAME_SWIRL      0x16 /* 'so' model: cone of swirling brown-orange flame tongues (fx_0d3_sharedModel) */
#define FX_PHOENIX_FLAME_FEATHERS   0x17 /* emitter: flame-feather quads */
#define FX_PHOENIX_FLAME_FEATHERS_2 0x18 /* emitter: flame-feather quads (cylinder) */
#define FX_PHOENIX_GOLD_FEATHERS_2  0x19 /* emitter: golden feather sprites */
#define FX_PHOENIX_EMBERS_2         0x1A /* emitter: glowing ember sprites */
#define FX_PHOENIX_COPPER_RINGS     0x1B /* emitter: orbiting copper / brown gouraud rings */
#define FX_PHOENIX_EMBERS_3         0x1C /* emitter: glowing ember sprites and red sparks */
#define FX_PHOENIX_FLAME_FEATHERS_3 0x1D /* emitter: flame-feather quads */
#define FX_PHOENIX_FLAME_SWIRL_2    0x26 /* 'so' model: cone of swirling brown-orange flame tongues (other texture page) */

/* fx_0d3 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and length (the flame swirl and
 * the summon's arrival), op81's states and their keyframes (the rebirth), the VRAM rects the scrolling textures
 * fill (checked against the models' tpage + v), the ribbon texture, the floor texel frames, GPU words. */
#define FX_PHOENIX_MODEL_COUNT        7    /* op80: btlfx_initEffModels slots */
#define FX_PHOENIX_PEACH_GLOW_FRAME   0x1E /* op80: FX_PHOENIX_PEACH_GLOW starts (FX_PHOENIX_RED_RINGS at frame 0) */
#define FX_PHOENIX_FIRE_TRAILS_FRAME  0x28 /* op80: FX_PHOENIX_FIRE_TRAILS starts */
#define FX_PHOENIX_FEATHERS_FRAME     0x32 /* op80: FX_PHOENIX_FEATHERS start */
#define FX_PHOENIX_SUMMON_FRAME       0x3C /* op80: the summon appears, */
#define FX_PHOENIX_SUMMON_FADE_FRAMES 8    /* brightening over this many frames */
#define FX_PHOENIX_SWIRL_FRAMES       0x4B /* op80: FLAME_SWIRL / FLAME_SWIRL_2 brighten over this many frames, then the task ends */
#define FX_PHOENIX_FIRE_TRAILS_TRAIL_SLOTS   0xC0 /* FX_PHOENIX_FIRE_TRAILS' trail buffer: slots of */
#define FX_PHOENIX_FIRE_TRAILS_TRAIL_STRIDE  0x1C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_PHOENIX_STATE_RAINBOW 0 /* op81: GLOW_RINGS, RAINBOW_FAN, FEATHERS, FIRE_TRAILS_2, FLAME_FEATHERS, LENS_FLARE */
#define FX_PHOENIX_STATE_FEATHERS 1 /* op81: FIRE_FEATHERS, FEATHERS_2, RAINBOW_FAN_2 */
#define FX_PHOENIX_STATE_BURST   2 /* op81: PINK_RINGS, FLAME_FEATHERS_2, GOLD_FEATHERS, the SMOKE_ARC wave, screen tints */
#define FX_PHOENIX_STATE_RIPPLE  3 /* op81: ripple grid and screen grid */
#define FX_PHOENIX_STATE_COPPER  4 /* op81: EMBERS_2, COPPER_RINGS, ripple grid */
#define FX_PHOENIX_STATE_REBIRTH 5 /* op81: GOLD_FEATHERS_2, FIRE_DISC, the floor glow, the summon's ribbons; white fade-in */
#define FX_PHOENIX_STATE_TAIL    6 /* op81: white fade-out, EMBERS_3, FLAME_FEATHERS_3 */
#define FX_PHOENIX_RAINBOW_FRAMES  0x42 /* FX_PHOENIX_STATE_RAINBOW's length */
#define FX_PHOENIX_TRAILS_2_FRAME  1    /*   FX_PHOENIX_FIRE_TRAILS_2 starts */
#define FX_PHOENIX_FLAME_FEATHERS_FRAME 7 /*   FX_PHOENIX_FLAME_FEATHERS and FX_PHOENIX_EMBERS start */
#define FX_PHOENIX_MOTION_FRAME    18   /*   the summon's second motion */
#define FX_PHOENIX_LENS_FLARE_FRAME 0x18 /*  FX_PHOENIX_LENS_FLARE is drawn from here to the state's end */
#define FX_PHOENIX_RAINBOW_FADE_FRAMES 0x23 /* FLAME_SWIRL fades over this many frames */
#define FX_PHOENIX_FEATHERS_STATE_FRAMES 0x25 /* FX_PHOENIX_STATE_FEATHERS' length */
#define FX_PHOENIX_GOLD_FEATHERS_FRAME 7 /* FX_PHOENIX_STATE_BURST: GOLD_FEATHERS start, the SMOKE_ARC wave rises */
#define FX_PHOENIX_WAVE_FRAMES     20   /*   over this many frames, */
#define FX_PHOENIX_SMOKE_ARCS      7    /*   SMOKE_ARC drawn this many times up the wave */
#define FX_PHOENIX_BLUE_FRAME      5    /*   one frame of additive blue screen tint, */
#define FX_PHOENIX_BLUE_RGB        0, 0, 0x64
#define FX_PHOENIX_DARKEN_FRAME    6    /*   then one of subtractive tint, */
#define FX_PHOENIX_DARKEN_RGB      0, 0x80, 0xFF
#define FX_PHOENIX_FLASH_FRAME     7    /*   then an additive yellow-white flash fading over */
#define FX_PHOENIX_FLASH_FRAMES    8    /*   this many frames */
#define FX_PHOENIX_BURST_FRAMES    0x20 /* FX_PHOENIX_STATE_BURST's length */
#define FX_PHOENIX_RIPPLE_FRAMES   0x17 /* FX_PHOENIX_STATE_RIPPLE's length */
#define FX_PHOENIX_COPPER_FRAMES   0xE  /* FX_PHOENIX_STATE_COPPER's length */
#define FX_PHOENIX_REBIRTH_FRAMES  0x3C /* FX_PHOENIX_STATE_REBIRTH's length */
#define FX_PHOENIX_WHITE_IN_FRAME  0x30 /*   the additive white fade-in starts */
#define FX_PHOENIX_WHITE_OUT_FRAMES 0x18 /* FX_PHOENIX_STATE_TAIL: the white fades out over this many frames */
#define FX_PHOENIX_FLAME_FEATHERS_3_FRAME 0x20 /* FX_PHOENIX_STATE_TAIL: FX_PHOENIX_FLAME_FEATHERS_3 start */
#define FX_PHOENIX_TAIL_FRAMES     0x57 /* FX_PHOENIX_STATE_TAIL's length (the op ends) */
#define FX_PHOENIX_TRAILS_2_TRAIL_SLOTS  0x4A /* FX_PHOENIX_FIRE_TRAILS_2's trail buffer: slots of */
#define FX_PHOENIX_TRAILS_2_TRAIL_STRIDE 0x1C /* this many bytes */
#define FX_PHOENIX_FIRE_FEATHERS_TRAIL_SLOTS  0x40 /* FX_PHOENIX_FIRE_FEATHERS' trail buffer: slots of */
#define FX_PHOENIX_FIRE_FEATHERS_TRAIL_STRIDE 0x1C /* this many bytes */
#define FX_PHOENIX_RIBBONS         6    /* fx_0d3_ribbonBones: textured ribbons between pairs of summon bones */
/* Scrolling textures: each btlfx_drawScrollMove copies the file's texture from (SRC_X, SRC_Y) into the rect the
 * model samples: FLAME_SWIRL(_2) (tpage 0x19 = 4-bit page (0x240, 0x100)) in two halves, FIRE_DOME (tpage 0x9B =
 * 8-bit page (0x2C0, 0x100)), FIRE_DISC (tpage 0x1A = 4-bit page (0x280, 0x100)), the ribbons (RIBBON_TPAGE, v 0x80). */
#define FX_PHOENIX_SWIRL_TOP_VRAM_X    0x240
#define FX_PHOENIX_SWIRL_TOP_VRAM_Y    0x100
#define FX_PHOENIX_SWIRL_BOTTOM_VRAM_Y 0x170
#define FX_PHOENIX_SWIRL_VRAM_W        0x1C
#define FX_PHOENIX_SWIRL_VRAM_H        0x70
#define FX_PHOENIX_SWIRL_SRC_X         0x25C
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
#define FX_PHOENIX_RIBBON_TPAGE 0x1C   /* fxm_drawTexRibbonTrail: 4-bit page (0x300, 0x100) */
#define FX_PHOENIX_RIBBON_CLUT  0x3D09 /* CLUT (0x90, 0xF4) */
#define FX_PHOENIX_RIBBON_U     0
#define FX_PHOENIX_RIBBON_V     0x80
#define FX_PHOENIX_RIBBON_W     0x40
#define FX_PHOENIX_RIBBON_H     0x80
#define FX_PHOENIX_RIBBON_VRAM_X 0x300
#define FX_PHOENIX_RIBBON_VRAM_Y 0x180
#define FX_PHOENIX_RIBBON_VRAM_W 0x10
#define FX_PHOENIX_RIBBON_VRAM_H 0x80
#define FX_PHOENIX_RIBBON_SRC_X  0x310
#define FX_PHOENIX_RIBBON_SRC_Y  0x180
/* The floor glow (STATE_REBIRTH): a gouraud grid flat at the centre whose vertex colours are a 12x12 frame of the
 * file's glow animation, read back with StoreImage: 32 frames in 4 columns at (0x300, 0x100) of the 15-bit page. */
#define FX_PHOENIX_FLOOR_GLOW_TPAGE      0x11C /* 15-bit page (0x300, 0x100) */
#define FX_PHOENIX_FLOOR_GLOW_COLS       4
#define FX_PHOENIX_FLOOR_GLOW_FRAME_MASK 0x1F
#define FX_PHOENIX_FLOOR_GLOW_CELL       0x190
#define FX_PHOENIX_FLOOR_GLOW_GRID       0xC

extern Unk800F78A4 *fx_0d3_env;
extern s32 fx_0d3_spawnBones[5];
extern s32 fx_0d3_spawnBoneNext;
extern u16 fx_0d3_camKeyBlocks[3]; /* file ids for op87 tracks, end frame */
extern s32 fx_0d3_waveAmp;
extern s32 fx_0d3_wavePhase;

static void fx_0d3_op80GridVertex(s32 a0, s32 frame, s32 a2, s32 a3, s16 *v);
static s32 fx_0d3_op81SpawnPoint(SVECTOR *v);
static void fxm_waveVertexX(s16 *vert);
typedef struct {
    s16 unk0; /* not read by the module */
    s16 count2; /* count */
    s16 keys4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx0d3Track;
static void fxm_keyGetVec(Fx0d3Track *track, s32 t, SVECTOR *out);
static void fxm_keyGetVec2(Fx0d3Track *track, s32 t, SVECTOR *out0, SVECTOR *out1);
static void fxm_keyGetVec3(Fx0d3Track *track, s32 t, SVECTOR *out0, SVECTOR *out1, SVECTOR *out2);
static s32 fxm_keyGetValue(Fx0d3Track *track, s32 t);
/* ribbon history: two edge points per step, edgeA0.pad = live */
typedef struct {
    SVECTOR edgeA0;
    SVECTOR edgeB8;
} Fx0d3Seg; /* 0x10 */

#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

static void fxm_drawTexRibbonTrail(SVECTOR *posA, SVECTOR *posB, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 bright,
                   Fx0d3Seg *buf, s32 abr, s32 k);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0d3Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    SVECTOR pos8; /* summon position */
    Fx0d3Vec3 summonPos10;
    Fx0d3Vec3 summonScale1C; /* summon scale */
    SVECTOR rot28; /* summon rotation */
    Pool6CEntry *emit30;
    Pool6CEntry *emit34;
    u8 pad38[0x60 - 0x38];
    s32 model60; /* models */
    s32 model64;
    s32 scroll68; /* tint */
} Fx0d3Work80; /* 0x6C */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx0d3Summon;

extern SVECTOR fx_0d3_summonPos;
extern s32 fx_0d3_sharedModel;
extern u8 fx_0d3_trails[];
extern BtlModelBuf fx_0d3_effModels[];
s32 fx_0d3_op80(s32 mode, Fx0d3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    SVECTOR pos8;
    Fx0d3Vec3 summonPos10; /* summon position */
    Fx0d3Vec3 summonScale1C; /* summon scale */
    SVECTOR rot28; /* summon rotation */
    SVECTOR pos30;
    SVECTOR pos38;
    SVECTOR camAt40; /* camera */
    u8 pad48[4];
    Pool6CEntry *emit4C;
    Pool6CEntry *emit50;
    s32 model54; /* models */
    s32 model58;
    s32 model5C;
    s32 model60;
} Fx0d3Work81; /* 0x64 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180[4];
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx0d3Summon81;

typedef struct {
    s32 bone0;
    s32 bone4;
} Fx0d3BonePair;

extern SVECTOR fx_0d3_centerPos;
extern Fx0d3BonePair fx_0d3_ribbonBones[FX_PHOENIX_RIBBONS];
extern CVECTOR fx_0d3_rippleColors[];
extern CVECTOR fx_0d3_rippleColors2[];
extern CVECTOR fx_0d3_rippleColors3[];
extern u8 fx_0d3_trails2[];
extern u8 fx_0d3_trails3[];
extern Fx0d3Seg fx_0d3_ribbonSegs[FX_PHOENIX_RIBBONS][12];
extern u16 fx_0d3_texels[];
s32 fx_0d3_op81(s32 mode, Fx0d3Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */
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
static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 segs, s32 rings, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

#endif
