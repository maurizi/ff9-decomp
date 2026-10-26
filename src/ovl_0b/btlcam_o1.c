#include "ovl_0b.h"

s32 func_800C2DF4(void) {
    return D_800FE470;
}

void btlcam_init(void) {
    g_btlfxEnv->camera = &g_btlCamera;
    bzero(&g_btlCamera, 0xD0);
    g_btlCamera.baseProjDist = 0x200;
    g_btlCamera.projDist = 0x200;
}

void btlcam_beginEffect(void) {
    bzero(&g_btlCamScript, 0x64);
    bcopy(g_btlfxEnv->targets, &g_btlCamTargets, 0x4C);
    g_btlCamera.unk5C = 0;
    g_btlCamera.unk53 = btlfx_rand() & 1;
    if (g_btlfxEnv->state->flags & 4) {
        btlcam_getUnitAnchors(g_btlCamTargets.partner, g_btlCamScript.partnerAnchors);
    }
    btlcam_getUnitAnchors(g_btlCamTargets.caster, g_btlCamScript.casterAnchors);
    btlcam_getUnitAnchors(g_btlCamTargets.target, g_btlCamScript.targetAnchors);
}

void btlcam_playScene(s32 arg0) {
    g_btlCamera.unkB9 = 0;
    g_btlCamera.unk52 = 0;
    g_btlCamera.unk5C = 0;
    bzero(&g_btlCamScript, 0x64);
    bzero(&g_btlCamTargets, 0x4C);
    btlcam_loadScript(arg0);
    if (g_btlCamera.unkCC != 0) {
        btlcam_getUnitAnchors(g_btlCamera.unkCC, g_btlCamScript.casterAnchors);
    }
}

void btlcam_play(s32 arg0) {
    g_btlCamera.unkB9 = 1;
    g_btlCamera.unk52 = 0;
    g_btlCamScript.unk8 = 0;
    if (g_btlCamera.unk5C & 2) {
        g_btlCamera.unk5C = 0;
    }
    btlcam_finishPaths();
    btlcam_loadScript(arg0);
}

void btlcam_playSeq(s32 arg0) {
    bzero(&g_btlCamScript, 0x64);
    g_btlCamera.unk53 = btlfx_rand() & 1;
    btlcam_getUnitAnchors(g_gameState->gameData->unk8E4.battle->seq.camCaster, g_btlCamScript.casterAnchors);
    btlcam_getUnitAnchors((BtlUnit *)g_gameState->gameData->unk8E4.battle->seq.camTarget, g_btlCamScript.targetAnchors);
    btlcam_play(arg0);
    g_btlCamera.unkB9 = 2;
    g_btlCamera.unk5C = 0;
}

void func_800C30B8(void) {
    BtlUnit *u = g_btlfxEnv->targets->caster;
    if (u->flags & 1) {
        D_800FE484 = u;
    }
}

void btlcam_endEffect(void) {
    if (g_btlCamScript.unk45 == 0) {
        g_btlCamera.unkA8 = g_btlCamScript.unk18;
        btlcam_loadScript(btl_getSceneData(1));
        g_btlCamera.projDist = g_btlCamera.baseProjDist;
    }
    g_btlCamera.unk5C = 1;
    g_btlCamScript.unk45 = 0;
}

