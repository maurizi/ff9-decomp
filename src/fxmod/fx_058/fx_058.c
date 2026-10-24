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
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 2), (s32)&it->unkC, NULL, -1, 0, 0, 0, it->unk1C,
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
            /* The empty loop's notes end cse's block here, so the 1 passed to the second drawTrail below is loaded
             * with li instead of reusing the register holding the mode test's 1. */
            do {
            } while (0);
            ex->btlfx_aimAngles((s16 *)&it->unkC, (s16 *)&it->unk20, (s16 *)&a);
            ex->btlfx_turnToward((s16 *)&a, (s16 *)&it->unk14, it->unk8);
            a.vz = frame << 5;
            ex->btlfx_pointAhead((s16 *)&it->unkC, (s16 *)&it->unk14, it->unk4, (s16 *)&it->unkC);
            pos.vx = it->unkC.vx;
            pos.vy = it->unkC.vy;
            pos.vz = it->unkC.vz;
            scale.vx = scale.vy = 0x249;
            scale.vz = it->unk20.pad;
            it->unk20.pad += 0x97;
            it->unk14.vx += 0x800;
            ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, work->unk24);
            it->unk14.vx -= 0x800;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 2), (s32)&it->unkC, NULL, 0, 0x800, 0x1000, 1,
                              it->unk1C, 0);
            d.vx = work->unk4.vx - it->unkC.vx;
            d.vy = work->unk4.vy - it->unkC.vy;
            d.vz = work->unk4.vz - it->unkC.vz;
            if (d.vx * d.vx + d.vy * d.vy + d.vz * d.vz < 0x40000 || frame >= 0x11) {
                it->unk0 = 1;
                ((s16 *)it)[-1] = -1;
                if (work->unk20 & 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x20, 0x10, 0);
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
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 9), &d, NULL, 0x4000, frame, 1, -1, 0, 0);
            env->unk28 = 0;
            if (frame < 0x10) {
                BTLFX_COPY_SVEC(&d, &it->unkC);
                d.vy = 0;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 8), &d, &a, 0x1800, frame, 1, -1, 0, 0);
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 2), (s32)&it->unkC, NULL, 0, 0x800, 0x1000, 1,
                              it->unk1C, 0);
            if (frame >= 0x18) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_058_op80(s32 mode, Fx058Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx058Item *it;
    s32 k;
    s16 j;

    if (mode == 0) {
        init->unk0 = 0x28;
        init->unk8 = 0x28;
        init->unk14 = (s32)func_801E7704;
        init->unk4 = 0x20;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk1C = 0;
        work->unk20 = 0;
        D_801E7E64 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk14);
        work->unk14.vy += 0x800;
        ex->btlfx_initEffModels(D_801E7E74, 1);
        work->unk24 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xA), 1);
    } else if (work->unk0 == 0) {
        if (work->unk1C < 0x20) {
            work->unk1C++;
            it = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (it != NULL) {
                BTLFX_COPY_SVEC(&it->unkC, &work->unkC);
                it->unk20.vx = work->unk4.vx;
                it->unk20.vz = work->unk4.vz;
                it->unk20.vy = work->unk4.vy;
                k = work->unk1C & 3;
                j = work->unk14.vy - 0x300;
                it->unk14.vy = j + k * 0x200;
                k = work->unk1C >> 2;
                it->unk14.vx = work->unk14.vx + k * 0xFF;
                it->unk14.vz = 0;
                it->unk1C = D_801E7EF0[work->unk1C];
            }
        }
        work->unk20++;
        if (work->unk1C >= 0x20) {
            return 1;
        }
    }
    return 0;
}

s32 fx_058_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 1) {
        D_801E7E64 = env;
        ex->btlfx_clearEffModels();
    }
    return 1;
}

INCLUDE_RODATA("asm/us/fx_058/nonmatchings/fx_058", fx_058_headerPtr);
