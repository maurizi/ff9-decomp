#include "fxmod/fx_0a5.h"

s32 fx_0a5_op80(s32 mode, Fx0a5Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    s32 ratio;

    if (mode == 0) {
        *args = sizeof(Fx0a5Work);
    } else if (mode == 1) {
        fx_0a5_env = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        work->pos18.vx = env->targets->centerX;
        work->pos18.vy = env->targets->centerY;
        work->pos18.vz = env->targets->centerZ;
        work->pos20.vx = env->targets->centerX;
        work->pos20.vy = env->targets->centerY;
        work->pos20.vz = env->targets->centerZ;
        work->pos18.vx = FX_SPORE_SWEEP_X;
        work->pos20.vx = -FX_SPORE_SWEEP_X;
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SPORE_SPORES), (s32)&work->pos8);
        ex->btlfx_aimAngles((s16 *)&work->pos8, &env->targets->centerX, (s16 *)&work->aim10);
    } else {
        frame = entry->frame;
        if (frame == FX_SPORE_GLOW_FRAME) {
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SPORE_GREEN_GLOW), (s32)&work->pos18);
            if (work->emit0 != NULL) {
                work->emit0->trails = (s32)fx_0a5_trails;
                work->emit0->trailSlots = FX_SPORE_GREEN_GLOW_TRAIL_SLOTS;
                work->emit0->trailStride = FX_SPORE_GREEN_GLOW_TRAIL_STRIDE;
            }
            if (work->emit0 != NULL) {
                work->emit0->otZ = 0x80;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim10.vx;
                work->emit4->rot.vy = work->aim10.vy;
                work->emit4->rot.vz = work->aim10.vz;
            }
        }
        if (frame >= FX_SPORE_GLOW_FRAME) {
            t = (frame << 12) / FX_SPORE_SWEEP_FRAMES;
            ratio = 0x1000;
            if (t <= 0x1000) {
                ratio = t;
            }
            ex->btlfx_lerpVec((s16 *)&work->pos18, (s16 *)&work->pos20, ratio, (s16 *)&work->pos28);
            if (work->emit0 != NULL) {
                if (work->emit0->state == BTLFX_EMIT_DONE) {
                    work->emit0 = NULL;
                }
                if (work->emit0 != NULL) {
                    work->emit0->target = (s32)&work->pos28;
                }
            }
        }
        return frame >= FX_SPORE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a5/nonmatchings/fx_0a5", fx_0a5_headerPtr);