void btlcam_loadScript(s32 arg) {
    s16 *arg0 = (s16 *)arg;
    u16 *p[4];
    s32 f0;
    s32 flags;
    u16 *q;
    u16 *t;
    u8 *data;
    s32 i;
    s32 n;
    s32 r;
    BtlUnit *u;
    BtlUnit *w;
    BtlUnit9C8 *m;

    g_btlCamScript.frame = 1;
    f0 = flags = arg0[0];
    q = (u16 *)arg0 + 1;
    for (i = 0; i < 4; i++) {
        p[i] = NULL;
        if (flags & 1) {
            p[i] = (u16 *)((u8 *)arg0 + *q);
            q++;
        }
        flags >>= 1;
    }
    data = (u8 *)arg0 + *q;
    for (i = 0; i < 4; i++) {
        if (flags & 1) {
            bcopy(data, &g_btlCamera.points[i], 6);
            data += 6;
        } else {
            g_btlCamera.points[i].vx = 0;
            g_btlCamera.points[i].vy = 0;
            g_btlCamera.points[i].vz = 0;
        }
        g_btlCamera.unk30[i].vx = 0;
        g_btlCamera.unk30[i].vy = 0;
        g_btlCamera.unk30[i].vz = 0;
        flags >>= 1;
    }
    if (p[3] != NULL) {
        t = p[3];
        switch (*t >> 14) {
        case 0:
            g_btlCamera.unkA8 = (*t >> 12) & 3;
            break;
        case 1:
            r = btlfx_randRange(0, 9);
            if (!(*t & 0x3000)) {
                if (r < ((*t >> 10) & 3) + 5) {
                    g_btlCamera.unkA8 = 0;
                } else {
                    g_btlCamera.unkA8 = 1;
                }
            } else {
                n = ((*t >> 10) & 3) + 3;
                if (r < n) {
                    g_btlCamera.unkA8 = 0;
                } else if (r < n + 3) {
                    g_btlCamera.unkA8 = 1;
                } else {
                    g_btlCamera.unkA8 = 2;
                }
            }
            break;
        case 2:
            switch ((*t >> 12) & 3) {
            case 0:
                n = g_gameState->gameData->unk8E4.battle->scene.unk0 & 3;
                if (n == 2) {
                    g_btlCamera.unkA8 = n;
                } else {
                    g_btlCamera.unkA8 = btlfx_randRange(0, 1);
                }
                g_btlCamScript.unk18 = g_btlCamera.unkA8;
                break;
            case 1:
                g_btlCamera.unkA8 = g_gameState->gameData->unk8E4.battle->unk131C;
                break;
            case 2:
                w = g_btlCamTargets.target;
                n = 0;
                r = n;
                if (w != NULL) {
                    m = w->model->header;
                    r = m->height;
                    n = m->radius;
                }
                if (n == 0x4D7 || n >= 0xBB8) {
                    g_btlCamera.unkA8 = 2;
                } else if (n < 0x65) {
                    g_btlCamera.unkA8 = 0;
                } else {
                    g_btlCamera.unkA8 = 1;
                    if (r < 0x159) {
                        g_btlCamera.unkA8 = 0;
                    }
                    if (r >= 0x3E9) {
                        g_btlCamera.unkA8 = 2;
                    }
                }
                break;
            case 3:
                if (g_btlCamera.unkCC == NULL) {
                    g_btlCamera.unkA8 = 0;
                } else if ((g_btlCamera.unkCC->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_JUMP)) || !((g_btlRewards.unkA >> ((u8)g_btlCamera.unkCC->flags >> 5)) & 1)) {
                    g_btlCamera.unkA8 = 0;
                } else {
                    n = g_gameState->gameData->unk8E4.battle->scene.unk0 & 3;
                    if (n == 2) {
                        g_btlCamera.unkA8 = n;
                    } else {
                        r = 0;
                        for (u = g_gameState->gameData->unk8E4.battle->unitList; u != NULL; u = u->next) {
                            if ((u->flags & 1) && !(u->status & (BTL_STATUS_KO | BTL_STATUS_JUMP))) {
                                r++;
                            }
                        }
                        if (r < 3) {
                            g_btlCamera.unkA8 = btlfx_randRange(0, 1) + 1;
                        } else {
                            r = btlfx_randRange(0, 5);
                            g_btlCamera.unkA8 = 0;
                            if (r == 4) {
                                g_btlCamera.unkA8 = 1;
                            }
                            if (r == 5) {
                                g_btlCamera.unkA8 = 2;
                            }
                        }
                    }
                }
                break;
            }
            break;
        case 3:
            g_btlCamera.unkA8 = g_btlCamera.unkB8 & 3;
            if (g_btlCamera.unkA8 == 2) {
                g_btlCamera.unkA8 = 0;
            }
            if (g_btlCamera.unkA8 == 3) {
                g_btlCamera.unkA8 = 2;
            }
            break;
        }
    }
    g_btlCamScript.pc = (u8 *)p[g_btlCamera.unkA8];
    if (f0 & 0x200) {
        g_btlCamera.unkB8 = (f0 >> (g_btlCamera.unkA8 * 2 + 10)) & 3;
    }
}

void btlcam_finishPaths(void) {
    s32 i;
    s32 t;
    for (i = 0; i < 2; i++) {
        t = g_btlCamera.unk68[i].unk1D != 0;
        if (t && (u32)(g_btlCamera.unk68[i].unk18 - 2) < 2) {
            btlcam_evalPath(1, &g_btlCamera.unk68[i], g_btlCamera.unk68[i].unk0[1].unk0);
            g_btlCamera.unk68[i].unk18 = 1;
        }
    }
}

void btlcam_update(void) {
    if (g_btlCamera.unk5D == 0 && btlcam_step() != 0) {
        btlcam_step();
    }
    g_btlCamera.unk5D = 0;
}

