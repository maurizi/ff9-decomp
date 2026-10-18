/* Draft of fx_0d2 fx_0d2_op82. Status: same instructions as the original apart from register allocation: the original
 * gives work $s3, ex $s4 and the D_801EA440 giv $s2 (ours $s2/$s3/$s4). */
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

s32 fx_0d2_op82(s32 mode, Fx0d2Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR w;
    s32 t;
    s32 frame;
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    Fx0d2Spawn *sp;
    SVECTOR *rp;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d2Work82);
    } else if (mode == 1) {
        work->unk1C[0].unk0.vx = 0;
        work->unk1C[0].unk0.vy = 0x800;
        work->unk1C[0].unk0.vz = 0;
        work->unk1C[0].unk8.vx = 0;
        work->unk1C[0].unk8.vy = 0;
        work->unk1C[0].unk8.vz = 0;
        work->unk1C[0].unk18.vx = 0x1C00;
        work->unk1C[0].unk18.vy = 0x1C00;
        work->unk1C[0].unk18.vz = 0x1C00;
        D_801ED8E0 = env;
        func_801E9EEC(&D_801EF240, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x84));
        work->unkF4[0] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8E), 1);
        work->unk1C[1].unk0.vx = 0;
        work->unk1C[1].unk0.vy = 0;
        work->unk1C[1].unk0.vz = 0;
        work->unk1C[1].unk8.vx = 0;
        work->unk1C[1].unk8.vy = 0;
        work->unk1C[1].unk8.vz = 0;
        work->unk1C[1].unk18.vx = 0x1000;
        work->unk1C[1].unk18.vy = 0x1000;
        work->unk1C[1].unk18.vz = 0x1000;
        func_801E9F98(&D_801EF258, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x8F));
        work->unkF4[1] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x86), 1);
        work->unkF4[2] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8D), 1);
        ex->Hi_SetEffModelOffset(work->unkF4[1], 0x32);
        work->unk1C[2].unk0.vx = 0;
        work->unk1C[2].unk0.vy = 0;
        work->unk1C[2].unk0.vz = 0;
        work->unk1C[2].unk8.vx = 0;
        work->unk1C[2].unk8.vy = 0;
        work->unk1C[2].unk8.vz = 0;
        work->unk1C[2].unk18.vx = 0x1C00;
        work->unk1C[2].unk18.vy = 0x1C00;
        work->unk1C[2].unk18.vz = 0x1C00;
        for (k = 0; k < 6; k++) {
            func_801E9EEC(&D_801EF270[k], (u8 *)func_801E9DBC((u8 *)ex->btlfx_getFileData(entry->unk8, 0x91), k));
            work->unkF4[k + 3] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801EA440[k]), 1);
        }
        work->unk1C[3].unk0.vx = 0;
        work->unk1C[3].unk0.vy = 0;
        work->unk1C[3].unk0.vz = 0;
        work->unk1C[3].unk8.vx = 0;
        work->unk1C[3].unk8.vy = 0;
        work->unk1C[3].unk8.vz = 0;
        work->unk1C[3].unk18.vx = 0x1000;
        work->unk1C[3].unk18.vy = 0x1000;
        work->unk1C[3].unk18.vz = 0x1000;
        work->unkF4[9] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x9E), 1);
    } else {
        frame = entry->unk0;
        if (frame == 0x29) {
            ex->Hi_FreeEffModel(work->unkF4[0]);
            ex->Hi_FreeEffModel(work->unkF4[1]);
            ex->Hi_FreeEffModel(work->unkF4[2]);
            for (j = 0; j < 6; j++) {
                ex->Hi_FreeEffModel(work->unkF4[j + 3]);
            }
            ex->Hi_FreeEffModel(work->unkF4[9]);
        }
        for (sp = D_801EA3DC, i = 0; i < 7; i++, sp++) {
            if (sp->unk4 == 0) {
                if (frame == sp->unk2) {
                    (&work->unkBC[i])->vx = sp->unk6[0], (&work->unkBC[i])->vy = sp->unk6[1],
                    (&work->unkBC[i])->vz = sp->unk6[2];
                    work->unk0[i] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, sp->unk0), (s32)&work->unkBC[i]);
                    if (i == 6 && work->unk0[6] != NULL) {
                        work->unk0[6]->unk22 = 0x100;
                    }
                } else if (frame == 0x29) {
                    sp->unk4 = 1;
                    ex->func_800E3D54(work->unk0[i]);
                }
                if (frame >= sp->unk2 && work->unk0[i] != NULL) {
                    if (work->unk0[i]->unk30 == -1) {
                        work->unk0[i] = NULL;
                    }
                    if (work->unk0[i] != NULL) {
                        work->unk0[i]->unk5C.vx = D_801EA474.vx;
                        work->unk0[i]->unk5C.vy = D_801EA474.vy;
                        work->unk0[i]->unk5C.vz = D_801EA474.vz;
                    }
                }
            }
        }
        if (frame >= 1 && frame < 0x29) {
            rp = &work->unk1C[0].unk0;
            func_801E9F04(&D_801EF240, frame, rp, &v);
            work->unk1C[0].unk8.vx = v.vx;
            work->unk1C[0].unk8.vy = v.vy;
            work->unk1C[0].unk8.vz = v.vz;
            work->unk1C[0].unk0.vz = -work->unk1C[0].unk0.vz;
            ex->Hi_SetEffModelSlice(work->unkF4[0], work->unk1C[0].unk8.vy - 0x2D5);
            ex->Hi_DrawSliceEffModel(rp, (VECTOR *)&work->unk1C[0].unk8, (s32)&work->unk1C[0].unk18,
                                     work->unkF4[0]);
            func_801E9FB0(&D_801EF258, frame, &t);
            ex->Hi_DrawMorphEffModel(&work->unk1C[1].unk0, (VECTOR *)&work->unk1C[1].unk8, (s32)&work->unk1C[1].unk18,
                                     work->unkF4[1], work->unkF4[2], t);
            for (n = 0; n < 6; n++) {
                func_801E9F04(&D_801EF270[n], frame, &work->unk1C[2].unk0, &w);
                work->unk1C[2].unk8.vx = w.vx;
                work->unk1C[2].unk8.vy = w.vy;
                work->unk1C[2].unk8.vz = w.vz;
                ex->Hi_SetEffModelSlice(work->unkF4[n + 3], work->unk1C[2].unk8.vy);
                ex->Hi_DrawSliceEffModel(&work->unk1C[2].unk0, (VECTOR *)&work->unk1C[2].unk8,
                                         (s32)&work->unk1C[2].unk18, work->unkF4[n + 3]);
            }
            if (frame >= 1 && frame < 0x29) {
                ex->Hi_DrawEffModel(&work->unk1C[3].unk0, (VECTOR *)&work->unk1C[3].unk8, (s32)&work->unk1C[3].unk18,
                                    work->unkF4[9]);
            }
        }
        return frame >= 0x2D;
    }
    return 0;
}
