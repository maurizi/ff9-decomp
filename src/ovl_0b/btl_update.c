#include "ovl_0b.h"

s32 btl_update(void) {
    GameData *g = g_gameState->unk1C;
    Battle *b = g->unk8E4.battle;
    BtlUnit *u;
    u16 id;
    s32 n;

    btlcam_update();
    switch (b->unkD80) {
    case 2:
        btl_updateIntro(g, b);
        break;
    case 1:
        func_800AF578(g, b);
        break;
    case 3:
        if ((u8)func_800AF578(g, b)) {
            b->unkD80 = 4;
            b->unkD86 = 0;
            b->unkD7C &= 0xF;
            for (u = b->unk6F8; u != NULL; u = u->unk0) {
                if (!(u->unk68 & 1) && (id = func_800CA378()) != 0xFFFF) {
                    btl_playUnitSound(u, id, 0x7F);
                    break;
                }
            }
            if (D_8007AFB1 == 0) {
                func_800E9F3C(1);
                func_800E9F68(1);
            }
            n = b->unk132C.unk0 & 3;
            if (n == 0) {
                btl_showMessage(2);
            } else if (n == 1) {
                btl_showMessage(1);
            }
        }
        break;
    case 4:
        btl_updateCombat(g, b);
        break;
    case 5:
        func_800E9F68(0);
        btl_victory(g, b);
        if (b->unkD81 != 3) {
            b->escapeHeld = 0;
        }
        break;
    case 6:
        func_800AF578(g, b);
        if (b->unkD81 == 1) {
            func_8002FAAC(0x20);
            b->unkD81++;
        } else if (b->unkD81 == 2) {
            if (b->unkD82++ > 0x20) {
                return (u8)btl_saveParty(b);
            }
        }
        break;
    case 7:
    case 8:
        if ((u8)func_800AF578(g, b)) {
            g->unk56F |= 0x40;
            if (b->unkD81 == 0) {
                if (g->unk56E != 5) {
                    func_8002FAAC(0x20);
                }
                b->unkD81++;
            } else if (b->unkD81 == 1) {
                if (b->unkD82++ > 0x20) {
                    return (u8)btl_saveParty(b);
                }
            }
        }
        break;
    }
    func_8004BC38();
    b->unkD7C++;
    return 0;
}

s32 func_800AF578(GameData *arg0, Battle *arg1) {
    BtlUnit *u = arg1->unk6F8;
    s32 r = 1;
    s32 sp = arg1->unk11C0->unk34[arg0->unk8];
    BtlUnit9C *m;
    BtlUnit9C *n;
    u32 f;
    u32 g;
    for (; u != NULL; u = u->unk0) {
        if (!((u->unk68 >> 22) & 1)) {
            btl_updateUnitModel(u, &sp);
            if (arg1->unkD80 == 3) {
                n = u->unk9C;
                g = u->unk68 & ~0x200000;
                u->unk68 = g;
                if (u->unkA0->unk3A >= n->unkC->unk2) {
                    if (!(u->status & BTL_STATUS_KO)) {
                        n->unkC = (BtlUnit9CC *)u->unkA4[(g >> 18) & 1];
                    }
                    u->unkA0->unk3A = 0;
                }
                if (!(u->status & BTL_STATUS_PETRIFY)) {
                    if ((s32)u->unk9C->unkC != u->unkA4[(u->unk68 >> 18) & 1] && (s32)u->unk9C->unkC != u->unkA4[4]) {
                        r = 0;
                    }
                }
            } else if (arg1->unkD80 == 6) {
                f = u->unk68;
                if ((f & 1) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP)) && ((arg1->unk132C.unk0 >> 6) & 1) && (g_gameState->unk1C->characters + ((f >> 1) & 0xF))->unk34.b.unkA) {
                    m = u->unk9C;
                    if (u->unkA0->unk3A >= m->unkC->unk2) {
                        m->unkC = (BtlUnit9CC *)u->unkA4[19];
                        u->unkA0->unk3A = 0;
                    }
                }
            }
            btl_updateUnitTint(u);
        }
        btl_updateKo(u);
    }
    if (arg1->unkD80 == 7 && !((arg1->unk132C.unk0 >> 4) & 1) && (u8)btl_playVictoryMusic(arg0, 0x1E, 6) == 0) {
        r = 0;
    }
    btl_drawScene(arg0, arg1, sp);
    return r;
}

