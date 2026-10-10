/* 2 rows differ: c = D_8007BD48 in the first branch is built as lui a1/addiu a1,a1 in the original (hi part in a1); we put the hi part in v0. The two do{}while(0) and reusing v for the quotient fixed the rest. */
#include "ff9.h"
void func_8005B1B8(Unk80080AA0 *p) {
    s32 n = 1;
    Unk80080A10 *s;
    AkaoChannel *c;
    s32 v;

    if (p->unk4 != 0) {
        n = p->unk4;
    }
    v = (p->unk8 & 0x7F) << 16;
    if (p->unk0 == 0 || p->unk0 == D_80080A10->unk6A) {
        s = D_80080A10;
        v = v - s->unk5C;
        do { } while (0);
        v /= n;
        c = D_8007BD48;
    } else {
        s = D_8007F790;
        if (s == NULL || p->unk0 == 0 || p->unk0 != s->unk6A) {
            return;
        }
        v = v - s->unk5C;
        do { } while (0);
        v /= n;
        c = D_8007F71C;
    }
    s->unk64 = n;
    s->unk60 = v;
    func_8005A284(s, c);
}
