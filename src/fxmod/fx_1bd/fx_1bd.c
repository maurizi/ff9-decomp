#include "fxmod/fx_1bd.h"

void func_801E77C0(Fx1bdKeys *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip) {
    SVECTOR v;
    SVECTOR *p;
    s32 cnt;
    s32 i;
    s32 j;
    s32 k;

    cnt = (size - 4) / 8;
    p = (SVECTOR *)(buf + 4);
    buf[0] = buf[2] = cnt - 1;
    buf[1] = 0;
    if (n < cnt) {
        for (i = 0; i < cnt - n; i++) {
            p->pad = 0;
            p++;
        }
    } else {
        n = cnt;
    }
    for (i = 0; i < n; i++) {
        k = i + 1;
        j = n - k + start;
        if (clip != 0 && j >= keys->unk2) {
            break;
        }
        func_801E8E90(keys, j, &v);
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
        p->pad = from + (to - from) / keys->unk2 * (n - i);
        p++;
    }
}

s32 fx_1bd_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801E9E54 = env;
        ex->btlfx_initEffModels(D_801E9E64, 0x20);
        func_801E90CC();
        func_801E9118(0, 1, 0, ex->btlfx_getFileData(entry->unk8, 0x15));
        func_801E9118(0, 2, 0, ex->btlfx_getFileData(entry->unk8, 0x14));
        return 0;
    } else {
        frame = entry->unk0;
        func_801E9174(0);
        if (frame >= 0x5A) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

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

s32 fx_1bd_op82(s32 mode, Fx1bdWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    CVECTOR col;
    DrMode2Words *dm;
    s32 frame;
    s32 i;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x6C;
    } else if (mode == 1) {
        work->unk4.vx = 0;
        work->unk4.vy = 0;
        work->unk4.vz = 0;
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unkC.vz = 0x320;
        work->unk1C.vx = 0x1000;
        work->unk1C.vy = 0x1000;
        work->unk1C.vz = 0x1000;
        D_801E9E54 = env;
        work->unk66 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xD), 0x117, 0, 0, 0);
        ex->Hi_SetEffModelOffset(work->unk66, 0x6E);
        work->unk2C.vx = 0;
        work->unk2C.vy = 0;
        work->unk2C.vz = 0;
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0x320;
        work->unk44.vx = 0x1000;
        work->unk44.vy = 0x1000;
        work->unk44.vz = 0x1000;
        work->unk68 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x2B), 0x17, 0x3D00, 0x80, 0xE0);
        D_801EB764.unk10 = 0x60;
        D_801EB764.unk8 = 0;
        D_801EB764.unkA = 0;
        D_801EB764.unkC = 0x1000;
        D_801EB764.unkE = 0x1000;
        D_801EB764.unk12 = 0x30;
        work->unk60 = -1;
        work->unk62 = -1;
        work->unk64 = 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            ex->func_800E6E1C(0x1C0, 0x100, 0x80, 0xA0, D_801EB854, 0, 0x168, 0xF6E);
            break;
        case 22:
            work->unk58.vx = -3;
            work->unk58.vy = 0;
            work->unk58.vz = 0x320;
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&work->unk58);
            break;
        case 21:
            work->unk60 = 4;
            break;
        case 15:
            work->unk62 = 6;
            break;
        case 46:
            ex->Hi_FreeEffModel(work->unk66);
            break;
        case 33:
            ex->Hi_FreeEffModel(work->unk68);
            break;
        }
        if (frame >= 0 && frame < 0x2E) {
            ex->Hi_DrawEffModel(&work->unk4, &work->unkC, (s32)&work->unk1C, work->unk66);
            ex->Hi_ModifyEffModelRGB(work->unk66, 6, 6, 6);
        }
        if (frame >= 21 && frame < 31) {
            D_801EB764.unkA = (frame - 21) * 2;
            D_801EB764.unkA = D_801EB764.unkA >= 10 ? 9 : D_801EB764.unkA;
            if (work->unk60 >= 0) {
                D_801EB764.unkC = func_801E8DA0(0, 0x1000, 4 - work->unk60, 4);
                work->unk60--;
            }
            if (work->unk62 >= 0) {
                D_801EB764.unkC = func_801E8DA0(0x1000, 0, 6 - work->unk62, 6);
                work->unk62--;
            }
            for (i = 0; i < 13; i++) {
                func_801E8E78(&D_801EB764.unk0, func_801E901C((u16 *)ex->btlfx_getFileData(entry->unk8, 0x16), i));
                func_801E77C0(&D_801EB764, D_801EBC54[i], 0x50, D_801EB764.unk8, D_801EB764.unkA, D_801EB764.unk10,
                              D_801EB764.unk12, 0);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x17), 0, NULL, 0, D_801EB764.unkE,
                                    D_801EB764.unkC, 1, D_801EBC54[i], 0);
            }
        }
        if (frame >= 26 && frame < 30) {
            t = frame - 26;
            dm = (DrMode2Words *)env->unk24;
            env->unk24 += sizeof(DrMode2Words);
            setlen(dm, 2);
            dm->code[0] = 0xE6000001;
            dm->code[1] = 0;
            addPrim(&env->unk20[1], dm);
            scale.vx = (t << 7) + 0x1000;
            scale.vy = (t << 7) + 0x1000;
            scale.vz = (t << 7) + 0x1000;
            col.r = col.g = col.b = 0x3C;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->func_800E6764(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, 0);
            env->unk2C->unk5E = 0x208 - t * t * 4;
        }
        work->unk64++;
        return frame >= 0x32;
    }
    return 0;
}

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

u16 *func_801E901C(u16 *base, u16 i) {
    return (s32)base + ((s32 *)base)[(u16)i + 1];
}

void func_801E9034(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E9E54)->btlfx_drawScreenTint((u32 *)(D_801E9E54->unk20 + ot), arg4, c, c, c);
}

void func_801E90CC(void) {
    s32 i;
    s32 m = -1;

    for (i = 63; i >= 0; i--) {
        D_801EC064[i].unk6 = m;
    }
    for (i = 3; i >= 0; i--) {
        D_801EC464[i] = 0;
    }
}

void func_801E9118(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (D_801EC064[i].unk6 == -1) {
            D_801EC064[i].unk6 = 0;
            D_801EC064[i].unk0 = data;
            D_801EC064[i].unk4 = start;
            D_801EC064[i].unkA = dst;
            D_801EC064[i].unkC = 0;
            D_801EC064[i].unkE = c;
            D_801EC064[i].unk8 = ch;
            return;
        }
    }
}

void func_801E9174(s32 ch) {
    Unk800F78A4 *env = D_801E9E54;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (D_801EC064[i].unk8 == ch) {
            if (D_801EC064[i].unk6 == 0 && D_801EC064[i].unk4 == D_801EC464[ch]) {
                D_801EC064[i].unk6 = *D_801EC064[i].unk0;
                D_801EC064[i].unk0++;
            }
            if (D_801EC064[i].unk6 > 0) {
                D_801EC064[i].unk6--;
                for (k = 0; k < 3; k++) {
                    d = ((D_801EC064[i].unkA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->unk2C)[d].vx = D_801EC064[i].unk0[0];
                        ((SVECTOR *)env->unk2C)[d].vy = D_801EC064[i].unk0[1];
                        ((SVECTOR *)env->unk2C)[d].vz = D_801EC064[i].unk0[2];
                        if (D_801EC064[i].unkC < D_801EC064[i].unkE) {
                            D_801EC064[i].unk0 += 3;
                            D_801EC064[i].unkC++;
                        }
                    }
                }
            }
        }
    }
    D_801EC464[ch]++;
}

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
