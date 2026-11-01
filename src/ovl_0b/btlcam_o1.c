#include "ovl_0b.h"

s32 btlcam_getNextVariant(void) {
    return g_btlCamera.nextVariant;
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
    g_btlCamera.flags = 0;
    g_btlCamera.effectPick = btlfx_rand() & 1;
    if (g_btlfxEnv->state->flags & BTLFX_HDR_PARTNER) {
        btlcam_getUnitAnchors(g_btlCamTargets.partner, g_btlCamScript.partnerAnchors);
    }
    btlcam_getUnitAnchors(g_btlCamTargets.caster, g_btlCamScript.casterAnchors);
    btlcam_getUnitAnchors(g_btlCamTargets.target, g_btlCamScript.targetAnchors);
}

void btlcam_playScene(s32 script) {
    g_btlCamera.mode = BTLCAM_MODE_SCENE;
    g_btlCamera.paused = 0;
    g_btlCamera.flags = 0;
    bzero(&g_btlCamScript, 0x64);
    bzero(&g_btlCamTargets, 0x4C);
    btlcam_loadScript(script);
    if (g_btlCamera.focusUnit != 0) {
        btlcam_getUnitAnchors(g_btlCamera.focusUnit, g_btlCamScript.casterAnchors);
    }
}

void btlcam_play(s32 script) {
    g_btlCamera.mode = BTLCAM_MODE_EFFECT;
    g_btlCamera.paused = 0;
    g_btlCamScript.accel = 0;
    if (g_btlCamera.flags & BTLCAM_F_EFFECT_RUNNING) {
        g_btlCamera.flags = 0;
    }
    btlcam_finishPaths();
    btlcam_loadScript(script);
}

void btlcam_playSeq(s32 script) {
    bzero(&g_btlCamScript, 0x64);
    g_btlCamera.effectPick = btlfx_rand() & 1;
    btlcam_getUnitAnchors(g_gameState->gameData->ctx.battle->seq.camCaster, g_btlCamScript.casterAnchors);
    btlcam_getUnitAnchors((BtlUnit *)g_gameState->gameData->ctx.battle->seq.camTarget, g_btlCamScript.targetAnchors);
    btlcam_play(script);
    g_btlCamera.mode = BTLCAM_MODE_SEQ;
    g_btlCamera.flags = 0;
}

void btlcam_focusCaster(void) {
    BtlUnit *unit = g_btlfxEnv->targets->caster;
    if (unit->flags & BTL_UNIT_PARTY) {
        g_btlCamera.focusUnit = unit;
    }
}

void btlcam_endEffect(void) {
    if (g_btlCamScript.keepCamera == 0) {
        g_btlCamera.variant = g_btlCamScript.savedVariant;
        btlcam_loadScript(btl_getSceneData(1));
        g_btlCamera.projDist = g_btlCamera.baseProjDist;
    }
    g_btlCamera.flags = 1;
    g_btlCamScript.keepCamera = 0;
}

