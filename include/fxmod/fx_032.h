#ifndef FX_032_H
#define FX_032_H

/* Effect module fx_032 (dir 13 file 0x32). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* target body position */
    SVECTOR unk8;
    SVECTOR unk10;
    SVECTOR unk18;
    SVECTOR unk20;
    SVECTOR unk28;
    SVECTOR unk30;
    SVECTOR unk38;
    SVECTOR unk40;
    Pool6CEntry *emit48;
    Pool6CEntry *emit4C;
    Pool6CEntry *emit50;
    Pool6CEntry *emit54;
} Fx032Work; /* 0x58 */

extern Unk800F78A4 *fx_032_env;

#endif
