/*
 * ovl_0a func_800B4A5C (0x540 bytes): shows a full-screen TIM (fade in, hold, fade out) with
 * its own draw loop.
 * WIP (pass 10): 336 of 336 instructions, 5 aligned diff rows, frame and stack slots now right.
 * The hole: a dead `p = (u8 *)&pad;` right after the func_8001D910 call. Taking the address in
 * an expression that is expanded (then deleted by flow because p is reassigned) puts pad on the
 * stack after clut/img (0x28, frame 0x58); `(void)&pad` is never expanded and does nothing.
 * Left: sprite-loop scheduling only. The original bumps p (`addiu t1, t1, 0x14`) and builds
 * the 0xE1000600 tpage constant a few slots earlier, and `addiu s2, sp, 0x18` (&tp[0]) sits
 * two slots earlier.
 */
#include "ovl_0a.h"

void func_8001D910(s32, RECT **, RECT **);

void func_800B4A5C(void) {
    SPRT *spr[2];
    DR_TPAGE *tp[2];
    RECT *clut;
    RECT *img;
    s32 pad;
    u8 *p;
    s32 tim;
    s32 i;
    s32 n;
    u32 state;

    tim = (s32)func_8001C7FC(4, 0x44E20, 0);
    func_8001D910(tim, &clut, &img);
    p = (u8 *)&pad;
    setRECT(img, 0, 0x100, img->w, img->h);
    setRECT(clut, 0x100, 0xE0, clut->w, clut->h);
    func_8001D8A8(tim);
    DrawSync(0);
    n = 0;
    state = 0;
    i = 1;
    p = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20;
    do {
        spr[i] = (SPRT *)p;
        p += sizeof(SPRT);
        setlen(spr[i], 4);
        setcode(spr[i], 0x64);
        setRGB0(spr[i], 0x80, 0x80, 0x80);
        spr[i]->x0 = (0x140 - img->w * 4) >> 1;
        spr[i]->y0 = (0xE0 - img->h) / 2;
        spr[i]->u0 = 0;
        spr[i]->v0 = 0;
        spr[i]->w = img->w * 4 - 1;
        spr[i]->h = img->h;
        spr[i]->clut = getClut(clut->x, clut->y);
        tp[i] = (DR_TPAGE *)p;
        p += sizeof(DR_TPAGE);
        setlen(tp[i], 1);
        tp[i]->code[0] = _get_mode(1, 1, getTPage(0, 0, img->x, img->y));
        i--;
    } while (i >= 0);
    func_800AC67C((s32)p);
    func_800223CC();
    func_8001CCB4(0, 0, 0);
    do {
        ClearOTagR(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], 0x1000);
        switch (state) {
        case 0:
            n = 0x3C;
            i = 0;
            func_800320B8(1, 1, 0, 0xFFFFFF);
            func_800320B8(1, 1, n, i);
            state = 1;
            break;
        case 1:
            if (i < n) {
                i++;
            } else {
                n = 0x78;
                i = 0;
                state = 2;
            }
            break;
        case 2:
            if (i < n) {
                i++;
            } else {
                n = 0x3C;
                i = 0;
                func_800320B8(1, 1, n, 0xFFFFFF);
                state = 3;
            }
            break;
        case 3:
            if (i < n) {
                i++;
            } else {
                state = 4;
            }
            break;
        }
        addPrim((u32 *)D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][8], spr[D_8006794C->unk1C->unk8]);
        addPrim((u32 *)D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][8], tp[D_8006794C->unk1C->unk8]);
        func_80032098();
        DrawSync(0);
        D_8006794C->unk9 = 2;
        VSync(D_8006794C->unk9);
        ResetGraph(1);
        PutDispEnv(&D_8006794C->unkC[D_8006794C->unk1C->unk8]);
        DrawOTagEnv(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8] + 0xFFF, &D_8006794C->unk10[D_8006794C->unk1C->unk8]);
        D_8006794C->unk1C->unk8 ^= 1;
    } while (state < 4);
}
