/* Draft of fx_0d2 fx_0d2_op81. Status: ~41 aligned diff lines with registers, 3 without. Found: case 0 tests
 * work->unk0 (cross-jumped with case 0x43's unk8 test), both unkA clamps are 'x = (x > K) ? K : x' (copy in a1),
 * the n clamp is a ternary (copy n = unk2), the trail loops index D_801EF700[i] directly, and the loops use
 * separate counters (j: mode 1 + case 0x19 + second trail loop, i: first trail loop, i5: slice loop, i2: 0x2C).
 * Left: case 0x19 counter/pointer registers swapped (s0/s1), n gets v1 instead of a3 (global-alloc priority of n
 * vs the unkA temp, which also costs the bnez delay slot), and li 0x1000 schedules later in mode 1. */
#include "btlfx_mod.h"

typedef struct {
    u16 unk0;
    u16 unk2; /* count */
    s16 *unk4;
} Fx0d2Keys;

typedef struct {
    s16 *unk0;
    s16 unk4; /* start frame */
    s16 unk6; /* frames left, -1 = free */
    s16 unk8; /* channel */
    u16 unkA; /* destination slots, 4 bits each */
    u16 unkC;
    u16 unkE;
} Fx0d2Track; /* 0x10 */

typedef struct {
    Pool6CEntry *unk0[8];
    SVECTOR unk20[8]; /* unit positions */
    SVECTOR unk60[8];
    s16 unkA0[8]; /* done */
} Fx0d2Work84; /* 0xB0 */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0d2Vec3;

typedef struct {
    SVECTOR unk0; /* rotation */
    Fx0d2Vec3 unk8; /* position */
    s32 pad14;
    Fx0d2Vec3 unk18; /* scale */
    s32 pad24;
} Fx0d2Model; /* 0x28 */

typedef struct {
    u16 unk0; /* file id */
    u16 unk2; /* start frame */
    u16 unk4; /* released */
    s16 unk6[3]; /* position */
    u16 unkC;
} Fx0d2Spawn; /* 0xE */

typedef struct {
    Pool6CEntry *unk0[7];
    Fx0d2Model unk1C[4];
    SVECTOR unkBC[7];
    u16 unkF4[10]; /* models: 0 ring, 1-2 morph pair, 3-8 slices, 9 */
} Fx0d2Work82; /* 0x108 */

extern Fx0d2Keys D_801EF240;
extern Fx0d2Keys D_801EF258;
extern Fx0d2Keys D_801EF270[6];
extern u16 D_801EA440[6];
extern SVECTOR D_801EA474;
extern Fx0d2Spawn D_801EA3DC[7];
extern Fx0d2Track D_801F0060[64];
extern s32 D_801F0460[4];

void func_801E77A4(Fx0d2Keys *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip);
s32 func_801E9DBC(u8 *base, u16 i);
void func_801E9DD4(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E9E18(s32 from, s32 to, s32 t, s32 n);
s32 func_801E9FB0(Fx0d2Keys *keys, s32 i, s32 *out);
s32 func_801EA108(s32 a0, s32 t, s32 n, SVECTOR *b);
void func_801EA1D4(void);
void func_801EA220(s16 start, u16 dst, s16 ch, s16 *data);
void func_801EA27C(s32 ch);
typedef struct {
    s16 x, y, z;
} Fx0d2Point;
typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx0d2Point *unk4;
} Fx0d2Path;
typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx0d2Key; /* 0xC */
typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx0d2Key *unk4;
} Fx0d2KeyPath;
typedef struct {
    s16 x, y, z;
} Fx0d2Key6;
typedef struct {
    u16 unk0; /* count */
    Fx0d2Key6 *unk4;
} Fx0d2Track6;
void func_801E9E70(Fx0d2Path *path, u16 *data);
s32 func_801E9E88(Fx0d2Path *path, s32 t, SVECTOR *out);
void func_801E9EEC(Fx0d2Path *path, u16 *data);
s32 func_801E9F04(Fx0d2KeyPath *path, s32 i, SVECTOR *rot, SVECTOR *pos);
void func_801E9F98(Fx0d2Path *path, u16 *data);
void func_801E9FF4(Fx0d2Path *path, u16 *data);
s32 func_801EA00C(Fx0d2Path *track, s32 t, VECTOR *out);
extern Unk800F78A4 *D_801ED8E0;
void func_801EA070(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);
typedef struct {
    Fx0d2Keys keys;
    s16 unk8; /* start */
    s16 unkA; /* points */
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
} Fx0d2Trail;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* slice rotation */
    Fx0d2Vec3 unk14; /* slice position */
    s32 pad20;
    Fx0d2Vec3 unk24; /* slice scale */
    s32 pad30;
    SVECTOR unk34;
    SVECTOR unk3C;
    u16 unk44[6]; /* slice models */
} Fx0d2Work81; /* 0x50 */

