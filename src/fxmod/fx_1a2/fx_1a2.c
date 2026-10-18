#include "fxmod/fx_1a2.h"

void func_801E7718(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx1a2Seg *buf, s32 abr) {
    Unk800F78A4 *env = D_801E9844;
    CVECTOR c0;
    CVECTOR c1;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx1a2Seg *s;
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

void func_801E7B88(s32 model, SVECTOR *center) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9844);
    SVECTOR v;
    Fx1a2Tri *t;
    Fx1a2Piece *p;
    s32 i;

    ex->Hi_SplitMdlVertex(model, -1, (s32)D_801E9854);
    t = D_801E9854;
    p = D_801EA484;
    for (i = 0; i < 130; i++, t++, p++) {
        p->unk18 = 0;
        p->unk0 = ((t->unk0[0].vx + t->unk0[1].vx + t->unk0[2].vx) / 3) << 12;
        p->unk4 = ((t->unk0[0].vy + t->unk0[1].vy + t->unk0[2].vy) / 3) << 12;
        p->unk8 = ((t->unk0[0].vz + t->unk0[1].vz + t->unk0[2].vz) / 3) << 12;
        p->unk1A = 0;
        p->unk12 = 0x1000;
        p->unk14 = ex->btlfx_randCentered(0x100);
        p->unk16 = ex->btlfx_randCentered(0x200);
        v.vx = (p->unk0 >> 12) - center->vx;
        v.vy = (p->unk4 >> 12) - center->vy;
        v.vz = (p->unk8 >> 12) - center->vz;
        p->unkC = v.vx >> 2;
        p->unkE = v.vy >> 3;
        p->unk10 = v.vz >> 2;
    }
}

void func_801E7D68(void) {
    SVECTOR out;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR v[3];
    SVECTOR bias;
    VECTOR o;
    VECTOR scale;
    MATRIX m;
    BtlfxExports *ex;
    Fx1a2Tri *t;
    Fx1a2Piece *p;
    s32 i;
    s32 k;

    t = D_801E9854;
    p = D_801EA484;
    ((s32 *)&bias)[0] = 0;
    ((s32 *)&bias)[1] = 0;
    ex = BTLFX_EXPORTS(D_801E9844);
    scale.vx = scale.vy = scale.vz = 0xF80;
    for (i = 0; i < 130; i++, p++, t++) {
        if (p->unk18 >= 0) {
            if (p->unk18 < p->unk1A) {
                p->unk18++;
            } else {
                c.vx = p->unk0 >> 12;
                c.vy = p->unk4 >> 12;
                c.vz = p->unk8 >> 12;
                for (k = 0; k < 3; k++) {
                    v[k].vx = t->unk0[k].vx - c.vx;
                    v[k].vy = t->unk0[k].vy - c.vy;
                    v[k].vz = t->unk0[k].vz - c.vz;
                }
                if (p->unk18 >= 3) {
                    ex->func_800E0014((Pool6CEntry *)p, -3, &bias, &out);
                    if (p->unkE < 0 && p->unk4 < 0) {
                        p->unkE = -(p->unkE >> 1);
                    }
                }
                rot.vx = p->unk14;
                rot.vy = p->unk16;
                rot.vz = 0;
                ex->func_800316D8(&rot, &m);
                ex->ScaleMatrix(&m, &scale);
                m.t[0] = p->unk0 >> 12;
                m.t[1] = p->unk4 >> 12;
                m.t[2] = p->unk8 >> 12;
                ex->SetRotMatrix(&m);
                ex->SetTransMatrix(&m);
                for (k = 0; k < 3; k++) {
                    gte_ldv0(&v[k]);
                    gte_rtv0tr();
                    gte_stlvnl(&o);
                    t->unk0[k].vx = o.vx;
                    t->unk0[k].vy = o.vy;
                    t->unk0[k].vz = o.vz;
                }
                if (++p->unk18 >= 0x40) {
                    p->unk18 = -1;
                    t->unk0[2].vz = 0;
                    *(s32 *)&t->unk0[2] = 0;
                    t->unk0[1].vz = 0;
                    *(s32 *)&t->unk0[1] = 0;
                    t->unk0[0].vz = 0;
                    *(s32 *)&t->unk0[0] = 0;
                }
            }
        }
    }
}

s32 func_801E804C(s32 mode, Fx1a2Rock *o, Unk800F78A4 *env, s32 arg3, s32 frame, s32 arg5, Fx1a2Inst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    VECTOR pos;
    SVECTOR rot;
    s32 t;
    s32 d;
    s32 y;

    if (mode == 1) {
        o->unk0 = 0;
        o->unk4 = (ex->btlfx_rand() & 0x7F) + 100;
    } else {
    d = 0;
    switch (o->unk8) {
    case 0:
        t = frame << 9;
        ex->func_800CFD78((s16 *)&o->unkC, (s16 *)&o->unk14, (o->unk1C << 5) >> 12, (s16 *)&o->unkC);
        pos.vx = o->unkC.vx;
        pos.vy = o->unkC.vy;
        pos.vz = o->unkC.vz;
        pos.vy -= (o->unk2 * (0x1000 - t)) >> 12;
        y = t >> 1; /* a fresh single-set temporary (sched1 birthing boost) */
        pos.vy -= (ex->math_sin(y) * o->unk4) >> 12;
        d = -(ex->math_sin(y) >> 2);
        if (frame >= 8) {
            o->unk8 = 1;
            ((s16 *)o)[-1] = -1;
            o->unk2 = 0;
            o->unk6 = (ex->btlfx_rand() & 3) + 1;
        }
        break;
    case 1:
        pos.vx = o->unkC.vx;
        pos.vy = o->unkC.vy;
        pos.vz = o->unkC.vz;
        pos.vy -= (ex->btlfx_rand() & 0xF) + o->unk2;
        if (frame >= o->unk6) {
            o->unk8 = 0;
            ((s16 *)o)[-1] = -1;
            if (o->unk1C < 0x1800) {
                o->unk14.vy += ex->btlfx_randCentered(0x400);
            }
            o->unk4 = ex->btlfx_rand() % 160 + 0x118;
            if (++o->unk0 >= 10) {
                return 1;
            }
        }
        break;
    }
    rot.vx = o->unk14.vx;
    rot.vy = o->unk14.vy;
    rot.vz = o->unk14.vz;
    rot.vx += 0x400;
    rot.vx += d;
    rot.vy += 0x800;
    scale.vx = scale.vy = scale.vz = o->unk1C;
    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, inst->unk48);
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1a2/nonmatchings/fx_1a2", fx_1a2_headerPtr);

INCLUDE_ASM("asm/us/fx_1a2/nonmatchings/fx_1a2", fx_1a2_op80);

s32 fx_1a2_op81(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 a;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E9844 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)work);
        return 0;
    } else {
        frame = entry->unk0;
        work->vz += 0x60;
        ex->btlfx_setUnitPos(0x10, (s16 *)work);
        if (frame < 0x24) {
            a = 0x80;
        } else {
            a = 0x80 - (frame - 0x24) * 0x10;
        }
        ex->func_800E806C(0x10, a);
        if (frame >= 0x2C) {
            return 1;
        }
    }
    return 0;
}
