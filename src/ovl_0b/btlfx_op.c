#include "ovl_0b.h"

void btlfx_initCommonData(void) {
    u8 buf[0x40];
    RECT r;
    g_btlfxCommonTable = D_800F78B4;
    D_80104DE8 = 1;
    g_btlfxEnv->prim = buf;
    btlfx_reloadCommonData(0);
    while (btlfx_reloadCommonData(1) == 0) {
    }
    r.x = 0;
    r.y = 0xF0;
    r.w = 0x100;
    r.h = 2;
    LoadImage(&r, (u32 *)0x801F7700);
    DrawSync(0);
    g_btlfxEnv->prim = 0;
}

s32 btlfx_reloadCommonData(s32 arg0) {
    RECT r;
    s32 ret = 0;
    s32 i;
    u32 *p = (u32 *)0x801E7700;
    if (arg0 == 0) {
        FileEntry *e = file_find(1, 0x2BF);
        cd_readAsync(g_gameState->unk20 + e->unk4, (e->unkC - e->unk4) << 11, (void *)0x801E7700, 0);
    } else if (cd_getState() == 0) {
        btlfx_resetMaskBit();
        r.w = 0x40;
        r.h = 0x80;
        for (i = 0; i < 4; i++) {
            r.x = (i >> 1) * 64 + 0x300;
            r.y = (i & 1) * 128 + 0x100;
            LoadImage(&r, p);
            p += 0x1000;
        }
        ret = 1;
    }
    return ret;
}

s32 btlfx_stepBack(s32 arg0, s32 arg1, s32 arg2) {
    s32 a[2];
    s32 b[2];
    s32 c[2];
    s32 d[2];
    s32 n;
    s32 t;
    btlfx_getUnitHomePos(arg0, (s16 *)a);
    c[0] = a[0];
    c[1] = a[1];
    if (arg1 == 0xFF) {
        arg1 = 0;
    }
    if (btlfx_isEnemyUnit(arg1) != 0) {
        btlfx_getUnitRot(arg0, (s16 *)d);
        btlfx_pointAhead((s16 *)c, (s16 *)d, 400, (s16 *)b);
        if (arg2 == 0) {
            btlfx_playUnitMotion(arg0, 0x1D);
        }
        n = btlfx_getMotionLength(arg0);
        if (arg2 < n) {
            btlfx_applyFloatHeight(arg0, (s16 *)a);
            btlfx_applyFloatHeight(arg0, (s16 *)b);
        }
        t = ((arg2 + 1) << 12) / n;
        if (t > 0x1000) {
            t = 0x1000;
        }
        btlfx_lerpVec((s16 *)a, (s16 *)b, t, (s16 *)c);
        btlfx_setUnitPos(arg0, (s16 *)c);
        return arg2 >= n;
    }
    return 1;
}

void func_800E436C(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    btlfx_getMotionLength(arg0);
    btlfx_turnUnitToward(arg0, arg1, (s16 *)arg2, 2, 0, arg3, 3);
}

s32 btlfx_moveUnitHome(s32 arg0, s32 arg1, s16 *arg2, s32 arg3) {
    s16 a[4];
    s16 b[4];
    s32 n;
    s32 t;
    btlfx_getUnitHomePos(arg0, a);
    if (arg1 == 0) {
        btlfx_getUnitPos(arg0, arg2);
        if (arg2[0] == a[0] && arg2[2] == a[2]) {
            return -1;
        }
        if (arg3 == 0) {
            btlfx_playUnitMotion(arg0, 0x1E);
        }
        btlfx_turnUnitToward(arg0, a, 0, 0, 0, 0, 0);
    }
    if (arg3 == 0) {
        arg3 = btlfx_getMotionLength(arg0);
    } else {
        arg3 = 0x18;
    }
    n = arg3;
    if (arg1 < n) {
        btlfx_applyFloatHeight(arg0, arg2);
        btlfx_applyFloatHeight(arg0, a);
    }
    t = ((arg1 + 1) << 12) / n;
    if (t > 0x1000) {
        t = 0x1000;
    }
    btlfx_lerpVec(arg2, a, t, b);
    btlfx_setUnitPos(arg0, b);
    return arg1 >= n;
}

u16 *func_800E4530(s32 arg0) {
    s32 *p = (s32 *)0x801E9700;
    return (u16 *)(p[arg0] + (s32)p);
}

s32 btlfx_getCommonData(s32 arg0) {
    return (s32)g_btlfxCommonTable + g_btlfxCommonTable[arg0];
}

s32 btlfx_opSetResArgsA(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    switch (arg0) {
    case 1:
        g_btlfxResArgs.unk0 = arg4[0];
        g_btlfxResArgs.unk1 = arg4[4];
        break;
    }
    return 1;
}

s32 btlfx_opSetResArgsB(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    if (arg0 == 1) {
        g_btlfxResArgs.unk2 = arg4[0];
        g_btlfxResArgs.unk3 = arg4[4];
    }
    return 1;
}

s32 btlfx_opSetFrameRate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4) {
    switch (arg0) {
    case 1:
        btl_setVsyncMode(60 / *arg4);
        break;
    }
    return 1;
}

s32 func_800E4634(s32 arg0, Unk800E4634 *c, Unk800E4634Arg2 *arg2, s32 *arg3, s32 *arg4) {
    s32 t;
    s32 i;

    if (arg0 == 0) {
        *arg4 = 0x40;
        return 0;
    }
    if (arg0 == 1) {
        c->unk0 = 0;
        c->unk4 = 0;
        if (arg2->unkC->unk27 == 0) {
            c->unk4 = 0xFFFF;
            if (btlfx_isEnemyUnit(0x10) != 0) {
                c->unk0 = 0;
            } else {
                c->unk0 = 12;
            }
        }
        btlfx_playBounceCamera();
        g_btlfxState.unk10.b[2] = 0;
        {
            s32 j;
            for (j = 3; j >= 0; j--) {
                c->unk30[j] = NULL;
            }
        }
        return 0;
    }
    t = *arg3;
    if (c->unk4 == 0xFFFF) {
        if (--c->unk0 < 0) {
            g_btlfxState.unk10.b[2] = 1;
            return 1;
        }
    } else if (c->unk4 != 0) {
        c->unk4++;
        if ((c->unk4 >= 5 && c->unk0 >= 0) || (c->unk4 >= 15 && c->unk0 < 0)) {
            c->unk4 = 0;
            if (c->unk0 < 0) {
                g_btlfxState.unk10.b[2] = 1;
                g_btlfxState.unk25 = 1;
                return 1;
            }
        }
        for (i = 0; i < 4; i++) {
            if (c->unk30[i] != NULL) {
                c->unk30[i]->unk5C.vx = c->unk28[0];
                c->unk30[i]->unk5C.vy = c->unk28[1];
                c->unk30[i]->unk5C.vz = c->unk28[2];
                if (c->unk30[i]->unk30 == -1) {
                    c->unk30[i] = NULL;
                }
            }
        }
    } else if (btlfx_getBouncePos(c->unk0, (s16 *)&c->unk8[c->unk0], c->unk28) == 0) {
        c->unk30[c->unk0] = btlfx_startEmitter(btlfx_getCommonData(3), (s32)&c->unk8[c->unk0]);
        if (c->unk30[c->unk0] != NULL) {
            c->unk30[c->unk0]->unk22 = 0x73;
            if (c->unk30[c->unk0] != NULL) {
                c->unk30[c->unk0]->unk18[0].unk1C = btlfx_getCommonData(7);
                if (c->unk30[c->unk0] != NULL) {
                    c->unk30[c->unk0]->unk18[1].unk1C = btlfx_getCommonData(6);
                }
            }
        }
        c->unk28[0] = 0x400;
        c->unk4 = 1;
        c->unk28[1] += 0x800;
        btlfx_playSoundSet(3, c->unk0 + 8);
        c->unk0++;
    } else {
        c->unk0 = -1;
        c->unk4 = 1;
    }
    *arg3 = t;
    return 0;
}

void btlfx_startCasterTask(s32 arg0) {
    s32 a = 0xFF;
    s32 b = arg0 * 16;
    if (g_btlfxEnv->targets->targetCount == 1) {
        a = 0;
    }
    btlfx_spawnTask(7, -1, a, b);
}

void btlfx_playSoundSet(s32 arg0, s32 arg1) {
    s32 i;
    for (i = 0; i < 3; i++) {
        btlfx_playCommonSound(D_800F8CF4[arg0][i], arg1);
    }
}

Pool6CEntry *func_800E4A3C_def(s32 arg0, s32 arg1, Unk800E4A3CArgs arg2) {
    Pool6CEntry *e;
    Unk800E4A3CArgs *p;

    e = NULL;
    arg2.b = arg2.a;
    /* Read back through a pointer: CSE then keeps the load, which reload_cse turns into the original's
       `move v0,a2` for the 0x30 store (a direct `arg2.c = arg2.b` stores a2 twice). */
    p = &arg2;
    arg2.c = p->b;
    switch (arg1) {
    case 0:
    case 2:
        e = btlfx_startEmitter(btlfx_getCommonData(5), (s32)arg2.a);
        if (arg1 == 0) {
            if (e != NULL) {
                e->unk18[0].unk1C = btlfx_getCommonData(7);
                goto common;
            }
        } else {
            if (e != NULL) {
                e->unk18[0].unk1C = btlfx_getCommonData(8);
            common:
                if (e != NULL) {
                    e->unk18[1].unk1C = btlfx_getCommonData(6);
                    e->unk22 = 8;
                }
            }
        }
        if (btlfx_isUnitKind(0x10, 0) == 0) {
            goto zero;
        }
        btlfx_playSoundSet(1, arg0);
        break;
    case 1:
    case 7:
        arg2.b[1] -= 0x40;
        e = btlfx_startEmitter(btlfx_getCommonData(1), (s32)arg2.a);
        if (e != NULL) {
            e->unk18[0].unk1C = btlfx_getCommonData(0x14);
            e->unk18[1].unk1C = btlfx_getCommonData(6);
            e->unk18[2].unk1C = btlfx_getCommonData(2);
            e->unk2C = *(s32 *)&arg2.b;
            e->unk22 = 0x20;
        }
        btlfx_playSoundSet(2, arg0);
        break;
    case 4:
        e = btlfx_startEmitter(btlfx_getCommonData(4), (s32)arg2.a);
        if (e != NULL) {
            e->unk18[0].unk1C = btlfx_getCommonData(7);
            e->unk18[1].unk1C = btlfx_getCommonData(6);
            e->unk22 = 0x80;
        }
    zero:
        btlfx_playSoundSet(0, arg0);
        break;
    }
    return e;
}

