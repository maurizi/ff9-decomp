#include "fxmod/fx_036.h"

static s32 fx_036_op80SpawnPoint(SVECTOR *out, SVECTOR *dir, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_036_env);

    out->vx = fx_036_spawnX[fx_036_spawnCount];
    out->vy = fx_036_targetPos.vy - ex->btlfx_rand() % 400;
    out->vz = fx_036_targetPos.vz;
    if (ex->btlfx_rand() & 1) {
        out->vz += ex->btlfx_rand() % 600;
    } else {
        out->vz -= ex->btlfx_rand() % 600;
    }
    out->pad = 1;
    fx_036_spawnCount++;
    return 0;
}

s32 fx_036_op80(s32 mode, Fx036Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x2C;
    } else if (mode == 1) {
        fx_036_targetPos.vx = env->targets->centerX;
        fx_036_targetPos.vy = env->targets->centerY;
        fx_036_env = env;
        fx_036_targetPos.vz = env->targets->centerZ;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 1:
            ex->btlfx_getUnitBonePos(0x10, 1, (u16 *)&work->pos1C);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos1C);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 5:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos1C);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos1C);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos1C);
            break;
        case 9:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos24);
            if (work->emit10 != NULL) {
                work->emit10->tail->shape = -1;
                work->emit10->tail->s.module.spawnFn = fx_036_op80SpawnPoint;
            }
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&env->targets->centerX);
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&env->targets->centerX);
            if (work->emit10 != NULL) {
                work->emit10->otZ = 0x100;
            }
            break;
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_036_op81(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 2;
    } else if (mode == 1) {
        fx_036_env = env;
        *work = 0x80;
    } else {
        ex->btlfx_fadeUnitById(0x10, *work);
        *work -= 8;
        return *work < 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_036/nonmatchings/fx_036", fx_036_headerPtr);
