/* Draft of fx_018 op83 (bolt spawner for the lightning summons; the spawn blocks are written out like fx_005 op83).
 * Status: 921/966 instructions match. Left: in kind 1 (frames 6-15) and kind 2 the original groups the radius as
 * base + (randCentered(n) + c) with the inner sum computed first (here fold reassociates it, so the temporary
 * through ang2/ang3 is a stand-in), and the it/r/ang registers of those spawns are permuted (orig ang s0, it s1, r
 * s2). Each spawn needs its own it/r/ang variables (the original assigns them different registers per block), the
 * mode-1 loop counter must differ from the kind-1 counter, and the kind-2 base s from both. */
#include "fxmod/fx_018.h"

s32 fx_018_op83(s32 mode, Fx018Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    Fx018Bolt *it0;
    s32 r0;
    s32 ang0;
    Fx018Bolt *it1;
    s32 r1;
    s32 ang1;
    Fx018Bolt *it2;
    s32 r2;
    s32 ang2;
    Fx018Bolt *it3;
    s32 r3;
    s32 ang3;
    Fx018Bolt *it4;
    s32 r4;
    s32 ang4;
    Fx018Bolt *it5;
    s32 r5;
    s32 ang5;
    Pool6CEntry *p;
    s32 *q;
    s32 frame;
    s32 kind;
    s32 i;
    s32 j;
    s32 s;

    if (mode == 0) {
        init->unk0 = 0x1C;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801E88DC;
        init->unk4 = 0x10;
    } else if (mode == 1) {
        D_801E9CE8 = env;
        kind = init->unk0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        i = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        q = work->unk10 = D_801EA9E0;
        for (; i < 0x40; i++) {
            *q++ = ex->btlfx_randCentered(0x100);
            *q++ = ex->btlfx_randCentered(0x200);
            *q++ = ex->btlfx_randCentered(0xF5);
        }
        work->unk18 = kind;
        D_801E9CEC = entry;
        work->unk14 = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk18) {
        case 0:
            D_801EA9D0 = 0;
            if (frame < 29) {
                if (frame & 1) {
                    work->unk14++;
                    BTLFX_COPY_SVEC(&a, &work->unk0);
                    a.vy = 0;
                    it0 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (it0 != NULL) {
                        BTLFX_COPY_SVEC(&it0->unk0, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        b.vy -= 0xC00;
                        r0 = ex->btlfx_randCentered(0x100) + 0x4B0;
                        ang0 = (work->unk14 << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                        b.vx += (ex->math_cos(ang0) * r0) >> 12;
                        b.vz += (ex->math_sin(ang0) * r0) >> 12;
                        BTLFX_COPY_SVEC(&it0->unk8, &b);
                        it0->unk10 = work->unk14 % 16;
                        it0->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                        it0->unk12 = ex->btlfx_rand() % 13;
                        it0->unk13 = ex->btlfx_rand() % 13;
                        it0->unk15 = 0;
                        it0->unk14 = 2;
                    }
                }
            }
            if (frame >= 29) {
                return 1;
            }
            break;
        case 1:
            D_801EA9D0 = 0;
            if (frame == 0) {
                for (j = 0; j < 6; j++) {
                    work->unk14++;
                    BTLFX_COPY_SVEC(&a, &work->unk0);
                    a.vy = 0;
                    it1 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (it1 != NULL) {
                        BTLFX_COPY_SVEC(&it1->unk0, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        b.vy -= 0xC00;
                        r1 = ex->btlfx_randCentered(0x80) + 0x3E8;
                        ang1 = (work->unk14 << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                        b.vx += (ex->math_cos(ang1) * r1) >> 12;
                        b.vz += (ex->math_sin(ang1) * r1) >> 12;
                        BTLFX_COPY_SVEC(&it1->unk8, &b);
                        it1->unk10 = work->unk14 % 16;
                        it1->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                        it1->unk12 = ex->btlfx_rand() % 13;
                        it1->unk13 = ex->btlfx_rand() % 13;
                        it1->unk15 = -12;
                        it1->unk14 = ex->btlfx_rand() & 1;
                    }
                }
            }
            if (frame >= 6 && frame < 16) {
                work->unk14++;
                BTLFX_COPY_SVEC(&c, &work->unk0);
                c.vy = 0;
                it2 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it2 != NULL) {
                    BTLFX_COPY_SVEC(&it2->unk0, &c);
                    BTLFX_COPY_SVEC(&d, &c);
                    d.vy -= 0xC00;
                    ang2 = ex->btlfx_randCentered(0x100) + 0x5DC;
                    r2 = ((frame - 6) << 8) + ang2;
                    ang2 = ((work->unk14 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    d.vx += (ex->math_cos(ang2) * r2) >> 12;
                    d.vz += (ex->math_sin(ang2) * r2) >> 12;
                    BTLFX_COPY_SVEC(&it2->unk8, &d);
                    it2->unk10 = work->unk14 % 16;
                    it2->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                    it2->unk12 = ex->btlfx_rand() % 13;
                    it2->unk13 = ex->btlfx_rand() % 13;
                    it2->unk15 = 7;
                    it2->unk14 = ex->btlfx_rand() & 1;
                }
                work->unk14++;
                BTLFX_COPY_SVEC(&c, &work->unk0);
                c.vy = 0;
                it3 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it3 != NULL) {
                    BTLFX_COPY_SVEC(&it3->unk0, &c);
                    BTLFX_COPY_SVEC(&d, &c);
                    d.vy -= 0xC00;
                    ang3 = ex->btlfx_randCentered(0x100) + 0x7D0;
                    r3 = ((frame - 6) << 8) + ang3;
                    ang3 = ((work->unk14 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    d.vx += (ex->math_cos(ang3) * r3) >> 12;
                    d.vz += (ex->math_sin(ang3) * r3) >> 12;
                    BTLFX_COPY_SVEC(&it3->unk8, &d);
                    it3->unk10 = work->unk14 % 16;
                    it3->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                    it3->unk12 = ex->btlfx_rand() % 13;
                    it3->unk13 = ex->btlfx_rand() % 13;
                    it3->unk15 = 0;
                    it3->unk14 = 2;
                }
            }
            if (frame == 8 || frame == 10 || frame == 13) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x10, 0x40, 0x80);
            }
            if (frame == 9) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0);
            }
            if (frame >= 20) {
                return 1;
            }
            break;
        case 2:
            D_801EA9D0 = 1;
            if (frame == 0) {
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x25), (s32)&work->unk0);
                if (p != NULL) {
                    p->unk24->unk0 = -1;
                    ((EffTailSpawn *)p->unk24)->unk3C = func_801E8C84;
                    p->unk22 = 0x20;
                }
            }
            if (frame < 31) {
                if (frame == 0) {
                    BTLFX_COPY_SVEC(&D_801E9CE0, &work->unk0);
                }
                s = frame * 250 / 31 + 0x333;
                work->unk14++;
                BTLFX_COPY_SVEC(&a, &work->unk0);
                a.vy = 0;
                c.vz = 0;
                BTLFX_COPY_SVEC(&b, &a);
                r4 = s + ex->btlfx_randCentered(0x46);
                c.vy = ex->btlfx_rand();
                c.vx = (ex->btlfx_rand() & 0xFF) + 0x80;
                ex->func_800CFD78((s16 *)&b, (s16 *)&c, r4, (s16 *)&b);
                work->unk14++;
                BTLFX_COPY_SVEC(&a, &work->unk0);
                a.vy = 0;
                it4 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it4 != NULL) {
                    BTLFX_COPY_SVEC(&it4->unk0, &b);
                    BTLFX_COPY_SVEC(&it4->unk8, &D_801E9CE0);
                    it4->unk10 = work->unk14 % 16;
                    it4->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                    it4->unk12 = ex->btlfx_rand() % 13;
                    it4->unk13 = ex->btlfx_rand() % 13;
                    it4->unk15 = -(frame * 50 / 31);
                    it4->unk14 = ex->btlfx_rand() & 1;
                }
                BTLFX_COPY_SVEC(&D_801E9CE0, &b);
                if (frame < 19) {
                    work->unk14++;
                    BTLFX_COPY_SVEC(&a, &work->unk0);
                    a.vy = 0;
                    c.vz = 0;
                    it5 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (it5 != NULL) {
                        BTLFX_COPY_SVEC(&it5->unk8, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        r5 = s + (ex->btlfx_randCentered(0x46) + 0x400);
                        c.vy = ex->btlfx_rand();
                        c.vx = (ex->btlfx_rand() & 0xFF) + 0x80;
                        ex->func_800CFD78((s16 *)&b, (s16 *)&c, r5, (s16 *)&b);
                        BTLFX_COPY_SVEC(&it5->unk0, &b);
                        it5->unk10 = work->unk14 % 16;
                        it5->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                        it5->unk12 = ex->btlfx_rand() % 13;
                        it5->unk13 = ex->btlfx_rand() % 13;
                        it5->unk15 = 0;
                        it5->unk14 = 2;
                    }
                }
            }
            if (frame & 1) {
                if (ex->btlfx_rand() & 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x10, 0x20, 0x46);
                }
            }
            if (frame >= 31) {
                return 1;
            }
            break;
        }
    }
    return 0;
}
