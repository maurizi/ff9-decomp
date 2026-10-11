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
