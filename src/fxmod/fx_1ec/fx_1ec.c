#include "fxmod/fx_1ec.h"

s32 func_801E78E0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E8C60(&D_801E9034[(s16)(e->unk1F - 1)], frame, &v);
    v.vy = v.vy * 4 / 5;
    v.vz = v.vz * 3 / 4;
    v.vx += D_801EFB64.vx;
    v.vy += D_801EFB64.vy;
    v.vz += D_801EFB64.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 func_801E79DC(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E8C60(&D_801E90C4[(s16)(e->unk1F - 1)], frame, &v);
    v.vx += D_801EFB6C.vx;
    v.vy += D_801EFB6C.vy;
    v.vz += D_801EFB6C.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_op80);

INCLUDE_RODATA("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_headerPtr);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_op81);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_op82);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8AFC);

void func_801E8B14(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8E94)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8B58(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8BB0(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E8E94)->btlfx_drawScreenTint((u32 *)(D_801E8E94->unk20 + ot), arg4, c, c, c);
}

void func_801E8C48(Fx1ecPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1ecPoint *)data;
}

s32 func_801E8C60(Fx1ecPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1ecPoint *p;

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

void func_801E8CC4(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8E94);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

s32 func_801E8D30(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8E94);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

void func_801E8DB4(s32 unit) {
    if (BTLFX_EXPORTS(D_801E8E94)->btlfx_isUnitActive(unit)) {
        func_801E8CC4(unit, 2);
    }
}

void func_801E8E00(s32 unit) {
    if (BTLFX_EXPORTS(D_801E8E94)->btlfx_isUnitActive(unit)) {
        func_801E8CC4(unit, 3);
    }
}