extern Fx0d2Trail D_801EF1F0;
extern Fx0d2Trail D_801EF210;
extern u32 D_801EF300[];
extern u8 D_801EF700[9][0x90];
extern u8 D_801EFC10[11][0x50];
extern SVECTOR D_801EFF80[8];
extern VECTOR D_801EFFC0[8];
s32 fx_0d2_op81(s32 mode, Fx0d2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14; /* summon rotation */
    Fx0d2Vec3 unk1C; /* summon position */
    s32 pad28;
    Fx0d2Vec3 unk2C; /* summon scale */
    s32 pad38;
    SVECTOR unk3C; /* slice rotation */
    Fx0d2Vec3 unk44; /* slice position */
    s32 pad50;
    Fx0d2Vec3 unk54; /* slice scale */
    u8 pad60[0x6C - 0x60];
    SVECTOR unk6C;
    SVECTOR unk74;
    SVECTOR unk7C;
    SVECTOR unk84;
    SVECTOR unk8C;
    s32 unk94;
    s32 unk98;
    s32 unk9C; /* path step */
    s32 padA0;
    u16 unkA4; /* slice model */
} Fx0d2Work80; /* 0xA8 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
        BtlUnit9CC *unk184;
        BtlUnit9CC *unk188;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0d2Summon;

extern BtlModel D_801ED8F0[];
extern Fx0d2Keys D_801EF228;
extern Fx0d2Keys D_801EF230;
extern Fx0d2Keys D_801EF250;
extern SVECTOR D_801F0040;
extern SVECTOR D_801F0050[2];
s32 fx_0d2_op80(s32 mode, Fx0d2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* slice rotation */
    Fx0d2Vec3 unk10; /* slice position */
    s32 pad1C;
    Fx0d2Vec3 unk20; /* slice scale */
    s32 pad2C;
    SVECTOR unk30;
    Fx0d2Vec3 unk38;
    s32 pad44;
    Fx0d2Vec3 unk48;
    s32 pad54;
    SVECTOR unk58[8]; /* unit positions */
    VECTOR unk98[8]; /* unit scales */
    SVECTOR unk118;
    SVECTOR unk120;
    SVECTOR unk128;
    s16 unk130; /* fade counters */
    s16 unk132;
    s16 unk134;
    u16 unk136[9]; /* models */
} Fx0d2Work83; /* 0x148 */

