#include "fxmod/fx_0fa.h"

static void fxm_easeBlend4(s32 mode, s32 len, s32 t, SVECTOR *ctrl, SVECTOR *out) {
    s32 weights[4];
    s32 invSq;
    s32 ratio;
    s32 inv;
    s32 i;
    s32 *weight;

    invSq = 0x2000000 / (len * len);
    ratio = 0;
    switch (mode) {
    case 0:
        ratio = ((0x1000000 / len) * t) >> 16;
        break;
    case 1:
        ratio = 0x100 - ((invSq * (len - t) * (len - t)) >> 17);
        break;
    case 2:
        ratio = (invSq * t * t) >> 17;
        break;
    }
    inv = 0x100 - ratio;
    weights[0] = (ratio * ratio * ratio) >> 12;
    weights[1] = (inv * ratio * ratio * 6) >> 12;
    weights[2] = (inv * inv * ratio * 6) >> 12;
    weights[3] = (inv * inv * inv) >> 12;
    out->vx = 0;
    out->vy = 0;
    out->vz = 0;
    weight = weights;
    for (i = 0; i < 4; i++) {
        out->vx += (ctrl[i].vx * *weight) >> 12;
        out->vy += (ctrl[i].vy * *weight) >> 12;
        out->vz += (ctrl[i].vz * *weight) >> 12;
        weight++;
    }
}

