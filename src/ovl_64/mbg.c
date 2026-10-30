#include "ovl_64.h"

u8 *mbg_readFrame(Ovl64Dec *out) {
    RECT r;
    u8 *data;
    Ovl64RingHdr *hdr;
    Ovl64FrameInfo *info;
    Ovl64Pair *mask;
    MbgRumble *rumble;
    s32 tries;
    s32 ret;
    s8 hi;

    tries = 1000;
    info = NULL;
    mask = NULL;
    rumble = NULL;
    ret = mbg_streamGetFrame(&data, &hdr, &info, &mask, &rumble);
    while (ret != 0) {
        if (ret == 5) {
            g_mbgPlayer.ended = 1;
            return NULL;
        }
        if (--tries == 0) {
            return NULL;
        }
        ret = mbg_streamGetFrame(&data, &hdr, &info, &mask, &rumble);
    }
    if (info != NULL) {
        gte_SetGeomOffset((s16)info->ofsX + 0xA0, (s16)info->ofsY + 0x70);
        if (g_mbgPlayer.firstFrame != 0) {
            g_mbgGeomOffset.x = (s16)info->ofsX + 0xA0;
            g_mbgGeomOffset.y = (s16)info->ofsY + 0x70;
            g_mbgStartCamera.m00 = info->m00;
            g_mbgStartCamera.m01 = info->m01;
            g_mbgStartCamera.m02 = info->m02;
            g_mbgStartCamera.m10 = info->m10;
            g_mbgStartCamera.m11 = info->m11;
            g_mbgStartCamera.m12 = info->m12;
            g_mbgStartCamera.m20 = info->m20;
            g_mbgStartCamera.m21 = info->m21;
            g_mbgStartCamera.m22 = info->m22;
            g_mbgStartCamera.tx = info->tx;
            g_mbgStartCamera.ty = info->ty;
            g_mbgStartCamera.tz = info->tz;
            g_mbgStartCameraId = info->id;
            g_mbgPlayer.firstFrame = 0;
        } else {
            g_gameState->gameData->viewMatrix.m[0][0] = info->m00;
            g_gameState->gameData->viewMatrix.m[0][1] = info->m01;
            g_gameState->gameData->viewMatrix.m[0][2] = info->m02;
            g_gameState->gameData->viewMatrix.m[1][0] = info->m10;
            g_gameState->gameData->viewMatrix.m[1][1] = info->m11;
            g_gameState->gameData->viewMatrix.m[1][2] = info->m12;
            g_gameState->gameData->viewMatrix.m[2][0] = info->m20;
            g_gameState->gameData->viewMatrix.m[2][1] = info->m21;
            g_gameState->gameData->viewMatrix.m[2][2] = info->m22;
            g_gameState->gameData->viewMatrix.t[0] = info->tx;
            g_gameState->gameData->viewMatrix.t[1] = info->ty;
            g_gameState->gameData->viewMatrix.t[2] = info->tz;
            g_gameState->gameData->projDist = info->id;
        }
    }
    if (rumble != NULL && (!((Ovl64Cfg *)&g_gameState->config)->vibrationOff || g_gameState->mode == 5)) {
        g_padActuator[0].unk0 = rumble->on;
        g_padActuator[0].unk1 = rumble->level;
    }
    if (mask != NULL && mask->size != 0) {
        bcopy((void *)mask->data, &g_mbgMaskData[g_gameState->gameData->drawBuf ^ 1], mask->size);
        g_mbgMaskSize[g_gameState->gameData->drawBuf ^ 1] = mask->size;
    } else {
        g_mbgMaskSize[g_gameState->gameData->drawBuf ^ 1] = 0;
    }
    if (g_mbgFrame >= g_mbgMovie->lastFrame) {
        g_mbgPlayer.ended = 1;
    }
    if (g_mbgPlayer.width != hdr->width || g_mbgPlayer.height != hdr->height) {
        hi = g_mbgRgb24;
        if (hi) {
            r.x = 0;
            r.y = 0;
            r.w = 0x3C0;
            r.h = 0xE0;
        }
        g_mbgPlayer.width = hdr->width;
        g_mbgPlayer.height = hdr->height;
        if (hi) {
            out->frameRect[0].w = g_mbgPlayer.width * 3 / 2;
            out->frameRect[1].w = g_mbgPlayer.width * 3 / 2;
        } else {
            out->frameRect[0].w = g_mbgPlayer.width;
            out->frameRect[1].w = g_mbgPlayer.width;
        }
        out->frameRect[0].h = g_mbgPlayer.height;
        out->frameRect[1].h = g_mbgPlayer.height;
        out->slice.h = g_mbgPlayer.height;
    }
    return data;
}

