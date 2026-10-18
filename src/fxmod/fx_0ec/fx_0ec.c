#include "fxmod/fx_0ec.h"

s32 func_801E7704(s32 mode, SVECTOR *pos, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 t;
    u8 *d;

    if (mode != 1) {
        t = (frame << 12) / 6;
        rot.vx = 0x400;
        rot.vy = 0;
        rot.vz = 0;
        rot.pad = 1;
        d = (u8 *)ex->btlfx_getFileData(entry->unk8, 6);
        ex->btlfx_drawParticles(d, pos, &rot, (ex->math_sin(t >> 2) >> 1) + 0x400, 0, 1, 0x1000 - t, 0, 0);
        if (frame >= 6) {
            return 1;
        }
    }
    return 0;
}

void func_801E77FC(SVECTOR *pos) {
    Unk800F78A4 *env = D_801E98A0;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *p = ex->btlfx_poolSpawn((s16 *)D_801E98A4->unk10);
    MATRIX m;
    VECTOR v;

    if (p != NULL) {
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        ex->func_800316D8(&D_801E98A8, &m);
        ex->ScaleMatrix(&m, &D_801E98C0);
        m.t[0] = D_801E98B0.vx;
        m.t[1] = D_801E98B0.vy;
        m.t[2] = D_801E98B0.vz;
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(pos);
        gte_rtv0tr();
        gte_stlvnl(&v);
        v.vy = 0;
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
    }
}

INCLUDE_ASM("asm/us/fx_0ec/nonmatchings/fx_0ec", func_801E798C);

INCLUDE_ASM("asm/us/fx_0ec/nonmatchings/fx_0ec", func_801E7B84);

s32 func_801E7F00(u8 *buf) {
    Fx0ecShard *p = D_801ECAB0;
    s32 i;

    D_801ECA90.unk0 = 0;
    D_801ECA90.unk2 = 0;
    D_801ECA90.unk10 = buf;
    D_801ECA90.unkC = buf;
    for (i = 0; i < 300; i++) {
        p->unk18 = -1;
        p++;
    }
}

s32 func_801E7F40(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E98A0);
    Fx0ecSplit *sp = &D_801ECA90;
    Fx0ecVertex *v = (Fx0ecVertex *)sp->unk10;
    s32 n;
    s32 i;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / sizeof(Fx0ecVertex);
    ex->Hi_SplitMdlVertex(model, -1, (s32)v);
    {
        s32 k = D_801ECA90.unk0;
        sp->unk14[k] = (u8 *)v;
        sp->unk4[k] = n;
    }
    sp->unk10 = (u8 *)(v + n);
    sp->unk2 += n;
    D_801ECA90.unk0++;
    for (i = 0; i < n; i++) {
        v->unk6 = 0;
        v++;
    }
    return sp->unk2;
}

Fx0ecShard *func_801E802C(void) {
    Fx0ecShard *p = D_801ECAB0;
    s32 i;

    for (i = 0; i < 300; i++) {
        if (p->unk18 < 0) {
            return p;
        }
        p++;
    }
    return NULL;
}

INCLUDE_ASM("asm/us/fx_0ec/nonmatchings/fx_0ec", func_801E8064);

void func_801E82D8(void) {
    Fx0ecSplit *sp = &D_801ECA90;
    Fx0ecShard *s = D_801ECAB0;
    BtlfxExports *ex;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR loc[3];
    s32 bias[2];
    VECTOR v;
    VECTOR sc;
    MATRIX m;
    Fx0ecTri *t;
    s32 i;
    s32 j;

    bias[0] = 0;
    bias[1] = 0;
    ex = BTLFX_EXPORTS(D_801E98A0);
    sc.vx = sc.vy = sc.vz = 0xEC0;
    for (i = 0; i < 300; i++, s++) {
        if (s->unk18 >= 0) {
            t = (Fx0ecTri *)sp->unkC + s->unk1A;
            c.vx = s->unk0.vx >> 12;
            c.vy = s->unk0.vy >> 12;
            c.vz = s->unk0.vz >> 12;
            for (j = 0; j < 3; j++) {
                loc[j].vx = t->unk0[j].vx - c.vx;
                loc[j].vy = t->unk0[j].vy - c.vy;
                loc[j].vz = t->unk0[j].vz - c.vz;
            }
            ex->func_800E0014((Pool6CEntry *)s, -10, (SVECTOR *)bias, &out);
            if (s->unkC.vy < 0 && s->unk0.vy < 0) {
                func_801E77FC(&c);
                goto kill;
            }
            rot.vx = s->unk14;
            rot.vy = s->unk16;
            rot.vz = 0;
            ex->func_800316D8(&rot, &m);
            ex->ScaleMatrix(&m, &sc);
            m.t[0] = s->unk0.vx >> 12;
            m.t[1] = s->unk0.vy >> 12;
            m.t[2] = s->unk0.vz >> 12;
            ex->SetRotMatrix(&m);
            ex->SetTransMatrix(&m);
            for (j = 0; j < 3; j++) {
                gte_ldv0(&loc[j]);
                gte_rtv0tr();
                gte_stlvnl(&v);
                t->unk0[j].vx = v.vx;
                t->unk0[j].vy = v.vy;
                t->unk0[j].vz = v.vz;
            }
            if (++s->unk18 >= 0x20) {
            kill:
                s->unk18 = -1;
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

INCLUDE_ASM("asm/us/fx_0ec/nonmatchings/fx_0ec", fx_0ec_op80);

INCLUDE_RODATA("asm/us/fx_0ec/nonmatchings/fx_0ec", fx_0ec_headerPtr);
