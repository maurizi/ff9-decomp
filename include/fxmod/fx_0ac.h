#ifndef FX_0AC_H
#define FX_0AC_H

/* Effect module fx_0ac (dir 13 file 0xAC). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* caster position */
    SVECTOR pos20; /* unit 0x10 bone position, raised by 0x96 */
    SVECTOR unk28; /* angles from unk20 to unk10 */
} Fx0acWork;

extern Unk800F78A4 *fx_0ac_env;
extern s32 D_801E7A38[];

#endif
