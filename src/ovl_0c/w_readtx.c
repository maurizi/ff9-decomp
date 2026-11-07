#include "ovl_0c.h"

s32 w_readtxRequest(texId)
    s16 texId;
{
    WmReadtxSlot *slot;
    u32 i;

    slot = g_worldReadtxHead.next;
    i = 0;
    while (1) {
        if (slot->texId == texId) {
            slot->prev->next = slot->next;
            slot->next->prev = slot->prev;
            slot->next = g_worldReadtxHead.next;
            slot->prev = &g_worldReadtxHead;
            g_worldReadtxHead.next->prev = slot;
            g_worldReadtxHead.next = slot;
            break;
        }
        if (slot->texId == -1) {
            g_worldReadtxReqCount++;
            if (g_worldReadtxReqCount >= 4) {
                sprintf(g_worldMapErrorMsg, "w_readtxRequest/reqover[%d/%d]\n", g_worldReadtxReqCount, 3);
                printf("==== ERROR ===== / %s", g_worldMapErrorMsg);
                g_worldMapError = 1;
            }
            slot->texId = texId;
            slot->prev->next = slot->next;
            slot->next->prev = slot->prev;
            slot->next = g_worldReadtxHead.next;
            slot->prev = &g_worldReadtxHead;
            g_worldReadtxDirty++;
            g_worldReadtxHead.next->prev = slot;
            g_worldReadtxHead.next = slot;
            break;
        }
        slot = slot->next;
        i++;
        if (i >= 6) {
            break;
        }
    }
    if (i == 6) {
        printf("-- w_readtxRequest Error --\n");
        return 0;
    }
    return 1;
}

void wm_readtxCompact(void) {
    WmReadtxSlot *srcSlot;
    WmReadtxSlot *dstSlot;
    u32 i;

    g_worldReadtxReqCount = 0;
    if (g_worldReadtxDirty != 0) {
        srcSlot = g_worldReadtxHead.next;
        i = 0;
        g_worldReadtxDirty = 0;
        dstSlot = srcSlot;
        do {
            srcSlot = srcSlot->next;
            i++;
        } while (i < WM_READTX_BUFFERS);
        for (;;) {
            while ((srcSlot != &g_worldReadtxTail) & (srcSlot->pack == NULL)) {
                if ((srcSlot->pack == NULL) & (srcSlot->texId != -1)) {
                    srcSlot->texId = -1;
                }
                srcSlot = srcSlot->next;
            }
            if (srcSlot == &g_worldReadtxTail) {
                break;
            }
            while (dstSlot->pack != NULL) {
                dstSlot = dstSlot->next;
            }
            dstSlot->pack = srcSlot->pack;
            srcSlot->pack = NULL;
            srcSlot->extracted = 0;
            srcSlot->texId = -1;
            srcSlot = srcSlot->next;
        }
    }
}

void wm_readtxReset(void) {
}

void wm_readtxUpdate(void) {
}

/* find the loaded frame pack for (id, variant) */
s32 wm_readtxFind(s32 texId, s32 variant) {
    WmReadtxSlot *slot = g_worldReadtxHead.next;
    s32 i;

    for (i = 0; i < WM_READTX_BUFFERS; i++, slot = slot->next) {
        if ((slot->texId & WM_TEXID_MASK) == (texId & WM_TEXID_MASK) && slot->extracted == (variant & 0xFF)) {
            return (s32)slot->pack;
        }
    }
    return 0;
}

void wm_readtxDone(s32 *pack) {
    WmReadtxSlot *slot;
    s32 i;

    if (g_worldMapDebugFlags & WM_DEBUG_TEXTURE) {
        printf(s_wmStrReadtxDone, pack);
    }
    w_frameExtruct(pack);
    slot = g_worldReadtxHead.next;
    for (i = 0; i < WM_READTX_BUFFERS;) {
        if (slot->pack == pack) {
            slot->extracted = 1;
            return;
        }
        i++;
        slot = slot->next;
    }
}

void wm_frmbufInit(void) {
    u32 i;
    WmFrmbufSlot *cur;

    g_worldFrmbufSlots[0].prev = &g_worldFrmbufHead;
    g_worldFrmbufSlots[0].vramArea = 1;
    g_worldFrmbufSlots[0].next = &g_worldFrmbufSlots[1];
    g_worldFrmbufSlots[0].transferred = 0;
    g_worldFrmbufSlots[0].texId = -1;
    for (i = 1; i < 3; i++) {
        g_worldFrmbufSlots[i].prev = &g_worldFrmbufSlots[i - 1];
        g_worldFrmbufSlots[i].vramArea = 0;
        g_worldFrmbufSlots[i].transferred = 0;
        g_worldFrmbufSlots[i].next = &g_worldFrmbufSlots[i + 1];
        g_worldFrmbufSlots[i].texId = -1;
    }
    for (i = 1; i < 2; i++) {
        g_worldFrmbufSlots[i].vramArea = i + 1;
    }
    g_worldFrmbufSlots[3].prev = &g_worldFrmbufSlots[2];
    cur = &g_worldFrmbufSlots[3];
    cur->next = &g_worldFrmbufTail;
    cur->texId = -1;
    cur->vramArea = 0;
    cur->transferred = 0;
    g_worldFrmbufHead.prev = 0;
    g_worldFrmbufHead.next = g_worldFrmbufSlots;
    g_worldFrmbufReqCount = 0;
    g_worldFrmbufTail.next = &g_worldFrmbufSlots[2];
    g_worldFrmbufTail.prev = 0;
    g_worldFrmbufDirty = 0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_readtx", s_wmStrReadtxDone);
