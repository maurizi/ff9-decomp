#ifndef FX_026_H
#define FX_026_H

/* Effect module fx_026 (dir 13 file 0x26). */
#include "btlfx_mod.h"

/* keyframe track read from the effect file: count, then the keys */
typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    s16 unk4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx026Track;

extern Unk800F78A4 *D_801EACCC;

void func_801E7704(Fx026Track *track, s32 t, SVECTOR *out);
void func_801E7754(Fx026Track *track, s32 t, SVECTOR *a, SVECTOR *b);
void func_801E77DC(Fx026Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c);
s32 func_801E7890(Fx026Track *track, s32 t);
void func_801E78BC(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

#endif
