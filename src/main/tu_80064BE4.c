#include "ff9.h"

void pen_nop(void) {
}

void pen_beginOt(s32 unused, u32 *ot) {
    msg_beginDraw();
    g_penOt = ot;
    pen_setColor(0x808080);
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
    TimBlock *blk;
    LoadImage((RECT *)&tim->clut.x, (u32 *)(tim + 1));
    blk = &tim->clut;
    blk = (TimBlock *)((u8 *)blk + tim->clut.bnum);
    LoadImage((RECT *)&blk->x, (u32 *)(blk + 1));
    DrawSync(0);
}
