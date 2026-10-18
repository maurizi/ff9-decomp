#ifndef FX_1EA_H
#define FX_1EA_H

/* Effect module fx_1ea (dir 13 file 0x1ea). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1eaPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1eaPoint *unk4;
} Fx1eaPath;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* caster rotation */
} Fx1eaWork80; /* 0x18 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[1];
} Fx1eaTrail;

typedef struct {
    s16 unk0; /* fade-in countdown */
    s16 unk2; /* fade-out countdown */
} Fx1eaWork82;

extern SVECTOR D_801E9B58; /* origin */

void func_801E8B9C(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);
s32 func_801E8C4C(Fx1eaPath *path, s32 t, SVECTOR *out);

/* copied */
extern Unk800F78A4 *D_801E8DD8;
/* end copied */

/* propagated */

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx1eaKey; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx1eaKey *unk4;
} Fx1eaPath_0b4;
s32 func_801E8CC8(Fx1eaPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);

#endif
