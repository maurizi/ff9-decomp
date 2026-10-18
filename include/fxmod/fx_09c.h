#ifndef FX_09C_H
#define FX_09C_H

/* Effect module fx_09c (dir 13 file 0x9C). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* unk10 + random */
    SVECTOR unk8; /* unk10 + random */
    SVECTOR unk10; /* caster body position */
    SVECTOR unk18; /* caster body position */
    Pool6CEntry *unk20;
    u8 pad24[0x2C - 0x24];
    SVECTOR unk2C;
    SVECTOR unk34; /* angles */
    Pool6CEntry *unk3C;
} Fx09cWork;

extern Unk800F78A4 *D_801E7BB8;

#endif
