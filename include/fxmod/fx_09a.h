#ifndef FX_09A_H
#define FX_09A_H

/* Effect module fx_09a (dir 13 file 0x9A). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx09aSeg; /* 0x10 */
void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx09aSeg *buf, s32 abr);
extern Unk800F78A4 *D_801E8858;
void func_801E7B74(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    s32 unk8; /* unit */
    s32 unkC; /* units done */
    s32 unk10; /* unit count */
    SVECTOR unk14; /* unit position */
    SVECTOR unk1C; /* unit rotation */
    SVECTOR unk24; /* summon position */
    s32 unk2C; /* ribbon head */
    s32 unk30;
    SVECTOR unk34; /* summon rotation */
    s32 unk3C[3]; /* summon scale */
} Fx09aWork; /* 0x48 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx09aSummon;

extern s32 D_801E87E4[2];
extern SVECTOR D_801E87EC;
extern SVECTOR D_801E87F0;
extern SVECTOR D_801E87F4;
extern SVECTOR D_801E87F8;
extern SVECTOR D_801E8800;
extern SVECTOR D_801E8808;
extern Unk80100DC054 D_801E8868;
extern u8 D_801E89F8[];
extern Fx09aSeg D_801E9058[];
extern Fx09aSeg D_801E9168[];

#endif
