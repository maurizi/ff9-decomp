#include "ovl_0b.h"

void btlfx_reset(void) {
    g_btlfxState.unk0 = -1;
    g_btlfxState.unk22 = 0;
    g_btlfxState.unk24 = 0;
}

s32 btlfx_resolveFirstUse(s32 arg0) {
    s32 id;
    s32 i;
    u8 *p;
    s32 bit;
    GameState *g;

    id = arg0;
    if (func_800E8498(0) >= 0x385) {
        for (p = D_800F727C[0]; *p != 0xFF; p += 2) {
            if (p[0] == arg0) {
                id = p[1];
                break;
            }
        }
    }
    bit = 1;
    g = g_gameState;
    for (i = 0; i < 16; i++, bit <<= 1) {
        if (g_btlfxFirstUsePairs[i][0] == id && !(g->unk1C->firstUseFxSeen & bit)) {
            id = g_btlfxFirstUsePairs[i][1];
            g->unk1C->firstUseFxSeen |= bit;
            break;
        }
        if (g_btlfxFirstUsePairs[i][1] == id) {
            if (!(g->unk1C->firstUseFxSeen & bit)) {
                g->unk1C->firstUseFxSeen |= bit;
            }
            break;
        }
    }
    return id;
}

s32 btlfx_start(s32 arg0) {
    if (g_btlfxState.unk0 != -1) {
        return -1;
    }
    g_btlfxState.unk0 = 0;
    g_btlfxState.unk2 = btlfx_resolveFirstUse(arg0);
    g_btlfxState.unkF = 0;
    g_btlfxState.unk23 = 0;
    g_btlfxState.unk4 = 0;
    g_btlfxState.unk10.w = 0;
    g_btlfxState.unk6 = 0;
    g_btlfxState.unk8 = 0;
    g_btlfxState.unk18 = 0;
    g_btlfxState.unk22 = 0;
    g_btlfxState.unk24 = 0;
    g_btlfxState.unk25 = 0;
    g_btlfxState.unk1C = 0x8000;
    func_800C30B8();
    g_btlfxEnv->unk30 = g_gameState->unk1C->unk8E4.battle->unk11C0->unkC;
    return 0;
}

void btlfx_retryOp(void) {
    g_btlfxState.unk20 = 1;
    g_btlfxState.unk18 -= 3;
}

u8 *btlfx_findScript(s32 arg0) {
    s32 i;
    u8 *p;
    s32 n;
    for (i = 0, p = (u8 *)0x801E7700; i < 0x200; i++) {
        n = p[0];
        if (p[1] & 0x80) {
            n += 0x100;
        }
        if (n == arg0) {
            break;
        }
        p += (p[1] & 0x7F) * 3 + 2;
    }
    return p + 2;
}

void btlfx_swapVramPages(s32 arg0, u32 *arg1, s32 arg2) {
    RECT r;
    s32 i;
    s32 bit = 1;
    u32 *p;
    if (arg0 != 2) {
        p = (u32 *)(g_gameState->unk1C->unk8E4.battle->unk153C[0] + 0x50000);
    } else {
        arg2 = 1 << arg1[0x1FFF];
        p = arg1;
    }
    r.w = 0x40;
    r.h = 0x80;
    func_800D0940();
    for (i = 0; i < 4; i++) {
        if (bit & arg2) {
            p -= 0x2000;
            r.x = (i >> 1) * 64 + 0x380;
            r.y = (i & 1) << 7;
            if (arg0 == 0) {
                StoreImage(&r, p);
            } else {
                LoadImage(&r, p);
            }
        }
        if (i & 1) {
            bit <<= 1;
        }
    }
}

void btlfx_callScript(s32 arg0) {
    D_800FEC38 = 1;
    D_800FEC3C = g_btlfxState.unk18;
    g_btlfxState.unk18 = btlfx_findScript(arg0);
}

