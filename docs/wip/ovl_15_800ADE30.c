/* STATUS: near miss, 28 rows. ovl_15 func_800ADE30 (0x160, draw a text string glyph by glyph, then a DR_TPAGE). The original has two func_800AE028 calls (one per glyph-code branch, merged by cross-jumping after the argument setup), signed compares (s32 c/hi) and s[1] read before s++. Left: s-register order (original: s0 p, s1 s, s2 x, s3 f; ours s0 s, s1 p, s2 f, s3 x), and after the call the original loads w into v1 before 'p = v0' so 'x += w' lands in the bne delay slot; ours loads it into v0 after the move and the delay slot takes the loop-top slt. Permuter 15 min: only semantics-breaking outputs. */
#include "ovl_15.h"
void *func_800ADE30(s32 x, s32 y, u32 *ot, u8 *s, void *p) {
    Unk7D4 *f = D_8006794C->unk1C->unk7D4;
    s32 w;
    s32 c;
    s32 hi = f->unk4 >> 8;

    c = *s;
    while (c != 0xFF) {
        if (c >= hi) {
            c = (c << 8) + s[1] - f->unk4 + f->unk6;
            s++;
            p = func_800AE028(x, y, ot, f->unkC[c], &w, p);
        } else {
            p = func_800AE028(x, y, ot, f->unkC[c], &w, p);
        }
        c = *++s;
        x += w;
    }
    SetDrawTPage(p, 1, 1, 0x1E);
    addPrim(ot, p);
    return (DR_TPAGE *)p + 1;
}
