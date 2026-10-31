#include "fxmod/fx_18a.h"

s32 fx_18a_op80(s32 mode, Fx18aWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR sc;
    VECTOR pos;
    CVECTOR col;
    SVECTOR ang;
    VECTOR scale;
    SVECTOR rot2;
    s32 t;
    s32 c;
    s32 state;
    s32 n1;
    s32 n2;
    s32 frame;
    s32 a;
    s32 f;
    s32 g1;
    s32 g2;
    s32 g3;
    s32 g4;
    s32 g;
    s32 x1;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->state0 = 0;
        fx_18a_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->posC);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos14);
        work->unk20 = 0;
        work->frame4 = 0;
        fx_18a_op80Done = 0;
        ex->btlfx_initEffModels(fx_18a_effModels, 1);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 1), 0xB7, 0x3D80, 0, 0);
    } else {
        c = 0;
        state = work->state0;
        frame = entry->frame;
        switch (state) {
        case 0:
            t = frame << 8;
            n2 = t >> 2;
            n1 = (ex->math_sin(n2) * 0x1400) >> 12;
            env->otZ = 0x100;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 4), &work->pos14, NULL, n1, 0, 1, t);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->pos14, NULL, 0x1000, 0, 1, t);
            env->otZ = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 3), &work->posC, &rot, n1, 0, 1, t);
            sc.vx = sc.vz = 0x200;
            sc.vy = ex->math_sin(n2) / 5;
            c = t;
            if (frame >= 0x10) {
                work->state0 = 1;
                entry->frame = -1;
                work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos14);
                if (work->emit8 != NULL) {
                    work->emit8->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
                }
                work->frame1C = 0;
            }
            break;
        case 1:
            t = ((++work->frame1C % 10) << 12) / 10;
            a = (ex->math_sin(t) >> 4) + 0x1400;
            env->otZ = 0x100;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 4), &work->pos14, NULL, a, 0, 1, 0x1000);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->pos14, NULL, 0x1000, 0, 1, 0x1000);
            env->otZ = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 3), &work->posC, &rot, a, 0, 1, 0x1000);
            sc.vx = sc.vz = 0x200;
            sc.vy = (ex->math_sin(t) >> 7) + 0x333;
            c = 0x1000;
            if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
                work->emit8 = NULL;
            }
            if (frame >= 0xB && t == 0) {
                work->state0 = 2;
                entry->frame = -1;
                env->camera->paused = 0;
                ex->btlfx_fadeUnitById(0x10, -1);
                ex->btlfx_fadeUnitById(0, -2);
                work->posC.vz += 0x64;
                ex->btlfx_setUnitPos(0, (s16 *)&work->posC);
                ex->btlfx_stopEmitter(work->emit8);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->posC);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->posC);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->posC);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
                }
            }
            break;
        case 2:
            ang.vx = 0;
            ang.vy = 0;
            ang.vz = 0;
            t = (frame << 12) / 180;
            col.r = col.g = col.b = (((0x1000 - t) << 6) >> 12) - 0x74;
            if (frame < 0x8C) {
                scale.vx = scale.vy = scale.vz = (ex->math_cos(t >> 2) >> 3) + 0x1000;
            } else {
                scale.vx = scale.vy = scale.vz = (ex->math_cos(((frame - 0x8C) << 10) / 40) >> 2) + 0x1000;
            }
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &ang, &scale, &col, 0, 0);
            if (frame >= 0x8C) {
                rot2.vx = 0x400;
                rot2.pad = 1;
                rot2.vz = 0;
                rot2.vy = 0;
                env->otZ = 0x20;
                g1 = ((frame - 0x8C) << 12) / 40;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x12), &work->pos14, &rot2,
                                  ex->math_sin(g1 >> 2) * 5, 0, 1, ex->math_cos(g1 >> 2));
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x12), &work->pos14, &rot2,
                                  ex->math_sin(g1 >> 2) * 2, 0, 1, ex->math_cos(g1 >> 2));
                rot2.vx += 0x200;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x12), &work->pos14, &rot2,
                                  ex->math_sin(g1 >> 2) * 3, 0, 1, ex->math_cos(g1 >> 2));
                rot2.vx -= 0x400;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x12), &work->pos14, &rot2,
                                  ex->math_sin(g1 >> 2) * 3, 0, 1, ex->math_cos(g1 >> 2));
                env->otZ = 0;
            }
            f = 0x1000 - t;
            env->otZ = 0x100;
            x1 = t + 0x1400;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 4), &work->pos14, NULL, x1, 0, 1, f);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->pos14, NULL, 0x1000, 0, 1, f);
            if (frame < 6) {
                g2 = (frame << 10) / 6;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->pos14, NULL,
                                  ex->math_sin(g2) * 2, 0, 1, ex->math_cos(g2));
            }
            env->otZ = 0;
            if (frame == 0x1E) {
                ex->btlfx_signalScriptCue();
            }
            if (frame < 4) {
                g3 = ex->math_cos(frame << 8) * 0xFF;
                f = g3 >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, f, g3 >> 14, g3 >> 13);
            }
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 3), &work->posC, &rot, x1, 0, 1, f);
            if (frame < 0x16) {
                g4 = (frame << 12) / 22 >> 2;
                c = ex->math_cos(g4);
                sc.vx = sc.vz = (ex->math_sin(g4) >> 2) + 0x174;
                sc.vy = ex->math_cos(g4) / 4;
            }
            if (frame >= 0xB4) {
                fx_18a_op80Done = 1;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (c != 0) {
            pos.vx = work->posC.vx;
            pos.vy = work->posC.vy;
            pos.vz = work->posC.vz;
            rot.vx = 0;
            rot.vy = work->frame4 * 32;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model24);
            ex->Hi_ModifyEffModelRGB(work->model24, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
            sc.vy >>= 1;
            sc.vx = sc.vz = sc.vx * 3 / 2;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model24);
            ex->Hi_ModifyEffModelRGB(work->model24, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        }
        ex->btlfx_drawScrollMove(work->frame4 * 4, 0x1C0, 0x120, 0x20, 0x40, 0x1E0, 0x120);
        work->frame4++;
    }
    return 0;
}

