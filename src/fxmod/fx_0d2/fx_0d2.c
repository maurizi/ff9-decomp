#include "fxmod/fx_0d2.h"

void func_801E77A4(Fx0d2Keys *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip) {
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
        func_801E9E88(keys, j, &v);
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
        p->pad = from + (to - from) / keys->unk2 * (n - i);
        p++;
    }
}

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op80);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op81);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op82);

INCLUDE_RODATA("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_headerPtr);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op83);

s32 fx_0d2_op84(s32 mode, Fx0d2Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    Pool6CEntry *e;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d2Work84);
    } else if (mode == 1) {
        D_801ED8E0 = env;
        for (j = 0; j < env->unkC->unk24; j++) {
            work->unkA0[j] = 0;
            ex->btlfx_getUnitHomePos(j, (s16 *)&work->unk20[j]);
            work->unk20[j].vy = -3000;
            ex->btlfx_setUnitPos(j, (s16 *)&work->unk20[j]);
        }
    } else {
        frame = entry->unk0;
        for (i = 0; i < env->unkC->unk24; i++) {
            if (work->unkA0[i] == 0 && func_801EA108(i, frame, 4, &work->unk20[i]) != 0) {
                work->unkA0[i] = 1;
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk20[i]);
                ex->btlfx_setUnitPos(i, (s16 *)&work->unk20[i]);
                /* PsyQ copyVector() */
                (&work->unk60[i])->vx = (&work->unk20[i])->vx, (&work->unk60[i])->vy = (&work->unk20[i])->vy,
                (&work->unk60[i])->vz = (&work->unk20[i])->vz;
                e = ex->func_800E3CEC((s32)ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk60[i]);
                work->unk0[i] = e;
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_0d2_op85(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    s32 frame;

    if (mode == 0) {
        *arg4 = 2;
    } else if (mode == 1) {
        D_801ED8E0 = env;
        *work = -1;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            *work = 0x10;
        }
        if (*work >= 0) {
            func_801EA070(0xFF, 0, 0x10 - *work, 0x10, 2, 0);
            (*work)--;
        }
        return frame >= 0x10;
    }
    return 0;
}

s32 func_801E9DBC(u8 *base, u16 i) {
    return (s32)base + ((s32 *)base)[(u16)i + 1];
}

void func_801E9DD4(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801ED8E0)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E9E18(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E9E70(Fx0d2Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0d2Point *)data;
}

s32 func_801E9E88(Fx0d2Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx0d2Point *p;

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

void func_801E9EEC(Fx0d2Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0d2Point *)data;
}

s32 func_801E9F04(Fx0d2KeyPath *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx0d2Key *k;
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

void func_801E9F98(Fx0d2Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0d2Point *)data;
}

s32 func_801E9FB0(Fx0d2Keys *keys, s32 i, s32 *out) {
    s32 r = 1;

    if (i > keys->unk2 - 1) {
        i = keys->unk2 - 1;
    } else {
        r = 0;
    }
    *out = *(s16 *)((u8 *)keys->unk4 + (i << 1));
    return r;
}

void func_801E9FF4(Fx0d2Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0d2Point *)data;
}

s32 func_801EA00C(Fx0d2Path *track, s32 t, VECTOR *out) {
    s32 last = track->unk2 - 1;
    s32 done = 1;
    Fx0d2Point *k;

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

void func_801EA070(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801ED8E0)->btlfx_drawScreenTint((u32 *)(D_801ED8E0->unk20 + ot), arg4, c, c, c);
}

s32 func_801EA108(s32 a0, s32 t, s32 n, SVECTOR *b) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801ED8E0);
    SVECTOR a;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(a0, (s16 *)&a);
    func_801E9DD4(b, &a, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(a0, (s16 *)&out);
    return t >= n;
}

void func_801EA1D4(void) {
    s32 i;
    s32 m = -1;

    for (i = 63; i >= 0; i--) {
        D_801F0060[i].unk6 = m;
    }
    for (i = 3; i >= 0; i--) {
        D_801F0460[i] = 0;
    }
}

void func_801EA220(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (D_801F0060[i].unk6 == -1) {
            D_801F0060[i].unk6 = 0;
            D_801F0060[i].unk0 = data;
            D_801F0060[i].unk4 = start;
            D_801F0060[i].unkA = dst;
            D_801F0060[i].unkC = 0;
            D_801F0060[i].unkE = c;
            D_801F0060[i].unk8 = ch;
            return;
        }
    }
}

void func_801EA27C(s32 ch) {
    Unk800F78A4 *env = D_801ED8E0;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (D_801F0060[i].unk8 == ch) {
            if (D_801F0060[i].unk6 == 0 && D_801F0060[i].unk4 == D_801F0460[ch]) {
                D_801F0060[i].unk6 = *D_801F0060[i].unk0;
                D_801F0060[i].unk0++;
            }
            if (D_801F0060[i].unk6 > 0) {
                D_801F0060[i].unk6--;
                for (k = 0; k < 3; k++) {
                    d = ((D_801F0060[i].unkA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->unk2C)[d].vx = D_801F0060[i].unk0[0];
                        ((SVECTOR *)env->unk2C)[d].vy = D_801F0060[i].unk0[1];
                        ((SVECTOR *)env->unk2C)[d].vz = D_801F0060[i].unk0[2];
                        if (D_801F0060[i].unkC < D_801F0060[i].unkE) {
                            D_801F0060[i].unk0 += 3;
                            D_801F0060[i].unkC++;
                        }
                    }
                }
            }
        }
    }
    D_801F0460[ch]++;
}
