#include "ovl_0c.h"

void wm_initCamera(void) {
    void *cam = &g_gameState->gameData->worldCamera;

    g_worldCameraFlags = cam;
    g_worldCamera = cam;
    g_gameState->gameData->projDist = 0xDC;
    SetGeomOffset(SCREEN_W / 2, SCREEN_H / 2);
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
    SVECTOR rot;
    VECTOR upVec;
    MATRIX rotMat;
    s32 altitude;
    s32 angle;

    if (g_worldCameraHold == 0) {
        wm_updateCameraPreset();
    }
    g_gameState->gameData->projDist = g_worldCameraParams.projDist;
    g_worldCamera->yaw &= 0xFFF;
    g_worldCameraTargetHeight = g_worldCameraParams.targetHeight;
    if ((u8)g_worldMoveMode == WM_MOVE_HILDA_GARDE || (u8)g_worldMoveMode == WM_MOVE_INVINCIBLE) {
        altitude = g_worldPlayer->pos.vy + 0x1518;
        g_worldCameraTargetHeight = g_worldCameraParams.targetHeight - (altitude > 0 ? altitude >> 3 : altitude / 6);
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
    memset(&rot, 0, 8);
    rot.vz = g_worldCameraRoll;
    rot.vy = g_worldCamera->yaw;
    math_rotMatrixXYZ(&rot, &rotMat);
    rotMat.t[0] = 0;
    rotMat.t[1] = 0;
    rotMat.t[2] = 0;
    ApplyMatrixLV(&rotMat, &g_worldCameraUp, &upVec);
    wm_lookAt(g_worldViewMatrix, &g_worldCameraEyeRel, &g_worldCameraLookAt, &upVec);
    g_worldCameraDir[1] = 0;
    g_worldCameraDir[0] = g_worldCameraTarget[0] - g_worldCameraEye[0];
    g_worldCameraDir[2] = g_worldCameraTarget[2] - g_worldCameraEye[2];
    if (g_worldCameraDir[0] > 0xFFFF) {
        g_worldCameraDir[0] -= WM_MAP_SIZE_X;
    }
    if (g_worldCameraDir[2] > 0xFFFF) {
        g_worldCameraDir[2] -= WM_MAP_SIZE_Z;
    }
    if (g_worldCameraDir[0] < -0xFFFF) {
        g_worldCameraDir[0] += WM_MAP_SIZE_X;
    }
    if (g_worldCameraDir[2] < -0xFFFF) {
        g_worldCameraDir[2] += WM_MAP_SIZE_Z;
    }
    angle = ratan2(g_worldCameraDir[2], g_worldCameraDir[0]);
    g_worldCameraDir[0] = math_cos(angle);
    g_worldCameraDir[2] = math_sin(angle);
    VectorNormal((VECTOR *)g_worldCameraDir, (VECTOR *)g_worldCameraDir);
    if (*g_worldStoryCounterPtr < WM_STORY_CLEYRA_FALLEN && WM_ATTR_AREA(g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr) == WM_AREA_VUBE_DESERT) {
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
    s32 dx, dz, distSq, domeY;
    u8 *flagByte;

    eyeY = 0;
    if (evt_getWorldCamTargetObj() == 0) {
        y = g_worldPlayer->pos.vy + g_worldCameraTargetHeight;
        x = g_worldPlayer->pos.vx;
        z = g_worldPlayer->pos.vz;
        if ((g_worldCameraSmoothing = g_worldCameraSmoothX | g_worldCameraSmoothY | g_worldCameraSmoothZ) != 0) {
            if (g_worldCameraSmoothX == 1) {
                if (abs(g_worldCameraTarget[0] - x) < WM_CAM_SMOOTH_SNAP) {
                    g_worldCameraSmoothX = 0;
                } else {
                    g_worldCameraTarget[0] += (evt_getPlayer()->pos.vx - g_worldCameraTarget[0]) / WM_CAM_SMOOTH_DIV;
                }
            }
            if (g_worldCameraSmoothY == 1) {
                if (abs(g_worldCameraTarget[1] - y) < WM_CAM_SMOOTH_SNAP) {
                    g_worldCameraSmoothY = 0;
                } else {
                    g_worldCameraTarget[1] += (evt_getPlayer()->pos.vy - g_worldCameraTarget[1] + g_worldCameraTargetHeight) / WM_CAM_SMOOTH_DIV;
                }
            }
            if (g_worldCameraSmoothZ == 1) {
                if (abs(g_worldCameraTarget[2] - z) < WM_CAM_SMOOTH_SNAP) {
                    g_worldCameraSmoothZ = 0;
                } else {
                    g_worldCameraTarget[2] += (evt_getPlayer()->pos.vz - g_worldCameraTarget[2]) / WM_CAM_SMOOTH_DIV;
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
            dir = dy > WM_CAM_TARGET_DEADZONE;
            if (dy < -WM_CAM_TARGET_DEADZONE) {
                dir = WM_CAM_RAISE;
            }
            g_worldCameraEaseDiv = (dy >= 0 ? dy : -dy) >> 5;
            if (g_worldCameraEaseDiv < WM_CAM_TARGET_EASE_MIN) {
                g_worldCameraEaseDiv = WM_CAM_TARGET_EASE_MIN;
            }
            if (g_worldCameraEaseDiv > WM_CAM_TARGET_EASE_MAX) {
                g_worldCameraEaseDiv = WM_CAM_TARGET_EASE_MAX;
            }
            if (dir != WM_CAM_HOLD) {
                switch (g_worldMoveModeInfo->controlType) {
                case WM_CTRL_FLY:
                case WM_CTRL_STEER:
                    switch (dir) {
                    case WM_CAM_RAISE:
                        step = (dy + WM_CAM_TARGET_DEADZONE) / WM_CAM_TARGET_RAISE_DIV;
                        break;
                    case WM_CAM_LOWER:
                        step = (dy - WM_CAM_TARGET_DEADZONE) / WM_CAM_TARGET_LOWER_DIV;
                        break;
                    }
                    break;
                case WM_CTRL_WALK:
                case WM_CTRL_WALK_FAST:
                    switch (dir) {
                    case WM_CAM_RAISE:
                        step = ((dy + WM_CAM_TARGET_DEADZONE) << 8) / (g_worldCameraEaseDiv << 5);
                        break;
                    case WM_CAM_LOWER:
                        step = ((dy - WM_CAM_TARGET_DEADZONE) << 8) / (g_worldCameraEaseDiv << 5);
                        break;
                    }
                    break;
                }
            }
            g_worldCameraTarget[0] = x;
            g_worldCameraTarget[2] = z;
            if (ABS(step) >= WM_CAM_MIN_STEP && g_worldCameraLookupFix == 0) {
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
            probe.vx += WM_CAM_PROBE_OFS;
            wm_wrapPos(&probe.vx, &probe.vz);
            wm_getGroundHeight(&probe, &c0, &polyIdx, &g_worldCameraPolyCache[0], (u16 *)&h0);
            probe.vx -= 2 * WM_CAM_PROBE_OFS;
            wm_wrapPos(&probe.vx, &probe.vz);
            wm_getGroundHeight(&probe, &c1, &polyIdx, &g_worldCameraPolyCache[1], (u16 *)&h1);
            probe.vx += WM_CAM_PROBE_OFS;
            probe.vz += WM_CAM_PROBE_OFS;
            wm_wrapPos(&probe.vx, &probe.vz);
            wm_getGroundHeight(&probe, &c2, &polyIdx, &g_worldCameraPolyCache[2], (u16 *)&h2);
            probe.vz -= 2 * WM_CAM_PROBE_OFS;
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
            if (h < WM_CAM_GROUND_TOP_FLY) {
                h = WM_CAM_GROUND_TOP_FLY;
            }
        } else if (h < WM_CAM_GROUND_TOP) {
            h = WM_CAM_GROUND_TOP;
        }
        if (g_worldCameraParams.groundClearance + h < y2) {
            k = ((u8)cell >> 2 == WM_TOPO_MOUNTAIN) * WM_CAM_RISE_SHIFT_FAST;
            if (g_worldCameraFastRise != 0) {
                k = WM_CAM_RISE_SHIFT_FAST;
            }
            if (g_worldCameraTurning != 0) {
                k = WM_CAM_RISE_SHIFT_FAST;
            }
            if (g_worldCamera->viewBlend != 0 && g_worldCamera->viewBlend != 0x1000) {
                k = WM_CAM_RISE_SHIFT_BLEND;
            }
            y2 = g_worldCameraParams.groundClearance + h;
        }
        if (g_worldMoveModeInfo->controlType == WM_CTRL_FLY && y2 > WM_CAM_FLY_EYE_LOWEST) {
            y2 = WM_CAM_FLY_EYE_LOWEST;
        }
        dy = y2 - g_worldCameraEye[1];
        dir = dy > WM_CAM_EYE_DEADZONE;
        step = 0;
        if (dy < -WM_CAM_EYE_DEADZONE) {
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
                    step = (dy + WM_CAM_EYE_DEADZONE) / WM_CAM_EYE_RAISE_DIV;
                    break;
                case WM_CAM_LOWER:
                    step = (dy - WM_CAM_EYE_DEADZONE) / WM_CAM_EYE_LOWER_DIV;
                    break;
                }
                break;
            case WM_CTRL_WALK:
            case WM_CTRL_WALK_FAST:
                switch (dir) {
                case WM_CAM_RAISE:
                    step = ((dy + WM_CAM_EYE_DEADZONE) << 8) / (((g_worldCameraEaseScale - 2) * g_worldCameraEaseDiv * 7) >> k);
                    break;
                case WM_CAM_LOWER:
                    step = ((dy - WM_CAM_EYE_DEADZONE) << 8) / (g_worldCameraEaseScale * g_worldCameraEaseDiv * 3);
                    break;
                }
                ease = step >= 0 ? step : -step;
                if (ease > WM_CAM_EYE_STEP_MAX) {
                    ease = WM_CAM_EYE_STEP_MAX;
                }
                ease = (((WM_CAM_EYE_STEP_MAX - ease) * g_worldCameraEaseBoost >> 12) + 0x800) << 1;
                step *= ease;
                step /= 4096;
                break;
            }
        }
        g_worldCameraEye[0] = x2;
        g_worldCameraEye[2] = z2;
        if (ABS(step) >= WM_CAM_MIN_STEP && g_worldCameraLookupFix == 0) {
            g_worldCameraEye[1] += step;
        }
        eyeY = y2;
    }
    dx = g_worldCameraEye[0] - WM_IIFA_X;
    dz = g_worldCameraEye[2] - WM_IIFA_Z;
    if (ABS(dx) < WM_IIFA_CAM_R && ABS(dz) < WM_IIFA_CAM_R) {
        distSq = WM_IIFA_CAM_R * WM_IIFA_CAM_R - dx * dx;
        distSq -= dz * dz;
        domeY = -SquareRoot0(distSq) + WM_IIFA_CAM_Y;
        if (distSq > 0 && domeY < g_worldCameraEye[1]) {
            g_worldCameraEye[1] = domeY;
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
        flagByte = &g_evtGlobalVars.b[EVT_GB_GENERAL_FLAGS];
        if (*flagByte & EVT_GB_GENERAL_WM_WHITE_FADE) {
            fade_inAdditiveDefault();
            g_evtGlobalVars.b[EVT_GB_GENERAL_FLAGS] &= ~EVT_GB_GENERAL_WM_WHITE_FADE;
        } else {
            fade_inDefault();
        }
        break;
    }
    if (g_worldCameraEye[1] < WM_CAM_EYE_CEILING) {
        g_worldCameraEye[1] = WM_CAM_EYE_CEILING;
    }
    wm_wrapPos(&g_worldCameraTarget[0], &g_worldCameraTarget[2]);
    wm_wrapPos(&g_worldCameraEye[0], &g_worldCameraEye[2]);
}

void wm_updateCameraPreset(void) {
    s32 k;
    s32 row;
    WmCameraParams preset;

    k = g_worldAreaCameraGroup[w_frameGetParameter(0xC0)];
    row = g_worldMoveModeInfo->cameraRow;
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
        wm_calcCameraPreset(-g_worldPlayer->pos.vy, row, k, math_sin(g_worldCamera->viewBlend / 4), &g_worldCameraParams);
    }
}

void wm_calcCameraPreset(s32 altitude, s32 row, s32 col, s32 viewBlend, WmCameraParams *out) {
    WmCameraParams *lowPreset;
    WmCameraParams *highPreset;
    WmCameraParams *viewPreset;
    s32 t;
    s16 blended[5];

    viewPreset = &g_worldCameraPresets[g_worldCameraPresetIndex[row][col][2]];
    lowPreset = &g_worldCameraPresets[g_worldCameraPresetIndex[row][col][0]];
    highPreset = &g_worldCameraPresets[g_worldCameraPresetIndex[row][col][1]];
    t = wm_cameraAltitudeBlend(altitude) & 0xFFFF;
    blended[0] = wm_lerpShift(lowPreset->projDist, highPreset->projDist, t, 9);
    blended[1] = wm_lerpShift(lowPreset->distance, highPreset->distance, t, 0xE);
    blended[2] = wm_lerpShift(lowPreset->eyeHeight, highPreset->eyeHeight, t, 0xD);
    blended[3] = wm_lerpShift(lowPreset->groundClearance, highPreset->groundClearance, t, 0xD);
    blended[4] = wm_lerpShift(lowPreset->targetHeight, highPreset->targetHeight, t, 0xD);
    out->projDist = wm_lerpShift(blended[0], viewPreset->projDist, viewBlend, 9);
    out->distance = wm_lerpShift(blended[1], viewPreset->distance, viewBlend, 0xE);
    out->eyeHeight = wm_lerpShift(blended[2], viewPreset->eyeHeight, viewBlend, 0xD);
    out->groundClearance = wm_lerpShift(blended[3], viewPreset->groundClearance, viewBlend, 0xD);
    out->targetHeight = wm_lerpShift(blended[4], viewPreset->targetHeight, viewBlend, 0xD);
}

void wm_toggleCameraView(void) {
    u8 atEnd;

    if (g_worldCamera->viewForceNormal == 0
        && (*g_worldStoryCounterPtr >= WM_STORY_CLEYRA_FALLEN || WM_ATTR_AREA(g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr) != WM_AREA_VUBE_DESERT)) {
        atEnd = 0;
        if (g_worldCamera->viewBlend == 0) {
            atEnd = 1;
            g_worldCamera->viewBlendSpeed = 0x100;
        }
        if (g_worldCamera->viewBlend == 0x1000) {
            atEnd = 1;
            g_worldCamera->viewBlendSpeed = -0x100;
        }
        if (atEnd == 0) {
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
    s16 blend = g_worldCamera->viewBlend;

    if (blend == 0) {
        return 0;
    }
    if (blend == 0x1000) {
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
    VECTOR raw;
    VECTOR x;
    VECTOR y;
    VECTOR z;

    raw.vx = target->vx - eye->vx;
    raw.vy = target->vy - eye->vy;
    raw.vz = target->vz - eye->vz;
    VectorNormal(&raw, &z);
    if (z.vz == up->vz) {
        z.vz++;
    }
    gte_ldopv1(&z);
    gte_ldopv2(up);
    gte_op12();
    gte_stlvnl(&raw);
    VectorNormal(&raw, &x);
    gte_ldopv1(&z);
    gte_ldopv2(&x);
    gte_op12();
    gte_stlvnl(&raw);
    VectorNormal(&raw, &y);
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
    WmPalAnim *palAnim;
    u16 *pal;
    u16 *dst;
    EvtListNode *node;
    EvtObject *obj;
    s32 matched;
    s32 i;
    s32 j;

    id = 0;
    k = 0;
    palAnim = g_gameState->gameData->ctx.world->obj->palAnims;
    pal = w_framePackGetPtr(g_worldDataPack, WM_PACK_MOUNT_PALETTES);
    wm_mountPalettePre();
    for (node = evt_getObjectList(); node != NULL; node = node->next) {
        obj = node->obj;
        if (obj->state == EVT_STATE_RUNNING && (obj->flags & EVT_OBJ_ENABLED) && obj->kind == EVT_KIND_ACTOR) {
            if ((obj->actorType >= WM_ACTOR_CHOCOBO0 && obj->actorType <= WM_ACTOR_CHOCOBO4)) {
                id = obj->modelId;
                switch (obj->actorType) {
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
    matched = 0;
    for (i = 0; i < g_gameState->gameData->ctx.world->obj->palAnimCount; i++, palAnim++) {
        if (palAnim->modelId == id) {
            dst = palAnim->srcPal;
            switch (matched) {
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
            matched++;
        }
    }
}

/* find the world map actors (vehicles) and sync the camera with the player */
void wm_findVehicles(void) {
    EvtListNode *node;
    EvtObject *obj;

    for (node = evt_getObjectList(); node != NULL; node = node->next) {
        if (node->obj->kind == EVT_KIND_ACTOR) {
            obj = node->obj;
            switch (obj->actorType) {
            case WM_ACTOR_WALKER:
            case WM_ACTOR_WALKER_ALT:
                g_worldWalkerActor = obj;
                break;
            case WM_ACTOR_CHOCOBO0:
            case WM_ACTOR_CHOCOBO1:
            case WM_ACTOR_CHOCOBO2:
            case WM_ACTOR_CHOCOBO3:
            case WM_ACTOR_CHOCOBO4:
                g_worldMountActor = obj;
                break;
            case WM_ACTOR_BOAT:
                g_worldVehicleActor = obj;
                break;
            case WM_ACTOR_AIRSHIP:
                g_worldVehicleActor = obj;
                break;
            case WM_ACTOR_AIRSHIP2:
                g_worldVehicleActor = obj;
                break;
            }
            if (obj == evt_getPlayer()) {
                if (g_gameState->prevMode == GS_MODE_FIELD && g_gameState->gameData->inWorldFrame == 0) {
                    if (g_worldMoveModeInfo->cameraInFront != 0) {
                        g_worldCamera->yawOffset = 0x800;
                    } else {
                        g_worldCamera->yawOffset = 0;
                    }
                    g_worldCamera->yaw = obj->pos.rotY + g_worldCamera->yawOffset;
                }
                g_worldPlayerHeading = obj->pos.rotY;
            }
        }
    }
    g_gameState->gameData->worldCamKeep = 0;
    g_worldVehiclesFound = 1;
}

void wm_drawActors(void) {
    SVECTOR camDir;
    VECTOR relPos;
    VECTOR viewPos;
    VECTOR dv;
    SVECTOR drawPos;
    s32 z;
    s32 base;
    s32 angDiff;
    s32 ang;
    EvtListNode *node;
    EvtObject *act;
    u8 showShadow;

    VectorNormalS((VECTOR *)g_worldCameraDir, &camDir);
    base = (-ratan2(camDir.vz, camDir.vx) + 0x400) % 4096;
    pen_locate(10, 100);
    for (node = evt_getObjectList(); node != NULL; node = node->next) {
        act = node->obj;
        if (act->kind == EVT_KIND_ACTOR && act->state == EVT_STATE_RUNNING && (act->flags & EVT_OBJ_ENABLED) && (u8)wm_worldToView((VECTOR *)&act->pos, &relPos, &viewPos, &z)) {
            showShadow = 1;
            dv.vx = act->pos.vx - (g_worldCameraEye[0] - camDir.vx);
            dv.vz = act->pos.vz - (g_worldCameraEye[2] - camDir.vz);
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
            angDiff = ang % 4096;
            angDiff = angDiff - base;
            if (angDiff < -0x800) {
                angDiff += 0x1000;
            }
            if (angDiff > 0x800) {
                angDiff -= 0x1000;
            }
            if (((angDiff < 0 ? -angDiff : angDiff) < 400 || wm_getCameraViewState(ang) != 0) && (viewPos.vz >= WM_ACTOR_VIEW_NEAR && viewPos.vz < WM_ACTOR_VIEW_FAR) && viewPos.vx > -WM_ACTOR_VIEW_SIDE && viewPos.vx < WM_ACTOR_VIEW_SIDE && viewPos.vy > -WM_ACTOR_VIEW_SIDE && viewPos.vy < WM_ACTOR_VIEW_SIDE && relPos.vx > -WM_ACTOR_VIEW_FAR && relPos.vx < WM_ACTOR_VIEW_FAR && relPos.vz > -WM_ACTOR_VIEW_FAR && relPos.vz < WM_ACTOR_VIEW_FAR) {
                drawPos.vx = relPos.vx;
                drawPos.vy = relPos.vy;
                drawPos.vz = relPos.vz;
                wm_setActorFog(act, viewPos.vz >> 3);
                wm_drawActorModel(act, &drawPos, z);
                if (act->actorType == WM_ACTOR_BOAT) {
                    showShadow = 0;
                }
                if (act->actorType == WM_ACTOR_WALKER || act->actorType == WM_ACTOR_WALKER_ALT) {
                    if ((u8)g_worldMoveMode >= WM_MOVE_CHOCOBO_FIELD && (u8)g_worldMoveMode <= WM_MOVE_CHOCOBO_FLY) {
                        showShadow = 0;
                    }
                }
                if (g_worldActorTypeInfo[act->actorType].rideOffset < 4 && showShadow) {
                    wm_drawActorShadow(act, &drawPos, viewPos.vz);
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
    s32 cue;
    s32 fogOff;
    s32 fogBase;
    s32 i;
    s32 id;
    WmPalAnim *palAnim;
    RECT *rect;

    id = act->modelId;
    palAnim = g_gameState->gameData->ctx.world->obj->palAnims;
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
    fogOff = wm_lerp(g_worldWeatherData.work[0].fog[1].fogOffset, g_worldWeatherData.work[0].fog[0].fogOffset, t);
    col[0] = wm_lerp(g_worldWeatherData.work[0].fog[1].color[0], g_worldWeatherData.work[0].fog[0].color[0], t);
    col[1] = wm_lerp(g_worldWeatherData.work[0].fog[1].color[1], g_worldWeatherData.work[0].fog[0].color[1], t);
    col[2] = wm_lerp(g_worldWeatherData.work[0].fog[1].color[2], g_worldWeatherData.work[0].fog[0].color[2], t);
    SetFarColor((u8)col[0], (u8)col[1], (u8)col[2]);
    cue = (depth * 8 + (fogBase = fogOff - 0x8000)) >> 2;
    if (cue < 0) {
        cue = 0;
    }
    if (cue >= 0x1000) {
        cue = 0xFFF;
    }
    for (i = 0; i < g_gameState->gameData->ctx.world->obj->palAnimCount; i++, palAnim++) {
        if (palAnim->modelId == id) {
            rect = (RECT *)g_worldPrimPtr;
            if (cue < 0) {
                cue = 0;
            }
            if (cue > 0x1000) {
                cue = 0x1000;
            }
            wm_depthCueClut(palAnim->srcPal, palAnim->workPal[*g_worldBufIndex], cue, g_worldWeatherData.work[0].fxBackColor);
            rect->x = palAnim->clutX;
            rect->y = palAnim->clutY;
            rect->w = 16;
            rect->h = 1;
            LoadImage(rect, (u32 *)palAnim->workPal[*g_worldBufIndex]);
        }
    }
}

s32 wm_drawActorModel(EvtObject *act, SVECTOR *pos, s32 unused) { /* its caller passes this, unused */
    WmActorType *typeInfo;
    WmObjSlot *objSlot;
    ModelObj *obj;
    WmAnimHdr *h;
    SVECTOR r2;
    SVECTOR rot;
    s32 keysLo;
    void *t2;
    s32 groups;
    s32 i;

    typeInfo = &g_worldActorTypeInfo[act->actorType];
    objSlot = (WmObjSlot *)game_getModel(act->modelSlot);
    obj = objSlot->model;
    memset(&rot, 0, 8);
    rot.vx = objSlot->actor->pos.rotX;
    rot.vy = objSlot->actor->pos.rotY;
    rot.vz = objSlot->actor->rotZ;
    r2 = rot;
    math_rotMatrixYXZ(&r2, &obj->worldMatrix);
    obj->worldMatrix.t[0] = pos->vx;
    obj->worldMatrix.t[1] = pos->vy;
    obj->worldMatrix.t[2] = pos->vz;
    wm_applyScale(obj);
    obj->anim = h = db_lookupRegistered(DB_GET_ADDR, objSlot->actor->anim | DB_ANIM, 0);
    if (h != NULL) {
        if (IS_OFFSET(h->rotKeys)) {
            keysLo = h->rotKeysLo;
            h->rotKeys = (s32)h + h->rotKeys;
            if (keysLo != 0) {
                h->rotKeysLo = (s32)obj->anim + keysLo;
            }
        }
        wm_calcBoneMatrices(obj, objSlot->actor->animFrame);
        if (act->actorType != WM_ACTOR_SHADOW_ONLY) {
            groups = obj->model->groupCount;
            if (typeInfo->terrainClass != 0 && typeInfo->rideOffset > 0) {
                obj->clipY = pos->vy - typeInfo->rideOffset + 100;
                if (obj->parent != NULL) {
                    obj->clipY = obj->parent->clipY;
                }
                for (i = 0; i < groups; i++) {
                    wm_projectMeshClipY(obj, i);
                    wm_drawMeshClipY(obj, i);
                }
            } else {
                for (i = 0; i < groups; i++) {
                    wm_projectMesh(obj, i);
                    wm_drawMesh(obj, i);
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
    WmActorType *typeInfo;
    s32 k;
    s32 lift;
    s32 sx;
    s32 sz;
    s32 fade;
    s32 alpha;
    s32 size;
    u32 *ot;

    typeInfo = &g_worldActorTypeInfo[act->actorType];
    k = act->modelSlot;
    sx = 0x1000;
    if (act->actorType != WM_ACTOR_SHADOW_ONLY) {
        lift = typeInfo->floorY - act->pos.vy;
        pos->vy += lift;
        pos->vy -= lift / 8;
        sz = sx - lift / 2;
        sx -= lift / 4;
        g_worldOtBias = -typeInfo->shadowWidth * 4 - (s8)typeInfo->shadowOtBias;
        if (typeInfo->shadowWidth == 0) {
            return;
        }
        sx = (typeInfo->shadowWidth * sx) >> 11;
        sz = (typeInfo->shadowDepth * sz) >> 12;
        fade = (WM_SHADOW_FADE_DEPTH - depth) >> 3;
        fade *= g_worldWeatherData.work[0].shadowShade;
        fade >>= 12;
        if (fade < 0) {
            fade = 0;
        }
        if (fade >= 0x1000) {
            fade = 0xFFF;
        }
        sz *= fade;
        sz >>= 12;
        if (sz >= 0x100) {
            sz = 0xFF;
        }
        if (sz < 0) {
            sz = 0;
        }
        g_gameState->gameData->ctx.world->obj->shadows[k].scaleX = sx * WM_SHADOW_SCALE_X_MUL;
        g_gameState->gameData->ctx.world->obj->shadows[k].scaleZ = sx * WM_SHADOW_SCALE_Z_MUL;
        g_gameState->gameData->ctx.world->obj->shadows[k].shade = sz;
        ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf];
        g_gameState->gameData->ot[g_gameState->gameData->drawBuf] = ot + g_worldOtBias;
        wm_drawShadow(g_gameState->gameData->ctx.world->obj->shadows, k, pos->vy, typeInfo->shadowJoint);
        g_gameState->gameData->ot[g_gameState->gameData->drawBuf] = ot;
    } else {
        size = WM_SHADOW_PLAIN_SIZE;
        if (g_worldFog != 0) {
            size = WM_SHADOW_PLAIN_SIZE_FOG;
        }
        fade = (WM_SHADOW_PLAIN_FADE_DEPTH - depth) >> 2;
        fade *= g_worldWeatherData.work[4].shadowShade;
        fade >>= 12;
        if (fade < 0) {
            fade = 0;
        }
        if (fade >= 0x1000) {
            fade = 0xFFF;
        }
        alpha = (typeInfo->shadowDepth * size) >> 12;
        alpha *= fade;
        alpha >>= 12;
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
        if (alpha < 0) {
            alpha = 0;
        }
        g_gameState->gameData->ctx.world->obj->shadows[k].scaleX = WM_SHADOW_PLAIN_SCALE_X;
        g_gameState->gameData->ctx.world->obj->shadows[k].scaleZ = WM_SHADOW_PLAIN_SCALE_Z;
        g_gameState->gameData->ctx.world->obj->shadows[k].shade = alpha;
        ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf];
        g_worldOtBias = -WM_SHADOW_PLAIN_OT_BIAS;
        g_gameState->gameData->ot[g_gameState->gameData->drawBuf] -= WM_SHADOW_PLAIN_OT_BIAS;
        wm_drawShadow(g_gameState->gameData->ctx.world->obj->shadows, k, pos->vy, typeInfo->shadowJoint);
        g_gameState->gameData->ot[g_gameState->gameData->drawBuf] = ot;
    }
}

void wm_updateActors(void) {
    s32 polyIdx;
    s32 h;
    s32 cell;
    EvtListNode *node;
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
    for (node = evt_getObjectList(); node != NULL; node = node->next) {
        act = node->obj;
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
            g_worldMountActor->rotZ += WM_ROLL_SETTLE_STEP;
        }
        if (g_worldMountActor->rotZ > 0) {
            g_worldMountActor->rotZ -= WM_ROLL_SETTLE_STEP;
        }
        if (g_worldMountActor->rotZ > -WM_ROLL_SETTLE_STEP && g_worldMountActor->rotZ < WM_ROLL_SETTLE_STEP) {
            g_worldMountActor->rotZ = 0;
        }
    }
    if (g_worldVehicleActor != NULL) {
        if (g_worldMoveModeInfo->controlType != WM_CTRL_FLY) {
            if (g_worldVehicleActor->rotZ < WM_VEHICLE_REST_ROLL) {
                g_worldVehicleActor->rotZ += WM_ROLL_SETTLE_STEP;
            }
            if (g_worldVehicleActor->rotZ >= WM_VEHICLE_REST_ROLL + 1) {
                g_worldVehicleActor->rotZ -= WM_ROLL_SETTLE_STEP;
            }
            if (g_worldVehicleActor->rotZ >= WM_VEHICLE_REST_ROLL - WM_ROLL_SETTLE_STEP + 1 &&
                g_worldVehicleActor->rotZ < WM_VEHICLE_REST_ROLL + WM_ROLL_SETTLE_STEP) {
                g_worldVehicleActor->rotZ = WM_VEHICLE_REST_ROLL;
            }
        } else if (g_worldFlyTurning == 0) {
            if (g_worldVehicleActor->rotZ < WM_VEHICLE_REST_ROLL) {
                g_worldVehicleActor->rotZ += WM_VEHICLE_FLY_SETTLE_STEP;
            }
            if (g_worldVehicleActor->rotZ >= WM_VEHICLE_REST_ROLL + 1) {
                g_worldVehicleActor->rotZ -= WM_VEHICLE_FLY_SETTLE_STEP;
            }
            if (g_worldVehicleActor->rotZ >= WM_VEHICLE_REST_ROLL - WM_VEHICLE_FLY_SETTLE_STEP + 1 &&
                g_worldVehicleActor->rotZ < WM_VEHICLE_REST_ROLL + WM_VEHICLE_FLY_SETTLE_STEP) {
                g_worldVehicleActor->rotZ = WM_VEHICLE_REST_ROLL;
            }
        }
    }
    if (evt_getPlayer() != NULL) {
        if (g_worldPlayerTurn == 0 || g_worldPlayerSpeed < WM_ROLL_SETTLE_SPEED) {
            if (g_worldCameraRoll > 0) {
                g_worldCameraRoll -= WM_ROLL_SETTLE_STEP;
                if (g_worldCameraRoll < 0) {
                    g_worldCameraRoll = 0;
                }
            }
            if (g_worldCameraRoll < 0) {
                g_worldCameraRoll += WM_ROLL_SETTLE_STEP;
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
    WmActorType *typeInfo;
    s32 y;
    s16 lift;

    if (g_worldMapLoaded != 0) {
        typeInfo = &g_worldActorTypeInfo[act->actorType];
        if (typeInfo->polyCache != 0xA) {
            h = wm_getTerrainHeightOffset(typeInfo->terrainClass, typeInfo->cellAttr, &flat);
            if (typeInfo->flying == 0) {
                if (typeInfo->terrainClass != 0) {
                    if (typeInfo->rideMode == WM_MOVE_BOAT) {
                        typeInfo->rideOffset = 0x118;
                    } else if (flat != 0 || h == 0) {
                        typeInfo->rideOffset = h;
                    } else {
                        if (typeInfo->rideOffset < h - 0x15) {
                            typeInfo->rideOffset += 0x14;
                        }
                        if (typeInfo->rideOffset > h + 0x15) {
                            typeInfo->rideOffset -= 0x14;
                        }
                    }
                }
                act->pos.vy = typeInfo->floorY + typeInfo->rideOffset;
            } else {
                y = typeInfo->floorY + h;
                if (y < act->pos.vy) {
                    act->pos.vy = y;
                }
                if (g_worldCameraHold == 0 && g_worldPlayer->pos.vy < -0x2A30 && g_worldCameraLookupFix == 0) {
                    g_worldPlayer->pos.vy = -0x2A30;
                }
                lift = act->pos.vy - typeInfo->floorY;
                typeInfo->rideOffset = lift;
                if (lift < 0) {
                    typeInfo->rideOffset = 0;
                }
            }
        }
    }
}

void wm_movePlayer(WmActorType *cc) {
    ActorPos probe;
    ActorPos stepPos;
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
    s32 climb;
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
            spd *= WM_FAST_WALK_MUL;
        }
        i = 0;
        do {
            j = i;
            cell = -1;
            h = -1;
            probe = g_worldPlayer->pos;
            if (wm_tryStep((VECTOR *)&probe, i, g_worldMoveMode, spd, &cell, &h) ||
                wm_tryStep((VECTOR *)&probe, j = -i, g_worldMoveMode, spd, &cell, &h)) {
                dx = probe.vx - g_worldPlayer->prevPos.vx;
                dy = probe.vy - g_worldPlayer->prevPos.vy;
                dz = probe.vz - g_worldPlayer->prevPos.vz;
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
                stepPos = g_worldPlayer->pos;
                if (wm_tryStep((VECTOR *)&stepPos, j, g_worldMoveMode, k, &cell, &h)) {
                    g_worldPlayer->pos = stepPos;
                    cc->cellAttr = cell;
                    cc->floorY = stepPos.vy;
                    g_worldPlayerFloorPoly = h;
                    break;
                }
            }
            i += WM_STEP_TRY_ANGLE;
        } while (i < WM_STEP_TRY_MAX);
        if (g_worldFastWalk != 0) {
            g_worldMoveMode = saved;
        }
    } else {
        stepPos = g_worldPlayer->pos;
        if (wm_tryStep((VECTOR *)&stepPos, 0, g_worldMoveMode, spd, &cell2, &h2)) {
            g_worldPlayer->pos.vx = stepPos.vx;
            g_worldPlayer->pos.vz = stepPos.vz;
            g_worldCeilingGap = g_worldPlayer->pos.vy - WM_FLY_CEILING;
            if (g_worldCeilingGap < WM_FLY_CEILING_SOFT && g_worldCameraHold == 0 && g_worldCameraLookupFix == 0 && g_worldPlayerClimbSpeed < 0) {
                climb = (g_worldPlayerClimbSpeed << 1) * g_worldCeilingGap;
                climb >>= 12;
                g_worldPlayer->pos.vy += climb;
            } else {
                g_worldPlayer->pos.vy += g_worldPlayerClimbSpeed;
            }
            cc->cellAttr = cell2;
            cc->floorY = stepPos.vy;
            g_worldPlayerFloorPoly = h2;
        }
        off = WM_IIFA_CROWN_Y_DISC4;
        if (g_worldDataSet == WM_DATASET_DISC1) {
            off = WM_IIFA_CROWN_Y_DISC1;
            lim = WM_IIFA_CROWN_NOCACHE_DISC1;
        } else {
            lim = WM_IIFA_CROWN_NOCACHE_DISC4;
        }
        /* ez holds the x offset here; the names follow the original register use */
        ez = g_worldPlayer->pos.vx - WM_IIFA_X;
        y = g_worldPlayer->pos.vy;
        wz = g_worldPlayer->pos.vz - WM_IIFA_Z;
        dy = y + off;
        if (ABS(ez) < WM_IIFA_CROWN_R && ABS(wz) < WM_IIFA_CROWN_R) {
            dx = WM_IIFA_CROWN_R * WM_IIFA_CROWN_R - ez * ez;
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
    ex = g_worldPlayer->pos.vx - WM_NOCACHE_RIVER_GORGE_X;
    ez = g_worldPlayer->pos.vz - WM_NOCACHE_RIVER_GORGE_Z;
    if (ABS(ex) < WM_NOCACHE_RIVER_GORGE_R && ABS(ez) < WM_NOCACHE_RIVER_GORGE_R) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - WM_NOCACHE_CONDE_PETIE_X;
    ez = g_worldPlayer->pos.vz - WM_NOCACHE_CONDE_PETIE_Z;
    if (ABS(ex) < WM_NOCACHE_CONDE_PETIE_R && ABS(ez) < WM_NOCACHE_CONDE_PETIE_R) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - WM_NOCACHE_ICE_CAVERN_X;
    ez = g_worldPlayer->pos.vz - WM_NOCACHE_ICE_CAVERN_Z;
    if (ABS(ex) < WM_NOCACHE_ICE_CAVERN_R && ABS(ez) < WM_NOCACHE_ICE_CAVERN_R) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - WM_NOCACHE_ICE_CAVERN_2_X;
    ez = g_worldPlayer->pos.vz - WM_NOCACHE_ICE_CAVERN_2_Z;
    if (ABS(ex) < WM_NOCACHE_ICE_CAVERN_2_R && ABS(ez) < WM_NOCACHE_ICE_CAVERN_2_R) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - WM_NOCACHE_CHOCOBOS_LAGOON_X;
    ez = g_worldPlayer->pos.vz - WM_NOCACHE_CHOCOBOS_LAGOON_Z;
    if (ABS(ex) < WM_NOCACHE_CHOCOBOS_LAGOON_R && ABS(ez) < WM_NOCACHE_CHOCOBOS_LAGOON_R) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - WM_NOCACHE_CONDE_PETIE_PATH_X;
    ez = g_worldPlayer->pos.vz - WM_NOCACHE_CONDE_PETIE_PATH_Z;
    if (ABS(ex) < WM_NOCACHE_CONDE_PETIE_PATH_RX && ABS(ez) < WM_NOCACHE_CONDE_PETIE_PATH_RZ) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - WM_NOCACHE_GIZAMALUKE_X;
    ez = g_worldPlayer->pos.vz - WM_NOCACHE_GIZAMALUKE_Z;
    if (ABS(ex) < WM_NOCACHE_GIZAMALUKE_R && ABS(ez) < WM_NOCACHE_GIZAMALUKE_R) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - WM_NOCACHE_DESERT_PALACE_X;
    ez = g_worldPlayer->pos.vz - WM_NOCACHE_DESERT_PALACE_Z;
    if (ABS(ex) < WM_NOCACHE_DESERT_PALACE_R && ABS(ez) < WM_NOCACHE_DESERT_PALACE_R) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - WM_NOCACHE_IPSENS_CASTLE_X;
    ez = g_worldPlayer->pos.vz - WM_NOCACHE_IPSENS_CASTLE_Z;
    if (ABS(ex) < WM_NOCACHE_IPSENS_CASTLE_R && ABS(ez) < WM_NOCACHE_IPSENS_CASTLE_R) {
        g_worldUsePolyCache = 0;
    }
    ex = g_worldPlayer->pos.vx - WM_NOCACHE_DAGUERREO_X;
    ez = g_worldPlayer->pos.vz - WM_NOCACHE_DAGUERREO_Z;
    if (ABS(ex) < WM_NOCACHE_DAGUERREO_R && ABS(ez) < WM_NOCACHE_DAGUERREO_R) {
        g_worldUsePolyCache = 0;
    }
    wm_wrapPos(&g_worldPlayer->pos.vx, &g_worldPlayer->pos.vz);
}

u8 wm_tryStep(VECTOR *pos, s32 angle, s32 moveMode, s32 speed, u32 *outCell, s32 *outHeight) {
    VECTOR newPos;
    u32 cell;
    s32 h;
    WmMoveMode *mode;
    WmPolyCache *cache;
    u8 ok;
    s32 k;
    s32 ang;
    u8 toWater;
    u8 toLand;
    u8 x;
    u8 y;

    k = g_worldActorTypeInfo[g_worldPlayer->actorType].polyCache;
    mode = &g_worldMoveModes[moveMode];
    newPos = *pos;
    cache = &g_worldPolyCache[k];
    newPos.vx += math_sin(g_worldPlayerHeading + (ang = angle + 0x800)) * speed >> 12;
    newPos.vz += math_cos(g_worldPlayerHeading + ang) * speed >> 12;
    if ((u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2 == WM_TOPO_NO_ENCOUNTER && g_worldMoveModeInfo->controlType != WM_CTRL_FLY) {
        wm_getGroundHeight(&newPos, &cell, &h, NULL, (u16 *)&newPos.vy);
    } else {
        wm_getGroundHeight(&newPos, &cell, &h, cache, (u16 *)&newPos.vy);
    }
    if (h >= 0) {
        ok = wm_testCellType(mode->allowedCells, cell);
        if (ok) {
            if (mode->checkTransitions != 0) {
                toWater = wm_testCellType(g_worldWaterCellTypes, cell);
                toLand = wm_testCellType(g_worldLandCellTypes, cell);
                x = wm_testCellType(g_worldWaterCellTypes, g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr);
                y = wm_testCellType(g_worldLandCellTypes, g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr);
                if (toLand & x) {
                    ok = 0;
                }
                if (y & toWater) {
                    ok = 0;
                }
            }
            if (ok) {
                if (pos != NULL) {
                    *pos = newPos;
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
    s32 dirs[2];
    ActorPos tryPos;
    u32 cell;
    u8 ok;
    u8 allOk;
    s32 angOff;
    s32 i;
    s32 j;
    s32 topo;
    s32 dist;
    s32 k;
    s32 x;
    s32 z;

    memset(&tryPos, 0, sizeof(tryPos));
    ok = 0;
    if (g_worldAmbientSeDist[WM_AMBIENT_WIND_SHRINE] < WM_AMBIENT_BLOCK_RADIUS && ((u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2) == WM_TOPO_GUSTY_VALLEY) {
        return 0;
    }
    g_worldFlyTurning = 0;
    act = g_worldPlayer;
    g_worldGetoffSearching = 1;
    tryPos = act->pos;
    switch (g_worldMoveMode) {
    case WM_MOVE_HILDA_GARDE:
    case WM_MOVE_INVINCIBLE:
        x = (g_worldPlayer->pos.vx >> 3) - (WM_IPSEN_CASTLE_X >> 3);
        z = (g_worldPlayer->pos.vz >> 3) - (WM_IPSEN_CASTLE_Z >> 3);
        if (SquareRoot0(x * x + z * z) < (WM_IPSEN_CASTLE_R >> 3)) {
            return 0;
        }
        if (!wm_tryStep((VECTOR *)&tryPos, 0, 0, 0, &cell, 0)) {
            return 0;
        }
        topo = (u8)cell >> 2;
        if (topo >= WM_TOPO_NO_LANDING && (topo <= WM_TOPO_NO_LANDING_2 || topo == WM_TOPO_NO_ENCOUNTER)) {
            return 0;
        }
        break;
    case WM_MOVE_BOAT:
        if (!wm_tryStep((VECTOR *)&tryPos, 0, 10, 0, &cell, 0)) {
            return 0;
        }
        if (((u8)cell >> 2) != WM_TOPO_SHALLOWS) {
            return 0;
        }
        break;
    default:
        if (!wm_tryStep((VECTOR *)&tryPos, 0, 0, 0, &cell, 0)) {
            return 0;
        }
        if (((u8)cell >> 2) == WM_TOPO_NO_ENCOUNTER) {
            return 0;
        }
        break;
    }
    g_worldGetoffSearching = 0;
    for (angOff = 0; angOff < 0x800; angOff += 0x100) {
        dirs[0] = (angOff + 0xC00) % 0x1000;
        dirs[1] = -angOff + 0xC00;
        for (i = 0; i < 2; i++) {
            allOk = 1;
            if (ok) {
                continue;
            }
            for (j = 1; j < 9; j++) {
                dist = g_worldMoveModeInfo->getoffDist * j / 8;
                tryPos = g_worldPlayer->pos;
                allOk &= wm_tryStep((VECTOR *)&tryPos, dirs[i], 0, dist, &cell, 0);
            }
            ok = allOk;
            if (ok) {
                switch (g_worldMoveMode) {
                case WM_MOVE_WALK:
                    if (abs(tryPos.vy - g_worldPlayer->pos.vy) >= 0x1F5) {
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
                    if (abs(tryPos.vy - w_frameGetParameter(WM_PARAM_RIDE_Y)) >= 0x15F) {
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
            case WM_TOPO_WATER:
                k = 2;
                break;
            case WM_TOPO_WATER_2:
                k = 3;
                break;
            case WM_TOPO_SEA_2:
                k = 4;
                break;
            case WM_TOPO_SEA:
                k = 5;
                break;
            case WM_TOPO_RIVER:
                k = 6;
                break;
            case WM_TOPO_WATERFALL:
                k = 7;
                break;
            default:
                k = 8;
                break;
            }
            tryPos.vy += g_worldTerrainHeightOffsets[1][k];
            *out = tryPos;
            return 1;
        }
    }
    return 0;
}

void w_movementChange(void) {
    s8 old;
    WmMoveMode *info;
    WmMoveMode *prev;

    evt_resetIdleTimer(0);
    g_worldSpeedSmooth = 0;
    g_worldClimbSmooth = 0;
    wm_autoPilotOff();
    old = g_worldMoveMode;
    prev = g_worldMoveModeInfo;
    g_worldMoveMode = g_evtGlobalVars.b[0xBE];
    g_worldMoveModeInfo = info = &g_worldMoveModes[g_worldMoveMode];
    if (old == WM_MOVE_BOAT) {
        wm_seStop(prev->loopSe);
    }
    if (info->loopSe != 0) {
        wm_sePlay(info->loopSe, 0);
        if (g_worldMoveMode != WM_MOVE_BOAT) {
            wm_seFadeVolume(info->loopSe, WM_SE_FADE_LOOP_START, WM_SE_LOOP_VOL);
        }
        if ((u8)(g_worldMoveMode - WM_MOVE_HILDA_GARDE) < 2) {
            wm_sePlay(WM_SE_AIRSHIP_WATER, 0);
        }
        g_worldCamera->viewForceNormal = 1;
    }
    if (prev == NULL || prev->music != info->music) {
        w_musicRequestSend(info->music);
    }
    if (g_worldMoveMode == WM_MOVE_WALK && (u8)(old - WM_MOVE_CHOCOBO_FIELD) < 6) {
        g_worldPlayerHeading = (g_worldMountActor->pos.rotY + 0x400) & 0xFFF;
    }
    if ((u8)(g_worldMoveMode - WM_MOVE_CHOCOBO_FIELD) < 6 && old == WM_MOVE_WALK) {
        g_worldPlayerHeading = g_worldMountActor->pos.rotY & 0xFFF;
    }
    if (g_worldMoveModes[old].airborne == g_worldMoveModes[g_worldMoveMode].airborne) {
        wm_blendCameraPreset(info->cameraRow, 0x200);
    } else {
        wm_blendCameraPreset(info->cameraRow, 0x40);
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
    s32 turn;
    s16 cur;
    u32 k;
    s32 rx;
    s32 lo;
    s32 rawAng;

    wm_getLeftStickX(&x);
    wm_getLeftStickY(&z);
    z = -z;
    rawAng = -ratan2(z, x) + 0x400;
    g_worldCameraTurning = 0;
    ang = rawAng & 0xFFF;
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
        g_worldCamera->yaw -= WM_CAM_TURN_SPEED_WALK;
    }
    if (wm_getPadHeld() & BTN_R1) {
        g_worldCameraTurning = 1;
        g_worldCameraTurnedByHand = 1;
        g_worldCamera->yaw += WM_CAM_TURN_SPEED_WALK;
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
    turn = g_worldPlayerHeading;
    cur = g_worldPlayer->pos.rotY;
    if (turn - cur > 0x800) {
        turn -= 0x1000;
    }
    if (turn - cur < -0x800) {
        turn += 0x1000;
    }
    turn -= cur;
    if ((x | z) != 0) {
        g_worldPlayer->pos.rotY = (g_worldPlayer->pos.rotY + turn / WM_WALK_TURN_DIV) & 0xFFF;
    }
    if (g_worldPlayerPosChanged != 0 && mv != 0) {
        if (wm_testCellType_inline(g_worldWaterCellTypes, (u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2)) {
            wm_spawnEffect(g_worldPlayer->pos.vx, WM_FX_WATER_Y, g_worldPlayer->pos.vz, WM_FX_WATER_RIPPLE, WM_FX_SCALE_WATER_RIPPLE);
        }
        if (!(g_worldFrameCount & 3)) {
            k = (u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2;
            if (k == 0x1E || k == WM_TOPO_BEACH || k == 0x23) {
                wm_spawnEffect(g_worldPlayer->pos.vx, g_worldPlayer->pos.vy, g_worldPlayer->pos.vz, WM_FX_BEACH_SPLASH, WM_FX_SCALE_BEACH_SPLASH);
            }
            if (((u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2) == WM_TOPO_SAND) {
                wm_spawnEffect(g_worldPlayer->pos.vx, g_worldPlayer->pos.vy, g_worldPlayer->pos.vz, WM_FX_SAND_DUST, WM_FX_SCALE_SAND_DUST);
            }
        }
        if (!(g_worldFrameCount & 1) && (u32)(((u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2) - WM_TOPO_FOREST) < 3) {
            rx = g_worldPlayer->prevPos.vx + rand8_inline() - WM_FX_SCATTER;
            lo = rand8_inline();
            wm_spawnEffect(rx, g_worldPlayer->prevPos.vy, g_worldPlayer->prevPos.vz + lo - WM_FX_SCATTER, WM_FX_FOREST_LEAVES, WM_FX_SCALE_FOREST_LEAVES);
            rx = g_worldPlayer->prevPos.vx + rand8_inline() - WM_FX_SCATTER;
            lo = rand8_inline();
            wm_spawnEffect(rx, g_worldPlayer->prevPos.vy, g_worldPlayer->prevPos.vz + lo - WM_FX_SCATTER, WM_FX_FOREST_LEAVES, WM_FX_SCALE_FOREST_LEAVES);
        }
    }
}

void wm_controlSteer(void) {
    s32 lx;
    s32 rx;
    s32 ly;
    s16 smoothSpeed;
    s32 sp;
    s32 vol;
    s32 turn;
    s32 ang;
    WmMoveMode *info;
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
    turn = g_worldMoveModeInfo->turnSpeed * rx >> 7;
    g_worldCameraTurnedByHand = 0;
    g_worldPlayerTurn = turn;
    if (wm_getPadHeld() & BTN_L1) {
        g_worldCameraTurnedByHand = 1;
        g_worldCamera->yaw -= WM_CAM_TURN_SPEED_WALK;
    }
    if (wm_getPadHeld() & BTN_R1) {
        g_worldCameraTurnedByHand = 1;
        g_worldCamera->yaw += WM_CAM_TURN_SPEED_WALK;
    }
    g_worldCamera->yawOffset = 0;
    g_worldCamera->yaw &= 0xFFF;
    info = g_worldMoveModeInfo;
    smoothSpeed = g_worldSpeedSmooth + (info->speed * ly / g_worldStickDiv - g_worldSpeedSmooth) / g_worldSpeedSmoothDiv;
    g_worldSpeedSmooth = smoothSpeed;
    sp = smoothSpeed / g_worldSpeedScaleDiv;
    g_worldPlayerSpeed = sp;
    vol = ABS(sp) / WM_STEER_SE_VOL_DIV + WM_STEER_SE_VOL_MIN;
    if (vol >= 0x80) {
        vol = 0x7F;
    }
    wm_seFadeVolume(info->loopSe, WM_STEER_SE_FADE, vol);
    g_worldPlayerHeading = g_worldPlayer->pos.rotY = (g_worldPlayer->pos.rotY + g_worldPlayerTurn) & 0xFFF;
    if (ABS(g_worldPlayerSpeed) > WM_STEER_WAKE_SPEED && g_worldPlayerPosChanged != 0) {
        ang = (g_worldPlayerSpeed <= 0) << 11;
        dx = math_sin(ang + g_worldPlayer->pos.rotY) >> 2;
        dz = math_cos(ang + g_worldPlayer->pos.rotY) >> 2;
        wm_spawnEffect(g_worldPlayer->pos.vx + dx, WM_FX_WATER_Y, g_worldPlayer->pos.vz + dz, WM_FX_BOAT_WAKE, WM_FX_SCALE_BOAT_WAKE);
        if (g_worldFrameCount & 1) {
            dx = math_sin(ang + g_worldPlayer->pos.rotY + 0x80) >> 3;
            dz = math_cos(ang + g_worldPlayer->pos.rotY + 0x80) >> 3;
            wm_spawnEffect(g_worldPlayer->pos.vx + dx, WM_FX_WATER_Y, g_worldPlayer->pos.vz + dz, WM_FX_BOAT_SIDE_WAKE, WM_FX_SCALE_BOAT_WAKE);
            dx = math_sin(ang + g_worldPlayer->pos.rotY - 0x80) >> 3;
            dz = math_cos(ang + g_worldPlayer->pos.rotY - 0x80) >> 3;
            wm_spawnEffect(g_worldPlayer->pos.vx + dx, WM_FX_WATER_Y, g_worldPlayer->pos.vz + dz, WM_FX_BOAT_SIDE_WAKE, WM_FX_SCALE_BOAT_WAKE);
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
    s32 unused;
    s32 k;
    s32 dist;
    s32 h;
    u32 topo;
    u8 onWater;
    s32 smoothDiv;
    u32 w;
    s32 speedStep;
    s32 climbStep;
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
            g_worldCamera->yaw += WM_CAM_TURN_SPEED_FLY;
        }
        if (btn & BTN_L1) {
            g_worldCameraTurnedByHand = 1;
            g_worldCamera->yaw -= WM_CAM_TURN_SPEED_FLY;
        }
    }
    g_worldCamera->yawOffset = 0;
    speedStep = g_worldMoveModeInfo->speed * z / g_worldStickDiv;
    g_worldPlayerTurn = g_worldMoveModeInfo->turnSpeed * x >> 7;
    smoothDiv = g_worldSpeedSmoothDiv;
    climbStep = g_worldMoveModeInfo->climbSpeed * y / g_worldStickDiv;
    speedStep = (speedStep - g_worldSpeedSmooth) / smoothDiv;
    climbStep = (climbStep - g_worldClimbSmooth) / smoothDiv;
    sa = g_worldSpeedSmooth + speedStep;
    sb = g_worldClimbSmooth + climbStep;
    g_worldSpeedSmooth = sa;
    g_worldClimbSmooth = sb;
    g_worldPlayerSpeed = sa / g_worldSpeedScaleDiv;
    g_worldPlayerClimbSpeed = sb / g_worldSpeedScaleDiv;
    if (g_worldFastFly != 0) {
        g_worldPlayerTurn *= 2;
        g_worldPlayerSpeed *= 2;
    }
    if (g_worldMoveMode == WM_MOVE_INVINCIBLE) {
        g_worldPlayerTurn = g_worldPlayerTurn * (WM_FLY_TURN_INVINCIBLE - g_worldPlayerSpeed / 2) >> 8;
    }
    if (g_worldMoveMode == WM_MOVE_HILDA_GARDE) {
        g_worldPlayerTurn = g_worldPlayerTurn * (WM_FLY_TURN_HILDA_GARDE - g_worldPlayerSpeed / 2) >> 8;
    }
    if (x != 0 || z < 0) {
        wm_autoPilotOff();
    }
    switch (g_worldAutoPilot) {
    case 1:
        ex = (g_worldNaviPlaces[g_worldNaviPage][g_worldAutoPilotDest].worldX - g_worldPlayer->pos.vx) >> 5;
        ez = (g_worldNaviPlaces[g_worldNaviPage][g_worldAutoPilotDest].worldZ - g_worldPlayer->pos.vz) >> 5;
        dist = SquareRoot0(ex * ex + ez * ez);
        g_worldPlayerSpeed = dist;
        if (g_worldPlayerSpeed > WM_AUTOPILOT_SPEED_MAX) {
            g_worldPlayerSpeed = WM_AUTOPILOT_SPEED_MAX;
        }
        if (g_worldAutoPilotDist < g_worldPlayerSpeed || g_worldPlayerSpeed < WM_AUTOPILOT_ARRIVE) {
            wm_autoPilotOff();
        }
        g_worldAutoPilotDist = g_worldPlayerSpeed;
    case 2:
        dx = g_worldPlayer->pos.vx - g_worldNaviPlaces[g_worldNaviPage][g_worldAutoPilotDest].worldX;
        dz = g_worldPlayer->pos.vz - g_worldNaviPlaces[g_worldNaviPage][g_worldAutoPilotDest].worldZ;
        if (dx > WM_MAP_SIZE_X / 2) {
            dx -= WM_MAP_SIZE_X;
        }
        if (dx < -(WM_MAP_SIZE_X / 2)) {
            dx += WM_MAP_SIZE_X;
        }
        if (dz > WM_MAP_SIZE_Z / 2) {
            dz -= WM_MAP_SIZE_Z;
        }
        if (dz < -(WM_MAP_SIZE_Z / 2)) {
            dz += WM_MAP_SIZE_Z;
        }
        dz = (-ratan2(dz, dx) + 0x400) & 0xFFF;
        dz -= g_worldPlayer->pos.rotY;
        if (dz < -0x800) {
            dz += 0x1000;
        }
        if (dz > 0x800) {
            dz -= 0x1000;
        }
        if (g_worldAutoPilotDist > WM_AUTOPILOT_ARRIVE) {
            if (dz > WM_AUTOPILOT_TURN) {
                g_worldPlayerTurn = WM_AUTOPILOT_TURN;
            } else if (dz < -WM_AUTOPILOT_TURN) {
                g_worldPlayerTurn = -WM_AUTOPILOT_TURN;
            } else if (g_worldAutoPilot == 2) {
                g_worldAutoPilot = 1;
            }
        }
        break;
    }
    g_worldPlayerHeading = g_worldPlayer->pos.rotY = (g_worldPlayer->pos.rotY + g_worldPlayerTurn) & 0xFFF;
    if (g_worldPlayerTurn > 0 && -ox < g_worldCameraRoll) {
        g_worldCameraRoll -= ox / WM_FLY_ROLL_DIV;
    }
    if (g_worldPlayerTurn < 0 && g_worldCameraRoll < -ox) {
        g_worldCameraRoll -= ox / WM_FLY_ROLL_DIV;
    }
    if (g_worldPlayerSpeed == 0) {
        g_worldCameraRoll += ox / WM_FLY_ROLL_DIV;
    }
    if ((s8)g_worldMoveModeInfo->maxRoll < g_worldCameraRoll) {
        g_worldCameraRoll = (s8)g_worldMoveModeInfo->maxRoll;
    }
    if (g_worldCameraRoll < -(s8)g_worldMoveModeInfo->maxRoll) {
        g_worldCameraRoll = -(s8)g_worldMoveModeInfo->maxRoll;
    }
    if (g_worldPlayerTurn > 0 && -(ox * 2) < g_worldPlayer->rotZ) {
        g_worldPlayer->rotZ -= ox / WM_FLY_BANK_DIV;
    }
    if (g_worldPlayerTurn < 0 && g_worldPlayer->rotZ < -(ox * 2)) {
        g_worldPlayer->rotZ -= ox / WM_FLY_BANK_DIV;
    }
    if (g_worldPlayer->rotZ > (s8)g_worldMoveModeInfo->maxRoll * 2) {
        g_worldPlayer->rotZ = (s8)g_worldMoveModeInfo->maxRoll * 2;
    }
    if (g_worldPlayer->rotZ < -(s8)g_worldMoveModeInfo->maxRoll * 2) {
        g_worldPlayer->rotZ = -(s8)g_worldMoveModeInfo->maxRoll * 2;
    }
    if (g_worldFrameCount & 1) {
        topo = (u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2;
        onWater = wm_testCellType_inline(g_worldWaterCellTypes, topo);
        switch (g_worldPlayer->actorType) {
        case WM_ACTOR_AIRSHIP:
        case WM_ACTOR_AIRSHIP2:
            if (onWater && abs(g_worldPlayerSpeed) > WM_AIRSHIP_SPRAY_SPEED) {
                h = g_worldPlayer->pos.vy;
                if (h >= WM_AIRSHIP_SPRAY_ALT) {
                    g_worldSprayTarget = 0x1000 - abs(h + 400) * 2;
                    wm_spawnEffect(g_worldPlayer->pos.vx, WM_FX_SPRAY_Y, g_worldPlayer->pos.vz, WM_FX_AIRSHIP_SPRAY, WM_FX_SCALE_AIRSHIP_SPRAY);
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
                    wm_seFadeVolume(WM_SE_AIRSHIP_WATER, WM_SE_FADE_SPRAY_IN, (dx >> 9) & 0xFF);
                } else {
                    wm_seFadeVolume(WM_SE_AIRSHIP_WATER, WM_SE_FADE_SPRAY_OUT, 0);
                }
            } else {
                wm_seFadeVolume(WM_SE_AIRSHIP_WATER, WM_SE_FADE_SPRAY_OUT, 0);
            }
            if (topo == WM_TOPO_SAND && g_worldPlayerSpeed != 0 && g_worldPlayer->pos.vy >= WM_SAND_DUST_ALT) {
                g_worldSprayTarget = 0x1000 - (g_worldPlayer->pos.vy - 0x514) * 2;
                wm_spawnEffect(g_worldPlayer->pos.vx, g_worldPlayer->pos.vy + WM_LOW_FX_Y, g_worldPlayer->pos.vz, WM_FX_AIRSHIP_SAND, WM_FX_SCALE_AIRSHIP_SPRAY);
            }
            break;
        case WM_ACTOR_CHOCOBO4:
            if (onWater && abs(g_worldPlayerSpeed) > WM_CHOCOBO_SPRAY_SPEED && g_worldPlayer->pos.vy >= WM_CHOCOBO_SPRAY_ALT) {
                g_worldSprayTarget = 0x1000 - abs(g_worldPlayer->pos.vy - WM_CHOCOBO_SPRAY_ALT) * 2;
                wm_spawnEffect(g_worldPlayer->pos.vx, WM_FX_SPRAY_Y, g_worldPlayer->pos.vz, WM_FX_CHOCOBO_SPRAY, WM_FX_SCALE_CHOCOBO_SPRAY);
            }
            if (topo == WM_TOPO_SAND && g_worldPlayerSpeed != 0 && g_worldPlayer->pos.vy >= WM_SAND_DUST_ALT) {
                g_worldSprayTarget = 0x1000 - abs(g_worldPlayer->pos.vy - WM_SAND_DUST_ALT) * 2;
                wm_spawnEffect(g_worldPlayer->pos.vx, g_worldPlayer->pos.vy + WM_LOW_FX_Y, g_worldPlayer->pos.vz, WM_FX_CHOCOBO_SAND, WM_FX_SCALE_CHOCOBO_SAND);
            }
            break;
        }
    }
}

void wm_cameraAutoTurnWalk(void) {
    s32 camYaw;
    s32 cam;
    s32 yawDiff;
    s32 boost;
    s32 turnStep;

    camYaw = (g_worldCamera->yaw + g_worldCamera->yawOffset) & 0xFFF;
    cam = g_worldPlayerHeading & 0xFFF;
    if (camYaw - cam >= 0x800) {
        camYaw -= 0x1000;
    }
    if (camYaw - cam < -0x800) {
        camYaw += 0x1000;
    }
    yawDiff = (g_worldPlayerHeading - camYaw) * 0xC00 / 4096;
    g_worldAutoTurnDeadZone = abs(g_worldCamera->stickSide) >> 3;
    if (abs(yawDiff) < g_worldAutoTurnDeadZone) {
        yawDiff = 0;
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
        g_worldAutoTurnDiv = abs(yawDiff);
        if (abs(yawDiff) < g_worldAutoTurnDivMin) {
            g_worldAutoTurnDiv = g_worldAutoTurnDivMin;
        }
        if (g_worldAutoTurnDiv > g_worldAutoTurnDivMax) {
            g_worldAutoTurnDiv = g_worldAutoTurnDivMax;
        }
        boost = (((0x800 - abs(yawDiff)) * g_worldAutoTurnBoost >> 12) + 0x800) * (2 * g_worldCameraTurnGain);
        turnStep = (yawDiff * 256 / g_worldAutoTurnDiv) * (boost / 4096);
        g_worldCamera->yaw += turnStep / 4096;
    }
    g_worldCamera->yaw &= 0xFFF;
}

void wm_cameraAutoTurnVehicle(void) {
    s32 yaw; /* camera yaw, then the heading minus yaw difference (separate locals don't match) */
    s32 step;
    s32 turnStep;
    s32 cur;

    yaw = g_worldCamera->yaw;
    if (yaw - g_worldPlayer->pos.rotY > 0x800) {
        g_worldCamera->yaw = yaw - 0x1000;
    }
    if (g_worldCamera->yaw - g_worldPlayer->pos.rotY < -0x800) {
        g_worldCamera->yaw += 0x1000;
    }
    if (yaw < 0) {
        yaw += 0x1000;
    }
    yaw %= 4096;
    cur = g_worldPlayer->pos.rotY;
    if (yaw - cur > 0x800) {
        yaw -= 0x1000;
    }
    if (yaw - cur < -0x800) {
        yaw += 0x1000;
    }
    yaw = cur - yaw;
    yaw %= 4096;
    if (g_worldCameraTurnedByHand == 0 && g_worldCameraFlags->autoTurnOff == 0) {
        step = 16;
        if (g_worldCameraRecenter != 0) {
            step = ABS(yaw) * 256 >> 12;
            if (step == 0) {
                step = 1;
            }
        }
        turnStep = yaw % 4096 / step;
        if (turnStep != 0) {
            g_worldCamera->yaw += turnStep;
        } else if (step >= 2) {
            do {
                step /= 2;
                if (step == 0) {
                    break;
                }
                turnStep = yaw % 4096 / step;
            } while (turnStep == 0);
            g_worldCamera->yaw += turnStep;
        } else {
            g_worldCamera->yaw = g_worldPlayer->pos.rotY;
        }
        g_worldCamera->yaw &= 0xFFF;
        if (ABS(yaw) < 100) {
            g_worldCameraRecenter = 0;
        }
    }
}

void wm_snapActorsToFloor(void) {
    EvtListNode *node;
    EvtObject *obj;
    WmActorType *typeInfo;
    ActorPos pos;
    s32 polyIdx;
    u16 h;

    node = evt_getObjectList();
    if (node != NULL) {
        do {
            obj = node->obj;
            if (obj->kind == EVT_KIND_ACTOR) {
                pos = obj->pos;
                typeInfo = &g_worldActorTypeInfo[obj->actorType];
                wm_getGroundHeight((VECTOR *)&pos, &typeInfo->cellAttr, &polyIdx, NULL, &h);
                typeInfo->floorY = h;
                typeInfo->rideOffset = wm_getTerrainHeightOffset(typeInfo->terrainClass, typeInfo->cellAttr, 0);
                wm_updateActorHeight(obj);
                obj->prevPos = obj->pos;
            }
            node = node->next;
        } while (node != NULL);
    }
}

void w_movementChangeSP1(void) {
    s8 old;
    WmMoveMode *info;

    wm_autoPilotOff();
    g_worldSpeedSmooth = 0;
    g_worldClimbSmooth = 0;
    old = g_worldMoveMode;
    info = &g_worldMoveModes[0];
    wm_seFadeVolume(g_worldMoveModeInfo->loopSe, WM_SE_FADE_STOP, 0);
    wm_seFadeVolume(WM_SE_AIRSHIP_WATER, WM_SE_FADE_STOP, 0);
    w_musicRequestSend(info->music);
    wm_blendCameraPreset(info->cameraRow, 0x40);
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

/* Same logic as wm_testCellType_inline (ovl_0c.h) after WM_ATTR_TOPOGRAPHY. Written out because a call of the inline
   compiles differently. */
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
    s32 stick;
    s32 held;

    *outValue = 0;
    if (PAD_TYPE(g_padBuf[0].hdr) == PAD_ID_ANALOG) {
        stick = g_padAnalog[0].b[2] - 0x80;
        if (g_padAnalog[0].b[2] - 0x40U <= 0x80) {
            stick = 0;
        }
        if (stick > 0x40) {
            stick = (stick - 0x40) * 2;
        }
        if (stick < -0x40) {
            stick = (stick + 0x40) * 2;
        }
        if (stick != 0) {
            *outValue = stick;
            return 1;
        }
    }
    held = wm_getPadHeld();
    if (held & PAD_LEFT) {
        *outValue = -0x80;
    }
    if (held & PAD_RIGHT) {
        *outValue = 0x80;
    }
    return 0;
}

s32 wm_getLeftStickY(s32 *outValue) {
    s32 stick;

    *outValue = 0;
    if (PAD_TYPE(g_padBuf[0].hdr) == PAD_ID_ANALOG) {
        stick = g_padAnalog[0].b[3] - 0x80;
        if (g_padAnalog[0].b[3] - 0x40U <= 0x80) {
            stick = 0;
        }
        if (stick > 0x40) {
            stick = (stick - 0x40) * 2;
        }
        if (stick < -0x40) {
            stick = (stick + 0x40) * 2;
        }
        if (stick != 0) {
            *outValue = stick;
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
    s32 stick;

    *outValue = 0;
    if (PAD_TYPE(g_padBuf[0].hdr) == PAD_ID_ANALOG) {
        stick = 0x80 - g_padAnalog[0].b[1];
        if (stick + 0x40U <= 0x80) {
            stick = 0;
        }
        if (stick > 0x40) {
            stick = (stick - 0x40) * 2;
        }
        if (stick < -0x40) {
            stick = (stick + 0x40) * 2;
        }
        if (stick != 0) {
            *outValue = stick;
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
    switch ((s32)(cellAttr & WM_ATTR_TOPO_MASK) >> 2) {
    case WM_TOPO_FOREST:
    case WM_TOPO_FOREST_2:
    case WM_TOPO_FOREST_3:
        i = 0;
        break;
    case WM_TOPO_SHALLOWS:
        i = 1;
        break;
    case WM_TOPO_WATER:
        i = 2;
        break;
    case WM_TOPO_WATER_2:
        i = 3;
        break;
    case WM_TOPO_SEA_2:
        i = 4;
        break;
    case WM_TOPO_SEA:
        i = 5;
        break;
    case WM_TOPO_RIVER:
        i = 6;
        break;
    case WM_TOPO_WATERFALL:
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
