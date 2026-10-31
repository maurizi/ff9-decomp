#include "fxmod/fx_1e9.h"

s32 fx_1e9_op80(s32 mode, Fx1e9Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    VECTOR pos;
    s32 b;
    s32 frame;
    s32 t0;
    s32 t2;
    s32 a0;
    s32 a1;
    s32 a2;
    s32 a;
    s32 c;
    s32 n;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->state0 = 0;
        fx_1e9_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->posC);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos14);
        work->unk20 = 0;
        work->frame4 = 0;
        fx_1e9_stopBolts = 0;
        ex->btlfx_initEffModels(fx_1e9_effModels, 1);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 3), 0xB7, 0x3D80, 0, 0);
    } else {
    b = 0;
    frame = entry->frame;
    switch (work->state0) {
    case 0:
        t0 = frame << 8;
        a0 = (ex->math_sin(t0 >> 2) * 0x10FF) >> 12;
        env->otZ = 0x7C;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &work->pos14, NULL, a0, 0, 1, t0);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->pos14, NULL, 0x1000, 0, 1, t0);
        env->otZ = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->posC, &rot, a0, 0, 1, t0);
        scale.vx = scale.vz = 0x174;
        scale.vy = ex->math_sin(t0 >> 2) / 6;
        b = t0;
        if (frame >= 0x10) {
            work->state0 = 1;
            entry->frame = -1;
            env->state->signals[0] = 0;
            ex->btlfx_fadeUnitById(0x10, 0);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos14);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            work->frame1C = 0;
        }
        break;
    case 1:
        n = ++work->frame1C;
        t0 = ((n % 10) << 12) / 10;
        a1 = (ex->math_sin(t0) >> 4) + 0x10FF;
        env->otZ = 0x7C;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &work->pos14, NULL, a1, 0, 1, 0x1000);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->pos14, NULL, 0x1000, 0, 1, 0x1000);
        env->otZ = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->posC, &rot, a1, 0, 1, 0x1000);
        scale.vx = scale.vz = 0x174;
        scale.vy = (ex->math_sin(t0) >> 7) + 0x2AA;
        b = 0x1000;
        if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
            work->emit8 = NULL;
        }
        if (frame != 0 && t0 == 0 && env->state->signals[0] != 0) {
            work->state0 = 2;
            entry->frame = -1;
            env->camera->paused = 0;
            ex->btlfx_fadeUnitById(0x10, 0x80);
            ex->btlfx_stopEmitter(work->emit8);
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->posC);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->posC);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
        }
        break;
    case 2:
        t0 = (frame << 12) / 47;
        c = 0x1000 - t0;
        env->otZ = 0x7C;
        a2 = t0 + 0x10FF;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &work->pos14, NULL, a2, 0, 1, c);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->pos14, NULL, 0x1000, 0, 1, c);
        env->otZ = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->posC, &rot, a2, 0, 1, c);
        if (frame < 0x10) {
            t2 = (frame << 8) >> 2;
            b = ex->math_cos(t2);
            scale.vx = scale.vz = (ex->math_sin(t2) >> 3) + 0x174;
            scale.vy = ex->math_cos(t2) / 6;
        }
        if (frame == 3) {
            ex->btlfx_resetIdleMotion(0, 0x10);
            work->unk20 = 0;
        }
        if (frame >= 0x2F) {
            fx_1e9_stopBolts = 1;
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    if (b != 0) {
        pos.vx = work->posC.vx;
        pos.vy = work->posC.vy;
        pos.vz = work->posC.vz;
        rot.vx = 0;
        rot.vy = work->frame4 * 32;
        rot.vz = 0;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
        a = (b >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->model24, a, a, a);
        scale.vy >>= 1;
        scale.vx = scale.vz = scale.vx * 3 / 2;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
        ex->Hi_ModifyEffModelRGB(work->model24, a, a, a);
    }
    ex->btlfx_drawScrollMove(work->frame4 * 4, 0x1C0, 0x120, 0x20, 0x40, 0x1E0, 0x120);
    work->frame4++;
    }
    return 0;
}

