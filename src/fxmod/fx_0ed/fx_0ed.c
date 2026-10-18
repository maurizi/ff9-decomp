#include "fxmod/fx_0ed.h"

void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx0edSeg *buf, s32 abr) {
    Unk800F78A4 *env = D_801E9224;
    CVECTOR c0;
    CVECTOR c1;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx0edSeg *s;
    u32 i;
    s32 p0;
    s32 p1;

    if (p < 0) {
        p = 0x1000;
    }
    if (n >= 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].unk0.pad = 0;
        }
        return;
    }
    s = &buf[(s32)(n - 1)];
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&s[1].unk0, &s[0].unk0);
        BTLFX_COPY_SVEC(&s[1].unk8, &s[0].unk8);
        s--;
    }
    s = buf;
    for (i = 0; i < n; i++, s++) {
        if (!(s[0].unk0.pad & s[1].unk0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->unk0, a);
    buf->unk0.pad = 1;
    BTLFX_COPY_SVEC(&buf->unk8, b);
    n = i;
    prim = (POLY_G4 *)env->unk24;
    env->unk24 = (s32)(prim + n);
    if (ca == NULL) {
        c0.r = c0.g = c0.b = 0;
    } else {
        gte_DpqColorFar_nst(ca, p);
        gte_strgb(&c0);
    }
    if (cb == NULL) {
        c1.r = c1.g = c1.b = 0;
    } else {
        gte_DpqColorFar_nst(cb, p);
        gte_strgb(&c1);
    }
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    c0.cd = abr != 0xFF ? 0x3A : 0x38;
    c1.cd = abr != 0xFF ? 0x3A : 0x38;
    for (i = 0; i < n; i++, buf++, prim++) {
        gte_ldv3c(buf);
        p0 = 0x1000 - (i << 12) / n;
        gte_rtpt_b();
        p1 = 0x1000 - ((i + 1) << 12) / n;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].unk8);
            gte_rtps();
            ex->btlfx_addPrimAbr(0x8000000, otz, prim, abr);
            gte_stsxy(&prim->x3);
            gte_DpqColorFar_nst(&c0, p0);
            gte_strgb(&prim->r0);
            gte_DpqColorFar_nst(&c1, p0);
            gte_strgb(&prim->r1);
            gte_DpqColorFar_nst(&c0, p1);
            gte_strgb(&prim->r2);
            gte_DpqColorFar_nst(&c1, p1);
            gte_strgb(&prim->r3);
        }
    }
}

void func_801E7B74(s32 model, s32 bone, SVECTOR *in, SVECTOR *out) {
    Unk800F78A4 *env = D_801E9224;
    MATRIX m;

    BTLFX_EXPORTS(env)->Hi_GetSummonBoneMatrix(model, bone, (BtlBone *)&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    gte_ldv0(in);
    gte_rtv0tr();
    gte_stlvnl(m.t);
    out->vx = m.t[0];
    out->vy = m.t[1];
    out->vz = m.t[2];
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
}

INCLUDE_ASM("asm/us/fx_0ed/nonmatchings/fx_0ed", fx_0ed_op80);

s32 fx_0ed_op81(s32 mode, Fx0edWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    union {
        SVECTOR v;
        CVECTOR c;
    } u;
    SVECTOR rot;
    VECTOR sc;
    s32 tmp;
    s32 frame;
    s32 r;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        work->unk4 = 0xA;
        work->unk0 = 0;
        work->unkC = (BtlUnit *)-1;
        work->unk8 = 0;
        D_801E9224 = env;
        ((Fx0edBattleState *)env->unk0)->unk10 = 0;
    } else {
        frame = entry->unk0;
        c = 0;
        switch (work->unk0) {
        case 0:
            do {
                r = ex->func_800E82CC(0, (s16 *)&work->unk10, &work->unkC, &tmp);
                if (r == -2) {
                    work->unkC = (BtlUnit *)-1;
                }
            } while (r == -2 || r == -1);
            env->unk2C->unk0[0].vx = work->unk10.vx;
            env->unk2C->unk0[0].vy = work->unk10.vy;
            env->unk2C->unk0[0].vz = work->unk10.vz;
            u.v.vx = work->unk10.vx;
            u.v.vy = work->unk10.vy;
            u.v.vz = work->unk10.vz;
            if (u.v.vz < 0) {
                u.v.vz += 0x640;
            } else {
                u.v.vz -= 0x640;
            }
            u.v.vy -= 0x200;
            env->unk2C->unk0[1].vx = u.v.vx;
            env->unk2C->unk0[1].vy = u.v.vy;
            env->unk2C->unk0[1].vz = u.v.vz;
            ex->func_800E9930(3, entry->unk8, -1);
            if (++work->unk8 >= 0x10 && r == 1) {
                work->unk0 = 2;
            } else {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            if (work->unk8 < 0x10) {
                c = work->unk8 * 100 / 16;
            } else {
                c = 100;
            }
            break;
        case 1:
            env->unk28 = 0x80;
            ex->func_800D198C((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &work->unk10, 0, 0xFF, -1);
            env->unk28 = 0;
            if (work->unk8 < 0x10) {
                c = work->unk8 * 100 / 16;
            } else {
                c = 100;
            }
            if (frame >= work->unk4) {
                work->unk0 = 0;
                work->unk4--;
            }
            break;
        case 2:
            ((Fx0edBattleState *)env->unk0)->unk10 = 1;
            env->unk2C->unk52 = 0;
            return 1;
        }
        if (c != 0) {
            sc.vx = 0x1000;
            sc.vy = 0x1000;
            sc.vz = 0x1000;
            u.c.r = u.c.g = u.c.b = c;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->func_800E6764(6, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &sc, &u.c, 1, 0);
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ed/nonmatchings/fx_0ed", fx_0ed_headerPtr);
