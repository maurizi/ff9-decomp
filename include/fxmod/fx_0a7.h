#ifndef FX_0A7_H
#define FX_0A7_H

/* Effect module fx_0a7 (dir 13 file 0xA7). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    s32 unk8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* unit 0x10 body position */
    SVECTOR pos20; /* caster body position */
    SVECTOR pos28; /* caster position */
    SVECTOR pos30; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos38; /* unk30 moved 0x12C along unk48 */
    SVECTOR pos40; /* unk30 moved 0xC8 along unk48 */
    SVECTOR unk48; /* angles from unk30 to unk20 */
    SVECTOR unk50; /* angles from unk20 to unk30 */
} Fx0a7Work;

extern Unk800F78A4 *fx_0a7_env;
extern s32 D_801E7C88[];
extern s32 D_801E7F48[];
extern s32 D_801E8388[];

#endif
