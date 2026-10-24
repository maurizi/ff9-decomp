#include "fxmod/fx_11c.h"

s32 fx_11c_op80(s32 mode, Fx11cWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E7F9C = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        work->unk10.vx = work->unk8.vx;
        work->unk10.vy = work->unk8.vy;
        work->unk10.vz = work->unk8.vz;
        work->unk10.vy += 0x12C;
        work->unk10.vz -= 0x12C;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk10);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk10);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7FAC;
            work->unk0->unk12 = 0xD;
            work->unk0->unkC = 0x90;
        }
        if (work->unk4 != NULL) {
            work->unk4->unk14 = (s32)D_801E86FC;
            work->unk4->unk12 = 0xD;
            work->unk4->unkC = 0x90;
        }
    } else {
        return entry->unk0 >= 0x28;
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
        work->unk20.vx = env->targets->centerX;
        work->unk20.vy = env->targets->centerY;
        work->unk20.vz = env->targets->centerZ;
        D_801E7F9C = env;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk20);
        work->unk18 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk20);
        work->unk1C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk20);
        o = D_801E7EDC;
        for (i = 0; i < 5; i++, o++) {
            o->unk10 = 0;
            o->unk12 = 0;
            o->unk14 = i * 2;
            o->unk0.vx = work->unk20.vx;
            o->unk0.vy = work->unk20.vy;
            o->unk0.vz = work->unk20.vz;
            o->unk8.vx = work->unk20.vx;
            o->unk8.vy = work->unk20.vy;
            o->unk8.vz = work->unk20.vz;
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
        frame = entry->unk0;
        q = D_801E7EDC;
        for (j = 0; j < 5; j++, q++) {
            if (q->unk14 == frame) {
                work->unk4[j] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, q->unk16),
                                                  (s32)&q->unk0);
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
        work->unk18.vx = env->targets->centerX;
        work->unk18.vy = env->targets->centerY;
        work->unk18.vz = env->targets->centerZ;
        work->unk20.vx = env->targets->centerX;
        work->unk20.vy = env->targets->centerY;
        work->unk20.vz = env->targets->centerZ;
        work->unk28.vx = env->targets->centerX;
        work->unk28.vy = env->targets->centerY;
        work->unk28.vz = env->targets->centerZ;
        work->unk20.vx -= 0x190;
        work->unk28.vx += 0x190;
        D_801E7F9C = env;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk18);
        work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk18);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E8E4C;
            work->unk0->unk12 = 8;
            work->unk0->unkC = 0x90;
        }
    } else {
        frame = entry->unk0;
        if (frame == 5) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk18);
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk18);
        }
        if (frame == 0xD) {
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk20);
            work->unk14 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk28);
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_11c/nonmatchings/fx_11c", fx_11c_headerPtr);
