/* 28 rows differ: the first SE call (unk26 == 0xFF path) now keeps its own jalr like the original, because the three list-cursor branches are an if/else-if chain that falls into one shared return 1 (separate returns let jump2 cross-jump all four calls). Remaining: jump2 now merges the whole argument setup of the 0x10/0x40/0x10000 calls, while the original shares only the jalr (each branch keeps its own setup and jumps to the jalr), and in the 0x10000 path the original stores D_8007B958 = D_8007B96C after the setup, right before the jalr. That suggests the store sits between the setup and the call in RTL, which no C order we tried gives. The 0x10 path is written as if (D_8007B958 > 0) D_8007B958--; else D_8007B958 = D_8007B95C - 1; (bgtz with the decrement in the delay slot). Pass 15: the list-cursor part as a static inline returning 1/0 (`return list_cursor(w, pad)`), with shared or per-branch `return 1`: 28/32; the early part as `if (D_8007B968 >= 0) { se; return 1; }` or as an if/else with `goto list` for the f & 8 case: 32-41. In our output the 0x10/0x40 calls cross-jump into the early 0x67 call's setup (j 0x80); in the original they jump only to the 0x10000 path's jalr. Pass 16: the cross-jump target is decided by jump2 on hard regs. In the original the 0x10000 path falls through into the shared `v0 = 1; j end`, so the 0x10/0x40 calls (identical setup ending in `jalr v0`) are cross-jumped onto its jalr. Ours: that call is `jalr v1` (fn pointer P in v1, %hi(D_8007B958) H in v0), the jalr insns differ, and jump2 instead merges the return-1 tails and sends the 0x10/0x40 paths into the early 0x67 call (j 0x80). The draft below puts `else if (!(pad & 0x10000) || D_8007B96C < 0) return 0; else { store; SE(0x65); }` last so the 0x10000 call falls into `return 1` (same 28 rows, but the layout the original needs); the draft's `else return 0` order gives the same code. What remains is P/H in that block: local-alloc ranks both at 2 refs and H (lui right before the sw, after lw P) has the shorter life, so H takes v0. Making H lose needs H to be a global pseudo or a reload register; no C form found (fn pointer variable, GameData *gd local, ret/tmp for D_8007B96C, &D_8007B958 pointer, func_8002F488 as inline, every D_8007B95x global as a struct field: D_8007B958 as a field pins its store above the loads, 37 rows). A 20-min permuter run found only fake constructs (an identical-branch if around the early call breaks its cross-jump, 17 rows). Pass 17: our sched1 order in the 0x10000 call block is a0, a1, a2, %hi(gs), gs, a3, gs->unk1C, 0x7F, sw 16(sp), lw P, lui H, sw H, call. P is born before H and dies at the call, so H's interval lies inside P's, and with equal refs H always outranks P and takes v0. Any block-local H loses, so H has to be a global pseudo or a reload register, as pass 16 said. Pass 18: the final order of the 0x10000 block pins its local-alloc order. gs->unk1C (v1) dies at `lw P`, and H is v1, so H is born after lw P, and P's interval always contains H's. With equal refs (QTY_CMP_PRI = floor_log2(n)*n/len) H always wins v0. P can only win if it has 3 or more refs, or if H is not a local qty. If H were a global pseudo (set or used in another block), local-alloc would give P v0 and global-alloc would give H v1, which is exactly the original. update_equiv_regs moves a 2-ref global equiv init next to its use (local-alloc.c ~1240) and makes it local again, so H needs 3 or more refs or two sets. MIPS splits every word-sized symbolic address at expand time (mips_check_split), so a reload-register H would need an unallocated equiv pseudo. No C form found. Pass 19: a function-scope s32 *pb assigned &D_8007B958 in both the 0x10 and the 0x10000 blocks (two sets, so no single-set REG_EQUIV): 28, no change, also with the 0x40 block through pb. CSE folds pb into the constant address at each use (the .greg dump has only the split %hi pseudos), so the user variable never reaches flow. REG_USERVAR_P matters to loop.c's scan_loop (and combine's small-register-class checks, which MIPS doesn't use); there's no loop here. Pass 20: the 0x10000 path's `D_8007B958 = D_8007B96C` store is unconstrained by the gs->unk1C / ->unk83C loads: in 2.8 alias.c a fixed scalar MEM (the global) never conflicts with a varying MEM_IN_STRUCT one, so sched1/sched2 move it freely (that is why D_8007B958 as a struct field pins it). Its source position therefore doesn't matter. The P/H problem from pass 18 is unchanged. Per-file flags were checked as an alternative explanation for all 13 functions: no single -f flag helps any draft without breaking the matched neighbours (e.g. -fno-force-mem fixes 8001DE50's `li t0,2` but breaks 172 matched main functions, including 8001CD04..8001E350). */
#include "ff9.h"
extern s32 D_8007B95C;
extern s32 D_8007B96C;
#define SE(id) D_8006794C->unk1C->unk83C(0xD000, id, 0x800000, 0x80, 0x7F)
s32 func_8004DAD0(Unk8007B9C0 *w, s32 pad) {
    u8 f;

    if ((D_8007B708 == 1 && (pad & 0xA0000)) || (D_8007B708 != 1 && (pad & 0x20000))) {
        f = w->unk28;
        if (!(f & 0x40)) {
            if (w->unk26 == 0xFF) {
                w->unk26 = 1;
                if (D_8007B968 >= 0) {
                    SE(0x67);
                }
                return 1;
            }
            if (!(f & 8)) {
                w->unk28 = f | 4;
                return 1;
            }
        }
    }
    if (D_8007B968 >= 0) {
        if (D_8007B970 != w->unkA) {
            return 0;
        }
        if (pad & 0x10) {
            if (D_8007B958 > 0) {
                D_8007B958--;
            } else {
                D_8007B958 = D_8007B95C - 1;
            }
            SE(0x67);
        } else if (pad & 0x40) {
            if (D_8007B958 < D_8007B95C - 1) {
                D_8007B958++;
            } else {
                D_8007B958 = 0;
            }
            SE(0x67);
        } else if (!(pad & 0x10000) || D_8007B96C < 0) {
            return 0;
        } else {
            D_8007B958 = D_8007B96C;
            SE(0x65);
        }
        return 1;
    }
    return 0;
}
