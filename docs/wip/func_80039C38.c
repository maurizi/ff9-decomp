#include "ff9.h"

/* ---- decls (to move to ff9.h) ---- */
extern u8 *D_8007AEA0;
extern s32 D_8007AE04;
Unk8004AB64 *func_80038150(void);
void func_80038278(Unk8004AB64 *);
s32 func_800500A8(Unk8004AB64 *, s32);
s32 func_80050410(Unk8004AB64 *, s32);
s32 func_80050744(Unk8004AB64 *, s32);

extern s32 D_8007AEA4;
extern Unk8004AB64 *D_8007AE68;
extern s32 D_8007AE6C;
extern s32 D_8007BBD8;
void func_800398D0(Unk8004AB64 *, s32, s32, s32);
s32 func_80052F24(s32, s32);
void func_8004EC74(s32, s32);
void func_8004ED00(s32, s32);
void func_8004EFA0(void);
s32 func_80039660(Unk8004AB64 *, s32, s32);
void func_8004DCEC(s32, s32, s32, s32);

static inline s32 func_8004AF58_inline(void) {
    s32 r;
    if (D_8007B6F4 & 1) {
        func_80052DA0();
        r = func_80052D40();
    } else {
        Unk8004AB64 *pp = D_8007B70C;
        r = *pp->unk0++;
        r |= *pp->unk0++ << 8;
        r = (s16)r;
    }
    D_8007B6F4 >>= 1;
    D_8007B6F8 = 1;
    return r;
}

/* uid of the object the current script runs for */
#define CUR_UID() (((u8 *)((s32 *)D_8007B70C + D_8007B70C->unk9) + D_8007B70C->unkA * 4)[-1])

static inline s32 func_8004AEDC_inline(void) {
    s32 r;
    if (D_8007B6F4 & 1) {
        func_80052DA0();
        r = func_80052D40();
    } else {
        r = *D_8007B70C->unk0++;
    }
    D_8007B6F4 >>= 1;
    D_8007B6F8 = 1;
    return r;
}

extern s32 D_8007B710;
extern s32 D_8007AE24;
extern s32 D_8007AE28;
void func_8004EA4C(s32);
void func_8004E924(void);
void func_8002DD54(void);
void func_8004ED3C(void);
void *func_8004F058(s32);
s32 func_80038990(s32, s32, s32, s32);
void func_8004B9B8(Unk8004B9B8 *);

/* Inlined helper: start the animation set up in unk60/unk61/unk62. */
static inline void func_80039C38_anim(Unk8004AB64 *p, s32 anim) {
    if ((p->unk62 & 0x80) && (p->unkF & 0x80)) {
        func_8004B9B8((Unk8004B9B8 *)p);
    }
    p->unk36 = anim;
    p->unk3A = p->unk60;
    p->unk62 &= 0xB9;
    if (p->unk61 < p->unk60) {
        p->unk62 |= 0xC0;
    } else {
        p->unk62 |= 0x80;
    }
    p->unk64 = 0;
    p->unk38 = func_800216D8(p->unk34, anim);
    p->unk5D = p->unk69;
}

/* event region (quad) object, same header as Unk8004AB64 */
typedef struct {
    s16 unk0; /* x */
    s16 unk2; /* z */
} Unk80039C38Pt;
typedef struct {
    u8 pad0[0x14];
    s32 unk14; /* vertex count */
    Unk80039C38Pt unk18[8];
} Unk80039C38Quad;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0xC - 5];
    s32 unkC;
    u8 pad10[0x12 - 0x10];
    s16 unk12;
} Unk8004AFF0Sub; /* 0x14 */

/* model object (Unk8E0.unk8) */
typedef struct Unk8004AFF0 {
    u16 unk0; /* flags */
    u8 pad2[4 - 2];
    u8 unk4;
    u8 pad5[0x14 - 5];
    u32 unk14;
    u8 pad18[0x1C - 0x18];
    struct Unk8004AFF0 *unk1C;
    u8 pad20[0x50 - 0x20];
    Unk8004AFF0Sub *unk50;
    s16 unk54;
    s16 unk56;
    s16 unk58;
    u8 pad5A[0x74 - 0x5A];
    s16 unk74;
    s16 unk76;
    s16 unk78;
} Unk8004AFF0;

/* Inlined copy of func_8004AFF0. */
static inline Unk8004AFF0 *func_8004AFF0_inline(Unk8004AB64 *p) {
    Unk8E0 *q;

    if (p != NULL) {
        if (p->unk34 == 0xFFFF) {
            return NULL;
        }
        q = (Unk8E0 *)func_8001E350(p->unk3B);
        if (q != NULL) {
            return (Unk8004AFF0 *)q->unk8;
        }
    }
    return NULL;
}

extern u8 D_8007BD2C;
extern s32 D_8007AE50;
extern u8 D_8007AE48;
extern s16 D_8007AE40;
extern s16 D_8007AE42;
extern s16 D_8007AE44;
extern s16 D_8007AE46;
extern u8 D_8006E658[];
void func_80021660(u16);
void func_80021680(s16);
void func_8004FF98(s32);
void func_80052D0C(u8 **);
Unk16D8 *func_80036978(Unk8004AB64 *);
void func_800499D8(Unk8004B9B8 *);

void func_800AFAC0(s32, s32, s32, s32, s32);
void func_800B1A20(s16, s16, s16, u8 *arg4, u8 *arg5);
s32 func_800B2CC0(s32, s32, s32, s32);
void func_800B5004(s32, s16);
void func_800B73F8(u32, s32);
s32 func_800C02D0(s16, s16, s16, s8, u8, u8, u8);
s32 func_800C0BCC(s32);
s32 func_800C0DB0(s32, s32, s32);
s32 func_800C0DF4(s16, s16, s16, s32);
s32 func_800C0E6C(u16 *arg0, u16 *arg1);
s32 func_800C0E94(s32, s32, s32);
s32 func_800C1024(s16);
s32 func_800C104C(void);
s32 func_800C1100(void);
s32 func_800C119C(s32, s16, s16, s16, s16);
s32 func_800C11C8(s32, s16, s16);
s32 func_800C11E8(s32, s16, s16, s16);
s32 func_800C1368(s32, u8, u8, u8);
s32 func_800C142C(s32, s32);
s32 func_800C147C(s32, s32, s16, s16);
s32 func_800C14DC(s32, s32);
s32 func_800C152C(s32, s32, s16, s16, s32);
s32 func_800C15BC(s32, s32, s16, s16);
s32 func_800C1614(s32, s16, s16);
s32 func_800C1668(s32, s32);
s32 func_800C16B0(s32, s32);
s32 func_800C16F4(s32, s32);
s32 func_800C17AC(s32, s32);
s32 func_800C17F0(s32, s16);
s32 func_800C1868(s32, s32, u8);
s32 func_800C18A0(s32, s32);
s32 func_800C18D4(s32, u32, u32);
s32 func_800C196C(s32, s32);
s32 func_800C1A8C(s32, s16, s16, s16, s16);
s32 func_800C47E4(s32, s32);
s32 func_800C6408(u32, u32);
s32 func_800C7464(u8);
s32 func_800C778C(s32, s32);
s32 func_800C7AF8(u32, u32);
s32 func_800C7B68(u32, s32);
s32 func_800C7BC4(u32, s16);
s32 func_800C7C04(u32, s16);
s32 func_800C7C70(u32, u32, s16);
s32 func_800C7CDC(u32, s32);
s32 func_800C7DF0(u32, s32);
s32 func_800C7F34(s32, s32);
s32 func_800C7F7C(s32, s32);
s32 func_800C7FBC(s32, s32);
s32 func_800C8010(s32, s32);
s32 func_800C8044(s32, s16);
s32 func_800C8068(s32, s16);
s32 func_800C808C(s32, s16);
s32 func_800C80B0(s32, s16, s16);
s32 func_800C80E4(s32, s16);
void func_800F4F04(s32, s32);
void func_801F0FF4(void);
void func_800AA3F4(s32, s32, s32, s32);
s32 func_800C3E18(s32, s32);
s32 func_800C7570(s32, u16);
void func_800B1520(u16, u16, s32, s32);
void func_800CA694(u16, u16, s32);
void func_800BE99C(s32, s32);
void func_800ABA98(s32, s32, s32, s32, s32);
extern s32 D_8007AE2C;
extern s32 D_8007AE30;
extern s32 D_8007AE38;
extern s32 D_8007AE3C;
void func_80050068(s32);
void func_80050080(s32);
void func_8004A9E8(void);
void func_80028074(s32);
void func_800280CC(s32);
s32 func_8006238C(s32);
void func_80033C0C(u32);

