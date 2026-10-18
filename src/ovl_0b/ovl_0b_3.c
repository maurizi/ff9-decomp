#include "ovl_0b.h"

void func_800C8BE0(BtlUnit9C *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    BtlMesh *g = &arg0->unk8->unk10[arg1];
    u32 *p = g->unk1C;
    u32 i;
    for (i = 0; i < g->unk2; i++) {
        p[4] = btl_addRgbClamped(p[4], arg2, arg3, arg4);
        p += 6;
    }
    for (i = 0; i < g->unk4; i++) {
        p[2] = btl_addRgbClamped(p[2], arg2, arg3, arg4);
        p += 5;
    }
}

void func_800C8D18(BtlUnit9C *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    BtlMesh *g = &arg0->unk8->unk10[arg1];
    u32 *p = g->unk1C;
    u32 v = (arg4 << 16) | (arg3 << 8) | arg2;
    u32 i;
    for (i = 0; i < g->unk2; i++) {
        p[4] = (p[4] & 0xFF000000) | v;
        p += 6;
    }
    for (i = 0; i < g->unk4; i++) {
        p[2] = (p[2] & 0xFF000000) | v;
        p += 5;
    }
}

u32 btl_addRgbClamped(u32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 b = arg3 + ((arg0 >> 16) & 0xFF);
    s16 g = arg2 + ((arg0 >> 8) & 0xFF);
    s16 r = arg1 + (arg0 & 0xFF);
    arg0 &= 0xFF000000;
    if (r >= 0x100) {
        arg0 |= 0xFF;
    } else if (r > 0) {
        arg0 |= r;
    }
    if (g >= 0x100) {
        arg0 |= 0xFF00;
    } else if (g > 0) {
        arg0 |= g << 8;
    }
    if (b >= 0x100) {
        arg0 |= 0xFF0000;
    } else if (b > 0) {
        arg0 |= b << 16;
    }
    return arg0;
}

void btl_setModelAbr(BtlUnit9C *arg0, s32 arg1) {
    u8 *p;
    u32 i;
    u8 code;
    if (!(arg0->unk0 & 0x10)) {
        arg0->unk5 = arg1;
        p = (u8 *)arg0->unk10;
        for (i = 0; i < arg0->unk8->unk3; i++) {
            if (!(arg0->unk14 & (1 << i))) {
                do {
                    code = getcode(p);
                    if (code == 0xFF) {
                        p += sizeof(DR_TPAGE);
                    } else if (code == 0xE1) {
                        ((DR_TPAGE *)p)->code[0] = (u16)arg1 | 0xE1000000;
                        p += sizeof(DR_TPAGE);
                    } else {
                        code &= 0x3C;
                    }
                    switch (code) {
                    case 0x2C:
                        setSemiTrans(p, 1);
                        ((POLY_FT4 *)p)->tpage = arg1 | (((POLY_FT4 *)p)->tpage & ~0x60);
                        p += sizeof(POLY_FT4);
                        break;
                    case 0x24:
                        setSemiTrans(p, 1);
                        ((POLY_FT3 *)p)->tpage = arg1 | (((POLY_FT3 *)p)->tpage & ~0x60);
                        p += sizeof(POLY_FT3);
                        break;
                    case 0x3C:
                        setSemiTrans(p, 1);
                        ((POLY_GT4 *)p)->tpage = arg1 | (((POLY_GT4 *)p)->tpage & ~0x60);
                        p += sizeof(POLY_GT4);
                        break;
                    case 0x34:
                        setSemiTrans(p, 1);
                        ((POLY_GT3 *)p)->tpage = arg1 | (((POLY_GT3 *)p)->tpage & ~0x60);
                        p += sizeof(POLY_GT3);
                        break;
                    case 0x38:
                        setSemiTrans(p, 1);
                        p += sizeof(POLY_G4);
                        break;
                    case 0x30:
                        setSemiTrans(p, 1);
                        p += sizeof(POLY_G3);
                        break;
                    case 0x28:
                        p += sizeof(POLY_F4);
                        break;
                    case 0x20:
                        p += sizeof(POLY_F3);
                        break;
                    }
                } while (code != 0xFF);
            }
        }
    }
}

void func_800C9040(TexAnimSet *arg0) {
    RECT r;
    u32 i;
    u32 j;
    TexAnim *e;
    TexAnimRect *t;
    s32 pos;
    s16 frame;
    s16 last;

    if (arg0 == NULL) {
        return;
    }
    for (i = 0; i < arg0->unk0; i++) {
        e = &arg0->unk4[i];
        if (!(e->unk0 & 1)) {
            continue;
        }
        if (e->unk1 != 0) {
            pos = e->unk8;
            frame = pos >> 12;
            last = e->unkE;
            if (frame >= 0) {
                if (frame != last) {
                    for (j = 0; j < e->unkC; j++) {
                        t = &e->unk10[j];
                        r.x = t->unk8[frame][0];
                        r.y = t->unk8[frame][1];
                        r.w = t->unk4;
                        r.h = t->unk6;
                        MoveImage(&r, t->unk0, t->unk2);
                    }
                    e->unkE = frame;
                }
                pos += e->unk2;
            } else {
                pos += 0x1000;
            }
            if ((pos >> 12) < e->unk1) {
                e->unk8 = pos;
            } else if (e->unk6 != 0) {
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                    }
                }
                e->unk8 = -((e->unk4 + (e->unk6 + 1) * ((*g_randPtr << 8) | *g_randPtr++) / 0x10000) << 12);
            } else if (e->unk0 & 2) {
                e->unk0 &= 0xFC;
            } else {
                e->unk8 = 0;
            }
        } else if (e->unk0 & 4) {
            pos = e->unk8;
            frame = pos >> 12;
            for (j = 0; j < e->unkC; j++) {
                t = &e->unk10[j];
                MoveImage((RECT *)t, t->unk0, t->unk2 - frame);
                r.x = t->unk0;
                r.y = t->unk2 - frame;
                r.w = t->unk4;
                r.h = frame;
                MoveImage(&r, t->unk0, t->unk2 + t->unk6 - frame);
            }
            e->unk8 = (pos & 0xFFF) + e->unk2;
        }
    }
}

void func_800C9374(TexAnimSet *set, u16 (*ofs)[4]) {
    u32 i;
    u32 j;
    u32 k;
    TexAnimRect *r;
    u16 *f;
    u16 *g;
    u8 idx;

    if (set == NULL) {
        return;
    }
    for (i = 0; i < set->unk0; i++) {
        set->unk4[i].unkE = 1;
        f = (u16 *)set->unk4[i].unk10;
        if ((u32)f > 0x80000000) {
            return;
        }
        if (f == NULL) {
            continue;
        }
        idx = set->unk4[i].unkD;
        f = (u16 *)((u8 *)f + (s32)set);
        set->unk4[i].unk10 = (TexAnimRect *)f;
        if (ofs != NULL) {
            f[0] += ofs[idx][2];
            f[1] += ofs[idx][3];
        }
        r = (TexAnimRect *)f;
        for (j = 0; j < set->unk4[i].unkC; j++) {
            f = (u16 *)r[j].unk8;
            f = (u16 *)((u8 *)f + (s32)set);
            r[j].unk8 = (void *)f;
            if (ofs != NULL) {
                for (k = 0; k < set->unk4[i].unk1; k++) {
                    g = f + 1;
                    *f += ofs[idx][0];
                    *g += ofs[idx][1];
                    f += 2;
                }
            }
        }
    }
}

Battle11C020 *func_800C94B8(void) {
    Battle *b;
    Battle11C020 *s = g_gameState->unk1C->unk8E4.battle->unk11C0->unk20;
    u16 *t = g_gameState->unk1C->unk8E4.battle->unk11C0->unk24;
    s32 r;
    Battle132C *sc = &g_gameState->unk1C->unk8E4.battle->unk132C;
    u8 n;
    u8 m;

    sc->unkC = (u16 *)((u8 *)t + t[0]);
    sc->unk10 = (s16 *)((u8 *)t + t[1]);
    sc->unk4 = func_800C9600(g_gameState->unk1C->unk8E4.battle->unk11C0->unk20);
    sc->unk8 = (Battle1334 *)&g_gameState->unk1C->unk8E4.battle->unk11C0->unk20->unk8[s->unk1];
    n = sc->unk4->unk2;
    b = g_gameState->unk1C->unk8E4.battle;
    if (n >= 3) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        r = *g_randPtr++;
        m = r % 3;
    } else {
        m = n;
    }
    b->unk131C = m;
    return s;
}

Battle1330 *func_800C9600(Battle11C020 *arg0) {
    Battle1330 *e;
    u16 r;
    u16 sum;
    u16 i;
    s8 v;
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    r = (*g_randPtr++ * 100) / 256;
    e = arg0->unk8;
    sum = e->unk0;
    i = 0;
    while (r >= sum) {
        e++;
        sum += e->unk0;
        i++;
    }
    v = g_gameState->unk1C->unk848;
    if (v != -1) {
        i = v;
        if (i >= arg0->unk1) {
            i = 0;
        }
        e = &arg0->unk8[i];
    }
    g_gameState->unk1C->unk8E4.battle->unk1340 = i;
    return e;
}

void func_800C972C(void) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    b->unk132C.unk0 = (b->unk11C0->unk20->unk4 ^ 0x30) * 4;
    D_800FDF38 = g_gameState->unk1C->unk8E4.battle->unk132C.unk4->unk4;
}

