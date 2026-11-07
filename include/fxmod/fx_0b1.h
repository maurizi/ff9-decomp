#ifndef FX_0B1_H
#define FX_0B1_H

/* Effect module fx_0b1 (dir 13 file 0xB1): Carbuncle, full summon. Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_0b1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CARBUNCLE_RING_BURST_2     0x1 /* emitter: cone of white-red, lilac, cyan and blue-grey gouraud rings, fan */
#define FX_CARBUNCLE_LENS_FLARE       0x9 /* emitter: orbiting white glow trail with a blue lens streak */
#define FX_CARBUNCLE_GREEN_MOTES      0xA /* emitter: faint pale round glow, the green motes drifting round the emerald crystal; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_CARBUNCLE_RED_RING         0xC /* emitter: white-red gouraud rings, fans */
#define FX_CARBUNCLE_WHITE_LIGHT_STREAK 0xD /* emitter: fan, a white horizontal light streak */
#define FX_CARBUNCLE_LENS_STREAK      0xE /* emitter: blue-white lens streak, white-red ring */
#define FX_CARBUNCLE_CAMERA_KEYS      0xF /* table: camera key path for fxm_camKeyAdd (target 1) */
#define FX_CARBUNCLE_LIGHT_BEAM       0x10 /* trail: blue-white light beam, drawn twice (fx_0b1_beams) */
#define FX_CARBUNCLE_RING_BURST       0x11 /* emitter: cone of white-yellow, lilac, cyan and blue-grey gouraud rings, fan */
#define FX_CARBUNCLE_CRYSTAL_SPARKLES 0x12 /* emitter: white crystal-flake sprites and blue sparkles, fans */
#define FX_CARBUNCLE_FLAME_GLINT      0x13 /* emitter: orange-blue flame-shaped glints, red gouraud rings, fans */
#define FX_CARBUNCLE_RING_BURST_3     0x14 /* emitter: cone of white-red, lilac, cyan and blue-grey gouraud rings, fan */
#define FX_CARBUNCLE_LENS_STREAK_2    0x15 /* emitter: blue-white lens streak, white-red ring, orange sprites */
#define FX_CARBUNCLE_ORANGE_MOTES     0x16 /* emitter: small orange glow, the orange motes swirling after the burst; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_CARBUNCLE_CAMERA_KEYS_3    0x17 /* table: camera key path for fxm_camKeyAdd (target 3) */
#define FX_CARBUNCLE_COLOR_RINGS      0x18 /* emitter: pink / green / pink gouraud rings at each target body */
#define FX_CARBUNCLE_CAMERA_KEYS_2    0x19 /* table: camera key path for fxm_camKeyAdd (target 2) */
#define FX_CARBUNCLE_PINK_RING        0x21 /* emitter: pink gouraud ring and swirl-ring particles at each target body */
#define FX_CARBUNCLE_BACKDROP         0x22 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_CARBUNCLE_STAR_CRYSTAL     0x23 /* model: four-point star crystal (texture 0x177), one per target */
#define FX_CARBUNCLE_ORANGE_MOTES_2   0x24 /* emitter: the same small orange glow motes; runtime (docs/runtime-findings.md effect-data pass) */

/* fx_0b1 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes (the summon sequence), its
 * timers (fx_0b1_timers indices), the light beam, the camera key tracks; op81's star crystals. */
