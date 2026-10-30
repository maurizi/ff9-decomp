#include "fxmod/fx_040.h"

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *arg2, s32 arg3, s32 t, s32 n) {
    SVECTOR v;

    v.vx = D_801E7F08.vx;
    v.vy = D_801E7F08.vy;
    v.vz = D_801E7F08.vz;
    v.vx -= 0x190;
    return func_801E7864(out, arg2, &v, t, n);
}

s32 func_801E775C(VECTOR *out, s32 arg1, SVECTOR *arg2, s32 arg3, s32 t, s32 n) {
    SVECTOR v;

    v.vx = D_801E7F08.vx;
    v.vy = D_801E7F08.vy;
    v.vz = D_801E7F08.vz;
    v.vx -= 0xC8;
    return func_801E7864(out, arg2, &v, t, n);
}

s32 func_801E77B4(VECTOR *out, s32 arg1, SVECTOR *arg2, s32 arg3, s32 t, s32 n) {
    SVECTOR v;

    v.vx = D_801E7F08.vx;
    v.vy = D_801E7F08.vy;
    v.vz = D_801E7F08.vz;
    v.vx += 0xC8;
    return func_801E7864(out, arg2, &v, t, n);
}

s32 func_801E780C(VECTOR *out, s32 arg1, SVECTOR *arg2, s32 arg3, s32 t, s32 n) {
    SVECTOR v;

    v.vx = D_801E7F08.vx;
    v.vy = D_801E7F08.vy;
    v.vz = D_801E7F08.vz;
    v.vx += 0x190;
    return func_801E7864(out, arg2, &v, t, n);
}

s32 func_801E7864(VECTOR *out, SVECTOR *pos, SVECTOR *target, s32 t, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7EF8);
    s32 k;
    s32 r;

    if (ex->btlfx_rand() & 1) {
        target->vx += ex->btlfx_rand() % 300;
    } else {
        target->vx -= ex->btlfx_rand() % 300;
    }
    target->vy += ex->btlfx_rand() % 400;
    if (ex->btlfx_rand() & 1) {
        target->vz += ex->btlfx_rand() % 200;
    } else {
        target->vz -= ex->btlfx_rand() % 200;
    }
    k = ex->btlfx_rand() % 2 + 0x18;
    ex->btlfx_lerpVec((s16 *)&D_801E7F00, (s16 *)target, ((t * t / k) << 12) / k, (s16 *)pos);
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (t >= n) {
        return -1;
    }
    return 0;
}

s32 fx_040_op80(s32 mode, Fx040Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x50;
    } else if (mode == 1) {
        work->unk28.vx = env->targets->centerX;
        work->unk28.vy = env->targets->centerY;
        work->unk28.vz = env->targets->centerZ;
        D_801E7F08.vx = env->targets->centerX;
        D_801E7F08.vy = env->targets->centerY;
        D_801E7F08.vz = env->targets->centerZ;
        D_801E7EF8 = env;
        D_801E7F08.vy -= 0x190;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk40);
        D_801E7F00.vx = work->unk40.vx;
        D_801E7F00.vy = work->unk40.vy;
        D_801E7F00.vz = work->unk40.vz;
        env->camera->points[0].vx = work->unk40.vx;
        env->camera->points[0].vy = work->unk40.vy;
        env->camera->points[0].vz = work->unk40.vz;
        for (i = 0; i < 4; i++) {
            work->unk0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E7E98[i]), (s32)&D_801E7F18[i]);
            if (work->unk0[i] != NULL) {
                ((s32 *)work->unk0[i]->file)[2] = D_801E7EA0[i];
            }
        }
    } else {
        frame = entry->frame;
        if (frame == 0x12) {
            work->unk30.vx = env->targets->centerX;
            work->unk30.vy = env->targets->centerY;
            work->unk30.vz = env->targets->centerZ;
            work->unk38.vx = env->targets->centerX;
            work->unk38.vy = env->targets->centerY;
            work->unk38.vz = env->targets->centerZ;
            work->unk30.vx -= 0xC8;
            work->unk38.vx += 0xC8;
            work->unk18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk30);
            work->unk1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk38);
            work->unk20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->unk28);
            work->unk24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unk28);
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_040/nonmatchings/fx_040", fx_040_headerPtr);
