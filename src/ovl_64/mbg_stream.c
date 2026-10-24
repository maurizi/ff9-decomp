#include "ovl_64.h"

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
            cd_pause();
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
            cd_pause();
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
            cd_pause();
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
    cd_readStreamAsync(g_mbgStream.sector, mbg_streamReadyCallback);
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
        cd_pause();
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
                cd_pause();
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
    cd_readStreamAsync(pos, mbg_streamReadyCallback);
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
        cd_pause();
    } else {
        cd_readStreamAsync(g_mbgStream.sector, mbg_streamReadyCallback);
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
        cd_readStreamAsync(g_mbgStream.sector, mbg_streamReadyCallback);
        g_mbgStream.unk1 = 0;
    }
    return 0;
}
