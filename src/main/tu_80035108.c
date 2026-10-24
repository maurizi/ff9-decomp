#include "ff9.h"

void func_80035108(void) {
    s32 otz;
    Unk80035214 r;

    otz = 1;
    if (D_80077DE8.unk1D == 0) {
        otz = D_80077DE8.unk4;
    }
    do {
    } while (0); /* empty debug macro in the original; changes the register allocation */
    if (D_80077DE8.unk1C != 0) {
        if (D_80077DE8.unk1D != 0) {
            ui_drawHand(D_80077DE8.unk0, D_80077DE8.unk2, otz);
        }
        func_80035C2C(D_80077DE8.unk0 + 5, D_80077DE8.unk2 + 2, otz, &D_80077DE8.unk20);
    }
    func_8003573C();
    if (D_80077DE8.unk1A != 0) {
        func_80035214(&r);
        ui_drawSprite(r.unk0.x + 8, r.unk0.y, otz, 0xB7, 0);
        text_drawBlock(r.unk0.x + 6, r.unk0.y + 8, otz, D_80077DE8.unk8, 8);
        win_drawHelpFrame(&r.unk0, otz, r.unk8, r.unkA);
    }
}

void func_80035214(Unk80035214 *w) {
    RECT *r = &w->unk0;
    RECT *c = &D_80077DE8.unk10;
    Unk80077DE8 *m = &D_80077DE8;
    TextSize sz;
    s32 x;
    s32 v;
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
    v = m->unk2;
    t = c->y + 16;
    d2 = v - t;
    d1 = c->y + c->h - 10;
    d1 -= v;
    if (d1 < d2) {
        w->unk8 = 2;
        w->unk0.y = m->unk2 - (s16)(w->unk0.h + 16);
    } else {
        w->unk8 = 0;
        w->unk0.y = m->unk2 + 10;
    }
    v = D_80077DE8.unk0;
    t = c->x + 19;
    d2 = v - t;
    d1 = c->x + c->w - 18;
    d1 -= v;
    if (d1 < d2) {
        w->unk8 |= 1;
        x = D_80077DE8.unk0 - 19;
    } else {
        x = v + 18;
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

void func_80035430(void) {
    D_80077DE8.unk18 = 0;
    D_80077DE8.unkC = 0;
    D_80077DE8.unk8 = 0;
    D_80077DE8.unk1A = 0;
    D_80077DE8.unk19 = 0;
    D_80077DE8.unk1B = 0;
    D_80077DE8.unk1C = 1;
    D_80077DE8.unk1D = 1;
    func_80035580(NULL);
    func_80035B24(&D_80077DE8.unk20, &D_80010E48);
}

void func_80035490(void) {
    Unk80077DE8 *p;
    if (D_80077DE8.unk1B == 0 && func_8002F810() == 0 && (g_padRepeat & 0x8000000)) {
        func_80035680();
    }
    p = &D_80077DE8;
    if (p->unk18 != 0 && func_8002F810() == 0 && p->unkC != NULL) {
        p->unkC();
        if (p->unk8 != NULL && *p->unk8 != 0xFF) {
            func_80035108();
        }
    }
    func_80035B90(&D_80077DE8.unk20);
}

void func_80035564(void) {
}

void func_8003556C(void (*a)(void)) {
    D_80077DE8.unk8 = 0;
    D_80077DE8.unkC = a;
}

void func_80035580(RECT *r) {
    if (r == NULL) {
        D_80077DE8.unk10.x = 8;
        D_80077DE8.unk10.y = 8;
        D_80077DE8.unk10.w = 0x131;
        D_80077DE8.unk10.h = 0xD1;
    } else {
        D_80077DE8.unk10 = *r;
    }
}

void func_800355DC(void) {
}

void func_800355E4(Unk80077DE8 *unused) {
    ui_drawSprite(D_80077DE8.unk0, D_80077DE8.unk2, D_80077DE8.unk4, 1, 0);
}

u8 func_8003561C(void) {
    return D_80077DE8.unk18;
}

u8 func_8003562C(void) {
    return D_80077DE8.unk1B;
}

void func_8003563C(u8 a) {
    D_80077DE8.unk1B = a;
}

u8 func_80035648(void) {
    return D_80077DE8.unk1C;
}

void func_80035658(u8 a) {
    D_80077DE8.unk1C = a;
}

u8 func_80035664(void) {
    return D_80077DE8.unk1D;
}

void func_80035674(u8 a) {
    D_80077DE8.unk1D = a;
}

void func_80035680(void) {
    if (D_80077DE8.unk19 == 0 && D_80077DE8.unkC != NULL) {
        if (D_80077DE8.unk18 == 0) {
            snd_playSe(0x2AA);
            D_80077DE8.unk18 = 1;
            D_80077DE8.unk19 = 1;
            D_80077DE8.unk1A = 0;
            func_80035B7C(&D_80077DE8.unk20);
            func_80035B74(&D_80077DE8.unk20, func_80035744);
            func_80035B64(&D_80077DE8.unk20, 1);
        } else {
            snd_playSe(0x65);
            D_80077DE8.unk18 = 0;
            D_80077DE8.unk1A = 0;
            func_80035B64(&D_80077DE8.unk20, 0);
        }
    }
}

void func_8003573C(void) {
}

void func_80035744(void) {
    D_80077DE8.unk1A = 1;
    D_80077DE8.unk19 = 0;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/tu_80035108", D_80010E48);
