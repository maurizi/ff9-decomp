#include "ovl_0c.h"

void wm_initCamera(void) {
    void *p = &g_gameState->gameData->worldCamera;

    g_worldCameraFlags = p;
    g_worldCamera = p;
    g_gameState->gameData->projDist = 0xDC;
    SetGeomOffset(0xA0, 0x70);
    g_worldCameraUp.vx = 0;
    g_worldCameraUp.vy = -0x4E20;
    g_worldCameraUp.vz = 0;
    g_worldCameraHold = 1;
    g_worldCameraLookupFix = 0;
    g_worldCameraRoll = 0;
    g_worldCameraInitFlag = 0;
    if (g_gameState->prevMode == GS_MODE_FIELD && g_gameState->gameData->worldCamKeep == 0) {
        g_worldCamera->viewBlend = 0;
        g_worldCamera->viewBlendSpeed = 0;
        g_worldCamera->viewForceNormal = 0;
        g_worldCamera->yaw = 0;
        g_worldCameraFlags->autoTurnOff = 0;
    }
    if (g_worldCamera->viewBlendSpeed < 0) {
        g_worldCamera->viewBlend = 0;
    }
    if (g_worldCamera->viewBlendSpeed > 0) {
        g_worldCamera->viewBlend = 0x1000;
    }
    g_worldCameraBlend = 0;
    g_worldCameraBlendSpeed = 0;
    g_worldCameraParams = g_worldCameraPresets[1];
    g_worldCameraSmoothX = 0;
    g_worldCameraSmoothY = 0;
    g_worldCameraSmoothZ = 0;
    g_worldCameraFourProbes = 1;
    g_worldCameraStartPhase = 0;
}

void wm_updateCamera(void) {
    SVECTOR r;
    VECTOR v;
    MATRIX m;
    s32 t;
    s32 a;

    if (g_worldCameraHold == 0) {
        wm_updateCameraPreset();
    }
    g_gameState->gameData->projDist = g_worldCameraParams.projDist;
    g_worldCamera->yaw &= 0xFFF;
    g_worldCameraTargetHeight = g_worldCameraParams.targetHeight;
    if ((u8)g_worldMoveMode == WM_MOVE_HILDA_GARDE || (u8)g_worldMoveMode == WM_MOVE_INVINCIBLE) {
        t = g_worldPlayer->pos.vy + 0x1518;
        g_worldCameraTargetHeight = g_worldCameraParams.targetHeight - (t > 0 ? t >> 3 : t / 6);
    }
    g_worldCameraEyeOffset.vx = math_sin(g_worldCamera->yaw) * (g_worldCameraParams.distance * g_worldCameraDistScale / 4096) >> 12;
    g_worldCameraEyeOffset.vy = g_worldCameraParams.eyeHeight;
    g_worldCameraEyeOffset.vz = math_cos(g_worldCamera->yaw) * (g_worldCameraParams.distance * g_worldCameraDistScale / 4096) >> 12;
    if (g_worldCameraHold == 0) {
        wm_cameraFollow();
    }
    g_worldCameraEyeRel.vx = g_worldCameraEye[0] - g_worldCameraTarget[0];
    g_worldCameraEyeRel.vy = g_worldCameraEye[1] - g_worldCameraTarget[1];
    g_worldCameraEyeRel.vz = g_worldCameraEye[2] - g_worldCameraTarget[2];
    memset(&r, 0, 8);
    r.vz = g_worldCameraRoll;
    r.vy = g_worldCamera->yaw;
    math_rotMatrixXYZ(&r, &m);
    m.t[0] = 0;
    m.t[1] = 0;
    m.t[2] = 0;
    ApplyMatrixLV(&m, &g_worldCameraUp, &v);
    wm_lookAt(g_worldViewMatrix, &g_worldCameraEyeRel, &g_worldCameraLookAt, &v);
    g_worldCameraDir[1] = 0;
    g_worldCameraDir[0] = g_worldCameraTarget[0] - g_worldCameraEye[0];
    g_worldCameraDir[2] = g_worldCameraTarget[2] - g_worldCameraEye[2];
    if (g_worldCameraDir[0] > 0xFFFF) {
        g_worldCameraDir[0] -= 0x60000;
    }
    if (g_worldCameraDir[2] > 0xFFFF) {
        g_worldCameraDir[2] -= 0x50000;
    }
    if (g_worldCameraDir[0] < -0xFFFF) {
        g_worldCameraDir[0] += 0x60000;
    }
    if (g_worldCameraDir[2] < -0xFFFF) {
        g_worldCameraDir[2] += 0x50000;
    }
    a = ratan2(g_worldCameraDir[2], g_worldCameraDir[0]);
    g_worldCameraDir[0] = math_cos(a);
    g_worldCameraDir[2] = math_sin(a);
    VectorNormal((VECTOR *)g_worldCameraDir, (VECTOR *)g_worldCameraDir);
    if (*g_worldStoryCounterPtr < 0x137E && WM_ATTR_AREA(g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr) == 0xC) {
        g_worldCamera->viewForceNormal = 1;
    }
}

void wm_cameraFollow(void) {
    VECTOR probe;
    u32 c0;
    s32 polyIdx;
    s32 h0;
    u32 c1;
    s32 h1;
    u32 c2;
    s32 h2;
    u32 c3;
    s32 h3;
    u32 cell;
    s32 h;
    s32 eyeY;
    s32 x;
    s32 y;
    s32 z;
    s32 dy;
    s32 dir;
    s32 step;
    s32 ease;
    s32 k;
    s32 x2, y2, z2, d2, step2, dir2;
    s32 dx, dz, s, r;
    u8 *p;

    eyeY = 0;
    if (evt_getWorldCamTargetObj() == 0) {
        y = g_worldPlayer->pos.vy + g_worldCameraTargetHeight;
        x = g_worldPlayer->pos.vx;
        z = g_worldPlayer->pos.vz;
        if ((g_worldCameraSmoothing = g_worldCameraSmoothX | g_worldCameraSmoothY | g_worldCameraSmoothZ) != 0) {
            if (g_worldCameraSmoothX == 1) {
                if (abs(g_worldCameraTarget[0] - x) < 16) {
                    g_worldCameraSmoothX = 0;
                } else {
                    g_worldCameraTarget[0] += (evt_getPlayer()->pos.vx - g_worldCameraTarget[0]) / 8;
                }
            }
            if (g_worldCameraSmoothY == 1) {
                if (abs(g_worldCameraTarget[1] - y) < 16) {
                    g_worldCameraSmoothY = 0;
                } else {
                    g_worldCameraTarget[1] += (evt_getPlayer()->pos.vy - g_worldCameraTarget[1] + g_worldCameraTargetHeight) / 8;
                }
            }
            if (g_worldCameraSmoothZ == 1) {
                if (abs(g_worldCameraTarget[2] - z) < 16) {
                    g_worldCameraSmoothZ = 0;
                } else {
                    g_worldCameraTarget[2] += (evt_getPlayer()->pos.vz - g_worldCameraTarget[2]) / 8;
                }
            }
            if (g_worldCameraSmoothX == 0) {
                g_worldCameraTarget[0] = x;
            }
            if (g_worldCameraSmoothY == 0) {
                g_worldCameraTarget[1] = y;
            }
            if (g_worldCameraSmoothZ == 0) {
                g_worldCameraTarget[2] = z;
            }
        } else {
            step = 0;
            dy = y - g_worldCameraTarget[1];
            dir = dy > 0x80;
            if (dy < -0x80) {
                dir = WM_CAM_RAISE;
            }
            g_worldCameraEaseDiv = (dy >= 0 ? dy : -dy) >> 5;
            if (g_worldCameraEaseDiv < 0x20) {
                g_worldCameraEaseDiv = 0x20;
            }
            if (g_worldCameraEaseDiv > 0x100) {
                g_worldCameraEaseDiv = 0x100;
            }
            if (dir != WM_CAM_HOLD) {
                switch (g_worldMoveModeInfo->controlType) {
                case WM_CTRL_FLY:
                case WM_CTRL_STEER:
                    switch (dir) {
                    case WM_CAM_RAISE:
                        step = (dy + 0x80) / 8;
                        break;
                    case WM_CAM_LOWER:
                        step = (dy - 0x80) / 2;
                        break;
                    }
                    break;
                case WM_CTRL_WALK:
                case WM_CTRL_WALK_FAST:
                    switch (dir) {
                    case WM_CAM_RAISE:
                        step = ((dy + 0x80) << 8) / (g_worldCameraEaseDiv << 5);
                        break;
                    case WM_CAM_LOWER:
                        step = ((dy - 0x80) << 8) / (g_worldCameraEaseDiv << 5);
                        break;
                    }
                    break;
                }
            }
            g_worldCameraTarget[0] = x;
            g_worldCameraTarget[2] = z;
            if (ABS(step) >= 5 && g_worldCameraLookupFix == 0) {
                g_worldCameraTarget[1] += step;
            }
        }
    } else {
        g_worldPlayer->pos.vx = g_worldCameraTarget[0];
        g_worldPlayer->pos.vy = g_worldCameraTarget[1];
        g_worldPlayer->pos.vz = g_worldCameraTarget[2];
    }
    if (evt_getWorldCamEyeObj() == 0) {
        k = 0;
        y2 = g_worldPlayer->pos.vy + g_worldCameraEyeOffset.vy;
        x2 = g_worldPlayer->pos.vx + g_worldCameraEyeOffset.vx;
        z2 = g_worldPlayer->pos.vz + g_worldCameraEyeOffset.vz;
        probe.vx = x2 + (g_worldCameraDir[0] >> 2);
        probe.vy = y2;
        probe.vz = z2 + (g_worldCameraDir[2] >> 2);
        if (g_worldCameraFourProbes != 0) {
            probe.vx += 0x580;
            wm_wrapPos(&probe.vx, &probe.vz);
            wm_getGroundHeight(&probe, &c0, &polyIdx, &g_worldCameraPolyCache[0], (u16 *)&h0);
            probe.vx -= 0xB00;
            wm_wrapPos(&probe.vx, &probe.vz);
            wm_getGroundHeight(&probe, &c1, &polyIdx, &g_worldCameraPolyCache[1], (u16 *)&h1);
            probe.vx += 0x580;
            probe.vz += 0x580;
            wm_wrapPos(&probe.vx, &probe.vz);
            wm_getGroundHeight(&probe, &c2, &polyIdx, &g_worldCameraPolyCache[2], (u16 *)&h2);
            probe.vz -= 0xB00;
            wm_wrapPos(&probe.vx, &probe.vz);
            wm_getGroundHeight(&probe, &c3, &polyIdx, &g_worldCameraPolyCache[3], (u16 *)&h3);
            h = h0;
            cell = c0;
            if (h1 < h) {
                h = h1;
                cell = c1;
            }
            if (h2 < h) {
                h = h2;
                cell = c2;
            }
            if (h3 < h) {
                h = h3;
                cell = c3;
            }
        } else {
            wm_wrapPos(&probe.vx, &probe.vz);
            wm_getGroundHeight(&probe, &cell, &polyIdx, &g_worldCameraPolyCache[0], (u16 *)&h);
        }
        if (g_worldMoveModeInfo->controlType == WM_CTRL_FLY) {
            if (h < -0x2134) {
                h = -0x2134;
            }
        } else if (h < -0x251C) {
            h = -0x251C;
        }
        if (g_worldCameraParams.groundClearance + h < y2) {
            k = ((u8)cell >> 2 == WM_TOPO_MOUNTAIN) * 2;
            if (g_worldCameraFastRise != 0) {
                k = 2;
            }
            if (g_worldCameraTurning != 0) {
                k = 2;
            }
            if (g_worldCamera->viewBlend != 0 && g_worldCamera->viewBlend != 0x1000) {
                k = 3;
            }
            y2 = g_worldCameraParams.groundClearance + h;
        }
        if (g_worldMoveModeInfo->controlType == WM_CTRL_FLY && y2 > -0x1194) {
            y2 = -0x1194;
        }
        dy = y2 - g_worldCameraEye[1];
        dir = dy > 0x100;
        step = 0;
        if (dy < -0x100) {
            dir = WM_CAM_RAISE;
        }
        g_worldCameraEaseDiv = (dy >= 0 ? dy : -dy) >> g_worldCameraEaseShift;
        if (g_worldCameraEaseDiv < g_worldCameraEaseDivMin) {
            g_worldCameraEaseDiv = g_worldCameraEaseDivMin;
        }
        if (g_worldCameraEaseDiv > g_worldCameraEaseDivMax) {
            g_worldCameraEaseDiv = g_worldCameraEaseDivMax;
        }
        if (dir != WM_CAM_HOLD) {
            switch (g_worldMoveModeInfo->controlType) {
            case WM_CTRL_FLY:
            case WM_CTRL_STEER:
                switch (dir) {
                case WM_CAM_RAISE:
                    step = (dy + 0x100) / 4;
                    break;
                case WM_CAM_LOWER:
                    step = (dy - 0x100) / 32;
                    break;
                }
                break;
            case WM_CTRL_WALK:
            case WM_CTRL_WALK_FAST:
                switch (dir) {
                case WM_CAM_RAISE:
                    step = ((dy + 0x100) << 8) / (((g_worldCameraEaseScale - 2) * g_worldCameraEaseDiv * 7) >> k);
                    break;
                case WM_CAM_LOWER:
                    step = ((dy - 0x100) << 8) / (g_worldCameraEaseScale * g_worldCameraEaseDiv * 3);
                    break;
                }
                ease = step >= 0 ? step : -step;
                if (ease > 0x200) {
                    ease = 0x200;
                }
                ease = (((0x200 - ease) * g_worldCameraEaseBoost >> 12) + 0x800) << 1;
                step *= ease;
                step /= 4096;
                break;
            }
        }
        g_worldCameraEye[0] = x2;
        g_worldCameraEye[2] = z2;
        if (ABS(step) >= 5 && g_worldCameraLookupFix == 0) {
            g_worldCameraEye[1] += step;
        }
        eyeY = y2;
    }
    dx = g_worldCameraEye[0] - 0x2FF77;
    dz = g_worldCameraEye[2] + 0x14031;
    if (ABS(dx) < 0x36B0 && ABS(dz) < 0x36B0) {
        s = 0xBAEB900 - dx * dx;
        s -= dz * dz;
        r = -SquareRoot0(s) + 0x898;
        if (s > 0 && r < g_worldCameraEye[1]) {
            g_worldCameraEye[1] = r;
        }
    }
    switch (g_worldCameraStartPhase) {
    case 0:
        if (g_worldMapLoaded != 0) {
            g_worldCameraStartPhase = 1;
            g_worldCameraEye[1] = eyeY;
            g_worldCameraTarget[1] = g_worldPlayer->pos.vy + g_worldCameraTargetHeight;
        }
        break;
    case 1:
        g_worldCameraStartPhase = 2;
        p = &g_evtGlobalVars.b[0x65];
        if (*p & 1) {
            fade_inAdditiveDefault();
            g_evtGlobalVars.b[0x65] &= ~1;
        } else {
            fade_inDefault();
        }
        break;
    }
    if (g_worldCameraEye[1] < -0x47CF) {
        g_worldCameraEye[1] = -0x47CF;
    }
    wm_wrapPos(&g_worldCameraTarget[0], &g_worldCameraTarget[2]);
    wm_wrapPos(&g_worldCameraEye[0], &g_worldCameraEye[2]);
}