s32 btlcam_step(void) {
    SVECTOR sv;
    SVECTOR sv2;
    s32 n;
    s32 flags;
    s32 i;
    s32 m;
    s32 v;
    s32 r;
    u16 *p;
    s32 c;
    s32 j;
    s32 e;
    Battle *b;

    if (g_btlCamScript.frame != 0) {
        if (g_btlCamScript.pc != NULL) {
            for (n = 0; n < 10; n++) {
                p = (u16 *)g_btlCamScript.pc;
                if (g_btlCamScript.frame != (p[0] & 0x3FF)) {
                    break;
                }
                if (!(g_btlCamera.unk5C & 1)) {
                    g_btlCamera.unk5C |= 1;
                    g_btlCamera.projDist = 0x200;
                }
                flags = p[1] | ((p[0] >> 10) << 16);
                g_btlCamScript.pc += 4;
                for (i = 0; i < 2; i++) {
                    m = (flags >> (i * 3)) & 7;
                    if ((u32)(m - 2) < 2 && (u32)(g_btlCamera.unk68[i].unk18 - 2) < 2 && g_btlCamera.unk68[i].unk1D != 0) {
                        btlcam_evalPath(1, &g_btlCamera.unk68[i], g_btlCamera.unk68[i].unk0[1].unk0);
                    }
                    if (m > 0 && g_btlCamera.unk68[i].unk1B == 0) {
                        g_btlCamera.unk68[i].unk18 = m;
                    }
                    if ((u32)(m - 1) < 3) {
                        btlcam_setPathKey(&g_btlCamera.unk68[i], (Unk800C3F78 *)g_btlCamScript.pc);
                        g_btlCamScript.pc += 6;
                    }
                    if ((u32)(m - 2) < 3) {
                        btlcam_setPathTiming(&g_btlCamera.unk68[i], g_btlCamScript.pc);
                        if (m == 4) {
                            g_btlCamera.unk68[i].unk0[1].unk0[2] = g_btlCamScript.pc[1] << 5;
                        }
                        g_btlCamScript.pc += 4;
                    }
                    g_btlCamera.unk68[i].unk1B = 0;
                }
                if (flags & 0x40) {
                    g_btlCamScript.unk8 = *g_btlCamScript.pc;
                    g_btlCamScript.pc += 2;
                }
                if (flags & 0x80) {
                    g_btlCamScript.unk44 = 1;
                }
                if (flags & 0x100100) {
                    g_btlCamera.unk52 = 1;
                }
                if (flags & 0x200) {
                    g_btlCamScript.sway = *g_btlCamScript.pc;
                    g_btlCamScript.pc += 2;
                }
                if (flags & 0x400) {
                    g_btlCamera.unkAC = *g_btlCamScript.pc;
                    g_btlCamScript.pc += 2;
                }
                if (flags & 0x800) {
                    g_btlCamScript.zoomFrames = g_btlCamScript.zoomLeft = g_btlCamScript.pc[0];
                    g_btlCamScript.zoomTarget = g_btlCamScript.pc[1];
                    g_btlCamScript.zoomFrom = g_btlCamera.projDist;
                    v = ((u16 *)g_btlCamScript.pc)[1];
                    if (v == 0) {
                        v = g_btlCamera.baseProjDist;
                    }
                    g_btlCamScript.zoomTo = v;
                    g_btlCamScript.pc += 4;
                }
                if (flags & 0x1000) {
                    g_btlCamScript.shakeFrom = g_btlCamScript.pc[0];
                    g_btlCamScript.shakeTo = g_btlCamScript.pc[1];
                    g_btlCamScript.shakeFrames = g_btlCamScript.shakeLeft = g_btlCamScript.pc[2];
                    g_btlCamScript.shakeAxes = g_btlCamScript.pc[3] & 3;
                    g_btlCamScript.pc += 4;
                }
                if (flags & 0x2000) {
                    g_btlCamera.unk53 = g_btlCamera.unkA8;
                }
                if (flags & 0x4000) {
                    c = *g_btlCamScript.pc;
                    if (c == 1) {
                        g_btlCamera.unk67 = c;
                    } else {
                        g_gameState->gameData->unk8E4.battle->subPhase++;
                    }
                    g_btlCamScript.pc += 2;
                }
                if (flags & 0x8000) {
                    btlcam_finishPaths();
                    for (j = 0; j < 2; j++) {
                        g_btlCamera.unk68[j].unk1B = 1;
                        g_btlCamera.unk68[j].unk18 = (g_btlCamScript.pc[0] >> 5) & 3;
                        g_btlCamera.unk68[j].unk1C = g_btlCamera.unk68[j].unk1D = g_btlCamScript.pc[1];
                        g_btlCamera.unk68[j].unk1A = g_btlCamScript.pc[2];
                        g_btlCamera.unk68[j].unk19 = 0;
                    }
                    g_btlCamera.unkA8 = g_btlCamScript.unk18;
                    btlcam_loadScript(btl_getSceneData(1));
                    g_btlCamera.projDist = g_btlCamera.baseProjDist;
                    return 1;
                }
                if (flags & 0x80000) {
                    g_btlCamScript.unk45 = 1;
                }
            }
            if (((u16 *)g_btlCamScript.pc)[0] == 0) {
                g_btlCamScript.pc = NULL;
            }
        }
        if (g_btlCamera.unk68[0].unk1D != 0) {
            g_btlCamera.unk68[0].unk1D--;
        }
        if (g_btlCamera.unk68[1].unk1D != 0) {
            g_btlCamera.unk68[1].unk1D--;
        }
    }
    if (g_btlCamera.unk68[0].unk18 != 0 && g_btlCamera.unk68[1].unk18 != 0) {
        g_btlCamera.unk66 = 0;
        g_btlCamera.unkBB = 0;
        btlcam_evalPath(0, &g_btlCamera.unk68[0], &g_btlCamera.eye.vx);
        btlcam_evalPath(0, &g_btlCamera.unk68[1], &sv.vx);
        if (g_btlCamScript.unk44 == 1) {
            g_btlCamScript.unk44 = 2;
            bcopy(&g_btlCamera.eye, &g_btlCamera.points[4], 8);
            bcopy(&sv, &g_btlCamera.points[5], 8);
        }
        btlcam_applyTargetView(&g_btlCamera.eye, &sv);
        btlcam_applySway(&g_btlCamera.eye.vx, &sv.vx);
        btlcam_applyShake(&sv.vx);
        btlcam_lookAt(&g_gameState->gameData->unk18, &g_btlCamera.eye, &sv);
        g_btlCamera.yaw = -ratan2(g_btlCamera.eye.vx - sv.vx, g_btlCamera.eye.vz - sv.vz) - 0x800;
    }
    if (g_btlCamScript.zoomLeft != 0) {
        g_btlCamScript.zoomLeft--;
        r = btlfx_lerpRatio(g_btlCamScript.zoomTo, g_btlCamScript.zoomFrom, g_btlCamScript.zoomLeft, g_btlCamScript.zoomFrames);
        if (g_btlCamScript.zoomTarget & 1) {
            g_btlCamera.baseProjDist = r;
        }
        if (g_btlCamScript.zoomTarget & 2) {
            g_btlCamera.projDist = r;
        }
    }
    if (g_btlCamera.unk66 == 0) {
        g_gameState->gameData->unkA = g_btlCamera.projDist;
    }
    SetGeomScreen(g_gameState->gameData->unkA);
    e = g_btlCamera.unk67;
    if (e == 1) {
        b = g_gameState->gameData->unk8E4.battle;
        if ((b->loadFlags & 7) == 7) {
            if ((b->scene.unk0 >> 2) & 1) {
                b->phase = e;
            } else {
                g_gameState->gameData->unk8E4.battle->phase = BTL_PHASE_ENTRANCE;
            }
            g_btlCamera.unk67 = 0;
        }
    }
    if (g_btlCamScript.frame < 0x3FF && g_btlCamera.unk52 == 0) {
        g_btlCamScript.frame++;
    }
    return 0;
}

