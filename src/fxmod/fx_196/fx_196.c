#include "fxmod/fx_196.h"

void func_801E7704(Fx196Track *track, s32 t, SVECTOR *out) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    track = (Fx196Track *)&track->unk4[t * 3];
    out->vx = ((s16 *)track)[0];
    out->vy = ((s16 *)track)[1];
    out->vz = ((s16 *)track)[2];
}

void func_801E7754(Fx196Track *track, s32 t, SVECTOR *a, SVECTOR *b) {
    s16 *k;

    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    k = &track->unk4[t * 6];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
}

void func_801E77DC(Fx196Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c) {
    s16 *k;

    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    k = &track->unk4[t * 9];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
    if (c != NULL) {
        c->vx = k[6];
        c->vy = k[7];
        c->vz = k[8];
    }
}

s32 func_801E7890(Fx196Track *track, s32 t) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    return (track->unk4 + t)[0];
}

void func_801E78BC(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801E915C;
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

s32 func_801E7A5C(s32 a) {
    s32 v = BTLFX_EXPORTS(D_801E915C)->math_sin(a);

    if (v < 0) {
        v >>= 1;
    }
    return v;
}

INCLUDE_ASM("asm/us/fx_196/nonmatchings/fx_196", func_801E7AA0);

s32 func_801E8370(SVECTOR *v) {
    s32 i = D_801E90F8;

    v->pad = 1;
    v->vx = D_801E916C[i].vx;
    v->vy = D_801E916C[i].vy;
    v->vz = D_801E916C[i].vz;
    i++;
    if (i >= 16) {
        i = 0;
    }
    D_801E90F8 = i;
    return 0;
}

INCLUDE_ASM("asm/us/fx_196/nonmatchings/fx_196", fx_196_op85);

s32 fx_196_op86(s32 mode, Fx196Work86 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x44;
    } else if (mode == 1) {
        D_801E915C = env;
        work->unk40 = env->unkC->unk24;
        for (i = 0; i < work->unk40; i++) {
            if (ex->btlfx_isEnemyUnit(i) != 0) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk0[i]);
                work->unk0[i].vz -= ex->btlfx_getUnitRadius(i);
                work->unk0[i].vy -= 0x200;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk0[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x200);
                }
            }
        }
    } else {
        if (entry->unk0 >= 0x2E) {
            return 1;
        }
    }
    return 0;
}

s32 fx_196_op87(s32 mode, s32 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;
    s32 n;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        n = *arg4;
        D_801E915C = env;
        *work = n;
    } else {
        frame = entry->unk0;
        func_801E7704((Fx196Track *)ex->btlfx_getFileData(entry->unk8, D_801E910C[*work].unk2), frame, &v);
        env->unk2C->unk0[1].vx = v.vx;
        env->unk2C->unk0[1].vy = v.vy;
        env->unk2C->unk0[1].vz = v.vz;
        func_801E7704((Fx196Track *)ex->btlfx_getFileData(entry->unk8, D_801E910C[*work].unk0), frame, &v);
        env->unk2C->unk0[0].vx = v.vx;
        env->unk2C->unk0[0].vy = v.vy;
        env->unk2C->unk0[0].vz = v.vz;
        if (frame >= D_801E910C[*work].unk4) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_196/nonmatchings/fx_196", fx_196_headerPtr);
