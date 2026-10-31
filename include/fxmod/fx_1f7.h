#ifndef FX_1F7_H
#define FX_1F7_H

/* Effect module fx_1f7 (dir 13 file 0x1f7): enemy Blue Shockwave (Dummy/Necron). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR points4[33];
    s32 count10C; /* passes left */
    s32 index110; /* window start */
    s32 delay114; /* delay */
} Fx1f7Trail; /* 0x118 */

typedef struct {
    s32 index0; /* point */
    s32 offsetX4;
    s32 count8; /* repeats left */
    s32 frameC; /* frame */
    s32 delay10; /* delay */
} Fx1f7Spark; /* 0x14 */

typedef struct {
    s32 active0; /* trails on */
    s32 active4; /* sparks on */
    s32 timer8; /* flash */
    SVECTOR posC; /* caster body position */
} Fx1f7Work; /* 0x14 */

extern Unk800F78A4 *fx_1f7_env;
extern s32 fx_1f7_trailBones[6][17]; /* bones */
extern s32 fx_1f7_sparkBones[2][7]; /* bones */
extern SVECTOR fx_1f7_trailPoints[64];
extern SVECTOR fx_1f7_sparkPoints[48];
extern Fx1f7Trail fx_1f7_trails[6];
extern Fx1f7Spark fx_1f7_sparks[12];

#endif
