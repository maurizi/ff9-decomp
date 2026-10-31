#ifndef FX_092_H
#define FX_092_H

/* Effect module fx_092 (dir 13 file 0x92). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* unit 0x10 body position */
    SVECTOR rot18; /* unit 0x10 rotation */
    SVECTOR pos20; /* caster position */
    SVECTOR pos28; /* btlfx_getUnitArgBonePos(0x10, 0), moved 0xC8 along unk18 */
    SVECTOR pos30; /* unk20 raised by 0x1C2 */
    SVECTOR pos38; /* unk20 raised by 0xC8 */
    SVECTOR unk40; /* angles from env->camera->unk54 to unk28 */
} Fx092Work;

extern Unk800F78A4 *fx_092_env;

#endif
