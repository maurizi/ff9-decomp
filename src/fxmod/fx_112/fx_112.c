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
        *args = sizeof(Fx112Work);
    } else if (mode == 1) {
        fx_112_env = env;
        ex->btlfx_initEffModels(fx_112_effModels, 1);
        work->model20 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_EAT_SPHERE), FX_EAT_SPHERE_TPAGE, FX_EAT_SPHERE_CLUT, FX_EAT_SPHERE_U, FX_EAT_SPHERE_V);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rotC);
        ex->btlfx_getUnitScale(0, (VECTOR *)&work->scale14);
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos38);
        work->pos38.vy += 0x80;
        env->camera->points[3].vx = work->pos38.vx;
        env->camera->points[3].vy = work->pos38.vy;
        env->camera->points[3].vz = work->pos38.vz;
        work->state0 = FX_EAT_STATE_GRAB;
    } else {
    frame = entry->frame;
    switch (work->state0) {
    case FX_EAT_STATE_GRAB:
        if (frame == 0) {
            locals.s0.scale0.vx = locals.s0.scale0.vz = work->scale14.vx;
            locals.s0.scale0.vy = work->scale14.vx + 0x1CC;
            ex->btlfx_setUnitScale(0, (s32 *)&locals.s0.scale0);
        }
        if (frame == FX_EAT_CAPTURE_FRAME) {
            fxm_captureScreen(FX_EAT_CAPTURE_X, FX_EAT_CAPTURE_Y, FX_EAT_CAPTURE_W, FX_EAT_CAPTURE_H, VRAM_BTLFX_SCRATCH_X, VRAM_BTLFX_SCRATCH_Y);
        }
        if (frame == FX_EAT_VANISH_FRAME) {
            ex->btlfx_fadeUnitById(0, 0);
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos40);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EAT_SMOKE), (s32)&work->pos40);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        } else if (frame >= FX_EAT_VANISH_FRAME + 1) {
            t = ((frame - FX_EAT_VANISH_FRAME) << 12) / (FX_EAT_GRAB_FRAMES - FX_EAT_VANISH_FRAME);
            ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, 9, (u16 *)&locals.s0.a);
            ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, 0x1C, (u16 *)&locals.s0.b);
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
        if (frame >= FX_EAT_GRAB_FRAMES) {
            work->state0 = FX_EAT_STATE_SWALLOW;
            entry->frame = -1;
        }
        break;
    case FX_EAT_STATE_SWALLOW:
        t = (frame << 12) / FX_EAT_SWALLOW_FRAMES;
        ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)&locals.s1.pos);
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, 0x1C, (u16 *)&locals.s1.bone);
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
        if (frame >= FX_EAT_SWALLOW_FRAMES) {
            work->state0 = FX_EAT_STATE_CHEW;
            entry->frame = -1;
        }
        break;
    case FX_EAT_STATE_CHEW:
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, 4, (u16 *)&work->pos48);
        slot = frame / FX_EAT_TRAIL_INTERVAL;
        if (frame == slot * FX_EAT_TRAIL_INTERVAL) {
            trailEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EAT_ORBIT_TRAIL), (s32)&work->pos48);
            if (trailEmitter != NULL) {
                trailEmitter->trails = (s32)fx_112_op80Trails[slot];
                trailEmitter->trailSlots = FX_EAT_ORBIT_TRAIL_SLOTS;
                trailEmitter->trailStride = FX_EAT_ORBIT_TRAIL_STRIDE;
                trailEmitter->otZ = 0x80;
            }
        }
        if (frame >= FX_EAT_CHEW_FRAMES) {
            work->state0 = FX_EAT_STATE_PAUSE;
            entry->frame = -1;
        }
        break;
    case FX_EAT_STATE_PAUSE:
        if (frame >= FX_EAT_PAUSE_FRAMES) {
            work->state0 = FX_EAT_STATE_SPIT;
            entry->frame = -1;
            work->emit50 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EAT_GOLD_CUTLERY), (s32)&work->pos48);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        }
        break;
    case FX_EAT_STATE_SPIT:
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, 4, (u16 *)&work->pos48);
        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, 4, &locals.s4.m);
        ex->btlfx_matrixAngles((MATRIX *)&locals.s4.m, (s16 *)&locals.s4.rot);
        if (work->emit50 != NULL) {
            work->emit50->rot.vx = locals.s4.rot.vx;
            work->emit50->rot.vy = locals.s4.rot.vy;
            work->emit50->rot.vz = locals.s4.rot.vz;
        }
        if (frame <= FX_EAT_SPIT_FRAMES) {
            if (frame == 1) {
                ex->btlfx_fadeUnitById(0, 0x80);
            }
            ex->btlfx_lerpVec((s16 *)&work->pos48, (s16 *)&work->pos4, (frame << 12) / FX_EAT_SPIT_FRAMES, (s16 *)&locals.s4.v);
            locals.s4.v.vy -= ex->math_sin((frame << 12) / FX_EAT_SPIT_FRAMES >> 1) >> 3;
            ex->btlfx_setUnitPos(0, (s16 *)&locals.s4.v);
            locals.s4.scale.vx = locals.s4.scale.vy = locals.s4.scale.vz =
                work->scale14.vx / 2 + ((work->scale14.vx / 2 * ((frame << 12) / FX_EAT_SPIT_FRAMES)) >> 12);
            ex->btlfx_setUnitScale(0, (s32 *)&locals.s4.scale);
            locals.s4.r.vx = work->rotC.vx + ((0x1000 - (frame << 12) / FX_EAT_SPIT_FRAMES) >> 1);
            locals.s4.r.vy = work->rotC.vy - (0x1000 - (frame << 12) / FX_EAT_SPIT_FRAMES) / 3;
            locals.s4.r.vz = work->rotC.vz;
            ex->btlfx_setUnitRot(0, (s16 *)&locals.s4.r);
        }
        if (frame >= FX_EAT_SPIT_END_FRAME) {
            return 1;
        }
        break;
    }
    }
    return 0;
}
