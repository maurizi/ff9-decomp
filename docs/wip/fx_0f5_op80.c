/* Draft of fx_0f5 fx_0f5_op80. Status (build/sdiff.py --count --aligned): 567 total, 41 ignoring registers, 39
 * ignoring registers and stack offsets (was 605/107/67). Levers found: chained scale stores in the original's
 * order (`a = b = x` stores b first); `work->unk18.vz -= work->unk4 * 2 + 0x10`; top block reads work->unk4
 * directly (t is only the per-case value, fp in the original); `t >> 5` / `t >> 6` inline in case 0's calls
 * (single-set pseudos get sched1's birthing boost, so they are computed right before their stores); case 1's
 * t >> 5 in a separate variable from case 0/3's c (a call-crossing multi-set pseudo is hoisted across calls by
 * sched1); and a dead conditional `if (t) { c = 0; }` after case 0's first call: it survives until jump2, so
 * flow sees two basic blocks and the values used by both calls (&unk10, &unk18, 300, t >> 6) become global-alloc
 * pseudos, as in the original (its local-alloc only ever uses s0-s3, work is s4; the case-local s5-s7 values and
 * the spilled t >> 6 at 0x70 are global). Open: the same structure for cases 1-3 (their 300 is spilled, i.e.
 * global, in the original), work in s4 and t in fp. */
#include "fxmod/fx_0f5.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8; /* base position */
    SVECTOR unk10; /* position */
    SVECTOR unk18; /* rotation */
    s32 unk20; /* model */
} Fx0f5Work80; /* 0x24 */

extern Unk800F78A4 *D_801E8A74;
extern BtlModel D_801E8A84[];
void func_801E7704(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 c, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 dr, s32 ang, s32 dang, s32 dseg, s32 abr, s32 flicker);

s32 fx_0f5_op80(s32 mode, Fx0f5Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 frame;
    s32 t;
    s32 r;
    s32 a;
    s32 d;
    s32 c;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        D_801E8A74 = env;
        ex->btlfx_initEffModels(D_801E8A84, 1);
        work->unk20 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 9), 1);
        work->unk0 = 0;
        work->unk4 = 0;
        BTLFX_COPY_SVEC(&work->unk8, &env->unkC->unk38);
        work->unk18.vx = 0x400;
        work->unk18.vy = 0x20;
        work->unk18.vz = 0;
    } else {
        r = (work->unk4 << 9) / 72;
        frame = entry->unk0;
        a = 0x1000 - ex->math_cos((work->unk4 << 10) / 72);
        work->unk10.vx = work->unk8.vx + ((ex->math_cos(a) * r) >> 12);
        work->unk10.vz = work->unk8.vz + ((ex->math_sin(a) * r) >> 12);
        work->unk10.vy = work->unk8.vy;
        rot.vz = 0;
        rot.vx = 0;
        rot.vy = work->unk4 * 32;
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 12;
            scale.vz = 0x2000;
            scale.vx = scale.vy = t;
            func_801E7704(&work->unk10, &work->unk18, &scale, 0xC8, 0x180, 0x80, 0x97, 0x3D40, 0, 0, t >> 6, work->unk4 * 40, 300,
                          0xC8, 1, 0);
            if (t) {
                c = 0;
            }
            scale.vx = scale.vy = 0x2000;
            scale.vz = ex->math_sin(t >> 2) * 2;
            func_801E7704(&work->unk10, &work->unk18, &scale, (ex->math_cos(t >> 2) >> 4) + 0x1C8, 0x280, t >> 5, 0x17, 0x3D00, 0, 0x80, t >> 6,
                          work->unk4 * 40, 300, 0x80, 1, 1);
            scale.vx = scale.vz = t;
            scale.vy = 0x1000 - ex->math_cos(t >> 2);
            pos.vx = work->unk10.vx;
            pos.vy = work->unk10.vy;
            pos.vz = work->unk10.vz;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk20);
            c = (t >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk20, c, c, c);
            if (frame >= 12) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 12;
            scale.vz = 0x2000;
            scale.vx = scale.vy = t + 0x1000;
            a = t >> 5;
            d = a + 0x40;
            func_801E7704(&work->unk10, &work->unk18, &scale, 0xC8, 0x180, 0x80, 0x97, 0x3D40, 0, 0, d, work->unk4 * 40, 300,
                          0xC8, 1, 0);
            scale.vx = scale.vy = 0x2000;
            func_801E7704(&work->unk10, &work->unk18, &scale, 0x1C8 - a, 0x280, 0x80, 0x17, 0x3D00, 0, 0x80, d,
                          work->unk4 * 40, 300, 0x80, 1, 1);
            do {
                pos.vx = work->unk10.vx;
            } while (0);
            pos.vy = work->unk10.vy;
            scale.vy = scale.vx = scale.vz = 0x1000;
            pos.vz = work->unk10.vz;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk20);
            if (frame >= 12) {
                work->unk0 = 2;
                entry->unk0 = -1;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x31;
                }
            }
            break;
        case 2:
            do {
                t = frame << 7;
                scale.vx = scale.vy = scale.vz = (t >> 1) + 0x2000;
                d = (t >> 5) + 0xC0;
                func_801E7704(&work->unk10, &work->unk18, &scale, 0xC8 - (t >> 6), 0x180, 0x80, 0x97, 0x3D40, 0, 0, d,
                              work->unk4 * 40, 300, (t >> 5) + 0xA0, 1, 0);
                scale.vx = scale.vy = scale.vz = (t >> 1) + 0x2000;
                func_801E7704(&work->unk10, &work->unk18, &scale, 0x148, 0x280, 0x80, 0x17, 0x3D00, 0, 0x80, d, work->unk4 * 40,
                              300, 0x80, 1, 1);
                do {
                    pos.vx = work->unk10.vx;
                } while (0);
                pos.vy = work->unk10.vy;
                scale.vy = scale.vx = scale.vz = 0x1000;
                pos.vz = work->unk10.vz;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk20);
            } while (0);
            if (frame >= 0x20) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            scale.vx = scale.vy = scale.vz = 0x2800;
            t = frame << 8;
            a = t >> 2;
            func_801E7704(&work->unk10, &work->unk18, &scale, 0x88, 0x180, ex->math_cos(a) >> 5, 0x97, 0x3D40, 0, 0, 0x140,
                          work->unk4 * 40, 300, 0x120, 1, 0);
            scale.vx = scale.vy = scale.vz = 0x2800;
            func_801E7704(&work->unk10, &work->unk18, &scale, 0x148, 0x280, ex->math_cos(a) >> 5, 0x17, 0x3D00, 0, 0x80, 0x140,
                          work->unk4 * 40, 300, 0x80, 1, 1);
            scale.vx = scale.vz = 0x1000;
            scale.vy = ex->math_cos(a);
            pos.vx = work->unk10.vx;
            pos.vy = work->unk10.vy;
            pos.vz = work->unk10.vz;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk20);
            c = ((0x1000 - t) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk20, c, c, c);
            if (frame >= 0x10) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk18.vz -= work->unk4 * 2 + 0x10;
        ex->func_800E6624(10000 - work->unk4 * 12, 0x1C0, 0x100, 0x40, 0x80, 0x200, 0x100);
        ex->func_800E6624(10000 - work->unk4 * 6, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
        ex->func_800E6624(work->unk4 * 8, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
        work->unk4++;
    }
    return 0;
}
