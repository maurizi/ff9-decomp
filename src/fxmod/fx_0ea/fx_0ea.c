#include "fxmod/fx_0ea.h"

s32 func_801E7704(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E77D4(pt, pos, &D_801E7E64[0], frame, n);
}

s32 func_801E7738(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E77D4(pt, pos, &D_801E7E64[1], frame, n);
}

s32 func_801E776C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E77D4(pt, pos, &D_801E7E64[2], frame, n);
}

s32 func_801E77A0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E77D4(pt, pos, &D_801E7E64[3], frame, n);
}

s32 func_801E77D4(Pool30Entry *pt, SVECTOR *pos, SVECTOR *to, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7E54);
    s32 t = (frame << 12) / 18;

    ex->btlfx_lerpVec((s16 *)&D_801E7E5C, (s16 *)to, t, (s16 *)pos);
    pos->vy -= ex->math_sin(t / 2) * 800 / 4096;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_0ea_op80(s32 mode, Fx0eaWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u16 n;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        D_801E7E54 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk8);
        work->unk10.vx = work->unk8.vx;
        work->unk10.vy = work->unk8.vy;
        work->unk10.vz = work->unk8.vz;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
        work->unk1A = 0x1000;
        work->unk18 = 0;
    } else {
        frame = entry->frame;
        if (frame == 0x1E) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk8);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
        }
        if (frame >= 0x1E && frame < 0x2B) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->unk10, work->unk18, work->unk1A);
            if (frame - 0x1E >= 5) {
                work->unk1A -= 0x200;
            }
            work->unk18++;
            n = work->unk18;
            if (n >= 6) {
                n = 5;
            }
            work->unk18 = n;
        }
        return frame >= 0x32;
    }
    return 0;
}

s32 fx_0ea_op81(s32 mode, Fx0eaWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 n;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        n = *arg4;
        work->unk20 = n;
        D_801E7E54 = env;
        if (n >= env->targets->targetCount || ex->btl_getUnit(0x10) == ex->btl_getUnit(work->unk20)) {
            func_801E7DE0(env, 7, 1);
            return 1;
        }
        func_801E7DE0(env, 7, 0);
        ex->btlfx_playFileSound(0x13, entry->file, -1);
        ex->btlfx_playFileSound(0x14, entry->file, -1);
        ex->btlfx_playFileSound(0x15, entry->file, -1);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        D_801E7E5C.vx = work->unk8.vx;
        D_801E7E5C.vy = work->unk8.vy;
        D_801E7E5C.vz = work->unk8.vz;
        ex->btlfx_getUnitBodyPos(work->unk20, 0, (s16 *)&work->unk10);
        D_801E7E64[work->unk20].vx = work->unk10.vx;
        D_801E7E64[work->unk20].vy = work->unk10.vy;
        D_801E7E64[work->unk20].vz = work->unk10.vz;
        ex->btlfx_getUnitPos(work->unk20, (s16 *)&work->unk18);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E7DF4[work->unk20]), (s32)&D_801E7E84);
        if (work->unk0 != NULL) {
            ((Fx0eaEffHdr *)work->unk0->file)->unk8 = D_801E7DFC[work->unk20];
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
        }
    } else {
        frame = entry->frame;
        if (frame == 0x14) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk18);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(work->unk20, 0x80);
            }
        }
        if (frame == 0xF) {
            func_801E7DE0(env, 7, 1);
        }
        return frame >= 0x32;
    }
    return 0;
}

void func_801E7DE0(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->state + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_0ea/nonmatchings/fx_0ea", fx_0ea_headerPtr);
