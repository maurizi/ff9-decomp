/*
 * ovl_0a func_800B4A5C (0x540 bytes): shows a full-screen TIM (fade in, hold, fade out) with
 * its own draw loop.
 * WIP (pass 10): 336 of 336 instructions, 5 aligned diff rows, frame and stack slots now right.
 * The hole: a dead `p = (u8 *)&pad;` right after the func_8001D910 call. Taking the address in
 * an expression that is expanded (then deleted by flow because p is reassigned) puts pad on the
 * stack after clut/img (0x28, frame 0x58); `(void)&pad` is never expanded and does nothing.
 * Left: sprite-loop scheduling only. The original bumps p (`addiu t1, t1, 0x14`) and builds
 * the 0xE1000600 tpage constant a few slots earlier, and `addiu s2, sp, 0x18` (&tp[0]) sits
 * two slots earlier. Permuter hint: `spr[i]->u0 = (spr[i]->v0 = 0);` fixes exactly those rows
 * but drops the spr[i] reload between the two byte stores (4 rows, 334 vs 336 instructions).
 * Pass 12 (sched2 trace, cc1 -dS): the five loop fillers (p += 0x14, lui/ori 0xE1000600, &tp[0])
 * are priority-1 insns that gcc's backward list scheduler drops into the load-delay slots as they
 * become ready; ours fills the slots 0x108..0x164 and leaves 0xFC empty, the original fills
 * 0xFC..0x158 and leaves 0x164 (between the second spr[i] reload and sb zero,13) empty, i.e.
 * every filler became ready one slot later there. No C spelling found that delays them: setUV0/
 * setWH/setXY0 macros and the chained u0/v0 stores change nothing or lose the reload.
 * Pass 13 (sched.c read against the trace): the original's slot 0x164 held an insn that sched2
 * scheduled and a later pass deleted (jump2's no-op move deletion: `move r,r` from a copy whose
 * source dies and shares the register), which is why the assembler's load-delay nop is there. It
 * must sit between `addiu s2,sp,0x18` (342) and `addu a3,s2,a3` (344) in dependence order
 * (342 -> N -> 344): then N is launched before 344 in sched1 (LUID above 342), is ready at T-45
 * in sched2, 342 only becomes ready after it (T-48), and the other fillers shift one gap each,
 * exactly the original. An empty `__asm__("" : "=r"(i) : "0"(i))` after the code store proves the
 * slot mechanism (2 rows left, only the LUID order). Candidates tried without success: pointer
 * copies of p/t/q (coalesced by combine), a `tpp = tp` draw-loop pointer assigned in the loop
 * (loop.c hoists it: REG_EQUAL fp+24, life > 1), tpp with a pre-loop init / later tpp[i] uses,
 * do{}while(0) wraps, for/while loop forms, statement moves. cse.c make_regs_eqv keeps a copy
 * `(set B A)` only when B lives beyond the block (or is first mentioned before it); loop.c then
 * hoists it unless its lifetime*savings is below the threshold.
 * Pass 14: a goto-built sprite loop (no loop notes) is 48 rows, so loop.c does run on it; a `tpp = tp`
 * copy in the loop with tp or tpp in the draw loop (6/58 rows), a reused `void **q` for both arrays
 * (18-22) and setDrawTPage() instead of setlen + code[0] (5) don't help.
 Pass 20: shape diff (cmp_shape.py --shape) = the same 5 rows (pure scheduling). Note on the hole: the frame needs
 only 4 more bytes (var_size 24 -> 28 rounds to 32), and since purge_addressof runs after cse1 and puts an
 ADDRESSOF into the stack when it is used as a value, any variable whose address escapes as a value later than the
 func_8001D910 call (and is never otherwise accessed) gives the slot at 0x28. A sprite-setup inline taking
 (SPRT **, DR_TPAGE **, ...) to explain the deleted `move` (the copy of &tp[0]) is much worse (75-127 rows,
 331-333 insns: the spr[i] reloads go away).
 * Pass 21: not changed. Note on the hole: unused BLKmode locals get their slot at declaration time (before
 * clut/img, which are placed when purge_addressof runs), so the variable at 0x28 must be an addressable scalar
 * whose address use is expanded after the func_8001D910 call and later deleted, or a reload spill slot whose
 * references all went away (which would also fit the deleted move after `addiu s2,sp,0x18`). Not found.
 */
#include "ovl_0a.h"

void func_8001D910(s32, RECT **, RECT **);

void func_800B4A5C(void) {
    SPRT *spr[2];
    DR_TPAGE *tp[2];
    RECT *clut;
    RECT *img;
    s32 pad;
    u8 *p;
    s32 tim;
    s32 i;
    s32 n;
    u32 state;

    tim = (s32)func_8001C7FC(4, 0x44E20, 0);
    func_8001D910(tim, &clut, &img);
    p = (u8 *)&pad;
    setRECT(img, 0, 0x100, img->w, img->h);
    setRECT(clut, 0x100, 0xE0, clut->w, clut->h);
    func_8001D8A8(tim);
    DrawSync(0);
    n = 0;
    state = 0;
    i = 1;
    p = g_gameState->unk1C->unk8E4.field->unkC->unk14->unk20;
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
        ClearOTagR(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8], 0x1000);
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
        addPrim((u32 *)g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], spr[g_gameState->unk1C->unk8]);
        addPrim((u32 *)g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], tp[g_gameState->unk1C->unk8]);
        func_80032098();
        DrawSync(0);
        g_gameState->vsyncMode = 2;
        VSync(g_gameState->vsyncMode);
        ResetGraph(1);
        PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
        DrawOTagEnv(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 0xFFF, &g_gameState->unk10[g_gameState->unk1C->unk8]);
        g_gameState->unk1C->unk8 ^= 1;
    } while (state < 4);
}
