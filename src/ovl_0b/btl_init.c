#include "ovl_0b.h"

void btl_initParty(Battle *arg0) {
    s16 n;
    s16 i;
    Unk16D8 *p;
    Character *c;
    BtlUnit *u;
    s32 k;

    i = n = 0;
    p = evt_getObjectList()->unk0;
    for (; i < 4; i++) {
        c = g_gameState->unk1C->party[i];
        if (c != NULL) {
            u = &arg0->units[n];
            u->unk84 = D_800F5200[(c->unk34.w >> 4) & 0x1F];
            func_800BBF30(p->unk4, u->unk84);
            btl_initPartyUnit(c, u, i, n);
            n++;
            p = p->unk0;
            btl_insertUnit(u);
        }
    }
    for (; n < 4; n++) {
        arg0->units[n].unkB0 = 0;
    }
    arg0->unkD85 |= 0x10;
    func_800C9A28();
    k = arg0->unk132C.unk0 & 3;
    if (k == 0) {
        for (u = arg0->unk6F8; u != NULL; u = u->unk0) {
            if (u->unk68 & 1) {
                u->atb = 0;
            }
        }
    } else if (k == 1) {
        for (u = arg0->unk6F8; u != NULL; u = u->unk0) {
            if (!(u->unk68 & 1)) {
                u->atb = 0;
            }
        }
    }
}

void btl_initEnemies(Battle *arg0) {
    s16 i = 0;
    s16 n = 4;
    Unk16D8 *p = func_8004AEBC()->unk0;
    BtlUnit *prev = NULL;
    Battle488 *e;
    BtlUnit *u;
    s16 m;
    s32 k;

    for (; i < g_gameState->unk1C->unk8E4.battle->unk132C.unk4->unk1; i++, n++) {
        e = &arg0->unk488[i];
        e->unk4 = (e->unk4 & ~0x3F) | 0x20;
        u = &arg0->units[n];
        u->unk84 = m = func_800CA2E4(i);
        if (m < 0) {
            e->unk4 |= 0x200;
        } else {
            func_800BBF30(p->unk4, m);
            e->unk4 &= ~0x200;
            p = p->unk0;
        }
        k = i;
        u->unkB0 = 0x10 << k;
        u->unk68 &= ~1;
        u->unk68 = (u->unk68 & ~0x1E) | ((k & 0xF) << 1);
        u->unk68 = (u->unk68 & ~0xE0) | (((k + 4) & 7) << 5);
        u->unk68 &= ~0x40000000;
        u->unk68 = (u->unk68 & ~0x800000) | (((e->unk4 >> 9) & 1) << 23);
        btl_resetUnit(u, &g_gameState->unk1C->unk8E0[k]);
        u->unk98 = (struct BtlUnit98 *)&e->unkC;
        u->unkA8 = NULL;
        u->unkC4 = (u32 *)D_800F5244;
        u->unk68 &= ~0x40000;
        if (prev != NULL) {
            prev->unk0 = u;
        }
        prev = u;
    }
    for (; n < 8; n++) {
        arg0->units[n].unkB0 = 0;
    }
    prev->unk0 = NULL;
    arg0->unk6F8 = &arg0->units[4];
    arg0->unkD85 |= 8;
    func_800C9794();
    btlseq_init();
}