void func_800C9794(void) {
    Battle *b;
    Battle1330 *grp;
    Battle11C020 *hdr;
    s16 i;
    s16 k;
    u8 n;
    Unk800C9D1C *recs;
    Unk800C9D1C *rec;
    Battle4880 *en;
    U16Pair *ofs;
    BtlCmd8 *src;
    u32 *flags;
    BtlCmd8 *dst;
    BtlCmd8 *cmd;
    BtlUnit *u;
    Battle1330 *grp2;

    b = g_gameState->unk1C->unk8E4.battle;
    flags = &b->unk132C.unk0;
    grp = b->unk132C.unk4;
    hdr = b->unk11C0->unk20;
    b->unk132C.unk0 = (b->unk132C.unk0 & ~3) | (func_800C1184(flags) & 3);
    i = 0;
    n = hdr->unk2 + hdr->unk3;
    grp2 = hdr->unk8;
    recs = (Unk800C9D1C *)(grp2 + hdr->unk1);
    rec = recs;
    en = g_gameState->unk1C->unk8E4.battle->unk4E8;
    ofs = (U16Pair *)(g_gameState->unk1C->unk8E4.battle->unk11C0->unk14 + 4);
    src = (BtlCmd8 *)&recs[hdr->unk2];
    for (; i < hdr->unk2; i++) {
        func_800C9D1C(rec, en);
        en->unk2C = func_800CA2B0(i + 5);
        en->unk0 = ofs->unk0 + 4;
        ofs++;
        en->unk28 = n;
        n += rec->unk4F;
        en++;
        rec++;
    }
    dst = g_gameState->unk1C->unk8E4.battle->unk5D8;
    for (i = 0, cmd = src; i < hdr->unk3; i++, cmd++) {
        func_800CA28C(cmd, dst);
        dst->nameOfs = ofs->unk0 + 4;
        ofs++;
        dst++;
    }
    u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    for (i = 0; i < grp->unk1; i++) {
        if (u == NULL) {
            break;
        }
        func_800C9EE0(&grp->unk8[i], u, (s32 *)flags, i);
        ((void (*)(Unk800C9D1C *, BtlUnit *, s32))func_800CA0A0)(&recs[grp->unk8[i].unk0], u, i);
        u = u->unk0;
    }
}

void func_800C9A28(void) {
    s16 n;
    s16 i;
    Battle *b;
    s32 x;
    BtlUnit *u;
    s16 rot;
    BtlUnit98 *p98;
    BtlUnitA0 *pa0;
    BtlUnit98 *q98;
    BtlUnitA0 *qa0;
    s32 w;
    s32 v;
    Unk800F57F0 *e;
    Unk800F57F0 *tbl;
    u32 *flags;
    u32 m;

    n = 0;
    b = g_gameState->unk1C->unk8E4.battle;
    flags = &b->unk132C.unk0;
    for (i = 0; i < 4; i++) {
        if (g_gameState->unk1C->party[i] != NULL) {
            n++;
        }
    }
    x = (n - 1) * 0x13C;
    i = 0;
    rot = ((*flags & 3) != 0) << 11;
    u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    for (; i < n; i++) {
        if (!(u->unk68 & 1)) {
            return;
        }
        m = u->unk68 & ~0x300;
        m |= (g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.w >> 1) & 0x100;
        u->unk68 = m;
        if (!(*flags & 3)) {
            u->unk68 = (u->unk68 & ~0x300) | ((((u->unk68 >> 8) & 3) ^ 1) << 8);
        }
        u->unk8C[0] = u->unk98->unk0[0] = u->unkA0->unk14 = x << 16;
        p98 = u->unk98;
        pa0 = u->unkA0;
        if ((u8)btl_hasStatus(u, 0x200000)) {
            v = 0xFF380000;
        } else {
            v = 0;
        }
        u->unk8C[1] = p98->unk0[1] = pa0->unk18 = v;
        q98 = u->unk98;
        qa0 = u->unkA0;
        w = 0xF9E80000;
        if (!((u->unk68 >> 8) & 3)) {
            w = 0xF8580000;
        }
        u->unk8C[2] = q98->unk0[2] = qa0->unk1C = w;
        u->unkA0->unk20 = 0;
        u->unk86[0] = 0;
        u->unkA0->unk22 = rot;
        u->unk86[1] = rot;
        u->unkA0->unk24 = 0;
        u->unk86[2] = 0;
        tbl = D_800F57F0;
        e = &tbl[g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4];
        u->unkBD = e->unk0;
        u->unkBE = e->unk1;
        func_800C23C8(u, e->unk2, e->unk3);
        x -= 0x278;
        g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[(u->unk68 >> 1) & 0xF].unk4_22 = e->unk4;
        u = u->unk0;
    }
}

void func_800C9D1C(Unk800C9D1C *arg0, Battle4880 *arg1) {
    s16 i;
    BtlUnit9CC *h;
    arg1->unk24 = arg0->unk1C;
    arg1->unk2 = arg0->unk41;
    arg1->unk3 = arg0->unk40;
    arg1->unk27 = arg0->unk47;
    arg1->unk4.maxHp = arg0->unkC;
    arg1->unk4.maxMp = arg0->unkE;
    arg1->unk44 = arg0->unk10;
    arg1->unk46 = arg0->unk12;
    arg1->unk4C = arg0->unk69;
    arg1->unk48.w = arg0->unk14;
    for (i = 0; i < 6; i++) {
        h = func_8001C7FC(4, arg0->unk20[i] | 0x30000, 0);
        arg1->unkC[i] = h;
        if (IS_OFFSET(h->unkC)) {
            h->unkC = (s32)h + h->unkC;
            if (h->unk10 != 0) {
                h->unk10 = (s32)arg1->unkC[i] + h->unk10;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        arg1->unk29[i] = arg0->unk48[i];
    }
    arg1->unk30 = arg0->unk4C;
    arg1->unk26 = arg0->unk4E;
    for (i = 0; i < 6; i++) {
        arg1->unk32[i] = arg0->unk50[i];
        arg1->unk38[i] = arg0->unk56[i];
        arg1->unk3E[i] = arg0->unk5C[i];
    }
}

void func_800C9EE0(Unk800C9EE0 *arg0, BtlUnit *arg1, s32 *arg2, s32 arg3) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    Battle488 *s = &b->unk488[(arg1->unk68 >> 1) & 0xF];
    Battle4880 *e;
    s16 r;
    s->unk0 = &b->unk4E8[arg0->unk0];
    arg1->unk68 = (((arg1->unk68 & ~0x10000000) | ((arg0->unk1 & 1) << 28)) & ~0x300) | 0x200;
    e = &g_gameState->unk1C->unk8E4.battle->unk4E8[arg0->unk0];
    arg1->max = &e->unk4;
    arg1->hp = e->unk4.maxHp;
    arg1->mp = arg1->max->maxMp;
    s->unk4 = (s->unk4 & ~0x100) | ((arg0->unk1 << 7) & 0x100);
    if (!((s->unk4 >> 9) & 1)) {
        arg1->unk8C[0] = arg1->unk98->unk0[0] = arg1->unkA0->unk14 = arg0->unk4 << 16;
        arg1->unk8C[1] = arg1->unk98->unk0[1] = arg1->unkA0->unk18 = arg0->unk6 << 16;
        arg1->unk8C[2] = arg1->unk98->unk0[2] = arg1->unkA0->unk1C = arg0->unk8 << 16;
        arg1->unkA0->unk20 = 0;
        arg1->unk86[0] = 0;
        arg1->unkA0->unk24 = 0;
        arg1->unk86[2] = 0;
        r = (arg0->unkA + D_800F5850[*arg2 & 3]) & 0xFFF;
        arg1->unkA0->unk22 = r;
        arg1->unk86[1] = r;
    }
    arg1->unkA4 = (s32 *)s->unk0->unkC;
    arg1->unk9C->unkC = s->unk0->unkC[0];
}

void func_800CA0A0(Unk800C9D1C *arg0, BtlUnit *arg1) {
    Battle488 *s;
    Battle11C028 *t;

    arg1->statusImmune = arg0->unk0;
    arg1->unk38 = arg0->unk4;
    arg1->status = arg0->unk8;
    arg1->hp = arg0->unkC;
    arg1->mp = arg0->unkE;
    arg1->unk6C = arg0->unk43[0];
    arg1->unk6D = arg0->unk43[1];
    arg1->unk6E = arg0->unk43[2];
    arg1->unk6F = arg0->unk43[3];
    arg1->unk30 = arg0->unk34[0];
    arg1->unk31 = arg0->unk34[1];
    arg1->unk32 = arg0->unk34[2];
    arg1->unk33 = arg0->unk34[3];
    *(s32 *)&arg1->unk70 = arg0->unk3C;
    *(s32 *)&arg1->unkAC = arg0->unk2C;
    arg1->unk7C = arg0->unk4B;
    s = &g_gameState->unk1C->unk8E4.battle->unk488[(arg1->unk68 >> 1) & 0xF];
    s->unk8.w = arg0->unk18;
    s->unk4 = (s->unk4 & ~0x40) | ((arg0->unk30 & 1) << 6);
    s->unk4 = (s->unk4 & ~0x80) | ((arg0->unk30 << 6) & 0x80);
    func_800C23C8(arg1, arg0->unk64, arg0->unk66);
    arg1->unkBD = arg0->unk68;
    arg1->unkBE = arg0->unk6E;
    t = &g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[((arg1->unk68 >> 1) & 0xF) + 9];
    t->unk4_12 = arg0->unk6A;
    t->unk4_22 = arg0->unk6C;
}

void func_800CA28C(BtlCmd8 *arg0, BtlCmd8 *arg1) {
    *arg1 = *arg0;
}

s32 func_800CA2B0(s32 arg0) {
    s16 *t = g_gameState->unk1C->unk8E4.battle->unk132C.unk10;
    return (s32)t + t[arg0];
}

s16 func_800CA2E4(s16 arg0) {
    Unk800C9EE0 *e = &g_gameState->unk1C->unk8E4.battle->unk132C.unk4->unk8[arg0];
    s32 r;
    if (arg0 > 0 && (e->unk1 & 2)) {
        r = -1;
    } else {
        r = g_gameState->unk1C->unk8E4.battle->unk132C.unk8[e->unk0].unk1E;
    }
    return r;
}

u16 func_800CA378(void) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    return b->unk132C.unk8[b->unk132C.unk4->unk8[0].unk0].unk62;
}

