#include "fxmod/fx_038.h"

INCLUDE_ASM("asm/us/fx_038/nonmatchings/fx_038", func_801E7704);

s32 fx_038_op80(s32 mode, Fx038Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u8 spawn;
    Fx038Item *it;

    if (mode == 0) {
        init->unk0 = 0x1C;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801E7704;
        init->unk4 = 3;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unkC = 0;
        D_801E830C = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        ex->btlfx_initEffModels(D_801E831C, 3);
        work->unk10 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0x97, 0x3C80, 0, 0);
        work->unk18 =
            ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0x137, 0xFFFF, -0x40, 0x80);
        work->unk14 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xA));
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            spawn = 0;
            if (frame < 0x18) {
                spawn = D_801E82AC[frame];
            }
            if (spawn != 0 && work->unkC < 3) {
                work->unkC++;
                it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it != NULL) {
                    it->unk18.vx = work->unk4.vx + ex->btlfx_randCentered(0x258);
                    it->unk18.vz = work->unk4.vz + ex->btlfx_randCentered(0x258);
                    it->unk18.vy = 0;
                    it->unk20 = work->unkC - 1;
                    it->unkC.vx = ex->btlfx_rand();
                    it->unkC.vy = ex->btlfx_rand();
                    it->unkC.vz = ex->btlfx_rand();
                }
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xC), frame);
            if (work->unkC >= 3 && frame >= 0x3E) {
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_038/nonmatchings/fx_038", fx_038_headerPtr);
