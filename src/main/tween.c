#include "ff9.h"

void tween_start(void *arg0, void *arg1, s32 jump) {
    WinDef *w = arg0;
    TweenDef *t = arg1;

    w->anim = t;
    w->frame = 0;
    w->delay = 0;
    if (t != NULL) {
        w->delay = t->delay;
        if ((u8)jump) {
            w->rect.x -= tween_sumDeltas(t->frames, t->dx);
            w->rect.y -= tween_sumDeltas(t->frames, t->dy);
        }
    }
}

s32 tween_step(WinDef *t) {
    if (t->anim == NULL) {
        return 0;
    }
    if (t->delay != 0) {
        t->delay--;
    } else {
        if (t->anim->dx != NULL) {
            t->rect.x += t->anim->dx[t->frame];
        }
        if (t->anim->dy != NULL) {
            t->rect.y += t->anim->dy[t->frame];
        }
        if (++t->frame < t->anim->frames) {
            return 0;
        }
        if (t->anim->onDone != NULL) {
            t->anim->onDone(t->anim);
        }
        t->anim = NULL;
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