void mbg_mdecOutCallback(void) {
    u8 res[8];
    s32 cur;

    memset(res, 0, 8);
    if (g_mbgStreamDeferred != 0) {
        mbg_streamReadyCallback(2, res);
        g_mbgStreamDeferred = 0;
    }
    if (g_mbgRgb24 == 0 && g_mbgMaskCursor.size != 0) {
        mbg_applyMask(&g_mbgMaskCursor, g_mbgDecode.mdecOut[g_mbgDecode.outIdx], g_mbgMaskBit);
    }
    cur = g_mbgDecode.outIdx;
    g_mbgLoadRects[g_mbgLoadRectIdx] = g_mbgDecode.slice;
    g_mbgDecode.outIdx ^= 1;
    g_mbgDecode.slice.x += g_mbgDecode.slice.w;
    if (g_mbgDecode.slice.x < g_mbgDecode.frameRect[g_gameState->gameData->drawBuf].x + g_mbgDecode.frameRect[g_gameState->gameData->drawBuf].w) {
        if (DecDCTinSync(1) == 0) {
            g_mbgDecode.frameDone = 1;
            g_mbgDecode.slice.x = g_mbgDecode.frameRect[g_gameState->gameData->drawBuf ^ 1].x;
            g_mbgDecode.slice.y = g_mbgDecode.frameRect[g_gameState->gameData->drawBuf ^ 1].y;
        }
        DecDCTout(g_mbgDecode.mdecOut[g_mbgDecode.outIdx], (g_mbgDecode.slice.w * g_mbgDecode.slice.h) >> 1);
    } else {
        if (DecDCTinSync(1) == 1) {
            DecDCTReset(1);
        }
        g_mbgDecode.frameDone = 1;
        if ((s8)g_mbgPlayer.modeSwitch == 1) {
            if (g_mbgRgb24 == 1 && g_gameState->gameData->drawBuf == 0) {
                g_mbgRgb24 = 0;
                g_mbgPlayer.rgb24 = 0;
                g_mbgDecode.frameRect[0].x = 0x140;
                g_mbgDecode.frameRect[0].w = 0x140;
                g_mbgDecode.frameRect[1].w = 0x140;
                g_mbgDecode.slice.w = 0x10;
                if (g_gameState->mode == 1 || g_gameState->mode == 5) {
                    g_fieldFlagQueue.mapFlags[1][0] = 9;
                    g_fieldFlagQueue.gameFlags[1][0] |= 0x14;
                }
                g_mbgPlayer.modeSwitch = 2;
            }
            if (g_mbgRgb24 == 0 && g_gameState->gameData->drawBuf == 1) {
                g_mbgPlayer.rgb24 = 1;
                g_mbgDecode.frameRect[0].x = 0x1E0;
                g_mbgDecode.frameRect[0].w = 0x1E0;
                g_mbgDecode.frameRect[1].w = 0x1E0;
                g_mbgDecode.slice.w = 0x18;
                g_gameState->gameData->flags |= 0x14;
                g_mbgRgb24 = 1;
                if (g_gameState->mode == 1 || g_gameState->mode == 5) {
                    g_gameState->gameData->ctx.ovl65->unk0 |= 9;
                }
                g_mbgPlayer.modeSwitch = 2;
            }
        }
        if ((s8)g_mbgPlayer.pauseReq != 0) {
            g_mbgPaused = 1;
            g_mbgPlayer.pauseReq = 0;
            mbg_streamSetPaused(1);
            g_padActuator[0].unk0 = 0;
            g_padActuator[0].unk1 = 0;
        }
        g_mbgDecode.slice.x = g_mbgDecode.frameRect[g_gameState->gameData->drawBuf ^ 1].x;
        g_mbgDecode.slice.y = g_mbgDecode.frameRect[g_gameState->gameData->drawBuf ^ 1].y;
    }
    LoadImage(&g_mbgLoadRects[g_mbgLoadRectIdx], g_mbgDecode.mdecOut[cur]);
    g_mbgLoadRectIdx = (g_mbgLoadRectIdx + 1) % 4;
}

