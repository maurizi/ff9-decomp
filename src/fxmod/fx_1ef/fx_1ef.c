#include "fxmod/fx_1ef.h"

void func_801E7704(void) {
    s32 i;
    s32 m;

    D_801E8BA0 = 0;
    m = -1;
    for (i = 15; i >= 0; i--) {
        D_801E8BAC[i].unk4 = m;
    }
}

void func_801E7738(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (D_801E8BAC[i].unk4 == -1) {
            D_801E8BAC[i].unk4 = 0;
            D_801E8BAC[i].unk10 = data + 1;
            D_801E8BAC[i].unk0 = start;
            D_801E8BAC[i].unk8 = dst;
            D_801E8BAC[i].unkC = src;
            return;
        }
    }
}

void func_801E7788(void) {
    SVECTOR *ext = D_801E8CEC;
    s32 i;
    Unk800F78A4 *env = D_801E8B9C;
    s32 frame = D_801E8BA0;
    s32 k;
    s32 d;
    s32 s;
    s32 x, y, z;
    s32 j;

    for (i = 0; i < 16; i++) {
        if (D_801E8BAC[i].unk4 == 0 && D_801E8BAC[i].unk0 == frame) {
            D_801E8BAC[i].unk4 = *D_801E8BAC[i].unk10;
            D_801E8BAC[i].unk10++;
        }
        if (D_801E8BAC[i].unk4 > 0) {
            D_801E8BAC[i].unk4--;
            for (k = 0; k < 3; k++) {
                d = ((D_801E8BAC[i].unk8 >> (k * 4)) & 0xF) - 1;
                s = ((D_801E8BAC[i].unkC >> (k * 4)) & 0xF) - 1;
                if (d >= 0) {
                    x = *D_801E8BAC[i].unk10;
                    D_801E8BAC[i].unk10++;
                    y = *D_801E8BAC[i].unk10;
                    D_801E8BAC[i].unk10++;
                    z = *D_801E8BAC[i].unk10;
                    D_801E8BAC[i].unk10++;
                    if (s >= 0) {
                        /* each branch adds its own source; jump2's cross-jumping merges the identical tails */
                        if (s >= 10) {
                            j = s - 10;
                            x += ext[j].vx;
                            y += ext[j].vy;
                            z += ext[j].vz;
                        } else {
                            x += ((SVECTOR *)env->camera)[s].vx;
                            y += ((SVECTOR *)env->camera)[s].vy;
                            z += ((SVECTOR *)env->camera)[s].vz;
                        }
                    }
                    if (d >= 10) {
                        j = d - 10;
                        ext[j].vx = x;
                        ext[j].vy = y;
                        ext[j].vz = z;
                    } else {
                        ((SVECTOR *)env->camera)[d].vx = x;
                        ((SVECTOR *)env->camera)[d].vy = y;
                        ((SVECTOR *)env->camera)[d].vz = z;
                    }
                }
            }
        }
    }
    D_801E8BA0++;
}

s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8B9C);
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

