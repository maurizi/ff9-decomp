#include "fxmod/fx_045.h"

s32 fx_045_op80(s32 mode, Fx045Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    union { /* one stack area: bone matrix in mode 1, draw arguments afterwards */
        MATRIX m;
        struct {
            VECTOR scale;
            SVECTOR rot;
            CVECTOR col;
        } d;
    } u;
    MATRIX m2;

    if (mode == 0) {
        *args = 0x54;
    } else if (mode == 1) {
        fx_045_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos18);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot20);
        work->pos38.vx = work->pos18.vx;
        work->pos38.vy = work->pos18.vy;
        work->pos38.vz = work->pos18.vz;
        ex->btlfx_pointAhead((s16 *)&work->pos18, (s16 *)&work->rot20, 0x3E8, (s16 *)&work->pos38);
        work->pos38.vy = -0xC8;
        ex->btlfx_getUnitBoneMatrix(0x10, 0x1C, (BtlBone *)&u.m);
        ex->btlfx_matrixPointAhead(&u.m, 0x320, (s32)&work->pos28);
        ex->btlfx_getUnitBoneMatrix(0x10, 0x36, (BtlBone *)&m2);
        ex->btlfx_matrixPointAhead(&m2, 0x320, (s32)&work->pos30);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos28);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos30);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
        }
        ex->btlfx_aimAngles((s16 *)&work->pos28, (s16 *)&work->pos38, (s16 *)&work->aim40);
        ex->btlfx_aimAngles((s16 *)&work->pos30, (s16 *)&work->pos38, (s16 *)&work->aim48);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 11:
            work->tint50 = 0x80;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->tint50, work->tint50, 0x80);
            break;
        case 10:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos38);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos38);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos38);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos38);
            break;
        }
        if (frame >= 0xD) {
            s32 s = (frame << 7) + 0x980;

            u.d.scale.vx = s;
            u.d.scale.vy = s;
            u.d.scale.vz = s;
            u.d.col.b = 0x28;
            u.d.col.g = 0x28;
            u.d.col.r = 0x28;
            u.d.rot.vx = 0;
            u.d.rot.vy = 0;
            u.d.rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &u.d.rot, &u.d.scale, &u.d.col, 1, NULL);
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aim40.vx;
                work->emit0->rot.vy = work->aim40.vy;
                work->emit0->rot.vz = work->aim40.vz;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim48.vx;
                work->emit4->rot.vy = work->aim48.vy;
                work->emit4->rot.vz = work->aim48.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_045_op81(s32 mode, Fx045Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        s32 n = *args;

        work->targetC = n;
        fx_045_env = env;
        if (n >= env->targets->targetCount) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            return 1;
        }
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_getUnitPos(work->targetC, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->targetC, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 1) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
        }
        return frame >= 0x28;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value) {
    env->state->signals[signal] = value;
}

INCLUDE_RODATA("asm/us/fx_045/nonmatchings/fx_045", fx_045_headerPtr);
