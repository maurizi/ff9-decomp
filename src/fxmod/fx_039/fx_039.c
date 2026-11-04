#include "fxmod/fx_039.h"

s32 fx_039_op80(s32 mode, Fx039Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        fx_039_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_OSMOSE_SPARKLES), (s32)&work->pos8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_OSMOSE_FAN_EMITTER), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 fx_039_op81(s32 mode, Fx039Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;

    if (mode == 0) {
        *args = 0x24;
    } else if (mode == 1) {
        s32 proj = *args;

        fx_039_env = env;
        work->proj20 = proj;
        if (proj >= 3) {
            return 1;
        }
        if (ex->btlfx_isEnemyUnit(0x10)) {
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        } else {
            ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        }
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_OSMOSE_ABSORB_FLASH), (s32)&work->pos10);
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 0xA) {
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_OSMOSE_DRAIN_ORB), (s32)&fx_039_emitPos[work->proj20]);
            if (work->emit0 != NULL) {
                work->emit0->trails = (s32)fx_039_trails[work->proj20];
                work->emit0->trailSlots = 0x12;
                work->emit0->trailStride = 0xB0;
            }
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x10);
            }
        }
        if (frame >= 0xA) {
            s32 elapsed = frame - 0xA;

            if (elapsed < 0x10) {
                t = (elapsed << 12) / 15;
                ex->btlfx_lerpVec((s16 *)&work->pos10, (s16 *)&work->pos8, t, (s16 *)&work->pos18);
                if (work->proj20 == FX_039_PROJ_ARC_NEG_X) {
                    work->pos18.vx -= (ex->math_sin(t >> 1) * 0x4B0) >> 12;
                } else if (work->proj20 == FX_039_PROJ_ARC_POS_X) {
                    work->pos18.vx += (ex->math_sin(t >> 1) * 0x4B0) >> 12;
                }
                work->pos18.vy -= (ex->math_sin(t >> 1) * 0x258) >> 12;
                if (work->emit0 != NULL && work->emit0->state == BTLFX_EMIT_DONE) {
                    work->emit0 = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit0, (s32)&work->pos18);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_039/nonmatchings/fx_039", fx_039_headerPtr);
