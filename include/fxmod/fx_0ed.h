#ifndef FX_0ED_H
#define FX_0ED_H

/* Effect module fx_0ed (dir 13 file 0x0ED). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* end frame of the current target */
    s32 unk8; /* frame counter */
    BtlUnit *unkC; /* target */
    SVECTOR unk10; /* target position */
} Fx0edWork81; /* 0x18 */

/* View of Unk800F78A40 up to the byte fx_0ed writes (ovl_0b.h has it as padding). */
typedef struct {
    u8 pad0[0x10];
    u8 unk10;
} Fx0edBattleState;

extern Unk800F78A4 *D_801E9224;

s32 fx_0ed_op81(s32 mode, Fx0edWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx0edSeg; /* 0x10 */
void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx0edSeg *buf, s32 abr);
void func_801E7B74(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* unit position */
    SVECTOR unk10; /* unit rotation */
    SVECTOR unk18; /* model position */
    SVECTOR unk20;
    s32 unk28; /* model */
    SVECTOR unk2C; /* model rotation */
    s32 unk34[3]; /* model scale */
} Fx0edWork80; /* 0x40 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
    } *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx0edSummon;

extern s32 D_801E8C10[2];
extern CVECTOR D_801E8C18;
extern CVECTOR D_801E8C1C;
extern CVECTOR D_801E8C20;
extern SVECTOR D_801E8C24;
extern SVECTOR D_801E8C2C;
extern SVECTOR D_801E8C34;
extern u8 D_801E8C84[];
extern Unk80100DC054 D_801E9234;
extern u8 D_801E9494[];
extern Fx0edSeg D_801E9AF4[17];
extern Fx0edSeg D_801E9C04[17];
extern BtlModel D_801E93C4[];

#endif
