#ifndef FX_020_H
#define FX_020_H

/* Effect module fx_020 (dir 13 file 0x20). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
} Fx020Work;

extern SVECTOR fx_020_emitRot;
extern Unk800F78A4 *fx_020_env;
extern s32 fx_020_trails[];

#endif
