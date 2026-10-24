#include "fxmod/fx_18c.h"

s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7B10);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

s32 fx_18c_op80(s32 mode, Fx18cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E7B10 = env;
        for (i = 2; i >= 0; i--) {
            D_801E7B20[i] = 0;
        }
        ex->btlfx_getUnitHomePos(0, (s16 *)&work->unk0);
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk0);
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk0);
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            D_801E7B20[1] = 0x28;
            D_801E7B20[0] = 0x10;
            break;
        case 24:
            D_801E7B20[2] = 0x10;
            break;
        case 40:
            ex->btlfx_getUnitHomePos(0, (s16 *)&work->unk0);
            ex->btlfx_setUnitPos(0, (s16 *)&work->unk0);
            break;
        }
        if (D_801E7B20[0] != 0) {
            D_801E7B20[0]--;
            work->unk8.vy = func_801E7704(2, 0x10, D_801E7B20[0], work->unk0.vy, work->unk0.vy - 0x320);
        }
        if (D_801E7B20[2] != 0) {
            D_801E7B20[2]--;
            work->unk8.vy = func_801E7704(0, 0x10, D_801E7B20[2], work->unk0.vy - 0x320, work->unk0.vy);
        }
        if (D_801E7B20[1] != 0) {
            D_801E7B20[1]--;
            pos.vx = work->unk0.vx + (ex->math_sin(frame << 6) >> 7);
            pos.vy = work->unk8.vy + (ex->math_sin(frame << 8) >> 5);
            pos.vz = work->unk0.vz + (ex->math_sin(frame << 6) >> 7);
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        return frame >= 0x48;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18c/nonmatchings/fx_18c", fx_18c_headerPtr);
