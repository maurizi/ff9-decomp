#include "ovl_0b.h"

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_4", func_800C58E4);

void func_800C6528(BtlUnit9C *arg0, u8 arg1) {
    MATRIX m;
    BtlMesh *g;
    MATRIX *bone;
    SVECTOR *vtx;
    s32 *sxy;
    s32 *sz;
    s32 k;
    u32 i;
    s32 n;
    u32 cnt;
    MATRIX *b;
    SVECTOR *v;
    s16 *c1;
    s16 *c2;

    g = &arg0->unk8->unk10[arg1];
    gte_SetGeomScreen(D_8006794C->unk1C->unkA);
    sxy = D_800711A8;
    sz = D_80070520;
    k = 0;
    cnt = arg0->unk8->unk2;
    bone = (MATRIX *)arg0->unk20;
    for (i = 0; i < cnt; bone++) {
        n = g->unk14[i++];
        if (n != 0) {
            gte_SetRotMatrix(&D_8006794C->unk1C->unk18);
            gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
            b = bone;
            gte_ldclmv(b);
            c1 = &b->m[0][1];
            c2 = &b->m[0][2];
            gte_rtir_b();
            sxy = &D_800711A8[k];
            gte_stclmv(&m);
            gte_ldclmv(c1);
            gte_rtir();
            sz = &D_80070520[k];
            gte_stclmv(&m.m[0][1]);
            gte_ldclmv(c2);
            c1 = (s16 *)b->t;
            gte_rtir_n1();
            vtx = &g->unk18[k];
            gte_stclmv(&m.m[0][2]);
            gte_ldlv0(c1);
            k += n;
            gte_rtv0tr_n1();
            gte_ldv3c(vtx);
            gte_stlvnl(m.t);
            gte_SetRotMatrix(&m);
            gte_SetTransMatrix(&m);
            v = vtx;
            do {
                v += 3;
                n -= 3;
                gte_rtpt_b();
                gte_ldv3c(v);
                gte_stsxy3c(sxy);
                gte_stsz3c(sz);
                sxy += 3;
                sz += 3;
            } while (n > 0);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_4", func_800C67BC);

void func_800C7730(BtlUnit9C *arg0, s16 *arg1, s16 *arg2) {
    BtlUnit9C8 *h;
    BtlMesh *m;
    u32 i;
    u32 j;
    u32 k;
    u8 *src;
    WmSrcFT4 *ft4;
    WmSrcFT3 *ft3;
    WmSrcGT4 *gt4;
    WmSrcGT3 *gt3;
    u8 *mark;
    s32 du;
    s32 dv;

    arg0->unk54 = 0x1000;
    arg0->unk56 = 0x1000;
    arg0->unk58 = 0x1000;
    arg0->unk0 |= 2;
    h = arg0->unk8;
    arg0->unk18 = (BtlUnit9C18 *)arg1;
    arg0->unk1C = NULL;
    arg0->unk60 = NULL;
    arg0->unk5C = NULL;
    arg0->unk44 = 0;
    arg0->unk20 = NULL;
    arg0->unk10 = 0;
    arg0->unk50 = NULL;
    arg0->unk74 = 0;
    arg0->unk76 = 0;
    arg0->unk78 = 0;
    arg0->unk14 = 0;
    arg0->unk0 = 0;
    if (!(h->unk0 & 1)) {
        h->unk0 |= 1;
        m = h->unk10 = (BtlMesh *)((u8 *)h + (s32)h->unk10);
        h->unkC = (s32)h + h->unkC;
        for (i = 0; i < h->unk3; i++, m++) {
            m->unk14 = (u16 *)((u8 *)h + (s32)m->unk14);
            m->unk18 = (SVECTOR *)((u8 *)h + (s32)m->unk18);
            m->unk1C = (u32 *)((u8 *)h + (s32)m->unk1C);
            m->unk20 = (void *)((u8 *)h + (s32)m->unk20);
            m->unk24 = (void *)((u8 *)h + (s32)m->unk24);
        }
        if (arg2 != NULL) {
            for (i = 0; i < h->unk3; i++) {
                mark = (u8 *)D_80070520;
                for (k = 0; k < 0x1908; k++) {
                    mark[k] = 0;
                }
                m = &h->unk10[i];
                src = (u8 *)m->unk1C;
                for (k = 0; k < m->unk2; k++) {
                    ft4 = (WmSrcFT4 *)src;
                    du = arg2[ft4->unk13 * 2];
                    dv = arg2[ft4->unk13 * 2 + 1];
                    for (j = 0; j < 4; j++) {
                        if (mark[ft4->unk8[j]] == 0) {
                            m->unk20[ft4->unk8[j]][0] += du;
                            m->unk20[ft4->unk8[j]][1] += dv;
                            mark[ft4->unk8[j]] = 1;
                        }
                    }
                    src += sizeof(WmSrcFT4);
                }
                for (k = 0; k < m->unk4; k++) {
                    ft3 = (WmSrcFT3 *)src;
                    du = arg2[ft3->unk6 * 2];
                    dv = arg2[ft3->unk6 * 2 + 1];
                    for (j = 0; j < 3; j++) {
                        if (mark[ft3->unkC[j]] == 0) {
                            m->unk20[ft3->unkC[j]][0] += du;
                            m->unk20[ft3->unkC[j]][1] += dv;
                            mark[ft3->unkC[j]] = 1;
                        }
                    }
                    src += sizeof(WmSrcFT3);
                }
                for (k = 0; k < m->unk6; k++) {
                    gt4 = (WmSrcGT4 *)src;
                    du = arg2[gt4->unk1D * 2];
                    dv = arg2[gt4->unk1D * 2 + 1];
                    for (j = 0; j < 4; j++) {
                        if (mark[gt4->unk8[j]] == 0) {
                            m->unk20[gt4->unk8[j]][0] += du;
                            m->unk20[gt4->unk8[j]][1] += dv;
                            mark[gt4->unk8[j]] = 1;
                        }
                    }
                    src += sizeof(WmSrcGT4);
                }
                for (k = 0; k < m->unk8; k++) {
                    gt3 = (WmSrcGT3 *)src;
                    du = arg2[gt3->unk16 * 2];
                    dv = arg2[gt3->unk16 * 2 + 1];
                    for (j = 0; j < 3; j++) {
                        if (mark[gt3->unk6[j]] == 0) {
                            m->unk20[gt3->unk6[j]][0] += du;
                            m->unk20[gt3->unk6[j]][1] += dv;
                            mark[gt3->unk6[j]] = 1;
                        }
                    }
                    src += sizeof(WmSrcGT3);
                }
            }
        }
    }
}

void func_800C7B84(BtlUnit9C *arg0) {
    union {
        MATRIX m;
        s32 w[8];
    } s;
    s16 x;

    if (arg0->unk0 & 2) {
        x = arg0->unk54;
        s.w[0] = 0x1000;
        s.w[1] = 0;
        s.w[2] = 0x1000;
        s.w[3] = 0;
        s.w[4] = 0x1000;
        s.m.m[0][0] = x;
        gte_SetRotMatrix(&arg0->unk24);
        gte_ldclmv(&s.m);
        gte_rtir();
        s.m.m[1][1] = arg0->unk56;
        gte_stclmv(&arg0->unk24);
        gte_ldclmv((s16 *)&s.m + 1);
        gte_rtir();
        s.m.m[2][2] = arg0->unk58;
        gte_stclmv((s16 *)&arg0->unk24 + 1);
        gte_ldclmv((s16 *)&s.m + 2);
        gte_rtir();
        gte_stclmv((s16 *)&arg0->unk24 + 2);
    }
}

void func_800C7CC8(BtlUnit9C *arg0) {
    SVECTOR v;
    s32 z;

    arg0->unk0 &= ~0x10;
    if (arg0->unk1C != NULL) {
        arg0->unk0 |= arg0->unk1C->unk0 & 0x10;
        return;
    }
    v.vx = arg0->unk24.t[0];
    v.vy = arg0->unk24.t[1];
    v.vz = arg0->unk24.t[2];
    gte_SetRotMatrix(&D_8006794C->unk1C->unk18);
    gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
    gte_ldv0(&v);
    gte_rtps();
    gte_stsz(&z);
    if (z == 0) {
        arg0->unk0 |= 0x10;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_4", func_800C7DAC);

void func_800C86C8(BtlUnit9C *arg0, s32 r, s32 g, s32 b) {
    u32 i;
    BtlPrim *p;
    u8 code;

    if (!(arg0->unk0 & 0x10)) {
    i = 0;
    p = (BtlPrim *)arg0->unk10;
    if (arg0->unk8->unk3 != 0) {
    do {
        if (!(arg0->unk14 & (1 << i))) {
        do {
            code = p->tag.code;
            if (code == 0xFF || code == 0xE1) {
                p = (BtlPrim *)&p->w[2];
                continue;
            }
            switch (code & 0x3C) {
            case 0x2C:
                p->w[1] = func_800C8DE0(p->w[1], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[10];
                break;
            case 0x24:
                p->w[1] = func_800C8DE0(p->w[1], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[8];
                break;
            case 0x3C:
                p->w[1] = func_800C8DE0(p->w[1], (s16)r, (s16)g, (s16)b);
                p->w[4] = func_800C8DE0(p->w[4], (s16)r, (s16)g, (s16)b);
                p->w[7] = func_800C8DE0(p->w[7], (s16)r, (s16)g, (s16)b);
                p->w[10] = func_800C8DE0(p->w[10], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[13];
                break;
            case 0x34:
                p->w[1] = func_800C8DE0(p->w[1], (s16)r, (s16)g, (s16)b);
                p->w[4] = func_800C8DE0(p->w[4], (s16)r, (s16)g, (s16)b);
                p->w[7] = func_800C8DE0(p->w[7], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[10];
                break;
            case 0x38:
                p->w[1] = func_800C8DE0(p->w[1], (s16)r, (s16)g, (s16)b);
                p->w[3] = func_800C8DE0(p->w[3], (s16)r, (s16)g, (s16)b);
                p->w[5] = func_800C8DE0(p->w[5], (s16)r, (s16)g, (s16)b);
                p->w[7] = func_800C8DE0(p->w[7], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[9];
                break;
            case 0x30:
                p->w[1] = func_800C8DE0(p->w[1], (s16)r, (s16)g, (s16)b);
                p->w[3] = func_800C8DE0(p->w[3], (s16)r, (s16)g, (s16)b);
                p->w[5] = func_800C8DE0(p->w[5], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[7];
                break;
            case 0x28:
                p->w[1] = func_800C8DE0(p->w[1], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[6];
                break;
            case 0x20:
                p->w[1] = func_800C8DE0(p->w[1], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[5];
                break;
            }
        } while (code != 0xFF);
        }
    } while (++i < arg0->unk8->unk3);
    }
    }
}

/* padded: the next jump table is 8-aligned in its object */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

void func_800C8A3C(BtlUnit9C *arg0, s32 r, s32 g, s32 b) {
    u32 i;
    BtlPrim *p;
    u32 col;
    u8 code;

    i = 0;
    if (!(arg0->unk0 & 0x10)) {
    col = (b << 16) | ((g << 16) >> 8) | (s16)r;
    p = (BtlPrim *)arg0->unk10;
    if (arg0->unk8->unk3 != 0) {
    do {
        if (!(arg0->unk14 & (1 << i))) {
        do {
            code = p->tag.code;
            if (code == 0xFF || code == 0xE1) {
                p = (BtlPrim *)&p->w[2];
                continue;
            }
            switch (code & 0x3C) {
            case 0x2C:
                p->w[1] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[10];
                break;
            case 0x24:
                p->w[1] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[8];
                break;
            case 0x3C:
                p->w[1] = col;
                p->w[4] = col;
                p->w[7] = col;
                p->w[10] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[13];
                break;
            case 0x34:
                p->w[1] = col;
                p->w[4] = col;
                p->w[7] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[10];
                break;
            case 0x38:
                p->w[1] = col;
                p->w[3] = col;
                p->w[5] = col;
                p->w[7] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[9];
                break;
            case 0x30:
                p->w[1] = col;
                p->w[3] = col;
                p->w[5] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[7];
                break;
            case 0x28:
                p->w[1] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[6];
                break;
            case 0x20:
                p->w[1] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[5];
                break;
            }
        } while (code != 0xFF);
        }
    } while (++i < arg0->unk8->unk3);
    }
    }
}
