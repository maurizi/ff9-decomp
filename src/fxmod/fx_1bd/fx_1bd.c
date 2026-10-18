#include "fxmod/fx_1bd.h"

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E77C0);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", fx_1bd_op80);

s32 fx_1bd_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E9E54 = env;
    } else {
        return entry->unk0 > 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1bd/nonmatchings/fx_1bd", fx_1bd_headerPtr);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", fx_1bd_op82);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", fx_1bd_op83);

void func_801E8D5C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E9E54)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8DA0(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8DF8(Fx1bdTrack12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx1bdKey12 *)data;
}

s32 func_801E8E0C(Fx1bdTrack6 *track, s32 t, SVECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx1bdKey6 *k;

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

void func_801E8E78(Fx1bdPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1bdPoint *)data;
}

s32 func_801E8E90(Fx1bdPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1bdPoint *p;

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

void func_801E8EF4(Fx1bdTrack12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx1bdKey12 *)data;
}

s32 func_801E8F08(Fx1bdTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx1bdKey12 *k;

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

void func_801E8FA4(Fx1bdTrack12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx1bdKey12 *)data;
}

s32 func_801E8FB8(Fx1bdTrack6 *track, s32 t, VECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx1bdKey6 *k;

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

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E901C);

void func_801E9034(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E9E54)->btlfx_drawScreenTint((u32 *)(D_801E9E54->unk20 + ot), arg4, c, c, c);
}

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E90CC);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E9118);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E9174);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E92D4);

void func_801E9874(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801E9E54;
    u32 *p = (u32 *)env->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;
    RECT r2;
    RECT r;

    env->unk24 = (u8 *)(p + 12);
    n = w - m;
    if (m != 0) {
        r.x = sx;
        r.y = sy;
        r.w = m;
        r.h = h;
        ex->SetDrawMove(p, &r, dx + n, dy);
        addPrim(env->unk20 + 0xFFF, p);
    }
    p += 6;
    if (n != 0) {
        r2.x = sx + m;
        r2.y = sy;
        r2.w = n;
        r2.h = h;
        ex->SetDrawMove(p, &r2, dx, dy);
        addPrim(env->unk20 + 0xFFF, p);
    }
}
