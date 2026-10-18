#ifndef FX_08F_H
#define FX_08F_H

/* Effect module fx_08f (dir 13 file 0x8F). */
#include "btlfx_mod.h"

#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
#define gte_ReadGeomScreen(r0) __asm__ volatile( \
    "cfc2\t$12, $26;" \
    "nop;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
extern Unk800F78A4 *D_801E8A04;
void func_801E7858(u32 *ot, s32 n, s32 h, s32 dz, void *src, SVECTOR *pts, CVECTOR *col, s32 p, s32 abr);
typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    s16 unk8[6];
    SVECTOR unk14; /* summon position */
    SVECTOR unk1C;
    s32 unk24; /* models */
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    Pool6CEntry *unk44;
} Fx08fWork80; /* 0x48 */

extern BtlModel D_801E8A14[];
extern SVECTOR D_801E9054[];
extern u8 D_801EA254[];
void func_801E7704(s32 n, s32 r, SVECTOR *pts);
s32 fx_08f_op80(s32 mode, Fx08fWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
