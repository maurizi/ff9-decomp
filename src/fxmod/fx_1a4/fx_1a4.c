#include "fxmod/fx_1a4.h"

s32 fx_1a4_op80(s32 mode, Fx1a4Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR risePos;
    SVECTOR hopPos;
    s32 frame;
    s32 hopFrame;
    s32 riseFrame;
    s32 alpha;

    if (mode == 0) {
        *args = sizeof(Fx1a4Work80);
    } else if (mode == 1) {
        fx_1a4_env = env;
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos14);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH0_COLOUR_STREAKS), (s32)&work->pos14);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
        }
        work->timer3C = -1;
        work->timer3E = -1;
        work->timer40 = -1;
        work->flag38 = env->targets->fxArgs[1] == 0;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos1C);
        switch (frame) {
        case FX_DEATH0_BURST_FRAME:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH0_DUST_BURST), (s32)&fx_1a4_emitPos0);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH0_FIRE_DEBRIS), (s32)&fx_1a4_emitPos1);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
            }
            break;
        case FX_DEATH0_DEBRIS_FRAME:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH0_ROCK_DEBRIS), (s32)&work->pos1C);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH0_PINK_RINGS), (s32)&work->pos1C);
            break;
        case FX_DEATH0_SINK_FRAME:
            if (work->flag38 != 0) {
                work->timer3E = -1;
                work->timer3C = FX_DEATH0_SINK_FRAMES;
                ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)&work->pos24);
                work->pos24.vy = 0;
            }
            break;
        case FX_DEATH0_FADE_FRAME:
            work->timer40 = FX_DEATH0_FADE_FRAMES;
            break;
        }
        if (work->timer3C >= 0) {
            work->timer3C--;
            riseFrame = FX_DEATH0_SINK_FRAMES - work->timer3C;
            risePos.vx = work->pos24.vx;
            risePos.vy = work->pos24.vy + riseFrame * riseFrame;
            risePos.vz = work->pos24.vz;
            ex->btlfx_setUnitPos(BTLFX_UNIT_CASTER, (s16 *)&risePos);
        }
        if (work->timer3E >= 0) {
            work->timer3E--;
            hopFrame = FX_DEATH0_HOP_FRAMES - work->timer3E;
            hopPos.vx = work->pos2C.vx;
            hopPos.vy = work->pos2C.vy - (hopFrame * 0x50 - hopFrame * 5 * hopFrame);
            hopPos.vz = work->pos2C.vz;
            ex->btlfx_setUnitPos(BTLFX_UNIT_CASTER, (s16 *)&hopPos);
        }
        if (work->timer40 >= 0) {
            work->fade34 = fxm_lerpRatio(0, -0x80, FX_DEATH0_FADE_FRAMES - work->timer40, FX_DEATH0_FADE_FRAMES);
            alpha = work->fade34 + 0x80;
            if (alpha < FX_DEATH0_FADE_MIN) {
                ex->btlfx_fadeUnitById(BTLFX_UNIT_CASTER, FX_DEATH0_FADE_MIN);
            } else {
                ex->btlfx_fadeUnitById(BTLFX_UNIT_CASTER, alpha);
            }
            work->timer40--;
        }
        if (frame >= FX_DEATH0_BURST_FRAME) {
            if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&work->pos1C);
            if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
                work->emit8 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit8, (s32)&work->pos1C);
        }
        return frame >= FX_DEATH0_FRAMES;
    }
    return 0;
}

