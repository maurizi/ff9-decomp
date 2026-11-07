#ifndef FX_1EE_H
#define FX_1EE_H

/* Effect module fx_1ee (dir 13 file 0x1ee): Carbuncle (2). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1ee effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CARBUNCLE_YELLOW_RED_RINGS 0x2 /* emitter: white-yellow and white-red gouraud rings at each target */
#define FX_CARBUNCLE_VIOLET_RINGS     0x13 /* emitter: white / pale blue / violet gouraud rings, 24 frames */
#define FX_CARBUNCLE_GREEN_RING       0x14 /* emitter: white-green gouraud ring */
#define FX_CARBUNCLE_CLOCK_FACE       0x17 /* emitter: clock face with Roman numerals and a blue ring glow */
#define FX_CARBUNCLE_CAMERA_KEYS      0x1E /* table: camera key path for fxm_camKeyAdd (target 1) (same call site as fx_0b1 entry 0xF, same kind) */
#define FX_CARBUNCLE_CAMERA_KEYS_3    0x1F /* table: camera key path for fxm_camKeyAdd (target 3) (same call site as fx_0b1 entry 0x17, same kind) */
#define FX_CARBUNCLE_CAMERA_KEYS_2    0x20 /* table: camera key path for fxm_camKeyAdd (target 2) (same call site as fx_0b1 entry 0x19, same kind) */
#define FX_CARBUNCLE_LENS_STREAK      0x2B /* emitter: blue-white lens streak and a gouraud ring (same call site as fx_0b1 entry 0xE, same kind) */
#define FX_CARBUNCLE_LENS_STREAK_2    0x2C /* emitter: blue-white lens streak, sparkle sprites (same call site as fx_0b1 entry 0x15, same kind) */
#define FX_CARBUNCLE_ORBIT_GLOW       0x2E /* emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0xA, same kind) */
#define FX_CARBUNCLE_RING_BURST_2     0x30 /* emitter: cone of white, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x1, same kind) */
#define FX_CARBUNCLE_RING_BURST_3     0x31 /* emitter: cone of white, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x14, same kind) */
#define FX_CARBUNCLE_ORBIT_GLOW_2     0x32 /* emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x16, same kind) */
#define FX_CARBUNCLE_LIGHT_BEAM       0x33 /* trail: blue-white light beam, drawn twice (same call site as fx_0b1 entry 0x10, same kind) */
#define FX_CARBUNCLE_FLAME_GLINT      0x35 /* emitter: orange-blue flame-shaped glints, gouraud rings, fans (same call site as fx_0b1 entry 0x13, same kind) */
#define FX_CARBUNCLE_ORBIT_GLOW_3     0x36 /* emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x24, same kind) */
#define FX_CARBUNCLE_CRYSTAL_SPARKLES 0x38 /* emitter: white crystal-flake sprites and blue sparkles, fans (same call site as fx_0b1 entry 0x12, same kind) */
#define FX_CARBUNCLE_GLOW_RING        0x39 /* emitter: white glow fans and a white / pale green gouraud ring (same call site as fx_0b1 entry 0xC, same kind) */
#define FX_CARBUNCLE_RING_BURST       0x3A /* emitter: cone of white-yellow, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x11, same kind) */
#define FX_CARBUNCLE_LENS_FLARE       0x3B /* emitter: orbiting white glow trail with a blue lens streak (same call site as fx_0b1 entry 0x9, same kind) */
#define FX_CARBUNCLE_WHITE_LIGHT_STREAK 0x3C /* emitter: one fan, white glow streak (same call site as fx_0b1 entry 0xD, same kind) */

/* fx_1ee choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes (the summon sequence), its
 * timers (fx_1ee_timers indices), the light beam, the camera key tracks; op81's spark spiral. Same
 * sequence and values as Carbuncle's fx_0b1 (twin names; the emitters differ: orbit glows for the motes, GLOW_RING for
 * the red ring). */
