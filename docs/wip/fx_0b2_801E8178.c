/* Draft of fx_0b2_op80 (Ark's attack effect, module fx_0b2). Same statements, calls and arguments as the
 * original (ccprobe shows the instruction sequences lining up case by case), but register allocation differs:
 * the original keeps work in s5, the export table in s7 and a per-case shift in fp, and spills one more value
 * (frame 0xB8 vs 0xB0). The permuter (1 h, -j 4) did not close it. */
#include "fxmod/fx_0b2.h"

s32 fx_0b2_op80(s32 mode, Fx0b2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    s32 frame;
    s32 alpha;
    s32 r;
    s32 a;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E90A0 = env;
        ((s32 *)&work->unk8)[0] = ((s32 *)&env->unkC->unk38)[0];
        ((s32 *)&work->unk8)[1] = ((s32 *)&env->unkC->unk38)[1];
        work->unk18.vx = 0x400;
        work->unk18.vy = 0x20;
        work->unk18.vz = 0;
    } else {
    alpha = 0x1000;
    r = work->unk4 * 0x300 / 72;
    frame = entry->unk0;
    a = alpha - ex->math_cos(work->unk4 * 1500 / 72);
    work->unk10.vx = work->unk8.vx + ((ex->math_cos(a) * r) >> 12);
    work->unk10.vz = work->unk8.vz + ((ex->math_sin(a) * r) >> 12);
    work->unk10.vy = work->unk8.vy;
    switch (work->unk0) {
    case 0: {
        SVECTOR rot;
        s32 w;
        s32 t;

        scale.vz = 0x2000;
        alpha = (frame << 12) / 12;
        scale.vy = alpha;
        scale.vx = alpha;
        w = alpha >> 6;
        func_801E78FC(&work->unk10, &work->unk18, &scale, 0xC8, 0x180, 0x80, 0x11A, 0xFFFF, 0x10, 0x90, w,
                      work->unk4 * 0xC8, 0x190, 0xC8, 1, 0x10);
        rot.vx = 0x400;
        rot.vy = 0;
        rot.pad = 1;
        rot.vz = work->unk4 * 90;
        ex->func_800D1680((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk10, &rot, 0x6000, 0, 1, alpha / 2, 0,
                          0);
        scale.vy = 0x2000;
        scale.vx = 0x2000;
        t = alpha >> 2;
        scale.vz = ex->math_sin(t) * 2;
        func_801E78FC(&work->unk10, &work->unk18, &scale, (ex->math_cos(t) >> 4) + 0x1C8, 0x280, alpha >> 5, 0x1A,
                      0x3D01, 0, 0, w, work->unk4 * 0xC8, 0x190, 0x80, 1, 8);
        scale.vx = scale.vy = scale.vx * 3 / 2;
        func_801E78FC(&work->unk10, &work->unk18, &scale, (ex->math_cos(t) >> 4) + 0x1C8, 0x280, w, 0x17, 0x3D03, 0,
                      0x70, w, work->unk4 * 0xC8, 0x190, 0x40, 1, 8);
        if (frame >= 12) {
            work->unk0 = 1;
            entry->unk0 = -1;
        }
        break;
    }
    case 1: {
        SVECTOR rot;
        s32 t;
        s32 w;
        s32 x;

        scale.vz = 0x2000;
        t = (frame << 12) / 12;
        scale.vx = scale.vy = t + 0x1000;
        x = t >> 5;
        w = x + 0x40;
        func_801E78FC(&work->unk10, &work->unk18, &scale, 0xC8, 0x180, 0x80, 0x11A, 0xFFFF, 0x10, 0x90, w,
                      work->unk4 * 0xC8, 0x190, 0xC8, 1, 0x10);
        rot.vx = 0x400;
        rot.vy = 0;
        rot.pad = 1;
        rot.vz = work->unk4 * 90;
        ex->func_800D1680((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk10, &rot, 0x6000, 0, 1,
                          t / 2 + 0x800, 0, 0);
        scale.vz = scale.vx = scale.vy = 0x2000;
        x = 0x1C8 - x;
        func_801E78FC(&work->unk10, &work->unk18, &scale, x, 0x280, 0x80, 0x1A, 0x3D01, 0, 0, w, work->unk4 * 0xC8,
                      0x190, 0x80, 1, 8);
        scale.vx = scale.vy = scale.vx * 3 / 2;
        func_801E78FC(&work->unk10, &work->unk18, &scale, x, 0x280, 0x40, 0x17, 0x3D03, 0, 0x70, w,
                      work->unk4 * 0xC8, 0x190, 0x40, 1, 8);
        if (frame >= 12) {
            Pool6CEntry *p;

            work->unk0 = 2;
            entry->unk0 = -1;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk10);
            if (p != NULL) {
                p->unk22 = 0x58;
            }
        }
        break;
    }
    case 2: {
        SVECTOR rot;
        s32 t;
        s32 s;
        s32 w;

        t = frame << 7;
        s = (t >> 1) + 0x2000;
        scale.vx = scale.vy = scale.vz = s;
        w = (t >> 5) + 0xC0;
        func_801E78FC(&work->unk10, &work->unk18, &scale, 0xC8 - (t >> 6), 0x180, 0x80, 0x11A, 0xFFFF, 0x10, 0x90, w,
                      work->unk4 * 0xC8, 0x190, (t >> 5) + 0xA0, 1, 0x10);
        rot.vx = 0x400;
        rot.vy = 0;
        rot.pad = 1;
        rot.vz = work->unk4 * 90;
        ex->func_800D1680((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk10, &rot, 0x6000, 0, 1, alpha, 0, 0);
        scale.vx = scale.vy = scale.vz = s;
        func_801E78FC(&work->unk10, &work->unk18, &scale, 0x148, 0x280, 0x80, 0x1A, 0x3D01, 0, 0, w,
                      work->unk4 * 0xC8, 0x190, 0x80, 1, 8);
        scale.vx = scale.vy = scale.vx * 3 / 2;
        func_801E78FC(&work->unk10, &work->unk18, &scale, 0x148, 0x280, 0x40, 0x17, 0x3D03, 0, 0x70, w,
                      work->unk4 * 0xC8, 0x190, 0x40, 1, 8);
        if (frame >= 0x20) {
            work->unk0 = 3;
            entry->unk0 = -1;
        }
        break;
    }
    case 3: {
        SVECTOR rot;
        s32 t;
        s32 u;

        scale.vx = scale.vy = scale.vz = 0x2800;
        t = frame << 8;
        u = t >> 2;
        func_801E78FC(&work->unk10, &work->unk18, &scale, 0x88, 0x180, ex->math_cos(u) >> 5, 0x11A, 0xFFFF, 0x10, 0x90,
                      0x140, work->unk4 * 0xC8, 0x190, 0x120, 1, 0x10);
        rot.vx = 0x400;
        rot.vy = 0;
        rot.pad = 1;
        rot.vz = work->unk4 * 90;
        ex->func_800D1680((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk10, &rot, 0x6000, 0, 1, 0x1000 - t, 0,
                          0);
        scale.vx = scale.vy = scale.vz = 0x2800;
        func_801E78FC(&work->unk10, &work->unk18, &scale, 0x148, 0x280, ex->math_cos(u) >> 5, 0x1A, 0x3D01, 0, 0, 0x140,
                      work->unk4 * 0xC8, 0x190, 0x80, 1, 8);
        alpha -= t;
        scale.vx = scale.vy = scale.vx * 3 / 2;
        func_801E78FC(&work->unk10, &work->unk18, &scale, 0x148, 0x280, ex->math_cos(u) >> 6, 0x17, 0x3D03, 0, 0x70,
                      0x140, work->unk4 * 0xC8, 0x190, 0x40, 1, 8);
        if (frame >= 0x10) {
            return 1;
        }
        break;
    }
    }
    work->unk18.vz = work->unk18.vz - 0x10 - work->unk4 * 2;
    ex->func_800D3030((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 2), work->unk4);
    ex->func_800E6624(10000 - work->unk4 * 8, 0x1C0, 0x170, 0x1C, 0x70, 0x1DC, 0x170);
    ex->func_800E6624(10000 - work->unk4 * 12, 0x280, 0x100, 0x1C, 0x70, 0x29C, 0x100);
    i = 0;
    if (alpha != 0) {
        SVECTOR from;
        SVECTOR to;
        Fx0b2Trail *p;
        s32 ang;
        s32 h;
        s32 d;

        from.vx = work->unk10.vx;
        from.vy = work->unk10.vy;
        from.vy -= 0x640;
        from.vz = work->unk10.vz;
        p = D_801E90B0;
        ang = work->unk4 * 0x180 + (ex->btlfx_rand() & 0xFF);
        do {
            i++;
            h = ((ex->btlfx_rand() & 0xFFF) * alpha) >> 12;
            d = (ex->btlfx_rand() & 0x1FF) + 0x400;
            BTLFX_COPY_SVEC(&to, &work->unk10);
            to.vx += (ex->math_cos(ang) * d) >> 12;
            to.vz += (ex->math_sin(ang) * d) >> 12;
            d = ang + ex->btlfx_rand();
            func_801E7704(entry, p, &from, &to, 0x100, 0x180, d, 0x100, 0, h, (ex->btlfx_rand() & 0xF) + 0xF);
            p++;
            ang = ang + 0x300 + (ex->btlfx_rand() & 0x3FF);
        } while (i < 3);
    }
    work->unk4++;
    }
    return 0;
}
