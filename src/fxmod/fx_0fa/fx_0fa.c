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
        *args = 0x78;
    } else if (mode == 1) {
        work->emit74 = NULL;
        fx_0fa_env = env;
        ex->btlfx_initEffModels(fx_0fa_effModels, 2);
        for (i = 9, timer = &work->timers0[9]; i >= 0; i--) {
            *timer-- = 0;
        }
        work->model38 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 1), 0xFA, 0x3DC0, 0x80, 0x80);
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
            work->timers0[0] = 0x10;
            work->timers0[2] = 0x50;
            break;
        case 0x14:
            work->timers0[4] = 8;
            work->timers0[6] = 1;
            work->emit74 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos44);
            break;
        case 0x20:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos30);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 0x40:
            work->timers0[1] = 0x10;
            break;
        case 0x50:
            work->timers0[2] = 0;
            break;
        }
        if (work->timers0[0] != 0) {
            work->timers0[0]--;
            work->pos4C.vx = work->pos44.vx = work->homePos28.vx;
            work->pos4C.vy = work->pos44.vy = work->homePos28.vy - ((ex->math_sin(work->timers0[0] << 7) * (0x10 - work->timers0[0])) >> 7);
            work->pos4C.vz = work->pos44.vz = work->homePos28.vz + ((ex->math_cos(work->timers0[0] << 7) * (0x10 - work->timers0[0])) >> 7);
            ex->btlfx_aimAngles(&work->pos30.vx, &work->pos44.vx, &work->rot3C.vx);
            ex->btlfx_aimAngles(&work->pos44.vx, &work->pos30.vx, &work->aimRot6C.vx);
            work->scale5C.vx = work->scale5C.vy = work->scale5C.vz = ex->math_cos(work->timers0[0] << 6) / 5;
        }
        if (work->timers0[1] != 0) {
            work->timers0[1]--;
            work->pos4C.vx = work->pos44.vx = work->homePos28.vx;
            work->pos4C.vy = work->pos44.vy = work->homePos28.vy - ((ex->math_sin((0x10 - work->timers0[1]) << 7) * work->timers0[1]) >> 7);
            work->pos4C.vz = work->pos44.vz = work->homePos28.vz + ((ex->math_cos((0x10 - work->timers0[1]) << 7) * work->timers0[1]) >> 7);
            work->rot3C.vx -= work->timers0[1] * work->timers0[1] * 4;
            work->scale5C.vx = work->scale5C.vy = work->scale5C.vz = ex->math_cos((0x10 - work->timers0[1]) << 6) / 5;
        }
        if (work->timers0[6] != 0 && work->emit74 != NULL) {
            work->emit74->rot.vx = work->aimRot6C.vx;
            work->emit74->rot.vy = work->aimRot6C.vy;
            work->emit74->rot.vz = work->aimRot6C.vz;
        }
        if (work->timers0[2] != 0) {
            work->timers0[2]--;
            work->pos4C.vy = work->pos44.vy + (ex->math_sin(frame << 7) >> 8);
            ex->Hi_DrawEffModel(&work->rot3C, &work->pos4C, (s32)&work->scale5C, work->model38);
        }
        if (work->timers0[4] != 0) {
            work->timers0[4]--;
            ex->btlfx_drawScrollMove(0, 0x2C0, 0x180, 0x40, 0x40, fx_0fa_scrollSrc0[work->timers0[4] >> 1].srcX0,
                              fx_0fa_scrollSrc0[work->timers0[4] >> 1].srcY4);
        }
        if (work->timers0[5] != 0) {
            work->timers0[5]--;
            ex->btlfx_drawScrollMove(0, 0x2C0, 0x180, 0x40, 0x40, fx_0fa_scrollSrc1[work->timers0[5]].srcX0, fx_0fa_scrollSrc1[work->timers0[5]].srcY4);
        } else if (frame > 0x20) {
            if (!(ex->btlfx_rand() & 7)) {
                work->timers0[5] = 4;
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0fa/nonmatchings/fx_0fa", fx_0fa_headerPtr);
