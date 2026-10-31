#include "fxmod/fx_03d.h"

s32 fx_03d_op80(s32 mode, Fx03dWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        fx_03d_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->unk10.vx;
                work->emit0->rot.vy = work->unk10.vy;
                work->emit0->rot.vz = work->unk10.vz;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->unk10.vx;
                work->emit4->rot.vy = work->unk10.vy;
                work->emit4->rot.vz = work->unk10.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_03d_op81(s32 mode, Fx03dWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->pos18.vx = 0;
        work->pos18.vy = 0;
        work->pos18.vz = 0;
        fx_03d_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos18);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E8000;
            work->emit0->trailSlots = 9;
            work->emit0->trailStride = 0x88;
        }
    } else {
        frame = entry->frame;
        if (frame == 0x10) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos18);
        }
        if (frame == 0x28) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos18);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos18);
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_03d_op82(s32 mode, Fx03dWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 s;

    if (mode == 0) {
        *arg4 = 0x44;
    } else if (mode == 1) {
        fx_03d_env = env;
        ex->btlfx_initEffModels(fx_03d_effModels, 2);
        work->pos8.vx = 0;
        work->pos8.vy = 0;
        work->pos8.vz = 0;
        work->rot0.vx = 0;
        work->rot0.vy = 0;
        work->rot0.vz = 0;
        work->scale18.vx = 0x1000;
        work->scale18.vy = 0x1000;
        work->scale18.vz = 0x1000;
        work->scale28.vx = 0x1000;
        work->scale28.vy = 0x1000;
        work->scale28.vz = 0x1000;
        work->model40 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x17), 1);
        work->model42 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x16), 1);
        work->unk3C = -1;
        work->unk3E = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->unk3C = 0xF;
            break;
        case 0x19:
            work->unk3E = 0xF;
            break;
        }
        if (frame >= 0 && frame < 0x28) {
            if (frame >= 0 && frame < 0xF) {
                s = fxm_lerpRatio(0, 0x1800, frame, 0xF);
                work->scale18.vx = s;
                work->scale18.vy = s;
                work->scale18.vz = s;
                work->scale28.vx = s;
                work->scale28.vy = s;
                work->scale28.vz = s;
            }
            if (frame >= 0xF && frame < 0x28) {
                s = fxm_lerpRatio(0x1800, 0x2000, frame - 0xF, 0x19);
                work->scale18.vx = s;
                work->scale18.vz = s;
                s = fxm_lerpRatio(0x1800, 0x2800, frame - 0xF, 0x19);
                work->scale28.vx = s;
                work->scale28.vz = s;
            }
            ex->Hi_DrawEffModel(&work->rot0, &work->pos8, (s32)&work->scale18, work->model40);
            ex->Hi_DrawEffModel(&work->rot0, &work->pos8, (s32)&work->scale28, work->model42);
            if (work->unk3C >= 0 || work->unk3E >= 0) {
                if (work->unk3C >= 0) {
                    work->unk38 = fxm_lerpRatio(-0x80, 0, 0xF - work->unk3C, 0xF);
                    work->unk3C--;
                }
                if (work->unk3E >= 0) {
                    work->unk38 = fxm_lerpRatio(0, -0x80, 0xF - work->unk3E, 0xF);
                    work->unk3E--;
                }
                ex->Hi_ModifyEffModelRGB(work->model40, work->unk38, work->unk38, work->unk38);
                ex->Hi_ModifyEffModelRGB(work->model42, work->unk38, work->unk38, work->unk38);
            }
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_03d_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_03d/nonmatchings/fx_03d", fx_03d_headerPtr);
