#include "ovl_0b.h"

s32 func_800DA0FC(void) {
    return 0;
}

s32 btlfx_poolSize(s32 arg0, s32 arg1) {
    return (btlfx_align4(arg0) + 4) * arg1 + 0xC;
}

void btlfx_poolInit(Unk800DA1B8 *arg0, Unk80100C78Entry *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sz;
    s32 i;
    u8 *p;

    /* sz in two statements and the do-while around p += sz give sz and p the reference counts
       the original's register allocation needs (sz v1, p a0, i a1). */
    sz = btlfx_align4(arg2);
    sz += 4;
    p = (u8 *)arg0->unkC;
    i = 0;
    arg0->unk0 = sz;
    arg0->unk2 = arg3;
    arg0->unk8 = (Unk800DA1B88 *)arg1;
    arg0->unk4 = (void *)arg4;
    if (arg3 > 0) {
    loop:
        *(s32 *)p = 0;
        i++;
        do {
            p += sz;
        } while (0);
        if (i < arg3) goto loop;
    }
}

void btlfx_poolClear(Unk800DA1B8 *arg0) {
    s32 i;
    s32 *p = arg0->unkC;
    for (i = 0; i < arg0->unk2; i++) {
        *p = 0;
        p = (s32 *)((u8 *)p + arg0->unk0);
    }
}

void *btlfx_poolSpawn(s16 *p) {
    Unk800DA1B8 *pool;
    Unk800DA1B8 *x;
    s16 *e;
    s32 i;
    s32 stride;
    u8 *d;

    x = (Unk800DA1B8 *)p;
    pool = (Unk800DA1B8 *)p;
    p = (s16 *)((Unk800DA1B8 *)p)->unkC;
    stride = x->unk0;
    for (i = 0; i < pool->unk2; i++) {
        e = p;
        if (*p == 0) {
            break;
        }
        p = (s16 *)((u8 *)p + stride);
    }
    if (i >= pool->unk2) {
        return NULL;
    }
    d = (u8 *)(p + 2);
    e[0] = 1;
    e[1] = 0;
    pool->unk4(1, d, g_btlfxEnv, pool->unk8, 0, pool, pool->unk8->unkC);
    /* Probably macro residue: a dead store that flow deletes (no code). Its only effect is that p is
       mentioned last, which keeps p (not pool) the CSE class head so `p += 12` and the header reads stay
       on the original registers. */
    p = NULL;
    return d;
}

s32 btlfx_poolUpdate(Unk800DA1B8 *arg0) {
    Unk800DA1B8 *pool = arg0;
    s16 *p = (s16 *)pool->unkC;
    s32 i = 0;
    s32 n = i;
    s32 (*fn)(s32, void *, Unk800F78A4 *, Unk800DA1B88 *, s32, struct Unk800DA1B8 *, s32) = pool->unk4;
    s32 stride = pool->unk0;
    for (; i < pool->unk2; i++, p = (s16 *)((u8 *)p + stride)) {
        if (p[0] != 0) {
            g_btlfxEnv->unk28 = 0;
            n++;
            if (fn(2, p + 2, g_btlfxEnv, pool->unk8, p[1], pool, pool->unk8->unkC) != 0) {
                *(s32 *)p = 0;
            } else {
                p[1]++;
            }
        }
    }
    return n;
}

s32 btlfx_findFileSlot(s32 arg0) {
    s32 i;
    for (i = 0; i < 2; i++) {
        if (arg0 == g_btlfxLoader.unk2C.h[i]) {
            break;
        }
    }
    return i;
}

s32 btlfx_getFileData(s32 arg0, s32 arg1) {
    s32 *base;
    if (arg1 < 0x40) {
        base = g_btlfxEnv->unk4->unk30[arg0];
    } else if (arg1 < 0x80) {
        base = (s32 *)0x801F1700;
        arg1 -= 0x40;
    } else if (arg1 < 0xC0) {
        arg1 -= 0x80;
        base = D_80100E14->unk50[0];
        arg1 &= 0x7F;
    } else {
        base = g_gameState->unk1C->unk8E4.battle->unk11C0->unkC;
        arg1 -= 0xC0;
    }
    return (s32)base + base[arg1];
}