INCLUDE_RODATA("asm/us/ovl_64/nonmatchings/mbg", s_mbgTestMoviePaths);

s32 MBG_init(s32 mode, u8 **buf) {
    u8 *b;
    s32 off;

    if (g_mbgState != 0) {
        printf("Warning! MBG_init() called when already initialized\n");
        g_mbgState = 0;
    }
    switch (mode) {
    case 1:
        b = buf[0];
        g_mbgPlayer.mdecIn0 = b;
        off = 0x10000;
        g_mbgPlayer.mdecIn1 = b + off;
        off = 0x20000;
        g_mbgPlayer.ringBuf = b + off;
        off = 0x43E20;
        g_mbgPlayer.audioBuf = b + off;
        off = 0x49E20;
        g_mbgPlayer.mdecOut0 = b + off;
        off = 0x4CB20;
        g_mbgPlayer.mdecOut1 = b + off;
        break;
    case 2:
        b = buf[0];
        g_mbgPlayer.mdecIn0 = b;
        off = 0x10000;
        g_mbgPlayer.mdecIn1 = b + off;
        off = 0x20000;
        g_mbgPlayer.mdecOut0 = b + off;
        off = 0x22D00;
        g_mbgPlayer.mdecOut1 = b + off;
        b = buf[1];
        g_mbgPlayer.ringBuf = b;
        off = 0x23E20;
        g_mbgPlayer.audioBuf = b + off;
        break;
    }
    SetGraphDebug(0);
    g_mbgRgb24 = -1;
    g_mbgState = 1;
    g_mbgFrame = 0;
    D_80071E30 = 0;
    g_mbgPlayer.volumeDelay = 3;
    g_mbgPlayer.volumeFade = 4;
    g_mbgPlayer.volume = 0x7F;
    g_mbgPlayer.modeSwitch = 0;
    g_mbgPlayer.vlcTable = g_mbgVlcTable;
    return 1;

}

static inline s32 func_8019_decode_first(void) {
    s32 tries;
    s32 res;
    u8 *data;
    Ovl64Dec *dec = &g_mbgDecode;

    while (1) {
        tries = 1000;
        res = -1;
        do {
            data = (u8 *)mbg_readFrame(dec);
            if (data != NULL) {
                goto found;
            }
            if ((s8)g_mbgPlayer.ended == 1) {
                res = -1;
                goto check;
            }
        } while (--tries != 0);
        goto check;
    found:
        g_mbgFrame++;
        dec->inIdx ^= 1;
        if (DecDCTBufSize((u16 *)data) > 0x4000) {
            mbg_streamFreeFrame(data);
            res = -1;
        } else {
            mbg_vlcDecode(data, dec->mdecIn[dec->inIdx], g_mbgPlayer.vlcTable);
            if (g_mbgPaused) {
                mbg_streamUngetFrame();
                res = 0;
            } else {
                mbg_streamFreeFrame(data);
                res = 0;
            }
        }
    check:
        if (res != -1) {
            break;
        }
        if ((s8)g_mbgPlayer.ended != 0) {
            return 0;
        }
        mbg_streamGetPos(&g_mbgPlayer.streamPos);
        mbg_streamSeek(&g_mbgPlayer.streamPos);
    }
    return 1;
}

