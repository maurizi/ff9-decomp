#include "fxmod/fx_112.h"

static void fxm_captureScreen(s32 x, s32 y, s32 w, s32 h, s32 dx, s32 dy) {
    Unk800F78A4 *env = fx_112_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT rect;

    rect.x = env->dispEnv->disp.x + x - (w >> 1);
    rect.y = env->dispEnv->disp.y + y - (h >> 1);
    rect.w = w;
    rect.h = h;
    ex->MoveImage(&rect, dx, dy);
}

INCLUDE_RODATA("asm/us/fx_112/nonmatchings/fx_112", fx_112_headerPtr);

s32 fx_112_op80(s32 mode, Fx112Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 slot;
    s32 t;
    s32 arc;
    Pool6CEntry *emitter;
    Pool6CEntry *trailEmitter;
    Fx112Locals locals;

    if (mode == 0) {
        *args = 0x54;
    } else if (mode == 1) {
        fx_112_env = env;
        ex->btlfx_initEffModels(fx_112_effModels, 1);
        work->model20 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_EAT_SPHERE), 0x137, 0xFFFF, 0, 0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rotC);
        ex->btlfx_getUnitScale(0, (VECTOR *)&work->scale14);
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos38);
        work->pos38.vy += 0x80;
        env->camera->points[3].vx = work->pos38.vx;
        env->camera->points[3].vy = work->pos38.vy;
        env->camera->points[3].vz = work->pos38.vz;
        work->state0 = 0;
    } else {
    frame = entry->frame;
    switch (work->state0) {
    case 0:
        if (frame == 0) {
            locals.s0.scale0.vx = locals.s0.scale0.vz = work->scale14.vx;
            locals.s0.scale0.vy = work->scale14.vx + 0x1CC;
            ex->btlfx_setUnitScale(0, (s32 *)&locals.s0.scale0);
        }
        if (frame == 3) {
            fxm_captureScreen(0xA0, 0x8C, 0x80, 0xC0, VRAM_BTLFX_SCRATCH_X, VRAM_BTLFX_SCRATCH_Y);
        }
        if (frame == 4) {
            ex->btlfx_fadeUnitById(0, 0);
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos40);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EAT_SMOKE), (s32)&work->pos40);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        } else if (frame >= 5) {
            t = ((frame - 4) << 12) / 28;
            ex->btlfx_getUnitBonePos(0x10, 9, (u16 *)&locals.s0.a);
            ex->btlfx_getUnitBonePos(0x10, 0x1C, (u16 *)&locals.s0.b);
            ex->btlfx_aimAngles((s16 *)&locals.s0.a, (s16 *)&locals.s0.b, (s16 *)&locals.s0.rot);
            locals.s0.rot.vy += 0x800;
            locals.s0.pos.vx = (locals.s0.a.vx + locals.s0.b.vx) >> 1;
            locals.s0.pos.vy = (locals.s0.a.vy + locals.s0.b.vy) >> 1;
            locals.s0.pos.vz = (locals.s0.a.vz + locals.s0.b.vz) >> 1;
            locals.s0.scale.vx = ((ex->math_sin(t * 5 + 0x400) / 3 + 0x1400) * (0x1000 - (t >> 1))) >> 12;
            locals.s0.scale.vy = ((ex->math_sin(t * 5) / 3 + 0x1400) * (0x1000 - (t >> 1))) >> 12;
            locals.s0.scale.vz = ((ex->math_cos(t * 6) / 3 + 0x1400) * (0x1000 - (t >> 1))) >> 12;
            ex->Hi_DrawEffModel(&locals.s0.rot, &locals.s0.pos, (s32)&locals.s0.scale, work->model20);
        }
        if (frame >= 0x20) {
            work->state0 = 1;
            entry->frame = -1;
        }
        break;
    case 1:
        t = (frame << 12) / 9;
        ex->btlfx_getUnitPos(0x10, (s16 *)&locals.s1.pos);
        ex->btlfx_getUnitBonePos(0x10, 0x1C, (u16 *)&locals.s1.bone);
        ex->btlfx_lerpVec((s16 *)&locals.s1.bone, (s16 *)&locals.s1.pos, t, (s16 *)&locals.s1.v);
        arc = ex->math_sin((t * 0x992) >> 12);
        locals.s1.rot.vx = -frame << 8;
        locals.s1.rot.vy = locals.s1.rot.vz = frame * 6;
        locals.s1.scale.vx = locals.s1.scale.vy = locals.s1.scale.vz = ((0x1000 - t) * 0x1400) >> 13;
        locals.s1.v.vy -= arc >> 5;
        locals.s1.tr.vx = locals.s1.v.vx;
        locals.s1.tr.vy = locals.s1.v.vy;
        locals.s1.tr.vz = locals.s1.v.vz;
        ex->Hi_DrawEffModel(&locals.s1.rot, &locals.s1.tr, (s32)&locals.s1.scale, work->model20);
        if (frame >= 9) {
            work->state0 = 2;
            entry->frame = -1;
        }
        break;
    case 2:
        ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&work->pos48);
        slot = frame / 5;
        if (frame == slot * 5) {
            trailEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EAT_ORBIT_TRAIL), (s32)&work->pos48);
            if (trailEmitter != NULL) {
                trailEmitter->trails = (s32)fx_112_op80Trails[slot];
                trailEmitter->trailSlots = 0x14;
                trailEmitter->trailStride = 0x24;
                trailEmitter->otZ = 0x80;
            }
        }
        if (frame >= 0x13) {
            work->state0 = 3;
            entry->frame = -1;
        }
        break;
    case 3:
        if (frame >= 0x10) {
            work->state0 = 4;
            entry->frame = -1;
            work->emit50 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EAT_GOLD_CUTLERY), (s32)&work->pos48);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        }
        break;
    case 4:
        ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&work->pos48);
        ex->btlfx_getUnitBoneMatrix(0x10, 4, &locals.s4.m);
        ex->btlfx_matrixAngles((MATRIX *)&locals.s4.m, (s16 *)&locals.s4.rot);
        if (work->emit50 != NULL) {
            work->emit50->rot.vx = locals.s4.rot.vx;
            work->emit50->rot.vy = locals.s4.rot.vy;
            work->emit50->rot.vz = locals.s4.rot.vz;
        }
        if (frame < 0xD) {
            if (frame == 1) {
                ex->btlfx_fadeUnitById(0, 0x80);
            }
            ex->btlfx_lerpVec((s16 *)&work->pos48, (s16 *)&work->pos4, (frame << 12) / 12, (s16 *)&locals.s4.v);
            locals.s4.v.vy -= ex->math_sin((frame << 12) / 12 >> 1) >> 3;
            ex->btlfx_setUnitPos(0, (s16 *)&locals.s4.v);
            locals.s4.scale.vx = locals.s4.scale.vy = locals.s4.scale.vz =
                work->scale14.vx / 2 + ((work->scale14.vx / 2 * ((frame << 12) / 12)) >> 12);
            ex->btlfx_setUnitScale(0, (s32 *)&locals.s4.scale);
            locals.s4.r.vx = work->rotC.vx + ((0x1000 - (frame << 12) / 12) >> 1);
            locals.s4.r.vy = work->rotC.vy - (0x1000 - (frame << 12) / 12) / 3;
            locals.s4.r.vz = work->rotC.vz;
            ex->btlfx_setUnitRot(0, (s16 *)&locals.s4.r);
        }
        if (frame >= 0x10) {
            return 1;
        }
        break;
    }
    }
    return 0;
}
