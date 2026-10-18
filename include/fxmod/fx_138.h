#ifndef FX_138_H
#define FX_138_H

/* Effect module fx_138 (dir 13 file 0x138). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* key count */
    SVECTOR *unk4; /* keys */
} Fx138Path;

extern Unk800F78A4 *D_801E8500;
extern Fx138Path D_801E8830[6];
extern SVECTOR D_801E8E60; /* path origin */
extern SVECTOR D_801E8E68; /* path rotation */

s32 func_801E7704(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E7728(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E774C(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E7770(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E7794(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E77B8(SVECTOR *out, s32 arg1, s32 i);
void func_801E77DC(SVECTOR *out, Fx138Path *path, s32 i);
s32 func_801E81F0(Fx138Path *path, s32 i, SVECTOR *out);

#endif
