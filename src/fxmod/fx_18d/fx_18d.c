#include "fxmod/fx_18d.h"

s32 fx_18d_op80(s32 mode, Fx18dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
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
        work->unk8.vx = env->targets->centerX;
        work->unk8.vz = env->targets->centerZ;
        ex->btlfx_getUnitHomePos(0x10, (s16 *)&work->unk10);
        work->unk18.vx = env->targets->centerX;
        work->unk18.vz = env->targets->centerZ;
        work->unk18.vy = 0;
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk18, (s16 *)&work->unk28);
        work->unk18.vx += (ex->math_sin(work->unk28.vy + 0x800) * 0x320) >> 12;
        work->unk18.vz += (ex->math_cos(work->unk28.vy + 0x800) * 0x320) >> 12;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->unk4 = 0xC;
            work->unk0 = 0xC;
            break;
        case 5:
            work->unk4 = 7;
            work->unk0 = 7;
            work->unk10.vx = work->unk20.vx;
            work->unk10.vy = work->unk20.vy;
            work->unk10.vz = work->unk20.vz;
            break;
        case 13:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk8);
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
            env->camera->points[0].vx = work->unk20.vx;
            env->camera->points[0].vz = work->unk20.vz;
            env->camera->points[0].vy = 0;
        }
        return frame >= 0x20;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18d/nonmatchings/fx_18d", fx_18d_headerPtr);
