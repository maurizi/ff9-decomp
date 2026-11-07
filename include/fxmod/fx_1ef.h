#ifndef FX_1EF_H
#define FX_1EF_H

/* Effect module fx_1ef (dir 13 file 0x1ef): Carbuncle (4). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1ef effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CARBUNCLE_WHITE_STREAK     0x3 /* emitter: orbiting sprite (blank) + screen trail with a white light-streak texture */
#define FX_CARBUNCLE_LENS_FLARE       0x4 /* emitter: orbiting white glow trail with a blue lens streak (same call site as fx_0b1 entry 0x9, same kind) */
#define FX_CARBUNCLE_RING_BURST_2     0x5 /* emitter: cone of white, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x1, same kind) */
#define FX_CARBUNCLE_LENS_STREAK      0x6 /* emitter: blue-white lens streak and a gouraud ring (same call site as fx_0b1 entry 0xE, same kind) */
#define FX_CARBUNCLE_RING_BURST_3     0x7 /* emitter: cone of white, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x14, same kind) */
#define FX_CARBUNCLE_LENS_STREAK_2    0x8 /* emitter: blue-white lens streak, sparkle sprites (same call site as fx_0b1 entry 0x15, same kind) */
#define FX_CARBUNCLE_RING_BURST       0x9 /* emitter: cone of white-yellow, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x11, same kind) */
#define FX_CARBUNCLE_CRYSTAL_SPARKLES 0xA /* emitter: white crystal-flake sprites and blue sparkles, fans (same call site as fx_0b1 entry 0x12, same kind) */
#define FX_CARBUNCLE_ORBIT_GLOW       0xB /* emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0xA, same kind) */
#define FX_CARBUNCLE_WHITE_STREAK_2   0xC /* emitter: orbiting screen trail with a white light-streak texture */
#define FX_CARBUNCLE_GOLD_GLOW        0xD /* emitter: golden glow sprite + pale yellow gouraud rings */
#define FX_CARBUNCLE_FLAME_GLINT      0xE /* emitter: orange-blue flame-shaped glints, gouraud rings, fans (same call site as fx_0b1 entry 0x13, same kind) */
#define FX_CARBUNCLE_ORBIT_GLOW_3     0xF /* emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x24, same kind) */
#define FX_CARBUNCLE_WHITE_LIGHT_STREAK 0x10 /* emitter: one fan, white glow streak (same call site as fx_0b1 entry 0xD, same kind) */
#define FX_CARBUNCLE_GLOW_RING        0x11 /* emitter: white glow fans and a white / pale green gouraud ring (same call site as fx_0b1 entry 0xC, same kind) */
#define FX_CARBUNCLE_LIGHT_BEAM       0x12 /* trail: blue-white light beam, drawn twice (same call site as fx_0b1 entry 0x10, same kind) */
#define FX_CARBUNCLE_CAMERA_KEYS      0x13 /* table: camera key path for fxm_camKeyAdd (target 1) (same call site as fx_0b1 entry 0xF, same kind) */
#define FX_CARBUNCLE_CAMERA_KEYS_3    0x14 /* table: camera key path for fxm_camKeyAdd (target 3) (same call site as fx_0b1 entry 0x17, same kind) */
#define FX_CARBUNCLE_CAMERA_KEYS_2    0x15 /* table: camera key path for fxm_camKeyAdd (target 2) (same call site as fx_0b1 entry 0x19, same kind) */
#define FX_CARBUNCLE_ORBIT_GLOW_2     0x1F /* emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x16, same kind) */

/* fx_1ef choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes (the summon sequence), its
 * timers (fx_1ef_timers indices), the light beam, the camera key tracks; op81's streaks and target fade. Same
 * sequence and values as Carbuncle's fx_0b1 (twin names; the emitters differ: orbit glows for the motes, GLOW_RING for
 * the red ring). */