void btlcam_loadScript(s32 arg) {
    s16 *script = (s16 *)arg;
    u16 *variantPcs[4];
    s32 hdrFlags;
    s32 flags;
    u16 *ofs;
    u16 *sel;
    u8 *data;
    s32 i;
    s32 tmp;
    s32 val;
    BtlUnit *unit;
    BtlUnit *target;
    BtlModelHeader *hdr;

    g_btlCamScript.frame = 1;
    hdrFlags = flags = script[0];
    ofs = (u16 *)script + 1;
    for (i = 0; i < 4; i++) {
        variantPcs[i] = NULL;
        if (flags & 1) {
            variantPcs[i] = (u16 *)((u8 *)script + *ofs);
            ofs++;
        }
        flags >>= 1;
    }
    data = (u8 *)script + *ofs;
    for (i = 0; i < 4; i++) {
        if (flags & 1) {
            bcopy(data, &g_btlCamera.points[i], 6);
            data += 6;
        } else {
            g_btlCamera.points[i].vx = 0;
            g_btlCamera.points[i].vy = 0;
            g_btlCamera.points[i].vz = 0;
        }
        g_btlCamera.pointRots[i].vx = 0;
        g_btlCamera.pointRots[i].vy = 0;
        g_btlCamera.pointRots[i].vz = 0;
        flags >>= 1;
    }
    if (variantPcs[3] != NULL) {
        sel = variantPcs[3];
        switch (*sel >> 14) {
        case 0:
            g_btlCamera.variant = (*sel >> 12) & 3;
            break;
        case 1:
            val = btlfx_randRange(0, 9);
            if (!(*sel & 0x3000)) {
                if (val < ((*sel >> 10) & 3) + 5) {
                    g_btlCamera.variant = 0;
                } else {
                    g_btlCamera.variant = 1;
                }
            } else {
                tmp = ((*sel >> 10) & 3) + 3;
                if (val < tmp) {
                    g_btlCamera.variant = 0;
                } else if (val < tmp + 3) {
                    g_btlCamera.variant = 1;
                } else {
                    g_btlCamera.variant = 2;
                }
            }
            break;
        case 2:
            switch ((*sel >> 12) & 3) {
            case 0:
                tmp = g_gameState->gameData->ctx.battle->scene.flags & BTL_SCENE_AMBUSH;
                if (tmp == BTL_AMBUSH_NORMAL) {
                    g_btlCamera.variant = tmp;
                } else {
                    g_btlCamera.variant = btlfx_randRange(0, 1);
                }
                g_btlCamScript.savedVariant = g_btlCamera.variant;
                break;
            case 1:
                g_btlCamera.variant = g_gameState->gameData->ctx.battle->cameraNo;
                break;
            case 2:
                target = g_btlCamTargets.target;
                tmp = 0;
                val = tmp;
                if (target != NULL) {
                    hdr = target->model->header;
                    val = hdr->height;
                    tmp = hdr->radius;
                }
                if (tmp == 0x4D7 || tmp >= 0xBB8) {
                    g_btlCamera.variant = 2;
                } else if (tmp < 0x65) {
                    g_btlCamera.variant = 0;
                } else {
                    g_btlCamera.variant = 1;
                    if (val < 0x159) {
                        g_btlCamera.variant = 0;
                    }
                    if (val >= 0x3E9) {
                        g_btlCamera.variant = 2;
                    }
                }
                break;
            case 3:
                if (g_btlCamera.focusUnit == NULL) {
                    g_btlCamera.variant = 0;
                } else if ((g_btlCamera.focusUnit->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_JUMP)) || !((g_btlRewards.expMask >> ((u8)g_btlCamera.focusUnit->flags >> BTL_UNIT_SLOT_SHIFT)) & 1)) {
                    g_btlCamera.variant = 0;
                } else {
                    tmp = g_gameState->gameData->ctx.battle->scene.flags & BTL_SCENE_AMBUSH;
                    if (tmp == BTL_AMBUSH_NORMAL) {
                        g_btlCamera.variant = tmp;
                    } else {
                        val = 0;
                        for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
                            if ((unit->flags & BTL_UNIT_PARTY) && !(unit->status & (BTL_STATUS_KO | BTL_STATUS_JUMP))) {
                                val++;
                            }
                        }
                        if (val < 3) {
                            g_btlCamera.variant = btlfx_randRange(0, 1) + 1;
                        } else {
                            val = btlfx_randRange(0, 5);
                            g_btlCamera.variant = 0;
                            if (val == 4) {
                                g_btlCamera.variant = 1;
                            }
                            if (val == 5) {
                                g_btlCamera.variant = 2;
                            }
                        }
                    }
                }
                break;
            }
            break;
        case 3:
            g_btlCamera.variant = g_btlCamera.nextVariant & 3;
            if (g_btlCamera.variant == 2) {
                g_btlCamera.variant = 0;
            }
            if (g_btlCamera.variant == 3) {
                g_btlCamera.variant = 2;
            }
            break;
        }
    }
    g_btlCamScript.pc = (u8 *)variantPcs[g_btlCamera.variant];
    if (hdrFlags & 0x200) {
        g_btlCamera.nextVariant = (hdrFlags >> (g_btlCamera.variant * 2 + 10)) & 3;
    }
}

void btlcam_finishPaths(void) {
    s32 i;
    s32 running;
    for (i = 0; i < 2; i++) {
        running = g_btlCamera.paths[i].framesLeft != 0;
        if (running && (u32)(g_btlCamera.paths[i].mode - 2) < 2) {
            btlcam_evalPath(1, &g_btlCamera.paths[i], g_btlCamera.paths[i].keys[1].pos);
            g_btlCamera.paths[i].mode = BTLCAM_PATH_HOLD;
        }
    }
}

void btlcam_update(void) {
    if (g_btlCamera.stepped == 0 && btlcam_step() != 0) {
        btlcam_step();
    }
    g_btlCamera.stepped = 0;
}

