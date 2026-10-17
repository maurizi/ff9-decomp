#include "ovl_64.h"

u8 *mbg_readFrame(Ovl64Out *out) {
    RECT r;
    u8 *data;
    Ovl64RingHdr *hdr;
    Ovl64FrameInfo *info;
    Ovl64Pair *pair;
    Ovl64Flag *flag;
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
        D_800739B0[0].unk0 = flag->unk0;
        D_800739B0[0].unk1 = flag->unk1;
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

INCLUDE_RODATA("asm/us/ovl_64/nonmatchings/ovl_64", D_80198700);

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
        printf(D_8019903C);
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
    g_gameState->unk9 = 4;
    if (g_mbgFrame != 0 && g_mbgFrame % 50 == 0) {
        D_8019DC9C = 1;
    }
    if (g_mbgPlayer.unk8 != 0 && g_gameState->unk9 >= 4) {
        g_gameState->unk9--;
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

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", mbg_vlcBuildTable);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", mbg_vlcSetSize);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", mbg_vlcDecode);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", mbg_vlcDecodeBlock);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", mbg_vlcDecodeLumaBlockV3);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", mbg_applyMask);

void mbg_drawStreamDebug(u32 *ot, s32 buf) {
    POLY_F4 *p;
    Ovl64RingHdr *h;
    s32 j;
    u8 i;
    s32 n;
    u8 r;
    u8 g;
    u8 b;

    h = g_mbgStream.unk74;
    n = 0;
    j = 0;
    for (i = 0; i < g_mbgStream.unk60; i++) {
        r = g_mbgDebugRingColors[j].r;
        g = g_mbgDebugRingColors[j].g;
        b = g_mbgDebugRingColors[j].b;
        if (h->unk0 & 0x8000) {
            if (r != 0) {
                r += 0x60;
            }
            if (g != 0) {
                g += 0x60;
            }
            if (b != 0) {
                b += 0x60;
            }
        } else if (h->unk0 & 0x4000) {
            if (r != 0) {
                r -= 0x20;
            }
            if (g != 0) {
                g -= 0x20;
            }
            if (b != 0) {
                b -= 0x20;
            }
        }
        p = &g_mbgDebugRingPolys[buf][n++];
        SetPolyF4(p);
        setRGB0(p, r, g, b);
        setXY4(p, i * 6 + 0x11, 0xBC, i * 6 + 0x16, 0xBC, i * 6 + 0x11, 0xC4, i * 6 + 0x16, 0xC4);
        addPrim(ot, p);
        if (((i + 1) & 7) == 0) {
            j++;
            h++;
        }
    }
    p = &g_mbgDebugRingPolys[buf][n];
    SetPolyF4(p);
    setRGB0(p, 0x10, 0x10, 0x10);
    setXY4(p, 0x10, 0xBB, 0x132, 0xBB, 0x10, 0xC5, 0x132, 0xC5);
    addPrim(ot, p);
}

void mbg_streamOnVideoSector(void) {
    Ovl64SectHdr *s;
    u32 *p;
    u32 *src;
    u32 *dst;
    u32 i;
    Ovl64RingHdr *h;

    p = (u32 *)(g_mbgStream.unk78 +
                (g_mbgStream.unk68 * g_mbgStream.unk5C - (g_mbgStream.unk64 + 1 - g_mbgStream.unk5C)) * 0x8F4);
    p -= 8;
    if (g_mbgStream.unk64 == 0) {
        dst = (u32 *)&g_mbgStream.unk74[g_mbgStream.unk68];
        for (i = 0, src = p; i < 8; i++) {
            *dst++ = *src++;
        }
        s = (Ovl64SectHdr *)p;
        if (s->unk8 != g_mbgStream.frame) {
            g_mbgStream.unk2 = 2;
            func_80023018();
        }
        if (s->unk2 != 4) {
            goto end;
        }
    }
    s = (Ovl64SectHdr *)p;
    if ((s->unk1C.w >> 28) & 2) {
        g_mbgSlotMasks[g_mbgStream.unk68].unk0 = s->unk1C.w & 0xFFFF;
        g_mbgSlotMasks[g_mbgStream.unk68].unk4 = (s32)((u8 *)s + (s->unk1C.h.hi & 0xFFF));
    }
end:
    g_mbgStream.sector++;
    if (++g_mbgStream.unk48 >= g_mbgStream.frameSectors) {
        g_mbgStream.unk64++;
        g_mbgStream.unk48 = 0;
        g_mbgStream.frame++;
        if (g_mbgStream.unk8 != NULL) {
            g_mbgStream.unk8();
        }
        h = &g_mbgStream.unk74[g_mbgStream.unk68];
        h->unk4 = g_mbgStream.unk5C - g_mbgStream.unk64;
        h->unk0 = (h->unk0 & 0xFFF) | 0xC000;
        if (g_mbgStream.frame > g_mbgStream.lastFrame) {
            g_mbgStream.unk2 = 1;
            func_80023018();
        }
        g_mbgStream.unk64 = 0;
        if (g_mbgStream.unk68 == 0) {
            g_mbgStream.unk68 = g_mbgStream.slotCount;
        }
        g_mbgStream.unk68--;
        g_mbgSlotMasks[g_mbgStream.unk68].unk0 = 0;
        g_mbgSlotMasks[g_mbgStream.unk68].unk4 = 0;
    } else {
        g_mbgStream.unk64++;
    }
    if (++g_mbgStream.unk4C >= g_mbgStream.unk50) {
        g_mbgStream.unk4C = 0;
    }
}

