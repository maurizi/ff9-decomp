/* Draft of fx_0b1 fx_0b1_op80. Status: full draft, ~31 aligned diff lines ignoring registers per ccprobe (several are ccprobe artifacts of the split
 * symbols: sw x,20(base D_801E8D3C) vs sw x,0(base D_801E8D50) link to the same bytes): writes the strip headers through
 * &D_801EA8FC directly, and the D_801E8D7C/D_801EA86C/D_801EA884 stores schedule differently. The original addresses
 * D_801E8D3C[4]/[5] and D_801EA89C.vy through the split symbols D_801E8D4C/D_801E8D50/D_801EA89E. 
 * The 21-point loop's li/beqz guard comes from 's32 n = 21' at the top and 'i < n' (n spilled, rematerialized). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* start frame */
    s32 unk4; /* frames left, -1 = free */
    u32 unk8; /* destination slots, 4 bits each (1-based) */
    u32 unkC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *unk10; /* data */
} Fx0b1Track; /* 0x14 */

extern s32 D_801E8B90; /* frame */
extern Fx0b1Track D_801E8B9C[16];
extern SVECTOR D_801E8CDC[];
extern s32 D_801EAA54; /* fade timer */
extern s32 D_801EAA5C[4]; /* models */
extern SVECTOR D_801EAA6C; /* model rotation */
extern VECTOR D_801EAA7C; /* model scale */
extern VECTOR D_801EAA8C[4]; /* model positions */
extern SVECTOR D_801EAACC[4]; /* unit body positions */
extern BtlModel D_801EAAEC[];

void func_801E7704(void);
void func_801E7738(s32 start, u32 dst, u32 src, s16 *data);
void func_801E7788(void);
extern Unk800F78A4 *D_801E8B8C;
s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to);
typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *unk180;
} Fx0b1Unit;

typedef struct {
    u8 pad0[0x54];
    Fx0b1Unit *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx0b1Summon;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[21];
} Fx0b1Strip; /* 0xAC */

extern SVECTOR D_801E8D04;
extern VECTOR D_801E8D0C;
extern VECTOR D_801E8D1C;
extern SVECTOR D_801E8D2C;
extern s32 D_801E8D3C[8];
extern Pool6CEntry *D_801E8D5C;
extern SVECTOR D_801E8D64;
extern Pool6CEntry *D_801E8D6C;
extern SVECTOR D_801E8D74;
extern Pool6CEntry *D_801E8D7C;
extern SVECTOR D_801E8D84;
extern u8 D_801E8D8C[];
extern Pool6CEntry *D_801EA86C;
extern SVECTOR D_801EA874;
extern SVECTOR D_801EA87C;
extern Pool6CEntry *D_801EA884;
extern SVECTOR D_801EA88C;
extern Pool6CEntry *D_801EA894;
extern SVECTOR D_801EA89C;
extern Pool6CEntry *D_801EA8A4;
extern SVECTOR D_801EA8AC;
extern Pool6CEntry *D_801EA8B4;
extern SVECTOR D_801EA8BC;
extern Pool6CEntry *D_801EA8C4;
extern SVECTOR D_801EA8CC;
extern Pool6CEntry *D_801EA8D4;
extern SVECTOR D_801EA8DC;
extern Pool6CEntry *D_801EA8E4;
extern SVECTOR D_801EA8EC;
extern Fx0b1Strip D_801EA8FC[2];
s32 fx_0b1_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