void btlcam_setPathKey(Unk800C412C *arg0, Unk800C3F78 *arg1) {
    BtlUnit *u;
    Unk800F5758 *t;
    s32 idx;
    s32 k;
    Unk800F5758 *tb;

    bcopy(&arg0->unk0[1], &arg0->unk0[0], 0xC);
    arg0->unk0[1].unk8 = arg1->unk0;
    arg0->unk0[1].unk9 = arg1->unk1;
    if (arg1->unk2_14 != 0) {
        k = arg1->unk0 & 0x1F;
        /* The ternary after the range test gives the original's beq 1 / bne 5 / j through jump threading
           (a switch emits beq 1 / beq 5 / j other). */
        if (k != 1 && k != 5) {
            goto other;
        }
        u = (k == 1) ? g_btlCamTargets.caster : g_btlCamTargets.target;
        if (u->flags & 1) {
            idx = ((g_gameState->gameData->characters[(u->flags >> 1) & 0xF].unk34.w >> 4) & 0x1F) +
                  (arg1->unk2_14 - 1) * 19;
            tb = D_800F5758;
            t = &tb[idx];
            arg0->unk0[1].unk0[3] = t->unk0 << 6;
            arg0->unk0[1].unk0[0] = t->unk1 << 4;
            arg0->unk0[1].unk0[1] = t->unk2 << 4;
            arg0->unk0[1].unk0[2] = t->unk3 << 4;
            return;
        }
    }
other:
    arg0->unk0[1].unk0[0] = arg1->unk2_0 << 5;
    arg0->unk0[1].unk0[1] = arg1->unk2_7 << 5;
    arg0->unk0[1].unk0[2] = arg1->unk4_0 << 5;
    arg0->unk0[1].unk0[3] = arg1->unk4_7 << 7;
}

