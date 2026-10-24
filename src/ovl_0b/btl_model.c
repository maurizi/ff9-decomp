#include "ovl_0b.h"

void btl_addMeshRgb(BtlUnit9C *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    BtlMesh *g = &arg0->header->meshes[arg1];
    u32 *p = g->prims;
    u32 i;
    for (i = 0; i < g->nFT4; i++) {
        p[4] = btl_addRgbClamped(p[4], arg2, arg3, arg4);
        p += 6;
    }
    for (i = 0; i < g->nFT3; i++) {
        p[2] = btl_addRgbClamped(p[2], arg2, arg3, arg4);
        p += 5;
    }
}

void btl_setMeshRgb(BtlUnit9C *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    BtlMesh *g = &arg0->header->meshes[arg1];
    u32 *p = g->prims;
    u32 v = (arg4 << 16) | (arg3 << 8) | arg2;
    u32 i;
    for (i = 0; i < g->nFT4; i++) {
        p[4] = (p[4] & 0xFF000000) | v;
        p += 6;
    }
    for (i = 0; i < g->nFT3; i++) {
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
    if (!(arg0->flags & 0x10)) {
        arg0->abr = arg1;
        p = (u8 *)arg0->prims;
        for (i = 0; i < arg0->header->meshCount; i++) {
            if (!(arg0->hiddenMeshes & (1 << i))) {
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

void btl_updateTexAnims(TexAnimSet *arg0) {
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

void btl_relocTexAnims(TexAnimSet *set, u16 (*ofs)[4]) {
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

Battle11C020 *btl_initScene(void) {
    Battle *b;
    Battle11C020 *s = g_gameState->unk1C->unk8E4.battle->unk11C0->unk20;
    u16 *t = g_gameState->unk1C->unk8E4.battle->unk11C0->unk24;
    s32 r;
    Battle132C *sc = &g_gameState->unk1C->unk8E4.battle->scene;
    u8 n;
    u8 m;

    sc->unkC = (u16 *)((u8 *)t + t[0]);
    sc->unk10 = (s16 *)((u8 *)t + t[1]);
    sc->unk4 = btl_pickEnemyGroup(g_gameState->unk1C->unk8E4.battle->unk11C0->unk20);
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

Battle1330 *btl_pickEnemyGroup(Battle11C020 *arg0) {
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
    b->scene.unk0 = (b->unk11C0->unk20->unk4 ^ 0x30) * 4;
    D_800FDF38 = g_gameState->unk1C->unk8E4.battle->scene.unk4->unk4;
}

void btl_setupEnemies(void) {
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
    flags = &b->scene.unk0;
    grp = b->scene.unk4;
    hdr = b->unk11C0->unk20;
    b->scene.unk0 = (b->scene.unk0 & ~3) | (btl_rollAmbush(flags) & 3);
    i = 0;
    n = hdr->unk2 + hdr->unk3;
    grp2 = hdr->unk8;
    recs = (Unk800C9D1C *)(grp2 + hdr->unk1);
    rec = recs;
    en = g_gameState->unk1C->unk8E4.battle->enemyTypes;
    ofs = (U16Pair *)(g_gameState->unk1C->unk8E4.battle->unk11C0->unk14 + 4);
    src = (BtlCmd8 *)&recs[hdr->unk2];
    for (; i < hdr->unk2; i++) {
        btl_initEnemyType(rec, en);
        en->unk2C = btl_getSceneData(i + 5);
        en->unk0 = ofs->unk0 + 4;
        ofs++;
        en->unk28 = n;
        n += rec->unk4F;
        en++;
        rec++;
    }
    dst = g_gameState->unk1C->unk8E4.battle->enemyAbilities;
    for (i = 0, cmd = src; i < hdr->unk3; i++, cmd++) {
        btl_copyAbility(cmd, dst);
        dst->nameOfs = ofs->unk0 + 4;
        ofs++;
        dst++;
    }
    u = g_gameState->unk1C->unk8E4.battle->unitList;
    for (i = 0; i < grp->unk1; i++) {
        if (u == NULL) {
            break;
        }
        btl_initEnemyUnit(&grp->unk8[i], u, (s32 *)flags, i);
        ((void (*)(Unk800C9D1C *, BtlUnit *, s32))btl_setEnemyStats)(&recs[grp->unk8[i].unk0], u, i);
        u = u->next;
    }
}

void btl_placeParty(void) {
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
    flags = &b->scene.unk0;
    for (i = 0; i < 4; i++) {
        if (g_gameState->unk1C->party[i] != NULL) {
            n++;
        }
    }
    x = (n - 1) * 0x13C;
    i = 0;
    rot = ((*flags & 3) != 0) << 11;
    u = g_gameState->unk1C->unk8E4.battle->unitList;
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
        u->pos[0] = u->unk98->unk0[0] = u->unkA0->unk14 = x << 16;
        p98 = u->unk98;
        pa0 = u->unkA0;
        if ((u8)btl_hasStatus(u, BTL_STATUS_FLOAT)) {
            v = 0xFF380000;
        } else {
            v = 0;
        }
        u->pos[1] = p98->unk0[1] = pa0->unk18 = v;
        q98 = u->unk98;
        qa0 = u->unkA0;
        w = 0xF9E80000;
        if (!((u->unk68 >> 8) & 3)) {
            w = 0xF8580000;
        }
        u->pos[2] = q98->unk0[2] = qa0->unk1C = w;
        u->unkA0->unk20 = 0;
        u->rot[0] = 0;
        u->unkA0->unk22 = rot;
        u->rot[1] = rot;
        u->unkA0->unk24 = 0;
        u->rot[2] = 0;
        tbl = D_800F57F0;
        e = &tbl[g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4];
        u->unkBD = e->unk0;
        u->unkBE = e->unk1;
        btl_setShadowSize(u, e->unk2, e->unk3);
        x -= 0x278;
        g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[(u->unk68 >> 1) & 0xF].unk4_22 = e->unk4;
        u = u->next;
    }
}

void btl_initEnemyType(Unk800C9D1C *arg0, Battle4880 *arg1) {
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
        h = db_lookupRegistered(4, arg0->unk20[i] | 0x30000, 0);
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

void btl_initEnemyUnit(Unk800C9EE0 *arg0, BtlUnit *arg1, s32 *arg2, s32 arg3) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    Battle488 *s = &b->enemies[(arg1->unk68 >> 1) & 0xF];
    Battle4880 *e;
    s16 r;
    s->unk0 = &b->enemyTypes[arg0->unk0];
    arg1->unk68 = (((arg1->unk68 & ~0x10000000) | ((arg0->unk1 & 1) << 28)) & ~0x300) | 0x200;
    e = &g_gameState->unk1C->unk8E4.battle->enemyTypes[arg0->unk0];
    arg1->max = &e->unk4;
    arg1->hp = e->unk4.maxHp;
    arg1->mp = arg1->max->maxMp;
    s->unk4 = (s->unk4 & ~0x100) | ((arg0->unk1 << 7) & 0x100);
    if (!((s->unk4 >> 9) & 1)) {
        arg1->pos[0] = arg1->unk98->unk0[0] = arg1->unkA0->unk14 = arg0->unk4 << 16;
        arg1->pos[1] = arg1->unk98->unk0[1] = arg1->unkA0->unk18 = arg0->unk6 << 16;
        arg1->pos[2] = arg1->unk98->unk0[2] = arg1->unkA0->unk1C = arg0->unk8 << 16;
        arg1->unkA0->unk20 = 0;
        arg1->rot[0] = 0;
        arg1->unkA0->unk24 = 0;
        arg1->rot[2] = 0;
        r = (arg0->unkA + D_800F5850[*arg2 & 3]) & 0xFFF;
        arg1->unkA0->unk22 = r;
        arg1->rot[1] = r;
    }
    arg1->motions = (s32 *)s->unk0->unkC;
    arg1->model->motion = s->unk0->unkC[0];
}

void btl_setEnemyStats(Unk800C9D1C *arg0, BtlUnit *arg1) {
    Battle488 *s;
    BtlShadow *t;

    arg1->statusImmune = arg0->unk0;
    arg1->autoStatus = arg0->unk4;
    arg1->status = arg0->unk8;
    arg1->hp = arg0->unkC;
    arg1->mp = arg0->unkE;
    arg1->defense = arg0->unk43[0];
    arg1->evade = arg0->unk43[1];
    arg1->magicDef = arg0->unk43[2];
    arg1->magicEva = arg0->unk43[3];
    arg1->speed = arg0->unk34[0];
    arg1->strength = arg0->unk34[1];
    arg1->magic = arg0->unk34[2];
    arg1->spirit = arg0->unk34[3];
    *(s32 *)&arg1->elemGuard = arg0->unk3C;
    *(s32 *)&arg1->hiddenMeshes = arg0->unk2C;
    arg1->unk7C = arg0->unk4B;
    s = &g_gameState->unk1C->unk8E4.battle->enemies[(arg1->unk68 >> 1) & 0xF];
    s->unk8.w = arg0->unk18;
    s->unk4 = (s->unk4 & ~0x40) | ((arg0->unk30 & 1) << 6);
    s->unk4 = (s->unk4 & ~0x80) | ((arg0->unk30 << 6) & 0x80);
    btl_setShadowSize(arg1, arg0->unk64, arg0->unk66);
    arg1->unkBD = arg0->unk68;
    arg1->unkBE = arg0->unk6E;
    t = &g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[((arg1->unk68 >> 1) & 0xF) + 9];
    t->unk4_12 = arg0->unk6A;
    t->unk4_22 = arg0->unk6C;
}

void btl_copyAbility(BtlCmd8 *arg0, BtlCmd8 *arg1) {
    *arg1 = *arg0;
}

s32 btl_getSceneData(s32 arg0) {
    s16 *t = g_gameState->unk1C->unk8E4.battle->scene.unk10;
    return (s32)t + t[arg0];
}

s16 btl_getEnemyModelId(s16 arg0) {
    Unk800C9EE0 *e = &g_gameState->unk1C->unk8E4.battle->scene.unk4->unk8[arg0];
    s32 r;
    if (arg0 > 0 && (e->unk1 & 2)) {
        r = -1;
    } else {
        r = g_gameState->unk1C->unk8E4.battle->scene.unk8[e->unk0].unk1E;
    }
    return r;
}

u16 btl_getEntranceSound(void) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    return b->scene.unk8[b->scene.unk4->unk8[0].unk0].unk62;
}
