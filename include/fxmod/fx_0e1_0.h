#ifndef FX_0E1_0_H
#define FX_0E1_0_H

/* Effect module fx_0e1_0 (dir 13 file 0x0E1). */
#include "btlfx_mod.h"

/* the effect environment block as these callbacks read it */
typedef struct {
    u8 pad0[0x60];
    s32 unk60; /* summon model */
} Fx0e10EnvB;

typedef struct {
    SVECTOR unk0; /* pad: point valid */
    SVECTOR unk8;
} Fx0e10Seg; /* 0x10: one step of the ribbon history */

extern s32 D_801E9D84[];
extern s32 D_801E9DB8;
extern s32 D_801E9DBC;
extern Unk800F78A4 *D_801E9E38;

void func_801E7CB4(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0e10Seg *buf, s32 abr, s32 phase);
void func_801E80A8(s32 ot, s32 x, s32 y, s32 nx, s32 ny, s32 dx, s32 dy, s32 abr, s32 sy, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 w, s32 h, s32 c, s32 amp, s32 ph, s32 dphx, s32 dphy);
s32 func_801E8844(SVECTOR *out);
void func_801E88F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 *v);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0e10Vec3;

typedef struct {
    u32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* summon position */
    Fx0e10Vec3 unk10; /* summon model position */
    Fx0e10Vec3 unk1C; /* summon model scale */
    SVECTOR unk28; /* summon model rotation */
    s32 unk30;
    SVECTOR unk34; /* bone position */
    SVECTOR unk3C;
    Pool6CEntry *unk44;
    Pool6CEntry *unk48;
    s32 unk4C; /* ribbons need a reset */
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    SVECTOR unk64;
    Fx0e10Vec3 unk6C;
    Fx0e10Vec3 unk78;
    Fx0e10Vec3 unk84;
    Fx0e10Vec3 unk90;
    SVECTOR unk9C;
} Fx0e10Work80; /* 0xA4 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0e10Summon;

typedef struct {
    s32 unk0;
    s32 unk4;
} Fx0e10BonePair;

extern SVECTOR D_801E9D7C;
extern Fx0e10BonePair D_801E9D98[4];
extern u8 D_801E9DC0[];
extern Fx0e10Seg D_801E9E48[4][21];
extern BtlModel D_801EA388[];
extern u8 D_801EA778[];
extern u8 D_801EAE08[];

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
void func_801E7718(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

#endif