s32 btlcam_step(void) {
    SVECTOR sv;
    SVECTOR sv2;
    s32 cmdIdx;
    s32 flags;
    s32 i;
    s32 pathMode;
    s32 zoomDist;
    s32 dist;
    u16 *cmd;
    s32 arg;
    s32 j;
    s32 introEnd;
    Battle *battle;

    if (g_btlCamScript.frame != 0) {
        if (g_btlCamScript.pc != NULL) {
            for (cmdIdx = 0; cmdIdx < 10; cmdIdx++) {
                cmd = (u16 *)g_btlCamScript.pc;
                if (g_btlCamScript.frame != (cmd[0] & 0x3FF)) {
                    break;
                }
                if (!(g_btlCamera.flags & 1)) {
                    g_btlCamera.flags |= 1;
                    g_btlCamera.projDist = 0x200;
                }
                flags = cmd[1] | ((cmd[0] >> 10) << 16);
                g_btlCamScript.pc += 4;
                for (i = 0; i < 2; i++) {
                    pathMode = (flags >> (i * 3)) & 7;
                    if ((u32)(pathMode - 2) < 2 && (u32)(g_btlCamera.paths[i].mode - 2) < 2 && g_btlCamera.paths[i].framesLeft != 0) {
                        btlcam_evalPath(1, &g_btlCamera.paths[i], g_btlCamera.paths[i].keys[1].pos);
                    }
                    if (pathMode > 0 && g_btlCamera.paths[i].locked == 0) {
                        g_btlCamera.paths[i].mode = pathMode;
                    }
                    if ((u32)(pathMode - 1) < 3) {
                        btlcam_setPathKey(&g_btlCamera.paths[i], (Unk800C3F78 *)g_btlCamScript.pc);
                        g_btlCamScript.pc += 6;
                    }
                    if ((u32)(pathMode - 2) < 3) {
                        btlcam_setPathTiming(&g_btlCamera.paths[i], g_btlCamScript.pc);
                        if (pathMode == 4) {
                            g_btlCamera.paths[i].keys[1].pos[2] = g_btlCamScript.pc[1] << 5;
                        }
                        g_btlCamScript.pc += 4;
                    }
                    g_btlCamera.paths[i].locked = 0;
                }
                if (flags & 0x40) {
                    g_btlCamScript.accel = *g_btlCamScript.pc;
                    g_btlCamScript.pc += 2;
                }
                if (flags & 0x80) {
                    g_btlCamScript.saveView = 1;
                }
                if (flags & 0x100100) {
                    g_btlCamera.paused = 1;
                }
                if (flags & 0x200) {
                    g_btlCamScript.sway = *g_btlCamScript.pc;
                    g_btlCamScript.pc += 2;
                }
                if (flags & 0x400) {
                    g_btlCamera.idleSway = *g_btlCamScript.pc;
                    g_btlCamScript.pc += 2;
                }
                if (flags & 0x800) {
                    g_btlCamScript.zoomFrames = g_btlCamScript.zoomLeft = g_btlCamScript.pc[0];
                    g_btlCamScript.zoomTarget = g_btlCamScript.pc[1];
                    g_btlCamScript.zoomFrom = g_btlCamera.projDist;
                    zoomDist = ((u16 *)g_btlCamScript.pc)[1];
                    if (zoomDist == 0) {
                        zoomDist = g_btlCamera.baseProjDist;
                    }
                    g_btlCamScript.zoomTo = zoomDist;
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
                    g_btlCamera.effectPick = g_btlCamera.variant;
                }
                if (flags & 0x4000) {
                    arg = *g_btlCamScript.pc;
                    if (arg == 1) {
                        g_btlCamera.endIntro = arg;
                    } else {
                        g_gameState->gameData->ctx.battle->subPhase++;
                    }
                    g_btlCamScript.pc += 2;
                }
                if (flags & 0x8000) {
                    btlcam_finishPaths();
                    for (j = 0; j < 2; j++) {
                        g_btlCamera.paths[j].locked = 1;
                        g_btlCamera.paths[j].mode = (g_btlCamScript.pc[0] >> 5) & 3;
                        g_btlCamera.paths[j].frames = g_btlCamera.paths[j].framesLeft = g_btlCamScript.pc[1];
                        g_btlCamera.paths[j].easeModes = g_btlCamScript.pc[2];
                        g_btlCamera.paths[j].wrapModes = 0;
                    }
                    g_btlCamera.variant = g_btlCamScript.savedVariant;
                    btlcam_loadScript(btl_getSceneData(1));
                    g_btlCamera.projDist = g_btlCamera.baseProjDist;
                    return 1;
                }
                if (flags & 0x80000) {
                    g_btlCamScript.keepCamera = 1;
                }
            }
            if (((u16 *)g_btlCamScript.pc)[0] == 0) {
                g_btlCamScript.pc = NULL;
            }
        }
        if (g_btlCamera.paths[0].framesLeft != 0) {
            g_btlCamera.paths[0].framesLeft--;
        }
        if (g_btlCamera.paths[1].framesLeft != 0) {
            g_btlCamera.paths[1].framesLeft--;
        }
    }
    if (g_btlCamera.paths[0].mode != BTLCAM_PATH_OFF && g_btlCamera.paths[1].mode != BTLCAM_PATH_OFF) {
        g_btlCamera.projFixed = 0;
        g_btlCamera.viewState = 0;
        btlcam_evalPath(0, &g_btlCamera.paths[0], &g_btlCamera.eye.vx);
        btlcam_evalPath(0, &g_btlCamera.paths[1], &sv.vx);
        if (g_btlCamScript.saveView == 1) {
            g_btlCamScript.saveView = 2;
            bcopy(&g_btlCamera.eye, &g_btlCamera.points[4], 8);
            bcopy(&sv, &g_btlCamera.points[5], 8);
        }
        btlcam_applyTargetView(&g_btlCamera.eye, &sv);
        btlcam_applySway(&g_btlCamera.eye.vx, &sv.vx);
        btlcam_applyShake(&sv.vx);
        btlcam_lookAt(&g_gameState->gameData->viewMatrix, &g_btlCamera.eye, &sv);
        g_btlCamera.yaw = -ratan2(g_btlCamera.eye.vx - sv.vx, g_btlCamera.eye.vz - sv.vz) - 0x800;
    }
    if (g_btlCamScript.zoomLeft != 0) {
        g_btlCamScript.zoomLeft--;
        dist = btlfx_lerpRatio(g_btlCamScript.zoomTo, g_btlCamScript.zoomFrom, g_btlCamScript.zoomLeft, g_btlCamScript.zoomFrames);
        if (g_btlCamScript.zoomTarget & 1) {
            g_btlCamera.baseProjDist = dist;
        }
        if (g_btlCamScript.zoomTarget & 2) {
            g_btlCamera.projDist = dist;
        }
    }
    if (g_btlCamera.projFixed == 0) {
        g_gameState->gameData->projDist = g_btlCamera.projDist;
    }
    SetGeomScreen(g_gameState->gameData->projDist);
    introEnd = g_btlCamera.endIntro;
    if (introEnd == 1) {
        battle = g_gameState->gameData->ctx.battle;
        if ((battle->loadFlags & BTL_LOAD_READY) == BTL_LOAD_READY) {
            if ((battle->scene.flags >> BTL_SCENE_SCRIPT_START_BIT) & 1) {
                battle->phase = introEnd;
            } else {
                g_gameState->gameData->ctx.battle->phase = BTL_PHASE_ENTRANCE;
            }
            g_btlCamera.endIntro = 0;
        }
    }
    if (g_btlCamScript.frame < 0x3FF && g_btlCamera.paused == 0) {
        g_btlCamScript.frame++;
    }
    return 0;
}

