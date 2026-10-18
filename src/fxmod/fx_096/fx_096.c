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

INCLUDE_ASM("asm/us/fx_096/nonmatchings/fx_096", func_801E7A74);

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
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x50;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_096/nonmatchings/fx_096", fx_096_op81);

void func_801E8238(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E831C)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E827C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}
