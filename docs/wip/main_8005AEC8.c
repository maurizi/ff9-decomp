/* 8 rows differ: twin of func_8005B1B8. c = D_8007BD48 is scheduled before the division here and after it in the original, and the (t->unk50 & 0xFFFF0000) registers are swapped. The second branch needs its own pointer t, copied to s at the end. */
#include "ff9.h"

void func_8005AEC8(Unk80080AA0 *p) {
    s32 n = 1;
    Unk80080A10 *s;
    Unk80080A10 *t;
    AkaoChannel *c;
    s32 v;
    s32 cur;

    if (p->unk4 != 0) {
        n = p->unk4;
    }
    v = ((p->unk8 & 0x7F) << 16) | 0x8000;
    if (p->unk0 == 0 || p->unk0 == D_80080A10->unk6A) {
        s = D_80080A10;
        do { } while (0);
        cur = (s->unk50 & 0xFFFF0000) | 0x8000;
        v = v - cur;
        v /= n;
        c = D_8007BD48;
    } else {
        t = D_8007F790;
        if (t == NULL || p->unk0 == 0 || p->unk0 != t->unk6A) {
            return;
        }
        cur = (t->unk50 & 0xFFFF0000) | 0x8000;
        v = v - cur;
        do { } while (0);
        v /= n;
        c = D_8007F71C;
        s = t;
    }
    s->unk58 = n;
    s->unk50 = cur;
    s->unk54 = v;
    func_8005A284(s, c);
}
