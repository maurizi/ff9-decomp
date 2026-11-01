#ifndef FX_179_H
#define FX_179_H

/* Effect module fx_179 (dir 13 file 0x179): Medeo. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR verts0[3];
} Fx179Tri; /* model triangle */

typedef struct {
    s32 pos0; /* centre, 20.12 */
    s32 pos4;
    s32 pos8;
    s16 velC; /* velocity */
    s16 velE;
    s16 vel10;
    s16 unk12; /* write-only: scale (0x1000) */
    s16 rot14; /* spin */
    s16 rot16;
    s16 frame18; /* frame, -1 = gone */
    s16 delay1A; /* first frame */
} Fx179Shard; /* 0x1C */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* orb position */
    SVECTOR pos10; /* caster position */
    s32 model18; /* model */
} Fx179Work;

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx179Rgbc; /* primitive colour word */

extern Unk800F78A4 *fx_179_env;
extern BtlModelBuf fx_179_effModels[];
extern u8 fx_179_op80Trails[];
extern Fx179Tri fx_179_shatterTris[160];
extern Fx179Shard fx_179_shatterPieces[160];

static void fxm_shatterModel(s32 model, SVECTOR *center);
void fxm_updateShatterPieces(void);
static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);
s32 fx_179_op80(s32 mode, Fx179Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
