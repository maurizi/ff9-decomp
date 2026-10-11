/* STATUS: near miss, 16 rows (pass 13; was 34). ovl_14 func_800A741C (0x15C, look up the current area/field ids
 in the two directories at D_80067944 + 0x14C700 and store their sector ranges).
 Pass 13: loop 1 is `goto test1; do { e--; if (e < start) goto skip1; test1: if (e->unk0 == key) { found; goto skip1; } } while (1);`:
 the found block sits inside the loop notes (its refs count double for id/ctx, which lifts id to t0 and ctx to a1
 like the original) and NOTE_INSN_LOOP_BEG is not followed by a jump, so jump.c does not duplicate the exit test.
 Loop 2 (`for (;;)` with the found block inside) is additionally wrapped in `do { } while (0)` (ctx 17 refs > f).
 The arms are `blk = f->unk48; key; n; blk -= base;` (load order unk48/key/n).
 Left (register allocation only, build/search/galloc.py + gsim.py): (1) area must be allocated before mode
 (original area t3, mode t4): area has 3 refs / len 35 (floor_log2(3) = 1 -> 0.086) vs mode 5 refs / len 74
 (0.135). A 4th area ref (each arm computing `start = area + ((A - base) << 11)` itself, build/v14/f1.c)
 gives 4 refs / len 40 = 0.2, but then area passes gs (5 / 51 = 0.196) and key's life grows to 23 so id
 (9 / 76 = 0.355) passes key (8 / 23 = 0.348): both margins are one insn of live length. (2) blk/n must land in
 v1/v0: n (92) conflicts with v0 because the local `blk << 11` (born first in the join block) takes v0 and
 blk prefers it (global.c set_preference takes the first operand of any expression); the f1 arms fix this
 (each arm's sub/shift temps share the loaded values' registers) but cost (1). do{}while(0) around the
 second lookup's start/e breaks the loop-2 layout (54+ rows). */
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
            blk = f->unk48;
            key = (s16)ctx->unkC;
            n = f->unk44;
            blk -= base;
        } else {
            blk = f->unk38;
            key = ctx->unk7DC;
            n = f->unk34;
            blk -= base;
        }
        start = area + (blk << 11);
        e = (Ovl14Dir *)start + n - 1;
        goto test1;
        do {
            e--;
            if ((u8 *)e < start) {
                goto skip1;
            }
        test1:
            if (e->unk0 == key) {
                id = e->unk2;
                ctx->unk7FC[1] = e->unk4;
                ctx->unk804[1] = e[1].unk4 - e->unk4;
                goto skip1;
            }
        } while (1);
    skip1:
        start = area + ((gs->unk14->unk28 - base) << 11);
        e = (Ovl14Dir *)start + gs->unk14->unk24 - 1;
        do {
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
        } while (0);
        if (mode == 1) {
            ctx->unk3A = id;
        } else {
            ctx->unk7DE = id;
        }
    }
}
