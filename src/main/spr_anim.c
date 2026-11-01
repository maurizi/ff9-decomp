#include "ff9.h"

void sprAnim_drawCell(s32 x, s32 y, s32 otz, s32 unused, SprCell *d) {
    u32 t = d->clutTpage;
    SprtWords *s;

    setlen(g_primPtr, 4);
    s = (SprtWords *)g_primPtr;
    s->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    s->xy = ((x + d->x - 0x80) & 0xFFFF) | ((y + d->y - 0x80) << 16);
    s->uvclut = d->u | (d->v << 8) | ((u16)d->clutTpage << 16);
    g_primPtr = (u8 *)(s + 1);
    s->wh = d->w | (d->h << 16);
    addPrim(&g_ot[otz], s);
    SetDrawTPage(g_primPtr, 1, 1, ((t >> 16) & 0x1F) | (((t >> 21) & 3) << 5) | (((t >> 23) & 3) << 7));
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

void sprAnim_drawScaledCell(s32 x, s32 y, s32 otz, SprAnim *unused, AnimCell *c) {
    u32 t;
    s32 sw;
    u32 tp;
    s32 sh;
    PolyFT4Words *p;

    t = c->cell.clutTpage;
    sw = (c->cell.w * c->scaleX) >> 12;
    tp = ((t >> 16) & 0x1F) | (((t >> 21) & 3) << 5) | (((t >> 23) & 3) << 7);
    sh = (c->cell.h * c->scaleY) >> 12;
    x = x + c->cell.x - 0x80 + ((c->cell.w - sw) >> 1);
    y = y + c->cell.y - 0x80 + ((c->cell.h - sh) >> 1);
    setlen(g_primPtr, 9);
    p = (PolyFT4Words *)g_primPtr;
    p->rgbc = GPU_RGBC(GPU_POLY_FT4, 0x808080);
    p->xy0 = (x & 0xFFFF) | (y << 16);
    p->xy1 = ((x + sw) & 0xFFFF) | (y << 16);
    p->xy2 = (x & 0xFFFF) | ((y + sh) << 16);
    p->xy3 = ((x + sw) & 0xFFFF) | ((y + sh) << 16);
    p->uv0clut = c->cell.u | (c->cell.v << 8) | ((u16)c->cell.clutTpage << 16);
    p->uv1tpage = (c->cell.u + c->cell.w - 1) | (c->cell.v << 8) | (tp << 16);
    p->uv2 = c->cell.u | ((c->cell.v + c->cell.h - 1) << 8);
    p->uv3 = (c->cell.u + c->cell.w - 1) | ((c->cell.v + c->cell.h - 1) << 8);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
}

void sprAnim_init(SprAnim *anim, SprAnimData *data) {
    anim->data = data;
    anim->frames = data->frames;
    anim->cellLists = &anim->frames[anim->data->frameCount];
    anim->base = data;
    anim->playing = 0;
    anim->frame = 0;
    anim->tick = 0;
    anim->onDone = 0;
    anim->frameCount = data->frameCount;
}

u8 sprAnim_setPlaying(SprAnim *anim, u8 playing) {
    u8 old = anim->playing;
    anim->playing = playing;
    return old;
}

void sprAnim_setOnDone(SprAnim *a, void (*b)()) {
    a->onDone = b;
}

void sprAnim_reset(SprAnim *anim) {
    anim->playing = 0;
    anim->frame = 0;
    anim->tick = 0;
    anim->onDone = 0;
}

void sprAnim_update(SprAnim *anim) {
    SprAnimFrame *f = &anim->frames[anim->frame];

    if (anim->playing != 0) {
        if (++anim->tick >= f->ticks - 1) {
            anim->tick = 0;
            if (++anim->frame >= anim->frameCount - 1) {
                anim->playing = 0;
                if (anim->onDone != NULL) {
                    anim->onDone(anim);
                }
            }
        }
    }
}

void sprAnim_draw(s32 x, s32 y, s32 otz, SprAnim *anim) {
    s32 i;
    SprAnimFrame *f = &anim->frames[anim->frame];
    AnimCellList *cl = &((AnimCellList *)anim->cellLists)[f->cellList];
    AnimCell *c = (AnimCell *)((u8 *)anim->base + cl->offset);

    for (i = 0; i < cl->count; i++) {
        if (!((c->cell.clutTpage >> 26) & 1)) {
            sprAnim_drawCell(x + f->x, y + f->y, otz, (s32)anim, &c->cell);
        } else {
            sprAnim_drawScaledCell(x + f->x, y + f->y, otz, anim, c);
        }
        c++;
    }
}
