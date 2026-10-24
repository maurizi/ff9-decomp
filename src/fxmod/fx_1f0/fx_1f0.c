#include "fxmod/fx_1f0.h"

void func_801E7704(s32 mode, s32 len, s32 t, SVECTOR *ctrl, SVECTOR *out) {
    s32 w[4];
    s32 k;
    s32 s;
    s32 r;
    s32 i;
    s32 *wp;

    k = 0x2000000 / (len * len);
    s = 0;
    switch (mode) {
    case 0:
        s = ((0x1000000 / len) * t) >> 16;
        break;
    case 1:
        s = 0x100 - ((k * (len - t) * (len - t)) >> 17);
        break;
    case 2:
        s = (k * t * t) >> 17;
        break;
    }
    r = 0x100 - s;
    w[0] = (s * s * s) >> 12;
    w[1] = (r * s * s * 6) >> 12;
    w[2] = (r * r * s * 6) >> 12;
    w[3] = (r * r * r) >> 12;
    out->vx = 0;
    out->vy = 0;
    out->vz = 0;
    wp = w;
    for (i = 0; i < 4; i++) {
        out->vx += (ctrl[i].vx * *wp) >> 12;
        out->vy += (ctrl[i].vy * *wp) >> 12;
        out->vz += (ctrl[i].vz * *wp) >> 12;
        wp++;
    }
}

s32 func_801E7948(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8548);
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

