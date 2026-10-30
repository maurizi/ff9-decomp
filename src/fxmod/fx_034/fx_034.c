#include "fxmod/fx_034.h"

void func_801E7744(s32 model, s32 scale, u8 *buf) {
    s32 n;
    s32 j = 0;

    n = BTLFX_EXPORTS(D_801E82A0)->btlfx_getEffModelMeshCount(model & 0xFFFF);
    for (; j < n; j++) {
        for (model = 0; model < 8; model++) {
            func_801E7810(model, D_801E82D0[model], scale, D_801E82B0[model]);
        }
    }
}

INCLUDE_RODATA("asm/us/fx_034/nonmatchings/fx_034", fx_034_headerPtr);

void func_801E7810(s32 kind, s32 count, s32 scale, SVECTOR *v) {
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    s32 n;
    s32 k;
    s32 m;

    switch (kind) {
    case 0:
    case 2:
    case 4:
    case 6:
        n = 4;
        break;
    case 1:
    case 3:
    case 5:
    case 7:
        n = 3;
        break;
    default:
        n = 3;
        break;
    }
    for (k = 0; k < count; k++) {
        a.vx = D_801E82E0[k].x;
        a.vy = D_801E82E0[k].y;
        a.vz = D_801E82E0[k].z;
        b.vx = D_801E82E0[k].x;
        b.vy = D_801E82E0[k].y;
        b.vz = D_801E82E0[k].z;
        c.vx = scale * a.vx / 200;
        a.vx += c.vx;
        c.vy = scale * a.vy / 200;
        a.vy += c.vy;
        c.vz = scale * a.vz / 200;
        a.vz += c.vz;
        for (m = 0; m < n; m++, v++) {
            v->vx -= b.vx;
            v->vy -= b.vy;
            v->vz -= b.vz;
            v->vx += a.vx;
            v->vy += a.vy;
            v->vz += a.vz;
        }
    }
}

void func_801E7A00(s32 model, u8 *buf) {
    BtlfxExports *ex;
    s32 n;
    s32 i;
    s32 j;
    s32 m;
    s32 mdl;

    ex = BTLFX_EXPORTS(D_801E82A0);
    mdl = model;
    j = 0;
    m = mdl & 0xFFFF;
    ex->Hi_SplitMdlVertex(m, -1, (s32)buf);
    n = ex->btlfx_getEffModelMeshCount(m);
    for (; j < n; j++) {
        for (i = 0; i < 8; i++) {
            D_801E82D0[i] = ex->Hi_GetMdlVertexPtr(mdl & 0xFFFF, j, (s32)buf, (s32 *)&D_801E82B0[i], i);
            func_801E7B1C(i, D_801E82D0[i], D_801E82B0[i]);
        }
    }
}

void func_801E7B1C(s32 kind, s32 count, SVECTOR *v) {
    SVECTOR sum;
    s32 n;
    s32 k;
    s32 m;

    switch (kind) {
    case 0:
    case 2:
    case 4:
    case 6:
        n = 4;
        break;
    case 1:
    case 3:
    case 5:
    case 7:
    default:
        n = 3;
        break;
    }
    for (k = 0; k < count; k++) {
        sum.vx = 0;
        sum.vy = 0;
        sum.vz = 0;
        for (m = 0; m < n; m++, v++) {
            sum.vx += v->vx;
            sum.vy += v->vy;
            sum.vz += v->vz;
        }
        D_801E82E0[k].x = sum.vx / n;
        D_801E82E0[k].y = sum.vy / n;
        D_801E82E0[k].z = sum.vz / n;
    }
}

s32 fx_034_op80(s32 mode, Fx034Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p;
    SVECTOR a;
    Pool6CEntry *q;
    s32 frame;
    s32 d;
    s32 v;
    s32 early;

    if (mode == 0) {
        *arg4 = 0x74;
    } else if (mode == 1) {
        D_801E82A0 = env;
        ex->btlfx_initEffModels(D_801E88C0, 2);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk34);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk3C);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk44);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk4C);
        work->unk0 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk4C);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801EA040;
            work->unk0->unk12 = 0x28;
            work->unk0->unkC = 0x30;
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        ex->btlfx_aimAngles((s16 *)&work->unk4C, (s16 *)&work->unk44, (s16 *)&work->unk54);
        a.vx = work->unk54.vx;
        a.vy = work->unk54.vy;
        a.vz = work->unk54.vz;
        a.vy += 0x800;
        ex->btlfx_pointAhead((s16 *)&work->unk44, (s16 *)&a, 0x320, (s16 *)&p);
        ex->btlfx_aimAngles((s16 *)&work->unk34, (s16 *)&p, (s16 *)&work->unk5C);
        work->unk5C.vy += 0x800;
        work->unk64.vx = p.vx;
        work->unk64.vy = p.vy;
        work->unk64.vz = p.vz;
        work->unk8.vx = 0;
        work->unk8.vz = 0;
        work->unk8.vy = work->unk54.vy;
        work->unk10.vx = p.vx;
        work->unk10.vy = p.vy;
        work->unk10.vz = p.vz;
        work->unk20.vx = 0x800;
        work->unk20.vy = 0x800;
        work->unk20.vz = 0x800;
        work->unk30 = -0x20;
        work->unk6E = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 1), 0x99, 0x3D80, 0, 0);
        work->unk70 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xA), 0x9A, 0x3DC0, 0, 0x80);
        work->unk6C = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 15:
            work->unk4 = q = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk64);
            if (q != NULL) {
                q->unk22 = 0x80;
            }
            break;
        case 28:
            func_801E7A00(work->unk6E, D_801E8990);
            break;
        }
        if (work->unk0 != NULL) {
            if (work->unk0->state == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk54.vx;
                work->unk0->unk5C.vy = work->unk54.vy;
                work->unk0->unk5C.vz = work->unk54.vz;
            }
        }
        if (frame >= 15 && work->unk4 != NULL) {
            if (work->unk4->state == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk5C.vx;
                work->unk4->unk5C.vy = work->unk5C.vy;
                work->unk4->unk5C.vz = work->unk5C.vz;
            }
        }
        d = frame - 15;
        if (d >= 0 && d < 30) {
            if (d < 4) {
                v = (d << 11) / 3;
                work->unk20.vx = v;
                work->unk20.vy = v;
                work->unk20.vz = v;
            }
            early = frame < 28;
            if (!early) {
                func_801E7744(work->unk6E, frame, D_801E8990);
            }
            ex->Hi_DrawEffModel(&work->unk8, &work->unk10, (s32)&work->unk20, work->unk6E);
            if (early) {
                ex->Hi_DrawEffModel(&work->unk8, &work->unk10, (s32)&work->unk20, work->unk70);
                ex->Hi_ModifyEffModelAbr(work->unk70, 1);
            } else {
                work->unk20.vx -= 0x10;
                work->unk20.vy -= 0x10;
                work->unk20.vz -= 0x10;
                ex->Hi_ModifyEffModelAbr(work->unk6E, 1);
                ex->Hi_ModifyEffModelRGB(work->unk6E, work->unk30, work->unk30, work->unk30);
                work->unk30 -= 8;
            }
            if (frame >= 24 && frame < 28) {
                ex->btlfx_drawScrollMove(0x3C0 - work->unk6C * 0x10, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
                work->unk6C++;
            }
        }
        work->unk6C++;
        if (frame >= 60) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}
