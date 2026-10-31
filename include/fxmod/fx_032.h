#ifndef FX_032_H
#define FX_032_H

/* Effect module fx_032 (dir 13 file 0x32). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* target body position */
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR pos20;
    SVECTOR pos28;
    SVECTOR pos30;
    SVECTOR pos38;
    SVECTOR pos40;
    Pool6CEntry *emit48;
    Pool6CEntry *emit4C;
    Pool6CEntry *emit50;
    Pool6CEntry *emit54;
} Fx032Work; /* 0x58 */

extern Unk800F78A4 *fx_032_env;

#endif
