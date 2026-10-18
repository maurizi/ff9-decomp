/* Draft of fx_0e2 op82 (four textured ribbons and two models). Status: 18 aligned mismatches (3 of them are only
 * relocation addends: D_801EE598 / D_801EE5C8 / D_801EE5CC are fields of the ribbon structs): the fourth ribbon's
 * frame % 24 in the original lands in a temporary (v1) used by the first two compares and the % 8, with a copy (a2)
 * for the third compare; here k is the copy and every compare uses it. Levers so far: frame copy t for the second
 * ribbon's % 30, separate clamp variables n (ribbon 1) and m (ribbons 2-3), mode passed as the 1 of mode 1, the
 * per-ribbon index variables split (a/b, a2/b2, a3/b3), the third ribbon's and the fourth's spin as i * 0x333 givs
 * and the fourth loop's own counter j, and the first ribbon's if-chain on frame % 60 directly. Same code as fx_1fd
 * op82 (other constants). */
#include "fxmod/fx_0e2.h"

typedef struct {
    Fx0e2Path unk0;
    Fx0e2Path unk8;
    SVECTOR unk10; /* scale */
    s16 unk18;
    s16 unk1A; /* length */
    s16 unk1C; /* brightness */
    s16 unk1E; /* width */
    s16 unk20;
    s16 unk22;
    u8 pad24[0xC];
} Fx0e2Ribbon; /* 0x30 */

typedef struct {
    Fx0e2Path unk0;
    SVECTOR unk8; /* scale */
    s16 unk10;
    s16 unk12; /* length */
    s16 unk14; /* brightness */
    s16 unk16; /* width */
    s16 unk18;
} Fx0e2Ribbon1;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* model rotations */
    SVECTOR unk14;
    VECTOR unk1C; /* model position */
    VECTOR unk2C; /* model scale */
    s32 unk3C; /* model brightness */
    SVECTOR unk40; /* effect positions */
    SVECTOR unk48;
    SVECTOR unk50; /* caster position */
    s32 unk58; /* fade counters */
    s32 unk5C;
    s32 unk60;
    s16 unk64;
    s16 unk66;
    u16 unk68; /* models */
    u16 unk6A;
} Fx0e2Work82; /* 0x6C */

extern Fx0e2Ribbon D_801EE57C;
extern Fx0e2Ribbon D_801EE5AC;
extern Fx0e2Ribbon D_801EE5DC;
extern Fx0e2Ribbon1 D_801EE60C;