void wm_updateCameraPreset(void) {
    s32 k;
    s32 m;
    WmCameraParams preset;

    k = g_worldAreaCameraGroup[w_frameGetParameter(0xC0)];
    m = g_worldMoveModeInfo->cameraRow;
    if (g_worldCameraGroupOverride != -1) {
        k = g_worldCameraGroupOverride;
    }
    if (g_worldCamera->viewForceNormal != 0) {
        g_worldCamera->viewBlendSpeed = -0x80;
    }
    g_worldCamera->viewBlend += g_worldCamera->viewBlendSpeed;
    if (g_worldCamera->viewBlend <= 0) {
        g_worldCamera->viewBlend = 0;
        g_worldCamera->viewBlendSpeed = 0;
        g_worldCamera->viewForceNormal = 0;
    }
    if (g_worldCamera->viewBlend >= 0x1000) {
        g_worldCamera->viewBlend = 0x1000;
        g_worldCamera->viewBlendSpeed = 0;
        g_worldCamera->viewForceNormal = 0;
    }
    if (g_worldCameraBlendSpeed != 0) {
        g_worldCameraBlend += g_worldCameraBlendSpeed;
        if (g_worldCameraBlend > 0x1000) {
            g_worldCameraBlend = 0x1000;
        }
        wm_calcCameraPreset(-g_worldPlayer->pos.vy, g_worldCameraBlendRow, k, g_worldCamera->viewBlend, &preset);
        g_worldCameraParams.projDist = wm_lerpShift(g_worldCameraParamsFrom.projDist, preset.projDist, g_worldCameraBlend, 9);
        g_worldCameraParams.distance = wm_lerpShift(g_worldCameraParamsFrom.distance, preset.distance, g_worldCameraBlend, 0xF);
        g_worldCameraParams.eyeHeight = wm_lerpShift(g_worldCameraParamsFrom.eyeHeight, preset.eyeHeight, g_worldCameraBlend, 0xE);
        g_worldCameraParams.groundClearance = wm_lerpShift(g_worldCameraParamsFrom.groundClearance, preset.groundClearance, g_worldCameraBlend, 0xE);
        g_worldCameraParams.targetHeight = wm_lerpShift(g_worldCameraParamsFrom.targetHeight, preset.targetHeight, g_worldCameraBlend, 0xE);
        if (g_worldCameraBlend >= 0x1000) {
            g_worldCameraBlendSpeed = 0;
        }
    } else {
        wm_calcCameraPreset(-g_worldPlayer->pos.vy, m, k, math_sin(g_worldCamera->viewBlend / 4), &g_worldCameraParams);
    }
}

void wm_calcCameraPreset(s32 altitude, s32 row, s32 col, s32 viewBlend, WmCameraParams *out) {
    WmCameraParams *a;
    WmCameraParams *b;
    WmCameraParams *c;
    s32 t;
    s16 blended[5];

    c = &g_worldCameraPresets[g_worldCameraPresetIndex[row][col][2]];
    a = &g_worldCameraPresets[g_worldCameraPresetIndex[row][col][0]];
    b = &g_worldCameraPresets[g_worldCameraPresetIndex[row][col][1]];
    t = wm_cameraAltitudeBlend(altitude) & 0xFFFF;
    blended[0] = wm_lerpShift(a->projDist, b->projDist, t, 9);
    blended[1] = wm_lerpShift(a->distance, b->distance, t, 0xE);
    blended[2] = wm_lerpShift(a->eyeHeight, b->eyeHeight, t, 0xD);
    blended[3] = wm_lerpShift(a->groundClearance, b->groundClearance, t, 0xD);
    blended[4] = wm_lerpShift(a->targetHeight, b->targetHeight, t, 0xD);
    out->projDist = wm_lerpShift(blended[0], c->projDist, viewBlend, 9);
    out->distance = wm_lerpShift(blended[1], c->distance, viewBlend, 0xE);
    out->eyeHeight = wm_lerpShift(blended[2], c->eyeHeight, viewBlend, 0xD);
    out->groundClearance = wm_lerpShift(blended[3], c->groundClearance, viewBlend, 0xD);
    out->targetHeight = wm_lerpShift(blended[4], c->targetHeight, viewBlend, 0xD);
}

void wm_toggleCameraView(void) {
    u8 r;

    if (g_worldCamera->viewForceNormal == 0
        && (*g_worldStoryCounterPtr >= 0x137E || WM_ATTR_AREA(g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr) != 0xC)) {
        r = 0;
        if (g_worldCamera->viewBlend == 0) {
            r = 1;
            g_worldCamera->viewBlendSpeed = 0x100;
        }
        if (g_worldCamera->viewBlend == 0x1000) {
            r = 1;
            g_worldCamera->viewBlendSpeed = -0x100;
        }
        if (r == 0) {
            g_worldCamera->viewBlendSpeed = -g_worldCamera->viewBlendSpeed;
        }
    }
}

s32 *wm_getCameraTarget(void) {
    return g_worldCameraTarget;
}

s32 *wm_getCameraEye(void) {
    return g_worldCameraEye;
}

void wm_updateCameraNoView(void) {
    wm_updateCameraPreset();
    wm_cameraFollow();
}

void wm_blendCameraPreset(s16 row, s16 speed) {
    g_worldCameraBlend = 0;
    g_worldCameraBlendRow = row;
    g_worldCameraParamsFrom = g_worldCameraParams;
    if (g_worldMapLoaded != 0) {
        g_worldCameraBlendSpeed = speed;
    } else {
        g_worldCameraBlendSpeed = 0;
    }
}

s32 wm_getCameraViewState() {
    s16 v = g_worldCamera->viewBlend;

    if (v == 0) {
        return 0;
    }
    if (v == 0x1000) {
        return 2;
    }
    return 1;
}

void wm_toggleCameraAutoTurn(void) {
    if (g_worldCameraFlags->autoTurnOff != 0) {
        g_worldCameraRecenter = 1;
    }
    g_worldCameraFlags->autoTurnOff = ~g_worldCameraFlags->autoTurnOff;
}

void wm_cameraSmooth(void) {
    g_worldCameraSmoothX = 1;
    g_worldCameraSmoothY = 1;
    g_worldCameraSmoothZ = 1;
}