s32 fx_1ef_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR *p;
    Pool6CEntry *e;
    Pool6CEntry *e2;
    Pool6CEntry *e3;
    SVECTOR *q;
    Fx1efTrail *tr;
    Fx1efTrail *tr2;
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
        D_801E8B9C = env;
        for (i = 7; i >= 0; i--) {
            D_801E8D4C[i] = 0;
        }
        D_801E8D6C = NULL;
        D_801E8D7C = NULL;
        D_801E8D8C = NULL;
        D_801EA87C = NULL;
        D_801EA894 = NULL;
        D_801EA8A4 = NULL;
        D_801EA8B4 = NULL;
        D_801EA8C4 = NULL;
        D_801EA8D4 = NULL;
        D_801EA8F4 = NULL;
        D_801EA8E4 = NULL;
        D_801EA8EC.vx = 0;
        D_801EA8EC.vy = 0;
        D_801EA8EC.vz = -0x618;
        D_801E8D14.vx = 0;
        D_801E8D14.vy = 0;
        D_801E8D14.vz = 0;
        D_801E8D1C.vx = 0;
        D_801E8D1C.vy = 0;
        D_801E8D1C.vz = 0;
        D_801E8D2C.vx = 0x1000;
        D_801E8D2C.vy = 0x1000;
        D_801E8D2C.vz = 0x1000;
        D_801E8D3C.vx = 0;
        D_801E8D3C.vy = 0;
        D_801E8D3C.vz = -0x2EE;
        func_801E7704();
        func_801E7738(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x14));
        func_801E7738(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x13));
        func_801E7738(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x15));
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&D_801EA90C[0], 0);
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&D_801EA90C[1], 0);
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 1:
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&D_801E8D3C);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&D_801E8D3C);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&D_801E8D3C);
            D_801E8D4C[5] = 0x10;
            break;
        case 9:
            ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk180, SUMMON(env)->unk60);
            D_801E8D4C[0] = 1;
            D_801E8D6C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&D_801E8D74);
            break;
        case 0x18:
            D_801E8D7C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&D_801E8D84);
            break;
        case 0x40:
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x11, (u16 *)&D_801E8D94);
            D_801E8D8C = e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&D_801E8D94);
            if (e != NULL) {
                e->unk14 = (s32)D_801E8D9C;
                e->unk12 = 0x28;
                e->unkC = 0xAC;
                e->unk22 = 0x50;
            }
            break;
        case 0x48:
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x11, (u16 *)&D_801EA88C);
            D_801EA87C = e2 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&D_801EA88C);
            if (e2 != NULL) {
                e2->unk22 = 0x40;
            }
            break;
        case 0x67:
            ex->func_800E3D54(D_801EA87C);
            break;
        case 0x6C:
            ex->func_800E3D54(D_801E8D8C);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0, (u16 *)&D_801EA89C);
            D_801EA89C.vy -= 0x180;
            D_801EA894 = e3 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&D_801EA89C);
            if (e3 != NULL) {
                e3->unk22 = -0x80;
            }
            D_801E8D4C[1] = 0x10;
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x11, (u16 *)&D_801EA8AC);
            D_801EA8A4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&D_801EA8AC);
            D_801E8D4C[2] = 8;
            D_801E8D4C[3] = 1;
            break;
        case 0x77:
            D_801E8D4C[0] = 0;
            ex->func_800E3D54(D_801E8D6C);
            ex->func_800E3D54(D_801E8D7C);
            D_801EA8BC.vy = D_801EA8AC.vy - 0x1800;
            D_801EA8BC.vx = D_801EA8AC.vx;
            D_801EA8BC.vz = D_801EA8AC.vz;
            D_801EA8B4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&D_801EA8BC);
            break;
        case 0x97:
            ex->func_800E3D54(D_801EA894);
            ex->func_800E3D54(D_801EA8A4);
            ex->func_800E3D54(D_801EA8B4);
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xB0, 0xB0, 0xB0);
            D_801EA8CC.vy = D_801EA8BC.vy;
            D_801EA8CC.vx = D_801EA8BC.vx;
            D_801EA8CC.vz = D_801EA8BC.vz;
            D_801EA8C4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&D_801EA8CC);
            break;
        case 0x99:
            D_801E8D4C[4] = 4;
            D_801EA8DC.vx = D_801EA8FC.vx = D_801EA8BC.vx;
            D_801EA8DC.vy = D_801EA8FC.vy = D_801EA8BC.vy;
            D_801EA8DC.vz = D_801EA8FC.vz = D_801EA8BC.vz;
            D_801EA8D4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&D_801EA8DC);
            break;
        case 0x9F:
            D_801EA8F4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&D_801EA8FC);
            break;
        case 0xAE:
            D_801EA8E4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1F), (s32)&D_801EA8EC);
            break;
        case 0xCA:
            ex->func_800E3D54(D_801EA8F4);
            break;
        }
        if (D_801E8D4C[0] != 0) {
            D_801E8D1C.vx = env->camera->unk0[2].vx;
            D_801E8D1C.vy = env->camera->unk0[2].vy;
            D_801E8D1C.vz = env->camera->unk0[2].vz;
            ex->Hi_DrawSummonModel(&D_801E8D14, &D_801E8D1C, (s32)&D_801E8D2C, SUMMON(env)->unk60, 1);
            if (D_801E8D4C[1] != 0) {
                D_801E8D4C[1]--;
                ex->Hi_ModifySummonModelRGB(SUMMON(env)->unk60, -0x40, -0x40, -0x40);
            }
        }
        if (D_801E8D6C != NULL) {
            if (D_801E8D6C->unk30 == -1) {
                D_801E8D6C = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0, (u16 *)&D_801E8D74);
        }
        if (D_801E8D7C != NULL) {
            if (D_801E8D7C->unk30 == -1) {
                D_801E8D7C = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0, (u16 *)&D_801E8D84);
        }
        if (D_801EA87C != NULL) {
            if (D_801EA87C->unk30 == -1) {
                D_801EA87C = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x11, (u16 *)&D_801EA884);
            ex->func_800E3D20((s32)D_801EA87C, (s32)&D_801EA884);
        }
        if (D_801E8D4C[3] != 0) {
            tr = D_801EA90C;
            p = tr->unk4;
            tr->unk0 = tr->unk2 = 0x14;
            tr->unk1 = 0;
            tr2 = tr + 1;
            tr2->unk0 = tr2->unk2 = 0x14;
            tr2->unk1 = 0;
            do {
                            } while (0);
            q = tr[1].unk4;
            if (D_801E8D4C[2] != 0) {
                D_801E8D4C[2]--;
                y = D_801EA8AC.vy - func_801E792C(1, 8, D_801E8D4C[2], 0, 0x1800);
            } else {
                y = D_801EA8AC.vy - 0x1800;
            }
            if (D_801E8D4C[4] != 0) {
                D_801E8D4C[4]--;
                if (D_801E8D4C[4] == 0) {
                    D_801E8D4C[3] = 0;
                }
                c = D_801E8D4C[4] << 4;
                a = D_801E8D4C[4] << 10;
            } else {
                c = 0x40;
                a = 0x1000;
            }
            for (i = 0; i < n; i++) {
                p->vy = q->vy = ex->btlfx_lerpRatio(D_801EA8AC.vy, y, i, 0x15);
                p->vx = q->vx = D_801EA8AC.vx;
                p->vz = q->vz = D_801EA8AC.vz;
                p->pad = c;
                q->pad = c * 2;
                p++;
                q++;
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), 0, NULL, frame, 0x1000, a, 1, (u8 *)&D_801EA90C[0], 0);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), 0, NULL, frame, 0x1000, a, 1, (u8 *)&D_801EA90C[1], 0);
        }
        if (D_801E8D4C[5] != 0) {
            D_801E8D4C[5]--;
            if (D_801E8D4C[5] >= 9) {
                t = (0x10 - D_801E8D4C[5]) << 4;
            } else {
                t = D_801E8D4C[5] << 4;
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

s32 fx_1ef_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801EAA64[0] = 0;
        D_801EAA64[1] = 0;
        D_801E8B9C = env;
        for (i = 0; i < env->targets->unk24; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801EAA6C[i]);
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&D_801EAA6C[i]);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(i, 0x200);
                p->unk12 = 0x1E;
                p->unk14 = (s32)D_801EAA8C[i];
                p->unkC = 0x9C;
            }
        }
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 16:
            for (i = 0; i < env->targets->unk24; i++) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&D_801EAA6C[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x200);
                    p->unk12 = 0x1E;
                    p->unk14 = (s32)D_801ECA6C[i];
                    p->unkC = 0x9C;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&D_801EAA6C[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x200);
                }
            }
            break;
        case 24:
            D_801EAA64[0] = 0x10;
            break;
        case 40:
            D_801EAA64[1] = 0x10;
            break;
        }
        if (D_801EAA64[0] != 0) {
            D_801EAA64[0]--;
            for (i = 0; i < env->targets->unk24; i++) {
                ex->func_800E806C(i, D_801EAA64[0] * 8);
            }
        }
        if (D_801EAA64[1] != 0) {
            D_801EAA64[1]--;
            for (i = 0; i < env->targets->unk24; i++) {
                ex->func_800E806C(i, 0x80 - D_801EAA64[1] * 8);
            }
        }
        return frame >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ef/nonmatchings/fx_1ef", fx_1ef_headerPtr);
