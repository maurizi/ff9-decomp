#include "fxmod/fx_003.h"

void func_801E7704(Unk80100C78Entry *entry, Fx003Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8580);
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
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E8528[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E8528[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 2);
    }
}

void func_801E796C(Unk80100C78Entry *entry, Fx003Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8580);
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
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E8530[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E8530[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 2);
    }
}

void func_801E7BF8(Unk80100C78Entry *entry, Fx003Strip *strip, Fx003Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *p = strip->unk4;
    SVECTOR v;

    p += i;
    ex = BTLFX_EXPORTS(D_801E8580);
    ex->btlfx_lerpVec((s16 *)p, (s16 *)(p + 1), 0x4000, (s16 *)&v);
    func_801E796C(entry, out, tbl, p, &v, 0x80, ex->btlfx_rand(), 0x37, 0, arg5, arg6, kind);
}

s32 func_801E7CEC(s32 mode, Fx003Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx003Strip *strip;
    Fx003Strip *br;
    s32 c;
    s32 t;

    if (mode == 1) {
        it->unk11 = 0;
        return 0;
    }
    switch (it->unk11) {
    case 0:
        if (it->unk14 < 2) {
            t = (frame << 12) / 6;
            c = ex->math_cos(t >> 2);
            strip = &D_801E8590[it->unk10];
            br = (Fx003Strip *)&D_801E8BC0[it->unk10];
            func_801E7704(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame * 0x9B, 0x186, frame, c,
                          ((0x1000 - t) * 0xF1) >> 12, it->unk14);
            func_801E7BF8(entry, strip, br, it->unk18, it->unk12, c, (0x1000 - t) >> 6, it->unk14);
            func_801E7BF8(entry, strip, br, it->unk18 + 3, it->unk13, c, (0x1000 - t) >> 6, it->unk14);
            if (frame == 0) {
                it->unk1C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&it->unk0);
                if (it->unk1C != NULL) {
                    it->unk1C->unk22 = ex->func_800E7CC4(0, 0x40);
                }
                env->unk28 = ex->func_800E7CC4(0, 0x46);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &it->unk0, NULL, 0x1400, 4, 1, 0x1000);
                env->unk28 = 0;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 7, 0x20);
            }
            if (frame == 1) {
                it->unk20 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&it->unk0);
                if (it->unk20 != NULL) {
                    it->unk20->unk22 = ex->func_800E7CC4(0, 0x40);
                }
            }
            if (frame >= 6) {
                it->unk11 = 1;
                ((s16 *)it)[-1] = -1;
            }
        } else {
            t = frame << 10;
            c = ex->math_cos(t >> 2);
            strip = &D_801E8590[it->unk10];
            br = (Fx003Strip *)&D_801E8BC0[it->unk10];
            func_801E7704(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame << 7, 0x186, frame, c,
                          ((0x1000 - t) * 0xAA) >> 12, it->unk14);
            func_801E7BF8(entry, strip, br, it->unk18, it->unk12, c, (0x1000 - t) >> 6, it->unk14);
            func_801E7BF8(entry, strip, br, it->unk18 + 3, it->unk13, c, (0x1000 - t) >> 6, it->unk14);
            if (frame >= 4) {
                return 1;
            }
        }
        break;
    case 1:
        if (frame >= 0x10) {
            return 1;
        }
        break;
    }
    return 0;
}

s32 fx_003_op83(s32 mode, Fx003Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 r;
    s32 ang;
    Fx003Bolt *it;
    s32 *q;
    SVECTOR a;
    SVECTOR b;

    if (mode == 0) {
        init->unk0 = 0x10;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801E7CEC;
        init->unk4 = 0xC;
    } else if (mode == 1) {
        D_801E8580 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        work->unk8 = D_801E8F20;
        q = D_801E8F20;
        for (i = 0; i < 0x40; i++) {
            *q++ = ex->btlfx_randCentered(0x100);
            *q++ = ex->btlfx_randCentered(0x180);
            *q++ = ex->btlfx_randCentered(0xC0);
        }
        work->unkC = 0;
    } else {
        frame = entry->unk0;
        if (frame == 0 || frame == 3 || frame == 5 || frame == 6 || frame == 7) {
            if (work->unkC < 5) {
                work->unkC++;
                BTLFX_COPY_SVEC(&a, &work->unk0);
                if (work->unkC == 3) {
                    a.vx += ex->btlfx_randCentered(0x190);
                    a.vy += ex->btlfx_randCentered(0x190);
                    a.vz += ex->btlfx_randCentered(0x190);
                }
                it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it != NULL) {
                    BTLFX_COPY_SVEC(&it->unk0, &a);
                    BTLFX_COPY_SVEC(&b, &a);
                    b.vy -= 0xC00;
                    r = ex->btlfx_randCentered(0x100) + 0x640;
                    ang = (work->unkC << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    b.vx += (ex->math_cos(ang) * r) >> 12;
                    b.vz += (ex->math_sin(ang) * r) >> 12;
                    BTLFX_COPY_SVEC(&it->unk8, &b);
                    it->unk10 = work->unkC;
                    it->unk18 = work->unk8 + (ex->btlfx_rand() & 0x1F);
                    it->unk12 = ex->btlfx_rand() % 14;
                    it->unk13 = ex->btlfx_rand() % 14;
                    if ((work->unkC - 1) & 1) {
                        it->unk14 = 2;
                    } else {
                        it->unk14 = ex->btlfx_rand() & 1;
                    }
                }
            }
        }
        if (frame >= 0xC) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_003/nonmatchings/fx_003", fx_003_headerPtr);