void btlcam_setPathKey(Unk800C412C *path, Unk800C3F78 *key) {
    BtlUnit *unit;
    Unk800F5758 *preset;
    s32 idx;
    s32 k;
    Unk800F5758 *presets;

    bcopy(&path->keys[1], &path->keys[0], 0xC);
    path->keys[1].anchor = key->anchor;
    path->keys[1].anchorFlags = key->anchorFlags;
    if (key->preset != 0) {
        k = key->anchor & 0x1F;
        /* The ternary after the range test gives the original's beq 1 / bne 5 / j through jump threading
           (a switch emits beq 1 / beq 5 / j other). */
        if (k != 1 && k != 5) {
            goto other;
        }
        unit = (k == 1) ? g_btlCamTargets.caster : g_btlCamTargets.target;
        if (unit->flags & BTL_UNIT_PARTY) {
            idx = ((g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)].info.w >> CHR_INFO_KIND_SHIFT) & 0x1F) +
                  (key->preset - 1) * CHR_KIND_COUNT;
            presets = g_btlCamPartyPresets;
            preset = &presets[idx];
            path->keys[1].pos[3] = preset->w << 6;
            path->keys[1].pos[0] = preset->x << 4;
            path->keys[1].pos[1] = preset->y << 4;
            path->keys[1].pos[2] = preset->z << 4;
            return;
        }
    }
other:
    path->keys[1].pos[0] = key->x << 5;
    path->keys[1].pos[1] = key->y << 5;
    path->keys[1].pos[2] = key->z << 5;
    path->keys[1].pos[3] = key->w << 7;
}

void btlcam_setPathTiming(Unk800C412C *path, u8 *timing) {
    if (path->locked == 0) {
        path->frames = path->framesLeft = timing[0];
        path->easeModes = timing[2];
        path->wrapModes = timing[3];
    }
}

