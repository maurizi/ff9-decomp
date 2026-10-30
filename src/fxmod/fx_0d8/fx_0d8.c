#include "fxmod/fx_0d8.h"

void func_801E7704(Unk80100C78Entry *entry, Fx0d8Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8FA0);
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
        ex->btlfx_offsetPoint(&v, (s32)p, 0, (XY32 *)&ang, s, width);
        p->pad = arg11;
        width += dwidth + *tbl++;
        p++;
    }
    if (kind < 2) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, D_801E8F28[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, D_801E8F28[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 2);
    }
}

void func_801E796C(Unk80100C78Entry *entry, Fx0d8Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8FA0);
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
        ex->btlfx_offsetPoint(&v, (s32)p, 0, (XY32 *)&ang, s, width);
        p->pad = arg10;
        width += dwidth + *tbl++ / 2;
        p++;
    }
    if (kind < 2) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, D_801E8F30[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, D_801E8F30[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 2);
    }
}

void func_801E7BF8(Unk80100C78Entry *entry, Fx0d8Strip *strip, Fx0d8Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *p = strip->unk4;
    SVECTOR v;

    p += i;
    ex = BTLFX_EXPORTS(D_801E8FA0);
    ex->btlfx_lerpVec((s16 *)p, (s16 *)(p + 1), 0x4000, (s16 *)&v);
    func_801E796C(entry, out, tbl, p, &v, 0x80, ex->btlfx_rand(), 0x37, 0, arg5, arg6, kind);
}

void func_801E7CEC(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *arg5, s32 arg6,
                   s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8FA0);
    Unk80100C78Entry *entry;
    SVECTOR r;
    s32 s;
    s32 t;

    BTLFX_COPY_SVEC(&r, rot);
    entry = D_801E9940;
    s = ex->math_sin((frame << 11) / n);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x1A), pos, &r, *scale * 3 / 2, frame, 1, s, 0, 0);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x19), pos, &r, *scale * 3 / 2, frame, 1, s, 2, 2);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x1D), pos, &r, *scale * 5 / 2, frame, 1, s, 0, 0);
    t = (frame << 12) / n + 0x1800;
    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, 0x1B), pos, &r, t, frame, 1, *arg5);
    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, 0x1C), pos, &r, t, frame, 1, *arg5);
}

