#include "ovl_64.h"

u8 *mbg_readFrame(Ovl64Out *out) {
    RECT r;
    u8 *data;
    Ovl64RingHdr *hdr;
    Ovl64FrameInfo *info;
    Ovl64Pair *pair;
    MbgRumble *flag;
    s32 tries;
    s32 ret;
    s8 hi;

    tries = 1000;
    info = NULL;
    pair = NULL;
    flag = NULL;
    ret = mbg_streamGetFrame(&data, &hdr, &info, &pair, &flag);
    while (ret != 0) {
        if (ret == 5) {
            g_mbgPlayer.ended = 1;
            return NULL;
        }
        if (--tries == 0) {
            return NULL;
        }
        ret = mbg_streamGetFrame(&data, &hdr, &info, &pair, &flag);
    }
    if (info != NULL) {
        gte_SetGeomOffset((s16)info->unk20 + 0xA0, (s16)info->unk22 + 0x70);
        if (g_mbgPlayer.unk6 != 0) {
            g_mbgGeomOffset.unk0 = (s16)info->unk20 + 0xA0;
            g_mbgGeomOffset.unk4 = (s16)info->unk22 + 0x70;
            g_mbgStartCamera.unk0 = info->unk2;
            g_mbgStartCamera.unk2 = info->unk4;
            g_mbgStartCamera.unk4 = info->unk6;
            g_mbgStartCamera.unk6 = info->unk8;
            g_mbgStartCamera.unk8 = info->unkA;
            g_mbgStartCamera.unkA = info->unkC;
            g_mbgStartCamera.unkC = info->unkE;
            g_mbgStartCamera.unkE = info->unk10;
            g_mbgStartCamera.unk10 = info->unk12;
            g_mbgStartCamera.unk14 = info->unk14;
            g_mbgStartCamera.unk18 = info->unk18;
            g_mbgStartCamera.unk1C = info->unk1C;
            D_8019F278 = info->unk0;
            g_mbgPlayer.unk6 = 0;
        } else {
            g_gameState->unk1C->unk18.m[0][0] = info->unk2;
            g_gameState->unk1C->unk18.m[0][1] = info->unk4;
            g_gameState->unk1C->unk18.m[0][2] = info->unk6;
            g_gameState->unk1C->unk18.m[1][0] = info->unk8;
            g_gameState->unk1C->unk18.m[1][1] = info->unkA;
            g_gameState->unk1C->unk18.m[1][2] = info->unkC;
            g_gameState->unk1C->unk18.m[2][0] = info->unkE;
            g_gameState->unk1C->unk18.m[2][1] = info->unk10;
            g_gameState->unk1C->unk18.m[2][2] = info->unk12;
            g_gameState->unk1C->unk18.t[0] = info->unk14;
            g_gameState->unk1C->unk18.t[1] = info->unk18;
            g_gameState->unk1C->unk18.t[2] = info->unk1C;
            g_gameState->unk1C->unkA = info->unk0;
        }
    }
    if (flag != NULL && (!((Ovl64Cfg *)&g_gameState->unk24)->b7 || g_gameState->mode == 5)) {
        D_800739B0[0].unk0 = flag->on;
        D_800739B0[0].unk1 = flag->level;
    }
    if (pair != NULL && pair->unk0 != 0) {
        bcopy((void *)pair->unk4, &g_mbgMaskData[g_gameState->unk1C->unk8 ^ 1], pair->unk0);
        g_mbgMaskSize[g_gameState->unk1C->unk8 ^ 1] = pair->unk0;
    } else {
        g_mbgMaskSize[g_gameState->unk1C->unk8 ^ 1] = 0;
    }
    if (g_mbgFrame >= g_mbgMovie->lastFrame) {
        g_mbgPlayer.ended = 1;
    }
    if (g_mbgPlayer.width != hdr->unk10 || g_mbgPlayer.height != hdr->unk12) {
        hi = g_mbgRgb24;
        if (hi) {
            r.x = 0;
            r.y = 0;
            r.w = 0x3C0;
            r.h = 0xE0;
        }
        g_mbgPlayer.width = hdr->unk10;
        g_mbgPlayer.height = hdr->unk12;
        if (hi) {
            out->unk1C = g_mbgPlayer.width * 3 / 2;
            out->unk24 = g_mbgPlayer.width * 3 / 2;
        } else {
            out->unk1C = g_mbgPlayer.width;
            out->unk24 = g_mbgPlayer.width;
        }
        out->unk1E = g_mbgPlayer.height;
        out->unk26 = g_mbgPlayer.height;
        out->unk2E = g_mbgPlayer.height;
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
    if (g_mbgRgb24 == 0 && g_mbgMaskCursor.unk4 != 0) {
        mbg_applyMask(&g_mbgMaskCursor, g_mbgDecode.mdecOut[g_mbgDecode.outIdx], g_mbgMaskBit);
    }
    cur = g_mbgDecode.outIdx;
    g_mbgLoadRects[g_mbgLoadRectIdx] = g_mbgDecode.slice;
    g_mbgDecode.outIdx ^= 1;
    g_mbgDecode.slice.x += g_mbgDecode.slice.w;
    if (g_mbgDecode.slice.x < g_mbgDecode.frameRect[g_gameState->unk1C->unk8].x + g_mbgDecode.frameRect[g_gameState->unk1C->unk8].w) {
        if (DecDCTinSync(1) == 0) {
            g_mbgDecode.frameDone = 1;
            g_mbgDecode.slice.x = g_mbgDecode.frameRect[g_gameState->unk1C->unk8 ^ 1].x;
            g_mbgDecode.slice.y = g_mbgDecode.frameRect[g_gameState->unk1C->unk8 ^ 1].y;
        }
        DecDCTout(g_mbgDecode.mdecOut[g_mbgDecode.outIdx], (g_mbgDecode.slice.w * g_mbgDecode.slice.h) >> 1);
    } else {
        if (DecDCTinSync(1) == 1) {
            DecDCTReset(1);
        }
        g_mbgDecode.frameDone = 1;
        if ((s8)g_mbgPlayer.unk3 == 1) {
            if (g_mbgRgb24 == 1 && g_gameState->unk1C->unk8 == 0) {
                g_mbgRgb24 = 0;
                g_mbgPlayer.rgb24 = 0;
                g_mbgDecode.frameRect[0].x = 0x140;
                g_mbgDecode.frameRect[0].w = 0x140;
                g_mbgDecode.frameRect[1].w = 0x140;
                g_mbgDecode.slice.w = 0x10;
                if (g_gameState->mode == 1 || g_gameState->mode == 5) {
                    D_800C9EF0.unk18 = 9;
                    D_800C9EF0.unk8 |= 0x14;
                }
                D_8019DC97 = 2;
            }
            if (g_mbgRgb24 == 0 && g_gameState->unk1C->unk8 == 1) {
                g_mbgPlayer.rgb24 = 1;
                g_mbgDecode.frameRect[0].x = 0x1E0;
                g_mbgDecode.frameRect[0].w = 0x1E0;
                g_mbgDecode.frameRect[1].w = 0x1E0;
                g_mbgDecode.slice.w = 0x18;
                g_gameState->unk1C->unk0 |= 0x14;
                g_mbgRgb24 = 1;
                if (g_gameState->mode == 1 || g_gameState->mode == 5) {
                    g_gameState->unk1C->unk8E4.ovl65->unk0 |= 9;
                }
                D_8019DC97 = 2;
            }
        }
        if ((s8)g_mbgPlayer.pauseReq != 0) {
            g_mbgPaused = 1;
            g_mbgPlayer.pauseReq = 0;
            mbg_streamSetPaused(1);
            D_800739B0[0].unk0 = 0;
            D_800739B0[0].unk1 = 0;
        }
        g_mbgDecode.slice.x = g_mbgDecode.frameRect[g_gameState->unk1C->unk8 ^ 1].x;
        g_mbgDecode.slice.y = g_mbgDecode.frameRect[g_gameState->unk1C->unk8 ^ 1].y;
    }
    LoadImage(&g_mbgLoadRects[g_mbgLoadRectIdx], g_mbgDecode.mdecOut[cur]);
    g_mbgLoadRectIdx = (g_mbgLoadRectIdx + 1) % 4;
}

INCLUDE_RODATA("asm/us/ovl_64/nonmatchings/mbg", D_80198700);

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
    g_mbgPlayer.unk38 = 3;
    g_mbgPlayer.unk3C = 4;
    g_mbgPlayer.unk40 = 0x7F;
    g_mbgPlayer.unk3 = 0;
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
            data = (u8 *)mbg_readFrame((Ovl64Out *)dec);
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
        if (func_8019D3F0((u16 *)data) > 0x4000) {
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
        mbg_streamGetPos(&g_mbgPlayer.unk24);
        mbg_streamSeek(&g_mbgPlayer.unk24);
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
    sz.unk4 = 0;
    sz.unk0 = w;
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
    file = g_mbgMovie->unk0;
    cb = mbg_mdecOutCallback;
    DecDCTReset(0);
    DecDCToutCallback(cb);
    mbg_streamSetRing((Ovl64RingHdr *)g_mbgPlayer.ringBuf, 8, 0x40);
    mbg_streamSetAudioBuffer((u8 *)g_mbgPlayer.audioBuf, 2, 0xC);
    if (mbg_streamReset() != 0) {
        ok = 0;
    } else {
        if (g_gameState->mode == 1 || g_gameState->mode == 5) {
            mbg_streamSetBaseSector(((Ovl64Field8E4 *)g_gameState->unk1C->unk8E4.ovl65)->unkC->unk14->unk44);
        }
        ok = mbg_streamStart(file, g_mbgMovie->startFrame, 99999, NULL, 0) == 0;
    }
    if (!ok) {
        return 0;
    }
    st = &g_mbgPlayer;
    g_mbgMaskCursor.unk0 = 0;
    g_mbgMaskCursor.unk2 = 0;
    g_mbgMaskCursor.unk4 = 0;
    g_mbgMaskCursor.unk8 = 0;
    g_mbgMaskCursor.unkC = 0;
    if (st->vlcTable == NULL) {
        printf(g_mbgScratchpadMsg);
    }
    mbg_vlcBuildTable(st->vlcTable);
    g_mbgPaused = 0;
    D_80071E30 = 0;
    st->ended = 0;
    st->pauseReq = 0;
    st->unk5 = 0;
    st->unk6 = 1;
    st->unk8 = 0;
    st->unk34 = 0;
    g_mbgState++;
    return func_8019_decode_first();
}

static inline void func_8019_wait_frame(Ovl64Dec *dec) {
    volatile s32 n;

    n = 0x800000;
    while (dec->frameDone == 0) {
        if (--n == 0) {
            dec->frameDone = 1;
            dec->slice.x = dec->frameRect[g_gameState->unk1C->unk8].x;
            dec->slice.y = dec->frameRect[g_gameState->unk1C->unk8].y;
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
        if (st->unk34++ == st->unk38) {
            func_80056278(st->unk3C, st->unk40);
        }
    }
    st = &g_mbgPlayer;
    if ((s8)st->unk3 == 2) {
        mbg_setGraphEnv(0xE0, g_mbgRgb24);
        st->unk3 = 0;
    }
    st->unk24 = st->unkC;
    mode = g_gameState->mode;
    if (mode == 1 || mode == 5) {
        func_800C1B5C(&st->unk2);
        st->unk28 = ((Ovl64Field8E4 *)g_gameState->unk1C->unk8E4.ovl65)->unkC->unk14->unk30;
        ((Ovl64Field8E4 *)g_gameState->unk1C->unk8E4.ovl65)->unkC->unk14->unk30 = 0;
    }
    g_mbgMaskCursor.unk0 = 0;
    g_mbgMaskCursor.unk2 = 0;
    g_mbgMaskCursor.unk4 = g_mbgMaskSize[g_gameState->unk1C->unk8];
    g_mbgMaskCursor.unkC = g_mbgMaskCursor.unk8 = (s32)&g_mbgMaskData[g_gameState->unk1C->unk8];
    if (g_mbgRgb24 == 0 && g_mbgMaskCursor.unk4 != 0) {
        Ovl64Stp *p = &D_8019F4A8[g_gameState->unk1C->unk8];
        p->unk3 = 2;
        p->unk4 = 0xE6000002;
        p->unk8 = 0;
    }
    dec = &g_mbgDecode;
    dec->slice.x = dec->frameRect[g_gameState->unk1C->unk8].x;
    dec->slice.y = dec->frameRect[g_gameState->unk1C->unk8].y;
    DecDCTin(dec->mdecIn[dec->inIdx], (s8)g_mbgPlayer.rgb24);
    DecDCTout(dec->mdecOut[dec->outIdx], (dec->slice.w * dec->slice.h) >> 1);
    while (1) {
        d = &g_mbgDecode;
        tries = 1000;
        res = -1;
        do {
            data = (u8 *)mbg_readFrame((Ovl64Out *)d);
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
        if (func_8019D3F0((u16 *)data) > 0x4000) {
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
                ((Ovl64Field8E4 *)g_gameState->unk1C->unk8E4.ovl65)->unkC->unk14->unk30 = g_mbgPlayer.unk28;
                func_800C0BCC((s8)g_mbgPlayer.unk2);
            }
            return 1;
        }
        mbg_streamGetPos(&g_mbgPlayer.unk24);
        mbg_streamSeek(&g_mbgPlayer.unk24);
    }
    func_8019_wait_frame(&g_mbgDecode);
    g_gameState->vsyncMode = 4;
    if (g_mbgFrame != 0 && g_mbgFrame % 50 == 0) {
        D_8019DC9C = 1;
    }
    if (g_mbgPlayer.unk8 != 0 && g_gameState->vsyncMode >= 4) {
        g_gameState->vsyncMode--;
        g_mbgPlayer.unk8 = 0;
    }
    if (arg0 != 0) {
        env = (DISPENV *)(g_gameState->unk1C->unk8 * 20 + (s32)g_gameState->unkC);
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
    if (func_80012AD4() & 2) {
        D_8019DC97 = 1;
    } else if (on != 0) {
        k = 1;
        g_mbgRgb24 = k;
        g_mbgPlayer.rgb24 = k;
        g_mbgDecode.frameRect[0].x = 0x1E0;
        g_mbgDecode.slice.w = 0x18;
        g_mbgPlayer.unk3 = 2;
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
    SetDefDispEnv(&g_gameState->unkC[i], w, 0, w, h);
    if (hires != 0) {
        g_gameState->unkC[i].isrgb24 = 1;
        g_gameState->unkC[i].disp.w = g_gameState->unkC->disp.w * 2 / 3;
    }
    g_gameState->unkC[i].isinter = 0;
    g_gameState->unkC[i].screen.x = 0;
    g_gameState->unkC[i].screen.y = (0xF0 - h) >> 1;
    g_gameState->unkC[i].screen.h = h;
}

void mbg_setGraphEnv(s32 h, s32 hires) {
    s32 w = 0x140;

    if (hires != 0) {
        w = 0x1E0;
    }
    SetDefDispEnv(&g_gameState->unkC[0], w, 0, w, h);
    SetDefDispEnv(&g_gameState->unkC[1], 0, 0, w, h);
    if (hires != 0) {
        g_gameState->unkC[0].isrgb24 = 1;
        g_gameState->unkC[0].disp.w = g_gameState->unkC[0].disp.w * 2 / 3;
        g_gameState->unkC[1].isrgb24 = 1;
        g_gameState->unkC[1].disp.w = g_gameState->unkC[1].disp.w * 2 / 3;
    }
    g_gameState->unkC[0].isinter = 0;
    g_gameState->unkC[0].screen.x = 0;
    g_gameState->unkC[0].screen.y = (0xF0 - h) >> 1;
    g_gameState->unkC[0].screen.h = h;
    g_gameState->unkC[1].isinter = 0;
    g_gameState->unkC[1].screen.x = 0;
    g_gameState->unkC[1].screen.y = (0xF0 - h) >> 1;
    g_gameState->unkC[1].screen.h = h;
    SetDefDrawEnv(&g_gameState->unk10[0], 0, 0, w, h);
    SetDefDrawEnv(&g_gameState->unk10[1], w, 0, w, h);
    g_gameState->unk10[0].dfe = 0;
    g_gameState->unk10[0].dtd = 1;
    g_gameState->unk10[0].isbg = 1;
    g_gameState->unk10[1].dfe = 0;
    g_gameState->unk10[1].dtd = 1;
    g_gameState->unk10[1].isbg = 1;
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
    func_80056228();
    mbg_streamStop();
    D_800739B0[0].unk0 = 0;
    D_800739B0[0].unk1 = 0;
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

void func_8019AB98(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 < 0) {
        g_mbgPlayer.unk38 = 3;
    } else {
        g_mbgPlayer.unk38 = arg0;
    }
    if (arg1 < 0) {
        g_mbgPlayer.unk3C = 4;
    } else {
        g_mbgPlayer.unk3C = arg1;
    }
    if (arg2 < 0) {
        g_mbgPlayer.unk40 = 0x7F;
    } else {
        g_mbgPlayer.unk40 = arg2;
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
    strcpy((char *)arg0, (char *)g_mbgMovieLists[arg1].entries[arg2].unk0);
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
    if (g_mbgPlayer.unk6 != 0) {
        return 0;
    }
    arg0->unk0 = g_mbgStartCamera.unk0;
    arg0->unk2 = g_mbgStartCamera.unk2;
    arg0->unk4 = g_mbgStartCamera.unk4;
    arg0->unk6 = g_mbgStartCamera.unk6;
    arg0->unk8 = g_mbgStartCamera.unk8;
    arg0->unkA = g_mbgStartCamera.unkA;
    arg0->unkC = g_mbgStartCamera.unkC;
    arg0->unkE = g_mbgStartCamera.unkE;
    arg0->unk10 = g_mbgStartCamera.unk10;
    arg0->unk14 = g_mbgStartCamera.unk14;
    arg0->unk18 = g_mbgStartCamera.unk18;
    arg0->unk1C = g_mbgStartCamera.unk1C;
    *arg1 = D_8019F278;
    return 1;
}

s32 mbg_getGeomOffset(s32 *arg0, s32 *arg1) {
    if (g_mbgPlayer.unk6 != 0) {
        return 0;
    }
    *arg0 = g_mbgGeomOffset.unk0;
    *arg1 = g_mbgGeomOffset.unk4;
    return 1;
}

INCLUDE_RODATA("asm/us/ovl_64/nonmatchings/mbg", g_mbgScratchpadMsg);
