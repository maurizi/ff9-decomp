#ifndef FX_0E1_0_H
#define FX_0E1_0_H

/* Effect module fx_0e1_0 (dir 13 file 0x0E1): Rebirth Flame (Phoenix's second attack), load group 0. Fire / ice
 * drawn as a grid of texel quads (fxm_drawTexelQuads). Also fxm_drawTexRibbonTrail. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0e1_0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_REBIRTH_FLAME_BLUE_ORBS   0x1 /* emitter: clusters of blue-white glowing orbs */
#define FX_REBIRTH_FLAME_SWIRL_SPHERE 0x3 /* 'so' model drawn at the end (Hi_DrawEffModel): sphere with a purple-red-blue swirl texture */
#define FX_REBIRTH_FLAME_STAR_FLARES 0x4 /* emitter: pink-white lens arcs + orange star flares */
#define FX_REBIRTH_FLAME_RED_RINGS   0x5 /* rings: white-grey-violet-red gouraud rings, 24 frames */
#define FX_REBIRTH_FLAME_RED_RINGS_2 0x6 /* emitter: white-grey-violet-red gouraud rings */
#define FX_REBIRTH_FLAME_MIST_TRAIL  0x7 /* emitter: orbiting trail with a white cloudy strip texture */
#define FX_REBIRTH_FLAME_BLUE_RING   0x8 /* emitter: blue gouraud rings */
#define FX_REBIRTH_FLAME_BLUE_BURST  0x9 /* emitter: deep blue rings, sub-emitter of white-blue-indigo rings */
#define FX_REBIRTH_FLAME_WHITE_STREAK 0xA /* emitter: two fans, a long white-grey light streak */
#define FX_REBIRTH_FLAME_RED_SPARKS  0xC /* emitter: four elements of tiny red spark sprites */
#define FX_REBIRTH_FLAME_BLUE_ORBS_2 0xD /* emitter: faint blue orbs */
#define FX_REBIRTH_FLAME_SCREEN_SPHERE 0x80 /* summon data, 'so' model, morph end, then drawn fading: the captured battle scene wrapped on a sphere, the morph target the flat capture bends into; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_REBIRTH_FLAME_SCREEN_BULGE 0x81 /* summon data, 'so' model, morph middle: the in-between morph shape with the same captured scene texture; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_REBIRTH_FLAME_SCREEN_PLANE 0x82 /* summon data, 'so' model, morph start: flat grid carrying the captured battle scene, the picture before it is bent; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_REBIRTH_FLAME_GLOW_BAND   0x83 /* summon data: 'so' model, ring band with a white cloud texture, drawn three times with a fading glow */

/* fx_0e1_0 choreography (docs/fxdata.md, "Choreography literals"): op80's states, their lengths, keyframes and
 * screen tints, the trail buffers, the scrolled texture rects of the wave grids and ribbons, the ribbon / spawn
 * bone tables, and the module's primitive helpers. */
