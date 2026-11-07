#include "fxmod/fx_099.h"

s32 fx_099_op80(s32 mode, Fx099Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 t;
    s32 scale;
    s32 bright;

    if (mode == 0) {
        *args = sizeof(Fx099Work);
    } else if (mode == 1) {
        work->state0 = FX_BREAK_STATE_RISE;
        work->frame4 = 0;
        fx_099_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->pos8.vy = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_initEffModels(fx_099_effModels, FX_BREAK_MODEL_COUNT);
        work->model1C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BREAK_STONE_CRUST), 0);
        scale = (ex->btlfx_getUnitRadius(0) << 12) / FX_BREAK_CRUST_RADIUS;
        if (scale < 0x800) {
            scale = 0x800;
        }
        if (scale > 0x2000) {
            scale = 0x2000;
        }
        work->scale18 = scale / 2;
        work->rot20.vx = 0;
        work->rot20.vy = 0;
        work->rot20.vz = 0;
        work->scale28[0] = work->scale18;
        work->scale28[1] = work->scale18;
        work->scale28[2] = work->scale18;
        work->modelPos34[0] = work->pos8.vx;
        work->modelPos34[1] = work->pos8.vy;
        work->modelPos34[2] = work->pos8.vz;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_BREAK_STATE_RISE:
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BREAK_ROCK_RING), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
            }
            for (i = 0; i < FX_BREAK_CRUST_PIECES; i++) {
                if (frame >= i * FX_BREAK_PIECE_STAGGER) {
                    t = ((frame - i * FX_BREAK_PIECE_STAGGER) << 12) / FX_BREAK_PIECE_RISE_FRAMES;
                    if (t > 0x1000) {
                        t = 0x1000;
                    }
                    rot.vx = work->rot20.vx;
                    rot.vy = work->rot20.vy;
                    rot.vz = work->rot20.vz;
                    rot.vy += (i << 12) / FX_BREAK_CRUST_PIECES;
                    rot.vx += 0x200 - (ex->math_sin(t >> 2) >> 3);
                    ex->Hi_SetEffModelSlice(work->model1C, 0);
                    ex->Hi_DrawSliceEffModel(&rot, (VECTOR *)work->modelPos34, (s32)work->scale28, work->model1C);
                }
            }
            if (frame >= FX_BREAK_RISE_FRAMES) {
                work->state0 = FX_BREAK_STATE_FADE;
                entry->frame = -1;
            }
            break;
        case FX_BREAK_STATE_FADE:
            if (frame < FX_BREAK_FADE_FRAMES) {
                bright = (ex->math_cos((frame << 10) >> 4) >> 5) - 0x80;
                for (i = 0; i < FX_BREAK_CRUST_PIECES; i++) {
                    if (frame >= i * FX_BREAK_PIECE_STAGGER) {
                        t = ((frame - i * FX_BREAK_PIECE_STAGGER) << 12) / FX_BREAK_PIECE_RISE_FRAMES;
                        if (t > 0x1000) {
                            t = 0x1000;
                        }
                        rot.vx = work->rot20.vx;
                        rot.vy = work->rot20.vy;
                        rot.vz = work->rot20.vz;
                        rot.vy += (i << 12) / FX_BREAK_CRUST_PIECES;
                        ex->Hi_SetEffModelSlice(work->model1C, 0);
                        ex->Hi_DrawSliceEffModel(&rot, (VECTOR *)work->modelPos34, (s32)work->scale28, work->model1C);
                        ex->Hi_ModifyEffModelAbr(work->model1C, BTLFX_ABR_ADD);
                        ex->Hi_ModifyEffModelRGB(work->model1C, bright, bright, bright);
                    }
                }
            }
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BREAK_CYAN_RINGS), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
            if (frame == FX_BREAK_DUST_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BREAK_DUST_CLOUD), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
            if (frame >= FX_BREAK_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_099/nonmatchings/fx_099", fx_099_headerPtr);