s32 btlfx_getFileDataSize(s32 arg0, s32 arg1) {
    s32 v = btlfx_getFileData(arg0, arg1);
    return btlfx_getFileData(arg0, arg1 + 1) - v;
}

/* takes arg0 bytes from the effect work area (g_btlfxLoader.unk58/unk5C), returns the old pointer */
s32 func_800DA4C4(s32 arg0) {
    Unk80100DC0 *d = &g_btlfxLoader;
    s32 p = d->unk58;

    d->unk5C -= arg0;
    d->unk58 = p + arg0;
    return p;
}

void func_800DA4E8(void) {
    D_80100DCC = 0;
}

s32 btlfx_loadFile(s32 arg0) {
    s32 ofs;
    s32 base;

    if (g_btlfxLoader.unkC != 0) {
        return -1;
    }
    ofs = ((FileEntry16 *)file_find(0xD, arg0))->unk0;
    base = g_gameState->unk1C->unk8E4.battle->dataSector;
    g_btlfxLoader.unk2C.w = -1;
    g_btlfxLoader.unk4 = arg0;
    g_btlfxLoader.unk8 = 0;
    g_btlfxLoader.unk1A = 0;
    g_btlfxLoader.unkC = 1;
    g_btlfxLoader.unkE = 0;
    g_btlfxLoader.unk50 = 0;
    g_btlfxLoader.unk52 = 0;
    g_btlfxLoader.unk53 = 0;
    g_btlfxLoader.unk68 = 0;
    g_btlfxLoader.unk69 = 0;
    g_btlfxLoader.unk64 = 0;
    g_btlfxLoader.unk66 = 0;
    g_btlfxLoader.unk65 = 0;
    g_btlfxLoader.unk67 = 0;
    g_btlfxLoader.unk38 = 0;
    g_btlfxLoader.unk3C[0] = 0;
    g_btlfxLoader.unk3C[1] = 0;
    g_btlfxLoader.unk0 = base + ofs;
    return 0;
}

s32 func_800DA5C4(void) {
    if (g_btlfxLoader.unkC != 0) {
        return -1;
    }
    g_btlfxLoader.unkC = 3;
    g_btlfxLoader.unkE = 0;
    g_btlfxLoader.unk50 = 0;
    g_btlfxLoader.unk68 = 0;
    g_btlfxLoader.unk64 = 0;
    g_btlfxLoader.unk65 = 0;
    g_btlfxLoader.unk67 = 0;
    g_btlfxLoader.unk1A++;
    g_btlfxLoader.unk8 = (g_btlfxLoader.unk8 + 1) & 1;
    g_btlfxLoader.unk2C.h[g_btlfxLoader.unk8] = -1;
    return 0;
}

s32 func_800DA634(s32 arg0) {
    if (arg0 == 0) {
        if (D_80100DCC != 7 && D_80100DCC != 0) {
            return 1;
        }
    }
    func_800562E0(g_gameState->unk1C->unk8E4.battle->unk11C0->unkC, 0);
    g_btlfxLoader.unk67 = 1;
    g_btlfxLoader.unk66 = 1;
    return 0;
}

s32 func_800DA6B4(s32 arg0, s32 arg1) {
    if (g_btlfxLoader.unk66 != 0) {
        if (func_80055DCC() == 0) {
            g_btlfxLoader.unk66 = 0;
            func_80056388(0x80, arg1);
            return 0;
        }
    }
    return 1;
}

void func_800DA71C(RECT *arg0) {
    if (arg0->x >= 0x300 && arg0->x + arg0->w < 0x381) {
        if (arg0->y >= 0x100 && arg0->y + arg0->h < 0x201) {
            g_btlfxState.unk1C |= 1;
        }
    }
}