void wm_lookAt(MATRIX *out, SVECTOR *eye, SVECTOR *target, VECTOR *up) {
    VECTOR d;
    VECTOR x;
    VECTOR y;
    VECTOR z;

    d.vx = target->vx - eye->vx;
    d.vy = target->vy - eye->vy;
    d.vz = target->vz - eye->vz;
    VectorNormal(&d, &z);
    if (z.vz == up->vz) {
        z.vz++;
    }
    gte_ldopv1(&z);
    gte_ldopv2(up);
    gte_op12();
    gte_stlvnl(&d);
    VectorNormal(&d, &x);
    gte_ldopv1(&z);
    gte_ldopv2(&x);
    gte_op12();
    gte_stlvnl(&d);
    VectorNormal(&d, &y);
    out->m[0][0] = x.vx;
    out->m[0][1] = x.vy;
    out->m[0][2] = x.vz;
    out->m[1][0] = y.vx;
    out->m[1][1] = y.vy;
    out->m[1][2] = y.vz;
    out->m[2][0] = z.vx;
    out->m[2][1] = z.vy;
    out->m[2][2] = z.vz;
    gte_SetRotMatrix(out);
    gte_ldv0(eye);
    gte_rtv0();
    gte_stlvnl(out->t);
    out->t[0] = -out->t[0];
    out->t[1] = -out->t[1];
    out->t[2] = -out->t[2];
}

s32 wm_cameraAltitudeBlend(s32 altitude) {
    altitude = (altitude - 1000) << 12;
    altitude /= 4500;
    if (altitude > 0x1000) {
        altitude = 0x1000;
    }
    if (altitude < 0) {
        altitude = 0;
    }
    return altitude & 0xFFFF;
}

void wm_initMovement(void) {
    s32 i;
    s32 j;

    g_worldPlayer = &g_worldDummyActor;
    g_worldWalkerActor = NULL;
    g_worldVehicleActor = NULL;
    g_worldMountActor = 0;
    g_worldMoogleActor = 0;
    g_worldMoveMode = -1;
    g_worldMoveModeInfo = 0;
    g_worldPlayerPosChanged = 0;
    g_worldCameraTurnedByHand = 0;
    g_worldPlayerSpeed = 0;
    g_worldPlayerClimbSpeed = 0;
    g_worldPlayerTurn = 0;
    g_worldSpeedSmooth = 0;
    g_worldClimbSmooth = 0;
    g_worldPlayerHeading = 0;
    g_worldMoveInitWord = 0;
    g_worldCameraTurnGain = 0x1000;
    g_worldCeilingGap = 0;
    g_worldDrawActors = 1;
    g_worldFastFly = 0;
    g_worldMountPaletteOverride = 0;
    g_worldAutoPilot = 0;
    g_worldCameraRecenter = 0;
    g_worldCameraTurning = 0;
    g_worldCameraFastRise = 0;
    g_worldFlyTurning = 0;
    for (i = 0; i < 11; i++) {
        g_worldPolyCache[i].lastSlot = 0;
        for (j = 0; j < 6; j++) {
            g_worldPolyCache[i].polys[j] = j;
            g_worldPolyCache[i].cells[j] = -1;
        }
    }
    for (i = 0; i < 22; i++) {
        g_worldActorTypeInfo[i].cellAttr = 0;
        g_worldActorTypeInfo[i].rideOffset = 0;
        g_worldActorTypeInfo[i].floorY = 0;
    }
    if (g_worldDataSet == WM_DATASET_DISC4) {
        g_worldMoveModes[WM_MOVE_INVINCIBLE].music = WM_SONG_DISC4;
        g_worldMoveModes[WM_MOVE_WALK].music = WM_SONG_DISC4;
    } else {
        g_worldMoveModes[WM_MOVE_INVINCIBLE].music = WM_SONG_AIRSHIP;
        g_worldMoveModes[WM_MOVE_WALK].music = WM_SONG_NORMAL;
    }
}

void wm_setMountPalette(void) {
    s32 id;
    s32 k;
    WmPalAnim *e;
    u16 *pal;
    u16 *dst;
    EvtListNode *n;
    EvtObject *a;
    s32 c;
    s32 i;
    s32 j;

    id = 0;
    k = 0;
    e = g_gameState->gameData->ctx.world->obj->palAnims;
    pal = w_framePackGetPtr(g_worldDataPack, 1);
    wm_mountPalettePre();
    for (n = evt_getObjectList(); n != NULL; n = n->next) {
        a = n->obj;
        if (a->state == EVT_STATE_RUNNING && (a->flags & EVT_OBJ_ENABLED) && a->kind == EVT_KIND_ACTOR) {
            if ((a->actorType >= WM_ACTOR_CHOCOBO0 && a->actorType <= WM_ACTOR_CHOCOBO4)) {
                id = a->modelId;
                switch (a->actorType) {
                case WM_ACTOR_CHOCOBO0:
                    k = 0;
                    break;
                case WM_ACTOR_CHOCOBO1:
                    k = 1;
                    break;
                case WM_ACTOR_CHOCOBO2:
                    k = 2;
                    break;
                case WM_ACTOR_CHOCOBO3:
                    k = 3;
                    break;
                case WM_ACTOR_CHOCOBO4:
                    k = 4;
                    break;
                }
                if (g_worldMountPaletteOverride != 0) {
                    k = g_worldMountPaletteOverride - 1;
                }
            }
        }
    }
    c = 0;
    for (i = 0; i < g_gameState->gameData->ctx.world->obj->palAnimCount; i++, e++) {
        if (e->modelId == id) {
            dst = e->srcPal;
            switch (c) {
            case 0:
                for (j = 0; j < 16; j++) {
                    *dst++ = pal[k * 48 + j + 32];
                }
                break;
            case 1:
                for (j = 0; j < 16; j++) {
                    *dst++ = pal[k * 48 + j + 16];
                }
                break;
            case 2:
                for (j = 0; j < 16; j++) {
                    *dst++ = pal[k * 48 + j + 0];
                }
                break;
            }
            c++;
        }
    }
}

/* find the world map actors (vehicles) and sync the camera with the player */
void wm_findVehicles(void) {
    EvtListNode *n;
    EvtObject *a;

    for (n = evt_getObjectList(); n != NULL; n = n->next) {
        if (n->obj->kind == EVT_KIND_ACTOR) {
            a = n->obj;
            switch (a->actorType) {
            case WM_ACTOR_WALKER:
            case WM_ACTOR_WALKER_ALT:
                g_worldWalkerActor = a;
                break;
            case WM_ACTOR_CHOCOBO0:
            case WM_ACTOR_CHOCOBO1:
            case WM_ACTOR_CHOCOBO2:
            case WM_ACTOR_CHOCOBO3:
            case WM_ACTOR_CHOCOBO4:
                g_worldMountActor = a;
                break;
            case WM_ACTOR_BOAT:
                g_worldVehicleActor = a;
                break;
            case WM_ACTOR_AIRSHIP:
                g_worldVehicleActor = a;
                break;
            case WM_ACTOR_AIRSHIP2:
                g_worldVehicleActor = a;
                break;
            }
            if (a == evt_getPlayer()) {
                if (g_gameState->prevMode == GS_MODE_FIELD && g_gameState->gameData->inWorldFrame == 0) {
                    if (g_worldMoveModeInfo->cameraInFront != 0) {
                        g_worldCamera->yawOffset = 0x800;
                    } else {
                        g_worldCamera->yawOffset = 0;
                    }
                    g_worldCamera->yaw = a->pos.rotY + g_worldCamera->yawOffset;
                }
                g_worldPlayerHeading = a->pos.rotY;
            }
        }
    }
    g_gameState->gameData->worldCamKeep = 0;
    g_worldVehiclesFound = 1;
}

void wm_drawActors(void) {
    SVECTOR n;
    VECTOR a;
    VECTOR b;
    VECTOR dv;
    SVECTOR s;
    s32 z;
    s32 base;
    s32 d;
    s32 ang;
    EvtListNode *e;
    EvtObject *act;
    u8 f;

    VectorNormalS((VECTOR *)g_worldCameraDir, &n);
    base = (-ratan2(n.vz, n.vx) + 0x400) % 4096;
    pen_locate(10, 100);
    for (e = evt_getObjectList(); e != NULL; e = e->next) {
        act = e->obj;
        if (act->kind == EVT_KIND_ACTOR && act->state == EVT_STATE_RUNNING && (act->flags & EVT_OBJ_ENABLED) && (u8)wm_worldToView((VECTOR *)&act->pos, &a, &b, &z)) {
            f = 1;
            dv.vx = act->pos.vx - (g_worldCameraEye[0] - n.vx);
            dv.vz = act->pos.vz - (g_worldCameraEye[2] - n.vz);
            if (dv.vx > g_worldWidth - 0x8000) {
                dv.vx -= g_worldWidth;
            }
            if (dv.vz > g_worldDepth - 0x8000) {
                dv.vz -= g_worldDepth;
            }
            if (dv.vx < 0x8000 - g_worldWidth) {
                dv.vx = g_worldWidth + dv.vx;
            }
            if (dv.vz < 0x8000 - g_worldDepth) {
                dv.vz = g_worldDepth + dv.vz;
            }
            ang = -ratan2(dv.vz, dv.vx) + 0x400;
            d = ang % 4096;
            d = d - base;
            if (d < -0x800) {
                d += 0x1000;
            }
            if (d > 0x800) {
                d -= 0x1000;
            }
            if (((d < 0 ? -d : d) < 400 || wm_getCameraViewState(ang) != 0) && (b.vz >= 2001 && b.vz < 29000) && b.vx > -20000 && b.vx < 20000 && b.vy > -20000 && b.vy < 20000 && a.vx > -29000 && a.vx < 29000 && a.vz > -29000 && a.vz < 29000) {
                s.vx = a.vx;
                s.vy = a.vy;
                s.vz = a.vz;
                wm_setActorFog(act, b.vz >> 3);
                wm_drawActorModel(act, &s, z);
                if (act->actorType == WM_ACTOR_BOAT) {
                    f = 0;
                }
                if (act->actorType == WM_ACTOR_WALKER || act->actorType == WM_ACTOR_WALKER_ALT) {
                    if ((u8)g_worldMoveMode >= WM_MOVE_CHOCOBO_FIELD && (u8)g_worldMoveMode <= WM_MOVE_CHOCOBO_FLY) {
                        f = 0;
                    }
                }
                if (g_worldActorTypeInfo[act->actorType].rideOffset < 4 && f) {
                    wm_drawActorShadow(act, &s, b.vz);
                }
            }
        }
    }
    pen_setColorPreset(0);
    SetFarColor(0, 0, 0);
}

