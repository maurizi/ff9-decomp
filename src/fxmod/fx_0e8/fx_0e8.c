#include "fxmod/fx_0e8.h"

s32 fx_0e8_op80(s32 mode, Fx0e8Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    s32 frame;
    s32 step;

    if (mode == 0) {
        *args = 0x44;
    } else if (mode == 1) {
        fx_0e8_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos14);
        work->pos3C.vx = work->pos14.vx;
        work->pos3C.vy = work->pos14.vy;
        work->pos3C.vz = work->pos14.vz;
        work->pos3C.vy -= 0x12C;
        ex->btlfx_getUnitScale(0x10, &work->scale2C);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos3C);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0xC:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos14);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos14);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos14);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_0e8_op80Trails;
                work->emit4->trailSlots = 0x14;
                work->emit4->trailStride = 0xD0;
            }
            break;
        case 0x12:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos14);
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        }
        if (frame >= 0x12 && frame < 0x17) {
            step = frame - 0x12;
            if (step == 4) {
                scale.vx = 0;
                scale.vy = 0;
                scale.vz = 0;
                ex->btlfx_fadeUnitById(0x10, 0);
            } else {
                ex->btlfx_getUnitScale(0x10, &scale);
                scale.vx = work->scale2C.vx - work->scale2C.vx / 4 * step;
                scale.vy += 0x190;
                scale.vz = work->scale2C.vz - work->scale2C.vz / 4 * step;
            }
            ex->btlfx_setUnitScale(0x10, (s32 *)&scale);
        }
        return frame >= 0x2D;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e8/nonmatchings/fx_0e8", fx_0e8_headerPtr);
