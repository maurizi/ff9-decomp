#include "fxmod/fx_0b1.h"

void func_801E7704(void) {
    s32 i;
    s32 m;

    D_801E8B90 = 0;
    m = -1;
    for (i = 15; i >= 0; i--) {
        D_801E8B9C[i].unk4 = m;
    }
}

void func_801E7738(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (D_801E8B9C[i].unk4 == -1) {
            D_801E8B9C[i].unk4 = 0;
            D_801E8B9C[i].unk10 = data + 1;
            D_801E8B9C[i].unk0 = start;
            D_801E8B9C[i].unk8 = dst;
            D_801E8B9C[i].unkC = src;
            return;
        }
    }
}

void func_801E7788(void) {
    SVECTOR *ext = D_801E8CDC;
    s32 i;
    Unk800F78A4 *env = D_801E8B8C;
    s32 frame = D_801E8B90;
    s32 k;
    s32 d;
    s32 s;
    s32 x, y, z;
    s32 j;

    for (i = 0; i < 16; i++) {
        if (D_801E8B9C[i].unk4 == 0 && D_801E8B9C[i].unk0 == frame) {
            D_801E8B9C[i].unk4 = *D_801E8B9C[i].unk10;
            D_801E8B9C[i].unk10++;
        }
        if (D_801E8B9C[i].unk4 > 0) {
            D_801E8B9C[i].unk4--;
            for (k = 0; k < 3; k++) {
                d = ((D_801E8B9C[i].unk8 >> (k * 4)) & 0xF) - 1;
                s = ((D_801E8B9C[i].unkC >> (k * 4)) & 0xF) - 1;
                if (d >= 0) {
                    x = *D_801E8B9C[i].unk10;
                    D_801E8B9C[i].unk10++;
                    y = *D_801E8B9C[i].unk10;
                    D_801E8B9C[i].unk10++;
                    z = *D_801E8B9C[i].unk10;
                    D_801E8B9C[i].unk10++;
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
    D_801E8B90++;
}

s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8B8C);
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

s32 fx_0b1_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR *p;
    Pool6CEntry *e;
    Pool6CEntry *e2;
    Pool6CEntry *e3;
    SVECTOR *q;
    Fx0b1Trail *tr;
    Fx0b1Trail *tr2;
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
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&D_801EA8FC[0], 0);
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&D_801EA8FC[1], 0);
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
            ex->Hi_SetSummonMotion(SUMMON0B1(env)->unk54->unk180, SUMMON0B1(env)->unk60);
            D_801E8D3C[0] = 1;
            D_801E8D5C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&D_801E8D64);
            break;
        case 0x18:
            D_801E8D6C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&D_801E8D74);
            break;
        case 0x40:
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->unk60, 0x11, (u16 *)&D_801E8D84);
            D_801E8D7C = e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x9), (s32)&D_801E8D84);
            if (e != NULL) {
                e->unk14 = (s32)D_801E8D8C;
                e->unk12 = 0x28;
                e->unkC = 0xAC;
                e->unk22 = 0x50;
            }
            break;
        case 0x48:
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->unk60, 0x11, (u16 *)&D_801EA87C);
            D_801EA86C = e2 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&D_801EA87C);
            if (e2 != NULL) {
                e2->unk22 = 0x40;
            }
            break;
        case 0x67:
            ex->func_800E3D54(D_801EA86C);
            break;
        case 0x6C:
            ex->func_800E3D54(D_801E8D7C);
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->unk60, 0, (u16 *)&D_801EA88C);
            D_801EA88C.vy -= 0x180;
            D_801EA884 = e3 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1), (s32)&D_801EA88C);
            if (e3 != NULL) {
                e3->unk22 = -0x80;
            }
            D_801E8D3C[1] = 0x10;
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->unk60, 0x11, (u16 *)&D_801EA89C);
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
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xB0, 0xB0, 0xB0);
            D_801EA8BC.vy = D_801EA8AC.vy;
            D_801EA8BC.vx = D_801EA8AC.vx;
            D_801EA8BC.vz = D_801EA8AC.vz;
            D_801EA8B4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&D_801EA8BC);
            break;
        case 0x99:
            D_801E8D3C[4] = 4;
            D_801EA8CC.vx = D_801EA8EC.vx = D_801EA8AC.vx;
            D_801EA8CC.vy = D_801EA8EC.vy = D_801EA8AC.vy;
            D_801EA8CC.vz = D_801EA8EC.vz = D_801EA8AC.vz;
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
            D_801E8D0C.vx = env->camera->unk0[2].vx;
            D_801E8D0C.vy = env->camera->unk0[2].vy;
            D_801E8D0C.vz = env->camera->unk0[2].vz;
            ex->Hi_DrawSummonModel(&D_801E8D04, &D_801E8D0C, (s32)&D_801E8D1C, SUMMON0B1(env)->unk60, 1);
            if (D_801E8D3C[1] != 0) {
                D_801E8D3C[1]--;
                ex->Hi_ModifySummonModelRGB(SUMMON0B1(env)->unk60, -0x40, -0x40, -0x40);
            }
        }
        if (D_801E8D5C != NULL) {
            if (D_801E8D5C->unk30 == -1) {
                D_801E8D5C = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->unk60, 0, (u16 *)&D_801E8D64);
        }
        if (D_801E8D6C != NULL) {
            if (D_801E8D6C->unk30 == -1) {
                D_801E8D6C = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->unk60, 0, (u16 *)&D_801E8D74);
        }
        if (D_801EA86C != NULL) {
            if (D_801EA86C->unk30 == -1) {
                D_801EA86C = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->unk60, 0x11, (u16 *)&D_801EA874);
            ex->func_800E3D20((s32)D_801EA86C, (s32)&D_801EA874);
        }
        if (D_801E8D3C[3] != 0) {
            tr = D_801EA8FC;
            p = tr->unk4;
            tr->unk0 = tr->unk2 = 0x14;
            tr->unk1 = 0;
            tr2 = tr + 1;
            tr2->unk0 = tr2->unk2 = 0x14;
            tr2->unk1 = 0;
            do {
                /* empty: the original has a block boundary here (an emptied macro?) */
            } while (0);
            q = tr[1].unk4;
            if (D_801E8D3C[2] != 0) {
                D_801E8D3C[2]--;
                y = D_801EA89C.vy - func_801E792C(1, 8, D_801E8D3C[2], 0, 0x1800);
            } else {
                y = D_801EA89C.vy - 0x1800;
            }
            if (D_801E8D3C[4] != 0) {
                D_801E8D3C[4]--;
                if (D_801E8D3C[4] == 0) {
                    D_801E8D3C[3] = 0;
                }
                c = D_801E8D3C[4] << 4;
                a = D_801E8D3C[4] << 10;
            } else {
                c = 0x40;
                a = 0x1000;
            }
            for (i = 0; i < n; i++) {
                p->vy = q->vy = ex->btlfx_lerpRatio(D_801EA89C.vy, y, i, 0x15);
                p->vx = q->vx = D_801EA89C.vx;
                p->vz = q->vz = D_801EA89C.vz;
                p->pad = c;
                q->pad = c * 2;
                p++;
                q++;
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0, NULL, frame, 0x1000, a, 1, (u8 *)&D_801EA8FC[0], 0);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0, NULL, frame, 0x1000, a, 1, (u8 *)&D_801EA8FC[1], 0);
        }
        if (D_801E8D3C[5] != 0) {
            D_801E8D3C[5]--;
            if (D_801E8D3C[5] >= 9) {
                t = (0x10 - D_801E8D3C[5]) << 4;
            } else {
                t = D_801E8D3C[5] << 4;
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

s32 fx_0b1_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 c;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801EAA54 = 0;
        D_801E8B8C = env;
        ex->btlfx_initEffModels(D_801EAAEC, 4);
        for (i = 0; i < env->targets->unk24; i++) {
            D_801EAA5C[i] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x23), 0x177,
                                                       0xFFFF, 0, 0x80);
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801EAACC[i]);
            D_801EAA8C[i].vx = D_801EAACC[i].vx;
            D_801EAA8C[i].vy = D_801EAACC[i].vy;
            D_801EAA8C[i].vz = D_801EAACC[i].vz + 0x100;
            D_801EAACC[i].vz += 0x200;
        }
        D_801EAA6C.vx = 0;
        D_801EAA6C.vy = 0x800;
        D_801EAA6C.vz = 0;
        D_801EAA7C.vx = 0x1800;
        D_801EAA7C.vy = 0x1800;
        D_801EAA7C.vz = 0x1800;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            do {
                D_801EAA54 = 0x20;
            } while (0);
            for (i = 0; i < env->targets->unk24; i++) {
                ex->func_800E3CEC((s32)ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&D_801EAACC[i]);
                ex->func_800E3CEC((s32)ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&D_801EAACC[i]);
            }
        }
        if (D_801EAA54 != 0) {
            D_801EAA54--;
            do {
                for (i = 0; i < env->targets->unk24; i++) {
                    ex->Hi_DrawEffModel(&D_801EAA6C, &D_801EAA8C[i], (s32)&D_801EAA7C, D_801EAA5C[i]);
                    if (D_801EAA54 >= 0x19) {
                        c = (0x18 - D_801EAA54) * 0x10;
                        ex->Hi_ModifyEffModelRGB(D_801EAA5C[i], c, c, c);
                    }
                    if (D_801EAA54 < 8) {
                        c = (D_801EAA54 - 8) * 0x10;
                        ex->Hi_ModifyEffModelRGB(D_801EAA5C[i], c, c, c);
                    }
                }
            } while (0);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x22), frame);
        }
        return frame >= 0x20;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b1/nonmatchings/fx_0b1", fx_0b1_headerPtr);