void wm_setActorFog(EvtObject *act, s32 depth) {
    s16 col[3];
    s32 t;
    s32 f;
    s32 a;
    s32 d;
    s32 i;
    s32 id;
    WmPalAnim *e;
    RECT *r;

    id = act->modelId;
    e = g_gameState->gameData->ctx.world->obj->palAnims;
    t = -act->pos.vy;
    t -= 0xC1F;
    t <<= 12;
    t /= 2176;
    if (t > 0x1000) {
        t = 0x1000;
    }
    if (t < 0) {
        t = 0;
    }
    a = wm_lerp(g_worldWeatherData.work[0].fog[1].fogOffset, g_worldWeatherData.work[0].fog[0].fogOffset, t);
    col[0] = wm_lerp(g_worldWeatherData.work[0].fog[1].color[0], g_worldWeatherData.work[0].fog[0].color[0], t);
    col[1] = wm_lerp(g_worldWeatherData.work[0].fog[1].color[1], g_worldWeatherData.work[0].fog[0].color[1], t);
    col[2] = wm_lerp(g_worldWeatherData.work[0].fog[1].color[2], g_worldWeatherData.work[0].fog[0].color[2], t);
    SetFarColor((u8)col[0], (u8)col[1], (u8)col[2]);
    f = (depth * 8 + (d = a - 0x8000)) >> 2;
    if (f < 0) {
        f = 0;
    }
    if (f >= 0x1000) {
        f = 0xFFF;
    }
    for (i = 0; i < g_gameState->gameData->ctx.world->obj->palAnimCount; i++, e++) {
        if (e->modelId == id) {
            r = (RECT *)g_worldPrimPtr;
            if (f < 0) {
                f = 0;
            }
            if (f > 0x1000) {
                f = 0x1000;
            }
            wm_depthCueClut(e->srcPal, e->workPal[*g_worldBufIndex], f, g_worldWeatherData.work[0].fxBackColor);
            r->x = e->clutX;
            r->y = e->clutY;
            r->w = 16;
            r->h = 1;
            LoadImage(r, (u32 *)e->workPal[*g_worldBufIndex]);
        }
    }
}

s32 wm_drawActorModel(EvtObject *act, SVECTOR *pos, s32 unused) { /* its caller passes this, unused */
    WmActorType *c;
    WmObjSlot *s;
    ModelObj *m;
    WmAnimHdr *h;
    SVECTOR r2;
    SVECTOR r;
    s32 t;
    void *t2;
    s32 n;
    s32 i;

    c = &g_worldActorTypeInfo[act->actorType];
    s = (WmObjSlot *)game_getModel(act->modelSlot);
    m = s->model;
    memset(&r, 0, 8);
    r.vx = s->actor->pos.rotX;
    r.vy = s->actor->pos.rotY;
    r.vz = s->actor->rotZ;
    r2 = r;
    math_rotMatrixYXZ(&r2, &m->worldMatrix);
    m->worldMatrix.t[0] = pos->vx;
    m->worldMatrix.t[1] = pos->vy;
    m->worldMatrix.t[2] = pos->vz;
    wm_applyScale(m);
    m->anim = h = db_lookupRegistered(DB_GET_ADDR, s->actor->anim | DB_ANIM, 0);
    if (h != NULL) {
        if (IS_OFFSET(h->rotKeys)) {
            t = h->rotKeysLo;
            h->rotKeys = (s32)h + h->rotKeys;
            if (t != 0) {
                h->rotKeysLo = (s32)m->anim + t;
            }
        }
        wm_calcBoneMatrices(m, s->actor->animFrame);
        if (act->actorType != WM_ACTOR_NO_MODEL) {
            n = m->model->groupCount;
            if (c->terrainClass != 0 && c->rideOffset > 0) {
                m->clipY = pos->vy - c->rideOffset + 100;
                if (m->parent != NULL) {
                    m->clipY = m->parent->clipY;
                }
                for (i = 0; i < n; i++) {
                    wm_projectMeshClipY(m, i);
                    wm_drawMeshClipY(m, i);
                }
            } else {
                for (i = 0; i < n; i++) {
                    wm_projectMesh(m, i);
                    wm_drawMesh(m, i);
                }
            }
        }
    } else {
        printf("Illigal Animation Data\n");
    }
    return 0;
}

/* shadow under world map object act at pos; depth = distance from the camera */
void wm_drawActorShadow(EvtObject *act, SVECTOR *pos, s32 depth) {
    WmActorType *c;
    s32 k;
    s32 d;
    s32 sx;
    s32 sz;
    s32 f;
    s32 a;
    s32 s;
    u32 *ot;

    c = &g_worldActorTypeInfo[act->actorType];
    k = act->modelSlot;
    sx = 0x1000;
    if (act->actorType != WM_ACTOR_NO_MODEL) {
        d = c->floorY - act->pos.vy;
        pos->vy += d;
        pos->vy -= d / 8;
        sz = sx - d / 2;
        sx -= d / 4;
        g_worldOtBias = -c->shadowWidth * 4 - (s8)c->shadowOtBias;
        if (c->shadowWidth == 0) {
            return;
        }
        sx = (c->shadowWidth * sx) >> 11;
        sz = (c->shadowDepth * sz) >> 12;
        f = (0x8000 - depth) >> 3;
        f *= g_worldWeatherData.work[0].shadowShade;
        f >>= 12;
        if (f < 0) {
            f = 0;
        }
        if (f >= 0x1000) {
            f = 0xFFF;
        }
        sz *= f;
        sz >>= 12;
        if (sz >= 0x100) {
            sz = 0xFF;
        }
        if (sz < 0) {
            sz = 0;
        }
        g_gameState->gameData->ctx.world->obj->shadows[k].scaleX = sx * 14;
        g_gameState->gameData->ctx.world->obj->shadows[k].scaleZ = sx * 12;
        g_gameState->gameData->ctx.world->obj->shadows[k].shade = sz;
        ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf];
        g_gameState->gameData->ot[g_gameState->gameData->drawBuf] = ot + g_worldOtBias;
        wm_drawShadow(g_gameState->gameData->ctx.world->obj->shadows, k, pos->vy, c->shadowJoint);
        g_gameState->gameData->ot[g_gameState->gameData->drawBuf] = ot;
    } else {
        s = 0x7D0;
        if (g_worldFog != 0) {
            s = 0xAF0;
        }
        f = (0x7060 - depth) >> 2;
        f *= g_worldWeatherData.work[4].shadowShade;
        f >>= 12;
        if (f < 0) {
            f = 0;
        }
        if (f >= 0x1000) {
            f = 0xFFF;
        }
        a = (c->shadowDepth * s) >> 12;
        a *= f;
        a >>= 12;
        if (a >= 0x100) {
            a = 0xFF;
        }
        if (a < 0) {
            a = 0;
        }
        g_gameState->gameData->ctx.world->obj->shadows[k].scaleX = 0x302;
        g_gameState->gameData->ctx.world->obj->shadows[k].scaleZ = 0x294;
        g_gameState->gameData->ctx.world->obj->shadows[k].shade = a;
        ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf];
        g_worldOtBias = -0x46;
        g_gameState->gameData->ot[g_gameState->gameData->drawBuf] -= 0x46;
        wm_drawShadow(g_gameState->gameData->ctx.world->obj->shadows, k, pos->vy, c->shadowJoint);
        g_gameState->gameData->ot[g_gameState->gameData->drawBuf] = ot;
    }
}

void wm_updateActors(void) {
    s32 polyIdx;
    s32 h;
    s32 cell;
    EvtListNode *e;
    EvtObject *act;
    WmActorType *cc;

    g_worldPlayerStepped = 0;
    g_worldPlayerSpeed = 0;
    g_worldPlayerClimbSpeed = 0;
    g_worldPlayerTurn = 0;
    if (evt_getPlayer() != NULL) {
        g_worldPlayer = evt_getPlayer();
        g_worldPlayer->prevPos.vx = g_worldPlayer->pos.vx;
        g_worldPlayer->prevPos.vy = g_worldPlayer->pos.vy;
        g_worldPlayer->prevPos.vz = g_worldPlayer->pos.vz;
    }
    for (e = evt_getObjectList(); e != NULL; e = e->next) {
        act = e->obj;
        cc = &g_worldActorTypeInfo[act->actorType];
        if (act->kind == EVT_KIND_ACTOR && act->state == EVT_STATE_RUNNING && (act->flags & EVT_OBJ_ENABLED)) {
            switch (act->actorType) {
            case WM_ACTOR_CHOCOBO4:
                cc->flying = g_worldMoveMode == WM_MOVE_CHOCOBO_FLY;
                break;
            case WM_ACTOR_MOOGLE:
                g_worldMoogleActor = act;
                break;
            }
            if (act == g_worldPlayer && evt_isControlEnabled() != 0) {
                wm_movePlayer(cc);
            } else {
                cc = &g_worldActorTypeInfo[act->actorType];
                wm_getGroundHeight((VECTOR *)&act->pos, &cc->cellAttr, &polyIdx, &g_worldPolyCache[cc->polyCache], (u16 *)&h);
                if (act->actorType == WM_ACTOR_BOAT) {
                    h = 0;
                }
                cc->floorY = h;
            }
            wm_updateActorHeight(act);
        }
    }
    pen_setColorPreset(0);
    switch (g_worldMoveModeInfo->controlType) {
    case WM_CTRL_WALK:
    case WM_CTRL_WALK_FAST:
        if (g_worldMapLoaded != 0) {
            wm_cameraAutoTurnWalk();
        }
        break;
    case WM_CTRL_FLY:
    case WM_CTRL_STEER:
        wm_cameraAutoTurnVehicle();
        break;
    }
    if ((u8)g_worldMoveMode >= WM_MOVE_CHOCOBO_FIELD && (u8)g_worldMoveMode <= WM_MOVE_CHOCOBO_FLY) {
        g_worldActorTypeInfo[WM_ACTOR_WALKER].rideOffset = g_worldActorTypeInfo[g_worldPlayer->actorType].rideOffset;
    }
    if (g_worldMountActor != NULL) {
        if (g_worldMountActor->rotZ < 0) {
            g_worldMountActor->rotZ += 10;
        }
        if (g_worldMountActor->rotZ > 0) {
            g_worldMountActor->rotZ -= 10;
        }
        if (g_worldMountActor->rotZ > -10 && g_worldMountActor->rotZ < 10) {
            g_worldMountActor->rotZ = 0;
        }
    }
    if (g_worldVehicleActor != NULL) {
        if (g_worldMoveModeInfo->controlType != WM_CTRL_FLY) {
            if (g_worldVehicleActor->rotZ < 14) {
                g_worldVehicleActor->rotZ += 10;
            }
            if (g_worldVehicleActor->rotZ >= 15) {
                g_worldVehicleActor->rotZ -= 10;
            }
            if (g_worldVehicleActor->rotZ >= 5 && g_worldVehicleActor->rotZ < 24) {
                g_worldVehicleActor->rotZ = 14;
            }
        } else if (g_worldFlyTurning == 0) {
            if (g_worldVehicleActor->rotZ < 14) {
                g_worldVehicleActor->rotZ += 16;
            }
            if (g_worldVehicleActor->rotZ >= 15) {
                g_worldVehicleActor->rotZ -= 16;
            }
            if (g_worldVehicleActor->rotZ >= -1 && g_worldVehicleActor->rotZ < 30) {
                g_worldVehicleActor->rotZ = 14;
            }
        }
    }
    if (evt_getPlayer() != NULL) {
        if (g_worldPlayerTurn == 0 || g_worldPlayerSpeed < 300) {
            if (g_worldCameraRoll > 0) {
                g_worldCameraRoll -= 10;
                if (g_worldCameraRoll < 0) {
                    g_worldCameraRoll = 0;
                }
            }
            if (g_worldCameraRoll < 0) {
                g_worldCameraRoll += 10;
                if (g_worldCameraRoll > 0) {
                    g_worldCameraRoll = 0;
                }
            }
        }
        cc = &g_worldActorTypeInfo[g_worldPlayer->actorType];
        if ((cc->cellAttr & WM_ATTR_TRIGGER_MASK) && g_worldCellTriggersOn != 0) {
            wm_posToCell(g_worldPlayer->pos.vx, g_worldPlayer->pos.vz, &cell);
            evt_callWorldCellTrigger(cell % g_worldCellsW, cell / g_worldCellsW, WM_ATTR_TRIGGER(cc->cellAttr));
        }
    }
}

