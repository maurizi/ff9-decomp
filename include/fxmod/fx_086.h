#ifndef FX_086_H
#define FX_086_H

/* Effect module fx_086 (dir 13 file 0x86). */
#include "btlfx_mod.h"

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx086Rgbc; /* primitive colour word */

extern Unk800F78A4 *D_801E9D6C;

void func_801E7E18(s32 n, s32 r, SVECTOR *pts);
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
void func_801E7F6C(u32 *ot, s32 n, s32 h, s32 dz, void *src, SVECTOR *pts, CVECTOR *col, s32 p, s32 abr);

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* counter */
    s16 unk8[6];
    SVECTOR unk14; /* position */
    SVECTOR unk1C; /* target position */
    s32 unk24; /* models */
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    Pool6CEntry *unk44;
} Fx086Work80; /* 0x48 */

extern BtlModel D_801E9D7C[];
extern SVECTOR D_801EA3BC[];
extern SVECTOR D_801EB5BC[64];
void func_801E7704(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);

#endif
