/* STATUS: near miss, 5 rows. ovl_65 func_801F2E58 (0x190, add reward items; "inventory full" window). k = 0x10 keeps the centring math as (w - 0x10) - ts.w. Left: the original keeps full in s4 and copies it into a separate u8 f in s0 with an eager andi (f tested and returned as f ^ 1); a u8 full coalesces f into s4, an s32 full gives a move plus a late andi. */
#include "ovl_65.h"

u8 func_801F2E58(void) {
    TextSize ts;
    RECT rr;
    u32 i;
    u8 full;
    u8 f;
    Rewards *rw;
    WinSlot *ws;
    s32 n;
    s32 k;

    full = 0;
    rw = &D_801F7A78.unk24;
    ws = &D_801F7A78.unk348;
    for (i = 0; i < rw->unkC; i++) {
        n = rw->unk10[i][1];
        if (n != func_80027D84(rw->unk10[i][0], n)) {
            full = 1;
        }
    }
    f = full;
    if (rw->unk8 != 0xFF) {
        func_8006238C(rw->unk8);
    }
    if (f) {
        k = 0x10;
        ts = func_8002AF70(func_80035774(0xA));
        rr.x = D_801F7A78.unk368->unk0 + ((D_801F7A78.unk368->unk4 - k - ts.w) >> 1);
        rr.y = D_801F7A78.unk368->unk2 + ((D_801F7A78.unk368->unk6 - k - ts.h) >> 1);
        rr.w = ts.w + 0x10;
        rr.h = ts.h + 0xF;
        ws->unk0 = func_8002DC54(&rr, 6, 2);
        func_8002E2EC(ws->unk0, func_801F3434);
        func_8002E2FC(ws->unk0, (void (*)(Window *))func_801F347C);
        func_8002CF20(ws->unk0, 0);
        func_8002E31C(ws->unk0, 1);
        func_8002E304(ws->unk0, D_800704C8);
    }
    return f ^ 1;
}
