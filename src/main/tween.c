#include "ff9.h"

void tween_start(void *arg0, void *arg1, s32 jump) {
    WinDef *w = arg0;
    TweenDef *t = arg1;

    w->unkC = t;
    w->unk2 = 0;
    w->unk0 = 0;
    if (t != NULL) {
        w->unk0 = t->unk0;
        if ((u8)jump) {
            w->unk4.x -= tween_sumDeltas(t->unk2, t->unk4);
            w->unk4.y -= tween_sumDeltas(t->unk2, t->unk8);
        }
    }
}

s32 tween_step(WinDef *t) {
    if (t->unkC == NULL) {
        return 0;
    }
    if (t->unk0 != 0) {
        t->unk0--;
    } else {
        if (t->unkC->unk4 != NULL) {
            t->unk4.x += t->unkC->unk4[t->unk2];
        }
        if (t->unkC->unk8 != NULL) {
            t->unk4.y += t->unkC->unk8[t->unk2];
        }
        if (++t->unk2 < t->unkC->unk2) {
            return 0;
        }
        if (t->unkC->unkC != NULL) {
            t->unkC->unkC(t->unkC);
        }
        t->unkC = NULL;
        return 1;
    }
    return 0;
}

s32 tween_sumDeltas(s32 n, s8 *p) {
    s32 sum = 0;
    s32 i;
    if (n == 0 || p == NULL) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        sum += *p++;
    }
    return sum;
}
