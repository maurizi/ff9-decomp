#include "ovl_0b.h"

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_4", func_800C58E4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_4", func_800C6528);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_4", func_800C67BC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_4", func_800C7730);

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

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_4", func_800C8A3C);
