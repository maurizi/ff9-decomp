/* WIP func_800AA504: 3 differing rows (build/cmp.py-style aligned diff). Pass 7: the case 2 header store order (unkA, unkC, then the two -1 words) fixed the collect-loop registers. Left: three pre-header insns in load delay slots: the original has li t1,2 (hoisted type constant), addiu t0,s5,16, move a3,t0 (q copied from the hoisted base); ours computes q first (addiu a3) and copies t0 from it. Moving the q init, dropping dst2, do-while insertion: no change. Pass 8: dst2 = arg2->unk10.h before the stores with q = dst2 after them gets the t0/a3 order right but moves li t1,2 later (5 rows); lo/dst2 copies, a grid of dst2/q placements and 9 min of permuter: 3 rows at best. */
#include "ovl_0b.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} Unk800AA504Pos8;

s32 func_800AA504(s32 arg0, s32 arg1, Unk800AA504 *arg2) {
    Unk800AA504 *st;
    Battle *b;
    Unk800AA958Req *req;
    FileTable *t;
    FileEntryX *first;
    FileEntryX *e;
    s16 *q;
    s16 *s;
    u32 w;
    u32 best;
    s16 *bp;
    FileEntry *bf;
    s16 *p;
    FileEntry *f;
    s32 sp1C;
    s32 dst;
    Unk800AA504Tbl *tbl;
    Unk800AA504Ent *te;
    Unk800AA504Pos *a;
    Unk800AA504Pos8 *r;
    Unk800AA504Ent *tf;
    Unk800AA504Pos *bb;
    s32 n;
    s32 h;
    s32 x;
    u16 id;
    s16 *lo;
    s16 *dst2;

    st = arg2;
    b = D_8006794C->unk1C->unk8E4.battle;
    req = (Unk800AA958Req *)&b->unk11C0->unkC[0x9FA0 / 4];
    if (arg1 & 0x8000) {
        b->unk0 &= ~2;
        st->unk0 = 2;
    }
loop:
    switch (st->unk0) {
    case 1:
        if (func_80022B18() != 0) {
            break;
        }
        st->unk0 = arg2->unk2;
        goto loop;
    case 2:
        t = D_8006794C->unk1C->unk7E4;
        q = arg2->unk10.h;
        arg2->unkA = 0;
        arg2->unkC = b->unk153C[0];
        arg2->unk10.w[0] = -1;
        arg2->unk10.w[1] = -1;
        first = (FileEntryX *)t->unk4;
        e = &first[t->unk2] - 1;
        do {
            w = e->unk0.w;
            if (e->unk0.b.type == 2) {
                w &= 0xFFFF;
                s = q - 1;
                do {
                    if (*s == w) {
                        goto next;
                    }
                    s--;
                } while (s >= arg2->unk10.h);
                *q++ = w;
            }
        next:
            e--;
        } while (e >= first);
        st->unk0 = 4;
        goto loop;
    case 4:
        best = -1;
        bp = NULL;
        bf = NULL;
        lo = arg2->unk10.h;
        p = &arg2->unk10.h[2];
        do {
            id = *p;
            if (id != 0xFFFF) {
                f = func_80063088(D_8006794C->unk1C->unk7E4, id | 0x20000);
                if ((u32)f->unk4 < best) {
                    best = f->unk4;
                    bf = f;
                    bp = p;
                }
            }
            p--;
        } while (p >= lo);
        if (bp != NULL) {
            arg2->unk8 = *bp;
            *bp = -1;
            req->unk0 = bf->unkC << 11;
            req->unk4 = (s32)b->unk11C0->unk48;
            req->unk8 = (bf->unk8 - bf->unkC) << 11;
            req->unkC = arg2->unkC;
            func_80022CC4(D_8006794C->unk20 + best, req, 2, 0);
            arg2->unk2 = 6;
            st->unk0 = 1;
        } else {
            st->unk0 = 7;
        }
        break;
    case 6:
        sp1C = func_8001C8B0((s32)b->unk11C0->unk48, 2, 0x1B0000, 0);
        dst = arg2->unkC;
        tbl = (Unk800AA504Tbl *)func_8001C8B0((s32)b->unk1354->unk0[2], 2, 0x120000, 0);
        tf = tbl->unk4;
        te = &tf[tbl->unk1] - 1;
        do {
            if (te->unk0 == arg2->unk8) {
                n = te->unk4.b[3];
                a = (Unk800AA504Pos *)((u8 *)te + (te->unk4.w & 0xFFFFFF));
                bb = a + n;
                r = (Unk800AA504Pos8 *)(bb + n);
                while (n > 0) {
                    n--;
                    x = func_8001C8B0(dst, 2, (n & 0xFFFF) | 0x40000, 0);
                    h = func_8001DA60(a[n].unk2);
                    func_8001D9CC(x, h, func_8001DA6C(a[n].unk2), r[n].unk0, r[n].unk2);
                    func_8001D8A8(x);
                }
                break;
            }
            te--;
        } while (te >= tf);
        D_80067948 = sp1C + func_8001CA70(sp1C);
        b->unk11C0->unk48 = (u8 *)D_80067948;
        arg2->unkC += func_8001CA70(dst);
        *(b->unk1354->unk10 + arg2->unkA) = (void *)sp1C;
        func_8001C750(sp1C, 0);
        arg2->unkA++;
        st->unk0 = 4;
        goto loop;
    case 7:
        b->unk0 |= 2;
        st->unk0 = 0;
        break;
    }
    return st->unk0;
}
