#ifndef FX_0E1_1_H
#define FX_0E1_1_H

/* Effect module fx_0e1_1 (dir 13 file 0x0E1). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8; /* effect origin */
    s32 unk10;
    SVECTOR unk14;
    SVECTOR unk1C;
    SVECTOR unk24; /* camera position */
    Pool6CEntry *unk2C;
    Pool6CEntry *unk30;
    s32 unk34; /* sway amplitude */
    s32 unk38; /* fading out */
    s32 unk3C; /* particle alpha */
} Fx0e11Work; /* 0x40 */

/* View of Unk800F78A40 up to the byte fx_0e1_1 reads (ovl_0b.h has it as padding). */
typedef struct {
    u8 pad0[0x22];
    u8 unk22;
} Fx0e11BattleState;

extern SVECTOR D_801ECD70;
extern SVECTOR D_801ECD78;
extern CVECTOR D_801ECD80;
extern Unk800F78A4 *D_801ECDD0;

s32 fx_0e1_1_op80(s32 mode, Fx0e11Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

/* propagated */
s32 fx_0e1_1_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
