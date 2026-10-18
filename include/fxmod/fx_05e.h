#ifndef FX_05E_H
#define FX_05E_H

/* Effect module fx_05e (dir 13 file 0x5E). */
#include "btlfx_mod.h"

/* ribbon history: two edge points per step, unk0.pad = live */
typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx05eSeg; /* 0x10 */

#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

typedef struct {
    s32 unk0;
    s32 unk4; /* frame */
    SVECTOR unk8; /* summoner position */
    SVECTOR unk10; /* summoner rotation */
    SVECTOR unk18; /* summon position */
    s32 unk20; /* ribbon head */
    s32 unk24;
    s32 unk28; /* slice model */
    SVECTOR unk2C; /* summon rotation */
    s32 unk34[3]; /* summon scale */
} Fx05eWork; /* 0x40 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx05eSummon;

extern s32 D_801E8860[2];
extern SVECTOR D_801E8868;
extern SVECTOR D_801E886C;
extern SVECTOR D_801E8870;
extern SVECTOR D_801E8874;
extern SVECTOR D_801E887C;
extern SVECTOR D_801E8884;
extern u8 D_801E88D4[];
extern Unk80100DC054 D_801E8E84;
extern BtlModel D_801E9014[];
extern u8 D_801E90E4[];
extern Fx05eSeg D_801E9744[];
extern Fx05eSeg D_801E9854[];
extern Unk800F78A4 *D_801E8E74;

void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx05eSeg *buf, s32 abr);
void func_801E7B74(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

#endif
