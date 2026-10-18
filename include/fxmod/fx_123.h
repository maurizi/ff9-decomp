#ifndef FX_123_H
#define FX_123_H

/* Effect module fx_123 (dir 13 file 0x123). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0[5];
    SVECTOR unk14; /* centre of the units */
} Fx123Work82;

typedef struct {
    Pool6CEntry *unk0[6];
    Pool6CEntry *unk18[6];
    Pool6CEntry *unk30[6];
    Pool6CEntry *unk48;
    SVECTOR unk4C; /* caster position */
    SVECTOR unk54;
    SVECTOR unk5C[6]; /* bone positions */
} Fx123Work80; /* 0x8C */

typedef struct {
    u16 unk0; /* bone */
    u16 unk2; /* offset */
} Fx123Bone;

extern Fx123Bone D_801E8F64[6];
extern Unk800F78A4 *D_801E90CC;
extern SVECTOR D_801E9BFC[6];
extern SVECTOR D_801E9C2C[6];

s32 fx_123_op80(s32 mode, Fx123Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_123_op82(s32 mode, Fx123Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
