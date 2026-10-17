/*
 * ovl_0a func_800C07D8 (0x3F4 bytes): sets up a palette fade entry (FieldPalAnim) for a background
 * object: per-channel steps, StoreImage of the current CLUT row and a volatile read-back loop.
 * WIP (pass 10): 253 of 253 instructions, 11 aligned diff rows (was 35).
 * Fixed: `t = arg3 - 1; n = (u8)arg4 - t;` with a block-local t (the original keeps arg3 - 1 in v0;
 * y itself is reused later, and `(u8)arg4 - (arg3 - 1)` gets reassociated by fold); statement
 * order t/n, size, e, unk0 = 0x80, unk8, h (found by brute force); the object table address in
 * its own local (`q = (u8 *)h + h->unk10;`, permuter).
 * Left: the original loads the u8 stack args 5-7 (lbu t3-t5) right at the top, ours a few
 * slots later (after e->unk8 = size), and the volatile read-back loop uses a0/v1 where we use
 * v1/v0. 40 permuter minutes found nothing more.
 * Pass 12: the lbu loads of arg5-7 only float to the top if sched1 does not launch them
 * (their pseudos are single-set, so they are 'birthing' and get scheduled next to the first
 * QImode store, which aliases the stack); a second assignment to the args does not change it.
 * The e->unk8/e->unk0 store order decides which of the two stores sched1 keeps last (ties go to
 * the later statement); swapping them reorders the whole prologue (39 rows).
 * Pass 13 (sched.c + trace): after reload every sp-relative address is 'varying' (rtx_varies_p only
 * exempts fp/ap), so in sched2 the stack-arg lbu loads conflict with every store through a pointer;
 * they get priority 3 from the e->unk0/unk8 stores and land right above them. In sched1 (ap-based,
 * non-varying) only the QImode store conflicts (true_dependence's in-struct exception excludes QImode),
 * so sched1 launches them before `sb e->unk0`. The original's order is what sched2 produces when the
 * three loads sit right after the arg4 `lw` in the chain with priority 1 and no dependents (the
 * potential-hazard rule then puts them after the prologue stores and ahead of the arg4 load by LUID).
 * `const` on the parameters gives that (RTX_UNCHANGING_P stack slots) but reload then rematerialises
 * the loads at their uses (REG_EQUIV mem) and sinks them (85 rows). Not found: a form that keeps the
 * pseudos and still frees the loads of the store dependences.
 Pass 20: shape diff 8 rows (the three lbu and the e->unk8 store), registers otherwise right except the read-back
 loop. Our sched1 order is `sw e->unk8`, the three stack-arg lbu, `lbu e->unk1`, `sb e->unk0`; the original's has
 the lbu at the top and `sb unk0` before `sw unk8`. Making arg5-7 multi-set (a later reassignment, so they are
 not "birthing" in adjust_priority) does not move them, nor does a K&R definition. With `e->unk0 = 0x80;` first the
 lbu land right before the sb but the n/size computation sinks to the sw (29 rows); reusing size/t/y for p->unk20
 (to make n/size multi-set) is 17-74.
 * Pass 21: no change (11 rows). The read-back loop's registers (w in a0, the loaded value in v1 in the original)
 * do not move with the loaded value assigned to a local (t, y, size, arg0, arg4, a new u16/s32), `(void)`, a
 * pointer-free or while form of the loop. Single-line moves within each straight-line run: 11 at best.
 * Pass 23 (param lever): the lbu loads are combine's merge of the entry `lw tempreg` (PROMOTE_PROTOTYPES: u8 args
 * arrive as int) into the conversion insn, so they sit after the notes of the deleted lw's and are not in the
 * unmovable head run (sched.c only pins SETs from hard regs; arg4's mem load already ends that run). Correction to
 * pass 20: a real second set does move them. `if (arg2 == 99) { arg5 = 3; arg6 = 3; arg7 = 3; }` before the
 * `arg2 >= 2` test (REG_N_SETS 2, so not birthing) puts all three lbu at the top exactly like the original (the
 * extra code itself costs 29 rows). A dead set does not count (flow deletes it before REG_N_SETS is recounted):
 * `arg5 = w[i];` etc. in the read-back loop change nothing. So the original probably assigns arg5-7 somewhere live;
 * no natural spelling found (they are u8, so `arg5 = -arg5` for the fade-out case would change the stored value).
 * Pass 24: still 11 rows. BEHAVIOURALLY EQUIVALENT to the original (tools/equiv.py: 26/26 trials agree, 23 paths,
 * 89% of the original's insns ran). A zero-cost second set exists: `arg5 = (arg5 << 8) >> 8;` (also `<< 24 >> 24`)
 * on the u8 params survives fold, flow counts it (REG_N_SETS 2, not birthing) and combine turns it into a no-op
 * `(set r r)` that reload_cse deletes - no extra code (`*1`, `/1`, `^1^1` via a temp are folded earlier by fold/cse,
 * flow also deletes plain self-moves). With it on arg5-7 the lbu land at the very top; plus moving `p->unk1 = n`
 * before `e->unk4` (hill climb): 18 rows. Left there: `lw arg4` comes after the lbu: sched2's tie-break among the
 * equal-priority loads is "first with the largest potential hazard" in LUID order, and arg4's lw (birthing in
 * sched1, placed right before its use) has the higher LUID. The original needs arg4 multi-set too, and an s32 has
 * no combine-only identity (no known nonzero bits). Making e->unk0 the first store (needed for sb before sw) sinks
 * n/size in sched1 (size becomes ready right after the sw and is birthing). Annealing over the 18 prologue lines
 * with def/use-checked moves (build/w/sa.py, ~6k compiles, also with e->unk0 first and with `k = e->unk1` split
 * out of y), const params (85), and 1 h of permuter: nothing below 11.
 */