void wm_updateActorHeight(EvtObject *act) {
    u8 flat;
    s32 h;
    WmActorType *c;
    s32 y;
    s16 d;

    if (g_worldMapLoaded != 0) {
        c = &g_worldActorTypeInfo[act->actorType];
        if (c->polyCache != 0xA) {
            h = wm_getTerrainHeightOffset(c->terrainClass, c->cellAttr, &flat);
            if (c->flying == 0) {
                if (c->terrainClass != 0) {
                    if (c->rideMode == WM_MOVE_BOAT) {
                        c->rideOffset = 0x118;
                    } else if (flat != 0 || h == 0) {
                        c->rideOffset = h;
                    } else {
                        if (c->rideOffset < h - 0x15) {
                            c->rideOffset += 0x14;
                        }
                        if (c->rideOffset > h + 0x15) {
                            c->rideOffset -= 0x14;
                        }
                    }
                }
                act->pos.vy = c->floorY + c->rideOffset;
            } else {
                y = c->floorY + h;
                if (y < act->pos.vy) {
                    act->pos.vy = y;
                }
                if (g_worldCameraHold == 0 && g_worldPlayer->pos.vy < -0x2A30 && g_worldCameraLookupFix == 0) {
                    g_worldPlayer->pos.vy = -0x2A30;
                }
                d = act->pos.vy - c->floorY;
                c->rideOffset = d;
                if (d < 0) {
                    c->rideOffset = 0;
                }
            }
        }
    }
}

void wm_movePlayer(WmActorType *cc) {
    ActorPos p;
    ActorPos q;
    u32 cell;
    s32 h;
    u32 cell2;
    s32 h2;
    s32 saved;
    s32 spd;
    s32 i;
    s32 j;
    s32 dx, dy, dz;
    s32 ex, ez;
    s32 k;
    s32 dist;
    s32 m;
    s32 off, lim, y, zz, wz, r1, r2;

    if (g_worldMoveModeInfo->countsSteps != 0 && g_worldPlayerPosChanged != 0 && w_frameGetParameter(WM_PARAM_TOPOGRAPHY) != WM_TOPO_NO_ENCOUNTER && g_worldTitleId == -1) {
        g_worldPlayerStepped = g_worldStepFlagOn;
    }
    if (g_worldPlayerControlOff == 0 && g_worldNaviMode != WM_NAVI_FULL && g_worldMusicState == WM_MUSIC_READY && g_worldMapLoaded != 0 && g_worldCameraSmoothing == 0) {
        wm_controlPlayer();
    }
    spd = g_worldPlayerSpeed;
    if (spd < 0) {
        spd /= 2;
    }
    g_worldUsePolyCache = 1;
    if (g_worldMoveModeInfo->airborne == 0) {
        saved = g_worldMoveMode;
        if (g_worldFastWalk != 0) {
            g_worldMoveMode = WM_MOVE_FAST_WALK;
            spd *= 4;
        }
        i = 0;
        do {
            j = i;
            cell = -1;
            h = -1;
            p = g_worldPlayer->pos;
            if (wm_tryStep((VECTOR *)&p, i, g_worldMoveMode, spd, &cell, &h) ||
                wm_tryStep((VECTOR *)&p, j = -i, g_worldMoveMode, spd, &cell, &h)) {
                dx = p.vx - g_worldPlayer->prevPos.vx;
                dy = p.vy - g_worldPlayer->prevPos.vy;
                dz = p.vz - g_worldPlayer->prevPos.vz;
                if (wm_getTerrainHeightOffset(cc->terrainClass, cell, 0) != 0) {
                    dist = SquareRoot0(dx * dx + dz * dz);
                } else {
                    dist = SquareRoot0(dx * dx + dy * dy + dz * dz);
                }
                if (dist != 0) {
                    k = spd * spd / dist;
                    if (spd < 0) {
                        k = -k;
                    }
                } else {
                    k = 0;
                }
                q = g_worldPlayer->pos;
                if (wm_tryStep((VECTOR *)&q, j, g_worldMoveMode, k, &cell, &h)) {
                    g_worldPlayer->pos = q;
                    cc->cellAttr = cell;
                    cc->floorY = q.vy;
                    g_worldPlayerFloorPoly = h;
                    break;
                }
            }
            i += 0x80;
        } while (i < 0x400);
        if (g_worldFastWalk != 0) {
            g_worldMoveMode = saved;
        }
    } else {
        q = g_worldPlayer->pos;
        if (wm_tryStep((VECTOR *)&q, 0, g_worldMoveMode, spd, &cell2, &h2)) {
            g_worldPlayer->pos.vx = q.vx;
            g_worldPlayer->pos.vz = q.vz;
            g_worldCeilingGap = g_worldPlayer->pos.vy + 0x2A30;
            if (g_worldCeilingGap < 0x7E8 && g_worldCameraHold == 0 && g_worldCameraLookupFix == 0 && g_worldPlayerClimbSpeed < 0) {
                m = (g_worldPlayerClimbSpeed << 1) * g_worldCeilingGap;
                m >>= 12;
                g_worldPlayer->pos.vy += m;
            } else {
                g_worldPlayer->pos.vy += g_worldPlayerClimbSpeed;
            }
            cc->cellAttr = cell2;
            cc->floorY = q.vy;
            g_worldPlayerFloorPoly = h2;
        }
        off = 0x1C20;
        if (g_worldDataSet == WM_DATASET_DISC1) {
            off = 0x1964;
            lim = 0x1E78;
        } else {
            lim = 0x2134;
        }
        /* ez holds the x offset here; the names follow the original register use */
        ez = g_worldPlayer->pos.vx - 0x2FF77;
        y = g_worldPlayer->pos.vy;
        wz = g_worldPlayer->pos.vz + 0x14031;
        dy = y + off;
        if (ABS(ez) < 0x9C4 && ABS(wz) < 0x9C4) {
            dx = 0x5F5E10 - ez * ez;
            zz = wz * wz;
            dz = dx - dy * dy - zz;
            if (-lim < y) {
                g_worldUsePolyCache = 0;
            }
            if (dz > 0) {
                r1 = SquareRoot0(dz);
                r2 = SquareRoot0(dz);
                dz = dy - r1;
                if (dz < 0) {
                    dz = -dz;
                }
                dz = dz < abs(dy + r2);
                if (dz) {
                    g_worldPlayer->prevPos.vy = g_worldPlayer->pos.vy = SquareRoot0(dx - zz) - off;
                } else {
                    g_worldPlayer->prevPos.vy = g_worldPlayer->pos.vy = -SquareRoot0(dx - zz) - off;
                }
            }
        }
    }
    ex = g_worldPlayer->pos.vx - 0x407F7;
    ez = g_worldPlayer->pos.vz + 0x3BDC3;
    if (ABS(ex) < 0x800 && ABS(ez) < 0x800) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - 0x37FA8;
    ez = g_worldPlayer->pos.vz + 0x1606D;
    if (ABS(ex) < 0xC00 && ABS(ez) < 0xC00) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - 0x4AD96;
    ez = g_worldPlayer->pos.vz + 0x32133;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - 0x489E4;
    ez = g_worldPlayer->pos.vz + 0x34BBF;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - 0x250D4;
    ez = g_worldPlayer->pos.vz + 0x46B95;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - 0x36F54;
    ez = g_worldPlayer->pos.vz + 0xF131;
    if (ABS(ex) < 0x60C && ABS(ez) < 0x800) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - 0x3D3D2;
    ez = g_worldPlayer->pos.vz + 0x36C0F;
    if (ABS(ex) < 0x500 && ABS(ez) < 0x500) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - 0x4A02F;
    ez = g_worldPlayer->pos.vz + 0x11DD8;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - 0xC02F;
    ez = g_worldPlayer->pos.vz + 0x1D9EC;
    if (ABS(ex) < 0x500 && ABS(ez) < 0x500) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - 0x17A3B;
    ez = g_worldPlayer->pos.vz + 0x4014A;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        g_worldUsePolyCache = 0;
    }
    wm_wrapPos(&g_worldPlayer->pos.vx, &g_worldPlayer->pos.vz);
}

