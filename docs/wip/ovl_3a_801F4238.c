/* STATUS: 63 rows. ovl_3a func_801F4238 (0x364, name entry: underline cursor and character grid). The original uses explicit offset inductions (0x3E/+16, 0x3F/+16, 0x22/+21, 0x3E/+14) and computes y as r->y + (j + 0x15); left: it materialises 0xB0 inside the loop (only 0x40 hoisted) and copies xo into s5 before the inner loop. */
#include "ovl_3a.h"

void func_801F4238(void) {
    LINE_F2 *p;
    s32 cursor;
    s32 i;
    s32 j;
    s32 k;
    s32 xo;
    s32 yo;
    s32 y;

    k = 0;
    cursor = 6;
    if (D_801F4BBC.unk4 < 6) {
        cursor = D_801F4BBC.unk4;
    }
    func_8002B524(D_801F4B64->x + 0x23, D_801F4B64->y - 4, 5, D_8007047C, 0);
    func_800269F4(D_801F4B64->x + 4, D_801F4B64->y + 0x1A, 6, D_801F4B64->w - 8);
    func_8002DEE4(D_801F4B64, 6, 1);
    func_8002705C(D_801F4B64, 6, 1);
    for (i = 0, xo = 0x3E; i < 7; i++, xo += 16) {
        if (i != cursor || (D_80077DB0.unk10 & 4)) {
            for (j = 0; j < 2; j++) {
                p = (LINE_F2 *)D_80077170;
                SetLineF2(p);
                if (j == 0) {
                    setRGB0(p, 0xB0, 0xB0, 0xB0);
                } else {
                    setRGB0(p, 0x40, 0x40, 0x40);
                }
                y = j + 0x15;
                p->x0 = D_801F4B64->x + xo + j;
                p->y1 = p->y0 = D_801F4B64->y + y;
                p->x1 = p->x0 + 0xB;
                D_80077170 += sizeof(LINE_F2);
                addPrim(&D_8007716C[5], p);
            }
        }
    }
    for (i = 0, xo = 0x3F; i < D_801F4BBC.unk4; i++, xo += 16) {
        func_80028F3C(D_801F4B64->x + xo, D_801F4B64->y + 7, 5, D_801F4BBC.unk7[i], 0);
    }
    for (i = 0, yo = 0x22; i < (u8)(D_801F4BBC.unk5[D_801F4BBC.unk2] / 15); i++, yo += 21) {
        for (j = 0, xo = 0x3E; j < 15; j++, xo += 14) {
            func_80028F3C(D_801F4B64->x + xo, D_801F4B64->y + yo, 5,
                          D_801F4BBC.unk1C[D_801F4BBC.unk2][k++], 0);
        }
    }
    D_80077170 = func_8002BA90(&D_8007716C[5], D_80077170);
}
