/* STATUS: near miss, 36 rows. ovl_14 func_800A741C (0x15C, look up the current area/field ids in the two directories at D_80067944 + 0x14C700 and store their sector ranges). Control flow matches: loop 1 is entered with 'goto test' into a do/while, loop 2 is a for(;;) with the found block inside (gcc then duplicates its first load the way the original does); mode and id must be s32 (no andi). Left: register assignment only (original t0 id, t1 base, t2 gs, t3 area, t4 mode, a1 ctx, a2 f; ours shifted by one) and the blk/n loads in the if/else come out in v0/v1 swapped. Permuter 15 min: only outputs that break the semantics (reusing id/n as temporaries). */
#include "ovl_14.h"
void func_800A741C(void) {
    GameState *gs = D_8006794C;
    s32 mode = gs->unk8;
    GameData *ctx = gs->unk1C;
    GameStateUnk14 *f;
    s32 base;
    u8 *area;
    u8 *start;
    Ovl14Dir *e;
    s32 key;
    s32 id;
    s32 n;
    s32 blk;

    if (mode == 1 || mode == 3) {
        id = -1;
        f = gs->unk14;
        base = f->unk8;
        area = D_80067944 + ((base << 11) + 0x14C700);
        if (mode == 1) {
            blk = f->unk48 - base;
            key = (s16)ctx->unkC;
            n = f->unk44;
        } else {
            blk = f->unk38 - base;
            key = ctx->unk7DC;
            n = f->unk34;
        }
        start = area + (blk << 11);
        e = (Ovl14Dir *)start + n - 1;
        goto test1;
        do {
            e--;
            if ((u8 *)e < start) {
                goto skip1;
            }
        test1:;
        } while (e->unk0 != key);
        id = e->unk2;
        ctx->unk7FC[1] = e->unk4;
        ctx->unk804[1] = e[1].unk4 - e->unk4;
    skip1:
        f = gs->unk14;
        start = area + ((f->unk28 - base) << 11);
        e = (Ovl14Dir *)start + f->unk24 - 1;
        for (;;) {
            if (e->unk0 == id) {
                ctx->unk7FC[0] = e->unk4;
                ctx->unk804[0] = e[1].unk4 - e->unk4;
                break;
            }
            e--;
            if ((u8 *)e < start) {
                break;
            }
        }
        if (mode == 1) {
            ctx->unk3A = id;
        } else {
            ctx->unk7DE = id;
        }
    }
}
