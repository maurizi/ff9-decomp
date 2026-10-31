#include "fxmod/fx_043.h"

static s32 fx_043_op80ParticleMotion(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 t, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_043_env);
    s32 r = t << 10;

    if (r > 0x1000) {
        r = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)&D_801E7BD0, (s16 *)&D_801E7BD8, r, (s16 *)pos);
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (t >= n) {
        return -1;
    }
    return 0;
}

s32 fx_043_op80(s32 mode, Fx043Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        fx_043_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        work->unk14.vy -= 0x64;
        D_801E7BD8.vx = work->unk14.vx;
        D_801E7BD8.vy = work->unk14.vy;
        D_801E7BD8.vz = work->unk14.vz;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unkC);
        D_801E7BD0.vx = work->unkC.vx;
        D_801E7BD0.vy = work->unkC.vy;
        D_801E7BD0.vz = work->unkC.vz;
        env->camera->points[0].vx = work->unkC.vx;
        env->camera->points[0].vy = work->unkC.vy;
        env->camera->points[0].vz = work->unkC.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&D_801E7BE0);
        if (work->emit0 != NULL) {
            work->emit0->unk14 = (s32)D_801E7BE8;
            work->emit0->unk12 = 0xB;
            work->emit0->unkC = 0x20;
        }
        if (work->emit0 != NULL) {
            ((s32 *)work->emit0->file)[2] = (s32)fx_043_op80ParticleMotion;
        }
    } else {
        return entry->frame >= 0x1E;
    }
    return 0;
}

s32 fx_043_op81(s32 mode, Fx043Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        fx_043_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk10);
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&D_801E7BD0, (s16 *)&work->unk20);
        ex->btlfx_pointAhead((s16 *)&work->unk8, (s16 *)&work->unk20, 0xC8, (s16 *)&work->unk18);
        work->unk18.vy -= 0x64;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk18);
        if (work->emit0 != NULL) {
            work->emit0->unk14 = (s32)D_801E7D48;
            work->emit0->unk12 = 0x34;
            work->emit0->unkC = 0x20;
        }
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk10);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->unk20.vx;
                work->emit0->rot.vy = work->unk20.vy;
                work->emit0->rot.vz = work->unk20.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_043/nonmatchings/fx_043", fx_043_headerPtr);
