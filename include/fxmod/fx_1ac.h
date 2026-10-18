#ifndef FX_1AC_H
#define FX_1AC_H

/* Effect module fx_1ac (dir 13 file 0x1ac). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0[8]; /* countdowns */
    SVECTOR unk20; /* caster position */
    SVECTOR unk28; /* target position */
    SVECTOR unk30; /* bone position */
    SVECTOR unk38; /* target rotation */
    u8 pad40[0x44 - 0x40];
    SVECTOR unk44; /* saved target rotation */
    Pool6CEntry *unk4C[5];
} Fx1acWork; /* 0x60 */

extern Unk800F78A4 *D_801E8300;
extern Pool6CEntry *D_801E8304;
extern s32 D_801E8310[];
extern SVECTOR D_801E8580;
extern SVECTOR D_801E8588;
extern SVECTOR D_801E8590;
extern SVECTOR D_801E8598;
extern SVECTOR D_801E85A0;
extern SVECTOR D_801E85A8;
extern SVECTOR D_801E85B0;

s32 func_801E81C4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);

#endif
