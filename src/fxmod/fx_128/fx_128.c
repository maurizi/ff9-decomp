#include "fxmod/fx_128.h"

s32 func_801E7704(Fx128Particle *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    BTLFX_EXPORTS(D_801E89D4)->btlfx_lerpVec((s16 *)&D_801E8F9C, (s16 *)&D_801E8FA4, (frame << 12) / 12, (s16 *)pos);
    out->unk0.vx = pos->vx << 12;
    out->unk0.vy = pos->vy << 12;
    out->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 func_801E77C8(Fx128Particle *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    BTLFX_EXPORTS(D_801E89D4)->btlfx_lerpVec((s16 *)&D_801E8F9C, (s16 *)((u8 *)D_801E8FB4 + (out->unk1F - 1) * 8),
                                             (frame << 12) / 12, (s16 *)pos);
    out->unk0.vx = pos->vx << 12;
    out->unk0.vy = pos->vy << 12;
    out->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_128_op80(s32 mode, Fx128Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E89D4 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk10);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk8);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        frame = entry->unk0;
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk10.vx;
                work->unk0->unk5C.vy = work->unk10.vy;
                work->unk0->unk5C.vz = work->unk10.vz;
            }
        }
        if (work->unk4 != NULL) {
            if (work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk10.vx;
                work->unk4->unk5C.vy = work->unk10.vy;
                work->unk4->unk5C.vz = work->unk10.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_128/nonmatchings/fx_128", fx_128_op81);

void func_801E8754(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E89D4)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8798(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E87F0(Fx128Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx128Point *)data;
}

INCLUDE_ASM("asm/us/fx_128/nonmatchings/fx_128", func_801E8808);

void func_801E889C(Fx128Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx128Point *)data;
}

s32 func_801E88B4(Fx128Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx128Point *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->unk4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

void func_801E8918(Fx128Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx128Point *)data;
}

INCLUDE_ASM("asm/us/fx_128/nonmatchings/fx_128", func_801E8930);

INCLUDE_RODATA("asm/us/fx_128/nonmatchings/fx_128", fx_128_headerPtr);