void mbg_streamOnAudioSector(void) {
    Ovl64StreamSect *s;
    Ovl64FrameInfo *info;
    Ovl64RingHdr *h;
    u32 cur;

    info = (Ovl64FrameInfo *)&g_mbgStream; /* what the callback gets when the sector has no frame info */
    g_mbgStream.sector++;
    s = (Ovl64StreamSect *)(g_mbgStream.unk90 + (g_mbgStream.unk80 << 11));
    if (++g_mbgStream.unk4C >= g_mbgStream.unk50) {
        g_mbgStream.unk4C = 0;
    }
    if (g_mbgStream.unk4C == 2) {
        if (s->unk12 == 1) {
            if (g_mbgStream.cameraSel == 1) {
                info = &s->unk14[1];
            } else {
                info = &s->unk14[0];
            }
            g_mbgSlotCameras[g_mbgStream.unk68].unk0 = info->unk0;
            g_mbgSlotCameras[g_mbgStream.unk68].unk2 = info->unk2;
            g_mbgSlotCameras[g_mbgStream.unk68].unk4 = info->unk4;
            g_mbgSlotCameras[g_mbgStream.unk68].unk6 = info->unk6;
            g_mbgSlotCameras[g_mbgStream.unk68].unk8 = info->unk8;
            g_mbgSlotCameras[g_mbgStream.unk68].unkA = info->unkA;
            g_mbgSlotCameras[g_mbgStream.unk68].unkC = info->unkC;
            g_mbgSlotCameras[g_mbgStream.unk68].unkE = info->unkE;
            g_mbgSlotCameras[g_mbgStream.unk68].unk10 = info->unk10;
            g_mbgSlotCameras[g_mbgStream.unk68].unk12 = info->unk12;
            g_mbgSlotCameras[g_mbgStream.unk68].unk14 = info->unk14;
            g_mbgSlotCameras[g_mbgStream.unk68].unk18 = info->unk18;
            g_mbgSlotCameras[g_mbgStream.unk68].unk1C = info->unk1C;
            g_mbgSlotCameras[g_mbgStream.unk68].unk20 = info->unk20;
            g_mbgSlotCameras[g_mbgStream.unk68].unk22 = info->unk22;
            g_mbgStream.unk3 = 1;
        }
        if (s->unk5C < 0) {
            if (((u32)s->unk5C >> 8 & 0xFF) >= 0x80) {
                D_8019F7D0[g_mbgStream.unk68].unk0 = 1;
            } else {
                D_8019F7D0[g_mbgStream.unk68].unk0 = 0;
            }
            D_8019F7D0[g_mbgStream.unk68].unk1 = s->unk5C;
        } else {
            D_8019F7D0[g_mbgStream.unk68].unk0 = 0;
            D_8019F7D0[g_mbgStream.unk68].unk1 = 0;
        }
        if (g_mbgStream.unk4 != NULL) {
            g_mbgStream.unk4(g_mbgStream.unk90 + ((g_mbgStream.unk80 - 1) << 11),
                            g_mbgStream.unk90 + (g_mbgStream.unk80 << 11), info);
        }
    }
    cur = g_mbgStream.unk80;
    g_mbgStream.unk84 = cur;
    g_mbgStream.unk88++;
    if (g_mbgStream.unk88 > g_mbgStream.unk8C) {
        g_mbgStream.unk8C = g_mbgStream.unk88;
    }
    if (++g_mbgStream.unk80 >= g_mbgStream.unk7C) {
        g_mbgStream.unk80 = 0;
    }
    if (++g_mbgStream.unk48 >= g_mbgStream.frameSectors) {
        g_mbgStream.unk48 = 0;
        g_mbgStream.frame++;
        if (g_mbgStream.unk8 != NULL) {
            g_mbgStream.unk8();
        }
        h = &g_mbgStream.unk74[g_mbgStream.unk68];
        h->unk4 = g_mbgStream.unk5C - g_mbgStream.unk64;
        h->unk0 = (h->unk0 & 0xFFF) | 0xC000;
        if (g_mbgStream.frame > g_mbgStream.lastFrame) {
            g_mbgStream.unk2 = 1;
            func_80023018();
        }
        g_mbgStream.unk64 = 0;
        if (g_mbgStream.unk68 == 0) {
            g_mbgStream.unk68 = g_mbgStream.slotCount;
        }
        g_mbgStream.unk68--;
        g_mbgSlotMasks[g_mbgStream.unk68].unk0 = 0;
        g_mbgSlotMasks[g_mbgStream.unk68].unk4 = 0;
    }
    if (s->unk60 != 0xEEEEEEEE && s->unk60 != g_mbgStream.lastFrame) {
        g_mbgStream.lastFrame = s->unk60;
    }
}