void btl_initPartyUnit(Character *c, BtlUnit *u, u16 slot, u16 n) {
    s32 k;
    s32 s;
    s32 i;

    u->unkB0 = 1 << n;
    g_btlRewards.unkA |= 1 << (s = slot);
    u->unk68 |= 1;
    u->unk68 = (u->unk68 & ~0x1E) | ((c->unk34.w & 0xF) << 1);
    u->unk68 |= 0x10000000;
    u->unk68 = (u->unk68 & ~0xE0) | ((s & 7) << 5);
    u->unk68 &= ~0x800000;
    if (g_evtGlobalVars.b[0x10] == 0 || (c->unkA & 0x10) ||
        (((u->unk68 >> 1) & 0xF) == 2 && g_evtGlobalVars.b[0x11] != 0)) {
        u->unk78 = 0;
        u->unk68 &= ~0x40000000;
    } else {
        u->unk68 |= 0x40000000;
        u->unk78 = c->trance;
    }
    u->unk7C = 0;
    u->unkC4 = c->unk88;
    u->unk30 = c->speed;
    u->unk31 = c->strength;
    u->unk32 = c->magic;
    u->unk33 = c->spirit;
    u->unk7A = c->level;
    u->max = (BtlUnit24 *)&c->maxHp;
    func_800BD8C0((Unk800BD8C0 *)&u->hp, (Unk800BD8C0 *)&c->hp);
    btl_resetUnit(u, &g_gameState->unk1C->unk8D8[c->unk34.w & 0xF]);
    u->unkAE = 0xFFFF;
    func_800C0A3C((Unk800C09D8 *)u);
    u->max->maxAtb = (0x3C - u->unk30) * 160;
    btl_resetAtbSpeed((Unk800BD8F0 *)u);
    k = g_gameState->unk1C->unk8E4.battle->unk132C.unk0 & 3;
    if (k == 0) {
        u->atb = 0;
    } else if (k == 1) {
        u->atb = u->max->maxAtb - 1;
    } else {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
        }
        u->atb = ((*g_randPtr << 8) | *g_randPtr++) % u->max->maxAtb;
    }
    u->unkA8 = &g_gameState->unk1C->unk8E4.battle->unk228[n].unk0;
    func_800BD698(u, (c->unk34.w >> 4) & 0x1F, n);
    btl_initWeapon(c, u);
    u->unk6C = c->defense;
    u->unk6D = c->evade;
    u->unk6E = c->magicDef;
    u->unk6F = c->magicEva;
    btl_initEquipElements(c, u);
    for (i = 0; i < u->unkA8->unk8->unk3; i++) {
        func_800C8D18(u->unkA8, i, 0, 0, 0);
    }
    if (u->hp * 6 < u->max->maxHp) {
        u->status |= BTL_STATUS_LOW_HP;
    }
    btl_addStatuses(u, c->unk38 & 0xFE);
    if (c->unk38 & 1) {
        btl_addStatus(u, BTL_STATUS_PETRIFY);
    }
    btl_applyAutoAbilities(u);
    u->unk98 = (struct BtlUnit98 *)&u->unkA0->unk14;
    if (u->hp == 0 && btl_addStatus(u, BTL_STATUS_KO) == 2) {
        u->unk7D = 5;
        func_800BD5A0(u);
        return;
    }
    u->unk68 = (u->unk68 & ~0x40000) | ((btl_hasStatus(u, (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) & 1) << 18);
    u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
    u->unkA0->unk3A = 0;
}

u32 *func_800BB974(Battle *arg0, u32 *arg1) {
    SVECTOR r2;
    SVECTOR r;
    BtlUnit *u;
    BtlUnit9CC *cc;
    s32 t;
    BtlUnitA0 *a0;
    s32 i;
    Battle4880 *p;

    for (i = 0; i < g_gameState->unk1C->unk8E4.battle->unk132C.unk4->unk1; i++) {
        {
            p = arg0->unk488[i].unk0;
            u = &arg0->units[i + 4];
            u->unk7A = p->unk3;
            u->max->maxAtb = (0x3C - u->unk30) * 160;
            btl_resetAtbSpeed((Unk800BD8F0 *)u);
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
            }
            u->atb = ((*g_randPtr << 8) | *g_randPtr++) % u->max->maxAtb;
            u->unk74 = NULL;
            func_800C0A3C((Unk800C09D8 *)u);
            btl_hideMeshes(u, u->unkAC);
            if ((u->unk68 >> 23) & 1) {
                u->atb = 0;
                u->unk2E = 0;
                memset(&r, 0, 8);
                r.vx = u->unk86[0];
                r.vy = u->unk86[1];
                r.vz = u->unk86[2];
                r2 = r;
                func_80031244(&r2, &u->unk9C->unk24);
                u->unk9C->unk24.t[0] = u->unkA0->unk14 >> 16;
                u->unk9C->unk24.t[1] = u->unkA0->unk18 >> 16;
                u->unk9C->unk24.t[2] = u->unkA0->unk1C >> 16;
                cc = u->unk9C->unkC;
                if (IS_OFFSET(cc->unkC)) {
                    t = cc->unk10;
                    cc->unkC = (s32)cc + cc->unkC;
                    if (t != 0) {
                        cc->unk10 = (s32)u->unk9C->unkC + t;
                    }
                }
                arg1 = (u32 *)btl_calcBoneMatrices(u->unk9C, 0, (u8 *)arg1);
                u->unk98->unk0[0] = u->unk9C->unk20[u->unk7C].unk14 << 16;
                u->unk98->unk0[1] = 0;
                u->unk98->unk0[2] = u->unk9C->unk20[u->unk7C].unk1C << 16;
            } else {
                u->unk98->unk0[0] = u->unkA0->unk14;
                u->unk98->unk0[1] = u->unkA0->unk18;
                u->unk98->unk0[2] = u->unkA0->unk1C;
                a0 = u->unkA0;
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
                a0->unk3A = *g_randPtr++ % u->unk9C->unkC->unk2;
            }
        }
    }
    return arg1;
}

