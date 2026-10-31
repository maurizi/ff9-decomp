#ifndef FX_1F2_H
#define FX_1F2_H

/* Effect module fx_1f2 (dir 13 file 0x1f2): enemy Grand Cross (Dummy/Necron). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* copied */
extern Unk800F78A4 *fx_1f2_env;
/* end copied */

typedef struct {
    s32 active0; /* models drawn */
    s32 timer4; /* flash model timer */
    s32 timer8; /* twin model timer */
    s32 timerC; /* tint timers */
    s32 timer10;
    s32 timer14;
    s32 timer18;
    s32 unk1C;
    s32 timer20; /* screen ripple timer */
    s32 timer24;
    s32 timer28;
    s32 spin2C; /* spin speed */
    s32 unk30;
    s32 timer34; /* sparks timer */
    s32 halfWidth38; /* ribbon half-width, stored in the trail points' pad */
    s32 timer3C;
    s32 done40;
    s32 timer44;
    s32 done48;
    s32 timer4C;
    s32 timer50;
    s32 timer54;
    s32 timer58;
    s32 pad5C[(0x80 - 0x5C) / 4];
    Pool6CEntry *emit80;
    Pool6CEntry *emit84;
    Pool6CEntry *emit88;
    Pool6CEntry *emit8C;
    s32 pad90[(0x100 - 0x90) / 4];
    SVECTOR pos100;
    SVECTOR pos108;
    SVECTOR bonePos110[2]; /* bone 5 and bone 9 positions */
    SVECTOR pos120; /* midpoint */
} Fx1f2Work; /* 0x128 */

typedef struct {
    s32 model0; /* model */
    s32 radius4; /* radius */
    SVECTOR rot8; /* rotation */
    VECTOR pos10; /* position */
    VECTOR scale20; /* scale */
} Fx1f2Model; /* 0x30 */

typedef struct {
    s32 model0; /* models */
    s32 model4;
    s32 model8;
    s32 radiusC; /* radius */
    SVECTOR rot10; /* rotation */
    VECTOR pos18; /* position */
    VECTOR scale28; /* scale */
} Fx1f2Ring; /* 0x38 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[9];
    s32 unk4C; /* timer */
    SVECTOR pos50[16]; /* points (pad = next) */
    SVECTOR velD0[16]; /* velocities */
} Fx1f2Trail; /* 0x150 */

extern Fx1f2Model fx_1f2_model0;
extern Fx1f2Model fx_1f2_model1;
extern Fx1f2Model fx_1f2_model2;
extern Fx1f2Ring fx_1f2_orbitModels[4];
extern Fx1f2Model fx_1f2_model3;
extern Fx1f2Model fx_1f2_model4[2];
extern Fx1f2Model fx_1f2_model5;
extern Fx1f2Model fx_1f2_model6[2];
extern Fx1f2Model fx_1f2_model7;
extern Fx1f2Model fx_1f2_model8[4];
extern SVECTOR fx_1f2_sparkRot;
extern Fx1f2Model fx_1f2_model9;
extern VECTOR fx_1f2_model9Pos;
extern Fx1f2Model fx_1f2_model10;
extern SVECTOR fx_1f2_emitPos0; /* target */
extern SVECTOR fx_1f2_faceAngles; /* angles to the target */
extern SVECTOR fx_1f2_emitPos1;
extern SVECTOR fx_1f2_unitPos[4]; /* unit positions */
extern Fx1f2Trail fx_1f2_trails[16];
extern Fx1f2Trail fx_1f2_unitTrails[16]; /* 0x1500 bytes up to fx_1f2_effModels, cleared as 16; only entries below
                                         * targetCount are drawn */
extern BtlModel fx_1f2_effModels[];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