s32 func_801E7F80(s32 mode, Fx0d8Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0d8Strip *strip;
    Fx0d8Strip *br;
    Pool6CEntry *p;
    s32 c;
    s32 t;

    if (mode == 1) {
        part->unk11 = 0;
        return 0;
    }
    switch (part->unk11) {
    case 0:
        if (part->unk14 < 2) {
            t = frame << 9;
            c = ex->math_cos(t >> 2);
            strip = (Fx0d8Strip *)D_801E8FB0[part->unk10];
            br = (Fx0d8Strip *)D_801E95E0[part->unk10];
            func_801E7704(entry, strip, part->unk18, &part->unk0, &part->unk8, 0, 0x200, frame * 0x97, 0x186, frame, c,
                          ((0x1000 - t) * (part->unk15 + 0xFB)) >> 12, part->unk14);
            func_801E7BF8(entry, strip, br, part->unk18, part->unk12, c, (0x1000 - t) >> 6, part->unk14);
            func_801E7BF8(entry, strip, br, part->unk18 + 3, part->unk13, c, (0x1000 - t) >> 6, part->unk14);
            if (frame == 0) {
                part->unk1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&part->unk0);
                if (part->unk1C != NULL) {
                    part->unk1C->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                if (part->unk10 & 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0xC, 0x46);
                    p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&part->unk0);
                    if (p != NULL) {
                        p->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                    }
                }
            }
            if (frame >= 8) {
                part->unk11 = 1;
                ((s16 *)part)[-1] = -1;
            }
        } else {
            t = (frame << 12) / 6;
            c = ex->math_cos(t >> 2);
            strip = (Fx0d8Strip *)D_801E8FB0[part->unk10];
            br = (Fx0d8Strip *)D_801E95E0[part->unk10];
            func_801E7704(entry, strip, part->unk18, &part->unk0, &part->unk8, 0, 0x200, frame << 6, 0x186, frame, c,
                          ((0x1000 - t) * 0xAA) >> 12, part->unk14);
            func_801E7BF8(entry, strip, br, part->unk18, part->unk12, c, (0x1000 - t) >> 6, part->unk14);
            func_801E7BF8(entry, strip, br, part->unk18 + 3, part->unk13, c, (0x1000 - t) >> 6, part->unk14);
            if (frame == 0) {
                part->unk1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&part->unk0);
                if (part->unk1C != NULL) {
                    part->unk1C->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= 6) {
                part->unk11 = 1;
                ((s16 *)part)[-1] = -1;
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

s32 fx_0d8_op83(s32 mode, Fx0d8Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    Fx0d8Part *it;
    Pool6CEntry *p;
    s32 *q;
    s32 frame;
    s32 kind;
    s32 i;
    s32 r;
    s32 ang;
    s32 c;
    s32 n;

    if (mode == 0) {
        init->workSize = sizeof(Fx0d8Work83);
        init->elemSize = 0x24;
        init->elemFn = (s32)func_801E7F80;
        init->poolCount = 0xC;
    } else if (mode == 1) {
        D_801E8FA0 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        work->unk10 = D_801E9950;
        for (n = 0, q = D_801E9950; n < 0x40; n++) {
            *q++ = ex->btlfx_randCentered(0x100);
            *q++ = ex->btlfx_randCentered(0x200);
            *q++ = ex->btlfx_randCentered(0xF5);
        }
        D_801E9940 = entry;
        work->unk14 = 0;
    } else {
        frame = entry->frame;
        kind = 0;
        if (frame < 16) {
            kind = D_801E8F38[frame];
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
                for (i = 0; i < 2; i++) {
                    it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it != NULL) {
                        BTLFX_COPY_SVEC(&it->unk0, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        b.vy -= 0xC00;
                        r = ex->btlfx_randCentered(0x80) + 0x80;
                        ang = ((work->unk14 + i) << 11) - 0x200 + ex->btlfx_randCentered(0x80);
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
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk0);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->unk8);
            }
        }
        if (frame == 15) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x40, 0x40, 0x40);
        }
        if ((u32)(frame - 16) < 3) {
            c = 0xC8 - ((frame - 16) * 0xC8) / 3;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c >> 1, c >> 1, c);
        }
        if (frame == 20) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->unk8);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->unk0);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                p->elems->unk24 = (void (*)())func_801E7CEC;
            }
        }
        if (frame >= 0x30) {
            return 1;
        }
    }
    return 0;
}

s32 fx_0d8_op84(s32 mode, Fx0d8Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 t;
    s32 u;
    s32 k;
    s32 v;
    u16 ry;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E8FA0 = env;
        work->unk0 = 0;
        work->unk4 = 0;
    } else {
        a = 0;
        b = 0;
        c = 0;
        frame = entry->frame;
        ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->unk8);
        ry = 0;
        switch (work->unk0) {
        case 0:
            ry = 0x200;
            a = (frame << 12) / 3;
            u = ex->math_sin(a >> 2);
            b = u >> 1;
            c = ((0x1000 - a) >> 1) + 0x800;
            if (frame >= 3) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            u = (frame << 12) / 9;
            k = (frame & 1) << 8;
            c = k + 0x1000;
            v = (u >> 1) + 0x800;
            a = k + v;
            ry = (u >> 3) + 0x200;
            b = c;
            if (frame >= 9) {
                work->unk0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            u = (frame << 12) / 9;
            t = u >> 2;
            ry = 0x400 - t;
            a = ex->math_cos(t);
            b = ex->math_sin(t) + 0x1000;
            c = ex->math_sin(t) * 3 / 2 + 0x1000;
            if (frame >= 9) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->otZ = 0x80;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), &work->unk8, NULL, b / 3, 0, 1, a / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1F), &work->unk8, &rot, c / 2, 0, 1, a * 2 / 3);
        env->otZ = 0;
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d8/nonmatchings/fx_0d8", fx_0d8_headerPtr);
