/* 11 rows differ: unkD/unk9 land in v0/v1 instead of v1/v0 in the second (unk8 & 2 == 0) branch, and its unk2 = 6 store is scheduled earlier. Reading back v->unk0 (instead of a u8 n temp) fixed the first branch: CSE turns the reload into andi v0,a0,0xFF like the original. */
#include "ff9.h"
MenuView *func_8002ECA0(MenuList *m, MenuView *v) {
    u8 n;

    if (m->unk8 & 4) {
        if (m->unk8 & 2) {
            v->unk0 = (m->unkD - m->unk9 + 8) / 15;
            v->unk1 = m->unk5 + (m->unk3 - v->unk0);
            v->unk2 = (m->unk3 - v->unk0) * 15 + 6;
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
