#include "ovl_64.h"

void mbg_drawStreamDebug(u32 *ot, s32 bufIdx) {
    POLY_F4 *poly;
    Ovl64RingHdr *h;
    s32 j;
    u8 i;
    s32 polyIdx;
    u8 r;
    u8 g;
    u8 b;

    h = g_mbgStream.ringHdrs;
    polyIdx = 0;
    j = 0;
    for (i = 0; i < g_mbgStream.ringSectors; i++) {
        r = g_mbgDebugRingColors[j].r;
        g = g_mbgDebugRingColors[j].g;
        b = g_mbgDebugRingColors[j].b;
        if (h->status & MBG_RING_READY) {
            if (r != 0) {
                r += 0x60;
            }
            if (g != 0) {
                g += 0x60;
            }
            if (b != 0) {
                b += 0x60;
            }
        } else if (h->status & MBG_RING_BUSY) {
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
        poly = &g_mbgDebugRingPolys[bufIdx][polyIdx++];
        SetPolyF4(poly);
        setRGB0(poly, r, g, b);
        setXY4(poly, i * 6 + 0x11, 0xBC, i * 6 + 0x16, 0xBC, i * 6 + 0x11, 0xC4, i * 6 + 0x16, 0xC4);
        addPrim(ot, poly);
        if (((i + 1) & 7) == 0) {
            j++;
            h++;
        }
    }
    poly = &g_mbgDebugRingPolys[bufIdx][polyIdx];
    SetPolyF4(poly);
    setRGB0(poly, 0x10, 0x10, 0x10);
    setXY4(poly, 0x10, 0xBB, 0x132, 0xBB, 0x10, 0xC5, 0x132, 0xC5);
    addPrim(ot, poly);
}

void mbg_streamOnVideoSector(void) {
    Ovl64SectHdr *sectHdr;
    u32 *sectData;
    u32 *src;
    u32 *dst;
    u32 i;
    Ovl64RingHdr *h;

    sectData = (u32 *)(g_mbgStream.ringData +
                (g_mbgStream.writeSlot * g_mbgStream.slotSectors - (g_mbgStream.slotSector + 1 - g_mbgStream.slotSectors)) * MBG_SECTOR_DATA);
    sectData -= 8;
    if (g_mbgStream.slotSector == 0) {
        dst = (u32 *)&g_mbgStream.ringHdrs[g_mbgStream.writeSlot];
        for (i = 0, src = sectData; i < 8; i++) {
            *dst++ = *src++;
        }
        sectHdr = (Ovl64SectHdr *)sectData;
        if (sectHdr->frame != g_mbgStream.frame) {
            g_mbgStream.endState = MBG_END_ERROR;
            cd_pause();
        }
        if (sectHdr->type != 4) {
            goto end;
        }
    }
    sectHdr = (Ovl64SectHdr *)sectData;
    if ((sectHdr->maskInfo.w >> 28) & 2) {
        g_mbgSlotMasks[g_mbgStream.writeSlot].size = sectHdr->maskInfo.w & 0xFFFF;
        g_mbgSlotMasks[g_mbgStream.writeSlot].data = (s32)((u8 *)sectHdr + (sectHdr->maskInfo.h.hi & 0xFFF));
    }
end:
    g_mbgStream.sector++;
    if (++g_mbgStream.frameSector >= g_mbgStream.frameSectors) {
        g_mbgStream.slotSector++;
        g_mbgStream.frameSector = 0;
        g_mbgStream.frame++;
        if (g_mbgStream.frameCallback != NULL) {
            g_mbgStream.frameCallback();
        }
        h = &g_mbgStream.ringHdrs[g_mbgStream.writeSlot];
        h->firstSector = g_mbgStream.slotSectors - g_mbgStream.slotSector;
        h->status = (h->status & ~MBG_RING_STATE) | MBG_RING_READY | MBG_RING_BUSY;
        if (g_mbgStream.frame > g_mbgStream.lastFrame) {
            g_mbgStream.endState = MBG_END_LAST_FRAME;
            cd_pause();
        }
        g_mbgStream.slotSector = 0;
        if (g_mbgStream.writeSlot == 0) {
            g_mbgStream.writeSlot = g_mbgStream.slotCount;
        }
        g_mbgStream.writeSlot--;
        g_mbgSlotMasks[g_mbgStream.writeSlot].size = 0;
        g_mbgSlotMasks[g_mbgStream.writeSlot].data = 0;
    } else {
        g_mbgStream.slotSector++;
    }
    if (++g_mbgStream.cycleSector >= g_mbgStream.cycleLength) {
        g_mbgStream.cycleSector = 0;
    }
}

void mbg_streamOnAudioSector(void) {
    Ovl64StreamSect *sect;
    Ovl64FrameInfo *info;
    Ovl64RingHdr *h;
    u32 cur;

    info = (Ovl64FrameInfo *)&g_mbgStream; /* what the callback gets when the sector has no frame info */
    g_mbgStream.sector++;
    sect = (Ovl64StreamSect *)(g_mbgStream.audioBuf + (g_mbgStream.audioWrite << 11));
    if (++g_mbgStream.cycleSector >= g_mbgStream.cycleLength) {
        g_mbgStream.cycleSector = 0;
    }
    if (g_mbgStream.cycleSector == 2) {
        if (sect->hasCamera == 1) {
            if (g_mbgStream.cameraSel == 1) {
                info = &sect->cameras[1];
            } else {
                info = &sect->cameras[0];
            }
            g_mbgSlotCameras[g_mbgStream.writeSlot].id = info->id;
            g_mbgSlotCameras[g_mbgStream.writeSlot].m00 = info->m00;
            g_mbgSlotCameras[g_mbgStream.writeSlot].m01 = info->m01;
            g_mbgSlotCameras[g_mbgStream.writeSlot].m02 = info->m02;
            g_mbgSlotCameras[g_mbgStream.writeSlot].m10 = info->m10;
            g_mbgSlotCameras[g_mbgStream.writeSlot].m11 = info->m11;
            g_mbgSlotCameras[g_mbgStream.writeSlot].m12 = info->m12;
            g_mbgSlotCameras[g_mbgStream.writeSlot].m20 = info->m20;
            g_mbgSlotCameras[g_mbgStream.writeSlot].m21 = info->m21;
            g_mbgSlotCameras[g_mbgStream.writeSlot].m22 = info->m22;
            g_mbgSlotCameras[g_mbgStream.writeSlot].tx = info->tx;
            g_mbgSlotCameras[g_mbgStream.writeSlot].ty = info->ty;
            g_mbgSlotCameras[g_mbgStream.writeSlot].tz = info->tz;
            g_mbgSlotCameras[g_mbgStream.writeSlot].ofsX = info->ofsX;
            g_mbgSlotCameras[g_mbgStream.writeSlot].ofsY = info->ofsY;
            g_mbgStream.hasCamera = 1;
        }
        if (sect->rumble < 0) {
            if (((u32)sect->rumble >> 8 & 0xFF) >= 0x80) {
                g_mbgSlotRumble[g_mbgStream.writeSlot].on = 1;
            } else {
                g_mbgSlotRumble[g_mbgStream.writeSlot].on = 0;
            }
            g_mbgSlotRumble[g_mbgStream.writeSlot].level = sect->rumble;
        } else {
            g_mbgSlotRumble[g_mbgStream.writeSlot].on = 0;
            g_mbgSlotRumble[g_mbgStream.writeSlot].level = 0;
        }
        if (g_mbgStream.frameInfoCallback != NULL) {
            g_mbgStream.frameInfoCallback(g_mbgStream.audioBuf + ((g_mbgStream.audioWrite - 1) << 11),
                            g_mbgStream.audioBuf + (g_mbgStream.audioWrite << 11), info);
        }
    }
    cur = g_mbgStream.audioWrite;
    g_mbgStream.audioLast = cur;
    g_mbgStream.audioPending++;
    if (g_mbgStream.audioPending > g_mbgStream.audioPendingMax) {
        g_mbgStream.audioPendingMax = g_mbgStream.audioPending;
    }
    if (++g_mbgStream.audioWrite >= g_mbgStream.audioSectors) {
        g_mbgStream.audioWrite = 0;
    }
    if (++g_mbgStream.frameSector >= g_mbgStream.frameSectors) {
        g_mbgStream.frameSector = 0;
        g_mbgStream.frame++;
        if (g_mbgStream.frameCallback != NULL) {
            g_mbgStream.frameCallback();
        }
        h = &g_mbgStream.ringHdrs[g_mbgStream.writeSlot];
        h->firstSector = g_mbgStream.slotSectors - g_mbgStream.slotSector;
        h->status = (h->status & ~MBG_RING_STATE) | MBG_RING_READY | MBG_RING_BUSY;
        if (g_mbgStream.frame > g_mbgStream.lastFrame) {
            g_mbgStream.endState = MBG_END_LAST_FRAME;
            cd_pause();
        }
        g_mbgStream.slotSector = 0;
        if (g_mbgStream.writeSlot == 0) {
            g_mbgStream.writeSlot = g_mbgStream.slotCount;
        }
        g_mbgStream.writeSlot--;
        g_mbgSlotMasks[g_mbgStream.writeSlot].size = 0;
        g_mbgSlotMasks[g_mbgStream.writeSlot].data = 0;
    }
    if (sect->lastFrame != MBG_LAST_FRAME_NONE && sect->lastFrame != g_mbgStream.lastFrame) {
        g_mbgStream.lastFrame = sect->lastFrame;
    }
}

s32 mbg_streamStart(s32 unused, u32 startFrame, s32 lastFrame, void (*frameCallback)(void), s32 unkC) {
    CdlLOC *loc;
    s32 i;
    Ovl64Pair *pair;

    CdIntToPos(g_mbgStream.baseSector, &g_mbgStream.startPos);
    g_mbgStream.startSector = CdPosToInt(&g_mbgStream.startPos);
    if (startFrame >= 2) {
        g_mbgStream.startSector += (startFrame - 1) * g_mbgStream.frameSectors;
    }
    g_mbgSectorLogCount = 0;
    g_mbgStream.channel = -1;
    g_mbgStream.startFrame = startFrame;
    g_mbgStream.lastFrame = lastFrame;
    g_mbgStream.frameCallback = frameCallback;
    g_mbgStream.unkC = unkC;
    g_mbgStream.frameSector = 0;
    g_mbgStream.cycleSector = 0;
    g_mbgStream.cameraSel = 0;
    g_mbgStream.frame = 1;
    g_mbgStream.sector = g_mbgStream.startSector;
    g_mbgStream.lastReadTime = VSync(-1);
    g_mbgStreamDeferred = 0;
    for (i = 0; i < 16; i++) {
        g_mbgSlotMasks[i].size = 0;
        g_mbgSlotMasks[i].data = 0;
    }
    g_mbgStreamBusy = 0;
    CdSetDebug(0);
    cd_readStreamAsync(g_mbgStream.sector, mbg_streamReadyCallback);
    akao_sendCmdE8((s32)g_mbgStream.audioBuf, g_mbgStream.audioSectors << 11);
    return 0;
}

s32 mbg_streamGetFrame(u8 **data, Ovl64RingHdr **hdr, Ovl64FrameInfo **outCamera, Ovl64Pair **outMask, MbgRumble **outRumble) {
    Ovl64RingHdr *h = &g_mbgStream.ringHdrs[g_mbgStream.readSlot];

    if (!(h->status & MBG_RING_READY)) {
        if (g_mbgStream.endState == MBG_END_NONE) {
            return MBG_FRAME_WAIT;
        }
        return MBG_FRAME_END;
    }
    *hdr = h;
    *data = g_mbgStream.ringData + (h->firstSector + g_mbgStream.readSlot * g_mbgStream.slotSectors) * MBG_SECTOR_DATA;
    if (g_mbgStream.hasCamera != 0) {
        *outCamera = &g_mbgSlotCameras[g_mbgStream.readSlot];
        *outMask = &g_mbgSlotMasks[g_mbgStream.readSlot];
    } else {
        *outCamera = NULL;
        *outMask = NULL;
    }
    *outRumble = &g_mbgSlotRumble[g_mbgStream.readSlot];
    if (g_mbgStream.readSlot == 0) {
        g_mbgStream.readSlot = g_mbgStream.slotCount;
    }
    g_mbgStream.readSlot--;
    return MBG_FRAME_OK;
}

void mbg_streamReadyCallback(u8 status, u8 *result) {
    u8 hdr[12];
    s32 limit;
    s32 syncResult;
    s32 prev;
    s32 sect;
    u8 *addr;
    Ovl64RingHdr *h;

    if (g_mbgStreamBusy != 0) {
        return;
    }
    if (status == CdlDiskError || status == CdlNoIntr) {
        mbg_streamReadyCallback_retry();
        return;
    }
    if (g_mbgStreamDeferred == 0 && g_mbgStream.frame >= 2) {
        syncResult = DecDCToutSync(1);
        if (syncResult == 1) {
            g_mbgStreamDeferred = syncResult;
            return;
        }
    }
    CdGetSector(hdr, CD_HEADER_WORDS);
    if (CdPosToInt((CdlLOC *)hdr) != g_mbgStream.sector) {
        mbg_streamReadyCallback_retry();
        return;
    }
    if (g_mbgStream.channel < 0) {
        if (hdr[5] == hdr[9]) {
            g_mbgStream.channel = hdr[5];
        }
    } else if (hdr[5] == hdr[9] && g_mbgStream.channel != hdr[5]) {
        g_mbgStream.endState = MBG_END_ERROR;
        cd_pause();
    }
    g_mbgStreamBusy = 1;
    g_mbgSectorLogCount = 0;
    if (g_mbgStream.cycleSector < 2) {
        CdGetSector(g_mbgStream.audioBuf + (g_mbgStream.audioWrite << 11), CD_FORM1_WORDS);
        g_cdLoad.bytesRead += 0x800;
        mbg_streamOnAudioSector();
    } else {
        if (g_mbgStream.slotSector == 0) {
            if (g_mbgStream.writeSlot != 0) {
                prev = g_mbgStream.writeSlot - 1;
            } else {
                prev = g_mbgStream.slotCount - 1;
            }
            h = &g_mbgStream.ringHdrs[prev];
            if (!(h->status & MBG_RING_FREE)) {
                {
                    /* `last` is never read: assigning it inside the expression stops fold from
                     * rewriting `a - (b - 1)` as `(a + 1) - b`, as in the original. */
                    u32 last;
                    sect = g_mbgStream.writeSlot * g_mbgStream.slotSectors - (g_mbgStream.slotSector - (last = g_mbgStream.slotSectors - 1));
                }
                addr = g_mbgStream.ringData + sect * MBG_SECTOR_DATA;
                addr -= MBG_SECTOR_HDR;
                g_mbgSectorLogFrame[g_mbgSectorLogCount] = g_mbgStream.frame;
                g_mbgSectorLogSlot[g_mbgSectorLogCount] = prev;
                g_mbgSectorLogCount++;
                CdGetSector(addr, CD_FORM2_WORDS);
                g_cdLoad.bytesRead += 0x800;
                cd_pause();
                mbg_streamOnVideoSector();
                g_mbgStream.stalled = 1;
                goto end;
            }
            h->status = (h->status & ~MBG_RING_STATE) | MBG_RING_BUSY | MBG_RING_FILLING;
        }
        {
            u32 last;
            sect = g_mbgStream.writeSlot * g_mbgStream.slotSectors - (g_mbgStream.slotSector - (last = g_mbgStream.slotSectors - 1));
        }
        addr = g_mbgStream.ringData + sect * MBG_SECTOR_DATA;
        addr -= MBG_SECTOR_HDR;
        CdGetSector(addr, CD_FORM2_WORDS);
        g_cdLoad.bytesRead += 0x800;
        mbg_streamOnVideoSector();
    }
    g_mbgStream.lastReadTime = VSync(-1);
end:
    g_mbgStreamBusy = 0;
}

s32 mbg_streamReset(void) {
    if (g_mbgStream.audioBuf == NULL || g_mbgStream.ringHdrs == NULL || g_mbgStream.ringData == NULL || g_mbgStream.ringSectors == 0 ||
        g_mbgStream.audioSectors == 0) {
        return 4;
    }
    g_mbgStream.unk0 = 1;
    g_mbgStream.stalled = 0;
    g_mbgStream.endState = MBG_END_NONE;
    g_mbgStream.hasCamera = 0;
    g_mbgStream.channel = -1;
    return 0;
}

s32 mbg_streamStop(void) {
    CdReadyCallback(0);
    return 0;
}

s32 mbg_streamSetRing(Ovl64RingHdr *ring, u32 per, u32 size) {
    u32 slots;
    u32 i;

    if (size / per * per != size) {
        size = size / per * per;
    }
    if (per == 0 || size < per) {
        return 2;
    }
    slots = size / per;
    if (slots > 16) {
        return 2;
    }
    g_mbgStream.ringData = (u8 *)ring + (slots + 1) * sizeof(Ovl64RingHdr);
    g_mbgStream.ringHdrs = ring;
    g_mbgStream.slotSectors = per;
    g_mbgStream.ringSectors = size;
    g_mbgStream.slotSector = 0;
    g_mbgStream.writeSlot = slots - 1;
    g_mbgStream.slotCount = slots;
    g_mbgStream.readSlot = slots - 1;
    for (i = 0; i < slots; i++) {
        ring->status = (ring->status & ~MBG_RING_STATE) | MBG_RING_FREE;
        ring++;
    }
    return 0;
}

s32 mbg_streamSetAudioBuffer(u8 *audioBuf, s32 mode, u32 sectors) {
    if (mode != 2) {
        return 2;
    }
    g_mbgStream.audioSectors = sectors;
    g_mbgStream.audioWrite = 0;
    g_mbgStream.audioLast = 0;
    g_mbgStream.audioPending = 0;
    g_mbgStream.audioPendingMax = 0;
    g_mbgStream.audioBuf = audioBuf;
    return 0;
}

s32 mbg_streamSetBaseSector(s32 sector) {
    g_mbgStream.baseSector = sector;
    return 0;
}

s32 mbg_streamGetCameraSelect(s32 *outCamera) {
    *outCamera = g_mbgStream.cameraSel;
    return 0;
}

s32 mbg_streamSetCameraSelect(u32 camera) {
    if (camera >= 2) {
        return 2;
    }
    g_mbgStream.cameraSel = camera;
    return 0;
}

s32 mbg_streamPushAudio(void) {
    if (g_mbgStream.audioPending < 2) {
        return 6;
    }
    g_mbgStream.audioPending -= 2;
    akao_streamPushBlock(&g_mbgStream);
    return 0;
}

s32 mbg_streamSeek(CdlLOC *loc) {
    s32 pos;

    mbg_streamClearRing();
    pos = CdPosToInt(loc);
    if (cd_getState() != 0) {
        cd_pauseAsync(0);
        while (cd_getState() != 0) {
        }
    }
    cd_readStreamAsync(pos, mbg_streamReadyCallback);
    return 0;
}

s32 mbg_streamGetStartPos(CdlLOC *loc) {
    loc->minute = g_mbgStream.startPos.minute;
    loc->second = g_mbgStream.startPos.second;
    loc->sector = g_mbgStream.startPos.sector;
    loc->track = g_mbgStream.startPos.track;
    return 0;
}

s32 mbg_streamGetPos(CdlLOC *loc) {
    CdIntToPos(g_mbgStream.sector, loc);
    return 0;
}

s32 mbg_streamPeekFrame(u8 **data, Ovl64RingHdr **hdr, s32 *kind) {
    Ovl64RingHdr *h;

    h = &g_mbgStream.ringHdrs[g_mbgStream.readSlot];
    if (h->status & MBG_RING_FREE) {
        *kind = 0;
    } else if (h->status & MBG_RING_READY) {
        *kind = 1;
    } else if (h->status & MBG_RING_FILLING) {
        *kind = 2;
    } else if (h->status & MBG_RING_BUSY) {
        *kind = 3;
    }
    *hdr = h;
    *data = g_mbgStream.ringData + (h->firstSector + g_mbgStream.readSlot * g_mbgStream.slotSectors) * MBG_SECTOR_DATA;
    return 0;
}

s32 mbg_streamCountFree(s32 *count) {
    Ovl64RingHdr *h = g_mbgStream.ringHdrs;
    s32 slots = g_mbgStream.slotCount;
    s32 i;

    for (i = 0; i < slots; i++, h++) {
        if (h->status & MBG_RING_FREE) {
            (*count)++;
        }
    }
    return 0;
}

s32 mbg_streamGetLastFrame(s32 *outFrame) {
    *outFrame = g_mbgStream.lastFrame;
    return 0;
}

s32 mbg_streamUngetFrame(void) {
    if (++g_mbgStream.readSlot >= g_mbgStream.slotCount) {
        g_mbgStream.readSlot = 0;
    }
    return 0;
}

s32 mbg_streamSetPaused(s32 paused) {
    if (paused != 0) {
        cd_pause();
    } else {
        cd_readStreamAsync(g_mbgStream.sector, mbg_streamReadyCallback);
    }
    return 0;
}

s32 mbg_streamClearRing(void) {
    Ovl64RingHdr *h = g_mbgStream.ringHdrs;
    s32 slots = g_mbgStream.slotCount;
    s32 i;

    for (i = 0; i < slots; i++, h++) {
        h->status = (h->status & ~MBG_RING_STATE) | MBG_RING_FREE;
    }
    return 0;
}

s32 mbg_streamFreeFrame(u8 *data) {
    Ovl64RingHdr *h = &g_mbgStream.ringHdrs[(u32)(data - g_mbgStream.ringData) / MBG_SECTOR_DATA / g_mbgStream.slotSectors];

    h->status = (h->status & ~MBG_RING_STATE) | MBG_RING_FREE;
    if (g_mbgStream.stalled != 0) {
        cd_readStreamAsync(g_mbgStream.sector, mbg_streamReadyCallback);
        g_mbgStream.stalled = 0;
    }
    return 0;
}
