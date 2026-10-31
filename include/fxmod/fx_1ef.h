#ifndef FX_1EF_H
#define FX_1EF_H

/* Effect module fx_1ef (dir 13 file 0x1ef): Carbuncle (4). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *motion180;
} Fx1efSummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1efSummonData *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx1efSummon;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[21];
} Fx1efTrail; /* 0xAC */

extern Unk800F78A4 *fx_1ef_env;
extern SVECTOR D_801E8D14; /* summon rotation */
extern VECTOR D_801E8D1C; /* summon position */
extern VECTOR D_801E8D2C; /* summon scale */
extern SVECTOR D_801E8D3C;
extern s32 D_801E8D4C[8]; /* timers */
extern Pool6CEntry *fx_1ef_emitter0;
extern SVECTOR D_801E8D74;
extern Pool6CEntry *fx_1ef_emitter1;
extern SVECTOR D_801E8D84;
extern Pool6CEntry *fx_1ef_emitter2;
extern SVECTOR D_801E8D94;
extern s32 fx_1ef_op80Trails[];
extern Pool6CEntry *fx_1ef_emitter3;
extern SVECTOR D_801EA884;
extern SVECTOR D_801EA88C;
extern Pool6CEntry *fx_1ef_emitter4;
extern SVECTOR D_801EA89C;
extern Pool6CEntry *fx_1ef_emitter5;
extern SVECTOR D_801EA8AC;
extern Pool6CEntry *fx_1ef_emitter6;
extern SVECTOR D_801EA8BC;
extern Pool6CEntry *fx_1ef_emitter7;
extern SVECTOR D_801EA8CC;
extern Pool6CEntry *fx_1ef_emitter8;
extern SVECTOR D_801EA8DC;
extern Pool6CEntry *fx_1ef_emitter9;
extern SVECTOR D_801EA8EC;
extern Pool6CEntry *fx_1ef_emitter10;
extern SVECTOR D_801EA8FC;
extern Fx1efTrail D_801EA90C[2];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#define SUMMON(env) ((Fx1efSummon *)(env)->loader)
extern s32 D_801EAA64[2]; /* fade timers */
extern SVECTOR D_801EAA6C[4]; /* unit body positions */
extern s32 fx_1ef_op81Trails0[4][0x1FE];
extern s32 fx_1ef_op81Trails1[4][0x1FE];

/* copied */
extern Unk800F78A4 *fx_1ef_env;
/* end copied */

/* propagated */

typedef struct {
    s32 unk0; /* start frame */
    s32 unk4; /* frames left, -1 = free */
    u32 unk8; /* destination slots, 4 bits each (1-based) */
    u32 unkC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *unk10; /* data */
} Fx1efTrack; /* 0x14 */

extern s32 D_801E8BA0; /* frame */
extern Fx1efTrack D_801E8BAC[];
static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
extern SVECTOR D_801E8CEC[];
static void fxm_camKeyUpdate(void);

#endif
