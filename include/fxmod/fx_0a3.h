#ifndef FX_0A3_H
#define FX_0A3_H

/* Effect module fx_0a3 (dir 13 file 0xA3). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* unit 0x10 body position, moved 0x190 along unk18 */
    SVECTOR unk10; /* unit 0x10 position, moved 0xC8 along unk18 */
    SVECTOR unk18; /* unit 0x10 rotation */
} Fx0a3Work;

extern Unk800F78A4 *D_801E791C;
extern s32 D_801E792C[];
extern s32 D_801E800C[];

#endif
