#include "ovl_0b.h"

void btlseq_init(void) {
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
        m = db_lookupRegistered(4, (*q & 0xFFFF) | 0x30000, 0);
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

void btl_startEnemySeq(BtlCmd *arg0) {
    if (g_gameState->unk1C->unk8E4.battle->unk11C4.unk8[arg0->subNo] != 0 && btlseq_start(arg0) != NULL) {
        BtlUnit *u = arg0->unk4;
        if (!(u->unk68 & 1)) {
            u->unk86[1] = 0;
        }
    }
}

Battle122C *btlseq_start(BtlCmd *arg0) {
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

void btlseq_startDirect(u16 mask, s16 arg1, u16 id) {
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
        e = btlseq_start(c);
        if (e != NULL) {
            e->unk0 |= 8;
        }
    }
}

s32 btlseq_isBusy(void) {
    s16 i;
    Battle122C *p = g_gameState->unk1C->unk8E4.battle->unk11C4.unk68;
    for (i = 0; i < 4; i++, p++) {
        if (p->unk4 != 0) {
            return 1;
        }
    }
    return 0;
}

void btlseq_update(void) {
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
                if (s->unk8 != s->unkC && g_btlseqOps[op].unk0 != NULL) {
                    g_btlseqOps[op].unk0(s, u);
                }
                s->unkC = s->unk8;
            } while ((u16)g_btlseqOps[op].unk4(s, u));
            if (s->unk21 != 0) {
                u->unk86[1] = s->unk1C + s->unk1E * s->unk20 / s->unk21;
                if (s->unk20++ >= s->unk21) {
                    s->unk21 = 0;
                }
            }
            if (s->unk17 != 0) {
                if (--s->unk17 == 0) {
                    btl_playUnitSound(u, s->unk18, s->unk1B);
                }
            }
            if (s->unk25 != 0) {
                if (--s->unk25 == 0) {
                    btlfx_resetAll();
                    v[0] = s->unk24;
                    v[1] = 0;
                    v[2] = 0;
                    v[3] = 0;
                    btl_playCmdEffect((BtlCmd *)s->unk4, (u16)s->unk22, v);
                }
            }
            if (s->unk26 != 0) {
                s->unk27--;
                btl_setUnitFade(u, (s->unk27 << 5) / s->unk26);
                if (s->unk27 == 0) {
                    s->unk26 = 0;
                }
            }
        }
        s++;
    } while (--i != -1);
}

s32 btlseq_opEnd(BtlScript *arg0, BtlUnit *arg1) {
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
            btl_onCmdEffectDone();
        }
        arg0->unk4 = 0;
    }
    return 0;
}

s32 btlseq_opEndNoAnimWait(BtlScript *arg0) {
    if (g_btlfxEnv->unk0->unk0 == -1) {
        if (arg0->unk21 != 0) {
            return 0;
        }
        if (!((arg0->unk0 >> 3) & 1)) {
            btl_onCmdEffectDone();
        }
        arg0->unk4 = 0;
    }
    return 0;
}

void btlseq_opWait(BtlScript *arg0) {
    arg0->unk12 = arg0->unk8[1];
}

s32 btlseq_stepWait(BtlScript *arg0) {
    if (arg0->unk12 <= 0) {
        arg0->unk8 += 2;
        return 1;
    }
    return 0;
}