void btlcam_setPathTiming(Unk800C412C *arg0, u8 *arg1) {
    if (arg0->unk1B == 0) {
        arg0->unk1C = arg0->unk1D = arg1[0];
        arg0->unk1A = arg1[2];
        arg0->unk19 = arg1[3];
    }
}

void btlcam_evalPath(s32 arg0, Unk800C412C *arg1, s16 *arg2) {
    s32 coef[4];
    SVECTOR v[2];
    SVECTOR out;
    SVECTOR w[2];
    s32 wa[2];
    s32 wb[2];
    s32 z;
    s32 i;
    s32 t;
    s32 k;
    s32 d;
    s16 *a;
    s16 *b;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 h;

    if ((u32)(arg1->unk18 - 2) < 2 && arg1->unk1D == 0) {
        arg1->unk18 = 1;
    }
    if (arg1->unk18 >= 2) {
        t = arg1->unk1A;
        for (i = 0; i < 2; i++) {
            if (arg1->unk1C == 0 || arg1->unk1D == 0) {
                wa[i] = 0;
            } else {
                k = (u32)((0x1000 - arg1->unk1C * g_btlCamScript.unk8) << 17) / (u32)(arg1->unk1C * arg1->unk1C);
                switch (t & 3) {
                case 0:
                    wa[i] = (0x10000000 / arg1->unk1C * arg1->unk1D) >> 16;
                    break;
                case 1:
                    d = arg1->unk1C - arg1->unk1D;
                    wa[i] = 0x1000 - ((k * d * d) >> 17) - d * g_btlCamScript.unk8;
                    break;
                case 2:
                    d = arg1->unk1D;
                    wa[i] = ((k * d * d) >> 17) + d * g_btlCamScript.unk8;
                    break;
                }
            }
            wb[i] = 0x1000 - wa[i];
            t >>= 2;
        }
    }
    for (i = 0; i < 2; i++) {
        v[i].vx = arg1->unk0[i].unk0[0];
        v[i].vy = arg1->unk0[i].unk0[1];
        v[i].vz = arg1->unk0[i].unk0[2];
        v[i].pad = arg1->unk0[i].unk0[3];
    }
    if (arg1->unk18 >= 2) {
        t = arg1->unk19;
        a = &v[0].vx;
        b = &v[1].vx;
        for (i = 0; i < 3; i++) {
            switch (t & 3) {
            case 0:
                if (*a != *b) {
                    if ((*a - *b) & 0x800) {
                        if (*a >= *b) {
                            *b += 0x1000;
                        }
                    } else if (*a <= *b) {
                        *a += 0x1000;
                    }
                }
                break;
            case 1:
                if (*a >= *b) {
                    *b += 0x1000;
                }
                break;
            case 2:
                if (*a <= *b) {
                    *a += 0x1000;
                }
                break;
            }
            a++;
            b++;
            t >>= 2;
        }
    }
    LoadAverageShort12(&v[0], &v[1], wa[0], wb[0], &out);
    switch (arg1->unk18) {
    case 2:
        out.pad = btlfx_lerp(v[0].pad, v[1].pad, wb[1]);
        if (arg0 != 0) {
            arg2[0] = out.vx;
            arg2[1] = out.vy;
            arg2[2] = out.vz;
            arg2[3] = out.pad;
            return;
        }
        func_800C4B9C(arg1->unk0[1].unk8, arg1->unk0[1].unk9, &out.vx, arg2);
        goto tail;
    case 3:
        if (arg0 != 0) {
            for (i = 0; i < 2; i++) {
                func_800C4B9C(arg1->unk0[i].unk8, arg1->unk0[i].unk9, &v[i].vx, &w[i].vx);
            }
            LoadAverageShort12(&w[0], &w[1], wa[0], wb[0], &w[1]);
            func_800C4DC4(arg1->unk0[1].unk8, arg1->unk0[1].unk9, &w[0].vx, &v[0].vx, &z);
            dx = w[1].vx - w[0].vx;
            dy = w[1].vy - w[0].vy;
            dz = w[1].vz - w[0].vz;
            h = dx * dx + dz * dz;
            arg2[1] = (ratan2(dx, dz) + 0x1800) & 0xFFF;
            arg2[0] = (ratan2(dy, btlfx_sqrt(h, 0x14)) + 0x1000) & 0xFFF;
            arg2[2] = out.vz;
            arg2[3] = btlfx_sqrt(h + dy * dy, 0x14);
            arg1->unk0[1].unk9 |= 0xC0;
            return;
        }
        for (i = 0; i < 2; i++) {
            func_800C4B9C(arg1->unk0[i].unk8, arg1->unk0[i].unk9, &v[i].vx, &w[i].vx);
        }
        LoadAverageShort12(&w[0], &w[1], wa[0], wb[0], (SVECTOR *)arg2);
    tail:
        arg2[3] = out.vz;
        if (arg1->unk1D == 0) {
            arg1->unk18 = 1;
        }
        break;
    case 4:
        wa[0] >>= 4;
        wb[0] >>= 4;
        coef[0] = (wa[0] * wa[0] * wa[0]) >> 12;
        coef[1] = (wb[0] * wa[0] * wa[0] * 6) >> 12;
        coef[2] = (wb[0] * wb[0] * wa[0] * 6) >> 12;
        coef[3] = (wb[0] * wb[0] * wb[0]) >> 12;
        arg2[0] = 0;
        arg2[1] = 0;
        arg2[2] = 0;
        arg2[3] = out.vz;
        for (i = 0; i < 4; i++) {
            arg2[0] += (g_btlCamera.points[i].vx * coef[i]) >> 12;
            arg2[1] += (g_btlCamera.points[i].vy * coef[i]) >> 12;
            arg2[2] += (g_btlCamera.points[i].vz * coef[i]) >> 12;
        }
        break;
    default:
        func_800C4B9C(arg1->unk0[1].unk8, arg1->unk0[1].unk9, &v[1].vx, arg2);
        arg2[3] = v[1].vz;
        if ((u32)((arg1->unk0[1].unk8 & 0x1F) - 0x19) < 2) {
            g_btlCamera.unkBB++;
        }
        break;
    }
}

