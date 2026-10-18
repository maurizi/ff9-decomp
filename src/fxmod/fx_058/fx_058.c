#include "fxmod/fx_058.h"

INCLUDE_ASM("asm/us/fx_058/nonmatchings/fx_058", func_801E7704);

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
        work->unk24 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xA), 1);
    } else if (work->unk0 == 0) {
        if (work->unk1C < 0x20) {
            work->unk1C++;
            it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
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