s32 fx_1a4_op81(s32 mode, Fx1a4Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx1a4Work81);
    } else if (mode == 1) {
        fx_1a4_env = env;
        if (env->targets->fxArgs[1] != 0) {
            return 1;
        }
        ex->btlfx_initEffModels(fx_1a4_effModels, FX_DEATH0_MODELS);
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos44);
        work->rot8.vx = 0x800;
        work->rot8.vy = 0;
        work->rot8.vz = 0;
        work->rot10.vx = 0x800;
        work->rot10.vy = 0;
        work->rot10.vz = 0;
        work->rot18.vx = 0x800;
        work->rot18.vy = 0;
        work->rot18.vz = 0;
        work->scale30.vx = 0;
        work->scale30.vy = 0;
        work->scale30.vz = 0;
        work->pos20.vx = work->pos44.vx;
        work->pos20.vy = work->pos44.vy;
        work->pos20.vz = work->pos44.vz;
        work->model54 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_DEATH0_FLAME_COLUMN), 1);
        work->model56 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_DEATH0_FLAME_COIL), 1);
        work->model58 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_DEATH0_FLAME_COIL_2), 1);
        work->timer52 = -1;
        work->speed4E = 8;
        work->frame4C = 0;
        work->speed50 = 0x10;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH0_DUST_SPIRAL), (s32)&work->pos44);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH0_DUST_SPIRAL_2), (s32)&work->pos44);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
            }
            break;
        case FX_DEATH0_FLAMES_FADE_FRAME:
            work->timer52 = FX_DEATH0_FLAMES_FADE_FRAMES;
            break;
        }
        if (frame >= 0 && frame < FX_DEATH0_FLAMES_FADE_FRAME + FX_DEATH0_FLAMES_FADE_FRAMES) {
            ex->Hi_DrawEffModel(&work->rot8, &work->pos20, (s32)&work->scale30, work->model54);
            ex->Hi_DrawEffModel(&work->rot10, &work->pos20, (s32)&work->scale30, work->model56);
            ex->Hi_DrawEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model58);
            if (work->timer52 >= 0) {
                work->color40 = fxm_lerpRatio(0, -0x80, FX_DEATH0_FLAMES_FADE_FRAMES - work->timer52, FX_DEATH0_FLAMES_FADE_FRAMES);
                ex->Hi_ModifyEffModelRGB(work->model54, work->color40, work->color40, work->color40);
                ex->Hi_ModifyEffModelRGB(work->model56, work->color40, work->color40, work->color40);
                ex->Hi_ModifyEffModelRGB(work->model58, work->color40, work->color40, work->color40);
                work->timer52--;
            }
            work->rot8.vy += 0x80;
            work->rot10.vy += 0x80;
            work->rot18.vy += 0x40;
            if (frame >= FX_DEATH0_GROW_FRAMES) {
                if (work->scale30.vx >= 0x40) {
                    work->scale30.vx -= 0x40;
                } else {
                    work->scale30.vx = 0;
                }
                if (work->scale30.vz >= 0x40) {
                    work->scale30.vz -= 0x40;
                } else {
                    work->scale30.vz = 0;
                }
            } else {
                work->scale30.vx += 0x800;
                work->scale30.vy += 0x400;
                work->scale30.vz += 0x800;
                work->scale30.vx = work->scale30.vx > 0xCCC ? 0xCCC : work->scale30.vx;
                work->scale30.vy = work->scale30.vy > 0x1000 ? 0x1000 : work->scale30.vy;
                work->scale30.vz = work->scale30.vz > 0xCCC ? 0xCCC : work->scale30.vz;
            }
            ex->btlfx_drawScrollMove(work->speed4E * 0x64 - work->frame4C * work->speed4E, FX_DEATH0_COLUMN_VRAM_X, FX_DEATH0_COLUMN_VRAM_Y, FX_DEATH0_FLAME_VRAM_W, FX_DEATH0_FLAME_VRAM_H, FX_DEATH0_COLUMN_SRC_X, FX_DEATH0_COLUMN_SRC_Y);
            ex->btlfx_drawScrollMove(work->speed50 * 0x64 - work->frame4C * work->speed50, FX_DEATH0_COIL_VRAM_X, FX_DEATH0_COIL_VRAM_Y, FX_DEATH0_FLAME_VRAM_W, FX_DEATH0_FLAME_VRAM_H, FX_DEATH0_COIL_SRC_X, FX_DEATH0_COIL_SRC_Y);
            work->frame4C++;
        }
        if (frame >= FX_DEATH0_FLAMES_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1a4_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_1a4/nonmatchings/fx_1a4", fx_1a4_headerPtr);