void btl_resetUnit(BtlUnit *arg0, Unk8E0 *arg1) {
    s32 i;
    u32 f;
    btl_initUnitCmds(arg0);
    btl_clearUnitStatus(arg0);
    arg0->unk7B = 0;
    arg0->unkB4 = NULL;
    arg0->unkCF = 0;
    arg0->unk7E = 0;
    arg0->unk80 = 0;
    arg0->unk82 = 0;
    arg0->unkCE = 0;
    arg0->unkC8 = 0;
    arg0->unkCA = 0;
    arg0->unkCC = 0;
    arg0->unk7D = 0;
    f = arg0->unk68;
    f &= ~0x20000;
    f &= ~0x80000;
    f &= ~0x100000;
    f &= ~0x200000;
    f &= ~0x400000;
    f |= 0x1000000;
    f &= ~0x2000000;
    f &= ~0x4000000;
    f &= ~0x8000000;
    f &= ~0x20000000;
    arg0->unk68 = f;
    arg1->unk10 = arg0;
    if ((arg0->unk68 >> 23) & 1) {
        arg0->unk9C = g_gameState->unk1C->unk8E0->unk8;
        arg0->unkA0 = g_gameState->unk1C->unk8E0->unkC;
    } else {
        arg0->unk9C = arg1->unk8;
        arg0->unkA0 = arg1->unkC;
        arg0->unk9C->unk50[2].unk4 |= 1;
        arg0->unk9C->unk50[2].unkC = 0;
        arg0->unk9C->unk50[2].unk12 = 0x1000;
        for (i = 0; i < arg0->unk9C->unk8->unk3; i++) {
            func_800C8D18(arg0->unk9C, i, 0, 0, 0);
        }
    }
    arg0->unkAC = 0;
}

void func_800BBF30(Unk8004AB64 *arg0, s16 arg1) {
    arg0->unk34 = arg1;
    arg0->unk3B = evt_createModel(arg0);
}

void btl_initWeapon(Character *arg0, BtlUnit *arg1) {
    s32 *a;
    s32 *b;
    s32 *c;
    s32 id;
    arg1->unk74 = (u8 *)&D_8006E0E8[arg0->equip[0]];
    id = ((Unk8006E0E8 *)arg1->unk74)->unk2 | 0x20000;
    arg1->unkA8->unk8 = (BtlUnit9C8 *)func_800AB6F0(id, (u8)arg1->unk68 >> 5);
    func_800AB758(id, (u8)arg1->unk68 >> 5, &a, &b, &c);
    func_800C7730(arg1->unkA8, (s16 *)a, (s16 *)b);
    func_800C2344(arg1->unkA8);
    arg1->unkA8->unkC = 0;
    switch (arg0->unk34.b.unk4) {
    case 0:
        arg0->unk21 = 0xD;
        break;
    case 1:
        arg0->unk21 = 0xD;
        break;
    case 3:
    case 5:
        arg0->unk21 = 0xF;
        break;
    case 4:
    case 6:
        arg0->unk21 = 0xF;
        break;
    case 9:
        arg0->unk21 = 0xE;
        break;
    case 10:
    case 11:
        arg0->unk21 = 0xF;
        break;
    case 12:
        arg0->unk21 = 6;
        break;
    case 14:
        arg0->unk21 = 0x19;
        break;
    case 15:
        arg0->unk21 = 6;
        break;
    case 16:
    case 17:
        arg0->unk21 = 0xE;
        break;
    case 2:
    case 7:
    case 8:
    case 13:
    case 18:
        arg0->unk21 = 0x10;
        break;
    }
    arg1->unkA8->unk1C = arg1->unk9C;
    arg1->unkA8->unk4 = arg0->unk21;
}
