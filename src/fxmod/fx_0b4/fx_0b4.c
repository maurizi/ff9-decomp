#include "fxmod/fx_0b4.h"

s32 func_801E77BC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E793C(pt, pos, &D_801E80EC[0], &D_801E81C4[0], frame, n);
}

s32 func_801E77FC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E793C(pt, pos, &D_801E80EC[1], &D_801E81C4[1], frame, n);
}

s32 func_801E783C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E793C(pt, pos, &D_801E80EC[2], &D_801E81C4[2], frame, n);
}

s32 func_801E787C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E793C(pt, pos, &D_801E80EC[3], &D_801E81C4[3], frame, n);
}

s32 func_801E78BC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E793C(pt, pos, &D_801E80EC[4], &D_801E81C4[4], frame, n);
}

s32 func_801E78FC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E793C(pt, pos, &D_801E80EC[5], &D_801E81C4[5], frame, n);
}

s32 func_801E793C(Pool30Entry *pt, SVECTOR *pos, Fx0b4Timer *timer, Fx0b4Path *path, s32 frame, s32 n) {
    SVECTOR rot;
    SVECTOR p;

    func_801E8058(path, frame, &rot, &p);
    p.vx /= 2;
    p.vz /= 2;
    p.vx = p.vx * 12 / 10;
    p.vy = p.vy * 12 / 10;
    p.vz = p.vz * 12 / 10;
    p.vx += D_801E818C.vx;
    p.vy += D_801E818C.vy;
    p.vz += D_801E818C.vz;
    pos->vx = p.vx;
    pos->vy = p.vy;
    pos->vz = p.vz;
    pt->unk0.vx = pos->vx << 12;
    pt->unk0.vy = pos->vy << 12;
    pt->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b4/nonmatchings/fx_0b4", fx_0b4_headerPtr);

INCLUDE_ASM("asm/us/fx_0b4/nonmatchings/fx_0b4", fx_0b4_op80);

INCLUDE_ASM("asm/us/fx_0b4/nonmatchings/fx_0b4", func_801E8040);

s32 func_801E8058(Fx0b4Path *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx0b4Key *k;
    s32 r = 1;

    if (i > path->unk2 - 1) {
        i = path->unk2 - 1;
    } else {
        r = 0;
    }
    k = &path->unk4[i];
    if (rot != NULL) {
        rot->vx = k->unk6[0];
        rot->vy = k->unk6[1];
        rot->vz = k->unk6[2];
    }
    if (pos != NULL) {
        pos->vx = k->unk0[0];
        pos->vy = k->unk0[1];
        pos->vz = k->unk0[2];
    }
    return r;
}
