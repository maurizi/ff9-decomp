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

INCLUDE_ASM("asm/us/fx_179/nonmatchings/fx_179", fx_179_op80);

INCLUDE_RODATA("asm/us/fx_179/nonmatchings/fx_179", fx_179_headerPtr);