void btlfx_setPartyTexAnim(s32 arg0, s32 arg1, s32 arg2) {
    if (btlfx_isEnemyUnit(arg0) == 0) {
        btlfx_setUnitTexAnim(arg0, arg1, arg2);
    }
}

void func_800E4C54(void) {
    if (g_btlfxEnv->targets->bounceCount == 0) {
        D_800FEC65 = 1;
    }
}

s32 btlfx_opCast(s32 arg0, Unk800E4C88 *arg1, Unk800F78A4 *arg2, s32 *arg3, s32 *arg4) {
    s16 v[4];
    s32 t;
    s32 a;
    s32 b;
    s32 *q;
    s32 k;
    s32 c;

    if (arg0 == 0) {
        *arg4 = 0x28;
    } else if (arg0 == 1) {
        q = arg4;
        arg1->unk1 = 0;
        arg1->unk6 = 0;
        arg1->unk2 = (q[1] & 0xF) + 0x10;
        arg1->unk3 = q[0];
        arg1->unk24 = NULL;
        if (arg2->unk0->unk1C & 8) {
            func_800E8C84(0);
        }
        if (arg2->targets->bounceCount != 0 && arg1->unk3 != 0xFF) {
            if (arg2->targets->bounceCount == 1) {
                arg1->unk3 += 8;
            } else {
                arg1->unk3 = 0xFF;
            }
        }
        if (arg2->targets->kind & 4) {
            arg1->unk2 = 0x11;
            arg1->unk3 = 0x10;
        }
        arg1->unk0 = (u8)q[1] >> 4;
        arg1->unk4 = ((u32)g_btlfxState.unk1C & 0x3000) >> 12;
        g_btlfxState.unk10.b[2] = 0;
        g_btlfxState.unk10.b[3] = 0;
        a = arg2->targets->kind & 1;
        c = btlfx_isEnemyUnit(arg1->unk2);
        arg1->unk8 = 100;
        btlfx_setPartyTexAnim(arg1->unk2, 1, 0);
        if ((a != 0 || arg1->unk0 == 0xF) && c == 0) {
            arg1->unk5 = 2;
            arg1->unk1 = 2;
        } else if (c != 0) {
            arg1->unk5 = 1;
            arg1->unk1 = 2;
            if (a == 0 && arg1->unk0 != 0xF) {
                btlfx_getUnitBodyPos(arg1->unk2, 0, arg1->unkC);
                arg1->unk24 = func_800E4A3C(arg1->unk2, 4, arg1->unkC, arg1->unk14);
            } else {
                arg1->unk24 = NULL;
            }
        } else if (arg2->targets->kind & 2) {
            arg1->unk5 = 3;
            arg1->unk1 = 2;
        } else {
            arg1->unk5 = 0;
        }
    } else {
    t = *arg3;
    switch (arg1->unk1) {
    case 0:
        if (btlfx_stepBack(arg1->unk2, arg1->unk3, t) != 0) {
            b = arg1->unk5;
            if (b != 3) {
                btlfx_playUnitMotion(arg1->unk2, 0x1A);
                arg1->unk1 = 1;
                goto set_t;
            }
            btlfx_playUnitMotion(arg1->unk2, 0x1F);
            arg1->unk1 = b;
            btlfx_showCasterCmdName();
            goto set_t;
        }
        break;
    case 1:
        if (arg1->unk3 != 0xFF && (s16)D_800FEC42 != 0xED) {
            btlfx_getUnitPos(arg1->unk3, v);
            func_800E436C(arg1->unk2, v, (s32)arg1->unk1C, t, 0);
        }
        if (t >= btlfx_getMotionLength(arg1->unk2) - 1) {
            btlfx_playUnitMotion(arg1->unk2, 0x1B);
            t = -1;
            arg1->unk1 = 2;
            btlfx_getUnitPos(arg1->unk2, arg1->unkC);
            arg1->unk24 = func_800E4A3C(arg1->unk2, arg1->unk0, arg1->unkC, arg1->unk14);
            btlfx_setPartyTexAnim(arg1->unk2, 0, 0);
            btlfx_showCasterCmdName();
        }
        break;
    case 2:
        if (arg1->unk24 != NULL && arg1->unk24->unk30 == -1) {
            arg1->unk24 = NULL;
        }
        if (t == 0) {
            k = ((u32)g_btlfxState.unk1C & 0x300) >> 8;
            if (k != 3) {
                btlfx_startFadeBackground(k, arg1->unk0 == 1 ? 0xC : 0x14);
            }
        }
        b = t - 9;
        if (arg1->unk0 == 1) {
            if (b >= 0) {
                if (b < 16) {
                    a = (b * 255) >> 4;
                } else {
                    a = 255;
                }
                if (b == 0) {
                    arg1->unk8 = btlfx_getMotionLength(arg1->unk2);
                    if (arg2->targets->kind & 1) {
                        btlfx_showCasterCmdName();
                    }
                }
                switch (arg1->unk4) {
                case 0:
                    btlfx_drawScreenTint((u32 *)(arg2->ot + 1), 1, a, a, a);
                    if (b == 16) {
                        btlfx_fadeUnitGroup(2, 0);
                    }
                    break;
                case 1:
                    btlfx_drawScreenTint((u32 *)(arg2->ot + 1), 2, a, a, a);
                    if (b == 16) {
                        btlfx_fadeUnitGroup(2, 0);
                    }
                    break;
                case 2:
                    btlfx_fadeUnitGroup(2, 0x80 - (a >> 1));
                    break;
                case 3:
                    btlfx_fadeUnitGroup(0, 0x80 - (a >> 1));
                    break;
                }
            }
            if (t % arg1->unk8 == 0 && t >= 25 && arg1->unk6 >= 27 && g_btlfxState.unk10.b[2] != 0) {
                btlfx_stopEmitter(arg1->unk24);
                g_btlfxState.unk10.b[3] = 1;
                g_btlfxState.unk25 = 1;
                arg2->camera->unk52 = 0;
                btlfx_setPartyTexAnim(arg1->unk2, 2, 0);
                return 1;
            }
        } else if (arg1->unk5 == 0) {
            if (t % btlfx_getMotionLength(arg1->unk2) == 0 && t > 0 && arg1->unk6 >= 27 && g_btlfxState.unk10.b[2] != 0) {
                btlfx_playUnitMotion(arg1->unk2, 0x1C);
                t = -1;
                arg1->unk1 = 3;
            }
        } else if (arg1->unk5 == 1) {
            if (g_btlfxState.unk10.b[2] != 0 && arg1->unk6 >= 27) {
                arg1->unk1 = 3;
            set_t:
                t = -1;
            }
        } else if (arg1->unk5 == 3) {
            if (g_btlfxState.unk10.b[2] != 0) {
                arg1->unk1 = 0;
                goto set_t;
            }
        } else if (g_btlfxState.unk10.b[2] != 0) {
            g_btlfxState.unk10.b[3] = 1;
            func_800E4C54();
            arg2->camera->unk52 = 0;
            btlfx_setPartyTexAnim(arg1->unk2, 2, 0);
            btlfx_showCasterCmdName();
            return 1;
        }
        break;
    case 3:
        if (t == 0 && arg1->unk5 != 1) {
            arg2->camera->unk52 = 0;
            btlfx_setPartyTexAnim(arg1->unk2, 0, 1);
        }
        if (arg1->unk5 != 3) {
            if (arg1->unk24 != NULL) {
                if (arg1->unk24->unk30 == -1) {
                    arg1->unk24 = NULL;
                }
                if (arg1->unk24 != NULL && t < 5) {
                    k = (t << 12) >> 2;
                    arg1->unk24->unk20 = 0x1000 - k;
                    if (arg1->unk24 != NULL) {
                        arg1->unk24->unk1C = k * 2 + 0x1000;
                    }
                    if (t != 4) {
                        goto skip;
                    }
                    btlfx_stopEmitter(arg1->unk24);
                }
            }
            g_btlfxState.unk10.b[3] = 1;
            func_800E4C54();
        } else {
            g_btlfxState.unk10.b[3] = 1;
            if (arg1->unk3 != 0xFF) {
                btlfx_getUnitPos(arg1->unk3, v);
                func_800E436C(arg1->unk2, v, (s32)arg1->unk1C, t, 0);
            }
        }
    skip:
        if (arg1->unk5 == 1) {
            if (t >= 5) {
                return 1;
            }
            break;
        }
        if (t >= btlfx_getMotionLength(arg1->unk2) - 1) {
            if (arg1->unk5 == 0 || arg1->unk5 == 3) {
                btlfx_playUnitMotion(arg1->unk2, 9);
            }
            btlfx_setPartyTexAnim(arg1->unk2, 2, 0);
            g_btlfxState.unk10.b[3] = 1;
            func_800E4C54();
            return 1;
        }
        break;
    }
    *arg3 = t;
    arg1->unk6++;
    }
    return 0;
}

