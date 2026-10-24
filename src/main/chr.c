#include "ff9.h"

void chr_init(s32 ch) {
    Character *c = &g_gameState->unk1C->characters[ch];
    u32 *init;
    u32 *tbl;
    s32 r;

    memset(c, 0, 0x90);
    tbl = D_8006CA78;
    init = tbl + ch;
    c->unk34.w = *init;
    c->unk38 = 0;
    c->unkA = func_80033A50(c->unk34.b.row);
    memset(c->unk3E, 0, 10);
    func_80034394(ch);
    r = chr_getDefaultSet(c);
    chr_setDefaultName(c->unk0, r);
    chr_setDefaultEquip(c->equip, r);
    c->unk34.b.unk4 = func_80033280(c->unk34.b.id, (c->unkA >> 4) & 1, c->equip);
    chr_setLevel(ch, 1, init, 0);
    c->hp = c->maxHp;
    c->mp = c->maxMp;
    c->stones = c->maxStones;
}

void chr_setLevel(s32 ch, s32 lv, u32 *heal, u8 f) {
    Character *c = &g_gameState->unk1C->characters[ch];
    s32 hp;
    s32 mp;
    s32 d;

    if (!f && c->level != lv) {
        c->exp = g_levelExp[lv - 1];
    }
    hp = c->hp;
    mp = c->mp;
    d = c->maxStones - c->stones;
    c->level = lv;
    memset(&c->hp, 0, 8);
    memset(&c->maxHp, 0, 8);
    c->unk16 = 10;
    c->baseSpeed = chr_calcBaseSpeed(ch, c->level, f);
    c->baseStrength = chr_calcBaseStrength(ch, c->level, f);
    c->baseMagic = chr_calcBaseMagic(ch, c->level, f);
    c->baseSpirit = chr_calcBaseSpirit(ch, c->level, f);
    c->maxHp = c->unk2C = chr_calcMaxHp(c->level, c->baseStrength);
    c->maxMp = c->unk2E = chr_calcMaxMp(c->level, c->baseMagic);
    c->maxStones = chr_calcMaxStones(ch, c->level, f);
    c->hp = (hp < c->maxHp) ? hp : c->maxHp;
    c->mp = (mp < c->maxMp) ? mp : c->maxMp;
    c->stones = c->maxStones - d;
    if (heal) {
        c->hp = c->maxHp;
        c->mp = c->maxMp;
        c->stones = c->maxStones;
    }
    func_8003384C(c);
    if (f) {
        c->hp = (hp < c->maxHp) ? hp : c->maxHp;
        c->mp = (mp < c->maxMp) ? mp : c->maxMp;
    }
    if (c->maxStones < d) {
        func_80033DCC(c);
    }
}