#define FX_REBIRTH_FLAME_STATE_SCENE   0 /* op80: the captured scene morphs SCREEN_PLANE -> BULGE -> SPHERE and spins in, the white fades */
#define FX_REBIRTH_FLAME_STATE_PHOENIX 1 /* the summon model appears: BLUE_RING / BLUE_BURST / orbs / MIST_TRAIL */
#define FX_REBIRTH_FLAME_STATE_FLIGHT  2 /* Phoenix flies (hidden for a while), RED_RINGS, the particles and the glow fade */
#define FX_REBIRTH_FLAME_STATE_DIVE    3 /* motion frame 0xA9, STAR_FLARES */
#define FX_REBIRTH_FLAME_STATE_FIRE    4 /* the wave grids and ribbons of fire (the op ends) */
#define FX_REBIRTH_FLAME_UNWHITE_FRAMES 0x10 /* STATE_SCENE: the white screen fades over this many frames */
#define FX_REBIRTH_FLAME_SPIN_FRAMES    0x2E /* STATE_SCENE: the scene sphere spirals in for this many frames, */
#define FX_REBIRTH_FLAME_SETTLE_FRAMES  18   /* then settles over this many */
#define FX_REBIRTH_FLAME_SCENE_FRAMES   0x40 /* STATE_SCENE's length; at its end the blue emitters and ORANGE_TINT */
#define FX_REBIRTH_FLAME_PHOENIX_FRAMES 0x20 /* STATE_PHOENIX's length (frame << 7 ramp); at its end RED_SPARKS */
#define FX_REBIRTH_FLAME_RISE_FRAMES    0x10 /* STATE_FLIGHT: the summon model rises for this many frames */
#define FX_REBIRTH_FLAME_HIDE_FRAME     0x36 /* STATE_FLIGHT: the summon model is hidden from this frame */
#define FX_REBIRTH_FLAME_HIDE_FRAMES    0x1A /* for this many; it comes back at motion frame 0x5A and the bone spawns start */
#define FX_REBIRTH_FLAME_FADE_FRAME     0x7F /* STATE_FLIGHT: BLUE_ORBS / MIST_TRAIL fade out from this frame, */
#define FX_REBIRTH_FLAME_FADE_FRAMES    0x10 /* over this many (then stopped) */
#define FX_REBIRTH_FLAME_GLOW_FADE_FRAME  0x87 /* STATE_FLIGHT: the glow fades from this frame, */
#define FX_REBIRTH_FLAME_GLOW_FADE_FRAMES 24   /* over this many; then STATE_FLIGHT ends (WHITE_STREAK, RED_RINGS_2) */
#define FX_REBIRTH_FLAME_STARS_FRAME    0xF  /* STATE_DIVE: FX_REBIRTH_FLAME_STAR_FLARES */
#define FX_REBIRTH_FLAME_DIVE_FRAMES    0x33 /* STATE_DIVE's length */
#define FX_REBIRTH_FLAME_WAVE_FRAMES    0x20 /* STATE_FIRE: the first wave grid fades over this many frames */
#define FX_REBIRTH_FLAME_MODEL_FRAMES   0x2C /* STATE_FIRE: the summon model draws until this frame */
#define FX_REBIRTH_FLAME_FIRE_FRAMES    0x31 /* STATE_FIRE's length (the op ends) */
#define FX_REBIRTH_FLAME_ORANGE_TINT_RGB 0xFF, 0x80, 0 /* STATE_SCENE's end: additive screen tint (orange) */
#define FX_REBIRTH_FLAME_MODEL_COUNT    5    /* btlfx_initEffModels slots */
#define FX_REBIRTH_FLAME_MIST_TRAIL_TRAIL_SLOTS  0xC  /* FX_REBIRTH_FLAME_MIST_TRAIL's trail buffers fx_0e1_0_op80Trails0 / 1: slots of */
#define FX_REBIRTH_FLAME_MIST_TRAIL_TRAIL_STRIDE 0x8C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_REBIRTH_FLAME_RIBBONS        4    /* fx_0e1_0_ribbonBones / ribbons entries */
#define FX_REBIRTH_FLAME_RIBBON_SEGMENTS 21  /* a ribbon's history steps */
#define FX_REBIRTH_FLAME_SPAWN_BONES    5    /* fx_0e1_0_spawnBones entries (the bone spawn point cycles through them) */
/* The two wave grids (fx_0e1_0_drawWaveQuadGrid) and the ribbons sample these rects; each frame btlfx_drawScrollMove
 * copies the file's texture from (SRC_X, SRC_Y) into them, scrolled. */
#define FX_REBIRTH_FLAME_WAVE_TPAGE   0x99   /* first wave grid: 8-bit page (0x240, 0x100), */
#define FX_REBIRTH_FLAME_WAVE_CLUT    0x3E00 /* CLUT at (0x0, 0xF8), */
#define FX_REBIRTH_FLAME_WAVE_U       0x80   /* texels from (u, v) = the WAVE rect */
#define FX_REBIRTH_FLAME_WAVE_V       0
#define FX_REBIRTH_FLAME_WAVE_VRAM_X  0x280
#define FX_REBIRTH_FLAME_WAVE_VRAM_Y  0x100
#define FX_REBIRTH_FLAME_WAVE_VRAM_W  0x20
#define FX_REBIRTH_FLAME_WAVE_VRAM_H  0x80
#define FX_REBIRTH_FLAME_WAVE_SRC_X   0x2A0
#define FX_REBIRTH_FLAME_WAVE_SRC_Y   0x100
#define FX_REBIRTH_FLAME_WAVE_2_TPAGE 0x1B   /* second wave grid and the ribbons: 4-bit page (0x2C0, 0x100), */
#define FX_REBIRTH_FLAME_WAVE_2_CLUT  0x3D05 /* CLUT at (0x50, 0xF4), */
#define FX_REBIRTH_FLAME_WAVE_2_U     0xC0   /* texels from (u, v) = the WAVE_2 rect */
#define FX_REBIRTH_FLAME_WAVE_2_V     0
#define FX_REBIRTH_FLAME_WAVE_2_VRAM_X 0x2F0
#define FX_REBIRTH_FLAME_WAVE_2_VRAM_Y 0x100
#define FX_REBIRTH_FLAME_WAVE_2_VRAM_W 0x10
#define FX_REBIRTH_FLAME_WAVE_2_VRAM_H 0x80
#define FX_REBIRTH_FLAME_WAVE_2_SRC_X  0x2E0
#define FX_REBIRTH_FLAME_WAVE_2_SRC_Y  0x100
/* a third scrolled rect, sampled by a summon-data model of STATE_FIRE (which one is not checked) */
#define FX_REBIRTH_FLAME_BAND_VRAM_X  0x2A0
#define FX_REBIRTH_FLAME_BAND_VRAM_Y  0x180
#define FX_REBIRTH_FLAME_BAND_VRAM_W  0x10
#define FX_REBIRTH_FLAME_BAND_VRAM_H  0x40
#define FX_REBIRTH_FLAME_BAND_SRC_X   0x2B0
#define FX_REBIRTH_FLAME_BAND_SRC_Y   0x180

