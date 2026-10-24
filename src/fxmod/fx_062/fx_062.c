#include "fxmod/fx_062.h"

s32 func_801E77F0(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E882C);
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

INCLUDE_RODATA("asm/us/fx_062/nonmatchings/fx_062", fx_062_headerPtr);

s32 fx_062_op80(s32 mode, Fx062Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    VECTOR scale;
    SVECTOR ang;
    SVECTOR cur;
    VECTOR b;
    s32 frame;
    u8 *strip;
    s32 n;
    Fx062Strip *tr;
    Fx062Pt *pt;
    s32 i;
    s32 j;
    s32 r;
    s32 s;
    s32 t;
    s32 u;
    s32 k;

    if (mode == 0) {
        *arg4 = 0xD4;
    } else if (mode == 1) {
        D_801E882C = env;
        ex->btlfx_initEffModels(D_801E883C, 4);
        for (i = 15; i >= 0; i--) {
            work->unk0[i] = 0;
        }
        for (i = 0; i < 5; i++) {
            D_801E8F7C[i].unk118 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xD), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&D_801E8F7C[i], 0);
        }
        ex->btlfx_getUnitHomePos(0, (s16 *)&D_801E8F64);
        ex->btlfx_getUnitScale(0, &D_801E8F6C);
        D_801E8F78 = ex->btlfx_getUnitHeight(0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&D_801E8F5C);
        env->camera->points[0].vx = work->unk94.vx = D_801E8F5C.vx;
        env->camera->points[0].vz = work->unk94.vz = D_801E8F5C.vz;
        work->unk40[0].unk0 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 8), 0x137,
                                                         0xFFFF, 0, 0x80);
        work->unk40[0].unk4.vx = 0x800;
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
        work->unk40[2].unkC.vy = 0;
        work->unk40[2].unkC.vz = 0;
        work->unkA4 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xA), 0x17A, 0xFFFF, 0,
                                                 0x80);
        work->unkB0.vx = D_801E8F64.vx;
        work->unkB0.vy = 0;
        work->unkB0.vz = D_801E8F64.vz;
        work->unkA8.vx = 0;
        work->unkA8.vy = 0;
        work->unkA8.vz = 0;
        work->unkC0.vx = 0x249;
        work->unkC0.vy = 0;
        work->unkC0.vz = 0x249;
        ex->Hi_SetEffModelOffset(work->unkA4, 0x20);
        return 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x280, 0x100, 0x70, 0x70, D_801E8B5C, D_801E8F64.vx, D_801E8F64.vz, -0x1000);
            break;
        case 1:
            work->unk0[1] = 1;
            work->unk0[2] = 0x10;
            work->unk0[0] = 0x30;
            work->unk0[7] = 0x10;
            break;
        case 9:
            work->unk0[4] = 1;
            work->unk0[5] = 8;
            break;
        case 0x11:
            work->unk0[9] = 0x19;
            break;
        case 0x2A:
            work->unk0[3] = 0x10;
            work->unk0[8] = 0x10;
            work->unk0[6] = 8;
            break;
        case 0x3A:
            work->unk0[1] = 0;
            work->unk0[4] = 0;
            break;
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            work->unk40[0].unkC.vx = work->unk40[0].unkC.vy = work->unk40[0].unkC.vz =
                func_801E77F0(2, 0x10, work->unk0[2], 0, 0x170);
            work->unk40[1].unkC.vx = work->unk40[1].unkC.vy = work->unk40[1].unkC.vz =
                func_801E77F0(2, 0x10, work->unk0[2], 0, 0x143);
            work->unk40[2].unkC.vx = work->unk40[2].unkC.vy = work->unk40[2].unkC.vz =
                func_801E77F0(2, 0x10, work->unk0[2], 0, 0x199);
        }
        if (work->unk0[3] != 0) {
            work->unk0[3]--;
            work->unk40[0].unkC.vx = work->unk40[0].unkC.vy = work->unk40[0].unkC.vz =
                func_801E77F0(2, 0x10, work->unk0[3], 0x170, 0);
            work->unk40[1].unkC.vx = work->unk40[1].unkC.vy = work->unk40[1].unkC.vz =
                func_801E77F0(2, 0x10, work->unk0[3], 0x143, 0);
            work->unk40[2].unkC.vx = work->unk40[2].unkC.vy = work->unk40[2].unkC.vz =
                func_801E77F0(2, 0x10, work->unk0[3], 0x199, 0);
        }
        if (work->unk0[5] != 0) {
            work->unk0[5]--;
            work->unkC0.vy += 0x40;
        }
        if (work->unk0[6] != 0) {
            work->unk0[6]--;
            work->unkC0.vy -= 0x40;
        }
        if (work->unk0[7] != 0) {
            work->unk0[7]--;
            if (D_801E8F78 > 0x200) {
                scale.vx = scale.vy = scale.vz =
                    func_801E77F0(2, 0x10, work->unk0[7], D_801E8F6C.vy, D_801E8F6C.vy >> 1);
                ex->btlfx_setUnitScale(0, (s32 *)&scale);
            }
            pos.vx = D_801E8F64.vx;
            pos.vz = D_801E8F64.vz;
            pos.vy = D_801E8F64.vy + func_801E77F0(2, 0x10, work->unk0[7], 0, -0x200) +
                ((ex->math_sin(frame << 8) * (0x10 - work->unk0[7])) >> 10);
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&D_801E8F5C);
        }
        if (work->unk0[9] != 0) {
            work->unk0[9]--;
            pos.vx = D_801E8F64.vx;
            pos.vz = D_801E8F64.vz;
            k = ((ex->math_sin(frame << 8) * 16) >> 10) - 0x200;
            pos.vy = D_801E8F64.vy + k;
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        if (work->unk0[8] != 0) {
            if (--work->unk0[8] != 0) {
                if (D_801E8F78 > 0x200) {
                    scale.vx = scale.vy = scale.vz =
                        func_801E77F0(2, 0x10, work->unk0[8], D_801E8F6C.vy >> 1, D_801E8F6C.vy);
                    ex->btlfx_setUnitScale(0, (s32 *)&scale);
                }
                pos.vy = D_801E8F64.vy + func_801E77F0(2, 0x10, work->unk0[8], -0x200, 0) +
                    ((ex->math_sin(frame << 8) * work->unk0[8]) >> 10);
                pos.vx = D_801E8F64.vx;
                pos.vz = D_801E8F64.vz;
            } else {
                ex->btlfx_getUnitHomePos(0, (s16 *)&pos);
                ex->btlfx_setUnitScale(0, (s32 *)&D_801E8F6C);
            }
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&D_801E8F5C);
        }
        env->camera->points[0].vy = work->unk94.vy = D_801E8F5C.vy;
        if (work->unk0[1] != 0) {
            ex->btlfx_aimAngles((s16 *)&D_801E8F5C, (s16 *)&env->camera->eye, (s16 *)&work->unk40[1].unk4);
            work->unk40[0].unk4.vx = work->unk40[1].unk4.vx + 0x800;
            work->unk40[0].unk4.vy = work->unk40[1].unk4.vy;
            work->unk40[2].unk4.vy = work->unk40[1].unk4.vy;
            work->unk40[2].unk4.vx = work->unk40[1].unk4.vx + 0x400;
            work->unk40[1].unk4.vx -= 0x400;
            ex->Hi_DrawEffModel(&work->unk40[1].unk4, &work->unk94, (s32)&work->unk40[1].unkC, work->unk40[1].unk0);
            ex->Hi_DrawEffModel(&work->unk40[2].unk4, &work->unk94, (s32)&work->unk40[2].unkC, work->unk40[2].unk0);
            ex->Hi_DrawEffModel(&work->unk40[0].unk4, &work->unk94, (s32)&work->unk40[0].unkC, work->unk40[0].unk0);
            ex->Hi_ModifyEffModelAbr(work->unk40[0].unk0, 1);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 7), frame);
        }
        if (work->unk0[4] != 0) {
            ex->Hi_DrawEffModel(&work->unkA8, &work->unkB0, (s32)&work->unkC0, work->unkA4);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 9), frame);
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            for (n = 0; n < 5; n++) {
                tr = &D_801E8F7C[n];
                if (tr->unk118 != 0) {
                    tr->unk118--;
                    tr->unk0 = tr->unk2 = 10;
                    tr->unk1 = 0;
                    s = work->unk40[0].unkC.vx;
                    tr->unk104 += tr->unkB4;
                    tr->unk108 += tr->unkB8;
                    pt = tr->unk4;
                    r = (ex->math_sin(tr->unk104) * s) >> 12;
                    tr->unk10C = (ex->math_cos(tr->unk108) * r) >> 12;
                    tr->unk110 = (ex->math_sin(tr->unk108) * r) >> 12;
                    pt->vy = tr->unk5C.vy = work->unk94.vy + ((ex->math_cos(tr->unk104) * s) >> 12);
                    strip = (u8 *)tr;
                    pt->vx = tr->unk5C.vx = work->unk94.vx + tr->unk10C;
                    pt->vz = tr->unk5C.vz = work->unk94.vz + tr->unk110;
                    pt->unk6 = tr->unk114;
                    tr->unk64.vy = 0;
                    tr->unk64.vx = work->unk94.vx + (tr->unk10C >> 4);
                    tr->unk64.vz = work->unk94.vz + (tr->unk110 >> 4);
                    ex->btlfx_aimAngles((s16 *)&D_801E8F5C, (s16 *)&tr->unk5C, (s16 *)&ang);
                    pt++;
                    for (j = 0; j < 9; j++, pt++) {
                        t = ((j + 1) * 0x2E8BA) >> 12;
                        u = 0x200 - t;
                        b.vx = (u * u) >> 6;
                        b.vy = (t * u + u * 4) >> 6;
                        b.vz = (t * t) >> 6;
                        tr->unk6C[j].vx += tr->unkBC[j];
                        tr->unk6C[j].vz += tr->unkE0[j];
                        cur.vx = tr->unk6C[j].vx +
                            ((tr->unk5C.vx * b.vx + tr->unk64.vx * b.vz +
                              (work->unk94.vx + ((ex->math_sin(ang.vy) * -tr->unk5C.vy) >> 10) * b.vy)) >> 12);
                        pt->vy = (tr->unk5C.vy * b.vx + tr->unk64.vy * b.vz + (work->unk94.vy >> 1) * b.vy) >> 12;
                        cur.vz = tr->unk6C[j].vz +
                            ((tr->unk5C.vz * b.vx + tr->unk64.vz * b.vz +
                              (work->unk94.vz + ((ex->math_cos(ang.vy) * -tr->unk5C.vy) >> 10) * b.vy)) >> 12);
                        pt->vx = cur.vx + tr->unk6C[j].vx;
                        pt->vz = cur.vz + tr->unk6C[j].vz;
                        pt->unk6 = tr->unk114;
                    }
                    pt->vx = tr->unk64.vx;
                    pt->vy = tr->unk64.vy;
                    pt->vz = tr->unk64.vz;
                    pt->unk6 = tr->unk114;
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xD), 0, NULL, frame, 0x1000,
                                        tr->unk118 << 9, 1, strip, 0);
                } else if (work->unk0[0] >= 9) {
                    tr->unk118 = ex->btlfx_randRange(2, 8);
                    tr->unk104 = ex->btlfx_randRange(0x280, 0x600);
                    tr->unk108 = ex->btlfx_rand() & 0xFFF;
                    tr->unkB4 = ex->btlfx_randRange(-0x60, 0x60);
                    tr->unkB8 = ex->btlfx_randRange(-0x60, 0x60);
                    tr->unk114 = ex->btlfx_randRange(8, 0x20);
                    for (j = 0; j < 9; j++) {
                        tr->unk6C[j].vz = 0;
                        tr->unk6C[j].vx = 0;
                        tr->unkBC[j] = ex->btlfx_randRange(-0x20, 0x20);
                        tr->unkE0[j] = ex->btlfx_randRange(-0x20, 0x20);
                    }
                }
            }
        }
        return frame >= 0x46;
    }
    return 0;
}