Unk80032E84Out *func_80032E84(Unk80032E84In *in, Unk80032E84Out *out) {
    s32 i;
    u8 *a;
    u8 *e;
    s32 id;

    memset(out, 0, sizeof(Unk80032E84Out));
    out->unk0 = in->unk10;
    out->unk2 = in->unk12;
    out->unk4 = *(u16 *)&in->unk0.unk0[0];
    out->unk6 = *(u16 *)&in->unk0.unk0[2];
    out->unk8[0] = in->unk0.unk0[4];
    out->unk8[1] = in->unk0.unk0[5];
    out->unk8[2] = in->unk0.unk0[6];
    out->unk8[3] = in->unk0.unk0[7];
    id = in->unk14.unk0[0];
    if (id != 0xFF) {
        out->unkC[0] = D_8006E0E8[id].unk5;
    }
    for (i = 0; i < 4; i++) {
        id = (&in->unk14.unk0[1])[i];
        if (id != 0xFF) {
            if ((u32)(id - 0x58) < 0x88) {
                a = D_8006D1C8[id];
                out->unkC[1] += a[0];
                out->unkC[2] += a[1];
                out->unkC[3] += a[2];
                out->unkC[4] += a[3];
            }
        }
    }
    for (i = 0; i < 5; i++) {
        id = in->unk14.unk0[i];
        if (id != 0xFF) {
            e = D_8006D548[D_80069018[id].unkB].unk0;
            out->unk8[0] += e[0];
            out->unk8[1] += e[1];
            out->unk8[2] += e[2];
            out->unk8[3] += e[3];
        }
    }
    for (i = 0; i < 4; i++) {
        if (func_80034A04(in->unk8.unk0, D_8006CA9C[i])) {
            switch (D_8006CA9C[i]) {
            case 0xC5:
                out->unk4 += out->unk4 / 10;
                break;
            case 0xC6:
                out->unk4 += out->unk4 / 5;
                break;
            case 0xC7:
                out->unk6 += out->unk6 / 10;
                break;
            case 0xC8:
                out->unk6 += out->unk6 / 5;
                break;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (out->unk8[i] > D_8006CAA0[i]) {
            out->unk8[i] = D_8006CAA0[i];
        }
    }
    for (i = 0; i < 5; i++) {
        if (out->unkC[i] >= 1000) {
            out->unkC[i] = 999;
        }
    }
    if (out->unk4 >= 10000) {
        out->unk4 = 9999;
    }
    if (out->unk6 >= 1000) {
        out->unk6 = 999;
    }
    if (out->unk4 < out->unk0) {
        out->unk0 = out->unk4;
    }
    if (out->unk6 < out->unk2) {
        out->unk2 = out->unk6;
    }
    return out;
}

s32 func_80033280(s32 kind, s32 flag, u8 *item) {
    u8 t = D_8006CAA4[kind][(u8)flag != 0];
    u8 k = D_80069018[*item].unk8;
    s32 r;

    switch (t) {
    case 0:
        return k != 1;
    case 1:
        return 2;
    case 2:
        if ((u16)g_evtGlobalVars.h[0] >= 10300) {
            r = 5;
            if (k == 7) {
                r = 6;
            }
            return r;
        }
        r = 3;
        if (k == 7) {
            r = 4;
        }
        return r;
    case 3:
        return 7;
    case 4:
        return 12;
    case 5:
        return 9;
    case 6:
        r = 10;
        if (k == 7) {
            r = 11;
        }
        return r;
    case 7:
        return 13;
    case 8:
        return 14;
    case 9:
        return 15;
    case 10:
        r = 16;
        if ((u16)(g_evtGlobalVars.h[0] - 1500) < 100) {
            r = 17;
        }
        return r;
    case 11:
        return 18;
    }
    return -1;
}

void func_800333B0(s32 ch, u8 avg, s32 eq) {
    Character *c = &g_gameState->unk1C->characters[ch];
    u8 *p;
    s32 i;
    s32 n;
    s32 sum;
    s32 lv;

    if (eq != 0xFF) {
        p = c->equip;
        for (i = 0; i < 5; i++) {
            p[i] = D_8006CAD8[eq][i];
        }
    }
    c->unk34.b.unk4 = func_80033280(c->unk34.b.id, (c->unkA >> 4) & 1, c->equip);
    if (avg == 0) {
        if (eq == 0xFF) {
            return;
        }
        goto calc;
    }
    n = 0;
    sum = 0;
    for (i = 0; i < 9; i++) {
        if (ch != i) {
            if (g_gameState->unk1C->characters[i].unk34.b.unkB) {
                n++;
                sum += g_gameState->unk1C->characters[i].level;
            }
        }
    }
    if (n != 0) {
        lv = sum / n;
    } else {
        lv = 1;
    }
    if ((u32)(c->unk34.b.unk4 - 10) >= 2) {
        if (lv < c->level) {
            lv = c->level;
        }
    }
    chr_setLevel(ch, lv, NULL, 0);
    return;
calc:
    func_8003384C_inline(c);
}

void party_initNewGame(void) {
    Character **party = g_gameState->unk1C->party;
    s32 i;
    func_80033D1C(0);
    memset(party, 0, 0x1C);
    for (i = 0; i < 9; i++) {
        chr_init(i);
    }
    func_80033BD0(0);
    func_80033BD0(1);
    func_80033BD0(2);
    func_80033BD0(3);
    func_80033BD0(4);
    func_80033BD0(5);
    func_80033BD0(7);
    func_80033BD0(6);
    party_setMember(0, 0);
    party_setMember(1, 1);
    party_setMember(2, 2);
    party_setMember(3, 3);
    g_gameState->unk1C->gil = 500;
    g_gameState->unk1C->firstUseFxSeen = 0;
}

s32 party_getFirstMember(void) {
    GameData *gd = g_gameState->unk1C;
    s32 i = 0;
    do {
    loop:
        if (gd->party[i] == NULL) {
            i = i < 3 ? i + 1 : 0;
            goto loop;
        }
    } while (0);
    return i;
}

s32 func_800337D0(s32 i) {
    do {
        i = i == 0 ? 3 : i - 1;
    } while (g_gameState->unk1C->party[i] == NULL);
    return i;
}

s32 func_8003380C(s32 i) {
    do {
        i = i < 3 ? i + 1 : 0;
    } while (g_gameState->unk1C->party[i] == NULL);
    return i;
}

void func_8003384C(Character *c) {
    Unk80032E84In in;
    Unk80032E84Out out;

    in.unk0 = *(Bytes8 *)&c->unk2C;
    in.unk10 = c->hp;
    in.unk12 = c->mp;
    in.unk8 = *(Words2 *)c->unk88;
    in.unk14 = *(Bytes5 *)c->equip;
    func_80032E84(&in, &out);
    c->speed = out.unk8[0];
    c->strength = out.unk8[1];
    c->magic = out.unk8[2];
    c->spirit = out.unk8[3];
    c->defense = out.unkC[1];
    c->evade = out.unkC[2];
    c->magicDef = out.unkC[3];
    c->magicEva = out.unkC[4];
    c->hp = out.unk0;
    c->mp = out.unk2;
    c->maxHp = out.unk4;
    c->maxMp = out.unk6;
}

s32 func_80033964(u32 x) {
    switch (x) {
    case 0:
    case 16:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    case 4:
        return 4;
    case 5:
        return 5;
    case 6:
        return 6;
    case 7:
        return 7;
    case 8:
    case 9:
    case 17:
        return 8;
    case 10:
    case 11:
    case 18:
        return 9;
    case 12:
    case 13:
    case 19:
        return 10;
    case 14:
    case 15:
        return 11;
    }
    return -1;
}

u8 func_800339F4(s32 a, u8 b) {
    return D_8006CAA4[a][b != 0];
}

s32 chr_getDefaultSet(Character *c) {
    return D_8006CAB8[c->unk34.b.id][(c->unkA & 0x10) != 0];
}

u8 func_80033A50(u32 x) {
    u8 *tbl = D_8006CACC;
    s32 i;
    switch (x) {
    case 0:
    case 16:
        i = 0;
        break;
    case 1:
        i = 1;
        break;
    case 2:
        i = 2;
        break;
    case 3:
        i = 3;
        break;
    case 4:
        i = 4;
        break;
    case 5:
        i = 5;
        break;
    case 6:
        i = 6;
        break;
    case 7:
        i = 7;
        break;
    case 8:
    case 9:
    case 17:
        i = 8;
        break;
    case 10:
    case 11:
    case 18:
        i = 9;
        break;
    case 12:
    case 13:
    case 19:
        i = 10;
        break;
    case 14:
    case 15:
        i = 11;
        break;
    default:
        i = -1;
        break;
    }
    return tbl[i];
}

void party_setMember(s32 slot, s32 ch) {
    Character *old = g_gameState->unk1C->party[slot];
    if (ch >= 9) {
        ch = -1;
    }
    if (ch < 0) {
        g_gameState->unk1C->party[slot] = NULL;
    } else {
        func_80033BD0(ch);
        g_gameState->unk1C->party[slot] = &g_gameState->unk1C->characters[ch];
    }
    if (old != g_gameState->unk1C->party[slot]) {
        func_80033D1C(1);
    }
}

void func_80033BD0(u32 i) {
    if (i < 9) {
        g_gameState->unk1C->characters[i].unk34.b.unkB = 1;
    }
}

void func_80033C0C(u32 i) {
    if (i < 9) {
        g_gameState->unk1C->characters[i].unk34.b.unkB = 0;
    }
}

s32 chr_resetToLevel(s32 ch, s32 b, u8 flag) {
    Character *c = &g_gameState->unk1C->characters[ch];
    s32 lv = c->level;
    c->level = 0;
    chr_setLevel(ch, b, 0, 0);
    if (flag) {
        c->hp = c->maxHp;
        c->mp = c->maxMp;
    }
    return lv;
}

s32 func_80033CCC(s32 ch, s32 b) {
    Character *c = &g_gameState->unk1C->characters[ch];
    s32 r = c->level;
    chr_setLevel(ch, b, 0, 1);
    return r;
}

void func_80033D1C(u8 a) {
    D_80077DE0 = a;
}

u8 func_80033D28(void) {
    return D_80077DE0;
}

/* default character names: count, then {offset, length} entries */
INCLUDE_RODATA("asm/us/main/nonmatchings/chr", D_80010DC4);

void chr_setDefaultName(u8 *dst, s32 id) {
    memset(dst, 0xFF, 10);
    func_8002BD54(dst, (u8 *)text_getFrom((u8 *)D_80010DC4, id));
}

void chr_setDefaultEquip(u8 *dst, s32 b) {
    s32 i;
    for (i = 0; i < 5; i++) {
        dst[i] = D_8006CAD8[b][i];
    }
}

void func_80033DCC(Character *c) {
    s32 i;
    Unk8006D128 *e;
    s32 id;

    i = 0;
    e = D_8006D128;
    id = c->unk34.b.id;
    c->stones = c->maxStones;
    do {
        if (func_800349BC(id, i + 0xC0)) {
            if (c->stones >= e->unk1) {
                c->stones -= e->unk1;
            } else {
                func_8003493C(id, i + 0xC0, 0);
            }
        }
        i++;
        e++;
    } while (i < 64);
}

s32 func_80033E84(s32 ch, s32 id, s32 add) {
    s32 i = func_80034430_inline(ch, id);
    s32 old;
    s32 need;

    if (i < 0) {
        return 0;
    }
    old = g_gameState->unk1C->characters[ch].unk58[i];
    need = func_800347D8_inline(ch, id);
    if (need - g_gameState->unk1C->characters[ch].unk58[i] < add) {
        g_gameState->unk1C->characters[ch].unk58[i] = need;
    } else {
        g_gameState->unk1C->characters[ch].unk58[i] += add;
    }
    return g_gameState->unk1C->characters[ch].unk58[i] - old;
}

s32 func_80034070(s32 ch, s32 id) {
    s32 ap = func_80034430_ap_inline(ch, id);
    s32 need = func_800347D8_inline(ch, id);

    if (ap < 0 || need < 0) {
        return 0;
    }
    return ap >= need;
}

s32 func_80034220(s32 ch, s32 id) {
    s32 i = func_80034430_inline(ch, id);
    Character *c = g_gameState->unk1C->characters + ch;
    if (i < 0) {
        return 0;
    }
    c->unk58[i] = D_8006CB28[c->unk34.b.row][i].unk1;
    return 1;
}

void func_8003431C(void) {
}

void func_80034324(void) {
    s32 i;
    for (i = 0; i < 9; i++) {
        func_800343C0(i);
    }
}

void func_8003435C(void) {
    s32 i;
    for (i = 0; i < 9; i++) {
        func_80034404(i);
    }
}

void func_80034394(s32 a) {
    func_800343C0(a);
    func_80034404(a);
}

void func_800343C0(s32 ch) {
    s32 i;
    Character *c;

    if (ch < 8) {
        c = &g_gameState->unk1C->characters[ch];
        for (i = 0; i < 0x30; i++) {
            c->unk58[i] = 0;
        }
    }
}

void func_80034404(s32 i) {
    Character *c = &g_gameState->unk1C->characters[i];
    c->unk88[1] = 0;
    c->unk88[0] = 0;
}

s32 func_80034430(s32 ch, s32 id) {
    return func_80034430_inline(ch, id);
}

u8 *func_800344C0(s32 ch, s32 id) {
    s32 i = func_80034430_inline(ch, id);
    if (i < 0) {
        return NULL;
    }
    return &g_gameState->unk1C->characters[ch].unk58[i];
}

s32 func_80034584(s32 ch, s32 id) {
    s32 i = func_80034430_inline(ch, id);
    if (i < 0) {
        return -1;
    }
    return g_gameState->unk1C->characters[ch].unk58[i];
}

s32 func_80034648(s32 ch, s32 id, u8 v) {
    s32 i = func_80034430_inline(ch, id);
    s32 old;
    if (i < 0) {
        return -1;
    }
    old = g_gameState->unk1C->characters[ch].unk58[i];
    g_gameState->unk1C->characters[ch].unk58[i] = v;
    return old;
}

static inline s32 func_80034648_inline(s32 ch, s32 id, s32 v) {
    s32 i = func_80034430_inline(ch, id);
    s32 old;
    if (i < 0) {
        return -1;
    }
    old = g_gameState->unk1C->characters[ch].unk58[i];
    g_gameState->unk1C->characters[ch].unk58[i] = v;
    return old;
}

s32 func_8003470C(s32 ch, s32 id) {
    s32 r = func_80034648_inline(ch, id, 0);
    return r;
}

u8 func_800347D8(s32 ch, s32 id) {
    s32 i = func_80034430_inline(ch, id);
    Character *c = g_gameState->unk1C->characters + ch;
    if (i < 0) {
        return 0;
    }
    return D_8006CB28[c->unk34.b.row][i].unk1;
}

s32 func_800348C0(s32 ch) {
    s32 sum = 0;
    s32 i;
    u32 *bits = g_gameState->unk1C->characters[ch].unk88;
    for (i = 0; i < 0x40; i++) {
        if (bits[i >> 5] & (1 << (i & 0x1F))) {
            sum += D_8006D128[i].unk1;
        }
    }
    return sum;
}

void func_8003493C(s32 ch, s32 id, u8 on) {
    u32 *bits = g_gameState->unk1C->characters[ch].unk88;
    id -= 0xC0;
    if (on) {
        bits[id >> 5] |= 1 << (id & 0x1F);
    } else {
        bits[id >> 5] &= ~(1 << (id & 0x1F));
    }
}

u8 func_800349BC(s32 ch, s32 id) {
    return func_80034A04_inline(g_gameState->unk1C->characters[ch].unk88, id);
}

u8 func_80034A04(u32 *bits, s32 b) {
    b -= 0xC0;
    return (bits[b >> 5] & (1 << (b & 0x1F))) != 0;
}

u8 func_80034A30(Character *a) {
    if (a->unkA & 0x10) {
        return 0;
    }
    return (s32)a->unk34.b.row < 8;
}

s32 chr_calcBaseSpeed(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->unk1C->characters + ch;
    Unk8006DD88 *e = &D_8006DD88[func_80033964(c->unk34.b.row)];
    s32 r;
    if (add) {
        c->unk3E[0] += func_8003502C(c->equip, 0);
    }
    r = e->unk0 + lv / 10 + (c->unk3E[0] >> 5);
    if (r >= 51) {
        r = 50;
    }
    return r;
}

s32 chr_calcBaseStrength(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->unk1C->characters + ch;
    s32 k = func_80033964(c->unk34.b.row);
    u16 *bonus = c->unk3E;
    Unk8006DD88 *e = &D_8006DD88[k];
    s32 r;
    s32 n;
    if (add) {
        n = 0;
        if (c->stones == 0) {
            n = 3;
        }
        bonus[1] += n + func_8003502C(c->equip, 1);
    }
    r = e->unk1 + lv * 3 / 10 + (bonus[1] >> 5);
    if (r >= 100) {
        r = 99;
    }
    return r;
}

s32 chr_calcBaseMagic(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->unk1C->characters + ch;
    s32 k = func_80033964(c->unk34.b.row);
    u16 *bonus = c->unk3E;
    Unk8006DD88 *e = &D_8006DD88[k];
    s32 r;
    s32 n;
    if (add) {
        n = 0;
        if (c->stones == 0) {
            n = 3;
        }
        bonus[2] += n + func_8003502C(c->equip, 2);
    }
    r = e->unk2 + lv * 3 / 10 + (bonus[2] >> 5);
    if (r >= 100) {
        r = 99;
    }
    return r;
}

s32 chr_calcBaseSpirit(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->unk1C->characters + ch;
    s32 k = func_80033964(c->unk34.b.row);
    u16 *bonus = c->unk3E;
    Unk8006DD88 *e = &D_8006DD88[k];
    s32 r;
    s32 n;
    if (add) {
        n = c->stones == 0;
        bonus[3] += n + func_8003502C(c->equip, 3);
    }
    r = e->unk3 + lv * 3 / 20 + (bonus[3] >> 5);
    if (r >= 51) {
        r = 50;
    }
    return r;
}

s32 chr_calcMaxStones(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->unk1C->characters + ch;
    u16 *bonus = c->unk3E;
    s32 k = func_80033964(c->unk34.b.row);
    Unk8006DD88 *e = &D_8006DD88[k];
    s32 r;
    s32 n;

    if (add) {
        n = 0;
        if (c->stones == 0) {
            n = 5;
        }
        bonus[4] += n;
    }
    r = e->unk4 + lv * 4 / 10 + (bonus[4] >> 5);
    if (r >= 100) {
        r = 99;
    }
    return r;
}

s32 chr_calcMaxHp(s32 lv, s32 m) {
    s32 r = D_8006DDD0[lv - 1].unk0 * m / 50;
    if (r >= 10000) {
        r = 9999;
    }
    return r;
}

s32 chr_calcMaxMp(s32 lv, s32 m) {
    s32 r = D_8006DDD0[lv - 1].unk2 * m / 100;
    if (r >= 1000) {
        r = 999;
    }
    return r;
}

s32 func_8003502C(u8 *eq, s32 k) {
    s32 sum = 0;
    s32 i;
    u8 *e;

    for (i = 0; i < 5; i++) {
        if (eq[i] != 0xFF) {
            e = D_8006D548[D_80069018[eq[i]].unkB].unk0;
            switch (k) {
            case 0:
                sum += e[0];
                break;
            case 1:
                sum += e[1];
                break;
            case 2:
                sum += e[2];
                break;
            case 3:
                sum += e[3];
                break;
            }
        }
    }
    return sum;
}

