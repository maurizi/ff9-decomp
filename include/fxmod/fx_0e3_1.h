#ifndef FX_0E3_1_H
#define FX_0E3_1_H

/* Effect module fx_0e3_1 (dir 13 file 0x0E3). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[57];
} Fx0e31Bolt;

extern Unk800F78A4 *D_801F0A04;

void func_801EC71C(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9);
void func_801EC910(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang, s32 dang,
                   s32 arg7, s32 arg8);
void func_801ECAF8(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 col);
void func_801ECCF4(Unk80100C78Entry *entry, Fx0e31Bolt *src, Fx0e31Bolt *dst, s32 arg3);
void func_801ECDEC(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 col);
void func_801ED708(VECTOR *pos, SVECTOR *rot);

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx0e31Rgbc; /* primitive colour word */

#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
void func_801ECFE4(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0e31Vec3;

typedef struct {
    u32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* centre */
    Fx0e31Vec3 unk10; /* summon position */
    Fx0e31Vec3 unk1C; /* summon scale */
    SVECTOR unk28; /* summon rotation */
    SVECTOR unk30; /* hand position */
    SVECTOR unk38;
    SVECTOR unk40; /* camera target */
    SVECTOR unk48;
    u8 pad50[8];
    SVECTOR unk58; /* hand rotation */
    Pool6CEntry *unk60;
    Pool6CEntry *unk64;
    Pool6CEntry *unk68;
    s32 unk6C;
    Pool6CEntry *unk70;
    Pool6CEntry *unk74;
    s16 unk78;
    s16 unk7A;
    s32 unk7C; /* model */
    SVECTOR unk80;
    Fx0e31Vec3 unk88;
    Fx0e31Vec3 unk94;
    s32 unkA0; /* model */
    Fx0e31Vec3 unkA4;
    Fx0e31Vec3 unkB0;
    SVECTOR unkBC;
    s32 unkC4; /* models */
    s32 unkC8;
    s32 unkCC;
    s32 unkD0;
    s32 unkD4;
    s32 unkD8;
    s32 unkDC;
    s32 unkE0;
} Fx0e31Work80; /* 0xE4 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[13];
} Fx0e31Bolt13; /* 0x6C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[5];
} Fx0e31Bolt5; /* 0x2C */

typedef struct {
    u8 pad0[0x198];
    BtlUnit9CC *unk198;
    BtlUnit9CC *unk19C;
} Fx0e31SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx0e31SummonData *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0e31Summon;

#define SUMMON(env) ((Fx0e31Summon *)(env)->loader)

extern SVECTOR D_801F09B4;
extern Fx0e31Bolt13 D_801F0A14[];
extern Fx0e31Bolt D_801F0D14[3];

#endif
