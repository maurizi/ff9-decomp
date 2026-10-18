#ifndef FX_19E_H
#define FX_19E_H

/* Effect module fx_19e (dir 13 file 0x19e). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
} Fx19eWork;

typedef struct {
    SVECTOR unk0; /* caster body position */
    SVECTOR unk8; /* angles */
    SVECTOR unk10; /* destination */
    SVECTOR unk18; /* target position */
    SVECTOR unk20;
    u8 pad28[0x30 - 0x28];
} Fx19eWork81;

extern Unk800F78A4 *D_801E7EC8;


/* propagated */
s32 func_801E7AAC(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta);
s32 func_801E7C28(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang);
s32 func_801E7990(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start);

#endif
