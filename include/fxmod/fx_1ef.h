#ifndef FX_1EF_H
#define FX_1EF_H

/* Effect module fx_1ef (dir 13 file 0x1ef): Carbuncle (4). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

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
    SVECTOR pos4[21];
} Fx1efTrail; /* 0xAC */

extern Unk800F78A4 *fx_1ef_env;
extern SVECTOR fx_1ef_modelRot; /* summon rotation */
extern VECTOR fx_1ef_modelPos; /* summon position */
extern VECTOR fx_1ef_modelScale; /* summon scale */
extern SVECTOR fx_1ef_emitPos0;
extern s32 fx_1ef_timers[8]; /* timers */
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
extern s32 fx_1ef_fadeTimers[2]; /* fade timers */
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
static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
extern SVECTOR fx_1ef_camKeyPoints[];
static void fxm_camKeyUpdate(void);

#endif
