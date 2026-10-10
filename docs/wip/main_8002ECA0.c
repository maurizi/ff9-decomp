/* 13 rows differ: registers only. The original loads unkD into v1 and unk9 into v0 for (unkD - unk9 + 8) / 15 (both copies), and the (u8) n extension lands in v0. No expression form, n type, statement order or do{}while(0) changed it. */
#include "ff9.h"
MenuView *func_8002ECA0(MenuList *m, MenuView *v) {
    u8 n;

    if (m->unk8 & 4) {
        if (m->unk8 & 2) {
            n = (m->unkD - m->unk9 + 8) / 15;
            v->unk0 = n;
            v->unk1 = m->unk5 + (m->unk3 - n);
            v->unk2 = (m->unk3 - n) * 15 + 6;
        } else {
            v->unk0 = (m->unkD - m->unk9 + 8) / 15;
            v->unk2 = 6;
            v->unk1 = m->unk5;
        }
    } else if (m->unk8 & 2) {
        v->unk0 = m->unk3 + 1;
        v->unk1 = m->unk5 - 1;
        v->unk2 = m->unk9 - 9;
    } else if (m->unk8 & 1) {
        v->unk1 = m->unk5;
        v->unk0 = m->unk3 + 1;
        v->unk2 = -m->unk9 + 6;
    } else {
        v->unk1 = m->unk5;
        v->unk0 = m->unk3;
        v->unk2 = 6;
    }
    return v;
}