s32 fx_1f0_op80(s32 mode, Fx1f0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    Fx1f0Unit *u;
    Pool6CEntry *p;
    s32 frame;
    s32 i;
    s32 t;
    s32 v;
    s32 n;

    if (mode == 0) {
        *arg4 = 0xB8;
    } else if (mode == 1) {
        D_801E8548 = env;
        ex->btlfx_initEffModels(D_801E8608, 4);
        for (i = 15; i >= 0; i--) {
            work->unk0[i] = 0;
        }
        work->unk60 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 3), 1);
        work->unk64 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 5), 1);
        work->unk68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 6), 1);
        work->unk6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 7), 1);
        work->unk80.vx = 0x19B0;
        work->unk80.vy = 0x19B0;
        work->unk80.vz = 0x19B0;
        work->unk90.vx = 0x1400;
        work->unk90.vy = 0x1400;
        work->unk90.vz = 0x1400;
        work->unk70.vx = 0;
        work->unk70.vy = 0;
        work->unk70.vz = 0;
        work->unkA0.vx = 0;
        work->unkA0.vy = 0;
        work->unkA0.vz = 0;
        work->unkA8.vx = 0;
        work->unkA8.vy = 0;
        work->unkA8.vz = 0;
        work->unkB0 = 0;
        work->unkB4 = 0;
        for (i = 0; i < 4; i++) {
            D_801E8558[i].unk0 = -1;
            D_801E8558[i].unk4 = 0x80;
            D_801E8558[i].unk28 = NULL;
        }
        for (i = 0; i < env->targets->targetCount; i++) {
            u = &D_801E8558[i];
            u->unk28 = NULL;
            ex->btlfx_fadeUnitById(i, -2);
            ex->btlfx_getUnitHomePos(i, (s16 *)&D_801E8558[i].unk10);
            u->unk8.vx = u->unk10.vx;
            u->unk8.vy = u->unk10.vy + 0x3000;
            u->unk8.vz = u->unk10.vz - 0x1000;
            u->unk18.vx = u->unk20.vx = u->unk8.vx;
            u->unk18.vy = u->unk20.vy = u->unk8.vy;
            u->unk18.vz = u->unk20.vz = u->unk8.vz;
            ex->btlfx_setUnitPos(i, (s16 *)&D_801E8558[i].unk8);
            u->unk0 = D_801E84F0[i];
        }
        D_801E8558[0].unk20.vy -= 0x100;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk0[0] = 0x32;
            break;
        case 0x32:
            work->unk0[1] = 0xA0;
            break;
        case 0xC0:
            work->unk0[7] = 0x18;
            work->unk0[8] = 0x18;
            work->unk0[6] = 0x18;
            break;
        case 0xC8:
            work->unk0[9] = 0x20;
            break;
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_setUnitPos(i, (s16 *)&D_801E8558[i].unk8);
            }
        }
        if (work->unk0[1] != 0) {
            work->unk0[1]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                if (D_801E8558[i].unk28 != NULL && D_801E8558[i].unk28->unk30 == -1) {
                    D_801E8558[i].unk28 = NULL;
                }
                if (D_801E8558[i].unk0 == 0) {
                    if (D_801E8558[i].unk4 != 0) {
                        D_801E8558[i].unk4--;
                        work->unk40[0].vx = D_801E8558[i].unk8.vx;
                        work->unk40[0].vy = D_801E8558[i].unk8.vy;
                        work->unk40[0].vz = D_801E8558[i].unk8.vz;
                        work->unk40[1].vx = (D_801E8558[i].unk8.vx + D_801E8558[i].unk10.vx) >> 1;
                        work->unk40[1].vy = (D_801E8558[i].unk8.vy + D_801E8558[i].unk10.vy) >> 1;
                        work->unk40[1].vz = D_801E8558[i].unk10.vz;
                        work->unk40[2].vx = D_801E8558[i].unk10.vx;
                        work->unk40[2].vy = D_801E8558[i].unk10.vy - 0x400;
                        work->unk40[2].vz = D_801E8558[i].unk10.vz;
                        work->unk40[3].vx = D_801E8558[i].unk10.vx;
                        work->unk40[3].vy = D_801E8558[i].unk10.vy;
                        work->unk40[3].vz = D_801E8558[i].unk10.vz;
                        func_801E7704(0, 0x80, D_801E8558[i].unk4, work->unk40, &D_801E8558[i].unk20);
                        ex->func_800E3D20((s32)D_801E8558[i].unk28, (s32)&D_801E8558[i].unk20);
                        if (D_801E8558[i].unk4 < 0x10 && D_801E8558[i].unk28 != NULL) {
                            D_801E8558[i].unk28->unk20 = D_801E8558[i].unk4 << 8;
                        }
                        ex->btlfx_setUnitPos(i, (s16 *)&D_801E8558[i].unk20);
                        D_801E8558[i].unk20.vy -= 0x100;
                    } else {
                        D_801E8558[i].unk0 = -1;
                        ex->btlfx_getUnitHomePos(i, (s16 *)&D_801E8558[i].unk20);
                        ex->btlfx_setUnitPos(i, (s16 *)&D_801E8558[i].unk20);
                        ex->func_800E3D54(D_801E8558[i].unk28);
                    }
                }
                if (D_801E8558[i].unk0 > 0) {
                    ex->btlfx_setUnitPos(i, (s16 *)&D_801E8558[i].unk8);
                    if (D_801E8558[i].unk0 == 8) {
                        D_801E8558[i].unk28 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&D_801E8558[i].unk18);
                        if (D_801E8558[i].unk28 != NULL) {
                            D_801E8558[i].unk28->unk22 = 0x80;
                        }
                        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&D_801E8558[i].unk20);
                        if (p != NULL) {
                            p->unk22 = 0x80;
                        }
                    }
                    if (D_801E8558[i].unk0 < 8 && D_801E8558[i].unk28 != NULL) {
                        D_801E8558[i].unk28->unk20 = (8 - D_801E8558[i].unk0) << 9;
                    }
                    D_801E8558[i].unk0--;
                }
            }
        }
        env->camera->points[0].vx = 0;
        env->camera->points[0].vy = D_801E8558[0].unk20.vy;
        env->camera->points[0].vz = D_801E8558[0].unk20.vz;
        work->unkA0.vy = work->unkA0.vy + work->unkB0 + 0x20;
        work->unkA8.vy = work->unkA8.vy - work->unkB4 - 0x20;
        if (work->unk0[4] | work->unk0[5] | work->unk0[6]) {
            ex->btlfx_drawScrollMove(frame, 0x200, 0x100, 0x40, 0x80, 0x200, 0x180);
            ex->btlfx_drawScrollMove(frame * 2, 0x1C0, 0x120, 0x38, 0x70, 0x1C0, 0x190);
        }
        if (work->unk0[6] != 0) {
            work->unk0[6]--;
            t = func_801E7948(0, 0x18, work->unk0[6], 0, 0x1000);
            ex->Hi_DrawMorphEffModel(&work->unkA0, &work->unk70, (s32)&work->unk80, work->unk64, work->unk60, t);
            ex->Hi_DrawMorphEffModel(&work->unkA8, &work->unk70, (s32)&work->unk90, work->unk6C, work->unk68, t);
            if (work->unk0[6] > 0x10) {
                v = -((work->unk0[6] - 0x10) << 4);
                ex->Hi_ModifyEffModelRGB(work->unk64, v, v, v);
                ex->Hi_ModifyEffModelRGB(work->unk6C, v, v, v);
            }
            if (work->unk0[6] < 8) {
                v = -((8 - work->unk0[6]) << 4);
                ex->Hi_ModifyEffModelRGB(work->unk64, v, v, v);
                ex->Hi_ModifyEffModelRGB(work->unk6C, v, v, v);
            }
        }
        if (work->unk0[7] != 0) {
            work->unk0[7]--;
            work->unk80.vx = work->unk80.vy = work->unk80.vz = func_801E7948(1, 0x18, work->unk0[7], 0x19B0, 0x3000);
            work->unk90.vx = work->unk90.vy = work->unk90.vz = func_801E7948(1, 0x18, work->unk0[7], 0x1400, 0x2C00);
        }
        if (work->unk0[8] != 0) {
            work->unk0[8]--;
            work->unkB0 = func_801E7948(1, 0x18, work->unk0[8], 0, 0x80);
            work->unkB4 = func_801E7948(1, 0x18, work->unk0[8], 0, 0x80);
        }
        if (work->unk0[9] != 0) {
            n = work->unk0[9] - 1;
            work->unk0[9] = n;
            scale.vz = scale.vy = scale.vx = (n << 2) + 0x1000;
            col.r = col.g = col.b = n * 3;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, 0);
        }
        return frame >= 0x108;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f0/nonmatchings/fx_1f0", fx_1f0_headerPtr);
