#include "ff9.h"

void func_800357E0(s32 x, s32 y, s32 otz, s32 unused, Unk800357E0 *d) {
    u32 t = d->unk4;
    SprtWords *s;

    setlen(g_primPtr, 4);
    s = (SprtWords *)g_primPtr;
    s->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    s->xy = ((x + d->unk2 - 0x80) & 0xFFFF) | ((y + d->unk3 - 0x80) << 16);
    s->uvclut = d->unk0 | (d->unk1 << 8) | ((u16)d->unk4 << 16);
    g_primPtr = (u8 *)(s + 1);
    s->wh = d->unk8 | (d->unkA << 16);
    addPrim(&g_ot[otz], s);
    SetDrawTPage(g_primPtr, 1, 1, ((t >> 16) & 0x1F) | (((t >> 21) & 3) << 5) | (((t >> 23) & 3) << 7));
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

void func_8003595C(s32 x, s32 y, s32 otz, Unk80035B90 *unused, AnimCell *c) {
    u32 t;
    s32 sw;
    u32 tp;
    s32 sh;
    PolyFT4Words *p;

    t = c->unk0.unk4;
    sw = (c->unk0.unk8 * c->unk10) >> 12;
    tp = ((t >> 16) & 0x1F) | (((t >> 21) & 3) << 5) | (((t >> 23) & 3) << 7);
    sh = (c->unk0.unkA * c->unk12) >> 12;
    x = x + c->unk0.unk2 - 0x80 + ((c->unk0.unk8 - sw) >> 1);
    y = y + c->unk0.unk3 - 0x80 + ((c->unk0.unkA - sh) >> 1);
    setlen(g_primPtr, 9);
    p = (PolyFT4Words *)g_primPtr;
    p->rgbc = GPU_RGBC(GPU_POLY_FT4, 0x808080);
    p->xy0 = (x & 0xFFFF) | (y << 16);
    p->xy1 = ((x + sw) & 0xFFFF) | (y << 16);
    p->xy2 = (x & 0xFFFF) | ((y + sh) << 16);
    p->xy3 = ((x + sw) & 0xFFFF) | ((y + sh) << 16);
    p->uv0clut = c->unk0.unk0 | (c->unk0.unk1 << 8) | ((u16)c->unk0.unk4 << 16);
    p->uv1tpage = (c->unk0.unk0 + c->unk0.unk8 - 1) | (c->unk0.unk1 << 8) | (tp << 16);
    p->uv2 = c->unk0.unk0 | ((c->unk0.unk1 + c->unk0.unkA - 1) << 8);
    p->uv3 = (c->unk0.unk0 + c->unk0.unk8 - 1) | ((c->unk0.unk1 + c->unk0.unkA - 1) << 8);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
}

void func_80035B24(Unk80035B90 *a, Unk80035B90Data *d) {
    a->unk4 = d;
    a->unk8 = d->unk4;
    a->unkC = &a->unk8[a->unk4->unk0];
    a->unk10 = d;
    a->unk0 = 0;
    a->unk1 = 0;
    a->unk2 = 0;
    a->unk14 = 0;
    a->unk3 = d->unk0;
}

u8 func_80035B64(Unk80035B90 *a, u8 b) {
    u8 old = a->unk0;
    a->unk0 = b;
    return old;
}

void func_80035B74(Unk80035B90 *a, void (*b)()) {
    a->unk14 = b;
}

void func_80035B7C(Unk80035B90 *a) {
    a->unk0 = 0;
    a->unk1 = 0;
    a->unk2 = 0;
    a->unk14 = 0;
}

void func_80035B90(Unk80035B90 *arg0) {
    Unk80035B90Frame *f = &arg0->unk8[arg0->unk1];

    if (arg0->unk0 != 0) {
        if (++arg0->unk2 >= f->unk2 - 1) {
            arg0->unk2 = 0;
            if (++arg0->unk1 >= arg0->unk3 - 1) {
                arg0->unk0 = 0;
                if (arg0->unk14 != NULL) {
                    arg0->unk14(arg0);
                }
            }
        }
    }
}

void func_80035C2C(s32 x, s32 y, s32 otz, Unk80035B90 *a) {
    s32 i;
    Unk80035B90Frame *f = &a->unk8[a->unk1];
    AnimCellList *cl = &((AnimCellList *)a->unkC)[f->unk0];
    AnimCell *c = (AnimCell *)((u8 *)a->unk10 + cl->unk2);

    for (i = 0; i < cl->unk0; i++) {
        if (!((c->unk0.unk4 >> 26) & 1)) {
            func_800357E0(x + f->unk4, y + f->unk6, otz, (s32)a, &c->unk0);
        } else {
            func_8003595C(x + f->unk4, y + f->unk6, otz, a, c);
        }
        c++;
    }
}