s32 fx_0e2_op82(s32 mode, Fx0e2Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    s32 k;
    s32 a;
    s32 b;
    s32 c;
    s32 t;
    s32 x;
    s32 a2;
    s32 b2;
    s32 a3;
    s32 b3;
    s32 r;
    s16 n;
    s16 m;

    if (mode == 0) {
        *arg4 = 0x6C;
    } else if (mode == 1) {
        work->unk50.vx = env->unkC->unk38;
        D_801ECC0C = env;
        work->unk50.vy = env->unkC->unk3A;
        work->unk50.vz = env->unkC->unk3C;
        work->unk50.vy -= 0x3E8;
        D_801EE57C.unk1E = 0x200;
        D_801EE57C.unk18 = 0;
        D_801EE57C.unk1A = 0;
        D_801EE57C.unk1C = 0x80;
        D_801EE57C.unk20 = 0;
        D_801EE57C.unk22 = 0;
        D_801EE5AC.unk1E = 0x180;
        D_801EE5AC.unk18 = 0;
        D_801EE5AC.unk1A = 0;
        D_801EE5AC.unk1C = 0x80;
        D_801EE5AC.unk20 = 0;
        D_801EE5AC.unk22 = 0;
        D_801EE60C.unk10 = 0;
        D_801EE60C.unk12 = 0;
        D_801EE60C.unk14 = 0x80;
        D_801EE60C.unk16 = 0x400;
        D_801EE60C.unk18 = 0;
        D_801EE5DC.unk1A = 0x20;
        D_801EE5DC.unk18 = 0;
        D_801EE5DC.unk1C = 0x80;
        D_801EE5DC.unk1E = 0x400;
        D_801EE5DC.unk20 = 0;
        D_801EE5DC.unk22 = 0;
        setVector(&D_801EE57C.unk10, 0x1000, 0x1000, 0x1000);
        setVector(&D_801EE5AC.unk10, 0x1000, 0x1000, 0x1000);
        setVector(&D_801EE60C.unk8, 0x1000, 0x1000, 0x1000);
        setVector(&D_801EE5DC.unk10, 0x1000, 0x1000, 0x1000);
        setVector(&work->unkC, 0, 0, 0);
        setVector(&work->unk14, 0, 0, 0);
        setVector(&work->unk1C, 0, 0, 0x4B0);
        setVector(&work->unk2C, 0x1CCC, 0x1000, 0x1CCC);
        work->unk68 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x11), mode);
        work->unk6A = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1D), mode);
        work->unk58 = -1;
        work->unk5C = -1;
        work->unk60 = mode;
        work->unk64 = 0;
        work->unk66 = 0xC;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk40.vx = 0;
            work->unk40.vy = 0;
            work->unk40.vz = 0x4B0;
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk40);
            break;
        case 1:
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk40);
            break;
        case 6:
            work->unk48.vx = work->unk40.vx;
            work->unk48.vy = work->unk40.vy;
            work->unk48.vz = work->unk40.vz;
            work->unk48.vy -= 0xC80;
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk48);
            break;
        case 0x1C:
            work->unk58 = 0x20;
            work->unk5C = 0x20;
            work->unk60 = 0x20;
            break;
        }
        if (frame >= 0 && frame < 60) {
            D_801EE57C.unk1A += 6;
            n = D_801EE57C.unk1A;
            if (n > 60) {
                n = 60;
            }
            D_801EE57C.unk1A = n;
            D_801EE57C.unk20 = (frame % 60 % 10) * 0x199;
            if (frame % 60 < 10) {
                a = 0;
                b = 6;
            } else if (frame % 60 < 20) {
                a = 6;
                b = 12;
            } else if (frame % 60 < 30) {
                a = 12;
                b = 18;
            } else if (frame % 60 < 40) {
                a = 18;
                b = 24;
            } else if (frame % 60 < 50) {
                a = 24;
                b = 30;
            } else if (frame % 60 < 60) {
                a = 30;
                b = 0;
            } else {
                a = 0;
                b = 0;
            }
            if (D_801EE57C.unk1A >= 60) {
                D_801EE57C.unk10.vy = 0x1000;
                D_801EE57C.unk10.vx += 0x80;
                D_801EE57C.unk10.vz += 0x80;
            } else {
                D_801EE57C.unk10.vx = D_801EE57C.unk10.vy = D_801EE57C.unk10.vz =
                    (s16)(D_801EE57C.unk1A / 10) * 0x199 + 0x800;
            }
            if (work->unk58 >= 0) {
                D_801EE57C.unk1C = func_801EC434(0x80, 0, 0x20 - work->unk58, 0x20);
                work->unk58--;
            }
            for (i = 0; i < 6; i++) {
                func_801EC48C(&D_801EE57C.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), a + i));
                func_801EC48C(&D_801EE57C.unk8, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), b + i));
                func_801E7FBC(&D_801EE57C.unk0, &D_801EE57C.unk8, D_801EE57C.unk18, D_801EE57C.unk1A, &D_801EE57C.unk10,
                              D_801EE57C.unk1E, D_801EE57C.unk20, D_801EE57C.unk22, 1);
                func_801E86D0(D_801EE57C.unk1A, D_801EE57C.unk1C, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 0 && frame < 60) {
            t = frame;
            D_801EE5AC.unk1A += 6;
            m = D_801EE5AC.unk1A;
            if (m > 60) {
                m = 60;
            }
            D_801EE5AC.unk1A = m;
            if (D_801EE5AC.unk1A >= 60) {
                D_801EE5AC.unk10.vy = 0x1000;
                D_801EE5AC.unk10.vx += 0x80;
                D_801EE5AC.unk10.vz += 0x80;
            } else {
                D_801EE5AC.unk10.vx = D_801EE5AC.unk10.vy = D_801EE5AC.unk10.vz =
                    (s16)(D_801EE5AC.unk1A / 10) * 0x199 + 0x800;
            }
            if (work->unk5C >= 0) {
                D_801EE5AC.unk1C = func_801EC434(0x80, 0, 0x20 - work->unk5C, 0x20);
                work->unk5C--;
            }
            k = t;
            k %= 30;
            D_801EE5AC.unk20 = (k % 10) * 0x199;
            if (k < 10) {
                a2 = 0;
                b2 = 3;
            } else if (k < 20) {
                a2 = 3;
                b2 = 6;
            } else if (k < 30) {
                a2 = 6;
                b2 = 0;
            } else {
                a2 = 0;
                b2 = 0;
            }
            for (i = 0; i < 3; i++) {
                func_801EC48C(&D_801EE5AC.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), a2 + i));
                func_801EC48C(&D_801EE5AC.unk8, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), b2 + i));
                func_801E7FBC(&D_801EE5AC.unk0, &D_801EE5AC.unk8, D_801EE5AC.unk18, D_801EE5AC.unk1A, &D_801EE5AC.unk10,
                              D_801EE5AC.unk1E, D_801EE5AC.unk20, D_801EE5AC.unk22, 1);
                func_801E86D0(D_801EE5AC.unk1A, D_801EE5AC.unk1C, 0x9C, 0x3D40, 0x80, 0xBF);
            }
        }
        if (frame >= 0 && frame < 16) {
            D_801EE60C.unk12 += 2;
            m = D_801EE60C.unk12;
            if (m > 0x20) {
                m = 0x20;
            }
            D_801EE60C.unk12 = m;
            for (i = 0; i < 5; i++) {
                if (i & 1) {
                    func_801EC48C(&D_801EE60C.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), 0));
                } else {
                    func_801EC48C(&D_801EE60C.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0));
                }
                D_801EE60C.unk18 = i * 0x333;
                func_801E83B0(&D_801EE60C.unk0, D_801EE60C.unk10, D_801EE60C.unk12, &D_801EE60C.unk8, D_801EE60C.unk16,
                              D_801EE60C.unk18);
                func_801E86D0(D_801EE60C.unk12, D_801EE60C.unk14, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 16 && frame < 60) {
            x = frame - 16;
            D_801EE5DC.unk20 = (x % 24 % 8) << 9;
            k = x % 24;
            if (k < 8) {
                a3 = 0;
                b3 = 1;
            } else if (k < 16) {
                a3 = 1;
                b3 = 2;
            } else if (k < 24) {
                a3 = 2;
                b3 = 0;
            } else {
                a3 = 0;
                b3 = 0;
            }
            if (x >= 0) {
                D_801EE5DC.unk10.vx += 0x80;
                D_801EE5DC.unk10.vz += 0x80;
            }
            if (work->unk58 >= 0) {
                D_801EE5DC.unk1C = func_801EC434(0x80, 0, 0x20 - work->unk58, 0x20);
                work->unk58--;
            }
            for (j = 0; j < 5; j++) {
                if (j & 1) {
                    func_801EC48C(&D_801EE5DC.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), a3));
                    func_801EC48C(&D_801EE5DC.unk8, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), b3));
                } else {
                    func_801EC48C(&D_801EE5DC.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), a3));
                    func_801EC48C(&D_801EE5DC.unk8, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), b3));
                }
                D_801EE5DC.unk22 = j * 0x333;
                func_801E7FBC(&D_801EE5DC.unk0, &D_801EE5DC.unk8, D_801EE5DC.unk18, D_801EE5DC.unk1A, &D_801EE5DC.unk10,
                              D_801EE5DC.unk1E, D_801EE5DC.unk20, D_801EE5DC.unk22, 0);
                func_801E86D0(D_801EE5DC.unk1A, D_801EE5DC.unk1C, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 8 && frame < 60) {
            ex->Hi_DrawEffModel(&work->unkC, &work->unk1C, (s32)&work->unk2C, work->unk68);
            ex->Hi_DrawEffModel(&work->unk14, &work->unk1C, (s32)&work->unk2C, work->unk6A);
            if (work->unk60 >= 0) {
                work->unk3C = func_801EC434(0, -0x80, 0x20 - work->unk60, 0x20);
                work->unk60--;
                ex->Hi_ModifyEffModelRGB(work->unk68, work->unk3C, work->unk3C, work->unk3C);
                ex->Hi_ModifyEffModelRGB(work->unk6A, work->unk3C, work->unk3C, work->unk3C);
            }
        }
        func_801EC920(work->unk66 * 0x78 - work->unk64 * work->unk66, 0x1C0, 0x180, 0x80, 0x40, 0x1C0, 0x1C0);
        func_801EC920(work->unk66 * 0x78 - work->unk64 * work->unk66, 0x300, 0x180, 0x80, 0x40, 0x300, 0x140);
        work->unk64++;
        if (frame >= 0x78) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}
