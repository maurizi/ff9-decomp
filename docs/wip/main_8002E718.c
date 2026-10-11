/* 16 rows differ, all in the first call: m->unk5 lands in a1 here (a3 in the original) so y goes through v1, and the otz param copy (move s3,a1) is scheduled earlier than in the original. The second call matches once y and x are computed into locals (y first). The permuter got to 7 rows by putting the 6 of both x expressions in a variable (new_var = 6; r->x + r->w - new_var), which moves m->unk5 to a0 (still not a3). Pass 13: all of these score worse (24-51 rows): the first call's x/y in locals (either order), the ternary split into an if/else around two calls, and a local for m->unk5. Pass 14: the original computes x/y of the first call into pseudos that global-alloc puts straight into a0/a1 (copy preference), before the ternary branch. y can only get a1 if m->unk5 (block-local, live to the beqz) does not, and m->unk5 avoids a1 in the original only because the otz param copy (reads hard a1) is scheduled after the m->unk5 load. So the open question is sched1's placement of the param copy. Pass 15: otz typed s16/u16/s8/u8 (to move the param copy) gives 24-25; u32/int 16. Pass 16: the original's m->unk5 in a3 means v0, v1, a0, a1 and a2 are all busy over its life in local-alloc; x/y (103/107) are global pseudos, so a0/a1 can only be busy through the hard params, i.e. the otz param copy would have to sit after the unk5 load at local-alloc time. Tried: u8/s32 `up = m->unk5 != 0` flags (14 rows, unk5 reloaded), a `top` variable (16), if/else flag (51), unk5 kept live to the end (24). A `u8 up` declared before `more` gives 14 but reloads unk5. Pass 17: sched.c's schedule_block never schedules the leading param copies (insns 4/6/8 before NOTE_INSN_FUNCTION_BEG) in sched1, so the otz copy is always at the top for local-alloc. Its mid-block position in the original comes from sched2 (anti-dependence on the a1 write `addiu a1,v0,5`), so it follows from y being in a1 and does not cause it. The open question is why m->unk5 (local) cannot take a1 in local-alloc. Tried: `z = otz` local (15), an inline arrow() wrapper for one or both calls (25/30), both calls written directly with r->... expressions (23), s32 more (32), narrow otz types with the header prototype hidden (24-25). */
#include "ff9.h"

void func_8002E718(RECT *r, s32 otz, MenuList *m) {
    s32 blink = ((u32)D_80077150 >> 4) & 1;
    u8 more = m->unk5 < m->unk2 - m->unk3;
    s32 x;
    s32 y;

    func_800275DC(r->x + r->w - 6, r->y + 5, otz, 0xB8, m->unk5 != 0 ? blink : 1);
    y = r->y + r->h - 14;
    x = r->x + r->w - 6;
    func_800275DC(x, y, otz, 0xB9, more ? blink : 1);
}