#define FX_CARBUNCLE_CAM_TRACKS     16   /* fx_1ee_camKeys: camera key tracks (fxm_camKeyAdd / fxm_camKeyUpdate) */
#define FX_CARBUNCLE_TIMERS         8    /* fx_1ee_timers: */
#define FX_CARBUNCLE_TIMER_SUMMON   0    /*   the summon model is drawn at camera point 2 while set */
#define FX_CARBUNCLE_TIMER_DARKEN   1    /*   frames the summon model is drawn darkened */
#define FX_CARBUNCLE_TIMER_BEAM_RISE 2   /*   frames left of the light beam's rise */
#define FX_CARBUNCLE_TIMER_BEAM     3    /*   the light beam is drawn while set */
#define FX_CARBUNCLE_TIMER_BEAM_FADE 4   /*   frames left of the light beam's fade */
#define FX_CARBUNCLE_TIMER_FLASH    5    /*   frames left of the additive screen flash (btlfx_drawScreenGrid) */
#define FX_CARBUNCLE_START_FRAME    1    /* op80: RING_BURST, CRYSTAL_SPARKLES, WHITE_LIGHT_STREAK and the flash start */
#define FX_CARBUNCLE_FLASH_FRAMES   0x10 /* the flash brightens, then fades, over this many frames */
#define FX_CARBUNCLE_SUMMON_FRAME   9    /* op80: the summon motion starts, the model shows, FX_CARBUNCLE_ORBIT_GLOW starts */
#define FX_CARBUNCLE_ORBIT_GLOW_AGAIN_FRAME  0x18 /* op80: a second FX_CARBUNCLE_ORBIT_GLOW starts */
#define FX_CARBUNCLE_LENS_FLARE_FRAME 0x40 /* op80: FX_CARBUNCLE_LENS_FLARE starts at FX_CARBUNCLE_BEAM_BONE */
#define FX_CARBUNCLE_GLOW_RING_FRAME 0x48 /* op80: FX_CARBUNCLE_GLOW_RING starts there (and follows the bone) */
#define FX_CARBUNCLE_GLOW_RING_STOP_FRAME 0x67 /* op80: FX_CARBUNCLE_GLOW_RING stops */
#define FX_CARBUNCLE_BEAM_FRAME     0x6C /* op80: LENS_FLARE stops; RING_BURST_2, LENS_STREAK and the light beam start */
#define FX_CARBUNCLE_DARKEN_FRAMES  0x10 /*   the summon model is drawn darkened this long */
#define FX_CARBUNCLE_BEAM_RISE_FRAMES 8  /*   the beam rises (eased) to its height over this many frames */
#define FX_CARBUNCLE_GLINT_FRAME    0x77 /* op80: the summon and the motes go; FX_CARBUNCLE_FLAME_GLINT starts at the beam's top */
#define FX_CARBUNCLE_BURST_FRAME    0x97 /* op80: emitters stop; additive grey screen tint, FX_CARBUNCLE_RING_BURST_3 */
#define FX_CARBUNCLE_BURST_TINT_RGB 0xB0, 0xB0, 0xB0 /* its colour */
#define FX_CARBUNCLE_LENS_STREAK_2_FRAME 0x99 /* op80: FX_CARBUNCLE_LENS_STREAK_2 starts and the beam fades */
#define FX_CARBUNCLE_BEAM_FADE_FRAMES 4  /*   over this many frames */
#define FX_CARBUNCLE_ORBIT_GLOW_3_FRAME 0x9F /* op80: FX_CARBUNCLE_ORBIT_GLOW_3 starts */
#define FX_CARBUNCLE_ORBIT_GLOW_2_FRAME 0xAE /* op80: FX_CARBUNCLE_ORBIT_GLOW_2 starts */
#define FX_CARBUNCLE_ORBIT_GLOW_3_STOP_FRAME 0xCA /* op80: FX_CARBUNCLE_ORBIT_GLOW_3 stops */
#define FX_CARBUNCLE_FRAMES         0x10A /* op80: the task ends at this frame */
#define FX_CARBUNCLE_BEAM_BONE      0x11 /* summon model bone LENS_FLARE, GLOW_RING, LENS_STREAK and the beam start from */
#define FX_CARBUNCLE_BEAM_POINTS    0x15 /* points of each FX_CARBUNCLE_LIGHT_BEAM strip (fx_1ee_trails) */
#define FX_CARBUNCLE_BEAM_HEIGHT    0x1800 /* the beam's height above the bone (FLAME_GLINT starts at its top) */
#define FX_CARBUNCLE_BEAM_WIDTH     0x40 /* line width of the inner strip (the outer one twice that) */
#define FX_CARBUNCLE_LENS_FLARE_TRAIL_SLOTS  0x28 /* FX_CARBUNCLE_LENS_FLARE's trail buffer fx_1ee_trails: slots of */
#define FX_CARBUNCLE_LENS_FLARE_TRAIL_STRIDE 0xAC /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_CARBUNCLE_SPARK_ROWS     12   /* op81: fx_1ee_sparks: rows of sparks (each row's angle -0x2A9 further on) */
#define FX_CARBUNCLE_SPARK_COLS     4    /*   sparks per row (the first uses the row's own texture, the others a shared one) */
#define FX_CARBUNCLE_SPARK_FRAMES   8    /*   each spark spirals in (and fades, timer4 << 9) over this many frames */
#define FX_CARBUNCLE_SPARKS_FRAMES  0x20 /* op81: the sparks are drawn (fx_1ee_op81Timer) this many frames from frame 0 */
#define FX_CARBUNCLE_VIOLET_RINGS_FRAME 8 /* op81: FX_CARBUNCLE_VIOLET_RINGS start at camera point 3 */
#define FX_CARBUNCLE_TARGET_RINGS_FRAME 0x18 /* op81: GREEN_RING, YELLOW_RED_RINGS and CLOCK_FACE start at each target */
#define FX_CARBUNCLE_OP81_FRAMES    0x40 /* op81: the task ends at this frame */

