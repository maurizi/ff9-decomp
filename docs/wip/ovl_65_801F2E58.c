/* STATUS: near miss, ~38 rows. ovl_65 func_801F2E58 (0x190, add reward items; "inventory full" window). Left: the original keeps full in s4 and does the u8 andi into s0 eagerly (ours elides it or does it at the test); the centring math is (w - 0x10) - ts.w in the original, gcc reassociates ours to w - (ts.w + 0x10); D_801F7A78.unk368 is loaded through its own %hi/%lo (D_801F7DE0). rw/ws pointer locals give the s3/s5 bases. */
#include "ovl_65.h"

u8 func_801F2E58(void) {
    TextSize ts;
    RECT rr;
    u32 i;
    s32 full;
    u8 f;
    Rewards *rw;
    WinSlot *ws;
    s32 n;

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
        ts = func_8002AF70(func_80035774(0xA));
        rr.x = D_801F7A78.unk368->unk0 + ((D_801F7A78.unk368->unk4 - 0x10 - ts.w) >> 1);
        rr.y = D_801F7A78.unk368->unk2 + ((D_801F7A78.unk368->unk6 - 0x10 - ts.h) >> 1);
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