s32 mbg_play(u32 movie, s32 idx) {
    Ovl64Size sz;
    s32 w;
    s32 file;
    s32 tries;
    s32 res;
    u8 *data;
    void (*cb)(void);
    s32 ok;
    Ovl64State *st;

    if (movie >= 5 || idx < 0 || idx >= g_mbgMovieLists[movie].count) {
        return 0;
    }
    g_mbgMovie = &g_mbgMovieLists[movie].entries[idx];
    D_80071E30 = g_mbgMovie->unk5;
    if (g_mbgRgb24 < 0) {
        mbg_setRgb24(g_mbgMovie->rgb24);
    }
    w = g_mbgMovie->xOffset;
    sz.y = 0;
    sz.x = w;
    if (g_mbgRgb24) {
        g_mbgPlayer.rgb24 = 1;
        g_mbgDecode.frameRect[0].x = w + 0x1E0;
        g_mbgDecode.inIdx = 0;
        g_mbgDecode.outIdx = 0;
        g_mbgDecode.frameRect[0].y = 0;
        g_mbgDecode.frameRect[1].x = w;
        g_mbgDecode.frameRect[1].y = 0;
        g_mbgDecode.slice.w = 0x18;
        g_mbgDecode.frameRect[0].w = 0x1E0;
        g_mbgDecode.frameRect[1].w = 0x1E0;
        g_mbgDecode.frameDone = 0;
        g_mbgDecode.mdecIn[0] = (u32 *)g_mbgPlayer.mdecIn0;
        g_mbgDecode.mdecIn[1] = (u32 *)g_mbgPlayer.mdecIn1;
        g_mbgDecode.mdecOut[0] = (u32 *)g_mbgPlayer.mdecOut0;
        g_mbgDecode.mdecOut[1] = (u32 *)g_mbgPlayer.mdecOut1;
    } else {
        g_mbgPlayer.rgb24 = 0;
        g_mbgDecode.frameRect[0].x = w + 0x140;
        g_mbgDecode.inIdx = 0;
        g_mbgDecode.outIdx = 0;
        g_mbgDecode.frameRect[0].y = 0;
        g_mbgDecode.frameRect[1].x = w;
        g_mbgDecode.frameRect[1].y = 0;
        g_mbgDecode.slice.w = 0x10;
        g_mbgDecode.frameRect[0].w = 0x140;
        g_mbgDecode.frameRect[1].w = 0x140;
        g_mbgDecode.frameDone = 0;
        g_mbgDecode.mdecIn[0] = (u32 *)g_mbgPlayer.mdecIn0;
        g_mbgDecode.mdecIn[1] = (u32 *)g_mbgPlayer.mdecIn1;
        g_mbgDecode.mdecOut[0] = (u32 *)g_mbgPlayer.mdecOut0;
        g_mbgDecode.mdecOut[1] = (u32 *)g_mbgPlayer.mdecOut1;
    }
    file = (s32)g_mbgMovie->path;
    cb = mbg_mdecOutCallback;
    DecDCTReset(0);
    DecDCToutCallback(cb);
    mbg_streamSetRing((Ovl64RingHdr *)g_mbgPlayer.ringBuf, 8, 0x40);
    mbg_streamSetAudioBuffer((u8 *)g_mbgPlayer.audioBuf, 2, 0xC);
    if (mbg_streamReset() != 0) {
        ok = 0;
    } else {
        if (g_gameState->mode == 1 || g_gameState->mode == 5) {
            mbg_streamSetBaseSector(((Ovl64Field8E4 *)g_gameState->gameData->ctx.ovl65)->area->scene->movieFile);
        }
        ok = mbg_streamStart(file, g_mbgMovie->startFrame, 99999, NULL, 0) == 0;
    }
    if (!ok) {
        return 0;
    }
    st = &g_mbgPlayer;
    g_mbgMaskCursor.runMasked = 0;
    g_mbgMaskCursor.runLength = 0;
    g_mbgMaskCursor.size = 0;
    g_mbgMaskCursor.data = 0;
    g_mbgMaskCursor.cursor = 0;
    if (st->vlcTable == NULL) {
        printf(g_mbgScratchpadMsg);
    }
    mbg_vlcBuildTable(st->vlcTable);
    g_mbgPaused = 0;
    D_80071E30 = 0;
    st->ended = 0;
    st->pauseReq = 0;
    st->unk5 = 0;
    st->firstFrame = 1;
    st->unk8 = 0;
    st->audioFrames = 0;
    g_mbgState++;
    return func_8019_decode_first();
}

static inline void func_8019_wait_frame(Ovl64Dec *dec) {
    volatile s32 n;

    n = 0x800000;
    while (dec->frameDone == 0) {
        if (--n == 0) {
            dec->frameDone = 1;
            dec->slice.x = dec->frameRect[g_gameState->gameData->drawBuf].x;
            dec->slice.y = dec->frameRect[g_gameState->gameData->drawBuf].y;
            DecDCTReset(1);
        }
    }
    dec->frameDone = 0;
}

