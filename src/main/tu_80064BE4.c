#include "ff9.h"

void pen_nop(void) {
}

void pen_beginOt(s32 unused, u32 *ot) {
    msg_beginDraw();
    g_penOt = ot;
    pen_setColor(GPU_RGB_NEUTRAL);
}

INCLUDE_ASM("asm/us/main/nonmatchings/tu_80064BE4", pen_setColorPreset);

void pen_setColorRGB(u8 r, u8 g, u8 b) {
    pen_setColor(r | (g << 8) | (b << 16));
}

void pen_locate(u16 x, u16 y) {
    pen_begin();
    pen_setPos(x, y);
    g_penLineStart = x;
    pen_end();
}

void tim_uploadBuffer(void) {
    TimHeader *tim = &g_timBuffer;
    TimBlock *b;
    LoadImage((RECT *)&tim->clut.x, (u32 *)(tim + 1));
    b = &tim->clut;
    b = (TimBlock *)((u8 *)b + tim->clut.bnum);
    LoadImage((RECT *)&b->x, (u32 *)(b + 1));
    DrawSync(0);
}
