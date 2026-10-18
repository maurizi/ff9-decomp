#ifndef FX_114_H
#define FX_114_H

/* Effect module fx_114 (dir 13 file 0x114). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* model rotation */
    VECTOR unk10; /* model position */
    VECTOR unk20; /* model scale */
    s32 unk30; /* model brightness */
    SVECTOR unk34; /* summon bone position */
    SVECTOR unk3C;
    s16 unk44; /* fade-in countdown */
    u16 unk46; /* model */
} Fx114Work81; /* 0x48 */

/* View of Unk800F78A44 up to the word fx_114 reads (ovl_0b.h has it as padding). */
typedef struct {
    u8 pad0[0x60];
    s32 unk60; /* summon model */
} Fx114Summon;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* model rotation */
    VECTOR unkC; /* model position */
    VECTOR unk1C; /* model scale */
    SVECTOR unk2C;
    VECTOR unk34;
    VECTOR unk44;
    u8 pad54[0x58 - 0x54];
    SVECTOR unk58;
    s16 unk60; /* fade-in countdown */
    s16 unk62; /* fade-out countdown */
    u16 unk64; /* frame */
    u16 unk66; /* model */
    u16 unk68; /* model */
} Fx114Work82; /* 0x6C */


typedef struct {
    s16 unk0, unk2, unk4; /* position */
    s16 unk6, unk8, unkA; /* rotation */
} Fx114Key12;

typedef struct {
    u16 unk0; /* count */
    Fx114Key12 *unk4;
} Fx114Track12;
void func_801EA65C(Fx114Track12 *track, u16 *data);

typedef struct {
    s16 x, y, z;
} Fx114Key6;

typedef struct {
    u16 unk0; /* count */
    Fx114Key6 *unk4;
} Fx114Track6;
s32 func_801EA670(Fx114Track6 *track, s32 t, SVECTOR *out);

typedef struct {
    s16 x, y, z;
} Fx114Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx114Point *unk4;
} Fx114Path;
s32 func_801EA6F4(Fx114Path *path, s32 t, SVECTOR *out);
void func_801EA758(Fx114Track12 *track, u16 *data);
s32 func_801EA76C(Fx114Track12 *track, s32 t, SVECTOR *rot, SVECTOR *pos);
void func_801EA880(Fx114Path *path, u16 *data);
s32 func_801EA898(Fx114Path *path, s32 t, VECTOR *out);
void func_801EB154(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

typedef struct {
    Fx114Path unk0;
    s16 unk8;
    s16 unkA;
    s16 unkC; /* brightness */
    s16 unkE;
    s16 unk10;
    s16 unk12;
} Fx114Wave;

extern Unk800F78A4 *D_801EB768;
extern Fx114Wave D_801ED078;
extern Fx114Track6 D_801ED0E8; /* scale keys */
extern u32 D_801ED1C8[];
extern u8 D_801EE658[13][0x50];

s32 fx_114_op81(s32 mode, Fx114Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_114_op82(s32 mode, Fx114Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
void func_801EA6DC(Fx114Path *path, u16 *data);
void func_801EA808(Fx114Track6 *track, u16 *data);
s32 func_801EA81C(Fx114Track6 *track, s32 t, VECTOR *out);
void func_801EA914(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);

void func_801EA5C0(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801EA604(s32 from, s32 to, s32 t, s32 n);

/* propagated */
extern SVECTOR D_801EF268[]; /* particle positions */
s32 func_801E777C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E77B0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E77E4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7818(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E784C(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n);

void func_801E78AC(Fx114Path *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip);
s32 func_801EA8FC(s32 base, s32 i);

typedef struct {
    s16 *unk0;
    s16 unk4; /* start frame */
    s16 unk6; /* frames left, -1 = free */
    s16 unk8; /* channel */
    u16 unkA; /* destination slots, 4 bits each */
    u16 unkC;
    u16 unkE;
} Fx114Track; /* 0x10 */
extern Fx114Track D_801EF2B8[];
extern s32 D_801EF6B8[];
void func_801EA9AC(void);
void func_801EA9F8(s16 start, u16 dst, s16 ch, s16 *data);
void func_801EAA54(s32 ch);


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
/* gte_rtps_b with a memory clobber: fx_114's copy of the fx_007 colour grid reloads c3 after it */
#define gte_rtps_bm() __asm__ volatile(".word\t0x4A180001" : : : "memory")

void func_801EABB4(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx114Vec3;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* rotation */
    SVECTOR unk14; /* position */
    s32 unk1C;
    SVECTOR unk20; /* body rotation */
    Fx114Vec3 unk28; /* body position */
    s32 pad34;
    Fx114Vec3 unk38; /* body scale */
    s32 pad44;
    SVECTOR unk48; /* rock rotation */
    Fx114Vec3 unk50; /* rock position */
    s32 pad5C;
    Fx114Vec3 unk60; /* rock scale */
    s32 pad6C;
    SVECTOR unk70;
    Fx114Vec3 unk78;
    s32 pad84;
    Fx114Vec3 unk88;
    s32 unk94;
    s32 unk98;
    SVECTOR unk9C;
    Fx114Vec3 unkA4;
    s32 padB0;
    Fx114Vec3 unkB4;
    s32 padC0;
    s32 unkC4;
    s32 unkC8;
    SVECTOR unkCC;
    Fx114Vec3 unkD4;
    s32 padE0;
    Fx114Vec3 unkE4;
    s32 padF0;
    SVECTOR unkF4;
    SVECTOR unkFC;
    SVECTOR unk104;
    s16 unk10C; /* frame */
    s16 unk10E;
    s16 unk110;
    s16 unk112;
    s16 unk114;
    s16 unk116;
    u16 unk118[24]; /* models */
    u16 unk148;
    u16 unk14A;
    u16 unk14C;
} Fx114Work83; /* 0x150 */

typedef struct {
    u8 unk0; /* start frame */
    u8 unk1; /* spin flags */
    u8 unk2; /* landed */
    u8 pad3;
    s16 unk4; /* speed */
    s16 unk6; /* lift */
    SVECTOR unk8; /* direction */
    SVECTOR unk10; /* rotation */
    Fx114Vec3 unk18; /* position */
    s32 pad24;
} Fx114Rock; /* 0x28 */

extern Fx114Rock D_801EB2F4[];
extern u8 D_801EB68C[];
extern u8 D_801EB6D4[];
extern s16 D_801EB6E0[];
extern u8 D_801EB6EC[];
extern u8 D_801EB6F4[];
extern s16 D_801EB6FC[];
extern s16 D_801EB704[];
extern u8 D_801EB70C[];
extern s16 D_801EB714[];
extern Fx114Track12 D_801ED0F8[];
extern Fx114Track12 D_801ED1B0;
extern Fx114Track6 D_801ED1B8;
extern Fx114Track6 D_801ED1C0;

/* Hi_RegisterTexListModel called with three arguments (the original leaves a3 unset) */
#define REG_TEX_LIST(ex) ((u16 (*)(void *, void *, void *))(ex)->Hi_RegisterTexListModel)

#endif
