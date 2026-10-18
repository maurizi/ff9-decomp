#ifndef FX_057_H
#define FX_057_H

/* Effect module fx_057 (dir 13 file 0x57). */
#include "btlfx_mod.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx057Vec3;

typedef struct {
    s32 unk0;
    s32 unk4; /* frame */
    s32 unk8;
    SVECTOR unkC; /* position */
    SVECTOR unk14;
    Fx057Vec3 unk1C;
    s32 unk28; /* model */
} Fx057Work81; /* 0x2C */

extern SVECTOR D_801EA714;
extern Unk800F78A4 *D_801E8EA4;
extern SVECTOR D_801EA6F4; /* camera rest position */
extern SVECTOR D_801EA6FC; /* particle head position */
extern Pool6CEntry *D_801EA704;
extern Unk80100C78Entry *D_801EA97C;

void func_801E7704(Pool30Entry *pt, Pool6CEntry *e, SVECTOR *pos, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 t,
                   s32 n);

#endif
