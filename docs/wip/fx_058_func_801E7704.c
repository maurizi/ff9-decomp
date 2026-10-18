/* Draft of fx_058 func_801E7704 (pool callback). Status: 3 aligned mismatches: the 1 passed as the 7th argument of
 * the second func_800D1494 call in case 0 is 'li v0,1' in the original but CSE reuses s7 (the constant 1 of the
 * mode test) here. */
#include "fxmod/fx_058.h"

s32 func_801E7704(s32 mode, Fx058Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx058Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR d;
    VECTOR pos;
    VECTOR scale;

    if (mode == 1) {
        it->unk0 = 0;
        it->unk4 = (ex->btlfx_rand() & 0x1F) + 0x27;
        it->unk8 = ex->btlfx_rand() & 0x1F;
        it->unk20.pad = 0x249;
    } else {
        switch (it->unk0) {
        case 0:
            if (frame == 0) {
                ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 2), (s32)&it->unkC, NULL, -1, 0, 0, 0, it->unk1C,
                                  0);
            }
            it->unk4 += 0x20;
            if (it->unk4 > 0x300) {
                it->unk4 = 0x300;
            }
            it->unk8 += 0x66;
            if (it->unk8 >= 0x1000) {
                it->unk8 = 0x1000;
            }
            ex->btlfx_aimAngles((s16 *)&it->unkC, (s16 *)&it->unk20, (s16 *)&a);
            ex->btlfx_turnToward((s16 *)&a, (s16 *)&it->unk14, it->unk8);
            a.vz = frame << 5;
            ex->func_800CFD78((s16 *)&it->unkC, (s16 *)&it->unk14, it->unk4, (s16 *)&it->unkC);
            pos.vx = it->unkC.vx;
            pos.vy = it->unkC.vy;
            pos.vz = it->unkC.vz;
            scale.vx = scale.vy = 0x249;
            scale.vz = it->unk20.pad;
            it->unk20.pad += 0x97;
            it->unk14.vx += 0x800;
            ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, work->unk24);
            it->unk14.vx -= 0x800;
            ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 2), (s32)&it->unkC, NULL, 0, 0x800, 0x1000, 1,
                              it->unk1C, 0);
            d.vx = work->unk4.vx - it->unkC.vx;
            d.vy = work->unk4.vy - it->unkC.vy;
            d.vz = work->unk4.vz - it->unkC.vz;
            if (d.vx * d.vx + d.vy * d.vy + d.vz * d.vz < 0x40000 || frame >= 0x11) {
                it->unk0 = 1;
                ((s16 *)it)[-1] = -1;
                if (work->unk20 & 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x20, 0x10, 0);
                }
            }
            break;
        case 1:
            a.vx = 0x400;
            a.vy = 0;
            a.vz = frame << 5;
            a.pad = 1;
            BTLFX_COPY_SVEC(&d, &it->unkC);
            d.vy -= frame * 4;
            env->unk28 = 0x40;
            ex->func_800D1680((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &d, NULL, 0x4000, frame, 1, -1, 0, 0);
            env->unk28 = 0;
            if (frame < 0x10) {
                BTLFX_COPY_SVEC(&d, &it->unkC);
                d.vy = 0;
                ex->func_800D1680((u8 *)ex->btlfx_getFileData(entry->unk8, 8), &d, &a, 0x1800, frame, 1, -1, 0, 0);
            }
            ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 2), (s32)&it->unkC, NULL, 0, 0x800, 0x1000, 1,
                              it->unk1C, 0);
            if (frame >= 0x18) {
                return 1;
            }
            break;
        }
    }
    return 0;
}
