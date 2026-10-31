#ifndef FX_104_H
#define FX_104_H

/* Effect module fx_104 (dir 13 file 0x104). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4;
    SVECTOR pos8;
    SVECTOR unk10;
} Fx104Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC;
} Fx104Work81;

extern Unk800F78A4 *fx_104_env;
extern s32 fx_104_op81Trails[];

#endif
