#include "fxmod/fx_1b7.h"

void func_801E7704(Fx1b7Split *s, SVECTOR (*faces)[3]) {
    Fx1b7Piece *p;
    s32 i;

    D_801E87E8 = s;
    D_801E87EC = s->unk4C;
    p = s->unk4C;
    s->unk0 = 0;
    s->unk2 = 0;
    s->unk28 = faces;
    s->unk24 = faces;
    for (i = 0; i < 0x6A4; i++) {
        p->unk18 = -1;
        p++;
    }
}

s32 func_801E774C(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E87E0);
    Fx1b7Split *s = D_801E87E8;
    SVECTOR (*f)[3] = s->unk28;
    s32 n;
    s32 i;
    s32 k;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / 24;
    ex->Hi_SplitMdlVertex(model, -1, (s32)f);
    k = s->unk0;
    s->unk2C[k] = f;
    s->unk4[k] = n;
    s->unk28 = f + n;
    s->unk2 += n;
    s->unk0++;
    for (i = 0; i < n; i++) {
        (*f)[0].pad = 0;
        f++;
    }
    return s->unk2;
}

Fx1b7Piece *func_801E7830(void) {
    Fx1b7Piece *p = D_801E87EC;
    s32 i;

    for (i = 0; i < 0x6A4; i++, p++) {
        if (p->unk18 < 0) {
            return p;
        }
    }
    return NULL;
}

s32 func_801E7868(void) {
    Fx1b7Split *s = D_801E87E8;
    SVECTOR (*f)[3] = s->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E87E0);
    s32 t = D_801E87F0;
    SVECTOR d;
    Fx1b7Piece *p;
    s32 j;
    s32 i;
    s32 n;

    j = 0;
    i = 0;
    n = 0;
    for (; i < s->unk2; i++, f++, j++) {
        if (t < (*f)[1].vy && (*f)[0].pad == 0) {
            p = func_801E7830();
            if (p != NULL) {
                (*f)[0].pad = 1;
                p->unk0 = (((*f)[0].vx + (*f)[1].vx + (*f)[2].vx) / 3) << 12;
                p->unk4 = (((*f)[0].vy + (*f)[1].vy + (*f)[2].vy) / 3) << 12;
                p->unk8 = (((*f)[0].vz + (*f)[1].vz + (*f)[2].vz) / 3) << 12;
                p->unk18 = 0;
                p->unk1A = j;
                p->unkC.pad = 0x1000;
                p->unk14 = ex->btlfx_randCentered(0x100);
                p->unk16 = ex->btlfx_randCentered(0x200);
                d.vx = p->unk0 >> 12;
                d.vy = p->unk4 >> 12;
                d.vz = p->unk8 >> 12;
                p->unkC.vx = (d.vx >> 3) + ex->btlfx_randCentered(0x10);
                n++;
                p->unkC.vy = (d.vy >> 5) + (ex->btlfx_rand() & 0xF);
                p->unkC.vz = (d.vz >> 3) + ex->btlfx_randCentered(0x10);
            }
        }
    }
    return n;
}

void func_801E7ACC(void) {
    SVECTOR dir;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR ang;
    SVECTOR v[3];
    s32 zero[2];
    VECTOR o;
    VECTOR scale;
    MATRIX m;
    Fx1b7Split *s;
    Fx1b7Piece *p;
    BtlfxExports *ex;
    SVECTOR (*f)[3];
    s32 i;

    s = D_801E87E8;
    p = D_801E87EC;
    zero[0] = 0;
    zero[1] = 0;
    ex = BTLFX_EXPORTS(D_801E87E0);
    dir.vx = 0;
    dir.vy = 0;
    dir.vz = -0x2BC;
    scale.vx = scale.vy = scale.vz = 0xF80;
    rot.vz = 0;
    for (i = 0; i < 0x6A4; i++, p++) {
        if (p->unk18 >= 0) {
            f = &s->unk24[p->unk1A];
            c.vx = p->unk0 >> 12;
            c.vy = p->unk4 >> 12;
            c.vz = p->unk8 >> 12;
            v[0].vx = (*f)[0].vx - c.vx;
            v[0].vy = (*f)[0].vy - c.vy;
            v[0].vz = (*f)[0].vz - c.vz;
            v[1].vx = (*f)[1].vx - c.vx;
            v[1].vy = (*f)[1].vy - c.vy;
            v[1].vz = (*f)[1].vz - c.vz;
            v[2].vx = (*f)[2].vx - c.vx;
            v[2].vy = (*f)[2].vy - c.vy;
            v[2].vz = (*f)[2].vz - c.vz;
            ex->func_800E0014((Pool6CEntry *)p, 0, (SVECTOR *)zero, &out);
            ex->btlfx_aimAngles((s16 *)&out, (s16 *)&dir, (s16 *)&ang);
            ang.vx = 0;
            ang.vy += 0x400;
            ex->func_800CFCA8(&ang, 0xA0, (s32)&rot);
            p->unk0 += rot.vx << 12;
            p->unk4 += rot.vy << 12;
            p->unk8 += rot.vz << 12;
            *(s32 *)&rot.vx = *(s32 *)&p->unk14;
            ex->func_800316D8(&rot, &m);
            ex->ScaleMatrix(&m, &scale);
            m.t[0] = p->unk0 >> 12;
            m.t[1] = p->unk4 >> 12;
            m.t[2] = p->unk8 >> 12;
            ex->SetRotMatrix(&m);
            ex->SetTransMatrix(&m);
            gte_ldv0(&v[0]);
            gte_rtv0tr();
            gte_stlvnl(&o);
            gte_ldv0(&v[1]);
            gte_rtv0tr();
            (*f)[0].vx = o.vx;
            (*f)[0].vy = o.vy;
            (*f)[0].vz = o.vz;
            gte_stlvnl(&o);
            gte_ldv0(&v[2]);
            gte_rtv0tr();
            (*f)[1].vx = o.vx;
            (*f)[1].vy = o.vy;
            (*f)[1].vz = o.vz;
            gte_stlvnl(&o);
            (*f)[2].vx = o.vx;
            (*f)[2].vy = o.vy;
            (*f)[2].vz = o.vz;
            p->unk18++;
            if (p->unk18 >= 0x20) {
                p->unk18 = -1;
                (*f)[2].vz = 0;
                *(s32 *)&(*f)[2].vx = 0;
                (*f)[1].vz = 0;
                *(s32 *)&(*f)[1].vx = 0;
                (*f)[0].vz = 0;
                *(s32 *)&(*f)[0].vx = 0;
            }
        }
    }
}

INCLUDE_ASM("asm/us/fx_1b7/nonmatchings/fx_1b7", fx_1b7_op80);

s32 fx_1b7_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E87E0 = env;
        ex->func_800E8274(0x10);
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b7/nonmatchings/fx_1b7", fx_1b7_headerPtr);