void btlfx_runScript(void) {
    RECT r;
    u8 *p;
    s16 op;
    s16 a;
    s16 b;
    s32 cont;
    u16 hdr;
    s32 n;
    s32 k;
    Unk800FEC40 *d = &g_btlfxState;

    if (d->unk22 != 0 && func_800AB410(0) == 0) {
        d->unk22 = 0;
        btlfx_restorePartyLooks();
    }
    if (d->unk24 != 0 && func_800AB3E4(d->unk24 - 1, 0) == 0) {
        d->unk24 = 0;
        btlfx_restoreUnitLook(0x10);
    }
    switch (d->unk0) {
    case 0:
        r.x = 0x300;
        r.y = 0x140;
        r.w = 0x40;
        r.h = 0x10;
        func_800D0940();
        StoreImage(&r, (u32 *)0x801E7700);
        D_800FEC38 = 0;
        d->unk0 = 1;
        break;
    case 1:
        hdr = ((u16 *)0x801E7700)[d->unk2];
        d->unk1C = hdr & 0xFFFE;
        g_btlfxEnv->unkC->unk40 &= ~8;
        if (hdr & 0xC00) {
            btlfx_swapVramPages(0, NULL, (hdr & 0xC00) >> 10);
        }
        if (hdr & 0x8000) {
            r.x = 0x300;
            r.y = 0x150;
            r.w = 0x40;
            r.h = 0xB0;
            func_800D0940();
            StoreImage(&r, (u32 *)0x801E7700);
            d->unk0 = 2;
            d->unk1C &= ~8;
            d->unk1C &= ~4;
            break;
        }
        d->unk21 = 0;
        func_800E9168();
        btlcam_beginEffect();
        if (!(g_btlfxEnv->unkC->unk40 & 1)) {
            func_800E9290(d->unk1C);
        }
        if (btlfx_loadFile(d->unk2) == 0) {
            k = (u8)d->unk1C >> 4;
            if (k == 1) {
                g_btlfxEnv->unkC->unk40 |= 8;
            }
            btlfx_startCasterTask(k);
            d->unk18 = D_800FF068;
            d->unk0 = 3;
        }
        break;
    case 3:
        if (g_btlfxEnv->unk4->unkE == 3) {
            d->unk0 = 4;
            g_btlfxEnv->unk2C->unk5C |= 2;
        }
        break;
    case 2:
        d->unk18 = btlfx_findScript(d->unk2);
        d->unk0 = 4;
        func_800E8D14();
        break;
    case 4:
        /* padded: a zero word follows the first jump table */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        cont = 1;
        do {
            if (d->unk4 != 0) {
                if (--d->unk4 > 0) {
                    return;
                }
                d->unk4 = 0;
            }
            p = d->unk18;
            op = p[0];
            a = p[1];
            b = p[2];
            d->unk18 = p + 3;
            if (op < 0x20) {
                switch (op) {
                case 0:
                    cont = 0;
                    d->unk18 -= 3;
                    if (func_800E8CE4() == 0) {
                        func_800E8C84(1);
                    }
                    if (g_btlfxTasks.unk0 == 0) {
                        if (d->unk1C & 1) {
                            if (d->unk21 == 0) {
                                btlcam_endEffect();
                                func_800E3C58();
                                func_800E970C();
                                func_800E954C();
                                btlfx_reloadCommonData(0);
                                d->unk21 = 1;
                            } else if (btlfx_reloadCommonData(1) != 0) {
                                d->unk1C &= ~1;
                            }
                        } else {
                            d->unk0 = -1;
                            g_btlfxEnv->unkC = NULL;
                            btlcam_endEffect();
                            func_800E3C58();
                            func_800E970C();
                            func_800E954C();
                            func_80054F80();
                        }
                    }
                    break;
                case 1:
                    if (a == 0) {
                        d->unk4 = b;
                        cont = 0;
                    } else if (d->unk10.b[b] == 0) {
                        cont = 0;
                        d->unk18 -= 3;
                        d->unk10.b[b] = 0;
                    }
                    break;
                case 2:
                    d->unk10.b[a] = b;
                    break;
                case 3:
                    func_800DA5C4();
                    break;
                case 4:
                    if (g_btlfxEnv->unk4->unkE < a) {
                        cont = 0;
                        btlfx_retryOp();
                    }
                    break;
                case 5:
                    d->unk6 = a;
                    d->unk8 = btlfx_findFileSlot(a);
                    break;
                case 6:
                    switch (a) {
                    case 2:
                        if (g_btlfxEnv->unk4->unk53 == 0) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    case 1:
                        if (g_btlfxEnv->unk4->unk52 != 2) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    case 0:
                        if (g_btlfxEnv->unk4->unk50 != 2) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    }
                    break;
                case 8:
                    if (btlfx_isEnemyUnit(0x10) != 0) {
                        g_btlfxState.unk23 = 1;
                        if (!(g_btlfxEnv->unkC->unk40 & 0x10) && d->unkF == 0) {
                            cont = 0;
                            d->unk18 -= 3;
                        }
                    }
                    break;
                case 9:
                    if (func_800DA6B4(a, b)) {
                        cont = 0;
                        btlfx_retryOp();
                    }
                    break;
                case 10:
                    if (func_800DA634(0)) {
                        cont = 0;
                        btlfx_retryOp();
                    }
                    break;
                case 11:
                    if (a != 0) {
                        if (d->unk22 != 0) {
                            d->unk18 -= 3;
                        }
                        cont = 0;
                    } else {
                        func_800AB410(1);
                        d->unk22 = 1;
                        cont = 0;
                    }
                    break;
                case 12:
                    if (g_btlfxEnv->unk4->unk68 != 2) {
                        cont = 0;
                        d->unk18 -= 3;
                    }
                    break;
                case 13:
                    if (a != 0) {
                        if (d->unk24 != 0) {
                            d->unk18 -= 3;
                        }
                        cont = 0;
                    } else {
                        n = func_800E7D18(b);
                        func_800AB3E4(n, 1);
                        n++;
                        d->unk24 = n;
                        cont = 0;
                    }
                    break;
                case 7:
                    switch (a) {
                    case 0:
                        btlfx_swapVramPages(1, NULL, (d->unk1C & 0xC00) >> 10);
                        break;
                    case 1:
                        btlfx_swapVramPages(2, (u32 *)btlfx_getFileData(-1, b), 0);
                        break;
                    case 2:
                        if (g_btlfxEnv->unk4->unk69 < b) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    }
                    break;
                case 14:
                    if (D_800FEC38 != 0) {
                        D_800FEC38 = 0;
                        d->unk18 = D_800FEC3C;
                    }
                    break;
                case 15:
                    if (b == 0) {
                        btlfx_copyWords(D_800FE530, (s32 *)btlfx_getFileData(d->unk8, a), 0x708);
                        func_80054A28(D_800FE530);
                        func_80054E88(0, 0, 1);
                        func_80054E88(0, 1, 1);
                    } else {
                        func_80054FE0(0);
                        func_80054E88(1, 0, 1);
                        func_80054E88(1, 1, 1);
                    }
                    func_80054FB8(1);
                    break;
                case 20:
                    break;
                }
            } else {
                if (op < 0x80) {
                    btlfx_spawnTask(op - 0x20, d->unk8, a, b);
                } else {
                    btlfx_spawnTask(op, d->unk8, a, b);
                }
            }
        } while (cont);
        break;
    }
}

