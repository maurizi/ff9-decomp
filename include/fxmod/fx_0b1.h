#ifndef FX_0B1_H
#define FX_0B1_H

/* Effect module fx_0b1 (dir 13 file 0xB1): Carbuncle, full summon. Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    s32 start0; /* start frame */
    s32 framesLeft4; /* frames left, -1 = free */
    u32 dst8; /* destination slots, 4 bits each (1-based) */
    u32 srcC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *data10; /* data */
} Fx0b1Track; /* 0x14 */

extern s32 fx_0b1_camKeyFrame; /* frame */
extern Fx0b1Track fx_0b1_camKeys[16];
extern SVECTOR fx_0b1_camKeyPoints[];
extern s32 fx_0b1_fadeTimer; /* fade timer */
extern s32 fx_0b1_models[4]; /* models */
extern SVECTOR fx_0b1_modelRot; /* model rotation */
extern VECTOR fx_0b1_modelScale; /* model scale */
extern VECTOR fx_0b1_modelPos[4]; /* model positions */
extern SVECTOR fx_0b1_bodyPos[4]; /* unit body positions */
extern BtlModel fx_0b1_effModels[];

static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
static void fxm_camKeyUpdate(void);
extern Unk800F78A4 *fx_0b1_env;
static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *motion180;
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
    SVECTOR pts4[21];
} Fx0b1Trail; /* 0xAC */

#define SUMMON0B1(env) ((Fx0b1Summon *)(env)->loader)

extern SVECTOR fx_0b1_summonRot;
extern VECTOR fx_0b1_summonPos;
extern VECTOR fx_0b1_summonScale;
extern SVECTOR fx_0b1_startPos;
extern s32 fx_0b1_timers[8];
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
