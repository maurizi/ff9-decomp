/*
 * ovl_0a func_800B4A5C (0x540 bytes): shows a full-screen TIM (fade in, hold, fade out) with
 * its own draw loop.
 * WIP: 336 of 336 instructions, 13 aligned diff rows. The frame is 0x58 in the original:
 * spr[2]/tp[2] at 0x10/0x18, clut/img at 0x20/0x24 and 8 unused bytes at 0x28. Without the
 * `unused` RECT the frame is 0x50 (25 rows). With it the size is right, but since aggregates get
 * their slot at the declaration while clut/img only get theirs at the `&clut, &img` call, it
 * lands below clut/img (clut/img at 0x28/0x2C). A block-scoped one in either loop does the
 * same; a scalar whose address is taken in dead code is dropped. The hole must be something
 * allocated after the call that leaves no code. Also: setRECT self-assignments explain the dead
 * lhu loads.
 * Pass 9: a `{ RECT u; }` block placed after the func_8001D910 call still lands at 0x20, below
 * clut/img, so clut/img get their slots later than any declaration; an address-taken scalar in
 * a block there is dropped. Since the locals end at 0x28 and the saves start at 0x30, the hole
 * may be a single 4-byte slot (a reload spill whose code was later deleted?).
 */
#include "ovl_0a.h"

void func_8001D910(s32, RECT **, RECT **);

void func_800B4A5C(void) {
    SPRT *spr[2];
    DR_TPAGE *tp[2];
    RECT *clut;
    RECT *img;
    RECT unused; /* frame size only, see header */
    u8 *p;
    s32 tim;
    s32 i;
    s32 n;
    u32 state;

    tim = (s32)func_8001C7FC(4, 0x44E20, 0);
    func_8001D910(tim, &clut, &img);
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
