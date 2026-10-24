#include "fxmod/fx_103.h"

s32 fx_103_op80(s32 mode, Fx103Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        work->unk8.vx = env->targets->centerX + 0x1F4;
        work->unk8.vy = 0;
        work->unk8.vz = env->targets->centerZ;
        work->unk0.vx = env->targets->centerX - 0x1F4;
        work->unk0.vy = 0;
        work->unk0.vz = env->targets->centerZ;
        work->unk30.vx = env->targets->centerX;
        work->unk30.vy = 0;
        work->unk30.vz = env->targets->centerZ;
        D_801E7A48 = env;
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk10);
        ex->func_800DC858(0x10, 1, (u16 *)&work->unk18);
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk0, (s16 *)&work->unk20);
        ex->btlfx_aimAngles((s16 *)&work->unk18, (s16 *)&work->unk8, (s16 *)&work->unk28);
        work->unk28.vx = -0x200;
        work->unk20.vx = -0x200;
        work->unk38 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk10);
        if (work->unk38 != NULL) {
            work->unk38->unk22 = ex->func_800E7CC4(0, 0x40);
        }
        work->unk3C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk18);
        if (work->unk3C != NULL) {
            work->unk3C->unk22 = ex->func_800E7CC4(0, 0x40);
        }
    } else {
        frame = entry->unk0;
        if (work->unk38 != NULL && work->unk38->unk30 == -1) {
            work->unk38 = NULL;
        }
        if (work->unk3C != NULL && work->unk3C->unk30 == -1) {
            work->unk3C = NULL;
        }
        if (work->unk38 != NULL) {
            work->unk38->unk5C.vx = work->unk20.vx;
            work->unk38->unk5C.vy = work->unk20.vy;
            work->unk38->unk5C.vz = work->unk20.vz;
        }
        if (work->unk3C != NULL) {
            work->unk3C->unk5C.vx = work->unk28.vx;
            work->unk3C->unk5C.vy = work->unk28.vy;
            work->unk3C->unk5C.vz = work->unk28.vz;
        }
        if (frame == 0x10) {
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk30);
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_103/nonmatchings/fx_103", fx_103_headerPtr);