s32 btlfx_opReturnHome(s32 arg0, Unk800E5538 *arg1, Unk800F78A4 *arg2, s32 *arg3, s32 *arg4) {
    s32 t;

    if (arg0 == 0) {
        *arg4 = 0x28;
        return 0;
    }
    if (arg0 == 1) {
        arg1->unk1 = 0;
        arg1->unk2 = *arg4;
        if (arg2->targets->kind & 4) {
            arg1->unk2 = 0x11;
        }
        if (arg2->targets->kind & 1) {
            return 1;
        }
        if (btlfx_isEnemyUnit(arg1->unk2) != 0) {
            return 1;
        }
        arg1->unk3 = btl_hasStatus(btl_getUnit(arg1->unk2), BTL_STATUS_STOP);
        return 0;
    }
    t = *arg3;
    if (arg1->unk1 == 0) {
        if (btlfx_moveUnitHome(arg1->unk2, t, arg1->unkC, arg1->unk3) != 0) {
            t = -1;
            arg1->unk1 = 1;
            if (func_800E84F0(arg1->unk2) != 0) {
                btlfx_playUnitMotion(arg1->unk2, 9);
                return 1;
            }
            if (arg1->unk3 == 0) {
                btlfx_playUnitMotion(arg1->unk2, 0x20);
            }
        }
    } else {
        func_800E436C(arg1->unk2, NULL, (s32)arg1->unk1C, t, 1);
        if (t >= btlfx_getMotionLength(arg1->unk2) - 1) {
            btlfx_resetUnitMotion(arg1->unk2, 1);
            return 1;
        }
    }
    *arg3 = t;
    return 0;
}

s32 btlfx_opSetResArgsC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    switch (arg0) {
    case 1:
        g_btlfxResArgs2.unk0 = arg4[0];
        g_btlfxResArgs2.unk1 = arg4[4];
        break;
    }
    return 1;
}

s32 func_800E5714(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4) {
    s32 idx;
    s32 k;
    u16 *p;
    if (arg0 == 1) {
        idx = arg4[1] & 0x1F;
        k = (arg4[1] & 0xE0) >> 5;
        p = func_800E4530(arg4[0]);
        if (k >= 0) {
            if (k < 2) {
                p[1] = D_800F8CB4[idx];
            } else if (k < 4) {
                p[2] = D_800F8CB4[idx];
            }
        }
    }
    return 1;
}

s32 func_800E57B0(s32 arg0, Unk800E57B0 *arg1, Unk800F78A4 *arg2, s32 *arg3, s32 *arg4) {
    SVECTOR rot;
    s16 v[4];
    s32 a;
    s32 b;
    Pool6CEntry *e;
    u16 *p;

    if (arg0 == 0) {
        *arg4 = 0x2C;
    } else if (arg0 == 1) {
        a = arg4[0];
        b = arg4[1];
        arg1->unk0 = g_btlfxResArgs.unk0 >> 6;
        arg1->unk4 = D_800F8D04[g_btlfxResArgs.unk0 & 0xF];
        arg1->unk1 = g_btlfxResArgs2.unk0;
        arg1->unk2 = g_btlfxResArgs2.unk1 * 8;
        arg1->unk14 = 0;
        if (arg1->unk1 != 0xFF) {
            btlfx_getUnitBodyPos(arg1->unk1, 0, &arg1->unk1C.vx);
        } else {
            *(s32 *)&arg1->unk1C.vx = *(s32 *)&arg2->targets->centerX;
            *(s32 *)&arg1->unk1C.vz = *(s32 *)&arg2->targets->centerZ;
            arg1->unk1C.vy -= 0xC0;
        }
        switch (arg1->unk0) {
        case 0:
            arg1->unk8 = g_btlfxResArgs.unk1;
            arg1->unkC = a;
            arg1->unk6 = (s8)b << 8;
            arg1->unk10 = func_800E4530(arg1->unk8);
            ((u16 *)arg1->unk10)[1] = D_800F8CB4[arg1->unkC];
            arg1->unkE = *(u16 *)arg1->unk10 & 0x7FFF;
            arg1->unk18 = (s8)g_btlfxResArgs.unk2 * 4;
            arg1->unk1A = (s8)g_btlfxResArgs.unk3 * 4;
            break;
        case 1:
            arg1->unk8 = g_btlfxResArgs.unk1;
            arg1->unkC = a;
            arg1->unk6 = (s8)b << 8;
            arg1->unk10 = func_800E4530(arg1->unk8);
            ((u16 *)arg1->unk10)[2] = D_800F8CB4[arg1->unkC];
            arg1->unkE = *(u16 *)arg1->unk10 & 0x7FFF;
            break;
        case 2:
            arg1->unkA = g_btlfxResArgs.unk1;
            arg1->unk8 = a;
            arg1->unkE = g_btlfxResArgs.unk0 & 0xF;
            arg1->unk4 = b;
            break;
        case 3:
            arg1->unk8 = a;
            arg1->unkE = g_btlfxResArgs.unk1;
            arg1->unk10 = func_800E4530(arg1->unk8);
            arg1->unk6 = 0;
            break;
        default:
            return 0;
        }
    } else {
        if (arg1->unk1 != 0xFF) {
            arg2->unk28 = func_800E7CC4(arg1->unk1, arg1->unk2);
            rot.vy = btlfx_compareDepth(0x10, arg1->unk1) << 11;
            rot.pad = 0;
            rot.vx = 0;
            rot.vz = arg1->unk6;
        }
        switch (arg1->unk0) {
        case 0:
            btlfx_drawParticles(arg1->unk10, &arg1->unk1C, &rot, arg1->unk4, arg1->unk14, 1, 0x1000, arg1->unk18, arg1->unk1A);
            break;
        case 1:
            btlfx_drawAnimQuads(arg1->unk10, &arg1->unk1C, &rot, arg1->unk4, arg1->unk14, 1, 0x1000);
            break;
        case 2:
            if (arg1->unk14 == arg1->unkE) {
                if (arg1->unk1 != 0xFF) {
                    if (arg1->unk1 == 0x10) {
                        btlfx_getWeaponPos(arg1->unk1, 1, &arg1->unk1C.vx);
                        btlfx_getUnitBodyPos(0, 0, v);
                        btlfx_aimAngles(&arg1->unk1C.vx, v, arg1->unk24);
                        if (btl_getUnit(0) == btl_getUnit(arg1->unk1) && arg1->unkA == 0x36) {
                            return 1;
                        }
                    } else {
                        btlfx_getUnitBodyPos(arg1->unk1, 0, &arg1->unk1C.vx);
                    }
                }
                e = btlfx_startEmitter((s32)func_800E4530(arg1->unkA), (s32)&arg1->unk1C);
                if (e != NULL) {
                    e->unk22 = arg1->unk2;
                }
                p = func_800E4530(arg1->unk8);
                if (e != NULL) {
                    e->unk18->unk1C = (s32)p;
                    e->unk18->unk0 = arg1->unk4;
                }
                e->unk18->unk8 = *p & 0x7FFF; /* outside the NULL check in the original */
                arg1->unk10 = e;
            }
            if (arg1->unkE < arg1->unk14) {
                e = arg1->unk10;
                if (arg1->unk1 == 0x10 && e != NULL) {
                    e->unk5C.vx = arg1->unk24[0];
                    e->unk5C.vy = arg1->unk24[1];
                    e->unk5C.vz = arg1->unk24[2];
                }
                if (e->unk30 == -1) {
                    return 1;
                }
            }
            goto next;
        case 3:
            btlfx_drawRings(arg1->unk10, &arg1->unk1C, &rot, arg1->unk4, arg1->unk14, 1, math_cos((arg1->unk14 << 10) / arg1->unkE));
            break;
        default:
            arg2->unk28 = 0;
            goto inc;
        }
        if (arg1->unk14 >= arg1->unkE - 1) {
            return 1;
        }
    next:
        arg2->unk28 = 0;
    inc:
        arg1->unk14++;
    }
    return 0;
}

void func_800E5D48(s8 arg0, s8 arg1, s8 arg2, s32 arg3, s32 arg4, s32 arg5) {
    g_btlfxResArgs2.unk0 = arg0;
    g_btlfxResArgs2.unk1 = arg1;
    g_btlfxResArgs.unk1 = arg2;
    g_btlfxResArgs.unk0 = arg5 | 0x80;
    btlfx_spawnTask(1, g_btlfxEnv->unk0->unk8, arg3, arg4);
}

s32 btlfx_opApplyResult(s32 arg0, void *arg1, Unk800F78A4 *arg2, void *arg3, s32 *arg4) {
    s32 id;
    s32 *a;

    if (arg0 == 1) {
        a = arg4;
        id = a[0];
        if (id == 0xFF) {
            id = -1;
        } else if (id == 0xFE) {
            id = btlfx_rand() % g_btlfxEnv->targets->targetCount;
        }
        if (a[1] == 0) {
            btlfx_execCmd(id);
            return 1;
        }
        if (a[1] == 1) {
            if (D_80100D8C == 0x16D || (u32)(D_800FEC42 - 0x6B) >= 2 || id != 0x10) {
                btlfx_showPopups(id);
            }
            return 1;
        }
    }
    return 1;
}