void btlcam_applyShake(s16 *arg0) {
    s32 t;
    if (g_btlCamScript.shakeLeft != 0) {
        g_btlCamScript.shakeLeft--;
        t = btlfx_lerpRatio(g_btlCamScript.shakeTo, g_btlCamScript.shakeFrom, g_btlCamScript.shakeLeft, g_btlCamScript.shakeFrames) * 2;
        if (t != 0) {
            if (!(g_btlCamScript.shakeAxes & 1)) {
                arg0[1] += btlfx_randRange(0, t) * 4 - t;
            }
            if (!(g_btlCamScript.shakeAxes & 2)) {
                arg0[0] += btlfx_randRange(0, t) * 4 - t;
                arg0[2] += btlfx_randRange(0, t) * 4 - t;
            }
        }
    }
}

void btlcam_applySway(s16 *arg0, s16 *arg1) {
    s32 dx = arg0[0] - arg1[0];
    s32 dz = arg0[2] - arg1[2];
    s32 d = btlfx_sqrt(dx * dx + dz * dz, 0xF);
    u32 v = 0;
    if (g_btlCamScript.sway != 0) {
        v = ((math_sin(g_btlCamScript.swayPhase << 6) * d) >> 13) * (g_btlCamScript.sway & 0xF0) >> 11;
        g_btlCamScript.swayPhase += g_btlCamScript.sway & 0xF;
    } else if (g_btlCamera.unkAC != 0) {
        v = ((math_sin(g_btlCamera.unkB0 << 6) * d) >> 13) * (g_btlCamera.unkAC & 0xF0) >> 11;
        g_btlCamera.unkB0 += g_btlCamera.unkAC & 0xF;
    }
    arg0[1] += v;
    arg1[1] += v;
}

void func_800C4B9C(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3) {
    s16 a[4];
    s16 b[4];
    s32 c;
    s32 t;
    s32 ang;
    BtlUnit *u;

    func_800C4DC4(arg0, arg1, a, b, &c);
    if (arg1 & 0x80) {
        c = arg2[3];
    } else {
        c += arg2[3];
    }
    if (arg1 & 0x40) {
        t = (math_cos(arg2[0]) * c) >> 12;
        arg3[0] = (-math_sin(arg2[1]) * t) >> 12;
        arg3[2] = (-math_cos(arg2[1]) * t) >> 12;
        arg3[1] = (math_sin(arg2[0]) * c) >> 12;
    } else {
        t = (math_cos(arg2[0] + b[0]) * c) >> 12;
        ang = arg2[1] + b[1];
        u = (BtlUnit *)btlcam_getAnchorUnit(arg0 & 0x1F);
        if (u != NULL && (u->flags & 1)) {
            ang ^= 0xFFF;
        }
        arg3[0] = (-math_sin(ang) * t) >> 12;
        arg3[2] = (-math_cos(ang) * t) >> 12;
        arg3[1] = (math_sin(arg2[0] + b[0]) * c) >> 12;
        arg2[2] += b[2];
    }
    arg3[0] += a[0];
    arg3[1] += a[1];
    arg3[2] += a[2];
}

