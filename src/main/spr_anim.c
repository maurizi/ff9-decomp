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

void sprAnim_init(SprAnim *a, SprAnimData *d) {
    a->data = d;
    a->frames = d->frames;
    a->cellLists = &a->frames[a->data->frameCount];
    a->base = d;
    a->playing = 0;
    a->frame = 0;
    a->tick = 0;
    a->onDone = 0;
    a->frameCount = d->frameCount;
}

u8 sprAnim_setPlaying(SprAnim *a, u8 b) {
    u8 old = a->playing;
    a->playing = b;
    return old;
}

void sprAnim_setOnDone(SprAnim *a, void (*b)()) {
    a->onDone = b;
}

void sprAnim_reset(SprAnim *a) {
    a->playing = 0;
    a->frame = 0;
    a->tick = 0;
    a->onDone = 0;
}

void sprAnim_update(SprAnim *arg0) {
    SprAnimFrame *f = &arg0->frames[arg0->frame];

    if (arg0->playing != 0) {
        if (++arg0->tick >= f->ticks - 1) {
            arg0->tick = 0;
            if (++arg0->frame >= arg0->frameCount - 1) {
                arg0->playing = 0;
                if (arg0->onDone != NULL) {
                    arg0->onDone(arg0);
                }
            }
        }
    }
}

void sprAnim_draw(s32 x, s32 y, s32 otz, SprAnim *a) {
    s32 i;
    SprAnimFrame *f = &a->frames[a->frame];
    AnimCellList *cl = &((AnimCellList *)a->cellLists)[f->cellList];
    AnimCell *c = (AnimCell *)((u8 *)a->base + cl->offset);

    for (i = 0; i < cl->count; i++) {
        if (!((c->cell.clutTpage >> 26) & 1)) {
            sprAnim_drawCell(x + f->x, y + f->y, otz, (s32)a, &c->cell);
        } else {
            sprAnim_drawScaledCell(x + f->x, y + f->y, otz, a, c);
        }
        c++;
    }
}
