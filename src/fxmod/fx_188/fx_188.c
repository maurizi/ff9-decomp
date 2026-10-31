#include "fxmod/fx_188.h"

static s32 fx_188_op80PoolTask(s32 mode, Fx188Elem *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    s32 t;

    if (mode == 1) {
        e->unk0 = 0;
        e->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&e->posC);
        if (e->emit1C != NULL) {
            e->emit1C->otZ = 0;
        }
        e->unk4 = (ex->btlfx_rand() & 0x1F) + 0x27;
        e->unk8 = (ex->btlfx_rand() & 0x1F) + 0x20;
    } else if (e->unk0 == 0) {
        e->unk4 += 0xC;
        if (e->unk4 > 0x140) {
            e->unk4 = 0x140;
        }
        e->unk8 += 0x1D;
        if (e->unk8 > 0x22C) {
            e->unk8 = 0x22D;
        }
        t = 0x1000;
        if (frame < 5) {
            t = frame << 10;
        }
        t = (t * D_801E86CC) >> 12;
        ex->btlfx_aimAngles((s16 *)&e->posC, (s16 *)&e->unk20, (s16 *)&ang);
        ex->btlfx_turnToward((s16 *)&ang, (s16 *)&e->unk14, e->unk8);
        ex->btlfx_pointAhead((s16 *)&e->posC, (s16 *)&e->unk14, e->unk4, (s16 *)&e->posC);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0xE), &e->posC, 0, t + 0x800, frame, 1, -1, 0, 0);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 3), &e->posC, 0, t + 0x800, frame, 2, -1, 0, 0);
        if (e->emit1C != NULL && e->emit1C->state == BTLFX_EMIT_DONE) {
            e->emit1C = NULL;
        }
        if (D_801E86CC == 0) {
            if (e->emit1C != NULL) {
                e->emit1C->frame = e->emit1C->tail->anim.duration;
            }
            e->unk0 = 1;
            return 1;
        }
    }
    return 0;
}

s32 fx_188_op80(s32 mode, Fx188Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx188Elem *e;
    SVECTOR v;
    Pool6CEntry *p;
    s32 frame;
    s32 s;
    s32 t;
    s32 n;

    if (mode == 0) {
        arg4->workSize = 0x20;
        arg4->elemSize = 0x28;
        arg4->elemFn = (s32)fx_188_op80PoolTask;
        arg4->poolCount = 0xC;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk1C = 0;
        fx_188_env = env;
        BTLFX_COPY_SVEC(&work->pos4, &env->targets->centerX);
        work->pos4.vy -= 0x200;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot14);
        D_801E86CC = 0x1000;
        work->rot14.vy += 0x800;
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            if (frame & 1) {
                if (work->unk1C < 0xC) {
                    work->unk1C++;
                    e = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (e != NULL) {
                        BTLFX_COPY_SVEC(&e->posC, &work->unkC);
                        e->unk20.vx = work->pos4.vx;
                        e->unk20.vz = work->pos4.vz;
                        e->unk20.vy = work->pos4.vy;
                        n = work->unk1C & 3;
                        e->unk14.vy = (s16)(work->rot14.vy - 0x300) + (n << 9);
                        n = work->unk1C >> 2;
                        e->unk14.vx = work->rot14.vx + n * 0x180;
                        e->unk14.vz = 0;
                    }
                }
            }
            if (frame >= 0x3E) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = frame << 9;
            if (frame < 8) {
                s = 0x1000;
                D_801E86CC = ex->math_cos(frame << 7);
            } else {
                s = ex->math_cos((frame - 8) << 7);
                t = 0x1000;
                D_801E86CC = 0;
            }
            s = s * 3 + 0x400;
            v.vx = 0x400;
            v.vz = 0;
            v.vy = 0;
            v.pad = 1;
            env->otZ = 0x20;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x14), &work->pos4, &v, s, 0, 1, t);
            env->otZ = 0;
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xF), frame);
            if (frame >= 0x10) {
                work->unk0 = 2;
                entry->frame = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos4);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos4);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos4);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->pos4);
                if (p != NULL) {
                    p->trails = (s32)D_801E7E98;
                    p->trailSlots = 0x19;
                    p->otZ = 0x20;
                    p->trailStride = 0x54;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->pos4);
                if (p != NULL) {
                    p->otZ = 0x20;
                }
            }
            break;
        case 2:
            if (frame >= 0x20) {
                work->unk0 = 2;
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_188/nonmatchings/fx_188", fx_188_headerPtr);
