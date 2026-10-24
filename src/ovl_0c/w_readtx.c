#include "ovl_0c.h"

s32 w_readtxRequest(s16 arg0) {
    WmReadtxSlot *p;
    u32 i;

    p = g_worldReadtxHead.unk4;
    i = 0;
    while (1) {
        if (p->texId == arg0) {
            p->unk0->unk4 = p->unk4;
            p->unk4->unk0 = p->unk0;
            p->unk4 = g_worldReadtxHead.unk4;
            p->unk0 = &g_worldReadtxHead;
            g_worldReadtxHead.unk4->unk0 = p;
            g_worldReadtxHead.unk4 = p;
            break;
        }
        if (p->texId == -1) {
            g_worldReadtxReqCount++;
            if (g_worldReadtxReqCount >= 4) {
                sprintf(g_worldMapErrorMsg, "w_readtxRequest/reqover[%d/%d]\n", g_worldReadtxReqCount, 3);
                printf("==== ERROR ===== / %s", g_worldMapErrorMsg);
                g_worldMapError = 1;
            }
            p->texId = arg0;
            p->unk0->unk4 = p->unk4;
            p->unk4->unk0 = p->unk0;
            p->unk4 = g_worldReadtxHead.unk4;
            p->unk0 = &g_worldReadtxHead;
            g_worldReadtxDirty++;
            g_worldReadtxHead.unk4->unk0 = p;
            g_worldReadtxHead.unk4 = p;
            break;
        }
        p = p->unk4;
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
    WmReadtxSlot *p;
    WmReadtxSlot *q;
    u32 i;

    g_worldReadtxReqCount = 0;
    if (g_worldReadtxDirty != 0) {
        p = g_worldReadtxHead.unk4;
        i = 0;
        g_worldReadtxDirty = 0;
        q = p;
        do {
            p = p->unk4;
            i++;
        } while (i < 3);
        for (;;) {
            while ((p != &g_worldReadtxTail) & (p->pack == NULL)) {
                if ((p->pack == NULL) & (p->texId != -1)) {
                    p->texId = -1;
                }
                p = p->unk4;
            }
            if (p == &g_worldReadtxTail) {
                break;
            }
            while (q->pack != NULL) {
                q = q->unk4;
            }
            q->pack = p->pack;
            p->pack = NULL;
            p->extracted = 0;
            p->texId = -1;
            p = p->unk4;
        }
    }
}

void wm_readtxReset(void) {
}

void wm_readtxUpdate(void) {
}

/* find the loaded frame pack for (id, variant) */
s32 wm_readtxFind(s32 arg0, s32 arg1) {
    WmReadtxSlot *p = g_worldReadtxHead.unk4;
    s32 i;

    for (i = 0; i < 3; i++, p = p->unk4) {
        if ((p->texId & 0x3F) == (arg0 & 0x3F) && p->extracted == (arg1 & 0xFF)) {
            return (s32)p->pack;
        }
    }
    return 0;
}

void wm_readtxDone(s32 *arg0) {
    WmReadtxSlot *p;
    s32 i;

    if (g_worldMapDebugFlags & WM_DEBUG_TEXTURE) {
        printf(D_800A8098, arg0);
    }
    w_frameExtruct(arg0);
    p = g_worldReadtxHead.unk4;
    for (i = 0; i < 3;) {
        if (p->pack == arg0) {
            p->extracted = 1;
            return;
        }
        i++;
        p = p->unk4;
    }
}

void wm_frmbufInit(void) {
    u32 i;
    WmFrmbufSlot *cur;

    g_worldFrmbufSlots[0].unk0 = &g_worldFrmbufHead;
    g_worldFrmbufSlots[0].vramArea = 1;
    g_worldFrmbufSlots[0].unk4 = &g_worldFrmbufSlots[1];
    g_worldFrmbufSlots[0].transferred = 0;
    g_worldFrmbufSlots[0].texId = -1;
    for (i = 1; i < 3; i++) {
        g_worldFrmbufSlots[i].unk0 = &g_worldFrmbufSlots[i - 1];
        g_worldFrmbufSlots[i].vramArea = 0;
        g_worldFrmbufSlots[i].transferred = 0;
        g_worldFrmbufSlots[i].unk4 = &g_worldFrmbufSlots[i + 1];
        g_worldFrmbufSlots[i].texId = -1;
    }
    for (i = 1; i < 2; i++) {
        g_worldFrmbufSlots[i].vramArea = i + 1;
    }
    g_worldFrmbufSlots[3].unk0 = &g_worldFrmbufSlots[2];
    cur = &g_worldFrmbufSlots[3];
    cur->unk4 = &g_worldFrmbufTail;
    cur->texId = -1;
    cur->vramArea = 0;
    cur->transferred = 0;
    g_worldFrmbufHead.unk0 = 0;
    g_worldFrmbufHead.unk4 = g_worldFrmbufSlots;
    g_worldFrmbufReqCount = 0;
    g_worldFrmbufTail.unk4 = &g_worldFrmbufSlots[2];
    g_worldFrmbufTail.unk0 = 0;
    g_worldFrmbufDirty = 0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_readtx", D_800A8098);
