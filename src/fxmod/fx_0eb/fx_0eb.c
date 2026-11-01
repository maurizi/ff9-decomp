#include "fxmod/fx_0eb.h"

s32 fx_0eb_op80(s32 mode, Fx0ebWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p0;
    SVECTOR p1;
    SVECTOR p2;
    s32 frame;

    if (mode == 0) {
        *args = 0x3C;
    } else if (mode == 1) {
        fx_0eb_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos14);
        ex->btlfx_getUnitHomePos(0, (s16 *)&work->pos1C);
        work->liftTimer34 = -1;
        work->dropTimer36 = -1;
        work->hold38 = 0;
        work->pos1C.vx = work->pos14.vx;
        work->pos1C.vz = work->pos14.vz;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->liftTimer34 = 0xC;
            work->pos24.vx = work->pos1C.vx;
            work->pos24.vy = work->pos1C.vy;
            work->pos24.vz = work->pos1C.vz;
            work->emitTarget2C.vx = work->pos1C.vx;
            work->emitTarget2C.vy = work->pos1C.vy;
            work->emitTarget2C.vz = work->pos1C.vz;
            work->emitTarget2C.vy = -0x3E8;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos24);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos14);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 0x1C:
            work->dropTimer36 = 3;
            work->hold38 = 0;
            work->emitTarget2C.vy = work->pos1C.vy;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos14);
            break;
        }
        if (work->liftTimer34 >= 0) {
            ex->btlfx_getUnitPos(0, (s16 *)&p0);
            p0.vy = fxm_lerpRatio(work->pos1C.vy, -0x3E8, 0xC - work->liftTimer34, 0xC);
            ex->btlfx_setUnitPos(0, (s16 *)&p0);
            if (--work->liftTimer34 == 0) {
                work->hold38 = 1;
            }
        }
        if (work->hold38 != 0) {
            ex->btlfx_getUnitPos(0, (s16 *)&p1);
            p1.vy = -0x3E8;
            ex->btlfx_setUnitPos(0, (s16 *)&p1);
        }
        if (work->dropTimer36 >= 0) {
            ex->btlfx_getUnitPos(0, (s16 *)&p2);
            ex->btlfx_getUnitHomePos(0, (s16 *)&work->pos1C);
            work->pos1C.vx = p2.vx;
            work->pos1C.vz = p2.vz;
            p2.vy = fxm_lerpRatio(-0x3E8, work->pos1C.vy, 3 - work->dropTimer36, 3);
            ex->btlfx_setUnitPos(0, (s16 *)&p2);
            if (--work->dropTimer36 == 0) {
                ex->btlfx_getUnitHomePos(0, (s16 *)&work->pos1C);
                ex->btlfx_setUnitPos(0, (s16 *)&work->pos1C);
                work->pos1C.vx = work->pos14.vx;
                work->pos1C.vz = work->pos14.vz;
                work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos14);
                work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos14);
                if (work->emit10 != NULL) {
                    work->emit10->trails = (s32)fx_0eb_op80Trails;
                    work->emit10->trailSlots = 0x3C;
                    work->emit10->trailStride = 0x18;
                }
            }
        }
        if (frame >= 0) {
            if (work->emit0 != NULL) {
                if (work->emit0->state == BTLFX_EMIT_DONE) {
                    work->emit0 = NULL;
                }
                if (work->emit0 != NULL) {
                    work->emit0->target = (s32)&work->emitTarget2C;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0eb_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0eb/nonmatchings/fx_0eb", fx_0eb_headerPtr);