void func_800CA3CC(void) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    u16 i = 0;
    u16 *h = b->unk132C.unkC;
    Battle11C4 *t = &b->unk11C4;
    u16 *p = h + 2;
    u32 *q;
    BtlUnit9CC *m;
    u16 n = h[0];
    for (; i < n; i++) {
        t->unk8[i] = (s32)h + *p++;
    }
    for (; i < 0x12; i++) {
        t->unk8[i] = 0;
    }
    q = (u32 *)p;
    t->unk0 = (BtlUnit9CC **)q;
    n = h[1];
    for (i = 0; i < n; i++, q++) {
        m = func_8001C7FC(4, (*q & 0xFFFF) | 0x30000, 0);
        *q = (u32)m;
        if (IS_OFFSET(m->unkC)) {
            m->unkC = (s32)m + m->unkC;
            if (m->unk10 != 0) {
                m->unk10 = *q + m->unk10;
            }
        }
    }
    t->unk4 = (u8 *)q;
    {
        Battle122C *c = t->unk68;
        for (i = 4; i-- != 0;) {
            c->unk4 = 0;
            c++;
        }
    }
}

void func_800CA530(BtlCmd *arg0) {
    if (g_gameState->unk1C->unk8E4.battle->unk11C4.unk8[arg0->subNo] != 0 && func_800CA5AC(arg0) != NULL) {
        BtlUnit *u = arg0->unk4;
        if (!(u->unk68 & 1)) {
            u->unk86[1] = 0;
        }
    }
}

Battle122C *func_800CA5AC(BtlCmd *arg0) {
    Battle122C *e = g_gameState->unk1C->unk8E4.battle->unk11C4.unk68;
    GameState *g;
    s16 i;
    for (i = 0; i < 4; i++, e++) {
        if (e->unk4 == NULL) {
            break;
        }
    }
    if (i < 0) {
        return NULL;
    }
    g = g_gameState;
    e->unk0 = 0;
    e->unk4 = arg0;
    e->unk8 = g->unk1C->unk8E4.battle->unk11C4.unk8[arg0->subNo];
    e->unkC = 0;
    e->unk10 = 0;
    e->unk12 = 0;
    e->unk14 = 0;
    e->unk16 = g->unk1C->unk8E4.battle->unk11C4.unk4[arg0->subNo];
    e->unk17 = 0;
    e->unk21 = 0;
    e->unk25 = 0;
    e->unk26 = 0;
    g_gameState->unk1C->unk8E4.battle->unk131C = 0;
    return e;
}

void func_800CA694(u16 mask, s16 arg1, u16 id) {
    Battle *b;
    BtlUnit *u;
    BtlCmd *c;
    Battle122C *e;

    b = g_gameState->unk1C->unk8E4.battle;
    if (b->unk11C4.unk8[id] == 0) {
        return;
    }
    for (u = b->unk6F8; u != NULL; u = u->unk0) {
        if (u->unkB0 & mask) {
            break;
        }
    }
    if (u != NULL) {
        c = u->unk4;
        c->cmdNo = 0;
        c->subNo = id;
        c->targets = arg1;
        c->unk4 = u;
        c->ability = &g_gameState->unk1C->unk8E4.battle->unk5D8[id];
        c->unk10.w = 0;
        e = func_800CA5AC(c);
        if (e != NULL) {
            e->unk0 |= 8;
        }
    }
}

s32 func_800CA774(void) {
    s16 i;
    Battle122C *p = g_gameState->unk1C->unk8E4.battle->unk11C4.unk68;
    for (i = 0; i < 4; i++, p++) {
        if (p->unk4 != 0) {
            return 1;
        }
    }
    return 0;
}

void func_800CA7CC(void) {
    s16 i = 3;
    BtlScript *s = (BtlScript *)g_gameState->unk1C->unk8E4.battle->unk11C4.unk68;
    BtlUnit *u;
    u8 op;
    s16 v[4];

    do {
        if (s->unk4 != NULL) {
            s->unk10++;
            s->unk12--;
            s->unk14++;
            u = s->unk4->unk4;
            do {
                op = *s->unk8;
                if (op >= 0x23) {
                    op = 0;
                }
                if ((s->unk0 & 1) && g_btlfxEnv->unk0->unk23 != 0) {
                    s->unk0 = (s->unk0 | 2) & ~1;
                }
                if (s->unk8 != s->unkC && D_800F5858[op].unk0 != NULL) {
                    D_800F5858[op].unk0(s, u);
                }
                s->unkC = s->unk8;
            } while ((u16)D_800F5858[op].unk4(s, u));
            if (s->unk21 != 0) {
                u->unk86[1] = s->unk1C + s->unk1E * s->unk20 / s->unk21;
                if (s->unk20++ >= s->unk21) {
                    s->unk21 = 0;
                }
            }
            if (s->unk17 != 0) {
                if (--s->unk17 == 0) {
                    func_800C22CC(u, s->unk18, s->unk1B);
                }
            }
            if (s->unk25 != 0) {
                if (--s->unk25 == 0) {
                    func_800E3DA8();
                    v[0] = s->unk24;
                    v[1] = 0;
                    v[2] = 0;
                    v[3] = 0;
                    func_800C2598((BtlCmd *)s->unk4, (u16)s->unk22, v);
                }
            }
            if (s->unk26 != 0) {
                s->unk27--;
                func_800C24E8(u, (s->unk27 << 5) / s->unk26);
                if (s->unk27 == 0) {
                    s->unk26 = 0;
                }
            }
        }
        s++;
    } while (--i != -1);
}

s32 func_800CAAD0(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C *m = arg1->unk9C;
    if (!((arg0->unk0 >> 2) & 1) && arg0->unk14 != 0 && arg1->unkA0->unk3A >= m->unkC->unk2) {
        m->unkC = (BtlUnit9CC *)arg1->unkA4[(arg1->unk68 >> 18) & 1];
        arg1->unkA0->unk3A = 0;
        arg0->unk0 |= 4;
    }
    if ((arg1->status & 0x02001002) || ((arg0->unk0 >> 2) & 1)) {
        if (g_btlfxEnv->unk0->unk0 != -1) {
            return 0;
        }
        if (arg0->unk21 != 0) {
            return 0;
        }
        if (g_gameState->unk1C->unk8E4.battle->unk1358.unk1C4 != 0) {
            return 0;
        }
        if (!((arg0->unk0 >> 3) & 1)) {
            func_800B3C00();
        }
        arg0->unk4 = 0;
    }
    return 0;
}

s32 func_800CAC24(BtlScript *arg0) {
    if (g_btlfxEnv->unk0->unk0 == -1) {
        if (arg0->unk21 != 0) {
            return 0;
        }
        if (!((arg0->unk0 >> 3) & 1)) {
            func_800B3C00();
        }
        arg0->unk4 = 0;
    }
    return 0;
}

void func_800CAC98(BtlScript *arg0) {
    arg0->unk12 = arg0->unk8[1];
}

s32 func_800CACAC(BtlScript *arg0) {
    if (arg0->unk12 <= 0) {
        arg0->unk8 += 2;
        return 1;
    }
    return 0;
}

s32 func_800CACD8(BtlScript *arg0) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    u16 mask = arg0->unk4->unkC;
    while (u != NULL) {
        if (u->unkB0 & mask) {
            btl_execCmd(u);
        }
        u = u->unk0;
    }
    arg0->unk8 += 1;
    return 1;
}

void func_800CAD6C(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->unk28;
    s16 i;
    s16 spd;
    s32 a;
    p[7] = 4;
    arg0->unk10 = 1;
    p[6] = arg0->unk8[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        *q = arg1->unk8C[i] >> 16;
    }
    p[4] = p[1];
    func_800CB1D4(arg0->unk4->unkC, (u16 *)&p[3], (u16 *)&p[5]);
    spd = arg0->unk8[2] | (arg0->unk8[3] << 8);
    if (p[0] != p[3] || p[2] != p[5]) {
        if (arg0->unk8[0] == 0x1E) {
            p[5] -= spd;
        } else {
            a = ratan2(p[3] - p[0], p[5] - p[2]) & 0xFFF;
            p[3] += (math_sin(a) * spd) >> 12;
            p[5] += (math_cos(a) * spd) >> 12;
        }
    }
}

s32 func_800CAEC4(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->unk28;
    s16 i;
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        arg1->unk8C[i] = ((q[3] - q[0]) * arg0->unk10 / p[6] + q[0]) << 16;
    }
    if (arg0->unk10 >= p[6]) {
        arg0->unk8 += (u16)p[7];
    }
    return 0;
}

void func_800CAF90(BtlScript *arg0, BtlUnit *arg1) {
    s32 *v;
    s16 i;
    s16 *p = arg0->unk28;
    p[7] = 2;
    arg0->unk10 = 1;
    p[6] = arg0->unk8[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        q[0] = arg1->unk8C[i] >> 16;
        v = arg1->unk98->unk0;
        q[3] = v[i] >> 16;
    }
}

void func_800CB008(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->unk28;
    s16 i;
    p[7] = 8;
    arg0->unk10 = 1;
    p[6] = arg0->unk8[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        q[0] = arg1->unk8C[i] >> 16;
        q[3] = arg0->unk8[i * 2 + 2] | (arg0->unk8[i * 2 + 3] << 8);
    }
}

s32 func_800CB090(s32 arg0, Unk800CB090 *arg1) {
    s16 i;
    func_800CAEC4((BtlScript *)arg0, (BtlUnit *)arg1);
    for (i = 0; i < 3; i++) {
        arg1->unk98[i] = arg1->unk8C[i];
    }
    return 0;
}

void func_800CB0F0(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->unk28;
    s16 i;
    p[7] = 8;
    arg0->unk10 = 1;
    p[6] = arg0->unk8[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        s16 v = arg1->unk8C[i] >> 16;
        q[0] = v;
        q[3] = v + (arg0->unk8[i * 2 + 2] | (arg0->unk8[i * 2 + 3] << 8));
    }
}

