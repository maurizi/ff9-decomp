/* Draft of fx_1f7 fx_1f7_op80. Status: 274 aligned mismatches, 112 ignoring registers (was 416/116).
 * Round 5: the 33-point copy loop uses its own counter jj (the original's a0; the shared j crossed calls) and a
 * copy tc = tr for tc->unk110 (the original's a2), which puts pseudos in t0/t1 so reloads use t2 as in the
 * original; d->pad = 0x80 after d->vz makes loop.c combine the vy/vz/pad givs into the pad address (a1 = d + 0xA).
 * Still open there: loop.c hoists the 0x80 constant (the original loads it inside the loop) and the copy loop's
 * registers (jj/tc/base/d/pad giv: v1/a1/a3/a2/a0 here, a0/a2/t0/a3/a1 in the original).
 * Pass 2: the original's stack holds a at 0x28, a separate b at 0x30 (second lerp point of the trail loop) and m at
 * 0x38 (it also passes &m.m[1][1] = sp+0x40 somewhere), so b is its own SVECTOR again; the frame is then 0xB0 vs
 * the original's 0xA8 because this C has one spill slot too many (0x58..0x80 vs 0x58..0x78). The trail loop as
 * tr = &D_801E8458[i] gets closer than the tr++ pointer loop, but the original keeps tr itself in s1 and steps it
 * by 0x118 (here: i * 0x118 giv spilled + base). With tr++ loop.c combines every tr->field DEST_ADDR giv into one
 * reduced tr + 0x10C register (combined benefit makes it worth while) and keeps the biv for (u8 *)tr; the
 * original reduces none of them (its 33-point loop reads unk110 through a register copy of tr, a2). A block-local
 * copy pointer for the inner loop gets propagated back by cse. Earlier notes: ex/entry reload register is t2 in the
 * original (t0/t1 hold pseudos), the 33-point copy loop holds n=0x21 in t1 and loads 0x80 inside the loop. */
#include "fxmod/fx_1f7.h"

s32 fx_1f7_op80(s32 mode, Fx1f7Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    MATRIX m;
    Pool6CEntry *p;
    Fx1f7Trail *tr;
    Fx1f7Spark *sp;
    s32 frame;
    s32 i;
    s32 k;
    s32 j;
    s32 al;
    s32 n;
    SVECTOR *d;
    s32 jj;
    Fx1f7Trail *tc;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk8 = 0;
        D_801E80C8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        for (i = 0; i < 6; i++) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 3), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&D_801E8458[i], 0);
            D_801E8458[i].unk10C = 4;
            D_801E8458[i].unk110 = 0x20;
            D_801E8458[i].unk114 = i * 2;
        }
        for (i = 0; i < 12; i++) {
            D_801E8AE8[i].unk0 = ex->btlfx_randRange(0, 0x2F);
            D_801E8AE8[i].unk4 = ex->btlfx_randRange(-0x10, 0x10);
            D_801E8AE8[i].unk10 = i * 3;
            D_801E8AE8[i].unk8 = 2;
            D_801E8AE8[i].unkC = 0;
        }
        work->unk0 = 1;
        work->unk4 = 1;
    } else {
        frame = entry->unk0;
        if (frame == 0x18) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x200);
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x200);
            }
        }
        if (frame == 0x19) {
            work->unk8 = 4;
        }
        if (work->unk0 != 0) {
            for (i = 0; i < 6; i++) {
                tr = &D_801E8458[i];
                if (tr->unk10C != 0) {
                    if (--tr->unk114 < 0) {
                        for (k = 0; k < 16; k++) {
                            if (k != 0) {
                                ex->func_800DC818(0x10, D_801E7EB0[i][k], (u16 *)&a);
                            } else {
                                ex->func_800DC8AC(0x10, D_801E7EB0[i][0], (BtlBone *)&m);
                                ex->func_800CFE88(&m, 0x400, (s32)&a);
                            }
                            ex->func_800DC818(0x10, D_801E7EB0[i][k + 1], (u16 *)&b);
                            for (j = 0; j < 4; j++) {
                                ex->btlfx_lerpVec((s16 *)&a, (s16 *)&b, j << 10, (s16 *)&D_801E80D8[k * 4 + j]);
                            }
                        }
                        d = tr->unk4;
                        al = 0x1000;
                        n = 0x21;
                        tr->unk0 = tr->unk2 = n - 1;
                        tr->unk1 = 0;
                        if (tr->unk110 < 0x10) {
                            al = tr->unk110 << 8;
                        }
                        tc = tr;
                        for (jj = 0; jj < n; jj++, d++) {
                            d->vx = D_801E80D8[tc->unk110 + jj].vx;
                            d->vy = D_801E80D8[tc->unk110 + jj].vy;
                            d->vz = D_801E80D8[tc->unk110 + jj].vz;
                            d->pad = 0x80;
                        }
                        env->unk28 = 0x200;
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 3), 0, NULL, frame, 0x1000, al, 1, (u8 *)tr, 0);
                        env->unk28 = 0;
                        tr->unk110 -= 4;
                        if (tr->unk110 < 0) {
                            tr->unk110 = 0x20;
                            tr->unk10C--;
                        }
                    }
                }
            }
        }
        if (work->unk4 != 0) {
            for (i = 0; i < 2; i++) {
                for (k = 0; k < 6; k++) {
                    ex->func_800DC818(0x10, D_801E8048[i][k], (u16 *)&m);
                    ex->func_800DC818(0x10, D_801E8048[i][k + 1], (u16 *)&b);
                    for (j = 0; j < 4; j++) {
                        ex->btlfx_lerpVec((s16 *)&m, (s16 *)&b, j << 10, (s16 *)&D_801E82D8[i * 24 + k * 4 + j]);
                    }
                }
            }
            for (i = 0, sp = D_801E8AE8; i < 12; i++, sp++) {
                if (sp->unk8 != 0) {
                    if (--sp->unk10 < 0) {
                        env->unk28 = 0x200;
                        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &D_801E82D8[sp->unk0], NULL, 0x4000,
                                                sp->unkC, 1, 0x1000, sp->unk4, 0);
                        env->unk28 = 0;
                        if (++sp->unkC >= 6) {
                            sp->unk0 = ex->btlfx_randRange(0, 0x2F);
                            sp->unk4 = ex->btlfx_randRange(-0x10, 0x10);
                            sp->unkC = 0;
                            sp->unk8--;
                        }
                    }
                }
            }
        }
        if (work->unk8 != 0) {
            work->unk8--;
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, 0, 0, work->unk8 << 6);
        }
        return frame >= 0x40;
    }
    return 0;
}
