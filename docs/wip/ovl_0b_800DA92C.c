/* WIP func_800DA92C: 2 differing rows. Left: case 0 header-load order (original: lw hdr[1], hdr[0], hdr[2];
   ours always schedules the hdr[1] load second, whatever the statement order). Fixed in pass 6: dst reused as
   the base pointer in cases 4/5, base used for the case 9 pointer and m for its bit mask. */
#include "ovl_0b.h"

extern u16 D_800FEC68[];
extern s32 D_80100D90[][2];
s32 func_800E8AD0(void);
s32 func_800DA8EC(void);
s32 func_800DA634(s32);
void func_800DA790(RECT *, u32 *, Unk80100DC0 *);
void func_800DA71C(RECT *);
void func_800DC748(BtlModel *, s32);
typedef void (*Unk800DA92CFn)(s32, Unk800F78A4 *, s32);

void func_800DA92C(void) {
    union {
        RECT r;
        s32 w;
    } r;
    Unk80100DC0 *d = &D_80100DC0;
    s32 cont;
    s16 st;
    u16 *p;
    u8 *dst;
    u32 rd;
    s32 sz;
    s32 k;
    s32 x;
    s32 m;
    u8 *src;
    Unk80100DC054 *e2;
    s32 i;
    s32 n;
    s32 cnt;
    s32 *hdr;
    u16 *q;
    Unk80100DC054 *e;
    u8 *base;
    s32 bit;
    Heap *h;
    s32 ni;
    u8 *fh;
    Unk800DA92CFn fn;

    if (d->unkC != 0) {
        do {
            st = d->unkC - 1;
            cont = 0;
            switch (st) {
            case 0:
                func_80022C60(d->unk0, 0x800, D_800FEC68, NULL);
                d->unkC = 2;
                d->unk0++;
                break;
            case 1:
                if (func_80022B18() == 0) {
                    p = D_800FEC68;
                    cont = 1;
                    d->unk1C = p + 1;
                    d->unkC = 3;
                    d->unk18 = p[0];
                }
                break;
            case 2:
                if (d->unk1A < d->unk18) {
p = d->unk1C; cont = 1; d->unk10 = p[0]; d->unk12 = p[1]; d->unk1C += 2; d->unkC = 4;
                } else {
                    d->unkC = 0;
                }
                break;
            case 3:
                p = d->unk1C;
                d->unk16 = ((u8 *)p)[0];
                d->unk17 = ((u8 *)p)[1];
                if (D_800F78A4->unkC->unk40 & 8) {
                    if (d->unk16 == 4 || d->unk16 == 9) {
                        if (func_800E8AD0() != 0) {
                            break;
                        }
                    }
                }
                dst = (u8 *)0x801E7700;
                if (d->unk10 == 1 && d->unk8 != 0) {
                    dst = (u8 *)0x801EC700;
                }
                rd = p[1] << 11;
                p += 2;
                switch (d->unk16) {
                case 2:
                    sz = rd;
                    d->unkE = 1;
                    if (d->unk17 != 0) {
x = *p++ << 11; rd += x; d->unk20 = (u8 *)0x801F1700 + x; dst = (u8 *)0x801F1700;
                        d->unk24 = 0xC800 - x;
                        d->unk30[d->unk8] = d->unk28 = d->unk20;
                    } else {
                        dst = d->unk30[d->unk8] = d->unk28;
                    }
                    d->unk28 += sz;
                    if (d->unk28 >= d->unk20 + d->unk24) {
                        d->unk28 = d->unk20;
                    }
                    break;
                case 10:
                    dst = d->unk28;
                    d->unk28 = dst + rd;
                    break;
                case 1:
                    dst = (u8 *)d->unk4C;
                    break;
                case 3:
                    d->unkE = 2;
                    break;
                case 4:
                    d->unk50 = 1;
                    func_800AB43C();
                    dst = (u8 *)D_8006794C->unk1C->unk8E4.battle->unk153C[0];
                    d->unk54 = (Unk80100DC054 *)dst;
                    ((Unk80100DC054 *)dst)->unk4C = 0;
                    for (i = 0; i < 16; i++) {
                        d->unk54->unk50[i] = 0;
                    }
                    break;
                case 5:
                    dst = (u8 *)d->unk54 + d->unk54->unk0;
                    break;
                case 6:
                    d->unk52 = 1;
                    e2 = d->unk54;
                    d->unk58 = (s32)e2->unk44;
                    d->unk5C = e2->unk48;
                    dst = (u8 *)d->unk58;
                    e2->unk50[0] = (s32 *)dst;
                    d->unk5C -= rd;
                    d->unk58 += rd;
                    break;
                case 7:
                    d->unk64 = 1;
                    dst = (u8 *)D_8006794C->unk1C->unk8E4.battle->unk11C0->unkC;
                    break;
                case 8:
                    d->unk68 = 1;
                    dst = (u8 *)D_8006794C->unk1C->unk8E4.battle->unk11C0->unkC;
                    break;
                case 9:
                    if (D_800F78A4->unkC->unk40 & 8) {
                        func_800AB43C();
                        dst = (u8 *)D_8006794C->unk1C->unk8E4.battle->unk153C[0];
                    } else {
                        dst = (u8 *)D_8006794C->unk1C->unk8E4.battle->unk11C0->unkC;
                    }
                    break;
                }
                d->unk1C = p;
                func_80022C60(d->unk0, rd, dst, NULL);
                d->unkC = 5;
                d->unk0 += rd >> 11;
                break;
            case 4:
                if (func_80022B18() != 0) {
                    break;
                }
                if (d->unk16 == 9 && (d->unk17 & 0xF) && D_800FEC65 == 0) {
                    break;
                }
                if (d->unk16 == 3 && func_800DA8EC() == 0) {
                    break;
                }
                switch (d->unk16) {
                case 2:
                    if (d->unk17 != 0 && func_800DA480(0, 0x40) != 0) {
                        d->unk38 = func_800DA3D8(0, 0x40);
                    }
                    m = func_800DA480(d->unk8, 0);
                    if (m != 0) {
                        h = (Heap *)func_800DA3D8(d->unk8, 0);
                        d->unk3C[d->unk8] = h;
                        func_800E3B34(h, m);
                    } else {
                        d->unk3C[d->unk8] = NULL;
                    }
                    break;
                case 0:
                    dst = (u8 *)0x801E7700;
                    if (d->unk10 == 1 && d->unk8 != 0) {
                        dst = (u8 *)0x801EC700;
                    }
                    hdr = (s32 *)dst;
                    q = (u16 *)(dst + hdr[1]);
                    k = hdr[0];
                    cnt = hdr[2];
                    func_800D0940();
                    for (i = 0; i < cnt; i++) {
                        r.r.x = q[0];
                        r.r.y = q[1];
                        r.r.w = q[2];
                        r.r.h = q[3];
                        n = q[2] * q[3];
                        q += 4;
                        LoadImage(&r.r, (u32 *)q);
                        q += n;
                    }
                    hdr = (s32 *)(dst + k);
                    q = (u16 *)(dst + hdr[0]);
                    d->unk44 = (RECT *)(hdr + 2);
                    d->unk48 = 0;
                    d->unk4C = (u32 *)q;
                    func_800DA790(&r.r, (u32 *)q, d);
                    break;
                case 1:
                    func_800DA790(&r.r, d->unk4C, d);
                    break;
                case 3:
                    dst = (u8 *)0x801E7700;
                    if (d->unk10 == 1 && d->unk8 != 0) {
                        dst = (u8 *)0x801EC700;
                    }
                    fh = *(u8 **)dst;
                    D_80100C70[d->unk8] = (EffFn *)(fh + 8);
                    fn = ((Unk800DA92CFn *)fh)[1];
                    if (fn != NULL) {
                        fn(d->unk38, D_800F78A4, D_800F78A4->unk10);
                    }
                    d->unkE = 3;
                    d->unk2C.h[d->unk8] = d->unk1A;
                    break;
                case 4:
                    cnt = 0;
                    e = d->unk54;
                    dst = (u8 *)e + e->unk0;
                    r.r.x = 0x100;
                    r.r.y = 0xE6;
                    r.r.w = 0x100;
                    r.r.h = e->unk6;
                    func_800D0940();
                    LoadImage(&r.r, (u32 *)(dst + e->unk8));
                    k = e->unk4;
                    for (src = dst; cnt < k; src += 0x4000, cnt++) {
                        r.r.x = (e->unk18[cnt] & 0xF) << 6;
                        r.r.y = e->unk30[cnt] + ((e->unk18[cnt] & 0x10) << 4);
                        r.r.w = 0x40;
                        r.r.h = 0x80;
                        LoadImage(&r.r, (u32 *)src);
                        ((s32 *)D_80100D90)[cnt * 2] = ((s32 *)D_80100D90)[cnt * 2 + 1] = r.w;
                    }
                    break;
                case 5:
                    e = d->unk54;
                    dst = (u8 *)e;
                    e->unk3C = dst + e->unk0;
                    e->unk40 = (BtlUnit9C50 *)(dst + e->unk14);
                    for (ni = 0; ni < e->unk2; ni++) {
                        e->unk180[ni] += (s32)dst;
                    }
                    e->unk44 = dst + e->unk10;
                    d->unk58 = (s32)e->unk44;
                    e->unk48 = 0x50000 - e->unk10;
                    d->unk5C = e->unk48;
                    func_800DC748(&e->unk90, 1);
                    d->unk60 = func_800DB8E0(e, e->unk180[0] != (s32)e->unk40 ? (s16 *)D_80100D90 : NULL);
                    d->unk50 = 2;
                    break;
                case 6:
                    d->unk52 = 2;
                    break;
                case 7:
                    d->unk64 = 2;
                    d->unk66 = 1;
                    if (d->unk65 == 0) {
                        func_800DA634(1);
                    }
                    break;
                case 8:
                    d->unk68 = 2;
                    break;
                case 9:
                    m = 1;
                    d->unk53 = 1;
                    if (D_800F78A4->unkC->unk40 & 8) {
                        func_800AB43C();
                        base = (void *)D_8006794C->unk1C->unk8E4.battle->unk153C[0];
                    } else {
                        base = (void *)D_8006794C->unk1C->unk8E4.battle->unk11C0->unkC;
                    }
                    r.r.w = 0x40;
                    r.r.h = 0x80;
                    func_800D0940();
                    for (i = 0; i < 8; i++) {
                        if (m & d->unk17) {
                            if (i < 4) {
                                r.r.x = ((i >> 1) << 6) + 0x300;
                            } else {
                                r.r.x = (((i - 4) >> 1) << 6) + 0x140;
                            }
                            r.r.y = ((i & 1) << 7) + 0x100;
                            LoadImage(&r.r, (u32 *)base);
                            func_800DA71C(&r.r);
                            base += 0x4000;
                        }
                        if (i >= 4 || (i & 1)) {
                            m <<= 1;
                        }
                    }
                    d->unk69++;
                    break;
                }
                if (--d->unk12 <= 0) {
                    d->unkC = 0;
                    break;
                }
                if (d->unk16 == 7) {
                    if (d->unk65 == 0) {
                        d->unkC = 6;
                    } else {
                        d->unkC = 7;
                    }
                    cont = 0;
                    d->unk65++;
                } else {
                    cont = 0;
                    if (d->unk16 < 2 || d->unk16 == 4 || d->unk16 == 9) {
                        cont = 0;
                    } else {
                        cont = 1;
                    }
                    d->unkC = 4;
                }
                break;
            case 5:
                if (func_80055DCC() == 0) {
                    d->unkC = 4;
                    cont = 1;
                }
                break;
            case 6:
                if (d->unk67 != 0) {
                    d->unkC = 6;
                    d->unk67 = 0;
                }
                break;
            }
        } while (cont);
    }
}