s32 btlfx_opAddCameraChoice(s32 arg0, s32 arg1, s32 arg2, Unk800F78A40 *arg3, s32 *arg4) {
    s32 v;
    Unk800FEC40 *p;
    s16 *h;

    if (arg0 == 1) {
        v = arg4[0];
        if (v == 0xFF) {
            h = g_btlfxState.unkA.h;
            h[0] = 0;
            h[1] = 0;
        } else {
            p = &g_btlfxState;
            if (p->unkA.b.unk3 < 3) {
                p->unkA.b.unk0[p->unkA.b.unk3] = v;
                p->unkA.b.unk3++;
            }
        }
    }
    return 1;
}

s32 btlfx_opPlayCamera(s32 arg0, s32 arg1, s32 arg2, Unk800F78A40 *arg3, s32 *arg4) {
    s32 r;
    s32 k;
    if (arg0 == 1) {
        r = 0;
        switch (arg4[1]) {
        case 0:
            r = btlfx_getFileData(arg3->unk8, arg4[0]);
            break;
        case 1:
            r = btlfx_getFileData(arg3->unk8, g_btlfxState.unkA.b.unk0[g_btlfxState.unkE]);
            break;
        case 2:
            if (g_btlfxState.unkA.b.unk3 != 0) {
                g_btlfxState.unkE = btlfx_rand() % g_btlfxState.unkA.b.unk3;
            }
            r = btlfx_getFileData(arg3->unk8, g_btlfxState.unkA.b.unk0[g_btlfxState.unkE]);
            break;
        case 3:
            k = g_btlfxEnv->camera->unk53;
            if (k >= g_btlfxState.unkA.b.unk3) {
                k = g_btlfxState.unkA.b.unk3 - 1;
            }
            r = btlfx_getFileData(arg3->unk8, g_btlfxState.unkA.b.unk0[k]);
            break;
        }
        if (g_btlfxCameraOn != 0) {
            btlcam_play(r);
        }
    }
    return 1;
}

void btlfx_startFadeBackground(s32 arg0, s32 arg1) {
    s32 v;
    if (arg0 < 0) {
        v = 0x80;
    } else {
        v = D_800F8D0C[arg0];
    }
    btlfx_spawnTask(0xA, -1, v, arg1);
}

s32 btlfx_opFadeBackground(s32 arg0, Unk800E6270 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 n;
    s32 v;
    if (arg0 == 0) {
        *arg4 = 8;
        return 0;
    }
    if (arg0 == 1) {
        arg1->unk4 = btlbg_getBrightness() & 0xFF;
        arg1->unk6 = arg4[0];
        if (arg1->unk6 == 0xFF) {
            arg1->unk6 = 0x80;
        }
        arg1->unk6 -= arg1->unk4;
        if ((arg1->unk0 = arg4[1]) == 0 || arg1->unk6 == 0) {
            btlbg_setBrightness(arg1->unk4 + arg1->unk6);
            return 1;
        }
        return 0;
    }
    n = *arg3;
    v = arg1->unk4 + n * arg1->unk6 / arg1->unk0;
    if (v < 0x10) {
        v = 0;
    }
    btlbg_setBrightness(v);
    if (n < arg1->unk0) {
        *arg3 = n;
        return 0;
    }
    return 1;
}

void btlfx_startFadeUnits(s32 arg0, s32 arg1) {
    btlfx_spawnTask(4, -1, arg0 & 0xFF, arg1 & 0xFF);
}

s32 btlfx_opFadeUnits(s32 arg0, Unk800E6270 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s16 v;
    s32 n;
    if (arg0 == 0) {
        *arg4 = 0xC;
        return 0;
    }
    if (arg0 == 1) {
        if (arg4[1] & 4) {
            arg1->unk4 = 0x80;
            v = -0x80;
        } else {
            arg1->unk4 = 0;
            v = 0x80;
        }
        arg1->unk6 = v;
        arg1->unk8 = arg4[1] & 0xB;
        if ((arg1->unk0 = arg4[0]) != 0) {
            return 0;
        }
        btlfx_fadeUnitGroup(arg1->unk8, arg1->unk4 + arg1->unk6);
        return 1;
    }
    n = *arg3;
    btlfx_fadeUnitGroup(arg1->unk8, arg1->unk4 + n * arg1->unk6 / arg1->unk0);
    if (n < arg1->unk0) {
        *arg3 = n;
        return 0;
    }
    return 1;
}

s32 btlfx_opPlaySound(s32 arg0, s32 arg1, s32 arg2, Unk800F78A40 *arg3, s32 *arg4) {
    if (arg0 == 1) {
        s32 v = -1;
        if (arg4[1] != 0xFF) {
            v = arg4[1];
        }
        btlfx_playFileSound(arg4[0], arg3->unk8, v);
    }
    return 1;
}

void btlfx_startScreenFade(s32 arg0, s32 arg1) {
    btlfx_spawnTask(0xE, -1, arg0, arg1);
}

s32 btlfx_opScreenFade(s32 arg0, Unk800E6410 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 n;
    s32 *p;
    if (arg0 == 0) {
        *arg4 = 0xC;
        return 0;
    }
    if (arg0 == 1) {
        p = arg4;
        if (p[0] == 2) {
            return 1;
        }
        if (p[0] == 1) {
            arg1->unk4 = 0;
        } else if (p[0] == 0) {
            arg1->unk4 = 0xFF;
        }
        if ((arg1->unk0 = p[1]) != 0) {
            return 0;
        }
        btlfx_drawScreenTint(g_btlfxEnv->ot + 1, 0xFF, arg1->unk4, arg1->unk4, arg1->unk4);
        return 1;
    }
    n = *arg3;
    btlfx_drawScreenTint(g_btlfxEnv->ot + 1, 0xFF, arg1->unk4, arg1->unk4, arg1->unk4);
    if (n < arg1->unk0) {
        *arg3 = n;
        return 0;
    }
    return 1;
}

void btlfx_sampleGradient(u8 *p, CVECTOR *out, s32 x) {
    Grad *g = (Grad *)p;
    s32 n = g->count;
    s32 i;
    s32 c;

    p += 8;
    if (g->total == 0) {
        i = n = 0; /* i: running start */
        p += 4;
    loop:
        c = p[-1];
        if (c != 0) {
            ((u16 *)p)[1] = i;
            i += c;
            n++;
            ((u16 *)p)[0] = c;
            p += 8;
            goto loop;
        }
        p = (u8 *)g + 8;
        g->total = i;
        g->count = n;
    }
    if (g->total < x) {
        x = g->total;
    }
    p += n * 8 - 8;
    for (i = 0; i < n; i++, p -= 8) {
        if (x >= ((u16 *)p)[3]) {
            break;
        }
    }
    c = x - ((u16 *)p)[3];
    c = (c << 12) / ((u16 *)p)[2];
    LoadAverageCol(p, p + 8, 0x1000 - c, c, (u8 *)out);
}

