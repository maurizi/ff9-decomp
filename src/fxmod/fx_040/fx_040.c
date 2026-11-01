#include "fxmod/fx_040.h"

s32 fx_040_op80ParticleMotion0(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    SVECTOR dest;

    dest.vx = fx_040_aboveTarget.vx;
    dest.vy = fx_040_aboveTarget.vy;
    dest.vz = fx_040_aboveTarget.vz;
    dest.vx -= 0x190;
    return fx_040_flyToTarget(out, pos, &dest, frame, n);
}

s32 fx_040_op80ParticleMotion1(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    SVECTOR dest;

    dest.vx = fx_040_aboveTarget.vx;
    dest.vy = fx_040_aboveTarget.vy;
    dest.vz = fx_040_aboveTarget.vz;
    dest.vx -= 0xC8;
    return fx_040_flyToTarget(out, pos, &dest, frame, n);
}

s32 fx_040_op80ParticleMotion2(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    SVECTOR dest;

    dest.vx = fx_040_aboveTarget.vx;
    dest.vy = fx_040_aboveTarget.vy;
    dest.vz = fx_040_aboveTarget.vz;
    dest.vx += 0xC8;
    return fx_040_flyToTarget(out, pos, &dest, frame, n);
}

s32 fx_040_op80ParticleMotion3(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    SVECTOR dest;

    dest.vx = fx_040_aboveTarget.vx;
    dest.vy = fx_040_aboveTarget.vy;
    dest.vz = fx_040_aboveTarget.vz;
    dest.vx += 0x190;
    return fx_040_flyToTarget(out, pos, &dest, frame, n);
}

s32 fx_040_flyToTarget(VECTOR *out, SVECTOR *pos, SVECTOR *target, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_040_env);
    s32 span;
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
    span = ex->btlfx_rand() % 2 + 0x18;
    ex->btlfx_lerpVec((s16 *)&fx_040_origin, (s16 *)target, ((frame * frame / span) << 12) / span, (s16 *)pos);
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_040_op80(s32 mode, Fx040Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = 0x50;
    } else if (mode == 1) {
        work->pos28.vx = env->targets->centerX;
        work->pos28.vy = env->targets->centerY;
        work->pos28.vz = env->targets->centerZ;
        fx_040_aboveTarget.vx = env->targets->centerX;
        fx_040_aboveTarget.vy = env->targets->centerY;
        fx_040_aboveTarget.vz = env->targets->centerZ;
        fx_040_env = env;
        fx_040_aboveTarget.vy -= 0x190;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos40);
        fx_040_origin.vx = work->pos40.vx;
        fx_040_origin.vy = work->pos40.vy;
        fx_040_origin.vz = work->pos40.vz;
        env->camera->points[0].vx = work->pos40.vx;
        env->camera->points[0].vy = work->pos40.vy;
        env->camera->points[0].vz = work->pos40.vz;
        for (i = 0; i < 4; i++) {
            work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_040_emitBlocks[i]), (s32)&fx_040_emitPos[i]);
            if (work->emit0[i] != NULL) {
                ((s32 *)work->emit0[i]->file)[2] = fx_040_op80ParticleMotions[i];
            }
        }
    } else {
        frame = entry->frame;
        if (frame == 0x12) {
            work->pos30.vx = env->targets->centerX;
            work->pos30.vy = env->targets->centerY;
            work->pos30.vz = env->targets->centerZ;
            work->pos38.vx = env->targets->centerX;
            work->pos38.vy = env->targets->centerY;
            work->pos38.vz = env->targets->centerZ;
            work->pos30.vx -= 0xC8;
            work->pos38.vx += 0xC8;
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos30);
            work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos38);
            work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos28);
            work->emit24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos28);
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_040/nonmatchings/fx_040", fx_040_headerPtr);
