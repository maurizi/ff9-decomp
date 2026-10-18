#ifndef FX_0D3_H
#define FX_0D3_H

/* Effect module fx_0d3 (dir 13 file 0xD3). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801ED534;
extern s32 D_801EB084[5];
extern s32 D_801EB098;
extern u16 D_801EB0C4[3]; /* file ids for op87 tracks, end frame */
extern s32 D_801EDF44;
extern s32 D_801EDF48;

void func_801E8A10(s32 a0, s32 frame, s32 a2, s32 a3, s16 *v);
s32 func_801E9050(SVECTOR *v);
void func_801E90D4(s16 *v);
typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    s16 unk4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx0d3Track;
void func_801E8470(Fx0d3Track *track, s32 t, SVECTOR *out);
void func_801E84C0(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b);
void func_801E8548(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c);
s32 func_801E85FC(Fx0d3Track *track, s32 t);
/* ribbon history: two edge points per step, unk0.pad = live */
typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx0d3Seg; /* 0x10 */

#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

void func_801E8628(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0d3Seg *buf, s32 abr, s32 k);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0d3Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8; /* summon position */
    Fx0d3Vec3 unk10;
    Fx0d3Vec3 unk1C; /* summon scale */
    SVECTOR unk28; /* summon rotation */
    Pool6CEntry *unk30;
    Pool6CEntry *unk34;
    u8 pad38[0x60 - 0x38];
    s32 unk60; /* models */
    s32 unk64;
    s32 unk68; /* tint */
} Fx0d3Work80; /* 0x6C */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0d3Summon;

extern SVECTOR D_801EB048;
extern s32 D_801EB050;
extern u8 D_801EB114[];
extern BtlModel D_801ED544[];
void func_801E7CBC(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));
s32 fx_0d3_op80(s32 mode, Fx0d3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8;
    Fx0d3Vec3 unk10; /* summon position */
    Fx0d3Vec3 unk1C; /* summon scale */
    SVECTOR unk28; /* summon rotation */
    SVECTOR unk30;
    SVECTOR unk38;
    SVECTOR unk40; /* camera */
    u8 pad48[4];
    Pool6CEntry *unk4C;
    Pool6CEntry *unk50;
    s32 unk54; /* models */
    s32 unk58;
    s32 unk5C;
    s32 unk60;
} Fx0d3Work81; /* 0x64 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180[4];
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0d3Summon81;

typedef struct {
    s32 unk0;
    s32 unk4;
} Fx0d3BonePair;

extern SVECTOR D_801EB040;
extern Fx0d3BonePair D_801EB054[6];
extern CVECTOR D_801EB09C[];
extern CVECTOR D_801EB0AC[];
extern CVECTOR D_801EB0B8[];
extern u8 D_801EC614[];
extern u8 D_801ECE34[];
extern Fx0d3Seg D_801EDAC4[6][12];
extern u16 D_801EDF54[];
void func_801E7720(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);
s32 fx_0d3_op81(s32 mode, Fx0d3Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