extern u16 D_801EA44C[3];
extern s16 D_801EA454[];
extern s16 D_801EA464[];
extern u8 D_801EA47C[];
extern Unk800E8F50 D_801EB138;
extern Fx0d2Keys D_801EF238;
extern Fx0d2Keys D_801EF248;
extern Fx0d2Keys D_801EF260;
extern Fx0d2Keys D_801EF2A0[10];
extern Fx0d2Keys D_801EF2F0;
extern Fx0d2Keys D_801EF2F8;
s32 fx_0d2_op83(s32 mode, Fx0d2Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

s32 fx_0d2_op81(s32 mode, Fx0d2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;
    s32 f;
    s32 i;
    s32 i5;
    s32 j;
    s32 i2;
    s32 n;
    u8 *buf;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d2Work81);
    } else if (mode == 1) {
        D_801ED8E0 = env;
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unkC.vz = 0;
        work->unk14.vx = 0;
        work->unk14.vy = 0;
        work->unk14.vz = 0;
        work->unk24.vx = 0x1C00;
        work->unk24.vy = 0x1C00;
        work->unk24.vz = 0x1C00;
        for (j = 0; j < 6; j++) {
            func_801E9EEC((Fx0d2Path *)&D_801EF270[j],
                          (u16 *)func_801E9DBC((u8 *)ex->btlfx_getFileData(entry->unk8, 0x90), j));
            work->unk44[j] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801EA440[j]), 1);
        }
        D_801EF1F0.unkA = 6;
        D_801EF1F0.unk10 = 0x40;
        D_801EF1F0.unk8 = 0;
        D_801EF1F0.unkC = 0x1000;
        D_801EF1F0.unkE = 0x1000;
        D_801EF1F0.unk12 = 0x20;
        D_801EF210.unk10 = 0x80;
        D_801EF210.unk8 = 0;
        D_801EF210.unkA = 0;
        D_801EF210.unkC = 0x1000;
        D_801EF210.unkE = 0x1000;
        D_801EF210.unk12 = 0x60;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk34.vx = 0;
            work->unk34.vy = 0;
            work->unk34.vz = -0x7D4;
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA6), (s32)&work->unk34);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = 0x100;
            }
            break;
        case 0x43:
            work->unk3C.vx = 0;
            work->unk3C.vy = 0;
            work->unk3C.vz = 0x435;
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk3C);
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk3C);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = 0x100;
            }
            if (work->unk8 != NULL) {
                work->unk8->unk22 = 0x100;
            }
            break;
        case 0x41:
            ex->func_800E6E1C(0x2C0, 0x100, 0x80, 0x80, D_801EF300, 0, 0x200, 0x2BC);
            break;
        case 0x2C:
            for (i2 = 0; i2 < env->unkC->unk24; i2++) {
                ex->btlfx_getUnitPos(i2, (s16 *)&D_801EFF80[i2]);
                ex->btlfx_getUnitScale(i2, &D_801EFFC0[i2]);
            }
            break;
        case 0x19:
            for (j = 0; j < 6; j++) {
                ex->Hi_FreeEffModel(work->unk44[j]);
            }
            break;
        }
        if ((u32)frame < 0x19) {
            f = frame;
            for (i5 = 0; i5 < 6; i5++) {
                func_801E9F04((Fx0d2KeyPath *)&D_801EF270[i5], f, &work->unkC, &v);
                work->unk14.vx = v.vx;
                work->unk14.vy = v.vy;
                work->unk14.vz = v.vz;
                ex->Hi_SetEffModelSlice(work->unk44[i5], work->unk14.vy);
                ex->Hi_DrawSliceEffModel(&work->unkC, (VECTOR *)&work->unk14, (s32)&work->unk24, work->unk44[i5]);
            }
        }
        if ((u32)(frame - 0x40) < 0x15) {
            D_801EF1F0.unkA = (frame - 0x40) * 4;
            D_801EF1F0.unkA = (D_801EF1F0.unkA > 0x10) ? 0x10 : D_801EF1F0.unkA;
            for (i = 0; i < 9; i++) {
                func_801E9E70((Fx0d2Path *)&D_801EF1F0,
                              (u16 *)func_801E9DBC((u8 *)ex->btlfx_getFileData(entry->unk8, 0x80), i));
                n = (D_801EF1F0.keys.unk2 >= D_801EF1F0.unkA) ? D_801EF1F0.unkA : D_801EF1F0.keys.unk2;
                func_801E77A4(&D_801EF1F0.keys, D_801EF700[i], 0x90, D_801EF1F0.unk8, n, D_801EF1F0.unk10, D_801EF1F0.unk12, 1);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), 0, NULL, 0, D_801EF1F0.unkE,
                                    D_801EF1F0.unkC, 1, D_801EF700[i], 0);
            }
        }
        if ((u32)(frame - 0x43) < 0xF) {
            D_801EF210.unkA = (frame - 0x43) * 2;
            D_801EF210.unkA = (D_801EF210.unkA > 9) ? 9 : D_801EF210.unkA;
            for (j = 0; j < 11; j++) {
                func_801E9E70((Fx0d2Path *)&D_801EF210,
                              (u16 *)func_801E9DBC((u8 *)ex->btlfx_getFileData(entry->unk8, 0x81), j));
                func_801E77A4(&D_801EF210.keys, D_801EFC10[j], 0x50, D_801EF210.unk8, D_801EF210.unkA, D_801EF210.unk10,
                              D_801EF210.unk12, 0);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x25), 0, NULL, 0, D_801EF210.unkE,
                                    D_801EF210.unkC, 1, D_801EFC10[j], 0);
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 5), frame);
        return frame >= 0x64;
    }
    return 0;
}
