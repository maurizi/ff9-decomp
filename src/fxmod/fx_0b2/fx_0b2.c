#include "fxmod/fx_0b2.h"

void func_801E7704(Unk80100C78Entry *entry, Fx0b2Trail *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9, s32 arg10) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E90A0);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 h;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = 0x18;
    trail->unk1 = 0;
    do {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0xA0);
        i++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(arg4, arg5, t);
        ex->func_800D044C(&pos, (s32)p, 0, (XY32 *)&rot, h, arg6);
        p->pad = arg10;
        arg6 += arg7 + ex->btlfx_randCentered(0x80);
        p++;
    } while (i < 25);
    ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 4), 0, NULL, arg8, 0x1000, arg9, 1, (u8 *)trail, 0);
}

INCLUDE_ASM("asm/us/fx_0b2/nonmatchings/fx_0b2", func_801E78FC);

s32 fx_0b2_op80(s32 mode, Fx0b2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    s32 frame;
    s32 alpha;
    s32 r;
    s32 a;
    s32 i;
    s32 t; /* one variable for cases 1-3 (fp in the original) */
    s32 state;
    s32 one = 1; /* a variable, not a literal: the original reloads it for `mode == 1` and `return 1` */

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == one) {
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
        state = work->unk0;
        /* Dead stores only (flow deletes them, jump2 the branch): the labels end cse's block here, so case 3
         * computes 0x1000 - t from the constant instead of reusing alpha (which cse would know is 0x1000). */
        if (a) {
            r = 0;
            a = 0;
        } else {
            r = 1;
            a = 1;
        }
        switch (state) {
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
            rot.vz = work->unk4 * 90;
            rot.pad = 1;
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
            s32 y;

            scale.vz = 0x2000;
            t = (frame << 12) / 12;
            scale.vx = scale.vy = t + 0x1000;
            func_801E78FC(&work->unk10, &work->unk18, &scale, 0xC8, 0x180, 0x80, 0x11A, 0xFFFF, 0x10, 0x90, (t >> 5) + 0x40,
                          work->unk4 * 0xC8, 0x190, 0xC8, 1, 0x10);
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = work->unk4 * 90;
            rot.pad = 1;
            ex->func_800D1680((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk10, &rot, 0x6000, 0, 1,
                              t / 2 + 0x800, 0, 0);
            scale.vz = scale.vx = scale.vy = 0x2000;
            y = 0x1C8 - (t >> 5);
            func_801E78FC(&work->unk10, &work->unk18, &scale, y, 0x280, 0x80, 0x1A, 0x3D01, 0, 0, (t >> 5) + 0x40, work->unk4 * 0xC8,
                          0x190, 0x80, 1, 8);
            scale.vx = scale.vy = scale.vx * 3 / 2;
            func_801E78FC(&work->unk10, &work->unk18, &scale, y, 0x280, 0x40, 0x17, 0x3D03, 0, 0x70, (t >> 5) + 0x40,
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
            rot.vz = work->unk4 * 90;
            rot.pad = 1;
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
            s32 u;

            scale.vx = scale.vy = scale.vz = 0x2800;
            t = frame << 8;
            u = t >> 2;
            func_801E78FC(&work->unk10, &work->unk18, &scale, 0x88, 0x180, ex->math_cos(u) >> 5, 0x11A, 0xFFFF, 0x10, 0x90,
                          0x140, work->unk4 * 0xC8, 0x190, 0x120, 1, 0x10);
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = work->unk4 * 90;
            rot.pad = 1;
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
                return one;
            }
            break;
        }
        }
        work->unk18.vz += -0x10 - work->unk4 * 2;
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
            s32 e;

            from.vx = work->unk10.vx;
            from.vy = work->unk10.vy;
            from.vz = work->unk10.vz;
            from.vy -= 0x640;
            ang = work->unk4 * 0x180 + (ex->btlfx_rand() & 0xFF);
            p = D_801E90B0;
            do {
                i++;
                h = ((ex->btlfx_rand() & 0xFFF) * alpha) >> 12;
                d = (ex->btlfx_rand() & 0x1FF) + 0x400;
                BTLFX_COPY_SVEC(&to, &work->unk10);
                to.vx += (ex->math_cos(ang) * d) >> 12;
                to.vz += (ex->math_sin(ang) * d) >> 12;
                e = ang + ex->btlfx_rand();
                func_801E7704(entry, p, &from, &to, 0x100, 0x180, e, 0x100, 0, h, (ex->btlfx_rand() & 0xF) + 0xF);
                p++;
                ang += 0x300 + (ex->btlfx_rand() & 0x3FF);
            } while (i < 3);
        }
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b2/nonmatchings/fx_0b2", fx_0b2_headerPtr);
