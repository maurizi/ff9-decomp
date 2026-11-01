#ifndef FX_1BF_0_H
#define FX_1BF_0_H

/* Effect module fx_1bf_0 (dir 13 file 0x1bf): Ark (short); Eternal Darkness (load group 0). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* copied */
extern SVECTOR fx_1bf_0_particleOffsets[3];
extern s16 fx_1bf_0_particleFileIds[16];
extern Unk800F78A4 *fx_1bf_0_env;
/* end copied */

/* propagated */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[2];
} Fx1bf0Line;

extern Fx1bf0Line fx_1bf_0_lineTrail;
static void fxm_drawTrailLine(u8 *file, SVECTOR *a, SVECTOR *b, s32 frame, s32 fade, s32 abr, s32 lineWidth);

extern s16 fx_1bf_0_strandParams[18 * 4]; /* per strand: file, start, stride, width */
extern Fx1bf0Line *fx_1bf_0_strandTrails[18];
void fx_1bf_0_drawStrands(Unk80100C78Entry *entry, SVECTOR *from, s32 reach, s32 lineWidth, s32 alpha, s32 fade, s32 flag);
extern s16 fx_1bf_0_pointFileIds[];

typedef struct {
    s16 x, y, z;
} Fx1bf0Pt;
void fxm_drawPatternParticles(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot);

typedef struct {
    s32 state0; /* state */
    s32 unk4; /* write-only: frame counter */
    SVECTOR pos8;
    SVECTOR unk10; /* write-only (copy of pos8) */
    u8 pad18[0x28 - 0x18];
} Fx1bf0Work; /* 0x28 */

extern SVECTOR fx_1bf_0_origin;
extern BtlModelBuf fx_1bf_0_effModels[];
extern u8 fx_1bf_0_buf400[];
s32 fx_1bf_0_op80(s32 mode, Fx1bf0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
