#include "fxmod/fx_063.h"

s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8710);
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

s32 fx_063_op80(s32 mode, Fx063Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR home;
    SVECTOR ang;
    SVECTOR start;
    VECTOR b;
    s32 frame;
    u8 *strip;
    s32 n;
    Fx063Strip *tr;
    Fx063Pt *pt;
    s32 i;
    s32 j;
    s32 r;
    s32 s;
    s32 t;
    s32 u;
    s32 k;
    s32 c;

    if (mode == 0) {
        *arg4 = 0xDC;
    } else if (mode == 1) {
        D_801E8710 = env;
        ex->btlfx_initEffModels(D_801E8720, 4);
        for (i = 15; i >= 0; i--) {
            work->unk0[i] = 0;
        }
        for (i = 0; i < 10; i++) {
            D_801E8E40[i].unk100 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xD), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&D_801E8E40[i], 0);
        }
        if (env->targets->centerZ < 0) {
            env->camera->points[0].vz = work->unkB8.vz = work->unk94.vz = work->unkA4.vz = -0x30C;
        } else {
            env->camera->points[0].vz = work->unkB8.vz = work->unk94.vz = work->unkA4.vz = 0x30C;
        }
        env->camera->points[0].vx = work->unkB8.vx = work->unk94.vx = work->unkA4.vx = 0;
        env->camera->points[0].vy = work->unk94.vy = work->unkA4.vy = -0x600;
        work->unkB8.vy = 0;
        for (i = 0; i < env->targets->targetCount; i++) {
            D_801E9870[i].unk0 = (ex->btlfx_rand() & 7) + 0x30;
            D_801E9870[i].unk4 = 0;
            D_801E9870[i].unk8 = ex->btlfx_rand() % 511 + 0x100;
            ex->btlfx_getUnitHomePos(i, (s16 *)&D_801E9870[i].unkC);
            ex->btlfx_getUnitHomePos(i, (s16 *)&D_801E9870[i].unk14);
        }
        work->unk40[0].unk0 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 8), 0x137,
                                                         0xFFFF, 0, 0x80);
        work->unk40[0].unk4.vx = 0;
        work->unk40[0].unk4.vy = 0;
        work->unk40[0].unk4.vz = 0;
        work->unk40[0].unkC.vx = 0;
        work->unk40[0].unkC.vy = 0;
        work->unk40[0].unkC.vz = 0;
        work->unk40[1].unk0 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xB));
        work->unk40[1].unk4.vx = 0;
        work->unk40[1].unk4.vy = 0;
        work->unk40[1].unk4.vz = 0;
        work->unk40[1].unkC.vx = 0;
        work->unk40[1].unkC.vy = 0;
        work->unk40[1].unkC.vz = 0;
        work->unk40[2].unk0 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xC));
        work->unk40[2].unk4.vx = 0;
        work->unk40[2].unk4.vy = 0;
        work->unk40[2].unk4.vz = 0;
        work->unk40[2].unkC.vx = 0;
        work->unk40[2].unkC.vy = 0x100;
        work->unk40[2].unkC.vz = 0;
        work->unkAC = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 9), 0x17A, 0xFFFF, 0,
                                                 0x80);
        work->unkB0.vx = 0;
        work->unkB0.vy = 0;
        work->unkB0.vz = 0;
        work->unkC8.vx = 0x400;
        work->unkC8.vy = 0;
        work->unkC8.vz = 0x400;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x280, 0x100, 0x70, 0x70, D_801E8A40, work->unkB8.vx, work->unkB8.vz, 0x200);
            work->unk0[9] = 1;
            work->unk0[10] = 1;
            break;
        case 1:
            work->unk0[1] = 1;
            work->unk0[7] = 1;
            work->unk0[4] = 1;
            work->unk0[2] = 0x10;
            work->unk0[0] = 0x30;
            work->unk0[5] = 0x10;
            break;
        case 42:
            work->unk0[3] = 8;
            work->unk0[6] = 8;
            break;
        case 49:
            work->unk0[1] = 0;
            break;
        case 51:
            work->unk0[4] = 0;
            break;
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            work->unk40[0].unkC.vx = work->unk40[0].unkC.vy = work->unk40[0].unkC.vz =
                func_801E7704(2, 0x10, work->unk0[2], 0, 0x400);
            work->unk40[1].unkC.vx = work->unk40[1].unkC.vy = work->unk40[1].unkC.vz =
                func_801E7704(2, 0x10, work->unk0[2], 0, 0x300);
            work->unk40[2].unkC.vx = work->unk40[2].unkC.vz = func_801E7704(2, 0x10, work->unk0[2], 0, 0x480);
        }
        if (work->unk0[3] != 0) {
            work->unk0[3]--;
            work->unk40[0].unkC.vx = work->unk40[0].unkC.vy = work->unk40[0].unkC.vz =
                func_801E7704(2, 8, work->unk0[3], 0x400, 0x200);
            work->unk40[1].unkC.vx = work->unk40[1].unkC.vy = work->unk40[1].unkC.vz =
                func_801E7704(2, 8, work->unk0[3], 0x300, 0x200);
            work->unk40[2].unkC.vx = work->unk40[2].unkC.vz = func_801E7704(2, 8, work->unk0[3], 0x480, 0x200);
        }
        if (work->unk0[5] != 0) {
            work->unk0[5]--;
            work->unkC8.vy += 0x40;
        }
        if (work->unk0[6] != 0) {
            work->unk0[6]--;
            work->unkC8.vy -= 0x80;
        }
        if (work->unk0[1] != 0) {
            ex->btlfx_aimAngles((s16 *)&work->unkA4, (s16 *)&env->camera->eye, (s16 *)&work->unk40[1].unk4);
            work->unk40[0].unk4.vx = work->unk40[1].unk4.vx + 0x800;
            work->unk40[0].unk4.vy = work->unk40[1].unk4.vy;
            work->unk40[2].unk4.vy = work->unk40[1].unk4.vy;
            work->unk40[2].unk4.vx = work->unk40[1].unk4.vx + 0x400;
            work->unk40[1].unk4.vx -= 0x400;
            ex->Hi_DrawEffModel(&work->unk40[1].unk4, &work->unk94, (s32)&work->unk40[1].unkC, work->unk40[1].unk0);
            ex->Hi_DrawEffModel(&work->unk40[2].unk4, &work->unk94, (s32)&work->unk40[2].unkC, work->unk40[2].unk0);
            ex->Hi_DrawEffModel(&work->unk40[0].unk4, &work->unk94, (s32)&work->unk40[0].unkC, work->unk40[0].unk0);
            ex->Hi_ModifyEffModelAbr(work->unk40[0].unk0, 1);
        }
        if (work->unk0[3] != 0) {
            c = -((8 - work->unk0[3]) << 5);
            ex->Hi_ModifyEffModelRGB(work->unk40[1].unk0, c, c, c);
            ex->Hi_ModifyEffModelRGB(work->unk40[2].unk0, c, c, c);
            ex->Hi_ModifyEffModelRGB(work->unk40[0].unk0, c, c, c);
        }
        if (work->unk0[4] != 0) {
            ex->Hi_DrawEffModel(&work->unkB0, &work->unkB8, (s32)&work->unkC8, work->unkAC);
        }
        if (work->unk0[7] != 0) {
            for (i = 0; i < env->targets->targetCount; i++) {
                if (D_801E9870[i].unk0 != 0) {
                    if (--D_801E9870[i].unk0 != 0) {
                        D_801E9870[i].unk4 += D_801E9870[i].unk8;
                        if (D_801E9870[i].unk0 < 0x10) {
                            D_801E9870[i].unk14.vy = D_801E9870[i].unkC.vy + func_801E7704(1, 0x10, D_801E9870[i].unk0, -0x80, 0) +
                                ((ex->math_sin(D_801E9870[i].unk4) * D_801E9870[i].unk0) >> 10);
                        }
                        if (D_801E9870[i].unk0 >= 0x10 && D_801E9870[i].unk0 < 0x20) {
                            k = ((ex->math_sin(D_801E9870[i].unk4) * 16) >> 10) - 0x80;
                            D_801E9870[i].unk14.vy = D_801E9870[i].unkC.vy + k;
                        }
                        if (D_801E9870[i].unk0 >= 0x20 && D_801E9870[i].unk0 < 0x30) {
                            D_801E9870[i].unk14.vy = D_801E9870[i].unkC.vy + func_801E7704(2, 0x10, D_801E9870[i].unk0 - 0x20, 0, -0x80) +
                                ((ex->math_sin(D_801E9870[i].unk4) * (0x30 - D_801E9870[i].unk0)) >> 10);
                        }
                        ex->btlfx_setUnitPos(i, (s16 *)&D_801E9870[i].unk14);
                    } else {
                        ex->btlfx_getUnitHomePos(i, (s16 *)&home);
                        ex->btlfx_setUnitPos(i, (s16 *)&home);
                    }
                }
            }
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            for (n = 0; n < 10; n++) {
                tr = &D_801E8E40[n];
                if (tr->unk100 != 0) {
                    tr->unk100--;
                    tr->unk0 = tr->unk2 = 10;
                    tr->unk1 = 0;
                    tr->unkEC += tr->unkF4;
                    tr->unkF0 += tr->unkF8;
                    s = work->unk40[0].unkC.vx;
                    pt = tr->unk4;
                    r = (ex->math_sin(tr->unkEC) * s) >> 12;
                    pt->vy = start.vy = work->unk94.vy + ((ex->math_cos(tr->unkEC) * s) >> 12);
                    pt->vx = start.vx = work->unk94.vx + ((ex->math_cos(tr->unkF0) * r) >> 12);
                    pt->vz = start.vz = work->unk94.vz + ((ex->math_sin(tr->unkF0) * r) >> 12);
                    strip = (u8 *)tr;
                    pt->unk6 = tr->unkFC;
                    ex->btlfx_aimAngles((s16 *)&work->unkA4, (s16 *)&start, (s16 *)&ang);
                    pt++;
                    for (j = 0; j < 9; j++, pt++) {
                        t = ((j + 1) * 0x2E8BA) >> 12;
                        u = 0x200 - t;
                        b.vx = (u * u) >> 6;
                        b.vy = (t * u) >> 4;
                        b.vz = (t * t) >> 6;
                        tr->unk5C[j].vx += tr->unkA4[j];
                        tr->unk5C[j].vz += tr->unkC8[j];
                        pt->vx = tr->unk5C[j].vx +
                            ((start.vx * b.vx + start.vx * b.vz +
                              (work->unk94.vx + ((ex->math_sin(ang.vy) * r) >> 11)) * b.vy) >> 12);
                        pt->vz = tr->unk5C[j].vz +
                            ((start.vz * b.vx + start.vz * b.vz +
                              (work->unk94.vz + ((ex->math_cos(ang.vy) * r) >> 11)) * b.vy) >> 12);
                        pt->vy = ex->btlfx_lerpRatio(start.vy, 0, j + 1, 11);
                        pt->unk6 = tr->unkFC;
                    }
                    pt->vx = start.vx;
                    pt->vz = start.vz;
                    pt->vy = 0;
                    pt->unk6 = tr->unkFC;
                    if (tr->unk100 < 4) {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xD), 0, NULL, frame, 0x1000,
                                            tr->unk100 << 10, 1, strip, 0);
                    } else {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xD), 0, NULL, frame, 0x1000,
                                            0x1000, 1, strip, 0);
                    }
                } else if (work->unk0[0] >= 7) {
                    tr->unk100 = ex->btlfx_randRange(2, 6);
                    tr->unkEC = ex->btlfx_randRange(0x100, 0x400);
                    tr->unkF0 = ex->btlfx_rand() & 0xFFF;
                    tr->unkF4 = ex->btlfx_randRange(-0x10, 0x10);
                    tr->unkF8 = ex->btlfx_randRange(-0x80, 0x80);
                    tr->unkFC = ex->btlfx_randRange(0x40, 0x80);
                    for (j = 0; j < 9; j++) {
                        tr->unk5C[j].vz = 0;
                        tr->unk5C[j].vx = 0;
                        tr->unkA4[j] = ex->btlfx_randRange(-0x60, 0x60);
                        tr->unkC8[j] = ex->btlfx_randRange(-0x60, 0x60);
                    }
                }
            }
        }
        if (work->unk0[9] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 7), work->unk0[9] - 1);
            work->unk0[9]++;
        }
        if (work->unk0[10] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xA), work->unk0[10] - 1);
            work->unk0[10]++;
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_063/nonmatchings/fx_063", fx_063_headerPtr);
