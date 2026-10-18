#ifndef FX_098_H
#define FX_098_H

/* Effect module fx_098 (dir 13 file 0x98). */
#include "btlfx_mod.h"

extern s16 D_801E9CB8[];
extern Unk800F78A4 *D_801E9D1C;

void func_801E8284(s16 *a, s16 *b, s32 i, s16 *out);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx098Vec3;

/* one shard, a pool item */
typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* flying */
    s32 unk4; /* index */
    s32 unk8; /* frame */
    Fx098Vec3 unkC; /* position << 4 */
    SVECTOR unk18; /* velocity */
    SVECTOR unk20; /* rotation */
    s16 unk28; /* rotation speed */
    s16 unk2A;
    s32 unk2C; /* models */
    s32 unk30;
    Pool6CEntry *unk34;
} Fx098Item; /* 0x38 */

typedef struct {
    u8 pad0[0x20];
    s32 unk20[22]; /* models */
    s16 unk78;
} Fx098Work;

extern SVECTOR D_801E9C50[];
s32 func_801E82F4(s32 mode, Fx098Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx098Work *work);
s32 func_801E82DC(s32 mode, Fx098Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx098Work *work);

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    s32 unk8; /* models */
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20[14]; /* piece models */
    SVECTOR unk58; /* position */
    SVECTOR unk60;
    SVECTOR unk68;
    SVECTOR unk70;
    s16 unk78;
    s16 unk7A; /* spawned directions */
    s32 unk7C;
    s32 unk80;
    Pool6CEntry *unk84; /* particle */
} Fx098Work80; /* 0x88 */

extern BtlModel D_801E9D2C[];
extern s32 D_801E9C80[14];
void func_801E7CBC(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 t, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9);
s32 fx_098_op80(s32 mode, Fx098Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

#endif
