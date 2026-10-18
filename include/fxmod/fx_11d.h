#ifndef FX_11D_H
#define FX_11D_H

/* Effect module fx_11d (dir 13 file 0x11D). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    Pool6CEntry *unk18;
    SVECTOR unk1C; /* caster position */
    SVECTOR unk24;
    SVECTOR unk2C;
} Fx11dWork;

typedef struct {
    u16 unk0; /* bone from */
    u16 unk2; /* bone to */
    u16 unk4; /* blend */
} Fx11dPathKey;

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    s32 (*unk8)(VECTOR *, s32, SVECTOR *, s32, s32, s32);
} Fx11dEffHdr;

extern Fx11dPathKey D_801E7D24[4];
extern Fx11dPathKey D_801E7D3C[4];
extern Unk800F78A4 *D_801E7D9C;
extern s32 D_801E7DAC[];
extern s32 D_801E7F6C[];
extern s32 D_801E812C[];
extern SVECTOR D_801E914C;
extern SVECTOR D_801E9154;

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7754(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E77A4(VECTOR *out, SVECTOR *pos, Fx11dPathKey *key, s32 frame, s32 n);

#endif
