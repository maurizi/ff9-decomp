#ifndef FX_0D6_H
#define FX_0D6_H

/* Effect module fx_0d6 (dir 13 file 0xD6). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* func_800E7D3C(0x10, 1) */
} Fx0d6Work84;

extern Unk800F78A4 *D_801E9150;

typedef struct {
    SVECTOR unk0; /* caster body position */
    SVECTOR unk8; /* caster position */
    s32 unk10; /* state */
    s32 unk14[4]; /* models */
    s32 unk24; /* model */
    s32 unk28; /* model */
    Pool6CEntry *unk2C;
    Pool6CEntry *unk30;
    s32 unk34; /* frame counter */
} Fx0d6Work80; /* 0x38 */

extern BtlModel D_801E9160[];
extern s32 D_801E9610[];
extern u16 D_801E98A0[];
s32 fx_0d6_op80(s32 mode, Fx0d6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

/* propagated */

/* PsyQ gte_stszotz: SZ3 / 4 */
#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
/* VZ0 = VZ1 = VZ2 = 0 */
#define gte_ldVZ012_0() __asm__ volatile( \
    "mtc2\t$0, $1;" \
    "mtc2\t$0, $3;" \
    "mtc2\t$0, $5")

/* V0..V2 x and y only (VZ0..VZ2 are loaded once with gte_ldVZ012_0) */
#define gte_ldv3xy(r0, r1, r2) __asm__ volatile( \
    "lwc2\t$0, 0(%0);" \
    "lwc2\t$2, 0(%1);" \
    "lwc2\t$4, 0(%2)" \
    : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldv0xy(r0) __asm__ volatile( \
    "lwc2\t$0, 0(%0)" \
    : : "r"(r0))
void func_801E7704(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

#endif
