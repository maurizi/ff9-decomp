#ifndef FX_196_H
#define FX_196_H

/* Effect module fx_196 (dir 13 file 0x196). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    s16 unk4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx196Track;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
} Fx196CamPath;

typedef struct {
    SVECTOR unk0[8]; /* unit positions */
    s32 unk40; /* unit count */
} Fx196Work86; /* 0x44 */

extern Fx196CamPath D_801E910C[];
extern Unk800F78A4 *D_801E915C;

void func_801E7704(Fx196Track *track, s32 t, SVECTOR *out);
void func_801E7AA0(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 col, s32 col2, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 amp, s32 ph, s32 step, s32 r, s32 ang, s32 d);

/* copied */
extern Unk800F78A4 *D_801E915C;
/* end copied */

/* propagated */
s32 func_801E7A5C(s32 a);

extern s32 D_801E90F8;
extern SVECTOR D_801E916C[];
s32 func_801E8370(SVECTOR *v);

#endif