void btl_victory(GameData *arg0, Battle *arg1) {
    s32 sp18;
    s32 done;
    BtlUnit *u;
    u32 exp;

    sp18 = arg1->unk11C0->unk34[arg0->unk8];
    done = 1;
    if (g_btlfxEnv->unk0->unk0 != -1 || arg1->cmdQueue.unk0 != NULL || arg1->curCmd != NULL) {
        done = 0;
    }
    for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
        if (!((u->unk68 >> 22) & 1)) {
            btl_updateUnitModel(u, &sp18);
            switch (arg1->unkD81) {
            case 0:
                btl_updateHpStatus(u);
                if ((!(u->unk68 & 1) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP))) || ((u->unk68 & 1) && (u->status & BTL_STATUS_KO) && u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[4])) {
                    done = 0;
                }
                break;
            case 1:
                if ((u->unk68 & 1) && u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[4] && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP))) {
                    done = 0;
                }
                break;
            case 2:
                btl_updateHpStatus(u);
                if (u->unk68 & 1) {
                    btl_removeStatuses(u, (BTL_STATUS_CONFUSE | BTL_STATUS_STOP | BTL_STATUS_DEFEND | BTL_STATUS_FREEZE));
                }
                if (arg1->unk11BA != 0) {
                    done = 0;
                }
                if (u->status & BTL_STATUS_KO) {
                    if (u->unk7D == 0 && btl_isCmdPending(u->unkC) == 0) {
                        btl_setCmd(u->unkC, 0x3C, 0, u->unkB0, 0);
                    }
                    if (u->unk7D != 6) {
                        done = 0;
                    }
                } else if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE))) {
                    if (u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[0] &&
                        u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[1]) {
                        done = 0;
                    }
                }
                break;
            case 3:
                if (u->status & BTL_STATUS_KO) {
                    if (u->unk7D == 0) {
                        u->unk7D = 1;
                    }
                    if (u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[4]) {
                        done = 0;
                    } else if (btl_isCmdQueued(u, 0x3D) != 0) {
                        done = 0;
                    }
                } else if ((u->unk68 & 1) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP))) {
                    u->unk8C[2] += 0xFF9C0000;
                    btl_setUnitBrightness(u, (s8)arg1->unkD84);
                    if ((s8)arg1->unkD84 <= 0) {
                        u->unk68 |= 0x400000;
                    } else {
                        done = 0;
                        if ((s8)arg1->unkD84 == 0x20) {
                            btl_soundCmd(0xD000, 0xB5A, 0, 0x80, 0x7F);
                            btl_soundCmd(0xD000, 0xB5B, 0, 0x80, 0x7F);
                            btl_soundCmd(0xD000, 0xB5C, 0, 0x80, 0x7F);
                            arg1->unkD84 -= 2;
                        }
                    }
                }
                break;
            }
            btl_updateUnitTint(u);
        }
        btl_updateKo(u);
    }
    if (arg1->unkD81 == 3 && (s8)arg1->unkD84 < 0x20) {
        arg1->unkD84 -= 2;
    }
    if (done) {
        func_800E9F3C(0);
        switch (arg1->unkD81) {
        case 0:
        case 4:
            arg0->unk56F |= 0x40;
            arg0->unk56E = 1;
            if ((arg1->unk132C.unk0 >> 6) & 1) {
                if ((u8)btl_playVictoryMusic(arg0, 0x1E, 5) == 0) {
                    break;
                }
                for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
                    if (u->unk68 & 1) {
                        btl_removeStatuses(u, (BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_AUTO_LIFE | BTL_STATUS_DEFEND | BTL_STATUS_POISON | BTL_STATUS_SLEEP | BTL_STATUS_REGEN | BTL_STATUS_HASTE | BTL_STATUS_SLOW | BTL_STATUS_FLOAT | BTL_STATUS_SHELL | BTL_STATUS_PROTECT | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_VANISH | BTL_STATUS_DOOM | BTL_STATUS_MINI | BTL_STATUS_REFLECT | BTL_STATUS_GRADUAL_PETRIFY));
                        if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP))) {
                            if (u->hp != 0) {
                                u->unk86[1] = u->unkA0->unk22 = (u->unk68 & 1) * 0x800;
                                u->unk68 = (u->unk68 & ~0x40000) | ((btl_hasStatus(u, (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) & 1) << 18);
                                u->unk68 &= ~0x80000;
                                if (((g_gameState->unk1C->characters + ((u->unk68 >> 1) & 0xF))->unk34.w >> 10) & 1) {
                                    u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[0x12];
                                } else {
                                    u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
                                }
                                u->unkA0->unk3A = 0;
                            } else {
                                btl_addStatus(u, BTL_STATUS_KO);
                            }
                        }
                    }
                }
                btlcam_playScene(func_800CA2B0(2));
            } else if ((arg1->unk132C.unk0 >> 14) & 1) {
                arg0->unk56F |= 8;
            }
            arg1->unkD80 = 6;
            break;
        case 1:
            arg0->unk56E = 3;
            arg1->unkD80 = 7;
            break;
        case 2:
            if ((arg1->unk132C.unk0 & 0x4040) == 0x4000) {
                arg0->unk56F |= 8;
            }
            arg1->unkD80 = 1;
            btl_clearCmdQueue(arg1);
            btl_initCmdQueue(arg1);
            for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
                btl_initUnitCmds(u);
                if (!(u->unk68 & 1)) {
                    u->unkA0->unk22 = 0;
                    u->unk86[1] = 0;
                }
            }
            break;
        case 3:
            exp = g_btlRewards.unk0;
            arg0->unk56F |= 0x40;
            for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
                if (!(u->unk68 & 1)) {
                    exp += g_gameState->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0->unk44;
                }
            }
            if ((arg1->unk132C.unk0 & 0x4040) == 0x4000) {
                arg0->unk56F |= 8;
            }
            arg0->unk56E = 4;
            arg1->unkD80 = 8;
            btl_clearRewards();
            if (arg0->unk56F & 4) {
                exp /= 10;
                if (exp < arg0->gil) {
                    arg0->gil -= exp;
                } else {
                    exp = arg0->gil;
                    arg0->gil = 0;
                }
                btl_showMessage(0x1E, exp);
            } else if ((u8)btl_partyHasSupport(1, 0x4000) != 0) {
                g_btlRewards.unk1D = 1;
                g_btlRewards.unk0 = exp / 10;
            }
            break;
        }
        if (arg1->unkD80 != 5) {
            if (arg1->unkD80 == 6 && !((arg1->unk132C.unk0 >> 6) & 1)) {
                arg1->unkD81 = 1;
            } else {
                arg1->unkD81 = 0;
            }
        }
    } else {
        btl_processCmdQueue(arg1);
    }
    btl_drawScene(arg0, arg1, sp18);
}

