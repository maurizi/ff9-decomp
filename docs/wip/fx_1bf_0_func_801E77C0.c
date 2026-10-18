/* Draft of fx_1bf_0 func_801E77C0 (builds 3 x 18 trail strands from file 0x1D). Status: 2 instructions off (build/sdiff.py
 * --aligned): operand order of two addu in the strand-offset load (orig `addu v1,fp,v1` = j + k*18 and `addu v1,t1,v1`
 * = data + index; ours has them swapped). tools/equiv.py: 1/1 trials agree, 92% coverage (other trials fault on
 * unaligned random data pointers). What got it here: ex assigned first and u8 *data/k/width/ex declared first (spill
 * slots), `tr = D_801E8754[j];` as its own statement before `tr += stride * k`, width computed before the tr stores
 * (sb can alias the table), `t = start; if (i == 0) {...} t += i;`, loop increments `i++, dst++, pts += 3`, and dst
 * stores in x, y, z, pad order. */
#include "fxmod/fx_1bf_0.h"

extern s16 D_801E86C4[18 * 4]; /* per strand: file, start, stride, width */
extern Fx1bf0Line *D_801E8754[18];

void func_801E77C0(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 w, s32 a, s32 fade, s32 flag) {
    u8 *data;
    s32 k;
    s32 width;
    BtlfxExports *ex;
    SVECTOR last;
    SVECTOR p0;
    SVECTOR p1;
    s16 *pts;
    Fx1bf0Line *tr;
    SVECTOR *dst;
    s32 j;
    s32 i;
    s32 cnt;
    s32 t;

    ex = BTLFX_EXPORTS(D_801E8830);
    k = 0;
    data = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D);
    w = (w * 3) >> 1;
    do {
        for (j = 0; j < 18; j++) {
            cnt = ((u16 *)data)[(k * 18 + j) * 2];
            pts = (s16 *)(data + cnt);
            cnt = pts[1];
            tr = D_801E8754[j];
            tr = (Fx1bf0Line *)((u8 *)tr + D_801E86C4[j * 4 + 2] * k);
            width = w * (D_801E86C4[j * 4 + 3] + 1);
            tr->unk0 = tr->unk2 = cnt - 1;
            tr->unk1 = 0;
            pts += 2;
            dst = tr->unk4;
            for (i = 0; i < cnt; i++, dst++, pts += 3) {
                t = D_801E86C4[j * 4 + 1];
                if (i == 0) {
                    last.vx = from->vx + pts[0] * 2;
                    last.vy = from->vy + pts[1] * 2;
                    last.vz = from->vz + pts[2] * 2;
                    last.pad = 0;
                }
                t += i;
                if (n >= t) {
                    dst->vx = from->vx + pts[0] * 2;
                    dst->vy = from->vy + pts[1] * 2;
                    dst->vz = from->vz + pts[2] * 2;
                    dst->pad = width;
                    BTLFX_COPY_SVEC(&last, dst);
                    if (t == n && flag) {
                        BTLFX_COPY_SVEC(&p0, dst);
                        BTLFX_COPY_SVEC(&p1, dst);
                        p1.vy -= 0x1000;
                        func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &p1, &p0, 0, 0x800, 1, 0x32);
                    }
                } else {
                    BTLFX_COPY_SVEC(dst, &last);
                }
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E86C4[j * 4]), 0, (s32 *)from, 0, 0x1000, a,
                                1, (u8 *)tr, fade);
        }
        k++;
    } while (k < 3);
}
