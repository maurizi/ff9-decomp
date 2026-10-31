#ifndef FX_1EA_H
#define FX_1EA_H

/* Effect module fx_1ea (dir 13 file 0x1ea): Dragon's Crest. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1eaPoint;

typedef struct {
    s16 unk0;
    u16 count2; /* count */
    Fx1eaPoint *keys4;
} Fx1eaPath;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR rot10; /* caster rotation */
} Fx1eaWork80; /* 0x18 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pos4[1];
} Fx1eaTrail;

typedef struct {
    s16 timer0; /* fade-in countdown */
    s16 timer2; /* fade-out countdown */
} Fx1eaWork82;

extern SVECTOR fx_1ea_trailOrigin; /* origin */


static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);
static s32 fxm_posPathGet(Fx1eaPath *path, s32 t, SVECTOR *out);

/* copied */
extern Unk800F78A4 *fx_1ea_env;
/* end copied */

/* propagated */

typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx1eaKey; /* 0xC */

typedef struct {
    s16 unk0;
    u16 count2; /* keys */
    Fx1eaKey *keys4;
} Fx1eaPath_0b4;
static s32 fxm_rotPosPathGet(Fx1eaPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1eaVec3;

typedef struct {
    Pool6CEntry *emit0[11];
    SVECTOR rot2C; /* model rotation */
    Fx1eaVec3 pos34; /* model position */
    s32 pad40;
    Fx1eaVec3 scale44; /* model scale */
    s32 pad50;
    s32 rgb54; /* model colour */
    u8 pad58[0x84 - 0x58];
    SVECTOR pos84; /* caster position */
    SVECTOR rot8C; /* caster rotation */
    s32 frame94; /* morph frame */
    s32 timer98; /* fade-in countdown */
    s32 timer9C;
    s32 timerA0;
    u16 modelA4[10]; /* models */
} Fx1eaWork81; /* 0xB8 */

typedef struct {
    Fx1eaPath path0;
    s16 index8; /* base */
    s16 countA; /* count */
    s16 alphaC; /* alpha */
    s16 scaleE;
    s16 unk10; /* pad */
    u8 pad12[0x18 - 0x12];
} Fx1eaRibbon; /* 0x18 */

extern u16 fx_1ea_morphModelFileIds[];
extern u16 fx_1ea_sliceModelFileIds[];
extern u16 fx_1ea_slicePathFileIds[];
extern u16 fx_1ea_trailPathFileIds0[];
extern u16 fx_1ea_trailPathFileIds1[];
extern BtlModelBuf fx_1ea_effModels[];
extern Fx1eaRibbon fx_1ea_ribbon0;
extern Fx1eaRibbon fx_1ea_ribbon1;
extern Fx1eaPath_0b4 fx_1ea_rotPosPath0;
extern Fx1eaPath_0b4 fx_1ea_rotPosPath1[];
extern u8 fx_1ea_ribbonTrails0[][0x100];
extern u8 fx_1ea_ribbonTrails1[][0x88];

void fx_1ea_buildTrail(Fx1eaPath *path, Fx1eaTrail *trail, s32 size, s32 base, s32 start, s32 pad);
void fx_1ea_buildTrailRotY(Fx1eaPath *path, Fx1eaTrail *trail, s32 size, s32 base, s32 start, s32 pad, s32 ang);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_posPathInit(Fx1eaPath *path, u16 *data);
static void fxm_rotPosPathInit(Fx1eaPath *path, u16 *data);

#endif