s32 fx_0b1_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    Pool6CEntry *p;
    SVECTOR *v0;
    SVECTOR *v1;
    s32 frame;
    s32 i;
    s32 y;
    s32 w;
    s32 a;
    s32 n = 21;
    s32 c;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E8B8C = env;
        for (i = 7; i >= 0; i--) {
            D_801E8D3C[i] = 0;
        }
        D_801E8D5C = NULL;
        D_801E8D6C = NULL;
        D_801E8D7C = NULL;
        D_801EA86C = NULL;
        D_801EA884 = NULL;
        D_801EA894 = NULL;
        D_801EA8A4 = NULL;
        D_801EA8B4 = NULL;
        D_801EA8C4 = NULL;
        D_801EA8E4 = NULL;
        D_801EA8D4 = NULL;
        D_801EA8DC.vx = 0;
        D_801EA8DC.vy = 0;
        D_801EA8DC.vz = -0x618;
        D_801E8D04.vx = 0;
        D_801E8D04.vy = 0;
        D_801E8D04.vz = 0;
        D_801E8D0C.vx = 0;
        D_801E8D0C.vy = 0;
        D_801E8D0C.vz = 0;
        D_801E8D1C.vx = 0x1000;
        D_801E8D1C.vy = 0x1000;
        D_801E8D1C.vz = 0x1000;
        D_801E8D2C.vx = 0;
        D_801E8D2C.vy = 0;
        D_801E8D2C.vz = -0x2EE;
        func_801E7704();
        func_801E7738(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x17));
        func_801E7738(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0xF));
        func_801E7738(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x19));
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0, NULL, -1, 0x1000, 0x1000, 1,
                            (u8 *)&D_801EA8FC[0], 0);
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0, NULL, -1, 0x1000, 0x1000, 1,
                            (u8 *)&D_801EA8FC[1], 0);
    } else {
    frame = entry->unk0;
    switch (frame) {
    case 1:
        ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&D_801E8D2C);
        ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&D_801E8D2C);
        ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&D_801E8D2C);
        D_801E8D3C[5] = 0x10;
        break;
    case 9:
        ex->Hi_SetSummonMotion(((Fx0b1Summon *)env->unk4)->unk54->unk180, ((Fx0b1Summon *)env->unk4)->unk60);
        D_801E8D3C[0] = 1;
        D_801E8D5C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&D_801E8D64);
        break;
    case 24:
        D_801E8D6C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&D_801E8D74);
        break;
    case 0x40:
        ex->Hi_GetSummonBonePos(((Fx0b1Summon *)env->unk4)->unk60, 0x11, (u16 *)&D_801E8D84);
        D_801E8D7C = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&D_801E8D84);
        if (p != NULL) {
            p->unk14 = (s32)D_801E8D8C;
            p->unk12 = 0x28;
            p->unkC = 0xAC;
            p->unk22 = 0x50;
        }
        break;
    case 0x48:
        ex->Hi_GetSummonBonePos(((Fx0b1Summon *)env->unk4)->unk60, 0x11, (u16 *)&D_801EA87C);
        D_801EA86C = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&D_801EA87C);
        if (p != NULL) {
            p->unk22 = 0x40;
        }
        break;
    case 0x67:
        ex->func_800E3D54(D_801EA86C);
        break;
    case 0x6C:
        ex->func_800E3D54(D_801E8D7C);
        ex->Hi_GetSummonBonePos(((Fx0b1Summon *)env->unk4)->unk60, 0, (u16 *)&D_801EA88C);
        D_801EA88C.vy -= 0x180;
        D_801EA884 = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&D_801EA88C);
        if (p != NULL) {
            p->unk22 = -0x80;
        }
        D_801E8D3C[1] = 0x10;
        ex->Hi_GetSummonBonePos(((Fx0b1Summon *)env->unk4)->unk60, 0x11, (u16 *)&D_801EA89C);
        D_801EA894 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&D_801EA89C);
        D_801E8D3C[2] = 8;
        D_801E8D3C[3] = 1;
        break;
    case 0x77:
        D_801E8D3C[0] = 0;
        ex->func_800E3D54(D_801E8D5C);
        ex->func_800E3D54(D_801E8D6C);
        D_801EA8AC.vy = D_801EA89C.vy - 0x1800;
        D_801EA8AC.vx = D_801EA89C.vx;
        D_801EA8AC.vz = D_801EA89C.vz;
        D_801EA8A4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&D_801EA8AC);
        break;
    case 0x97:
        ex->func_800E3D54(D_801EA884);
        ex->func_800E3D54(D_801EA894);
        ex->func_800E3D54(D_801EA8A4);
        ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, 0xB0, 0xB0, 0xB0);
        D_801EA8BC.vy = D_801EA8AC.vy;
        D_801EA8BC.vx = D_801EA8AC.vx;
        D_801EA8BC.vz = D_801EA8AC.vz;
        D_801EA8B4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&D_801EA8BC);
        break;
    case 0x99:
        D_801E8D3C[4] = 4;
        D_801EA8EC.vx = D_801EA8CC.vx = D_801EA8AC.vx;
        D_801EA8EC.vy = D_801EA8CC.vy = D_801EA8AC.vy;
        D_801EA8EC.vz = D_801EA8CC.vz = D_801EA8AC.vz;
        D_801EA8C4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&D_801EA8CC);
        break;
    case 0x9F:
        D_801EA8E4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x24), (s32)&D_801EA8EC);
        break;
    case 0xAE:
        D_801EA8D4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&D_801EA8DC);
        break;
    case 0xCA:
        ex->func_800E3D54(D_801EA8E4);
        break;
    }
    if (D_801E8D3C[0] != 0) {
        D_801E8D0C.vx = env->unk2C->unk0[2].vx;
        D_801E8D0C.vy = env->unk2C->unk0[2].vy;
        D_801E8D0C.vz = env->unk2C->unk0[2].vz;
        ex->Hi_DrawSummonModel(&D_801E8D04, &D_801E8D0C, (s32)&D_801E8D1C, ((Fx0b1Summon *)env->unk4)->unk60, 1);
        if (D_801E8D3C[1] != 0) {
            D_801E8D3C[1]--;
            ex->Hi_ModifySummonModelRGB(((Fx0b1Summon *)env->unk4)->unk60, -0x40, -0x40, -0x40);
        }
    }
    if (D_801E8D5C != NULL) {
        if (D_801E8D5C->unk30 == -1) {
            D_801E8D5C = NULL;
        }
        ex->Hi_GetSummonBonePos(((Fx0b1Summon *)env->unk4)->unk60, 0, (u16 *)&D_801E8D64);
    }
    if (D_801E8D6C != NULL) {
        if (D_801E8D6C->unk30 == -1) {
            D_801E8D6C = NULL;
        }
        ex->Hi_GetSummonBonePos(((Fx0b1Summon *)env->unk4)->unk60, 0, (u16 *)&D_801E8D74);
    }
    if (D_801EA86C != NULL) {
        if (D_801EA86C->unk30 == -1) {
            D_801EA86C = NULL;
        }
        ex->Hi_GetSummonBonePos(((Fx0b1Summon *)env->unk4)->unk60, 0x11, (u16 *)&D_801EA874);
        ex->func_800E3D20((s32)D_801EA86C, (s32)&D_801EA874);
    }
    if (D_801E8D3C[3] != 0) {
        v0 = D_801EA8FC[0].unk4;
        D_801EA8FC[0].unk0 = D_801EA8FC[0].unk2 = 0x14;
        D_801EA8FC[0].unk1 = 0;
        D_801EA8FC[1].unk0 = D_801EA8FC[1].unk2 = 0x14;
        D_801EA8FC[1].unk1 = 0;
        v1 = D_801EA8FC[1].unk4;
        if (D_801E8D3C[2] != 0) {
            y = D_801EA89C.vy - func_801E792C(1, 8, --D_801E8D3C[2], 0, 0x1800);
        } else {
            y = D_801EA89C.vy - 0x1800;
        }
        if (D_801E8D3C[4] != 0) {
            D_801E8D3C[4]--;
            if (D_801E8D3C[4] == 0) {
                D_801E8D3C[3] = 0;
            }
            w = D_801E8D3C[4] * 0x10;
            a = D_801E8D3C[4] << 10;
        } else {
            w = 0x40;
            a = 0x1000;
        }
        for (i = 0; i < n; i++, v0++, v1++) {
            v1->vy = v0->vy = ex->btlfx_lerpRatio(D_801EA89C.vy, y, i, n);
            v1->vx = v0->vx = D_801EA89C.vx;
            v1->vz = v0->vz = D_801EA89C.vz;
            v0->pad = w;
            v1->pad = w * 2;
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0, NULL, frame, 0x1000, a, 1,
                            (u8 *)&D_801EA8FC[0], 0);
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0, NULL, frame, 0x1000, a, 1,
                            (u8 *)&D_801EA8FC[1], 0);
    }
    if (D_801E8D3C[5] != 0) {
        D_801E8D3C[5]--;
        if (D_801E8D3C[5] >= 9) {
            c = (0x10 - D_801E8D3C[5]) * 0x10;
        } else {
            c = D_801E8D3C[5] * 0x10;
        }
        col.r = col.g = col.b = c;
        scale.vx = 0x1000;
        scale.vy = 0x1000;
        scale.vz = 0x1000;
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        ex->func_800E6764(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, NULL);
    }
    func_801E7788();
    return frame >= 0x10A;
    }
    return 0;
}
