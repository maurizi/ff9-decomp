#include "fxmod/fx_01c.h"

s32 fx_01c_op80(s32 mode, Fx01cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x2C;
    } else if (mode == 1) {
        fx_01c_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk14);
        if (env->targets->targetCount == 1) {
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk1C);
        } else {
            work->unk1C.vx = env->targets->centerX;
            work->unk1C.vy = env->targets->centerY;
            work->unk1C.vz = env->targets->centerZ;
        }
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk14);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk14);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        ex->btlfx_aimAngles((s16 *)&work->unk14, (s16 *)&work->unk1C, (s16 *)&work->unk24);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 30:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk14);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk14);
            if (work->emitC != NULL) {
                work->emitC->unk14 = (s32)D_801E7B48;
                work->emitC->unk12 = 8;
                work->emitC->unkC = 0x90;
            }
            break;
        case 50:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk1C);
            if (work->emit10 != NULL) {
                work->emit10->otZ = 0x80;
            }
            break;
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->unk24.vx;
                work->emit0->rot.vy = work->unk24.vy;
                work->emit0->rot.vz = work->unk24.vz;
            }
        }
        if (frame >= 0x1E) {
            if (work->emit8 != NULL) {
                if (work->emit8->state == BTLFX_EMIT_DONE) {
                    work->emit8 = NULL;
                }
                if (work->emit8 != NULL) {
                    work->emit8->rot.vx = work->unk24.vx;
                    work->emit8->rot.vy = work->unk24.vy;
                    work->emit8->rot.vz = work->unk24.vz;
                }
            }
            if (work->emitC != NULL) {
                if (work->emitC->state == BTLFX_EMIT_DONE) {
                    work->emitC = NULL;
                }
                if (work->emitC != NULL) {
                    work->emitC->rot.vx = work->unk24.vx;
                    work->emitC->rot.vy = work->unk24.vy;
                    work->emitC->rot.vz = work->unk24.vz;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_01c/nonmatchings/fx_01c", fx_01c_headerPtr);
