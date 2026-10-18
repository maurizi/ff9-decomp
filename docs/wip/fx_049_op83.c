/* Draft of fx_049 op83 (bolt spawner, like fx_005 op83 which matches). Status: 14 aligned mismatches, all in mode
 * 1: the original computes &D_801E9738 into v0 between the vx and vy stores and copies it into s0 (q); 'q =
 * work->unk8 = D_801E9738' (which matched fx_005) leaves the order different. */
#include "fxmod/fx_049.h"

s32 fx_049_op83(s32 mode, Fx049Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    Fx049Bolt *it;
    Pool6CEntry *p;
    Pool6CEntry *p2;
    s32 *q;
    s32 frame;
    s32 kind;
    s32 i;
    s32 r;
    s32 ang;
    s32 c;

    if (mode == 0) {
        init->unk0 = 0x10;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801E7F5C;
        init->unk4 = 0xC;
    } else {
        i = 0;
        if (mode == 1) {
            work->unk0.vx = env->unkC->unk38;
            work->unk0.vy = env->unkC->unk3A;
            work->unk0.vz = env->unkC->unk3C;
            D_801E8D88 = env;
            work->unk0.vy = 0;
            q = work->unk8 = D_801E9738;
            for (; i < 0x40; i++) {
                *q++ = ex->btlfx_randCentered(0x100);
                *q++ = ex->btlfx_randCentered(0x200);
                *q++ = ex->btlfx_randCentered(0xF5);
            }
            D_801E9728 = entry;
            work->unkC = 0;
    } else {
        frame = entry->unk0;
        kind = 0;
        if (frame < 0x10) {
            kind = D_801E8D20[frame];
        }
        if (kind > 0) {
            work->unkC++;
            BTLFX_COPY_SVEC(&a, &work->unk0);
            if (kind != 2) {
                a.vx += ex->btlfx_randCentered(0x200);
                a.vy += ex->btlfx_randCentered(0x300);
                a.vz += ex->btlfx_randCentered(0x200);
            }
            if (kind != 3) {
                it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it != NULL) {
                    BTLFX_COPY_SVEC(&it->unk0, &a);
                    BTLFX_COPY_SVEC(&b, &a);
                    b.vy -= 0xC00;
                    r = ex->btlfx_randCentered(0x100) + 0x640;
                    if (kind != 2) {
                        r = r * 3 / 8;
                    }
                    ang = (work->unkC << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    b.vx += (ex->math_cos(ang) * r) >> 12;
                    b.vz += (ex->math_sin(ang) * r) >> 12;
                    if (kind != 2) {
                        it->unk0.vx = b.vx;
                        it->unk0.vz = b.vz;
                    }
                    BTLFX_COPY_SVEC(&it->unk8, &b);
                    it->unk10 = work->unkC % 12;
                    it->unk18 = work->unk8 + (ex->btlfx_rand() & 0x1F);
                    it->unk12 = ex->btlfx_rand() % 14;
                    it->unk13 = ex->btlfx_rand() % 14;
                    it->unk15 = 0;
                    if (kind == 1) {
                        it->unk14 = 2;
                    } else if (kind == 2) {
                        it->unk14 = ex->btlfx_rand() & 1;
                    }
                }
            } else {
                for (kind = 0; kind < 2; kind++) {
                    it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (it != NULL) {
                        BTLFX_COPY_SVEC(&it->unk0, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        b.vy -= 0xC00;
                        r = ex->btlfx_randCentered(0x80) + 0x80;
                        ang = ((work->unkC + kind) << 11) - 0x200 + ex->btlfx_randCentered(0x80);
                        b.vx += (ex->math_cos(ang) * r) >> 12;
                        b.vz += (ex->math_sin(ang) * r) >> 12;
                        BTLFX_COPY_SVEC(&it->unk8, &b);
                        it->unk10 = work->unkC % 12;
                        it->unk18 = work->unk8 + (ex->btlfx_rand() & 0x1F);
                        it->unk12 = ex->btlfx_rand() % 14;
                        it->unk13 = ex->btlfx_rand() % 14;
                        it->unk14 = ex->btlfx_rand() & 1;
                        it->unk15 = 0x80;
                    }
                }
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk0);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk0);
            }
        }
        if (frame == 0xF) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x40, 0x40, 0x40);
        }
        if (frame >= 0x10 && frame < 0x13) {
            c = 200 - (frame - 0x10) * 200 / 3;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c >> 1, c >> 1, c);
        }
        if (frame == 0x14) {
            p2 = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk0);
            if (p2 != NULL) {
                p2->unk18->unk24 = (void (*)())func_801E7CEC;
            }
        }
        if (frame >= 0x22 && frame - 0x22 < env->unkC->unk24) {
            ex->btlfx_getUnitBodyPos(frame - 0x22, 0, (s16 *)&b);
            it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
            if (it != NULL) {
                BTLFX_COPY_SVEC(&it->unk0, &b);
                BTLFX_COPY_SVEC(&it->unk8, &work->unk0);
                it->unk10 = work->unkC % 12;
                it->unk18 = work->unk8 + (ex->btlfx_rand() & 0x1F);
                it->unk12 = ex->btlfx_rand() % 14;
                it->unk13 = ex->btlfx_rand() % 14;
                it->unk14 = ex->btlfx_rand() & 1;
                it->unk15 = 0xFF;
            }
        }
        if (frame >= 0x30) {
            return 1;
        }
        }
    }
    return 0;
}
