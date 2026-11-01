#ifndef FX_1E6_H
#define FX_1E6_H

/* Effect module fx_1e6 (dir 13 file 0x1e6): enemy Correct!!! (False/Ragtime Mouse/True). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1e6Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1e6Point *keys4;
} Fx1e6Path;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pos4[1];
} Fx1e6Trail;

typedef struct {
    s32 unk0; /* unused */
    SVECTOR pos4; /* caster position */
    u16 alphaC; /* fade */
    s16 unkE; /* unused */
} Fx1e6Work80; /* 0x10 */

typedef struct {
    Pool6CEntry *emit0[7];
    SVECTOR rot1C; /* model rotation */
    VECTOR pos24; /* model position */
    VECTOR scale34; /* model scale */
    s32 rgb44; /* brightness */
    SVECTOR pos48; /* caster body position */
    SVECTOR pos50;
    s32 frame58; /* morph counter */
    s32 timer5C; /* fade-in countdown */
    s32 timer60; /* fade-out countdown */
    u16 model64; /* models */
    u16 model66;
} Fx1e6Work81; /* 0x68 */

extern Unk800F78A4 *fx_1e6_env;
extern BtlModelBuf fx_1e6_effModels[];
extern Fx1e6Path fx_1e6_posPath;
extern s32 fx_1e6_op81Trails0[];
extern s32 fx_1e6_op81Trails1[];
extern s32 fx_1e6_op81Trails2[];
extern s32 fx_1e6_op81Trails3[];
extern s32 fx_1e6_op81Trails4[];
extern s32 fx_1e6_op81Trails5[];
extern Fx1e6Trail fx_1e6_op80Trail;
extern SVECTOR fx_1e6_trailOrigin; /* origin */
extern SVECTOR fx_1e6_trailRot; /* rotation */

void fx_1e6_buildTrail(Fx1e6Trail *trail, s32 size, s32 start);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_posPathInit(Fx1e6Path *path, u16 *data);
static s32 fxm_posPathGet(Fx1e6Path *path, s32 t, SVECTOR *out);

/* copied */
extern Unk800F78A4 *fx_1e6_env;
/* end copied */

#endif
