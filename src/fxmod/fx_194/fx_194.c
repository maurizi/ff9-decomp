#include "fxmod/fx_194.h"

static void fx_194_op80GridVertex(SVECTOR *v) {
    s32 y = v->vy;
    s32 x = v->vx;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_194_env);
    s32 nx = x + ((ex->math_cos(fx_194_wavePhase + y * 0x18) * fx_194_waveAmp) >> 13);
    s32 ny = y + ((ex->math_sin(fx_194_wavePhase + x * 0x18) * fx_194_waveAmp) >> 12);

    v->vx = nx;
    v->vy = ny;
}

void fx_194_shakeCamPoint(s32 frame, s32 amp, SVECTOR *v) {
    s32 d;

    if (frame == 2) {
        d = (amp * 16) >> 7;
        v->vx -= d;
        v->vz -= d;
        v->vy -= d;
    }
    if (frame >= 0x13 && frame < 0x19) {
        SVECTOR *t = &fx_194_shakeOffsets[frame - 0x13];
        v->vx += (t->vx * amp) >> 7;
        v->vy += (t->vy * amp) >> 7;
        v->vz += (t->vz * amp) >> 7;
    }
}

s32 fx_194_op80(s32 mode, Fx194Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR v;
    Pool6CEntry *p;
    s32 frame;
    s32 c;
    s32 t;
    s32 state;
    s32 i;

    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        work->state0 = 0;
        fx_194_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos4);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->posC);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos4);
        if (p != NULL) {
            p->otZ = 0x200;
        }
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->posC);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos4);
        if (p != NULL) {
            p->otZ = 0x200;
        }
        work->camPos14.vx = work->pos4.vx;
        work->camPos14.vy = work->pos4.vy;
        work->camPos14.vz = work->pos4.vz;
        env->camera->points[3].vx = work->camPos14.vx;
        env->camera->points[3].vy = work->camPos14.vy;
        env->camera->points[3].vz = work->camPos14.vz;
    } else {
        frame = entry->frame;
        state = work->state0;
        switch (state) {
        case 0:
            if (frame >= 0x13 && frame < 0x19) {
                s32 k = fx_194_flashLevels[frame - 0x13] * 0xFF / 100;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, k, k, k);
            }
            v.vx = work->camPos14.vx;
            v.vy = work->camPos14.vy;
            v.vz = work->camPos14.vz;
            fx_194_shakeCamPoint(frame, 0x300, &v);
            env->camera->points[3].vx = v.vx;
            env->camera->points[3].vy = v.vy;
            env->camera->points[3].vz = v.vz;
            if (frame >= 0x18) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = frame << 7;
            if (frame < 4) {
                s32 k = 0xFF - ((frame * 0xFF) >> 2);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, k, k, k);
            }
            t >>= 2;
            fx_194_waveAmp = (ex->math_cos(t) * 0xC) >> 12;
            fx_194_wavePhase = frame << 6;
            col.r = col.g = col.b = (ex->math_cos(t) * 0x64) >> 12;
            scale.vx = 0x1080;
            scale.vy = 0x1200;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x73, 0x20, 0x17, 0xA, 0xA, 5, 5, &rot, &scale, &col, state,
                              fx_194_op80GridVertex);
            if (frame >= 0x20) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_194_op81(s32 mode, Fx194Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        fx_194_env = env;
        work->state0 = 0;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos4);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos4);
        if (p != NULL) {
            p->otZ = 0x200;
        }
        env->camera->points[3].vx = work->pos4.vx;
        env->camera->points[3].vy = work->pos4.vy;
        env->camera->points[3].vz = work->pos4.vz;
    } else {
        s32 frame = entry->frame;
        if (work->state0 == 0 && frame >= 0x20) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_194/nonmatchings/fx_194", fx_194_headerPtr);
