#include "fxmod/fx_1ea.h"

void func_801E7804(Fx1eaPath *path, Fx1eaTrail *trail, s32 size, s32 base, s32 start, s32 pad) {
    SVECTOR v;
    SVECTOR *p;
    s32 n;
    s32 i;

    n = (size - 4) / 8;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
    if (start < n) {
        for (i = 0; i < n - start; i++) {
            p->pad = 0;
            p++;
        }
    } else {
        start = n;
    }
    for (i = 0; i < start;) {
        i++;
        func_801E8C4C(path, start - i + base, &v);
        v.vx += v.vx / 10;
        v.vy += v.vy / 10;
        v.vz += v.vz / 10;
        v.vx += D_801E9B58.vx;
        v.vy += D_801E9B58.vy;
        v.vz += D_801E9B58.vz;
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
        p->pad = pad;
        p++;
    }
}

void func_801E79D0(Fx1eaPath *path, Fx1eaTrail *trail, s32 size, s32 base, s32 start, s32 pad, s32 ang) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8DD8);
    MATRIX m;
    VECTOR o;
    SVECTOR rot;
    SVECTOR v;
    SVECTOR *p;
    s32 n;
    s32 i;

    n = (size - 4) / 8;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
    if (start < n) {
        for (i = 0; i < n - start; i++) {
            p->pad = 0;
            p++;
        }
    } else {
        start = n;
    }
    for (i = 0; i < start;) {
        i++;
        func_801E8C4C(path, start - i + base, &v);
        rot.vx = 0;
        rot.vy = ang;
        rot.vz = 0;
        v.vx += v.vx / 10;
        v.vy += v.vy / 10;
        v.vz += v.vz / 10;
        ex->func_800316D8(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(&o);
        o.vx += D_801E9B58.vx;
        o.vy += D_801E9B58.vy;
        o.vz += D_801E9B58.vz;
        p->vx = o.vx;
        p->vy = o.vy;
        p->vz = o.vz;
        p->pad = pad;
        p++;
    }
}

s32 fx_1ea_op80(s32 mode, Fx1eaWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E8DD8 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk10);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk8);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        s32 frame = entry->unk0;

        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk10.vx;
                work->unk0->unk5C.vy = work->unk10.vy;
                work->unk0->unk5C.vz = work->unk10.vz;
            }
        }
        if (work->unk4 != NULL) {
            if (work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk10.vx;
                work->unk4->unk5C.vy = work->unk10.vy;
                work->unk4->unk5C.vz = work->unk10.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ea/nonmatchings/fx_1ea", fx_1ea_headerPtr);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", fx_1ea_op81);

s32 fx_1ea_op82(s32 mode, Fx1eaWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801E8DD8 = env;
        work->unk2 = -1;
        work->unk0 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x0:
            work->unk0 = 0x10;
            break;
        case 0x33:
            work->unk2 = 0x10;
            break;
        }
        if (work->unk0 >= 0) {
            func_801E8B9C(0, 0xFF, 0x10 - work->unk0, 0x10, 1, 0);
            work->unk0--;
        } else if (work->unk2 >= 0) {
            func_801E8B9C(0xFF, 0, 0x10 - work->unk2, 0x10, 1, 0);
            work->unk2--;
        } else {
            ex->btlfx_drawScreenTint((u32 *)env->unk20 + 0xFFF, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x43;
    }
    return 0;
}

void func_801E8B00(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8DD8)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8B44(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8B9C(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E8DD8)->btlfx_drawScreenTint((u32 *)(D_801E8DD8->unk20 + ot), arg4, c, c, c);
}

void func_801E8C34(Fx1eaPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1eaPoint *)data;
}

s32 func_801E8C4C(Fx1eaPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1eaPoint *p;

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

void func_801E8CB0(Fx1eaPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1eaPoint *)data;
}

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", func_801E8CC8);