void btlcam_evalPath(s32 finish, Unk800C412C *path, s16 *dst) {
    s32 coef[4];
    SVECTOR keyPos[2];
    SVECTOR out;
    SVECTOR w[2];
    s32 wa[2];
    s32 wb[2];
    s32 z;
    s32 i;
    s32 modes;
    s32 k;
    s32 steps;
    s16 *angA;
    s16 *angB;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 h;

    if ((u32)(path->mode - 2) < 2 && path->framesLeft == 0) {
        path->mode = BTLCAM_PATH_HOLD;
    }
    if (path->mode >= BTLCAM_PATH_LERP) {
        modes = path->easeModes;
        for (i = 0; i < 2; i++) {
            if (path->frames == 0 || path->framesLeft == 0) {
                wa[i] = 0;
            } else {
                k = (u32)((0x1000 - path->frames * g_btlCamScript.accel) << 17) / (u32)(path->frames * path->frames);
                switch (modes & 3) {
                case 0:
                    wa[i] = (0x10000000 / path->frames * path->framesLeft) >> 16;
                    break;
                case 1:
                    steps = path->frames - path->framesLeft;
                    wa[i] = 0x1000 - ((k * steps * steps) >> 17) - steps * g_btlCamScript.accel;
                    break;
                case 2:
                    steps = path->framesLeft;
                    wa[i] = ((k * steps * steps) >> 17) + steps * g_btlCamScript.accel;
                    break;
                }
            }
            wb[i] = 0x1000 - wa[i];
            modes >>= 2;
        }
    }
    for (i = 0; i < 2; i++) {
        keyPos[i].vx = path->keys[i].pos[0];
        keyPos[i].vy = path->keys[i].pos[1];
        keyPos[i].vz = path->keys[i].pos[2];
        keyPos[i].pad = path->keys[i].pos[3];
    }
    if (path->mode >= BTLCAM_PATH_LERP) {
        modes = path->wrapModes;
        angA = &keyPos[0].vx;
        angB = &keyPos[1].vx;
        for (i = 0; i < 3; i++) {
            switch (modes & 3) {
            case 0:
                if (*angA != *angB) {
                    if ((*angA - *angB) & 0x800) {
                        if (*angA >= *angB) {
                            *angB += 0x1000;
                        }
                    } else if (*angA <= *angB) {
                        *angA += 0x1000;
                    }
                }
                break;
            case 1:
                if (*angA >= *angB) {
                    *angB += 0x1000;
                }
                break;
            case 2:
                if (*angA <= *angB) {
                    *angA += 0x1000;
                }
                break;
            }
            angA++;
            angB++;
            modes >>= 2;
        }
    }
    LoadAverageShort12(&keyPos[0], &keyPos[1], wa[0], wb[0], &out);
    switch (path->mode) {
    case BTLCAM_PATH_LERP:
        out.pad = btlfx_lerp(keyPos[0].pad, keyPos[1].pad, wb[1]);
        if (finish != 0) {
            dst[0] = out.vx;
            dst[1] = out.vy;
            dst[2] = out.vz;
            dst[3] = out.pad;
            return;
        }
        btlcam_orbitAnchor(path->keys[1].anchor, path->keys[1].anchorFlags, &out.vx, dst);
        goto tail;
    case BTLCAM_PATH_ANCHORED:
        if (finish != 0) {
            for (i = 0; i < 2; i++) {
                btlcam_orbitAnchor(path->keys[i].anchor, path->keys[i].anchorFlags, &keyPos[i].vx, &w[i].vx);
            }
            LoadAverageShort12(&w[0], &w[1], wa[0], wb[0], &w[1]);
            btlcam_resolveAnchorPair(path->keys[1].anchor, path->keys[1].anchorFlags, &w[0].vx, &keyPos[0].vx, &z);
            dx = w[1].vx - w[0].vx;
            dy = w[1].vy - w[0].vy;
            dz = w[1].vz - w[0].vz;
            h = dx * dx + dz * dz;
            dst[1] = (ratan2(dx, dz) + 0x1800) & 0xFFF;
            dst[0] = (ratan2(dy, btlfx_sqrt(h, 0x14)) + 0x1000) & 0xFFF;
            dst[2] = out.vz;
            dst[3] = btlfx_sqrt(h + dy * dy, 0x14);
            path->keys[1].anchorFlags |= BTLCAM_AF_ABS_ANGLES | BTLCAM_AF_ABS_DIST;
            return;
        }
        for (i = 0; i < 2; i++) {
            btlcam_orbitAnchor(path->keys[i].anchor, path->keys[i].anchorFlags, &keyPos[i].vx, &w[i].vx);
        }
        LoadAverageShort12(&w[0], &w[1], wa[0], wb[0], (SVECTOR *)dst);
    tail:
        dst[3] = out.vz;
        if (path->framesLeft == 0) {
            path->mode = BTLCAM_PATH_HOLD;
        }
        break;
    case BTLCAM_PATH_CUBIC:
        wa[0] >>= 4;
        wb[0] >>= 4;
        coef[0] = (wa[0] * wa[0] * wa[0]) >> 12;
        coef[1] = (wb[0] * wa[0] * wa[0] * 6) >> 12;
        coef[2] = (wb[0] * wb[0] * wa[0] * 6) >> 12;
        coef[3] = (wb[0] * wb[0] * wb[0]) >> 12;
        dst[0] = 0;
        dst[1] = 0;
        dst[2] = 0;
        dst[3] = out.vz;
        for (i = 0; i < 4; i++) {
            dst[0] += (g_btlCamera.points[i].vx * coef[i]) >> 12;
            dst[1] += (g_btlCamera.points[i].vy * coef[i]) >> 12;
            dst[2] += (g_btlCamera.points[i].vz * coef[i]) >> 12;
        }
        break;
    default:
        btlcam_orbitAnchor(path->keys[1].anchor, path->keys[1].anchorFlags, &keyPos[1].vx, dst);
        dst[3] = keyPos[1].vz;
        if ((u32)((path->keys[1].anchor & 0x1F) - 0x19) < 2) {
            g_btlCamera.viewState++;
        }
        break;
    }
}