/* Inlined copy of func_8004B914 (24-bit script argument). */
static inline s32 func_8004B914_inline(void) {
    s32 v;
    Unk8004AB64 *pp;

    if (D_8007B6F4 & 1) {
        func_80052DA0();
        v = func_80052D40();
    } else {
        pp = D_8007B70C;
        v = *pp->unk0++;
        v |= *pp->unk0++ << 8;
        v |= *pp->unk0++ << 16;
    }
    D_8007B6F4 >>= (D_8007B6F8 = 1);
    return v;
}

extern u8 D_8006E630[];
extern u8 D_8006E63C[];
s32 func_8004E7A4(s32);
void func_800333B0(s32, u8, s32, s32);
s32 func_8003470C(s32, s32);
s32 func_80034220(s32, s32);
s32 func_80053960(Unk800539FC *, s32);
void func_8002412C(s32);
void func_800240B0(u8);
void func_800240F8(u8);
void func_8004BA20(SVECTOR *, s32 *, s32 *);
void func_800AD944(s32, s32, s32);
VECTOR *func_800AE750(void);
VECTOR *func_800AE744(void);
s32 func_80054FB8(s32);
s32 func_80054FE0(s32);
void func_80054E88(s32, s32, s32);
s32 func_80055060(s32, s32, s32);
s32 func_800550C0(s16);
s32 func_800550D0(s32);
s32 func_800550F0(s32, s32);
void func_800C7D30(s32, s32, s32);
extern u8 D_8007AE90[];
extern u8 *D_8007AE8C;

typedef struct {
    s32 unk0;
    u8 unk4[4]; /* party character ids */
    u8 unk8[8];
    u8 unk10[9];
} Unk8007AE70;
extern Unk8007AE70 D_8007AE70;

/* Inlined helper: rebuild D_8007AECC->unk16E4 from the current party. */
static inline void func_80039C38_party(void) {
    s32 mask = 0;
    s32 i = 0;
    u8 *p = D_8007AECC->unk16E4;
    s32 r;

    *(s32 *)D_8007AECC->unk16E4 = -1;
    for (; i < 4; i++) {
        r = func_8004E7A4(i);
        if (r >= 0) {
            mask |= 1 << D_8006E63C[r];
        }
    }
    for (i = 0; mask != 0; i++) {
        if (mask & 1) {
            *p++ = D_8006E630[i] + (*(u8 *)&D_8007ADFC - 9);
        }
        mask >>= 1;
    }
}

