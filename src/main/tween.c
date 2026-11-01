#include "ff9.h"

void tween_start(void *win, void *tween, s32 jump) {
    WinDef *w = win;
    TweenDef *def = tween;

    w->anim = def;
    w->frame = 0;
    w->delay = 0;
    if (def != NULL) {
        w->delay = def->delay;
        if ((u8)jump) {
            w->rect.x -= tween_sumDeltas(def->frames, def->dx);
            w->rect.y -= tween_sumDeltas(def->frames, def->dy);
        }
    }
}

s32 tween_step(WinDef *win) {
    if (win->anim == NULL) {
        return 0;
    }
    if (win->delay != 0) {
        win->delay--;
    } else {
        if (win->anim->dx != NULL) {
            win->rect.x += win->anim->dx[win->frame];
        }
        if (win->anim->dy != NULL) {
            win->rect.y += win->anim->dy[win->frame];
        }
        if (++win->frame < win->anim->frames) {
            return 0;
        }
        if (win->anim->onDone != NULL) {
            win->anim->onDone(win->anim);
        }
        win->anim = NULL;
        return 1;
    }
    return 0;
}

s32 tween_sumDeltas(s32 frames, s8 *p) {
    s32 sum = 0;
    s32 i;
    if (frames == 0 || p == NULL) {
        return 0;
    }
    for (i = 0; i < frames; i++) {
        sum += *p++;
    }
    return sum;
}
