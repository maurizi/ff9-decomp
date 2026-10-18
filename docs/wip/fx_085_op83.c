/* Draft of fx_085 fx_085_op83 (near-twin of fx_018 op83: mode 1 builds the target from env->unkC, case 2 spawns an
 * extra bolt). Status: ~19 aligned diff lines, all in mode 1's first block: the original computes &D_801EAAEC (and
 * copies it into s0) right after the vx store, ours after the vy -= 0x200. fx_049 op83 has the same open block. */
#include "fxmod/fx_085.h"

typedef struct {
    SVECTOR unk0; /* target body */
    SVECTOR unk8; /* target */
    s32 *unk10; /* jitter table */
    s32 unk14; /* bolts spawned */
    s32 unk18; /* kind */
} Fx085Work83; /* 0x1C */

extern SVECTOR D_801E9DEC; /* previous bolt end */
extern Unk80100C78Entry *D_801E9DF8;
extern s32 D_801EAAEC[];

s32 fx_085_op83(s32 mode, Fx085Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    Fx085Bolt *it0;
    s32 r0;
    s32 ang0;
    Fx085Bolt *it1;
    s32 r1;
    s32 ang1;
    Fx085Bolt *it2;
    s32 r2;
    s32 ang2;
    Fx085Bolt *it3;
    Fx085Bolt *it4;
    s32 r4;
    Fx085Bolt *it5;
    Fx085Bolt *it6;
    Pool6CEntry *p;
    s32 *q;
    s32 frame;
    s32 kind;
    s32 rnd2;
    s32 rnd3;
    s32 rnd5;
    s32 base2;
    s32 base3;
    s32 i;
    s32 j;
    s32 s;

    if (mode == 0) {
        init->unk0 = 0x1C;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801E8880;
        init->unk4 = 0x10;
    } else if (mode == 1) {
        i = 0;
        kind = init->unk0;
        work->unk0.vx = env->unkC->unk38;
        work->unk0.vy = env->unkC->unk3A;
        D_801E9DF4 = env;
        work->unk0.vz = env->unkC->unk3C;
        work->unk0.vy -= 0x200;
        BTLFX_COPY_SVEC(&work->unk8, &work->unk0);
        work->unk8.vy = 0;
        q = work->unk10 = D_801EAAEC;
        for (; i < 0x40; i++) {
            *q++ = ex->btlfx_randCentered(0x100);
            *q++ = ex->btlfx_randCentered(0x200);
            *q++ = ex->btlfx_randCentered(0xF5);
        }
        work->unk18 = kind;
        D_801E9DF8 = entry;
        work->unk14 = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk18) {
        case 0:
            D_801EAADC = 0;
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
                        r0 = ex->btlfx_randCentered(0x100) + 0x3E8;
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
            D_801EAADC = 0;
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
                        it1->unk15 = 0;
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
                    rnd2 = ex->btlfx_randCentered(0x100);
                    base2 = (frame - 6) << 8;
                    rnd2 += 0x7D0;
                    r2 = base2 + rnd2;
                    ang2 = ((work->unk14 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    d.vx += (ex->math_cos(ang2) * r2) >> 12;
                    d.vz += (ex->math_sin(ang2) * r2) >> 12;
                    BTLFX_COPY_SVEC(&it2->unk8, &d);
                    it2->unk10 = work->unk14 % 16;
                    it2->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                    it2->unk12 = ex->btlfx_rand() % 13;
                    it2->unk13 = ex->btlfx_rand() % 13;
                    it2->unk15 = 11;
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
                    rnd3 = ex->btlfx_randCentered(0x100);
                    base3 = (frame - 6) << 8;
                    rnd3 += 0xA28;
                    r2 = base3 + rnd3;
                    ang2 = ((work->unk14 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    d.vx += (ex->math_cos(ang2) * r2) >> 12;
                    d.vz += (ex->math_sin(ang2) * r2) >> 12;
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
            D_801EAADC = 1;
            if (frame == 0) {
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk0);
                if (p != NULL) {
                    p->unk24->unk0 = -1;
                    ((EffTailSpawn *)p->unk24)->unk3C = func_801E8C28;
                    p->unk22 = 0x20;
                }
            }
            if (frame < 31) {
                if (frame == 0) {
                    BTLFX_COPY_SVEC(&D_801E9DEC, &work->unk0);
                }
                s = (frame << 9) / 31 + 0x555;
                work->unk14++;
                BTLFX_COPY_SVEC(&a, &work->unk0);
                a.vy = 0;
                c.vz = 0;
                it6 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it6 != NULL) {
                    BTLFX_COPY_SVEC(&it6->unk8, &a);
                    BTLFX_COPY_SVEC(&b, &a);
                    r4 = s + ex->btlfx_randCentered(0x100);
                    c.vy = ex->btlfx_rand();
                    c.vx = (ex->btlfx_rand() & 0xFF) + 0x80;
                    ex->func_800CFD78((s16 *)&b, (s16 *)&c, r4, (s16 *)&b);
                    BTLFX_COPY_SVEC(&it6->unk0, &b);
                    it6->unk10 = work->unk14 % 16;
                    it6->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                    it6->unk12 = ex->btlfx_rand() % 13;
                    it6->unk13 = ex->btlfx_rand() % 13;
                    it6->unk15 = 7 - frame * 60 / 31;
                    it6->unk14 = ex->btlfx_rand() & 1;
                }
                work->unk14++;
                BTLFX_COPY_SVEC(&a, &work->unk0);
                a.vy = 0;
                it4 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it4 != NULL) {
                    BTLFX_COPY_SVEC(&it4->unk0, &b);
                    BTLFX_COPY_SVEC(&it4->unk8, &D_801E9DEC);
                    it4->unk10 = work->unk14 % 16;
                    it4->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                    it4->unk12 = ex->btlfx_rand() % 13;
                    it4->unk13 = ex->btlfx_rand() % 13;
                    it4->unk15 = -9 - frame * 60 / 31;
                    it4->unk14 = ex->btlfx_rand() & 1;
                }
                BTLFX_COPY_SVEC(&D_801E9DEC, &b);
                if (frame < 19) {
                    work->unk14++;
                    BTLFX_COPY_SVEC(&a, &work->unk0);
                    a.vy = 0;
                    c.vz = 0;
                    it5 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (it5 != NULL) {
                        BTLFX_COPY_SVEC(&it5->unk8, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        rnd5 = ex->btlfx_randCentered(0x100) + 0x400;
                        r4 = s + rnd5;
                        c.vy = ex->btlfx_rand();
                        c.vx = (ex->btlfx_rand() & 0xFF) + 0x80;
                        ex->func_800CFD78((s16 *)&b, (s16 *)&c, r4, (s16 *)&b);
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