s32 mbg_streamStart(s32 unused, u32 arg1, s32 arg2, void (*arg3)(void), s32 arg4) {
    CdlLOC *loc;
    s32 i;
    Ovl64Pair *p;

    CdIntToPos(g_mbgStream.baseSector, &g_mbgStream.unk20);
    g_mbgStream.unk40 = CdPosToInt(&g_mbgStream.unk20);
    if (arg1 >= 2) {
        g_mbgStream.unk40 += (arg1 - 1) * g_mbgStream.frameSectors;
    }
    D_8019DE0C = 0;
    g_mbgStream.channel = -1;
    g_mbgStream.unk38 = arg1;
    g_mbgStream.lastFrame = arg2;
    g_mbgStream.unk8 = arg3;
    g_mbgStream.unkC = arg4;
    g_mbgStream.unk48 = 0;
    g_mbgStream.unk4C = 0;
    g_mbgStream.cameraSel = 0;
    g_mbgStream.frame = 1;
    g_mbgStream.sector = g_mbgStream.unk40;
    g_mbgStream.lastReadTime = VSync(-1);
    g_mbgStreamDeferred = 0;
    for (i = 0; i < 16; i++) {
        g_mbgSlotMasks[i].unk0 = 0;
        g_mbgSlotMasks[i].unk4 = 0;
    }
    g_mbgStreamBusy = 0;
    CdSetDebug(0);
    func_80022F1C(g_mbgStream.sector, mbg_streamReadyCallback);
    func_80056448((s32)g_mbgStream.unk90, g_mbgStream.unk7C << 11);
    return 0;
}

s32 mbg_streamGetFrame(u8 **data, Ovl64RingHdr **hdr, Ovl64FrameInfo **arg2, Ovl64Pair **arg3, Ovl64Flag **arg4) {
    Ovl64RingHdr *h = &g_mbgStream.unk74[g_mbgStream.readSlot];

    if (!(h->unk0 & 0x8000)) {
        if (g_mbgStream.unk2 == 0) {
            return 3;
        }
        return 5;
    }
    *hdr = h;
    *data = g_mbgStream.unk78 + (h->unk4 + g_mbgStream.readSlot * g_mbgStream.unk5C) * 0x8F4;
    if (g_mbgStream.unk3 != 0) {
        *arg2 = &g_mbgSlotCameras[g_mbgStream.readSlot];
        *arg3 = &g_mbgSlotMasks[g_mbgStream.readSlot];
    } else {
        *arg2 = NULL;
        *arg3 = NULL;
    }
    *arg4 = &D_8019F7D0[g_mbgStream.readSlot];
    if (g_mbgStream.readSlot == 0) {
        g_mbgStream.readSlot = g_mbgStream.slotCount;
    }
    g_mbgStream.readSlot--;
    return 0;
}

