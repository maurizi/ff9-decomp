#include "fxmod/fx_083.h"

INCLUDE_RODATA("asm/us/fx_083/nonmatchings/fx_083", fx_083_headerPtr);

s32 fx_083_op80(s32 mode, Fx083Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s16 tex[8];
    Pool6CEntry *e;
    s32 frame;
    s32 i;
    s32 r;
    s32 n;
    s32 t;

    if (mode == 0) {
        *arg4 = sizeof(Fx083Work);
    } else if (mode == 1) {
        D_801E83B0 = env;
        ex->btlfx_initEffModels(D_801E83C0, 0x14);
        for (i = 9; i >= 0; i--) {
            work->unk0[i] = 0;
        }
        tex[0] = tex[2] = 0xF8;
        tex[1] = 0x3DC0;
        tex[3] = 0x3D80;
        tex[4] = 0;
        tex[6] = 0;
        tex[5] = 0x80;
        tex[7] = 0;
        for (i = 0; i < 4; i++) {
            work->unk28[i] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 5), 0xFA, 0x3DC0,
                                                        0x80, 0x80);
            work->unk38[i].vy = i << 10;
            work->unk38[i].vx = 0x100;
            work->unk38[i].vz = 0;
            ex->btlfx_getUnitPos(0, (s16 *)&work->unk180[i]);
        }
        work->unk98.vx = 0x333;
        work->unk98.vy = 0x333;
        work->unk98.vz = 0x333;
        for (i = 0; i < 5; i++) {
            work->unkB0[i] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 1), 0xB8, 0x3D40,
                                                        0xC0, 0);
            work->unkC4[i] = 0;
            work->unkD8[i] = -1;
        }
        work->unk168 = ex->btlfx_getUnitRadius(0);
        work->unk16C = ex->btlfx_getUnitHeight(0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk160);
        work->unk160.vy = -0x80;
        work->unkC4[0] = 1;
        work->unk140.vx = 0;
        work->unk140.vy = 0;
        work->unk140.vz = 0;
        work->unk13C = 0;
        work->unk1C0 = 0;
        work->unk158.vx = work->unk160.vx;
        work->unk158.vy = work->unk160.vy - work->unk16C;
        work->unk158.vz = work->unk160.vz;
        work->unk148.vx = work->unk168;
        work->unk148.vz = work->unk168;
        work->unk168 += 0x100 + (work->unk168 >> 2);
        /* Store order AC, vy, vx, vz: sets the local-alloc lifetimes so vx takes a3 (original register choice). */
        work->unkAC = work->unk158.vy;
        work->unkEC[0].vy = work->unk158.vy;
        work->unkEC[0].vx = work->unk158.vx;
        work->unkEC[0].vz = work->unk158.vz;
        work->unk148.vy = work->unk16C / 3;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x0:
            work->unk0[2] = 0x50;
            work->unk0[0] = 0x10;
            break;
        case 0x8:
            work->unk0[6] = 0x3C;
            work->unk13C = 0x10;
            break;
        case 0x10:
            work->unk0[4] = 8;
            break;
        case 0x14:
            work->unk0[3] = 0x20;
            work->unk0[7] = 0x20;
            work->unk170[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk180[0]);
            work->unk170[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk180[1]);
            work->unk170[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk180[2]);
            work->unk170[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk180[3]);
            work->unk0[8] = 0x30;
            break;
        case 0x30:
            e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk160);
            if (e != NULL) {
                e->unk22 = ex->func_800E7CC4(0, 0x100);
            }
            e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk160);
            if (e != NULL) {
                e->unk22 = ex->func_800E7CC4(0, 0x100);
            }
            e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk160);
            if (e != NULL) {
                e->unk22 = ex->func_800E7CC4(0, 0x100);
            }
            break;
        case 0x34:
            work->unk0[1] = 0x10;
            break;
        case 0x37:
            work->unkD8[4] = 8;
            break;
        case 0x38:
            work->unkD8[3] = 8;
            break;
        case 0x39:
            work->unkD8[2] = 8;
            break;
        case 0x3A:
            work->unkD8[1] = 8;
            break;
        case 0x3B:
            work->unkD8[0] = 8;
            break;
        }
        for (i = 0; i < 4; i++) {
            work->unk38[i].vy += 0x40;
        }
        work->unk1C0 += 0x10;
        if (work->unk0[3] != 0) {
            work->unk0[3]--;
            work->unkAC = ex->btlfx_lerp(work->unk160.vy, work->unk158.vy, work->unk0[3] << 7);
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            work->unkA8 = 0x1000 - ((ex->math_sin((0x10 - work->unk0[0]) << 6) << 12) >> 12);
        }
        if (work->unk0[1] != 0) {
            work->unk0[1]--;
            work->unkA8 = 0x1000 - ((ex->math_sin(work->unk0[1] << 6) << 12) >> 12);
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            for (i = 0; i < 4; i++) {
                work->unk58[i].vx =
                    work->unk158.vx + ((ex->math_sin(work->unk38[i].vy) * (work->unk168 + work->unkA8)) >> 12);
                work->unk58[i].vz =
                    work->unk158.vz + ((ex->math_cos(work->unk38[i].vy) * (work->unk168 + work->unkA8)) >> 12);
                work->unk58[i].vy = work->unkAC - ((ex->math_sin(0x200) * work->unkA8) >> 12);
                ex->Hi_DrawEffModel(&work->unk38[i], &work->unk58[i], (s32)&work->unk98, work->unk28[i]);
            }
            env->camera->points[0].vx = work->unk58[0].vx;
            env->camera->points[0].vy = work->unk58[0].vy;
            env->camera->points[0].vz = work->unk58[0].vz;
            env->camera->unk30[0].vx = 0x100;
            env->camera->unk30[0].vy = work->unk38[0].vy;
            env->camera->unk30[0].vz = 0;
        }
        if (work->unk0[8] != 0) {
            work->unk0[8]--;
            for (i = 0; i < 4; i++) {
                t = i << 10;
                work->unk1A0[i].vx = work->unk158.vx + ((ex->math_sin(t + work->unk1C0) *
                                                         (work->unk168 + work->unkA8 + 0x200)) >> 12);
                work->unk1A0[i].vz = work->unk158.vz + ((ex->math_cos(t + work->unk1C0) *
                                                         (work->unk168 + work->unkA8 + 0x200)) >> 12);
                work->unk1A0[i].vy = work->unk58[0].vy;
                if (work->unk170[i] != NULL && work->unk170[i]->unk30 == -1) {
                    work->unk170[i] = NULL;
                }
                ex->func_800E3D20((s32)work->unk170[i], (s32)&work->unk1A0[i]);
            }
        }
        if (work->unk0[4] != 0) {
            work->unk0[4]--;
            ex->func_800E6624(0, 0x2C0, 0x180, 0x40, 0x40, D_801E8328[work->unk0[4] >> 1].unk0,
                              D_801E8328[work->unk0[4] >> 1].unk4);
        }
        if (work->unk0[5] != 0) {
            work->unk0[5]--;
            ex->func_800E6624(0, 0x2C0, 0x180, 0x40, 0x40, D_801E8348[work->unk0[5]].unk0, D_801E8348[work->unk0[5]].unk4);
        } else if (frame >= 0x15 && !(ex->btlfx_rand() & 7)) {
            work->unk0[5] = 4;
        }
        if (work->unk0[7] != 0) {
            work->unkEC[0].vy = work->unkAC;
            if (!(work->unk0[7] & 7) && (n = work->unk0[7] >> 3) > 0) {
                work->unkC4[n] = 1;
                work->unkEC[n].vx = work->unkEC[0].vx;
                work->unkEC[n].vy = work->unkEC[0].vy;
                work->unkEC[n].vz = work->unkEC[0].vz;
            }
            work->unk0[7]--;
        }
        if (work->unk0[6] != 0) {
            work->unk0[6]--;
            ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->unk158, (s16 *)&work->unk140);
            work->unk140.vx = 0;
            work->unk140.vz = 0;
            for (i = 0; i < 5; i++) {
                if (work->unkC4[i] != 0) {
                    ex->Hi_DrawEffModel(&work->unk140, &work->unkEC[i], (s32)&work->unk148, work->unkB0[i]);
                    if (work->unkD8[i] > 0) {
                        work->unkD8[i]--;
                        r = (work->unkD8[i] << 5) - 0x100;
                        ex->Hi_ModifyEffModelRGB(work->unkB0[i], r, r, r);
                        if (work->unkD8[i] == 0) {
                            work->unkC4[i] = 0;
                        }
                    }
                    if (i == 0 && work->unk13C != 0) {
                        work->unk13C--;
                        r = -(work->unk13C * 0x10);
                        ex->Hi_ModifyEffModelRGB(work->unkB0[0], r, r, r);
                    }
                }
            }
        }
        if (frame >= 0x5C) {
            return 1;
        }
        ex->func_800E6624(0x2710 - frame * 4, 0x260, 0x100, 0x20, 0xF0, 0x240, 0x100);
    }
    return 0;
}