void btlcam_applyShake(s16 *at) {
    s32 amp;
    if (g_btlCamScript.shakeLeft != 0) {
        g_btlCamScript.shakeLeft--;
        amp = btlfx_lerpRatio(g_btlCamScript.shakeTo, g_btlCamScript.shakeFrom, g_btlCamScript.shakeLeft, g_btlCamScript.shakeFrames) * 2;
        if (amp != 0) {
            if (!(g_btlCamScript.shakeAxes & 1)) {
                at[1] += btlfx_randRange(0, amp) * 4 - amp;
            }
            if (!(g_btlCamScript.shakeAxes & 2)) {
                at[0] += btlfx_randRange(0, amp) * 4 - amp;
                at[2] += btlfx_randRange(0, amp) * 4 - amp;
            }
        }
    }
}

void btlcam_applySway(s16 *eye, s16 *at) {
    s32 dx = eye[0] - at[0];
    s32 dz = eye[2] - at[2];
    s32 dist = btlfx_sqrt(dx * dx + dz * dz, 0xF);
    u32 sway = 0;
    if (g_btlCamScript.sway != 0) {
        sway = ((math_sin(g_btlCamScript.swayPhase << 6) * dist) >> 13) * (g_btlCamScript.sway & 0xF0) >> 11;
        g_btlCamScript.swayPhase += g_btlCamScript.sway & 0xF;
    } else if (g_btlCamera.idleSway != 0) {
        sway = ((math_sin(g_btlCamera.idleSwayPhase << 6) * dist) >> 13) * (g_btlCamera.idleSway & 0xF0) >> 11;
        g_btlCamera.idleSwayPhase += g_btlCamera.idleSway & 0xF;
    }
    eye[1] += sway;
    at[1] += sway;
}

void btlcam_orbitAnchor(s32 anchor, s32 anchorFlags, s16 *offset, s16 *out) {
    s16 anchorPos[4];
    s16 anchorRot[4];
    s32 dist;
    s32 horiz;
    s32 ang;
    BtlUnit *unit;

    btlcam_resolveAnchorPair(anchor, anchorFlags, anchorPos, anchorRot, &dist);
    if (anchorFlags & BTLCAM_AF_ABS_DIST) {
        dist = offset[3];
    } else {
        dist += offset[3];
    }
    if (anchorFlags & BTLCAM_AF_ABS_ANGLES) {
        horiz = (math_cos(offset[0]) * dist) >> 12;
        out[0] = (-math_sin(offset[1]) * horiz) >> 12;
        out[2] = (-math_cos(offset[1]) * horiz) >> 12;
        out[1] = (math_sin(offset[0]) * dist) >> 12;
    } else {
        horiz = (math_cos(offset[0] + anchorRot[0]) * dist) >> 12;
        ang = offset[1] + anchorRot[1];
        unit = (BtlUnit *)btlcam_getAnchorUnit(anchor & 0x1F);
        if (unit != NULL && (unit->flags & BTL_UNIT_PARTY)) {
            ang ^= 0xFFF;
        }
        out[0] = (-math_sin(ang) * horiz) >> 12;
        out[2] = (-math_cos(ang) * horiz) >> 12;
        out[1] = (math_sin(offset[0] + anchorRot[0]) * dist) >> 12;
        offset[2] += anchorRot[2];
    }
    out[0] += anchorPos[0];
    out[1] += anchorPos[1];
    out[2] += anchorPos[2];
}

