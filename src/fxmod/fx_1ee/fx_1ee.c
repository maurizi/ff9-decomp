#include "fxmod/fx_1ee.h"

void func_801E7704(void) {
    s32 i;
    s32 m;

    D_801E9108 = 0;
    m = -1;
    for (i = 15; i >= 0; i--) {
        D_801E9114[i].unk4 = m;
    }
}

void func_801E7738(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (D_801E9114[i].unk4 == -1) {
            D_801E9114[i].unk4 = 0;
            D_801E9114[i].unk10 = data + 1;
            D_801E9114[i].unk0 = start;
            D_801E9114[i].unk8 = dst;
            D_801E9114[i].unkC = src;
            return;
        }
    }
}

INCLUDE_ASM("asm/us/fx_1ee/nonmatchings/fx_1ee", func_801E7788);

s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9104);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

s32 fx_1ee_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR *p;
    Pool6CEntry *e;
    Pool6CEntry *e2;
    Pool6CEntry *e3;
    SVECTOR *q;
    Fx1eeTrail *tr;
    Fx1eeTrail *tr2;
    s32 frame;
    s32 a;
    s32 c;
    s32 y;
    s32 i;
    u8 n = 0x15;
    s32 t;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E9104 = env;
        for (i = 7; i >= 0; i--) {
            D_801E92B4[i] = 0;
        }
        D_801E92D4 = NULL;
        D_801E92E4 = NULL;
        D_801E92F4 = NULL;
        D_801EADE4 = NULL;
        D_801EADFC = NULL;
        D_801EAE0C = NULL;
        D_801EAE1C = NULL;
        D_801EAE2C = NULL;
        D_801EAE3C = NULL;
        D_801EAE5C = NULL;
        D_801EAE4C = NULL;
        D_801EAE54.vx = 0;
        D_801EAE54.vy = 0;
        D_801EAE54.vz = -0x618;
        D_801E927C.vx = 0;
        D_801E927C.vy = 0;
        D_801E927C.vz = 0;
        D_801E9284.vx = 0;
        D_801E9284.vy = 0;
        D_801E9284.vz = 0;
        D_801E9294.vx = 0x1000;
        D_801E9294.vy = 0x1000;
        D_801E9294.vz = 0x1000;
        D_801E92A4.vx = 0;
        D_801E92A4.vy = 0;
        D_801E92A4.vz = -0x2EE;
        func_801E7704();
        func_801E7738(0, 3, 0, ex->btlfx_getFileData(entry->unk8, 0x1F));
        func_801E7738(0, 1, 0, ex->btlfx_getFileData(entry->unk8, 0x1E));
        func_801E7738(0, 2, 0, ex->btlfx_getFileData(entry->unk8, 0x20));
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x33), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&D_801EAE74[0], 0);
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x33), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&D_801EAE74[1], 0);
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 1:
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x3A), (s32)&D_801E92A4);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x38), (s32)&D_801E92A4);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x3C), (s32)&D_801E92A4);
            D_801E92B4[5] = 0x10;
            break;
        case 9:
            ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk180, SUMMON(env)->unk60);
            D_801E92B4[0] = 1;
            D_801E92D4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2E), (s32)&D_801E92DC);
            break;
        case 0x18:
            D_801E92E4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2E), (s32)&D_801E92EC);
            break;
        case 0x40:
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x11, (u16 *)&D_801E92FC);
            D_801E92F4 = e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x3B), (s32)&D_801E92FC);
            if (e != NULL) {
                e->unk14 = (s32)D_801E9304;
                e->unk12 = 0x28;
                e->unkC = 0xAC;
                e->unk22 = 0x50;
            }
            break;
        case 0x48:
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x11, (u16 *)&D_801EADF4);
            D_801EADE4 = e2 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x39), (s32)&D_801EADF4);
            if (e2 != NULL) {
                e2->unk22 = 0x40;
            }
            break;
        case 0x67:
            ex->func_800E3D54(D_801EADE4);
            break;
        case 0x6C:
            ex->func_800E3D54(D_801E92F4);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0, (u16 *)&D_801EAE04);
            D_801EAE04.vy -= 0x180;
            D_801EADFC = e3 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x30), (s32)&D_801EAE04);
            if (e3 != NULL) {
                e3->unk22 = -0x80;
            }
            D_801E92B4[1] = 0x10;
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x11, (u16 *)&D_801EAE14);
            D_801EAE0C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2B), (s32)&D_801EAE14);
            D_801E92B4[2] = 8;
            D_801E92B4[3] = 1;
            break;
        case 0x77:
            D_801E92B4[0] = 0;
            ex->func_800E3D54(D_801E92D4);
            ex->func_800E3D54(D_801E92E4);
            D_801EAE24.vy = D_801EAE14.vy - 0x1800;
            D_801EAE24.vx = D_801EAE14.vx;
            D_801EAE24.vz = D_801EAE14.vz;
            D_801EAE1C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x35), (s32)&D_801EAE24);
            break;
        case 0x97:
            ex->func_800E3D54(D_801EADFC);
            ex->func_800E3D54(D_801EAE0C);
            ex->func_800E3D54(D_801EAE1C);
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, 0xB0, 0xB0, 0xB0);
            D_801EAE34.vy = D_801EAE24.vy;
            D_801EAE34.vx = D_801EAE24.vx;
            D_801EAE34.vz = D_801EAE24.vz;
            D_801EAE2C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x31), (s32)&D_801EAE34);
            break;
        case 0x99:
            D_801E92B4[4] = 4;
            D_801EAE44.vx = D_801EAE64.vx = D_801EAE24.vx;
            D_801EAE44.vy = D_801EAE64.vy = D_801EAE24.vy;
            D_801EAE44.vz = D_801EAE64.vz = D_801EAE24.vz;
            D_801EAE3C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2C), (s32)&D_801EAE44);
            break;
        case 0x9F:
            D_801EAE5C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x36), (s32)&D_801EAE64);
            break;
        case 0xAE:
            D_801EAE4C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x32), (s32)&D_801EAE54);
            break;
        case 0xCA:
            ex->func_800E3D54(D_801EAE5C);
            break;
        }
        if (D_801E92B4[0] != 0) {
            D_801E9284.vx = env->unk2C->unk0[2].vx;
            D_801E9284.vy = env->unk2C->unk0[2].vy;
            D_801E9284.vz = env->unk2C->unk0[2].vz;
            ex->Hi_DrawSummonModel(&D_801E927C, &D_801E9284, (s32)&D_801E9294, SUMMON(env)->unk60, 1);
            if (D_801E92B4[1] != 0) {
                D_801E92B4[1]--;
                ex->Hi_ModifySummonModelRGB(SUMMON(env)->unk60, -0x40, -0x40, -0x40);
            }
        }
        if (D_801E92D4 != NULL) {
            if (D_801E92D4->unk30 == -1) {
                D_801E92D4 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0, (u16 *)&D_801E92DC);
        }
        if (D_801E92E4 != NULL) {
            if (D_801E92E4->unk30 == -1) {
                D_801E92E4 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0, (u16 *)&D_801E92EC);
        }
        if (D_801EADE4 != NULL) {
            if (D_801EADE4->unk30 == -1) {
                D_801EADE4 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x11, (u16 *)&D_801EADEC);
            ex->func_800E3D20((s32)D_801EADE4, (s32)&D_801EADEC);
        }
        if (D_801E92B4[3] != 0) {
            tr = D_801EAE74;
            p = tr->unk4;
            tr->unk0 = tr->unk2 = 0x14;
            tr->unk1 = 0;
            tr2 = tr + 1;
            tr2->unk0 = tr2->unk2 = 0x14;
            tr2->unk1 = 0;
            do {
                            } while (0);
            q = tr[1].unk4;
            if (D_801E92B4[2] != 0) {
                D_801E92B4[2]--;
                y = D_801EAE14.vy - func_801E792C(1, 8, D_801E92B4[2], 0, 0x1800);
            } else {
                y = D_801EAE14.vy - 0x1800;
            }
            if (D_801E92B4[4] != 0) {
                D_801E92B4[4]--;
                if (D_801E92B4[4] == 0) {
                    D_801E92B4[3] = 0;
                }
                c = D_801E92B4[4] << 4;
                a = D_801E92B4[4] << 10;
            } else {
                c = 0x40;
                a = 0x1000;
            }
            for (i = 0; i < n; i++) {
                p->vy = q->vy = ex->btlfx_lerpRatio(D_801EAE14.vy, y, i, 0x15);
                p->vx = q->vx = D_801EAE14.vx;
                p->vz = q->vz = D_801EAE14.vz;
                p->pad = c;
                q->pad = c * 2;
                p++;
                q++;
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x33), 0, NULL, frame, 0x1000, a, 1, (u8 *)&D_801EAE74[0], 0);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x33), 0, NULL, frame, 0x1000, a, 1, (u8 *)&D_801EAE74[1], 0);
        }
        if (D_801E92B4[5] != 0) {
            D_801E92B4[5]--;
            if (D_801E92B4[5] >= 9) {
                t = (0x10 - D_801E92B4[5]) << 4;
            } else {
                t = D_801E92B4[5] << 4;
            }
            col.r = col.g = col.b = t;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->func_800E6764(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, 0);
        }
        func_801E7788();
        return frame >= 0x10A;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1ee/nonmatchings/fx_1ee", fx_1ee_op81);

INCLUDE_RODATA("asm/us/fx_1ee/nonmatchings/fx_1ee", fx_1ee_headerPtr);
