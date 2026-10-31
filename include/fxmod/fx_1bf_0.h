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
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR points4[2];
} Fx1bf0Line;

extern Fx1bf0Line fx_1bf_0_lineTrail;
static void fxm_drawTrailLine(u8 *file, SVECTOR *a, SVECTOR *b, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern s16 fx_1bf_0_strandParams[18 * 4]; /* per strand: file, start, stride, width */
extern Fx1bf0Line *fx_1bf_0_strandTrails[18];
void fx_1bf_0_drawStrands(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 w, s32 a, s32 fade, s32 flag);
extern s16 fx_1bf_0_pointFileIds[];

typedef struct {
    s16 x, y, z;
} Fx1bf0Pt;
void fxm_drawPatternParticles(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot);

typedef struct {
    s32 state0; /* state */
    s32 unk4; /* frame */
    SVECTOR pos8;
    SVECTOR unk10;
    u8 pad18[0x28 - 0x18];
} Fx1bf0Work; /* 0x28 */

extern SVECTOR fx_1bf_0_origin;
extern BtlModel fx_1bf_0_effModels[];
extern u8 D_801EA1FC[];
s32 fx_1bf_0_op80(s32 mode, Fx1bf0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
