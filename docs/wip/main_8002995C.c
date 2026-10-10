/* 4 rows differ: only the x += adv tail. The original loads adv into v1 right after the jal and puts the add in the bne delay slot; we load it into v0 after the pkt copy. Needed: three separate func_8002B7B0 calls (jump2 cross-jumps them at the lw a3,0xC(v0)), a code temp read from s[1] before s++, *(tbl->unkC + c) instead of tbl->unkC[c] (operand order of the addu), and an identical-branch if (x) around the trailing s++ (replaces the old do { } while (0) around x += adv, 5 rows). Pass 7: second-level dup/wrap sweeps, x += adv before s++, for-loop increments and the permuter (best 6) did not move the adv load. */
#include "ff9.h"

u8 *func_8002995C(s32 x, s32 y, u32 *ot, Character *ch, s32 *w, u8 *pkt) {
    s32 adv;
    s32 x0;
    u8 th;
    Unk7D4 *font;
    Unk7D4 *kanji;
    s32 th2;
    u8 *s;
    s32 c;
    s32 code;

    font = D_8006794C->unk1C->unk7D4;
    s = ch->unk0;
    x0 = x;
    th = font->unk4 >> 8;
    kanji = D_8006794C->unk1C->unk84C;
    th2 = 0;
    if (kanji != NULL) {
        th2 = kanji->unk0 >> 8;
    }
    while ((c = *s) != 0xFF) {
        if (kanji != NULL && c >= th2) {
            code = (c << 8) + s[1];
            s++;
            c = code - kanji->unk0;
            pkt = func_8002B7B0(x, y, ot, *(kanji->unkC + c), D_800770AC, &adv, (SprtWords *)pkt);
        } else if (c >= th) {
            code = (c << 8) + s[1];
            s++;
            c = code - font->unk4 + font->unk6;
            pkt = func_8002B7B0(x, y, ot, *(font->unkC + c), D_800770AC, &adv, (SprtWords *)pkt);
        } else {
            pkt = func_8002B7B0(x, y, ot, *(font->unkC + c), D_800770AC, &adv, (SprtWords *)pkt);
        }
        if (x) {
            s++;
        } else {
            s++;
        }
        x += adv;
    }
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = 0xE100061E;
    addPrim(ot, pkt);
    *w = x - x0;
    return pkt + 8;
}