s32 btlseq_opExecCmd(BtlScript *arg0) {
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

void btlseq_opMoveToTarget(BtlScript *arg0, BtlUnit *arg1) {
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
    btlseq_getTargetCenter(arg0->unk4->unkC, (u16 *)&p[3], (u16 *)&p[5]);
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

s32 btlseq_stepMove(BtlScript *arg0, BtlUnit *arg1) {
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

void btlseq_opMoveHome(BtlScript *arg0, BtlUnit *arg1) {
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

void btlseq_opMoveTo(BtlScript *arg0, BtlUnit *arg1) {
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

s32 btlseq_stepMoveSetHome(s32 arg0, Unk800CB090 *arg1) {
    s16 i;
    btlseq_stepMove((BtlScript *)arg0, (BtlUnit *)arg1);
    for (i = 0; i < 3; i++) {
        arg1->unk98[i] = arg1->unk8C[i];
    }
    return 0;
}

void btlseq_opMoveBy(BtlScript *arg0, BtlUnit *arg1) {
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

BtlUnit *btlseq_findUnit(u16 arg0) {
    BtlUnit *u;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (u->unkB0 & arg0) {
            break;
        }
    }
    return u;
}

void btlseq_getTargetCenter(u16 arg0, u16 *arg1, u16 *arg2) {
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

s32 btlseq_opPlayAnim(BtlScript *arg0, BtlUnit *arg1) {
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

s32 btlseq_opDelayedEffect(BtlScript *arg0) {
    arg0->unk22 = arg0->unk8[1] | (arg0->unk8[2] << 8);
    arg0->unk24 = arg0->unk8[3];
    arg0->unk25 = arg0->unk8[4] + 1;
    arg0->unk8 += 5;
    return 1;
}

s32 btlseq_opEffect(BtlScript *arg0) {
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
    btl_playCmdEffect((BtlCmd *)arg0->unk4, id, v);
    arg0->unk0 |= 1;
    arg0->unk8 += 9;
    return 1;
}

s32 btlseq_opWaitAnim(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C *m = arg1->unk9C;
    BtlUnitA0 *a = arg1->unkA0;
    if (arg0->unk14 != 0 && (a->unk3A >= m->unkC->unk2 || (arg1->status & 0x02001002))) {
        g_btlfxEnv->unk2C->unk52 = 0;
        arg0->unk8 += 1;
        return 1;
    }
    return 0;
}

s32 btlseq_opWaitEffectHit(BtlScript *arg0) {
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

s32 btlseq_opWaitEffect(BtlScript *arg0) {
    Unk800F78A40 *p = g_btlfxEnv->unk0;
    if ((p->unk0 == -1) | ((p->unk1C >> 15) & 1)) {
        arg0->unk8 += 1;
        return 1;
    }
    return 0;
}

s32 btlseq_opHideMeshes(BtlScript *arg0, BtlUnit *arg1) {
    arg1->unkAC |= arg0->unk8[1] | (arg0->unk8[2] << 8);
    btl_hideMeshes(arg1, arg0->unk8[1] | (arg0->unk8[2] << 8));
    arg0->unk8 += 3;
    return 1;
}

s32 btlseq_opShowMeshes(BtlScript *arg0, BtlUnit *arg1) {
    arg1->unkAC &= ~(arg0->unk8[1] | (arg0->unk8[2] << 8));
    btl_showMeshes(arg1, arg0->unk8[1] | (arg0->unk8[2] << 8));
    arg0->unk8 += 3;
    return 1;
}

s32 btlseq_opShowName(BtlScript *arg0, BtlUnit *arg1) {
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

s32 btlseq_opSetCamera(BtlScript *arg0) {
    g_gameState->unk1C->unk8E4.battle->unk131C = arg0->unk8[1];
    arg0->unk8 += 2;
    return 1;
}

s32 btlseq_opPlayCamera(BtlScript *arg0, BtlUnit *arg1) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    Battle11C4 *s = &b->unk11C4;
    BtlUnit *u;
    u32 f;
    s32 r;
    s8 one;
    if (arg0->unk8[0] != 0x20 && !((arg0->unk4->unk8->unk0_17 >> 13) & 1)) {
        if (((g_gameState->config >> 4) & (one = 1)) == one) {
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
    btlseq_getTargetCenter(arg0->unk4->unkC, &s->unk150, &s->unk154);
    s->unk152 = 0;
    s->unk148 = arg1;
    s->unk14C = (s32)btlseq_findUnit(arg0->unk4->unkC);
    g_gameState->unk1C->unk8E4.battle->unk131C = arg0->unk8[1];
    btlcam_playSeq(g_gameState->unk1C->unk8E4.battle->unk488[(arg1->unk68 >> 1) & 0xF].unk0->unk2C);
end:
    arg0->unk8 += 2;
    return 1;
}

s32 btlseq_opTurn(BtlScript *arg0, BtlUnit *arg1) {
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
            btlseq_getTargetCenter(arg0->unk4->unkC, (u16 *)&d[0], (u16 *)&d[1]);
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

void btlseq_opScale(BtlScript *arg0, BtlUnit *arg1) {
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

s32 btlseq_stepScale(BtlScript *arg0, BtlUnit *arg1) {
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

s32 btlseq_opPlayTexAnim(BtlScript *arg0, BtlUnit *arg1) {
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

s32 btlseq_opLoopTexAnim(BtlScript *arg0, BtlUnit *arg1) {
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

s32 btlseq_opStopTexAnim(BtlScript *arg0, BtlUnit *arg1) {
    u8 i = arg0->unk8[1];
    BtlUnit9C *m = arg1->unk9C;
    BtlUnit9C50 *e;
    (*(e = m->unk50 + i)).unk4 &= 0xFC;
    arg0->unk8 += 2;
    return 1;
}

s32 btlseq_opDelayedSound(BtlScript *arg0) {
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

s32 btlseq_opFadeOut(BtlScript *arg0, BtlUnit *arg1) {
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
