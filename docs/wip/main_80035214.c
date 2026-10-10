/* 13 rows differ, registers only: in both compare blocks the s16 load and the (c->y + 16) temp land in a0/a1 the other way round. Needed: temps (t = c->y + 16; d2 = m->unk2 - t; d1 = ...; d1 -= m->unk2) so fold does not reassociate the constants, (s16) cast in m->unk2 - (s16)(h + 16), and r->x + r->w > c->x + c->w operand order. &D_80077DE8.unk10 is the D_80077DF8 symbol (same address, s3 = s2 - 0x10 comes from CSE). */
#include "ff9.h"

void func_80035214(Unk80035214 *w) {
    RECT *r = &w->unk0;
    RECT *c = &D_80077DE8.unk10;
    Unk80077DE8 *m = &D_80077DE8;
    TextSize sz;
    s32 x;
    s32 d1;
    s32 d2;
    s32 t;

    sz = func_8002AF70(m->unk8);
    w->unk0.w = sz.w + 12;
    w->unk0.h = sz.h + 12;
    if (w->unk0.w >= c->w) {
        w->unk0.w = c->w;
    }
    if (w->unk0.h >= c->h) {
        w->unk0.h = c->h;
    }
    if (m->unk1C == 0) {
        w->unk8 = 4;
        w->unk0.x = m->unk0;
        w->unk0.y = m->unk2;
        return;
    }
    t = c->y + 16;
    d2 = m->unk2 - t;
    d1 = c->y + c->h - 10;
    d1 -= m->unk2;
    if (d1 < d2) {
        w->unk8 = 2;
        w->unk0.y = m->unk2 - (s16)(w->unk0.h + 16);
    } else {
        w->unk8 = 0;
        w->unk0.y = m->unk2 + 10;
    }
    t = c->x + 19;
    d2 = D_80077DE8.unk0 - t;
    d1 = c->x + c->w - 18;
    d1 -= D_80077DE8.unk0;
    if (d1 < d2) {
        w->unk8 |= 1;
        x = D_80077DE8.unk0 - 19;
    } else {
        x = D_80077DE8.unk0 + 18;
    }
    r->x = x - (r->w >> 1);
    if (r->x < c->x) {
        r->x = c->x;
    }
    if (r->x + r->w > c->x + c->w) {
        r->x = c->x + c->w - r->w;
    }
    if (x - r->x < 8) {
        w->unkA = 8;
    } else {
        w->unkA = x - r->x;
    }
}