BtlUnit *func_800CB17C(u16 arg0) {
    BtlUnit *u;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (u->unkB0 & arg0) {
            break;
        }
    }
    return u;
}

void func_800CB1D4(u16 arg0, u16 *arg1, u16 *arg2) {
    BtlUnit *u;
    s32 n = 0;
    *arg1 = 0;
    *arg2 = 0;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if (u->unkB0 & arg0) {
            *arg1 += u->unk8C[0] >> 16;
            n++;
            *arg2 += u->unk8C[2] >> 16;
        }
    }
    if (n >= 2) {
        *arg1 = (s16)*arg1 / n;
        *arg2 = (s16)*arg2 / n;
    }
}

s32 func_800CB2E0(BtlScript *arg0, BtlUnit *arg1) {
    u32 f;
    u8 *p = arg0->unk8;
    s32 idx;
    if (p[1] == 0xFF) {
        f = arg1->unk68;
        arg1->unk9C->unkC = g_gameState->unk1C->unk8E4.battle->unk488[(f >> 1) & 0xF].unk0->unkC[(f >> 18) & 1];
    } else {
        idx = arg0->unk16 + p[1];
        arg1->unk9C->unkC = g_gameState->unk1C->unk8E4.battle->unk11C4.unk0[idx];
    }
    arg1->unkA0->unk3A = 0;
    arg0->unk14 = 0;
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CB3A4(BtlScript *arg0) {
    arg0->unk22 = arg0->unk8[1] | (arg0->unk8[2] << 8);
    arg0->unk24 = arg0->unk8[3];
    arg0->unk25 = arg0->unk8[4] + 1;
    arg0->unk8 += 5;
    return 1;
}

s32 func_800CB3EC(BtlScript *arg0) {
    s16 v[4];
    s32 id;
    Unk800F78A40 *e = g_btlfxEnv->unk0;
    if ((e->unk0 != -1) & !((e->unk1C >> 15) & 1)) {
        return 0;
    }
    id = arg0->unk8[1] | (arg0->unk8[2] << 8);
    v[0] = arg0->unk8[3] | (arg0->unk8[4] << 8);
    v[1] = arg0->unk8[5] | (arg0->unk8[6] << 8);
    v[2] = arg0->unk8[7] | (arg0->unk8[8] << 8);
    v[3] = arg0->unk8[0] == 0x1A;
    func_800C2598((BtlCmd *)arg0->unk4, id, v);
    arg0->unk0 |= 1;
    arg0->unk8 += 9;
    return 1;
}

s32 func_800CB4E8(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C *m = arg1->unk9C;
    BtlUnitA0 *a = arg1->unkA0;
    if (arg0->unk14 != 0 && (a->unk3A >= m->unkC->unk2 || (arg1->status & 0x02001002))) {
        g_btlfxEnv->unk2C->unk52 = 0;
        arg0->unk8 += 1;
        return 1;
    }
    return 0;
}

s32 func_800CB560(BtlScript *arg0) {
    if ((arg0->unk0 >> 1) & 1) {
        arg0->unk8 += 1;
        return 1;
    }
    return 0;
}

s32 func_800CB594(BtlScript *arg0) {
    g_btlfxEnv->unk0->unkF = 1;
    arg0->unk8 += 1;
    return 1;
}

s32 func_800CB5C4(BtlScript *arg0) {
    Unk800F78A40 *p = g_btlfxEnv->unk0;
    if ((p->unk0 == -1) | ((p->unk1C >> 15) & 1)) {
        arg0->unk8 += 1;
        return 1;
    }
    return 0;
}

s32 func_800CB618(BtlScript *arg0, BtlUnit *arg1) {
    arg1->unkAC |= arg0->unk8[1] | (arg0->unk8[2] << 8);
    func_800BD414(arg1, arg0->unk8[1] | (arg0->unk8[2] << 8));
    arg0->unk8 += 3;
    return 1;
}

s32 func_800CB68C(BtlScript *arg0, BtlUnit *arg1) {
    arg1->unkAC &= ~(arg0->unk8[1] | (arg0->unk8[2] << 8));
    func_800BD464(arg1, arg0->unk8[1] | (arg0->unk8[2] << 8));
    arg0->unk8 += 3;
    return 1;
}

s32 func_800CB700(BtlScript *arg0, BtlUnit *arg1) {
    u8 *pc;
    Battle *b;
    u8 *txt;
    u16 idx;
    u16 ofs;

    pc = arg0->unk8;
    idx = pc[1];
    b = g_gameState->unk1C->unk8E4.battle;
    txt = b->unk11C0->unk14;
    if (idx & 0x80) {
        func_800F4F44((s32)(txt + arg0->unk4->unk8->nameOfs), 1);
    } else {
        idx += b->unk488[(arg1->unk68 >> 1) & 0xF].unk0->unk28;
        ofs = ((u16 *)txt)[idx * 2 + 2] + 4;
        if (pc[0] == 0x21) {
            func_800F4F44((s32)(txt + ofs), 3);
        } else {
            btl_setBanner((s32)(txt + ofs), 3);
        }
    }
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CB808(BtlScript *arg0, BtlUnit *arg1) {
    arg1->unk68 = (arg1->unk68 & ~0x40000) | ((arg0->unk8[1] & 1) << 18);
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CB848(BtlScript *arg0) {
    g_gameState->unk1C->unk8E4.battle->unk131C = arg0->unk8[1];
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CB884(BtlScript *arg0, BtlUnit *arg1) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    Battle11C4 *s = &b->unk11C4;
    BtlUnit *u;
    u32 f;
    s32 r;
    s8 one;
    if (arg0->unk8[0] != 0x20 && !((arg0->unk4->unk8->unk0_17 >> 13) & 1)) {
        if (((g_gameState->unk24 >> 4) & (one = 1)) == one) {
            goto end;
        }
        f = arg1->status;
        for (u = b->unk6F8; u != NULL; u = u->unk0) {
            if (u->unkB0 & arg0->unk4->unkC) {
                f |= u->status;
            }
        }
        if (f & 0x14000000) {
            goto end;
        }
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        r = *g_randPtr++;
        if (r >= 0x80) {
            goto end;
        }
    }
    func_800CB1D4(arg0->unk4->unkC, &s->unk150, &s->unk154);
    s->unk152 = 0;
    s->unk148 = arg1;
    s->unk14C = (s32)func_800CB17C(arg0->unk4->unkC);
    g_gameState->unk1C->unk8E4.battle->unk131C = arg0->unk8[1];
    func_800C2FFC(g_gameState->unk1C->unk8E4.battle->unk488[(arg1->unk68 >> 1) & 0xF].unk0->unk2C);
end:
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CBA58(BtlScript *arg0, BtlUnit *arg1) {
    u8 *pc = arg0->unk8;
    s32 a;
    s16 x, z;
    u16 ang;
    u16 b;
    s16 d[2];
    s32 v;
    s16 w;

    a = pc[1] | (pc[2] << 8);
    ang = a;
    b = pc[3] | (pc[4] << 8);
    arg0->unk21 = pc[5];
    w = arg1->unk86[1];
    arg0->unk20 = 1;
    arg0->unk1C = w;
    if (a & 0x8000) {
        switch (a & 0x7FFF) {
        case 0:
            ang = arg1->unkA0->unk22;
            break;
        case 1:
            func_800CB1D4(arg0->unk4->unkC, (u16 *)&d[0], (u16 *)&d[1]);
            x = (arg1->unk8C[0] >> 16) - d[0];
            z = (arg1->unk8C[2] >> 16) - d[1];
            d[0] = x;
            d[1] = z;
            if (x == 0 && z == 0) {
                ang = arg1->unk86[1];
            } else {
                ang = ratan2(d[0], d[1]);
            }
            break;
        default:
            ang = arg1->unk86[1];
            break;
        }
    }
    ang = (ang + b) & 0xFFF;
    v = ((u16)arg1->unk86[1] - ang) & 0xFFF;
    if (v > 0x800) {
        arg0->unk1E = 0x1000 - v;
    } else {
        arg0->unk1E = -v;
    }
    arg0->unk8 += 6;
    return 1;
}

void func_800CBBB0(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->unk28;
    s16 a = arg1->unk9C->unk54;
    s16 v;
    s16 r;
    p[0] = a;
    v = arg0->unk8[1] | (arg0->unk8[2] << 8);
    if (v == -1) {
        r = 0x1000;
    } else {
        r = a * v / 0x1000;
    }
    p[1] = r - p[0];
    arg0->unk10 = 1;
    p[2] = arg0->unk8[3];
}

s32 func_800CBC38(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->unk28;
    u16 v = arg0->unk28[0] + p[1] * arg0->unk10 / p[2];
    btl_setUnitAttr((s32)arg1, 0x37, v);
    if (v == 0x1000) {
        arg1->unk9C->unk0 &= ~2;
    }
    if (arg0->unk10 >= p[2]) {
        arg0->unk8 += 4;
        return 1;
    }
    return 0;
}

s32 func_800CBD28(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C50 *g;
    BtlUnit9C50 *e = &arg1->unk9C->unk50[arg0->unk8[1]];
    e->unk4 |= 1;
    g = &arg1->unk9C->unk50[arg0->unk8[1]];
    g->unkC = 0;
    e = &arg1->unk9C->unk50[arg0->unk8[1]];
    e->unk12 = 0x1000;
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CBDC8(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C50 *g;
    BtlUnit9C50 *e = &arg1->unk9C->unk50[arg0->unk8[1]];
    e->unk4 |= 3;
    g = &arg1->unk9C->unk50[arg0->unk8[1]];
    g->unkC = 0;
    e = &arg1->unk9C->unk50[arg0->unk8[1]];
    e->unk12 = 0x1000;
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CBE68(BtlScript *arg0, BtlUnit *arg1) {
    u8 i = arg0->unk8[1];
    BtlUnit9C *m = arg1->unk9C;
    BtlUnit9C50 *e;
    (*(e = m->unk50 + i)).unk4 &= 0xFC;
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CBEB4(BtlScript *arg0) {
    arg0->unk18 = arg0->unk8[1] | (arg0->unk8[2] << 8);
    arg0->unk17 = arg0->unk8[3] + 1;
    arg0->unk1B = arg0->unk8[5];
    arg0->unk8 += 6;
    return 1;
}

s32 func_800CBEFC(BtlScript *arg0, BtlUnit *arg1) {
    arg1->unk7C = arg0->unk8[1];
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CBF28(BtlScript *arg0, BtlUnit *arg1) {
    arg0->unk26 = arg0->unk27 = arg0->unk8[1];
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CBF50(BtlScript *arg0, BtlUnit *arg1) {
    arg1->unk68 = (arg1->unk68 & ~0x1000000) | ((arg0->unk8[1] & 1) << 24);
    arg0->unk8 += 2;
    return 1;
}

void func_800CBF90(void) {
    s16 i;
    Battle1358 *p;
    g_gameState->unk1C->unk8E4.battle->unk1358.unk1C0 = 0;
    g_gameState->unk1C->unk8E4.battle->unk1358.unk1C2 = 0;
    g_gameState->unk1C->unk8E4.battle->unk1358.unk1D4 = 0xFF;
    p = g_gameState->unk1C->unk8E4.battle->unk1358.unk0;
    for (i = 15; i != -1; i--) {
        p->unk0 = NULL;
        p++;
    }
    g_gameState->unk1C->unk8E4.battle->unk1358.unk1C4 = 0;
    g_gameState->unk1C->unk8E4.battle->unk1358.unk1CC = 0;
}

void btl_spawnPopups(BtlUnit *arg0) {
    u16 f = arg0->unk7E;
    u8 n = 0;
    if (!((arg0->unk68 >> 22) & 1)) {
        if (f & 0x100) {
            func_800BD774(arg0);
        }
        if (f & 0x80) {
            btl_popupText((s32)arg0, 2, 0, 0);
            arg0->unk7E = 0;
        } else if (f & 0x60) {
            if (f & 0x20) {
                btl_popupText((s32)arg0, 0, 0, 0);
                n = 2;
            }
            if (f & 0x40) {
                btl_popupText((s32)arg0, 1, 0, n);
            }
            arg0->unk7E = 0;
        } else {
            if (f & 1) {
                if (f & 4) {
                    btl_popupText((s32)arg0, 3, 0x80, 0);
                    n = 2;
                }
                if (f & 2) {
                    btl_popupHp((s32)arg0, arg0->unk80, 0xC0, n);
                } else {
                    btl_popupHp((s32)arg0, arg0->unk80, 0, n);
                }
                n += 4;
            }
            if (f & 8) {
                if (f & 0x10) {
                    btl_popupMp((s32)arg0, arg0->unk82, 0xC0, n);
                } else {
                    btl_popupMp((s32)arg0, arg0->unk82, 0, n);
                }
            }
            arg0->unk7E = 0;
        }
    } else {
        arg0->unk7E = 0;
    }
    arg0->unk80 = 0;
    arg0->unk82 = 0;
}

void func_800CC1A0(BtlUnit *arg0) {
    u8 f = arg0->unkCE;
    u8 n = 0;
    if (!((arg0->unk68 >> 22) & 1)) {
        if (f & 1) {
            btl_popupHp((s32)arg0, arg0->unkC8, (f & 8) ? 0 : 0xC0, 0)->unk10.b.unk3 = 1;
            n = 4;
        }
        if (f & 2) {
            btl_popupHp((s32)arg0, arg0->unkCA, 0, n)->unk10.b.unk3 = 1;
            n += 4;
        }
        if (f & 4) {
            btl_popupMp((s32)arg0, arg0->unkCC, 0, n)->unk10.b.unk3 = 1;
        }
    }
    arg0->unkCE = 0;
    arg0->unkC8 = 0;
    arg0->unkCA = 0;
    arg0->unkCC = 0;
}

Unk800CC278 *func_800CC278(BtlUnit *u) {
    Battle1358Ring *r;
    Battle1358 *p;
    BtlBone *bone;
    s16 i;

    r = &g_gameState->unk1C->unk8E4.battle->unk1358;
    i = r->unk1C0 - 1;
    if (i < 0) {
        i = 15;
    }
    r->unk1C0 = i;
    p = &r->unk0[i];
    p->unk0 = u;
    p->unkE = 0;
    p->unk10.w = 0;
    bone = &u->unk9C->unk20[u->unk7C];
    p->unk8 = bone->unk14;
    p->unkA = bone->unk18 - 4;
    p->unkC = bone->unk1C;
    return p;
}

Unk800CC278 *btl_popupHp(s32 arg0, s16 arg1, s16 arg2, u8 arg3) {
    Unk800CC278 *p = func_800CC278((BtlUnit *)arg0);
    p->unk10.b.unk2 = 0;
    p->unk10.b.unk0 = arg3;
    p->unk18 = arg2;
    p->unk14.w = func_800CC444(arg1);
    return p;
}

Unk800CC278 *btl_popupMp(s32 arg0, s16 arg1, s16 arg2, u8 arg3) {
    Unk800CC278 *p = func_800CC278((BtlUnit *)arg0);
    p->unk10.b.unk2 = 1;
    p->unk10.b.unk0 = arg3;
    p->unk18 = arg2;
    p->unk14.w = func_800CC444(arg1);
    return p;
}

Unk800CC278 *btl_popupText(s32 arg0, u8 arg1, s16 arg2, s8 arg3) {
    Unk800CC278 *p = func_800CC278((BtlUnit *)arg0);
    p->unk10.b.unk2 = 2;
    p->unk10.b.unk0 = arg3;
    p->unk14.p = &D_800F6A54[arg1];
    p->unk18 = arg2;
    return p;
}

/* Packs n (< 9999) as decimal digits, one per nibble, with the digit count in bits 16+. */
s32 func_800CC444(u32 n) {
    u32 r = 0;
    u16 d;
    u16 i;
    u32 t;
    do {
        if (n >= 9999) {
            return 0x49999;
        }
        for (i = 3; i != 0; i--) {
            d = 0;
            if (n >= D_800F6A74[i]) {
                d = n / D_800F6A74[i];
                n = n % D_800F6A74[i];
            }
            r = (r << 4) | d;
        }
        r = (r << 4) | n;
    } while (0);
    for (i = 1, t = r >> 4; t != 0; t >>= 4) {
        i++;
    }
    return r | (i << 16);
}

u8 *btl_drawPopups(u8 *arg0) {
    Unk800CCD00 c;
    Battle1358Ring *r;
    Battle1358 *p;
    s16 i;
    s16 j;
    BtlUnit *u;
    u8 mask;
    Unk800F78A40 *e;

    r = &g_gameState->unk1C->unk8E4.battle->unk1358;
    c.unk0 = (u32 *)arg0;
    c.unk4 = &g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][4];
    gte_SetRotMatrix(&g_gameState->unk1C->unk18);
    gte_SetTransMatrix(&g_gameState->unk1C->unk18);
    j = r->unk1C0;
    for (i = 0; i < 16; i++) {
        p = &r->unk0[j];
        if (p->unk0 != NULL) {
            if (p->unk10.b.unk2 >= 3) {
                p->unk0 = NULL;
            } else if (p->unk10.b.unk0 != 0) {
                p->unk10.b.unk0--;
            } else {
                gte_ldv0(&p->unk8);
                gte_rtps();
                gte_stsxy(&c.unk10);
                if (p->unk10.b.unk1 != 0 && (u16)(c.unk10.s.y - 21) < 180) {
                    p->unk10.b.unk1 = 0;
                }
                D_800F6A7C[p->unk10.b.unk2](p, &c);
            }
        }
        j++;
        if (j >= 16) {
            j = 0;
        }
    }
    func_800CD210(&c);
    ((u8 *)c.unk0)[3] = 1;
    c.unk0[1] = (D_80068BF0[0].unk6 & 0x9FF) | 0xE1000000;
    addPrim(c.unk4, c.unk0);
    c.unk0 += 2;
    e = g_btlfxEnv->unk0;
    if (((e->unk1C ^ 1) & 1) | ((e->unk0 == -1) | ((e->unk1C >> 15) & 1))) {
        func_800CCDB8(&c);
    }
    r->unk1C2++;
    func_800CD83C();
    func_800CDD68();
    mask = 0xFF;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (!((u->unk68 >> 22) & 1)) {
            mask &= ~u->unkB0;
        }
    }
    r->unk1D4 = mask;
    return (u8 *)c.unk0;
}

void func_800CC818(Battle1358 *arg0, Unk800CCD00 *arg1) {
    u8 *anim;
    u8 c;
    u32 n;
    u32 w;
    s16 x;

    if (arg0->unk18 == 0xC0) {
        anim = D_800F6A38;
    } else {
        anim = D_800F6A28;
    }
    c = anim[arg0->unkE];
    if (!(arg0->unk0->unk9C->unk0 & 0x10)) {
        n = arg0->unk14.w;
        arg1->unk8 = (c & 0x80) ? 0x03000000 : 0x01000000;
        arg1->unk14 = arg0->unk18;
        w = (n >> 16) * 8;
        arg1->unk10.s.x -= w >> 1;
        arg1->unk10.s.y -= (c & 0x7F) + 5;
        if (arg0->unk10.b.unk3 == 0) {
            x = w;
            if (arg0->unk10.b.unk2 != 0) {
                x = D_80068BE8.unk4 + w;
            }
            func_800CCAF0(arg0, arg1, x, 10);
        } else {
            arg1->unk10.s.y += arg0->unk10.b.unk1;
        }
        func_800CCBC0(arg1, n);
        if (arg0->unk10.b.unk2 != 0) {
            arg1->unk10.s.x += (n >> 16) * 8;
            arg1->unk10.s.y += 3;
            func_800CCD00(arg1, &D_80068BE8);
        }
    }
    if (anim[++arg0->unkE] == 0xFF) {
        arg0->unk0 = NULL;
    }
}

void func_800CC9C8(Battle1358 *arg0, Unk800CCD00 *arg1) {
    u8 *anim;
    u8 c;
    Unk800686E8 *g;

    anim = arg0->unk14.p->unk0;
    c = anim[arg0->unkE];
    if (!(arg0->unk0->unk9C->unk0 & 0x10)) {
        arg1->unk8 = (c & 0x80) ? 0x03000000 : 0x01000000;
        arg1->unk14 = arg0->unk18;
        g = &D_800686D0[arg0->unk14.p->unk4];
        arg1->unk10.s.x -= g->unk4 >> 1;
        arg1->unk10.s.y -= (g->unk5 >> 1) + (c & 0x1F);
        if (arg0->unk10.b.unk3 == 0) {
            func_800CCAF0(arg0, arg1, g->unk4, g->unk5);
        }
        func_800CCD00(arg1, g);
    }
    arg0->unkE++;
    if (anim[arg0->unkE] == 0xFF) {
        arg0->unk0 = NULL;
    }
}

void func_800CCAF0(Battle1358 *arg0, Unk800CCD00 *arg1, s32 w, s32 h) {
    if (arg0->unkE == 0) {
        if (arg1->unk10.s.y - 21 < 0) {
            arg0->unk10.b.unk1 = 21 - arg1->unk10.s.y;
        } else if (arg1->unk10.s.y + h >= 211) {
            arg0->unk10.b.unk1 = -46 - (arg1->unk10.s.y + h);
        }
    }
    if (arg1->unk10.s.x < 8) {
        arg1->unk10.s.x = 8;
    } else if (arg1->unk10.s.x + w >= 313) {
        arg1->unk10.s.x = 312 - w;
    }
    arg1->unk10.s.y += arg0->unk10.b.unk1;
    if (arg1->unk10.s.y < 8) {
        arg1->unk10.s.y = 8;
    } else if (arg1->unk10.s.y + h >= 211) {
        arg1->unk10.s.y = 210 - h;
    }
}

void func_800CCBC0(Unk800CCD00 *arg0, u32 arg1) {
    DVECTOR xy;
    s16 i;
    Unk800686E8 *g;

    i = (arg1 >> 16) - 1;
    xy.vx = arg0->unk10.s.x + (((s32)arg1 >> 16) - 1) * 8;
    xy.vy = arg0->unk10.s.y;
    for (; i != -1; i--) {
        arg0->unk0[0] = 0x04000000;
        arg0->unk0[1] = arg0->unk8 | 0x64000000;
        arg0->unk0[2] = *(s32 *)&xy;
        g = &D_80068BF0[arg1 & 0xF];
        arg0->unk0[3] = g->unk0 + (arg0->unk14 << 16);
        arg0->unk0[4] = (g->unk5 << 16) | g->unk4;
        addPrim(arg0->unk4, arg0->unk0);
        arg0->unk0 += 5;
        xy.vx -= 8;
        arg1 >>= 4;
    }
}

void func_800CCD00(Unk800CCD00 *arg0, Unk800686E8 *arg1) {
    arg0->unk0[0] = 0x04000000;
    arg0->unk0[1] = arg0->unk8 | 0x64000000;
    arg0->unk0[2] = arg0->unk10.w;
    arg0->unk0[3] = arg1->unk0 + (arg0->unk14 << 16);
    arg0->unk0[4] = (arg1->unk5 << 16) | arg1->unk4;
    addPrim(arg0->unk4, arg0->unk0);
    arg0->unk0 += 5;
}

void func_800CCDB8(Unk800CCD00 *arg0) {
    Battle1358Ring *r;
    BtlUnit *u;
    BtlUnit9C *mdl;
    BtlBone *b;
    Unk800F6A88 *e;
    SVECTOR pos;
    SVECTOR rot;
    u32 st;
    s16 sn;
    s16 cs;
    s16 y;
    s32 y0;
    s32 z0;
    s16 z;
    s32 k;
    s32 f;
    s16 i;
    u8 *bone;
    u8 *ty;
    u8 *tz;

    r = &g_gameState->unk1C->unk8E4.battle->unk1358;
    func_800E70D0((u8 *)arg0->unk0, g_gameState->unk1C->unk10[g_gameState->unk1C->unk8]);
    g_btlfxEnv->unk28 = 0;
    rot.vx = 0;
    rot.vz = 0;
    rot.pad = 1;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (((u->unk68 >> 22) & 1) || (u->unk9C->unk0 & 0x10)) {
            continue;
        }
        if (r->unk1D4 & u->unkB0) {
            continue;
        }
        st = u->status | u->unk38;
        if ((st & 0x100) || !(st & 0x231B083A)) {
            continue;
        }
        if ((u->unk68 & 1) && u->unk9C->unkC == (BtlUnit9CC *)u->unkA4[0x11]) {
            continue;
        }
        sn = math_sin(u->unk86[1]);
        cs = math_cos(u->unk86[1]);
        if (u->unk68 & 1) {
            bone = D_800F6B48[g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4];
            ty = D_800F6BBC[g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4];
            tz = D_800F6C30[g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4];
        } else {
            Battle4880 *m = g_gameState->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0;
            bone = m->unk32;
            ty = m->unk38;
            tz = m->unk3E;
        }
        for (e = D_800F6A88, i = 11; i != -1; e++, i--) {
            if (!(st & e->unk0) || (st & e->unk4)) {
                continue;
            }
            k = e->unkC;
            mdl = u->unk9C;
            y0 = (ty[k] << 24) >> 20;
            z0 = (tz[k] << 24) >> 20;
            y = y0;
            z = z0;
            if (mdl->unk0 & 2) {
                y = (u32)(y0 * mdl->unk56) >> 12;
                z = (u32)(z0 * mdl->unk58) >> 12;
            }
            b = &mdl->unk20[bone[k]];
            pos.vx = b->unk14 + (((s16)z * sn) >> 12);
            pos.vz = b->unk1C + (((s16)z * cs) >> 12);
            pos.vy = b->unk18 + y;
            if (e->unkE != 0) {
                f = (u16)r->unk1C2;
                func_800D198C(e->unk8, &pos, f % (*(u16 *)e->unk8 & 0x7FFF), e->unkD, -1);
            } else {
                if ((rot.pad = e->unkF) != 0) {
                    rot.vy = (u->unk86[1] + 0xC00) & 0xFFF;
                } else {
                    rot.vy = 0;
                }
                func_800D1FFC(e->unk8, &pos, &rot, -1, (u16)r->unk1C2, e->unkD, -1);
            }
        }
    }
    arg0->unk0 = (u32 *)func_800E7154();
}

void func_800CD210(Unk800CCD00 *arg0) {
    BtlUnit *u;
    Battle4880 *m;
    BtlUnit9C *mdl;
    BtlBone *b;
    Unk800F6CA4 *e;
    SVECTOR v;
    u32 st;
    s16 bone;
    s16 y;
    s16 i;
    s16 t;
    s32 q;
    s16 d;
    u8 c;
    s32 blink;
    u32 n;

    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (((u->unk68 >> 22) & 1) || (u->unk9C->unk0 & 0x10)) {
            continue;
        }
        if (g_gameState->unk1C->unk8E4.battle->unk1358.unk1D4 & u->unkB0) {
            continue;
        }
        st = u->status;
        if ((st & 0x100) || !(st & 0x88000000)) {
            continue;
        }
        if (u->unk68 & 1) {
            bone = D_800F6B48[g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4][5];
            y = (s8)D_800F6BBC[g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4][5];
        } else {
            m = g_gameState->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0;
            bone = m->unk32[5];
            y = (s8)m->unk38[5];
        }
        mdl = u->unk9C;
        if (mdl->unk0 & 2) {
            y = (u32)(y * mdl->unk56) >> 12;
        }
        b = &mdl->unk20[bone];
        v.vx = b->unk14;
        v.vy = b->unk18 + (y << 4);
        v.vz = b->unk1C;
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&arg0->unk10);
        arg0->unk10.s.y -= 12;
        for (e = D_800F6CA4, i = 1; i != -1; e++, i--) {
            if (st & e->unk0) {
                if (u->unk66 > 0 && (t = u->unk46[e->unk4]) >= 0) {
                    q = t * 10 / u->unk66;
                    d = u->unk2E;
                    bone = q;
                    if (t <= 0) {
                        blink = 2;
                    } else {
                        blink = ((s16)q != (s16)((t - d) * 10 / u->unk66)) * 2;
                    }
                    if (e->unk6 != 0) {
                        c = (bone << 4) + 0x20;
                        arg0->unk8 = (c << 16) | (c << 8) | c;
                    } else {
                        arg0->unk8 = 0x1000000;
                    }
                    arg0->unk14 = 0;
                    arg0->unk8 |= blink << 24;
                    t = bone + 1;
                    if (t < 10) {
                        n = t | 0x10000;
                    } else {
                        n = 0x20010;
                    }
                    arg0->unk10.s.x -= (n >> 16) * 4;
                    func_800CCBC0(arg0, n);
                }
                break;
            }
        }
    }
}

void btl_showMessage(u32 arg0, ...) {
    u8 buf[0x50];
    va_list ap;
    TextTbl *t = (TextTbl *)D_800F6CB4;
    TextTblEnt *e;
    u8 *src;
    u8 *dst;
    u8 *special;
    s32 kind;
    u8 c;
    s32 v;

    arg0 += 8;
    if (arg0 < t->unk0) {
        va_start(ap, arg0);
        dst = &buf[1];
        e = t->unk4;
        src = (u8 *)e + e[arg0].unk0;
        kind = *src++;
        special = (u8 *)e + t->unk4[0].unk0;
        c = buf[0] = *src++;
        while (c != 0xFF) {
            if (c == special[0]) {
                dst--;
                v = va_arg(ap, s32);
                if (v != 0) {
                    func_8002BD54(dst, (u8 *)v);
                    goto adv;
                }
            } else if (c == special[1]) {
                dst--;
                func_8002BBE8(va_arg(ap, s32), dst, 0);
            adv:
                dst += func_8002BCC0(dst);
            }
            c = *dst++ = *src++;
        }
        btl_setBanner((s32)buf, kind);
    }
}

s32 func_800CD70C(u8 arg0) {
    s32 n = 0;
    while (arg0 >>= 1) {
        n++;
    }
    return text_getFrom(D_800F6CB4, n + 1);
}

u8 *func_800CD75C(u8 arg0) {
    return &D_8006A218[D_80069018[arg0].unk0];
}

void *func_800CD78C(BtlUnit *arg0) {
    u32 f = arg0->unk68;
    Battle *b;
    if (!(f & 1)) {
        b = g_gameState->unk1C->unk8E4.battle;
        return b->unk11C0->unk14 + b->unk488[(f >> 1) & 0xF].unk0->unk0;
    }
    return &g_gameState->unk1C->characters[(f >> 1) & 0xF];
}

void func_800CD808(s32 arg0) {
    g_gameState->unk1C->unk8E4.battle->unk1358.unk1C4 = arg0;
    g_gameState->unk1C->unk8E4.battle->unk1358.unk1C8.w = 0;
}

void func_800CD83C(void) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    BtlUnit *s = (BtlUnit *)b->unk1358.unk1C4;
    u8 buf[64];
    u32 k;
    u32 j;
    u16 mask;
    Battle *c;

    if (s == NULL) {
        return;
    }
    if (--b->unk1358.unk1C8.b[0] != 0xFF) {
        return;
    }
    c = g_gameState->unk1C->unk8E4.battle;
    switch (c->unk1358.unk1C8.b[2]) {
    case 0:
        func_8002BD54(buf, func_800CD78C(s));
        text_append(buf, (u8 *)text_getFrom(D_800F6FB8, 10));
        func_8002BBE8(s->unk7A, buf + func_8002BCC0(buf), 0);
        g_gameState->unk1C->unk8E4.battle->unk1358.unk1C8.b[2] = 1;
        break;
    case 1:
        func_8002BD54(buf, (u8 *)text_getFrom(D_800F6FB8, 11));
        func_8002BBE8(s->hp, buf + func_8002BCC0(buf), 0);
        text_append(buf, (u8 *)text_getFrom(D_800F6FB8, 13));
        func_8002BBE8(s->max->maxHp, buf + func_8002BCC0(buf), 0);
        text_append(buf, (u8 *)text_getFrom(D_800F6FB8, 12));
        func_8002BBE8(s->mp, buf + func_8002BCC0(buf), 0);
        text_append(buf, (u8 *)text_getFrom(D_800F6FB8, 13));
        func_8002BBE8(s->max->maxMp, buf + func_8002BCC0(buf), 0);
        g_gameState->unk1C->unk8E4.battle->unk1358.unk1C8.b[1] = 0;
        g_gameState->unk1C->unk8E4.battle->unk1358.unk1C8.b[2] = 2;
        break;
    case 2:
        if (!(s->unk68 & 1)) {
            mask = c->unk488[(s->unk68 >> 1) & 0xF].unk0->unk2;
            while ((j = g_gameState->unk1C->unk8E4.battle->unk1358.unk1C8.b[1]++) < 8) {
                if ((mask >> j) & 1) {
                    func_8002BD54(buf, (u8 *)text_getFrom(D_800F6FB8, j));
                    goto show;
                }
            }
        }
        g_gameState->unk1C->unk8E4.battle->unk1358.unk1C8.b[1] = 0;
        g_gameState->unk1C->unk8E4.battle->unk1358.unk1C8.b[2] = 3;
    case 3:
        mask = s->unk73 & ~s->unk70;
        while ((k = g_gameState->unk1C->unk8E4.battle->unk1358.unk1C8.b[1]++) < 8) {
            if ((mask >> k) & 1) {
                func_8002BD54(buf, (u8 *)text_getFrom(D_800F6FB8, k + 14));
                goto show;
            }
        }
    default:
        g_gameState->unk1C->unk8E4.battle->unk1358.unk1C4 = 0;
        btl_setBanner(0, 2);
        return;
    }
show:
    g_gameState->unk1C->unk8E4.battle->unk1358.unk1C8.b[0] = func_800F4FC8() - 1;
    btl_setBanner((s32)buf, 2);
}

void func_800CDC68(BtlUnit *arg0) {
    if (!(arg0->unk68 & 1)) {
        g_gameState->unk1C->unk8E4.battle->unk1358.unk1CC = &g_gameState->unk1C->unk8E4.battle->unk488[(arg0->unk68 >> 1) & 0xF];
        if (g_gameState->unk1C->unk8E4.battle->unk1358.unk1CC->unk8.w == -1) {
            btl_setBanner(text_getFrom(D_800F6FB8, 9), 2);
            g_gameState->unk1C->unk8E4.battle->unk1358.unk1D1 = 4;
            g_gameState->unk1C->unk8E4.battle->unk1358.unk1D0 = func_800F4FC8() - 1;
            return;
        }
        g_gameState->unk1C->unk8E4.battle->unk1358.unk1D1 = 0;
        g_gameState->unk1C->unk8E4.battle->unk1358.unk1D0 = 0;
    }
}

void func_800CDD68(void) {
    u8 buf[0x40];
    u8 i;
    u8 id;
    u8 *s;
    s32 kind;

    if (g_gameState->unk1C->unk8E4.battle->unk1358.unk1CC == NULL) {
        return;
    }
    if (--g_gameState->unk1C->unk8E4.battle->unk1358.unk1D0 != 0xFF) {
        return;
    }
    while ((i = g_gameState->unk1C->unk8E4.battle->unk1358.unk1D1++) < 4) {
        id = g_gameState->unk1C->unk8E4.battle->unk1358.unk1CC->unk8.b[3 - i];
        if (id != 0xFF) {
            g_gameState->unk1C->unk8E4.battle->unk1358.unk1D0 = func_800F4FC8() - 1;
            func_8002BD54(buf, (u8 *)text_getFrom(D_800F6FB8, 8));
            text_append(buf, func_800CD75C(id));
            text_append(buf, D_800A7C20);
            s = buf;
            kind = 2;
            goto show;
        }
    }
    s = NULL;
    kind = 1;
    g_gameState->unk1C->unk8E4.battle->unk1358.unk1CC = NULL;
show:
    btl_setBanner((s32)s, kind);
}

void func_800CDEF0(BtlCmd *arg0) {
    u8 *s = NULL;
    u8 buf[32];
    u16 t;
    switch (arg0->cmdNo) {
    case 0xE:
    case 0xF:
        s = func_800CD75C(arg0->subNo);
        break;
    case 0x32:
        s = g_gameState->unk1C->unk8E4.battle->unk11C0->unk14 + arg0->ability->nameOfs;
        break;
    default:
        if (arg0->subNo < 0xC0) {
            t = D_800F7124[arg0->subNo];
            switch (t) {
            case 0:
                break;
            case 0xFF:
                s = func_800EA0B4(arg0->subNo);
                break;
            case 0xFE:
                s = buf;
                func_8002BD54(s, func_800EA0B4(arg0->subNo));
                text_append(s, (u8 *)text_getFrom(D_800F71E4, 0));
                break;
            default:
                if (t < 0xC0) {
                    s = func_800EA0B4(t);
                } else {
                    s = (u8 *)text_getFrom(D_800F71E4, (t & 0x3F) + 1);
                }
                break;
            }
        }
        break;
    }
    if (s != NULL) {
        func_800F4F44((s32)s, 1);
    }
}

void func_800CE05C(Unk800CEE74 *arg0, BtlUnit9C50 *arg1) {
    s32 unused[4];
    s32 i;
    D_800FE514 = arg0;
    D_800FE504 = (BtlBgScroll *)arg0->unk10;
    D_800FE50C = arg0->unk0;
    btlbg_setOtShift(0);
    D_800FE4F8 = 0;
    if (D_800FE514->unk6 != 0) {
        D_800FE4F8 = 1;
    }
    D_800FE510 = D_800FE514->unk4;
    D_800FE4F4 = arg1;
    D_800FE508 = D_800FE514->unk2;
    if (D_800FE508 != 0) {
        func_800C9374((TexAnimSet *)arg1, NULL);
        for (i = 0; i < D_800FE508; i++) {
            BtlUnit9C50 *a;
            BtlUnit9C50 *e;
            a = D_800FE4F4 + i;
            a->unk4 |= 1;
            e = D_800FE4F4 + i;
            e->unkC = 0;
            e->unk12 = 0x1000;
        }
    }
    D_800FE518 = D_800FE514->unkA;
    func_800CEEC8(0xF);
    D_800FE4F0 = 0;
    D_800F7274 = g_gameState->unk1C->unk570;
    if (D_800F7274 >= 0x1F) {
        D_800F7274 = 0x1F;
    }
}

u8 *btlbg_draw(MATRIX *arg0, s32 arg1, s32 arg2, u32 *arg3, s32 arg4) {
    MATRIX m;
    SVECTOR rot;
    VECTOR v;
    VECTOR t;
    MATRIX m2;
    SVECTOR rot2;
    SVECTOR sv;
    s32 done;
    u8 *out;
    BtlBgGroup *grp;
    BtlBgRec *rec;
    s32 a;
    s32 d;
    s32 i;
    s32 k;
    s32 flag;

    out = (u8 *)arg2;
    memset(&rot, 0, 8);
    memset(&rot2, 0, 8);
    memset(&sv, 0, 8);
    done = 0;
    D_800FE520++;
    if (!(u8)btlbg_getBrightness()) {
        return out;
    }
    t.vx = arg0->t[0];
    t.vy = arg0->t[1];
    t.vz = arg0->t[2];
    d = arg0->m[2][1];
    if (d < 0) {
        d = -d;
    }
    t.vz = ((0x1000 - d) * 12500 + arg0->t[2] * d) / 0x1000;
    t.vx = 0;
    TransMatrix(&m2, &t);
    if (D_800F7270 == 0) {
        for (i = 0; i < D_800FE518; i++) {
            a = arg1 + D_800FE504[i].unk0;
            ((u8 *)a)[0] += D_800FE504[i].unk3;
            ((u8 *)a)[1] += D_800FE504[i].unk4;
            D_800FE504[i].unk5++;
            if (D_800FE504[i].unk5 >= D_800FE504[i].unk6) {
                switch (D_800FE504[i].unk2) {
                case 0:
                    ((u8 *)a)[0] -= D_800FE504[i].unk6 * D_800FE504[i].unk3;
                    ((u8 *)a)[1] -= D_800FE504[i].unk6 * D_800FE504[i].unk4;
                    break;
                case 1:
                    D_800FE504[i].unk3 = -D_800FE504[i].unk3;
                    D_800FE504[i].unk4 = -D_800FE504[i].unk4;
                    break;
                }
                D_800FE504[i].unk5 = 0;
            }
        }
    }
    if (D_800FE508 != 0) {
        func_800C9040((TexAnimSet *)D_800FE4F4);
    }
    SetRotMatrix(arg0);
    SetTransMatrix(arg0);
    if (D_800F7274 != 0) {
        out = func_800CEBC8(out, arg3, arg0);
    }
    grp = (BtlBgGroup *)arg1;
    k = 0;
    for (;;) {
        rec = (BtlBgRec *)((u8 *)grp + grp->unk6);
        for (i = 0; i < grp->unk4; rec++, i++) {
            switch (rec->unk0) {
            case 8:
                if (!done) {
                    D_800FE4F0 += D_800FE510;
                    rot.vy = D_800FE4F0 >> 3;
                    func_80030AF4(&rot, &m);
                    done = 1;
                    MulMatrix2(arg0, &m);
                    SetRotMatrix(&m);
                }
                if (D_800FE51C & 8) {
                    if (D_800FE4F8 != 0) {
                        SetTransMatrix(&m2);
                        SetGeomScreen(0x200);
                        out = btlbg_drawRecord(rec, out, (u32 *)arg4, grp);
                        SetGeomScreen(g_gameState->unk1C->unkA);
                    } else {
                        out = btlbg_drawRecord(rec, out, (u32 *)arg4, grp);
                    }
                }
                break;
            case 4:
                if (D_800FE51C & 4) {
                    out = btlbg_drawRecord(rec, out, (u32 *)arg4, grp);
                }
                break;
            case 2:
                if (D_800FE51C & 2) {
                    out = btlbg_drawRecord(rec, out, (u32 *)arg4, grp);
                }
                break;
            case 0:
                if (D_800FE51C & 1) {
                    out = btlbg_drawRecord(rec, out, arg3, grp);
                }
                break;
            }
        }
        if (k == D_800FE514->unk8) {
            return out;
        }
        k++;
        grp = (BtlBgGroup *)((u8 *)grp + grp->unk0);
        flag = 0;
        switch (D_800FE514->unk0) {
        case 0x44:
            a = math_sin((VSync(-1) * 26) & 0xFFE);
            sv.vx = 0x429;
            sv.vy = -0x541;
            sv.vz = 0xEA6;
            a /= 5;
            rot2.vz = a;
            break;
        case 0x70:
            switch (k) {
            case 1:
                a = (VSync(-1) * 5) & 0xFFF;
                rot2.vz = 0xFFF - a;
                a = (VSync(-1) * 3) & 0xFFF;
                rot2.vy = 0xFFF - a;
                a = math_sin(((VSync(-1) + 8) * 22) & 0xFFE);
                sv.vx = -0x834;
                sv.vz = -0x352;
                sv.vy = a / 45 - 250;
                break;
            case 2:
                rot2.vy = 0;
                rot2.vz = 0;
                a = math_sin((VSync(-1) * 20) & 0xFFC);
                sv.vx = 0x6BD;
                sv.vy = a / 64 - 0x5DC;
                sv.vz = -0x4B;
                break;
            case 3:
                a = (VSync(-1) * 4) & 0xFFF;
                rot2.vz = a;
                a = (VSync(-1) * 3) & 0xFFF;
                rot2.vy = a;
                a = math_sin(((VSync(-1) + 16) * 21) & 0xFFF);
                sv.vx = 0x6D6;
                sv.vz = 0x401;
                sv.vy = a / 50 - 0x307;
                break;
            }
            break;
        case 0x6E:
            a = math_sin((VSync(-1) * 12) & 0xFFC);
            a /= 64;
            rot2.vy = 0x200;
            sv.vx = 0x5DC;
            sv.vy = -0x1B58;
            rot2.vz = a;
            sv.vz = 0xEA6;
            break;
        case 0xA8:
            rot2.vx = 0;
            rot2.vz = 0;
            if (k == 1) {
                rot2.vy = (VSync(-1) / 16) & 0xFFF;
            } else {
                rot2.vy = (VSync(-1) / 8) & 0xFFF;
            }
            break;
        case 0xAB:
            if (k == 1) {
                a = -(VSync(-1) * 12) & 0xFFF;
                sv.vy = -0x947;
                rot2.vz = a;
                sv.vx = 0;
                sv.vz = 0xEA6;
            } else {
                a = -(VSync(-1) * 22) & 0xFFF;
                flag = 1;
                rot2.vz = 0xE00;
                sv.vy = -0x8CA;
                rot2.vy = a;
                sv.vx = 0;
                sv.vz = 0x1DC9;
            }
            break;
        case 7:
            if (k == 1) {
                if (!((D_800FE520 + 0x1F) & 0x3F)) {
                    D_800FE4FC = rand() & 0x1FF;
                }
                rot2.vy = D_800FE4FC + (rand() & 0x3F);
            } else {
                if (!(D_800FE520 & 0x3F)) {
                    D_800FE500 = rand() & 0x3FF;
                }
                rot2.vy = D_800FE500 + (rand() & 0x7F);
            }
            break;
        default:
            sv.vx = 0;
            sv.vy = -10;
            sv.vz = 0;
            rot2.vy += 3;
            break;
        }
        ApplyMatrix(arg0, &sv, &v);
        m.t[0] = arg0->t[0] + v.vx;
        m.t[1] = arg0->t[1] + v.vy;
        m.t[2] = arg0->t[2] + v.vz;
        if (flag == 0) {
            func_80030AF4(&rot2, &m);
        } else {
            func_80031244(&rot2, &m);
        }
        MulMatrix2(arg0, &m);
        SetRotMatrix(&m);
        SetTransMatrix(&m);
    }
}

void func_800CEBAC(void) {
    D_800F7274 = 0x1F;
}

void func_800CEBBC(void) {
    D_800F7274 = 0;
}

u8 *func_800CEBC8(u8 *arg0, u32 *arg1, MATRIX *arg2) {
    SVECTOR v;
    MATRIX m;
    long p;
    long flag;
    s32 i;

    SetGeomScreen(0x200);
    i = 0;
    PushMatrix();
    SetDrawStp((DR_STP *)arg0, 0);
    AddPrim(&arg1[2], arg0);
    arg0 += sizeof(DR_STP);
    m = *arg2;
    m.t[0] = 0;
    m.t[1] = 0;
    m.t[2] = 1000;
    SetTransMatrix(&m);
    for (; i < D_800F7274; i++) {
        SetLineG2((LINE_G2 *)arg0);
        SetSemiTrans(arg0, 1);
        v.vx = (((rand() & 0x1FF) - 0x100) * 41) / 32;
        v.vy = ((rand() & 0xFF) * 50) / 32 - 0xDC;
        v.vz = (((rand() & 0x1FF) - 0x100) * 41) / 32;
        RotTransPers(&v, (long *)&((LINE_G2 *)arg0)->x0, &p, &flag);
        v.vy += 0x55 + (rand() & 0x1F);
        RotTransPers(&v, (long *)&((LINE_G2 *)arg0)->x1, &p, &flag);
        setRGB0((LINE_G2 *)arg0, 0x19, 0x19, 0x32);
        setRGB1((LINE_G2 *)arg0, 0x50, 0x50, 0x82);
        AddPrim(&arg1[2], arg0);
        arg0 += sizeof(LINE_G2);
    }
    SetDrawStp((DR_STP *)arg0, 1);
    AddPrim(&arg1[2], arg0);
    arg0 += sizeof(DR_STP);
    SetDrawTPage(arg0, 0, 0, GetTPage(0, 3, 0, 0));
    AddPrim(&arg1[2], arg0);
    PopMatrix();
    arg0 += sizeof(DR_TPAGE);
    SetGeomScreen(g_gameState->unk1C->unkA);
    return arg0;
}

Unk800CEE74 *func_800CEE74(void) {
    return D_800FE514;
}

void func_800CEE84(s32 arg0) {
    D_800F7270 = arg0;
}

s32 func_800CEE90(void) {
    return D_800FE50C;
}

void func_800CEEA0(s32 arg0) {
    D_800FE510 += arg0;
}

s32 func_800CEEB8(void) {
    return D_800FE510;
}

void func_800CEEC8(s32 arg0) {
    D_800FE51C = arg0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlbg_drawRecord);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlbg_projectVertices);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlbg_setBrightness);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlbg_getBrightness);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", func_800CF588);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlbg_setOtShift);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_pushTrailPoint);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_distance);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_rand);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_sqrt);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_randRange);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_randCentered);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_randRotation);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_aimAngles);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_transformPoint);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_transformPolar);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_lerp);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_lerpRatio);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_3", btlfx_lerpVec);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_3", D_800A7C20);
