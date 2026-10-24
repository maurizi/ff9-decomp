#include "ovl_0b.h"

void btl_initEquipElements(Character *c, BtlUnit *u) {
    s32 i;

    u->elemBoost = 0;
    u->elemWeak = 0;
    u->elemHalf = 0;
    u->elemAbsorb = 0;
    u->elemGuard = 0;
    for (i = 0; i < 5; i++) {
        if (c->equip[i] != 0xFF) {
            func_800BDACC(&u->elemGuard, D_8006D548[D_80069018[c->equip[i]].unkB].unk4);
            u->elemBoost |= D_8006D548[D_80069018[c->equip[i]].unkB].unk8;
        }
    }
}

void func_800BC1E0(Unk800BC1E0 *arg0, s16 *arg1) {
    if (!(arg0->unk0 & 8)) {
        arg1[0] = 0;
        arg1[1] = arg0->unk8;
    } else {
        arg1[0] = arg0->unk8;
        arg1[1] = 0;
    }
    arg1[2] = arg0->unkA;
}

void btl_updateUnitModel(BtlUnit *u, s32 *ot) {
    BtlUnit9C *m = u->model;
    BtlUnitA0 *a = u->unkA0;
    BtlUnit *v;
    SVECTOR tmp;
    SVECTOR rot;
    s32 i;
    s32 n;
    s32 t;
    BtlUnit9CC *p;
    s32 id;
    BtlUnit9C *w;

    if ((u->unk68 >> 23) & 1) {
        v = btl_findBodyUnit();
        u->rot[0] = v->rot[0];
        u->rot[1] = v->rot[1];
        u->rot[2] = v->rot[2];
        u->unk98->unk0[0] = u->model->unk20[u->unk7C].unk14 << 16;
        u->unk98->unk0[1] = 0;
        u->unk98->unk0[2] = u->model->unk20[u->unk7C].unk1C << 16;
        u->pos[0] = u->unk98->unk0[0];
        u->pos[1] = u->unk98->unk0[1];
        u->pos[2] = u->unk98->unk0[2];
        return;
    }
    if ((u->unk68 & 1) && (s32)m->unkC == u->motions[17]) {
        memset(&rot, 0, 8);
        rot.vx = u->rot[0];
        rot.vy = 0x800;
        rot.vz = u->rot[2];
        tmp = rot;
        func_80031244(&tmp, &m->unk24);
    } else {
        memset(&rot, 0, 8);
        rot.vx = u->rot[0];
        rot.vy = u->rot[1];
        rot.vz = u->rot[2];
        tmp = rot;
        func_80031244(&tmp, &m->unk24);
    }
    m->unk24.t[0] = u->pos[0] >> 16;
    m->unk24.t[1] = u->pos[1] >> 16;
    m->unk24.t[2] = u->pos[2] >> 16;
    m->unk10 = 0;
    if (a->unk3A >= m->unkC->unk2) {
        if (!(u8)btl_setIdleMotion(u)) {
            u->unkA0->unk3A = 0;
        } else if (u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) {
            u->unkA0->unk3A--;
        }
    }
    btl_applyModelScale(m);
    p = m->unkC;
    if (IS_OFFSET(p->unkC)) {
        t = p->unk10;
        p->unkC = (s32)p + p->unkC;
        if (t != 0) {
            p->unk10 = (s32)m->unkC + t;
        }
    }
    *ot = (s32)btl_calcBoneMatrices(m, a->unk3A, (u8 *)*ot);
    if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) && !((u->unk68 >> 21) & 1)) {
        a->unk3A++;
    }
    n = m->unk8->meshCount;
    btl_updateModelVisibility(m);
    u->model->unk0 &= ~0x20;
    for (i = 0; i < n; i++) {
        if (!(m->unk14 & (1 << i))) {
            btl_projectMesh(m, i);
            *ot = (s32)btl_drawMesh(m, i, (u8 *)*ot);
            u->model->unk0 |= 0x20;
        }
    }
    if (!(u->status & BTL_STATUS_JUMP)) {
        btl_updateTexAnims((TexAnimSet *)m->unk50);
    }
    if (u->weaponModel != NULL) {
        u->weaponModel->unk10 = 0;
        *ot = (s32)btl_calcBoneMatrices(u->weaponModel, 0, (u8 *)*ot);
        w = u->weaponModel;
        n = w->unk8->meshCount;
        btl_updateModelVisibility(w);
        u->weaponModel->unk0 &= ~0x20;
        for (i = 0; i < n; i++) {
            if (!(u->weaponModel->unk14 & (1 << i))) {
                btl_projectMesh(u->weaponModel, i);
                *ot = (s32)btl_drawMesh(u->weaponModel, i, (u8 *)*ot);
                u->weaponModel->unk0 |= 0x20;
            }
        }
    }
    if ((u->unk68 >> 24) & 1) {
        id = (u->unkBE << 8) | u->unkBD;
        if (u->unk68 & 1) {
            if (!(g_gameState->unk1C->unk8E4.battle->cmdFlags & 1)) {
                if (u->unk7B != g_gameState->unk1C->unk8E4.battle->escapeHeld) {
                    btl_setIdleMotion(u);
                }
                u->unk7B = g_gameState->unk1C->unk8E4.battle->escapeHeld;
            }
            *ot = (s32)btl_drawShadow(g_gameState->unk1C->unk8E4.battle->unk11C0->unk28, (u->unk68 >> 1) & 0xF, 0, id, (u8 *)*ot);
        } else if (u->unk7D < 4) {
            *ot = (s32)btl_drawShadow(g_gameState->unk1C->unk8E4.battle->unk11C0->unk28, ((u->unk68 >> 1) & 0xF) + 9, 0, id, (u8 *)*ot);
        }
    }
}

