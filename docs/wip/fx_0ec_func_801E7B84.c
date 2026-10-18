/* Draft of func_801E7B84 (module fx_0ec). Status: 206/223 aligned instructions with ccprobe; only the t (triangle pointer) and ex registers are swapped (s3/s4).
 * Declarations it needs that are not yet in include/fxmod/fx_0ec.h are repeated below. */
#include "fxmod/fx_0ec.h"

void func_801E7B84(void) {
    Fx0ecShard *s = D_801EBE50;
    Fx0ecTri *t;
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
    ex = BTLFX_EXPORTS(D_801E98A0);
    sc.vx = sc.vy = sc.vz = 0xF80;
    for (t = D_801EB3D0, i = 0; i < 0x70; i++, s++, t++) {
        if (s->unk18 >= 0) {
            if (s->unk18 < s->unk1A) {
                s->unk18++;
            } else {
                c.vx = s->unk0.vx >> 12;
                c.vy = s->unk0.vy >> 12;
                c.vz = s->unk0.vz >> 12;
                for (j = 0; j < 3; j++) {
                    loc[j].vx = t->unk0[j].vx - c.vx;
                    loc[j].vy = t->unk0[j].vy - c.vy;
                    loc[j].vz = t->unk0[j].vz - c.vz;
                }
                if (s->unk18 >= 8) {
                    ex->func_800E0014((Pool6CEntry *)s, -10, (SVECTOR *)bias, &out);
                    if (s->unkC.vy < 0 && s->unk0.vy < 0) {
                        s->unkC.vy = -(s->unkC.vy >> 1);
                        if (s->unkC.vy < 0x3C) {
                            func_801E77FC(&c);
                            goto kill;
                        }
                    }
                }
                if (s->unk18 == ex->btlfx_rand() % 3 + 4 || s->unk18 >= 8) {
                    rot.vx = s->unk14;
                    rot.vy = s->unk16;
                    rot.vz = 0;
                } else {
                    rot.vx = 0;
                    rot.vy = 0;
                    rot.vz = 0;
                }
                ex->func_800316D8(&rot, &m);
                if (s->unk18 >= 8) {
                    ex->ScaleMatrix(&m, &sc);
                }
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
                if (++s->unk18 >= 0x47) {
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
}