void mbg_streamReadyCallback(u8 status, u8 *result) {
    u8 hdr[12];
    s32 limit;
    s32 r;
    s32 prev;
    s32 sect;
    u8 *addr;
    Ovl64RingHdr *h;

    if (g_mbgStreamBusy != 0) {
        return;
    }
    if (status == 5 || status == 0) {
        mbg_streamReadyCallback_retry();
        return;
    }
    if (g_mbgStreamDeferred == 0 && g_mbgStream.frame >= 2) {
        r = DecDCToutSync(1);
        if (r == 1) {
            g_mbgStreamDeferred = r;
            return;
        }
    }
    CdGetSector(hdr, 3);
    if (CdPosToInt((CdlLOC *)hdr) != g_mbgStream.sector) {
        mbg_streamReadyCallback_retry();
        return;
    }
    if (g_mbgStream.channel < 0) {
        if (hdr[5] == hdr[9]) {
            g_mbgStream.channel = hdr[5];
        }
    } else if (hdr[5] == hdr[9] && g_mbgStream.channel != hdr[5]) {
        g_mbgStream.unk2 = 2;
        func_80023018();
    }
    g_mbgStreamBusy = 1;
    D_8019DE0C = 0;
    if (g_mbgStream.unk4C < 2) {
        CdGetSector(g_mbgStream.unk90 + (g_mbgStream.unk80 << 11), 0x200);
        g_cdLoad.bytesRead += 0x800;
        mbg_streamOnAudioSector();
    } else {
        if (g_mbgStream.unk64 == 0) {
            if (g_mbgStream.unk68 != 0) {
                prev = g_mbgStream.unk68 - 1;
            } else {
                prev = g_mbgStream.slotCount - 1;
            }
            h = &g_mbgStream.unk74[prev];
            if (!(h->unk0 & 0x1000)) {
                {
                    /* `last` is never read: assigning it inside the expression stops fold from
                     * rewriting `a - (b - 1)` as `(a + 1) - b`, as in the original. */
                    u32 last;
                    sect = g_mbgStream.unk68 * g_mbgStream.unk5C - (g_mbgStream.unk64 - (last = g_mbgStream.unk5C - 1));
                }
                addr = g_mbgStream.unk78 + sect * 0x8F4;
                addr -= 0x20;
                D_801A07F0[D_8019DE0C] = g_mbgStream.frame;
                D_801A06F0[D_8019DE0C] = prev;
                D_8019DE0C++;
                CdGetSector(addr, 0x245);
                g_cdLoad.bytesRead += 0x800;
                func_80023018();
                mbg_streamOnVideoSector();
                g_mbgStream.unk1 = 1;
                goto end;
            }
            h->unk0 = (h->unk0 & 0xFFF) | 0x6000;
        }
        {
            u32 last;
            sect = g_mbgStream.unk68 * g_mbgStream.unk5C - (g_mbgStream.unk64 - (last = g_mbgStream.unk5C - 1));
        }
        addr = g_mbgStream.unk78 + sect * 0x8F4;
        addr -= 0x20;
        CdGetSector(addr, 0x245);
        g_cdLoad.bytesRead += 0x800;
        mbg_streamOnVideoSector();
    }
    g_mbgStream.lastReadTime = VSync(-1);
end:
    g_mbgStreamBusy = 0;
}

s32 mbg_streamReset(void) {
    if (g_mbgStream.unk90 == NULL || g_mbgStream.unk74 == NULL || g_mbgStream.unk78 == NULL || g_mbgStream.unk60 == 0 ||
        g_mbgStream.unk7C == 0) {
        return 4;
    }
    g_mbgStream.unk0 = 1;
    g_mbgStream.unk1 = 0;
    g_mbgStream.unk2 = 0;
    g_mbgStream.unk3 = 0;
    g_mbgStream.channel = -1;
    return 0;
}

s32 mbg_streamStop(void) {
    CdReadyCallback(0);
    return 0;
}

s32 mbg_streamSetRing(Ovl64RingHdr *ring, u32 per, u32 size) {
    u32 n;
    u32 i;

    if (size / per * per != size) {
        size = size / per * per;
    }
    if (per == 0 || size < per) {
        return 2;
    }
    n = size / per;
    if (n > 16) {
        return 2;
    }
    g_mbgStream.unk78 = (u8 *)ring + (n + 1) * sizeof(Ovl64RingHdr);
    g_mbgStream.unk74 = ring;
    g_mbgStream.unk5C = per;
    g_mbgStream.unk60 = size;
    g_mbgStream.unk64 = 0;
    g_mbgStream.unk68 = n - 1;
    g_mbgStream.slotCount = n;
    g_mbgStream.readSlot = n - 1;
    for (i = 0; i < n; i++) {
        ring->unk0 = (ring->unk0 & 0xFFF) | 0x1000;
        ring++;
    }
    return 0;
}

s32 mbg_streamSetAudioBuffer(u8 *arg0, s32 arg1, u32 arg2) {
    if (arg1 != 2) {
        return 2;
    }
    g_mbgStream.unk7C = arg2;
    g_mbgStream.unk80 = 0;
    g_mbgStream.unk84 = 0;
    g_mbgStream.unk88 = 0;
    g_mbgStream.unk8C = 0;
    g_mbgStream.unk90 = arg0;
    return 0;
}

