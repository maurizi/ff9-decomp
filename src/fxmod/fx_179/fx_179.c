#include "fxmod/fx_179.h"

void func_801E7704(s32 model, SVECTOR *center) {
    Fx179Tri *t = D_801E8A1C;
    Fx179Tri *u = D_801E8A1C;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8A0C);
    Fx179Shard *s;
    SVECTOR d;
    s32 i;

    ex->Hi_SplitMdlVertex(model, -1, (s32)D_801E8A1C);
    s = D_801E991C;
    for (i = 0; i < 160; i++, t++, u++, s++) {
        s->unk18 = 0;
        s->unk0 = (t->unk0[0].vx + u->unk0[1].vx + u->unk0[2].vx) / 3 << 12;
        s->unk4 = (u->unk0[0].vy + u->unk0[1].vy + u->unk0[2].vy) / 3 << 12;
        s->unk8 = (u->unk0[0].vz + u->unk0[1].vz + u->unk0[2].vz) / 3 << 12;
        s->unk1A = 0;
        s->unk12 = 0x1000;
        s->unk14 = ex->btlfx_randCentered(0x100);
        s->unk16 = ex->btlfx_randCentered(0x200);
        d.vx = (s->unk0 >> 12) - center->vx;
        d.vy = (s->unk4 >> 12) - center->vy;
        d.vz = (s->unk8 >> 12) - center->vz;
        s->unkC = d.vx >> 4;
        s->unkE = (d.vy >> 4) + 0x80;
        s->unk10 = d.vz >> 4;
    }
}

void func_801E78E8(void) {
    Fx179Tri *t = D_801E8A1C;
    Fx179Shard *s = D_801E991C;
    BtlfxExports *ex;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR loc[3];
    s32 bias[2];
    VECTOR v;
    VECTOR sc;
    MATRIX m;
    s32 i;
    s32 j;

    bias[0] = 0;
    bias[1] = 0;
    ex = BTLFX_EXPORTS(D_801E8A0C);
    sc.vx = sc.vy = sc.vz = 0xF00;
    for (i = 0; i < 160; i++, s++, t++) {
        if (s->unk18 >= 0) {
            if (s->unk18 >= s->unk1A) {
                c.vx = s->unk0 >> 12;
                c.vy = s->unk4 >> 12;
                c.vz = s->unk8 >> 12;
                for (j = 0; j < 3; j++) {
                    loc[j].vx = t->unk0[j].vx - c.vx;
                    loc[j].vy = t->unk0[j].vy - c.vy;
                    loc[j].vz = t->unk0[j].vz - c.vz;
                }
                ex->func_800E0014((Pool6CEntry *)s, -8, (SVECTOR *)bias, &out);
                rot.vx = s->unk14;
                rot.vy = s->unk16;
                rot.vz = 0;
                ex->func_800316D8(&rot, &m);
                ex->ScaleMatrix(&m, &sc);
                m.t[0] = s->unk0 >> 12;
                m.t[1] = s->unk4 >> 12;
                m.t[2] = s->unk8 >> 12;
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
                    s->unk18 = -1;
                }
            } else {
                s->unk18++;
            }
        }
    }
}

INCLUDE_ASM("asm/us/fx_179/nonmatchings/fx_179", func_801E7B68);

s32 fx_179_op80(s32 mode, Fx179Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR from;
    SVECTOR to;
    SVECTOR out;
    union {
        VECTOR v;
        SVECTOR s;
    } u;
    s32 frame;
    s32 a;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk10.vx = env->unkC->unk38;
        work->unk10.vy = env->unkC->unk3A;
        work->unk10.vz = env->unkC->unk3C;
        D_801E8A0C = env;
        ex->btlfx_initEffModels(D_801EAA9C, 1);
        work->unk18 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0x9A, 0x3D40, 0, 0);
    } else {
    frame = entry->unk0;
    switch (work->unk0) {
    case 0:
        a = (frame << 8) >> 2;
        pos.vz = work->unk10.vz + ex->math_cos(a) * 3 / 2;
        pos.vy = -(ex->math_cos(a) * 3 / 2 + 0x180) + work->unk10.vy;
        rot.vx = 0x200;
        rot.vz = -frame << 6;
        pos.vx = 0;
        rot.vy = 0;
        scale.vx = 0xAAA;
        scale.vy = 0xAAA;
        scale.vz = 0xAAA;
        work->unk8.vx = pos.vx;
        work->unk8.vy = pos.vy;
        work->unk8.vz = pos.vz;
        if (frame < 0xB) {
            from.vx = pos.vx;
            from.vy = pos.vy;
            from.vz = pos.vz;
            BTLFX_COPY_SVEC(&to, &work->unk10);
            to.vy -= 0x200;
            ex->btlfx_lerpVec((s16 *)&from, (s16 *)&to, (frame << 12) / 10, (s16 *)&out);
            env->unk2C->unk0[3].vx = out.vx;
            env->unk2C->unk0[3].vy = out.vy;
            env->unk2C->unk0[3].vz = out.vz;
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk18);
        u.v.vx = u.v.vy = u.v.vz = 0xAAA;
        func_801E7B68(&work->unk8, &rot, &u.v, 0x3E8, 0x2BC, 0x300, 0x398, 0x80, 0x17, 0x3D03, 0, 0x40,
                      -(work->unk4 * 0x180), 0x2000, 1, 1);
        func_801E7B68(&work->unk8, &rot, &u.v, 0x3E8, 0x2BC, 0x300, 0x400, 0x80, 0x17, 0x3D00, 0, 0x40,
                      -(work->unk4 * 0x180) + 0x400, 0x2000, 1, 1);
        func_801E7B68(&work->unk8, &rot, &u.v, 0x3E8, 0x2BC, 0x300, 0x300, 0x80, 0x17, 0x3D04, 0, 0x40,
                      -(work->unk4 * 0x180) + 0x800, 0x2000, 1, 1);
        func_801E7B68(&work->unk8, &rot, &u.v, 0x3E8, 0x2BC, 0x300, 0x300, 0x80, 0x17, 0x3D06, 0, 0x40,
                      -(work->unk4 * 0x180) + 0xC00, 0x2000, 1, 1);
        if (frame >= 0x10) {
            work->unk0 = 1;
            entry->unk0 = -1;
            u.s.vx = 0;
            u.s.vy = 0;
            u.s.vz = 0;
            func_801E7704(work->unk18, &u.s);
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
            if (p != NULL) {
                p->unk22 = 0x20;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk8);
            if (p != NULL) {
                p->unk14 = (s32)D_801EAB6C;
                p->unk12 = 0xA;
                p->unkC = 0x5C;
                p->unk22 = 0x20;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x20;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
        }
        break;
    case 1:
        if (frame == 0) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 100, 100, 100);
        }
        if (frame == 1) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 70, 70, 70);
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        scale.vx = 0xAAA;
        scale.vy = 0xAAA;
        scale.vz = 0xAAA;
        pos.vx = work->unk10.vx;
        pos.vy = work->unk10.vy;
        pos.vz = work->unk10.vz;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk18);
        func_801E78E8();
        if (frame >= 0x20) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_179/nonmatchings/fx_179", fx_179_headerPtr);