s32 mbg_update(s32 arg0) {
    Ovl64State *st;
    Ovl64Dec *dec, *d;
    s32 tries;
    s32 res;
    u8 *data;
    u8 mode;
    DISPENV *env;
    CdlLOC *loc;

    if (g_mbgState < 2) {
        return 0;
    }
    st = &g_mbgPlayer;
    if ((s8)st->ended != 0) {
        mbg_stop();
        return 0;
    }
    if (mbg_streamPushAudio() == 0) {
        if (st->audioFrames++ == st->volumeDelay) {
            akao_sendCmdE5(st->volumeFade, st->volume);
        }
    }
    st = &g_mbgPlayer;
    if ((s8)st->modeSwitch == 2) {
        mbg_setGraphEnv(0xE0, g_mbgRgb24);
        st->modeSwitch = 0;
    }
    st->streamPos = st->startPos;
    mode = g_gameState->mode;
    if (mode == 1 || mode == 5) {
        fldbg_getCamera(&st->savedCameraId);
        st->savedOtBias = ((Ovl64Field8E4 *)g_gameState->gameData->ctx.ovl65)->area->scene->otBias;
        ((Ovl64Field8E4 *)g_gameState->gameData->ctx.ovl65)->area->scene->otBias = 0;
    }
    g_mbgMaskCursor.runMasked = 0;
    g_mbgMaskCursor.runLength = 0;
    g_mbgMaskCursor.size = g_mbgMaskSize[g_gameState->gameData->drawBuf];
    g_mbgMaskCursor.cursor = g_mbgMaskCursor.data = (s32)&g_mbgMaskData[g_gameState->gameData->drawBuf];
    if (g_mbgRgb24 == 0 && g_mbgMaskCursor.size != 0) {
        Ovl64Stp *p = &g_mbgMaskStp[g_gameState->gameData->drawBuf];
        p->len = 2;
        p->code[0] = 0xE6000002;
        p->code[1] = 0;
    }
    dec = &g_mbgDecode;
    dec->slice.x = dec->frameRect[g_gameState->gameData->drawBuf].x;
    dec->slice.y = dec->frameRect[g_gameState->gameData->drawBuf].y;
    DecDCTin(dec->mdecIn[dec->inIdx], (s8)g_mbgPlayer.rgb24);
    DecDCTout(dec->mdecOut[dec->outIdx], (dec->slice.w * dec->slice.h) >> 1);
    while (1) {
        d = &g_mbgDecode;
        tries = 1000;
        res = -1;
        do {
            data = (u8 *)mbg_readFrame(d);
            if (data != NULL) {
                goto found;
            }
            if ((s8)g_mbgPlayer.ended == 1) {
                res = -1;
                goto check;
            }
        } while (--tries != 0);
        goto check;
    found:
        g_mbgFrame++;
        d->inIdx ^= 1;
        if (DecDCTBufSize((u16 *)data) > 0x4000) {
            mbg_streamFreeFrame(data);
            res = -1;
        } else {
            mbg_vlcDecode(data, d->mdecIn[d->inIdx], g_mbgPlayer.vlcTable);
            if (g_mbgPaused) {
                mbg_streamUngetFrame();
                res = 0;
            } else {
                mbg_streamFreeFrame(data);
                res = 0;
            }
        }
    check:
        if (res != -1) {
            break;
        }
        if ((s8)g_mbgPlayer.ended != 0) {
            func_8019_wait_frame(&g_mbgDecode);
            mode = g_gameState->mode;
            if (mode == 1 || mode == 5) {
                ((Ovl64Field8E4 *)g_gameState->gameData->ctx.ovl65)->area->scene->otBias = g_mbgPlayer.savedOtBias;
                fldbg_setCamera((s8)g_mbgPlayer.savedCameraId);
            }
            return 1;
        }
        mbg_streamGetPos(&g_mbgPlayer.streamPos);
        mbg_streamSeek(&g_mbgPlayer.streamPos);
    }
    func_8019_wait_frame(&g_mbgDecode);
    g_gameState->vsyncMode = 4;
    if (g_mbgFrame != 0 && g_mbgFrame % 50 == 0) {
        g_mbgFrame50Flag = 1;
    }
    if (g_mbgPlayer.unk8 != 0 && g_gameState->vsyncMode >= 4) {
        g_gameState->vsyncMode--;
        g_mbgPlayer.unk8 = 0;
    }
    if (arg0 != 0) {
        env = (DISPENV *)(g_gameState->gameData->drawBuf * 20 + (s32)g_gameState->dispEnv);
        VSync(0);
        PutDispEnv(env);
        SetDispMask(1);
    }
    return 1;
}