#define FX_CARBUNCLE_CAM_TRACKS     16   /* fx_0b1_camKeys: camera key tracks (fxm_camKeyAdd / fxm_camKeyUpdate) */
#define FX_CARBUNCLE_TIMERS         8    /* fx_0b1_timers: */
#define FX_CARBUNCLE_TIMER_SUMMON   0    /*   the summon model is drawn at camera point 2 while set */
#define FX_CARBUNCLE_TIMER_DARKEN   1    /*   frames the summon model is drawn darkened */
#define FX_CARBUNCLE_TIMER_BEAM_RISE 2   /*   frames left of the light beam's rise */
#define FX_CARBUNCLE_TIMER_BEAM     3    /*   the light beam is drawn while set */
#define FX_CARBUNCLE_TIMER_BEAM_FADE 4   /*   frames left of the light beam's fade */
#define FX_CARBUNCLE_TIMER_FLASH    5    /*   frames left of the additive screen flash (btlfx_drawScreenGrid) */
#define FX_CARBUNCLE_START_FRAME    1    /* op80: RING_BURST, CRYSTAL_SPARKLES, WHITE_LIGHT_STREAK and the flash start */
#define FX_CARBUNCLE_FLASH_FRAMES   0x10 /* the flash brightens, then fades, over this many frames */
#define FX_CARBUNCLE_SUMMON_FRAME   9    /* op80: the summon motion starts, the model shows, FX_CARBUNCLE_GREEN_MOTES start */
#define FX_CARBUNCLE_MOTES_2_FRAME  0x18 /* op80: the second FX_CARBUNCLE_GREEN_MOTES start */
#define FX_CARBUNCLE_LENS_FLARE_FRAME 0x40 /* op80: FX_CARBUNCLE_LENS_FLARE starts at FX_CARBUNCLE_BEAM_BONE */
#define FX_CARBUNCLE_RED_RING_FRAME 0x48 /* op80: FX_CARBUNCLE_RED_RING starts there (and follows the bone) */
#define FX_CARBUNCLE_RED_RING_STOP_FRAME 0x67 /* op80: FX_CARBUNCLE_RED_RING stops */
#define FX_CARBUNCLE_BEAM_FRAME     0x6C /* op80: LENS_FLARE stops; RING_BURST_2, LENS_STREAK and the light beam start */
#define FX_CARBUNCLE_DARKEN_FRAMES  0x10 /*   the summon model is drawn darkened this long */
#define FX_CARBUNCLE_BEAM_RISE_FRAMES 8  /*   the beam rises (eased) to its height over this many frames */
#define FX_CARBUNCLE_GLINT_FRAME    0x77 /* op80: the summon and the motes go; FX_CARBUNCLE_FLAME_GLINT starts at the beam's top */
#define FX_CARBUNCLE_BURST_FRAME    0x97 /* op80: emitters stop; additive grey screen tint, FX_CARBUNCLE_RING_BURST_3 */
#define FX_CARBUNCLE_BURST_TINT_RGB 0xB0, 0xB0, 0xB0
#define FX_CARBUNCLE_LENS_STREAK_2_FRAME 0x99 /* op80: FX_CARBUNCLE_LENS_STREAK_2 starts and the beam fades */
#define FX_CARBUNCLE_BEAM_FADE_FRAMES 4  /*   over this many frames */
#define FX_CARBUNCLE_ORANGE_MOTES_2_FRAME 0x9F /* op80: FX_CARBUNCLE_ORANGE_MOTES_2 start */
#define FX_CARBUNCLE_ORANGE_MOTES_FRAME 0xAE /* op80: FX_CARBUNCLE_ORANGE_MOTES start */
#define FX_CARBUNCLE_ORANGE_MOTES_2_STOP_FRAME 0xCA /* op80: FX_CARBUNCLE_ORANGE_MOTES_2 stop */
#define FX_CARBUNCLE_FRAMES         0x10A /* op80: the task ends at this frame */
#define FX_CARBUNCLE_BEAM_BONE      0x11 /* summon model bone LENS_FLARE, RED_RING, LENS_STREAK and the beam start from */
#define FX_CARBUNCLE_BEAM_POINTS    0x15 /* points of each FX_CARBUNCLE_LIGHT_BEAM strip (fx_0b1_beams) */
#define FX_CARBUNCLE_BEAM_HEIGHT    0x1800 /* the beam's height above the bone (FLAME_GLINT starts at its top) */
#define FX_CARBUNCLE_BEAM_WIDTH     0x40 /* line width of the inner strip (the outer one twice that) */
#define FX_CARBUNCLE_LENS_FLARE_TRAIL_SLOTS  0x28 /* FX_CARBUNCLE_LENS_FLARE's trail buffer fx_0b1_trails: slots of */
#define FX_CARBUNCLE_LENS_FLARE_TRAIL_STRIDE 0xAC /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_CARBUNCLE_MODEL_COUNT    4    /* op81: btlfx_initEffModels slots: a STAR_CRYSTAL per target (fx_0b1_models) */
#define FX_CARBUNCLE_STAR_CRYSTAL_TPAGE 0x177  /* FX_CARBUNCLE_STAR_CRYSTAL's texture (Hi_RegisterTexEffModel): 15-bit page (0x1C0, 0x100) */
#define FX_CARBUNCLE_STAR_CRYSTAL_CLUT  0xFFFF /* (no CLUT) */
#define FX_CARBUNCLE_STAR_CRYSTAL_U     0
#define FX_CARBUNCLE_STAR_CRYSTAL_V     0x80
#define FX_CARBUNCLE_CRYSTAL_FRAMES      0x20 /* op81: the crystals show (and the task ends) for this many frames, */
#define FX_CARBUNCLE_CRYSTAL_FADE_FRAMES 8    /* fading in over the first and out over the last this many */