void func_800DA790(RECT *arg0, u32 *arg1, Unk800DA790 *arg2) {
    s32 i;
    s32 n = arg2->unk17;
    Unk800DA790 *e;
    RECT *r;
    arg0->w = 0x40;
    arg0->h = 0x80;
    func_800D0940();
    for (i = 0; i < n; i++) {
        e = arg2;
        r = e->unk44;
        if (++e->unk48 >= 2) {
            arg0->x = r->x;
            arg0->y = r->y + 0x80;
            goto next;
        }
        arg0->x = r->x;
        arg0->y = r->y;
        if ((u16)r->h == 0x80) {
        next:
            e->unk48 = 0;
            e->unk44++;
        }
        LoadImage(arg0, arg1);
        func_800DA71C(arg0);
        arg1 += 0x1000;
    }
}

void func_800DA898(void) {
    if (D_800F750C == 2) {
        EnterCriticalSection();
        if (DrawSync(1) == 0) {
            D_800F750C = 1;
            FlushCache();
        }
        ExitCriticalSection();
    }
}

s32 func_800DA8EC(void) {
    s32 ret = 0;
    switch (D_800F750C) {
    case 0:
        D_800F750C = 2;
        break;
    case 1:
        D_800F750C = 0;
        ret = 1;
        break;
    case 2:
        break;
    }
    return ret;
}

void btlfx_updateLoader(void) {
    union {
        RECT r;
        s32 w;
    } r;
    Unk80100DC0 *d = &g_btlfxLoader;
    s32 cont;
    s16 st;
    u16 *p;
    u8 *dst;
    u32 rd;
    s32 sz;
    s32 k;
    s32 cnt2;
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
                cd_readAsync(d->unk0, 0x800, D_800FEC68, NULL);
                d->unkC = 2;
                d->unk0++;
                break;
            case 1:
                if (cd_getState() == 0) {
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
                __asm__(".section .rodata\n\t.word 0\n\t.section .text");
                p = d->unk1C;
                d->unk16 = ((u8 *)p)[0];
                d->unk17 = ((u8 *)p)[1];
                if (g_btlfxEnv->unkC->unk40 & 8) {
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
                    dst = (u8 *)g_gameState->unk1C->unk8E4.battle->unk153C[0];
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
                    dst = (u8 *)g_gameState->unk1C->unk8E4.battle->unk11C0->unkC;
                    break;
                case 8:
                    d->unk68 = 1;
                    dst = (u8 *)g_gameState->unk1C->unk8E4.battle->unk11C0->unkC;
                    break;
                case 9:
                    if (g_btlfxEnv->unkC->unk40 & 8) {
                        func_800AB43C();
                        dst = (u8 *)g_gameState->unk1C->unk8E4.battle->unk153C[0];
                    } else {
                        dst = (u8 *)g_gameState->unk1C->unk8E4.battle->unk11C0->unkC;
                    }
                    break;
                }
                d->unk1C = p;
                cd_readAsync(d->unk0, rd, dst, NULL);
                d->unkC = 5;
                d->unk0 += rd >> 11;
                break;
            case 4:
                if (cd_getState() != 0) {
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
                    if (d->unk17 != 0 && btlfx_getFileDataSize(0, 0x40) != 0) {
                        d->unk38 = btlfx_getFileData(0, 0x40);
                    }
                    m = btlfx_getFileDataSize(d->unk8, 0);
                    if (m != 0) {
                        h = (Heap *)btlfx_getFileData(d->unk8, 0);
                        d->unk3C[d->unk8] = h;
                        btlfx_heapInit(h, m);
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
                    cnt2 = hdr[2];
                    func_800D0940();
                    for (i = 0; i < cnt2; i++) {
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
                    func_800DA790(&r.r, (u32 *)q, (Unk800DA790 *)d);
                    break;
                case 1:
                    func_800DA790(&r.r, d->unk4C, (Unk800DA790 *)d);
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
                        fn(d->unk38, g_btlfxEnv, g_btlfxEnv->unk10);
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
                    btlfx_initSummonModels(&e->unk90, 1);
                    d->unk60 = Hi_RegisterSummonModel(e, e->unk180[0] != (s32)e->unk40 ? (s16 *)D_80100D90 : NULL);
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
                    if (g_btlfxEnv->unkC->unk40 & 8) {
                        func_800AB43C();
                        base = (void *)g_gameState->unk1C->unk8E4.battle->unk153C[0];
                    } else {
                        base = (void *)g_gameState->unk1C->unk8E4.battle->unk11C0->unkC;
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
