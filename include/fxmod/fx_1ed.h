#ifndef FX_1ED_H
#define FX_1ED_H

/* Effect module fx_1ed (dir 13 file 0x1ed): Carbuncle (3). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *motion180;
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
static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
extern SVECTOR fx_1ed_camKeyPoints[];
static void fxm_camKeyUpdate(void);

#endif
