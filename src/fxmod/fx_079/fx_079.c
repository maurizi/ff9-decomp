#include "fxmod/fx_079.h"

s32 func_801E77B8(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E86F4);
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

INCLUDE_RODATA("asm/us/fx_079/nonmatchings/fx_079", fx_079_headerPtr);

s32 fx_079_op80(s32 mode, Fx079Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    SVECTOR pos;
    VECTOR cur;
    VECTOR d;
    Fx079Strip *tr;
    Fx079Pt *pt;
    Pool6CEntry *p;
    s32 i;
    s32 frame;
    s32 j;
    s32 n = 11;
    s32 a;
    s32 b;
    s32 r;
    u8 *strip;
    s32 w1;
    s32 w2;

    if (mode == 0) {
        *arg4 = 0xA8;
    } else if (mode == 1) {
        D_801E86F4 = env;
        D_801E8990 = NULL;
        ex->btlfx_initEffModels(D_801E8704, 3);
        for (i = 15; i >= 0; i--) {
            work->unk0[i] = 0;
        }
        for (i = 0; i < 5; i++) {
            D_801E89A4[i].unk11C = -i;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 9), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&D_801E89A4[i], 0);
        }
        work->unk40[0].unk0 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xB), 0x137,
                                                         0xFFFF, 0, 0);
        work->unk40[0].unk4.vx = 0x800;
        work->unk40[0].unk4.vy = 0;
        work->unk40[0].unk4.vz = 0;
        work->unk40[0].unkC.vx = 0x5C;
        work->unk40[0].unkC.vy = 0x5C;
        work->unk40[0].unkC.vz = 0x5C;
        work->unk40[1].unk0 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xA));
        work->unk40[1].unk4.vx = 0;
        work->unk40[1].unk4.vy = 0;
        work->unk40[1].unk4.vz = 0;
        work->unk40[1].unkC.vx = 0x50;
        work->unk40[1].unkC.vy = 0x50;
        work->unk40[1].unkC.vz = 0x50;
        work->unk40[2].unk0 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 6));
        work->unk40[2].unk4.vx = 0;
        work->unk40[2].unk4.vy = 0;
        work->unk40[2].unk4.vz = 0;
        work->unk40[2].unkC.vx = 0x66;
        work->unk40[2].unkC.vy = 0x66;
        work->unk40[2].unkC.vz = 0x66;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&D_801E8964);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&D_801E8994);
        ex->btlfx_getUnitHomePos(0, (s16 *)&D_801E896C);
        ex->btlfx_getUnitScale(0, &D_801E8974);
        D_801E898C = ex->btlfx_getUnitHeight(0);
        work->unk94.vx = D_801E8984.vx = D_801E8964.vx;
        work->unk94.vy = D_801E8964.vy;
        work->unk94.vz = D_801E8984.vz = D_801E8964.vz;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBonePos(0x10, 0x17, (u16 *)&D_801E895C);
        switch (frame) {
        case 0:
            work->unk0[9] = 1;
            work->unk0[1] = 1;
            work->unk0[0] = 0x24;
            work->unk0[4] = 0x10;
            break;
        case 8:
            work->unk0[6] = 0x10;
            break;
        case 16:
            D_801E8990 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&D_801E8994);
            break;
        case 24:
            work->unk0[8] = 4;
            break;
        case 28:
            work->unk0[5] = 0x10;
            work->unk0[7] = 0x10;
            break;
        case 40:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&D_801E8964);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 44:
            ex->btlfx_drawScreenTint((u32 *)env->ot, 0xFF, 0xFF, 0xFF, 0xFF);
            work->unk0[1] = 0;
            break;
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            work->unk94.vx = D_801E8964.vx;
            work->unk94.vy = D_801E8964.vy + func_801E77B8(0, 8, work->unk0[2], 0, -0x200);
            work->unk94.vz = D_801E8964.vz;
        }
        if (work->unk0[4] != 0) {
            work->unk0[4]--;
            work->unk40[0].unkC.vx = work->unk40[0].unkC.vy = work->unk40[0].unkC.vz =
                func_801E77B8(0, 0x10, work->unk0[4], 0x5C, 0x18A);
            work->unk40[1].unkC.vx = work->unk40[1].unkC.vy = work->unk40[1].unkC.vz =
                func_801E77B8(0, 0x10, work->unk0[4], 0x50, 0x143);
            work->unk40[2].unkC.vx = work->unk40[2].unkC.vy = work->unk40[2].unkC.vz =
                func_801E77B8(0, 0x10, work->unk0[4], 0x66, 0x199);
        }
        if (work->unk0[5] != 0) {
            work->unk0[5]--;
            work->unk40[0].unkC.vx = work->unk40[0].unkC.vy = work->unk40[0].unkC.vz =
                func_801E77B8(0, 0x10, work->unk0[5], 0x18A, 0);
            work->unk40[1].unkC.vx = work->unk40[1].unkC.vy = work->unk40[1].unkC.vz =
                func_801E77B8(0, 0x10, work->unk0[5], 0x143, 0);
            work->unk40[2].unkC.vx = work->unk40[2].unkC.vy = work->unk40[2].unkC.vz =
                func_801E77B8(0, 0x10, work->unk0[5], 0x199, 0);
        }
        if (work->unk0[6] != 0) {
            work->unk0[6]--;
            if (D_801E898C > 0x200) {
                scale.vx = scale.vy = scale.vz =
                    func_801E77B8(2, 0x10, work->unk0[6], D_801E8974.vy, D_801E8974.vy >> 1);
                ex->btlfx_setUnitScale(0, (s32 *)&scale);
            }
            pos.vy = D_801E896C.vy + func_801E77B8(2, 0x10, work->unk0[6], 0, -0x200);
            pos.vx = D_801E896C.vx;
            pos.vz = D_801E896C.vz;
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        if (work->unk0[8] != 0) {
            work->unk0[8]--;
            pos.vy = D_801E896C.vy - 0x200;
            pos.vx = D_801E896C.vx;
            pos.vz = D_801E896C.vz;
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        if (work->unk0[7] != 0) {
            if (--work->unk0[7] != 0) {
                if (D_801E898C > 0x200) {
                    scale.vx = scale.vy = scale.vz =
                        func_801E77B8(2, 0x10, work->unk0[7], D_801E8974.vy >> 1, D_801E8974.vy);
                    ex->btlfx_setUnitScale(0, (s32 *)&scale);
                }
                pos.vy = D_801E896C.vy + func_801E77B8(2, 0x10, work->unk0[7], -0x200, 0);
                pos.vx = D_801E896C.vx;
                pos.vz = D_801E896C.vz;
            } else {
                ex->btlfx_getUnitHomePos(0, (s16 *)&pos);
                ex->btlfx_setUnitScale(0, (s32 *)&D_801E8974);
            }
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&D_801E8984);
        D_801E899C.vx = D_801E8984.vx;
        D_801E899C.vy = D_801E8984.vy;
        D_801E899C.vz = D_801E8984.vz;
        if (D_801E8990 != NULL && D_801E8990->state == -1) {
            D_801E8990 = NULL;
        }
        ex->btlfx_moveEmitterTo((s32)D_801E8990, (s32)&D_801E899C);
        if (work->unk0[1] != 0) {
            work->unk94.vx = D_801E8984.vx;
            work->unk94.vy = D_801E8984.vy;
            work->unk94.vz = D_801E8984.vz;
            ex->btlfx_aimAngles((s16 *)&D_801E8984, (s16 *)&env->camera->eye, (s16 *)&work->unk40[1].unk4);
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
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            for (i = 0; i < 5; i++) {
                tr = &D_801E89A4[i];
                if (tr->unk11C > 0) {
                    tr->unk11C--;
                    tr->unk0 = tr->unk2 = n - 1;
                    tr->unk1 = 0;
                    cur.vx = D_801E895C.vx << 12;
                    cur.vy = D_801E895C.vy << 12;
                    cur.vz = D_801E895C.vz << 12;
                    d.vx = ((work->unk94.vx + tr->unk5C - D_801E895C.vx) << 12) / n;
                    d.vy = ((work->unk94.vy + tr->unk60 - D_801E895C.vy) << 12) / n;
                    d.vz = ((work->unk94.vz + tr->unk64 - D_801E895C.vz) << 12) / n;
                    strip = (u8 *)tr;
                    pt = tr->unk4;
                    for (j = 0; j < n; pt++, j++) {
                        pt->vx = cur.vx >> 12;
                        pt->vz = cur.vz >> 12;
                        pt->vy = tr->unk6C[j] + (cur.vy >> 12);
                        pt->unk6 = tr->unk11C << 2;
                        cur.vx += d.vx;
                        cur.vy += d.vy;
                        cur.vz += d.vz;
                        tr->unk6C[j] += tr->unkC4[j];
                    }
                    if (tr->unk11C < 4) {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 9), 0, NULL, frame, 0x1000,
                                            tr->unk11C << 10, 1, strip, 0);
                    } else {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 9), 0, NULL, frame, 0x1000,
                                            0x1000, 1, strip, 0);
                    }
                } else if (tr->unk11C < 0) {
                    tr->unk11C++;
                } else if (work->unk0[0] >= 5) {
                    tr->unk11C = ex->btlfx_randRange(4, 8);
                    j = 1;
                    a = ex->btlfx_rand() & 0xFFF;
                    b = ex->btlfx_rand() & 0xFFF;
                    r = (ex->math_cos(a) * work->unk40[0].unkC.vx) >> 12;
                    tr->unk60 = r;
                    tr->unk5C = (ex->math_sin(b) * abs(r)) >> 12;
                    tr->unk64 = (ex->math_cos(b) * abs(r)) >> 12;
                    tr->unkC4[0] = 0;
                    tr->unk6C[0] = 0;
                    for (; j < n - 1; j++) {
                        w2 = j * 8 + 16;
                        w1 = -w2;
                        tr->unk6C[j] = ex->btlfx_randRange(-(j * 16) - 32, j * 16 + 32);
                        tr->unkC4[j] = ex->btlfx_randRange(w1, w2);
                    }
                    tr->unkC4[j] = 0;
                    tr->unk6C[j] = 0;
                }
            }
        }
        if (work->unk0[9] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xC), work->unk0[9]);
            work->unk0[9]++;
        }
        return frame >= 0x5A;
    }
    return 0;
}
