#include "fxmod/fx_0ee.h"

s32 fx_0ee_op80(s32 mode, Fx0eeWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        fx_0ee_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot8);
        work->pos0.vx += ex->math_sin(work->rot8.vy) >> 4;
        work->pos0.vz += ex->math_cos(work->rot8.vy) >> 4;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MIRROR_WHITE_RINGS_2), (s32)&work->pos0);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MIRROR_WHITE_RINGS), (s32)&work->pos0);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 8:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MIRROR_FAN_EMITTER), (s32)&work->pos0);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0x10:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MIRROR_GREEN_SWIRL), (s32)&work->pos0);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        }
        return frame >= 0x44;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ee/nonmatchings/fx_0ee", fx_0ee_headerPtr);