void btlfx_drawScrollMove(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    RECT b;
    RECT a;
    DR_MOVE *p;
    s32 m = arg0 % arg4;
    s32 h;
    p = (DR_MOVE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(p + 2);
    h = arg4 - m;
    if (m != 0) {
        a.x = arg5;
        a.y = arg6;
        a.w = arg3;
        a.h = m;
        SetDrawMove(p, &a, arg1, arg2 + h);
        AddPrim(&g_btlfxEnv->ot[0xFFF], p);
    }
    p++;
    if (h != 0) {
        b.x = arg5;
        b.y = arg6 + m;
        b.w = arg3;
        b.h = h;
        SetDrawMove(p, &b, arg1, arg2);
        AddPrim(&g_btlfxEnv->ot[0xFFF], p);
    }
}

void btlfx_drawScreenGrid(u32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   SVECTOR *arg9, VECTOR *arg10, CVECTOR *arg11, s32 arg12, void (*arg13)(SVECTOR *)) {
    XY32 buf[2][18];
    SVECTOR v[4];
    SVECTOR uv[4];
    MATRIX m;
    long ox;
    long oy;
    s32 cx;
    s32 cy;
    s32 tp0;
    s32 tp1;
    POLY_FT4 *p;
    s32 x;
    s32 y;
    s32 r;
    s32 c;
    s32 tp;
    s32 fl;
    s32 t;
    s32 sx;
    s32 sy;
    s32 ty;
    u32 len;

    ReadGeomOffset(&ox, &oy);
    p = (POLY_FT4 *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(p + arg5 * arg6);
    if (arg12 != 0xFF) {
        arg11->cd = 0x2E;
    } else {
        arg11->cd = 0x2C;
    }
    if (arg12 == 0xFF) {
        arg12 = 0;
    }
    tp0 = GetTPage(2, arg12, g_btlfxEnv->dispEnv->disp.x, g_btlfxEnv->dispEnv->disp.y);
    tp1 = GetTPage(2, arg12, g_btlfxEnv->dispEnv->disp.x + 0x80, g_btlfxEnv->dispEnv->disp.y) << 16;
    arg7 *= arg3;
    tp0 <<= 16;
    arg8 *= arg4;
    cx = arg1;
    cy = arg2;
    SetGeomOffset(arg1, arg2);
    func_800316D8(arg9, &m);
    arg10->vz = 0x1000;
    ScaleMatrix(&m, arg10);
    m.t[1] = 0;
    m.t[0] = 0;
    m.t[2] = ReadGeomScreen();
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    gte_ldvz012_0();
    for (y = 0, r = 0; r <= arg6; r++, y += arg4) {
        for (x = 0, c = 0; c <= arg5; c++, x += arg3) {
            ((XY32 *)&v[0])->w = (*buf)[c].w;
            sx = x - arg7;
            t = buf[1][c].w;
            uv[1].vx = (v[1].vx = sx) + cx;
            uv[1].vy = (v[1].vy = y - arg8) + cy;
            *(s32 *)&uv[0] = t;
            buf[1][c].w = *(s32 *)&uv[1];
            if (arg13 != NULL) {
                arg13(&v[1]);
            }
            buf[0][c].w = *(s32 *)&v[1];
            if (c != 0 && r != 0) {
                gte_ldxy3(&v[1], &v[0], &v[3]);
                *(u32 *)&p->r0 = *(u32 *)arg11;
                gte_rtpt_b();
                if (uv[0].vx < 0x100 && uv[1].vx < 0x100 && uv[2].vx < 0x100 && uv[3].vx < 0x100) {
                    fl = 0;
                    tp = tp0;
                } else {
                    fl = 1;
                    tp = tp1;
                    uv[0].vx -= 0x80;
                    uv[1].vx -= 0x80;
                    uv[2].vx -= 0x80;
                    uv[3].vx -= 0x80;
                }
                gte_stsxy3(&p->x0, &p->x1, &p->x2);
                gte_ldxy0(&v[2]);
                if (arg0 < 0x1000) {
                    len = 0x09000000;
                    addPrimLen(&g_btlfxEnv->ot[arg0], p, len);
                }
                gte_rtps_b();
                *(u32 *)&p->u0 = uv[1].vx | (uv[1].vy << 8);
                *(u32 *)&p->u1 = uv[0].vx | (uv[0].vy << 8) | tp;
                *(u32 *)&p->u2 = uv[3].vx | (uv[3].vy << 8);
                *(u32 *)&p->u3 = uv[2].vx | (uv[2].vy << 8);
                gte_stsxy(&p->x3);
                if (fl) {
                    uv[0].vx += 0x80;
                    uv[1].vx += 0x80;
                    uv[2].vx += 0x80;
                    uv[3].vx += 0x80;
                }
                p++;
            }
            *(s32 *)&v[2] = *(s32 *)&v[0];
            *(s32 *)&v[3] = *(s32 *)&v[1];
            *(s32 *)&uv[2] = *(s32 *)&uv[0];
            *(s32 *)&uv[3] = *(s32 *)&uv[1];
        }
    }
    SetGeomOffset(ox, oy);
}

void btlfx_pushViewRotation(SVECTOR *arg0) {
    MATRIX m;
    SVECTOR v;
    memset((s32)&v, 0, 8);
    if (arg0 == 0) {
        arg0 = &v;
    }
    func_800316D8(arg0, &m);
    m.t[1] = 0;
    m.t[0] = 0;
    m.t[2] = ReadGeomScreen();
    g_btlfxSavedView = *g_btlfxEnv->viewMatrix;
    *g_btlfxEnv->viewMatrix = m;
}

void btlfx_popViewMatrix(void) {
    *g_btlfxEnv->viewMatrix = g_btlfxSavedView;
}

void btlfx_pushGeomScreen(s32 arg0) {
    s32 *p = &g_btlfxSavedGeomScreen;
    *p = ReadGeomScreen();
    SetGeomScreen(arg0);
}

void btlfx_popGeomScreen(void) {
    SetGeomScreen(g_btlfxSavedGeomScreen);
}

void btlfx_drawBgOffscreen(s32 x, s32 y, s32 w, s32 h, u32 *ot, s32 arg5, s32 arg6, s32 arg7) {
    long ox;
    long oy;
    s32 save;
    s32 mode;
    DrawEnvPair *e;

    save = g_btlbgRainCount;
    e = (DrawEnvPair *)g_btlfxEnv->prim;
    g_btlbgRainCount = 0;
    g_btlfxEnv->prim = (u8 *)(e + 1);
    SetDefDrawEnv(&e->unk0, x, y, w, h);
    e->unk0.isbg = 1;
    e->unk0.dtd = 0;
    e->unk0.dfe = 0;
    e->unk0.r0 = 7;
    e->unk0.g0 = 7;
    e->unk0.b0 = 7;
    e->unk5C = *(DRAWENV *)&g_btlfxEnv->drawEnv;
    ClearOTag(ot, 0x100);
    mode = ReadGeomScreen();
    ReadGeomOffset(&ox, &oy);
    SetGeomScreen(0x200);
    SetGeomOffset(w >> 1, h >> 1);
    D_800F8D50.t[0] = -arg5;
    D_800F8D50.t[1] = arg6;
    D_800F8D50.t[2] = arg7 + 0x2900;
    btlbg_setLayerMask(2);
    g_btlfxEnv->prim = btlbg_draw(&D_800F8D50, g_gameState->unk1C->unk8E4.battle->unk11C0->unk1C, (s32)g_btlfxEnv->prim, NULL, (s32)ot);
    btlbg_setLayerMask(0xF);
    EnterCriticalSection();
    DrawOTagEnv(ot, &e->unk0);
    PutDrawEnv(&e->unk5C);
    ExitCriticalSection();
    g_btlbgRainCount = save;
    SetGeomScreen(mode);
    SetGeomOffset(ox, oy);
}

void btlfx_setTargets(BtlUnitList *arg0) {
    g_btlfxEnv->targets = arg0;
}

void btlfx_initEnv(void) {
    GameData *gd;
    g_btlfxEnv->unk28 = 0;
    gd = g_gameState->unk1C;
    g_btlfxEnv->ot = (s32 *)gd->unk10[gd->unk8];
    g_btlfxEnv->dispEnv = &g_gameState->unkC[g_gameState->unk1C->unk8];
    g_btlfxEnv->drawEnv = &g_gameState->unk10[g_gameState->unk1C->unk8];
    g_btlfxEnv->viewMatrix = &g_gameState->unk1C->unk18;
}

void btlfx_beginPrims(u8 *arg0, s32 *arg1) {
    Unk800F78A4 *p = g_btlfxEnv;
    GameState *g = g_gameState;
    p->unk28 = 0;
    p->ot = arg1;
    p->dispEnv = &g->unkC[g->unk1C->unk8];
    p->drawEnv = &g->unk10[g->unk1C->unk8];
    p->viewMatrix = &g->unk1C->unk18;
    p->prim = arg0;
}

u8 *btlfx_endPrims(void) {
    return g_btlfxEnv->prim;
}

BtlUnit *btl_getUnit(s32 arg0) {
    if (arg0 == 0x10) {
        return g_btlfxEnv->targets->caster;
    }
    if (arg0 == 0x11) {
        return g_btlfxEnv->targets->partner;
    }
    if (arg0 < 8) {
        if (arg0 < 0 || arg0 >= g_btlfxEnv->targets->targetCount) {
            SystemError(0x53, 0x136);
        }
        return g_btlfxEnv->targets->targets[arg0];
    }
    if (arg0 - 8 < 0 || arg0 - 8 >= g_btlfxEnv->targets->bounceCount) {
        SystemError(0x53, 0x136);
    }
    return g_btlfxEnv->targets->bounced[arg0 - 8];
}

s32 btl_isModelPart(BtlUnit *arg0) {
    return (arg0->unk68 & 0x800001) == 0x800000;
}

void btlfx_clearTargets(void) {
    g_btlfxEnv->targets = NULL;
}

s32 btlfx_getUnitRadius(s32 arg0) {
    s32 flag;
    BtlUnit *u;
    u32 f;
    if (arg0 & 0x100) {
        arg0 &= 0xFF;
        flag = 1;
    } else {
        flag = 0;
    }
    u = btl_getUnit(arg0);
    f = u->unk68;
    if ((f & 1) || flag != 0) {
        return u->model->unk8->unkA;
    }
    return g_gameState->unk1C->unk8E4.battle->enemies[(f >> 1) & 0xF].unk0->unk24;
}

s32 btl_getUnitHeight(BtlUnit *arg0) {
    return arg0->model->unk8->unk8;
}

s32 btlfx_getUnitHeight(s32 arg0) {
    return btl_getUnitHeight(btl_getUnit(arg0));
}

void btl_getBaseBonePos(BtlUnit *arg0, u16 *arg1) {
    BtlBone *b = &arg0->model->unk20[arg0->unk7C];
    arg1[0] = b->unk14;
    arg1[1] = b->unk18;
    arg1[2] = b->unk1C;
}

void btlfx_getUnitHomePos(s32 arg0, s16 *arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (btl_isModelPart(u) != 0) {
        arg1[0] = u->pos[0] >> 16;
        arg1[1] = u->pos[1] >> 16;
        arg1[2] = u->pos[2] >> 16;
    } else {
        arg1[0] = u->unk98->unk0[0] >> 16;
        arg1[1] = u->unk98->unk0[1] >> 16;
        arg1[2] = u->unk98->unk0[2] >> 16;
    }
}

void btl_getUnitPos(BtlUnit *arg0, s16 *arg1) {
    if (btl_isModelPart(arg0) != 0 || !(arg0->unk68 & 1)) {
        btl_getBaseBonePos(arg0, (u16 *)arg1);
        arg1[1] = 0;
    } else {
        arg1[0] = arg0->pos[0] >> 16;
        arg1[1] = arg0->pos[1] >> 16;
        arg1[2] = arg0->pos[2] >> 16;
    }
}

void btlfx_getUnitPos(s32 arg0, s16 *arg1) {
    btl_getUnitPos(btl_getUnit(arg0), arg1);
}

void btl_getUnitBodyPos(BtlUnit *arg0, s32 arg1, s16 *arg2) {
    s32 h;
    if (btl_isModelPart(arg0) != 0) {
        btl_getBaseBonePos(arg0, (u16 *)arg2);
        return;
    }
    btl_getUnitPos(arg0, arg2);
    if (arg0->unk68 & 1) {
        h = btl_getUnitHeight(arg0);
        if (arg1 == 0) {
            h >>= 1;
        }
        if ((u8)btl_hasStatus(arg0, BTL_STATUS_FLOAT)) {
            h -= 0x80;
        }
        arg2[1] -= h;
    } else {
        btl_getBaseBonePos(arg0, (u16 *)arg2);
        if (arg1 == 1) {
            arg2[1] = arg0->model->unk20[g_gameState->unk1C->unk8E4.battle->enemies[(arg0->unk68 >> 1) & 0xF].unk0->unk29[2]].unk18;
        }
    }
}

void btlfx_applyFloatHeight(s32 arg0, s16 *arg1) {
    if (btl_hasStatus(btl_getUnit(arg0), BTL_STATUS_FLOAT) & 0xFF) {
        arg1[1] -= 0x80;
    }
}

void btlfx_getUnitBodyPos(s32 arg0, s32 arg1, s16 *arg2) {
    btl_getUnitBodyPos(btl_getUnit(arg0), arg1, arg2);
}

void btlfx_setUnitPos(s32 arg0, s16 *arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (btl_isModelPart(u) == 0) {
        u->pos[0] = arg1[0] << 16;
        u->pos[1] = arg1[1] << 16;
        u->pos[2] = arg1[2] << 16;
    }
}

s32 btlfx_getBouncePos(s32 arg0, s16 *arg1, s16 *arg2) {
    BtlUnitList *l = g_btlfxEnv->targets;
    BtlUnit *u = l->bounced[arg0];
    if (arg0 >= l->bounceCount) {
        return 1;
    }
    btl_getUnitBodyPos(u, 0, arg1);
    arg2[0] = u->rot[0];
    arg2[1] = u->rot[1];
    arg2[2] = u->rot[2];
    return 0;
}

void btlfx_getUnitRot(s32 arg0, s16 *arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    s16 *p = u->rot;
    arg1[0] = p[0];
    arg1[1] = p[1] + 0x800;
    arg1[2] = p[2];
}

void btlfx_setUnitRot(s32 arg0, s16 *arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (btl_isModelPart(u) == 0) {
        s16 *p = u->rot;
        p[0] = arg1[0];
        p[1] = arg1[1] - 0x800;
        p[2] = arg1[2];
    }
}

s32 btlfx_isEnemyUnit(s32 arg0) {
    return (btl_getUnit(arg0)->unk68 & 1) ^ 1;
}

void btlfx_turnUnitToward(s32 arg0, s16 *arg1, s16 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    SVec4 rot;
    SVec4 tmp;
    SVec4 cur;
    s32 t;

    if (arg6 < arg5) {
        return;
    }
    btl_getUnit(arg0);
    if (arg6 == 0) {
        arg6 = 1;
    }
    t = (arg5 << 12) / arg6;
    if (arg5 == 0 || arg2 == NULL) {
        if (arg2 == NULL) {
            arg2 = tmp.h;
            t = 0x1000;
        }
        btlfx_getUnitRot(arg0, arg2);
    }
    if (arg1 != NULL) {
        btlfx_getUnitPos(arg0, cur.h);
        if (cur.w[0] != ((SVec4 *)arg1)->w[0] || cur.h[2] != arg1[2]) {
            btlfx_aimAngles(cur.h, arg1, rot.h);
            if (!(arg3 & 1)) {
                rot.h[0] = 0;
            }
            if (!(arg3 & 2)) {
                rot.h[1] += 0x800;
            }
        } else {
            rot.w[0] = ((SVec4 *)arg2)->w[0];
            rot.w[1] = ((SVec4 *)arg2)->w[1];
        }
    } else {
        rot.h[0] = 0;
        rot.h[1] = 0;
    }
    rot.h[2] = arg4;
    btlfx_turnToward(arg2, rot.h, 0x1000 - t);
    btlfx_setUnitRot(arg0, rot.h);
}

void btlfx_getUnitScale(s32 arg0, VECTOR *arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    BtlUnit9C *m = u->model;
    if (!(m->unk0 & 2)) {
        arg1->vz = 0x1000;
        arg1->vy = 0x1000;
        arg1->vx = 0x1000;
    } else {
        arg1->vx = m->unk54;
        arg1->vy = u->model->unk56;
        arg1->vz = u->model->unk58;
    }
}

void btlfx_setUnitScale(s32 arg0, s32 *arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (btl_isModelPart(u) == 0) {
        if (arg1[0] == 0x1000 && arg1[1] == arg1[0] && arg1[2] == arg1[1]) {
            u->model->unk0 &= ~2;
        } else {
            u->model->unk0 |= 2;
            u->model->unk54 = arg1[0];
            u->model->unk56 = arg1[1];
            u->model->unk58 = arg1[2];
        }
    }
}

void btlfx_getStrikePos(s32 arg0, s32 arg1, s16 *arg2) {
    s16 sp10[4];
    s16 sp18[4];
    s16 sp20[4];
    if (btl_getUnit(arg1) != btl_getUnit(arg0)) {
        btlfx_getUnitPos(arg0, sp10);
        btlfx_getUnitPos(arg1, sp18);
        btlfx_aimAngles(sp18, sp10, sp20);
        btlfx_pointAhead(sp18, sp20, btlfx_getUnitRadius(arg1), arg2);
    } else {
        btlfx_getUnitPos(arg0, arg2);
    }
}

s32 btlfx_compareDepth(s32 arg0, s32 arg1) {
    s16 a[4];
    s16 b[4];
    s32 r;
    btlfx_getUnitPos(arg0, a);
    btlfx_getUnitPos(arg1, b);
    r = a[2] <= b[2];
    if (((g_btlfxEnv->camera->yaw - 0x400) & 0xFFF) > 0x800) {
        r = -r;
    }
    return r;
}

s32 func_800E7CC4(s32 arg0, s32 arg1) {
    return (u16)(btl_getUnit(arg0)->model->unk8->unkA / 6) + arg1;
}

u32 btlfx_getUnitSlot(s32 arg0) {
    return (u8)btl_getUnit(arg0)->unk68 >> 5;
}

void btlfx_getWeaponPos(s32 arg0, s32 arg1, s16 *arg2) {
    BtlUnit *u;
    s32 bone;
    MATRIX m;

    SVECTOR v;

    u = btl_getUnit(arg0);
    bone = (g_gameState->unk1C->characters + ((u->unk68 >> 1) & 0xF))->unk21;
    if (arg1 == 0) {
        btlfx_getUnitBonePos(arg0, bone, (u16 *)arg2);
    } else {
        btlfx_getUnitBoneMatrix(arg0, bone, (BtlBone *)&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        func_800BC1E0((Unk800BC1E0 *)u->weapon, (s16 *)&v);
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        arg2[0] = m.t[0];
        arg2[1] = m.t[1];
        arg2[2] = m.t[2];
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void btlfx_setUnitBrightness(BtlUnit *arg0, s32 arg1) {
    s32 f;
    s16 v;
    if (arg1 >= 0x100) {
        f = 0;
        arg1 -= 0x100;
    } else {
        f = 1;
    }
    if ((arg0->unk68 & 1) && arg0->weaponModel != NULL && f && (*(u32 *)arg0->weaponModel & 0x30) == 0x20) {
        v = arg1 - 0x80;
        btl_setModelRgb(arg0->weaponModel, v, v, v);
        if (arg1 < 0x46) {
            btl_setModelAbr(arg0->weaponModel, 0x20);
        }
    }
    if ((*(u32 *)arg0->model & 0x30) == 0x20) {
        v = arg1 - 0x80;
        btl_setModelRgb(arg0->model, v, v, v);
        if (arg1 < 0x46) {
            btl_setModelAbr(arg0->model, 0x20);
        }
    }
}

void btlfx_fadeUnit(BtlUnit *arg0, s32 arg1) {
    if (btl_isModelPart(arg0) == 0) {
        if (arg1 < 8) {
            if (!((arg0->unk68 >> 22) & 1)) {
                arg0->unk68 |= 0x400000;
                btlfx_setUnitBrightness(arg0, arg1);
            }
        } else if ((arg0->unk68 >> 22) & 1) {
            if (!(arg0->status & BTL_STATUS_JUMP)) {
                arg0->unk68 &= ~0x400000;
            }
        } else {
            btlfx_setUnitBrightness(arg0, arg1);
        }
    }
}

void btlfx_fadeUnitById(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (arg1 < 0) {
        if (arg1 == -1) {
            btl_hideMeshes(u, 0xFFFF);
        } else {
            btl_showMeshes(u, 0xFFFF);
        }
    } else {
        btlfx_fadeUnit(u, arg1);
    }
}

void btlfx_setUnitLight(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (btl_isModelPart(u) == 0) {
        BtlUnit9C *m = u->model;
        s16 v = arg1 - 0x80;
        if ((*(u32 *)m & 0x30) == 0x20) {
            btl_setModelRgb(m, v, v, v);
        }
    }
}

s32 btlfx_fadeUnitGroup(u32 type, s32 arg1) {
    BtlUnit *u;
    s32 n = 0;
    for (u = g_gameState->unk1C->unk8E4.battle->unitList; u != NULL; u = u->next) {
        switch (type) {
        case 0:
            if (u->unk68 & 1) {
                btlfx_fadeUnit(u, arg1);
                n++;
            }
            break;
        case 1:
            if (!(u->unk68 & 1)) {
                btlfx_fadeUnit(u, arg1);
                n++;
            }
            break;
        case 9:
            if (u == btl_getUnit(0x10)) {
                btlfx_fadeUnit(u, arg1);
                n++;
            }
            break;
        case 2:
            btlfx_fadeUnit(u, arg1);
            n++;
            break;
        case 8:
            if (btlfx_isEnemyUnit(0x10)) {
                if (!(u->unk68 & 1)) {
                    btlfx_fadeUnit(u, arg1);
                    n++;
                }
            } else if (u != btl_getUnit(0x10) && (u->unk68 & 1)) {
                btlfx_fadeUnit(u, arg1);
                n++;
            }
            break;
        }
    }
    return n;
}

void btlfx_clearUnitStatus(s32 arg0) {
    if (btlfx_isEnemyUnit(arg0) != 0) {
        btl_removeStatuses(btl_getUnit(arg0), ~(BTL_STATUS_DEATH_IMMUNE | BTL_STATUS_KO | BTL_STATUS_LOW_HP | BTL_STATUS_STOP));
    } else {
        btl_clearUnitStatus(btl_getUnit(arg0));
    }
}

s32 btlfx_nextUnitPos(s32 id, s16 *arg1, BtlUnit **iter, s32 *out) {
    BtlUnit *u;
    s32 r = 0;

    if (*iter == (BtlUnit *)-1) {
        u = g_gameState->unk1C->unk8E4.battle->unitList;
    } else {
        if (*iter == NULL) {
            return -2;
        }
        u = *iter;
    }
    btl_getUnitBodyPos(u, 0, arg1);
    if (((u8)btl_hasStatus(u, BTL_STATUS_KO) && u != btl_getUnit(id)) || !((u->unk68 >> 28) & 1)) {
        *iter = u->next;
        return -1;
    }
    if (u == btl_getUnit(id)) {
        r = 1;
    }
    *out = u->unk68 & 1;
    *iter = u->next;
    return r;
}

void func_800E83EC(void) {
    g_evtGlobalVars.b[0xC7] |= 0x10;
}

void btlfx_reapplyMini(BtlUnit *arg0) {
    if (arg0->status & BTL_STATUS_MINI) {
        arg0->model->unk0 |= 2;
        arg0->model->unk54 = 0x800;
        arg0->model->unk56 = 0x800;
        arg0->model->unk58 = 0x800;
    }
}

void btlfx_reapplyVanish(BtlUnit *arg0) {
    if (arg0->status & BTL_STATUS_VANISH) {
        btl_hideMeshes(arg0, arg0->unkAE);
    }
}

s32 btlfx_getScaledRadius(s32 arg0) {
    VECTOR v;
    btlfx_getUnitScale(arg0, &v);
    return btl_getUnit(arg0)->model->unk8->unkA * v.vx >> 12;
}

s32 func_800E84F0(s32 arg0) {
    s32 ret = 0;
    if (g_gameState->unk1C->unk8E4.battle->curCmd->cmdNo == BTL_CMD_AUTO_POTION) {
        ret = btl_isCmdPending(btl_getUnit(arg0)->cmd) != 0;
    }
    return ret;
}

s32 func_800E8554(s32 arg0, s32 arg1) {
    s32 r = 0;
    BtlUnit *u = btl_getUnit(arg1);
    if (arg0 == 0) {
        if ((u8)btl_hasStatus(u, BTL_STATUS_KO)) {
            u->model->unkC = (BtlUnit9CC *)u->motions[4];
        } else if ((u->unk68 >> 19) & 1) {
            u->model->unkC = (BtlUnit9CC *)u->motions[9];
        } else {
            u->model->unkC = (BtlUnit9CC *)u->motions[(u->unk68 >> 18) & 1];
        }
    } else if ((u8)btl_hasStatus(u, BTL_STATUS_KO) || ((u->unk68 >> 1) & 0xF) == 4) {
        func_800E8554(0, 0x10);
        r = 1;
    } else {
        btlfx_playUnitMotion(0x10, 0x1C);
    }
    return r;
}

s32 btlfx_isUnitKind(s32 arg0, s32 arg1) {
    u32 f = btl_getUnit(arg0)->unk68;
    u8 k = D_800F8D70[arg1];
    return ((f & 1) ? g_gameState->unk1C->characters[(f >> 1) & 0xF].unk34.b.unk4 : 0x13) == k;
}

void btlfx_copyWords(s32 *arg0, s32 *arg1, u32 arg2) {
    u32 i;
    u32 t = arg2 & 3;
    arg2 >>= 2;
    if (t) {
        arg2++;
    }
    for (i = 0; i < arg2;) {
        i++;
        *arg0++ = *arg1++;
    }
}

s32 *btlfx_swapModelData(s32 arg0, s32 *arg1) {
    s32 id = (arg0 & 0xFFFF) | 0x20000;
    u32 size = (u32)db_lookupRegistered(5, id, 0);
    s32 *p = db_lookupRegistered(4, id, 0);
    if (*arg1 == 0x1234) {
        btlfx_copyWords(p, arg1 + 1, size);
    } else {
        btlfx_copyWords(arg1 + 1, p, size);
        *arg1 = 0x1234;
    }
    return p;
}

void func_800E87C4(s32 arg0) {
    btl_unloadEnemyModels();
    btlfx_copyWords(g_gameState->unk1C->unk8E4.battle->unk11C0->unk44, g_btlfxLoader.unk54->unk50[0], arg0 << 10);
    g_btlfxLoader.unk54->unk50[0] = g_gameState->unk1C->unk8E4.battle->unk11C0->unk44;
}

void btl_forEachModelVertex(BtlUnit9C8 *arg0, void (*arg1)(SVECTOR *, s32)) {
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    BtlMesh *m;
    BtlUnit9C8 *g = arg0;
    for (i = 0; i < g->unk3; i++) {
        m = &g->unk10[i];
        n = 0;
        for (j = 0; j < g->unk2; j++) {
            for (k = 0; k < m->vertexCounts[j]; k++) {
                arg1(&m->vertices[n + k], n + k);
            }
            n += m->vertexCounts[j];
        }
    }
}

void func_800E8980(s32 arg0) {
    g_btlfxEnv->targets->partner = btl_getUnitById(g_btlfxEnv->targets->unk25[arg0]);
}

void btlfx_restorePartyLooks(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unitList;
    while (u != NULL) {
        if (u->unk68 & 1) {
            btl_reapplyStatusLook(u);
            btlfx_reapplyMini(u);
            btlfx_reapplyVanish(u);
        }
        u = u->next;
    }
    if (g_btlfxEnv->unk0->unk2 == 0x1BF) {
        btl_setIdleMotion(btl_getUnit(0x10));
    }
}

void btlfx_restoreUnitLook(s32 arg0) {
    BtlUnit *u = btl_getUnit(arg0);
    if (u->unk68 & 1) {
        btl_reapplyStatusLook(u);
        btlfx_reapplyMini(u);
        btlfx_reapplyVanish(u);
    }
}

s32 btlfx_countVisibleParty(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unitList;
    s32 n = 0;
    while (u != NULL) {
        n += (u->unk68 & 0x400001) == 1;
        u = u->next;
    }
    return n;
}

/* weapon texture animation: arg1 0 = start anim (arg2 ? 1 : 0), 1 = stop anim 2, 2 = start anim 2 */
void btlfx_setUnitTexAnim(s32 arg0, s32 arg1, s32 arg2) {
    BtlUnit *u = btl_getUnit(arg0);

    switch (arg1) {
    case 0:
        ((BtlUnit9C50Arr *)u->model->unk50)->e[arg2 ? 1 : 0].unk4 |= 3;
        ((BtlUnit9C50Arr *)u->model->unk50)->e[arg2 ? 1 : 0].unkC = 0;
        ((BtlUnit9C50Arr *)u->model->unk50)->e[arg2 ? 1 : 0].unk12 = 0x1000;
        break;
    case 1:
        ((BtlUnit9C50Arr *)u->model->unk50)->e[2].unk4 &= ~3;
        break;
    case 2:
        ((BtlUnit9C50Arr *)u->model->unk50)->e[2].unk4 |= 1;
        ((BtlUnit9C50Arr *)u->model->unk50)->e[2].unkC = 0;
        ((BtlUnit9C50Arr *)u->model->unk50)->e[2].unk12 = 0x1000;
        break;
    }
}

void func_800E8C84(s32 arg0) {
    if (arg0 != 0) {
        g_gameState->unk1C->unk8E4.battle->cmdFlags |= 2;
    } else {
        g_gameState->unk1C->unk8E4.battle->cmdFlags &= ~2;
    }
}

s32 func_800E8CE4(void) {
    return (g_gameState->unk1C->unk8E4.battle->cmdFlags >> 1) & 1;
}

void btlfx_showCasterCmdName(void) {
    if (btlfx_isEnemyUnit(0x10) == 0) {
        btl_showCmdName(g_gameState->unk1C->unk8E4.battle->curCmd);
    }
}

s32 btlfx_isResultDeferred(void) {
    s32 r = 0;
    if (btlfx_isEnemyUnit(0x10) != 0) {
        if (g_btlfxEnv->targets->unk25[0] == 0) {
            return r;
        }
        r = (u8)(g_btlfxEnv->unk0->unk1C >> 8) >> 7;
    }
    return r;
}

void btlfx_execCmd(s32 arg0) {
    s32 i;
    if (btlfx_isResultDeferred() == 0) {
        if (arg0 == -1) {
            for (i = 0; i < g_btlfxEnv->targets->targetCount; i++) {
                btl_execCmd(g_btlfxEnv->targets->targets[i]);
            }
        } else {
            btl_execCmd(btl_getUnit(arg0));
        }
    }
}

void btlfx_showPopups(s32 arg0) {
    s32 i;
    if (btlfx_isResultDeferred() == 0) {
        if (arg0 == -1) {
            for (i = 0; i < g_btlfxEnv->targets->targetCount; i++) {
                btl_spawnPopups(g_btlfxEnv->targets->targets[i]);
            }
        } else {
            btl_spawnPopups(btl_getUnit(arg0));
        }
    }
}

void btl_setVsyncMode(s32 arg0) {
    g_gameState->vsyncMode = arg0;
}

/* Void in the original, but effect modules read $v0 after calling it (export 0x338): it holds the u16 from
   Hi_RegisterTexListModel / Hi_RegisterGouEffModel. A u16 return adds an andi, so this stays void; ports must
   return that value (docs/effect-modules.md). */
void btlfx_registerEffectModel(Unk800E8F50 *arg0, s32 arg1) {
    s32 i;
    s32 n;
    Unk800E8F50 *h;
    Unk800E8F50Entry *e;
    void *p;
    h = arg0;
    if (arg0->unk0 != 0x6F73) {
        SystemError(0x53, 0x190);
    }
    p = (u8 *)arg0 + h->unk4;
    if (h->unk2 != 0) {
        n = (arg0->unk4 - 8) >> 3;
        e = h->unk8;
        for (i = 0; i < n; i++) {
            e[i].unk0 |= (arg1 & 3) << 5;
        }
        Hi_RegisterTexListModel(p, (BtlTexInfo *)e, (s16 *)((u8 *)h + h->unk6), n);
    } else {
        Hi_RegisterGouEffModel(p);
    }
}

void btlfx_registerSummonModel(Unk800E9000 *arg0, s32 *src, s32 n, Unk80100DC054 *e) {
    Unk800E9000 *h = arg0;
    s32 *s;
    s32 i;
    s32 k;
    s32 k2;
    Unk800E9000Ent *a;
    Unk800E9000Ent *b;

    if (h->unk0 != 0x6F73) {
        SystemError(0x53, 0x190);
    }
    k = (h->unk4 - 8) >> 3;
    e->unk2 = n;
    e->unk40 = NULL;
    e->unk4 = k;
    e->unk3C = (u8 *)h + h->unk4;
    s = src;
    for (i = 0; i < n; i++, s++) {
        e->unk180[i] = *s;
    }
    b = (Unk800E9000Ent *)((u8 *)h + h->unk6);
    a = h->unk8;
    for (i = 0; i < k; i++) {
        e->unk18[i] = a[i].unk0;
        e->unk24[i] = a[i].unk2;
        e->unk30[i] = b[i].unk2;
    }
    btlfx_initSummonModels(&e->unk90, 1);
    g_btlfxLoader.unk54 = e;
    g_btlfxLoader.unk60 = Hi_RegisterSummonModel(e, NULL);
}

BtlUnit *btlfx_getFirstTarget(void) {
    if (g_btlfxEnv->targets->bounceCount == 0) {
        return btl_getUnit(0);
    }
    return btl_getUnit(8);
}

void btlfx_updateCameraEnable(void) {
    s32 r;
    BtlUnit *a;
    BtlUnit *b;
    s32 f;
    u32 v;
    if (g_btlfxEnv->unk0->unk1C & 8) {
        r = 1;
    } else {
        a = btl_getUnit(0x10);
        b = btl_getUnit(0);
        f = (g_gameState->unk24 >> 4) & 1;
        if (f == 1 || (u8)btl_hasStatus(a, BTL_STATUS_MINI) || (u8)btl_hasStatus(a, BTL_STATUS_VANISH)) {
            r = 0;
        } else {
            if (b->unk68 & 1) {
                v = g_gameState->unk1C->unk8E4.battle->scene.unk0 >> 11;
            } else {
                v = g_gameState->unk1C->unk8E4.battle->scene.unk0 >> 12;
            }
            v &= 1;
            r = v == 0;
        }
    }
    g_btlfxCameraOn = r;
}

void btlfx_playCmdCamera(s32 arg0) {
    u32 hi;
    s32 msg;
    BtlUnit *u;
    BtlUnit *t;

    if (arg0 == 0xFFFF) {
        return;
    }
    hi = arg0 & 0xF0;
    if (g_btlfxCameraOn == 0 || hi == 0xF0 || (g_btlfxEnv->targets->kind & 2)) {
        return;
    }
    u = btl_getUnit(0x10);
    if ((u8)btl_hasStatus(u, BTL_STATUS_MINI) != 0 || (u8)btl_hasStatus(u, BTL_STATUS_VANISH) != 0) {
        return;
    }
    t = btlfx_getFirstTarget();
    if (g_btlfxEnv->targets->kind & 4) {
        msg = btlfx_getCommonData(D_800F8D78[4]);
    } else if (u->unk68 & 1) {
        if ((u8)btl_hasStatus(u, BTL_STATUS_TRANCE) == 0) {
            if ((hi >> 4) == 1 || (hi >> 4) == 7) {
                if (((u->unk68 >> 1) & 0xF) == 2) {
                    msg = btlfx_getCommonData(D_800F8D78[2]);
                } else {
                    msg = btlfx_getCommonData(D_800F8D78[3]);
                }
            } else if (t->unk68 & 1) {
                msg = btl_getSceneData(3);
            } else {
                msg = btl_getSceneData(4);
            }
        } else if (t->unk68 & 1) {
            msg = btlfx_getCommonData(D_800F8D78[0]);
        } else {
            msg = btlfx_getCommonData(D_800F8D78[1]);
        }
    } else {
        msg = g_gameState->unk1C->unk8E4.battle->enemies[(u->unk68 >> 1) & 0xF].unk0->unk2C;
    }
    if (msg != 0) {
        btlcam_play(msg);
    }
}

void btlfx_playBounceCamera(void) {
    u8 v;
    if (g_btlfxCameraOn != 0 && g_btlfxEnv->targets->bounceCount != 0) {
        if (btl_getUnit(8)->unk68 & 1) {
            v = D_800F8D80;
        } else {
            v = D_800F8D81;
        }
        btlcam_play(btlfx_getCommonData(v));
    }
}

void btlfx_initSounds(void) {
    s32 i;
    D_80104E30.unk0 = 0;
    for (i = 5; i >= 0; i--) {
        D_80104E30.unk4[i].unk0 = 0;
    }
}

void btlfx_updateSounds(void) {
    s32 n = 0;
    s32 i;
    Unk80104E30 *p = &D_80104E30;
    Unk80104E30Entry *e = p->unk4;
    for (i = 0; i < 6; i++, e++) {
        if (e->unk0 != 0) {
            if (func_8005553C(e->unk0) == 0) {
                e->unk0 = 0;
            } else {
                n++;
                func_80055850(0, e->unk0, btl_soundPan(e->unk4));
                e->unk2++;
            }
        }
    }
    p->unk0 = n;
}

s32 btlfx_allocSound(s32 arg0) {
    Unk80104E30Entry *best = NULL;
    s32 bestbit = 0;
    s32 max = -1;
    s32 bit = 1;
    Unk80104E30 *p = &D_80104E30;
    Unk80104E30Entry *e = p->unk4;
    s32 i;
    for (i = 0; i < 6; i++, e++, bit <<= 1) {
        if (e->unk0 == 0) {
            break;
        }
        if (max < e->unk2) {
            max = e->unk2;
            best = e;
            bestbit = bit;
        }
    }
    if (i >= 6) {
        bit = bestbit;
        e = best;
    }
    if (func_8005553C(bit) != 0) {
        func_8005546C(0, bit);
    }
    if (e != NULL) {
        e->unk0 = bit;
        e->unk4 = arg0;
        e->unk2 = 0;
    }
    return bit;
}

void btlfx_stopAllSounds(void) {
    func_8005546C(0, 0x3F);
}

Unk80104E34 *btlfx_findSound(s32 arg0) {
    Unk80104E34 *p;
    s32 i;
    if (arg0 != 0) {
        p = D_80104E34;
        for (i = 0; i < 6; i++, p++) {
            if (p->unk0 == arg0) {
                return p;
            }
        }
    }
    return NULL;
}

s32 btl_soundPan(s32 arg0) {
    SVECTOR pos;
    SVECTOR *v;
    DVECTOR sxy;
    s32 x;
    s32 sign;

    if (arg0 == -1) {
        return 0x80;
    }
    if (arg0 & 0x90000000) {
        v = (SVECTOR *)arg0;
    } else {
        v = &pos;
        btlfx_getUnitPos(arg0, (s16 *)v);
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(v);
    gte_rtps();
    gte_stsxy(&sxy);
    x = sxy.vx - 160;
    sign = (x < 0) ? -1 : 1;
    x = (x >= 0) ? x : -x;
    if (x > 170) {
        x = 170;
    }
    return sign * (x * 63 / 170) + 0x80;
}

void btlfx_stopSound(s32 arg0) {
    Unk80104E34 *p = btlfx_findSound(arg0);
    if (p != NULL) {
        func_8005546C(0, arg0);
        p->unk0 = 0;
    }
}

void func_800E98C8(s32 arg0, s32 arg1) {
    Unk80104E34 *p = btlfx_findSound(arg0);
    if (p != NULL) {
        p->unk4 = arg1;
    }
}

void func_800E98F8(s32 arg0) {
    Unk80104E34 *p = D_80104E34;
    s32 i;
    for (i = 0; i < 6; i++, p++) {
        if (p->unk0 != 0) {
            p->unk4 = arg0;
        }
    }
}

s32 btlfx_playFileSound(s32 arg0, s32 arg1, s32 arg2) {
    s32 v = btlfx_allocSound(arg2);
    if (v != 0) {
        func_800553EC(btlfx_getFileData(arg1, arg0), v, btl_soundPan(arg2), 0x7F);
    }
    return v;
}

void btlfx_replaceSound(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    btlfx_stopSound(arg3);
    btlfx_playFileSound(arg0, arg1, arg2);
}

s32 btlfx_playCommonSound(s32 arg0, s32 arg1) {
    s32 v = btlfx_allocSound(arg1);
    if (v != 0) {
        func_800553EC(btlfx_getCommonData(arg0), v, btl_soundPan(arg1), 0x7F);
    }
    return v;
}