s32 mbg_setRgb24(s32 on) {
    s8 k;

    if (g_mbgRgb24 >= 0) {
        if (on != 0) {
            if (g_mbgRgb24 != 0) {
                return 0;
            }
        } else if (g_mbgRgb24 != 1) {
            return 0;
        }
    }
    if (mbg_getStatus() & 2) {
        g_mbgPlayer.modeSwitch = 1;
    } else if (on != 0) {
        k = 1;
        g_mbgRgb24 = k;
        g_mbgPlayer.rgb24 = k;
        g_mbgDecode.frameRect[0].x = 0x1E0;
        g_mbgDecode.slice.w = 0x18;
        g_mbgPlayer.modeSwitch = 2;
    } else {
        g_mbgRgb24 = 0;
        g_mbgPlayer.rgb24 = 0;
        g_mbgDecode.frameRect[0].x = 0x140;
        g_mbgDecode.slice.w = 0x10;
    }
    return 1;
}

void mbg_setDispEnv(s32 h, s32 hires, s32 i) {
    s32 w = 0x140;

    if (hires != 0) {
        w = 0x1E0;
    }
    SetDefDispEnv(&g_gameState->dispEnv[i], w, 0, w, h);
    if (hires != 0) {
        g_gameState->dispEnv[i].isrgb24 = 1;
        g_gameState->dispEnv[i].disp.w = g_gameState->dispEnv->disp.w * 2 / 3;
    }
    g_gameState->dispEnv[i].isinter = 0;
    g_gameState->dispEnv[i].screen.x = 0;
    g_gameState->dispEnv[i].screen.y = (0xF0 - h) >> 1;
    g_gameState->dispEnv[i].screen.h = h;
}

void mbg_setGraphEnv(s32 h, s32 hires) {
    s32 w = 0x140;

    if (hires != 0) {
        w = 0x1E0;
    }
    SetDefDispEnv(&g_gameState->dispEnv[0], w, 0, w, h);
    SetDefDispEnv(&g_gameState->dispEnv[1], 0, 0, w, h);
    if (hires != 0) {
        g_gameState->dispEnv[0].isrgb24 = 1;
        g_gameState->dispEnv[0].disp.w = g_gameState->dispEnv[0].disp.w * 2 / 3;
        g_gameState->dispEnv[1].isrgb24 = 1;
        g_gameState->dispEnv[1].disp.w = g_gameState->dispEnv[1].disp.w * 2 / 3;
    }
    g_gameState->dispEnv[0].isinter = 0;
    g_gameState->dispEnv[0].screen.x = 0;
    g_gameState->dispEnv[0].screen.y = (0xF0 - h) >> 1;
    g_gameState->dispEnv[0].screen.h = h;
    g_gameState->dispEnv[1].isinter = 0;
    g_gameState->dispEnv[1].screen.x = 0;
    g_gameState->dispEnv[1].screen.y = (0xF0 - h) >> 1;
    g_gameState->dispEnv[1].screen.h = h;
    SetDefDrawEnv(&g_gameState->drawEnv[0], 0, 0, w, h);
    SetDefDrawEnv(&g_gameState->drawEnv[1], w, 0, w, h);
    g_gameState->drawEnv[0].dfe = 0;
    g_gameState->drawEnv[0].dtd = 1;
    g_gameState->drawEnv[0].isbg = 1;
    g_gameState->drawEnv[1].dfe = 0;
    g_gameState->drawEnv[1].dtd = 1;
    g_gameState->drawEnv[1].isbg = 1;
}

s32 mbg_shutdown(void) {
    if (g_mbgState == 0) {
        return 0;
    }
    g_mbgPlayer.mdecIn0 = 0;
    g_mbgPlayer.mdecIn1 = 0;
    g_mbgPlayer.ringBuf = 0;
    g_mbgPlayer.audioBuf = 0;
    g_mbgPlayer.vlcTable = 0;
    g_mbgPlayer.mdecOut0 = 0;
    g_mbgPlayer.mdecOut1 = 0;
    g_mbgState = 0;
    return 1;
}

s32 mbg_putDispEnv(DISPENV *env) {
    VSync(0);
    PutDispEnv(env);
    SetDispMask(1);
    return 1;
}

