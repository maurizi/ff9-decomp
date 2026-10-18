/* Draft of fx_0d2 fx_0d2_op81. Status: 61 aligned mismatches, logic complete; switch case bodies are in the
 * original's order (0, 0x43, 0x41, 0x2C, 0x19). Open: the original copies frame into fp for the slice loop and keeps
 * the clamped point count in a1; otherwise register allocation. */
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

s32 fx_0d2_op81(s32 mode, Fx0d2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;
    s32 i;
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
        for (i = 0; i < 6; i++) {
            func_801E9EEC((Fx0d2Path *)&D_801EF270[i],
                          (u16 *)func_801E9DBC((u8 *)ex->btlfx_getFileData(entry->unk8, 0x90), i));
            work->unk44[i] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801EA440[i]), 1);
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
            for (i = 0; i < env->unkC->unk24; i++) {
                ex->btlfx_getUnitPos(i, (s16 *)&D_801EFF80[i]);
                ex->btlfx_getUnitScale(i, &D_801EFFC0[i]);
            }
            break;
        case 0x19:
            for (i = 0; i < 6; i++) {
                ex->Hi_FreeEffModel(work->unk44[i]);
            }
            break;
        }
        if ((u32)frame < 0x19) {
            for (i = 0; i < 6; i++) {
                func_801E9F04((Fx0d2KeyPath *)&D_801EF270[i], frame, &work->unkC, &v);
                work->unk14.vx = v.vx;
                work->unk14.vy = v.vy;
                work->unk14.vz = v.vz;
                ex->Hi_SetEffModelSlice(work->unk44[i], work->unk14.vy);
                ex->Hi_DrawSliceEffModel(&work->unkC, (VECTOR *)&work->unk14, (s32)&work->unk24, work->unk44[i]);
            }
        }
        if ((u32)(frame - 0x40) < 0x15) {
            D_801EF1F0.unkA = (frame - 0x40) * 4;
            if (D_801EF1F0.unkA > 0x10) {
                D_801EF1F0.unkA = 0x10;
            }
            for (i = 0, buf = D_801EF700[0]; i < 9; i++, buf += 0x90) {
                func_801E9E70((Fx0d2Path *)&D_801EF1F0,
                              (u16 *)func_801E9DBC((u8 *)ex->btlfx_getFileData(entry->unk8, 0x80), i));
                n = D_801EF1F0.keys.unk2;
                if (n >= D_801EF1F0.unkA) {
                    n = D_801EF1F0.unkA;
                }
                func_801E77A4(&D_801EF1F0.keys, buf, 0x90, D_801EF1F0.unk8, n, D_801EF1F0.unk10, D_801EF1F0.unk12, 1);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), 0, NULL, 0, D_801EF1F0.unkE,
                                    D_801EF1F0.unkC, 1, buf, 0);
            }
        }
        if ((u32)(frame - 0x43) < 0xF) {
            D_801EF210.unkA = (frame - 0x43) * 2;
            if (D_801EF210.unkA > 9) {
                D_801EF210.unkA = 9;
            }
            for (i = 0, buf = D_801EFC10[0]; i < 11; i++, buf += 0x50) {
                func_801E9E70((Fx0d2Path *)&D_801EF210,
                              (u16 *)func_801E9DBC((u8 *)ex->btlfx_getFileData(entry->unk8, 0x81), i));
                func_801E77A4(&D_801EF210.keys, buf, 0x50, D_801EF210.unk8, D_801EF210.unkA, D_801EF210.unk10,
                              D_801EF210.unk12, 0);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x25), 0, NULL, 0, D_801EF210.unkE,
                                    D_801EF210.unkC, 1, buf, 0);
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 5), frame);
        return frame >= 0x64;
    }
    return 0;
}
