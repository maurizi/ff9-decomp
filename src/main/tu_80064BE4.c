#include "ff9.h"

void func_80064BE4(void) {
}

void func_80064BEC(s32 a, u32 *b) {
    func_8004C204();
    D_8007BCA4 = b;
    func_800512E4(0x808080);
}

INCLUDE_ASM("asm/us/main/nonmatchings/tu_80064BE4", func_80064C24);

void func_80064CA4(u8 r, u8 g, u8 b) {
    func_800512E4(r | (g << 8) | (b << 16));
}

void func_80064CDC(u16 a, u16 b) {
    func_800515B8();
    func_800512B8(a, b);
    D_8007BCB0 = a;
    func_80051610();
}

void func_80064D28(void) {
    TimHeader *tim = &D_800834D0;
    TimBlock *b;
    LoadImage((RECT *)&tim->clut.x, (u32 *)(tim + 1));
    b = &tim->clut;
    b = (TimBlock *)((u8 *)b + tim->clut.bnum);
    LoadImage((RECT *)&b->x, (u32 *)(b + 1));
    DrawSync(0);
}
