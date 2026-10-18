/* Draft of fx_0d2 fx_0d2_op83. Status: 62 aligned mismatches, logic complete; switch bodies in the original's order
 * (3, 12, 38, 22, 42). Open: the model-registration loop indexes D_801EA44C without strength reduction (i is
 * incremented through a temporary), the unit-position sum reads work->unk58[i] through a second pointer, and register
 * allocation. */
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

s32 fx_0d2_op83(s32 mode, Fx0d2Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sum;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    VECTOR e;
    s32 t;
    s32 frame;
    s32 i;
    s32 j;
    s32 k;
    s32 f;
    s32 m0;
    s32 m1;
    u16 model;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d2Work83);
    } else if (mode == 1) {
        i = 0;
        work->unk8.vx = 0;
        work->unk8.vy = 0x800;
        work->unk8.vz = 0;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk20.vx = 0x1000;
        work->unk20.vy = 0x1000;
        work->unk20.vz = 0x1000;
        D_801ED8E0 = env;
        func_801E9EEC((Fx0d2Path *)&D_801EF238, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x28));
        work->unk136[0] = ex->func_800E8F50(&D_801EB138, 1);
        work->unk8.vx = 0;
        work->unk8.vy = 0x800;
        work->unk8.vz = 0;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk20.vx = 0x1000;
        work->unk20.vy = 0x1000;
        work->unk20.vz = 0x1000;
        func_801E9EEC((Fx0d2Path *)&D_801EF248, (u16 *)ex->btlfx_getFileData(entry->unk8, 9));
    loop:
        model = ex->Hi_RegisterTexPtrModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, D_801EA44C[i]),
                                           D_801EA454, D_801EA464);
        i++;
        work->unk136[i] = model;
        if (i < 3) {
            goto loop;
        }
        work->unk30.vx = 0;
        work->unk30.vy = 0;
        work->unk30.vz = 0;
        work->unk38.vx = 0;
        work->unk38.vy = 0;
        work->unk38.vz = 0;
        work->unk48.vx = 0x1000;
        work->unk48.vy = 0x1000;
        work->unk48.vz = 0x1000;
        for (i = 0; i < 5; i++) {
            work->unk136[i + 4] = ex->func_800E8F50((Unk800E8F50 *)func_801E9DBC(D_801EA47C, i), 1);
        }
        for (i = 0; i < 10; i++) {
            func_801E9EEC((Fx0d2Path *)&D_801EF2A0[i],
                          (u16 *)func_801E9DBC((u8 *)ex->btlfx_getFileData(entry->unk8, 8), i));
        }
        func_801E9E70((Fx0d2Path *)&D_801EF2F0, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x14));
        func_801E9FF4((Fx0d2Path *)&D_801EF2F8, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x15));
        work->unk130 = -1;
        work->unk132 = -1;
        work->unk134 = 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 3:
            work->unk118.vx = -0x1C;
            work->unk118.vy = -0x267;
            work->unk118.vz = 0x342;
            work->unk128.vx = 0x1CF;
            work->unk128.vy = -0x7B7;
            work->unk128.vz = 0x27B;
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unk118);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = 0x118;
            }
            break;
        case 12:
            sum.vx = 0;
            sum.vy = 0;
            sum.vz = 0;
            for (i = 0; i < env->unkC->unk24; i++) {
                sum.vx += work->unk58[i].vx;
                sum.vy += (&work->unk58[i])->vy;
                sum.vz += (&work->unk58[i])->vz;
            }
            work->unk120.vx = sum.vx / env->unkC->unk24;
            work->unk120.vy = sum.vy / env->unkC->unk24;
            work->unk120.vz = sum.vz / env->unkC->unk24;
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk120);
            break;
        case 38:
            ex->func_800E3D54(work->unk0);
            break;
        case 22:
            work->unk132 = 0x10;
            break;
        case 42:
            work->unk130 = 0x10;
            work->unk134 = 0;
            for (i = 0; i < env->unkC->unk24; i++) {
                ex->func_800E7ACC(i, (s32 *)&D_801EFFC0[i]);
            }
            env->unk2C->unk0[0].vx = 0;
            env->unk2C->unk0[0].vy = 0;
            env->unk2C->unk0[0].vz = 0x4B0;
            break;
        }
        k = frame - 1;
        if ((u32)k < 0x17) {
            func_801E9F04((Fx0d2KeyPath *)&D_801EF238, frame, &work->unk8, &a);
            work->unk10.vx = a.vx;
            work->unk10.vy = a.vy;
            work->unk8.vz = -work->unk8.vz;
            work->unk10.vz = a.vz;
            ex->Hi_SetEffModelSlice(work->unk136[0], a.vy);
            ex->Hi_DrawSliceEffModel(&work->unk8, (VECTOR *)&work->unk10, (s32)&work->unk20, work->unk136[0]);
        }
        if ((u32)k < 0x27) {
            f = frame;
            switch (frame) {
            case 1:
                func_801E9F98((Fx0d2Path *)&D_801EF260, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x19));
                break;
            case 4:
                func_801E9F98((Fx0d2Path *)&D_801EF260, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x1A));
                break;
            }
            if (f < 4) {
                m0 = 1;
                m1 = 2;
            } else {
                m0 = 2;
                m1 = 3;
            }
            func_801E9F04((Fx0d2KeyPath *)&D_801EF248, f, &work->unk8, &b);
            work->unk10.vx = b.vx;
            work->unk10.vy = b.vy;
            work->unk8.vz = -work->unk8.vz;
            work->unk10.vz = b.vz;
            func_801E9FB0(&D_801EF260, f + 1, &t);
            ex->Hi_DrawMorphEffModel(&work->unk8, (VECTOR *)&work->unk10, (s32)&work->unk20, work->unk136[m0],
                                     work->unk136[m1], t);
        }
        k = frame - 2;
        if ((u32)k < 0x18) {
            j = 4;
            for (i = 0; i < 10; i++) {
                func_801E9F04((Fx0d2KeyPath *)&D_801EF2A0[i], k, &work->unk30, &c);
                work->unk38.vx = c.vx;
                work->unk38.vy = c.vy;
                work->unk38.vz = c.vz;
                ex->Hi_DrawEffModel(&work->unk30, (VECTOR *)&work->unk38, (s32)&work->unk48, work->unk136[j]);
                j++;
                if (j >= 9) {
                    j = 4;
                }
            }
        }
        if ((u32)frame < 0x1C) {
            func_801E9E88((Fx0d2Path *)&D_801EF2F0, frame, &d);
            func_801EA00C((Fx0d2Path *)&D_801EF2F8, frame, &e);
            for (i = 0; i < env->unkC->unk24; i++) {
                work->unk58[i].vx = D_801EFF80[i].vx + d.vx;
                work->unk58[i].vy = D_801EFF80[i].vy + d.vy;
                work->unk58[i].vz = D_801EFF80[i].vz + d.vz;
                ex->btlfx_setUnitPos(i, (s16 *)&work->unk58[i]);
                work->unk98[i].vx = (D_801EFFC0[i].vx * e.vx) / 4096;
                work->unk98[i].vy = (D_801EFFC0[i].vy * e.vy) / 4096;
                work->unk98[i].vz = (D_801EFFC0[i].vz * e.vz) / 4096;
                ex->func_800E7ACC(i, (s32 *)&work->unk98[i]);
            }
        }
        if ((u32)(frame - 3) < 0x23 && work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk128.vx;
                work->unk0->unk5C.vy = work->unk128.vy;
                work->unk0->unk5C.vz = work->unk128.vz;
            }
        }
        if (work->unk132 >= 0) {
            func_801EA070(0, 0xFF, 0x10 - work->unk132, 0x10, 1, 0);
            if (--work->unk132 == -1) {
                work->unk134 = 1;
            }
        }
        if (work->unk130 >= 0) {
            func_801EA070(0xFF, 0, 0x10 - work->unk130, 0x10, 1, 0);
            work->unk130--;
        }
        if (work->unk134 != 0) {
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x3D;
    }
    return 0;
}