typedef struct {
    s32 start0; /* start frame */
    s32 framesLeft4; /* frames left, -1 = free */
    u32 dst8; /* destination slots, 4 bits each (1-based) */
    u32 srcC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *data10; /* data */
} Fx0b1Track; /* 0x14 */

extern s32 fx_0b1_camKeyFrame; /* frame */
extern Fx0b1Track fx_0b1_camKeys[FX_CARBUNCLE_CAM_TRACKS];
extern SVECTOR fx_0b1_camKeyPoints[];
extern s32 fx_0b1_fadeTimer; /* fade timer */
extern s32 fx_0b1_models[FX_CARBUNCLE_MODEL_COUNT]; /* models */
extern SVECTOR fx_0b1_modelRot; /* model rotation */
extern VECTOR fx_0b1_modelScale; /* model scale */
extern VECTOR fx_0b1_modelPos[FX_CARBUNCLE_MODEL_COUNT]; /* model positions */
extern SVECTOR fx_0b1_bodyPos[FX_CARBUNCLE_MODEL_COUNT]; /* unit body positions */
extern BtlModelBuf fx_0b1_effModels[];

static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
static void fxm_camKeyUpdate(void);
extern Unk800F78A4 *fx_0b1_env;
static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

typedef struct {
    u8 pad0[0x180];
    BtlMotion *motion180;
} Fx0b1SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx0b1SummonData *summon54; /* g_btlfxLoader.summon */
    u8 pad58[0x60 - 0x58];
    s32 model60; /* summon model */
} Fx0b1Summon;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_CARBUNCLE_BEAM_POINTS];
} Fx0b1Trail; /* 0xAC */

#define SUMMON0B1(env) ((Fx0b1Summon *)(env)->loader)

extern SVECTOR fx_0b1_summonRot;
extern VECTOR fx_0b1_summonPos;
extern VECTOR fx_0b1_summonScale;
extern SVECTOR fx_0b1_startPos;
extern s32 fx_0b1_timers[FX_CARBUNCLE_TIMERS];
extern Pool6CEntry *fx_0b1_emitter0;
extern SVECTOR fx_0b1_emitPos0;
extern Pool6CEntry *fx_0b1_emitter1;
extern SVECTOR fx_0b1_emitPos1;
extern Pool6CEntry *fx_0b1_emitter2;
extern SVECTOR fx_0b1_emitPos2;
extern s32 fx_0b1_trails[];
extern Pool6CEntry *fx_0b1_emitter3;
extern SVECTOR fx_0b1_emitPos3;
extern SVECTOR fx_0b1_emitStartPos3;
extern Pool6CEntry *fx_0b1_emitter4;
extern SVECTOR fx_0b1_emitPos4;
extern Pool6CEntry *fx_0b1_emitter5;
extern SVECTOR fx_0b1_emitPos5;
extern Pool6CEntry *fx_0b1_emitter6;
extern SVECTOR fx_0b1_emitPos6;
extern Pool6CEntry *fx_0b1_emitter7;
extern SVECTOR fx_0b1_emitPos7;
extern Pool6CEntry *fx_0b1_emitter8;
extern SVECTOR fx_0b1_emitPos8;
extern Pool6CEntry *fx_0b1_emitter9;
extern SVECTOR fx_0b1_emitPos9;
extern Pool6CEntry *fx_0b1_emitter10;
extern SVECTOR fx_0b1_emitPos10;
extern Fx0b1Trail fx_0b1_beams[2];

#endif