s32 mbg_playToEnd(void) {
    u8 done;

    if (g_mbgState < 2) {
        return 0;
    }
    done = 0;
    while (!done) {
        if (mbg_update(1) == 0) {
            done = 1;
        }
    }
    return 1;
}

s32 mbg_setPause(s32 arg0) {
    if (arg0 != 0) {
        g_mbgPlayer.pauseReq = 1;
    } else {
        g_mbgPlayer.pauseReq = 0;
        g_mbgPaused = 0;
        mbg_streamSetPaused(0);
    }
    return 1;
}

s32 mbg_stop(void) {
    akao_sendCmdE2();
    mbg_streamStop();
    g_padActuator[0].unk0 = 0;
    g_padActuator[0].unk1 = 0;
    g_mbgState--;
    if (g_mbgPlayer.endCallback != NULL) {
        g_mbgPlayer.endCallback();
    }
    DecDCToutCallback(0);
    mbg_setGraphEnv(0xE0, 0);
    ResetGraph(3);
    return 0;
}

s32 mbg_getFrame(void) {
    if (g_mbgState != 0) {
        return g_mbgFrame;
    }
    return -1;
}

void mbg_setVolume(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 < 0) {
        g_mbgPlayer.volumeDelay = 3;
    } else {
        g_mbgPlayer.volumeDelay = arg0;
    }
    if (arg1 < 0) {
        g_mbgPlayer.volumeFade = 4;
    } else {
        g_mbgPlayer.volumeFade = arg1;
    }
    if (arg2 < 0) {
        g_mbgPlayer.volume = 0x7F;
    } else {
        g_mbgPlayer.volume = arg2;
    }
}

s32 mbg_getCameraSelect(void) {
    s32 v;

    mbg_streamGetCameraSelect(&v);
    return v;
}

void mbg_setCameraSelect(u32 arg0) {
    if (arg0 < 2) {
        mbg_streamSetCameraSelect(arg0);
    }
}

void mbg_setEndCallback(void (*arg0)()) {
    g_mbgPlayer.endCallback = arg0;
}

void mbg_setScratchVlcTable(s32 arg0) {
    if (arg0 != 0) {
        g_mbgPlayer.vlcTable = NULL;
    } else {
        g_mbgPlayer.vlcTable = g_mbgVlcTable;
    }
}

s32 mbg_getEntryCount(s32 *arg0, u32 arg1) {
    if (arg1 >= 6) {
        return 0;
    }
    *arg0 = g_mbgMovieLists[arg1].count;
    return 1;
}

s32 mbg_getEntryName(s32 arg0, u32 arg1, s32 arg2) {
    if (arg1 >= 6 || arg2 < 0 || arg2 >= g_mbgMovieLists[arg1].count) {
        return 0;
    }
    strcpy((char *)arg0, g_mbgMovieLists[arg1].entries[arg2].path);
    return 1;
}

s32 mbg_getLastFrame(void) {
    s32 v;

    if (g_mbgState < 2) {
        return -1;
    }
    mbg_streamGetLastFrame(&v);
    return v;
}

s32 mbg_getStartCamera(Ovl64Header *arg0, u16 *arg1) {
    if (g_mbgPlayer.firstFrame != 0) {
        return 0;
    }
    arg0->m00 = g_mbgStartCamera.m00;
    arg0->m01 = g_mbgStartCamera.m01;
    arg0->m02 = g_mbgStartCamera.m02;
    arg0->m10 = g_mbgStartCamera.m10;
    arg0->m11 = g_mbgStartCamera.m11;
    arg0->m12 = g_mbgStartCamera.m12;
    arg0->m20 = g_mbgStartCamera.m20;
    arg0->m21 = g_mbgStartCamera.m21;
    arg0->m22 = g_mbgStartCamera.m22;
    arg0->tx = g_mbgStartCamera.tx;
    arg0->ty = g_mbgStartCamera.ty;
    arg0->tz = g_mbgStartCamera.tz;
    *arg1 = g_mbgStartCameraId;
    return 1;
}

s32 mbg_getGeomOffset(s32 *arg0, s32 *arg1) {
    if (g_mbgPlayer.firstFrame != 0) {
        return 0;
    }
    *arg0 = g_mbgGeomOffset.x;
    *arg1 = g_mbgGeomOffset.y;
    return 1;
}

INCLUDE_RODATA("asm/us/ovl_64/nonmatchings/mbg", g_mbgScratchpadMsg);
