#include "fxmod/fx_018.h"

INCLUDE_ASM("asm/us/fx_018/nonmatchings/fx_018", fx_018_op80);

void func_801E82FC(Unk80100C78Entry *entry, Fx018Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9CE8);
    SVECTOR v;
    SVECTOR ang;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 s;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    p = strip->unk4;
    strip->unk2 = 0xE;
    strip->unk0 = 0xE;
    strip->unk1 = 0;
    for (i = 0; i < 15; i++) {
        t = 0x1000 - (i << 12) / 14;
        t += *tbl++;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        s = *tbl++;
        s = (ex->math_sin(t >> 1) * s) >> 12;
        s += ex->btlfx_lerp(from1, to1, t);
        ex->func_800D044C(&v, (s32)p, 0, (XY32 *)&ang, s, width);
        p->pad = arg11;
        width += dwidth + *tbl++;
        p++;
    }
    if (kind < 2) {
        ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E9C88[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 0);
    } else {
        ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E9C88[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 2);
    }
}

void func_801E8564(Unk80100C78Entry *entry, Fx018Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9CE8);
    SVECTOR v;
    SVECTOR ang;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 s;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    p = strip->unk4;
    strip->unk2 = 2;
    strip->unk0 = 2;
    strip->unk1 = 0;
    for (i = 0; i < 3; i++) {
        t = 0x1000 - (i << 11);
        t += *tbl++ / 3;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        s = *tbl++ / 2;
        t >>= 1;
        s = (ex->math_sin(t) * s) >> 12;
        s += (ex->math_sin(t) * amp) >> 12;
        ex->func_800D044C(&v, (s32)p, 0, (XY32 *)&ang, s, width);
        p->pad = arg10;
        width += dwidth + *tbl++ / 2;
        p++;
    }
    if (kind < 2) {
        ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E9C90[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 0);
    } else {
        ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E9C90[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 2);
    }
}

void func_801E87E8(Unk80100C78Entry *entry, Fx018Strip *strip, Fx018Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *p = strip->unk4;
    SVECTOR v;

    p += i;
    ex = BTLFX_EXPORTS(D_801E9CE8);
    ex->btlfx_lerpVec((s16 *)p, (s16 *)(p + 1), 0x4000, (s16 *)&v);
    func_801E8564(entry, out, tbl, p, &v, 0x80, ex->btlfx_rand(), 0x33, 0, arg5, arg6, kind);
}

INCLUDE_ASM("asm/us/fx_018/nonmatchings/fx_018", func_801E88DC);

s32 func_801E8C84(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9CE8);
    SVECTOR *p = D_801E9E90[ex->btlfx_rand() % 16].unk4;

    p += ex->btlfx_rand() % 15;

    out->vx = p->vx;
    out->vy = p->vy;
    out->vz = p->vz;
    out->pad = 1;
    return 0;
}

INCLUDE_ASM("asm/us/fx_018/nonmatchings/fx_018", fx_018_op83);

INCLUDE_RODATA("asm/us/fx_018/nonmatchings/fx_018", fx_018_headerPtr);