void func_800C4DC4(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3, s32 *arg4) {
    s16 a[4], b[4];
    s32 c[2];
    func_800C4E84(arg0, arg2, arg3, arg4);
    if (arg1 & 0x1F) {
        arg1 = (arg0 & 0xC0) | (arg1 & 0x3F);
        func_800C4E84(arg1, a, b, c);
        arg2[0] = (s16)(arg2[0] + a[0]) >> 1;
        arg2[1] = (s16)(arg2[1] + a[1]) >> 1;
        arg2[2] = (s16)(arg2[2] + a[2]) >> 1;
    }
}

s32 func_800C4E84(s32 arg0, s16 *arg1, s16 *arg2, s32 *arg3) {
    s32 n;
    BtlUnit *u;
    s16 *s;
    GameState *gs;

    *arg3 = 0;
    n = arg0 & 0x1F;
    arg1[0] = 0;
    arg1[1] = 0;
    arg1[2] = 0;
    arg2[0] = 0;
    arg2[1] = 0;
    arg2[2] = 0;
    if (n == 0) {
        return;
    }
    if (n >= 0x15) {
        n -= 0x15;
        arg1[0] = g_btlCamera.points[n].vx;
        arg1[1] = g_btlCamera.points[n].vy;
        arg1[2] = g_btlCamera.points[n].vz;
        if (n < 4) {
            arg2[0] = g_btlCamera.unk30[n].vx;
            arg2[1] = g_btlCamera.unk30[n].vy;
            arg2[2] = g_btlCamera.unk30[n].vz;
        }
        return;
    }
    if (n == 0xB) {
        gs = g_gameState;
        arg1[0] = gs->gameData->unk8E4.battle->scene.unk4->unk8[0].unk4;
        arg1[2] = gs->gameData->unk8E4.battle->scene.unk4->unk8[0].unk8;
        return;
    }
    s = g_btlCamScript.casterAnchors;
    u = NULL;
    if (n == 1) {
        u = g_btlCamTargets.caster;
    }
    if (n == 0xA) {
        if (g_btlCamera.unkCC == NULL) {
            return;
        }
        u = g_btlCamera.unkCC;
    }
    if (n == 0xC) {
        u = g_gameState->gameData->unk8E4.battle->seq.camCaster;
    }
    if (n == 0xF) {
        u = g_btlCamTargets.partner;
        s = g_btlCamScript.partnerAnchors;
    }
    if (u != NULL) {
        if (arg0 & 0x20) {
            arg2[0] = u->obj->rotX;
            arg2[1] = u->obj->rotY;
            arg2[2] = u->obj->rotZ;
        } else {
            arg2[0] = u->rot[0];
            arg2[1] = u->rot[1];
            arg2[2] = u->rot[2];
        }
    } else {
        s = g_btlCamScript.targetAnchors;
        u = (BtlUnit *)btlcam_getAnchorUnit(n);
        if (u == NULL) {
            u = g_gameState->gameData->unk8E4.battle->unitList;
        }
        arg2[0] = u->obj->rotX;
        arg2[2] = u->obj->rotZ;
        if (u->flags & 1) {
            arg2[1] = 0x800;
        } else {
            arg2[1] = 0;
        }
    }
    if (!((u->flags >> 23) & 1)) {
        if (arg0 & 0x20) {
            arg1[0] = u->home->pos[0] >> 16;
            arg1[2] = u->home->pos[2] >> 16;
        } else {
            arg1[0] = u->pos[0] >> 16;
            arg1[2] = u->pos[2] >> 16;
        }
    } else {
        arg1[0] = s[3];
        arg1[2] = s[4];
    }
    if (n == 9) {
        arg1[0] = g_btlCamTargets.centerX;
        arg1[2] = g_btlCamTargets.centerZ;
    }
    if (n == 0xE) {
        gs = g_gameState;
        arg1[0] = gs->gameData->unk8E4.battle->seq.camCenterX;
        arg1[2] = gs->gameData->unk8E4.battle->seq.camCenterZ;
    }
    *arg3 = u->model->header->radius;
    arg1[1] = 0;
    if (n != 0xB) {
        arg0 &= 0xC0;
        if (arg0 == 0) {
            arg1[1] = s[1];
        }
        if (arg0 == 0x40) {
            arg1[1] = s[2];
        }
        if (arg0 == 0xC0) {
            arg1[1] = s[0];
        }
    }
    if ((u8)btl_hasStatus(u, BTL_STATUS_FLOAT)) {
        return (u16)arg1[1];
    }
}

