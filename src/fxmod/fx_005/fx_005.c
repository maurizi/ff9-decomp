#include "fxmod/fx_005.h"

void func_801E7704(Unk80100C78Entry *entry, Fx005Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8C40);
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
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, D_801E8BC8[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, D_801E8BC8[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 2);
    }
}

void func_801E796C(Unk80100C78Entry *entry, Fx005Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8C40);
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
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, D_801E8BD0[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, D_801E8BD0[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 2);
    }
}

void func_801E7BF8(Unk80100C78Entry *entry, Fx005Strip *strip, Fx005Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *p = strip->unk4;
    SVECTOR v;

    p += i;
    ex = BTLFX_EXPORTS(D_801E8C40);
    ex->btlfx_lerpVec((s16 *)p, (s16 *)(p + 1), 0x4000, (s16 *)&v);
    func_801E796C(entry, out, tbl, p, &v, 0x80, ex->btlfx_rand(), 0x37, 0, arg5, arg6, kind);
}

void func_801E7CEC(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *col, s32 arg6, s32 t, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8C40);
    Unk80100C78Entry *entry;
    SVECTOR r;
    s32 s;
    s32 w;

    BTLFX_COPY_SVEC(&r, rot);
    entry = D_801E95E0;
    s = ex->math_sin((t << 11) / n);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x1D), pos, &r, *scale + 0x200, t, 1, s, 0, 0);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x1C), pos, &r, *scale + 0x200, t, 1, s, 2, 2);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x16), pos, &r, *scale * 5 / 2, t, 1, s, 0, 0);
    w = (t << 12) / n + 0x1800;
    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), pos, &r, w, t, 1, *col);
    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, 0x1F), pos, &r, w, t, 1, *col);
}

s32 func_801E7F60(s32 mode, Fx005Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx005Strip *strip;
    Fx005Strip *br;
    Pool6CEntry *p;
    s32 c;
    s32 t;

    if (mode == 1) {
        it->unk11 = 0;
        return 0;
    }
    switch (it->unk11) {
    case 0:
        t = frame << 9;
        if (it->unk14 < 2) {
            c = ex->math_cos(t >> 2);
            strip = &D_801E8C50[it->unk10];
            br = (Fx005Strip *)&D_801E9280[it->unk10];
            func_801E7704(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame * 0x97, 0x186, frame, c,
                          ((0x1000 - t) * (it->unk15 + 0xFB)) >> 12, it->unk14);
            func_801E7BF8(entry, strip, br, it->unk18, it->unk12, c, (0x1000 - t) >> 6, it->unk14);
            func_801E7BF8(entry, strip, br, it->unk18 + 3, it->unk13, c, (0x1000 - t) >> 6, it->unk14);
            if (frame == 0) {
                it->unk1C = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&it->unk0);
                if (it->unk1C != NULL) {
                    it->unk1C->unk22 = ex->func_800E7CC4(0, 0x40);
                }
                if (it->unk10 & 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0xC, 0x46);
                    p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&it->unk0);
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
            strip = &D_801E8C50[it->unk10];
            br = (Fx005Strip *)&D_801E9280[it->unk10];
            func_801E7704(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame << 6, 0x186, frame, c,
                          ((0x1000 - t) * 0xAA) >> 12, it->unk14);
            func_801E7BF8(entry, strip, br, it->unk18, it->unk12, c, (0x1000 - t) >> 6, it->unk14);
            func_801E7BF8(entry, strip, br, it->unk18 + 3, it->unk13, c, (0x1000 - t) >> 6, it->unk14);
            if (frame == 0) {
                it->unk1C = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&it->unk0);
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

s32 fx_005_op83(s32 mode, Fx005Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    Fx005Bolt *it;
    Pool6CEntry *p;
    s32 *q;
    s32 frame;
    s32 kind;
    s32 i;
    s32 r;
    s32 ang;
    s32 c;

    if (mode == 0) {
        init->unk0 = 0x18;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801E7F60;
        init->unk4 = 0xC;
    } else if (mode == 1) {
        D_801E8C40 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        i = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        q = work->unk10 = D_801E95F0;
        for (; i < 0x40; i++) {
            *q++ = ex->btlfx_randCentered(0x100);
            *q++ = ex->btlfx_randCentered(0x200);
            *q++ = ex->btlfx_randCentered(0xF5);
        }
        D_801E95E0 = entry;
        work->unk14 = 0;
    } else {
        frame = entry->frame;
        kind = 0;
        if (frame < 0x10) {
            kind = D_801E8BD8[frame];
        }
        if (kind > 0) {
            work->unk14++;
            BTLFX_COPY_SVEC(&a, &work->unk0);
            if (kind != 2) {
                a.vx += ex->btlfx_randCentered(0x200);
                a.vy += ex->btlfx_randCentered(0x300);
                a.vz += ex->btlfx_randCentered(0x200);
            }
            if (kind != 3) {
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    BTLFX_COPY_SVEC(&it->unk0, &a);
                    BTLFX_COPY_SVEC(&b, &a);
                    b.vy -= 0xC00;
                    r = ex->btlfx_randCentered(0x100) + 0x640;
                    if (kind != 2) {
                        r = r * 3 / 8;
                    }
                    ang = (work->unk14 << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    b.vx += (ex->math_cos(ang) * r) >> 12;
                    b.vz += (ex->math_sin(ang) * r) >> 12;
                    if (kind != 2) {
                        it->unk0.vx = b.vx;
                        it->unk0.vz = b.vz;
                    }
                    BTLFX_COPY_SVEC(&it->unk8, &b);
                    it->unk10 = work->unk14 % 12;
                    it->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                    it->unk12 = ex->btlfx_rand() % 14;
                    it->unk13 = ex->btlfx_rand() % 14;
                    it->unk15 = 0;
                    if (kind == 1) {
                        it->unk14 = 2;
                    } else if (kind == 2) {
                        it->unk14 = ex->btlfx_rand() & 1;
                    }
                }
            } else {
                for (kind = 0; kind < 2; kind++) {
                    it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it != NULL) {
                        BTLFX_COPY_SVEC(&it->unk0, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        b.vy -= 0xC00;
                        r = ex->btlfx_randCentered(0x80) + 0x80;
                        ang = ((work->unk14 + kind) << 11) - 0x200 + ex->btlfx_randCentered(0x80);
                        b.vx += (ex->math_cos(ang) * r) >> 12;
                        b.vz += (ex->math_sin(ang) * r) >> 12;
                        BTLFX_COPY_SVEC(&it->unk8, &b);
                        it->unk10 = work->unk14 % 12;
                        it->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                        it->unk12 = ex->btlfx_rand() % 14;
                        it->unk13 = ex->btlfx_rand() % 14;
                        it->unk14 = ex->btlfx_rand() & 1;
                        it->unk15 = 0x80;
                    }
                }
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->unk0);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x19), (s32)&work->unk8);
            }
        }
        if (frame == 0xF) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x40, 0x40, 0x40);
        }
        if (frame >= 0x10 && frame < 0x13) {
            c = 200 - (frame - 0x10) * 200 / 3;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c >> 1, c >> 1, c);
        }
        if (frame == 0x14) {
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x19), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x40);
            }
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1A), (s32)&work->unk0);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x40);
                p->unk18->unk24 = (void (*)())func_801E7CEC;
            }
        }
        if (frame >= 0x30) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_005/nonmatchings/fx_005", fx_005_headerPtr);