void btl_updateIntro(GameData *arg0, Battle *arg1) {
    u32 *ot;
    BtlUnit *u;
    u8 f;
    u8 g;

    ot = (u32 *)arg1->unk11C0->unk34[arg0->unk8];
    func_800AB580(0);
    if ((arg1->unk0 & 2) && (*(u32 *)&arg1->unkD84 & 0x2800) == 0x800) {
        func_800C072C();
        arg1->unkD85 |= 0x20;
    } else if ((arg1->unk0 & 4) && (*(u32 *)&arg1->unkD84 & 0x5000) == 0x1000) {
        func_800C0840();
        arg1->unkD85 |= 0x40;
    }
    for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
        if ((u->unk68 >> 22) & 1) {
            continue;
        }
        btl_updateUnitModel(u, (s32 *)&ot);
        if (u->unk68 & 1) {
            btl_setUnitBrightness(u, (s8)arg1->unkD86);
        } else if (!((u->unk68 >> 23) & 1)) {
            btl_setUnitBrightness(u, (s8)arg1->unkD87);
        }
        btl_updateUnitTint(u);
    }
    if (arg1->unkD85 & 2) {
        btlseq_update();
    }
    if (arg1->unk0 & 1) {
        ot = (u32 *)btlbg_draw(&arg0->unk18, arg1->unk11C0->unk1C, (s32)ot, arg0->unk10[arg0->unk8],
                                  arg1->unk11C0->unk2C[arg0->unk8]);
    }
    ot = (u32 *)btl_drawBanner((u8 *)ot);
    if (arg1->unk0 & 2) {
        f = arg1->unkD85;
        if (!(f & 2)) {
            if (!(f & 8)) {
                func_800C0C7C(ot);
                btl_initEnemies(arg1);
                ot = func_800BB974(arg1, ot);
            } else if (f & 0x20) {
                if ((s8)arg1->unkD87 >= 0x20) {
                    arg1->unkD85 = f | 2;
                } else {
                    arg1->unkD87 += 4;
                }
            }
        }
    }
    if (arg1->unk0 & 4) {
        g = arg1->unkD85;
        if (!(g & 4)) {
            if (!(g & 0x10)) {
                func_800C0CD0(ot);
                btl_initParty(arg1);
            } else if (g & 0x40) {
                if ((s8)arg1->unkD86 >= 0x20) {
                    arg1->unkD85 = g | 4;
                } else {
                    arg1->unkD86 += 4;
                }
            }
        }
    }
}

void btl_initBattleState(void) {
    GameData *gd;
    s32 v;
    g_gameState->unk1C->unk56F = 0;
    gd = g_gameState->unk1C;
    v = 7;
    if (gd->unk571 != 0) {
        v = 8;
    }
    gd->characters[3].unk34.w = (gd->characters[3].unk34.w & ~0x1F0) | (v << 4);
    g_gameState->unk1C->unk56E = 0;
    btl_initState();
    func_800E3D80();
    btlcam_init();
    func_800CBF90();
    func_800F4DFC();
}
