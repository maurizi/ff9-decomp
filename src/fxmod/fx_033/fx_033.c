#include "fxmod/fx_033.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_033_env);
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
        fx_033_env = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitHomePos(0, (s16 *)&work->unk14);
        work->emit2C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos4);
        ex->btlfx_aimAngles((s16 *)&work->pos4, (s16 *)&work->unkC, (s16 *)&work->unk24);
        if (work->emit2C != NULL) {
            work->emit2C->trails = (s32)D_801E7AF4;
            work->emit2C->trailSlots = 0x2E;
            work->emit2C->trailStride = 0x4C;
        }
    } else {
        frame = entry->frame;
        if (work->emit2C != NULL) {
            if (work->emit2C->state == BTLFX_EMIT_DONE) {
                work->emit2C = NULL;
            }
            if (work->emit2C != NULL) {
                work->emit2C->rot.vx = work->unk24.vx;
                work->emit2C->rot.vy = work->unk24.vy;
                work->emit2C->rot.vz = work->unk24.vz;
            }
        }
        if (frame == 0x18) {
            work->unk0 = frame;
        }
        if (work->unk0 != 0) {
            work->unk0--;
            d = fxm_easeLerp(1, 0x18, work->unk0, 0, 0x2000);
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
