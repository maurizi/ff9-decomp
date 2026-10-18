/* Draft of fx_038 func_801E7704 (falling-rock pool callback). Status: 108 aligned mismatches, 22 ignoring registers
 * (was 203/57). Round 5: case 0's t is its own variable u (it now gets s3), case 1's later file pointers use their own variable g (frame now in s7, &it->unk4 in fp,
 * work spilled, as in the original), and the unkC updates in vx, vy, vz order. Open: env and case 0's u swap (s4 / s1 here,
 * s1 / s4 in the original): env (priority 0.066) must be allocated before u (2 * 4 refs / 89 = 0.090); plus case 1's
 * k / q / file registers. tools/equiv.py agreed with the earlier draft
 * (47/47 trials). Earlier levers: `a.vz += 0x1000 - t`, the tint value as a block-local
 * `c = 0x46; c -= frame * 0x46 / 4;`. */
#include "fxmod/fx_038.h"

s32 func_801E7704(s32 mode, Fx038Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx038Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR a;
    SVECTOR b;
    SVECTOR d;
    u8 *f;
    s32 t;
    s32 k;
    s32 q;
    s32 v;
    s32 u;
    u8 *g;

    if (mode == 1) {
        it->unk0 = 0;
        it->unk14 = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&it->unk4);
        if (it->unk14 != NULL) {
            it->unk14->unk22 = 0x10;
        }
        return 0;
    }
    switch (it->unk0) {
    case 0:
        u = (frame << 12) / 15;
        if (frame == 0) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 4), 0, NULL, -1, 0x1000, 0, 1,
                              (u8 *)&D_801E857C[it->unk20], 0);
        }
        BTLFX_COPY_SVEC(&a, &it->unk18);
        a.vy -= (0x1000 - u) * 2;
        a.vz += 0x1000 - u;
        BTLFX_COPY_SVEC(&it->unk4, &a);
        pos.vx = it->unk4.vx;
        pos.vy = it->unk4.vy;
        pos.vz = it->unk4.vz;
        it->unkC.vx += 0x46;
        it->unkC.vy += 0x40;
        it->unkC.vz += 8;
        scale.vz = 0x199;
        scale.vy = 0x199;
        scale.vx = 0x199;
        ex->Hi_DrawEffModel(&it->unkC, &pos, (s32)&scale, work->unk10);
        env->unk28 = 0x100;
        BTLFX_COPY_SVEC(&b, &it->unk4);
        b.vy += 0x140;
        b.vz -= 0xA0;
        f = (u8 *)ex->btlfx_getFileData(entry->unk8, 4);
        ex->btlfx_drawTrail(f, (s32)&b, NULL, 0, 0x6000, ex->math_cos(u >> 2) * 2 / 3 + 0x555, 1,
                          (u8 *)&D_801E857C[it->unk20], 0);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &it->unk4, NULL, 0x1000, 0, 1, -1, 0, 0);
        env->unk28 = 0;
        if (it->unk20 == 0) {
            env->unk2C->unk0[0].vx = it->unk4.vx;
            env->unk2C->unk0[0].vy = it->unk4.vy;
            env->unk2C->unk0[0].vz = it->unk4.vz;
        }
        if (it->unk14 != NULL && it->unk14->unk30 == -1) {
            it->unk14 = NULL;
        }
        if (frame >= 15) {
            it->unk0 = 1;
            ((s16 *)it)[-1] = -1;
            if (it->unk14 != NULL) {
                it->unk14->unk38 = it->unk14->unk24->unk4.unk2;
            }
            it->unk4.vy = 0;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x18, 0x18, 0x18);
            it->unk14 = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&it->unk4);
            if (it->unk14 != NULL) {
                it->unk14->unk22 = 0x46;
            }
            it->unkC.vx = 0;
            it->unkC.vy = ex->btlfx_rand();
            it->unkC.vz = 0;
        }
        break;
    case 1:
        t = (frame << 12) / 18;
        if (frame < 4) {
            s32 c = 0x46;
            c -= frame * 0x46 / 4;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c, c, c);
        }
        f = (u8 *)ex->btlfx_getFileData(entry->unk8, 1);
        k = t >> 2;
        ex->btlfx_drawParticles(f, &it->unk4, NULL, (t << 1) + 0x1800, 0, 1, ex->math_cos(k), 0, 0);
        BTLFX_COPY_SVEC(&b, &it->unk4);
        b.vy += 0x140;
        b.vz -= 0xA0;
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 4), (s32)&it->unk4, NULL, 0, 0, 0x555, 1,
                          (u8 *)&D_801E857C[it->unk20], 0);
        it->unkC.vy += ex->math_cos(k) >> 7;
        pos.vx = it->unk4.vx;
        pos.vy = it->unk4.vy;
        pos.vz = it->unk4.vz;
        if (frame < 8) {
            t = frame << 9;
            q = t >> 2;
            k = ex->math_cos(q);
            v = (ex->math_sin(q) / 2 + 0x800) / 3;
            scale.vz = v;
            scale.vx = v;
            scale.vy = (v * 9) >> 3;
            ex->Hi_DrawEffModel(&it->unkC, &pos, (s32)&scale, work->unk14);
            ex->Hi_ModifyEffModelRGB(work->unk14, ((k * 2 - 0x1000) >> 5) - 0x80, ((k * 2 - 0x1000) >> 5) - 0x80,
                                     ((k * 2 - 0x1000) >> 5) - 0x80);
        }
        t = (frame << 12) / 18;
        k = 0x1000 - t;
        q = t >> 2;
        scale.vz = scale.vx = (ex->math_sin(q) / 3 + 0xAAA) * 3 / 4;
        scale.vy = ex->math_cos(q) * 2 / 3 + 0x800;
        ex->Hi_DrawEffModel(&it->unkC, &pos, (s32)&scale, work->unk18);
        ex->Hi_ModifyEffModelRGB(work->unk18, (k >> 5) - 0x80, (k >> 5) - 0x80, (k >> 5) - 0x80);
        d.pad = 1;
        d.vx = 0x400;
        d.vz = 0;
        d.vy = 0;
        g = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xD);
        ex->func_800D23D8(g, &it->unk4, &d, ex->math_sin(q) + 0xAAA, 8, 1, k);
        if (frame < 12) {
            d.pad = 1;
            d.vx = 0x400;
            d.vz = 0;
            d.vy = 0;
            q = ((frame << 12) / 12) >> 2;
            g = (u8 *)ex->btlfx_getFileData(entry->unk8, 5);
            ex->btlfx_drawParticles(g, &it->unk4, &d, ex->math_cos(q) * 3, 6, 1, ex->math_cos(q), 0, 0);
        }
        if (frame >= 18) {
            return 1;
        }
        break;
    }
    return 0;
}
