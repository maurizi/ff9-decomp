#ifndef FX_043_H
#define FX_043_H

/* Effect module fx_043 (dir 13 file 0x43). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    u8 pad4[8];
    SVECTOR posC;
    SVECTOR pos14; /* above the target body */
} Fx043Work80; /* 0x1C */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position */
    SVECTOR pos10; /* target position */
    SVECTOR pos18;
    SVECTOR aim20; /* angles from pos8 to fx_043_particleFrom */
} Fx043Work81; /* 0x28 */

extern Unk800F78A4 *fx_043_env;
extern SVECTOR fx_043_particleFrom; /* particle start */
extern SVECTOR fx_043_particleTo; /* particle end */
extern SVECTOR fx_043_emitPos;
extern s32 fx_043_trails[];
extern s32 fx_043_trails2[];

static s32 fx_043_op80ParticleMotion(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 t, s32 n);

#endif