static s32 fx_18a_op82PoolTask(s32 mode, Fx18aSpark *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 zero[2];
    SVECTOR *pt;
    s32 a;

    if (mode == 1) {
        e->drag12 = ex->btlfx_randCentered(0x200) + 0xE00;
    } else {
        a = 0x1000 - (frame << 12) / 12;
        pt = e->point18;
        zero[0] = 0;
        zero[1] = 0;
        pt->pad = 0x1A;
        ex->btlfx_stepModuleParticle((Pool6CEntry *)e, 0, (SVECTOR *)zero, pt);
        if (e->index16 == 0) {
            TRAIL(e)->lastSlot0 = TRAIL(e)->count2 = 0xE;
            TRAIL(e)->head1 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, 5, 0x1000, a, 1, (u8 *)e->point18 - 4, 0);
        }
        if (frame >= 0xC) {
            return 1;
        }
    }
    return 0;
}

void fx_18a_resetTrails(void) {
    fx_18a_nextTrail = 0;
}

void fx_18a_spawnTrail(Fx18aInst *inst, SVECTOR *origin, s32 h, SVECTOR *from, SVECTOR *to) {
    Fx18aTrail *tr = &fx_18a_trails[fx_18a_nextTrail];
    SVECTOR *pt = &tr->points4[14];
    BtlfxExports *ex = BTLFX_EXPORTS(fx_18a_env);
    SVECTOR v;
    Fx18aSpark *e;
    s32 i;
    s32 t;

    tr->lastSlot0 = tr->count2 = 0xE;
    tr->head1 = 0;
    for (i = 0; i < 15; i++, pt--) {
        t = 0x1000 - (i << 12) / 14;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        v.vx += ex->btlfx_randCentered(0x20);
        v.vy += ex->btlfx_randCentered(0x20);
        ex->btlfx_pointAhead((s16 *)origin, (s16 *)&v, h + ex->btlfx_randCentered(0x10) + (ex->math_sin(t >> 1) >> 3), (s16 *)pt);
        e = ex->btlfx_poolSpawn(inst->pool10);
        if (e != NULL) {
            e->slot14 = fx_18a_nextTrail;
            e->index16 = 0xE - i;
            e->point18 = pt;
            e->pos0 = pt->vx << 12;
            e->pos4 = pt->vy << 12;
            e->pos8 = pt->vz << 12;
            e->velC = 0;
            e->velE = 0;
            e->vel10 = 0;
            e->velC = ((pt->vx - origin->vx) >> 4) + ex->btlfx_randCentered(0x20);
            e->velE = ((pt->vy - origin->vy) >> 3) + ex->btlfx_randCentered(0x20);
            e->vel10 = ((pt->vz - origin->vz) >> 4) + ex->btlfx_randCentered(0x20);
        }
    }
    fx_18a_nextTrail = (fx_18a_nextTrail + 1) % 12;
}

s32 fx_18a_op82(s32 mode, Fx18aWork82 *work, Unk800F78A4 *env, Fx18aInst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR from;
    s32 frame;
    s32 c;

    if (mode == 0) {
        init->workSize = 0x10;
        init->elemSize = 0x1C;
        init->elemFn = (s32)fx_18a_op82PoolTask;
        init->poolCount = 0xB4;
    } else if (mode == 1) {
        fx_18a_env = env;
        fx_18a_resetTrails();
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos0);
        work->rot8.vx = 0;
        work->rot8.vy = 0;
        work->rot8.vz = 0;
    } else {
        frame = inst->frame0;
        c = frame < 0xE6;
        if (c) {
            BTLFX_COPY_SVEC(&from, &work->rot8);
            work->rot8.vx += 0x300 + (ex->btlfx_rand() & 0x7F);
            work->rot8.vy += 0x400 + (ex->btlfx_rand() & 0x7F);
            fx_18a_spawnTrail(inst, &work->pos0, frame + 0x40, &from, &work->rot8);
        }
        work->rot8.vx &= 0xFFF;
        work->rot8.vy &= 0xFFF;
        if (!c) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18a/nonmatchings/fx_18a", fx_18a_headerPtr);
