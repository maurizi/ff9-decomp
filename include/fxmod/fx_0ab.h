#ifndef FX_0AB_H
#define FX_0AB_H

/* Effect module fx_0ab (dir 13 file 0xAB). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR unk10; /* caster position */
    SVECTOR unk18; /* unk10 raised by 0xC8 */
    SVECTOR unk20; /* unk10 raised by 0x190 */
} Fx0abWork;

extern Unk800F78A4 *fx_0ab_env;
extern s32 D_801E7928[];

#endif
