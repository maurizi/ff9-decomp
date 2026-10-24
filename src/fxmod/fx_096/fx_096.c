#include "fxmod/fx_096.h"

void func_801E7744(u16 model, s32 idx, s32 t, u16 *verts, Fx096Vec3 *centers) {
    s32 n;
    s32 i;
    s32 j;

    n = BTLFX_EXPORTS(D_801E831C)->func_800DD0DC(model);
    for (j = 0; j < n; j++) {
        for (i = 0; i < 8; i++) {
            func_801E7840(i, D_801EB2CC[idx][i], t, (SVECTOR *)D_801EB28C[idx][i], centers);
        }
    }
}

INCLUDE_RODATA("asm/us/fx_096/nonmatchings/fx_096", fx_096_headerPtr);

void func_801E7840(u32 kind, s32 count, s32 t, SVECTOR *verts, Fx096Vec3 *centers) {
    Fx096Vec3 d;
    Fx096Vec3 to;
    Fx096Vec3 from;
    s32 nv;
    s32 k;
    s32 j;
    s32 m;

    switch (kind) {
    case 0:
    case 2:
    case 4:
    case 6:
        do {
            nv = 4;
        } while (0); /* loop notes: changes the register allocation of nv */
        break;
    case 1:
    case 3:
    case 5:
    case 7:
    default:
        nv = 3;
        break;
    }
    for (k = 0; k < count; k++) {
        to.vx = centers[k].vx;
        to.vy = centers[k].vy;
        to.vz = centers[k].vz;
        from.vx = centers[k].vx;
        from.vy = centers[k].vy;
        from.vz = centers[k].vz;
        m = to.vx * 100;
        d.vx = t * m / 200;
        to.vx += d.vx;
        m = to.vy * 100;
        d.vy = t * m / 200;
        to.vy += d.vy;
        m = to.vz * 100;
        d.vz = t * m / 200;
        to.vz += d.vz;
        for (j = 0; j < nv; j++, verts++) {
            verts->vx -= from.vx;
            verts->vy -= from.vy;
            verts->vz -= from.vz;
            verts->vx += to.vx;
            verts->vy += to.vy;
            verts->vz += to.vz;
        }
    }
}

void func_801E7A74(u16 model, s32 idx, u16 *verts, Fx096Vec3 *centers) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E831C);
    s32 c;
    s32 n;
    s32 i;
    s32 j;

    ex->Hi_SplitMdlVertex(model, -1, (s32)verts);
    n = ex->func_800DD0DC(model);
    for (j = 0; j < n; j++) {
        for (i = 0; i < 8; i++) {
            c = ex->Hi_GetMdlVertexPtr(model, j, (s32)verts, &D_801EB28C[idx][i], i);
            D_801EB2CC[idx][i] = c;
            func_801E7BC4(i, D_801EB2CC[idx][i], (SVECTOR *)D_801EB28C[idx][i], centers);
        }
    }
}

void func_801E7BC4(u32 kind, s32 count, SVECTOR *verts, Fx096Vec3 *centers) {
    Fx096Vec3 sum;
    s32 nv;
    s32 k;
    s32 j;

    switch (kind) {
    case 0:
    case 2:
    case 4:
    case 6:
        nv = 4;
        break;
    case 1:
    case 3:
    case 5:
    case 7:
    default:
        nv = 3;
        break;
    }
    for (k = 0; k < count; k++) {
        sum.vx = 0;
        sum.vy = 0;
        sum.vz = 0;
        for (j = 0; j < nv; j++, verts++) {
            sum.vx += verts->vx;
            sum.vy += verts->vy;
            sum.vz += verts->vz;
        }
        centers[k].vx = sum.vx / nv;
        centers[k].vy = sum.vy / nv;
        centers[k].vz = sum.vz / nv;
    }
}

s32 fx_096_op80(s32 mode, Fx096Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        D_801E831C = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x50;
    }
    return 0;
}

s32 fx_096_op81(s32 mode, Fx096Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u32 i;
    s32 t;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x68;
    } else if (mode == 1) {
        D_801E831C = env;
        ex->btlfx_initEffModels(D_801E832C, 2);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk58);
        work->unk34.vx = 0x221;
        work->unk34.vy = 0x221;
        work->unk34.vz = 0x221;
        work->unk44.vy = 0x200;
        work->unk4.vx = 0;
        work->unk4.vy = 0;
        work->unk4.vz = 0;
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unkC.vz = 0;
        work->unk44.vx = 0;
        work->unk44.vz = 0;
        work->unk14.vx = work->unk58.vx;
        work->unk14.vy = work->unk58.vy;
        work->unk14.vz = work->unk58.vz;
        work->unk24.vx = work->unk58.vx;
        work->unk24.vy = work->unk58.vy;
        work->unk24.vz = work->unk58.vz;
        work->unk14.vy = -0x258;
        work->unk62 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xA), 1);
        work->unk64 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xB), 1);
        work->unk60 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 7:
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk58);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 0:
            func_801E7A74(work->unk62, 0, D_801E84BC, D_801EAD5C);
            for (i = 0; i < 0xA28; i++) {
                D_801E990C[i] = D_801E84BC[i];
            }
            break;
        case 0x18:
            work->unk60 = 0x10;
            break;
        }
        if (frame >= 0 && frame < 0x28) {
            if (frame >= 7) {
                c = ((frame - 7) << 9) / 3;
                work->unk44.vx = c;
                work->unk44.vz = c;
                work->unk44.vx = (work->unk44.vx > 0x200) ? 0x200 : work->unk44.vx;
                work->unk44.vz = (work->unk44.vz > 0x200) ? 0x200 : work->unk44.vz;
            }
            if (frame >= 0 && frame < 6) {
                t = frame;
                for (i = 0; i < 0xA28; i++) {
                    D_801E84BC[i] = D_801E990C[i];
                }
                func_801E7744(work->unk62, 0, 5 - t, D_801E84BC, D_801EAD5C);
                work->unk4.vy += 0x200;
            }
            ex->Hi_DrawEffModel(&work->unk4, &work->unk14, (s32)&work->unk34, work->unk62);
            ex->Hi_DrawEffModel(&work->unkC, &work->unk24, (s32)&work->unk44, work->unk64);
            if (work->unk60 >= 0) {
                work->unk54 = func_801E827C(0, -0x80, 0x10 - work->unk60, 0x10);
                work->unk60--;
                ex->Hi_ModifyEffModelRGB(work->unk62, work->unk54, work->unk54, work->unk54);
                ex->Hi_ModifyEffModelRGB(work->unk64, work->unk54, work->unk54, work->unk54);
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xC), frame);
        if (frame >= 0x5A) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E8238(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E831C)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E827C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}
