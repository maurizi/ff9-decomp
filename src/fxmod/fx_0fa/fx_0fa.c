#include "fxmod/fx_0fa.h"

static void fxm_easeBlend4(s32 mode, s32 len, s32 t, SVECTOR *ctrl, SVECTOR *out) {
    s32 w[4];
    s32 k;
    s32 s;
    s32 r;
    s32 i;
    s32 *wp;

    k = 0x2000000 / (len * len);
    s = 0;
    switch (mode) {
    case 0:
        s = ((0x1000000 / len) * t) >> 16;
        break;
    case 1:
        s = 0x100 - ((k * (len - t) * (len - t)) >> 17);
        break;
    case 2:
        s = (k * t * t) >> 17;
        break;
    }
    r = 0x100 - s;
    w[0] = (s * s * s) >> 12;
    w[1] = (r * s * s * 6) >> 12;
    w[2] = (r * r * s * 6) >> 12;
    w[3] = (r * r * r) >> 12;
    out->vx = 0;
    out->vy = 0;
    out->vz = 0;
    wp = w;
    for (i = 0; i < 4; i++) {
        out->vx += (ctrl[i].vx * *wp) >> 12;
        out->vy += (ctrl[i].vy * *wp) >> 12;
        out->vz += (ctrl[i].vz * *wp) >> 12;
        wp++;
    }
}

s32 fx_0fa_op80(s32 mode, Fx0faWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 *q;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x78;
    } else if (mode == 1) {
        work->emit74 = NULL;
        fx_0fa_env = env;
        ex->btlfx_initEffModels(fx_0fa_effModels, 2);
        for (i = 9, q = &work->timers0[9]; i >= 0; i--) {
            *q-- = 0;
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
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos30);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
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