void btlfx_initTasks(void) {
    s32 i;
    Unk80100C78Entry *p;
    g_btlfxTasks.unk0 = 0;
    btlfx_heapInit(&D_800FF470, 0x1800);
    p = g_btlfxTasks.unk4;
    for (i = 10; i >= 0; i--) {
        p->unk4.w = 0;
        p++;
    }
}

s32 btlfx_allocTaskMem(Unk80100C78Entry *arg0, s32 arg1) {
    s32 r = 0;
    s32 size = arg1 + 4;
    s32 i = 0;
    Heap *h = g_btlfxLoader.unk3C[arg0->unk8];
    for (; i < 2; i++) {
        if (arg0->unkA != 0 && h != NULL) {
            if (btlfx_heapWrapIfFull(h, size) == 0) {
                r = btlfx_heapAlloc(h, size);
                break;
            }
            arg0->unkA = 0;
        } else {
            if (btlfx_heapWrapIfFull(&D_800FF470, size) == 0) {
                r = btlfx_heapAlloc(&D_800FF470, size);
                break;
            }
            arg0->unkA = 1;
        }
    }
    return r;
}

void btlfx_spawnTask(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Unk80100C78Entry *e;
    s32 i;
    EffFn *tbl;
    s32 idx;
    Unk80100C78 *d = &g_btlfxTasks;
    EffInit init;
    s32 args[2];

    e = d->unk4;
    i = 0;
loop:
    if (e->unk4.h != 0) {
        i++;
        e++;
        if (i < 11) {
            goto loop;
        }
    }
    d->unk0++;
    e->unk4.h = 1;
    e->unkA = 1;
    e->unk0 = 0;
    e->unk8 = arg1;
    if (arg0 >= 0x80) {
        e->unk14 = D_80100C70[arg1][arg0 - 0x80];
    } else {
        if (arg0 < 0x30) {
            e->unk14 = g_btlfxResidentOps[arg0];
        } else {
            e->unk14 = g_btlfxUnitOps[arg0 - 0x30];
        }
    }
    init.unk4 = 0;
    init.unk0 = 0;
    e->unk10 = 0;
    e->unkC = 0;
    if (e->unk14(0, 0, 0, 0, &init) == 0) {
        if (init.unk4 != 0) {
            e->unk10 = (void *)btlfx_allocTaskMem(e, btlfx_poolSize(init.unk8, init.unk4));
            btlfx_poolInit(e->unk10, e, init.unk8, init.unk4, init.unk14);
        }
        if (init.unk0 != 0) {
            e->unkC = (void *)btlfx_allocTaskMem(e, btlfx_align4(init.unk0));
        }
    }
    args[0] = arg2;
    args[1] = arg3;
    if (e->unk14(1, e->unkC, g_btlfxEnv, e, args) != 0) {
        e->unk4.h = 0;
        d->unk0--;
        if (e->unkC != 0) {
            btlfx_heapFree(e->unkC);
        }
        if (e->unk10 != 0) {
            btlfx_heapFree(e->unk10);
        }
    }
}

s32 btlfx_updateTasks(void) {
    Unk80100C78 *d = &g_btlfxTasks;
    Unk80100C78Entry *e;
    s32 n;
    s32 i;
    s32 r;

    e = d->unk4;
    n = 0;
    for (i = 0; i < 11; i++, e++) {
        if (e->unk4.h != 0) {
            n++;
            g_btlfxEnv->unk28 = 0;
            if (e->unk14(2, e->unkC, g_btlfxEnv, e, NULL) != 0) {
                e->unk4.h = 2;
            } else {
                e->unk0++;
            }
            if (e->unk10 != NULL) {
                r = btlfx_poolUpdate(e->unk10);
            } else {
                r = 0;
            }
            if (r == 0 && e->unk4.h == 2) {
                e->unk4.h = 0;
                d->unk0--;
                if (e->unkC != NULL) {
                    btlfx_heapFree(e->unkC);
                }
                if (e->unk10 != NULL) {
                    btlfx_heapFree(e->unk10);
                }
            }
        }
    }
    return n;
}

s32 btlfx_moveUnitArc(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4, s16 *arg5) {
    s16 sp10[4];
    s32 t;
    if (arg2 == 1) {
        btlfx_getUnitPos(arg0, arg5);
    }
    t = (arg2 << 12) / arg3;
    btlfx_lerpVec(arg5, arg1, t, sp10);
    if (arg4 != 0) {
        sp10[1] -= (math_sin(t >> 1) * arg4) >> 12;
    }
    btlfx_setUnitPos(arg0, sp10);
    return arg2 >= arg3;
}

s32 btlfx_moveUnitLinear(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s16 *arg4, s32 arg5) {
    s16 sp10[4];
    s32 t;
    if (arg2 == 0) {
        switch (arg5) {
        case 0:
            btlfx_getUnitHomePos(arg0, arg4);
            break;
        case 1:
            btlfx_getUnitPos(arg0, arg4);
            break;
        default:
            return 1;
        }
    }
    t = ((arg2 + 1) << 12) / arg3;
    if (t > 0x1000) {
        t = 0x1000;
    }
    btlfx_lerpVec(arg4, arg1, t, sp10);
    btlfx_setUnitPos(0x10, sp10);
    return arg2 >= arg3 - 1;
}

s32 btlfx_opSetParamsA(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 1) {
        D_80100D88.unk0 = arg4[0];
        D_80100D88.unk1 = arg4[1];
    }
    return 1;
}

s32 btlfx_opSetParamsB(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    if (arg0 == 1) {
        D_80100D88.unk2 = arg4[0];
        D_80100D88.unk3 = arg4[4];
    }
    return 1;
}

