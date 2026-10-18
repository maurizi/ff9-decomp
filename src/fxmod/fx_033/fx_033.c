#include "fxmod/fx_033.h"

s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7AE4);
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

s32 fx_033_op80(s32 mode, Fx033Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 d;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7AE4 = env;
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitHomePos(0, (s16 *)&work->unk14);
        work->unk2C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk4);
        ex->btlfx_aimAngles((s16 *)&work->unk4, (s16 *)&work->unkC, (s16 *)&work->unk24);
        if (work->unk2C != NULL) {
            work->unk2C->unk14 = (s32)D_801E7AF4;
            work->unk2C->unk12 = 0x2E;
            work->unk2C->unkC = 0x4C;
        }
    } else {
        frame = entry->unk0;
        if (work->unk2C != NULL) {
            if (work->unk2C->unk30 == -1) {
                work->unk2C = NULL;
            }
            if (work->unk2C != NULL) {
                work->unk2C->unk5C.vx = work->unk24.vx;
                work->unk2C->unk5C.vy = work->unk24.vy;
                work->unk2C->unk5C.vz = work->unk24.vz;
            }
        }
        if (frame == 0x18) {
            work->unk0 = frame;
        }
        if (work->unk0 != 0) {
            work->unk0--;
            d = func_801E7704(1, 0x18, work->unk0, 0, 0x2000);
            work->unk1C.vx = work->unk14.vx + ((ex->math_sin(work->unk24.vy) * d) >> 12);
            c = ex->math_cos(work->unk24.vy) * d;
            work->unk1C.vy = work->unk14.vy - (d >> 6);
            work->unk1C.vz = work->unk14.vz + (c >> 12);
            ex->btlfx_setUnitPos(0, (s16 *)&work->unk1C);
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_033/nonmatchings/fx_033", fx_033_headerPtr);
