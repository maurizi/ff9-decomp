#include "fxmod/fx_1ed.h"

void func_801E7704(void) {
    s32 i;
    s32 m;

    D_801E8914 = 0;
    m = -1;
    for (i = 15; i >= 0; i--) {
        D_801E8920[i].unk4 = m;
    }
}

void func_801E7738(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (D_801E8920[i].unk4 == -1) {
            D_801E8920[i].unk4 = 0;
            D_801E8920[i].unk10 = data + 1;
            D_801E8920[i].unk0 = start;
            D_801E8920[i].unk8 = dst;
            D_801E8920[i].unkC = src;
            return;
        }
    }
}

void func_801E7788(void) {
    SVECTOR *ext = D_801E8A60;
    s32 i;
    Unk800F78A4 *env = D_801E8910;
    s32 frame = D_801E8914;
    s32 k;
    s32 d;
    s32 s;
    s32 x, y, z;
    s32 j;

    for (i = 0; i < 16; i++) {
        if (D_801E8920[i].unk4 == 0 && D_801E8920[i].unk0 == frame) {
            D_801E8920[i].unk4 = *D_801E8920[i].unk10;
            D_801E8920[i].unk10++;
        }
        if (D_801E8920[i].unk4 > 0) {
            D_801E8920[i].unk4--;
            for (k = 0; k < 3; k++) {
                d = ((D_801E8920[i].unk8 >> (k * 4)) & 0xF) - 1;
                s = ((D_801E8920[i].unkC >> (k * 4)) & 0xF) - 1;
                if (d >= 0) {
                    x = *D_801E8920[i].unk10;
                    D_801E8920[i].unk10++;
                    y = *D_801E8920[i].unk10;
                    D_801E8920[i].unk10++;
                    z = *D_801E8920[i].unk10;
                    D_801E8920[i].unk10++;
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
    D_801E8914++;
}

s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8910);
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

s32 fx_1ed_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR *p;
    Pool6CEntry *e;
    Pool6CEntry *e2;
    Pool6CEntry *e3;
    SVECTOR *q;
    Fx1edTrail *tr;
    Fx1edTrail *tr2;
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
        D_801E8910 = env;
        for (i = 7; i >= 0; i--) {
            D_801E8AC0[i] = 0;
        }
        D_801E8AE0 = NULL;
        D_801E8AF0 = NULL;
        D_801E8B00 = NULL;
        D_801EA5F0 = NULL;
        D_801EA608 = NULL;
        D_801EA618 = NULL;
        D_801EA628 = NULL;
        D_801EA638 = NULL;
        D_801EA648 = NULL;
        D_801EA668 = NULL;
        D_801EA658 = NULL;
        D_801EA660.vx = 0;
        D_801EA660.vy = 0;
        D_801EA660.vz = -0x618;
        D_801E8A88.vx = 0;
        D_801E8A88.vy = 0;
        D_801E8A88.vz = 0;
        D_801E8A90.vx = 0;
        D_801E8A90.vy = 0;
        D_801E8A90.vz = 0;
        D_801E8AA0.vx = 0x1000;
        D_801E8AA0.vy = 0x1000;
        D_801E8AA0.vz = 0x1000;
        D_801E8AB0.vx = 0;
        D_801E8AB0.vy = 0;
        D_801E8AB0.vz = -0x2EE;
        func_801E7704();
        func_801E7738(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 3));
        func_801E7738(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 4));
        func_801E7738(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 5));
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&D_801EA680[0], 0);
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&D_801EA680[1], 0);
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 1:
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x19), (s32)&D_801E8AB0);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1A), (s32)&D_801E8AB0);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1B), (s32)&D_801E8AB0);
            D_801E8AC0[5] = 0x10;
            break;
        case 9:
            ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk180, SUMMON(env)->unk60);
            D_801E8AC0[0] = 1;
            D_801E8AE0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&D_801E8AE8);
            break;
        case 0x18:
            D_801E8AF0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&D_801E8AF8);
            break;
        case 0x40:
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x11, (u16 *)&D_801E8B08);
            D_801E8B00 = e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&D_801E8B08);
            if (e != NULL) {
                e->unk14 = (s32)D_801E8B10;
                e->unk12 = 0x28;
                e->unkC = 0xAC;
                e->unk22 = 0x50;
            }
            break;
        case 0x48:
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x11, (u16 *)&D_801EA600);
            D_801EA5F0 = e2 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&D_801EA600);
            if (e2 != NULL) {
                e2->unk22 = 0x40;
            }
            break;
        case 0x67:
            ex->func_800E3D54(D_801EA5F0);
            break;
        case 0x6C:
            ex->func_800E3D54(D_801E8B00);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0, (u16 *)&D_801EA610);
            D_801EA610.vy -= 0x180;
            D_801EA608 = e3 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&D_801EA610);
            if (e3 != NULL) {
                e3->unk22 = -0x80;
            }
            D_801E8AC0[1] = 0x10;
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x11, (u16 *)&D_801EA620);
            D_801EA618 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&D_801EA620);
            D_801E8AC0[2] = 8;
            D_801E8AC0[3] = 1;
            break;
        case 0x77:
            D_801E8AC0[0] = 0;
            ex->func_800E3D54(D_801E8AE0);
            ex->func_800E3D54(D_801E8AF0);
            D_801EA630.vy = D_801EA620.vy - 0x1800;
            D_801EA630.vx = D_801EA620.vx;
            D_801EA630.vz = D_801EA620.vz;
            D_801EA628 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&D_801EA630);
            break;
        case 0x97:
            ex->func_800E3D54(D_801EA608);
            ex->func_800E3D54(D_801EA618);
            ex->func_800E3D54(D_801EA628);
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xB0, 0xB0, 0xB0);
            D_801EA640.vy = D_801EA630.vy;
            D_801EA640.vx = D_801EA630.vx;
            D_801EA640.vz = D_801EA630.vz;
            D_801EA638 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&D_801EA640);
            break;
        case 0x99:
            D_801E8AC0[4] = 4;
            D_801EA650.vx = D_801EA670.vx = D_801EA630.vx;
            D_801EA650.vy = D_801EA670.vy = D_801EA630.vy;
            D_801EA650.vz = D_801EA670.vz = D_801EA630.vz;
            D_801EA648 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&D_801EA650);
            break;
        case 0x9F:
            D_801EA668 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&D_801EA670);
            break;
        case 0xAE:
            D_801EA658 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&D_801EA660);
            break;
        case 0xCA:
            ex->func_800E3D54(D_801EA668);
            break;
        }
        if (D_801E8AC0[0] != 0) {
            D_801E8A90.vx = env->camera->unk0[2].vx;
            D_801E8A90.vy = env->camera->unk0[2].vy;
            D_801E8A90.vz = env->camera->unk0[2].vz;
            ex->Hi_DrawSummonModel(&D_801E8A88, &D_801E8A90, (s32)&D_801E8AA0, SUMMON(env)->unk60, 1);
            if (D_801E8AC0[1] != 0) {
                D_801E8AC0[1]--;
                ex->Hi_ModifySummonModelRGB(SUMMON(env)->unk60, -0x40, -0x40, -0x40);
            }
        }
        if (D_801E8AE0 != NULL) {
            if (D_801E8AE0->unk30 == -1) {
                D_801E8AE0 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0, (u16 *)&D_801E8AE8);
        }
        if (D_801E8AF0 != NULL) {
            if (D_801E8AF0->unk30 == -1) {
                D_801E8AF0 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0, (u16 *)&D_801E8AF8);
        }
        if (D_801EA5F0 != NULL) {
            if (D_801EA5F0->unk30 == -1) {
                D_801EA5F0 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x11, (u16 *)&D_801EA5F8);
            ex->func_800E3D20((s32)D_801EA5F0, (s32)&D_801EA5F8);
        }
        if (D_801E8AC0[3] != 0) {
            tr = D_801EA680;
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
            if (D_801E8AC0[2] != 0) {
                D_801E8AC0[2]--;
                y = D_801EA620.vy - func_801E792C(1, 8, D_801E8AC0[2], 0, 0x1800);
            } else {
                y = D_801EA620.vy - 0x1800;
            }
            if (D_801E8AC0[4] != 0) {
                D_801E8AC0[4]--;
                if (D_801E8AC0[4] == 0) {
                    D_801E8AC0[3] = 0;
                }
                c = D_801E8AC0[4] << 4;
                a = D_801E8AC0[4] << 10;
            } else {
                c = 0x40;
                a = 0x1000;
            }
            for (i = 0; i < n; i++) {
                p->vy = q->vy = ex->btlfx_lerpRatio(D_801EA620.vy, y, i, 0x15);
                p->vx = q->vx = D_801EA620.vx;
                p->vz = q->vz = D_801EA620.vz;
                p->pad = c;
                q->pad = c * 2;
                p++;
                q++;
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), 0, NULL, frame, 0x1000, a, 1, (u8 *)&D_801EA680[0], 0);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), 0, NULL, frame, 0x1000, a, 1, (u8 *)&D_801EA680[1], 0);
        }
        if (D_801E8AC0[5] != 0) {
            D_801E8AC0[5]--;
            if (D_801E8AC0[5] >= 9) {
                t = (0x10 - D_801E8AC0[5]) << 4;
            } else {
                t = D_801E8AC0[5] << 4;
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

s32 fx_1ed_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 i;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E8910 = env;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801EA7E0[i]);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&D_801EA7E0[i]);
        }
    } else {
        return entry->unk0 >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ed/nonmatchings/fx_1ed", fx_1ed_headerPtr);
