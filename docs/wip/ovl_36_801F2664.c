/* STATUS: near miss, 7 rows (was 8). ovl_36 func_801F2664 (0xCC, card picture window). Splitting the call into if/else (c->unk7 re-read) fixes the v0 copy; left is an s0/s1 swap between cur and the CardCache pointer. Declaration order and init placement do not change it. */
#include "ovl_36.h"

void func_801F2664(void) {
    RECT *r = &D_801F347C.unk18E0;
    CardCache *c = &D_801F347C.unk1900;
    s32 cur;
    s32 v;
    s32 id;

    cur = D_801F347C.unk2 * 10 + D_801F347C.unk1;
    func_8002B524(r->x + 8, r->y - 3, 0x1E, D_80070418, 0);
    if (c->unk7 != 0xFF) {
        if (c->unk7 != cur && c->unk9 != 0) {
            func_801F12B8(c->unk6);
        } else {
            func_801F12B8(cur);
        }
    }
    func_8002DEE4(r, 0x1E, 2);
    func_8002705C(r, 0x1F, 2);
}