void btlcam_lookAt(MATRIX *arg0, SVECTOR *arg1, SVECTOR *arg2) {
    VECTOR d;
    VECTOR x;
    VECTOR y;
    VECTOR z;
    VECTOR up;
    MATRIX m;
    SVECTOR r;

    up.vx = 0;
    up.vy = -30000;
    up.vz = 0;
    d.vx = arg2->vx - arg1->vx;
    d.vy = arg2->vy - arg1->vy;
    d.vz = arg2->vz - arg1->vz;
    VectorNormal(&d, &z);
    if (z.vz == up.vz) {
        z.vz++;
    }
    gte_ldopv1(&z);
    gte_ldopv2(&up);
    gte_op12();
    gte_stlvnl(&d);
    VectorNormal(&d, &x);
    gte_ldopv1(&z);
    gte_ldopv2(&x);
    gte_op12();
    gte_stlvnl(&d);
    VectorNormal(&d, &y);
    arg0->m[0][0] = x.vx;
    arg0->m[0][1] = x.vy;
    arg0->m[0][2] = x.vz;
    arg0->m[1][0] = y.vx;
    arg0->m[1][1] = y.vy;
    arg0->m[1][2] = y.vz;
    arg0->m[2][0] = z.vx;
    arg0->m[2][1] = z.vy;
    arg0->m[2][2] = z.vz;
    gte_SetRotMatrix(arg0);
    gte_ldv0(arg1);
    gte_rtv0();
    gte_stlvnl(arg0->t);
    arg0->t[0] = -arg0->t[0];
    arg0->t[1] = -arg0->t[1];
    arg0->t[2] = -arg0->t[2];
    r.vx = 0;
    r.vy = 0;
    r.vz = arg1->pad;
    math_rotMatrixXYZ(&r, &m);
    m.t[0] = 0;
    m.t[1] = 0;
    m.t[2] = 0;
    CompMatrix(&m, arg0, arg0);
}

void btlcam_applyTargetView(SVECTOR *arg0, SVECTOR *arg1) {
    s32 a, b;
    if (g_btlCamScript.unkC != 0) {
        g_btlCamScript.unkC--;
    }
    if (g_btlCamera.unkB4 != 0) {
        g_btlCamera.unkB4--;
    }
    if ((u8)func_800EA094() != 1 && ((u8)btlui_isTargetSelect() || g_btlCamScript.unkC != 0)) {
        if (g_btlCamera.unkBA != 1) {
            g_btlCamera.unkBA = 1;
            g_btlCamera.unkB4 = 2;
            g_btlCamScript.unkC = 0;
            bcopy(arg0, &g_btlCamera.unkBC, 8);
            bcopy(arg1, &g_btlCamera.unkC4, 8);
        }
        if (g_btlCamera.unkBA == 1 && ((g_btlCamera.unkB9 == 1 && !(g_btlfxEnv->state->flags & 8)) || g_btlCamera.unkB9 == 2)) {
            if (g_btlCamera.unkBB != 2) {
                if (g_btlCamera.unk65 != 0 || g_btlCamera.unkB4 == 0) {
                    bcopy(&g_btlCamera.points[4], arg0, 8);
                    bcopy(&g_btlCamera.points[5], arg1, 8);
                    g_btlCamera.unkBB = 2;
                } else {
                    a = g_btlCamera.unkB4 * 0x555;
                    b = 0x1000 - a;
                    LoadAverageShort12(&g_btlCamera.unkBC, &g_btlCamera.points[4], a, b, arg0);
                    LoadAverageShort12(&g_btlCamera.unkC4, &g_btlCamera.points[5], a, b, arg1);
                }
            }
            g_gameState->gameData->unkA = g_btlCamera.baseProjDist;
            g_btlCamera.unk66 = 1;
        }
    } else {
        g_btlCamera.unkBA = 0;
    }
    g_btlCamera.unk65 = g_btlCamera.unkBB;
}

void btlcam_getUnitAnchors(BtlUnit *arg0, s16 *arg1) {
    s32 i;
    if (arg0->flags & 1) {
        s32 h = arg0->model->header->height;
        s16 y = arg0->pos[1] >> 16;
        arg1[0] = y - (h >> 2);
        arg1[1] = y - (h >> 1);
        arg1[2] = y - (h - (h >> 2));
    } else {
        for (i = 0; i < 3; i++) {
            arg1[i] = arg0->model->bones[g_gameState->gameData->unk8E4.battle->enemies[(arg0->flags >> 1) & 0xF].type->bones[i]].unk18;
        }
    }
    arg1[3] = arg0->pos[0] >> 16;
    arg1[4] = arg0->pos[2] >> 16;
}

s32 btlcam_getAnchorUnit(s32 arg0) {
    s32 ret = 0;
    if (arg0 == 5 || arg0 == 9) {
        ret = (s32)g_btlCamTargets.target;
    }
    if ((u32)(arg0 - 0xD) < 2) {
        ret = g_gameState->gameData->unk8E4.battle->seq.camTarget;
    }
    return ret;
}