s32 btlfx_opSwingSound(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        if (btlfx_isPartyUnit(arg1[0]) == 0 || arg1[1] == arg0) {
            return 1;
        }
        return 0;
    } else {
        return btlfx_stepSwingSound(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opHitSound(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = D_80100D88.unk0;
        arg1[3] = D_80100D88.unk1;
        if (btlfx_isPartyUnit(arg1[0]) == 0) {
            return 1;
        }
        if (arg1[3] == arg0) {
            return 1;
        }
    } else {
        return btlfx_stepHitSound(arg1, arg2, arg3);
    }
    return 0;
}

s32 func_800D7578(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D8D08(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
    }
    return 0;
}

s32 btlfx_opPlayMotion(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return btlfx_stepPlayMotion(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
    }
    return 0;
}

s32 btlfx_opPlayMotionWait(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return btlfx_stepPlayMotionWait(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D76DC(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8E10(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        arg1->unk2 = D_80100D88.unk0;
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D7768(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8EA4(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D77E4(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8F1C(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        arg1->unk2 = D_80100D88.unk0;
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D7870(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8FAC(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC54 = 0;
    }
    return 0;
}

s32 btlfx_opResetMotion(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return btlfx_stepResetMotion(arg1, arg2, arg3);
    } else {
        *arg1 = *arg4;
    }
    return 0;
}

s32 func_800D7948(s32 arg0, u8 *arg1, s32 arg2, Unk800F78A40 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D905C(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
    }
    return 0;
}

s32 func_800D79BC(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D908C(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D7A38(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D9110(arg1, arg2, arg3);
    } else {
        *arg1 = *arg4;
    }
    return 0;
}

s32 func_800D7A94(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x18;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        if (btlfx_isSameUnit(arg1[0], arg1[1]) != 0) {
            D_800FEC55 = arg0;
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return func_800D9134((Unk800D9134 *)arg1, arg2, (s32 *)arg3);
    }
    return 0;
}

s32 btlfx_opRunHome(s32 arg0, Unk800D9260 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x10;
    } else if (arg0 != 1) {
        return btlfx_stepRunHome(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC55 = 0;
    }
    return 0;
}

s32 func_800D7BA8(s32 arg0, Unk800D7C9C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x1C;
    } else if (arg0 == 1) {
        arg1->unk0 = 0x10;
        arg1->unk1 = arg4[0];
        arg1->unk8 = arg4[1];
        arg1->unk4 = D_80100D88.unk0 | (D_80100D88.unk1 << 8);
        arg1->unk6 = D_80100D88.unk2 | (D_80100D88.unk3 << 8);
        if (btlfx_isSameUnit(arg1->unk0, arg1->unk1) != 0) {
            D_800FEC55 = arg0;
            return 1;
        }
        if (btlfx_isPartyUnit(arg1->unk1) == 0) {
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return func_800D9350(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opJumpForward(s32 arg0, Unk800D7C9C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    if (arg0 == 0) {
        *arg4 = 0x1C;
    } else if (arg0 == 1) {
        arg1->unk0 = 0x10;
        arg1->unk1 = arg4[0];
        arg1->unk2 = arg4[1];
        arg1->unk3 = 1;
        arg1->unk4 = D_80100D88.unk0 | (D_80100D88.unk1 << 8);
        if ((v = arg1->unk2) == arg0 && btlfx_isPartyUnit(arg1->unk1) != 0) {
            D_800FEC55 = v;
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return btlfx_stepJumpForward(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opJumpHome(s32 arg0, Unk800D955C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    if (arg0 == 0) {
        *arg4 = 0x18;
    } else if (arg0 == 1) {
        arg1->unk0 = 0x10;
        arg1->unk1 = arg4[0];
        arg1->unk2 = arg4[1];
        arg1->unk3 = 1;
        if ((v = arg1->unk2) == arg0 && btlfx_isPartyUnit(arg1->unk1) != 0) {
            D_800FEC55 = v;
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return btlfx_stepJumpHome(arg1, arg2, arg3);
    }
    return 0;
}

s32 func_800D7E14(s32 arg0, Unk800D7E14 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x10;
    } else if (arg0 != 1) {
        return func_800D960C(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        arg1->unk4 = D_80100D88.unk0;
        do {
            D_800FEC55 = 0;
        } while (0);
    }
    return 0;
}

s32 func_800D7EA0(s32 arg0, Unk800D7C9C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x1C;
    } else if (arg0 == 1) {
        arg1->unk0 = 0x10;
        arg1->unk1 = arg4[0];
        arg1->unk8 = arg4[1];
        arg1->unk4 = D_80100D88.unk0 | (D_80100D88.unk1 << 8);
        arg1->unk2 = D_80100D88.unk2;
        if (btlfx_isSameUnit(arg1->unk0, arg1->unk1) != 0 || btlfx_isPartyUnit(arg1->unk1) != 0) {
            D_800FEC55 = arg0;
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return func_800D974C(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opRunToTarget(s32 arg0, Unk800D7C9C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x1C;
    } else if (arg0 == 1) {
        arg1->unk0 = 0x10;
        arg1->unk1 = arg4[0];
        arg1->unk8 = arg4[1];
        arg1->unk4 = D_80100D88.unk0 | (D_80100D88.unk1 << 8);
        if (btlfx_isSameUnit(arg1->unk0, arg1->unk1) != 0) {
            D_800FEC55 = arg0;
            return 1;
        }
        if (btlfx_isPartyUnit(arg1->unk1) != 0) {
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return btlfx_stepRunToTarget(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opFaceTarget(s32 arg0, Unk800D805C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x14;
    } else if (arg0 == 1) {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        arg1->unk4 = D_80100D88.unk0;
        arg1->unk2 = D_80100D88.unk1;
        arg1->unk3 = D_80100D88.unk2;
        arg1->unk8 = 0;
        if (btlfx_isSameUnit(arg1->unk0, arg1->unk1) != 0) {
            D_800FEC56 = arg0;
            return 1;
        }
        D_800FEC56 = 0;
    } else {
        return btlfx_stepFaceTarget(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opFaceHome(s32 arg0, Unk800D8124 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x10;
    } else if (arg0 == 1) {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        arg1->unk4 = D_80100D88.unk0;
        arg1->unk2 = D_80100D88.unk1;
        if (arg1->unk2 == 2 && btlfx_isPartyUnit(arg1->unk1) != 0) {
            D_800FEC56 = arg0;
            return 1;
        }
        D_800FEC56 = 0;
    } else {
        return btlfx_stepFaceHome(arg1, arg2, arg3);
    }
    return 0;
}

s32 func_800D81EC(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D9F5C(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = D_80100D88.unk0;
    }
    return 0;
}

s32 func_800D8270(s32 arg0, u8 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    s32 w;
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = D_80100D88.unk0;
        if (func_800DA06C(arg1[0], arg1[2]) == 0) {
            D_800FEC57 = arg0;
            return 1;
        }
        if ((v = arg1[2]) == arg0 && (w = btlfx_isSameSide(arg1[0], arg1[1])) == v) {
            D_800FEC57 = w;
            return 1;
        }
        D_800FEC57 = 0;
    } else {
        return func_800D9F88(arg1, arg2, arg3);
    }
    return 0;
}

s32 func_800D8368(s32 arg0, u8 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = D_80100D88.unk0;
        if (btlfx_isSameSide(arg1[0], arg1[1]) == 0) {
            D_800FEC57 = arg0;
            return 1;
        }
        D_800FEC57 = 0;
    } else {
        return func_800D9FD8(arg1, arg2, arg3);
    }
    return 0;
}

s32 func_800D8414(s32 arg0, u8 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    u8 a, b;
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = D_80100D88.unk0;
        v = btlfx_isSameSide(arg1[0], arg1[1]);
        if (v == arg0) {
            D_800FEC57 = v;
            return 1;
        }
        D_800FEC57 = 0;
    } else {
        return func_800DA004(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opCallWeaponScript(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return btlfx_stepCallWeaponScript(arg1, arg2, arg3);
    } else {
        *arg1 = *arg4;
    }
    return 0;
}

s32 func_800D8520(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        if (btlfx_isSameSide(arg1[0], arg1[1]) == arg0) {
            return 1;
        }
    } else {
        return func_800DA064(arg1, arg2, arg3);
    }
    return 0;
}

void btlfx_playUnitMotion(s32 arg0, s32 arg1) {
    if (btlfx_isPartyUnit(arg0) != 0) {
        btlfx_setUnitMotion(arg0, btlfx_getUnitMotion(arg0, arg1));
    }
}

void func_800D85F8(Unk800F78A40 *arg0, s32 arg1, s32 arg2) {
    if (btlfx_isPartyUnit(arg1) != 0) {
        btlfx_setUnitMotion(arg1, btlfx_getFileData(arg0->unk8, arg2));
    }
}

void btlfx_resetUnitMotion(s32 arg0, u8 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    btlfx_setUnitMotion(arg0, u->motions[0]);
    if (arg1) {
        u->rot[0] = u->unkA0->unk20;
        u->rot[1] = u->unkA0->unk22;
        u->rot[2] = u->unkA0->unk24;
    }
}

void btlfx_setUnitMotion(s32 arg0, s32 arg1) {
    if (btlfx_isPartyUnit(arg0) != 0) {
        BtlUnit *u = btl_getUnit(arg0);
        BtlUnit9C *m = u->model;
        if (m != NULL) {
            m->unkC = (BtlUnit9CC *)arg1;
            u->unkA0->unk3A = 0;
        }
    }
}

s32 btlfx_getUnitMotion(s32 arg0, s32 arg1) {
    return btl_getUnit(arg0)->motions[arg1];
}

void btlfx_setMotionFrame(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (arg1 >= u->model->unkC->unk2) {
        arg1 = u->model->unkC->unk2;
    }
    u->unkA0->unk3A = arg1;
}

s32 btlfx_getMotionLength(s32 arg0) {
    return btl_getUnit(arg0)->model->unkC->unk2;
}

u8 btlfx_getMotionFrame(s32 arg0) {
    return btl_getUnit(arg0)->unkA0->unk3A;
}

void btlfx_turnUnit(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    s16 a[4];
    s16 b[4];
    s16 c[4];
    switch (arg1) {
    case 0:
        u->rot[1] = u->unkA0->unk22 + 0x800;
        break;
    case 1:
        btlfx_getUnitPos(arg0, a);
        b[0] = 0;
        b[1] = 0;
        b[2] = 0;
        btlfx_aimAngles(a, b, c);
        btlfx_setUnitRot(arg0, c);
        break;
    case 2:
        u->rot[0] = u->unkA0->unk20;
        u->rot[1] = u->unkA0->unk22;
        u->rot[2] = u->unkA0->unk24;
        break;
    }
}

void btlfx_playSwingSound(s32 arg0) {
    BtlUnit *u = btl_getUnit(arg0);
    if (btlfx_isPartyUnit(arg0) != 0) {
        btl_soundCmd(0xD000, func_800AE47C((u8)u->unk68 >> 5, 0), 0, btl_soundPan(arg0), 0x7F);
    }
}

void btlfx_playHitSound(s32 arg0, s32 arg1, s32 arg2) {
    BtlUnit *a = btl_getUnit(arg0);
    BtlUnit *b = btl_getUnit(arg1);
    if (btlfx_isPartyUnit(arg0) != 0) {
        btl_playHitSound(a, b);
    }
}

u32 func_800D89E8(s32 arg0) {
    BtlUnit *u = btl_getUnit(arg0);
    u32 f = u->unk68;
    u->unk68 = f | 0x200000;
    return (f >> 21) & 1;
}

void func_800D8A20(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    u->unk68 = (u->unk68 & ~0x200000) | ((arg1 & 1) << 21);
}

s32 btlfx_isPartyUnit(s32 arg0) {
    if (arg0 == 0xFF) {
        arg0 = 0;
    }
    return btl_getUnit(arg0)->unk68 & 1;
}

s32 btlfx_isSameSide(s32 arg0, s32 arg1) {
    return btlfx_isPartyUnit(arg0) == btlfx_isPartyUnit(arg1);
}

s32 btlfx_isSameUnit(s32 arg0, s32 arg1) {
    BtlUnit *a;
    if (arg0 == 0xFF || arg1 == 0xFF) {
        return 0;
    }
    a = btl_getUnit(arg0);
    return a == btl_getUnit(arg1);
}

s32 func_800D8B20(s32 arg0) {
    s32 idx = (btl_getUnit(arg0)->unk68 >> 1) & 0xF;
    return g_gameState->unk1C->characters[idx].unk34.b.unk4;
}

s32 btlfx_getUnitWeapon(s32 arg0) {
    s32 idx = (btl_getUnit(arg0)->unk68 >> 1) & 0xF;
    return g_gameState->unk1C->characters[idx].equip[0];
}

void btlfx_setUnitVisible(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (arg1 != 0) {
        u->unk68 &= ~0x400000;
    } else {
        u->unk68 |= 0x400000;
    }
}

void btlfx_setWeaponVisible(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (arg1 != 0) {
        btl_showWeapon(u);
    } else {
        btl_hideWeapon(u);
    }
}

s32 btlfx_isUnitActive(s32 arg0) {
    return btlfx_getUnitStatus(arg0, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) == 0;
}

u32 btlfx_getUnitStatus(s32 arg0, u32 arg1) {
    return btl_getUnit(arg0)->status & arg1;
}

s32 func_800D8C9C(void) {
    return 0;
}

s32 func_800D8CA4(void) {
    return -1;
}

void func_800D8CAC(void) {
}

s32 btlfx_stepSwingSound(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_playSwingSound(*arg0);
    return 1;
}

s32 btlfx_stepHitSound(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_playHitSound(arg0[0], arg0[1], arg0[2]);
    return 1;
}

s32 func_800D8D08(u8 *arg0, s32 arg1, s32 arg2) {
    BtlUnit *u = btl_getUnit(arg0[0]);
    switch (arg0[1]) {
    case 0:
        btl_playUnitSound(u, 0x456, 0x7F);
        break;
    case 1:
        btl_playUnitSound(u, 0xAC0, 0x7F);
        break;
    }
    return 1;
}

s32 btlfx_stepPlayMotion(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_playUnitMotion(arg0[0], arg0[1]);
    return 1;
}

s32 btlfx_stepPlayMotionWait(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_playUnitMotion(arg0->unk0, arg0->unk1);
        arg0->unk4 = btlfx_getMotionLength(arg0->unk0) - 1;
    }
    if (v >= arg0->unk4) {
        D_800FEC54 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D8E10(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_playUnitMotion(arg0->unk0, arg0->unk1);
        arg0->unk4 = btlfx_getMotionLength(arg0->unk0) - arg0->unk2 - 1;
    }
    if (v >= arg0->unk4) {
        D_800FEC54 = 1;
        func_800D89E8(arg0->unk0);
        return 1;
    }
    return 0;
}

s32 func_800D8EA4(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_playUnitMotion(arg0->unk0, 0x17);
        arg0->unk4 = btlfx_getMotionLength(arg0->unk0) - 1;
    }
    if (v >= arg0->unk4) {
        D_800FEC54 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D8F1C(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_playUnitMotion(arg0->unk0, 0x17);
        arg0->unk4 = btlfx_getMotionLength(arg0->unk0) - arg0->unk2 - 1;
    }
    if (v >= arg0->unk4) {
        func_800D89E8(arg0->unk0);
        D_800FEC54 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D8FAC(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (btlfx_isPartyUnit(arg0->unk1) == 0) {
        if (v == 0) {
            btlfx_playUnitMotion(arg0->unk0, 0x19);
            arg0->unk4 = btlfx_getMotionLength(arg0->unk0) - 1;
        }
        if (v < arg0->unk4) {
            return 0;
        }
    }
    D_800FEC54 = 1;
    return 1;
}

s32 btlfx_stepResetMotion(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_resetUnitMotion(*arg0, 1);
    return 1;
}

s32 func_800D905C(u8 *arg0, s32 arg1, Unk800F78A40 *arg2) {
    func_800D85F8(arg2, arg0[0], arg0[1]);
    return 1;
}

s32 func_800D908C(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        func_800D85F8((Unk800F78A40 *)arg2, arg0->unk0, arg0->unk1);
        arg0->unk4 = btlfx_getMotionLength(arg0->unk0) - 1;
    }
    if (v >= arg0->unk4) {
        D_800FEC54 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D9110(u8 *arg0, s32 arg1, s32 arg2) {
    func_800D8A20(*arg0, 0);
    return 1;
}

s32 func_800D9134(Unk800D9134 *p, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s16 *pos;
    s16 c[4];
    s32 n = *arg2;
    s32 v;
    if (btlfx_isPartyUnit(p->unk1) == 0) {
        if (n == 0) {
            p->unk4 = func_800DA06C(p->unk0, 0);
            pos = p->unk10;
            btlfx_getUnitPos(p->unk1, pos);
            v = p->unk4;
            if (v == 1) {
                btlfx_setUnitPos(p->unk0, p->unk10);
                D_800FEC55 = v;
                return 1;
            }
        }
        if (btlfx_moveUnitArc(p->unk0, p->unk10, n + 1, p->unk4, 0, p->unk8) != 0) {
            D_800FEC55 = 1;
            return 1;
        }
        return 0;
    }
    btlfx_getUnitHomePos(p->unk1, b);
    btlfx_getUnitRot(p->unk1, c);
    func_800CFD78(b, c, 1000, a);
    btlfx_setUnitPos(p->unk0, a);
    D_800FEC55 = 1;
    return 1;
}

s32 btlfx_stepRunHome(Unk800D9260 *arg0, s32 arg1, s32 *arg2) {
    s16 sp18[4];
    s32 n = *arg2;
    s32 v;
    btlfx_getUnitHomePos(arg0->unk0, sp18);
    if (btlfx_isPartyUnit(arg0->unk1) == 0) {
        if (n == 0) {
            v = func_800DA06C(arg0->unk0, 3);
            arg0->unk4 = v;
            if (v == 1) {
                btlfx_setUnitPos(arg0->unk0, sp18);
                D_800FEC55 = v;
                return 1;
            }
        }
        if (btlfx_moveUnitArc(arg0->unk0, sp18, n + 1, arg0->unk4, 0, arg0->unk8) != 0) {
            D_800FEC55 = 1;
            return 1;
        }
        return 0;
    }
    btlfx_setUnitPos(arg0->unk0, sp18);
    D_800FEC55 = 1;
    return 1;
}

s32 func_800D9350(Unk800D7C9C *arg0, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s32 m;
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitPos(arg0->unk0, a);
        btlfx_getUnitHomePos(arg0->unk1, b);
        c[0] = 0;
        c[1] = 0;
        c[2] = 0;
        if (a[1] == 0) {
            if (b[1] != 0) {
                b[1] = 0;
            }
        } else {
            b[1] = a[1];
        }
        func_800CFD78(b, c, arg0->unk4, a);
        func_800CFD78(b, c, arg0->unk6, arg0->unk14);
        btlfx_setUnitPos(arg0->unk0, a);
        m = arg0->unk8;
        if (m == 1) {
            btlfx_setUnitPos(arg0->unk0, arg0->unk14);
            D_800FEC55 = m;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(arg0->unk0, arg0->unk14, v + 1, arg0->unk8, 0, arg0->unkC) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepJumpForward(Unk800D7C9C *arg0, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitPos(arg0->unk0, a);
        btlfx_getUnitRot(arg0->unk0, b);
        func_800CFD78(a, b, arg0->unk4, arg0->unk14);
        if (arg0->unk3 != 0) {
            btlfx_playUnitMotion(arg0->unk0, 0x1D);
        }
        arg0->unk8 = btlfx_getMotionLength(arg0->unk0);
    }
    if (btlfx_moveUnitLinear(arg0->unk0, arg0->unk14, v, arg0->unk8, arg0->unkC, 0) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepJumpHome(Unk800D955C *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitHomePos(arg0->unk0, arg0->unk10);
        if (arg0->unk3 != 0) {
            btlfx_playUnitMotion(arg0->unk0, 0x1E);
        }
        arg0->unk4 = btlfx_getMotionLength(arg0->unk0);
    }
    if (btlfx_moveUnitLinear(arg0->unk0, arg0->unk10, v, arg0->unk4, arg0->unk8, 1) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D960C(Unk800D7E14 *arg0, s32 arg1, s32 arg2) {
    s16 sp18[4];
    s32 n = *(s32 *)arg2;
    s32 v;
    switch (arg0->unk1) {
    case 0:
        sp18[0] = 0;
        sp18[1] = 0;
        sp18[2] = -0x100;
        break;
    case 1:
        btlfx_getUnitHomePos(arg0->unk0, sp18);
        break;
    case 2:
        sp18[0] = ((Unk800F78A4 *)arg1)->unkC->unk38;
        sp18[1] = ((Unk800F78A4 *)arg1)->unkC->unk3A;
        sp18[2] = ((Unk800F78A4 *)arg1)->unkC->unk3C;
        break;
    default:
        sp18[0] = 0;
        sp18[1] = 0;
        sp18[2] = 0;
        break;
    }
    if (n == 0) {
        v = arg0->unk4;
        if (v == 1) {
            btlfx_setUnitPos(arg0->unk0, sp18);
            D_800FEC55 = v;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(arg0->unk0, sp18, n + 1, arg0->unk4, 0, arg0->unk8) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D974C(Unk800D7C9C *arg0, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s32 m;
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitPos(arg0->unk0, a);
        btlfx_getStrikePos(arg0->unk0, arg0->unk1, b);
        if (a[1] == 0) {
            if (b[1] != 0) {
                b[1] = 0;
            }
        } else {
            b[1] = a[1];
        }
        btlfx_aimAngles(b, a, c);
        func_800CFD78(b, c, arg0->unk4, arg0->unk14);
        arg0->unk14[0] -= a[0];
        arg0->unk14[1] -= a[1];
        arg0->unk14[2] -= a[2];
        arg0->unk14[0] *= arg0->unk2;
        arg0->unk14[1] *= arg0->unk2;
        arg0->unk14[2] *= arg0->unk2;
        arg0->unk14[0] /= 10;
        arg0->unk14[1] /= 10;
        arg0->unk14[2] /= 10;
        arg0->unk14[0] += a[0];
        arg0->unk14[1] += a[1];
        arg0->unk14[2] += a[2];
        m = arg0->unk8;
        if (m == 1) {
            btlfx_setUnitPos(arg0->unk0, arg0->unk14);
            D_800FEC55 = m;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(arg0->unk0, arg0->unk14, v + 1, arg0->unk8, 0, arg0->unkC) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepRunToTarget(Unk800D7C9C *arg0, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s32 m;
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitPos(arg0->unk0, a);
        btlfx_getStrikePos(arg0->unk0, arg0->unk1, b);
        if (a[1] == 0) {
            if (b[1] != 0) {
                b[1] = 0;
            }
        } else {
            b[1] = a[1];
        }
        btlfx_aimAngles(b, a, c);
        func_800CFD78(b, c, arg0->unk4, arg0->unk14);
        m = arg0->unk8;
        if (m == 1) {
            btlfx_setUnitPos(arg0->unk0, arg0->unk14);
            D_800FEC55 = m;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(arg0->unk0, arg0->unk14, v + 1, arg0->unk8, 0, arg0->unkC) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepFaceTarget(Unk800D805C *arg0, s32 arg1, s32 *arg2) {
    s16 pos[4];
    s16 rot[4];
    s16 tgt[4];
    s32 v = *arg2;

    if (arg0->unk3 == 0 || btlfx_isPartyUnit(arg0->unk1) == 0) {
        btl_getUnit(arg0->unk0);
        if (v == 0) {
            btlfx_getUnitPos(arg0->unk0, pos);
            if (arg0->unk1 == 0xFF) {
                tgt[0] = ((Unk800F78A4 *)arg1)->unkC->unk38;
                tgt[1] = ((Unk800F78A4 *)arg1)->unkC->unk3A;
                tgt[2] = ((Unk800F78A4 *)arg1)->unkC->unk3C;
            } else {
                btlfx_getUnitHomePos(arg0->unk1, tgt);
            }
            btlfx_aimAngles(pos, tgt, arg0->unkC);
            arg0->unkC[0] = 0;
            if (arg0->unk2 == 1) {
                arg0->unkC[1] += 0x800;
            }
        }
        btlfx_getUnitRot(arg0->unk0, rot);
        rot[0] += arg0->unkC[0] / arg0->unk4;
        rot[1] += arg0->unkC[1] / arg0->unk4;
        rot[2] += arg0->unkC[2] / arg0->unk4;
        btlfx_setUnitRot(arg0->unk0, rot);
        if (v != arg0->unk4 - 1) {
            goto end;
        }
        D_800FEC56 = 1;
        return 1;
    }
    if (arg0->unk1 == 0xFF) {
        tgt[0] = ((Unk800F78A4 *)arg1)->unkC->unk38;
        tgt[1] = ((Unk800F78A4 *)arg1)->unkC->unk3A;
        tgt[2] = ((Unk800F78A4 *)arg1)->unkC->unk3C;
    } else {
        btlfx_getUnitHomePos(arg0->unk1, tgt);
    }
    btlfx_getUnitRot(arg0->unk0, rot);
    rot[1] = 0x800;
    btlfx_setUnitRot(arg0->unk0, rot);
    D_800FEC56 = 1;
    return 1;
end:
    return 0;
}

s32 btlfx_stepFaceHome(Unk800D8124 *arg0, s32 arg1, s32 *arg2) {
    s16 rot[4];
    BtlUnit *u;
    s32 v = *arg2;
    BtlUnitA0 *a;

    u = btl_getUnit(arg0->unk0);
    if (arg0->unk2 == 0 || btlfx_isPartyUnit(arg0->unk1) == 0) {
        if (v == 0) {
            arg0->unk8[0] = u->unkA0->unk20 - u->rot[0];
            arg0->unk8[1] = u->unkA0->unk22 - u->rot[1];
            arg0->unk8[2] = u->unkA0->unk24 - u->rot[2];
        }
        btlfx_getUnitRot(arg0->unk0, rot);
        rot[0] += arg0->unk8[0] / arg0->unk4;
        rot[1] += arg0->unk8[1] / arg0->unk4;
        rot[2] += arg0->unk8[2] / arg0->unk4;
        btlfx_setUnitRot(arg0->unk0, rot);
        if (v != arg0->unk4 - 1) {
            goto end;
        }
        D_800FEC56 = 1;
        return 1;
    }
    u->rot[0] = u->unkA0->unk20;
    a = u->unkA0;
    u->rot[1] = a->unk22;
    *(volatile s16 *)&u->rot[2] = a->unk24;
    return D_800FEC56 = 1;
end:
    return 0;
}

s32 func_800D9F5C(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_turnUnit(arg0[0], arg0[2]);
    return 1;
}

s32 func_800D9F88(u8 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == func_800DA06C(arg0[0], arg0[2]) - 1) {
        D_800FEC57 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D9FD8(u8 *arg0, s32 arg1, s32 *arg2) {
    if (*arg2 == arg0[2] - 1) {
        D_800FEC57 = 1;
        return 1;
    }
    return 0;
}

s32 func_800DA004(u8 *arg0, s32 arg1, s32 *arg2) {
    if (*arg2 == arg0[2] - 1) {
        D_800FEC57 = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepCallWeaponScript(u8 *arg0, s32 arg1, s32 arg2) {
    s32 v = func_800DA0B8(*arg0);
    D_80100D8C = v;
    btlfx_callScript(v);
    return 1;
}

s32 func_800DA064(u8 *arg0, s32 arg1, s32 arg2) {
    return 1;
}

s32 func_800DA06C(s32 arg0, s32 arg1) {
    s32 i = func_800D8B20(arg0);
    return D_800F73A0[arg1][i];
}

s32 func_800DA0B8(s32 arg0) {
    s32 v = btlfx_getUnitWeapon(arg0);
    if (v >= 0x55) {
        v = 0;
    }
    return D_800F7460[v];
}
