#ifndef FX_1CC_H
#define FX_1CC_H

/* Effect module fx_1cc (dir 13 file 0x1cc). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801E9C34;
extern s16 D_801E9BD0[];

typedef struct Fx1ccObj {
    s16 unk0; /* state / piece index */
    s16 unk2; /* 0: main object, 1: piece */
    s32 unk4; /* direction index */
    s32 unk8; /* frame */
    s32 unkC; /* position, 28.4 (low halves used by the main object) */
    s32 unk10;
    s32 unk14;
    s16 unk18; /* velocity (main: SVECTOR position) */
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    SVECTOR unk20; /* rotation */
    s16 unk28; /* rotation speed */
    s16 unk2A;
    s32 unk2C; /* model */
    s32 unk30; /* model */
    Pool6CEntry *unk34; /* particle */
} Fx1ccObj;

typedef struct {
    u8 pad0[0x20];
    s32 unk20[14]; /* models */
    u8 pad58[0x78 - 0x58];
    s16 unk78; /* burst */
} Fx1ccInst;

extern SVECTOR D_801E9B68[];
void func_801E8284(s16 *from, s16 *to, s32 i, s16 *out);

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
} Fx1ccWork80; /* 0x84 */

extern BtlModel D_801E9C44[];
extern s32 D_801E9B98[14];
s32 func_801E82D0(s32 mode, Fx1ccObj *o, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 arg5,
                  Fx1ccInst *inst);

s32 fx_1cc_op80(s32 mode, Fx1ccWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

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
void func_801E7720(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);
void func_801E7CBC(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 h, s32 abr);

#endif
