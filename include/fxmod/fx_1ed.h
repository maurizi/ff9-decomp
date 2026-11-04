#ifndef FX_1ED_H
#define FX_1ED_H

/* Effect module fx_1ed (dir 13 file 0x1ed): Carbuncle (3). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1ed effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CARBUNCLE_BLUE_RINGS       0x1 /* emitter: pale blue gouraud rings at each target (op 0x81) */
#define FX_CARBUNCLE_CAMERA_KEYS_3    0x3 /* table: camera key path for fxm_camKeyAdd (target 3) (same call site as fx_0b1 entry 0x17, same kind) */
#define FX_CARBUNCLE_CAMERA_KEYS      0x4 /* table: camera key path for fxm_camKeyAdd (target 1) (same call site as fx_0b1 entry 0xF, same kind) */
#define FX_CARBUNCLE_CAMERA_KEYS_2    0x5 /* table: camera key path for fxm_camKeyAdd (target 2) (same call site as fx_0b1 entry 0x19, same kind) */
#define FX_CARBUNCLE_LENS_FLARE       0xF /* emitter: orbiting white glow trail with a blue lens streak (same call site as fx_0b1 entry 0x9, same kind) */
#define FX_CARBUNCLE_ORBIT_GLOW       0x10 /* emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0xA, same kind) */
#define FX_CARBUNCLE_GLOW_RING        0x11 /* emitter: white glow fans and a white / pale green gouraud ring (same call site as fx_0b1 entry 0xC, same kind) */
#define FX_CARBUNCLE_RING_BURST_2     0x12 /* emitter: cone of white, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x1, same kind) */
#define FX_CARBUNCLE_LENS_STREAK      0x13 /* emitter: blue-white lens streak and a gouraud ring (same call site as fx_0b1 entry 0xE, same kind) */
#define FX_CARBUNCLE_FLAME_GLINT      0x14 /* emitter: orange-blue flame-shaped glints, gouraud rings, fans (same call site as fx_0b1 entry 0x13, same kind) */
#define FX_CARBUNCLE_RING_BURST_3     0x15 /* emitter: cone of white, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x14, same kind) */
#define FX_CARBUNCLE_LENS_STREAK_2    0x16 /* emitter: blue-white lens streak, sparkle sprites (same call site as fx_0b1 entry 0x15, same kind) */
#define FX_CARBUNCLE_ORBIT_GLOW_3     0x17 /* emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x24, same kind) */
#define FX_CARBUNCLE_ORBIT_GLOW_2     0x18 /* emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x16, same kind) */
#define FX_CARBUNCLE_RING_BURST       0x19 /* emitter: cone of white-yellow, lilac, cyan and blue-grey gouraud rings (same call site as fx_0b1 entry 0x11, same kind) */
#define FX_CARBUNCLE_CRYSTAL_SPARKLES 0x1A /* emitter: white crystal-flake sprites and blue sparkles, fans (same call site as fx_0b1 entry 0x12, same kind) */
#define FX_CARBUNCLE_WHITE_LIGHT_STREAK 0x1B /* emitter: one fan, white glow streak (same call site as fx_0b1 entry 0xD, same kind) */
#define FX_CARBUNCLE_LIGHT_BEAM       0x1D /* trail: blue-white light beam, drawn twice (same call site as fx_0b1 entry 0x10, same kind) */

typedef struct {
    u8 pad0[0x180];
    BtlMotion *motion180;
} Fx1edSummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1edSummonData *summon54; /* g_btlfxLoader.summon */
    u8 pad58[0x60 - 0x58];
    s32 model60; /* summon model */
} Fx1edSummon;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pos4[21];
} Fx1edTrail; /* 0xAC */

extern Unk800F78A4 *fx_1ed_env;
extern SVECTOR fx_1ed_modelRot; /* summon rotation */
extern VECTOR fx_1ed_modelPos; /* summon position */
extern VECTOR fx_1ed_modelScale; /* summon scale */
extern SVECTOR fx_1ed_emitPos0;
extern s32 fx_1ed_timers[8]; /* timers */
extern Pool6CEntry *fx_1ed_emitter0;
extern SVECTOR fx_1ed_emitter0Pos;
extern Pool6CEntry *fx_1ed_emitter1;
extern SVECTOR fx_1ed_emitter1Pos;
extern Pool6CEntry *fx_1ed_emitter2;
extern SVECTOR fx_1ed_emitter2Pos;
extern s32 fx_1ed_op80Trails[];
extern Pool6CEntry *fx_1ed_emitter3;
extern SVECTOR fx_1ed_emitter3Target;
extern SVECTOR fx_1ed_emitter3Pos;
extern Pool6CEntry *fx_1ed_emitter4;
extern SVECTOR fx_1ed_emitter4Pos;
extern Pool6CEntry *fx_1ed_emitter5;
extern SVECTOR fx_1ed_emitter5Pos;
extern Pool6CEntry *fx_1ed_emitter6;
extern SVECTOR fx_1ed_emitter6Pos;
extern Pool6CEntry *fx_1ed_emitter7;
extern SVECTOR fx_1ed_emitter7Pos;
extern Pool6CEntry *fx_1ed_emitter8;
extern SVECTOR fx_1ed_emitter8Pos;
extern Pool6CEntry *fx_1ed_emitter9;
extern SVECTOR fx_1ed_emitter9Pos;
extern Pool6CEntry *fx_1ed_emitter10;
extern SVECTOR fx_1ed_emitter10Pos;
extern Fx1edTrail fx_1ed_trails[2];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#define SUMMON(env) ((Fx1edSummon *)(env)->loader)

/* copied */
extern Unk800F78A4 *fx_1ed_env;
extern SVECTOR fx_1ed_op81TargetPos[];
/* end copied */

/* propagated */

typedef struct {
    s32 frame0; /* start frame */
    s32 timer4; /* frames left, -1 = free */
    u32 slot8; /* destination slots, 4 bits each (1-based) */
    u32 slotC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *keys10; /* data */
} Fx1edTrack; /* 0x14 */

extern s32 fx_1ed_camFrame; /* frame */
extern Fx1edTrack fx_1ed_camKeys[];
#define FX_1ED_KEY_FREE (-1) /* timer4: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
extern SVECTOR fx_1ed_camKeyPoints[];
static void fxm_camKeyUpdate(void);

#endif