s32 btl_setIdleMotion(BtlUnit *u) {
    GameData *g = g_gameState->unk1C;
    Battle *b = g->unk8E4.battle;
    BtlCmd *cmd = b->curCmd;
    s32 cur;
    s32 *a;
    BtlUnit9C *m;

    if (u->status & BTL_STATUS_KO) {
        if ((u->unk68 & 0x20001) == 1 && u != cmd->caster && u->unk7D == 0) {
            cur = (s32)u->model->unkC;
            if (cur != u->motions[4] && cur != u->motions[9]) {
                u->model->unkC = (BtlUnit9CC *)u->motions[(u->unk68 >> 18) & 1];
            }
        }
        return 0;
    }
    if ((u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) || ((u->unk68 >> 17) & 1)
        || ((u->unk68 & 1) && g->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4 == 2 && cmd->cmdNo == BTL_CMD_SWD_MAG)) {
        return 0;
    }
    {
        if (u == cmd->caster && (u32)(cmd->cmdNo - 0x37) >= 7) {
            cur = (s32)u->model->unkC;
            if (cur == u->motions[10] || cur == u->motions[11]) {
                u->model->unkC = (BtlUnit9CC *)u->motions[9];
            }
            return 0;
        }
        if (u->unk68 & 1) {
            if (!(u->unk68 & 0x06000000)) {
                if ((b->escapeHeld != 0 || (b->cmdFlags & 1)) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP))) {
                    u->model->unkC = (BtlUnit9CC *)u->motions[17];
                } else if (u->status & BTL_STATUS_DEFEND) {
                    u->model->unkC = (BtlUnit9CC *)u->motions[13];
                } else if ((u->unk68 >> 19) & 1) {
                    if ((s32)u->model->unkC == u->motions[(u->unk68 >> 18) & 1]) {
                        u->model->unkC = (BtlUnit9CC *)u->motions[((u->unk68 >> 18) & 1) + 10];
                    } else {
                        u->model->unkC = (BtlUnit9CC *)u->motions[9];
                    }
                } else {
                    m = u->model;
                    a = u->motions;
                    cur = (s32)m->unkC;
                    if (cur == a[0] && (u->status & (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP))) {
                        m->unkC = (BtlUnit9CC *)a[7];
                    } else if ((cur == a[1] || cur == a[6]) && !(u->status & (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP))) {
                        m->unkC = (BtlUnit9CC *)a[5];
                    } else {
                        u->model->unkC = (BtlUnit9CC *)u->motions[(u->unk68 >> 18) & 1];
                    }
                }
            }
        } else {
            u->model->unkC = (BtlUnit9CC *)u->motions[(u->unk68 >> 18) & 1];
        }
        u->unkA0->unk3A = 0;
        return 1;
    }
}