/* ---- function ---- */
s32 func_80039C38(void) {
    Unk8004AB64 *po = D_8007B704;
    Unk8004AB64 *actor = po;
    Unk8004AB64 *obj;
    Unk16D8 *n;
    s32 op;
    s32 a, b, c, d, e, f, i;
    SVECTOR sv;
    Unk80039C38Pt *pt;
    Unk8004AFF0 *m, *m2;
    u8 *str;

    D_8007AEA0 = D_8007B70C->unk0;
    op = *D_8007B70C->unk0++;
    D_8007B6F4 = *D_8007B70C->unk0++;
    D_8007B6F8 = 0;

    switch (op) {
    case 0x07:
        func_800367D0(D_8007B6F4, *D_8007B70C->unk0++, 0x14, 0x10)->unk5 = 2;
        D_8007B6F8 = 1;
        return 0;
    case 0x08: {
        Unk800367D0 *p = func_800367D0(D_8007B6F4, *D_8007B70C->unk0++, 0x38, 0x10);
        p->unk5 = 3;
        p->unkF = 1;
        D_8007B6F8 = 1;
        return 0;
    }
    case 0x09:
        a = D_8007B6F4;
        b = *D_8007B70C->unk0++;
        if ((u32)(a - 0xFB) < 4) {
            a = D_8007AECC->unk16E4[a - 0xFB];
        }
        obj = (Unk8004AB64 *)func_800367D0(a, b, 0xAC, 0x10);
        obj->unk34 = 0xFFFF;
        obj->unk2F = 0x40;
        obj->unk5 = 4;
        obj->unk5C = 0x1E;
        obj->unk5E = 0x1F;
        obj->unk71 = 0x50;
        obj->unk3C = 0x10;
        obj->unk3D = 0x10;
        obj->unk3E = 0xFF;
        obj->unk69 = 0x10;
        obj->unk5D = 0x10;
        obj->unk61 = 0xFF;
        obj->unk5F = 0x10;
        obj->unk6E = 4;
        obj->unk72 = 4;
        obj->unk73 = 0x10;
        obj->unk93 = 0xFF;
        *(u32 *)obj->unk7C = 0x10101010;
        obj->unkF |= 4;
        switch (D_8007B708) {
        case 1:
            func_800AA3F4(a, 0x10, 0xFF, 0x10101010);
            break;
        case 2:
            D_8007AED0[D_8007AE04++] = obj;
            break;
        }
        D_8007B6F8 = 1;
        return 0;
    case 0x46:
    case 0x79:
    case 0xD2:
    case 0xD3:
        return 0;
    case 0x10:
        a = func_8004AEDC_inline();
        func_800384D0(func_80038150(), a, *D_8007B70C->unk0++, 0);
        return 0;
    case 0x12:
        a = func_8004AEDC_inline();
        obj = func_80038150();
        b = *D_8007B70C->unk0++;
        if (obj != NULL && a < obj->unk4) {
            func_800384D0(obj, a, b, 0);
            return 0;
        }
        D_8007B6F8 = 1;
        D_8007B70C->unk0 = D_8007AEA0;
        return 1;
    case 0x14:
        a = func_8004AEDC_inline();
        obj = func_80038150();
        b = *D_8007B70C->unk0++;
        if (obj != NULL && a < obj->unk4) {
            func_800384D0(obj, a, b, 1);
            return 1;
        }
        D_8007B6F8 = 1;
        D_8007B70C->unk0 = D_8007AEA0;
        return 1;
    case 0x16:
        a = func_8004AEDC_inline();
        func_800384D0(func_8004B040_inline(CUR_UID()), a, *D_8007B70C->unk0++, 0);
        return 0;
    case 0x18:
        a = func_8004AEDC_inline();
        if (func_8004B040_inline(CUR_UID()) != NULL && a < func_8004B040_inline(CUR_UID())->unk4) {
            func_800384D0(func_8004B040_inline(CUR_UID()), a, *D_8007B70C->unk0++, 0);
            return 0;
        }
        D_8007B6F8 = 1;
        D_8007B70C->unk0 = D_8007AEA0;
        return 1;
    case 0x1A:
        a = func_8004AEDC_inline();
        if (func_8004B040_inline(CUR_UID()) != NULL && a < func_8004B040_inline(CUR_UID())->unk4) {
            func_800384D0(func_8004B040_inline(CUR_UID()), a, *D_8007B70C->unk0++, 1);
            return 1;
        }
        D_8007B6F8 = 1;
        D_8007B70C->unk0 = D_8007AEA0;
        return 1;
    case 0x97:
        obj = func_80038150();
        if (obj != NULL) {
            obj->unkA = 0;
            obj->unkE = 2;
            func_80038278(obj);
        }
        return obj == D_8007B70C;
    case 0x93:
        D_8007B704->unkF &= 0xC0;
        D_8007B704->unkF |= func_8004AEDC_inline() & 0x3F;
        return 0;
    case 0x6A:
        D_8007AECC->unk16F8 = 1;
        return 0;
    case 0xF0:
        D_8007AECC->unk16F8 = 0;
        return 0;
    case 0xD5:
        for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
            obj = n->unk4;
            if (obj->unk5 == 4) {
                obj->unk2C = obj->unkF;
                if (!(obj->unkF & 0x20)) {
                    obj->unkF &= 0xFE;
                }
            }
        }
        return 0;
    case 0xD6:
        for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
            obj = n->unk4;
            if (obj->unk5 == 4) {
                obj->unkF &= 0xFE;
                obj->unkF |= obj->unk2C & 1;
            }
        }
        return 0;
    case 0x4B:
        if (D_8007B704->unk5 == 4) {
            a = func_8004AEDC_inline();
            if (D_8007B708 == 1 && po->unk34 != 0xFFFF) {
                func_800C7570(po->unk3B, (a << 2) & 0xFFFC);
            }
            po->unk3C = func_8004AEDC_inline();
            po->unk3D = func_8004AEDC_inline();
        } else {
            func_8004AEDC_inline();
            func_8004AEDC_inline();
            func_8004AEDC_inline();
        }
        return 0;
    case 0xA3:
        func_8004AEDC_inline();
        func_8004AEDC_inline();
        func_8004AEDC_inline();
        func_8004AEDC_inline();
        return 0;
    case 0xDF:
        po->unk2D = func_8004AEDC_inline();
        return 0;
    case 0x3B:
        D_8007B70C->unk11 = func_8004AEDC_inline();
        return 0;
    case 0xBF:
        po = func_80038150();
    case 0x1D:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = D_8007B708 == 1 && po->unk34 != 0xFFFF;
        if (c) {
            func_800C3E18(po->unk3B, 0);
        }
        po->unk14.vx = po->unk40.vx = a;
        po->unk14.vy = po->unk40.vy = -0x8000;
        po->unk14.vz = po->unk40.vz = b;
        if (c) {
            func_800C3E18(po->unk3B, 1);
        }
        if (po->unk5 == 4) {
            po->unk78 = 0x7FFFFFFF;
            po->unk30 = 0;
            po->unk50 &= 0xFFCF;
        }
        D_8007AEA4 = 1;
        return 0;
    case 0xAD:
        po = func_80038150();
    case 0xA1:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        d = D_8007B708 == 1 && po->unk34 != 0xFFFF;
        if (d) {
            func_800C3E18(po->unk3B, 0);
        }
        po->unk14.vx = po->unk40.vx = a;
        po->unk14.vy = po->unk40.vy = b;
        po->unk14.vz = po->unk40.vz = c;
        if (d) {
            func_800C3E18(po->unk3B, 1);
        }
        if (po->unk5 == 4) {
            po->unk78 = 0x7FFFFFFF;
            po->unk30 = 0;
            po->unk50 &= 0xFFCF;
        }
        return 0;
    case 0x87:
        po = func_80038150();
    case 0x36:
        po->unk14.unk22 = func_8004AEDC_inline() << 4;
        return 0;
    case 0x37:
        po->unk14.unk20 = func_8004AEDC_inline() << 4;
        po->unk24 = func_8004AEDC_inline() << 4;
        return 0;
    case 0x8B:
        actor->unk6A = func_8004AEDC_inline();
        actor->unk6B = func_8004AEDC_inline();
        actor->unk50 |= 3;
        return 0;
    case 0x47:
        actor->unk50 &= 0xFFF8;
        actor->unk50 |= func_8004AEDC_inline() & 7;
        return 0;
    case 0x91:
        actor->unk6E = func_8004AEDC_inline() ? 0xA7 : 4;
        return 0;
    case 0x99:
        actor->unk73 = func_8004AEDC_inline();
        if (actor->unk73 == 0) {
            actor->unk73 = 0x10;
        }
        return 0;
    case 0xBB:
        actor = func_80038150();
    case 0x56:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_800398D0(actor, a << 4, 1, b);
        return 0;
    case 0xA7:
        a = func_8004AEDC_inline();
        func_800398D0(actor, a << 4, 1, actor->unk73);
        return 0;
    case 0x9B:
        a = func_8004AF58_inline() - actor->unk14.vx;
        b = func_8004AF58_inline() - actor->unk14.vz;
        if (a | b) {
            func_800398D0(actor, func_80052F24(a, b), 1, actor->unk73);
        }
        return 0;
    case 0x51:
        obj = func_80038150();
        a = func_8004AEDC_inline();
        if (obj != NULL) {
            func_800398D0(actor, func_80052F24(obj->unk14.vx - actor->unk14.vx, obj->unk14.vz - actor->unk14.vz), 1, a);
        }
        return 0;
    case 0xBC:
        actor = func_80038150();
    case 0x50:
        if (actor->unkF & 0x80) {
            D_8007B6F8 = 1;
            D_8007B70C->unk0 = D_8007AEA0;
        }
        return 1;
    case 0x66:
        a = func_8004AEDC_inline();
        func_8004EC74(a, func_8004AF58_inline());
        return 0;
    case 0xE3:
        D_8007BBD8 = func_8004AEDC_inline();
        return 0;
    case 0x7C:
        a = func_8004AF58_inline();
        func_8004ED00(a, func_8004AEDC_inline());
        return 0;
    case 0x53:
        func_8004EFA0();
        return 0;
    case 0x1F:
        D_8007B704->unk10 = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = D_8007B704->unk10;
        if ((b & 0xA0) == 0x80 && D_8007B704->unk5 == 4) {
            for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
                obj = n->unk4;
                if (obj->unk5 == 4 && obj->unk3E == c) {
                    obj->unk3E = 0xFF;
                }
            }
            D_8007B704->unk3E = c;
            D_8007AE68 = D_8007B704;
            D_8007AE6C = 0;
        }
        a = func_8004AF58_inline();
        func_8004DCEC(a, D_8007B704->unk10, func_80039660(D_8007B704, a, b), b);
        D_8007B704->unkD = 0xFE;
        return 1;
    case 0x20:
        D_8007B704->unk10 = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = D_8007B704->unk10;
        if ((b & 0xA0) == 0x80 && D_8007B704->unk5 == 4) {
            for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
                obj = n->unk4;
                if (obj->unk5 == 4 && obj->unk3E == c) {
                    obj->unk3E = 0xFF;
                }
            }
            D_8007B704->unk3E = c;
            D_8007AE68 = D_8007B704;
            D_8007AE6C = 0;
        }
        a = func_8004AF58_inline();
        func_8004DCEC(a, D_8007B704->unk10, func_80039660(D_8007B704, a, b), b);
        return 0;
    case 0x95:
        po = func_80038150();
        D_8007B704->unk10 = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = D_8007B704->unk10;
        if ((b & 0xA0) == 0x80 && po->unk5 == 4) {
            for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
                obj = n->unk4;
                if (obj->unk5 == 4 && obj->unk3E == c) {
                    obj->unk3E = 0xFF;
                }
            }
            po->unk3E = c;
            D_8007AE68 = po;
            D_8007AE6C = 0;
        }
        a = func_8004AF58_inline();
        func_8004DCEC(a, D_8007B704->unk10, func_80039660(po, a, b), b);
        D_8007B704->unkD = 0xFE;
        return 1;
    case 0x96:
        po = func_80038150();
        D_8007B704->unk10 = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = D_8007B704->unk10;
        if ((b & 0xA0) == 0x80 && po->unk5 == 4) {
            for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
                obj = n->unk4;
                if (obj->unk5 == 4 && obj->unk3E == c) {
                    obj->unk3E = 0xFF;
                }
            }
            po->unk3E = c;
            D_8007AE68 = po;
            D_8007AE6C = 0;
        }
        a = func_8004AF58_inline();
        func_8004DCEC(a, D_8007B704->unk10, func_80039660(po, a, b), b);
        return 0;
    case 0x21:
        func_8004EA4C(func_8004AEDC_inline());
        return 0;
    case 0xEB:
        func_8004E924();
        func_8002DD54();
        return 0;
    case 0x54:
        D_8007B704->unk10 = func_8004AEDC_inline();
        D_8007B704->unkD = 0xFE;
        return 1;
    case 0x8E:
        func_8004ED3C();
        return 0;
    case 0xD0:
        func_800F4F04((s32)func_8004F058(func_8004AF58_inline()), 3);
        return 0;
    case 0x25:
        actor->unk63 = 0xFF;
        actor->unk78 = 0x7FFFFFFF;
        actor->unk30 = 0;
        actor->unk50 &= 0xFFCF;
        return 0;
    case 0x98:
        actor->unk63 = func_8004AEDC_inline();
        return 0;
    case 0x23:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        if (func_80038990(a, 0, b, 0)) {
            D_8007B6F8 = 1;
            D_8007B70C->unk0 = D_8007AEA0;
        }
        return 1;
    case 0xA5:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        if (func_80038990(a, 0, b, 1)) {
            D_8007B6F8 = 1;
            D_8007B70C->unk0 = D_8007AEA0;
        }
        return 1;
    case 0x24:
        po = func_80038150();
        if (func_80038990(po->unk14.vx, po->unk14.vy, po->unk14.vz, (s32)po)) {
            D_8007B6F8 = 1;
            D_8007B70C->unk0 = D_8007AEA0;
        }
        D_8007B6F8 = 1;
        return 1;
    case 0xA0:
        if (func_80038990(D_8007AE24, 0, D_8007AE28, 0)) {
            D_8007B6F8 = 1;
            D_8007B70C->unk0 = D_8007AEA0;
        }
        return 1;
    case 0xA2:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        if (func_80038990(a, b, c, 2)) {
            D_8007B6F8 = 1;
            D_8007B70C->unk0 = D_8007AEA0;
        }
        return 1;
    case 0xE8:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        if (func_80038990(a, b, c, 3)) {
            D_8007B6F8 = 1;
            D_8007B70C->unk0 = D_8007AEA0;
        }
        return 1;
    case 0x55:
        a = func_8004AEDC_inline();
        if (a == 0) {
            a = 0xFF;
        }
        actor->unk5F = a;
        return 0;
    case 0x26:
        actor->unk5C = func_8004AEDC_inline();
        return 0;
    case 0xA6:
        actor->unk5E = func_8004AEDC_inline();
        return 0;
    case 0x9C:
        actor->unk62 = 8;
        a = actor->unk76;
        actor->unk60 = 0;
        actor->unk61 = actor->unk90 - 1;
        func_80039C38_anim(actor, a);
        if (actor->unk7 == D_8007AECC->unk16ED) {
            D_8007B710++;
        }
        return 0;
    case 0x9D:
        *(u32 *)func_8001E350(actor->unk3B) &= ~0x10;
        actor->unk61 = 0xFF;
        actor->unk62 = 0;
        a = actor->unk76;
        actor->unk60 = actor->unk91 + 1;
        func_80039C38_anim(actor, a);
        if (actor->unk7 == D_8007AECC->unk16ED) {
            D_8007B710++;
        }
        return 0;
    case 0xE2:
        actor->unk94 = func_8004AF58_inline();
        actor->unkAA = func_8004AF58_inline();
        actor->unk96 = func_8004AF58_inline();
        b = func_8004AEDC_inline();
        if (b == 0) {
            b = 8;
        }
        actor->unk50 |= 8;
        *(u32 *)func_8001E350(actor->unk3B) |= 0x10;
        a = actor->unk76;
        actor->unk60 = actor->unk90;
        actor->unk61 = actor->unk91;
        func_80039C38_anim(actor, a);
        if (actor->unk7 == D_8007AECC->unk16ED) {
            D_8007B710++;
        }
        actor->unk68 = 0;
        actor->unk92 = b;
        actor->unkA6 = actor->unk14.vy;
        actor->unkA8 = actor->unk14.vz;
        actor->unkA4 = actor->unk14.vx;
        actor->unk5D = ((actor->unk61 - actor->unk60) << 4) / b;
        if (D_8007B708 == 1 && po->unk34 != 0xFFFF) {
            func_800C3E18(po->unk3B, 0);
        }
        return 0;
    case 0xDC:
        b = actor->unk92;
        a = ++actor->unk68;
        actor->unk14.vx = ((s16)actor->unkA4 * (b - a) + actor->unk94 * a) / b;
        actor->unk14.vy = (s16)actor->unkA6 + a * a * 8 + a * (actor->unkAA - (s16)actor->unkA6) / b - a * (b * 8);
        actor->unk14.vz = ((s16)actor->unkA8 * (b - a) + actor->unk96 * a) / b;
        if (a < b) {
            D_8007B6F8 = 1;
            D_8007B70C->unk0 = D_8007AEA0;
            return 1;
        }
        return 0;
    case 0x29:
        pt = ((Unk80039C38Quad *)D_8007B704)->unk18;
        a = func_8004AEDC_inline();
        ((Unk80039C38Quad *)D_8007B704)->unk14 = a;
        while (a-- != 0) {
            pt->unk0 = func_8004AF58_inline();
            pt->unk2 = func_8004AF58_inline();
            pt++;
        }
        return 0;
    case 0xA4:
        obj = func_8004B040_inline(D_8007AECC->unk16ED);
        if (obj != NULL) {
            pt = ((Unk80039C38Quad *)D_8007B704)->unk18;
            a = pt[1].unk0 - pt[0].unk0;
            b = pt[1].unk2 - pt[0].unk2;
            c = (a * a + b * b) >> 8;
            if (c != 0) {
                c = (a * (obj->unk14.vx - pt[0].unk0) + b * (obj->unk14.vz - pt[0].unk2)) / c;
                if (c < 0) {
                    c = 0;
                } else if (c > 0x100) {
                    c = 0x100;
                }
            }
            D_8007AE24 = ((c * a) >> 8) + pt[0].unk0;
            D_8007AE28 = ((c * b) >> 8) + pt[0].unk2;
        } else {
            D_8007AE28 = 0;
            D_8007AE24 = 0;
        }
        return 0;
    case 0x9E:
        obj = func_8004B040_inline(D_8007AECC->unk16ED);
        if (obj != NULL) {
            s32 *sp = (s32 *)obj + obj->unk9 + obj->unkA;
            sp[0] = obj->unk0 - D_8007AECC->unk16F0;
            sp[1] = obj->unkD | (obj->unk4 << 8) | (D_8007B70C->unk7 << 24) | 0xFF0000;
            obj->unk4 = 0;
            obj->unkD = 0;
            obj->unkA += 2;
            obj->unk0 = D_8006E658;
            D_8007AECC->unk16EC = 0;
            for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
                n->unk4->unkF |= 6;
            }
        }
        return 0;
    case 0x76: {
        Unk80039C38Quad *q = (Unk80039C38Quad *)D_8007B704;
        q->unk14 = 2;
        q->unk18[1].unk0 = q->unk18[0].unk0 = func_8004AF58_inline();
        q->unk18[1].unk2 = q->unk18[0].unk2 = func_8004AF58_inline();
        return 0;
    }
    case 0x77: {
        Unk80039C38Quad *q = (Unk80039C38Quad *)D_8007B704;
        a = q->unk14;
        if (a <= 0) {
            a = 1;
        }
        pt = &q->unk18[a - 1];
        pt->unk0 = func_8004AF58_inline();
        pt->unk2 = func_8004AF58_inline();
        return 0;
    }
    case 0x78: {
        Unk80039C38Quad *q = (Unk80039C38Quad *)D_8007B704;
        a = q->unk14;
        if (a >= 1 && a < 8) {
            q->unk18[a] = q->unk18[a - 1];
            q->unk14++;
        }
        return 0;
    }
    case 0x2B:
        func_80021640(func_8004AF58_inline());
        return 4;
    case 0xB6:
        func_80021640(func_8004AF58_inline());
        return 5;
    case 0xAE:
        func_80021680(func_8004AF58_inline() & 0xFFFF);
        return 7;
    case 0xF5:
        return 8;
    case 0x2A:
        D_8007BD2C = func_8004AEDC_inline();
        D_8006794C->unk1C->unk848 = -1;
        a = func_8004AF58_inline();
        D_8006794C->unk1C->unk571 = (a >> 15) & 1;
        func_80021660(a & 0x7FFF);
        return 3;
    case 0x8C:
        D_8007BD2C = func_8004AEDC_inline();
        D_8006794C->unk1C->unk848 = func_8004AEDC_inline();
        a = func_8004AF58_inline();
        D_8006794C->unk1C->unk571 = (a >> 15) & 1;
        func_80021660(a & 0x7FFF);
        return 3;
    case 0x57:
        D_8007AECC->unk16EF = func_8004AEDC_inline();
        if (D_8007AECC->unk16EF == 0) {
            D_8007AE50 = 0;
        }
        return 0;
    case 0x3C:
        D_8007AE48 = func_8004AEDC_inline();
        D_8007AE40 = func_8004AF58_inline();
        D_8007AE42 = func_8004AF58_inline();
        D_8007AE44 = func_8004AF58_inline();
        D_8007AE46 = func_8004AF58_inline();
        return 0;
    case 0x2C:
        D_8007AECC->unk16ED = D_8007B70C->unk7;
        return 0;
    case 0x2D:
        D_8007AECC->unk16EC = 0;
        func_8004FF98(0);
        D_8007B710 = 0;
        D_8007BBC8 = 0;
        return 0;
    case 0x2E: {
        Unk8007AECC *e;
        D_8007AECC->unk16EC = 1;
        func_8004FF98(1);
        e = D_8007AECC;
        if (e->unk1700 >= 0) {
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
            e->unk1700 = (*D_8007B720++ + 0xC8) * 4;
        }
        return 0;
    }
    case 0x2F:
        po->unk34 = func_8004AF58_inline();
        po->unk3B = func_80021698(D_8007B70C);
        D_8007B70C->unkF |= 1;
        po->unk32 = -(func_8004AEDC_inline() << 2);
        return 0;
    case 0xD4:
        m = func_8004AFF0_inline(D_8007B704);
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        if (m != NULL) {
            m->unk74 = a;
            m->unk76 = b;
            m->unk78 = c;
        }
        return 0;
    case 0x4C:
        m = func_8004AFF0_inline(func_80038150());
        m2 = func_8004AFF0_inline(func_80038150());
        a = func_8004AEDC_inline();
        if (m != NULL && m2 != NULL) {
            m->unk1C = m2;
            m->unk4 = a;
        }
        return 0;
    case 0x4D:
        m = func_8004AFF0_inline(func_80038150());
        if (m != NULL) {
            m->unk1C = NULL;
        }
        return 0;
    case 0x9F:
        obj = func_80038150();
        m = func_8004AFF0_inline(obj);
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AEDC_inline();
        if (m != NULL) {
            m->unk54 = (s8)a << 6;
            m->unk56 = (s8)b << 6;
            m->unk58 = (s8)c << 6;
            m->unk0 |= 2;
        }
        obj->unk2F = b;
        return 0;
    case 0xC0:
        m = func_8004AFF0_inline(func_80038150());
        a = func_8004AEDC_inline();
        if (m != NULL) {
            m->unk50[a].unk4 |= 1;
            m->unk50[a].unkC = 0;
            m->unk50[a].unk12 = 0x1000;
        }
        return 0;
    case 0xC1:
        m = func_8004AFF0_inline(func_80038150());
        a = func_8004AEDC_inline();
        if (m != NULL) {
            m->unk50[a].unk4 |= 3;
            m->unk50[a].unkC = 0;
            m->unk50[a].unk12 = 0x1000;
        }
        return 0;
    case 0xC2:
        m = func_8004AFF0_inline(func_80038150());
        a = func_8004AEDC_inline();
        if (m != NULL) {
            m->unk50[a].unk4 &= 0xFC;
        }
        return 0;
    case 0xCC: {
        u32 *f = (u32 *)func_8001E350(po->unk3B);
        *f |= func_8004AF58_inline();
        return 0;
    }
    case 0xCD: {
        u32 *f = (u32 *)func_8001E350(po->unk3B);
        *f &= ~func_8004AF58_inline();
        return 0;
    }
    case 0x4E:
        str = D_8007B70C->unk0 - 1;
        while (*str++ != 0) {
        }
        func_80052D0C(&str);
        D_8007B6F8 = 1;
        D_8007B70C->unk0 = str + 1;
        return 0;
    case 0x4F:
        D_8007B6F8 = 1;
        D_8007B70C->unk0++;
        return 6;
    case 0x33:
        actor->unk54 = func_8004AF58_inline();
        return 0;
    case 0x34:
        actor->unk56 = func_8004AF58_inline();
        return 0;
    case 0x35:
        actor->unk58 = func_8004AF58_inline();
        return 0;
    case 0x7A:
        actor->unk40.unk20 = func_8004AF58_inline();
        return 0;
    case 0x7B:
        actor->unk40.unk22 = func_8004AF58_inline();
        return 0;
    case 0x52:
        actor->unk74 = func_8004AF58_inline();
        return 0;
    case 0x94:
        actor->unk76 = func_8004AF58_inline();
        actor->unk90 = func_8004AEDC_inline();
        actor->unk91 = func_8004AEDC_inline();
        return 0;
    case 0x3F:
        actor->unk62 = (func_8004AEDC_inline() << 3) & 0x38;
        actor->unk63 = func_8004AEDC_inline();
        return 0;
    case 0x3E:
        actor->unk69 = func_8004AEDC_inline();
        return 0;
    case 0x86:
        actor->unk7C[0] = func_8004AEDC_inline();
        actor->unk7C[1] = func_8004AEDC_inline();
        actor->unk7C[2] = func_8004AEDC_inline();
        actor->unk7C[3] = func_8004AEDC_inline();
        return 0;
    case 0x3D:
        actor->unk60 = func_8004AEDC_inline();
        actor->unk61 = func_8004AEDC_inline();
        return 0;
    case 0xBD:
        actor = func_80038150();
    case 0x40:
        a = func_8004AF58_inline();
        func_80039C38_anim(actor, a);
        if (actor->unk7 == D_8007AECC->unk16ED) {
            D_8007B710++;
        }
        return 0;
    case 0xBE:
        actor = func_80038150();
    case 0x41:
        if (!(actor->unk62 & 0x80)) {
            return 0;
        }
        D_8007B6F8 = 1;
        D_8007B70C->unk0 = D_8007AEA0;
        return 1;
    case 0x42:
        func_800499D8((Unk8004B9B8 *)actor);
        return 0;
    case 0xB5:
        obj = func_80038150();
        if (obj != NULL) {
            if (obj != po) {
                actor->unkA0 = obj;
                actor->unk62 = 0x80;
            } else {
                actor->unkA0 = NULL;
                actor->unk62 = 0;
            }
        }
        return 0;
    case 0x43:
        a = D_8007B70C->unk7 + 0x40;
        obj = func_8004B040_inline(a);
        if (obj != NULL) {
            func_80036978(obj);
        }
        func_800367D0(func_8004AEDC_inline(), a, 0x14, 0)->unk5 = 1;
        return 0;
    case 0x44:
        if (func_8004B040_inline(D_8007B70C->unk7 + 0x40) != NULL) {
            D_8007B6F8 = 1;
            D_8007B70C->unk0 = D_8007AEA0;
            return 1;
        }
        return 0;
    case 0x45:
        obj = func_8004B040_inline(D_8007B70C->unk7 + 0x40);
        if (obj != NULL) {
            func_80036978(obj);
        }
        return 0;
    case 0x38:
        switch (D_8007B70C->unk4) {
        case 0:
            func_800B1520(D_8007AE10[1], D_8007AE10[0], 0x36, func_8004AEDC_inline());
            break;
        case 1:
            func_800B1520(D_8007AE10[1], D_8007AE10[0], 0x35, func_8004AEDC_inline());
            break;
        case 3:
            func_800B1520(D_8007AE10[1], D_8007AE10[0], 0x2F, func_8004AEDC_inline());
            D_8007B70C->unkB = 0;
            break;
        }
        return 0;
    case 0xE5:
        func_800CA694(D_8007AE10[1], D_8007AE10[0], func_8004AEDC_inline());
        return 0;
    case 0x4A:
        a = func_8004AEDC_inline();
        func_800BE99C(a, func_8004AF58_inline());
        return 0;
    case 0x1B:
        if (func_8004AEDC_inline()) {
            D_8006794C->unk1C->unk56F |= 8;
        } else {
            D_8006794C->unk1C->unk56F &= 0xF7;
        }
        return 0;
    case 0xE1:
        func_801F0FF4();
        D_8007B718 = 1;
        return 0;
    case 0xDE:
        a = func_8004AEDC_inline();
        if (a >= 9) {
            a -= 4;
        }
        if ((u32)a < 9) {
            u8 *src = func_8004F058(func_8004AF58_inline());
            u8 *dst = D_8006794C->unk1C->unk3C[a].unk0;
            u8 *end = dst + 9;
            while (dst < end) {
                *dst++ = *src++;
            }
            *dst = 0xFF;
        } else {
            func_8004AF58_inline();
        }
        return 0;
    case 0xDB:
        a = func_8004AEDC_inline();
        if (a >= 9) {
            a -= 4;
        }
        if ((u32)a < 9) {
            Character *ch = &D_8006794C->unk1C->unk3C[a];
            ch->unk34 = (ch->unk34 & ~0x400) | ((func_8004AEDC_inline() & 1) << 10);
        }
        return 0;
    case 0x67:
        *(s16 *)&D_8007AECC->unk16E4[4] = func_8004AEDC_inline() << 4;
        *(s16 *)&D_8007AECC->unk16E4[6] = func_8004AEDC_inline() << 4;
        return 0;
    case 0x6B:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_8001CCB4(a, b, func_8004AEDC_inline());
        return 0;
    case 0x58:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        func_800C0DB0(a, b, func_8004AF58_inline());
        return 0;
    case 0x6F:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004AEDC_inline();
        func_800C0DF4(a, b, c, func_8004AEDC_inline());
        return 0;
    case 0x70:
        a = func_8004AEDC_inline();
        func_800C0038(a, func_8004AEDC_inline());
        return 0;
    case 0x71:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_800C0E94(a, b, func_8004AEDC_inline());
        return 0;
    case 0x72:
        func_800C1024(func_8004AF58_inline());
        return 0;
    case 0x73:
        func_800C1100();
        return 0;
    case 0x74:
        func_800C104C();
        return 0;
    case 0x59:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AEDC_inline();
        func_800C1368(a, b, c, func_8004AEDC_inline());
        return 0;
    case 0x5A:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        func_800C11E8(a, b, c, func_8004AF58_inline());
        return 0;
    case 0x5B:
        a = func_8004AEDC_inline();
        func_800C142C(a, func_8004AEDC_inline());
        return 0;
    case 0x5C:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AF58_inline();
        func_800C147C(a, b, c, func_8004AF58_inline());
        return 0;
    case 0x5D:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AF58_inline();
        func_800C15BC(a, b, c, func_8004AF58_inline());
        return 0;
    case 0x5E:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        func_800C1614(a, b, func_8004AF58_inline());
        return 0;
    case 0x92:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        c = func_8004AEDC_inline();
        d = func_8004AEDC_inline();
        e = func_8004AEDC_inline();
        f = func_8004AEDC_inline();
        func_800C02D0(a, b, c, d, e, f, func_8004AEDC_inline());
        return 0;
    case 0xE6:
        a = func_8004AEDC_inline();
        func_800C14DC(a, func_8004AEDC_inline());
        return 0;
    case 0x5F:
        a = func_8004AEDC_inline();
        func_800C16B0(a, func_8004AEDC_inline());
        return 0;
    case 0x60:
        a = func_8004AEDC_inline();
        func_800C17AC(a, func_8004AEDC_inline());
        return 0;
    case 0x61:
        a = func_8004AEDC_inline();
        func_800C17F0(a, func_8004AF58_inline());
        return 0;
    case 0x63:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_800C1868(a, b, func_8004AEDC_inline());
        return 0;
    case 0x64:
        a = func_8004AEDC_inline();
        func_800C18A0(a, func_8004AEDC_inline());
        return 0;
    case 0x65:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_800C18D4(a, b, func_8004AEDC_inline());
        return 0;
    case 0xCA:
        a = func_8004AEDC_inline();
        func_800C196C(a, func_8004AEDC_inline());
        return 0;
    case 0xE7:
        a = func_8004AEDC_inline();
        func_800C16F4(a, func_8004AEDC_inline());
        return 0;
    case 0x9A:
        a = func_8004AF58_inline();
        func_800C778C(a, func_8004AEDC_inline());
        return 0;
    case 0xCB:
        a = func_8004AEDC_inline();
        func_800C47E4(a, func_8004AEDC_inline());
        return 0;
    case 0xA8:
        a = func_8004AEDC_inline();
        if (po->unk34 != 0xFFFF) {
            func_800C3E18(po->unk3B, a);
        }
        return 0;
    case 0x27:
        func_800C7464(func_8004AEDC_inline());
        return 0;
    case 0x1E:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        d = func_8004AF58_inline();
        func_800C1A8C(a, b, c, d, func_8004AF58_inline());
        return 0;
    case 0xC3:
        a = func_8004AEDC_inline();
        func_800C1668(a, func_8004AEDC_inline());
        return 0;
    case 0xC9:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        d = func_8004AF58_inline();
        func_800C119C(a, b, c, d, func_8004AF58_inline());
        return 0;
    case 0xED:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        func_800C11C8(a, b, func_8004AF58_inline());
        return 0;
    case 0xE4:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AF58_inline();
        d = func_8004AF58_inline();
        func_800C152C(a, b, c, d, func_8004AEDC_inline());
        return 0;
    case 0x7E:
        func_800C0BCC(func_8004AEDC_inline());
        return 0;
    case 0xEA: {
        s16 x, z;
        func_800C0E6C(&x, &z);
        D_8007AE2C = x;
        D_8007AE30 = z;
        return 0;
    }
    case 0x8F:
        obj = func_80038150();
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        if (func_800B2CC0(obj->unk3B, a, b, func_8004AEDC_inline())) {
            D_8007B6F8 = 1;
            D_8007B70C->unk0 = D_8007AEA0;
            return 1;
        }
        return 0;
    case 0x88:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        func_800ABA98(po->unk3B, a, b, c, func_8004AF58_inline());
        return 0;
    case 0x3A:
        m = func_8004AFF0_inline(func_80038150());
        a = func_8004AEDC_inline();
        if (m != NULL) {
            m->unk14 |= 1 << a;
        }
        return 0;
    case 0x39:
        m = func_8004AFF0_inline(func_80038150());
        a = func_8004AEDC_inline();
        if (m != NULL) {
            m->unk14 &= ~(1 << a);
        }
        return 0;
    case 0x68:
        func_80050068(func_8004AEDC_inline());
        return 0;
    case 0xD7:
        func_80050080(func_8004AEDC_inline());
        return 0;
    case 0xEF:
        func_8004FF98(func_8004AEDC_inline());
        return 0;
    case 0x90:
        D_8007AECC->unk1700 = -1;
        return 0;
    case 0xEE:
        D_8007AECC->unk1700 = 0;
        return 0;
    case 0xB9:
        a = func_8004AEDC_inline();
        D_80073998[a] |= func_8004AF58_inline();
        return 0;
    case 0xBA:
        a = func_8004AEDC_inline();
        D_80073998[a] &= ~func_8004AF58_inline();
        return 0;
    case 0xC5:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        D_8006794C->unk1C->unk83C(a, b, 0, 0, 0);
        return 0;
    case 0xC6:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004B914_inline();
        D_8006794C->unk1C->unk83C(a, b, c, 0, 0);
        return 0;
    case 0xC7:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004B914_inline();
        D_8006794C->unk1C->unk83C(a, b, c, func_8004AEDC_inline(), 0);
        return 0;
    case 0xC8:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004B914_inline();
        d = func_8004AEDC_inline();
        D_8006794C->unk1C->unk83C(a, b, c, d, func_8004AEDC_inline());
        return 0;
    case 0x89: {
        u8 vol, pan;
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        func_800B1A20(a, b, func_8004AF58_inline(), &vol, &pan);
        D_8007AE38 = vol;
        D_8007AE3C = (pan * func_8004AEDC_inline()) >> 7;
        if (D_8007AE3C >= 0x80) {
            D_8007AE3C = 0x7F;
        }
        return 0;
    }
    case 0x8A: {
        u8 vol, pan;
        obj = func_80038150();
        func_800B1A20(obj->unk14.vx, obj->unk14.vy, obj->unk14.vz, &vol, &pan);
        D_8007AE38 = vol;
        D_8007AE3C = (pan * func_8004AEDC_inline()) >> 7;
        if (D_8007AE3C >= 0x80) {
            D_8007AE3C = 0x7F;
        }
        return 0;
    }
    case 0xB3:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AF58_inline();
        e = func_8004AF58_inline();
        func_800AFAC0(a, b, c, e, func_8004AF58_inline());
        return 0;
    case 0xDA:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AEDC_inline();
        e = func_8004AF58_inline();
        func_800AFAC0(a, b, c, e, func_8004AF58_inline());
        return 0;
    case 0x75:
        a = func_8004AEDC_inline();
        func_80032120(a, func_8004AEDC_inline());
        func_8004A9E8();
        return 1;
    case 0xAC:
        a = func_8004AF58_inline();
        func_800B5004((a >> 14) & 3, a & 0x3FFF);
        return 1;
    case 0xAA:
        D_80073998[0] &= ~0x40000;
        return 0;
    case 0xAB:
        D_80073998[0] |= 0x40000;
        return 1;
    case 0x48:
        a = func_8004AF58_inline();
        b = func_8004AEDC_inline();
        if (a < 0x100) {
            func_80027D84(a, b);
        } else if (a < 0x200) {
            func_80028074(a - 0x100);
        } else {
            while (b-- != 0) {
                func_8006238C(a - 0x200);
            }
        }
        return 0;
    case 0x49:
        a = func_8004AF58_inline();
        b = func_8004AEDC_inline();
        if (a < 0x100) {
            func_80027E5C(a, b);
        } else if (a < 0x200) {
            func_800280CC(a - 0x100);
        }
        return 0;
    case 0xCE:
        D_8006794C->unk1C->unk560 += func_8004B914_inline();
        if (D_8006794C->unk1C->unk560 > 9999999) {
            D_8006794C->unk1C->unk560 = 9999999;
        }
        return 0;
    case 0xCF:
        D_8006794C->unk1C->unk560 -= func_8004B914_inline();
        if (D_8006794C->unk1C->unk560 > 9999999) {
            D_8006794C->unk1C->unk560 = 0;
        }
        return 0;
    case 0xB4:
        a = func_8004AF58_inline();
        a |= (a >> 4) & 0xE0;
        for (b = 0; b < 9; b++) {
            func_80033C0C(b);
        }
        for (b = 0; b < 9; b++) {
            if (a & 1) {
                func_80033BD0(b);
            }
            a >>= 1;
        }
        return 0;
    case 0xFE:
        a = func_8004AEDC_inline();
        if (a >= 9) {
            a -= 4;
        }
        b = func_8004AEDC_inline();
        c = func_8004AEDC_inline();
        if ((u32)a < 9) {
            Character *ch = &D_8006794C->unk1C->unk3C[a];
            d = func_8004AEDC_inline();
            if (d != 0xFF) {
                ch->unkA = d;
            }
            d = func_8004AEDC_inline();
            if (d != 0xFF) {
                ch->unk34 = (ch->unk34 & 0xFFFE0FFF) | ((d & 0x1F) << 12);
            }
            func_800333B0(a, b & 0xFF, c, d);
        } else {
            func_8004AEDC_inline();
            func_8004AEDC_inline();
        }
        return 0;
    case 0xDD:
        a = func_8004AEDC_inline();
        if (a >= 9) {
            a -= 4;
        }
        for (i = 0; i < 4; i++) {
            Character *ch = D_8006794C->unk1C->unk54C[i];
            if (ch != NULL && (ch->unk34 & 0xF) == a) {
                break;
            }
        }
        if (i < 4) {
            func_80033AF4(i, -1);
            func_80039C38_party();
        }
        return 0;
    case 0xB2:
        a = 0;
        D_8007AE70.unk0 = func_8004AEDC_inline();
        for (i = 8; i >= 0; i--) {
            a = (a << 1) | ((D_8006794C->unk1C->unk3C[i].unk34 >> 11) & 1);
        }
        b = func_8004AF58_inline();
        for (i = 0; i < 9; i++) {
            D_8007AE70.unk10[i] = b & 1;
            b >>= 1;
        }
        for (i = 0; i < 4; i++) {
            Character *ch = D_8006794C->unk1C->unk54C[i];
            if (ch != NULL) {
                b = ch->unk34 & 0xF;
                a &= ~(1 << b);
                D_8007AE70.unk4[i] = b;
            } else {
                D_8007AE70.unk4[i] = 0xFF;
            }
        }
        for (b = 0, i = 0; i < 9 && b < 8 && a != 0; i++, a >>= 1) {
            if (a & 1) {
                D_8007AE70.unk8[b++] = i;
            }
        }
        for (; b < 8; b++) {
            D_8007AE70.unk8[b] = 0xFF;
        }
        func_80032120(3, (s32)&D_8007AE70);
        return 1;
    case 0xE9:
        func_80039C38_party();
        return 0;
    case 0xF1:
        a = func_8004AEDC_inline();
        if (a >= 9) {
            a -= 4;
        }
        if ((u32)a < 9) {
            Character *ch = &D_8006794C->unk1C->unk3C[a];
            b = func_8004AF58_inline();
            if (ch->unk18 < b) {
                b = ch->unk18;
            }
            ch->unk10 = b;
        }
        return 0;
    case 0xF2:
        a = func_8004AEDC_inline();
        if (a >= 9) {
            a -= 4;
        }
        if ((u32)a < 9) {
            Character *ch = &D_8006794C->unk1C->unk3C[a];
            b = func_8004AF58_inline();
            if (ch->unk1A < b) {
                b = ch->unk1A;
            }
            ch->unk12 = b;
        }
        return 0;
    case 0xF3:
        a = func_8004AEDC_inline();
        if (a >= 9) {
            a -= 4;
        }
        func_8003470C(a, func_8004AEDC_inline());
        return 0;
    case 0xF4:
        a = func_8004AEDC_inline();
        if (a >= 9) {
            a -= 4;
        }
        func_80034220(a, func_8004AEDC_inline());
        return 0;
    case 0xD9:
        a = func_8004AEDC_inline();
        if (a >= 9) {
            a -= 4;
        }
        func_80053960(&D_8006794C->unk1C->unk3C[a], func_8004AEDC_inline() & 0xFF);
        return 0;
    case 0x62:
        a = func_8004AEDC_inline();
        if (a >= 9) {
            a -= 4;
        }
        if ((u32)a < 9) {
            Character *ch = &D_8006794C->unk1C->unk3C[a];
            ch->unk34 = (ch->unk34 & ~0x200) | ((func_8004AEDC_inline() & 1) << 9);
        }
        return 0;
    case 0xE0:
        if (D_8006794C->unk1C->unk568 < 0x7FFF) {
            D_8006794C->unk1C->unk568++;
        }
        return 0;
    case 0xB0: {
        u8 *src = func_8004F058(func_8004AF58_inline());
        u8 *dst = D_8007AE90;
        u8 *end = dst + 0xF;
        while (dst < end) {
            *dst++ = *src++;
        }
        *dst = 0xFF;
        D_8006794C->unk1C->unk80C = D_8007AE90;
        return 0;
    }
    case 0xB1:
        D_8006794C->unk1C->unk80C = D_8007AE8C;
        return 0;
    case 0x69:
        func_8002412C(func_8004AF58_inline());
        return 0;
    case 0x8D:
        func_800240B0(func_8004AEDC_inline());
        return 0;
    case 0x7D:
        func_800240F8(func_8004AEDC_inline());
        return 0;
    case 0xEC:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_8002F8BC(a & 1);
        func_8002F93C(((0x80 - func_8004AEDC_inline()) << 4) & 0xFFF0);
        c = func_8004AEDC_inline();
        d = func_8004AEDC_inline();
        func_8002F8E4(1);
        func_800320B8(((a >> 2) & 1) + 1, (a >> 1) & 1, b, c | (d << 8) | (func_8004AEDC_inline() << 16));
        return 0;
    case 0xA9:
        obj = func_80038150();
        if (obj != NULL) {
            sv.vx = obj->unk14.vx;
            sv.vy = obj->unk14.vy;
            sv.vz = obj->unk14.vz;
            func_8004BA20(&sv, &D_8007AE2C, &D_8007AE30);
        }
        return 0;
    case 0x28:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        func_800AD944(a, b, func_8004AEDC_inline());
        return 0;
    case 0xB7:
        if (D_8007B708 == 3) {
            VECTOR *v = func_800AE750();
            actor->unk14.vx = v->vx;
            actor->unk14.vy = v->vy;
            actor->unk50 |= 0x40;
            actor->unk14.vz = v->vz;
        }
        return 0;
    case 0xB8:
        if (D_8007B708 == 3) {
            VECTOR *v = func_800AE744();
            actor->unk14.vx = v->vx;
            actor->unk14.vy = v->vy;
            actor->unk50 |= 0x80;
            actor->unk14.vz = v->vz;
        }
        return 0;
    case 0xC4:
        a = func_8004AEDC_inline();
        func_800B73F8(a, func_8004AF58_inline());
        return 0;
    case 0xD1:
        func_80050A84();
        return 0;
    case 0xF6:
        func_80054FB8(func_8004AEDC_inline());
        return 0;
    case 0xF7:
        func_80054FE0(func_8004AEDC_inline());
        return 0;
    case 0xF8:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_80054E88(a, b, func_8004AEDC_inline());
        return 0;
    case 0xF9:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_80055060(a, b, func_8004AEDC_inline());
        return 0;
    case 0xFA:
        func_800550C0(func_8004AF58_inline());
        return 0;
    case 0xFB:
        func_800550D0(func_8004AEDC_inline());
        return 0;
    case 0xFC:
        a = func_8004AEDC_inline();
        func_800550F0(a, func_8004AEDC_inline());
        return 0;
    case 0xFD:
        a = func_8004AEDC_inline();
        ((void (*)(s32, s32))D_8006794C->unk1C->unk8BC)(a & 0xFF, func_8004AF58_inline() & 0xFFFF);
        return 0;
    case 0xFF:
        op = D_8007B6F4 | 0xFF00;
        D_8007B6F4 = *D_8007B70C->unk0++;
        switch (op) {
        case 0xFF00:
            a = func_8004AEDC_inline();
            func_800C7F34(a, func_8004AEDC_inline());
            return 0;
        case 0xFF01:
            a = func_8004AEDC_inline();
            func_800C7F7C(a, func_8004AEDC_inline());
            return 0;
        case 0xFF02:
            a = func_8004AEDC_inline();
            func_800C7FBC(a, func_8004AEDC_inline());
            return 0;
        case 0xFF03:
            a = func_8004AEDC_inline();
            func_800C8010(a, func_8004AEDC_inline());
            return 0;
        case 0xFF04:
            a = func_8004AEDC_inline();
            func_800C8044(a, func_8004AEDC_inline());
            return 0;
        case 0xFF05:
            a = func_8004AEDC_inline();
            func_800C8068(a, func_8004AF58_inline());
            return 0;
        case 0xFF06:
            a = func_8004AEDC_inline();
            func_800C808C(a, func_8004AEDC_inline());
            return 0;
        case 0xFF07:
            a = func_8004AEDC_inline();
            b = func_8004AF58_inline();
            func_800C80B0(a, b, func_8004AF58_inline());
            return 0;
        case 0xFF08:
            a = func_8004AEDC_inline();
            func_800C80E4(a, func_8004AEDC_inline());
            return 0;
        case 0xFF09:
            a = func_8004AEDC_inline();
            func_800C7AF8(a, func_8004AEDC_inline());
            return 0;
        case 0xFF0A:
            a = func_8004AEDC_inline();
            func_800C6408(a, func_8004AEDC_inline());
            return 0;
        case 0xFF0B:
            a = func_8004AEDC_inline();
            func_800C7B68(a, func_8004AEDC_inline());
            return 0;
        case 0xFF0C:
            a = func_8004AEDC_inline();
            func_800C7CDC(a, func_8004AEDC_inline());
            return 0;
        case 0xFF0D:
            a = func_8004AEDC_inline();
            func_800C7BC4(a, func_8004AF58_inline());
            return 0;
        case 0xFF0E:
            a = func_8004AEDC_inline();
            func_800C7C04(a, func_8004AEDC_inline());
            return 0;
        case 0xFF0F:
            a = func_8004AEDC_inline();
            b = func_8004AEDC_inline();
            func_800C7C70(a, b, func_8004AEDC_inline());
            return 0;
        case 0xFF10:
            a = func_8004AEDC_inline();
            b = func_8004AEDC_inline();
            func_800C7D30(a, b, func_8004AEDC_inline());
            return 0;
        case 0xFF11:
            a = func_8004AEDC_inline();
            func_800C7DF0(a, func_8004AEDC_inline());
            return 0;
        }
        return 0;
    default:
        switch (D_8007B708) {
        case 1:
            return func_800500A8(po, op);
        case 2:
            return func_80050410(po, op);
        case 3:
            return func_80050744(po, op);
        }
        return 1;
    }
}
