#include "fxmod/fx_193.h"

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7794(e, pos, &D_801E7CBC, &D_801E7CC4, -0x320, frame, frames);
}

s32 func_801E774C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7794(e, pos, &D_801E7CCC, &D_801E7CD4, 0x320, frame, frames);
}

s32 func_801E7794(Pool30Entry *e, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 amp, s32 frame, s32 frames) {
    s32 t = (frame << 12) / 7;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7CB4);

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)pos);
    if (frame >= 4) {
        amp /= 2;
    }
    pos->vx += amp * ex->math_sin(t) / 4096;
    e->unk0.vx = pos->vx << 12;
    e->unk0.vy = pos->vy << 12;
    e->unk0.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_193_op80(s32 mode, Fx193Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E7CB4 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        D_801E7CC4.vx = work->unk8.vx;
        D_801E7CC4.vy = work->unk8.vy;
        D_801E7CC4.vz = work->unk8.vz;
        D_801E7CD4.vx = work->unk8.vx;
        D_801E7CD4.vy = work->unk8.vy;
        D_801E7CD4.vz = work->unk8.vz;
        ex->func_800DC818(0x10, 0xE, (u16 *)&D_801E7CBC);
        ex->func_800DC818(0x10, 0x10, (u16 *)&D_801E7CCC);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&D_801E7CDC);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&D_801E7CDC);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7CE4;
            work->unk0->unk12 = 4;
            work->unk0->unkC = 0xF0;
        }
        if (work->unk4 != NULL) {
            work->unk4->unk14 = (s32)D_801E80A4;
            work->unk4->unk12 = 4;
            work->unk4->unkC = 0xF0;
        }
        if (work->unk0 != NULL) {
            ((EffMotion *)work->unk0->unk0)->unk8 = func_801E7704;
        }
        if (work->unk4 != NULL) {
            ((EffMotion *)work->unk4->unk0)->unk8 = func_801E774C;
        }
    } else {
        return entry->unk0 >= 0x1E;
    }
    return 0;
}

s32 fx_193_op81(s32 mode, Fx193Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        D_801E7CB4 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk18);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk10);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk10);
        work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk18);
    } else {
        frame = entry->unk0;
        if (frame == 4) {
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk18);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0, 0x20);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_193/nonmatchings/fx_193", fx_193_headerPtr);