void btl_startHitMotion(BtlUnit *arg0) {
    u32 f;
    if (arg0->popupFlags & 4) {
        arg0->pos[2] += (arg0->unkA0->unk22 == 0) ? 0xC80000 : 0xFF380000;
    }
    f = arg0->unk68;
    if (f & 1) {
        if ((g_gameState->unk1C->unk8E4.battle->curCmd->ability->unkB & 0x81) == 0x81) {
            arg0->model->unkC = (BtlUnit9CC *)arg0->motions[3];
        } else {
            arg0->model->unkC = (BtlUnit9CC *)arg0->motions[2];
        }
    } else if (arg0->hp == 0 && (u8)(g_gameState->unk1C->unk8E4.battle->enemies + ((f >> 1) & 0xF))->unk4 >> 7) {
        arg0->model->unkC = (BtlUnit9CC *)arg0->motions[3];
        btl_playDeathCry(arg0, g_gameState->unk1C->unk8E4.battle->enemies[(arg0->unk68 >> 1) & 0xF].unk0->unk30);
    } else {
        if ((arg0->unk68 >> 23) & 1) {
            arg0 = btl_findBodyUnit();
        }
        arg0->model->unkC = (BtlUnit9CC *)arg0->motions[((arg0->unk68 >> 18) & 1) + 2];
    }
    arg0->unk68 |= 0x20000;
    arg0->unkA0->unk3A = 0;
}