static s32 fx_1e9_op82PoolTask(s32 mode, Fx1e9Spark *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 zero[2];
    SVECTOR *pt;
    s32 a;

    if (mode == 1) {
        e->unk12 = ex->btlfx_randCentered(0x200) + 0xE00;
    } else {
        a = 0x1000 - (frame << 9);
        pt = e->pos18;
        zero[0] = 0;
        zero[1] = 0;
        pt->pad = 0x14;
        ex->btlfx_stepModuleParticle((Pool6CEntry *)e, 0, (SVECTOR *)zero, pt);
        if (e->index16 == 0) {
            TRAIL(e)->unk0 = TRAIL(e)->unk2 = 8;
            TRAIL(e)->unk1 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 8), 0, NULL, 5, 0x1000, a, 1, (u8 *)e->pos18 - 4, 0);
        }
        if (frame >= 8) {
            return 1;
        }
    }
    return 0;
}

void fx_1e9_resetBolts(void) {
    fx_1e9_nextBolt = 0;
}

void fx_1e9_spawnBolt(Fx1e9Inst *inst, SVECTOR *origin, s32 h, SVECTOR *from, SVECTOR *to) {
    Fx1e9Trail *tr = &fx_1e9_bolts[fx_1e9_nextBolt];
    SVECTOR *pt = &tr->pos4[8];
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1e9_env);
    SVECTOR v;
    Fx1e9Spark *e;
    s32 i;
    s32 t;

    tr->unk0 = tr->unk2 = 8;
    tr->unk1 = 0;
    for (i = 0; i < 9; i++, pt--) {
        t = 0x1000 - (i << 9);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        v.vx += ex->btlfx_randCentered(0x20);
        v.vy += ex->btlfx_randCentered(0x20);
        ex->btlfx_pointAhead((s16 *)origin, (s16 *)&v, h + ex->btlfx_randCentered(0x10) + (ex->math_sin(t >> 1) >> 5), (s16 *)pt);
        e = ex->btlfx_poolSpawn(inst->pool10);
        if (e != NULL) {
            e->index14 = fx_1e9_nextBolt;
            e->index16 = 8 - i;
            e->pos18 = pt;
            e->posX0 = pt->vx << 12;
            e->posY4 = pt->vy << 12;
            e->posZ8 = pt->vz << 12;
            e->velXC = 0;
            e->velYE = 0;
            e->velZ10 = 0;
            e->velXC = ((pt->vx - origin->vx) >> 4) + ex->btlfx_randCentered(0x20);
            e->velYE = ((pt->vy - origin->vy) >> 3) + ex->btlfx_randCentered(0x20);
            e->velZ10 = ((pt->vz - origin->vz) >> 4) + ex->btlfx_randCentered(0x20);
        }
    }
    fx_1e9_nextBolt++;
    fx_1e9_nextBolt %= 8;
}

s32 fx_1e9_op82(s32 mode, Fx1e9Work82 *work, Unk800F78A4 *env, Fx1e9Inst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR from;

    if (mode == 0) {
        init->workSize = 0x10;
        init->elemSize = 0x1C;
        init->elemFn = (s32)fx_1e9_op82PoolTask;
        init->poolCount = 0x48;
    } else if (mode == 1) {
        fx_1e9_env = env;
        fx_1e9_resetBolts();
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos0);
        work->angle8.vx = 0;
        work->angle8.vy = 0;
        work->angle8.vz = 0;
    } else {
        if (fx_1e9_stopBolts == 0) {
            BTLFX_COPY_SVEC(&from, &work->angle8);
            work->angle8.vx += 0x300 + (ex->btlfx_rand() & 0x7F);
            work->angle8.vy += 0x400 + (ex->btlfx_rand() & 0x7F);
            fx_1e9_spawnBolt(inst, &work->pos0, 0x80, &from, &work->angle8);
        }
        work->angle8.vx &= 0xFFF;
        work->angle8.vy &= 0xFFF;
        if (fx_1e9_stopBolts != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1e9/nonmatchings/fx_1e9", fx_1e9_headerPtr);
