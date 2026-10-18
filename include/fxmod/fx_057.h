#ifndef FX_057_H
#define FX_057_H

/* Effect module fx_057 (dir 13 file 0x57). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801E8EA4;
extern SVECTOR D_801EA6F4; /* camera rest position */
extern SVECTOR D_801EA6FC; /* particle head position */
extern Pool6CEntry *D_801EA704;
extern Unk80100C78Entry *D_801EA97C;

void func_801E7704(Pool30Entry *pt, Pool6CEntry *e, SVECTOR *pos, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 t,
                   s32 n);

#endif
