#include "fxmod/fx_0f8.h"

s32 fx_0f8_op80(s32 mode, Fx0f8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        D_801E7BD0 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk4);
        work->unkC.vx = env->targets->centerX;
        work->unkC.vy = env->targets->centerY;
        work->unkC.vz = env->targets->centerZ;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk4);
        ex->btlfx_aimAngles((s16 *)&work->unk4, (s16 *)&work->unkC, (s16 *)&work->unk14);
        work->unk14.vy -= 0x400;
    } else {
        frame = entry->frame;
        if (work->unk0 != NULL) {
            if (work->unk0->state == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk14.vx;
                work->unk0->unk5C.vy = work->unk14.vy;
                work->unk0->unk5C.vz = work->unk14.vz;
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

s32 fx_0f8_op81(s32 mode, Fx0f8Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 i;
    s32 j;
    Fx0f8Spawn *t;
    Fx0f8Spawn *u;

    if (mode == 0) {
        *arg4 = 0x50;
    } else if (mode == 1) {
        work->unk28.vx = env->targets->centerX;
        work->unk28.vy = env->targets->centerY;
        work->unk28.vz = env->targets->centerZ;
        D_801E7BD0 = env;
        for (t = D_801E7B78, i = 0; i < 4; i++, t++) {
            (work->unk30 + i)->vx = work->unk28.vx;
            (work->unk30 + i)->vy = work->unk28.vy;
            (work->unk30 + i)->vz = work->unk28.vz;
            t->unk2 = i * 4;
            rot.vx = 0;
            rot.vy = i << 10;
            rot.vz = 0;
            ex->btlfx_pointAhead((s16 *)&work->unk28, (s16 *)&rot, ex->btlfx_rand() % 400 + 0x578, (s16 *)&work->unk30[i]);
        }
    } else {
        frame = entry->frame;
        if (frame == 0xF) {
            work->unk20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->unk28);
            work->unk24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->unk28);
        }
        for (u = D_801E7B78, j = 0; j < 4; j++, u++) {
            if (frame == u->unk2) {
                work->unk0[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, u->unk0), (s32)&work->unk30[j]);
                work->unk10[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->unk30[j]);
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 1), frame);
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 2), frame);
        return frame >= 0x2D;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f8/nonmatchings/fx_0f8", fx_0f8_headerPtr);
