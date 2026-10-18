/* Draft of fx_0f5 fx_0f5_op80. Status: first full draft, ~237 aligned mismatches ignoring registers. Global
 * allocation differs: the original spills ex (0x68) and the case-0 t >> 6 (0x70) but keeps &scale (s3) and the
 * constants 0x2000/0x80/300 of each case in s0/s2/s5 across the two func_801E7704 calls; here ex stays in s3.
 * In the original no shift of t is hoisted above a call by sched1 (t >> 2 is now inline in its two calls, which
 * keeps it below the first call; t >> 5 and t >> 6 still move up). */
#include "btlfx_mod.h"

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
        t = work->unk4;
        r = (t << 9) / 72;
        frame = entry->unk0;
        a = 0x1000 - ex->math_cos((t << 10) / 72);
        work->unk10.vx = work->unk8.vx + ((ex->math_cos(a) * r) >> 12);
        work->unk10.vy = work->unk8.vy;
        work->unk10.vz = work->unk8.vz + ((ex->math_sin(a) * r) >> 12);
        rot.vz = 0;
        rot.vx = 0;
        rot.vy = work->unk4 << 5;
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 12;
            scale.vz = 0x2000;
            scale.vy = scale.vx = t;
            d = t >> 6;
            func_801E7704(&work->unk10, &work->unk18, &scale, 0xC8, 0x180, 0x80, 0x97, 0x3D40, 0, 0, d, work->unk4 * 40, 300,
                          0xC8, 1, 0);
            scale.vy = scale.vx = 0x2000;
            scale.vz = ex->math_sin(t >> 2) * 2;
            c = t >> 5;
            func_801E7704(&work->unk10, &work->unk18, &scale, (ex->math_cos(t >> 2) >> 4) + 0x1C8, 0x280, c, 0x17, 0x3D00, 0, 0x80, d,
                          work->unk4 * 40, 300, 0x80, 1, 1);
            scale.vz = scale.vx = t;
            scale.vy = 0x1000 - ex->math_cos(t >> 2);
            pos.vx = work->unk10.vx;
            pos.vy = work->unk10.vy;
            pos.vz = work->unk10.vz;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk20);
            c -= 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk20, c, c, c);
            if (frame >= 12) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 12;
            scale.vz = 0x2000;
            scale.vy = scale.vx = t + 0x1000;
            c = t >> 5;
            d = c + 0x40;
            func_801E7704(&work->unk10, &work->unk18, &scale, 0xC8, 0x180, 0x80, 0x97, 0x3D40, 0, 0, d, work->unk4 * 40, 300,
                          0xC8, 1, 0);
            scale.vy = scale.vx = 0x2000;
            func_801E7704(&work->unk10, &work->unk18, &scale, 0x1C8 - c, 0x280, 0x80, 0x17, 0x3D00, 0, 0x80, d,
                          work->unk4 * 40, 300, 0x80, 1, 1);
            pos.vx = work->unk10.vx;
            pos.vy = work->unk10.vy;
            scale.vz = scale.vx = scale.vy = 0x1000;
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
            t = frame << 7;
            scale.vz = scale.vy = scale.vx = (t >> 1) + 0x2000;
            d = (t >> 5) + 0xC0;
            func_801E7704(&work->unk10, &work->unk18, &scale, 0xC8 - (t >> 6), 0x180, 0x80, 0x97, 0x3D40, 0, 0, d,
                          work->unk4 * 40, 300, (t >> 5) + 0xA0, 1, 0);
            scale.vz = scale.vy = scale.vx = (t >> 1) + 0x2000;
            func_801E7704(&work->unk10, &work->unk18, &scale, 0x148, 0x280, 0x80, 0x17, 0x3D00, 0, 0x80, d, work->unk4 * 40,
                          300, 0x80, 1, 1);
            pos.vx = work->unk10.vx;
            pos.vy = work->unk10.vy;
            scale.vz = scale.vx = scale.vy = 0x1000;
            pos.vz = work->unk10.vz;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk20);
            if (frame >= 0x20) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            scale.vz = scale.vy = scale.vx = 0x2800;
            t = frame << 8;
            a = t >> 2;
            func_801E7704(&work->unk10, &work->unk18, &scale, 0x88, 0x180, ex->math_cos(a) >> 5, 0x97, 0x3D40, 0, 0, 0x140,
                          work->unk4 * 40, 300, 0x120, 1, 0);
            scale.vz = scale.vy = scale.vx = 0x2800;
            func_801E7704(&work->unk10, &work->unk18, &scale, 0x148, 0x280, ex->math_cos(a) >> 5, 0x17, 0x3D00, 0, 0x80, 0x140,
                          work->unk4 * 40, 300, 0x80, 1, 1);
            scale.vz = scale.vx = 0x1000;
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
        work->unk18.vz = work->unk18.vz - 0x10 - work->unk4 * 2;
        ex->func_800E6624(10000 - work->unk4 * 12, 0x1C0, 0x100, 0x40, 0x80, 0x200, 0x100);
        ex->func_800E6624(10000 - work->unk4 * 6, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
        ex->func_800E6624(work->unk4 * 8, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
        work->unk4++;
    }
    return 0;
}
