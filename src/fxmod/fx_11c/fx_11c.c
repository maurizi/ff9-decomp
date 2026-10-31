#include "fxmod/fx_11c.h"

s32 fx_11c_op80(s32 mode, Fx11cWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        fx_11c_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        work->pos10.vx = work->unk8.vx;
        work->pos10.vy = work->unk8.vy;
        work->pos10.vz = work->unk8.vz;
        work->pos10.vy += 0x12C;
        work->pos10.vz -= 0x12C;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E7FAC;
            work->emit0->trailSlots = 0xD;
            work->emit0->trailStride = 0x90;
        }
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)D_801E86FC;
            work->emit4->trailSlots = 0xD;
            work->emit4->trailStride = 0x90;
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 fx_11c_op81(s32 mode, Fx11cWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx11cBurst *o;
    Fx11cBurst *q;
    s32 frame;
    s32 i;
    s32 j;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->pos20.vx = env->targets->centerX;
        work->pos20.vy = env->targets->centerY;
        work->pos20.vz = env->targets->centerZ;
        fx_11c_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos20);
        work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos20);
        work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos20);
        o = D_801E7EDC;
        for (i = 0; i < 5; i++, o++) {
            o->unk10 = 0;
            o->unk12 = 0;
            o->unk14 = i * 2;
            o->pos0.vx = work->pos20.vx;
            o->pos0.vy = work->pos20.vy;
            o->pos0.vz = work->pos20.vz;
            o->unk8.vx = work->pos20.vx;
            o->unk8.vy = work->pos20.vy;
            o->unk8.vz = work->pos20.vz;
            if (ex->btlfx_rand() & 1) {
                o->unk8.vx += ex->btlfx_rand() % 800;
            } else {
                o->unk8.vx -= ex->btlfx_rand() % 800;
            }
            if (ex->btlfx_rand() & 1) {
                o->unk8.vz += ex->btlfx_rand() % 800;
            } else {
                o->unk8.vz -= ex->btlfx_rand() % 800;
            }
        }
    } else {
        frame = entry->frame;
        q = D_801E7EDC;
        for (j = 0; j < 5; j++, q++) {
            if (q->unk14 == frame) {
                work->emit4[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, q->unk16),
                                                  (s32)&q->pos0);
                q->unk10 = 1;
            } else if (q->unk10 != 0) {
                q->unk12++;
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

s32 fx_11c_op82(s32 mode, Fx11cWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        work->pos18.vx = env->targets->centerX;
        work->pos18.vy = env->targets->centerY;
        work->pos18.vz = env->targets->centerZ;
        work->pos20.vx = env->targets->centerX;
        work->pos20.vy = env->targets->centerY;
        work->pos20.vz = env->targets->centerZ;
        work->pos28.vx = env->targets->centerX;
        work->pos28.vy = env->targets->centerY;
        work->pos28.vz = env->targets->centerZ;
        work->pos20.vx -= 0x190;
        work->pos28.vx += 0x190;
        fx_11c_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos18);
        work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos18);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E8E4C;
            work->emit0->trailSlots = 8;
            work->emit0->trailStride = 0x90;
        }
    } else {
        frame = entry->frame;
        if (frame == 5) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos18);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos18);
        }
        if (frame == 0xD) {
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos20);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos28);
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_11c/nonmatchings/fx_11c", fx_11c_headerPtr);