/* the effect environment block as these callbacks read it */
typedef struct {
    u8 pad0[0x60];
    s32 summonModel60; /* summon model */
} Fx0e10EnvB;

typedef struct {
    SVECTOR a0; /* pad: point valid */
    SVECTOR b8;
} Fx0e10Seg; /* 0x10: one step of the ribbon history */

extern s32 fx_0e1_0_spawnBones[FX_REBIRTH_FLAME_SPAWN_BONES];
extern s32 fx_0e1_0_spawnIndex;
extern s32 fx_0e1_0_boneSpawnOn;
extern Unk800F78A4 *fx_0e1_0_env;

static void fxm_drawTexRibbonTrail(SVECTOR *posA, SVECTOR *posB, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 bright,
                   Fx0e10Seg *buf, s32 abr, s32 phase);
void fx_0e1_0_drawWaveQuadGrid(s32 ot, s32 x, s32 y, s32 nx, s32 ny, s32 dx, s32 dy, s32 abr, s32 sy, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 w, s32 h, s32 c, s32 amp, s32 ph, s32 dphx, s32 dphy);
static s32 fx_0e1_0_op80SpawnPoint(SVECTOR *out);
/* Never called (no reference, no hit on an exec breakpoint over Rebirth Flame, runtime round 33). */
void fx_0e1_0_scaleXY(s32 unused0, s32 unused1, s32 unused2, s32 unused3, s16 *v);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0e10Vec3;

typedef struct {
    u32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* summon position */
    Fx0e10Vec3 summonPos10; /* summon model position */
    Fx0e10Vec3 summonScale1C; /* summon model scale */
    SVECTOR rot28; /* summon model rotation */
    s32 unk30; /* unused (runtime watch, round 33) */
    SVECTOR pos34; /* bone position */
    SVECTOR camPos3C;
    Pool6CEntry *emit44;
    Pool6CEntry *emit48;
    s32 ribbonReset4C; /* ribbons need a reset */
    s32 model50;
    s32 model54;
    s32 model58;
    s32 model5C;
    s32 model60;
    SVECTOR rot64;
    Fx0e10Vec3 morphPos6C;
    Fx0e10Vec3 morphScale78;
    Fx0e10Vec3 pos84;
    Fx0e10Vec3 scale90;
    SVECTOR rot9C;
} Fx0e10Work80; /* 0xA4 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 summonModel60; /* summon model */
} Fx0e10Summon;

typedef struct {
    s32 boneA0;
    s32 boneB4;
} Fx0e10BonePair;

extern SVECTOR fx_0e1_0_origin;
extern Fx0e10BonePair fx_0e1_0_ribbonBones[FX_REBIRTH_FLAME_RIBBONS];
extern u8 fx_0e1_0_gradient[];
extern Fx0e10Seg fx_0e1_0_ribbons[FX_REBIRTH_FLAME_RIBBONS][FX_REBIRTH_FLAME_RIBBON_SEGMENTS];
extern BtlModelBuf fx_0e1_0_effModels[];
extern u8 fx_0e1_0_op80Trails0[];
extern u8 fx_0e1_0_op80Trails1[];

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
