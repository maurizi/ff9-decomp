#include "fxmod/fx_078.h"

INCLUDE_ASM("asm/us/fx_078/nonmatchings/fx_078", fx_078_op80);

void func_801E7B70(Fx078Track12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx078Key12 *)data;
}

s32 func_801E7B84(Fx078Track12 *track, s32 t, SVECTOR *rot, SVECTOR *pos) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx078Key12 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    if (rot != NULL) {
        rot->vx = k->unk6;
        rot->vy = k->unk8;
        rot->vz = k->unkA;
    }
    if (pos != NULL) {
        pos->vx = -k->unk0;
        pos->vy = k->unk2;
        pos->vz = -k->unk4;
    }
    return done;
}

void func_801E7C20(Fx078Track6 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx078Key6 *)data;
}

s32 func_801E7C34(Fx078Track6 *track, s32 t, SVECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx078Key6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    out->vx = -k->x;
    out->vy = k->y;
    out->vz = -k->z;
    return done;
}

void func_801E7CA0(Fx078Track6 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx078Key6 *)data;
}

s32 func_801E7CB4(Fx078Track6 *track, s32 t, VECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx078Key6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    out->vx = k->x;
    out->vy = k->y;
    out->vz = k->z;
    return done;
}

INCLUDE_RODATA("asm/us/fx_078/nonmatchings/fx_078", fx_078_headerPtr);
