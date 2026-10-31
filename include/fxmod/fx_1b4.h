#ifndef FX_1B4_H
#define FX_1B4_H

/* Effect module fx_1b4 (dir 13 file 0x1b4). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1b4Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1b4Point *unk4;
} Fx1b4Path;

typedef struct {
    Pool6CEntry *emit0[8];
    Pool6CEntry *emit20[8];
    Pool6CEntry *emit40;
    SVECTOR unk44; /* target position */
    SVECTOR unk4C[8];
    SVECTOR pos8C;
} Fx1b4Work80; /* 0x94 */

typedef struct {
    Pool6CEntry *emit0[4];
    SVECTOR pos10; /* target position */
} Fx1b4Work81; /* 0x18 */

typedef struct {
    Pool6CEntry *emit0[7];
    SVECTOR pos1C; /* target position */
} Fx1b4Work82; /* 0x24 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
} Fx1b4Work83; /* 0xC */

extern u16 D_801E85BC[8];
extern u16 D_801E85CC[8];
extern u16 D_801E85DC[8];
extern s32 (*fx_1b4_op80ParticleMotions[8])();
extern Unk800F78A4 *fx_1b4_env;
extern Fx1b4Path D_801E8664[8];
extern Fx1b4Path D_801E86A4[8];
extern s32 D_801E86E4[8][0x3C];
extern s32 D_801E8E64[8][0xE0];
extern s32 D_801EAA64[];
extern s32 D_801ECE64[];
extern s32 D_801EE0F4[];
extern SVECTOR D_801EF174; /* origin */
extern SVECTOR D_801EF17C;

static s32 fxm_followPathParticle(Pool30Entry *e, SVECTOR *pos, Fx1b4Path *path, s32 frame, s32 frames);
static void fxm_posPathInit(Fx1b4Path *path, u16 *data);
static s32 fxm_posPathGet(Fx1b4Path *path, s32 t, SVECTOR *out);

/* propagated */
s32 fx_1b4_op80ParticleMotion0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion2(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion3(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion5(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion6(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion7(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);

#endif
