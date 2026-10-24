#include "fxmod/fx_1a1.h"

s32 fx_1a1_op80(s32 mode, Fx1a1Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        work->unk4 = 0;
        work->unk0 = 0;
        D_801E7A38 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        work->unk8.vx = env->targets->unk38;
        work->unk8.vz = env->targets->unk3C;
        ex->btlfx_getUnitHomePos(0x10, (s16 *)&work->unk10);
        work->unk18.vx = env->targets->unk38;
        work->unk18.vz = env->targets->unk3C;
        work->unk18.vy = 0;
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk18, (s16 *)&work->unk28);
        work->unk18.vx += (ex->math_sin(work->unk28.vy + 0x800) * 0x514) >> 12;
        work->unk18.vz += (ex->math_cos(work->unk28.vy + 0x800) * 0x514) >> 12;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk4 = 5;
            work->unk0 = 5;
            break;
        case 2:
            work->unk4 = 5;
            work->unk0 = 5;
            work->unk10.vx = work->unk20.vx;
            work->unk10.vy = work->unk20.vy;
            work->unk10.vz = work->unk20.vz;
            break;
        case 10:
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        }
        if (work->unk0 != 0) {
            work->unk0--;
            ex->btlfx_lerpVec((s16 *)&work->unk18, (s16 *)&work->unk10, (0x1000 / work->unk4) * work->unk0,
                              (s16 *)&work->unk20);
            ex->btlfx_setUnitPos(0x10, (s16 *)&work->unk20);
            env->camera->unk0[0].vx = work->unk20.vx;
            env->camera->unk0[0].vz = work->unk20.vz;
            env->camera->unk0[0].vy = 0;
        }
        return frame >= 0x20;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1a1/nonmatchings/fx_1a1", fx_1a1_headerPtr);
