#include "fxmod/fx_1a4.h"

s32 fx_1a4_op80(s32 mode, Fx1a4Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR w;
    s32 frame;
    s32 d;
    s32 e;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x44;
    } else if (mode == 1) {
        fx_1a4_env = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos14);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos14);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        work->unk3C = -1;
        work->unk3E = -1;
        work->unk40 = -1;
        work->unk38 = env->targets->fxArgs[1] == 0;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos1C);
        switch (frame) {
        case 0x1E:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&fx_1a4_emitPos0);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&fx_1a4_emitPos1);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0x49:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos1C);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos1C);
            break;
        case 0x4C:
            if (work->unk38 != 0) {
                work->unk3E = -1;
                work->unk3C = 0x60;
                ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos24);
                work->pos24.vy = 0;
            }
            break;
        case 0x6A:
            work->unk40 = 0x20;
            break;
        }
        if (work->unk3C >= 0) {
            work->unk3C--;
            e = 0x60 - work->unk3C;
            v.vx = work->pos24.vx;
            v.vy = work->pos24.vy + e * e;
            v.vz = work->pos24.vz;
            ex->btlfx_setUnitPos(0x10, (s16 *)&v);
        }
        if (work->unk3E >= 0) {
            work->unk3E--;
            d = 0x10 - work->unk3E;
            w.vx = work->unk2C.vx;
            w.vy = work->unk2C.vy - (d * 0x50 - d * 5 * d);
            w.vz = work->unk2C.vz;
            ex->btlfx_setUnitPos(0x10, (s16 *)&w);
        }
        if (work->unk40 >= 0) {
            work->unk34 = fxm_lerpRatio(0, -0x80, 0x20 - work->unk40, 0x20);
            c = work->unk34 + 0x80;
            if (c < 0x46) {
                ex->btlfx_fadeUnitById(0x10, 0x46);
            } else {
                ex->btlfx_fadeUnitById(0x10, c);
            }
            work->unk40--;
        }
        if (frame >= 0x1E) {
            if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&work->pos1C);
            if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
                work->emit8 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit8, (s32)&work->pos1C);
        }
        return frame >= 0x96;
    }
    return 0;
}

s32 fx_1a4_op81(s32 mode, Fx1a4Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x5C;
    } else if (mode == 1) {
        fx_1a4_env = env;
        if (env->targets->fxArgs[1] != 0) {
            return 1;
        }
        ex->btlfx_initEffModels(fx_1a4_effModels, 3);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos44);
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
        work->model54 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 1);
        work->model56 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xE), 1);
        work->model58 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xF), 1);
        work->unk52 = -1;
        work->unk4E = 8;
        work->unk4C = 0;
        work->unk50 = 0x10;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos44);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos44);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0x2D:
            work->unk52 = 0x15;
            break;
        }
        if (frame >= 0 && frame < 0x42) {
            ex->Hi_DrawEffModel(&work->rot8, &work->pos20, (s32)&work->scale30, work->model54);
            ex->Hi_DrawEffModel(&work->rot10, &work->pos20, (s32)&work->scale30, work->model56);
            ex->Hi_DrawEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model58);
            if (work->unk52 >= 0) {
                work->unk40 = fxm_lerpRatio(0, -0x80, 0x15 - work->unk52, 0x15);
                ex->Hi_ModifyEffModelRGB(work->model54, work->unk40, work->unk40, work->unk40);
                ex->Hi_ModifyEffModelRGB(work->model56, work->unk40, work->unk40, work->unk40);
                ex->Hi_ModifyEffModelRGB(work->model58, work->unk40, work->unk40, work->unk40);
                work->unk52--;
            }
            work->rot8.vy += 0x80;
            work->rot10.vy += 0x80;
            work->rot18.vy += 0x40;
            if (frame >= 0xC) {
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
            ex->btlfx_drawScrollMove(work->unk4E * 0x64 - work->unk4C * work->unk4E, 0x1E0, 0x180, 0x20, 0x80, 0x1E0, 0x100);
            ex->btlfx_drawScrollMove(work->unk50 * 0x64 - work->unk4C * work->unk50, 0x200, 0x180, 0x20, 0x80, 0x200, 0x100);
            work->unk4C++;
        }
        if (frame >= 0x64) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1a4_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_1a4/nonmatchings/fx_1a4", fx_1a4_headerPtr);
