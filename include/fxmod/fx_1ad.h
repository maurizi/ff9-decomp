#ifndef FX_1AD_H
#define FX_1AD_H

/* Effect module fx_1ad (dir 13 file 0x1ad). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* done */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* bone position */
    SVECTOR unk10; /* target body position */
    u8 pad18[0x20 - 0x18];
    Pool6CEntry *unk20;
    s32 unk24; /* model */
} Fx1adWork;

extern u16 D_801E7CD8[6][9]; /* per row: count, bones */
extern Unk800F78A4 *D_801E7D8C;
extern BtlModel D_801E7D9C[];
extern s32 D_801E7E6C[];
extern s32 D_801E8F4C[];

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);

#endif
