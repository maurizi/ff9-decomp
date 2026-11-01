#include "fxmod/fx_002.h"

s32 fx_002_op80(s32 mode, Fx002Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 scale;
    SVECTOR rot;
    s32 data;
    s32 t;

    if (mode == 0) {
        *args = 0x14;
    } else if (mode == 1) {
        work->stop0 = 0;
        fx_002_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->posC);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (work->stop0 == 0) {
            if (frame < 0x2C) {
                t = (frame << 12) / 44;
                if (frame < 0x17) {
                    scale = ex->math_sin(frame << 6) / 4 + 0x800;
                } else {
                    scale = ex->math_sin(((frame - 0x17) << 10) / 21) * 2 / 3 + 0xC00;
                }
                rot.vx = 0x400;
                rot.vz = 0;
                rot.vy = 0;
                rot.pad = 1;
                data = ex->btlfx_getFileData(entry->file, 0xD);
                ex->btlfx_drawRings((u8 *)data, &work->pos4, &rot, scale * 2 / 3, 0xC, 1, ex->math_sin(t >> 1));
            }
            if (frame == 5) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame == 0x12) {
                Pool6CEntry *em;

                em = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos4);
                if (em != NULL) {
                    em->otZ = 0x80;
                    em->trails = (s32)fx_002_trails;
                    em->trailSlots = 0x15;
                    em->trailStride = 0xAC;
                }
                em = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->posC);
                if (em != NULL) {
                    em->otZ = 0x80;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos4);
            }
            if (frame == 0x17) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= 0x32) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_002/nonmatchings/fx_002", fx_002_headerPtr);
