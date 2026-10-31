#include "fxmod/fx_18d.h"

s32 fx_18d_op80(s32 mode, Fx18dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        work->duration4 = 0;
        work->timer0 = 0;
        fx_18d_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        work->pos8.vx = env->targets->centerX;
        work->pos8.vz = env->targets->centerZ;
        ex->btlfx_getUnitHomePos(0x10, (s16 *)&work->pos10);
        work->pos18.vx = env->targets->centerX;
        work->pos18.vz = env->targets->centerZ;
        work->pos18.vy = 0;
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos18, (s16 *)&work->rot28);
        work->pos18.vx += (ex->math_sin(work->rot28.vy + 0x800) * 0x320) >> 12;
        work->pos18.vz += (ex->math_cos(work->rot28.vy + 0x800) * 0x320) >> 12;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->duration4 = 0xC;
            work->timer0 = 0xC;
            break;
        case 5:
            work->duration4 = 7;
            work->timer0 = 7;
            work->pos10.vx = work->pos20.vx;
            work->pos10.vy = work->pos20.vy;
            work->pos10.vz = work->pos20.vz;
            break;
        case 13:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        if (work->timer0 != 0) {
            work->timer0--;
            ex->btlfx_lerpVec((s16 *)&work->pos18, (s16 *)&work->pos10, (0x1000 / work->duration4) * work->timer0,
                              (s16 *)&work->pos20);
            ex->btlfx_setUnitPos(0x10, (s16 *)&work->pos20);
            env->camera->points[0].vx = work->pos20.vx;
            env->camera->points[0].vz = work->pos20.vz;
            env->camera->points[0].vy = 0;
        }
        return frame >= 0x20;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18d/nonmatchings/fx_18d", fx_18d_headerPtr);