s32 fx_0fa_op80(s32 mode, Fx0faWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 *timer;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx0faWork);
    } else if (mode == 1) {
        work->emit74 = NULL;
        fx_0fa_env = env;
        ex->btlfx_initEffModels(fx_0fa_effModels, FX_DETECT_MODEL_SLOTS);
        for (i = FX_DETECT_TIMER_SLOTS - 1, timer = &work->timers0[FX_DETECT_TIMER_SLOTS - 1]; i >= 0; i--) {
            *timer-- = 0;
        }
        work->model38 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DETECT_METAL_SPHERE), FX_DETECT_METAL_SPHERE_TPAGE, FX_DETECT_METAL_SPHERE_CLUT, FX_DETECT_METAL_SPHERE_U, FX_DETECT_METAL_SPHERE_V);
        work->scale5C.vx = 0x333;
        work->scale5C.vy = 0x333;
        work->scale5C.vz = 0x333;
        work->rot3C.vx = 0;
        work->rot3C.vy = 0;
        work->rot3C.vz = 0;
        ex->btlfx_getUnitBodyPos(0x10, 0, &work->pos44.vx);
        ex->btlfx_getUnitBodyPos(0x10, 0, &work->homePos28.vx);
        work->pos4C.vx = work->pos44.vx;
        work->pos4C.vy = work->pos44.vy;
        work->pos4C.vz = work->pos44.vz;
        ex->btlfx_getUnitBodyPos(0, 0, &work->pos30.vx);
        return 0;
    } else {
        frame = entry->frame;
        if (work->emit74 != NULL && work->emit74->state == BTLFX_EMIT_DONE) {
            work->emit74 = NULL;
        }
        switch (frame) {
        case 0:
            work->timers0[FX_DETECT_T_RISE] = FX_DETECT_RISE_FRAMES;
            work->timers0[FX_DETECT_T_SPHERE] = FX_DETECT_FRAMES;
            break;
        case FX_DETECT_BLUE_SPIKES_FRAME:
            work->timers0[FX_DETECT_T_FLASH] = FX_DETECT_FLASH_FRAMES;
            work->timers0[FX_DETECT_T_AIM] = 1;
            work->emit74 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DETECT_BLUE_SPIKES), (s32)&work->pos44);
            break;
        case FX_DETECT_GLYPH_TEXT_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DETECT_GLYPH_TEXT), (s32)&work->pos30);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case FX_DETECT_SINK_FRAME:
            work->timers0[FX_DETECT_T_SINK] = FX_DETECT_SINK_FRAMES;
            break;
        case FX_DETECT_FRAMES:
            work->timers0[FX_DETECT_T_SPHERE] = 0;
            break;
        }
        if (work->timers0[FX_DETECT_T_RISE] != 0) {
            work->timers0[FX_DETECT_T_RISE]--;
            work->pos4C.vx = work->pos44.vx = work->homePos28.vx;
            work->pos4C.vy = work->pos44.vy = work->homePos28.vy - ((ex->math_sin(work->timers0[FX_DETECT_T_RISE] << 7) * (FX_DETECT_RISE_FRAMES - work->timers0[FX_DETECT_T_RISE])) >> 7);
            work->pos4C.vz = work->pos44.vz = work->homePos28.vz + ((ex->math_cos(work->timers0[FX_DETECT_T_RISE] << 7) * (FX_DETECT_RISE_FRAMES - work->timers0[FX_DETECT_T_RISE])) >> 7);
            ex->btlfx_aimAngles(&work->pos30.vx, &work->pos44.vx, &work->rot3C.vx);
            ex->btlfx_aimAngles(&work->pos44.vx, &work->pos30.vx, &work->aimRot6C.vx);
            work->scale5C.vx = work->scale5C.vy = work->scale5C.vz = ex->math_cos(work->timers0[FX_DETECT_T_RISE] << 6) / 5;
        }
        if (work->timers0[FX_DETECT_T_SINK] != 0) {
            work->timers0[FX_DETECT_T_SINK]--;
            work->pos4C.vx = work->pos44.vx = work->homePos28.vx;
            work->pos4C.vy = work->pos44.vy = work->homePos28.vy - ((ex->math_sin((FX_DETECT_SINK_FRAMES - work->timers0[FX_DETECT_T_SINK]) << 7) * work->timers0[FX_DETECT_T_SINK]) >> 7);
            work->pos4C.vz = work->pos44.vz = work->homePos28.vz + ((ex->math_cos((FX_DETECT_SINK_FRAMES - work->timers0[FX_DETECT_T_SINK]) << 7) * work->timers0[FX_DETECT_T_SINK]) >> 7);
            work->rot3C.vx -= work->timers0[FX_DETECT_T_SINK] * work->timers0[FX_DETECT_T_SINK] * 4;
            work->scale5C.vx = work->scale5C.vy = work->scale5C.vz = ex->math_cos((FX_DETECT_SINK_FRAMES - work->timers0[FX_DETECT_T_SINK]) << 6) / 5;
        }
        if (work->timers0[FX_DETECT_T_AIM] != 0 && work->emit74 != NULL) {
            work->emit74->rot.vx = work->aimRot6C.vx;
            work->emit74->rot.vy = work->aimRot6C.vy;
            work->emit74->rot.vz = work->aimRot6C.vz;
        }
        if (work->timers0[FX_DETECT_T_SPHERE] != 0) {
            work->timers0[FX_DETECT_T_SPHERE]--;
            work->pos4C.vy = work->pos44.vy + (ex->math_sin(frame << 7) >> 8);
            ex->Hi_DrawEffModel(&work->rot3C, &work->pos4C, (s32)&work->scale5C, work->model38);
        }
        if (work->timers0[FX_DETECT_T_FLASH] != 0) {
            work->timers0[FX_DETECT_T_FLASH]--;
            ex->btlfx_drawScrollMove(0, FX_DETECT_SPHERE_VRAM_X, FX_DETECT_SPHERE_VRAM_Y, FX_DETECT_SPHERE_VRAM_W, FX_DETECT_SPHERE_VRAM_H, fx_0fa_scrollSrc0[work->timers0[FX_DETECT_T_FLASH] >> 1].srcX0,
                              fx_0fa_scrollSrc0[work->timers0[FX_DETECT_T_FLASH] >> 1].srcY4);
        }
        if (work->timers0[FX_DETECT_T_FLICKER] != 0) {
            work->timers0[FX_DETECT_T_FLICKER]--;
            ex->btlfx_drawScrollMove(0, FX_DETECT_SPHERE_VRAM_X, FX_DETECT_SPHERE_VRAM_Y, FX_DETECT_SPHERE_VRAM_W, FX_DETECT_SPHERE_VRAM_H, fx_0fa_scrollSrc1[work->timers0[FX_DETECT_T_FLICKER]].srcX0, fx_0fa_scrollSrc1[work->timers0[FX_DETECT_T_FLICKER]].srcY4);
        } else if (frame > FX_DETECT_GLYPH_TEXT_FRAME) {
            if (!(ex->btlfx_rand() & 7)) {
                work->timers0[FX_DETECT_T_FLICKER] = FX_DETECT_FLICKER_FRAMES;
            }
        }
        return frame >= FX_DETECT_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0fa/nonmatchings/fx_0fa", fx_0fa_headerPtr);