s32 mbg_streamSetBaseSector(s32 arg0) {
    g_mbgStream.baseSector = arg0;
    return 0;
}

s32 mbg_streamGetCameraSelect(s32 *arg0) {
    *arg0 = g_mbgStream.cameraSel;
    return 0;
}

s32 mbg_streamSetCameraSelect(u32 arg0) {
    if (arg0 >= 2) {
        return 2;
    }
    g_mbgStream.cameraSel = arg0;
    return 0;
}

s32 mbg_streamPushAudio(void) {
    if (g_mbgStream.unk88 < 2) {
        return 6;
    }
    g_mbgStream.unk88 -= 2;
    func_80056488(&g_mbgStream);
    return 0;
}

s32 mbg_streamSeek(CdlLOC *loc) {
    s32 pos;

    mbg_streamClearRing();
    pos = CdPosToInt(loc);
    if (cd_getState() != 0) {
        func_80022FD0(0);
        while (cd_getState() != 0) {
        }
    }
    func_80022F1C(pos, mbg_streamReadyCallback);
    return 0;
}

s32 mbg_streamGetStartPos(CdlLOC *loc) {
    loc->minute = g_mbgStream.unk20.minute;
    loc->second = g_mbgStream.unk20.second;
    loc->sector = g_mbgStream.unk20.sector;
    loc->track = g_mbgStream.unk20.track;
    return 0;
}

s32 mbg_streamGetPos(CdlLOC *loc) {
    CdIntToPos(g_mbgStream.sector, loc);
    return 0;
}

s32 mbg_streamPeekFrame(u8 **data, Ovl64RingHdr **hdr, s32 *kind) {
    Ovl64RingHdr *h;

    h = &g_mbgStream.unk74[g_mbgStream.readSlot];
    if (h->unk0 & 0x1000) {
        *kind = 0;
    } else if (h->unk0 & 0x8000) {
        *kind = 1;
    } else if (h->unk0 & 0x2000) {
        *kind = 2;
    } else if (h->unk0 & 0x4000) {
        *kind = 3;
    }
    *hdr = h;
    *data = g_mbgStream.unk78 + (h->unk4 + g_mbgStream.readSlot * g_mbgStream.unk5C) * 0x8F4;
    return 0;
}

s32 mbg_streamCountFree(s32 *count) {
    Ovl64RingHdr *h = g_mbgStream.unk74;
    s32 n = g_mbgStream.slotCount;
    s32 i;

    for (i = 0; i < n; i++, h++) {
        if (h->unk0 & 0x1000) {
            (*count)++;
        }
    }
    return 0;
}

s32 mbg_streamGetLastFrame(s32 *arg0) {
    *arg0 = g_mbgStream.lastFrame;
    return 0;
}

s32 mbg_streamUngetFrame(void) {
    if (++g_mbgStream.readSlot >= g_mbgStream.slotCount) {
        g_mbgStream.readSlot = 0;
    }
    return 0;
}

s32 mbg_streamSetPaused(s32 arg0) {
    if (arg0 != 0) {
        func_80023018();
    } else {
        func_80022F1C(g_mbgStream.sector, mbg_streamReadyCallback);
    }
    return 0;
}

s32 mbg_streamClearRing(void) {
    Ovl64RingHdr *h = g_mbgStream.unk74;
    s32 n = g_mbgStream.slotCount;
    s32 i;

    for (i = 0; i < n; i++, h++) {
        h->unk0 = (h->unk0 & 0xFFF) | 0x1000;
    }
    return 0;
}

s32 mbg_streamFreeFrame(u8 *data) {
    Ovl64RingHdr *h = &g_mbgStream.unk74[(u32)(data - g_mbgStream.unk78) / 0x8F4 / g_mbgStream.unk5C];

    h->unk0 = (h->unk0 & 0xFFF) | 0x1000;
    if (g_mbgStream.unk1 != 0) {
        func_80022F1C(g_mbgStream.sector, mbg_streamReadyCallback);
        g_mbgStream.unk1 = 0;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTReset);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTGetEnv);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTPutEnv);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTin);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTout);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTinSync);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCToutSync);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTinCallback);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCToutCallback);

INCLUDE_RODATA("asm/us/ovl_64/nonmatchings/ovl_64", D_8019903C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D020);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D110);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D1A0);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D22C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D2C0);

u32 func_8019D354(void) {
    return *D_8019E008;
}

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D36C);

u32 func_8019D3F0(u16 *p) {
    return *p;
}
