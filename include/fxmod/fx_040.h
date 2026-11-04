#ifndef FX_040_H
#define FX_040_H

/* Effect module fx_040 (dir 13 file 0x40). */
#include "btlfx_mod.h"

/* fx_040 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BUBBLES_BUBBLE_POP 0x3 /* emitter: white bubble-shine quads */
#define FX_BUBBLES_CYAN_RING  0xA /* emitter: cyan-white gouraud ring */
#define FX_BUBBLES_BUBBLES    0xB /* emitter: iridescent soap bubbles */

typedef struct {
    Pool6CEntry *emit0[4];
    u8 pad10[8];
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
    Pool6CEntry *emit24;
    SVECTOR pos28; /* target position */
    SVECTOR pos30; /* left of the target */
    SVECTOR pos38; /* right of the target */
    SVECTOR pos40;
    u8 pad48[8];
} Fx040Work; /* 0x50 */

extern u16 fx_040_emitBlocks[4]; /* file data indices */
extern s32 fx_040_op80ParticleMotions[4]; /* particle callbacks */
extern Unk800F78A4 *fx_040_env;
extern SVECTOR fx_040_origin; /* particle origin */
extern SVECTOR fx_040_aboveTarget; /* above the target */
extern SVECTOR fx_040_emitPos[4];

s32 fx_040_op80ParticleMotion0(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_040_op80ParticleMotion1(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_040_op80ParticleMotion2(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_040_op80ParticleMotion3(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_040_flyToTarget(VECTOR *out, SVECTOR *pos, SVECTOR *target, s32 frame, s32 n);

#endif
