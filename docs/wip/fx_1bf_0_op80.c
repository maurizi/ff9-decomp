/* Draft of fx_1bf_0 fx_1bf_0_op80. Status: 39 aligned mismatches (21 ignoring registers). tools/equiv.py agrees
 * (low coverage: random modes mostly take the mode 0/1 paths). Left: case 5's reload of entry sits earlier in the
 * original and its p.vz sum is in a0 (ours v0); case 6 schedules `li s0,0x1000` before `li a1,28`; branch 1's
 * `li t0,1`; branch 2 computes x / y = x << 9 first, y in a2 (ours: after the constants, in v1); branch 3's quotient
 * is in a2 (ours a1; a block-local variable there makes it worse). What got it here: q.vx = p.vx copies after the
 * p stores, `c = 0x1000; c -= t;`, n/flag/m set first in branch 1, block-local x/y/z for the sin/cos call and for
 * branch 2, case 6's y as an s16 local loaded after p2.vz, alpha before w in branch 3, and D_801EA1FC as an array
 * (MEM_IN_STRUCT keeps its store after the env->unk2C stores). Levers that worked on fx_1bf_2 op80 (same family):
 * per-case variables for values the original keeps in callee-saved registers, function-scope multi-set temps where
 * the original hoists loads, separate statements instead of chained copies. */
#include "fxmod/fx_1bf_0.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8;
    SVECTOR unk10;
    u8 pad18[0x28 - 0x18];
} Fx1bf0Work; /* 0x28 */

extern SVECTOR D_801E86BC;
extern BtlModel D_801EA5FC[];
extern u8 D_801EA1FC[];

void func_801E7B70(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 a);

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
    s32 d;
    s32 e;
    s32 n;
    s32 flag;
    s32 m;
    s32 w;
    s32 alpha;
    s32 fade;
    s32 s2;
    s32 s0;
    s16 y6;

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
        D_801EA1FC[0] = 0;
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
                d = (ex->math_sin(a) * b) >> 12;
                p.vx = work->unk8.vx + c;
                p.vy = work->unk8.vy;
                p.vz = work->unk8.vz + d;
                q.vx = p.vx;
                q.vz = p.vz;
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
            p2.vz = work->unk8.vz;
            q2.vx = p2.vx;
            y6 = work->unk8.vy;
            q2.vz = p2.vz;
            q2.vy = y6 - 0x1000;
            env->unk28 = 0x10;
            t = (frame << 12) / 51;
            c = 0x1000;
            c -= t;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), &p2, NULL, 0xAAA, 0, 1, c);
            func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &p2, &q2, frame, c, 1, (t >> 7) + 0x20);
            env->unk28 = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            if (frame < 0x10) {
                s32 x = (frame << 12) >> 6;
                s32 y = ex->math_sin(x) * 5;
                s32 z = ex->math_cos(x);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &p2, &rot, y, 0, 1, z);
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
                if (frame < 0x1E) {
                    n = frame;
                    flag = 1;
                    m = 0;
                    w = 0x20;
                    alpha = 0x1000;
                    fade = 0;
                    s2 = fade;
                    s0 = fade;
                } else {
                    s32 x = frame - 0x1E;
                    s32 y = x << 9;
                    n = 0x1D;
                    w = 0x27;
                    alpha = 0x1000;
                    fade = 0;
                    m = x * 3;
                    s2 = alpha;
                    if (y >= 0x800) {
                        s0 = (y - 0x800) * 2;
                    } else {
                        s0 = 0;
                    }
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, ((0x1000 - y) * 100) >> 12, 0, 0);
                }
            } else {
                d = ((frame - 0x26) << 12) / 13;
                fade = 1;
                n = 0x1D;
                m = 0x15;
                alpha = 0x1000 - d * 2 / 3;
                w = (d >> 3) + 0x40;
                s0 = alpha;
                s2 = s0;
            }
            func_801E77C0(entry, &D_801E86BC, n, w, alpha, fade, flag);
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
