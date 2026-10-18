#include "fxmod/fx_049.h"

void func_801E7704(Unk80100C78Entry *entry, Fx049Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8D88);
    SVECTOR v;
    SVECTOR ang;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 s;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    p = strip->unk4;
    strip->unk2 = 0xF;
    strip->unk0 = 0xF;
    strip->unk1 = 0;
    for (i = 0; i < 16; i++) {
        t = 0x1000 - (i << 12) / 15;
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
        ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E8D10[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 0);
    } else {
        ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E8D10[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 2);
    }
}

void func_801E796C(Unk80100C78Entry *entry, Fx049Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8D88);
    SVECTOR v;
    SVECTOR ang;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 s;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    p = strip->unk4;
    strip->unk2 = 3;
    strip->unk0 = 3;
    strip->unk1 = 0;
    for (i = 0; i < 4; i++) {
        t = 0x1000 - (i << 12) / 3;
        t += *tbl++ / 2;
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
        ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E8D18[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 0);
    } else {
        ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E8D18[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 2);
    }
}

void func_801E7BF8(Unk80100C78Entry *entry, Fx049Strip *strip, Fx049Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *p = strip->unk4;
    SVECTOR v;

    p += i;
    ex = BTLFX_EXPORTS(D_801E8D88);
    ex->btlfx_lerpVec((s16 *)p, (s16 *)(p + 1), 0x4000, (s16 *)&v);
    func_801E796C(entry, out, tbl, p, &v, 0x80, ex->btlfx_rand(), 0x37, 0, arg5, arg6, kind);
}

void func_801E7CEC(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *col, s32 arg6, s32 t, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8D88);
    Unk80100C78Entry *entry;
    SVECTOR r;
    s32 s;
    s32 w;

    BTLFX_COPY_SVEC(&r, rot);
    entry = D_801E9728;
    s = ex->math_sin((t << 11) / n);
    ex->func_800D1680((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), pos, &r, *scale, t, 1, s, 0, 0);
    ex->func_800D1680((u8 *)ex->btlfx_getFileData(entry->unk8, 0x19), pos, &r, *scale, t, 1, s, 2, 2);
    ex->func_800D1680((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), pos, &r, *scale * 5 / 2, t, 1, *col, 0, 0);
    w = (t << 12) / n + 0x1800;
    ex->func_800D1FFC((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), pos, &r, w, t, 1, *col);
    ex->func_800D1FFC((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), pos, &r, w, t, 1, *col);
}

s32 func_801E7F5C(s32 mode, Fx049Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx049Strip *strip;
    Fx049Strip *br;
    Pool6CEntry *p;
    s32 c;
    s32 t;
    s32 w;

    if (mode == 1) {
        it->unk11 = 0;
        return 0;
    }
    switch (it->unk11) {
    case 0:
        t = frame << 9;
        if (it->unk14 < 2) {
            c = ex->math_cos(t >> 2);
            strip = &D_801E8D98[it->unk10];
            br = (Fx049Strip *)&D_801E93C8[it->unk10];
            w = it->unk15;
            if (w == 0xFF) {
                w = -0x20;
            }
            func_801E7704(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame * 0x97, 0x186, frame, c,
                          ((0x1000 - t) * (w + 0xFB)) >> 12, it->unk14);
            func_801E7BF8(entry, strip, br, it->unk18, it->unk12, c, (0x1000 - t) >> 6, it->unk14);
            func_801E7BF8(entry, strip, br, it->unk18 + 3, it->unk13, c, (0x1000 - t) >> 6, it->unk14);
            if (frame == 0) {
                it->unk1C = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&it->unk0);
                if (it->unk1C != NULL) {
                    it->unk1C->unk22 = ex->func_800E7CC4(0, 0x40);
                }
                if ((it->unk10 & 1) && it->unk15 != 0xFF) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0xC, 0x46);
                    p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&it->unk0);
                    if (p != NULL) {
                        p->unk22 = ex->func_800E7CC4(0, 0x40);
                    }
                }
            }
            if (frame >= 8) {
                it->unk11 = 1;
                ((s16 *)it)[-1] = -1;
            }
        } else {
            t = (frame << 12) / 6;
            c = ex->math_cos(t >> 2);
            strip = &D_801E8D98[it->unk10];
            br = (Fx049Strip *)&D_801E93C8[it->unk10];
            func_801E7704(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame << 6, 0x186, frame, c,
                          ((0x1000 - t) * 0xAA) >> 12, it->unk14);
            func_801E7BF8(entry, strip, br, it->unk18, it->unk12, c, (0x1000 - t) >> 6, it->unk14);
            func_801E7BF8(entry, strip, br, it->unk18 + 3, it->unk13, c, (0x1000 - t) >> 6, it->unk14);
            if (frame == 0) {
                it->unk1C = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&it->unk0);
                if (it->unk1C != NULL) {
                    it->unk1C->unk22 = ex->func_800E7CC4(0, 0x40);
                }
            }
            if (frame >= 6) {
                it->unk11 = 1;
                ((s16 *)it)[-1] = -1;
            }
        }
        break;
    case 1:
        if (frame >= 6) {
            return 1;
        }
        break;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_049/nonmatchings/fx_049", fx_049_op83);

INCLUDE_RODATA("asm/us/fx_049/nonmatchings/fx_049", fx_049_headerPtr);
