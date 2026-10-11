/* 19 rows: register allocation (buf in s7 not fp, list8 in s8 not s7) and two real diffs: (1) the original copies buf to s0 (addu s0,fp,zero) right before the first memmove and passes the copy to memmoves 0 and 1, buf itself to the later ones; every `data = buf` form is CSE'd away (the fake `data = buf + (base << 11) - (base << 11)` keeps the copy: 7 rows), (2) `e->unk2 = 2` uses li t0 (a spilled loop-invariant constant reloaded into t0, like 1 and 0xFFFF) here li v0. The do { } while (0) after `buf += base << 11` is worth 4 rows; key = s->unk2 hoisted before each inner loop, size = cnt * 0x10 and top -= size before gs->unk18[13] matter. Pass 15: a dead `u8 *data = NULL` initialiser (the pass-14 CSE copy rule) does not keep the copy (data's last use is before buf's); reusing e, s or dst as the copy: 19 (no change). Pass 16: a copy survives CSE when its uses are in a different extended basic block from the copy; here the copy and both memmoves are in one block (no label between them in the original), so the copy needs a C construct CSE cannot see through. Not found. Pass 17: cse.c make_regs_eqv makes a new copy canonical only if it lives past the block and longer than the old register, so `data = buf` (data dies first) is always replaced by buf. An inline memmove wrapper taking src (integrate.c copies non-const params into a fresh pseudo, which CSE folds the same way) and a block-scoped data both stay at 19. Pass 18: a `do { } while (0)` between `data = buf` and the memmoves does keep the copy through cse1 (cse_end_of_basic_block stops at NOTE_INSN_LOOP_END before loop.c), but cse2 runs with after_loop set, ignores the loop notes and folds it again (checked in the .cse/.cse2 dumps). So in the original the copy survives cse2 too: it needs a real basic-block boundary (label) between copy and uses, or data must outlive buf. Reusing data for a later memmove's src (`data = buf + ((dir[13].unk8 - base) << 11)`, also for [12]) stays at 19. The `addu a1,s7,v0`/`move s7,a1` rows cmp.py prints are alignment noise; the RTL updates buf in place like the original. Pass 19: in the original the s0 copy is later reused as the second loop's &e->unk2 pointer (addiu s0,s0,-8), so the copy is probably a reused pointer variable, but make_regs_eqv still needs the copy's last use after buf's (the last memmove reads buf), so reusing e/s/dst still folds. cse.c never tests REG_USERVAR_P (only loop.c, combine, regclass and jump do), so a user-variable copy can't help here. */
#include "ff9.h"

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
} Unk8001DE50;

typedef struct {
    u8 pad0[8];
    u16 unk8;
    u8 padA[6];
} Unk8001DE50b;

void func_8001DE50(void) {
    GameState *gs;
    u8 *buf;
    DirInfo *dir;
    s32 base;
    s32 size;
    s32 cnt;
    s32 t;
    u8 *top;
    u8 *data;
    u8 *list8;
    u8 *list11;
    Unk8001DE50 *s;
    Unk8001DE50 *e;
    Unk8001DE50b *dst;
    u16 key;

    gs = D_8006794C;
    if (!(gs->unk0 & 0x100000)) {
        return;
    }
    cnt = 0;
    buf = D_80067944 + 0x14C700;
    size = (((DirInfo *)gs->unk14)[13].unk8 << 11) + ((DirInfo *)gs->unk14)[13].unk4 * 2;
    base = ((DirInfo *)gs->unk14)[0].unk8;
    t = size & 0x800;
    if (t != 0) {
        size += 0x800 - t;
    }

    func_80022C60(gs->unk20, size, buf, NULL);
    while (func_80022B18() != 0) {
    }
    dir = (DirInfo *)(buf + 0x10);
    buf += base << 11;
    do { } while (0);
    data = buf;

    size = (dir[0].unk4 + 1) * 8;
    top = (u8 *)D_8006794C - size;
    gs->unk18[0] = top;
    memmove(gs->unk18[0], data, size);

    size = (dir[1].unk4 + 1) * 8;
    top -= size;
    gs->unk18[1] = top;
    memmove(gs->unk18[1], data + ((dir[1].unk8 - base) << 11), size);

    size = (dir[10].unk4 + 1) * 8;
    top -= size;
    gs->unk18[10] = top;
    memmove(gs->unk18[10], buf + ((dir[10].unk8 - base) << 11), size);

    size = (dir[8].unk4 + 1) * 8;
    top -= size;
    gs->unk18[8] = top;
    memmove(gs->unk18[8], buf + ((((DirInfo *)gs->unk14)[8].unk8 - base) << 11), size);

    list8 = buf + ((dir[8].unk8 - base) << 11);
    s = (Unk8001DE50 *)(list8 + dir[8].unk4 * 8) - 1;
    list11 = buf + ((dir[11].unk8 - base) << 11);
    do {
        key = s->unk2;
        e = (Unk8001DE50 *)(list11 + dir[11].unk4 * 8) - 1;
        do {
            if (e->unk0 == key) {
                if (e->unk2 == 0) {
                    e->unk2 = 1;
                    cnt++;
                }
                break;
            }
            e--;
        } while ((u8 *)e >= list11);
        s--;
    } while ((u8 *)s >= list8);
    size = cnt * 0x10;
    top -= size;
    gs->unk18[11] = top;
    s = (Unk8001DE50 *)(list8 + dir[8].unk4 * 8) - 1;
    dst = gs->unk18[11];
    do {
        key = s->unk2;
        e = (Unk8001DE50 *)(list11 + dir[11].unk4 * 8) - 1;
        do {
            if (e->unk0 == key) {
                if (e->unk2 == 1) {
                    memmove(dst, e, 0x10);
                    dst->unk8 = 0xFFFF;
                    dst++;
                    e->unk2 = 2;
                }
                break;
            }
            e--;
        } while ((u8 *)e >= list11);
        s--;
    } while ((u8 *)s >= list8);
    ((DirInfo *)gs->unk14)[11].unk4 = cnt * 2;

    size = (dir[12].unk4 + 1) * 8;
    top -= size;
    gs->unk18[12] = top;
    memmove(gs->unk18[12], buf + ((dir[12].unk8 - base) << 11), size);

    size = dir[13].unk4 * 2;
    top -= size;
    gs->unk18[13] = top;
    memmove(gs->unk18[13], buf + ((dir[13].unk8 - base) << 11), size);
    gs->unk0 &= ~0x100000;
}
