#include "fxmod/fx_023.h"

s32 fx_023_op80(s32 mode, Fx023Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    SVECTOR risePos;
    SVECTOR holdPos;
    SVECTOR fallPos;

    if (mode == 0) {
        *args = 0x3C;
    } else if (mode == 1) {
        fx_023_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos14);
        ex->btlfx_getUnitHomePos(0, (s16 *)&work->pos1C);
        work->rise34 = -1;
        work->fall36 = -1;
        work->held38 = 0;
        work->pos1C.vx = work->pos14.vx;
        work->pos1C.vz = work->pos14.vz;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 5:
            work->rise34 = 0x1E;
            work->pos24.vx = work->pos1C.vx;
            work->pos24.vy = work->pos1C.vy;
            work->pos24.vz = work->pos1C.vz;
            work->target2C.vx = work->pos1C.vx;
            work->target2C.vy = work->pos1C.vy;
            work->target2C.vz = work->pos1C.vz;
            work->target2C.vy = -1000;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PSYCHOKINESIS_GLYPH_ORBS), (s32)&work->pos24);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PSYCHOKINESIS_WHITE_RINGS), (s32)&work->pos14);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 40:
            work->fall36 = 3;
            work->held38 = 0;
            work->target2C.vy = work->pos1C.vy;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PSYCHOKINESIS_WHITE_RINGS_2), (s32)&work->pos14);
            break;
        }
        if (work->rise34 >= 0) {
            ex->btlfx_getUnitPos(0, (s16 *)&risePos);
            risePos.vy = fxm_lerpRatio(work->pos1C.vy, -1000, 0x1E - work->rise34, 0x1E);
            ex->btlfx_setUnitPos(0, (s16 *)&risePos);
            if (--work->rise34 == 0) {
                work->held38 = 1;
            }
        }
        if (work->held38 != 0) {
            ex->btlfx_getUnitPos(0, (s16 *)&holdPos);
            holdPos.vy = -1000;
            ex->btlfx_setUnitPos(0, (s16 *)&holdPos);
        }
        if (work->fall36 >= 0) {
            ex->btlfx_getUnitPos(0, (s16 *)&fallPos);
            ex->btlfx_getUnitHomePos(0, (s16 *)&work->pos1C);
            work->pos1C.vx = fallPos.vx;
            work->pos1C.vz = fallPos.vz;
            fallPos.vy = fxm_lerpRatio(-1000, work->pos1C.vy, 3 - work->fall36, 3);
            ex->btlfx_setUnitPos(0, (s16 *)&fallPos);
            if (--work->fall36 == 0) {
                ex->btlfx_setUnitPos(0, (s16 *)&work->pos14);
                ex->btlfx_getUnitHomePos(0, (s16 *)&work->pos1C);
                work->pos1C.vx = work->pos14.vx;
                work->pos1C.vz = work->pos14.vz;
                work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PSYCHOKINESIS_GREY_RINGS), (s32)&work->pos14);
                work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PSYCHOKINESIS_LIGHT_RAYS), (s32)&work->pos14);
                if (work->emit10 != NULL) {
                    work->emit10->trails = (s32)fx_023_trails;
                    work->emit10->trailSlots = 0x3C;
                    work->emit10->trailStride = 0x18;
                }
            }
        }
        if (frame >= 5 && work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->target = (s32)&work->target2C;
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_023_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_023/nonmatchings/fx_023", fx_023_headerPtr);
