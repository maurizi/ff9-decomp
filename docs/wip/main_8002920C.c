/* STATUS: 108 rows (cmp.py), 471 vs 468 words. Glyph path split into two func_8002B7B0 calls with a code temp read from s[1] (as in 8002995C); pkt = func_8002BA90(...) as its own statement in cases 0x6B and 0x4B (the other three stay nested) puts x in s3. Open: (1) the original cross-jumps the x += func_8002B9B8/8002AE7C/8002BA10(...) tails of 0x6A/0x6B, 0x6E/0x6F, 0x6C/0x6D (the first of each pair jumps into the second's jal); ours keeps both calls (the 3 extra words). (2) pkt is s1 and s is s2 in the original (swapped here). (3) the original keeps %hi(D_800770AC) in fp and spills font to 0x30(sp) (same pattern as 8005D6BC); ours keeps font in s8. */
#include "ff9.h"

extern u8 D_80077080[];
extern u8 *D_800770A8;
s32 func_8002B9B8(u8 *s);
s32 func_8002BA10(u8 *s);

u8 *func_8002920C(s32 x, s32 y, u32 *ot, u8 *s, s32 mode, u8 *pkt) {
    s32 w;
    s32 x0;
    u8 th;
    Unk7D4 *font;
    s32 c;
    s32 k;

    font = D_8006794C->unk1C->unk7D4;
    D_800770AC = mode;
    x0 = x;
    th = font->unk4 >> 8;
    while ((c = *s) != 0xFF) {
        if (c == 0xF9) {
            s++;
            if (*s == 0x43) {
                s++;
                x += (s8)*s;
            } else if (*s == 0x44) {
                s++;
                x = x0 + *s;
            } else if (*s == 0x49) {
                s++;
                y += *s;
            } else if (*s == 0x4A) {
                s++;
                y -= *s;
            } else if (*s == 0x40) {
                s++;
                func_8002BBE8(D_8007BBA8[*s & 7], D_80077080, 0);
                pkt = func_8002995C(x, y, ot, (Character *)D_80077080, &w, pkt);
                x += w;
            } else if (*s == 0x68) {
                s++;
                func_8002BBE8(D_800770F0[*s], D_80077080, 0);
                pkt = func_8002995C(x, y, ot, (Character *)D_80077080, &w, pkt);
                x += w;
            } else if (*s == 0x69) {
                s++;
                pkt = func_8002995C(x, y, ot, (Character *)D_800770B0[*s], &w, pkt);
                x += w;
            } else if (*s == 0x6A) {
                s++;
                func_8002BBE8(D_800770F0[*s], D_80077080, 1);
                pkt = func_80029D60(x, y, ot, D_80077080, D_800770AC, pkt);
                x += func_8002B9B8(D_80077080);
            } else if (*s == 0x6B) {
                s++;
                D_800770A8 = D_800770B0[*s];
                pkt = func_80029D60(x, y, ot, D_800770A8, D_800770AC, func_8002BA90(ot, (DrTpageWords *)pkt));
                x += func_8002B9B8(D_800770A8);
            } else if (*s == 0x6E) {
                s++;
                func_8002BBE8(D_800770F0[*s], D_80077080, 1);
                pkt = func_80029B4C(x, y, ot, (s32)D_80077080, D_800770AC, pkt);
                x += func_8002AE7C(D_80077080);
            } else if (*s == 0x6F) {
                s++;
                D_800770A8 = D_800770B0[*s];
                pkt = func_80029B4C(x, y, ot, (s32)D_800770A8, D_800770AC, pkt);
                x += func_8002AE7C(D_800770A8);
            } else if (*s == 0x6C) {
                s++;
                func_8002BBE8(D_800770F0[*s], D_80077080, 1);
                pkt = func_8002B644(x, y, ot, D_80077080, D_800770AC, pkt);
                x += func_8002BA10(D_80077080);
            } else if (*s == 0x6D) {
                s++;
                D_800770A8 = D_800770B0[*s];
                pkt = func_8002B644(x, y, ot, D_800770A8, D_800770AC, pkt);
                x += func_8002BA10(D_800770A8);
            } else if (*s == 0x4B) {
                pkt = func_80027834(x, y, ot, *++s, 0, &w, func_8002BA90(ot, (DrTpageWords *)pkt));
                x += w;
            } else if (*s == 0x4C) {
                s++;
                k = *s;
                if (k != 0xFF) {
                    pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
                    pkt = func_80026C7C(x, y, ot, k, 1, pkt);
                    x += D_800686D0[D_80069018[k].unk8 + 0x5C].unk4;
                }
            } else if (*s == 0x4D) {
                pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
                pkt = func_80027834(x, y, ot, D_80077130[*++s], 0, &w, pkt);
                x += w;
            } else if (*s == 0x4E) {
                s++;
                k = D_80077130[*s];
                if (k != 0xFF) {
                    pkt = func_80026C7C(x, y, ot, k, 1, func_8002BA90(ot, (DrTpageWords *)pkt));
                    x += D_800686D0[D_80069018[k].unk8 + 0x5C].unk4;
                }
            } else {
                pkt = func_80029FE4(x, y, ot, *s, &w, pkt);
                x += w;
            }
        } else if (c == 0xF7) {
            x = x0;
            y += 15;
        } else {
            if (c >= th) {
                k = (c << 8) + s[1];
                s++;
                c = k - font->unk4 + font->unk6;
                pkt = func_8002B7B0(x, y, ot, *(font->unkC + c), D_800770AC, &w, (SprtWords *)pkt);
            } else {
                pkt = func_8002B7B0(x, y, ot, *(font->unkC + c), D_800770AC, &w, (SprtWords *)pkt);
            }
            x += w;
        }
        s++;
    }
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = 0xE100061E;
    addPrim(ot, pkt);
    return pkt + sizeof(DrTpageWords);
}
