#ifndef FX_191_H
#define FX_191_H

/* Effect module fx_191 (dir 13 file 0x191). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* point count */
    SVECTOR *unk4; /* points (6 bytes each) */
} Fx191Path;

typedef struct {
    Pool6CEntry *unk0[4];
    SVECTOR unk10; /* caster position */
    SVECTOR unk18; /* target position */
    SVECTOR unk20; /* angles */
} Fx191Work80; /* 0x28 */

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    SVECTOR unk18; /* target body position */
    u8 pad20[0x30 - 0x20];
    SVECTOR unk30; /* from, to, from, to */
    SVECTOR unk38;
    SVECTOR unk40;
    SVECTOR unk48;
    SVECTOR unk50; /* current */
    SVECTOR unk58;
} Fx191Work81; /* 0x60 */

extern u16 D_801E84DC[4]; /* files */
extern SVECTOR D_801E84E4[4];
extern s32 (*D_801E8504[4])();
extern Unk800F78A4 *D_801E855C;
extern Fx191Path D_801E8564;
extern Fx191Path D_801E856C;
extern SVECTOR D_801E8574;
extern SVECTOR D_801E857C[4];
extern SVECTOR D_801E859C;
extern SVECTOR D_801E85CC;
extern SVECTOR D_801E85D4;
extern s32 D_801E85DC[4][0x60];
extern s32 D_801E8BDC[];
extern s32 D_801E8CBC[];
extern s32 D_801E8D9C; /* op80 frame */
extern s32 D_801E8DA0; /* op81 frame */

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E775C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E77B8(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7814(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7870(Pool30Entry *e, SVECTOR *pos, SVECTOR *base, SVECTOR *to, s32 frame, s32 frames);
s32 func_801E7988(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E79C4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7A00(Pool30Entry *e, SVECTOR *pos, SVECTOR *ofs, s32 frame, s32 frames);
s32 func_801E7B24(Pool30Entry *e, SVECTOR *pos, SVECTOR *ofs, s32 frame, s32 frames);
void func_801E8460(Fx191Path *path, u8 *data);
s32 func_801E8478(Fx191Path *path, s32 t, SVECTOR *out);

#endif