void btl_endPartyHitMotion(BtlUnit *arg0) {
    BtlUnit9C *m = arg0->model;
    s32 *a = arg0->motions;
    s32 c = (s32)m->unkC;
    if (c == a[2]) {
        if (arg0->status & BTL_STATUS_KO) {
            arg0->unk7D = 1;
            arg0->unk68 &= ~0x20000;
        } else if ((arg0->unk68 >> 19) & 1) {
            m->unkC = (BtlUnit9CC *)a[10];
        } else {
            arg0->unk68 &= ~0x20000;
        }
    } else if (c == a[3]) {
        if (arg0->status & BTL_STATUS_KO) {
            arg0->unk7D = 5;
            arg0->unk68 &= ~0x20000;
            arg0->model->unkC = (BtlUnit9CC *)arg0->motions[4];
        } else {
            m->unkC = (BtlUnit9CC *)a[6];
        }
    } else if (c == a[6]) {
        if ((arg0->unk68 >> 19) & 1) {
            m->unkC = (BtlUnit9CC *)a[11];
        } else if (arg0->status & (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) {
            arg0->unk68 &= ~0x20000;
        } else {
            m->unkC = (BtlUnit9CC *)a[5];
        }
    } else {
        arg0->unk68 &= ~0x20000;
    }
    arg0->unkA0->unk3A = 0;
}

void btl_endEnemyHitMotion(BtlUnit *arg0) {
    BtlUnit9C *m;
    s32 *a;
    arg0->unk68 &= ~0x20000;
    if (arg0->status & BTL_STATUS_KO) {
        m = arg0->model;
        a = arg0->motions;
        if ((s32)m->unkC == a[3] && ((u8)(g_gameState->unk1C->unk8E4.battle->enemies + ((arg0->unk68 >> 1) & 0xF))->unk4 >> 7)) {
            m->unkC = (BtlUnit9CC *)a[4];
            arg0->unkA0->unk3A--;
            arg0->unk7D = 3;
            arg0->unk68 |= 0x200000;
            return;
        }
        if (!(((g_gameState->unk1C->unk8E4.battle->enemies + ((arg0->unk68 >> 1) & 0xF))->unk4 >> 6) & 1) && !((arg0->unk68 >> 20) & 1)) {
            arg0->unk7D = 1;
            return;
        }
        arg0->model->unkC = (BtlUnit9CC *)arg0->motions[(arg0->unk68 >> 18) & 1];
    }
}

void btl_updateKo(BtlUnit *u) {
    Battle488 *e;
    BtlUnit9C *m;
    u32 f;

    if (u->unk7D == 0) {
        return;
    }
    if (!(u->unk68 & 1)) {
        e = &g_gameState->unk1C->unk8E4.battle->enemies[(u->unk68 >> 1) & 0xF];
        switch (u->unk7D) {
        case 1:
            if (!((u->unk68 >> 23) & 1)) {
                u->model->unkC = (BtlUnit9CC *)u->motions[4 + ((u->unk68 >> 18) & 1)];
                u->unkA0->unk3A = 0;
                u->unk7D++;
            } else {
                u->unk7D = 5;
            }
            btl_playDeathCry(u, e->unk0->unk30);
            return;
        case 2:
            if (u->unkA0->unk3A >= u->model->unkC->unk2 - 1) {
                u->unk68 |= 0x200000;
                u->unk7D++;
            }
            return;
        case 3:
            btl_playUnitSound(u, 0x79, 0x7F);
            btl_playUnitSound(u, 0x78, 0x7F);
            u->unk7D++;
        case 4:
            btl_setUnitFade(u, e->unk4 & 0x3F);
            if ((e->unk4 & 0x3F) == 0) {
                u->unk7D = 5;
                return;
            }
            e->unk4 = (e->unk4 & ~0x3F) | (((e->unk4 & 0x3F) - 2) & 0x3F);
            return;
        case 5:
            if (btl_hasEnemyFlag(u, 8) != 0) {
                if (g_gameState->unk1C->unk56C < 9999) {
                    g_gameState->unk1C->unk56C++;
                }
            }
            if (g_gameState->unk1C->unk56E != 4) {
                btl_addEnemyRewards(e->unk0);
            }
            u->unk7D++;
            if (g_gameState->unk1C->unk8E4.battle->phase != BTL_PHASE_END ||
                (u32)(g_gameState->unk1C->unk8E4.battle->subPhase - 2) >= 2) {
                btl_checkSideDefeated(u);
            }
            btl_unlinkUnit(u);
            return;
        }
    } else {
        /* padded: the first jump table is 8-aligned in its object */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        switch (u->unk7D) {
        case 1:
            m = u->model;
            if ((s32)m->unkC == u->motions[9]) {
                m->unkC = (BtlUnit9CC *)u->motions[32];
                u->unkA0->unk3A = 0;
                return;
            }
            if ((s32)m->unkC == u->motions[32] && u->unkA0->unk3A < m->unkC->unk2) {
                break;
            }
            u->unk7D = 2;
        case 2:
            if ((f = (u->unk68 >> 18) & 1) == 1) {
                u->model->unkC = (BtlUnit9CC *)u->motions[8];
                u->unk7D = 4;
            } else {
                u->model->unkC = (BtlUnit9CC *)u->motions[7];
                u->unk7D = 3;
            }
            u->unkA0->unk3A = 0;
            return;
        case 3:
            if (u->unkA0->unk3A >= u->model->unkC->unk2) {
                u->model->unkC = (BtlUnit9CC *)u->motions[8];
                u->unkA0->unk3A = 0;
                u->unk7D++;
            }
            break;
        case 4:
            if (u->unkA0->unk3A < u->model->unkC->unk2) {
                break;
            }
        case 5:
            if ((u8)btl_setKoMotion(u)) {
                btl_checkSideDefeated(u);
            }
            break;
        }
    }
}

s32 btl_waitHitMotions(BtlCmd *arg0) {
    BtlUnit *u;
    s32 r;
    u32 bit = arg0->unk10.b.covered;
    do {
        r = 1;
        if (bit) {
            return r;
        }
    } while (0);
    for (u = g_gameState->unk1C->unk8E4.battle->unitList; u != 0; u = u->next) {
        if ((u->unk68 >> 17) & 1) {
            r = 0;
            if (u->unkA0->unk3A >= u->model->unkC->unk2) {
                u->pos[2] = u->unk98->unk0[2];
                if (u->unk68 & 1) {
                    btl_endPartyHitMotion(u);
                } else {
                    btl_endEnemyHitMotion(u);
                }
                btl_setIdleMotion(u);
            }
        }
    }
    return r;
}
