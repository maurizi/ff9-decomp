/* STATUS: logic right, 75 rows. ovl_64 func_8019B7C4 (0x2AC, debug view of the CD ring: one POLY_F4 per sector). The original does not hoist buf * 0x780 out of the loop (recomputes sll/subu/sll in the loop) and keeps the colour pointer as one register with offsets 0/1/2; gcc 2.8.1 hoists the invariant and spills it. Tried 1-D/2-D indexing, an RGB struct, permuter (15 min, nothing useful). */
#include "ovl_64.h"
typedef struct {
    u8 r;
    u8 g;
    u8 b;
} Ovl64Rgb;
extern Ovl64Rgb D_8019DEAC[];
extern POLY_F4 D_8019F7F0[2][80];

void func_8019B7C4(u32 *ot, s32 buf) {
    POLY_F4 *p;
    Ovl64RingHdr *h;
    Ovl64Rgb *col;
    u8 i;
    s32 n;
    u8 r;
    u8 g;
    u8 b;

    i = 0;
    h = D_8019DE10.unk74;
    n = 0;
    if (D_8019DE10.unk60 != 0) {
        col = D_8019DEAC;
        do {
            r = col->r;
            g = col->g;
            b = col->b;
            if (h->unk0 & 0x8000) {
                if (r != 0) {
                    r += 0x60;
                }
                if (g != 0) {
                    g += 0x60;
                }
                if (b != 0) {
                    b += 0x60;
                }
            } else if (h->unk0 & 0x4000) {
                if (r != 0) {
                    r -= 0x20;
                }
                if (g != 0) {
                    g -= 0x20;
                }
                if (b != 0) {
                    b -= 0x20;
                }
            }
            p = &((POLY_F4 *)D_8019F7F0)[n + buf * 80];
            SetPolyF4(p);
            setXY4(p, i * 6 + 0x11, 0xBC, i * 6 + 0x16, 0xBC, i * 6 + 0x11, 0xC4, i * 6 + 0x16, 0xC4);
            setRGB0(p, r, g, b);
            addPrim(ot, p);
            n++;
            if (((i + 1) & 7) == 0) {
                col++;
                h++;
            }
            i++;
        } while (i < D_8019DE10.unk60);
    }
    p = &D_8019F7F0[buf][n];
    SetPolyF4(p);
    setRGB0(p, 0x10, 0x10, 0x10);
    setXY4(p, 0x10, 0xBB, 0x132, 0xBB, 0x10, 0xC5, 0x132, 0xC5);
    addPrim(ot, p);
}
