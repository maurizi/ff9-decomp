#ifndef FX_1EE_H
#define FX_1EE_H

/* Effect module fx_1ee (dir 13 file 0x1ee): Carbuncle (2). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *motion180;
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
    SVECTOR pos4[21];
} Fx1eeTrail; /* 0xAC */

extern Unk800F78A4 *fx_1ee_env;
extern SVECTOR fx_1ee_modelRot; /* summon rotation */
extern VECTOR fx_1ee_modelPos; /* summon position */
extern VECTOR fx_1ee_modelScale; /* summon scale */
extern SVECTOR fx_1ee_emitPos0;
extern s32 fx_1ee_timers[8]; /* timers */
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
extern Fx1eeSpark fx_1ee_sparks[12][4];

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
static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
extern SVECTOR fx_1ee_camKeyPoints[];
static void fxm_camKeyUpdate(void);

#endif