u8 wm_tryStep(VECTOR *pos, s32 angle, s32 moveMode, s32 speed, u32 *outCell, s32 *outHeight) {
    VECTOR v;
    u32 cell;
    s32 h;
    WmMoveMode *mode;
    WmPolyCache *c;
    u8 ok;
    s32 k;
    s32 ang;
    u8 a;
    u8 b;
    u8 x;
    u8 y;

    k = g_worldActorTypeInfo[g_worldPlayer->actorType].polyCache;
    mode = &g_worldMoveModes[moveMode];
    v = *pos;
    c = &g_worldPolyCache[k];
    v.vx += math_sin(g_worldPlayerHeading + (ang = angle + 0x800)) * speed >> 12;
    v.vz += math_cos(g_worldPlayerHeading + ang) * speed >> 12;
    if ((u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2 == WM_TOPO_NO_ENCOUNTER && g_worldMoveModeInfo->controlType != WM_CTRL_FLY) {
        wm_getGroundHeight(&v, &cell, &h, NULL, (u16 *)&v.vy);
    } else {
        wm_getGroundHeight(&v, &cell, &h, c, (u16 *)&v.vy);
    }
    if (h >= 0) {
        ok = wm_testCellType(mode->allowedCells, cell);
        if (ok) {
            if (mode->checkTransitions != 0) {
                a = wm_testCellType(g_worldWaterCellTypes, cell);
                b = wm_testCellType(g_worldLandCellTypes, cell);
                x = wm_testCellType(g_worldWaterCellTypes, g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr);
                y = wm_testCellType(g_worldLandCellTypes, g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr);
                if (b & x) {
                    ok = 0;
                }
                if (y & a) {
                    ok = 0;
                }
            }
            if (ok) {
                if (pos != NULL) {
                    *pos = v;
                }
                if (outHeight != NULL) {
                    *outHeight = h;
                }
                if (outCell != NULL) {
                    *outCell = cell;
                }
                return 1;
            }
        }
    }
    return 0;
}

u8 wm_findGetoffPos(ActorPos *out) {
    EvtObject *act;
    s32 d[2];
    ActorPos v;
    u32 cell;
    u8 ok;
    u8 r;
    s32 a;
    s32 i;
    s32 j;
    s32 t;
    s32 k;
    s32 x;
    s32 z;

    memset(&v, 0, sizeof(v));
    ok = 0;
    if (g_worldAmbientSeDist[1] < 0x16 && ((u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2) == 0x17) {
        return 0;
    }
    g_worldFlyTurning = 0;
    act = g_worldPlayer;
    g_worldGetoffSearching = 1;
    v = act->pos;
    switch (g_worldMoveMode) {
    case WM_MOVE_HILDA_GARDE:
    case WM_MOVE_INVINCIBLE:
        x = (g_worldPlayer->pos.vx >> 3) - 0x17FC;
        z = (g_worldPlayer->pos.vz >> 3) + 0x3B48;
        if (SquareRoot0(x * x + z * z) < 0xB4) {
            return 0;
        }
        if (!wm_tryStep((VECTOR *)&v, 0, 0, 0, &cell, 0)) {
            return 0;
        }
        t = (u8)cell >> 2;
        if (t >= 0x2D && (t < 0x2F || t == WM_TOPO_NO_ENCOUNTER)) {
            return 0;
        }
        break;
    case WM_MOVE_BOAT:
        if (!wm_tryStep((VECTOR *)&v, 0, 10, 0, &cell, 0)) {
            return 0;
        }
        if (((u8)cell >> 2) != WM_TOPO_SHALLOWS) {
            return 0;
        }
        break;
    default:
        if (!wm_tryStep((VECTOR *)&v, 0, 0, 0, &cell, 0)) {
            return 0;
        }
        if (((u8)cell >> 2) == WM_TOPO_NO_ENCOUNTER) {
            return 0;
        }
        break;
    }
    g_worldGetoffSearching = 0;
    for (a = 0; a < 0x800; a += 0x100) {
        d[0] = (a + 0xC00) % 0x1000;
        d[1] = -a + 0xC00;
        for (i = 0; i < 2; i++) {
            r = 1;
            if (ok) {
                continue;
            }
            for (j = 1; j < 9; j++) {
                t = g_worldMoveModeInfo->getoffDist * j / 8;
                v = g_worldPlayer->pos;
                r &= wm_tryStep((VECTOR *)&v, d[i], 0, t, &cell, 0);
            }
            ok = r;
            if (ok) {
                switch (g_worldMoveMode) {
                case WM_MOVE_WALK:
                    if (abs(v.vy - g_worldPlayer->pos.vy) >= 0x1F5) {
                        ok = 0;
                    }
                    break;
                case WM_MOVE_BOAT:
                    if (wm_testCellType(g_worldLandCellTypes, cell)) {
                        ok = 0;
                    }
                    break;
                case WM_MOVE_HILDA_GARDE:
                case WM_MOVE_INVINCIBLE:
                    if (abs(v.vy - w_frameGetParameter(WM_PARAM_RIDE_Y)) >= 0x15F) {
                        ok = 0;
                    }
                    break;
                }
            }
        }
        if (ok) {
            k = (u8)cell >> 2;
            switch (k) {
            case WM_TOPO_FOREST:
            case WM_TOPO_FOREST_2:
            case WM_TOPO_FOREST_3:
                k = 0;
                break;
            case WM_TOPO_SHALLOWS:
                k = 1;
                break;
            case 0x36:
                k = 2;
                break;
            case 0x37:
                k = 3;
                break;
            case 0x38:
                k = 4;
                break;
            case 0x39:
                k = 5;
                break;
            case 0x33:
                k = 6;
                break;
            case 0x30:
                k = 7;
                break;
            default:
                k = 8;
                break;
            }
            v.vy += g_worldTerrainHeightOffsets[1][k];
            *out = v;
            return 1;
        }
    }
    return 0;
}

void w_movementChange(void) {
    s8 old;
    WmMoveMode *p;
    WmMoveMode *prev;

    evt_resetIdleTimer(0);
    g_worldSpeedSmooth = 0;
    g_worldClimbSmooth = 0;
    wm_autoPilotOff();
    old = g_worldMoveMode;
    prev = g_worldMoveModeInfo;
    g_worldMoveMode = g_evtGlobalVars.b[0xBE];
    g_worldMoveModeInfo = p = &g_worldMoveModes[g_worldMoveMode];
    if (old == WM_MOVE_BOAT) {
        wm_seStop(prev->loopSe);
    }
    if (p->loopSe != 0) {
        wm_sePlay(p->loopSe, 0);
        if (g_worldMoveMode != WM_MOVE_BOAT) {
            wm_seFadeVolume(p->loopSe, 0x3C, 0x3F);
        }
        if ((u8)(g_worldMoveMode - WM_MOVE_HILDA_GARDE) < 2) {
            wm_sePlay(WM_SE_AIRSHIP_WATER, 0);
        }
        g_worldCamera->viewForceNormal = 1;
    }
    if (prev == NULL || prev->music != p->music) {
        w_musicRequestSend(p->music);
    }
    if (g_worldMoveMode == WM_MOVE_WALK && (u8)(old - WM_MOVE_CHOCOBO_FIELD) < 6) {
        g_worldPlayerHeading = (g_worldMountActor->pos.rotY + 0x400) & 0xFFF;
    }
    if ((u8)(g_worldMoveMode - WM_MOVE_CHOCOBO_FIELD) < 6 && old == WM_MOVE_WALK) {
        g_worldPlayerHeading = g_worldMountActor->pos.rotY & 0xFFF;
    }
    if (g_worldMoveModes[old].airborne == g_worldMoveModes[g_worldMoveMode].airborne) {
        wm_blendCameraPreset(p->cameraRow, 0x200);
    } else {
        wm_blendCameraPreset(p->cameraRow, 0x40);
    }
    g_worldCameraFourProbes = g_worldMoveModeInfo->airborne == 0;
    wm_updateActors();
    if (g_worldMapDebugFlags & WM_DEBUG_MOVEMENT) {
        printf("w_movementChange : %d -> %d\n", old, g_worldMoveMode);
    }
}

void wm_controlWalk(void) {
    s32 x;
    s32 z;
    s32 ang;
    u16 mv;
    s32 a;
    s16 cur;
    u32 k;
    s32 rx;
    s32 lo;
    s32 t;

    wm_getLeftStickX(&x);
    wm_getLeftStickY(&z);
    z = -z;
    t = -ratan2(z, x) + 0x400;
    g_worldCameraTurning = 0;
    ang = t & 0xFFF;
    g_worldCameraFastRise = 0;
    g_worldFlyTurning = 0;
    mv = ((x | z) != 0) << 12;
    if (wm_getPadHeld() & PAD_DOWN) {
        g_worldCameraFastRise = 1;
    } else {
        g_worldCameraFastRise = 0;
    }
    if (wm_getPadHeld() & BTN_L1) {
        g_worldCameraTurning = 1;
        g_worldCameraTurnedByHand = 1;
        g_worldCamera->yaw -= 0x20;
    }
    if (wm_getPadHeld() & BTN_R1) {
        g_worldCameraTurning = 1;
        g_worldCameraTurnedByHand = 1;
        g_worldCamera->yaw += 0x20;
    }
    g_worldCamera->yaw &= 0xFFF;
    if (mv != 0 || g_worldCameraTurning != 0) {
        g_worldCamera->stickSide = math_sin(ang);
        if (mv != 0) {
            g_worldCameraTurnedByHand = 0;
            if ((u32)(ang - 0x600) <= 0x400) {
                g_worldCamera->yawOffset = 0x800;
            } else {
                g_worldCamera->yawOffset = 0;
            }
            g_worldPlayerHeading = (g_worldCamera->yaw + ang) & 0xFFF;
            g_worldPlayerSpeed = mv * g_worldMoveModeInfo->speed >> 12;
        }
    }
    a = g_worldPlayerHeading;
    cur = g_worldPlayer->pos.rotY;
    if (a - cur > 0x800) {
        a -= 0x1000;
    }
    if (a - cur < -0x800) {
        a += 0x1000;
    }
    a -= cur;
    if ((x | z) != 0) {
        g_worldPlayer->pos.rotY = (g_worldPlayer->pos.rotY + a / 3) & 0xFFF;
    }
    if (g_worldPlayerPosChanged != 0 && mv != 0) {
        if (wm_testCellType_inline(g_worldWaterCellTypes, (u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2)) {
            wm_spawnEffect(g_worldPlayer->pos.vx, -100, g_worldPlayer->pos.vz, WM_FX_WATER_RIPPLE, 24000);
        }
        if (!(g_worldFrameCount & 3)) {
            k = (u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2;
            if (k == 0x1E || k == WM_TOPO_BEACH || k == 0x23) {
                wm_spawnEffect(g_worldPlayer->pos.vx, g_worldPlayer->pos.vy, g_worldPlayer->pos.vz, WM_FX_BEACH_SPLASH, 0x900);
            }
            if (((u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2) == WM_TOPO_SAND) {
                wm_spawnEffect(g_worldPlayer->pos.vx, g_worldPlayer->pos.vy, g_worldPlayer->pos.vz, WM_FX_SAND_DUST, 0x2000);
            }
        }
        if (!(g_worldFrameCount & 1) && (u32)(((u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2) - WM_TOPO_FOREST) < 3) {
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            rx = g_worldPlayer->prevPos.vx + *g_randPtr++ - 0x80;
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            lo = *g_randPtr++;
            wm_spawnEffect(rx, g_worldPlayer->prevPos.vy, g_worldPlayer->prevPos.vz + lo - 0x80, WM_FX_FOREST_LEAVES, 0x6D80);
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            rx = g_worldPlayer->prevPos.vx + *g_randPtr++ - 0x80;
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            lo = *g_randPtr++;
            wm_spawnEffect(rx, g_worldPlayer->prevPos.vy, g_worldPlayer->prevPos.vz + lo - 0x80, WM_FX_FOREST_LEAVES, 0x6D80);
        }
    }
}

void wm_controlSteer(void) {
    s32 lx;
    s32 rx;
    s32 ly;
    s16 v;
    s32 sp;
    s32 vol;
    s32 a;
    s32 ang;
    WmMoveMode *m;
    s32 dx;
    s32 dz;

    wm_getRightStickY(&lx);
    wm_getLeftStickX(&rx);
    if ((u8)wm_getLeftStickY(&ly)) {
        ly = 0;
    }
    ly = -ly;
    if (lx > 0 && ly < lx) {
        ly = lx;
    }
    if (lx < 0 && lx < ly) {
        ly = lx;
    }
    a = g_worldMoveModeInfo->turnSpeed * rx >> 7;
    g_worldCameraTurnedByHand = 0;
    g_worldPlayerTurn = a;
    if (wm_getPadHeld() & BTN_L1) {
        g_worldCameraTurnedByHand = 1;
        g_worldCamera->yaw -= 0x20;
    }
    if (wm_getPadHeld() & BTN_R1) {
        g_worldCameraTurnedByHand = 1;
        g_worldCamera->yaw += 0x20;
    }
    g_worldCamera->yawOffset = 0;
    g_worldCamera->yaw &= 0xFFF;
    m = g_worldMoveModeInfo;
    v = g_worldSpeedSmooth + (m->speed * ly / g_worldStickDiv - g_worldSpeedSmooth) / g_worldSpeedSmoothDiv;
    g_worldSpeedSmooth = v;
    sp = v / g_worldSpeedScaleDiv;
    g_worldPlayerSpeed = sp;
    vol = ABS(sp) / 8 + 0x14;
    if (vol >= 0x80) {
        vol = 0x7F;
    }
    wm_seFadeVolume(m->loopSe, 0x3C, vol);
    g_worldPlayerHeading = g_worldPlayer->pos.rotY = (g_worldPlayer->pos.rotY + g_worldPlayerTurn) & 0xFFF;
    if (ABS(g_worldPlayerSpeed) > 80 && g_worldPlayerPosChanged != 0) {
        ang = (g_worldPlayerSpeed <= 0) << 11;
        dx = math_sin(ang + g_worldPlayer->pos.rotY) >> 2;
        dz = math_cos(ang + g_worldPlayer->pos.rotY) >> 2;
        wm_spawnEffect(g_worldPlayer->pos.vx + dx, -100, g_worldPlayer->pos.vz + dz, WM_FX_BOAT_WAKE, 15000);
        if (g_worldFrameCount & 1) {
            dx = math_sin(ang + g_worldPlayer->pos.rotY + 0x80) >> 3;
            dz = math_cos(ang + g_worldPlayer->pos.rotY + 0x80) >> 3;
            wm_spawnEffect(g_worldPlayer->pos.vx + dx, -100, g_worldPlayer->pos.vz + dz, WM_FX_BOAT_SIDE_WAKE, 15000);
            dx = math_sin(ang + g_worldPlayer->pos.rotY - 0x80) >> 3;
            dz = math_cos(ang + g_worldPlayer->pos.rotY - 0x80) >> 3;
            wm_spawnEffect(g_worldPlayer->pos.vx + dx, -100, g_worldPlayer->pos.vz + dz, WM_FX_BOAT_SIDE_WAKE, 15000);
        }
    }
}

void wm_controlFly(void) {
    s32 x;
    s32 y;
    s32 z;
    s32 ox;
    s32 btn;
    s32 dx;
    s32 dz;
    s32 d;
    s32 k;
    s32 r;
    s32 h;
    u32 t;
    u8 f;
    s32 q;
    u32 w;
    s32 a;
    s32 b;
    s16 sa;
    s16 sb;
    s32 ex;
    s32 ez;

    wm_getLeftStickX(&x);
    wm_getLeftStickY(&y);
    wm_getRightStickY(&z);
    if (x != 0) {
        g_worldFlyTurning = 1;
    } else {
        g_worldFlyTurning = 0;
    }
    if (z < 0) {
        g_worldCameraFastRise = 1;
    } else {
        g_worldCameraFastRise = 0;
    }
    g_worldCameraTurnedByHand = 0;
    y = -y;
    ox = x;
    btn = wm_getPadHeld();
    if ((btn & (BTN_L1 | BTN_R1)) && g_worldCameraFlags->autoTurnOff == 0 && g_worldAutoPilot != 0) {
        wm_toggleCameraAutoTurn();
    }
    if (g_worldCameraFlags->autoTurnOff == 0) {
        if (btn & BTN_L1) {
            x = -0x7F;
        }
        if (btn & BTN_R1) {
            x = 0x7F;
        }
    } else {
        if (btn & BTN_R1) {
            g_worldCameraTurnedByHand = 1;
            g_worldCamera->yaw += 0x10;
        }
        if (btn & BTN_L1) {
            g_worldCameraTurnedByHand = 1;
            g_worldCamera->yaw -= 0x10;
        }
    }
    g_worldCamera->yawOffset = 0;
    a = g_worldMoveModeInfo->speed * z / g_worldStickDiv;
    g_worldPlayerTurn = g_worldMoveModeInfo->turnSpeed * x >> 7;
    q = g_worldSpeedSmoothDiv;
    b = g_worldMoveModeInfo->climbSpeed * y / g_worldStickDiv;
    a = (a - g_worldSpeedSmooth) / q;
    b = (b - g_worldClimbSmooth) / q;
    sa = g_worldSpeedSmooth + a;
    sb = g_worldClimbSmooth + b;
    g_worldSpeedSmooth = sa;
    g_worldClimbSmooth = sb;
    g_worldPlayerSpeed = sa / g_worldSpeedScaleDiv;
    g_worldPlayerClimbSpeed = sb / g_worldSpeedScaleDiv;
    if (g_worldFastFly != 0) {
        g_worldPlayerTurn *= 2;
        g_worldPlayerSpeed *= 2;
    }
    if (g_worldMoveMode == WM_MOVE_INVINCIBLE) {
        g_worldPlayerTurn = g_worldPlayerTurn * (600 - g_worldPlayerSpeed / 2) >> 8;
    }
    if (g_worldMoveMode == WM_MOVE_HILDA_GARDE) {
        g_worldPlayerTurn = g_worldPlayerTurn * (580 - g_worldPlayerSpeed / 2) >> 8;
    }
    if (x != 0 || z < 0) {
        wm_autoPilotOff();
    }
    switch (g_worldAutoPilot) {
    case 1:
        ex = (g_worldNaviPlaces[g_worldNaviPage][g_worldAutoPilotDest].worldX - g_worldPlayer->pos.vx) >> 5;
        ez = (g_worldNaviPlaces[g_worldNaviPage][g_worldAutoPilotDest].worldZ - g_worldPlayer->pos.vz) >> 5;
        r = SquareRoot0(ex * ex + ez * ez);
        g_worldPlayerSpeed = r;
        if (g_worldPlayerSpeed > 400) {
            g_worldPlayerSpeed = 400;
        }
        if (g_worldAutoPilotDist < g_worldPlayerSpeed || g_worldPlayerSpeed < 50) {
            wm_autoPilotOff();
        }
        g_worldAutoPilotDist = g_worldPlayerSpeed;
    case 2:
        dx = g_worldPlayer->pos.vx - g_worldNaviPlaces[g_worldNaviPage][g_worldAutoPilotDest].worldX;
        dz = g_worldPlayer->pos.vz - g_worldNaviPlaces[g_worldNaviPage][g_worldAutoPilotDest].worldZ;
        if (dx > 0x30000) {
            dx -= 0x60000;
        }
        if (dx < -0x30000) {
            dx += 0x60000;
        }
        if (dz > 0x28000) {
            dz -= 0x50000;
        }
        if (dz < -0x28000) {
            dz += 0x50000;
        }
        dz = (-ratan2(dz, dx) + 0x400) & 0xFFF;
        dz -= g_worldPlayer->pos.rotY;
        if (dz < -0x800) {
            dz += 0x1000;
        }
        if (dz > 0x800) {
            dz -= 0x1000;
        }
        if (g_worldAutoPilotDist > 50) {
            if (dz > 20) {
                g_worldPlayerTurn = 20;
            } else if (dz < -20) {
                g_worldPlayerTurn = -20;
            } else if (g_worldAutoPilot == 2) {
                g_worldAutoPilot = 1;
            }
        }
        break;
    }
    g_worldPlayerHeading = g_worldPlayer->pos.rotY = (g_worldPlayer->pos.rotY + g_worldPlayerTurn) & 0xFFF;
    if (g_worldPlayerTurn > 0 && -ox < g_worldCameraRoll) {
        g_worldCameraRoll -= ox / 8;
    }
    if (g_worldPlayerTurn < 0 && g_worldCameraRoll < -ox) {
        g_worldCameraRoll -= ox / 8;
    }
    if (g_worldPlayerSpeed == 0) {
        g_worldCameraRoll += ox / 8;
    }
    if ((s8)g_worldMoveModeInfo->maxRoll < g_worldCameraRoll) {
        g_worldCameraRoll = (s8)g_worldMoveModeInfo->maxRoll;
    }
    if (g_worldCameraRoll < -(s8)g_worldMoveModeInfo->maxRoll) {
        g_worldCameraRoll = -(s8)g_worldMoveModeInfo->maxRoll;
    }
    if (g_worldPlayerTurn > 0 && -(ox * 2) < g_worldPlayer->rotZ) {
        g_worldPlayer->rotZ -= ox / 4;
    }
    if (g_worldPlayerTurn < 0 && g_worldPlayer->rotZ < -(ox * 2)) {
        g_worldPlayer->rotZ -= ox / 4;
    }
    if (g_worldPlayer->rotZ > (s8)g_worldMoveModeInfo->maxRoll * 2) {
        g_worldPlayer->rotZ = (s8)g_worldMoveModeInfo->maxRoll * 2;
    }
    if (g_worldPlayer->rotZ < -(s8)g_worldMoveModeInfo->maxRoll * 2) {
        g_worldPlayer->rotZ = -(s8)g_worldMoveModeInfo->maxRoll * 2;
    }
    if (g_worldFrameCount & 1) {
        t = (u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2;
        f = wm_testCellType_inline2(g_worldWaterCellTypes, t);
        switch (g_worldPlayer->actorType) {
        case WM_ACTOR_AIRSHIP:
        case WM_ACTOR_AIRSHIP2:
            if (f && abs(g_worldPlayerSpeed) > 60) {
                h = g_worldPlayer->pos.vy;
                if (h >= -0x800) {
                    g_worldSprayTarget = 0x1000 - abs(h + 400) * 2;
                    wm_spawnEffect(g_worldPlayer->pos.vx, -200, g_worldPlayer->pos.vz, WM_FX_AIRSHIP_SPRAY, 32000);
                    h = abs(g_worldPlayerSpeed);
                    if (h >= 0x200) {
                        h = 0x1FF;
                    }
                    dx = abs(g_worldPlayer->pos.vy) >> 6;
                    if (dx > 48) {
                        dx = 48;
                    }
                    dx = 48 - dx;
                    dx *= h;
                    wm_seFadeVolume(WM_SE_AIRSHIP_WATER, 20, (dx >> 9) & 0xFF);
                } else {
                    wm_seFadeVolume(WM_SE_AIRSHIP_WATER, 30, 0);
                }
            } else {
                wm_seFadeVolume(WM_SE_AIRSHIP_WATER, 30, 0);
            }
            if (t == WM_TOPO_SAND && g_worldPlayerSpeed != 0 && g_worldPlayer->pos.vy >= -0x708) {
                g_worldSprayTarget = 0x1000 - (g_worldPlayer->pos.vy - 0x514) * 2;
                wm_spawnEffect(g_worldPlayer->pos.vx, g_worldPlayer->pos.vy + 100, g_worldPlayer->pos.vz, WM_FX_AIRSHIP_SAND, 32000);
            }
            break;
        case WM_ACTOR_CHOCOBO4:
            if (f && abs(g_worldPlayerSpeed) > 30 && g_worldPlayer->pos.vy >= -100) {
                g_worldSprayTarget = 0x1000 - abs(g_worldPlayer->pos.vy + 100) * 2;
                wm_spawnEffect(g_worldPlayer->pos.vx, -200, g_worldPlayer->pos.vz, WM_FX_CHOCOBO_SPRAY, 19200);
            }
            if (t == WM_TOPO_SAND && g_worldPlayerSpeed != 0 && g_worldPlayer->pos.vy >= -0x708) {
                g_worldSprayTarget = 0x1000 - abs(g_worldPlayer->pos.vy + 0x708) * 2;
                wm_spawnEffect(g_worldPlayer->pos.vx, g_worldPlayer->pos.vy + 100, g_worldPlayer->pos.vz, WM_FX_CHOCOBO_SAND, 25600);
            }
            break;
        }
    }
}

void wm_cameraAutoTurnWalk(void) {
    s32 a;
    s32 cam;
    s32 d;
    s32 t;
    s32 q;

    a = (g_worldCamera->yaw + g_worldCamera->yawOffset) & 0xFFF;
    cam = g_worldPlayerHeading & 0xFFF;
    if (a - cam >= 0x800) {
        a -= 0x1000;
    }
    if (a - cam < -0x800) {
        a += 0x1000;
    }
    d = (g_worldPlayerHeading - a) * 0xC00 / 4096;
    g_worldAutoTurnDeadZone = abs(g_worldCamera->stickSide) >> 3;
    if (abs(d) < g_worldAutoTurnDeadZone) {
        d = 0;
    }
    if (g_worldPlayerSpeed != 0) {
        g_worldCameraTurnGain = 0x1000;
    } else {
        g_worldCameraTurnGain -= 0x200;
        if (g_worldCameraTurnGain < 0) {
            g_worldCameraTurnGain = 0;
        }
    }
    if (g_worldCameraTurnedByHand == 0 && g_worldCameraFlags->autoTurnOff == 0 && g_worldState == WM_STATE_RUNNING) {
        g_worldAutoTurnDiv = abs(d);
        if (abs(d) < g_worldAutoTurnDivMin) {
            g_worldAutoTurnDiv = g_worldAutoTurnDivMin;
        }
        if (g_worldAutoTurnDiv > g_worldAutoTurnDivMax) {
            g_worldAutoTurnDiv = g_worldAutoTurnDivMax;
        }
        t = (((0x800 - abs(d)) * g_worldAutoTurnBoost >> 12) + 0x800) * (2 * g_worldCameraTurnGain);
        q = (d * 256 / g_worldAutoTurnDiv) * (t / 4096);
        g_worldCamera->yaw += q / 4096;
    }
    g_worldCamera->yaw &= 0xFFF;
}

void wm_cameraAutoTurnVehicle(void) {
    s32 a;
    s32 step;
    s32 q;
    s32 cur;

    a = g_worldCamera->yaw;
    if (a - g_worldPlayer->pos.rotY > 0x800) {
        g_worldCamera->yaw = a - 0x1000;
    }
    if (g_worldCamera->yaw - g_worldPlayer->pos.rotY < -0x800) {
        g_worldCamera->yaw += 0x1000;
    }
    if (a < 0) {
        a += 0x1000;
    }
    a %= 4096;
    cur = g_worldPlayer->pos.rotY;
    if (a - cur > 0x800) {
        a -= 0x1000;
    }
    if (a - cur < -0x800) {
        a += 0x1000;
    }
    a = cur - a;
    a %= 4096;
    if (g_worldCameraTurnedByHand == 0 && g_worldCameraFlags->autoTurnOff == 0) {
        step = 16;
        if (g_worldCameraRecenter != 0) {
            step = ABS(a) * 256 >> 12;
            if (step == 0) {
                step = 1;
            }
        }
        q = a % 4096 / step;
        if (q != 0) {
            g_worldCamera->yaw += q;
        } else if (step >= 2) {
            do {
                step /= 2;
                if (step == 0) {
                    break;
                }
                q = a % 4096 / step;
            } while (q == 0);
            g_worldCamera->yaw += q;
        } else {
            g_worldCamera->yaw = g_worldPlayer->pos.rotY;
        }
        g_worldCamera->yaw &= 0xFFF;
        if (ABS(a) < 100) {
            g_worldCameraRecenter = 0;
        }
    }
}

void wm_snapActorsToFloor(void) {
    EvtListNode *n;
    EvtObject *o;
    WmActorType *c;
    ActorPos pos;
    s32 polyIdx;
    u16 h;

    n = evt_getObjectList();
    if (n != NULL) {
        do {
            o = n->obj;
            if (o->kind == EVT_KIND_ACTOR) {
                pos = o->pos;
                c = &g_worldActorTypeInfo[o->actorType];
                wm_getGroundHeight((VECTOR *)&pos, &c->cellAttr, &polyIdx, NULL, &h);
                c->floorY = h;
                c->rideOffset = wm_getTerrainHeightOffset(c->terrainClass, c->cellAttr, 0);
                wm_updateActorHeight(o);
                o->prevPos = o->pos;
            }
            n = n->next;
        } while (n != NULL);
    }
}

void w_movementChangeSP1(void) {
    s8 old;
    WmMoveMode *p;

    wm_autoPilotOff();
    g_worldSpeedSmooth = 0;
    g_worldClimbSmooth = 0;
    old = g_worldMoveMode;
    p = &g_worldMoveModes[0];
    wm_seFadeVolume(g_worldMoveModeInfo->loopSe, 0x7F, 0);
    wm_seFadeVolume(WM_SE_AIRSHIP_WATER, 0x7F, 0);
    w_musicRequestSend(p->music);
    wm_blendCameraPreset(p->cameraRow, 0x40);
    if (g_worldMapDebugFlags & WM_DEBUG_MOVEMENT) {
        printf("w_movementChange SP1 : %d -> %d\n", old, 0);
    }
}

/* padded: the next jump table is 8-aligned */
const char s_wmStrMovementChangeSP2[0x1C] = "w_movementChange SP2\n";

void w_movementChangeSP2(void) {
    evt_resetIdleTimer(0);
    wm_seStop(WM_SE_AIRSHIP_WATER);
    wm_seStop(g_worldMoveModeInfo->loopSe);
    g_worldMoveMode = g_evtGlobalVars.b[0xBE];
    g_worldMoveModeInfo = &g_worldMoveModes[g_worldMoveMode];
    if (evt_getPlayer() != NULL) {
        g_worldPlayer = evt_getPlayer();
    }
    g_worldCameraFourProbes = g_worldMoveModeInfo->airborne == 0;
    wm_updateActors();
    if (g_worldMapDebugFlags & WM_DEBUG_MOVEMENT) {
        printf(s_wmStrMovementChangeSP2);
    }
}

u8 wm_testCellType(u32 *allowed, u32 cellAttr) {
    cellAttr = WM_ATTR_TOPOGRAPHY(cellAttr);
    if (cellAttr >= 32) {
        if ((allowed[0] >> (cellAttr - 32)) & 1) {
            return 1;
        }
    } else if ((allowed[1] >> cellAttr) & 1) {
        return 1;
    }
    return 0;
}

s32 wm_getLeftStickX(s32 *outValue) {
    s32 v;
    s32 b;

    *outValue = 0;
    if (((g_padBuf[0].hdr >> 12) & 0xF) == PAD_ID_ANALOG) {
        v = g_padAnalog[0].b[2] - 0x80;
        if (g_padAnalog[0].b[2] - 0x40U <= 0x80) {
            v = 0;
        }
        if (v > 0x40) {
            v = (v - 0x40) * 2;
        }
        if (v < -0x40) {
            v = (v + 0x40) * 2;
        }
        if (v != 0) {
            *outValue = v;
            return 1;
        }
    }
    b = wm_getPadHeld();
    if (b & PAD_LEFT) {
        *outValue = -0x80;
    }
    if (b & PAD_RIGHT) {
        *outValue = 0x80;
    }
    return 0;
}

s32 wm_getLeftStickY(s32 *outValue) {
    s32 v;

    *outValue = 0;
    if (((g_padBuf[0].hdr >> 12) & 0xF) == PAD_ID_ANALOG) {
        v = g_padAnalog[0].b[3] - 0x80;
        if (g_padAnalog[0].b[3] - 0x40U <= 0x80) {
            v = 0;
        }
        if (v > 0x40) {
            v = (v - 0x40) * 2;
        }
        if (v < -0x40) {
            v = (v + 0x40) * 2;
        }
        if (v != 0) {
            *outValue = v;
            return 1;
        }
    }
    if (wm_getPadHeld() & PAD_UP) {
        *outValue = -0x80;
    }
    if (wm_getPadHeld() & PAD_DOWN) {
        *outValue = 0x80;
    }
    return 0;
}

s32 wm_getRightStickY(s32 *outValue) {
    s32 v;

    *outValue = 0;
    if (((g_padBuf[0].hdr >> 12) & 0xF) == PAD_ID_ANALOG) {
        v = 0x80 - g_padAnalog[0].b[1];
        if (v + 0x40U <= 0x80) {
            v = 0;
        }
        if (v > 0x40) {
            v = (v - 0x40) * 2;
        }
        if (v < -0x40) {
            v = (v + 0x40) * 2;
        }
        if (v != 0) {
            *outValue = v;
            return 1;
        }
    }
    if (wm_getPadHeld() & BTN_SQUARE) {
        *outValue = -0x80;
    }
    if (wm_getPadHeld() & BTN_CROSS) {
        *outValue = 0x80;
    }
    return 0;
}

void wm_autoPilotOn(void) {
    g_worldAutoPilot = 2;
    g_worldAutoPilotDist = 10000;
    g_worldAutoPilotDest = g_evtGlobalVars.b[EVT_GB_NAVI_PLACE] - 1;
    g_worldAutoPilotSavedAutoTurn = g_worldCameraFlags->autoTurnOff;
}

void wm_autoPilotOff(void) {
    if (g_worldAutoPilot != 0) {
        g_worldAutoPilotDist = 0;
        g_worldAutoPilot = 0;
        if (g_worldAutoPilotSavedAutoTurn != g_worldCameraFlags->autoTurnOff) {
            wm_toggleCameraAutoTurn();
        }
        g_worldCameraFlags->autoTurnOff = g_worldAutoPilotSavedAutoTurn;
    }
}

void wm_mountPalettePre(void) {
}

s32 wm_getTerrainHeightOffset(s32 terrainClass, u32 cellAttr, u8 *outFlat) {
    s32 i;
    switch ((s32)(cellAttr & 0xFC) >> 2) {
    case WM_TOPO_FOREST:
    case WM_TOPO_FOREST_2:
    case WM_TOPO_FOREST_3:
        i = 0;
        break;
    case WM_TOPO_SHALLOWS:
        i = 1;
        break;
    case 0x36:
        i = 2;
        break;
    case 0x37:
        i = 3;
        break;
    case 0x38:
        i = 4;
        break;
    case 0x39:
        i = 5;
        break;
    case 0x33:
        i = 6;
        break;
    case 0x30:
        i = 7;
        break;
    default:
        i = 8;
        break;
    }
    if (outFlat != NULL) {
        *outFlat = i < 1;
    }
    return g_worldTerrainHeightOffsets[terrainClass][i];
}

void wm_controlPlayer(void) {
    switch (g_worldMoveModeInfo->controlType) {
    case WM_CTRL_WALK:
        wm_controlWalk();
        break;
    case WM_CTRL_STEER:
        wm_controlSteer();
        break;
    case WM_CTRL_FLY:
        wm_controlFly();
        break;
    }
    if (g_worldMoveModeInfo->canToggleView != 0 && (wm_getPadPressed() & BTN_R2)) {
        wm_toggleCameraView();
    }
    if (wm_getPadPressed() & BTN_L2) {
        wm_toggleCameraAutoTurn();
    }
}

void wm_movementNop(void) {
}