void btlcam_resolveAnchorPair(s32 anchor, s32 anchorFlags, s16 *pos, s16 *rot, s32 *radius) {
    s16 pos2[4], rot2[4];
    s32 radius2[2];
    btlcam_resolveAnchor(anchor, pos, rot, radius);
    if (anchorFlags & 0x1F) {
        anchorFlags = (anchor & 0xC0) | (anchorFlags & 0x3F);
        btlcam_resolveAnchor(anchorFlags, pos2, rot2, radius2);
        pos[0] = (s16)(pos[0] + pos2[0]) >> 1;
        pos[1] = (s16)(pos[1] + pos2[1]) >> 1;
        pos[2] = (s16)(pos[2] + pos2[2]) >> 1;
    }
}

s32 btlcam_resolveAnchor(s32 anchor, s16 *pos, s16 *rot, s32 *radius) {
    s32 kind;
    BtlUnit *unit;
    s16 *anchors;
    GameState *gs;

    *radius = 0;
    kind = anchor & 0x1F;
    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
    rot[0] = 0;
    rot[1] = 0;
    rot[2] = 0;
    if (kind == 0) {
        return;
    }
    if (kind >= 0x15) {
        kind -= 0x15;
        pos[0] = g_btlCamera.points[kind].vx;
        pos[1] = g_btlCamera.points[kind].vy;
        pos[2] = g_btlCamera.points[kind].vz;
        if (kind < 4) {
            rot[0] = g_btlCamera.pointRots[kind].vx;
            rot[1] = g_btlCamera.pointRots[kind].vy;
            rot[2] = g_btlCamera.pointRots[kind].vz;
        }
        return;
    }
    if (kind == 0xB) {
        gs = g_gameState;
        pos[0] = gs->gameData->ctx.battle->scene.group->enemies[0].x;
        pos[2] = gs->gameData->ctx.battle->scene.group->enemies[0].z;
        return;
    }
    anchors = g_btlCamScript.casterAnchors;
    unit = NULL;
    if (kind == 1) {
        unit = g_btlCamTargets.caster;
    }
    if (kind == 0xA) {
        if (g_btlCamera.focusUnit == NULL) {
            return;
        }
        unit = g_btlCamera.focusUnit;
    }
    if (kind == 0xC) {
        unit = g_gameState->gameData->ctx.battle->seq.camCaster;
    }
    if (kind == 0xF) {
        unit = g_btlCamTargets.partner;
        anchors = g_btlCamScript.partnerAnchors;
    }
    if (unit != NULL) {
        if (anchor & 0x20) {
            rot[0] = unit->obj->rotX;
            rot[1] = unit->obj->rotY;
            rot[2] = unit->obj->rotZ;
        } else {
            rot[0] = unit->rot[0];
            rot[1] = unit->rot[1];
            rot[2] = unit->rot[2];
        }
    } else {
        anchors = g_btlCamScript.targetAnchors;
        unit = (BtlUnit *)btlcam_getAnchorUnit(kind);
        if (unit == NULL) {
            unit = g_gameState->gameData->ctx.battle->unitList;
        }
        rot[0] = unit->obj->rotX;
        rot[2] = unit->obj->rotZ;
        if (unit->flags & BTL_UNIT_PARTY) {
            rot[1] = 0x800;
        } else {
            rot[1] = 0;
        }
    }
    if (!((unit->flags >> BTL_UNIT_MODEL_PART_BIT) & 1)) {
        if (anchor & 0x20) {
            pos[0] = unit->home->pos[0] >> 16;
            pos[2] = unit->home->pos[2] >> 16;
        } else {
            pos[0] = unit->pos[0] >> 16;
            pos[2] = unit->pos[2] >> 16;
        }
    } else {
        pos[0] = anchors[3];
        pos[2] = anchors[4];
    }
    if (kind == 9) {
        pos[0] = g_btlCamTargets.centerX;
        pos[2] = g_btlCamTargets.centerZ;
    }
    if (kind == 0xE) {
        gs = g_gameState;
        pos[0] = gs->gameData->ctx.battle->seq.camCenterX;
        pos[2] = gs->gameData->ctx.battle->seq.camCenterZ;
    }
    *radius = unit->model->header->radius;
    pos[1] = 0;
    if (kind != 0xB) {
        anchor &= 0xC0;
        if (anchor == 0) {
            pos[1] = anchors[1];
        }
        if (anchor == 0x40) {
            pos[1] = anchors[2];
        }
        if (anchor == 0xC0) {
            pos[1] = anchors[0];
        }
    }
    if ((u8)btl_hasStatus(unit, BTL_STATUS_FLOAT)) {
        return (u16)pos[1];
    }
}

