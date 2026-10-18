/* Draft of fx_042 op83 (bolt spawner, like fx_003 op83). Status: ~37 aligned mismatches: the mode-1 setup orders
 * the D_801E8F0C address (copied into s0) differently, and a.vx = a.vx + 0x46 + (rand() % 14) * 0x6E is associated
 * the other way. fx_049 op83 has the same mode-1 issue. */
#include "fxmod/fx_042.h"

s32 fx_042_op83(s32 mode, Fx042Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    Fx042Bolt *it;
    s32 *q;
    s32 i;
    s32 r;
    s32 ang;
    s32 frame;

    if (mode == 0) {
        init->unk0 = 0x10;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801E7CEC;
        init->unk4 = 0xC;
    } else {
        i = 0;
        if (mode == 1) {
            work->unk0.vx = env->unkC->unk38;
            work->unk0.vy = env->unkC->unk3A;
            work->unk0.vz = env->unkC->unk3C;
            D_801E856C = env;
            work->unk0.vy = 0;
            q = work->unk8 = D_801E8F0C;
            for (; i < 0x40; i++) {
                *q++ = ex->btlfx_randCentered(0x100);
                *q++ = ex->btlfx_randCentered(0x180);
                *q++ = ex->btlfx_randCentered(0xC0);
            }
            work->unkC = 0;
        } else {
            frame = entry->unk0;
            if (work->unkC < 0x18) {
                work->unkC++;
                BTLFX_COPY_SVEC(&a, &work->unk0);
                if (work->unkC != 1) {
                    if ((work->unkC / 2) & 1) {
                        a.vx = a.vx + 0x46 + (ex->btlfx_rand() % 14) * 0x6E;
                    } else {
                        a.vx = a.vx - 0x46 - (ex->btlfx_rand() % 14) * 0x6E;
                    }
                    a.vz = a.vz - 0x44C + (ex->btlfx_rand() % 10) * 0xDC;
                }
                it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it != NULL) {
                    BTLFX_COPY_SVEC(&it->unk0, &a);
                    BTLFX_COPY_SVEC(&b, &a);
                    b.vy -= 0xC00;
                    r = ex->btlfx_randCentered(0x100) + 0x400;
                    ang = (work->unkC << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    b.vx += (ex->math_cos(ang) * r) >> 12;
                    b.vz += (ex->math_sin(ang) * r) >> 12;
                    BTLFX_COPY_SVEC(&it->unk8, &b);
                    it->unk10 = work->unkC % 12;
                    it->unk18 = work->unk8 + (ex->btlfx_rand() & 0x1F);
                    it->unk12 = ex->btlfx_rand() % 14;
                    it->unk13 = ex->btlfx_rand() % 14;
                    if (work->unkC & 1) {
                        it->unk14 = 2;
                    } else {
                        it->unk14 = ex->btlfx_rand() & 1;
                    }
                }
            }
            if (frame >= 0x18) {
                return 1;
            }
        }
    }
    return 0;
}
