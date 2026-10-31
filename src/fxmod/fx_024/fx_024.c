#include "fxmod/fx_024.h"

s32 fx_024_op80(s32 mode, Fx024Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
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
        *arg4 = 0x54;
    } else if (mode == 1) {
        fx_024_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos18);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot20);
        work->pos38.vx = work->pos18.vx;
        work->pos38.vy = work->pos18.vy;
        work->pos38.vz = work->pos18.vz;
        ex->btlfx_pointAhead((s16 *)&work->pos18, (s16 *)&work->rot20, 0x3E8, (s16 *)&work->pos38);
        work->pos38.vy = -0xC8;
        ex->btlfx_getUnitBoneMatrix(0x10, 0xD, (BtlBone *)&u.m);
        ex->btlfx_matrixPointAhead(&u.m, 0x1F4, (s32)&work->pos28);
        ex->btlfx_getUnitBoneMatrix(0x10, 0x1D, (BtlBone *)&m2);
        ex->btlfx_matrixPointAhead(&m2, 0x1F4, (s32)&work->pos30);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos28);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos30);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
        }
        ex->btlfx_aimAngles((s16 *)&work->pos28, (s16 *)&work->pos38, (s16 *)&work->unk40);
        ex->btlfx_aimAngles((s16 *)&work->pos30, (s16 *)&work->pos38, (s16 *)&work->unk48);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 11:
            work->unk50 = 0x80;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->unk50, work->unk50, 0x80);
            break;
        case 10:
            work->unk50 = 0x80;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->unk50, work->unk50, 0x80);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos38);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos38);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos38);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos38);
            break;
        }
        if (frame >= 0xD) {
            s32 s = (frame << 7) + 0x980;

            u.d.scale.vx = s;
            u.d.scale.vy = s;
            u.d.scale.vz = s;
            u.d.col.b = 0x8;
            u.d.col.g = 0x8;
            u.d.col.r = 0x8;
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
                work->emit0->rot.vx = work->unk40.vx;
                work->emit0->rot.vy = work->unk40.vy;
                work->emit0->rot.vz = work->unk40.vz;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->unk48.vx;
                work->emit4->rot.vy = work->unk48.vy;
                work->emit4->rot.vz = work->unk48.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_024_op81(s32 mode, Fx024Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        s32 n = *arg4;

        work->unkC = n;
        fx_024_env = env;
        if (n >= env->targets->targetCount) {
            fxm_setSignal(env, 7, 1);
            return 1;
        }
        fxm_setSignal(env, 7, 0);
        ex->btlfx_getUnitPos(work->unkC, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->unkC, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 1) {
            fxm_setSignal(env, 7, 1);
        }
        return frame >= 0x28;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->state + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_024/nonmatchings/fx_024", fx_024_headerPtr);