#include "ovl_0a.h"

s32 func_800C07D8(s32 arg0, u8 arg1, u32 arg2, u8 arg3, s32 arg4, u8 arg5, u8 arg6, u8 arg7) {
    s32 rgb[3];
    Unk800C9DA4Ent *e;
    FieldPalAnim *p;
    Unk800C9DA4Hdr *h;
    s32 n;
    s32 y;
    s32 size;
    u16 i;
    u8 *q;
    volatile u16 *w;
    s32 t;

    t = arg3 - 1;
    n = (u8)arg4 - t;
    size = n * 4 + 0x2C;
    e = &D_800C9DA4.unk28[arg0];
    e->unk8 = size;
    e->unk0 = 0x80;
    h = D_800C9DA4.unk10;
    q = (u8 *)h + h->unk10;
    y = *(u32 *)((u8 *)h + ((Unk800C9DB4Obj *)q)[e->unk1].unk2C) & 0x1FF;
    p = (FieldPalAnim *)(D_800C9DA4.unk2C + D_800C9DA4.unk34);
    D_800C9DA4.unk34 += size;
    e->unk4 = (u8 *)p - (u8 *)D_800C9DA4.unk28;
    p->unk0 = 1;
    p->unk1 = n;
    p->unk2 = arg3;
    p->unk3 = arg4;
    p->unk8 = arg2;
    p->unkC = 0;
    if (arg2 >= 2) {
        rgb[0] = arg5 << 16;
        rgb[1] = arg6 << 16;
        rgb[2] = arg7 << 16;
        switch (arg1) {
        case 0:
            rgb[0] = ((u32)(arg5 << 24) / (arg2 << 16)) << 8;
            rgb[1] = ((u32)(arg6 << 24) / (arg2 << 16)) << 8;
            rgb[2] = ((u32)(arg7 << 24) / (arg2 << 16)) << 8;
            p->unk4 = 0;
            break;
        case 1:
            rgb[0] = -(((u32)(arg5 << 24) / (arg2 << 16)) << 8);
            rgb[1] = -(((u32)(arg6 << 24) / (arg2 << 16)) << 8);
            rgb[2] = -(((u32)(arg7 << 24) / (arg2 << 16)) << 8);
            p->unk4 = 0;
            break;
        case 2:
            rgb[0] = ((u32)(arg5 << 24) / (arg2 << 16)) << 8;
            rgb[1] = ((u32)(arg6 << 24) / (arg2 << 16)) << 8;
            rgb[2] = ((u32)(arg7 << 24) / (arg2 << 16)) << 8;
            p->unk4 = 1;
            break;
        }
    } else {
        switch (arg1) {
        case 0:
            rgb[0] = arg5;
            rgb[1] = arg6;
            rgb[2] = arg7;
            p->unk4 = 0;
            break;
        case 1:
            rgb[0] = -arg5;
            rgb[1] = -arg6;
            rgb[2] = -arg7;
            p->unk4 = 0;
            break;
        case 2:
            rgb[0] = arg5;
            rgb[1] = arg6;
            rgb[2] = arg7;
            p->unk4 = 1;
            break;
        }
    }
    p->unk10 = rgb[0];
    p->unk14 = rgb[1];
    p->unk18 = rgb[2];
    p->unk1C = 0x2C;
    p->unk20 = n * 2 + 0x2C;
    p->unk24.x = arg3;
    p->unk24.y = y;
    p->unk24.w = n;
    p->unk24.h = 1;
    StoreImage(&p->unk24, (u32 *)((u8 *)p + p->unk1C));
    w = (u16 *)((u8 *)p + p->unk20);
    for (i = 0; i < n; i++) {
        w[i];
    }
    D_800C9DA4.unk34 += e->unk8;
    return 1;
}
