#include "fxmod/fx_138.h"

s32 func_801E7704(SVECTOR *out, s32 arg1, s32 i) {
    func_801E77DC(out, &D_801E8830[0], i);
    return 0;
}

s32 func_801E7728(SVECTOR *out, s32 arg1, s32 i) {
    func_801E77DC(out, &D_801E8830[1], i);
    return 0;
}

s32 func_801E774C(SVECTOR *out, s32 arg1, s32 i) {
    func_801E77DC(out, &D_801E8830[2], i);
    return 0;
}

s32 func_801E7770(SVECTOR *out, s32 arg1, s32 i) {
    func_801E77DC(out, &D_801E8830[3], i);
    return 0;
}

s32 func_801E7794(SVECTOR *out, s32 arg1, s32 i) {
    func_801E77DC(out, &D_801E8830[4], i);
    return 0;
}

s32 func_801E77B8(SVECTOR *out, s32 arg1, s32 i) {
    func_801E77DC(out, &D_801E8830[5], i);
    return 0;
}

void func_801E77DC(SVECTOR *out, Fx138Path *path, s32 i) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8500);
    MATRIX m;
    VECTOR v;
    SVECTOR sv;

    func_801E81F0(path, i, &sv);
    sv.vx /= 2;
    sv.vy /= 2;
    sv.vz /= 2;
    ex->func_800316D8(&D_801E8E68, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    gte_ldv0(&sv);
    gte_rtv0tr();
    gte_stlvnl(&v);
    v.vx += D_801E8E60.vx;
    v.vy += D_801E8E60.vy;
    v.vz += D_801E8E60.vz;
    out->vx = v.vx;
    out->vy = v.vy;
    out->vz = v.vz;
}

INCLUDE_ASM("asm/us/fx_138/nonmatchings/fx_138", fx_138_op80);

s32 fx_138_op81(s32 mode, Fx138Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 i;
    u16 n;

    if (mode == 0) {
        *arg4 = 0x34;
    } else if (mode == 1) {
        D_801E8500 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk20);
        work->unk32 = 0x1000;
        work->unk30 = 0;
        work->unk28.vx = work->unk20.vx;
        work->unk28.vy = work->unk20.vy;
        work->unk28.vz = work->unk20.vz;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            for (i = 0; i < 6; i++) {
                work->unk0[i] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801E8488[i]), (s32)&D_801E8E70[i]);
                if (work->unk0[i] != NULL) {
                    work->unk0[i]->unk24->unk0 = -1;
                    *(s32 *)&work->unk0[i]->unk24->unk3C = D_801E84A0[i];
                    if (work->unk0[i] != NULL) {
                        work->unk0[i]->unk22 = ex->func_800E7CC4(0, 0x80);
                    }
                }
            }
            work->unk18 = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2A), (s32)&work->unk20);
            if (p != NULL) {
                p->unk14 = (s32)D_801E8860;
                work->unk18->unk12 = 0x18;
                work->unk18->unkC = 0x40;
                if (work->unk18 != NULL) {
                    work->unk18->unk22 = ex->func_800E7CC4(0, 0x80);
                }
            }
            break;
        case 0xC:
            work->unk1C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk20);
            if (work->unk1C != NULL) {
                work->unk1C->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        }
        if (frame >= 0 && frame < 0xD) {
            ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0xE), &work->unk28, work->unk30, work->unk32);
            if (frame >= 5) {
                work->unk32 -= 0x200;
            }
            work->unk30++;
            n = work->unk30;
            if (n >= 6) {
                n = 5;
            }
            work->unk30 = n;
        }
        return frame >= 0x3C;
    }
    return 0;
}

void func_801E813C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8500)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8180(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E81D8(Fx138Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx138Point *)data;
}

s32 func_801E81F0(Fx138Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx138Point *p;

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

void func_801E8254(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->unk0 + i + 0x10) = v;
}

void func_801E8268(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801E8500;
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

INCLUDE_RODATA("asm/us/fx_138/nonmatchings/fx_138", fx_138_headerPtr);