void btlcam_lookAt(MATRIX *view, SVECTOR *eye, SVECTOR *at) {
    VECTOR raw;
    VECTOR x;
    VECTOR y;
    VECTOR z;
    VECTOR up;
    MATRIX rollMtx;
    SVECTOR rollRot;

    up.vx = 0;
    up.vy = -30000;
    up.vz = 0;
    raw.vx = at->vx - eye->vx;
    raw.vy = at->vy - eye->vy;
    raw.vz = at->vz - eye->vz;
    VectorNormal(&raw, &z);
    if (z.vz == up.vz) {
        z.vz++;
    }
    gte_ldopv1(&z);
    gte_ldopv2(&up);
    gte_op12();
    gte_stlvnl(&raw);
    VectorNormal(&raw, &x);
    gte_ldopv1(&z);
    gte_ldopv2(&x);
    gte_op12();
    gte_stlvnl(&raw);
    VectorNormal(&raw, &y);
    view->m[0][0] = x.vx;
    view->m[0][1] = x.vy;
    view->m[0][2] = x.vz;
    view->m[1][0] = y.vx;
    view->m[1][1] = y.vy;
    view->m[1][2] = y.vz;
    view->m[2][0] = z.vx;
    view->m[2][1] = z.vy;
    view->m[2][2] = z.vz;
    gte_SetRotMatrix(view);
    gte_ldv0(eye);
    gte_rtv0();
    gte_stlvnl(view->t);
    view->t[0] = -view->t[0];
    view->t[1] = -view->t[1];
    view->t[2] = -view->t[2];
    rollRot.vx = 0;
    rollRot.vy = 0;
    rollRot.vz = eye->pad;
    math_rotMatrixXYZ(&rollRot, &rollMtx);
    rollMtx.t[0] = 0;
    rollMtx.t[1] = 0;
    rollMtx.t[2] = 0;
    CompMatrix(&rollMtx, view, view);
}

void btlcam_applyTargetView(SVECTOR *eye, SVECTOR *at) {
    s32 wFrom, wTo;
    if (g_btlCamScript.viewHold != 0) {
        g_btlCamScript.viewHold--;
    }
    if (g_btlCamera.viewBlend != 0) {
        g_btlCamera.viewBlend--;
    }
    if ((u8)btl_isTargetWindowOpen() != 1 && ((u8)btlui_isTargetSelect() || g_btlCamScript.viewHold != 0)) {
        if (g_btlCamera.targetView != 1) {
            g_btlCamera.targetView = 1;
            g_btlCamera.viewBlend = 2;
            g_btlCamScript.viewHold = 0;
            bcopy(eye, &g_btlCamera.viewFromEye, 8);
            bcopy(at, &g_btlCamera.viewFromAt, 8);
        }
        if (g_btlCamera.targetView == 1 && ((g_btlCamera.mode == BTLCAM_MODE_EFFECT && !(g_btlfxEnv->state->flags & BTLFX_HDR_CAMERA)) || g_btlCamera.mode == BTLCAM_MODE_SEQ)) {
            if (g_btlCamera.viewState != 2) {
                if (g_btlCamera.prevViewState != 0 || g_btlCamera.viewBlend == 0) {
                    bcopy(&g_btlCamera.points[4], eye, 8);
                    bcopy(&g_btlCamera.points[5], at, 8);
                    g_btlCamera.viewState = 2;
                } else {
                    wFrom = g_btlCamera.viewBlend * 0x555;
                    wTo = 0x1000 - wFrom;
                    LoadAverageShort12(&g_btlCamera.viewFromEye, &g_btlCamera.points[4], wFrom, wTo, eye);
                    LoadAverageShort12(&g_btlCamera.viewFromAt, &g_btlCamera.points[5], wFrom, wTo, at);
                }
            }
            g_gameState->gameData->projDist = g_btlCamera.baseProjDist;
            g_btlCamera.projFixed = 1;
        }
    } else {
        g_btlCamera.targetView = 0;
    }
    g_btlCamera.prevViewState = g_btlCamera.viewState;
}

void btlcam_getUnitAnchors(BtlUnit *unit, s16 *anchors) {
    s32 i;
    if (unit->flags & BTL_UNIT_PARTY) {
        s32 h = unit->model->header->height;
        s16 y = unit->pos[1] >> 16;
        anchors[0] = y - (h >> 2);
        anchors[1] = y - (h >> 1);
        anchors[2] = y - (h - (h >> 2));
    } else {
        for (i = 0; i < 3; i++) {
            anchors[i] = unit->model->bones[g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)].type->bones[i]].ty;
        }
    }
    anchors[3] = unit->pos[0] >> 16;
    anchors[4] = unit->pos[2] >> 16;
}

s32 btlcam_getAnchorUnit(s32 anchor) {
    s32 unit = 0;
    if (anchor == 5 || anchor == 9) {
        unit = (s32)g_btlCamTargets.target;
    }
    if ((u32)(anchor - 0xD) < 2) {
        unit = g_gameState->gameData->ctx.battle->seq.camTarget;
    }
    return unit;
}
