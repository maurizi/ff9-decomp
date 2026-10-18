#include "fxmod/fx_0e2.h"

void func_801E77C0(void *trackA, void *trackB, s32 t, Fx0e2Trail *trail, s32 size, s32 pad) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801ECC0C);
    SVECTOR rot;
    SVECTOR pos;
    SVECTOR rotA;
    SVECTOR rotB;
    SVECTOR posA;
    SVECTOR posB;
    SVECTOR *pt;
    s32 n;
    s32 i;
    s32 r;

    n = (size - 4) / 8;
    pt = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
    r = t;
    for (i = 0; i < n; i++, pt++) {
        func_801EC520(trackA, i, &rotA, &posA);
        func_801EC520(trackB, i, &rotB, &posB);
        ex->btlfx_lerpVec((s16 *)&posA, (s16 *)&posB, r, (s16 *)&pos);
        ex->btlfx_lerpVec((s16 *)&rotA, (s16 *)&rotB, r, (s16 *)&rot);
        pos.vx /= 2;
        pos.vy /= 2;
        pos.vz /= 2;
        pos.vx += D_801F0FEC.vx;
        pos.vy += D_801F0FEC.vy;
        pos.vz += D_801F0FEC.vz;
        pos.vx += 0x50;
        pos.vy -= 0x109;
        pos.vz += 0xE6;
        pt->vx = pos.vx;
        pt->vy = pos.vy;
        pt->vz = pos.vz;
        if (i == n - 1) {
            D_801F0FF4.vx = pos.vx;
            D_801F0FF4.vy = pos.vy;
            D_801F0FF4.vz = pos.vz;
            D_801F0FFC.vx = rot.vx;
            D_801F0FFC.vy = rot.vy;
            D_801F0FFC.vz = rot.vz;
            D_801F0FFC.vy += 0x800;
        }
        pt->pad = pad;
    }
}

void func_801E7A0C(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 pad) {
    SVECTOR rot;
    SVECTOR pos;
    SVECTOR *pt;
    s32 n;
    s32 i;

    n = (size - 4) / 8;
    pt = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
    for (i = 0; i < n; i++, pt++) {
        func_801EC520(track, i + base, &rot, &pos);
        pos.vx /= 2;
        pos.vy /= 2;
        pos.vz /= 2;
        pos.vx += D_801F0FEC.vx;
        pos.vy += D_801F0FEC.vy;
        pos.vz += D_801F0FEC.vz;
        pos.vx += 0x50;
        pos.vy -= 0x109;
        pos.vz += 0xE6;
        pt->vx = pos.vx;
        pt->vy = pos.vy;
        pt->vz = pos.vz;
        if (i == n - 1) {
            D_801F0FF4.vx = pos.vx;
            D_801F0FF4.vy = pos.vy;
            D_801F0FF4.vz = pos.vz;
            D_801F0FFC.vx = rot.vx;
            D_801F0FFC.vy = rot.vy;
            D_801F0FFC.vz = rot.vz;
            D_801F0FFC.vy += 0x800;
        }
        pt->pad = pad;
    }
}

void func_801E7BE8(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad) {
    s32 k;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801ECC0C);
    SVECTOR pos;
    SVECTOR *pt;
    s32 n;
    s32 i;

    n = (size - 4) / 8;
    pt = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
    if (count < n) {
        for (i = 0; i < n - count; i++, pt++) {
            pt->pad = 0;
        }
    } else {
        count = n;
    }
    for (i = 0; i < count; i++, pt++) {
        k = i + 1;
        func_801EC4A4(track, count - k + base, &pos);
        pt->vx = pos.vx;
        pt->vy = pos.vy;
        pt->vz = pos.vz;
        if (i == 0) {
            D_801F0FF4.vx = pos.vx;
            D_801F0FF4.vy = pos.vy;
            D_801F0FF4.vz = pos.vz;
            ex->btlfx_aimAngles((s16 *)&D_801F0FF4, (s16 *)&D_801F1004, (s16 *)&D_801F0FFC);
        }
        pt->pad = pad;
    }
}

void func_801E7D5C(void *trackA, void *trackB, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad, s32 t,
                   s32 rotY) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801ECC0C);
    MATRIX m;
    VECTOR out;
    SVECTOR rot;
    SVECTOR a;
    SVECTOR b;
    SVECTOR p;
    SVECTOR *pt;
    s32 n;
    s32 i;
    s32 k;

    n = (size - 4) / 8;
    pt = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
    if (count < n) {
        for (i = 0; i < n - count; i++, pt++) {
            pt->pad = 0;
        }
    } else {
        count = n;
    }
    for (i = 0; i < count;) {
        i++;
        k = count - i + base;
        func_801EC520(trackA, k, NULL, &a);
        func_801EC520(trackB, k, NULL, &b);
        ex->btlfx_lerpVec((s16 *)&a, (s16 *)&b, t, (s16 *)&p);
        rot.vx = 0;
        rot.vz = 0;
        rot.vy = rotY;
        p.vx *= 6;
        p.vy *= 3;
        p.vz *= 6;
        ex->func_800316D8(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&p);
        gte_rtv0tr();
        gte_stlvnl(&out);
        out.vx += D_801F1004.vx;
        out.vy += D_801F1004.vy;
        out.vz += D_801F1004.vz;
        pt->vx = out.vx;
        pt->vy = out.vy;
        pt->vz = out.vz;
        pt->pad = pad;
        pt++;
    }
}

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801E7FBC);

void func_801E83B0(void *track, s32 arg1, s32 arg2, SVECTOR *scale, s32 w, s32 rotY) {
    Unk800F78A4 *env = D_801ECC0C;
    Fx0e2Edge *e = D_801EE62C;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR v;
    SVECTOR ends[2];
    SVECTOR rot;
    SVECTOR p;
    s32 i;

    ends[0].vx = 0;
    ends[0].vy = -w / 2;
    ends[0].vz = 0;
    ends[1].vx = 0;
    ends[1].vy = w / 2;
    ends[1].vz = 0;
    for (i = 0; i < 60; i++, e++) {
        func_801EC4A4(track, i, &p);
        p.vx = p.vx * scale->vx / 4096;
        p.vy = p.vy * scale->vy / 4096;
        p.vz = p.vz * scale->vz / 4096;
        rot.vx = 0;
        rot.vy = rotY;
        rot.vz = 0;
        ex->func_800316D8(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&p);
        gte_rtv0tr();
        gte_stlvnl(&v);
        p.vx = v.vx;
        p.vy = v.vy;
        p.vz = v.vz + 0x4B0;
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        gte_ldv0(&p);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0x400;
        ex->func_800316D8(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&ends[0]);
        gte_rtps();
        gte_stsxy(&e->unk0);
        gte_ldv0(&ends[1]);
        gte_rtps();
        gte_stsxy(&e->unk4);
        gte_stszotz(&e->unk8);
    }
}

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801E86D0);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_op80);

INCLUDE_RODATA("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_headerPtr);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_op81);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_op82);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_op83);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_op84);

s32 fx_0e2_op85(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    s32 frame;

    if (mode == 0) {
        *arg4 = 2;
    } else if (mode == 1) {
        D_801ECC0C = env;
        *work = -1;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            *work = 0x10;
        }
        if (*work >= 0) {
            func_801EC5B4(0xFF, 0, 0x10 - *work, 0x10, 1, 0);
            (*work)--;
        }
        return frame >= 0x10;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC3D8);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC3F0);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC434);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC48C);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC4A4);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC508);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC520);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC5B4);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC64C);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC718);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC764);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC7C0);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801EC920);