#define FX_CARBUNCLE_CAM_TRACKS     16   /* fx_1ef_camKeys: camera key tracks (fxm_camKeyAdd / fxm_camKeyUpdate) */
#define FX_CARBUNCLE_TIMERS         8    /* fx_1ef_timers: */
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
#define FX_CARBUNCLE_BEAM_POINTS    0x15 /* points of each FX_CARBUNCLE_LIGHT_BEAM strip (fx_1ef_trails) */
#define FX_CARBUNCLE_BEAM_HEIGHT    0x1800 /* the beam's height above the bone (FLAME_GLINT starts at its top) */
#define FX_CARBUNCLE_BEAM_WIDTH     0x40 /* line width of the inner strip (the outer one twice that) */
#define FX_CARBUNCLE_LENS_FLARE_TRAIL_SLOTS  0x28 /* FX_CARBUNCLE_LENS_FLARE's trail buffer fx_1ef_trails: slots of */
#define FX_CARBUNCLE_LENS_FLARE_TRAIL_STRIDE 0xAC /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_CARBUNCLE_STREAK_TRAIL_SLOTS  0x1E /* op81: WHITE_STREAK / WHITE_STREAK_2 trail buffers (fx_1ef_op81Trails0/1): slots of */
#define FX_CARBUNCLE_STREAK_TRAIL_STRIDE 0x9C /*   this many bytes */
#define FX_CARBUNCLE_STREAK_2_FRAME 0x10 /* op81: WHITE_STREAK_2 and GOLD_GLOW start at each target (WHITE_STREAK at frame 0) */
#define FX_CARBUNCLE_T_FADE_OUT     0    /* fx_1ef_fadeTimers: the targets fade out (btlfx_fadeUnitById 0x80 -> 0) while set */
#define FX_CARBUNCLE_T_FADE_IN      1    /*   the targets fade back in while set */
#define FX_CARBUNCLE_FADE_FRAMES    0x10 /*   both last this many frames (8 per frame: 0x10 * 8 = 0x80) */
#define FX_CARBUNCLE_FADE_OUT_FRAME 0x18 /* op81: the fade out starts */
#define FX_CARBUNCLE_FADE_IN_FRAME  0x28 /* op81: the fade in starts */
#define FX_CARBUNCLE_OP81_FRAMES    0x30 /* op81: the task ends at this frame */

typedef struct {
    u8 pad0[0x180];
    BtlMotion *motion180;
} Fx1efSummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1efSummonData *summon54; /* g_btlfxLoader.summon */
    u8 pad58[0x60 - 0x58];
    s32 model60; /* summon model */
} Fx1efSummon;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pos4[FX_CARBUNCLE_BEAM_POINTS];
} Fx1efTrail; /* 0xAC */

extern Unk800F78A4 *fx_1ef_env;
extern SVECTOR fx_1ef_modelRot; /* summon rotation */
extern VECTOR fx_1ef_modelPos; /* summon position */
extern VECTOR fx_1ef_modelScale; /* summon scale */
extern SVECTOR fx_1ef_emitPos0;
extern s32 fx_1ef_timers[FX_CARBUNCLE_TIMERS]; /* timers */
extern Pool6CEntry *fx_1ef_emitter0;
extern SVECTOR fx_1ef_emitter0Pos;
extern Pool6CEntry *fx_1ef_emitter1;
extern SVECTOR fx_1ef_emitter1Pos;
extern Pool6CEntry *fx_1ef_emitter2;
extern SVECTOR fx_1ef_emitter2Pos;
extern s32 fx_1ef_op80Trails[];
extern Pool6CEntry *fx_1ef_emitter3;
extern SVECTOR fx_1ef_emitter3Target;
extern SVECTOR fx_1ef_emitter3Pos;
extern Pool6CEntry *fx_1ef_emitter4;
extern SVECTOR fx_1ef_emitter4Pos;
extern Pool6CEntry *fx_1ef_emitter5;
extern SVECTOR fx_1ef_emitter5Pos;
extern Pool6CEntry *fx_1ef_emitter6;
extern SVECTOR fx_1ef_emitter6Pos;
extern Pool6CEntry *fx_1ef_emitter7;
extern SVECTOR fx_1ef_emitter7Pos;
extern Pool6CEntry *fx_1ef_emitter8;
extern SVECTOR fx_1ef_emitter8Pos;
extern Pool6CEntry *fx_1ef_emitter9;
extern SVECTOR fx_1ef_emitter9Pos;
extern Pool6CEntry *fx_1ef_emitter10;
extern SVECTOR fx_1ef_emitter10Pos;
extern Fx1efTrail fx_1ef_trails[2];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#define SUMMON(env) ((Fx1efSummon *)(env)->loader)
extern s32 fx_1ef_fadeTimers[FX_CARBUNCLE_T_FADE_IN + 1]; /* fade timers */
extern SVECTOR fx_1ef_op81TargetPos[4]; /* unit body positions */
extern s32 fx_1ef_op81Trails0[4][0x1FE];
extern s32 fx_1ef_op81Trails1[4][0x1FE];

/* copied */
extern Unk800F78A4 *fx_1ef_env;
/* end copied */

/* propagated */

typedef struct {
    s32 frame0; /* start frame */
    s32 timer4; /* frames left, -1 = free */
    u32 slot8; /* destination slots, 4 bits each (1-based) */
    u32 slotC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *keys10; /* data */
} Fx1efTrack; /* 0x14 */

extern s32 fx_1ef_camFrame; /* frame */
extern Fx1efTrack fx_1ef_camKeys[];
#define FX_1EF_KEY_FREE (-1) /* timer4: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
extern SVECTOR fx_1ef_camKeyPoints[];
static void fxm_camKeyUpdate(void);

#endif
