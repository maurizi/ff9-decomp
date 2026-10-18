/* Draft of fx_1bf_0 fx_1bf_0_op80. Status: first draft, structure complete, about 150 strict mismatches (scheduling in
 * state 5, register allocation). Depends on func_801E77C0 / func_801E7B70, which are still asm. */
#include "fxmod/fx_1bf_0.h"

s32 fx_1bf_0_op80(s32 mode, Fx1bf0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR q;
    SVECTOR p;
    SVECTOR rot;
    SVECTOR q2;
    SVECTOR p2;
    s32 frame;
    s32 t;
    s32 a;
    s32 b;
    s32 c;
    s32 n;
    s32 flag;
    s32 m;
    s32 s7;
    s32 s5;
    s32 fp;
    s32 s2;
    s32 s0;
    s32 d;
    s32 e;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk0 = 5;
        work->unk4 = 0;
        D_801E8830 = env;
        ex->func_800E3C58();
        ex->btlfx_initEffModels(D_801EA5FC, 5);
        work->unk8.vx = D_801E86BC.vx;
        work->unk8.vy = D_801E86BC.vy;
        work->unk8.vz = D_801E86BC.vz;
        work->unk10.vx = work->unk8.vx;
        work->unk10.vy = work->unk8.vy;
        work->unk10.vz = work->unk8.vz;
        env->unk2C->unk0[3].vx = work->unk8.vx;
        env->unk2C->unk0[3].vy = work->unk8.vy;
        env->unk2C->unk0[3].vz = work->unk8.vz;
        D_801EA1FC = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 5:
            env->unk28 = 0x10;
            if (frame >= 0x26) {
                t = ((frame - 0x26) << 12) / 24;
                a = ex->math_cos(t >> 1) >> 1;
                b = ex->math_cos(t >> 2) >> 1;
                c = (ex->math_cos(a) * b) >> 12;
                d = ex->math_sin(a) * b;
                q.vx = p.vx = work->unk8.vx + c;
                p.vy = work->unk8.vy;
                q.vz = p.vz = work->unk8.vz + (d >> 12);
                q.vy = work->unk8.vy - 0x1000;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), &p, NULL, 0xAAA, 0, 1, t);
                func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &p, &q, frame, t, 1, 0x20);
            }
            env->unk28 = 0;
            if (frame >= 0x3E) {
                work->unk0 = 6;
                entry->unk0 = -1;
            }
            break;
        case 6:
            p2.vx = work->unk8.vx;
            p2.vy = work->unk8.vy;
            q2.vx = work->unk8.vx;
            p2.vz = work->unk8.vz;
            q2.vz = work->unk8.vz;
            q2.vy = work->unk8.vy - 0x1000;
            env->unk28 = 0x10;
            t = (frame << 12) / 51;
            c = 0x1000 - t;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), &p2, NULL, 0xAAA, 0, 1, c);
            func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &p2, &q2, frame, c, 1, (t >> 7) + 0x20);
            env->unk28 = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            if (frame < 0x10) {
                a = (frame << 12) >> 6;
                b = ex->math_sin(a) * 5;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &p2, &rot, b, 0, 1, ex->math_cos(a));
            }
            if (frame >= 0x1E) {
                s0 = (frame - 0x1E) << 9;
                if (s0 > 0x1000) {
                    s0 = 0x1000;
                }
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &p2, &rot, 0x5000, 0, 1, s0);
            }
            flag = 0;
            if (frame < 0x26) {
                s7 = 0x20;
                if (frame < 0x1E) {
                    s5 = 0x1000;
                    fp = 0;
                    s2 = 0;
                    s0 = 0;
                    n = frame;
                    flag = 1;
                    m = 0;
                } else {
                    d = frame - 0x1E;
                    e = d << 9;
                    n = 0x1D;
                    s7 = 0x27;
                    s5 = 0x1000;
                    fp = 0;
                    m = d * 3;
                    s2 = s5;
                    if (e >= 0x800) {
                        s0 = (e - 0x800) * 2;
                    } else {
                        s0 = 0;
                    }
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, ((0x1000 - e) * 100) >> 12, 0, 0);
                }
            } else {
                d = ((frame - 0x26) << 12) / 13;
                fp = 1;
                n = 0x1D;
                s7 = (d >> 3) + 0x40;
                m = 0x15;
                s5 = 0x1000 - d * 2 / 3;
                s0 = s5;
                s2 = s0;
            }
            func_801E77C0(entry, &D_801E86BC, n, s7, s5, fp, flag);
            if (s2 != 0) {
                func_801E7B70(entry, &D_801E86BC, m, s2);
                func_801E7E98(entry, &D_801E86BC, s0);
            }
            if (frame >= 0x33) {
                rot.vx = D_801E86BC.vx;
                rot.vy = D_801E86BC.vy;
                rot.vz = D_801E86BC.vz;
                rot.vy -= 0x300;
                env->unk2C->unk0[3].vx = rot.vx;
                env->unk2C->unk0[3].vy = rot.vy;
                env->unk2C->unk0[3].vz = rot.vz;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        env->unk28 = 0;
        work->unk4++;
    }
    return 0;
}
