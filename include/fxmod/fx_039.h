#ifndef FX_039_H
#define FX_039_H

/* Effect module fx_039 (dir 13 file 0x39). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
    SVECTOR pos10; /* target body position */
} Fx039Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* target position */
    SVECTOR pos18; /* projectile position */
    s32 proj20; /* projectile index (0..2) */
} Fx039Work81; /* 0x24 */

extern Unk800F78A4 *fx_039_env;
extern s32 fx_039_trails[][0x318];
extern SVECTOR fx_039_emitPos[];

#endif