typedef struct {
    u8 pad0[0x180];
    BtlMotion *motion180;
} Fx1eeSummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1eeSummonData *summon54; /* g_btlfxLoader.summon */
    u8 pad58[0x60 - 0x58];
    s32 model60; /* summon model */
} Fx1eeSummon;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pos4[FX_CARBUNCLE_BEAM_POINTS];
} Fx1eeTrail; /* 0xAC */

extern Unk800F78A4 *fx_1ee_env;
extern SVECTOR fx_1ee_modelRot; /* summon rotation */
extern VECTOR fx_1ee_modelPos; /* summon position */
extern VECTOR fx_1ee_modelScale; /* summon scale */
extern SVECTOR fx_1ee_emitPos0;
extern s32 fx_1ee_timers[FX_CARBUNCLE_TIMERS]; /* timers */
extern Pool6CEntry *fx_1ee_emitter0;
extern SVECTOR fx_1ee_emitter0Pos;
extern Pool6CEntry *fx_1ee_emitter1;
extern SVECTOR fx_1ee_emitter1Pos;
extern Pool6CEntry *fx_1ee_emitter2;
extern SVECTOR fx_1ee_emitter2Pos;
extern s32 fx_1ee_op80Trails[];
extern Pool6CEntry *fx_1ee_emitter3;
extern SVECTOR fx_1ee_emitter3Target;
extern SVECTOR fx_1ee_emitter3Pos;
extern Pool6CEntry *fx_1ee_emitter4;
extern SVECTOR fx_1ee_emitter4Pos;
extern Pool6CEntry *fx_1ee_emitter5;
extern SVECTOR fx_1ee_emitter5Pos;
extern Pool6CEntry *fx_1ee_emitter6;
extern SVECTOR fx_1ee_emitter6Pos;
extern Pool6CEntry *fx_1ee_emitter7;
extern SVECTOR fx_1ee_emitter7Pos;
extern Pool6CEntry *fx_1ee_emitter8;
extern SVECTOR fx_1ee_emitter8Pos;
extern Pool6CEntry *fx_1ee_emitter9;
extern SVECTOR fx_1ee_emitter9Pos;
extern Pool6CEntry *fx_1ee_emitter10;
extern SVECTOR fx_1ee_emitter10Pos;
extern Fx1eeTrail fx_1ee_trails[2];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

typedef struct {
    s32 timer0; /* delay */
    s32 timer4; /* count */
    u8 *tex8; /* data */
    s32 angleC; /* angle */
    s32 radius10; /* radius */
} Fx1eeSpark; /* 0x14 */

extern s32 fx_1ee_op81Timer[]; /* spark timer; array so it may alias entry (store order) */
extern SVECTOR fx_1ee_sparkOrigin;
extern SVECTOR fx_1ee_op81TargetPos[];
extern Fx1eeSpark fx_1ee_sparks[FX_CARBUNCLE_SPARK_ROWS][FX_CARBUNCLE_SPARK_COLS];

#define SUMMON(env) ((Fx1eeSummon *)(env)->loader)

/* copied */
extern Unk800F78A4 *fx_1ee_env;
/* end copied */

/* propagated */

typedef struct {
    s32 frame0; /* start frame */
    s32 timer4; /* frames left, -1 = free */
    u32 slot8; /* destination slots, 4 bits each (1-based) */
    u32 slotC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *keys10; /* data */
} Fx1eeTrack; /* 0x14 */

extern s32 fx_1ee_camFrame; /* frame */
extern Fx1eeTrack fx_1ee_camKeys[];
#define FX_1EE_KEY_FREE (-1) /* timer4: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
extern SVECTOR fx_1ee_camKeyPoints[];
static void fxm_camKeyUpdate(void);

#endif
